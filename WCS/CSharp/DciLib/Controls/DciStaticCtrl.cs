using System;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciStaticCtrl에 대응.
    /// gl/gt/gr/gb(%): 실제 그리기 영역 축소.
    /// border: 0=None, 1=Rectangle, 2=LargeFont(Rectangle), 3=RoundRect, 4=Ellipse (SKI 기준)
    /// pcolor/pwidth/pstyle: 테두리 펜 색상/두께/스타일
    /// </summary>
    public class DciStaticCtrl : DciControl
    {
        public int    GapLeft      { get; private set; }
        public int    GapTop       { get; private set; }
        public int    GapRight     { get; private set; }
        public int    GapBottom    { get; private set; }
        public string PenColorHex  { get; private set; } = "000000";
        public int    PenWidth     { get; private set; } = 1;

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            GapLeft    = ParseInt(node, "gl", 0);
            GapTop     = ParseInt(node, "gt", 0);
            GapRight   = ParseInt(node, "gr", 0);
            GapBottom  = ParseInt(node, "gb", 0);
            PenColorHex = Attr(node, "pcolor", "000000");
            PenWidth    = ParseInt(node, "pwidth", 1);
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var outer = ScreenRect;
            double W = outer.Width, H = outer.Height;

            // gl/gt/gr/gb % 적용 (SKI: rcControlS.left += w * m_rcGap.left / 100)
            double l = outer.Left   + W * GapLeft   / 100.0;
            double t = outer.Top    + H * GapTop    / 100.0;
            double r = outer.Right  - W * GapRight  / 100.0;
            double b = outer.Bottom - H * GapBottom / 100.0;

            if (r <= l || b <= t) return;
            var rect = new Rect(l, t, r - l, b - t);

            var bgBrush = BgMode == 2 ? DciColor.ToBrush(BgColorHex) : null;
            var fgBrush = DciColor.ToBrush(FgColorHex);

            // 테두리 펜 (BorderStyle != 0 이면 pcolor/pwidth 사용)
            Pen? borderPen = (BorderStyle != 0 && PenWidth > 0)
                ? new Pen(DciColor.ToBrush(PenColorHex, Colors.Black), Math.Max(0.5, PenWidth))
                : null;

            // ── 배경 + 테두리 (SKI: pDC->Rectangle/RoundRect/Ellipse with brush+pen) ──
            switch (BorderStyle)
            {
                case 3: // enBorderRoundRect
                {
                    double rxy = Math.Min(rect.Width, rect.Height) / 4.0;
                    var geom = new RectangleGeometry(rect, rxy, rxy);
                    dc.DrawGeometry(bgBrush, borderPen, geom);
                    break;
                }
                case 4: // enBorderEllipse
                    dc.DrawEllipse(bgBrush, borderPen,
                        new Point(rect.Left + rect.Width / 2, rect.Top + rect.Height / 2),
                        rect.Width / 2, rect.Height / 2);
                    break;
                default: // 0=None, 1=Rectangle, 2=LargeFont(Rectangle)
                    dc.DrawRectangle(bgBrush, borderPen, rect);
                    break;
            }

            // ── 텍스트 ──────────────────────────────────────────────────
            if (!string.IsNullOrEmpty(Text))
            {
                double dpi = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;
                double fs  = ResolveFont(rect);

                var ft = MakeText(Text, fs, fgBrush, dpi);

                if (FgMode == 2) // 텍스트 배경 채우기
                {
                    double ty0 = rect.Top + (rect.Height - ft.Height) / 2 - 1;
                    dc.DrawRectangle(bgBrush, null, new Rect(rect.Left, ty0, rect.Width, ft.Height + 2));
                }

                double ox = Pressed ? 1 : 0;
                double tx = rect.Left + (rect.Width  - ft.Width)  / 2 + ox;
                double ty = rect.Top  + (rect.Height - ft.Height) / 2 + ox;
                dc.DrawText(ft, new Point(tx, ty));
            }
        }

        public override void OnClick(System.Windows.Point pt) { }

        public void SetText(string text) => Text = text;
    }
}
