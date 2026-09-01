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
    public partial class ECSClientMainForm : Form
    {
        //[System.Security.SuppressUnmanagedCodeSecurity]
        //[DllImport("user32.dll", CharSet = CharSet.Auto)]
        //public static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam);
        //public const int WM_MDINEXT = 0x224;

        public ECSClientMainForm()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.LayoutMdi(MdiLayout.TileHorizontal);
            if (ECP.Global.GlobalConstant.CLIENT_TYPE == "QI")
            {
                TSMIPalletBlocking.Visible = true;
                toolStripButtonPalletBlocking.Visible = true;
                toolStripButtonMovementMonitroing.Visible = false;
                TSMIMovementMonitroing.Visible = false;
            }
            else
            {
                TSMIPalletBlocking.Visible = false;
                toolStripButtonPalletBlocking.Visible = false;
                toolStripButtonMovementMonitroing.Visible = false;
                TSMIMovementMonitroing.Visible = false; 
            }
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
                Util.Util.ShowMesagePopup("ȭ���� �غ�Ǿ� ���� �ʽ��ϴ�. [" + formNameArray[formNameArray.Length - 1] + "]");
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
            LoadForm("HECS.Gui.Monitor.Forms.MoveRequestForm");

            // [LGLS 2026-09-01] 설비 팝업 사전 생성 - 종전에는 첫 클릭 때 폼 생성(그라데이션
            //   컨트롤 + System.Web 어셈블리 JIT 로드)이 일어나 팝업이 현저히 늦게 떴다.
            //   기동 유휴 시점에 미리 만들어 캐시에 넣는다 (MonitorAllEquipmentPanel 의 캐시 키와 동일).
            this.BeginInvoke(new MethodInvoker(PrewarmPopupForms));
        }

        // [LGLS 2026-09-01] 설비 팝업 폼 3종 사전 생성 (핸들까지 만들어 첫 표시 JIT 비용 제거)
        private void PrewarmPopupForms()
        {
            try
            {
                if (SimpleCache.GetObject("Popup", "ConveyorForm") == null)
                {
                    HECS.Gui.Monitor.Popups.ConveyorForm f = new HECS.Gui.Monitor.Popups.ConveyorForm();
                    IntPtr h = f.Handle;
                    SimpleCache.AddObject("Popup", "ConveyorForm", f);
                }
                if (SimpleCache.GetObject("Popup", "StackerForm") == null)
                {
                    HECS.Gui.Monitor.Popups.StackerForm f = new HECS.Gui.Monitor.Popups.StackerForm();
                    IntPtr h = f.Handle;
                    SimpleCache.AddObject("Popup", "StackerForm", f);
                }
                if (SimpleCache.GetObject("Popup", "RGVForm") == null)
                {
                    HECS.Gui.Monitor.Popups.RGVForm f = new HECS.Gui.Monitor.Popups.RGVForm();
                    IntPtr h = f.Handle;
                    SimpleCache.AddObject("Popup", "RGVForm", f);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("PrewarmPopupForms:" + ex.Message);
            }
        }



        private void TSMISapMovement_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.TransferSummaryForm");

        }

        private void TSMILocalMovement_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MoveRequestForm");
        }

        private void TSMIECSMovement_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.ECSTransferRequestForm");
        }

        private void TSMIPalletBlocking_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.PalletBlockingForm");
        }

        private void TSMITransferTimeMonitorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.TransferTimeMonitorForm");
        }

        private void TSMITransferTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.TransferTestForm");
        }

        private void TSMITransferMovementTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.TransferMovementTestForm");
        }

        private void TSMISAPMovementIF_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.SAPMovementIFForm");
        }

        private void TSMIPalletStatus_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.PalletStatusForm");
        }

        private void TSMIHiRackStatus_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.HiRackStatusForm");
        }

        private void TSMIMovementHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MovementHistoryForm");
        }

        private void TSMIPalletHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.PalletHistoryForm");
        }

        private void TSMIHiRackHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.HiRackHistoryForm");
        }

        private void TSMIErrorHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.ErrorHistoryForm");
        }

        private void TSMISAPIFHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.SAPIFHistoryForm");
        }

        private void TSMIUserInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.UserInfoForm");
        }

        private void TSMIProgramInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.ProgramInfoForm");
        }

        private void TSMIRoleInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.RoleInfoForm");
        }

        private void TSMIRoleForUser_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.RoleForUserForm");
        }

        private void TSMIProgramForRole_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.ProgramForRoleForm");
        }

        private void TSMIMovementRule_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MovementRuleForm");
        }

        private void TSMISystemInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.SystemInfoForm");
        }

        private void TSMIDeviceInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.EquipmentInfoForm");
        }

        private void TSMIMaterialInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.MaterialInfoForm");
        }

        private void TSMIBatchInfo_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.BatchInfoForm");
        }

        private void TSMIPalletBlockingHistory_Click(object sender, EventArgs e)
        {
            LoadForm("HECS.Gui.Monitor.Forms.PalletBlockingHistoryForm");
        }

        private void logTESTToolStripMenuItem_Click(object sender, EventArgs e)
        {
        
            LoadForm("HECS.Gui.Monitor.Forms.LogTestForm");
        
        }

        private void TSMIEquipmentControl_Click(object sender, EventArgs e)
        {

        }

        private void toolStripButtonBatchInfo_Click(object sender, EventArgs e)
        {
            this.TSMIBatchInfo_Click(this, e);
        }

        private void toolStripButtonMaterialInfo_Click(object sender, EventArgs e)
        {
            this.TSMIMaterialInfo_Click(this, e); 
        }

        private void toolStripButtonDeviceInfo_Click(object sender, EventArgs e)
        {
            this.TSMIDeviceInfo_Click(this, e); 
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

        private void toolStripButtonHiRackHistory_Click(object sender, EventArgs e)
        {
            this.TSMIHiRackHistory_Click(this, e); 
        }

        private void toolStripButtonPalletBlockingHistory_Click(object sender, EventArgs e)
        {
            this.TSMIPalletBlocking_Click(this, e); 
        }

        private void toolStripButtonPalletHistory_Click(object sender, EventArgs e)
        {
            this.TSMIPalletHistory_Click(this, e); 
        }

        private void toolStripButtonMovementHistory_Click(object sender, EventArgs e)
        {
            this.TSMIMovementHistory_Click(this, e);
        }

        private void toolStripButtonHiRackStatus_Click(object sender, EventArgs e)
        {
            this.TSMIHiRackStatus_Click(this, e); 
        }

        private void toolStripButtonPalletStatus_Click(object sender, EventArgs e)
        {
            this.TSMIPalletStatus_Click(this, e); 
        }

        private void toolStripButtonSAPMovementIF_Click(object sender, EventArgs e)
        {
            this.TSMISAPMovementIF_Click(this, e); 
        }

        private void toolStripButtonTransferTimeMonitorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.TSMITransferTimeMonitorToolStripMenuItem_Click(this, e);
        }

        private void toolStripButtonMovementMonitroing_Click(object sender, EventArgs e)
        {
            this.TSMIMovementMonitoring_Click(this, e); 
        }

        private void toolStripButtonPalletBlocking_Click(object sender, EventArgs e)
        {
            this.TSMIPalletBlocking_Click(this, e); 
        }

        private void toolStripButtonECSMovement_Click(object sender, EventArgs e)
        {
            this.TSMIECSMovement_Click(this, e); 
        }

        private void toolStripButtonLocalMovement_Click(object sender, EventArgs e)
        {
            this.TSMILocalMovement_Click(this, e); 
        }

        private void toolStripButtonSapMovement_Click(object sender, EventArgs e)
        {
            TSMISapMovement_Click(this, e);
        }


    
    }
}