using System;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;

namespace EcsLog
{
    /// <summary>
    /// C++의 EcsLog에 대응하는 로그 뷰어 애플리케이션.
    /// 날짜별 로그 파일 로딩, 검색, 재생(PlayBack) 기능 제공.
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly ObservableCollection<LogRowVm> _rows = new();
        private LogRowVm[] _allRows = Array.Empty<LogRowVm>();
        private CancellationTokenSource _pbCts = new();

        public MainWindow()
        {
            InitializeComponent();
            LogList.ItemsSource = _rows;
            DpDate.SelectedDate = DateTime.Today;
        }

        private void BtnOpen_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new OpenFileDialog { Filter = "로그 파일|*.log|모든 파일|*.*" };
            if (dlg.ShowDialog() == true) LoadFile(dlg.FileName);
        }

        private void BtnLoad_Click(object sender, RoutedEventArgs e)
        {
            if (DpDate.SelectedDate == null) return;
            string date = DpDate.SelectedDate.Value.ToString("yyyyMMdd");
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Log", $"ECS_{date}.log");
            if (File.Exists(path)) LoadFile(path);
            else MessageBox.Show($"파일 없음: {path}");
        }

        private void LoadFile(string path)
        {
            try
            {
                var lines = File.ReadAllLines(path, System.Text.Encoding.UTF8);
                _allRows = lines.Select((l, i) => ParseLine(l, i + 1)).ToArray();
                ApplyFilter();
                TxtStatus.Text = path;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"로딩 오류: {ex.Message}");
            }
        }

        private static LogRowVm ParseLine(string line, int idx)
        {
            // 형식: [yyyy-MM-dd HH:mm:ss.fff] [LEVEL  ] [CATEGORY] MESSAGE
            var vm = new LogRowVm { Index = idx, Raw = line };
            try
            {
                if (line.Length > 30 && line.StartsWith("["))
                {
                    vm.Time     = line.Substring(1, 23);
                    int p1 = line.IndexOf('[', 26);
                    int p2 = line.IndexOf(']', p1 + 1);
                    vm.Level    = line.Substring(p1 + 1, p2 - p1 - 1).Trim();
                    int p3 = line.IndexOf('[', p2 + 1);
                    int p4 = line.IndexOf(']', p3 + 1);
                    vm.Category = line.Substring(p3 + 1, p4 - p3 - 1).Trim();
                    vm.Message  = line.Substring(p4 + 2).Trim();
                }
                else vm.Message = line;
            }
            catch { vm.Message = line; }
            return vm;
        }

        private void TxtSearch_Changed(object sender, TextChangedEventArgs e) => ApplyFilter();
        private void BtnSearch_Click(object sender, RoutedEventArgs e)        => ApplyFilter();

        private void ApplyFilter()
        {
            string search = TxtSearch.Text.ToLower();
            _rows.Clear();
            foreach (var row in _allRows)
            {
                if (!string.IsNullOrEmpty(search) &&
                    !row.Message.ToLower().Contains(search) &&
                    !row.Category.ToLower().Contains(search)) continue;
                _rows.Add(row);
            }
            TxtCount.Text = $"{_rows.Count} 건";
        }

        private async void BtnPlayback_Click(object sender, RoutedEventArgs e)
        {
            _pbCts = new CancellationTokenSource();
            _rows.Clear();
            foreach (var row in _allRows)
            {
                if (_pbCts.IsCancellationRequested) break;
                _rows.Add(row);
                if (_rows.Count > 0) LogList.ScrollIntoView(_rows[_rows.Count - 1]);
                await Task.Delay(50, _pbCts.Token).ContinueWith(_ => { });
            }
        }

        private void BtnStop_Click(object sender, RoutedEventArgs e) => _pbCts.Cancel();
    }

    public class LogRowVm
    {
        public int    Index    { get; set; }
        public string Time     { get; set; } = string.Empty;
        public string Level    { get; set; } = string.Empty;
        public string Category { get; set; } = string.Empty;
        public string Message  { get; set; } = string.Empty;
        public string Raw      { get; set; } = string.Empty;
    }
}
