using System;

namespace Ecs.Job
{
    /// <summary>
    /// C++의 TrayInfo 구조체에 대응하는 트레이 정보.
    /// </summary>
    public class TrayInfo
    {
        public string Barcode    { get; set; } = string.Empty;
        public string ProductId  { get; set; } = string.Empty;
        public int    TrayType   { get; set; }
        public int    TrayLevel  { get; set; }
        public bool   Rotated    { get; set; }
    }

    /// <summary>
    /// C++의 CJobItem에 대응하는 ECS Job 아이템.
    /// 개별 작업(이송/입고/출고)의 모든 정보를 담는다.
    /// </summary>
    public class JobItem
    {
        // ─── 기본 식별 ─────────────────────────────────────────────
        public int    LuggNum    { get; set; }
        public string TrayId     { get; set; } = string.Empty;
        public JobType JobType   { get; set; }
        public JobStatus Status  { get; set; } = JobStatus.Wait;
        public int    Priority   { get; set; } = 0;
        public int    Sequence   { get; set; }

        // ─── 트레이 정보 ───────────────────────────────────────────
        public TrayInfo Tray     { get; set; } = new TrayInfo();
        public int    CrateKind  { get; set; }
        public int    CrateStatus{ get; set; }

        // ─── 출발지 ────────────────────────────────────────────────
        public int    SrcWarehouse  { get; set; }
        public int    SrcStation    { get; set; }
        public int    SrcRow        { get; set; }
        public int    SrcCol        { get; set; }
        public int    SrcCvIndex    { get; set; }
        public int    SrcTrackNum   { get; set; }

        // ─── 목적지 ────────────────────────────────────────────────
        public int    DstWarehouse  { get; set; }
        public int    DstStation    { get; set; }
        public int    DstRow        { get; set; }
        public int    DstCol        { get; set; }
        public int    DstCvIndex    { get; set; }
        public int    DstTrackNum   { get; set; }
        public int    DestPos       { get; set; }

        // ─── 경유/스테이징 ─────────────────────────────────────────
        public int    ArriveTrackNum{ get; set; }
        public int    ArriveHsId    { get; set; }
        public int    ScIndex       { get; set; } = -1;
        public int    HsNum         { get; set; }

        // ─── 시간 ─────────────────────────────────────────────────
        public DateTime CreateTime  { get; set; } = DateTime.Now;
        public DateTime StartTime   { get; set; }
        public DateTime CompleteTime{ get; set; }

        // ─── 오류/결과 ────────────────────────────────────────────
        public int    ErrorCode     { get; set; }
        public string ErrorMessage  { get; set; } = string.Empty;
        public bool   FmsReported   { get; set; }

        // ─── 유틸리티 ────────────────────────────────────────────
        public bool IsWaiting   => Status == JobStatus.Wait;
        public bool IsCompleted => Status == JobStatus.Done;
        public bool IsError     => Status == JobStatus.Error;
        public bool IsCancelled => Status == JobStatus.Cancelled;

        public bool IsSrcStation(int cvIdx, int trackNum)
            => SrcCvIndex == cvIdx && SrcTrackNum == trackNum;

        public bool IsDstStation(int cvIdx, int trackNum)
            => DstCvIndex == cvIdx && DstTrackNum == trackNum;

        public string GetSrcLocation()
            => $"WH{SrcWarehouse}-ST{SrcStation}-R{SrcRow}C{SrcCol}";

        public string GetDstLocation()
            => $"WH{DstWarehouse}-ST{DstStation}-R{DstRow}C{DstCol}";

        public override string ToString()
            => $"Job[{LuggNum}] {JobType} {Status} {GetSrcLocation()} → {GetDstLocation()}";
    }
}
