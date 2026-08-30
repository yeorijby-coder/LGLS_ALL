using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;
using Ecs.Equipment;
using DciLib.Controls;

namespace Ecs.Views
{
    public partial class MainWindow : Window
    {
        private readonly EcsDoc _doc = App.Doc;
        private readonly DispatcherTimer _uiTimer = new();

        private static readonly SolidColorBrush _ledGreen  = new(Color.FromRgb(0x00, 0xFF, 0x66));
        private static readonly SolidColorBrush _ledRed    = new(Color.FromRgb(0xFF, 0x44, 0x44));
        private static readonly SolidColorBrush _ledGray   = new(Color.FromRgb(0x55, 0x55, 0x55));
        private static readonly SolidColorBrush _ledYellow = new(Color.FromRgb(0xFF, 0xDD, 0x00));

        public MainWindow()
        {
            InitializeComponent();

            // 앱 아이콘(작업표시줄/제목표시줄): 다중크기 app.ico를 파일에서 로드
            try
            {
                string ico = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "app.ico");
                if (System.IO.File.Exists(ico))
                    Icon = System.Windows.Media.Imaging.BitmapFrame.Create(
                        new Uri(ico), System.Windows.Media.Imaging.BitmapCreateOptions.None,
                        System.Windows.Media.Imaging.BitmapCacheOption.OnLoad);
            }
            catch { }

            // 실행 시 화면 왼쪽 절반에 배치(오른쪽은 Claude/터미널). 반반 분할 워크플로.
            try
            {
                var wa = SystemParameters.WorkArea;
                WindowStartupLocation = WindowStartupLocation.Manual;
                WindowState = WindowState.Normal;
                Left   = wa.Left;
                Top    = wa.Top;
                Width  = wa.Width / 2;
                Height = wa.Height;
            }
            catch { }

            _doc.CvStatusChanged  += (s, cv)  => Dispatcher.Invoke(UpdateStatus);
            _doc.ScStatusChanged  += (s, sc)  => Dispatcher.Invoke(UpdateStatus);
            _doc.RtvStatusChanged += (s, rtv) => Dispatcher.Invoke(UpdateStatus);
            _doc.SystemLog        += msg => Dispatcher.Invoke(() => TxtStatus.Text = msg);
            _doc.Jobs.JobAdded    += (s, j) => Dispatcher.Invoke(UpdateJobCount);
            _doc.Jobs.JobRemoved  += (s, j) => Dispatcher.Invoke(UpdateJobCount);

            _uiTimer.Interval = TimeSpan.FromMilliseconds(500);
            _uiTimer.Tick += (s, e) => { UpdateStatus(); UpdateClock(); };
            _uiTimer.Start();

            Loaded += (s, e) => { LoadLayouts(); UpdateClock(); };
        }

        // ─── 레이아웃 로딩 ──────────────────────────────────────

        private void LoadLayouts()
        {
            _doc.Layout?.Load();
            _doc.Layout2?.Load();
            _doc.Layout3?.Load();

            if (_doc.Layout != null)
            {
                LayoutView1.SetLayout(_doc.Layout);
                _doc.Layout.ControlClicked += OnLayoutControlClicked;
            }
            if (_doc.Layout2 != null)
            {
                LayoutView2.SetLayout(_doc.Layout2);
                _doc.Layout2.ControlClicked += OnLayoutControlClicked;
            }
            if (_doc.Layout3 != null)
            {
                LayoutView3.SetLayout(_doc.Layout3);
                _doc.Layout3.ControlClicked += OnLayoutControlClicked;
            }

            EqStatusPanel.SetDoc(_doc);
            JobListView.SetDoc(_doc);
            LogView.SetLogger(_doc.Logger);

            // 우측 Job 미니 목록 연결
            LvJobMini.ItemsSource = _doc.Jobs.Items;

            // 시작 시 DB(JOB_MST)에서 작업 로드 (접속 시). 실패해도 무시.
            int n = _doc.LoadJobsFromDb();
            if (n >= 0) TxtStatus.Text = $"JOB_MST에서 {n}건 로드";

            UpdateStatus();
        }

        // ─── UI 갱신 ────────────────────────────────────────────

        private void UpdateStatus()
        {
            TxtMode.Text  = $"[{_doc.Mode}]";
            TxtCtrl.Text  = $"[{_doc.CtrlState}]";
            UpdateJobCount();
            UpdateCommLeds();

            LayoutView1.Invalidate();
            LayoutView2.Invalidate();
            LayoutView3.Invalidate();
        }

        private void UpdateJobCount()
        {
            string cnt = $"Job: {_doc.Jobs.Count}";
            TxtJobCnt.Text  = cnt;
            TxtJobCnt2.Text = $"({_doc.Jobs.Count})";
        }

        private void UpdateClock()
        {
            TxtClock.Text = DateTime.Now.ToString("yyyy-MM-dd  HH:mm:ss");
        }

        private void UpdateCommLeds()
        {
            // HOST
            bool hostConn = _doc.HostClient?.IsConnected ?? false;
            LedHost.Background = hostConn ? _ledGreen : _ledGray;
            TxtLedHost.Foreground = hostConn ? _ledGreen : new SolidColorBrush(Color.FromRgb(0x66, 0x88, 0xAA));

            // CV
            SetCommLed(LedCv1, TxtLedCv1, _doc.CvArray.Count > 0 && _doc.CvArray[0].IsConnected);

            // SC #1-4
            SetCommLed(LedSc1, TxtLedSc1, _doc.ScArray.Count > 0 && _doc.ScArray[0].IsConnected);
            SetCommLed(LedSc2, TxtLedSc2, _doc.ScArray.Count > 1 && _doc.ScArray[1].IsConnected);
            SetCommLed(LedSc3, TxtLedSc3, _doc.ScArray.Count > 2 && _doc.ScArray[2].IsConnected);
            SetCommLed(LedSc4, TxtLedSc4, _doc.ScArray.Count > 3 && _doc.ScArray[3].IsConnected);

            // RTV
            SetCommLed(LedRtv1, TxtLedRtv1, _doc.RtvArray.Count > 0 && _doc.RtvArray[0].IsConnected);
            SetCommLed(LedRtv2, TxtLedRtv2, _doc.RtvArray.Count > 1 && _doc.RtvArray[1].IsConnected);

            // BCR
            SetCommLed(LedBcr1, TxtLedBcr1, _doc.BcrArray.Count > 0 && _doc.BcrArray[0].IsConnected);
            SetCommLed(LedBcr2, TxtLedBcr2, _doc.BcrArray.Count > 1 && _doc.BcrArray[1].IsConnected);
        }

        private static void SetCommLed(System.Windows.Controls.Border led,
                                       System.Windows.Controls.TextBlock lbl, bool connected)
        {
            led.Background  = connected ? _ledGreen : _ledGray;
            lbl.Foreground  = connected
                ? _ledGreen
                : new SolidColorBrush(Color.FromRgb(0x66, 0x88, 0xAA));
        }

        // ─── 툴바 버튼 핸들러 ───────────────────────────────────

        private void BtnOnline_Click(object sender, RoutedEventArgs e)
        {
            _doc.ProcessOnLine();
            TxtStatus.Text = "온라인 전환";
        }

        private void BtnOffline_Click(object sender, RoutedEventArgs e)
        {
            _doc.ProcessOffLine();
            TxtStatus.Text = "오프라인 전환";
        }

        private void BtnStart_Click(object sender, RoutedEventArgs e)
        {
            _doc.StartEquipments();
            TxtStatus.Text = "장비 시작됨";
        }

        private void BtnStop_Click(object sender, RoutedEventArgs e)
        {
            _doc.StopEquipments();
            TxtStatus.Text = "장비 중지됨";
        }

        private void BtnPause_Click(object sender, RoutedEventArgs e)
        {
            if (_doc.CtrlState == ControlState.Auto)
            {
                _doc.ProcessPausing();
                TxtStatus.Text = "일시 정지 중...";
            }
            else
            {
                _doc.ProcessResume();
                TxtStatus.Text = "재개됨";
            }
        }

        private void BtnBackup_Click(object sender, RoutedEventArgs e)
        {
            _doc.BackupJobs();
            TxtStatus.Text = "백업 완료";
        }

        // ─── 메뉴 핸들러 ────────────────────────────────────────

        private void Menu_Config_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("환경 설정 기능은 추후 구현 예정입니다.", "ECS",
                            MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Menu_HostConnect_Click(object sender, RoutedEventArgs e)
        {
            TxtStatus.Text = "Host 연결 시도...";
        }

        private void Menu_Exit_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("프로그램을 종료하시겠습니까?", "ECS 종료",
                                MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
                Application.Current.Shutdown();
        }

        private void Menu_SuspendAll_Click(object sender, RoutedEventArgs e)
        {
            TxtStatus.Text = "모든 설비 정지 요청";
        }

        private void Menu_ResumeAll_Click(object sender, RoutedEventArgs e)
        {
            TxtStatus.Text = "모든 설비 정지 해제 요청";
        }

        private void Menu_JobManager_Click(object sender, RoutedEventArgs e)
        {
            TabLayout.SelectedIndex = 4; // Job 목록 탭
        }

        private void Menu_JobOffline_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("반자동 작업 기능은 추후 구현 예정입니다.", "ECS",
                            MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Menu_ClearJobs_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("모든 작업 정보를 초기화하시겠습니까?", "작업 초기화",
                                MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.Yes)
            {
                _doc.Jobs.Clear();
                TxtStatus.Text = "작업 정보 초기화 완료";
            }
        }

        private void Menu_LogAll_Click(object sender, RoutedEventArgs e)
        {
            TabLayout.SelectedIndex = 3; // 로그 탭
        }

        private void Menu_Warning_Click(object sender, RoutedEventArgs e)
        {
            TxtStatus.Text = "알람 관리 기능은 추후 구현 예정입니다.";
        }

        private void Menu_ClearLog_Click(object sender, RoutedEventArgs e)
        {
            _doc.Logger.Clear();
            TxtStatus.Text = "로그 지움";
        }

        private void Menu_Layout1_Click(object sender, RoutedEventArgs e) => TabLayout.SelectedIndex = 0;
        private void Menu_Layout2_Click(object sender, RoutedEventArgs e) => TabLayout.SelectedIndex = 1;
        private void Menu_Layout3_Click(object sender, RoutedEventArgs e) => TabLayout.SelectedIndex = 2;
        private void Menu_JobList_Click(object sender, RoutedEventArgs e)  => TabLayout.SelectedIndex = 4;
        private void Menu_LogTab_Click(object sender, RoutedEventArgs e)   => TabLayout.SelectedIndex = 3;

        private void Menu_LayoutInfo_Click(object sender, RoutedEventArgs e)
        {
            var layout = _doc.Layout;
            if (layout == null) { MessageBox.Show("레이아웃 미로딩"); return; }
            MessageBox.Show(
                $"Grid: {layout.DciMaster.GridLeft},{layout.DciMaster.GridBottom} ~ " +
                $"{layout.DciMaster.GridRight},{layout.DciMaster.GridTop}\n" +
                $"컨트롤 수: {layout.DciMaster.AllControls.Count}",
                "LAYOUT 정보");
        }

        // 메뉴 조회창(중복 방지): 형식별 단일 인스턴스
        private readonly Dictionary<Type, Window> _menuDialogs = new();
        private void OpenMenuDialog<T>(Func<T> factory) where T : Window
        {
            if (_menuDialogs.TryGetValue(typeof(T), out var w) && w.IsLoaded)
            {
                if (w.WindowState == WindowState.Minimized) w.WindowState = WindowState.Normal;
                w.Activate(); return;
            }
            var dlg = factory();
            dlg.Owner = this;
            dlg.Closed += (s, e) => _menuDialogs.Remove(typeof(T));
            _menuDialogs[typeof(T)] = dlg;
            dlg.Show();
        }

        private void Menu_EquipLegend_Click(object sender, RoutedEventArgs e)
            => OpenMenuDialog(() => new LegendDialog(_doc));

        private void Menu_About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("ECS (Equipment Control System)\nVersion 1.0\n\nC# WPF Port from SKI C++ MBCS",
                            "ECS 정보", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // ─── 리본 핸들러 (C++ ECS 리본 대응) ───────────────────
        // 아직 전용 다이얼로그가 없는 항목은 대응 C++ 클래스명과 함께 안내.
        private void NotImpl(string title, string cppClass)
            => MessageBox.Show($"[{title}]\n\nC++의 {cppClass} 에 대응하는 기능입니다.\n현재 C# 포트에서는 구현 예정입니다.",
                               "ECS", MessageBoxButton.OK, MessageBoxImage.Information);

        // ECS 탭 · 환경설정 — C++ CEqpSuspendDlg / CConfigLogDelete / CUserUserDlg 대응 조회창
        private void Rb_EqpSuspend_Click(object sender, RoutedEventArgs e)
            => OpenMenuDialog(() => new EqpSuspendDialog(_doc));
        private void Rb_ConfigLogDelete_Click(object sender, RoutedEventArgs e)
            => OpenMenuDialog(() => new ConfigLogDeleteDialog(_doc));
        private void Rb_User_Click(object sender, RoutedEventArgs e)
            => OpenMenuDialog(() => new UserDialog(_doc));
        // ECS 탭 · 뷰 — 작업정보: DB(JOB_MST)에서 로드 후 Job 목록 탭 표시
        private void Rb_JobInfo_Click(object sender, RoutedEventArgs e)
        {
            TabLayout.SelectedIndex = 4;
            int n = _doc.LoadJobsFromDb();
            TxtStatus.Text = n >= 0
                ? $"JOB_MST에서 {n}건 로드"
                : $"DB 미연결 — 인메모리 작업 표시 ({_doc.Db?.LastError})";
        }
        private void Rb_HostEmptyPlt_Click(object sender, RoutedEventArgs e)
            => OpenMenuDialog(() => new HostEmptyPltDialog(_doc));
        private FindDialog? _findDlg;
        private void Rb_Search_Click(object sender, RoutedEventArgs e)
        {
            if (_findDlg == null || !_findDlg.IsLoaded)
            {
                _findDlg = new FindDialog(_doc) { Owner = this };
                _findDlg.Closed += (s, ev) => _findDlg = null;
            }
            _findDlg.Show(); _findDlg.Activate();
        }
        // ECS 탭 · 창고 모니터링 (C++ OnCommandTrackTextMode: 트랙 텍스트 표시 모드 0/1/2)
        private void Rb_TrackTextJob_Click(object sender, RoutedEventArgs e)    => SetTrackTextMode(0, "작업번호");
        private void Rb_TrackTextTrack_Click(object sender, RoutedEventArgs e)  => SetTrackTextMode(1, "트랙번호");
        private void Rb_TrackTextProd_Click(object sender, RoutedEventArgs e)   => SetTrackTextMode(2, "제품정보");
        private void SetTrackTextMode(int mode, string name)
        {
            _doc.TrackTextMode = mode;   // 레이아웃 트랙 표시 모드
            if (_doc.Layout  != null) _doc.Layout.DciMaster.TrackTextMode  = mode;
            if (_doc.Layout2 != null) _doc.Layout2.DciMaster.TrackTextMode = mode;
            if (_doc.Layout3 != null) _doc.Layout3.DciMaster.TrackTextMode = mode;
            LayoutView1.Invalidate();
            LayoutView2.Invalidate();
            LayoutView3.Invalidate();
            TxtStatus.Text = $"트랙 표시: {name}";
        }
        // MANUAL 탭 · 수동조작
        private ManualJobDialog? _manualJobDlg;
        private void Rb_ManualJob_Click(object sender, RoutedEventArgs e)
        {
            if (_manualJobDlg == null || !_manualJobDlg.IsLoaded)
            {
                _manualJobDlg = new ManualJobDialog(_doc) { Owner = this };
                _manualJobDlg.Closed += (s, ev) => _manualJobDlg = null;
            }
            _manualJobDlg.Show();
            _manualJobDlg.Activate();
        }
        private ManualScDialog? _manualScDlg;
        private void Rb_ManualSc_Click(object sender, RoutedEventArgs e)
        {
            if (_manualScDlg == null || !_manualScDlg.IsLoaded)
            {
                _manualScDlg = new ManualScDialog(_doc) { Owner = this };
                _manualScDlg.Closed += (s, ev) => _manualScDlg = null;
            }
            _manualScDlg.Show(); _manualScDlg.Activate();
        }
        private void Rb_ManualRtv_Click(object sender, RoutedEventArgs e)
        {
            // Rtv.cs에 명령 계층이 아직 없어 상태만 표시 (명령 발행은 Rtv 장비계층 구현 후)
            var sb = new System.Text.StringBuilder("RTV 상태:\n\n");
            if (_doc.RtvArray.Count == 0) sb.Append("(등록된 RTV 없음)");
            for (int i = 0; i < _doc.RtvArray.Count; i++)
            {
                var r = _doc.RtvArray[i];
                sb.AppendLine($"• RTV#{r.Number} ({r.DeviceName})  상태:{r.State}  연결:{(r.IsConnected ? "O" : "X")}");
            }
            sb.Append("\n※ 수동 RTV 명령(이동/입출고)은 Rtv 장비계층(Rtv.cs) 구현 후 활성화됩니다.");
            MessageBox.Show(sb.ToString(), "수동 RTV (CManualRtv 대응)", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // ─── 레이아웃 설비 클릭 → 대화상자 (C++ EcsView Cv/Sc/RtvSkinDlg 대응) ───
        // 컨트롤 타입 + ID 접두(SC=1712xxxx, RTV=1713xxxx)로 설비를 판별해 창을 연다.
        // 열려있는 설비 상태창(중복 방지): id → Window
        private readonly Dictionary<string, Window> _skinDialogs = new();

        private void OnLayoutControlClicked(object? sender, DciControl ctrl)
        {
            string id = ctrl.Id.ToString();
            switch (ctrl)
            {
                case DciRvCtrl:
                    if (id.StartsWith("1713"))                         // RTV/RGV
                        OpenSkin(id, () => new RtvSkinDialog(_doc, LastNo(id)));
                    else                                               // SC(스태커크레인)
                        OpenSkin(id, () => new ScSkinDialog(_doc, LastNo(id)));
                    break;
                case DciRackCtrl:
                    Rb_ManualSc_Click(this, new RoutedEventArgs());    // 랙 클릭 → 수동 SC 작업
                    break;
                case DciTrackCtrl:                                     // CV 트랙
                    OpenSkin(id, () => new CvSkinDialog(_doc, LastNo(id)));
                    break;
                // 그 외(Static/Button/Rack라벨 등)는 무시
            }
        }

        /// <summary>레이아웃 ID의 끝 4자리 = 설비 번호(SC=sc_no, RTV=rtv_no, CV=mc_no).</summary>
        private static int LastNo(string id)
            => id.Length >= 4 && int.TryParse(id.Substring(id.Length - 4), out int n) ? n : 0;

        private void OpenSkin(string id, Func<Window> factory)
        {
            if (_skinDialogs.TryGetValue(id, out var w) && w.IsLoaded)
            {
                if (w.WindowState == WindowState.Minimized) w.WindowState = WindowState.Normal;
                w.Activate();
                return;
            }
            var dlg = factory();
            dlg.Owner = this;
            dlg.Closed += (s, e) => _skinDialogs.Remove(id);
            _skinDialogs[id] = dlg;
            dlg.Show();
        }
        // MANUAL 탭 · 반자동 TEST
        private SemiTestDialog? _semiTestDlg;
        private void Rb_SemiTest_Click(object sender, RoutedEventArgs e)
        {
            if (_semiTestDlg == null || !_semiTestDlg.IsLoaded)
            {
                _semiTestDlg = new SemiTestDialog(_doc) { Owner = this };
                _semiTestDlg.Closed += (s, ev) => _semiTestDlg = null;
            }
            _semiTestDlg.Show();
            _semiTestDlg.Activate();
        }
        // LOG 탭 — 로그 종류(카테고리)별 필터로 분리 (C++ 로그 5종 대응)
        private void ShowLog(string category, string name)
        {
            TabLayout.SelectedIndex = 3; // 로그 탭
            LogView.SetCategoryFilter(category);
            TxtStatus.Text = $"로그: {name}";
        }
        private void Rb_LogIo_Click(object sender, RoutedEventArgs e)      => ShowLog("IO",     "작업로그");
        private void Rb_LogMes_Click(object sender, RoutedEventArgs e)     => ShowLog("HOST",   "HOST로그");
        private void Rb_LogEqpHis_Click(object sender, RoutedEventArgs e)  => ShowLog("EQP",    "설비에러이력");
        private void Rb_LogClient_Click(object sender, RoutedEventArgs e)  => ShowLog("USER",   "유저사용로그");
        private void Rb_LogEcs_Click(object sender, RoutedEventArgs e)     => ShowLog("",       "ECS프로그램로그(전체)");

        // ─── 윈도우 종료 ─────────────────────────────────────────

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _uiTimer.Stop();
            _doc.Shutdown();
        }
    }
}
