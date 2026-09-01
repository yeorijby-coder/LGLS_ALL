using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using System.Web.UI;
using ECP.Service.Statistics;

using ECP.Service.User;
using ECP.Util.Client;
using HECS.Util;


namespace HECS.Gui.Monitor.Popups
{

    public partial class DialogBaseForm : Form
    {

        //ReleaseCapture();
        //SendMessage(Handle, WM_NCLBUTTONDOWN, HT_CAPTION, 0);
        [DllImport("user32.dll")]
        extern public static bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, int uFlags);
        private bool isDirty = false;
        public const int HWND_TOPMOST = -1; // 0xffff 
        public const int SWP_NOSIZE = 1; // 0x0001 
        public const int SWP_NOMOVE = 2; // 0x0002 
        public const int SWP_NOACTIVATE = 16; // 0x0010 
        public const int SWP_SHOWWINDOW = 64; // 0x0040 
        public DialogBaseForm()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetTopLevel(true);
            this.ShowIcon = true;
            this.ResumeLayout();
            panelBack.MouseDown += new MouseEventHandler(panelBack_MouseDown);
            this.Shown += new EventHandler(DialogBaseForm_Shown);
            this.VisibleChanged += new EventHandler(DialogBaseForm_VisibleChanged);
            this.Shown +=new EventHandler(DialogBaseForm_Shown);
            this.Activated += new EventHandler(DialogBaseForm_Activated);
        }

        void DialogBaseForm_Activated(object sender, EventArgs e)
        {
            if (isDirty == true)
            {
                this.WindowState = FormWindowState.Normal;
            }
            else
            {
                isDirty = true;
            }
        }

        void DialogBaseForm_VisibleChanged(object sender, EventArgs e)
        {
            // [LGLS 2026-09-02] 숨길 때도 TopMost/Focus 를 걸던 것을 표시될 때만으로 제한
            if (!this.Visible)
            {
                return;
            }
            this.TopMost = true;
            this.BringToFront();
            this.Focus();
        }

        void DialogBaseForm_Shown(object sender, EventArgs e)
        {
   
        }






        void panelBack_MouseDown(object sender, MouseEventArgs e)
        {
            Win32API.ReleaseCapture();
            Win32API.SendMessage(Handle, Win32API.WM_NCLBUTTONDOWN, Win32API.HT_CAPTION, 0);
        }

        [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
        protected override void WndProc(ref Message m)
        {
            if (m.Msg == 0x0084) //WM_NCHITTEST
            {
                base.WndProc(ref m);
                if (m.Result == (IntPtr)1) //HTCLIENT. User clicks on the window client area
                {
                    m.Result = (IntPtr)2; //HTCAPTION. Now it's caption 
                }
            }
            else
            {
                base.WndProc(ref m);
            }
        }


        //protected override void OnShown(System.EventArgs e)
        //{
        //    base.OnShown(e);
        //    if (this.TopMost == false)
        //    {
        //        this.TopMost = true;
        //    }
        //    if (this.WindowState == System.Windows.Forms.FormWindowState.Minimized)
        //    {
        //        this.WindowState = System.Windows.Forms.FormWindowState.Normal;
        //    }
        //}
        //protected override void OnActivated(EventArgs e)
        //{
        //    base.OnActivated(e);
        //    if (this.WindowState == System.Windows.Forms.FormWindowState.Minimized)
        //    {
        //        this.WindowState = System.Windows.Forms.FormWindowState.Normal;
        //    }
        //    if (this.TopMost == false)
        //    {
        //        this.TopMost = true;
        //    }

        //}
        private void DialogBaseForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true; //don't close
            // [LGLS 2026-09-02] 종전에는 X 를 눌러도 닫기만 취소하고 아무 동작이 없어
            //   "닫기가 안 된다/느리다" 로 보였다 - 확인 버튼과 같게 숨긴다(폼 캐시는 유지).
            if (e.CloseReason == CloseReason.UserClosing)
            {
                this.Hide();
            }
        }


        public void Popup()
        {
            try
            {
                this.Invoke(new delegateShow(Show));
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private delegate void delegateShow();


        //new public void Show()
        //{
        //    ShowWindowTopMost(this.Handle);
        //}



    }
}