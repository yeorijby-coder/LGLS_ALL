using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using EQP_SIM.Core;

namespace EQP_SIM.Xgt
{
    /// <summary>
    /// LS XGT FEnet 전용 프로토콜 TCP 서버 (PLC 역할).
    /// ECS(ECP FenetDriver)가 클라이언트로 접속해 요청/응답 1:1 로 통신한다.
    ///
    /// 프레임 = [헤더 20B: "LSIS-XGT"+00 00 | PLCInfo 2B | CPU A0 | Src | InvokeID 2B | Length 2B(LE) | Pos | BCC]
    ///        + [명령부: CMD 2B | DTYPE 2B | RSV 2B | BLKCNT 2B | VARLEN 2B | '%'+dev+'B'/'X' | 주소(ASCII 10진) | COUNT 2B | (DATA)]
    ///
    /// FenetDriver 특이사항 (반드시 유지):
    ///  - COUNT 2바이트 {b1,b2} 는 10진수 "0###" 을 앞2/뒤2 자리로 쪼갠 값 → count = b1 + b2*100
    ///  - 쓰기(0x58, DTYPE 0x14)의 주소 = 바이트주소(워드주소*2, ASCII 10진)
    ///  - 읽기(0x54, DTYPE 0x14)의 주소 = 바이트주소 (모니터 블록읽기, 500바이트 청크)
    ///  - %MX 비트 쓰기(0x58, DTYPE 0x00) 주소 = 절대 비트번호, 데이터 1바이트(0/1)
    ///  - 읽기 응답은 헤더20 + 명령부12 (CMD 0x55, DTYPE, RSV, ERR=0, BLKCNT, COUNT) + 데이터 → 데이터는 오프셋 32부터
    /// </summary>
    public class XgtServer : IDisposable
    {
        private readonly PlcMemory memory;
        private readonly List<TcpListener> listeners = new List<TcpListener>();
        private readonly List<TcpClient> clients = new List<TcpClient>();
        private volatile bool running;
        private int clientCount;

        public event Action<string> Log;
        public event Action<int> ClientCountChanged;

        public XgtServer(PlcMemory memory)
        {
            this.memory = memory;
        }

        public void Start(int[] ports)
        {
            running = true;
            foreach (int port in ports)
            {
                var listener = new TcpListener(IPAddress.Any, port);
                listener.Start();
                listeners.Add(listener);
                int p = port;
                var t = new Thread(() => AcceptLoop(listener, p)) { IsBackground = true, Name = "XgtAccept:" + p };
                t.Start();
                RaiseLog("XGT 서버 수신대기: 포트 " + p);
            }
        }

        private void AcceptLoop(TcpListener listener, int port)
        {
            while (running)
            {
                try
                {
                    TcpClient c = listener.AcceptTcpClient();
                    c.NoDelay = true;
                    lock (clients) clients.Add(c);
                    Interlocked.Increment(ref clientCount);
                    RaiseClientCount();
                    RaiseLog("ECS 접속됨 (" + c.Client.RemoteEndPoint + " → 포트 " + port + ")");
                    var t = new Thread(() => ClientLoop(c)) { IsBackground = true };
                    t.Start();
                }
                catch
                {
                    if (!running) break;
                    Thread.Sleep(500);
                }
            }
        }

        private void ClientLoop(TcpClient c)
        {
            var stream = c.GetStream();
            byte[] buf = new byte[0];
            byte[] rx = new byte[32768];
            try
            {
                while (running)
                {
                    int n = stream.Read(rx, 0, rx.Length);
                    if (n <= 0) break;

                    byte[] merged = new byte[buf.Length + n];
                    Array.Copy(buf, 0, merged, 0, buf.Length);
                    Array.Copy(rx, 0, merged, buf.Length, n);
                    buf = merged;

                    while (buf.Length >= 20)
                    {
                        int bodyLen = buf[16] | (buf[17] << 8);
                        int total = 20 + bodyLen;
                        if (bodyLen < 0 || bodyLen > 16000)
                        {
                            RaiseLog("잘못된 프레임 길이 " + bodyLen + " — 버퍼 폐기");
                            buf = new byte[0];
                            break;
                        }
                        if (buf.Length < total) break;

                        byte[] frame = new byte[total];
                        Array.Copy(buf, 0, frame, 0, total);
                        byte[] remain = new byte[buf.Length - total];
                        Array.Copy(buf, total, remain, 0, remain.Length);
                        buf = remain;

                        byte[] reply = HandleFrame(frame);
                        if (reply != null) stream.Write(reply, 0, reply.Length);
                    }
                }
            }
            catch (Exception ex)
            {
                if (running) RaiseLog("XGT 세션 종료: " + ex.Message);
            }
            finally
            {
                lock (clients) clients.Remove(c);
                try { c.Close(); } catch { }
                Interlocked.Decrement(ref clientCount);
                RaiseClientCount();
                RaiseLog("ECS 접속 해제");
            }
        }

        private byte[] HandleFrame(byte[] f)
        {
            // 헤더 검증 (관대하게: LSIS-XGT 시그니처만)
            if (f.Length < 32 || f[0] != 'L' || f[1] != 'S' || f[2] != 'I' || f[3] != 'S') return null;

            int cmd = f[20] | (f[21] << 8);
            int dtype = f[22] | (f[23] << 8);
            int varLen = f[28] | (f[29] << 8);
            int addrLen = varLen - 3;
            if (addrLen < 1 || 33 + addrLen + 2 > f.Length) return null;

            char devLetter = (char)f[31];                       // 'M' / 'D' / 'R'
            char devSuffix = (char)f[32];                       // 'B' 연속 / 'X' 비트
            string addrText = Encoding.ASCII.GetString(f, 33, addrLen);
            int address;
            if (!int.TryParse(addrText, out address)) return null;

            int countOff = 33 + addrLen;
            int b1 = f[countOff];
            int b2 = f[countOff + 1];
            int dataOff = countOff + 2;

            // 주소/카운트 해석은 접미 문자로 클라이언트를 구분한다:
            //  - 'B' (바이트 접근): 구 ECS FenetDriver(%MB/%DB/%RB, 주소=바이트오프셋, DATACOUNT="0###" 10진분할)
            //                       및 WCS_TASK_CV V1.1(%MB/%DB/%RB, 주소=바이트, DATACOUNT=바이너리LE — 256 미만은 두 해석이 동일)
            //  - 'W' (워드 접근):   WCS_TASK_CV V0.9(%MW/%DW/%RW, 주소=워드 → *2, DATACOUNT=바이너리LE 바이트수)
            bool wordAccess = (devSuffix == 'W');
            int effAddr = wordAccess ? address * 2 : address;
            int cntDec = b1 + b2 * 100;                         // FenetDriver "0###" (b1≤99 보장)
            int cntBin = b1 | (b2 << 8);                        // XGT 공식 바이너리 LE

            if (cmd == 0x0054)
            {
                // 연속 읽기: 'B'는 b1≤99 이면 "0###"(구 ECS, 청크≤500), 아니면 바이너리
                int count = wordAccess ? cntBin : (b1 <= 99 ? cntDec : cntBin);
                byte[] data = memory.ReadBytes(devLetter, effAddr, count);
                return BuildReadReply(f, data);
            }

            if (cmd == 0x0058)
            {
                if (devSuffix == 'X' || dtype == 0x0000)
                {
                    // 개별 비트 쓰기 (%MX): 주소=절대비트번호, 데이터 1바이트
                    bool on = dataOff < f.Length && f[dataOff] != 0;
                    memory.WriteBitDirect(devLetter, address, on, true);
                }
                else
                {
                    // 연속 쓰기: 카운트는 실제 페이로드 길이와 일치하는 해석을 채택
                    int avail = f.Length - dataOff;
                    int n = wordAccess ? cntBin
                          : (cntDec == avail ? cntDec : (cntBin == avail ? cntBin : cntDec));
                    n = Math.Min(n, avail);
                    if (n > 0)
                    {
                        byte[] data = new byte[n];
                        Array.Copy(f, dataOff, data, 0, n);
                        memory.WriteBytes(devLetter, effAddr, data, n, true);
                    }
                }
                return BuildWriteReply(f);
            }

            return BuildWriteReply(f);   // 알 수 없는 명령 — 형식상 응답
        }

        private static byte[] BuildHeader(byte[] request, int instructionLen)
        {
            byte[] h = new byte[20];
            byte[] company = { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T', 0, 0 };
            Array.Copy(company, h, 10);
            h[10] = 0; h[11] = 0;          // PLC Info
            h[12] = 0xA0;                  // CPU Info
            h[13] = 0x11;                  // Source of frame (PLC→클라이언트)
            h[14] = request[14];           // Invoke ID 에코
            h[15] = request[15];
            h[16] = (byte)(instructionLen & 0xFF);
            h[17] = (byte)(instructionLen >> 8);
            h[18] = 0;                     // FEnet position
            int sum = 0;
            for (int i = 0; i < 19; i++) sum += h[i];
            h[19] = (byte)sum;             // BCC
            return h;
        }

        private static byte[] BuildReadReply(byte[] request, byte[] data)
        {
            int instrLen = 12 + data.Length;
            byte[] header = BuildHeader(request, instrLen);
            byte[] reply = new byte[20 + instrLen];
            Array.Copy(header, reply, 20);
            reply[20] = 0x55; reply[21] = 0x00;                          // 읽기 응답
            reply[22] = request[22]; reply[23] = request[23];            // 데이터타입 에코
            reply[24] = 0; reply[25] = 0;                                // 예약
            reply[26] = 0; reply[27] = 0;                                // 에러 = 0
            reply[28] = 0x01; reply[29] = 0x00;                          // 블록 수
            reply[30] = (byte)(data.Length & 0xFF);
            reply[31] = (byte)((data.Length >> 8) & 0xFF);               // 데이터 수
            Array.Copy(data, 0, reply, 32, data.Length);
            return reply;
        }

        private static byte[] BuildWriteReply(byte[] request)
        {
            const int instrLen = 12;
            byte[] header = BuildHeader(request, instrLen);
            byte[] reply = new byte[20 + instrLen];
            Array.Copy(header, reply, 20);
            reply[20] = 0x59; reply[21] = 0x00;                          // 쓰기 응답
            reply[22] = request.Length > 23 ? request[22] : (byte)0x14;
            reply[23] = request.Length > 23 ? request[23] : (byte)0x00;
            reply[24] = 0; reply[25] = 0;
            reply[26] = 0; reply[27] = 0;                                // 에러 = 0
            reply[28] = 0x01; reply[29] = 0x00;
            reply[30] = 0; reply[31] = 0;
            return reply;
        }

        private void RaiseLog(string msg) { var h = Log; if (h != null) h(msg); }
        private void RaiseClientCount() { var h = ClientCountChanged; if (h != null) h(clientCount); }

        public void Dispose()
        {
            running = false;
            foreach (var l in listeners) { try { l.Stop(); } catch { } }
            lock (clients)
            {
                foreach (var c in clients) { try { c.Close(); } catch { } }
                clients.Clear();
            }
        }
    }
}
