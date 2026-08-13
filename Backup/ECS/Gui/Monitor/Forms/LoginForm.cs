using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.Gui.Monitor.Panels;
using ECP.Service.Device;
using ECP.Util.Server;

namespace HECS.Gui.Monitor.Forms
{
    public partial class LoginForm : FormBase
    {
        LoginPanel loginPanel = null;
        public LoginForm()
        {
            InitializeComponent();
            this.panelContainer.BackColor = Color.White;
            this.webBrowser1.Url =new System.Uri (@Application.StartupPath +  @"\main.swf");
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            if (this.panelContainer.Controls.Contains(loginPanel) == false)
            {
                loginPanel = new LoginPanel();
                loginPanel.Name = "LoginPanel";
                loginPanel.Width = 350;
                loginPanel.Height = 190;
                loginPanel.BackColor = Color.White;
                this.panelContainer.Controls.Add(loginPanel);
                loginPanel.OnUserLogin += new UserLoginEventHandler(loginPanel_OnUserLogin);
            }

            if (webBrowser1 == null)
            {
//                loginPanel.SetBounds(this.Width / 2 - (loginPanel.Width / 2), this.Height / 2 - (loginPanel.Height / 2), loginPanel.Width, loginPanel.Height);
                loginPanel.SetBounds(600,250, loginPanel.Width, loginPanel.Height);
            }
            else
            {
//                loginPanel.SetBounds(this.Width / 2 - (loginPanel.Width / 2), this.Height / 2 - (loginPanel.Height / 2), loginPanel.Width, loginPanel.Height);
                loginPanel.SetBounds(600, 250, loginPanel.Width, loginPanel.Height);
            }
            
           

        }

        void loginPanel_OnUserLogin(object sender, UserInfoEventArgs e)
        {
            string id = e.UserId;
            string password = e.Password;

            if (string.IsNullOrEmpty(id) )
            {
                Util.Util.ShowMesagePopup("아이디를 입력하지 않았습니다!");
                return;
            }
            if (string.IsNullOrEmpty(password))
            {
                Util.Util.ShowMesagePopup("비밀번호를 입력하지 않았습니다!");
                return;
            }

            if (id.Equals( "admin") && password.Equals( "admin"))
            {
                this.Hide();
                return;
            }
            loginPanel.ClearFormText();
            Util.Util.ShowMesagePopup("아이디 또는 비밀번호가 일치하지 않습니다.");
            return;
        }
        
        
    }
}