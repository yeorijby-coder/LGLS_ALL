using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Controls
{
    public partial class LabelDualLine : HECS.Gui.Monitor.Controls.ControlBase
    {
        private string _captionTop = "";
        private string _captionBottom = "";
        private bool _blinkText = false;
        private Font _captionTopFont = new Font("Arial", 10);
        private Font _captionBottomFont = new Font("Arial", 10);
        private ContentAlignment _captionTopAlignment = ContentAlignment.MiddleCenter;
        private ContentAlignment _captionBottomAlignment = ContentAlignment.MiddleCenter;

        public ContentAlignment CaptionTopAlignment
        {
            get
            {
                return _captionTopAlignment;
            }
            set
            {
                _captionTopAlignment = value;
                labelTop.TextAlign = value;
            }
        }

        public ContentAlignment CaptionBottomAlignment
        {
            get
            {
                return _captionBottomAlignment;
            }
            set
            {
                _captionBottomAlignment = value;
                labelBottom.TextAlign = value;
            }
        }
        public Font CaptionTopFont
        {
            get
            {
                return _captionTopFont;
            }
            set
            {
                _captionTopFont = value;
                labelTop.Font = value;
            }
        }

        public Font CaptionBottomFont
        {
            get
            {
                return _captionBottomFont;
            }
            set
            {
                _captionBottomFont = value;
                labelBottom.Font = value;
            }
        }


        public LabelDualLine()
        {
            InitializeComponent();
            _captionTopFont = Font;
            _captionBottomFont = Font;

            labelTop.Height = base.Height / 2;
            labelBottom.Height = base.Height / 2;

            _captionTop = this.Name + "_Top";
            labelTop.Text = _captionTop;

            _captionBottom = this.Name + "_Bottom";
            labelBottom.Text = _captionBottom;


        }
        private void LabelDualLine_Resize(object sender, EventArgs e)
        {
            labelTop.Height = base.Height / 2;
            labelBottom.Height = base.Height / 2;
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
                    labelTop.Visible = true;
                    labelBottom.Visible = true;
                }

            }
        }
        public string CaptionTop
        {
            get
            {
                return _captionTop;
            }
            set
            {
                _captionTop = value;
                labelTop.Text = _captionTop;
            }
        }

        public string CaptionBottom
        {
            get
            {
                return _captionBottom;
            }
            set
            {
                _captionBottom = value;
                labelBottom.Text = _captionBottom;
            }
        }

        protected override void SetBlink(int interval)
        {
            base.SetBlink(interval);
            if (interval <= 0)
            {
                labelTop.Visible = true;
                labelBottom.Visible = true;
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
                labelTop.Visible = false;
                labelBottom.Visible = false;
            }
            else
            {
                labelTop.Visible = true;
                labelBottom.Visible = true;
            }
        }

        private void labelTop_Click(object sender, EventArgs e)
        {
            ClickEventRaised(sender, e);
        }

        private void labelBottom_Click(object sender, EventArgs e)
        {
            ClickEventRaised(sender, e);
        }
        //protected override void ClickEventRaised(object sender, EventArgs e)
        //{
        //    base.ClickEventRaised(sender, e);
        //}
    }
}

