using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS
{
    public partial class SplashForm : Form
    {
        private string statusInfo = "";

        public SplashForm()
        {
            InitializeComponent();
            this.SizeChanged += new EventHandler(SplashForm_SizeChanged);
        }

        void SplashForm_SizeChanged(object sender, EventArgs e)
        {
            panelDraw.Left = (this.Width - panelDraw.Width) / 2;
            panelDraw.Top = (this.Height - panelDraw.Height) / 2;
        }



        public string StatusInfo
        {
            set
            {
                statusInfo = value;
                //ChangeStatusText();
                UpdateDelegate();
            }
            get
            {
                return statusInfo;
            }
        }

        public void UpdateLoadInfo()
        {
            richTextProgress.AppendText("[" + DateTime.Now.ToString("HH:mm:ss") + "] " + statusInfo + "\r\n");
        }
        private delegate void delegateUpdateSplashInfo();

        public void UpdateDelegate()
        {
            try
            {
                //if(this.InvokeRequired)
                this.BeginInvoke(new delegateUpdateSplashInfo(UpdateLoadInfo));
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("SplashForm:" + ex.StackTrace);
            }
        }
        public void ChangeStatusText()
        {
            try
            {
                if (this.InvokeRequired)
                {
                    this.Invoke(new MethodInvoker(this.ChangeStatusText));
                    return;
                }
                richTextProgress.AppendText("[" + DateTime.Now.ToString("HH:mm:ss") + "] " + statusInfo + "\r\n");
                //lStatusInfo.Text = _StatusInfo;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("ChangeStatusText:" + ex.StackTrace);
            }
        }

    }
}