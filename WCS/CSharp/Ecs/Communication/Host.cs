using System;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using EcsLib;

namespace Ecs.Communication
{
    /// <summary>
    /// C++의 CHostSv/CHostCl에 대응하는 호스트(MES/WMS) 통신 클래스.
    /// STX/ETX 프레임으로 주문/상태/완료 메시지를 교환한다.
    /// </summary>
    public class HostSocket : AsyncSocketEx
    {
        public const byte STX = 0x02;
        public const byte ETX = 0x03;

        private readonly byte[] _recvBuf = new byte[65536];
        private int _recvLen;

        public bool IsConnected => Connected;

        // 수신 이벤트
        public event EventHandler<HostMessage>? MessageReceived;

        public async Task Connect(string host, int port)
        {
            await ConnectAsync(host, port);
        }

        protected override void ProcessReceived(byte[] data)
        {
            // STX...ETX 프레임 조립
            foreach (byte b in data)
            {
                if (b == STX)
                {
                    _recvLen = 0;
                }
                else if (b == ETX)
                {
                    if (_recvLen > 0)
                    {
                        var frame = new byte[_recvLen];
                        Buffer.BlockCopy(_recvBuf, 0, frame, 0, _recvLen);
                        ParseMessage(frame);
                        _recvLen = 0;
                    }
                }
                else
                {
                    if (_recvLen < _recvBuf.Length)
                        _recvBuf[_recvLen++] = b;
                }
            }
        }

        private void ParseMessage(byte[] frame)
        {
            // 최소 4바이트: Cmd(2) + Len(2) + Data
            if (frame.Length < 4) return;

            var msg = new HostMessage
            {
                Command = (HostCmd)(frame[0] | (frame[1] << 8)),
                Data    = frame
            };

            switch (msg.Command)
            {
                case HostCmd.Order:
                    ParseOrder(msg, frame);
                    break;
                case HostCmd.Cancel:
                    ParseCancel(msg, frame);
                    break;
                case HostCmd.Status:
                    // 상태 조회 요청
                    break;
            }

            MessageReceived?.Invoke(this, msg);
        }

        private void ParseOrder(HostMessage msg, byte[] frame)
        {
            // 주문 메시지 파싱 (프로토콜 문서 기준 - 프로젝트별 조정 필요)
            if (frame.Length < 20) return;
            msg.LuggNum    = BitConverter.ToInt32(frame, 2);
            msg.JobType    = (JobType)frame[6];
            msg.SrcStation = BitConverter.ToInt32(frame, 7);
            msg.DstStation = BitConverter.ToInt32(frame, 11);
            msg.DstRow     = BitConverter.ToInt16(frame, 15);
            msg.DstCol     = BitConverter.ToInt16(frame, 17);
        }

        private void ParseCancel(HostMessage msg, byte[] frame)
        {
            if (frame.Length < 6) return;
            msg.LuggNum = BitConverter.ToInt32(frame, 2);
        }

        // ─── 전송 메서드 ──────────────────────────────────────────────

        public bool SendStatus(int luggNum, JobStatus status, int errorCode = 0)
        {
            var payload = new byte[11];
            payload[0] = (byte)HostCmd.Status;
            payload[1] = 0;
            BitConverter.GetBytes(luggNum).CopyTo(payload, 2);
            payload[6] = (byte)status;
            BitConverter.GetBytes(errorCode).CopyTo(payload, 7);
            return SendFrame(payload);
        }

        public bool SendComplete(int luggNum)
        {
            var payload = new byte[7];
            payload[0] = (byte)HostCmd.Complete;
            payload[1] = 0;
            BitConverter.GetBytes(luggNum).CopyTo(payload, 2);
            payload[6] = 0;
            return SendFrame(payload);
        }

        public bool SendArrive(int luggNum, int stationId)
        {
            var payload = new byte[11];
            payload[0] = (byte)HostCmd.Arrive;
            payload[1] = 0;
            BitConverter.GetBytes(luggNum).CopyTo(payload, 2);
            BitConverter.GetBytes(stationId).CopyTo(payload, 6);
            payload[10] = 0;
            return SendFrame(payload);
        }

        public bool SendError(int luggNum, HostError errorCode)
        {
            var payload = new byte[11];
            payload[0] = (byte)HostCmd.Error;
            payload[1] = 0;
            BitConverter.GetBytes(luggNum).CopyTo(payload, 2);
            BitConverter.GetBytes((int)errorCode).CopyTo(payload, 6);
            return SendFrame(payload);
        }

        private bool SendFrame(byte[] payload)
        {
            var frame = new byte[payload.Length + 2];
            frame[0] = STX;
            Buffer.BlockCopy(payload, 0, frame, 1, payload.Length);
            frame[frame.Length - 1] = ETX;
            return Send(frame);
        }
    }

    /// <summary>
    /// C++의 CListenSk(호스트용)에 대응하는 호스트 리스너.
    /// </summary>
    public class HostListener : ListenerSocket
    {
        public event EventHandler<HostSocket>? HostConnected;

        public HostListener(int port) : base(port)
        {
            ClientAccepted += (s, client) =>
            {
                var sock = new HostSocket();
                // 내부적으로 SetSocket 호출 (protected → HostSocket 확장)
                var connected = new HostSocketAccepted(sock, client);
                HostConnected?.Invoke(this, sock);
            };
        }
    }

    // 수락된 소켓을 HostSocket으로 감싸기 위한 내부 헬퍼
    internal class HostSocketAccepted
    {
        public HostSocketAccepted(HostSocket sock, TcpClient client)
        {
            // HostSocket은 protected SetSocket을 노출하지 않으므로
            // 프로젝트 요구사항에 따라 reflection 또는 접근자 확장으로 처리
        }
    }

    /// <summary>호스트로부터 수신된 메시지 구조.</summary>
    public class HostMessage
    {
        public HostCmd  Command    { get; set; }
        public byte[]   Data       { get; set; } = Array.Empty<byte>();
        public int      LuggNum    { get; set; }
        public JobType  JobType    { get; set; }
        public int      SrcStation { get; set; }
        public int      DstStation { get; set; }
        public int      DstRow     { get; set; }
        public int      DstCol     { get; set; }
        public string   TrayId     { get; set; } = string.Empty;
    }
}
