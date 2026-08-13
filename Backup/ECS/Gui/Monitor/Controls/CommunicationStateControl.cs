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
    public partial class CommunicationStateControl : BaseControl
    {

        public enum COMMUNICATION_STATE
        {
            EQUIPMENT_OFFLINE = 1,
            EQUIPMENT_GOING_ONLINE = 2,
            HOST_OFFLINE = 3,
            ONLINE_LOCAL = 4,
            ONLINE_REMOTE = 5
        }

        Timer timer = new Timer();
        private int tickCount = 0;
        private COMMUNICATION_STATE communicationState = COMMUNICATION_STATE.EQUIPMENT_OFFLINE;
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

        public COMMUNICATION_STATE CommunicationState
        {
            get
            {
                return communicationState;
            }
            set
            {
                communicationState = value;
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



        public CommunicationStateControl()
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
            string textTop = "";
            string textBottom = "";
            switch (communicationState)
            {
                case COMMUNICATION_STATE.EQUIPMENT_OFFLINE:
                    textTop = "Equip";
                    textBottom = "Offline";
                    fillBrush = Brushes.Red ;
                    borderBrush = Brushes.Gray ;
                    break;
                case COMMUNICATION_STATE.EQUIPMENT_GOING_ONLINE:
                    textTop = "Equip";
                    textBottom = "Go Online";
                    fillBrush = Brushes.Yellow ;
                    borderBrush = Brushes.DarkRed;
                    break;
                case COMMUNICATION_STATE.HOST_OFFLINE:
                    textTop = "Host";
                    textBottom = "Offline";
                    fillBrush = Brushes.Goldenrod  ;
                    borderBrush = Brushes.DarkGoldenrod;
                    break;
                case COMMUNICATION_STATE.ONLINE_LOCAL:
                    textTop = "Local";
                    textBottom = "Online";
                    fillBrush = Brushes.Lime;
                    borderBrush = Brushes.GreenYellow;
                    break;
                case COMMUNICATION_STATE.ONLINE_REMOTE:
                    textTop = "Host";
                    textBottom = "Online";
                    fillBrush = Brushes.Lime;
                    borderBrush = Brushes.GreenYellow ;
                    break;
            }
            if (IsBlink == true)
            {
                if (tickCount == 1)
                {
                    borderBrush = Brushes.Black;
                }
                else
                {
                    borderBrush = fillBrush;
                }
            }
            else
            {
                borderBrush = Brushes.Black;
            }
            //Rectangle borderRect = new Rectangle((borderWith / 2), (borderWith / 2), this.Width - borderWith, this.Height - borderWith);
            Rectangle borderRect = new Rectangle(borderWith, borderWith, this.Width - (borderWith * 2), this.Height - (borderWith * 2));
            DrawRoundRect(g, borderRect, 4, fillBrush, borderBrush, borderWith);
            Font f = new Font("Arial", 12, FontStyle.Bold);
            StringFormat format = new StringFormat();
            format.Alignment = StringAlignment.Center;
            format.LineAlignment = StringAlignment.Center ;

            g.DrawString(textTop, f, Brushes.Black, new RectangleF(1, 0, this.Width + 1, this.Height / 2), format);
            g.DrawString(textTop, f, Brushes.Black, new RectangleF(0, 1, this.Width, this.Height / 2 + 1), format);
            g.DrawString(textTop, f, Brushes.White, new RectangleF(0, 0, this.Width, this.Height / 2), format);
            g.DrawString(textBottom, f, Brushes.Black, new RectangleF(1, this.Height / 2, this.Width + 1, this.Height / 2), format);
            g.DrawString(textBottom, f, Brushes.Black, new RectangleF(0, this.Height / 2 + 1, this.Width, this.Height / 2 + 1), format);
            g.DrawString(textBottom, f, Brushes.White, new RectangleF(0, this.Height / 2, this.Width, this.Height / 2), format);
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
