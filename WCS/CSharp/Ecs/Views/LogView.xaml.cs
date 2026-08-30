using System;
using System.Collections.ObjectModel;
using System.Windows.Controls;
using Ecs.Log;

namespace Ecs.Views
{
    public partial class LogView : UserControl
    {
        private EcsLogger? _logger;
        private readonly ObservableCollection<LogRowVm> _rows = new();
        private int _filterLevel = -1;
        private string _filterText = string.Empty;
        private string _filterCategory = string.Empty;

        /// <summary>로그 종류(카테고리)별 필터. 빈 문자열이면 전체. C++ 로그 5종 분리 대응.</summary>
        public void SetCategoryFilter(string category)
        {
            _filterCategory = category ?? string.Empty;
            Refresh();
        }

        public LogView()
        {
            InitializeComponent();
            LogListView.ItemsSource = _rows;
        }

        public void SetLogger(EcsLogger logger)
        {
            _logger = logger;
            logger.LogAdded += (s, entry) => Dispatcher.Invoke(() => AddEntry(entry));

            // 기존 로그 로딩
            foreach (var e in logger.GetRecent())
                AddEntry(e);
        }

        private void AddEntry(LogEntry entry)
        {
            if (_filterLevel >= 0 && (int)entry.Level < _filterLevel) return;
            if (!string.IsNullOrEmpty(_filterCategory) &&
                !entry.Category.Equals(_filterCategory, StringComparison.OrdinalIgnoreCase)) return;
            if (!string.IsNullOrEmpty(_filterText) &&
                !entry.Message.ToLower().Contains(_filterText.ToLower())) return;

            _rows.Add(new LogRowVm(entry));

            // 최대 2000개 유지
            if (_rows.Count > 2000) _rows.RemoveAt(0);

            if (ChkAutoScroll.IsChecked == true && _rows.Count > 0)
                LogListView.ScrollIntoView(_rows[_rows.Count - 1]);
        }

        private void CbLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var sel = CbLevel.SelectedItem as ComboBoxItem;
            _filterLevel = sel != null ? int.Parse(sel.Tag?.ToString() ?? "-1") : -1;
            Refresh();
        }

        private void TxtSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            _filterText = TxtSearch.Text;
            Refresh();
        }

        private void Refresh()
        {
            _rows.Clear();
            if (_logger == null) return;
            foreach (var e in _logger.GetRecent())
                AddEntry(e);
        }

        private void BtnClear_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            _rows.Clear();
        }
    }

    public class LogRowVm
    {
        public LogEntry Entry { get; }
        public string TimeStr  => Entry.Time.ToString("yyyy-MM-dd HH:mm:ss.fff");
        public string LevelStr => Entry.Level.ToString();
        public string Category => Entry.Category;
        public string Message  => Entry.Message;

        public LogRowVm(LogEntry entry) { Entry = entry; }
    }
}
