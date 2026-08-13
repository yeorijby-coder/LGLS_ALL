using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Widget
{
    public partial class OperationBoxWidget : HECSBaseWidget
    {
        public OperationBoxWidget()
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
            Rectangle rect = new Rectangle(this.borderWidth, this.borderWidth, this.Width - this.borderWidth * 2, this.Height - this.borderWidth * 2);
            SolidBrush brush = new SolidBrush (this.WidgetForeColor);
            graphics.FillEllipse(brush, rect);
            graphics.DrawEllipse(new Pen(this.WidgetBackColor,this.borderWidth), rect);

            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, 0, this.Width, this.Height), stringFormat);

        }
    }
}
