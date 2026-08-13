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
    public partial class AGVWidget : HECSBaseWidget
    {
        public AGVWidget()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.ContainerControl, false);
        }
        private Vehicle vehicleObject = null;
        public Vehicle VehicleObject
        {
            get
            {
                return vehicleObject;
            }
            set
            {
                vehicleObject = value;
                if (vehicleObject != null)
                {
                    if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                    {
                        vehicleObject.PropertyChanged += new PropertyChangedEventHandler(vehicleObject_PropertyChanged);
                        vehicleObject_PropertyChanged(vehicleObject, new PropertyChangedEventArgs("DefaultCall"));
                    }
                }
            }
        }

        void vehicleObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
             Invalidate();
        }
        public enum AVG_MOVE_DIRECTION
        {
            LEFT = 1,
            RIGHT = 2,
            UP = 3,
            DOWN = 4,
        }

        private AVG_MOVE_DIRECTION moveDirection = AVG_MOVE_DIRECTION.LEFT;

        public AVG_MOVE_DIRECTION MoveDirection
        {
            get
            {
                return moveDirection;
            }
            set
            {
                moveDirection = value;
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            Point[] point = new Point[5];
            point[0] = new Point(this.borderWidth, this.borderWidth);
            point[1] = new Point(this.borderWidth, this.Height - this.borderWidth);
            point[2] = new Point(this.Width - this.borderWidth - (this.Width / 4), this.Height - this.borderWidth);
            point[3] = new Point(this.Width - this.borderWidth, this.Height / 2);
            point[4] = new Point(this.Width - this.borderWidth - (this.Width / 4), this.borderWidth);


            if (vehicleObject == null)
            {
                graphics.FillPolygon(new SolidBrush(this.WidgetForeColor), point);
                graphics.DrawPolygon(new Pen(this.WidgetBackColor, this.borderWidth), point);
            }
            else
            {
                Color colorTemp = Color.Red;
                if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.IDLE)
                {
                    this.WidgetTextColor = Color.Black;
                    colorTemp = Color.Yellow;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.RUN)
                {
                    this.WidgetTextColor = Color.Black;
                    colorTemp = Color.Lime;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.DOWN)
                {
                    this.WidgetTextColor = Color.White;
                    colorTemp = Color.Red ;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.CHARGE)
                {
                    this.WidgetTextColor = Color.White;
                    colorTemp = Color.Blue;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.EXIT_MAP)
                {
                    this.WidgetTextColor = Color.Black;
                    colorTemp = Color.White;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.MANUAL)
                {
                    this.WidgetTextColor = Color.Black;
                    colorTemp = Color.Plum;
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.OFFLINE)
                {
                    this.WidgetTextColor = Color.Black;
                    colorTemp = Color.Crimson;
                }
                graphics.FillPolygon(new SolidBrush(colorTemp), point);
                graphics.DrawPolygon(new Pen(this.WidgetBackColor, this.borderWidth), point);

                if (string.IsNullOrEmpty (vehicleObject.AgvPalletId) == false)
                {
                    graphics.FillRectangle(Brushes.Blue, new Rectangle(4, 4, this.Width - 8, this.Height - 8));
                }
            }
            
            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, 0, this.Width, this.Height), stringFormat);

        }
    }
}
