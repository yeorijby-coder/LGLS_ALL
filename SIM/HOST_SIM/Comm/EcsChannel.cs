using System;
using System.Net.Sockets;
using System.Threading;
using HOST_SIM.Protocol;

namespace HOST_SIM.Comm
{
    /// <summary>
    /// ECS 명령 채널 (HOST_SIM = TCP Client → ECS_PORT:9911).
    /// O/R/M 전문을 전송하고 같은 소켓으로 ACK/NAK(BODY 11)를 수신한다.
    /// </summary>
    public class EcsChannel : IDisposable
    {
        private readonly string ip;
        private readonly int port;
        private TcpClient client;
        private NetworkStream stream;
        private Thread rxThread;
        private volatile bool running;
        private readonly object sendLock = new object();
        private readonly FrameAssembler assembler = new FrameAssembler();

        public event Action<bool> ConnectionChanged;         // true=connected
        public event Action<WmsFrame> ReplyReceived;          // ACK/NAK
        public event Action<string, string> Sent;             // (구분, 표시문자열)
        public event Action<string> Error;

        public bool Connected { get { return client != null && client.Connected; } }

        public EcsChannel(string ip, int port)
        {
            this.ip = ip;
            this.port = port;
            assembler.FrameReceived += f => { var h = ReplyReceived; if (h != null) h(f); };
            assembler.ProtocolError += m => RaiseError(m);
        }

        public void Start()
        {
            running = true;
            rxThread = new Thread(Run) { IsBackground = true, Name = "EcsChannel" };
            rxThread.Start();
        }

        private void Run()
        {
            byte[] rx = new byte[8192];
            while (running)
            {
                try
                {
                    client = new TcpClient();
                    client.NoDelay = true;
                    client.Connect(ip, port);
                    stream = client.GetStream();
                    assembler.Reset();
                    RaiseConnection(true);

                    while (running)
                    {
                        int n = stream.Read(rx, 0, rx.Length);
                        if (n <= 0) break;
                        assembler.Feed(rx, n);
                    }
                }
                catch (Exception ex)
                {
                    if (running) RaiseError("ECS 채널: " + ex.Message);
                }
                finally
                {
                    Cleanup();
                    RaiseConnection(false);
                }

                if (!running) break;
                Thread.Sleep(3000);   // 재접속 대기
            }
        }

        /// <summary>전문 송신. 실패 시 false.</summary>
        public bool Send(byte[] body, string kind)
        {
            lock (sendLock)
            {
                try
                {
                    if (stream == null || client == null || !client.Connected) return false;
                    byte[] frame = WmsMessage.Frame(WmsMessage.MBX_WMS, body);
                    stream.Write(frame, 0, frame.Length);
                    var h = Sent;
                    if (h != null) h(kind, WmsMessage.ToDisplay(frame, 0, frame.Length));
                    return true;
                }
                catch (Exception ex)
                {
                    RaiseError("송신 실패(" + kind + "): " + ex.Message);
                    return false;
                }
            }
        }

        private void Cleanup()
        {
            try { if (stream != null) stream.Close(); } catch { }
            try { if (client != null) client.Close(); } catch { }
            stream = null;
            client = null;
        }

        private void RaiseConnection(bool up) { var h = ConnectionChanged; if (h != null) h(up); }
        private void RaiseError(string msg) { var h = Error; if (h != null) h(msg); }

        public void Dispose()
        {
            running = false;
            Cleanup();
        }
    }
}
