using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using BuilderLib;
using DciLib.Controls;
using Microsoft.Win32;

namespace Builder
{
    public partial class MainWindow : Window
    {
        private readonly BuildManager _bm = new BuildManager();
        private DciControl? _selected;
        private bool _propChanging;
        private string _currentGroup = "LAYOUT";

        public MainWindow()
        {
            InitializeComponent();
        }

        // ─── 메뉴 이벤트 ──────────────────────────────────────────────

        private void MenuNew_Click(object sender, RoutedEventArgs e)
        {
            TxtStatus.Text = "새 레이아웃";
        }

        private void MenuOpen_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new OpenFileDialog { Filter = "XML 파일|*.xml|모든 파일|*.*" };
            if (dlg.ShowDialog() == true)
            {
                if (_bm.LoadLayout(dlg.FileName))
                {
                    TxtStatus.Text = $"로딩 완료: {dlg.FileName}";
                    RefreshTree();
                    Redraw();
                }
                else
                {
                    MessageBox.Show("파일 로딩 실패", "오류", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }

        private void MenuSave_Click(object sender, RoutedEventArgs e)
        {
            if (_bm.SaveLayout())
                TxtStatus.Text = "저장 완료";
            else
                MenuSaveAs_Click(sender, e);
        }

        private void MenuSaveAs_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new SaveFileDialog { Filter = "XML 파일|*.xml" };
            if (dlg.ShowDialog() == true)
            {
                if (_bm.SaveLayout(dlg.FileName))
                    TxtStatus.Text = $"저장 완료: {dlg.FileName}";
            }
        }

        private void MenuExit_Click(object sender, RoutedEventArgs e) => Close();
        private void MenuUndo_Click(object sender, RoutedEventArgs e) { }
        private void MenuRedo_Click(object sender, RoutedEventArgs e) { }

        private void MenuDelete_Click(object sender, RoutedEventArgs e)
        {
            if (_selected == null) return;
            _bm.RemoveControl(_selected.Id);
            _selected = null;
            RefreshTree();
            Redraw();
        }

        // ─── 컨트롤 추가 ──────────────────────────────────────────────

        private void MenuAddStatic_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciStaticCtrl");
        private void MenuAddButton_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciButtonCtrl");
        private void MenuAddTrack_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciTrackCtrl");
        private void MenuAddRack_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciRackCtrl");
        private void MenuAddRv_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciRvCtrl");
        private void MenuAddLifter_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciLifterCtrl");
        private void MenuAddOpbox_Click(object sender, RoutedEventArgs e)
            => AddControl("CDciOpboxCtrl");

        private void AddControl(string type)
        {
            var ctrl = _bm.AddControl(_currentGroup, type, 10, 10, 15, 15);
            _selected = ctrl;
            RefreshTree();
            Redraw();
            ShowProperties(ctrl);
        }

        // ─── 캔버스 렌더링 ────────────────────────────────────────────

        private void Redraw()
        {
            EditCanvas.Children.Clear();
            _bm.Master.Resize(EditCanvas.ActualWidth, EditCanvas.ActualHeight);

            var visual = new System.Windows.Media.DrawingVisual();
            using (var dc = visual.RenderOpen())
            {
                dc.DrawRectangle(new SolidColorBrush(Color.FromRgb(26, 26, 46)), null,
                    new Rect(0, 0, EditCanvas.ActualWidth, EditCanvas.ActualHeight));
                _bm.Master.Draw(dc);

                // 선택된 컨트롤 하이라이트
                if (_selected != null)
                {
                    var pen = new Pen(Brushes.Yellow, 1.5) { DashStyle = DashStyles.Dash };
                    dc.DrawRectangle(null, pen, _selected.ScreenRect);
                }
            }

            var host = new BuilderVisualHost(visual);
            EditCanvas.Children.Add(host);
        }

        // ─── 속성 편집 ────────────────────────────────────────────────

        private void ShowProperties(DciControl? ctrl)
        {
            _propChanging = true;
            PropId.Text      = ctrl?.Id.ToString() ?? "";
            PropText.Text    = ctrl?.Text ?? "";
            PropL.Text       = ctrl?.LogLeft.ToString() ?? "";
            PropB.Text       = ctrl?.LogBottom.ToString() ?? "";
            PropR.Text       = ctrl?.LogRight.ToString() ?? "";
            PropT.Text       = ctrl?.LogTop.ToString() ?? "";
            PropBgColor.Text = ctrl?.BgColorHex ?? "";
            PropFgColor.Text = ctrl?.FgColorHex ?? "";
            _propChanging    = false;
        }

        private void Prop_Changed(object sender, TextChangedEventArgs e)
        {
            if (_propChanging || _selected == null) return;
            _selected.Text      = PropText.Text;
            _selected.BgColorHex= PropBgColor.Text;
            _selected.FgColorHex= PropFgColor.Text;
            if (int.TryParse(PropL.Text, out int l)) _selected.LogLeft   = l;
            if (int.TryParse(PropB.Text, out int b)) _selected.LogBottom = b;
            if (int.TryParse(PropR.Text, out int r)) _selected.LogRight  = r;
            if (int.TryParse(PropT.Text, out int t)) _selected.LogTop    = t;
            Redraw();
        }

        // ─── 트리 뷰 ──────────────────────────────────────────────────

        private void RefreshTree()
        {
            CtrlTree.Items.Clear();
            foreach (var group in _bm.Groups)
            {
                var groupItem = new TreeViewItem { Header = group.Name };
                foreach (var ctrl in group.Controls)
                {
                    var ctrlItem = new TreeViewItem
                    {
                        Header = $"[{ctrl.GetType().Name.Replace("Dci","").Replace("Ctrl","")}] {ctrl.Id}",
                        Tag    = ctrl,
                    };
                    groupItem.Items.Add(ctrlItem);
                }
                groupItem.IsExpanded = true;
                CtrlTree.Items.Add(groupItem);
            }
        }

        private void CtrlTree_SelectionChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (CtrlTree.SelectedItem is TreeViewItem item && item.Tag is DciControl ctrl)
            {
                _selected = ctrl;
                ShowProperties(ctrl);
                Redraw();
            }
        }

        // ─── 캔버스 이벤트 ────────────────────────────────────────────

        private void EditCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            var pt = e.GetPosition(EditCanvas);
            var logPt = _bm.Master.ScreenToLogical(pt.X, pt.Y);
            TxtPos.Text = $"L={logPt.X:F0} T={logPt.Y:F0}";
        }

        private void EditCanvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var pt = e.GetPosition(EditCanvas);
            _selected = _bm.Master.HitTest(pt);
            ShowProperties(_selected);
            Redraw();
        }

        private void EditCanvas_MouseUp(object sender, MouseButtonEventArgs e) { }
    }

    internal class BuilderVisualHost : UIElement
    {
        private readonly DrawingVisual _v;
        public BuilderVisualHost(DrawingVisual v) { _v = v; AddVisualChild(v); }
        protected override int VisualChildrenCount => 1;
        protected override Visual GetVisualChild(int i) => _v;
    }
}
