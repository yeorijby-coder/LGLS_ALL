using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Controls
{
    public partial class LampInform : HECS.Gui.Monitor.Controls.ControlBase
    {
        public new event HECS.Gui.Monitor.Controls.ControlClick Click;
        private string _lampName = "";

        public string LampName
        {
            get
            {
                return _lampName;
            }
            set
            {
                _lampName = value;
            }
        }
        public LampInform()
        {
            InitializeComponent();
        }

        private void panelBack_Resize(object sender, EventArgs e)
        {
            int _size = 0;
            _size = this.Size.Height / 9;
            label1.Height = _size;
            label2.Height = _size;
            label3.Height = _size;
            label4.Height = _size;
            label5.Height = _size;
            label6.Height = _size;
            label7.Height = _size;
            label8.Height = _size;
            label9.Height = _size;
        }
        public new  event HECS.Gui.Monitor.Controls.ControlClick ControlClicked
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
        protected virtual void ClickEventRaised(object sender, ControlEvent e)
        {
            if (Click != null)
            {
                Click(sender, e);
            }
        }

        private void AnyControlClicked(object sender, EventArgs e)
        {
            ClickEventRaised(sender, new ControlEvent(LampName));
        }
    }
}


