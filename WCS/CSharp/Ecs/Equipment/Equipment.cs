using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Ecs.Communication;
using Ecs.Info;

namespace Ecs.Equipment
{
    /// <summary>
    /// C++의 CEquipment에 대응하는 장비 추상 기반 클래스.
    /// 각 장비(CV, SC, RTV, LGV, BCR, Lifter 등)의 공통 속성과
    /// 스레드 기반 자동 실행/명령 처리를 제공한다.
    /// </summary>
    public abstract class Equipment
    {
        // ─── 식별 ────────────────────────────────────────────────────
        public int          Index      { get; }
        public int          Number     { get; }
        public EquipmentKind Kind      { get; }
        public string       Device     { get; }
        public string       DeviceName { get; }

        // ─── 통신 ────────────────────────────────────────────────────
        public string       Host       { get; set; } = string.Empty;
        public int          Port       { get; set; }
        public int          ListenPort { get; set; }

        protected InterfaceSk? _socket;
        protected ListenerSk?  _listener;

        // ─── 명령 큐 (C++의 m_cmdList CObList에 대응) ───────────────
        protected readonly Queue<CmdMsg> _cmdQueue = new();
        protected readonly object        _cmdLock  = new();

        // ─── 상태 ────────────────────────────────────────────────────
        public EquipmentState State       { get; protected set; } = EquipmentState.Normal;
        public bool           Running     { get; private set; }
        public bool           IsConnected => _socket?.Connected ?? false;

        // ─── 스레드 ─────────────────────────────────────────────────
        private CancellationTokenSource _cts = new();
        private Task? _autoRunTask;
        private Task? _cmdTask;

        protected EcsDoc? _doc;

        // ─── 이벤트 ─────────────────────────────────────────────────
        public event EventHandler? StateChanged;
        public event EventHandler<string>? LogMessage;

        protected Equipment(EcsDoc? doc, int index, int number, EquipmentKind kind, string device, string devName)
        {
            _doc       = doc;
            Index      = index;
            Number     = number;
            Kind       = kind;
            Device     = device;
            DeviceName = devName;
        }

        // ─── 스레드 시작/중지 ────────────────────────────────────────

        public void Start()
        {
            if (Running) return;
            _cts    = new CancellationTokenSource();
            Running = true;

            _socket = CreateSocket();
            if (_socket != null && !string.IsNullOrEmpty(Host) && Port > 0)
                _ = _socket.ConnectToEquipment(Host, Port);

            if (ListenPort > 0)
            {
                _listener = new ListenerSk(ListenPort);
                _listener.ClientAccepted += OnClientAccepted;
                _listener.Start();
            }

            _autoRunTask = Task.Run(() => AutoRunLoop(_cts.Token));
            _cmdTask     = Task.Run(() => CmdLoop(_cts.Token));
        }

        public void Stop()
        {
            if (!Running) return;
            Running = false;
            _cts.Cancel();
            _socket?.Close();
            _listener?.Stop();
        }

        // ─── 자동 실행 루프 (C++의 AutoRunProc 스레드에 대응) ────────

        private async Task AutoRunLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                try
                {
                    await AutoRunProc(token);
                    await Task.Delay(GetAutoRunIntervalMs(), token);
                }
                catch (OperationCanceledException) { break; }
                catch (Exception ex)
                {
                    Log($"AutoRun 오류: {ex.Message}");
                    await Task.Delay(1000, token);
                }
            }
        }

        // ─── 명령 처리 루프 (C++의 CommandProc 스레드에 대응) ─────────

        private async Task CmdLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                try
                {
                    CmdMsg? cmd = null;
                    lock (_cmdLock)
                    {
                        if (_cmdQueue.Count > 0)
                            cmd = _cmdQueue.Dequeue();
                    }

                    if (cmd != null)
                        await CommandProc(cmd, token);
                    else
                        await Task.Delay(50, token);
                }
                catch (OperationCanceledException) { break; }
                catch (Exception ex)
                {
                    Log($"Command 오류: {ex.Message}");
                    await Task.Delay(500, token);
                }
            }
        }

        // ─── 파생 클래스 구현 필요 ────────────────────────────────────

        /// <summary>PLC/장비 상태 주기적 폴링 및 처리 (C++의 AutoRunProc 에 대응).</summary>
        protected abstract Task AutoRunProc(CancellationToken token);

        /// <summary>명령 큐에서 꺼낸 명령 실행 (C++의 CommandProc에 대응).</summary>
        protected abstract Task CommandProc(CmdMsg cmd, CancellationToken token);

        /// <summary>장비별 소켓 생성 (C++의 CreateSocket() 팩토리에 대응).</summary>
        protected abstract InterfaceSk? CreateSocket();

        /// <summary>AutoRun 폴링 간격 (ms). 기본 100ms.</summary>
        protected virtual int GetAutoRunIntervalMs() => 100;

        // ─── 공통 유틸리티 ───────────────────────────────────────────

        protected virtual void OnClientAccepted(object? sender, System.Net.Sockets.TcpClient client) { }

        public void EnqueueCommand(CmdMsg cmd)
        {
            lock (_cmdLock) _cmdQueue.Enqueue(cmd);
        }

        protected void SetState(EquipmentState state)
        {
            if (State == state) return;
            State = state;
            StateChanged?.Invoke(this, EventArgs.Empty);
        }

        protected void Log(string msg)
        {
            LogMessage?.Invoke(this, $"[{DeviceName}] {msg}");
        }

        public abstract EcsInfo GetInfo();

        public virtual void MulticastInfo(MonitorServer? monitorSv, bool first = false) { }

        public override string ToString() => $"{Kind}[{Number}] {DeviceName}";
    }

    /// <summary>
    /// C++의 CEquipmentArray에 대응하는 장비 컬렉션.
    /// </summary>
    public class EquipmentArray : List<Equipment>
    {
        public Equipment? FindByIndex(int index)
        {
            foreach (var e in this)
                if (e.Index == index) return e;
            return null;
        }

        public Equipment? FindByNumber(int number)
        {
            foreach (var e in this)
                if (e.Number == number) return e;
            return null;
        }

        public void StartAll()  { foreach (var e in this) e.Start(); }
        public void StopAll()   { foreach (var e in this) e.Stop(); }
    }

    /// <summary>
    /// C++의 CMonitorServer 멀티캐스트 서버에 대응.
    /// 원격 모니터 클라이언트에 장비 상태를 UDP 브로드캐스트.
    /// </summary>
    public class MonitorServer : IDisposable
    {
        private EcsLib.MulticastSocket? _mc;
        private bool _disposed;

        public bool Start(string groupAddr, int port)
        {
            try
            {
                _mc = new EcsLib.MulticastSocket(groupAddr, port);
                return true;
            }
            catch { return false; }
        }

        public void Broadcast(byte[] data) => _mc?.Send(data);

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            _mc?.Dispose();
        }
    }
}
