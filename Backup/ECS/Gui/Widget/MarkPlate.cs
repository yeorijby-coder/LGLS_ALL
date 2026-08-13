using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Widget
{
    public partial class MarkPlate : HECSBaseWidget
    {
        public MarkPlate()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.ContainerControl, false);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            Graphics graphics = e.Graphics;
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;
            graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            Rectangle rect = new Rectangle(this.borderWidth, this.borderWidth, this.Width - this.borderWidth * 2-1, this.Height - this.borderWidth * 2-1);
            SolidBrush brush = new SolidBrush(this.WidgetForeColor);
            graphics.FillRectangle (brush, rect);
            graphics.DrawRectangle(new Pen(this.WidgetBackColor, this.borderWidth), rect);
            if (this.Height >= 16 && this.Width >= 16)
            {
                graphics.DrawLine(new Pen(this.WidgetBackColor, 1), new Point(0, 0), new Point(this.Width - 1, this.Height - 1));
                graphics.DrawLine(new Pen(this.WidgetBackColor, 1), new Point(0, this.Height - 1), new Point(this.Width - 1, 0));
            }
            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, 0, this.Width, this.Height), stringFormat);

        }
    }
}
