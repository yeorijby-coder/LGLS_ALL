using System.Threading;
using System.Threading.Tasks;
using Ecs.Communication;
using Ecs.Info;

namespace Ecs.Equipment
{
    /// <summary>
    /// C++의 CCv에 대응하는 컨베이어 장비 클래스.
    /// Melsec Ethernet PLC 통신으로 트랙 상태를 폴링하고,
    /// 트레이 이송 작업을 처리한다.
    /// </summary>
    public class Cv : Equipment
    {
        public CvInfo Info { get; } = new CvInfo();
        public bool StoreRequesting    { get; set; }
        public bool RetrieveRequesting { get; set; }
        public int  WaitSum            { get; set; }

        private MelsecEthernetSk? _melsec;

        // PLC D 레지스터 기준 주소 (프로젝트별 조정 필요)
        private const int D_TRACK_BASE = 1000;
        private const int D_TRACK_SIZE = 10;  // 트랙당 레지스터 수

        public Cv(EcsDoc? doc, int index, int number, string device, string devName)
            : base(doc, index, number, EquipmentKind.CV, device, devName)
        { }

        protected override InterfaceSk? CreateSocket()
        {
            _melsec = new MelsecEthernetSk(this);
            return _melsec;
        }

        protected override async Task AutoRunProc(CancellationToken token)
        {
            if (_melsec == null || !_melsec.Connected) return;
            await ReadStatus();
        }

        protected override async Task CommandProc(CmdMsg cmd, CancellationToken token)
        {
            if (cmd is not CvCmdMsg cvCmd) return;

            switch (cvCmd.CmdType)
            {
                case CvCmd.WriteTrackInfo:
                    await WriteTrackInfo(cvCmd.TrackNum, cvCmd.LuggNum, cvCmd.JobType,
                        cvCmd.DestPos, cvCmd.FmsReport, cvCmd.WaitMinute, cvCmd.TrayType,
                        cvCmd.TrayLevel, cvCmd.Turn);
                    break;
                case CvCmd.WriteError:
                    await WriteError(cvCmd.TrackNum, cvCmd.ErrorCode);
                    break;
                case CvCmd.WriteSuspend:
                    await WriteSuspend(cvCmd.TrackNum, cvCmd.Suspend);
                    break;
                case CvCmd.WriteWaitMinute:
                    await WriteWaitMinute(cvCmd.TrackNum, cvCmd.WaitMinute);
                    break;
                case CvCmd.WriteFmsReport:
                    await WriteFmsReported(cvCmd.TrackNum, cvCmd.FmsReport == 1);
                    break;
                case CvCmd.WriteTrayLevel:
                    await WriteTrayLevel(cvCmd.TrackNum, cvCmd.TrayLevel);
                    break;
            }
        }

        // ─── PLC 읽기 ─────────────────────────────────────────────────

        private async Task ReadStatus()
        {
            if (_melsec == null) return;

            int trackCount = Info.Tracks.Length;
            int totalRegs  = trackCount * D_TRACK_SIZE;
            var regs = await _melsec.ReadWords(D_TRACK_BASE, totalRegs);
            if (regs == null) return;

            bool modified = false;
            for (int i = 0; i < trackCount && i < Info.Tracks.Length; i++)
            {
                int offset = i * D_TRACK_SIZE;
                var track  = Info.Tracks[i];

                bool present  = (regs[offset]     & PlcBit.TRACK_PRESENT) != 0;
                bool errBit   = (regs[offset]     & PlcBit.TRACK_ERROR)   != 0;
                int  luggNum  = regs[offset + 1];
                int  jobType  = regs[offset + 2];
                int  destPos  = regs[offset + 3];

                if (track.Present != present || track.LuggNum != luggNum)
                {
                    track.Present = present;
                    track.LuggNum = luggNum;
                    track.JobType = jobType;
                    track.DestPos = destPos;
                    track.ErrorCode = errBit ? 1 : 0;
                    modified = true;
                }
            }

            if (modified)
            {
                Info.Modified = true;
                _doc?.OnCvStatusChanged(this);
            }
        }

        // ─── PLC 쓰기 ─────────────────────────────────────────────────

        private async Task WriteTrackInfo(int trackNum, int luggNum, int jobType,
            int destPos, int fmsReport, int waitMin, int trayType, int trayLevel, int turn)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 1;
            await _melsec.WriteWords(addr, new[] { luggNum, jobType, destPos, trayType, trayLevel, waitMin });
            Log($"WriteTrackInfo Track={trackNum} Lugg={luggNum} Job={jobType}");
        }

        private async Task WriteError(int trackNum, int error)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 7;
            await _melsec.WriteWords(addr, new[] { error });
        }

        private async Task WriteSuspend(int trackNum, bool suspend)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 8;
            await _melsec.WriteWords(addr, new[] { suspend ? 1 : 0 });
        }

        private async Task WriteWaitMinute(int trackNum, int min)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 5;
            await _melsec.WriteWords(addr, new[] { min });
        }

        private async Task WriteFmsReported(int trackNum, bool reported)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 9;
            await _melsec.WriteWords(addr, new[] { reported ? 1 : 0 });
        }

        private async Task WriteTrayLevel(int trackNum, int level)
        {
            if (_melsec == null) return;
            int idx = FindTrackIndex(trackNum);
            if (idx < 0) return;
            int addr = D_TRACK_BASE + idx * D_TRACK_SIZE + 4;
            await _melsec.WriteWords(addr, new[] { level });
        }

        private int FindTrackIndex(int trackNum)
        {
            for (int i = 0; i < Info.Tracks.Length; i++)
                if (Info.Tracks[i].Number == trackNum) return i;
            return -1;
        }

        public override EcsInfo GetInfo() => Info;

        public override void MulticastInfo(MonitorServer? monitorSv, bool first = false)
        {
            if (monitorSv == null) return;
            // 멀티캐스트 패킷 직렬화 (프로젝트별 프로토콜에 맞게 구현)
            var buf = new byte[4 + Info.Tracks.Length * 8];
            buf[0] = (byte)EquipmentKind.CV;
            buf[1] = (byte)Number;
            buf[2] = (byte)Info.Tracks.Length;
            buf[3] = first ? (byte)1 : (byte)0;
            for (int i = 0; i < Info.Tracks.Length; i++)
            {
                var t = Info.Tracks[i];
                int offset = 4 + i * 8;
                buf[offset]     = (byte)(t.Present ? 1 : 0);
                buf[offset + 1] = (byte)(t.LuggNum & 0xFF);
                buf[offset + 2] = (byte)((t.LuggNum >> 8) & 0xFF);
                buf[offset + 3] = (byte)(t.LuggNum >> 16 & 0xFF);
                buf[offset + 4] = (byte)(t.LuggNum >> 24);
                buf[offset + 5] = (byte)t.JobType;
                buf[offset + 6] = (byte)t.ErrorCode;
                buf[offset + 7] = (byte)(t.Suspend ? 1 : 0);
            }
            monitorSv.Broadcast(buf);
        }
    }
}
