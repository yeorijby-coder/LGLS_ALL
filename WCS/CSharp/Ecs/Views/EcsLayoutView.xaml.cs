using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Ecs.Views
{
    /// <summary>
    /// C++의 CEcsView에 대응하는 WPF 레이아웃 렌더링 뷰.
    /// DciMaster가 관리하는 컨트롤들을 DrawingVisual로 렌더링한다.
    /// </summary>
    public partial class EcsLayoutView : UserControl
    {
        public int LayoutIndex { get; set; }

        private EcsLayout? _layout;
        private LayoutDrawingVisual? _visual;
        private double _zoom = 1.0;

        public EcsLayoutView()
        {
            InitializeComponent();
        }

        public void SetLayout(EcsLayout layout)
        {
            _layout = layout;
            _layout.InvalidateRequested += () => Dispatcher.Invoke(Redraw);

            _visual = new LayoutDrawingVisual();
            DrawCanvas.Children.Add(new VisualHost(_visual));

            // 이미 캔버스 크기가 확정된 경우 즉시 Resize 적용
            if (DrawCanvas.ActualWidth > 0 && DrawCanvas.ActualHeight > 0)
                _layout.Resize(DrawCanvas.ActualWidth, DrawCanvas.ActualHeight);

            Redraw();
        }

        public void Invalidate() => Redraw();

        private void Redraw()
        {
            if (_layout == null || _visual == null) return;

            using var dc = _visual.RenderOpen();
            dc.DrawRectangle(
                new SolidColorBrush(Color.FromRgb(212, 208, 200)), // #D4D0C8 밝은 회색
                null,
                new Rect(0, 0, DrawCanvas.ActualWidth, DrawCanvas.ActualHeight));

            _layout.DciMaster.Draw(dc);
        }

        private void DrawCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            _layout?.Resize(e.NewSize.Width, e.NewSize.Height);
            Redraw();
        }

        private void DrawCanvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton != MouseButtonState.Pressed) return;
            DrawCanvas.CaptureMouse();
            _layout?.HandleMouseDown(e.GetPosition(DrawCanvas));
        }

        private void DrawCanvas_MouseUp(object sender, MouseButtonEventArgs e)
        {
            DrawCanvas.ReleaseMouseCapture();
            _layout?.HandleMouseUp(e.GetPosition(DrawCanvas));
        }

        // ─── 줌 제어 ────────────────────────────────────────────────

        private void ApplyZoom()
        {
            ZoomTransform.ScaleX = _zoom;
            ZoomTransform.ScaleY = _zoom;
            TxtZoom.Text = $"{(int)(_zoom * 100)}%";
        }

        private void BtnZoomIn_Click(object sender, RoutedEventArgs e)
        {
            _zoom = Math.Min(_zoom + 0.1, 4.0);
            ApplyZoom();
        }

        private void BtnZoomOut_Click(object sender, RoutedEventArgs e)
        {
            _zoom = Math.Max(_zoom - 0.1, 0.2);
            ApplyZoom();
        }

        private void BtnZoomReset_Click(object sender, RoutedEventArgs e)
        {
            _zoom = 1.0;
            PanTransform.X = 0; PanTransform.Y = 0;
            ApplyZoom();
        }
    }

    /// <summary>DrawingVisual을 Canvas에 표시하기 위한 호스트 엘리먼트.</summary>
    internal class VisualHost : UIElement
    {
        private readonly DrawingVisual _visual;

        public VisualHost(DrawingVisual visual)
        {
            _visual = visual;
            AddVisualChild(visual);
        }

        protected override int VisualChildrenCount => 1;
        protected override Visual GetVisualChild(int index) => _visual;
    }

    /// <summary>DCI 컨트롤을 그리는 DrawingVisual.</summary>
    internal class LayoutDrawingVisual : DrawingVisual { }
}
