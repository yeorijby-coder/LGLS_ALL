using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciRvCtrl에 대응. Rail / Fork / Wing을 그리는 RTV 시각화 컨트롤.
    /// </summary>
    public class DciRvCtrl : DciControl
    {
        // type: 0=L2R, 1=R2L, 2=T2B, 3=B2T
        public int    RvType      { get; private set; }
        // forktype: 0=Single, 1=Twin, 2=Double
        public int    ForkType    { get; private set; }
        // forkpos: logical cell index along the rail
        public int    ForkPos     { get; private set; }
        public int    Prod        { get; set; }
        public bool   Loaded      { get; set; }
        public int    Status      { get; set; }
        public string ForkColorHex  { get; private set; } = "808080";
        public string WingColorHex  { get; private set; } = "C0C0C0";
        public string RailColorHex  { get; private set; } = "000000";

        private static readonly Pen _penProd = new Pen(Brushes.Black, 1);

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            RvType       = ParseInt(node, "type",      0);
            ForkType     = ParseInt(node, "forktype",  0);
            ForkPos      = ParseInt(node, "forkpos",   0);
            Prod         = ParseInt(node, "prod",      0);
            ForkColorHex = Attr(node, "forkcolor",  "808080");
            WingColorHex = Attr(node, "wingcolor",  "C0C0C0");
            RailColorHex = Attr(node, "railcolor",  "000000");
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;
            var railBrush = DciColor.ToBrush(RailColorHex, Colors.Black);
            var railPen   = new Pen(railBrush, 1);
            var forkBrush = DciColor.ToBrush(ForkColorHex, Colors.Gray);
            var wingBrush = DciColor.ToBrush(WingColorHex, Colors.Silver);

            // 배경 (BgMode 적용)
            DrawBackground(dc, rect);

            double l = rect.Left, r = rect.Right, t = rect.Top, b = rect.Bottom;
            double W = rect.Width, H = rect.Height;
            double cx = (l + r) / 2, cy = (t + b) / 2;

            bool horizontal = (RvType == 0 || RvType == 1); // L2R or R2L

            const int nRailEndS      = 4;
            const int nWingGapRatio  = 10;
            const int nWingScaleRatio= 30;

            // ─── Rail ───────────────────────────────────────────────
            Point railP1, railP2;
            Rect  rcRailEnd1, rcRailEnd2;
            if (horizontal)
            {
                railP1 = new Point(l, cy);
                railP2 = new Point(r, cy);
                rcRailEnd1 = new Rect(l,     cy - nRailEndS / 2.0, nRailEndS, nRailEndS);
                rcRailEnd2 = new Rect(r - nRailEndS, cy - nRailEndS / 2.0, nRailEndS, nRailEndS);
            }
            else
            {
                railP1 = new Point(cx, t);
                railP2 = new Point(cx, b);
                rcRailEnd1 = new Rect(cx - nRailEndS / 2.0, t, nRailEndS, nRailEndS);
                rcRailEnd2 = new Rect(cx - nRailEndS / 2.0, b - nRailEndS, nRailEndS, nRailEndS);
            }

            dc.DrawLine(railPen, railP1, railP2);
            DrawEllipseRect(dc, railBrush, rcRailEnd1);
            DrawEllipseRect(dc, railBrush, rcRailEnd2);

            // ─── Fork position ──────────────────────────────────────
            double forkSize1;
            Rect   rcFork1, rcFork2 = Rect.Empty;
            Rect   rcWing1, rcWing2;

            if (horizontal)
            {
                forkSize1 = H;
                double maxPos = forkSize1 > 0 ? W / forkSize1 - 1 : 0;
                double fp1 = ForkPos < 0 ? 0 : (ForkPos > maxPos ? maxPos : ForkPos);

                rcFork1 = new Rect(l + fp1 * forkSize1, t, forkSize1, H);

                // Wing1: left of fork1
                rcWing1 = new Rect(
                    rcFork1.Left  - rcFork1.Width  * nWingGapRatio  / 100.0 - rcFork1.Width  * nWingScaleRatio / 100.0,
                    rcFork1.Top   + rcFork1.Height / 2 - rcFork1.Height * nWingScaleRatio / 100.0,
                    rcFork1.Width  * nWingScaleRatio / 100.0,
                    rcFork1.Height * nWingScaleRatio / 100.0 * 2);

                if (ForkType == 1) // Twin: second fork immediately after first
                {
                    rcFork2 = new Rect(rcFork1.Right, t, forkSize1, H);
                    rcWing2 = new Rect(
                        rcFork2.Right + rcFork2.Width * nWingGapRatio / 100.0,
                        rcFork2.Top   + rcFork2.Height / 2 - rcFork2.Height * nWingScaleRatio / 100.0,
                        rcFork2.Width  * nWingScaleRatio / 100.0,
                        rcFork2.Height * nWingScaleRatio / 100.0 * 2);
                }
                else
                {
                    rcWing2 = new Rect(
                        rcFork1.Right + rcFork1.Width  * nWingGapRatio  / 100.0,
                        rcFork1.Top   + rcFork1.Height / 2 - rcFork1.Height * nWingScaleRatio / 100.0,
                        rcFork1.Width  * nWingScaleRatio / 100.0,
                        rcFork1.Height * nWingScaleRatio / 100.0 * 2);
                }
            }
            else // vertical
            {
                forkSize1 = W;
                double maxPos = forkSize1 > 0 ? H / forkSize1 - 1 : 0;
                double fp1 = ForkPos < 0 ? 0 : (ForkPos > maxPos ? maxPos : ForkPos);

                rcFork1 = new Rect(l, t + fp1 * forkSize1, W, forkSize1);

                rcWing1 = new Rect(
                    rcFork1.Left + rcFork1.Width / 2 - rcFork1.Width * nWingScaleRatio / 100.0,
                    rcFork1.Top  - rcFork1.Height * nWingGapRatio / 100.0 - rcFork1.Height * nWingScaleRatio / 100.0,
                    rcFork1.Width  * nWingScaleRatio / 100.0 * 2,
                    rcFork1.Height * nWingScaleRatio / 100.0);

                if (ForkType == 1) // Twin
                {
                    rcFork2 = new Rect(l, rcFork1.Bottom, W, forkSize1);
                    rcWing2 = new Rect(
                        rcFork2.Left + rcFork2.Width / 2 - rcFork2.Width * nWingScaleRatio / 100.0,
                        rcFork2.Bottom + rcFork2.Height * nWingGapRatio / 100.0,
                        rcFork2.Width  * nWingScaleRatio / 100.0 * 2,
                        rcFork2.Height * nWingScaleRatio / 100.0);
                }
                else
                {
                    rcWing2 = new Rect(
                        rcFork1.Left + rcFork1.Width / 2 - rcFork1.Width * nWingScaleRatio / 100.0,
                        rcFork1.Bottom + rcFork1.Height * nWingGapRatio / 100.0,
                        rcFork1.Width  * nWingScaleRatio / 100.0 * 2,
                        rcFork1.Height * nWingScaleRatio / 100.0);
                }
            }

            // ─── Draw Fork(s) ────────────────────────────────────────
            DrawButton(dc, rcFork1, forkBrush);
            if (!rcFork2.IsEmpty)
                DrawButton(dc, rcFork2, forkBrush);

            // ─── Draw Wings ──────────────────────────────────────────
            if (rcWing1.Width > 0 && rcWing1.Height > 0 && rect.Contains(rcWing1.TopLeft) && rect.Contains(rcWing1.BottomRight))
                DrawWing(dc, rcWing1, wingBrush);
            if (rcWing2.Width > 0 && rcWing2.Height > 0 && rect.Contains(rcWing2.TopLeft) && rect.Contains(rcWing2.BottomRight))
                DrawWing(dc, rcWing2, wingBrush);

            // ─── Prod sensor indicators ──────────────────────────────
            if (Prod == 1 || Prod == 3)
                DrawProdSensor(dc, rcFork1);
            if ((Prod == 2 || Prod == 3) && !rcFork2.IsEmpty)
                DrawProdSensor(dc, rcFork2);

            // ─── Text (Fork 중앙 표시) ───────────────────────────────
            if (!string.IsNullOrEmpty(Text))
            {
                var fg  = DciColor.ToBrush(FgColorHex, Colors.Black);
                var dpi = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;
                double fs = ResolveFont(rcFork1);
                var ft  = MakeText(Text, fs, fg, dpi);
                double ox = Pressed ? 1 : 0;
                double tx = rcFork1.Left + (rcFork1.Width  - ft.Width)  / 2 + ox;
                double ty = rcFork1.Top  + (rcFork1.Height - ft.Height) / 2 + ox;
                dc.DrawText(ft, new Point(tx, ty));
            }
        }

        private void DrawButton(DrawingContext dc, Rect rc, Brush fill)
        {
            if (rc.Width <= 0 || rc.Height <= 0) return;
            dc.DrawRectangle(fill, null, rc);
            // 3D highlight (Pressed 시 반전)
            var hi = Pressed ? Brushes.DarkGray : Brushes.White;
            var lo = Pressed ? Brushes.White    : Brushes.DarkGray;
            dc.DrawLine(new Pen(hi, 1), rc.TopLeft,    rc.TopRight);
            dc.DrawLine(new Pen(hi, 1), rc.TopLeft,    rc.BottomLeft);
            dc.DrawLine(new Pen(lo, 1), rc.BottomLeft, rc.BottomRight);
            dc.DrawLine(new Pen(lo, 1), rc.TopRight,   rc.BottomRight);
        }

        private static void DrawWing(DrawingContext dc, Rect rc, Brush fill)
        {
            if (rc.Width <= 0 || rc.Height <= 0) return;
            dc.DrawRectangle(fill, null, rc);
            dc.DrawLine(new Pen(Brushes.White, 1), rc.TopLeft,    rc.TopRight);
            dc.DrawLine(new Pen(Brushes.White, 1), rc.TopLeft,    rc.BottomLeft);
            dc.DrawLine(new Pen(Brushes.Black, 1), rc.BottomLeft, rc.BottomRight);
            dc.DrawLine(new Pen(Brushes.Black, 1), rc.TopRight,   rc.BottomRight);
        }

        private static void DrawProdSensor(DrawingContext dc, Rect rc)
        {
            const double g = 2, s = 4;
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Left + g,           rc.Top + g,            s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Right - g - s,      rc.Top + g,            s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Left + g,           rc.Bottom - g - s,     s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Right - g - s,      rc.Bottom - g - s,     s, s));
        }

        private static void DrawEllipseRect(DrawingContext dc, Brush fill, Rect rc)
        {
            if (rc.Width <= 0 || rc.Height <= 0) return;
            dc.DrawEllipse(fill, null, new Point(rc.X + rc.Width / 2, rc.Y + rc.Height / 2),
                           rc.Width / 2, rc.Height / 2);
        }
    }
}
