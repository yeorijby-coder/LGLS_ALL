using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;

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
                LayoutView1.SetLayout(_doc.Layout);
            if (_doc.Layout2 != null)
                LayoutView2.SetLayout(_doc.Layout2);
            if (_doc.Layout3 != null)
                LayoutView3.SetLayout(_doc.Layout3);

            EqStatusPanel.SetDoc(_doc);
            JobListView.SetDoc(_doc);
            LogView.SetLogger(_doc.Logger);

            // 우측 Job 미니 목록 연결
            LvJobMini.ItemsSource = _doc.Jobs.Items;

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

        private void Menu_EquipLegend_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("설비 범례 기능은 추후 구현 예정입니다.", "ECS",
                            MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Menu_About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("ECS (Equipment Control System)\nVersion 1.0\n\nC# WPF Port from SKI C++ MBCS",
                            "ECS 정보", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // ─── 윈도우 종료 ─────────────────────────────────────────

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _uiTimer.Stop();
            _doc.Shutdown();
        }
    }
}
