using System;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    public class DciButtonCtrl : DciControl
    {
        public int  Shape   { get; private set; }
        public event EventHandler<long>? Clicked;

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            Shape = ParseInt(node, "shape", 0);
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;
            var bg   = DciColor.ToBrush(BgColorHex, Colors.LightGray);
            var fg   = DciColor.ToBrush(FgColorHex, Colors.Black);

            double l = rect.Left,   r = rect.Right;
            double t = rect.Top,    b = rect.Bottom;
            double cx = (l + r) / 2, cy = (t + b) / 2;
            double W  = rect.Width,  H  = rect.Height;

            switch (Shape)
            {
                case 0: // Rectangle
                default:
                    DrawBackground(dc, rect);
                    break;

                case 1: // Ellipse
                {
                    double gap  = Math.Min(W, H) / 10.0;
                    double rx   = (W / 2 - gap);
                    double ry   = (H / 2 - gap);
                    // 3D effect: white ring then dark ring then fill
                    dc.DrawEllipse(Pressed ? Brushes.DarkGray : Brushes.White, null, new Point(cx, cy), rx + 1, ry + 1);
                    dc.DrawEllipse(Pressed ? Brushes.White : Brushes.DarkGray, null, new Point(cx, cy), rx,     ry);
                    dc.DrawEllipse(bg, null, new Point(cx, cy), rx - 1, ry - 1);
                    break;
                }

                case 2: // RoundRect
                {
                    double gap  = Math.Min(W, H) / 10.0;
                    double rxy  = gap * 5;
                    var geom = new RectangleGeometry(new Rect(l + gap, t + gap, W - gap * 2, H - gap * 2), rxy, rxy);
                    dc.DrawGeometry(bg, new Pen(Brushes.DarkGray, 1), geom);
                    break;
                }

                // 3: RectToLeft — right-angle triangle: (right,top),(left,top),(left,bottom) → upper-left
                case 3:
                    DrawPolygon(dc, bg, new Point(r-2, t+2), new Point(l+2, t+2), new Point(l+2, b-2));
                    break;

                // 4: RectToRight — (right,top),(right,bottom),(left,top)
                case 4:
                    DrawPolygon(dc, bg, new Point(r-2, t+2), new Point(r-2, b-2), new Point(l+2, t+2));
                    break;

                // 5: RectBoLeft — (right,bottom),(left,top),(left,bottom)
                case 5:
                    DrawPolygon(dc, bg, new Point(r-2, b-2), new Point(l+2, t+2), new Point(l+2, b-2));
                    break;

                // 6: RectBoRight — (right,top),(left,bottom),(right,bottom)
                case 6:
                    DrawPolygon(dc, bg, new Point(r-2, t+2), new Point(l+2, b-2), new Point(r-2, b-2));
                    break;

                // 7: IsoscelesLeft — left-pointing: base at right side
                case 7:
                    DrawPolygon(dc, bg, new Point(l+2, t+2), new Point(l+2, b-2), new Point(r-2, cy));
                    break;

                // 8: IsoscelesRight — right-pointing: base at left side
                case 8:
                    DrawPolygon(dc, bg, new Point(r-2, t+2), new Point(r-2, b-2), new Point(l+2, cy));
                    break;

                // 9: IsoscelesTop — upward: base at bottom
                case 9:
                    DrawPolygon(dc, bg, new Point(l+2, b-2), new Point(r-2, b-2), new Point(cx, t+2));
                    break;

                // 10: IsoscelesBottom — downward: base at top
                case 10:
                    DrawPolygon(dc, bg, new Point(l+2, t+2), new Point(r-2, t+2), new Point(cx, b-2));
                    break;

                // 11: DoubleLeft — 7-pt polygon (two left-pointing triangles sharing center column)
                case 11:
                    DrawPolygon(dc, bg,
                        new Point(l+2, t+2),
                        new Point(l+2, b-2),
                        new Point(cx,  cy),
                        new Point(cx,  b-2),
                        new Point(r-2, cy),
                        new Point(cx,  t+2),
                        new Point(cx,  cy));
                    break;

                // 12: DoubleRight
                case 12:
                    DrawPolygon(dc, bg,
                        new Point(r-2, t+2),
                        new Point(r-2, b-2),
                        new Point(cx,  cy),
                        new Point(cx,  b-2),
                        new Point(l+2, cy),
                        new Point(cx,  t+2),
                        new Point(cx,  cy));
                    break;

                // 13: DoubleTop
                case 13:
                    DrawPolygon(dc, bg,
                        new Point(l+2, b-2),
                        new Point(r-2, b-2),
                        new Point(cx,  cy),
                        new Point(r-2, cy),
                        new Point(cx,  t+2),
                        new Point(l+2, cy),
                        new Point(cx,  cy));
                    break;

                // 14: DoubleBottom
                case 14:
                    DrawPolygon(dc, bg,
                        new Point(l+2, t+2),
                        new Point(r-2, t+2),
                        new Point(cx,  cy),
                        new Point(r-2, cy),
                        new Point(cx,  b-2),
                        new Point(l+2, cy),
                        new Point(cx,  cy));
                    break;

                // 15: TripleLeft — 11-pt
                case 15:
                {
                    double w3 = W / 3.0, x1 = l+2+w3, x2 = l+2+w3*2;
                    DrawPolygon(dc, bg,
                        new Point(l+2, t+2),
                        new Point(l+2, b-2),
                        new Point(x1,  cy),
                        new Point(x1,  b-2),
                        new Point(x2,  cy),
                        new Point(x2,  b-2),
                        new Point(r-2, cy),
                        new Point(x2,  t+2),
                        new Point(x2,  cy),
                        new Point(x1,  t+2),
                        new Point(x1,  cy));
                    break;
                }

                // 16: TripleRight
                case 16:
                {
                    double w3 = W / 3.0, x1 = l+2+w3, x2 = l+2+w3*2;
                    DrawPolygon(dc, bg,
                        new Point(r-2, t+2),
                        new Point(r-2, b-2),
                        new Point(x1,  cy),
                        new Point(x1,  b-2),
                        new Point(x2,  cy),
                        new Point(x2,  b-2),
                        new Point(l+2, cy),
                        new Point(x2,  t+2),
                        new Point(x2,  cy),
                        new Point(x1,  t+2),
                        new Point(x1,  cy));
                    break;
                }

                // 17: TripleTop
                case 17:
                {
                    double h3 = H / 3.0, y1 = t+h3, y2 = t+h3*2;
                    DrawPolygon(dc, bg,
                        new Point(l+2, b-2),
                        new Point(r-2, b-2),
                        new Point(cx,  y1),
                        new Point(r-2, y1),
                        new Point(cx,  y2),
                        new Point(r-2, y2),
                        new Point(cx,  t+2),
                        new Point(l+2, y2),
                        new Point(cx,  y2),
                        new Point(l+2, y1),
                        new Point(cx,  y1));
                    break;
                }

                // 18: TripleBottom
                case 18:
                {
                    double h3 = H / 3.0, y1 = t+h3, y2 = t+h3*2;
                    DrawPolygon(dc, bg,
                        new Point(l+2, t+2),
                        new Point(r-2, t+2),
                        new Point(cx,  y1),
                        new Point(r-2, y1),
                        new Point(cx,  y2),
                        new Point(r-2, y2),
                        new Point(cx,  b-2),
                        new Point(l+2, y2),
                        new Point(cx,  y2),
                        new Point(l+2, y1),
                        new Point(cx,  y1));
                    break;
                }
            }

            DrawCenteredText(dc, rect, DciColor.ToBrush(FgColorHex, Colors.Black));
        }

        private static void DrawPolygon(DrawingContext dc, Brush fill, params Point[] pts)
        {
            var geo = new StreamGeometry();
            using (var ctx = geo.Open())
            {
                ctx.BeginFigure(pts[0], isFilled: true, isClosed: true);
                for (int i = 1; i < pts.Length; i++)
                    ctx.LineTo(pts[i], isStroked: false, isSmoothJoin: false);
            }
            geo.Freeze();
            dc.DrawGeometry(fill, null, geo);
        }

        public override void OnClick(Point pt)
        {
            if (!Enable || !Visible) return;
            Pressed = !Pressed;
            Clicked?.Invoke(this, Id);
        }
    }
}
