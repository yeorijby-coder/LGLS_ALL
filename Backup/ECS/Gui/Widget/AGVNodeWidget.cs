using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Widget
{
    public partial class AGVNodeWidget : HECSBaseWidget
    {
        private int drawType = 0;

        public int DrawType
        {
            get
            {
                return drawType;
            }
            set
            {
                drawType = value;
                Invalidate();
            }
        }

        public AGVNodeWidget()
        {
            InitializeComponent();
            this.borderWidth = 0;
            this.WidgetForeColor = Color.Black;
            this.WidgetBackColor = Color.Transparent;
            this.Radius = 5;
            this.SetStyle(ControlStyles.ContainerControl, false);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;

            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            //graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            List<Point> point = new List<Point>();
            int lineThick = this.borderWidth;
            bool isCurve = false;
            switch (drawType)
            {
                case 0:
                    point.Add(new Point(0, 0));
                    point.Add(new Point(this.Width, 0));
                    lineThick = lineThick * 2;
                    break;
                case 1:
                    point.Add(new Point(0, this.Height));
                    point.Add(new Point(this.Width, this.Height));
                    lineThick = lineThick * 2;
                    break;
                case 2:
                    point.Add(new Point(0, 0));
                    point.Add(new Point(0, this.Height));
                    lineThick = lineThick * 2;
                    break;
                case 3:
                    point.Add(new Point(this.Width, 0));
                    point.Add(new Point(this.Width, this.Height));
                    lineThick = lineThick * 2;
                    break;
                case 4:
                    point.Add(new Point(0, this.Height / 2));
                    point.Add(new Point(this.Width, this.Height / 2));
                    break;
                case 5:
                    point.Add(new Point(this.Width / 2, 0));
                    point.Add(new Point(this.Width / 2, this.Height));
                    break;
                case 6:
                    point.Add(new Point(0, 0));
                    point.Add(new Point(this.Width - this.Radius, 0));
                    point.Add(new Point(this.Width, this.Radius));
                    point.Add(new Point(this.Width - 1, this.Height));
                    lineThick = lineThick * 2;
                    isCurve = true;
                    break;
                case 7:
                    point.Add(new Point(0, this.Height-1 ));
                    point.Add(new Point(this.Width - this.Radius, this.Height-1));
                    point.Add(new Point(this.Width, this.Height -this.Radius-1));
                    point.Add(new Point(this.Width - 1, 0));
                    lineThick = lineThick * 2;
                    isCurve = true;
                    break;
                case 8:
                    point.Add(new Point(this.Width, 0));
                    point.Add(new Point(this.Radius, 0));
                    point.Add(new Point(0, this.Radius));
                    point.Add(new Point(1, this.Height));
                    lineThick = lineThick * 2;
                    isCurve = true;
                    break;
                case 9:
                    point.Add(new Point(this.Width, this.Height - 1));
                    point.Add(new Point(this.Radius, this.Height - 1));
                    point.Add(new Point(0, this.Height - this.Radius - 1));
                    point.Add(new Point(1, 0));
                    lineThick = lineThick * 2;
                    isCurve = true;
                    break;
            }
            if (isCurve)
            {
                graphics.DrawBeziers(new Pen(this.WidgetForeColor, lineThick), point.ToArray());
            }
            else
            {
                graphics.DrawLines(new Pen(this.WidgetForeColor, lineThick), point.ToArray());
            }
        }
    }
}
