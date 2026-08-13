using System;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using EcsLib;

namespace Ecs.Communication
{
    /// <summary>
    /// C++의 CInterfaceSk에 대응하는 장비 통신 소켓 기반 클래스.
    /// 각 장비(CV, SC, RTV 등)가 PLC/장비와 통신하는 소켓을 제공한다.
    /// </summary>
    public abstract class InterfaceSk : AsyncSocketEx
    {
        protected Equipment.Equipment _owner;
        protected string _host = string.Empty;
        protected int _port;
        protected int _reconnectMs = 3000;
        private CancellationTokenSource _reconCts = new();

        public bool AutoReconnect { get; set; } = true;

        protected InterfaceSk(Equipment.Equipment owner)
        {
            _owner = owner;
            OnDisconnect += (s, e) =>
            {
                if (AutoReconnect)
                    Task.Delay(_reconnectMs).ContinueWith(_ => TryReconnect());
            };
        }

        public virtual async Task ConnectToEquipment(string host, int port)
        {
            _host = host;
            _port = port;
            await ConnectAsync(host, port);
        }

        private async Task TryReconnect()
        {
            try { await ConnectAsync(_host, _port); }
            catch { /* 다음 타이머에서 재시도 */ }
        }

        /// <summary>장비로 명령 전송 후 응답 처리.</summary>
        public abstract Task<bool> SendCommand(CmdMsg cmd);
    }

    /// <summary>
    /// C++의 CMelsecEthernetSk에 대응하는 미쓰비시 Melsec PLC 이더넷 통신.
    /// 3E 프레임 형식으로 PLC 레지스터 Read/Write.
    /// </summary>
    public class MelsecEthernetSk : InterfaceSk
    {
        private const byte SubHeader1 = 0x50;
        private const byte SubHeader2 = 0x00;
        private const byte NetworkNo  = 0x00;
        private const byte PcNo       = 0xFF;
        private const byte IoNo1      = 0xFF;
        private const byte IoNo2      = 0x03;
        private const byte ChannelNo  = 0x00;
        private const byte CpuTimer1  = 0x0A;
        private const byte CpuTimer2  = 0x00;

        private readonly SemaphoreSlim _semaphore = new(1, 1);
        private byte[]? _lastResponse;
        private readonly ManualResetEventSlim _responseEvent = new(false);

        public MelsecEthernetSk(Equipment.Equipment owner) : base(owner) { }

        protected override void ProcessReceived(byte[] data)
        {
            _lastResponse = data;
            _responseEvent.Set();
        }

        /// <summary>
        /// D/W 레지스터 읽기 (배치 랜덤 Read).
        /// </summary>
        public async Task<int[]?> ReadWords(int startDevice, int count)
        {
            await _semaphore.WaitAsync();
            try
            {
                var req = BuildReadRequest(startDevice, count);
                _responseEvent.Reset();
                if (!Send(req)) return null;
                bool ok = await Task.Run(() => _responseEvent.Wait(2000));
                if (!ok || _lastResponse == null) return null;
                return ParseWordResponse(_lastResponse, count);
            }
            finally { _semaphore.Release(); }
        }

        /// <summary>
        /// D/W 레지스터 쓰기 (배치 Write).
        /// </summary>
        public async Task<bool> WriteWords(int startDevice, int[] values)
        {
            await _semaphore.WaitAsync();
            try
            {
                var req = BuildWriteRequest(startDevice, values);
                _responseEvent.Reset();
                if (!Send(req)) return false;
                bool ok = await Task.Run(() => _responseEvent.Wait(2000));
                return ok;
            }
            finally { _semaphore.Release(); }
        }

        private byte[] BuildReadRequest(int startDev, int count)
        {
            // Melsec 3E Frame: SubHeader + NetworkNo + PcNo + IoNo + ChannelNo + DataLen + Timer + Cmd + SubCmd + Device + Count
            var buf = new byte[21];
            buf[0] = SubHeader1; buf[1] = SubHeader2;
            buf[2] = NetworkNo;  buf[3] = PcNo;
            buf[4] = IoNo1;      buf[5] = IoNo2;
            buf[6] = ChannelNo;
            // DataLen = 12 bytes (Timer(2) + Cmd(2) + SubCmd(2) + Device(6) + Count(2))
            buf[7] = 0x0C; buf[8] = 0x00;
            buf[9] = CpuTimer1; buf[10] = CpuTimer2;
            // Command 0x0401 = Batch Read (Word)
            buf[11] = 0x01; buf[12] = 0x04;
            // SubCmd 0x0000 = Word units
            buf[13] = 0x00; buf[14] = 0x00;
            // Device address (3 bytes little-endian)
            buf[15] = (byte)(startDev & 0xFF);
            buf[16] = (byte)((startDev >> 8) & 0xFF);
            buf[17] = (byte)((startDev >> 16) & 0xFF);
            // Device type D=0xA8
            buf[18] = 0xA8;
            // Point count
            buf[19] = (byte)(count & 0xFF);
            buf[20] = (byte)((count >> 8) & 0xFF);
            return buf;
        }

        private byte[] BuildWriteRequest(int startDev, int[] values)
        {
            int dataLen = 10 + values.Length * 2;
            var buf = new byte[9 + dataLen];
            buf[0] = SubHeader1; buf[1] = SubHeader2;
            buf[2] = NetworkNo;  buf[3] = PcNo;
            buf[4] = IoNo1;      buf[5] = IoNo2;
            buf[6] = ChannelNo;
            buf[7] = (byte)(dataLen & 0xFF); buf[8] = (byte)((dataLen >> 8) & 0xFF);
            buf[9] = CpuTimer1; buf[10] = CpuTimer2;
            // Command 0x1401 = Batch Write (Word)
            buf[11] = 0x01; buf[12] = 0x14;
            buf[13] = 0x00; buf[14] = 0x00;
            buf[15] = (byte)(startDev & 0xFF);
            buf[16] = (byte)((startDev >> 8) & 0xFF);
            buf[17] = (byte)((startDev >> 16) & 0xFF);
            buf[18] = 0xA8;
            buf[19] = (byte)(values.Length & 0xFF);
            buf[20] = (byte)((values.Length >> 8) & 0xFF);
            for (int i = 0; i < values.Length; i++)
            {
                buf[21 + i * 2] = (byte)(values[i] & 0xFF);
                buf[22 + i * 2] = (byte)((values[i] >> 8) & 0xFF);
            }
            return buf;
        }

        private int[]? ParseWordResponse(byte[] resp, int count)
        {
            // 응답: SubHeader(2)+NetworkNo(1)+PcNo(1)+IoNo(2)+ChannelNo(1)+DataLen(2)+EndCode(2)+Data
            if (resp.Length < 11 + count * 2) return null;
            int endCode = resp[9] | (resp[10] << 8);
            if (endCode != 0x0000) return null;
            var result = new int[count];
            for (int i = 0; i < count; i++)
                result[i] = resp[11 + i * 2] | (resp[12 + i * 2] << 8);
            return result;
        }

        public override async Task<bool> SendCommand(CmdMsg cmd)
        {
            // 파생 장비 클래스에서 오버라이드
            return await Task.FromResult(false);
        }
    }

    /// <summary>
    /// C++의 CListenerSk에 대응하는 수신 소켓.
    /// 특정 포트로 들어오는 장비/호스트 연결을 수락한다.
    /// </summary>
    public class ListenerSk : EcsLib.ListenerSocket
    {
        public ListenerSk(int port) : base(port) { }
    }
}
