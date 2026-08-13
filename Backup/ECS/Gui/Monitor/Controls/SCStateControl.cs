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
    public partial class SCStateControl : BaseControl
    {

        public enum SCSTATE
        {
            SCINIT = 1,
            PAUSED = 2,
            PAUSING = 3,
            AUTO = 4
        }

        Timer timer = new Timer();
        private int tickCount = 0;
        private SCSTATE scState = SCSTATE.SCINIT;
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

        public SCSTATE ScState
        {
            get
            {
                return scState;
            }
            set
            {
                scState = value;
                Invalidate();
            }
        }
        private bool isBlink = false;

        public bool IsBlink
        {
            get
            {
                return isBlink;
            }
            set
            {
                isBlink = value;
                if (isBlink == true)
                {
                    timer.Enabled = true; 
                }
                else
                {
                    timer.Enabled = false; 
                }
                Invalidate();
            }
        }
        private int interval = 500;    // ms  500 = 0.5sec 

        public int Interval
        {
            get
            {
                return interval;
            }
            set
            {
                interval = value;
                Invalidate();
            }
        }



        public SCStateControl()
        {
            InitializeComponent();
            timer.Enabled = false;
            timer.Interval = 500;
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
            Brush fillBrush = Brushes.White;
            Brush borderBrush = Brushes.Black;
            Brush textBrush = Brushes.Black;

            string textSCState = "";
            switch (scState )
            {
                case SCSTATE.PAUSED :
                    textSCState = "PAUSED";
                    fillBrush = Brushes.Beige  ;
                    borderBrush = Brushes.Red  ;
                    textBrush = Brushes.Red ;
                    break;
                case SCSTATE.AUTO  :
                    textSCState = "AUTO";
                    fillBrush = Brushes.LightYellow;
                    borderBrush = Brushes.YellowGreen  ;
                    textBrush = Brushes.Green ;
                    break;
                case SCSTATE.PAUSING :
                    textSCState = "PAUSING";
                    fillBrush = Brushes.LightCyan   ;
                    borderBrush = Brushes.Crimson;
                    textBrush = Brushes.Crimson  ;
                    break;
                case SCSTATE.SCINIT :
                    textSCState = "SCINIT";
                    fillBrush = Brushes.LightGray;
                    borderBrush = Brushes.WhiteSmoke ;
                    textBrush = Brushes.Gray;
                    break;

            }
            if (IsBlink == true)
            {
                if (tickCount != 1)
                {
                    borderBrush = fillBrush;
                    //textBrush = fillBrush;
                }
            }
            //Rectangle borderRect = new Rectangle((borderWith / 2), (borderWith / 2), this.Width - borderWith, this.Height - borderWith);
            Rectangle borderRect = new Rectangle(borderWith, borderWith, this.Width - (borderWith * 2), this.Height - (borderWith * 2));
            DrawRoundRect(g, borderRect, 4, fillBrush, borderBrush, borderWith);
            Font f = new Font("Arial", 18, FontStyle.Bold );
            StringFormat format = new StringFormat();
            format.Alignment = StringAlignment.Center;
            format.LineAlignment = StringAlignment.Center ;

            g.DrawString(textSCState, f, Brushes.Black, new RectangleF(1, 0, this.Width+1, this.Height), format);
            g.DrawString(textSCState, f, Brushes.Black, new RectangleF(0, 1 ,this.Width, this.Height+1), format);
            g.DrawString(textSCState, f, textBrush, new RectangleF(0, 0, this.Width, this.Height), format);
        }

        void Timer_Tick(object sender, EventArgs e)
        {
            if (tickCount == 1)
            {
                tickCount = 0;
                Invalidate();
            }
            else
            {
                tickCount = 1;
                Invalidate();
            }
        }


    }
}
