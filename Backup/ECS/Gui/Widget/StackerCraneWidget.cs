using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using HECS.Device.Unit;

namespace HECS.Gui.Widget
{
    public partial class StackerCraneWidget : HECSBaseWidget
    {
        public StackerCraneWidget()
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
            Graphics graphics = e.Graphics;

            //GraphicsPath graphicPath = new GraphicsPath();
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            //Matrix matrix = new Matrix(1, 0, 0, 1, 1, 1);
            ////graphics.Transform = matrix;
            //matrix.RotateAt(270.0F,new Point(this.Width / 2 -1, this.Height / 2-1));
            ////graphics.RotateTransform(90.0F, MatrixOrder.Append  );
            //graphics.Transform = matrix ;
            Point[] point = new Point[6];
            point[0] = new Point(this.Width / 4 + this.borderWidth, this.borderWidth);
            point[1] = new Point(this.borderWidth, this.Height / 2);
            point[2] = new Point(this.Width / 4 + this.borderWidth, this.Height - this.borderWidth);
            point[3] = new Point(this.Width - this.borderWidth - (this.Width / 4), this.Height - this.borderWidth);
            point[4] = new Point(this.Width - this.borderWidth, this.Height / 2);
            point[5] = new Point(this.Width - this.borderWidth - (this.Width / 4), this.borderWidth);
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
