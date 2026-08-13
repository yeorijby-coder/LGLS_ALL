namespace Ecs
{
    // ─── 시스템 운영 모드 ─────────────────────────────────────────────
    public enum SystemMode
    {
        Offline = 0,
        Local   = 1,
        Remote  = 2,
    }

    // ─── 시스템 상태 ─────────────────────────────────────────────────
    public enum SystemState
    {
        Fault  = 0,
        Normal = 1,
        PM     = 2,
    }

    // ─── 제어 상태 ────────────────────────────────────────────────────
    public enum ControlState
    {
        Init    = 0,
        Paused  = 1,
        Auto    = 2,
        Pausing = 3,
    }

    // ─── 장비 종류 ────────────────────────────────────────────────────
    public enum EquipmentKind
    {
        CV      = 0,
        SC      = 1,
        RTV     = 2,
        Lifter  = 3,
        LGV     = 4,
        RFID    = 5,
        BCR     = 6,
        Display = 7,
        THS     = 8,
    }

    // ─── 호스트 명령 코드 ─────────────────────────────────────────────
    public enum HostCmd
    {
        Order    = 0x01,
        Status   = 0x02,
        Arrive   = 0x03,
        Barcode  = 0x04,
        Complete = 0x05,
        Cancel   = 0x06,
        Error    = 0x07,
        Alarm    = 0x08,
    }

    // ─── 호스트 오류 코드 ─────────────────────────────────────────────
    public enum HostError
    {
        None           = 0x00,
        StxError       = 0x01,
        EtxError       = 0x02,
        BufferFull     = 0x03,
        InvalidParam   = 0x04,
        Timeout        = 0x05,
        DupOrder       = 0x06,
        NoEquipment    = 0x07,
        JobNotFound    = 0x08,
    }

    // ─── Job 종류 ────────────────────────────────────────────────────
    public enum JobType
    {
        Store       = 0,
        Retrieve    = 1,
        RackToRack  = 2,
        HsToHs      = 3,
        Inspection  = 4,
    }

    // ─── Job 상태 ────────────────────────────────────────────────────
    public enum JobStatus
    {
        Wait       = 0,
        Invoking   = 1,
        Moving     = 2,
        Arrived    = 3,
        Done       = 4,
        Error      = 5,
        Cancelled  = 6,
    }

    // ─── SC(스태커 크레인) 상태 ──────────────────────────────────────
    public enum ScState
    {
        Idle    = 0,
        Storing = 1,
        Retrieving = 2,
        Moving  = 3,
        Error   = 4,
        PM      = 5,
    }

    // ─── 장비 공통 상태 ──────────────────────────────────────────────
    public enum EquipmentState
    {
        Normal  = 0,
        Fault   = 1,
        PM      = 2,
        Init    = 3,
        Offline = 4,
    }

    // ─── 파일 경로 상수 ──────────────────────────────────────────────
    public static class EcsPath
    {
        public const string EcsDefineXml = "EcsDefine.xml";
        public const string EcsLayoutXml = "EcsLayout.xml";
        public const string EcsLayout2Xml= "EcsLayout2.xml";
        public const string EcsLayout3Xml= "EcsLayout3.xml";
        public const string ConfigFile   = "EcsConfig.xml";
        public const string LogFolder    = "Log\\";
        public const string BackupFolder = "Backup\\";
    }

    // ─── 로그 레벨 ────────────────────────────────────────────────────
    public enum LogLevel
    {
        Debug   = 0,
        Info    = 1,
        Warning = 2,
        Error   = 3,
        Fatal   = 4,
    }

    // ─── PLC 비트 정의 ────────────────────────────────────────────────
    public static class PlcBit
    {
        public const int TRACK_PRESENT   = 0x01;
        public const int TRACK_ERROR     = 0x02;
        public const int TRACK_SUSPEND   = 0x04;
        public const int TRACK_REPORT    = 0x08;
        public const int DOOR_OPEN       = 0x10;
        public const int DOOR_CLOSE      = 0x20;
        public const int SC_BUSY         = 0x01;
        public const int SC_FAULT        = 0x02;
        public const int SC_READY        = 0x04;
    }
}
