using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace HECS.Gui.Monitor.Controls
{
    public partial class DateTimeControl : BaseControl
    {

        Timer timer = new Timer();
        private int borderWith = 2;
        public int BorderWith
        {
            get
            {
                return borderWith;
            }
            set
            {
                borderWith = value;
                Invalidate();
            }
        }

        public DateTimeControl()
        {
            InitializeComponent();
            timer.Enabled = true;
            timer.Interval = 1000;
            timer.Tick += new EventHandler(Timer_Tick);
     
        }
       
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            DrawBody(e.Graphics);
        }
        private void DrawBody(Graphics g)
        {

            if (g == null)
            {
                g = this.CreateGraphics();
            }

            g.SmoothingMode = SmoothingMode.HighQuality;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            string textDate = DateTime.Now.ToString("yyyy년MM월dd일");
            string textTime = DateTime.Now.ToString ("tt hh:mm.ss");
            Brush fillBrush = Brushes.LightYellow      ;
            Brush borderBrush = Brushes.Goldenrod ;
            Brush fontBrush = Brushes.Black;



            Rectangle borderRect = new Rectangle(borderWith, borderWith, this.Width - (borderWith * 2), this.Height - (borderWith * 2));
            DrawRoundRect(g, borderRect, 4, fillBrush, borderBrush, borderWith);
            Font f = new Font("Arial", 10, FontStyle.Bold);
            StringFormat format = new StringFormat();
            format.Alignment = StringAlignment.Center;
            format.LineAlignment = StringAlignment.Center;

            g.DrawString(textDate, f, fontBrush, new RectangleF(0, 0, this.Width, this.Height / 2), format);
            g.DrawString(textTime, f, fontBrush, new RectangleF(0, this.Height / 2, this.Width, this.Height / 2), format);

        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            base.OnPaintBackground(pevent);
        }

        void Timer_Tick(object sender, EventArgs e)
        {
            Invalidate();
        }

    }
}
