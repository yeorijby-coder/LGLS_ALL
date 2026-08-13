using System;
using System.IO;
using System.Collections.Generic;

namespace Ecs.Info
{
    /// <summary>
    /// C++의 CInfo에 대응하는 장비 상태 정보 기반 클래스.
    /// </summary>
    public abstract class EcsInfo
    {
        public bool Modified    { get; set; }
        public int  TabNumber   { get; set; }
        public int  PlcNumber   { get; set; }
        public EquipmentState State { get; set; } = EquipmentState.Normal;

        public abstract void Init();
        public abstract void Backup(BinaryWriter bw);
        public abstract void Restore(BinaryReader br);
    }

    /// <summary>
    /// C++의 CTrackInfo에 대응. 컨베이어 트랙 하나의 상태.
    /// </summary>
    public class TrackInfo
    {
        public int  Number      { get; set; }
        public int  TabNumber   { get; set; }
        public bool Loop        { get; set; }
        public int  NextTrack   { get; set; }
        public int  LuggNum     { get; set; }
        public int  JobType     { get; set; }
        public int  DestPos     { get; set; }
        public int  TrayType    { get; set; }
        public int  TrayLevel   { get; set; }
        public int  WaitMinute  { get; set; }
        public bool FmsReported { get; set; }
        public bool Suspend     { get; set; }
        public bool Present     { get; set; }
        public int  ErrorCode   { get; set; }

        // 상태 플래그 (EcsDefine.xml <Status> 노드)
        public bool IsScStoHS     { get; set; }
        public bool IsScRetHS     { get; set; }
        public bool IsCvCheckPos  { get; set; }
        public bool IsWaitForMain { get; set; }

        // 도착 스테이션 정보
        public string ArvStationName { get; set; } = string.Empty;
        public int    ArvStationId   { get; set; }
        public bool   ArvStationUse  { get; set; }

        // 위임 HS
        public int DelegateHsId { get; set; }

        public bool IsIdleJob(int luggNum) => LuggNum == luggNum && LuggNum != 0;

        public void Clear()
        {
            LuggNum = 0;
            JobType = 0;
            DestPos = 0;
            TrayType = 0;
            TrayLevel = 0;
            WaitMinute = 0;
            FmsReported = false;
            Suspend = false;
        }
    }

    /// <summary>C++의 CTrackHS에 대응하는 핸드오프 스테이션 트랙 정보.</summary>
    public class TrackHs
    {
        public int    HsId    { get; set; }
        public string HsName  { get; set; } = string.Empty;
        public int    TrackNum{ get; set; }
        public int    CvIndex { get; set; }
    }

    /// <summary>
    /// C++의 CStationInfo에 대응. HS(Hand-off Station) 정보.
    /// </summary>
    public class StationInfo
    {
        public int    Id         { get; set; }
        public string Name       { get; set; } = string.Empty;
        public int    CvIndex    { get; set; }
        public int    TrackNum   { get; set; }
        public bool   InUse      { get; set; }
        public int    LuggNum    { get; set; }
        public bool   IsBusy     => LuggNum != 0;
    }

    /// <summary>C++의 CCvInfo에 대응하는 컨베이어 상태 정보.</summary>
    public class CvInfo : EcsInfo
    {
        public TrackInfo[] Tracks { get; private set; } = Array.Empty<TrackInfo>();

        public void InitTracks(int count)
        {
            Tracks = new TrackInfo[count];
            for (int i = 0; i < count; i++)
                Tracks[i] = new TrackInfo();
        }

        public TrackInfo? FindTrack(int trackNum)
        {
            foreach (var t in Tracks)
                if (t.Number == trackNum) return t;
            return null;
        }

        public bool IsIdleJob(int luggNum)
        {
            foreach (var t in Tracks)
                if (t.IsIdleJob(luggNum)) return true;
            return false;
        }

        public override void Init()
        {
            foreach (var t in Tracks) t.Clear();
            State = EquipmentState.Normal;
        }

        public override void Backup(BinaryWriter bw)
        {
            bw.Write(Tracks.Length);
            foreach (var t in Tracks)
            {
                bw.Write(t.Number);
                bw.Write(t.LuggNum);
                bw.Write(t.JobType);
                bw.Write(t.DestPos);
                bw.Write(t.TrayType);
                bw.Write(t.TrayLevel);
                bw.Write(t.WaitMinute);
                bw.Write(t.FmsReported);
                bw.Write(t.Suspend);
            }
        }

        public override void Restore(BinaryReader br)
        {
            int count = br.ReadInt32();
            for (int i = 0; i < count && i < Tracks.Length; i++)
            {
                br.ReadInt32(); // Number
                Tracks[i].LuggNum    = br.ReadInt32();
                Tracks[i].JobType    = br.ReadInt32();
                Tracks[i].DestPos    = br.ReadInt32();
                Tracks[i].TrayType   = br.ReadInt32();
                Tracks[i].TrayLevel  = br.ReadInt32();
                Tracks[i].WaitMinute = br.ReadInt32();
                Tracks[i].FmsReported= br.ReadBoolean();
                Tracks[i].Suspend    = br.ReadBoolean();
            }
        }
    }

    /// <summary>C++의 CScInfo에 대응하는 스태커 크레인 상태 정보.</summary>
    public class ScInfo : EcsInfo
    {
        public ScState ScState    { get; set; } = ScState.Idle;
        public int     Row        { get; set; }
        public int     Col        { get; set; }
        public int     CurrentLuggNum { get; set; }
        public int     TargetRow  { get; set; }
        public int     TargetCol  { get; set; }
        public int     ErrorCode  { get; set; }
        public bool    Loaded     { get; set; }
        public int     PairIndex  { get; set; } = -1; // 쌍 SC 인덱스

        // 랙 점유 맵 (row, col) → LuggNum
        private Dictionary<(int r, int c), int> _rackMap = new();

        public void SetRack(int row, int col, int luggNum)
        {
            if (luggNum == 0) _rackMap.Remove((row, col));
            else _rackMap[(row, col)] = luggNum;
        }

        public int GetRack(int row, int col)
            => _rackMap.TryGetValue((row, col), out int v) ? v : 0;

        public bool IsRackOccupied(int row, int col) => GetRack(row, col) != 0;

        public override void Init()
        {
            ScState = ScState.Idle;
            CurrentLuggNum = 0;
            ErrorCode = 0;
            Loaded = false;
            _rackMap.Clear();
            State = EquipmentState.Normal;
        }

        public override void Backup(BinaryWriter bw)
        {
            bw.Write((int)ScState);
            bw.Write(Row); bw.Write(Col);
            bw.Write(CurrentLuggNum);
            bw.Write(_rackMap.Count);
            foreach (var kv in _rackMap)
            {
                bw.Write(kv.Key.r);
                bw.Write(kv.Key.c);
                bw.Write(kv.Value);
            }
        }

        public override void Restore(BinaryReader br)
        {
            ScState = (ScState)br.ReadInt32();
            Row = br.ReadInt32(); Col = br.ReadInt32();
            CurrentLuggNum = br.ReadInt32();
            int cnt = br.ReadInt32();
            _rackMap.Clear();
            for (int i = 0; i < cnt; i++)
            {
                int r = br.ReadInt32(), c = br.ReadInt32(), v = br.ReadInt32();
                _rackMap[(r, c)] = v;
            }
        }
    }

    /// <summary>C++의 CRtvInfo에 대응하는 RTV 상태 정보.</summary>
    public class RtvInfo : EcsInfo
    {
        public int  CurrentPos   { get; set; }
        public int  TargetPos    { get; set; }
        public bool Loaded       { get; set; }
        public int  LuggNum      { get; set; }
        public int  ErrorCode    { get; set; }
        public bool Busy         { get; set; }

        public override void Init()
        {
            CurrentPos = 0; TargetPos = 0;
            Loaded = false; LuggNum = 0; ErrorCode = 0; Busy = false;
            State = EquipmentState.Normal;
        }
        public override void Backup(BinaryWriter bw)
        {
            bw.Write(CurrentPos); bw.Write(LuggNum);
        }
        public override void Restore(BinaryReader br)
        {
            CurrentPos = br.ReadInt32(); LuggNum = br.ReadInt32();
        }
    }

    /// <summary>C++의 CBcrInfo에 대응하는 바코드 리더 상태 정보.</summary>
    public class BcrInfo : EcsInfo
    {
        public string LastBarcode { get; set; } = string.Empty;
        public bool   ReadOk      { get; set; }
        public int    ErrorCode   { get; set; }

        public override void Init() { LastBarcode = string.Empty; ReadOk = false; State = EquipmentState.Normal; }
        public override void Backup(BinaryWriter bw) { bw.Write(LastBarcode); }
        public override void Restore(BinaryReader br) { LastBarcode = br.ReadString(); }
    }
}
