using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Controls
{
    public partial class LabelSingleLine : ControlBase 
    {
        private string _caption;
        private bool _blinkText = false;

        public string Caption
        {
            get
            {
                return _caption;
            }
            set
            {
                _caption = value;
                labelText.Text = _caption;
            }
        }


        public bool BlinkText
        {
            get
            {
                return _blinkText;
            }
            set
            {
                _blinkText = value;
                if (_blinkText == false)
                {
                    labelText.Visible = true;
                }
               
            }
        } 
        

        public LabelSingleLine()
        {
            InitializeComponent();
            _caption = this.Name;
            labelText.Text = _caption;
        }
        protected override void SetBlink(int interval)
        {
            base.SetBlink(interval);
            if (interval <= 0 )
            {
                labelText.Visible = true; 
            }
        }
        protected override void TimerTick(int tick)
        {
            base.TimerTick(tick);
            if (BlinkText == false)
            {
                return;
            }
            if (tick % 2 == 1)
            {
                labelText.Visible = false;
            }
            else
            {
                labelText.Visible = true;
            }
        }

        private void labelText_Click(object sender, EventArgs e)
        {
            ClickEventRaised(sender, e);
        }


    }
}
