using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Threading;
using Ecs.Job;

namespace Ecs.Views
{
    /// <summary>
    /// 반자동 TEST 창. C++ CSemiTestDlg 대응.
    /// HOST 없이 여러 라인이 입고→(완료)→출고→(완료) 사이클을 자동 반복하며
    /// JobManager에 작업을 생성·완료·제거하여 파이프라인을 검증한다.
    /// </summary>
    public partial class SemiTestDialog : Window
    {
        private readonly EcsDoc _doc;
        private readonly DispatcherTimer _timer = new();

        public class LogRow
        {
            public string Time { get; set; } = "";
            public int    Row  { get; set; }
            public string Act  { get; set; } = "";
            public int    Lugg { get; set; }
            public string Msg  { get; set; } = "";
        }
        private readonly ObservableCollection<LogRow> _log = new();

        // 라인별 상태: 0=대기(입고), 1=입고완료대기(→출고), 2=출고완료대기(→다음)
        private int[] _state = Array.Empty<int>();
        private int[] _lugg  = Array.Empty<int>();
        private int[] _loc   = Array.Empty<int>();
        private int _rows;
        private int _luggSeq = 9001;   // 테스트 작업번호 시작(실작업과 구분)

        public SemiTestDialog(EcsDoc doc)
        {
            InitializeComponent();
            _doc = doc;
            LstLog.ItemsSource = _log;
            _timer.Tick += Timer_Tick;
            Closed += (s, e) => _timer.Stop();
        }

        private void BtnStart_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(TxtRows.Text, out _rows) || _rows <= 0) _rows = 5;
            if (!int.TryParse(TxtInterval.Text, out int ms) || ms < 100) ms = 800;

            _state = new int[_rows];
            _lugg  = new int[_rows];
            _loc   = new int[_rows];
            for (int r = 0; r < _rows; r++) _loc[r] = r * 100 + 1;

            _timer.Interval = TimeSpan.FromMilliseconds(ms);
            _timer.Start();
            TxtStat.Text = "동작 중...";
        }

        private void BtnStop_Click(object sender, RoutedEventArgs e)
        {
            _timer.Stop();
            TxtStat.Text = "정지됨";
        }

        private void Timer_Tick(object? sender, EventArgs e)
        {
            for (int r = 0; r < _rows; r++)
                StepRow(r);
            TxtStat.Text = $"동작 중... (작업 {_doc.Jobs.Count})";
        }

        private void StepRow(int r)
        {
            switch (_state[r])
            {
                case 0: // 입고 작업 생성
                    _lugg[r] = _luggSeq++;
                    var sto = new JobItem
                    {
                        LuggNum = _lugg[r], JobType = JobType.Store,
                        DstRow = _loc[r] / 100 + 1, DstCol = _loc[r] % 100,
                    };
                    if (_doc.Jobs.Add(sto))
                    {
                        AddLog(r, "입고", _lugg[r], $"입고 생성 R{sto.DstRow}C{sto.DstCol}");
                        _state[r] = 1;
                    }
                    break;

                case 1: // 입고 완료 → 제거 후 출고 생성
                    _doc.Jobs.Remove(_lugg[r]);
                    AddLog(r, "완료", _lugg[r], "입고 완료");
                    _lugg[r] = _luggSeq++;
                    var ret = new JobItem
                    {
                        LuggNum = _lugg[r], JobType = JobType.Retrieve,
                        SrcRow = _loc[r] / 100 + 1, SrcCol = _loc[r] % 100,
                    };
                    if (_doc.Jobs.Add(ret))
                    {
                        AddLog(r, "출고", _lugg[r], $"출고 생성 R{ret.SrcRow}C{ret.SrcCol}");
                        _state[r] = 2;
                    }
                    break;

                case 2: // 출고 완료 → 제거, 다음 로케이션
                    _doc.Jobs.Remove(_lugg[r]);
                    AddLog(r, "완료", _lugg[r], "출고 완료");
                    _loc[r]++;
                    _state[r] = 0;
                    break;
            }
        }

        private void AddLog(int row, string act, int lugg, string msg)
        {
            _log.Insert(0, new LogRow { Time = DateTime.Now.ToString("HH:mm:ss.fff"), Row = row + 1, Act = act, Lugg = lugg, Msg = msg });
            while (_log.Count > 300) _log.RemoveAt(_log.Count - 1);
        }

        private void BtnClearLog_Click(object sender, RoutedEventArgs e) => _log.Clear();
        private void BtnClose_Click(object sender, RoutedEventArgs e) { _timer.Stop(); Close(); }
    }
}
