using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.Gui.Monitor;
using System.Threading;

namespace HECS
{
    public partial class StockerMainForm :Form
    {
        public StockerMainForm()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint,true );
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.CacheText, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.SetStyle(ControlStyles.ResizeRedraw, false);
            this.ResumeLayout();
            this.Focus();
        }

        private void StockerMainForm_Activated(object sender, EventArgs e)
        {
        }

        private void StockerMainForm_Load(object sender, EventArgs e)
        {
            MonitorBuilder.BuildAll(this);

        }

        private void StockerMainForm_VisibleChanged(object sender, EventArgs e)
        {
        }

        private void StockerMainForm_ResizeEnd(object sender, EventArgs e)
        {
      

        }

        private void StockerMainForm_Shown(object sender, EventArgs e)
        {
            Splasher.Close();
        }
    }
}