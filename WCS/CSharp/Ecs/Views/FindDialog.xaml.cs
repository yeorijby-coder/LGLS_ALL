using System.Collections.ObjectModel;
using System.Windows;
using DciLib;

namespace Ecs.Views
{
    /// <summary>
    /// 찾기 다이얼로그. C++ CViewSearchDlg 대응.
    /// 레이아웃(DciMaster)의 컨트롤을 ID/라벨로 검색해 목록에 표시한다.
    /// </summary>
    public partial class FindDialog : Window
    {
        private readonly EcsDoc _doc;

        public class Row { public string Kind { get; set; } = ""; public string Id { get; set; } = ""; public string Text { get; set; } = ""; }
        private readonly ObservableCollection<Row> _rows = new();

        public FindDialog(EcsDoc doc)
        {
            InitializeComponent();
            _doc = doc;
            LstResult.ItemsSource = _rows;
        }

        private void BtnFind_Click(object sender, RoutedEventArgs e)
        {
            _rows.Clear();
            string q = (TxtQuery.Text ?? "").Trim().ToLower();

            foreach (var layout in new[] { _doc.Layout, _doc.Layout2, _doc.Layout3 })
            {
                var m = layout?.DciMaster;
                if (m == null) continue;
                foreach (var c in m.AllControls)
                {
                    string id = c.Id.ToString();
                    string tx = c.Text ?? "";
                    if (q.Length == 0 ||
                        id.ToLower().Contains(q) || tx.ToLower().Contains(q))
                    {
                        _rows.Add(new Row { Kind = c.GetType().Name.Replace("Dci", "").Replace("Ctrl", ""), Id = id, Text = tx });
                    }
                }
            }
            TxtCount.Text = $"{_rows.Count}건";
        }

        private void BtnClose_Click(object sender, RoutedEventArgs e) => Close();
    }
}
