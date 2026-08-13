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
    public partial class StockerIDControl : BaseControl
    {
        private int borderWith = 2;

        private string stockerID = "STOCKER";
        public string StockerID
        {
            get { return stockerID; }
            set { stockerID = value; }
        }

        private string softVersion = "0.1.1";
        public string SoftVersion
        {
            get { return softVersion; }
            set { softVersion = value; }
        }

        private Brush fillBrush = Brushes.RoyalBlue    ;

        public Brush FillBrush
        {
            get
            {
                return fillBrush;
            }
            set
            {
                fillBrush = value;
                Invalidate();
            }
        }
        private Brush  borderBrush = Brushes.DarkBlue   ;

        public Brush BorderBrush
        {
            get
            {
                return borderBrush;
            }
            set
            {
                borderBrush = value;
                Invalidate();
            }
        }
        private Brush fontBrush = Brushes.White;

        public Brush FontBrush
        {
            get
            {
                return fontBrush;
            }
            set
            {
                fontBrush = value;
                Invalidate();

            }
        }

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

        public StockerIDControl()
        {
            InitializeComponent();
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


            Rectangle borderRect = new Rectangle(borderWith, borderWith, this.Width - (borderWith * 2), this.Height - (borderWith * 2));
            DrawRoundRect(g, borderRect, 4, FillBrush, BorderBrush, borderWith);
            Font f = new Font("Arial", 14, FontStyle.Bold);
            StringFormat format = new StringFormat();
            format.Alignment = StringAlignment.Center;
            format.LineAlignment = StringAlignment.Center;
            g.DrawString(stockerID, f, Brushes.Black, new RectangleF(0, 1, this.Width, (this.Height / 3) * 2 + 1), format);
            g.DrawString(stockerID, f, Brushes.Black, new RectangleF(1, 0, this.Width-1, (this.Height / 3) * 2), format);
            g.DrawString(stockerID, f, fontBrush, new RectangleF(0, 0, this.Width, (this.Height / 3) * 2), format);

            f = new Font("Arial", 9, FontStyle.Bold);
            format = new StringFormat();
            format.Alignment = StringAlignment.Far;
            format.LineAlignment = StringAlignment.Center;
            g.DrawString(softVersion, f, fontBrush, new RectangleF(0, (this.Height / 3) * 2, this.Width - 10, this.Height / 3), format);
        }

    }
}
