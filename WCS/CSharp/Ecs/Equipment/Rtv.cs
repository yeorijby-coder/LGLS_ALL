using System.Threading;
using System.Threading.Tasks;
using Ecs.Communication;
using Ecs.Info;

namespace Ecs.Equipment
{
    /// <summary>
    /// C++의 CRtv에 대응하는 Rail-Guided Vehicle(RTV) 장비 클래스.
    /// HS-to-HS 이송, 이동, 홈 복귀 명령을 처리한다.
    /// </summary>
    public class Rtv : Equipment
    {
        public RtvInfo Info { get; } = new RtvInfo();

        private MelsecEthernetSk? _melsec;

        private const int D_RTV_STATUS_BASE = 3000;
        private const int D_RTV_CMD_BASE    = 3100;

        public Rtv(EcsDoc? doc, int index, int number, string device, string devName)
            : base(doc, index, number, EquipmentKind.RTV, device, devName)
        { }

        protected override InterfaceSk? CreateSocket()
        {
            _melsec = new MelsecEthernetSk(this);
            return _melsec;
        }

        protected override async Task AutoRunProc(CancellationToken token)
        {
            if (_melsec == null || !_melsec.Connected) return;

            var regs = await _melsec.ReadWords(D_RTV_STATUS_BASE, 5);
            if (regs == null) return;

            bool busy    = regs[0] != 0;
            int  pos     = regs[1];
            bool loaded  = regs[2] != 0;
            int  luggNum = regs[3];
            int  errCode = regs[4];

            if (Info.CurrentPos != pos || Info.Loaded != loaded || Info.LuggNum != luggNum)
            {
                Info.CurrentPos = pos;
                Info.Loaded     = loaded;
                Info.LuggNum    = luggNum;
                Info.ErrorCode  = errCode;
                Info.Busy       = busy;
                Info.Modified   = true;
                _doc?.OnRtvStatusChanged(this);
            }
        }

        protected override async Task CommandProc(CmdMsg cmd, CancellationToken token)
        {
            if (cmd is not RtvCmdMsg rtvCmd || _melsec == null) return;

            switch (rtvCmd.CmdType)
            {
                case RtvCmd.HsToHs:
                    Log($"HsToHs 명령: Lugg={rtvCmd.LuggNum} {rtvCmd.SrcHs} → {rtvCmd.DstHs}");
                    await _melsec.WriteWords(D_RTV_CMD_BASE, new[] { 1, rtvCmd.LuggNum, rtvCmd.SrcHs, rtvCmd.DstHs });
                    break;
                case RtvCmd.Move:
                    Log($"Move 명령: 위치={rtvCmd.TargetPos}");
                    await _melsec.WriteWords(D_RTV_CMD_BASE, new[] { 2, rtvCmd.TargetPos });
                    break;
                case RtvCmd.Home:
                    Log("Home 명령");
                    await _melsec.WriteWords(D_RTV_CMD_BASE, new[] { 10 });
                    break;
                case RtvCmd.Cancel:
                    await _melsec.WriteWords(D_RTV_CMD_BASE, new[] { 99 });
                    break;
            }
        }

        public override EcsInfo GetInfo() => Info;

        public override void MulticastInfo(MonitorServer? monitorSv, bool first = false)
        {
            if (monitorSv == null) return;
            var buf = new byte[8];
            buf[0] = (byte)EquipmentKind.RTV;
            buf[1] = (byte)Number;
            buf[2] = (byte)(Info.CurrentPos & 0xFF); buf[3] = (byte)(Info.CurrentPos >> 8);
            buf[4] = (byte)(Info.Loaded ? 1 : 0);
            buf[5] = (byte)(Info.Busy ? 1 : 0);
            buf[6] = (byte)Info.ErrorCode;
            buf[7] = (byte)(Info.LuggNum & 0xFF);
            monitorSv.Broadcast(buf);
        }
    }
}
