using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Global;

namespace ECP.Gui.Widget
{
    public partial class TextWidget : BaseWidget
    {
        public TextWidget()
            : base()
        {
            InitializeComponent();
        }
        const string CLASS = "TextWidget";

        public override ECP.Global.Gui.WidgetData WidgetData
        {
            get
            {
                return base.WidgetData;
            }
            set
            {
                base.WidgetData = value;
                SetBounds(widgetData.x, widgetData.y, widgetData.width, widgetData.height);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            
            this.BackColor = WidgetBackColor;
            this.BorderStyle = WidgetBorderStyle;            

            // Draw Text String

            StringFormat format = new StringFormat();
            if (WidgetTextAlignment == ContentAlignment.TopCenter || 
                WidgetTextAlignment == ContentAlignment.TopRight || 
                WidgetTextAlignment == ContentAlignment.TopLeft)
            {
                format.LineAlignment = StringAlignment.Near;
            }
            else if (WidgetTextAlignment == ContentAlignment.MiddleCenter ||
                WidgetTextAlignment == ContentAlignment.MiddleRight ||
                WidgetTextAlignment == ContentAlignment.MiddleLeft)
            {
                format.LineAlignment = StringAlignment.Center;
            }
            else if (WidgetTextAlignment == ContentAlignment.BottomCenter ||
                WidgetTextAlignment == ContentAlignment.BottomRight || 
                WidgetTextAlignment == ContentAlignment.BottomLeft)
            {
                format.LineAlignment = StringAlignment.Far;
            }

            if (WidgetTextAlignment == ContentAlignment.BottomLeft || 
                WidgetTextAlignment == ContentAlignment.TopLeft ||
                WidgetTextAlignment == ContentAlignment.MiddleLeft)
            {
                format.Alignment = StringAlignment.Near;
            }
            else if (WidgetTextAlignment == ContentAlignment.TopCenter || 
                WidgetTextAlignment == ContentAlignment.MiddleCenter || 
                WidgetTextAlignment == ContentAlignment.BottomCenter)
            {
                format.Alignment = StringAlignment.Center;
            }
            else if (WidgetTextAlignment == ContentAlignment.TopRight ||
                WidgetTextAlignment == ContentAlignment.MiddleRight ||
                WidgetTextAlignment == ContentAlignment.BottomRight)
            {
                format.Alignment = StringAlignment.Far;
            }

            Rectangle fontRect = new Rectangle(WidgetTextCornerMargin, WidgetTextCornerMargin, 
                this.Width - WidgetTextCornerMargin, this.Height - WidgetTextCornerMargin);

            g.DrawString(this.WidgetText, this.WidgetTextFont, 
                new SolidBrush(this.WidgetTextColor), fontRect, format);


            base.OnPaint(e);
            format.Dispose();
        }
    }
}
