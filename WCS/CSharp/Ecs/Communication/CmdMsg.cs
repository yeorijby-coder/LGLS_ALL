namespace Ecs.Communication
{
    /// <summary>
    /// C++의 CCmdMsg에 대응하는 명령 메시지 기반 클래스.
    /// 각 장비별 파생 클래스에서 명령 종류와 파라미터를 정의한다.
    /// </summary>
    public abstract class CmdMsg
    {
        public int CmdType { get; set; }
        public int LuggNum { get; set; }
    }

    // ─── CV 명령 ────────────────────────────────────────────────────
    public enum CvCmd { WriteTrackInfo = 1, WriteError, WriteSuspend, WriteWaitMinute, WriteFmsReport, WriteTrayLevel }

    public class CvCmdMsg : CmdMsg
    {
        public new CvCmd CmdType { get; set; }
        public int TrackNum   { get; set; }
        public int JobType    { get; set; }
        public int DestPos    { get; set; }
        public int FmsReport  { get; set; }
        public int WaitMinute { get; set; }
        public int TrayType   { get; set; }
        public int TrayLevel  { get; set; }
        public int Turn       { get; set; }
        public int ErrorCode  { get; set; }
        public bool Suspend   { get; set; }
    }

    // ─── SC 명령 ────────────────────────────────────────────────────
    public enum ScCmd { Store = 1, Retrieve, HsToHs, RackToRack, Home, Cancel, PM }

    public class ScCmdMsg : CmdMsg
    {
        public new ScCmd CmdType { get; set; }
        public int  Row      { get; set; }
        public int  Col      { get; set; }
        public int  SrcRow   { get; set; }
        public int  SrcCol   { get; set; }
        public int  HsNum    { get; set; }
        public int  SrcHsNum { get; set; }
    }

    // ─── RTV 명령 ────────────────────────────────────────────────────
    public enum RtvCmd { HsToHs = 1, Move, Home, Cancel, PM }

    public class RtvCmdMsg : CmdMsg
    {
        public new RtvCmd CmdType { get; set; }
        public int SrcHs  { get; set; }
        public int DstHs  { get; set; }
        public int TargetPos { get; set; }
    }

    // ─── LGV 명령 ────────────────────────────────────────────────────
    public enum LgvCmd { TRR = 1, MOD, DEL, Move, Home }

    public class LgvCmdMsg : CmdMsg
    {
        public new LgvCmd CmdType { get; set; }
        public int SrcStation { get; set; }
        public int DstStation { get; set; }
    }

    // ─── BCR 명령 ───────────────────────────────────────────────────
    public enum BcrCmd { ReadBarcode = 1, Reset }

    public class BcrCmdMsg : CmdMsg
    {
        public new BcrCmd CmdType { get; set; }
    }

    // ─── RFID 명령 ──────────────────────────────────────────────────
    public enum RfidCmd { Read = 1, Write, Reset }

    public class RfidCmdMsg : CmdMsg
    {
        public new RfidCmd CmdType { get; set; }
        public string TagData { get; set; } = string.Empty;
    }
}
