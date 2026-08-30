using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciTrackCtrl에 대응. 트랙 내부 아이템(화물 신호) 그리드를 표시.
    /// </summary>
    public class DciTrackCtrl : DciControl
    {
        // ─── 내부 TrackItem ──────────────────────────────────────────
        public class TrackItem
        {
            // 0=None, 1=Rectangle, 2=ToLeft, 3=ToRight, 4=ToUp, 5=ToDown
            public int    Shape    { get; set; }
            public string ColorHex { get; set; } = "808080";
            public int    GapLeft  { get; set; }
            public int    GapTop   { get; set; }
            public int    GapRight { get; set; }
            public int    GapBottom{ get; set; }
        }

        // ─── 속성 ────────────────────────────────────────────────────
        public int              Row         { get; private set; }
        public int              Col         { get; private set; }
        public bool             BExist      { get; set; }
        public bool             BDoubleExist{ get; set; }
        public List<TrackItem>  Items       { get; } = new List<TrackItem>();

        // 런타임 상태 (EcsLayout.cs에서 ID로 찾아서 업데이트)
        public int  TrackNumber { get; set; }
        public int  Status      { get; set; }
        public bool HasTray     { get => BExist;  set => BExist = value; }
        public int  LuggNum     { get; set; }

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            Row          = ParseInt(node, "row",   0);
            Col          = ParseInt(node, "col",   0);
            BExist       = ParseInt(node, "prod",  0) != 0;
            BDoubleExist = ParseInt(node, "prod2", 0) != 0;
            TrackNumber  = ParseInt(node, "tracknum", 0);

            Items.Clear();
            int count = Row * Col;
            if (count <= 0) return;

            foreach (XmlNode child in node.ChildNodes)
            {
                if (child.Name != "Item") continue;
                var item = new TrackItem
                {
                    Shape     = ParseInt(child, "shape", 0),
                    ColorHex  = Attr(child, "color", "808080"),
                    GapLeft   = ParseInt(child, "gl", 0),
                    GapTop    = ParseInt(child, "gt", 0),
                    GapRight  = ParseInt(child, "gr", 0),
                    GapBottom = ParseInt(child, "gb", 0),
                };
                Items.Add(item);
                if (Items.Count >= count) break;
            }
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;

            // 배경 (BgMode 적용)
            DrawBackground(dc, rect);

            // ─── 버튼처럼 보이는 3D 입체 테두리 (SKI DrawButton, Pressed 시 함몰) ──
            Draw3dRect(dc, rect, Pressed);

            const double g = 2, s = 4;
            double l = rect.Left, r = rect.Right, t = rect.Top, b = rect.Bottom;
            double W = rect.Width, H = rect.Height;

            // ─── bExist: 4 corner dots (drawn before items) ──────────
            if (BExist)
                DrawCornerDots(dc, rect, g, s);

            // ─── bDoubleExist: center square ─────────────────────────
            if (BDoubleExist)
            {
                double sel = W < H ? W : H;
                double half = sel * 0.25;
                double nx = l + W / 2 - half;
                double ny = t + H / 2 - half;
                dc.DrawRectangle(Brushes.Black, null, new Rect(nx, ny, half * 2, half * 2));
            }

            // ─── Item grid ───────────────────────────────────────────
            if (Row > 0 && Col > 0 && Items.Count > 0)
            {
                double unitX = W / Col;
                double unitY = H / Row;

                for (int i = 0; i < Row; i++)
                {
                    for (int j = 0; j < Col; j++)
                    {
                        int idx = i * Col + j;
                        if (idx >= Items.Count) break;

                        var item = Items[idx];
                        if (item.Shape == 0) continue; // None

                        double ul = l + j * unitX;
                        double ut = t + i * unitY;
                        double ur = ul + unitX;
                        double ub = ut + unitY;

                        // Apply gap as percent of unit size
                        ul += unitX * item.GapLeft   / 100.0;
                        ut += unitY * item.GapTop    / 100.0;
                        ur -= unitX * item.GapRight  / 100.0;
                        ub -= unitY * item.GapBottom / 100.0;
                        ur -= 1; ub -= 1; // SKI: DeflateRect(0,0,1,1)

                        if (ur <= ul || ub <= ut) continue;

                        var itemBrush = DciColor.ToBrush(item.ColorHex, Colors.Gray);
                        DrawItemShape(dc, item.Shape, itemBrush, ul, ut, ur, ub);
                    }
                }
            }

            // ─── bExist drawn again on top (SKI draws exist dots twice) ─
            if (BExist)
                DrawCornerDots(dc, rect, g, s);

            // ─── Text (창고 모니터링 모드: 0=작업번호, 1=트랙번호, 2=제품정보) ─
            string disp = master.TrackTextMode switch
            {
                0 => LuggNum > 0 ? LuggNum.ToString() : "",           // 작업번호(적재 화물)
                2 => string.IsNullOrEmpty(ProductInfo) ? Text : ProductInfo, // 제품정보
                _ => Text,                                            // 트랙번호(XML 라벨)
            };
            DrawCenteredText(dc, rect, DciColor.ToBrush(FgColorHex, Colors.Black), null, disp);
        }

        // 제품정보 표시용(런타임 업데이트). 없으면 트랙 라벨 사용.
        public string ProductInfo { get; set; } = string.Empty;

        private static void DrawCornerDots(DrawingContext dc, Rect rc, double g, double s)
        {
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Left + g,         rc.Top + g,          s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Right - g - s,    rc.Top + g,          s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Left + g,         rc.Bottom - g - s,   s, s));
            dc.DrawRectangle(Brushes.Black, null, new Rect(rc.Right - g - s,    rc.Bottom - g - s,   s, s));
        }

        private static void DrawItemShape(DrawingContext dc, int shape, Brush brush,
                                          double l, double t, double r, double b)
        {
            double cy = (t + b) / 2.0;
            double cx = (l + r) / 2.0;

            switch (shape)
            {
                case 1: // Rectangle
                    dc.DrawRectangle(brush, null, new Rect(new Point(l, t), new Point(r, b)));
                    break;

                case 2: // ToLeft — left-pointing triangle
                    DrawTriangle(dc, brush,
                        new Point(r, t), new Point(r, b), new Point(l, cy));
                    break;

                case 3: // ToRight — right-pointing triangle
                    DrawTriangle(dc, brush,
                        new Point(l, t), new Point(l, b), new Point(r, cy));
                    break;

                case 4: // ToUp — upward triangle
                    DrawTriangle(dc, brush,
                        new Point(l, b), new Point(r, b), new Point(cx, t));
                    break;

                case 5: // ToDown — downward triangle
                    DrawTriangle(dc, brush,
                        new Point(l, t), new Point(r, t), new Point(cx, b));
                    break;
            }
        }

        private static void DrawTriangle(DrawingContext dc, Brush brush, Point p0, Point p1, Point p2)
        {
            var geo = new StreamGeometry();
            using (var ctx = geo.Open())
            {
                ctx.BeginFigure(p0, isFilled: true, isClosed: true);
                ctx.LineTo(p1, isStroked: false, isSmoothJoin: false);
                ctx.LineTo(p2, isStroked: false, isSmoothJoin: false);
            }
            geo.Freeze();
            dc.DrawGeometry(brush, null, geo);
        }
    }
}
