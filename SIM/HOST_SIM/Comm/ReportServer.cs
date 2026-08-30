using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using HOST_SIM.Protocol;

namespace HOST_SIM.Comm
{
    /// <summary>
    /// ECS 보고 채널 서버 (HOST_SIM = TCP Server, WMS_PORT:9910 수신대기).
    /// ECS가 클라이언트로 접속해 S(상태)/E(에러)/F(작업완료) 전문을 전송한다. 응답은 불필요.
    /// </summary>
    public class ReportServer : IDisposable
    {
        private readonly int port;
        private TcpListener listener;
        private Thread acceptThread;
        private volatile bool running;
        private readonly List<TcpClient> clients = new List<TcpClient>();

        public event Action<WmsFrame> ReportReceived;   // S/E/F
        public event Action<bool> ClientChanged;        // true=ECS 접속됨
        public event Action<string> Error;

        public ReportServer(int port)
        {
            this.port = port;
        }

        public void Start()
        {
            running = true;
            listener = new TcpListener(IPAddress.Any, port);
            listener.Start();
            acceptThread = new Thread(AcceptLoop) { IsBackground = true, Name = "ReportServer" };
            acceptThread.Start();
        }

        private void AcceptLoop()
        {
            while (running)
            {
                try
                {
                    TcpClient c = listener.AcceptTcpClient();
                    c.NoDelay = true;
                    lock (clients) clients.Add(c);
                    var h = ClientChanged; if (h != null) h(true);
                    Thread t = new Thread(() => ClientLoop(c)) { IsBackground = true };
                    t.Start();
                }
                catch (Exception ex)
                {
                    if (running) { var h = Error; if (h != null) h("보고 채널 accept: " + ex.Message); }
                    if (!running) break;
                    Thread.Sleep(1000);
                }
            }
        }

        private void ClientLoop(TcpClient c)
        {
            NetworkStream stream = c.GetStream();
            var assembler = new FrameAssembler();
            assembler.FrameReceived += f =>
            {
                var h = ReportReceived; if (h != null) h(f);
                // [LGLS 2026-07-30] 인터페이스명세서(20100311): 상태(S)/에러(E) 보고는 응답을 송신하지 않는다.
                //   작업완료(F)만 규격 응답 회신 (WCS 도 S/E 는 응답을 기다리지 않도록 동시 수정됨)
                if (f.Type == 'F') SendAck(stream, f);
            };
            assembler.ProtocolError += m => { var h = Error; if (h != null) h(m); };

            byte[] rx = new byte[8192];
            try
            {
                while (running)
                {
                    int n = stream.Read(rx, 0, rx.Length);
                    if (n <= 0) break;
                    assembler.Feed(rx, n);
                }
            }
            catch (Exception ex)
            {
                if (running) { var h = Error; if (h != null) h("보고 채널: " + ex.Message); }
            }
            finally
            {
                lock (clients) clients.Remove(c);
                try { c.Close(); } catch { }
                var h = ClientChanged;
                if (h != null)
                {
                    bool any;
                    lock (clients) any = clients.Count > 0;
                    h(any);
                }
            }
        }

        /// <summary>
        /// 수신한 F(작업완료) 보고에 명세 규격 응답(BODY 11: STX+type소문자+'A'+Reason"00"+LuggNo(4)+StepCount+ETX) 회신.
        /// [2026-07-30] 명세서(20100311): S/E 보고는 무응답 — 호출부에서 F 만 회신한다.
        /// </summary>
        private void SendAck(NetworkStream stream, WmsFrame frame)
        {
            try
            {
                string luggage = "0000";
                byte step = (byte)'0';
                if (frame.Type == 'F' && frame.Body.Length >= 10)
                {
                    luggage = frame.BodyText(3, 4);
                    step = frame.Body[8];   // 명세: 응답 StepCount = 작업완료 차수(F 전용)
                }

                var body = new byte[11];
                body[0] = WmsMessage.STX;
                body[1] = (byte)char.ToLowerInvariant(frame.Type);
                body[2] = (byte)'A';
                body[3] = (byte)'0'; body[4] = (byte)'0';
                for (int i = 0; i < 4; i++) body[5 + i] = (byte)luggage[i];
                body[9] = step;
                body[10] = WmsMessage.ETX;

                byte[] reply = WmsMessage.Frame(WmsMessage.MBX_WMS, body);
                stream.Write(reply, 0, reply.Length);
            }
            catch (Exception ex)
            {
                var h = Error; if (h != null) h("보고 ACK 회신 실패: " + ex.Message);
            }
        }

        public void Dispose()
        {
            running = false;
            try { if (listener != null) listener.Stop(); } catch { }
            lock (clients)
            {
                foreach (var c in clients) { try { c.Close(); } catch { } }
                clients.Clear();
            }
        }
    }
}
