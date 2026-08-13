using System;
using System.Text;

namespace HOST_SIM.Protocol
{
    /// <summary>
    /// ECS(HECS) WMS 인터페이스 전문.
    /// 프레임 = [HEADER 15][STX(0x02)][BODY...][ETX(0x03)], ASCII 고정 필드, 체크섬 없음.
    /// HEADER = MessageBox(10, 좌측정렬 space) + MessageSize(4, '0'패딩, STX/ETX 포함 BODY 길이) + 예비(1, space)
    /// 원본: ECP\Com\Tcp\WMSCommand.cs
    /// </summary>
    public static class WmsMessage
    {
        public const byte STX = 0x02;
        public const byte ETX = 0x03;
        public const int HEADER_SIZE = 15;

        public const string MBX_WMS = "WMS_MBX";
        public const string MBX_ECS = "ECS_MBX";

        // O 작업지시 작업구분
        public const char JOB_INGO = '1';   // 입고
        public const char JOB_OUTGO = '2';  // 출고
        public const char JOB_MOVE = '6';   // 이동

        /// <summary>HEADER 15바이트 + BODY 를 결합해 송신 바이트열 생성</summary>
        public static byte[] Frame(string mailbox, byte[] body)
        {
            string header = mailbox.PadRight(10)
                          + body.Length.ToString().PadLeft(4, '0')
                          + " ";
            byte[] frame = new byte[HEADER_SIZE + body.Length];
            Encoding.ASCII.GetBytes(header, 0, HEADER_SIZE, frame, 0);
            Array.Copy(body, 0, frame, HEADER_SIZE, body.Length);
            return frame;
        }

        /// <summary>
        /// O 작업지시 BODY(66바이트) 생성.
        /// rack 위치는 6자리 "BBbbLL"(Bank2+Bay2+Level2), 스테이션 기본 위치는 null/"000000".
        /// </summary>
        public static byte[] BuildOrder(char jobDefine, string luggageNo, string seqNo, string palletNo,
                                        string srcStn, string srcCell, string destStn, string destCell,
                                        int priority, string productId)
        {
            StringBuilder sb = new StringBuilder(66);
            sb.Append((char)STX);
            sb.Append('O');
            sb.Append(jobDefine);
            sb.Append(Num(luggageNo, 4));
            sb.Append(Num(seqNo, 4));
            sb.Append(Txt(palletNo, 7));
            AppendStation(sb, srcStn, srcCell);
            sb.Append(Num("0", 3));                    // O_RouteStn (경유지 미사용)
            AppendStation(sb, destStn, destCell);
            sb.Append(priority.ToString().PadLeft(3, '0'));
            sb.Append('0');                            // O_JobRouting
            sb.Append(Txt(productId, 20));
            sb.Append((char)ETX);
            return Encoding.ASCII.GetBytes(sb.ToString());
        }

        // 스테이션(3) + Bank(2) + Bay(3) + Level(2) — ECS는 Bay 3자리 중 뒤 2자리만 사용
        private static void AppendStation(StringBuilder sb, string stn, string cell)
        {
            sb.Append(Num(stn, 3));
            if (string.IsNullOrEmpty(cell) || cell == "000000")
            {
                sb.Append("00").Append("000").Append("00");
            }
            else
            {
                if (cell.Length != 6) throw new ArgumentException("rack cell must be 6 digits(BBbbLL): " + cell);
                sb.Append(cell.Substring(0, 2));               // Bank
                sb.Append('0').Append(cell.Substring(2, 2));   // Bay (3자리, 앞 0)
                sb.Append(cell.Substring(4, 2));               // Level
            }
        }

        /// <summary>
        /// M 모드변경 BODY(7바이트) 생성. C/V#11(코드 101)의 RGV 방향 전환용.
        /// [2026-07-30] 인터페이스명세서(20100311)/원본 ECP 실전문("0007") 규격 — 구 11바이트(예비4) 폐기.
        /// ECS 파싱: ConveyorNo = body[2,3], Mode = body[5,1] (0=입고 RGV_INGO, 1=출고 RGV_OUTGO)
        /// </summary>
        public static byte[] BuildModeChange(string conveyorNo, char mode)
        {
            StringBuilder sb = new StringBuilder(7);
            sb.Append((char)STX);
            sb.Append('M');
            sb.Append(Num(conveyorNo, 3));
            sb.Append(mode);
            sb.Append((char)ETX);
            return Encoding.ASCII.GetBytes(sb.ToString());
        }

        /// <summary>
        /// R 재작업지시(재지정) BODY(31바이트) 생성. 이중입고/공출고 재지정용.
        /// [2026-07-30] 인터페이스명세서(20100311)/원본 ECP(WMSCommand.cs R) 규격 — 구 32바이트(spare 포함) 폐기.
        ///   STX 'R' JobDefine(2,1) LuggNo(3,4)
        ///   StartPos(7,3) StartBank(10,2) StartBay(12,3) StartLevel(15,2)
        ///   DestPos(17,3) DestBank(20,2) DestBay(22,3) DestLevel(25,2)
        ///   R_Kind(27,1: 1=입고재지정) R_ScNum(28,2) ETX(30)
        /// 스테이션+셀 10자리 레이아웃은 O 전문 AppendStation 과 동일. 재지정은 반드시 동일 S/C 호기여야 함.
        /// </summary>
        public static byte[] BuildRedirect(char jobDefine, string luggageNo,
                                           string startStn, string startCell,
                                           string destStn, string destCell,
                                           char rKind, string scNum)
        {
            StringBuilder sb = new StringBuilder(31);
            sb.Append((char)STX);                       // 0
            sb.Append('R');                             // 1
            sb.Append(jobDefine);                       // 2
            sb.Append(Num(luggageNo, 4));               // 3-6
            AppendStation(sb, startStn, startCell);     // 7-16  (Pos3+Bank2+Bay3+Level2)
            AppendStation(sb, destStn, destCell);       // 17-26
            sb.Append(rKind);                           // 27
            sb.Append(Num(scNum, 2));                   // 28-29
            sb.Append((char)ETX);                       // 30
            return Encoding.ASCII.GetBytes(sb.ToString());
        }

        private static string Num(string v, int len) { return (v ?? "0").Trim().PadLeft(len, '0'); }
        private static string Txt(string v, int len)
        {
            v = v ?? "";
            if (v.Length > len) return v.Substring(0, len);
            return v.PadRight(len);
        }

        /// <summary>제어문자를 표시용 문자열로 변환 ([STX]/[ETX])</summary>
        public static string ToDisplay(byte[] frame, int offset, int length)
        {
            StringBuilder sb = new StringBuilder(length + 8);
            for (int i = offset; i < offset + length; i++)
            {
                byte b = frame[i];
                if (b == STX) sb.Append("[STX]");
                else if (b == ETX) sb.Append("[ETX]");
                else if (b < 0x20) sb.Append('.');
                else sb.Append((char)b);
            }
            return sb.ToString();
        }
    }

    /// <summary>수신 전문(헤더 파싱 완료 단위) 표현</summary>
    public class WmsFrame
    {
        public string Mailbox;      // header[0,10] trim
        public byte[] Body;         // STX..ETX 포함
        public byte[] Raw;          // header + body

        public char Type { get { return Body.Length > 1 ? (char)Body[1] : '?'; } }

        public string BodyText(int offset, int length)
        {
            return Encoding.ASCII.GetString(Body, offset, length);
        }

        public string Display { get { return WmsMessage.ToDisplay(Raw, 0, Raw.Length); } }
    }

    /// <summary>
    /// 스트림 버퍼에서 [HEADER 15][BODY size] 단위 전문을 잘라내는 어셈블러 (TCP 분할 수신 대응)
    /// </summary>
    public class FrameAssembler
    {
        private byte[] buf = new byte[0];

        public event Action<WmsFrame> FrameReceived;
        public event Action<string> ProtocolError;

        public void Feed(byte[] data, int count)
        {
            byte[] merged = new byte[buf.Length + count];
            Array.Copy(buf, 0, merged, 0, buf.Length);
            Array.Copy(data, 0, merged, buf.Length, count);
            buf = merged;

            while (true)
            {
                if (buf.Length < WmsMessage.HEADER_SIZE) return;

                string sizeText = Encoding.ASCII.GetString(buf, 10, 4);
                int bodySize;
                if (!int.TryParse(sizeText, out bodySize) || bodySize < 2 || bodySize > 4096)
                {
                    // 헤더 동기화 상실 — STX 위치를 찾아 재동기화
                    if (ProtocolError != null)
                        ProtocolError("잘못된 MessageSize '" + sizeText + "' — 버퍼 재동기화");
                    int stx = Array.IndexOf(buf, WmsMessage.STX, 1);
                    if (stx < WmsMessage.HEADER_SIZE) { buf = new byte[0]; return; }
                    byte[] rest = new byte[buf.Length - (stx - WmsMessage.HEADER_SIZE)];
                    Array.Copy(buf, stx - WmsMessage.HEADER_SIZE, rest, 0, rest.Length);
                    buf = rest;
                    continue;
                }

                int total = WmsMessage.HEADER_SIZE + bodySize;
                if (buf.Length < total) return;

                WmsFrame frame = new WmsFrame();
                frame.Raw = new byte[total];
                Array.Copy(buf, 0, frame.Raw, 0, total);
                frame.Mailbox = Encoding.ASCII.GetString(buf, 0, 10).Trim();
                frame.Body = new byte[bodySize];
                Array.Copy(buf, WmsMessage.HEADER_SIZE, frame.Body, 0, bodySize);

                byte[] remain = new byte[buf.Length - total];
                Array.Copy(buf, total, remain, 0, remain.Length);
                buf = remain;

                if (FrameReceived != null) FrameReceived(frame);
            }
        }

        public void Reset() { buf = new byte[0]; }
    }
}
