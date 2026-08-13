using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using System.Text;
using XgtPlcSimulator.Memory;
using XgtPlcSimulator.Protocol;

namespace XgtPlcSimulator.Server;

public class ClientInfo
{
    public string    Ip          { get; set; } = "";
    public int       Port        { get; set; }
    public TcpClient Client      { get; set; } = null!;
    public DateTime  ConnectedAt { get; set; } = DateTime.Now;
    public override string ToString() => $"{Ip}:{Port}";
}

public class XgtTcpServer
{
    private TcpListener?             _listener;
    private CancellationTokenSource? _cts;
    private readonly XgtProtocolHandler _handler;
    private readonly ConcurrentDictionary<string, ClientInfo> _clients = new();

    // 헥스 덤프 로깅 여부 (UI에서 토글 가능)
    public bool HexDumpEnabled { get; set; } = true;

    // 프로토콜 모드 전환 (LGLS / SPL)
    public ProtocolMode ProtocolMode
    {
        get => _handler.Mode;
        set => _handler.Mode = value;
    }

    public event Action<ClientInfo>? ClientConnected;
    public event Action<ClientInfo>? ClientDisconnected;
    public event Action<string>?     LogMessage;

    public bool IsRunning { get; private set; }
    public IEnumerable<ClientInfo> GetClients() => _clients.Values;

    public XgtTcpServer(PlcMemory mem)
    {
        _handler = new XgtProtocolHandler(mem);
        _handler.ProtocolLog += msg => LogMessage?.Invoke(msg);
    }

    // ── 서버 시작/종료 ────────────────────────────────────────────────────────

    public void Start(int port, string bindIp = "0.0.0.0")
    {
        if (IsRunning) return;
        _cts = new CancellationTokenSource();
        var addr = string.IsNullOrWhiteSpace(bindIp) || bindIp == "0.0.0.0"
            ? IPAddress.Any
            : IPAddress.Parse(bindIp);
        _listener = new TcpListener(addr, port);
        _listener.Start();
        IsRunning = true;
        LogMessage?.Invoke($"[서버 시작] {addr}:{port} 리스닝 중...");
        Task.Run(() => AcceptLoop(_cts.Token));
    }

    public void Stop()
    {
        if (!IsRunning) return;
        _cts?.Cancel();
        _listener?.Stop();
        IsRunning = false;
        foreach (var ci in _clients.Values)
            try { ci.Client.Close(); } catch { }
        _clients.Clear();
        LogMessage?.Invoke("[서버 종료] 모든 연결 해제됨");
    }

    // ── Accept 루프 ───────────────────────────────────────────────────────────

    private async Task AcceptLoop(CancellationToken ct)
    {
        while (!ct.IsCancellationRequested)
        {
            try
            {
                var tcp = await _listener!.AcceptTcpClientAsync(ct);
                tcp.NoDelay = true;                          // Nagle 비활성화
                var ep  = (IPEndPoint)tcp.Client.RemoteEndPoint!;
                var ci  = new ClientInfo { Ip = ep.Address.ToString(), Port = ep.Port, Client = tcp };
                _clients[ci.ToString()] = ci;
                ClientConnected?.Invoke(ci);
                LogMessage?.Invoke($"[ACCEPT] {ci}");
                _ = Task.Run(() => ClientLoop(ci, ct), ct);
            }
            catch (OperationCanceledException) { break; }
            catch (Exception ex) when (!ct.IsCancellationRequested)
            {
                LogMessage?.Invoke($"[Accept 오류] {ex.Message}");
                await Task.Delay(500, ct).ConfigureAwait(false);
            }
        }
    }

    // ── 클라이언트 수신 루프 ──────────────────────────────────────────────────

    private async Task ClientLoop(ClientInfo ci, CancellationToken ct)
    {
        var stream = ci.Client.GetStream();

        try
        {
            while (!ct.IsCancellationRequested)
            {
                // ── 1) 헤더 20바이트 읽기 ─────────────────────────────────────
                var headerBuf = new byte[XgtHeader.SIZE];
                if (!await ReadExact(stream, headerBuf, 0, XgtHeader.SIZE, ct)) return;

                if (HexDumpEnabled)
                    LogMessage?.Invoke($"[{ci}] RX HDR: {Hex(headerBuf)}");

                // ── 2) 헤더 유효성 검사 ───────────────────────────────────────
                var header = XgtHeader.Parse(headerBuf);
                if (!header.IsValid())
                {
                    // 헤더가 맞지 않으면 실제 수신 내용을 덤프하고 연결 종료
                    LogMessage?.Invoke(
                        $"[{ci}] ★ 잘못된 헤더! CompanyID={Hex(header.CompanyId)} " +
                        $"(기대값: 4C534953 2D584754 0000)  → 연결 종료");
                    return;
                }

                // ── 3) 페이로드 읽기 ─────────────────────────────────────────
                var payload = new byte[header.DataLength];
                if (header.DataLength > 0)
                {
                    if (!await ReadExact(stream, payload, 0, header.DataLength, ct)) return;
                    if (HexDumpEnabled)
                        LogMessage?.Invoke($"[{ci}] RX PAY: {Hex(payload)}");
                }

                // ── 4) 전체 패킷 조립 ─────────────────────────────────────────
                var full = new byte[XgtHeader.SIZE + header.DataLength];
                Array.Copy(headerBuf, full, XgtHeader.SIZE);
                Array.Copy(payload, 0, full, XgtHeader.SIZE, header.DataLength);

                // ── 5) 프로토콜 처리 ─────────────────────────────────────────
                ushort cmd = header.DataLength >= 2
                    ? BitConverter.ToUInt16(full, XgtHeader.SIZE)
                    : (ushort)0;

                var resp = _handler.HandlePacket(full);
                if (resp != null)
                {
                    await stream.WriteAsync(resp.AsMemory(), ct);
                    await stream.FlushAsync(ct);

                    if (HexDumpEnabled)
                        LogMessage?.Invoke($"[{ci}] TX    : {Hex(resp)}");
                    else
                        LogMessage?.Invoke(
                            $"[{ci}] CMD=0x{cmd:X4}  RxLen={full.Length}  TxLen={resp.Length}");
                }
                else
                {
                    LogMessage?.Invoke(
                        $"[{ci}] ★ HandlePacket 반환 null  CMD=0x{cmd:X4}  PAY={Hex(payload)}");
                }
            }
        }
        catch (OperationCanceledException) { }
        catch (IOException ex)
        {
            LogMessage?.Invoke($"[{ci}] IO 오류: {ex.Message}");
        }
        catch (Exception ex) when (!ct.IsCancellationRequested)
        {
            LogMessage?.Invoke($"[{ci}] 오류: {ex.Message}");
        }
        finally
        {
            _clients.TryRemove(ci.ToString(), out _);
            try { ci.Client.Close(); } catch { }
            ClientDisconnected?.Invoke(ci);
            LogMessage?.Invoke($"[DISCONNECT] {ci}");
        }
    }

    // ── 유틸 ─────────────────────────────────────────────────────────────────

    // 정확히 count 바이트를 읽을 때까지 블로킹
    private static async Task<bool> ReadExact(
        NetworkStream stream, byte[] buf, int offset, int count, CancellationToken ct)
    {
        int read = 0;
        while (read < count)
        {
            int n = await stream.ReadAsync(buf.AsMemory(offset + read, count - read), ct);
            if (n == 0) return false; // 연결 종료
            read += n;
        }
        return true;
    }

    // 바이트 배열 → 16진수 문자열 (최대 64바이트)
    public static string Hex(byte[]? buf)
    {
        if (buf == null || buf.Length == 0) return "(empty)";
        int show = Math.Min(buf.Length, 64);
        var sb = new StringBuilder(show * 3 + 4);
        for (int i = 0; i < show; i++)
        {
            if (i > 0 && i % 4 == 0) sb.Append(' ');
            sb.Append(buf[i].ToString("X2"));
        }
        if (buf.Length > 64) sb.Append("...");
        return sb.ToString();
    }
}
