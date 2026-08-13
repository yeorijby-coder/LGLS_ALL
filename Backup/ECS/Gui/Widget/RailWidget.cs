using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;

namespace HECS.Gui.Widget
{
    [Serializable]
    public partial class RailWidget : HECSBaseWidget
    {
        private int railLineHeight = 0;

        private bool isHpDoorOpen = false;
        private bool isOpDoorOpen = false;

        Bitmap railImage = null;
        Rectangle innerRect;


        public RailWidget()
        {
            InitializeComponent();
            this.BackColor = Color.Transparent ;
            this.ForeColor = Color.Wheat;
            this.UseMouseHover = false;
            innerRect = this.Bounds;
            this.SetStyle(ControlStyles.ContainerControl, false);
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);

            railLineHeight = (int)(this.Height * 0.2);
            if (railLineHeight < 1) railLineHeight = 1;
            if (railLineHeight > 10) railLineHeight = 10;

            innerRect = GetRectangleForPanWidth(this.borderWidth);

            CreateRailImage();
            CreateDoor();
            this.BackgroundImage =(Image) railImage;
            this.BackgroundImageLayout = ImageLayout.Stretch;

        }
        private void CreateRailImage()
        {
            railImage = new Bitmap(innerRect.Width, innerRect.Height);
            Graphics graphic = Graphics.FromImage(railImage);
            graphic.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

            int panWidth = 2;
            Rectangle baseRect = GetRectangleForPanWidth(innerRect,panWidth);
            Rectangle railTopLine = new Rectangle(baseRect.X, baseRect.Y, baseRect.Width, railLineHeight);
            Rectangle railTopBgLine = new Rectangle(baseRect.X, railTopLine.Y + 1, baseRect.Width, railLineHeight);
            Rectangle railBottomLine = new Rectangle(baseRect.X, baseRect.Height - railLineHeight, baseRect.Width, railLineHeight);
            Rectangle railBottomBgLine = new Rectangle(baseRect.X, railBottomLine.Y - 1, baseRect.Width, railLineHeight);

            DrawRoundFillRectangle(graphic, railTopBgLine, 1, Color.Black, Color.Black, panWidth);
            DrawRoundFillRectangle(graphic, railTopLine, 1, ForeColor, Color.LightGray, panWidth);
            DrawRoundFillRectangle(graphic, railBottomBgLine, 1, Color.Black, Color.Black, panWidth);
            DrawRoundFillRectangle(graphic, railBottomLine, 1, ForeColor, Color.LightGray, panWidth);

            int height1, height2, height3;

            height1 = innerRect.Height - 26;
            height2 = innerRect.Height - 23;
            height3 = innerRect.Height - 20;

            for (int i = 25; i < innerRect.Width; i = i + 50)
            {
                DrawRoundFillRectangle(graphic, new Rectangle(i + 1, 11 + 1, 5, height1), 1, Color.Black, Color.Black, 2);
                DrawRoundFillRectangle(graphic, new Rectangle(i, 11, 5, height2), 1, ForeColor, Color.LightGray, 2);
                DrawEclipse(graphic, new Rectangle(i + 2, 13, 3, 5), Color.Black, Color.DarkRed);
                DrawEclipse(graphic, new Rectangle(i + 2, height3, 3, 5), Color.Black, Color.DarkRed);
            }

        }

        private void CreateDoor()
        {
            if (isHpDoorOpen == false && isOpDoorOpen == false) return;
            Graphics graphic = Graphics.FromImage(railImage);

            int panWidth = 2;
            Rectangle baseRect = GetRectangleForPanWidth(innerRect, panWidth);
            Font font = new Font("Arial@", 7, FontStyle.Bold);

            int doorWidth = (int)(baseRect.Width * 0.2);
            
            if (doorWidth < 1) doorWidth = 1;
            if (doorWidth > 40) doorWidth = 40;

            if (isHpDoorOpen)
            {
                Rectangle hpDoor = new Rectangle(baseRect.X, baseRect.Y, doorWidth, baseRect.Height);
                graphic.FillRectangle(new SolidBrush(Color.FromArgb(220, Color.Red)), hpDoor);    
                graphic.DrawRectangle(new Pen(Color.White,panWidth), hpDoor);

                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;
                //stringFormat.FormatFlags = StringFormatFlags.DirectionVertical | StringFormatFlags.DirectionRightToLeft;

                graphic.DrawString("DOOR OPEN", font, Brushes.White, hpDoor, stringFormat);
                
            }

            if (isOpDoorOpen)
            {
                Rectangle opDoor = new Rectangle(baseRect.Width - doorWidth, baseRect.Y, doorWidth, baseRect.Height);
                graphic.FillRectangle(new SolidBrush(Color.FromArgb(220,Color.Red)), opDoor);
                graphic.DrawRectangle(new Pen(Color.White, panWidth), opDoor);

                StringFormat stringFormat = new StringFormat();
                stringFormat.Alignment = StringAlignment.Center;
                stringFormat.LineAlignment = StringAlignment.Center;
                //stringFormat.FormatFlags = StringFormatFlags.DirectionVertical | StringFormatFlags.DirectionRightToLeft;

                graphic.DrawString("DOOR OPEN", font, Brushes.White, opDoor, stringFormat);
            }
            
            font.Dispose();

        }
        /// <summary>
        /// 컨트롤의 크기를 기준으로 선 두께를 고려하여 사각형을 반환합니다.
        /// </summary>
        /// <param name="panWidth">선 두께</param>
        /// <returns></returns>
        private Rectangle GetRectangleForPanWidth(int panWidth)
        {
            return GetRectangleForPanWidth(new Rectangle(panWidth / 2, panWidth/2, this.Width - panWidth, this.Height - panWidth), 0);
        }
        /// <summary>
        /// 컨트롤의 크기를 기준으로 내/외부의 선 두께를 고려하여 사각형을 반환합니다.
        /// </summary>
        /// <param name="innerPanWidth">내부의 선 두께</param>
        /// <param name="outterPanWidth">외부의 선 두께</param>
        /// <returns></returns>
        private Rectangle GetRectangleForPanWidth(int innerPanWidth, int outterPanWidth)
        {
            return GetRectangleForPanWidth(new Rectangle(outterPanWidth/2, outterPanWidth/2, this.Width - outterPanWidth, this.Height - outterPanWidth), innerPanWidth);
        }
        /// <summary>
        /// 기준 사각형을 기준으로 선 두께를 고려하여 사각형을 반환합니다.
        /// </summary>
        /// <param name="rect">기준 사각형</param>
        /// <param name="panWidth">선 두께</param>
        /// <returns></returns>
        private Rectangle GetRectangleForPanWidth(Rectangle rect, int panWidth)
        {
            int x = rect.X + (panWidth/2);
            int y = rect.Y + (panWidth/2);
            int width = rect.Width - panWidth;
            int heigth = rect.Height -  panWidth;

            Rectangle rect2 = new Rectangle(x, y, width, heigth);
            return rect2;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            if (this.selected == true)
            {
                int panWidth = 3;

                Pen p = new Pen(Color.Red, panWidth);
                Rectangle selectedRect = new Rectangle(panWidth / 2, panWidth / 2, this.Width - panWidth, this.Height - panWidth);
                e.Graphics.DrawRectangle(p, selectedRect);
            }
        }
        //RailWidget에 마우스 포인터 올라가거나, 사라지면 깜빡거리는 현상 생깁니다. 
        //깜빡 거림을 방지하기 위해, 오버라이딩 하고, 아무 작업 안 하는 겁니다. 
        protected override void OnMouseEnter(EventArgs e)
        {
            //base.OnMouseEnter(e);
        }

        //RailWidget 마우스 포인터 올라가거나, 사라지면, 깜빡거리는 현상 생깁니다. 
        //깜빡 거림을 방지하기 위해, 오버라이딩 하고, 아무 작업 안 하는 겁니다. 
        protected override void OnMouseLeave(EventArgs e)
        {
            //base.OnMouseLeave(e);
        }
    }
}
