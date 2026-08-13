using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Media;
using System.Xml;

namespace DciLib.Controls
{
    /// <summary>
    /// C++의 CDciRackCtrl에 대응. 랙 스토리지 시각화 컨트롤.
    /// start/type/len XML 속성으로 설정되며, 논리 좌표 기반으로 row×col 계산.
    /// type: 0=L2R, 1=R2L, 2=T2B, 3=B2T
    /// len: 논리 좌표 단위 셀 크기
    /// </summary>
    public class DciRackCtrl : DciControl
    {
        public int RackType  { get; private set; } // 0=L2R, 1=R2L, 2=T2B, 3=B2T
        public int UnitLen   { get; private set; } = 1;
        public int StartPos  { get; private set; } = 0;

        // 셀별 색상/상태 (런타임 업데이트용)
        private readonly Dictionary<int, Brush> _cellBrush = new();

        public override void LoadFromXml(XmlNode node)
        {
            base.LoadFromXml(node);
            StartPos = ParseInt(node, "start", 0);
            RackType = ParseInt(node, "type",  0);
            UnitLen  = Math.Max(1, ParseInt(node, "len", 1));
        }

        /// <summary>셀 bay번호(1-based) → 색상 설정. bay는 SKI의 nBay 계산과 동일.</summary>
        public void SetCellState(int row, int col, int state)
        {
            int nRow = Math.Max(1, Math.Abs(LogTop - LogBottom) / UnitLen);
            int nCol = Math.Max(1, Math.Abs(LogRight - LogLeft) / UnitLen);

            int bay;
            if (RackType == 0 || RackType == 1) // L2R / R2L
                bay = StartPos + row * nCol + col + 1;
            else
                bay = StartPos + col * nRow + row + 1;

            Brush b = state switch
            {
                1 => new SolidColorBrush(Color.FromRgb(0x00, 0xC0, 0xC0)),
                2 => Brushes.OrangeRed,
                3 => Brushes.Yellow,
                _ => DciColor.ToBrush(BgColorHex, Colors.DarkOliveGreen)
            };
            _cellBrush[bay] = b;
        }

        public override void Draw(DrawingContext dc, DciMaster master)
        {
            if (!Visible) return;

            var rect = ScreenRect;
            if (rect.Width <= 0 || rect.Height <= 0) return;

            // 논리 좌표에서 row/col 계산 (SKI C++ 동일 공식)
            int logH  = Math.Abs(LogTop - LogBottom);
            int logW  = Math.Abs(LogRight - LogLeft);
            int nRow  = Math.Max(1, logH / UnitLen);
            int nCol  = Math.Max(1, logW / UnitLen);

            double cellW = rect.Width  / nCol;
            double cellH = rect.Height / nRow;

            // 배경 (BgMode 적용)
            DrawBackground(dc, rect);

            var defaultBrush = DciColor.ToBrush(BgColorHex, Colors.DarkOliveGreen);
            var fg           = DciColor.ToBrush(FgColorHex, Colors.White);
            var dpi          = VisualTreeHelper.GetDpi(Application.Current.MainWindow).PixelsPerDip;

            for (int i = 0; i < nRow; i++)
            {
                for (int j = 0; j < nCol; j++)
                {
                    // SKI bay 번호 계산
                    int bay;
                    if (RackType == 0) // L2R
                        bay = StartPos + i * nCol + j + 1;
                    else if (RackType == 1) // R2L
                        bay = StartPos + i * nCol + (nCol - j);
                    else if (RackType == 2) // T2B
                        bay = StartPos + j * nRow + i + 1;
                    else // B2T
                        bay = StartPos + j * nRow + (nRow - i);

                    var cellRect = new Rect(
                        rect.Left + j * cellW,
                        rect.Top  + i * cellH,
                        cellW, cellH);

                    var fill = _cellBrush.TryGetValue(bay, out var cb) ? cb : defaultBrush;

                    // SKI DrawButton: FillSolidRect + Draw3dRect (순백/순흑으로 또렷한 경계)
                    dc.DrawRectangle(fill, null, cellRect);
                    Draw3dRect(dc, cellRect, Pressed);

                    // bay 번호 텍스트 (cellRect에 맞게 자동 크기)
                    double fs = ResolveFont(cellRect);
                    var ft = MakeText(bay.ToString(), fs, fg, dpi);
                    double tx = cellRect.Left + (cellRect.Width  - ft.Width)  / 2;
                    double ty = cellRect.Top  + (cellRect.Height - ft.Height) / 2;
                    dc.DrawText(ft, new Point(tx, ty));
                }
            }
        }
    }
}
