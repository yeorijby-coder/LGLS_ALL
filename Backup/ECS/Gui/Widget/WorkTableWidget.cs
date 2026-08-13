using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.Device.Unit;

namespace HECS.Gui.Widget
{
    public partial class WorkTableWidget : HECSBaseWidget
    {
        Timer refreshTimer = new Timer();
        private int timerTick = 0;
        public enum PointDirection
        {
            TOP = 1,
            RIGHT = 2,
            BOTTOM = 3,
            LEFT = 4,
        }

        private PointDirection direction = PointDirection.TOP;

        public PointDirection Direction
        {
            get
            {
                return direction;
            }
            set
            {
                direction = value;
                Invalidate();
            }
        }

        public enum WORK_TABLE_TYPE
        {
            INPUT = 1,
            OUTPUT = 2,
        }

        private WORK_TABLE_TYPE workTableType = WORK_TABLE_TYPE.INPUT;

        public WORK_TABLE_TYPE WorkTableType
        {
            get
            {
                return workTableType;
            }
            set
            {
                workTableType = value;
                Invalidate();
            }
        }

        private Conveyor conveyorObject = null;

        public Conveyor ConveyorObject
        {
            get
            {
                return conveyorObject;
            }
            set
            {
                conveyorObject = value;
                if (conveyorObject != null)
                {
                    if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                    {
                        conveyorObject.PropertyChanged += new PropertyChangedEventHandler(conveyorObject_PropertyChanged);
                        conveyorObject_PropertyChanged(conveyorObject, new PropertyChangedEventArgs("DefaultCall"));
                    }
                }
            }
        }

        void conveyorObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            this.timerBlinker.Interval = 1000;
            this.timerBlinker.Enabled = true;
        }



        public WorkTableWidget()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            if (conveyorObject == null)
            {
                this.timerBlinker.Enabled = false;
                this.tick = 0;
            }
            if (this.workTableType == WORK_TABLE_TYPE.INPUT)
            {
                //InputReady=false면 팔렛이 존재하거나 문제가 발생한 것으로 판단하여 Blinking 한다. 
                //반대로 InputReady=True 면, C/V 상태를 정상으로 판단하여, Timer의 수행을 중단한다.
                if (conveyorObject != null && conveyorObject.InReady == true)
                {
                    this.timerBlinker.Enabled = false;
                    this.tick = 0;
                }
            }
            else if (this.WorkTableType == WORK_TABLE_TYPE.OUTPUT)
            {
                //WaitOut=False 면, 타이머의 수행을 중단한다.
                if (conveyorObject != null && conveyorObject.IsWaitOut == false)
                {
                    this.timerBlinker.Enabled = false;
                    this.tick = 0;
                }
            }

            Graphics graphics = e.Graphics;
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            Point[] inPoint = new Point[3];
            switch (this.direction)
            {
                case PointDirection.TOP:
                    inPoint[0] = new Point(this.Width / 2, this.borderWidth);
                    inPoint[1] = new Point(this.borderWidth, this.Height - this.borderWidth);
                    inPoint[2] = new Point(this.Width - this.borderWidth, this.Height - this.borderWidth);
                    break;
                case PointDirection.RIGHT:
                    inPoint[0] = new Point(this.Width - this.borderWidth, this.Height / 2);
                    inPoint[1] = new Point(this.borderWidth, this.Height - this.borderWidth);
                    inPoint[2] = new Point(this.borderWidth, this.borderWidth);
                    break;
                case PointDirection.BOTTOM:
                    inPoint[0] = new Point(this.borderWidth, this.borderWidth);
                    inPoint[1] = new Point(this.Width - this.borderWidth, this.borderWidth);
                    inPoint[2] = new Point(this.Width / 2, this.Height - this.borderWidth);
                    break;
                case PointDirection.LEFT:
                    inPoint[0] = new Point(this.Width - this.borderWidth, this.borderWidth);
                    inPoint[1] = new Point(this.Width - this.borderWidth, this.Height - this.borderWidth);
                    inPoint[2] = new Point(this.borderWidth, this.Height / 2);
                    break;
            }
            if (this.tick == 0)
            {
                graphics.FillPolygon(new SolidBrush(this.WidgetForeColor), inPoint);
                graphics.DrawPolygon(new Pen(this.WidgetBackColor, this.borderWidth), inPoint);
            }
            else
            {
                if (workTableType == WORK_TABLE_TYPE.INPUT)
                {
                    graphics.FillPolygon(new SolidBrush(Color.Crimson), inPoint);
                }
                else
                {
                    graphics.FillPolygon(new SolidBrush(Color.White), inPoint);
                }
                graphics.DrawPolygon(new Pen(Color.Black, this.borderWidth), inPoint);
            }



            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, 0, this.Width, this.Height), stringFormat);

        }

    }
}
