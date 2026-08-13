using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Interfaces.Service.EventBroker;

namespace HECS.Gui.Monitor.Controls
{
    public delegate void EventDelegate(object sender, EventArgs e);

    [Designer("System.Windows.Forms.Design.ParentControlDesigner,System.Design", typeof(System.ComponentModel.Design.IDesigner))]
    public partial class ControlBase : UserControl
    {
        private Padding _borderThick ;
        private Color _borderColor;
        private Color _backColor;
        private Timer _timer;
        private int _blinkInterval;
        private int _blinkTick;

        public int BlinkInterval
        {
            get
            {
                return _blinkInterval;
            }
            set
            {
                _blinkInterval = value;
                SetBlink(_blinkInterval);
            }
        }

        protected virtual void SetBlink(int interval)
        {
            if (interval <= 0)
            {
                _blinkInterval = 0;
                _timer.Enabled = false;
                panelBack.BackColor = _borderColor;
            }
            else
            {
                _timer.Interval = interval;
                _timer.Enabled = true;
            }
        }
        public Padding BorderThick
        {
            get
            {
                return _borderThick;
            }
            set
            {
                _borderThick = value;
                panelBack.Padding = value;
            }
        }

        public Color BorderColor
        {
            get
            {
                return _borderColor;
            }
            set
            {
                _borderColor = value;
                panelBack.BackColor = _borderColor;
            }
        }

        public new Color BackColor
        {
            get
            {
                return base.BackColor;
            }
            set
            {
                base.BackColor = value;
                panelContent.BackColor = base.BackColor;
            }
        }


        public new Font Font
        {
            get
            {
                return base.Font;
            }
            set
            {
                base.Font = value;
                panelContent.Font = value;
            }
        }




        public ControlBase()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            _borderThick = new Padding(1, 1, 1, 1);
            _borderColor = Color.Black;
            _backColor = Color.White;
            _timer = new Timer();
            _blinkInterval = 0;
            _blinkTick = 1;

            panelBack.Padding = _borderThick;
            panelBack.BackColor = _borderColor;
            panelContent.BackColor = _backColor;
            _timer.Tick += new EventHandler(_timer_Tick);
        }

        protected void _timer_Tick(object sender, EventArgs e)
        {
            if (_blinkTick % 2 == 0)
            {
                _blinkTick = 1;
                panelBack.BackColor = Color.Transparent;
            }
            else
            {
                _blinkTick = 2;
                panelBack.BackColor = _borderColor;
            }
            TimerTick(_blinkTick);
        }
        
        protected virtual void TimerTick(int tick)
        {
            return;
        }


        public new event EventDelegate Click;

        public event EventDelegate ControlClicked
        {
            add
            {
                Click += value;
            }

            remove
            {
                Click -= value;
            }
        }

        protected virtual void ClickEventRaised(object sender, EventArgs e)
        {
            if (Click != null)
            {
                Click(sender, e);
            }
        }





        private void panelBack_Click(object sender, EventArgs e)
        {
            ClickEventRaised(this, e);
        }


        private void ControlBase_ControlAdded(object sender, ControlEventArgs e)
        {
            e.Control.BringToFront();
        }

        private void panelContent_Click(object sender, EventArgs e)
        {
            ClickEventRaised(this, e);
        }

    }
}
