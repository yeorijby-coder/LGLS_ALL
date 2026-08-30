using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciControl에 대응하는 WPF 컨트롤 추상 기반 클래스.
    /// bgmode/fgmode: 1=TRANSPARENT, 2=OPAQUE (Win32 SetBkMode 값과 동일)
    /// </summary>
    public abstract class DciControl
    {
        // ─── 기본 속성 ───────────────────────────────────────────────
        public long   Id          { get; set; }
        public string Text        { get; set; } = string.Empty;
        public bool   Visible     { get; set; } = true;
        public bool   Enable      { get; set; } = true;

        public int LogLeft   { get; set; }
        public int LogBottom { get; set; }
        public int LogRight  { get; set; }
        public int LogTop    { get; set; }

        // bgmode: 1=TRANSPARENT(배경 안 그림), 2=OPAQUE(bgcolor 채움)
        public int    BgMode     { get; set; } = 2;
        // fgmode: 1=TRANSPARENT(텍스트 배경 투명), 2=OPAQUE(텍스트 배경 불투명)
        public int    FgMode     { get; set; } = 1;
        public string BgColorHex { get; set; } = "FFFFFF";
        public string FgColorHex { get; set; } = "000000";

        // fontsize: 0이면 자동(rect 크기 기반), >0이면 XML 지정값 사용
        public int    FontSize   { get; set; } = 0;
        public int    BorderStyle{ get; set; } = 0;

        public bool   Pressed    { get; set; }

        protected List<DciProperty> _properties = new();
        protected List<DciControl>  _items       = new();

        public Rect ScreenRect { get; set; }

        // ─── XML 로딩 ─────────────────────────────────────────────────

        public virtual void LoadFromXml(XmlNode node)
        {
            Id          = ParseLong(node, "id");
            Text        = Attr(node, "text");
            LogLeft     = ParseInt(node, "l");
            LogBottom   = ParseInt(node, "b");
            LogRight    = ParseInt(node, "r");
            LogTop      = ParseInt(node, "t");
            BgMode      = ParseInt(node, "bgmode",  2);
            FgMode      = ParseInt(node, "fgmode",  1);
            BgColorHex  = Attr(node, "bgcolor", "FFFFFF");
            FgColorHex  = Attr(node, "fgcolor", "000000");
            FontSize    = ParseInt(node, "fontsize", 0);
            BorderStyle = ParseInt(node, "border",   0);
            Visible     = Attr(node, "visible", "1") != "0";
            Enable      = Attr(node, "enable",  "1") != "0";
        }

        // ─── 렌더링 인터페이스 ────────────────────────────────────────

        public abstract void Draw(DrawingContext dc, DciMaster master);

        // 클릭 후 Pressed 해제를 요청하는 콜백 (EcsLayout에서 설정)
        public Action? PressedResetCallback { get; set; }

        public virtual void OnClick(Point pt)
        {
            if (!Enable || !Visible) return;
            Pressed = true;
            PressedResetCallback?.Invoke();
        }

        // ─── 공통 렌더링 헬퍼 ─────────────────────────────────────────

        /// <summary>
        /// BgMode에 따라 배경 그리기.
        /// BgMode==1(TRANSPARENT): 아무것도 안 그림
        /// BgMode==2(OPAQUE):      bgcolor로 채움
        /// </summary>
        protected void DrawBackground(DrawingContext dc, Rect rect)
        {
            if (BgMode == 2) // OPAQUE
                dc.DrawRectangle(DciColor.ToBrush(BgColorHex), null, rect);
        }

        /// <summary>
        /// 중앙 정렬 텍스트 그리기.
        /// fontsize=0이면 rect에 맞게 자동 결정.
        /// FgMode==2이면 텍스트 영역 배경도 BgColor로 채움.
        /// </summary>
        protected void DrawCenteredText(DrawingContext dc, Rect rect, Brush fg, double? overrideFs = null, string? overrideText = null)
        {
            string text = overrideText ?? Text;
            if (string.IsNullOrEmpty(text)) return;

            double fs = overrideFs ?? ResolveFont(rect);
            if (fs <= 0) return;

            var dpi = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;
            var ft  = MakeText(text, fs, fg, dpi);

            if (FgMode == 2) // OPAQUE text background
                dc.DrawRectangle(DciColor.ToBrush(BgColorHex), null,
                    new Rect(rect.Left, rect.Top + (rect.Height - ft.Height) / 2 - 1,
                             rect.Width, ft.Height + 2));

            double ox = Pressed ? 1 : 0;
            double tx = rect.Left + (rect.Width  - ft.Width)  / 2 + ox;
            double ty = rect.Top  + (rect.Height - ft.Height) / 2 + ox;
            dc.DrawText(ft, new Point(tx, ty));
        }

        /// <summary>
        /// FontSize > 0이면 그 값 사용, 0이면 rect에 맞게 자동 결정.
        /// 지정된 fontsize가 rect에 비해 너무 크면 스케일 다운.
        /// </summary>
        protected double ResolveFont(Rect rect)
        {
            if (rect.Width <= 0 || rect.Height <= 0) return 1;

            double fs = FontSize > 0
                ? (double)FontSize
                : Math.Min(rect.Width, rect.Height) * 0.55;

            // rect 높이를 초과하면 스케일 다운
            if (fs > rect.Height * 0.9)
                fs = rect.Height * 0.9;
            if (fs < 1) fs = 1;

            // 텍스트가 rect 너비를 초과하면 추가 스케일 다운
            if (!string.IsNullOrEmpty(Text) && fs > 1)
            {
                var dpi = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;
                var ft  = MakeText(Text, fs, Brushes.Black, dpi);
                if (ft.Width > rect.Width && ft.Width > 0)
                    fs = Math.Max(1, fs * rect.Width / ft.Width * 0.95);
            }

            return fs;
        }

        /// <summary>
        /// SKI CDC::Draw3dRect 대응. 1px 입체 테두리.
        /// pressed=false: 좌상단 흰색 + 우하단 검정(돌출), pressed=true: 반전(함몰).
        /// 순백/순흑을 써서 경계가 또렷하게 보인다.
        /// </summary>
        protected static void Draw3dRect(DrawingContext dc, Rect rc, bool pressed)
        {
            var hi = new Pen(pressed ? Brushes.Black : Brushes.White, 1);
            var lo = new Pen(pressed ? Brushes.White : Brushes.Black, 1);
            dc.DrawLine(hi, rc.TopLeft,    rc.TopRight);
            dc.DrawLine(hi, rc.TopLeft,    rc.BottomLeft);
            dc.DrawLine(lo, rc.BottomLeft, rc.BottomRight);
            dc.DrawLine(lo, rc.TopRight,   rc.BottomRight);
        }

        protected static FormattedText MakeText(string text, double emSize, Brush fg, double dpi)
            => new FormattedText(
                text,
                System.Globalization.CultureInfo.CurrentCulture,
                FlowDirection.LeftToRight,
                new Typeface("Malgun Gothic"),
                Math.Max(1, emSize),
                fg, dpi);

        // ─── 헬퍼 ─────────────────────────────────────────────────────
        protected static string Attr(XmlNode n, string key, string def = "")
            => (n as XmlElement)?.GetAttribute(key) ?? def;

        protected static int ParseInt(XmlNode n, string key, int def = 0)
            => int.TryParse(Attr(n, key, def.ToString()), out int v) ? v : def;

        protected static long ParseLong(XmlNode n, string key, long def = 0)
            => long.TryParse(Attr(n, key, def.ToString()), out long v) ? v : def;

        public override string ToString() => $"[{GetType().Name}] Id={Id} Text={Text}";
    }

    public class DciControlArray : List<DciControl>
    {
        public DciControl? FindById(long id)
        {
            foreach (var c in this)
                if (c.Id == id) return c;
            return null;
        }
    }

    public class DciControlList : LinkedList<DciControl>
    {
        public DciControl? FindById(long id)
        {
            foreach (var c in this)
                if (c.Id == id) return c;
            return null;
        }
    }
}
