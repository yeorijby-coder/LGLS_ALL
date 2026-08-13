using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace HECS.Gui.Widget
{
    public partial class HiRackCellWidget : HECSBaseWidget
    {
        private Image materialImage = null;

        public Image MaterialImage 
        {
            get
            {
                return materialImage;
            }
            set
            {
                materialImage = value;
            }
        }


        private int row = -1;

        public int Row
        {
            get
            {
                return row;
            }
            set
            {
                row = value;
            }
        }
        private int column = -1;

        public int Column
        {
            get
            {
                return column;
            }
            set
            {
                column = value;
            }
        }
        private string stockerID = "";

        public string StockerID
        {
            get
            {
                return stockerID;
            }
            set
            {
                stockerID = value;
            }
        }

        public HiRackCellWidget()
        {
            InitializeComponent();
            this.hoveredColor = Color.Lime;
            this.SetStyle(ControlStyles.ContainerControl, false);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
            e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            int carrierGap = 4;
            Rectangle carrierRect = new Rectangle(carrierGap, carrierGap, this.Width - (carrierGap * 2), this.Height - (carrierGap * 3));
            DrawCarrier(e.Graphics, carrierRect);
            int prohibitGap = 3;
            Rectangle prohibitRect = new Rectangle(prohibitGap, prohibitGap, this.Width - (prohibitGap * 2) - 1, this.Height - (prohibitGap * 2) - 1);
            DrawProhibit(e.Graphics, prohibitRect);
        }

        private void DrawCarrier(Graphics graphics, Rectangle carrierRect)
        {
            if (this.materialImage == null) 
            {
                return;
            }

           // graphics.DrawImage(materialImage,  1, 1, this.Width - 3, this.Height -3);
            //
            // 캐리어가 있을 때, 

            // 캐리어가 없을때, 

            // 금지단 일때, 
            // 알수없을 떄.. .등등.등..등..등...등..등..
        }

        private void DrawProhibit(Graphics graphics, Rectangle phbRect)
        {

            // HiRack 의 Cell 이 금지단으로 설정되어 있으면
            // 금지영역으로 표시한다. 

            //if (conveyorObject != null && conveyorObject.IsProhibited == true)
            //{
            //    Color backColor = Color.MistyRose;
            //    Color foreColor = Color.Red;
            //    DrawRoundFillRectangle(graphics, phbRect, 1, backColor, foreColor, 2);
            //    Pen pen = new Pen(Color.Red, 2);
            //    graphics.DrawLine(pen, phbRect.X, phbRect.Y, phbRect.Width, phbRect.Height);
            //    graphics.DrawLine(pen, phbRect.Width, phbRect.Y, phbRect.X, phbRect.Height);
            //    pen.Dispose();
            //}
        }
    }
}
