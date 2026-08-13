using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;
using ECP.Global;
using System.Drawing.Drawing2D;

namespace HECS.Gui.Widget
{
    public partial class HECSBaseWidget : BaseWidget 
    {

        #region Widget Custom Property

        public const string PROPERTY_NAME_BORDER_WIDTH = "BorderWidth";

        public static PropertyInfo PROPERTY_BORDER_WIDTH = new PropertyInfo(PROPERTY_NAME_BORDER_WIDTH, PropertyInfo.PROPERTY_TYPE_NUMBER, "1");
        #endregion

        #region StockerUnitWidget 공통 속성
        protected System.Timers.Timer timerBlinker = new System.Timers.Timer();
        protected ToolTip tooltip = new ToolTip();
        protected int tick = 0;
        bool isHovered = false;
        #endregion

        #region Widget 공통 속성
        /// <summary>
        /// MouseHover Event 사용여부를 지정.
        /// </summary>
        private bool useMouseHover = true;
        public bool UseMouseHover
        {
            get
            {
                return useMouseHover;
            }
            set
            {
                useMouseHover = value;
            }
        }


        /// <summary>
        /// Subsystem의 예약 여부를 표시. 
        /// 테두리가 녹색으로 매초당 1회씩 깜빡거림. 
        /// </summary>
        protected bool isReserved = false;
        public bool IsReserved
        {
            get
            {
                return isReserved;
            }
            set
            {
                isReserved = value;
            }
        }
        protected bool imHere = false;

        public bool ImHere
        {
            get
            {
                return imHere;
            }
            set
            {
                imHere = value;
            }
        }

        private string ownerId = "";

        public string OwnerId
        {
            get
            {
                return ownerId;
            }
            set
            {
                ownerId = value;
            }
        }
        /// <summary>
        /// RM간 공유하는 Subsystem을 표시. 
        /// 공유된 Subsystem은 빨간색 테두리로 표시. 
        /// </summary>
        protected bool isShared = false;
        public bool IsShared
        {
            get
            {
                return isShared;
            }
            set
            {
                isShared = value;
            }
        }
        /// <summary>
        /// 사용가능여부를 표시.
        /// Subsystem이 Unavailible할 경우, 테두리가 빨간색으로 매초당 1회씩 깜빡거림. 
        /// </summary>
        protected bool isAvailible = true;
        public bool IsAvailible
        {
            get
            {
                return isAvailible;
            }
            set
            {
                isAvailible = value;
            }
        }
        /// <summary>
        /// Widget의 네 모서리의 곡선율. 
        /// 값의 범위는 1 -> 90까지. 
        /// </summary>
        private int radius = 1;
        public int Radius
        {
            get
            {
                return radius;
            }
            set
            {
                if (value < 1)
                {
                    return;
                }
                radius = value;
            }
        }

        /// <summary>
        /// Safety Sensor Detected 
        /// </summary>
        private bool isSafetySensorDetected = false;

        public bool IsSafetySensorDetected
        {
            get
            {
                return isSafetySensorDetected;
            }
            set
            {
                isSafetySensorDetected = value;
            }
        }

        /// <summary>
        /// Widget의 Border 두께.
        /// Widget의 BorderColor = BackColor로 표시된다. 
        /// </summary>
        protected int borderWidth = 1;
        public int BorderWidth
        {
            get
            {
                return borderWidth;
            }
            set
            {
                borderWidth = value;
                this.SetProperty(PROPERTY_NAME_BORDER_WIDTH, Convert.ToString(borderWidth));

            }
        }

        private int objectOrder = 0;

        public int ObjectOrder
        {
            get
            {
                return objectOrder;
            }
            set
            {
                objectOrder = value;
            }
        }

        protected Color hoveredColor = Color.White;
        #endregion





        public HECSBaseWidget()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.SetStyle(ControlStyles.ContainerControl, true);

            this.MouseEnter += new EventHandler(HECSBaseWidget_MouseEnter);
            this.MouseLeave += new EventHandler(HECSBaseWidget_MouseLeave);
            timerBlinker.Interval = 1000;
            timerBlinker.Enabled = false;
            timerBlinker.Elapsed += new System.Timers.ElapsedEventHandler(timerBlinker_Elapsed);


        }
        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            base.OnPaint(e);
            DrawWidgetDefaultState(e.Graphics);
        }
        /// <summary>
        /// Widget의 기본 상태를 표시한다. 
        /// </summary>
        /// <param name="g"></param>
        protected void DrawWidgetDefaultState(Graphics g)
        {
            Graphics graphic = g;
            graphic.SmoothingMode = SmoothingMode.HighQuality;
            graphic.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            //Blinking을 할것인지 결정한다. 
            //Availible == false, Reserved == true 이면.. Blinking..

            if (this.isAvailible == false || this.isReserved == true || this.isSafetySensorDetected == true || this.imHere == true)
            {
                if (timerBlinker.Enabled == false)
                {
                    timerBlinker.Enabled = true;
                }
            }
            else
            {
                if (timerBlinker.Enabled == true)
                {
                    timerBlinker.Enabled = false;
                    this.tick = 0;
                }
            }
            if (isSafetySensorDetected == true)
            {
                Rectangle rectLeft = new Rectangle(0, 0, 3, this.Height - 1);
                Rectangle rectRight = new Rectangle(this.Width - 4, 0, 3, this.Height - 1);
                if (this.tick == 0)
                {
                    DrawRoundFillRectangle(graphic, rectLeft, 1, Color.Red, Color.Red, 1);
                    DrawRoundFillRectangle(graphic, rectRight, 1, Color.Red, Color.Red, 1);
                }
                else
                {
                    DrawRoundFillRectangle(graphic, rectLeft, 1, Color.White, Color.Black, 1);
                    DrawRoundFillRectangle(graphic, rectRight, 1, Color.White, Color.Black, 1);
                }


            }
            else
            {
                //Border 처리 
                Rectangle rectSize = new Rectangle(0, 0, this.Width - 1, this.Height - 1);
                DrawRoundFillRectangle(graphic, rectSize, this.radius, this.WidgetForeColor, this.WidgetBackColor, this.borderWidth);

                //상태 처리 
                int stateBorderGap = 2;
                int stateBorderWidth = 2;
                Rectangle rectState = new Rectangle(stateBorderGap, stateBorderGap, this.Width - (stateBorderGap * 2) - 1, this.Height - (stateBorderGap * 2) - 1);
                Color stateBorderColor = this.ForeColor;

                if (this.IsAvailible == true)
                {
                    if (this.isShared == true)
                    {
                        stateBorderWidth = 1;
                        stateBorderColor = Color.Crimson;
                    }
                    if (this.isReserved == true && this.tick == 1)
                    {
                        stateBorderWidth = 2;
                        stateBorderColor = Color.Blue;
                    }
                }
                else
                {
                    if (this.tick == 1)
                    {
                        if (this.isReserved == true)
                        {                       //다운 상태에서, 예약이 걸려 있으면....검은색...
                            stateBorderColor = Color.Black;
                            stateBorderWidth = 3;
                        }
                        else
                        {
                            stateBorderColor = Color.Red;
                        }
                    }
                    else
                    {
                        stateBorderColor = this.ForeColor;
                    }
                }
                DrawRoundRectangle(graphic, rectState, this.radius, this.ForeColor, stateBorderColor, stateBorderWidth);
            }
            if (this.isHovered == true)
            {
                Color glowColor = hoveredColor;
                float glowOpacity = 0.8f;
                Rectangle rect = new Rectangle(0, 0, this.Width, this.Height);
                using (GraphicsPath clip = CreateRoundRectangle(rect, 2))
                {
                    graphic.SetClip(clip, CombineMode.Intersect);
                    using (GraphicsPath brad = CreateBottomRadialPath(rect))
                    {
                        using (PathGradientBrush pgr = new PathGradientBrush(brad))
                        {
                            unchecked
                            {
                                int opacity = (int)(0xB2 * glowOpacity + .5f);
                                RectangleF bounds = brad.GetBounds();
                                pgr.CenterPoint = new PointF((bounds.Left + bounds.Right) / 2f, (bounds.Top + bounds.Bottom) / 2f);
                                pgr.CenterColor = Color.FromArgb(opacity, glowColor);
                                pgr.SurroundColors = new Color[] { Color.FromArgb(0, glowColor) };
                            }
                            graphic.FillPath(pgr, brad);
                        }
                    }
                    graphic.ResetClip();
                }

            }
        }

        public override PropertyInfo[] GetPropertyInfoList()
        {
            return new PropertyInfo[]
            {
                PROPERTY_BORDER_WIDTH,
            };
        }


        protected void HECSBaseWidget_MouseEnter(object sender, EventArgs e)
        {
            //MouseHover를 사용할 경우
            if (useMouseHover == true)
            {
                isHovered = true;
            }
            Invalidate();
        }

        protected void HECSBaseWidget_MouseLeave(object sender, EventArgs e)
        {
            if (this.tooltip != null)
                this.tooltip.Hide(this);
            isHovered = false;
            Invalidate();
        }

        protected void timerBlinker_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            tick = tick + 1;
            if (tick >= 2)
            {
                tick = 0;
            }
            Invalidate();
        }


        #region DrawFunctions
        protected void DrawEclipse(Graphics graphic, Rectangle rect, Color backColor, Color foreColor)
        {
            Pen fgColor = new Pen(foreColor);
            Brush backColorBrush = new SolidBrush(backColor);
            graphic.FillEllipse(backColorBrush, rect);
            graphic.DrawEllipse(fgColor, rect);
        }
        protected void DrawFillRectangle(Graphics graphic, Rectangle rect, Color foreColor, Color backColor)
        {
            int penWidth = 1;

            int pointX = rect.X;
            int pointY = rect.Y;
            int width = rect.Width;
            int height = rect.Height;
            Brush backColorBrush = new SolidBrush(backColor);
            Brush foreColorBrush = new SolidBrush(foreColor);

            graphic.FillRectangle(foreColorBrush, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
            Pen pen = new Pen(backColor, penWidth);
            graphic.DrawRectangle(pen, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
            pen.Dispose();
        }
        protected void DrawRoundFillRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth)
        {
            int pointX = rect.X;
            int pointY = rect.Y;
            int width = rect.Width;
            int height = rect.Height;
            Brush backColorBrush = new SolidBrush(backColor);
            Brush foreColorBrush = new SolidBrush(foreColor);

            Pen pen = new Pen(backColor, penWidth);   //랙마
            graphic.FillRectangle(foreColorBrush, new Rectangle(pointX + (penWidth / 2), pointY + (penWidth / 2), width - penWidth, height - penWidth));
            GraphicsPath gp = new GraphicsPath();
            gp.AddLine(pointX + rad, pointY, pointX + width - (rad * 2), pointY);
            gp.AddArc(pointX + width - (rad * 2), pointY, rad * 2, rad * 2, 270, 90);
            gp.AddLine(pointX + width, pointY + rad, pointX + width, pointY + height - (rad * 2));
            gp.AddArc(pointX + width - (rad * 2), pointY + height - (rad * 2), rad * 2, rad * 2, 0, 90);
            gp.AddLine(pointX + width - (rad * 2), pointY + height, pointX + rad, pointY + height);
            gp.AddArc(pointX, pointY + height - (rad * 2), rad * 2, rad * 2, 90, 90);
            gp.AddLine(pointX, pointY + height - (rad * 2), pointX, pointY + rad);
            gp.AddArc(pointX, pointY, rad * 2, rad * 2, 180, 90);
            gp.CloseFigure();
            graphic.DrawPath(pen, gp);
            gp.Dispose();
        }
        protected void DrawRoundRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth)
        {
            Pen pen = new Pen(backColor, penWidth);
            GraphicsPath gp = new GraphicsPath();
            int pointLeft = rect.Left;
            int pointTop = rect.Top;
            int width = rect.Width;
            int height = rect.Height;
            int degree = rad << 1;
            gp.AddArc(pointLeft, pointTop, degree, degree, 180, 90); // topleft
            gp.AddLine(pointLeft + rad, pointTop, pointLeft + width - rad, pointTop); // top
            gp.AddArc(pointLeft + width - degree, pointTop, degree, degree, 270, 90); // topright
            gp.AddLine(pointLeft + width, pointTop + rad, pointLeft + width, pointTop + height - rad); // right
            gp.AddArc(pointLeft + width - degree, pointTop + height - degree, degree, degree, 0, 90); // bottomright
            gp.AddLine(pointLeft + width - rad, pointTop + height, pointLeft + rad, pointTop + height); // bottom
            gp.AddArc(pointLeft, pointTop + height - degree, degree, degree, 90, 90); // bottomleft
            gp.AddLine(pointLeft, pointTop + height - rad, pointLeft, pointTop + rad); // left
            gp.CloseFigure();
            graphic.DrawPath(pen, gp);
            gp.Dispose();
        }
        protected void DrawTopRoundRectangle(Graphics graphic, Rectangle rect, int rad, Color foreColor, Color backColor, int penWidth)
        {
            Pen pen = new Pen(backColor, penWidth);
            GraphicsPath gp = new GraphicsPath();
            int pointLeft = rect.Left;
            int pointTop = rect.Top;
            int width = rect.Width;
            int height = rect.Height;
            int degree = rad << 1;
            gp.AddArc(pointLeft, pointTop, degree, degree, 180, 90); // topleft
            gp.AddLine(pointLeft + rad, pointTop, pointLeft + width - rad, pointTop); // top
            gp.AddArc(pointLeft + width - degree, pointTop, degree, degree, 270, 90); // topright
            gp.AddLine(pointLeft + width, pointTop + rad, pointLeft + width, pointTop + height); // right
            gp.AddLine(pointLeft + width, pointTop + height, pointLeft, pointTop + height); // bottom
            gp.AddLine(pointLeft, pointTop + height, pointLeft, pointTop + rad); // left
            gp.CloseFigure();
            graphic.DrawPath(pen, gp);
            gp.Dispose();
        }
        protected void DrawBottomRadialPath(Graphics graphic, Rectangle rect, Color foreColor, Color backColor, int penWidth)
        {
            Pen pen = new Pen(backColor, penWidth);
            GraphicsPath gp = new GraphicsPath();
            RectangleF rectangle = rect;
            rectangle.X -= rectangle.Width * .35f;
            rectangle.Y -= rectangle.Height * .15f;
            rectangle.Width *= 1.7f;
            rectangle.Height *= 2.3f;
            gp.AddEllipse(rect);
            gp.CloseFigure();
            graphic.DrawPath(pen, gp);
            gp.Dispose();
        }
        protected void DrawHover(Graphics graphic)
        {

            Rectangle rcBorder = new Rectangle(5, 5, this.Width - 11, this.Height - 11);
            Pen penTop1 = new Pen(Color.FromArgb(255, 240, 207));
            Pen penTop2 = new Pen(Color.FromArgb(253, 216, 137));

            graphic.DrawLine(penTop1, rcBorder.Left + 2, rcBorder.Top + 1, rcBorder.Right - 2, rcBorder.Top + 1);
            graphic.DrawLine(penTop2, rcBorder.Left + 1, rcBorder.Top + 2, rcBorder.Right - 1, rcBorder.Top + 2);

            penTop1.Dispose();
            penTop2.Dispose();

            // Bottom
            Pen penBottom1 = new Pen(Color.FromArgb(248, 178, 48));
            Pen penBottom2 = new Pen(Color.FromArgb(229, 151, 0));

            graphic.DrawLine(penBottom1, rcBorder.Left + 1, rcBorder.Bottom - 2, rcBorder.Right - 1, rcBorder.Bottom - 2);
            graphic.DrawLine(penBottom2, rcBorder.Left + 2, rcBorder.Bottom - 1, rcBorder.Right - 2, rcBorder.Bottom - 1);

            penBottom1.Dispose();
            penBottom2.Dispose();

            // Left and Right
            Rectangle rcLeft = new Rectangle(rcBorder.Left + 1, rcBorder.Top + 3, 2, rcBorder.Height - 5);
            Rectangle rcRight = new Rectangle(rcBorder.Right - 2, rcBorder.Top + 3, 2, rcBorder.Height - 5);

            LinearGradientBrush brushSide = new LinearGradientBrush(rcLeft, Color.FromArgb(254, 221, 149), Color.FromArgb(249, 180, 53), LinearGradientMode.Vertical);
            graphic.FillRectangle(brushSide, rcLeft);
            graphic.FillRectangle(brushSide, rcRight);
            brushSide.Dispose();
        }


        #endregion

        #region Custom Property Modified
        public override void UserDefinedPropertyModified(string name, string newValue)
        {
            if (name.Equals(PROPERTY_NAME_BORDER_WIDTH))
            {
                try
                {
                    this.borderWidth = Int32.Parse(newValue);
                }
                catch (Exception ex)
                {
                    this.borderWidth = 1;
                    System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
                }
            }
            else
            {
                base.UserDefinedPropertyModified(name, newValue);
            }

            Invalidate();


        }
        #endregion


    }
}
