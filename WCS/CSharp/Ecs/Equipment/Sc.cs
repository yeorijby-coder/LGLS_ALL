using System.Threading;
using System.Threading.Tasks;
using Ecs.Communication;
using Ecs.Info;

namespace Ecs.Equipment
{
    /// <summary>
    /// C++의 CSc에 대응하는 스태커 크레인(Stacker Crane) 장비 클래스.
    /// Store/Retrieve/HsToHs/RackToRack 작업을 처리한다.
    /// </summary>
    public class Sc : Equipment
    {
        public ScInfo Info { get; } = new ScInfo();

        // 쌍(Pair) SC 운용 지원
        public int  PairScIndex { get; set; } = -1;
        public bool IsPairMaster { get; set; }

        public int MaxRow { get; set; } = 10;
        public int MaxCol { get; set; } = 50;

        private MelsecEthernetSk? _melsec;

        // PLC 레지스터 기준 주소
        private const int D_SC_STATUS_BASE = 2000;
        private const int D_SC_CMD_BASE    = 2100;

        public Sc(EcsDoc? doc, int index, int number, string device, string devName)
            : base(doc, index, number, EquipmentKind.SC, device, devName)
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
            if (cmd is not ScCmdMsg scCmd) return;

            switch (scCmd.CmdType)
            {
                case ScCmd.Store:
                    await Store(scCmd.LuggNum, scCmd.Row, scCmd.Col, scCmd.HsNum);
                    break;
                case ScCmd.Retrieve:
                    await Retrieve(scCmd.LuggNum, scCmd.Row, scCmd.Col, scCmd.HsNum);
                    break;
                case ScCmd.HsToHs:
                    await HsToHs(scCmd.LuggNum, scCmd.SrcHsNum, scCmd.HsNum);
                    break;
                case ScCmd.RackToRack:
                    await RackToRack(scCmd.LuggNum, scCmd.SrcRow, scCmd.SrcCol, scCmd.Row, scCmd.Col);
                    break;
                case ScCmd.Home:
                    await Home();
                    break;
                case ScCmd.Cancel:
                    await Cancel();
                    break;
            }
        }

        // ─── PLC 상태 읽기 ────────────────────────────────────────────

        private async Task ReadStatus()
        {
            if (_melsec == null) return;
            var regs = await _melsec.ReadWords(D_SC_STATUS_BASE, 10);
            if (regs == null) return;

            bool busy    = (regs[0] & PlcBit.SC_BUSY)  != 0;
            bool fault   = (regs[0] & PlcBit.SC_FAULT) != 0;
            bool ready   = (regs[0] & PlcBit.SC_READY) != 0;
            int  row     = regs[1];
            int  col     = regs[2];
            bool loaded  = regs[3] != 0;
            int  errCode = regs[4];

            if (Info.Row != row || Info.Col != col || Info.Loaded != loaded)
            {
                Info.Row    = row;
                Info.Col    = col;
                Info.Loaded = loaded;
                Info.ErrorCode = errCode;
                Info.Modified  = true;
                _doc?.OnScStatusChanged(this);
            }

            ScState newState = fault ? ScState.Error :
                               busy  ? Info.ScState  :
                               ready ? ScState.Idle   : Info.ScState;
            if (Info.ScState != newState)
            {
                Info.ScState = newState;
                Info.Modified = true;
            }
        }

        // ─── 작업 명령 ────────────────────────────────────────────────

        public async Task Store(int luggNum, int row, int col, int hsNum)
        {
            if (_melsec == null) return;
            Log($"Store 명령: Lugg={luggNum} Row={row} Col={col} HS={hsNum}");
            Info.ScState       = ScState.Storing;
            Info.CurrentLuggNum= luggNum;
            Info.TargetRow     = row;
            Info.TargetCol     = col;
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 1, luggNum, row, col, hsNum });
        }

        public async Task Retrieve(int luggNum, int row, int col, int hsNum)
        {
            if (_melsec == null) return;
            Log($"Retrieve 명령: Lugg={luggNum} Row={row} Col={col} HS={hsNum}");
            Info.ScState       = ScState.Retrieving;
            Info.CurrentLuggNum= luggNum;
            Info.TargetRow     = row;
            Info.TargetCol     = col;
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 2, luggNum, row, col, hsNum });
        }

        public async Task HsToHs(int luggNum, int srcHs, int dstHs)
        {
            if (_melsec == null) return;
            Log($"HsToHs 명령: Lugg={luggNum} SrcHS={srcHs} DstHS={dstHs}");
            Info.ScState       = ScState.Moving;
            Info.CurrentLuggNum= luggNum;
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 3, luggNum, srcHs, dstHs });
        }

        public async Task RackToRack(int luggNum, int srcRow, int srcCol, int dstRow, int dstCol)
        {
            if (_melsec == null) return;
            Log($"RackToRack 명령: Lugg={luggNum} ({srcRow},{srcCol}) → ({dstRow},{dstCol})");
            Info.ScState       = ScState.Moving;
            Info.CurrentLuggNum= luggNum;
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 4, luggNum, srcRow, srcCol, dstRow, dstCol });
        }

        public async Task Home()
        {
            if (_melsec == null) return;
            Log("Home 명령");
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 10 });
        }

        public async Task Cancel()
        {
            if (_melsec == null) return;
            Log("Cancel 명령");
            await _melsec.WriteWords(D_SC_CMD_BASE, new[] { 99 });
            Info.ScState = ScState.Idle;
        }

        public override EcsInfo GetInfo() => Info;

        public override void MulticastInfo(MonitorServer? monitorSv, bool first = false)
        {
            if (monitorSv == null) return;
            var buf = new byte[12];
            buf[0] = (byte)EquipmentKind.SC;
            buf[1] = (byte)Number;
            buf[2] = (byte)Info.ScState;
            buf[3] = (byte)(Info.Loaded ? 1 : 0);
            buf[4] = (byte)(Info.Row & 0xFF);  buf[5] = (byte)(Info.Row >> 8);
            buf[6] = (byte)(Info.Col & 0xFF);  buf[7] = (byte)(Info.Col >> 8);
            buf[8] = (byte)(Info.CurrentLuggNum & 0xFF);
            buf[9] = (byte)((Info.CurrentLuggNum >> 8) & 0xFF);
            buf[10]= (byte)((Info.CurrentLuggNum >> 16) & 0xFF);
            buf[11]= (byte)(Info.CurrentLuggNum >> 24);
            monitorSv.Broadcast(buf);
        }
    }
}
