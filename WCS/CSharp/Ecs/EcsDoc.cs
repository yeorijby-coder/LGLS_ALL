using System;
using System.IO;
using System.Threading.Tasks;
using Ecs.Communication;
using Ecs.Config;
using Ecs.Equipment;
using Ecs.Job;
using Ecs.Log;

namespace Ecs
{
    /// <summary>
    /// C++의 CEcsDoc에 대응하는 ECS 핵심 도큐먼트 클래스.
    /// 모든 장비, Job, 통신, 설정, 로그를 총괄하는 중앙 허브.
    /// MFC의 Document/View 패턴 → C#의 서비스 클래스로 구현.
    /// </summary>
    public class EcsDoc
    {
        // ─── 서브시스템 ──────────────────────────────────────────────
        public EcsConfig   Config   { get; } = new EcsConfig();
        public EcsLogger   Logger   { get; } = new EcsLogger();
        public JobManager  Jobs     { get; } = new JobManager();
        public EcsDefine?  Define   { get; private set; }

        // ─── 장비 배열 ───────────────────────────────────────────────
        public EquipmentArray CvArray  { get; } = new();
        public EquipmentArray ScArray  { get; } = new();
        public EquipmentArray RtvArray { get; } = new();
        public EquipmentArray BcrArray { get; } = new();

        // ─── 레이아웃 (WPF) ─────────────────────────────────────────
        public EcsLayout? Layout  { get; private set; }
        public EcsLayout? Layout2 { get; private set; }
        public EcsLayout? Layout3 { get; private set; }

        // ─── 통신 ────────────────────────────────────────────────────
        public HostSocket?   HostClient   { get; private set; }
        public HostListener? HostListener { get; private set; }
        public MonitorServer MonitorSv    { get; } = new MonitorServer();

        // ─── 시스템 상태 ────────────────────────────────────────────
        public SystemMode    Mode         { get; private set; } = SystemMode.Offline;
        public ControlState  CtrlState    { get; private set; } = ControlState.Init;
        public EquipmentState SysState    { get; private set; } = EquipmentState.Normal;

        // ─── 이벤트 ─────────────────────────────────────────────────
        public event EventHandler<Cv>?  CvStatusChanged;
        public event EventHandler<Sc>?  ScStatusChanged;
        public event EventHandler<Rtv>? RtvStatusChanged;
        public event Action<string>?    SystemLog;

        // ─── 초기화 ─────────────────────────────────────────────────

        public bool Initialize(string configPath = EcsPath.ConfigFile)
        {
            try
            {
                // 설정 로딩
                if (File.Exists(configPath))
                    Config.Load(configPath);

                // 로그 초기화
                Logger.Init(Path.GetFullPath(Config.BackupFolder.Replace("Backup", "Log")));
                Log("ECS 초기화 시작");

                // XML 장비 정의 파싱
                Define = new EcsDefine(this);
                string defPath = Path.GetFullPath(EcsPath.EcsDefineXml);
                if (File.Exists(defPath))
                {
                    if (!Define.ParseXml(defPath))
                        Log("EcsDefine.xml 파싱 실패 - 기본값 사용");
                    else
                        Log($"EcsDefine.xml 파싱 완료 CV:{CvArray.Count} SC:{ScArray.Count} RTV:{RtvArray.Count}");
                }

                // 레이아웃 준비 (WPF: View에서 별도 로드)
                Layout  = new EcsLayout(this, EcsPath.EcsLayoutXml);
                Layout2 = new EcsLayout(this, EcsPath.EcsLayout2Xml);
                Layout3 = new EcsLayout(this, EcsPath.EcsLayout3Xml);

                // 모니터 서버 시작
                MonitorSv.Start(Config.MulticastGroup, Config.MulticastPort);

                // Job 백업 복원
                RestoreJobs();

                // 호스트 리스너 시작
                HostListener = new HostListener(Config.HostListenPort);
                HostListener.HostConnected += OnHostConnected;
                HostListener.Start();

                Log("ECS 초기화 완료");
                return true;
            }
            catch (Exception ex)
            {
                Log($"초기화 오류: {ex.Message}");
                return false;
            }
        }

        // ─── 시스템 제어 ─────────────────────────────────────────────

        public void StartEquipments()
        {
            CvArray.StartAll();
            ScArray.StartAll();
            RtvArray.StartAll();
            BcrArray.StartAll();
            CtrlState = ControlState.Auto;
            Log("모든 장비 시작");
        }

        public void StopEquipments()
        {
            CvArray.StopAll();
            ScArray.StopAll();
            RtvArray.StopAll();
            BcrArray.StopAll();
            CtrlState = ControlState.Paused;
            Log("모든 장비 중지");
        }

        public void ProcessOnLine()
        {
            Mode = SystemMode.Remote;
            Log("온라인 전환");
        }

        public void ProcessOffLine()
        {
            Mode = SystemMode.Offline;
            Log("오프라인 전환");
        }

        public void ProcessPausing()
        {
            CtrlState = ControlState.Pausing;
            Log("일시정지 요청");
        }

        public void ProcessResume()
        {
            CtrlState = ControlState.Auto;
            Log("재개");
        }

        // ─── Job 처리 ────────────────────────────────────────────────

        public bool AddJob(Job.JobItem job)
        {
            if (!Jobs.Add(job)) return false;
            Log($"Job 추가: Lugg={job.LuggNum} Type={job.JobType}");
            DispatchJob(job);
            return true;
        }

        private void DispatchJob(Job.JobItem job)
        {
            // Job 타입에 따라 적절한 장비에 배분
            switch (job.JobType)
            {
                case JobType.Store:
                    // 빈 위치를 찾아 SC에 입고 명령 생성 - 향후 구현
                    break;
                case JobType.Retrieve:
                    // SC에 출고 명령 생성 - 향후 구현
                    break;
            }
        }

        // ─── 장비 상태 변경 콜백 ────────────────────────────────────

        internal void OnCvStatusChanged(Cv cv)
        {
            CvStatusChanged?.Invoke(this, cv);
            cv.MulticastInfo(MonitorSv);
        }

        internal void OnScStatusChanged(Sc sc)
        {
            ScStatusChanged?.Invoke(this, sc);
            sc.MulticastInfo(MonitorSv);
        }

        internal void OnRtvStatusChanged(Rtv rtv)
        {
            RtvStatusChanged?.Invoke(this, rtv);
            rtv.MulticastInfo(MonitorSv);
        }

        // ─── 호스트 통신 ─────────────────────────────────────────────

        private void OnHostConnected(object? sender, HostSocket socket)
        {
            HostClient = socket;
            socket.MessageReceived += OnHostMessage;
            Log($"호스트 연결: {socket.PeerAddress}:{socket.PeerPort}");
        }

        private void OnHostMessage(object? sender, HostMessage msg)
        {
            switch (msg.Command)
            {
                case HostCmd.Order:
                    ProcessHostOrder(msg);
                    break;
                case HostCmd.Cancel:
                    Jobs.Cancel(msg.LuggNum);
                    break;
                case HostCmd.Status:
                    SendAllStatus();
                    break;
            }
        }

        private void ProcessHostOrder(HostMessage msg)
        {
            var job = new Job.JobItem
            {
                LuggNum    = msg.LuggNum,
                JobType    = msg.JobType,
                DstStation = msg.DstStation,
                DstRow     = msg.DstRow,
                DstCol     = msg.DstCol,
            };
            AddJob(job);
        }

        private void SendAllStatus()
        {
            foreach (var cv in CvArray)
                HostClient?.SendStatus(0, JobStatus.Wait);
        }

        // ─── 백업/복원 ────────────────────────────────────────────────

        private void RestoreJobs()
        {
            string backupFile = Path.Combine(Config.BackupFolder, "jobs.dat");
            Jobs.Restore(backupFile);
        }

        public void BackupJobs()
        {
            Directory.CreateDirectory(Config.BackupFolder);
            string backupFile = Path.Combine(Config.BackupFolder, "jobs.dat");
            Jobs.Backup(backupFile);
        }

        // ─── 종료 ────────────────────────────────────────────────────

        public void Shutdown()
        {
            BackupJobs();
            StopEquipments();
            HostListener?.Stop();
            HostClient?.Close();
            MonitorSv.Dispose();
            Logger.Close();
        }

        private void Log(string msg)
        {
            Logger.Info(msg);
            SystemLog?.Invoke(msg);
        }
    }
}
