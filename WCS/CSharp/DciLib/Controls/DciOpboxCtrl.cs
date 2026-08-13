using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciOpboxCtrl에 대응. 오퍼레이션 박스(장비 상태 요약 박스) 컨트롤.
    /// 장비명, 상태, 작업 정보를 한 눈에 표시.
    /// </summary>
    public class DciOpboxCtrl : DciControl
    {
        public string DeviceName { get; set; } = string.Empty;
        public int    Status     { get; set; } // 0=Normal, 1=Fault, 2=PM, 3=Init
        public string StatusText { get; set; } = string.Empty;
        public int    LuggNum    { get; set; }

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            DeviceName = Attr(node, "device");
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;
            Brush bg = Status switch
            {
                1 => new SolidColorBrush(Color.FromRgb(255, 100, 100)),
                2 => new SolidColorBrush(Color.FromRgb(255, 200, 100)),
                3 => Brushes.LightYellow,
                _ => DciColor.ToBrush(BgColorHex)
            };

            dc.DrawRectangle(bg, new Pen(Brushes.DimGray, 1), rect);

            double dpi = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;
            var tf  = new Typeface("Malgun Gothic");

            // 장비명
            string label = string.IsNullOrEmpty(DeviceName) ? Text : DeviceName;
            if (!string.IsNullOrEmpty(label))
            {
                var ft = new FormattedText(label,
                    System.Globalization.CultureInfo.CurrentCulture,
                    FlowDirection.LeftToRight, tf,
                    FontSize > 0 ? FontSize : 10,
                    DciColor.ToBrush(FgColorHex), dpi);
                dc.DrawText(ft, new Point(rect.Left + 2, rect.Top + 1));
            }

            // 상태 텍스트
            if (!string.IsNullOrEmpty(StatusText))
            {
                var ft2 = new FormattedText(StatusText,
                    System.Globalization.CultureInfo.CurrentCulture,
                    FlowDirection.LeftToRight, tf, 9, Brushes.DarkBlue, dpi);
                dc.DrawText(ft2, new Point(rect.Left + 2, rect.Top + (FontSize > 0 ? FontSize : 10) + 3));
            }
        }
    }
}
