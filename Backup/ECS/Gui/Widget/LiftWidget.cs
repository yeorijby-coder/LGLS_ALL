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
    public partial class LiftWidget : HECSBaseWidget
    {
        public LiftWidget()
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
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            Graphics graphics = e.Graphics;
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            Point[] point = new Point[4];
            point[0] = new Point(this.Width / 2, this.borderWidth);
            point[1] = new Point(this.borderWidth, this.Height / 2);
            point[2] = new Point(this.Width / 2, this.Height - this.borderWidth);
            point[3] = new Point(this.Width - this.borderWidth, this.Height / 2);
            if (vehicleObject == null)
            {
                graphics.FillPolygon(new SolidBrush(this.WidgetForeColor), point);
                graphics.DrawPolygon(new Pen(this.WidgetBackColor, this.borderWidth), point);
            }
            else
            {
                Color colorTemp = Color.Red;
                if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.IDLE )
                {
                    colorTemp = Color.Yellow;
                }
                else if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.RUN )
                {
                    colorTemp = Color.Lime;
                }
                else
                {
                    colorTemp = Color.Red;
                }

                if (vehicleObject.IsAvailible == false)
                {
                    colorTemp = Color.Black;
                }

                graphics.FillPolygon(new SolidBrush(colorTemp), point);
                graphics.DrawPolygon(new Pen(this.WidgetBackColor, this.borderWidth), point);

                if (vehicleObject.IsPalletExist)
                {
                    graphics.FillRectangle(Brushes.Blue, new Rectangle(3, 3, this.Width - 6, this.Height - 6));
                }
            }

            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, 0, this.Width, this.Height), stringFormat);

        }
    }
}
