using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciLifterCtrl에 대응. 리프터 장비 시각화 컨트롤.
    /// 현재 높이 위치를 막대 형태로 표시.
    /// </summary>
    public class DciLifterCtrl : DciControl
    {
        public int  LifterNumber { get; set; }
        public int  Status       { get; set; }
        public int  CurrentFloor { get; set; }
        public int  TotalFloors  { get; set; } = 3;
        public bool HasTray      { get; set; }

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            LifterNumber = ParseInt(node, "lifternum");
            TotalFloors  = ParseInt(node, "floors", 3);
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;
            dc.DrawRectangle(DciColor.ToBrush(BgColorHex), new Pen(Brushes.DarkGray, 1), rect);

            // 층 구분선
            var linePen = new Pen(Brushes.Gray, 0.5);
            double floorH = rect.Height / TotalFloors;
            for (int i = 1; i < TotalFloors; i++)
                dc.DrawLine(linePen,
                    new Point(rect.Left,  rect.Top + i * floorH),
                    new Point(rect.Right, rect.Top + i * floorH));

            // 현재 위치 표시
            if (CurrentFloor >= 0 && CurrentFloor < TotalFloors)
            {
                double posY = rect.Bottom - (CurrentFloor + 1) * floorH;
                Brush lifterBrush = Status == 2 ? Brushes.OrangeRed : Brushes.LimeGreen;
                dc.DrawRectangle(lifterBrush, null,
                    new Rect(rect.Left + 2, posY + 2, rect.Width - 4, floorH - 4));
            }
        }
    }
}
