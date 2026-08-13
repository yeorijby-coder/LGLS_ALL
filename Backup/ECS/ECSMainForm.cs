using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;


using ECP.Service.Statistics;

using HECS.Statistics;
using HECS.Util;
using HECS.Gui.Movement;
using HECS.Gui.Monitor.Forms;
using System.Runtime.Remoting;
using System.Runtime.InteropServices;
using ECP.Service.Device;
using HECS.Device;

namespace HECS
{
    public partial class ECSMainForm : Form
    {
        //[System.Security.SuppressUnmanagedCodeSecurity]
        //[DllImport("user32.dll", CharSet = CharSet.Auto)]
        //public static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam);
        //public const int WM_MDINEXT = 0x224;

        public ECSMainForm()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.LayoutMdi(MdiLayout.TileHorizontal);
        }
        //public void ActivateMdiChild(Form childToActivate)
        //{
        //    if (this.ActiveMdiChild != childToActivate)
        //    {
        //        MdiClient mdiClient = GetMDIClient();
        //        int count = this.MdiChildren.Length;
        //        Control form = null;  // next or previous MDIChild form

        //        int pos = mdiClient.Controls.IndexOf(childToActivate);
        //        if (pos < 0)
        //            throw new InvalidOperationException("MDIChild form not found");
        //        if (pos == 0)
        //            form = mdiClient.Controls[1];  // get next and activate previous

        //        else
        //            form = mdiClient.Controls[pos - 1];  // get previous and activate next


        //        // flag indicating whether to activate previous or next MDIChild
        //        IntPtr direction = new IntPtr(pos == 0 ? 1 : 0);

        //        // bada bing, bada boom
        //        SendMessage(mdiClient.Handle, WM_MDINEXT, form.Handle, direction);
        //    }
        //}

        //public MdiClient GetMDIClient()
        //{
        //    foreach (Control c in this.Controls)
        //    {
        //        if (c is MdiClient)
        //            return (MdiClient)c;
        //    }
        //    throw new InvalidOperationException("No MDIClient !!!");
        //}

    


        private Form IsLoadedForm(string formName)
        {
            foreach (Form form in this.MdiChildren)
            {
                if (form.GetType().FullName == formName)
                {
                    return form;
                }
            }
            return null;
        }
        private void MinimizeForm(string formName)
        {
            foreach (Form form in this.MdiChildren)
            {
                if (form.GetType().FullName != formName)
                {
                    form.WindowState = FormWindowState.Minimized;
                    //form.Hide();
                }
            }
        }
        private void LoadForm(string formName)
        {
            Form form = IsLoadedForm(formName);
            if (form != null)
            {
                MinimizeForm(formName);
                form.WindowState = FormWindowState.Maximized;
                //form.Show();
            }
            else
            {
                try
                {
                    ObjectHandle objHandle = Activator.CreateInstance(this.GetType().Assembly.FullName, formName);
                    form = (Form)objHandle.Unwrap();
                    MinimizeForm(formName);
                    this.AddOwnedForm(form);
                    form.MdiParent = this;
                    form.Show();
                    form.WindowState = FormWindowState.Maximized;
                    form.BringToFront();
                }
                catch (Exception e)
                {
                    System.Console.WriteLine(this.GetType().FullName + ":" + e.StackTrace);
                }

            }
            if (form == null)
            {
                string[] formNameArray = formName.Split('.');
                Util.Util.ShowMesagePopup("화면이 준비되어 있지 않습니다. [" + formNameArray[formNameArray.Length - 1] + "]");
            }

        }
        private void TSMIMovementMonitoring_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MonitorMain");
        }


        private void ECSMainForm_Load(object sender, EventArgs e)
        {
            //LoadForm("HECS.Gui.Monitor.Forms.LoginForm");
            //LoadForm("HECS.FormTest"); 
            LoadForm("HECS.Gui.Monitor.Forms.MonitorMain");
        }

        private void TSMILocalMovement_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MoveRequestForm");
        }

        private void formTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.FormTest"); 
        }    

        private void TSMITransferTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.TransferTestForm");
        }

        private void TSMIHiRackStatus_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.HiRackStatusForm");
        }

        private void TSMIErrorHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.ErrorHistoryForm");
        }

        private void TSMISAPIFHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.SAPIFHistoryForm");
        }

        private void TSMIMovementRule_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MovementRuleForm");
        }

        private void TSMISystemInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.SystemInfoForm");
        }

        private void logTESTToolStripMenuItem_Click(object sender, EventArgs e)
        {
        
            LoadForm("HECS.Gui.Monitor.Forms.LogTestForm");
        
        }

        private void toolStripButtonSystemInfo_Click(object sender, EventArgs e)
        {
            this.TSMISystemInfo_Click(this, e);
        }

        private void toolStripButtonMovementRule_Click(object sender, EventArgs e)
        {
            this.TSMIMovementRule_Click(this, e); 
        }

        private void toolStripButtonSAPIFHistory_Click(object sender, EventArgs e)
        {
            this.TSMISAPIFHistory_Click(this, e); 
        }

        private void toolStripButtonErrorHistory_Click(object sender, EventArgs e)
        {
            this.TSMIErrorHistory_Click(this, e); 
        }

        private void toolStripButtonHiRackStatus_Click(object sender, EventArgs e)
        {
            this.TSMIHiRackStatus_Click(this, e); 
        }

        private void toolStripButtonMovementMonitroing_Click(object sender, EventArgs e)
        {
            this.TSMIMovementMonitoring_Click(this, e); 
        }

        private void toolStripButtonLocalMovement_Click(object sender, EventArgs e)
        {
            this.TSMILocalMovement_Click(this, e); 
        }

    }
}