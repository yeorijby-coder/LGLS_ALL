using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Text;

namespace EcsLib
{
    /// <summary>
    /// C++의 CAsyncSocketEx에 대응하는 비동기 소켓 기반 클래스.
    /// TcpClient 기반으로 비동기 수신/송신 처리.
    /// </summary>
    public abstract class AsyncSocketEx : IDisposable
    {
        // ─── 호스트 명령 코드 (SKI EcsDef.h 기준) ─────────────────────
        public const int CMD_ORDER    = 0x01;
        public const int CMD_PALLET   = 0x02;
        public const int CMD_CANCEL   = 0x03;
        public const int CMD_STATUS   = 0x04;
        public const int CMD_ERROR    = 0x05;
        public const int CMD_COMPLETE = 0x06;
        public const int CMD_ARRIVE   = 0x07;
        public const int CMD_BARCODE  = 0x08;

        // ─── 오류 코드 ────────────────────────────────────────────────
        public const int ERR_NONE         = 0x00;
        public const int ERR_STX          = 0x01;
        public const int ERR_ETX          = 0x02;
        public const int ERR_BUFFER_FULL  = 0x03;
        public const int ERR_INVALID_PARAM= 0x04;
        public const int ERR_TIMEOUT      = 0x05;

        protected TcpClient? _client;
        protected NetworkStream? _stream;
        private CancellationTokenSource _cts = new CancellationTokenSource();
        private readonly object _sendLock = new object();
        private bool _disposed;

        public bool Connected => _client?.Connected ?? false;
        public string PeerAddress { get; protected set; } = string.Empty;
        public int PeerPort { get; protected set; }

        // ─── 이벤트 ──────────────────────────────────────────────────
        public event EventHandler? OnConnect;
        public event EventHandler? OnDisconnect;
        public event EventHandler<byte[]>? OnReceiveData;
        public event EventHandler<Exception>? OnError;

        protected void SetSocket(TcpClient client)
        {
            _client = client;
            _stream = client.GetStream();
            var ep = (IPEndPoint?)client.Client.RemoteEndPoint;
            PeerAddress = ep?.Address.ToString() ?? string.Empty;
            PeerPort = ep?.Port ?? 0;
            StartReceive();
        }

        protected async Task ConnectAsync(string host, int port, int timeoutMs = 5000)
        {
            _client = new TcpClient();
            _client.ReceiveBufferSize = 65536;
            _client.SendBufferSize = 65536;
            using var cts = new CancellationTokenSource(timeoutMs);
            await _client.ConnectAsync(host, port);
            _stream = _client.GetStream();
            PeerAddress = host;
            PeerPort = port;
            OnConnect?.Invoke(this, EventArgs.Empty);
            StartReceive();
        }

        private void StartReceive()
        {
            _cts = new CancellationTokenSource();
            Task.Run(() => ReceiveLoop(_cts.Token));
        }

        private async Task ReceiveLoop(CancellationToken token)
        {
            var buf = new byte[65536];
            try
            {
                while (!token.IsCancellationRequested && _stream != null)
                {
                    int n = await _stream.ReadAsync(buf, 0, buf.Length, token);
                    if (n == 0) break;
                    var data = new byte[n];
                    Buffer.BlockCopy(buf, 0, data, 0, n);
                    ProcessReceived(data);
                    OnReceiveData?.Invoke(this, data);
                }
            }
            catch (OperationCanceledException) { }
            catch (Exception ex)
            {
                OnError?.Invoke(this, ex);
            }
            finally
            {
                OnDisconnect?.Invoke(this, EventArgs.Empty);
                Close();
            }
        }

        /// <summary>파생 클래스에서 수신 데이터를 파싱한다.</summary>
        protected virtual void ProcessReceived(byte[] data) { }

        public virtual bool Send(byte[] data)
        {
            if (_stream == null || !Connected) return false;
            try
            {
                lock (_sendLock)
                {
                    _stream.Write(data, 0, data.Length);
                }
                return true;
            }
            catch (Exception ex)
            {
                OnError?.Invoke(this, ex);
                return false;
            }
        }

        public virtual bool Send(string msg, Encoding? enc = null)
        {
            var bytes = (enc ?? Encoding.UTF8).GetBytes(msg);
            return Send(bytes);
        }

        public virtual void Close()
        {
            _cts.Cancel();
            _stream?.Close();
            _client?.Close();
            _stream = null;
            _client = null;
        }

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            Close();
            GC.SuppressFinalize(this);
        }
    }

    /// <summary>
    /// C++의 CListenSk에 대응하는 TCP 리스너.
    /// 들어오는 연결을 수락하고 콜백으로 알린다.
    /// </summary>
    public class ListenerSocket : IDisposable
    {
        private TcpListener? _listener;
        private CancellationTokenSource _cts = new CancellationTokenSource();
        private bool _disposed;

        public int Port { get; }
        public event EventHandler<TcpClient>? ClientAccepted;

        public ListenerSocket(int port) { Port = port; }

        public void Start()
        {
            _listener = new TcpListener(IPAddress.Any, Port);
            _listener.Start();
            Task.Run(AcceptLoop);
        }

        private async Task AcceptLoop()
        {
            while (!_cts.IsCancellationRequested && _listener != null)
            {
                try
                {
                    var client = await _listener.AcceptTcpClientAsync();
                    client.ReceiveBufferSize = 65536;
                    client.SendBufferSize = 65536;
                    ClientAccepted?.Invoke(this, client);
                }
                catch (ObjectDisposedException) { break; }
                catch { }
            }
        }

        public void Stop()
        {
            _cts.Cancel();
            _listener?.Stop();
        }

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            Stop();
        }
    }

    /// <summary>
    /// C++의 CMultiCastSk에 대응하는 UDP 멀티캐스트 유틸리티.
    /// ECS 상태를 원격 모니터에 브로드캐스트한다.
    /// </summary>
    public class MulticastSocket : IDisposable
    {
        private UdpClient? _udp;
        private IPEndPoint? _ep;
        private bool _disposed;

        public MulticastSocket(string groupAddress, int port)
        {
            _ep = new IPEndPoint(IPAddress.Parse(groupAddress), port);
            _udp = new UdpClient();
            _udp.JoinMulticastGroup(IPAddress.Parse(groupAddress));
        }

        public bool Send(byte[] data)
        {
            try
            {
                _udp?.Send(data, data.Length, _ep);
                return true;
            }
            catch { return false; }
        }

        public void Dispose()
        {
            if (_disposed) return;
            _disposed = true;
            _udp?.Close();
        }
    }
}
