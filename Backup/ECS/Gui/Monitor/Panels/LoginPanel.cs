using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Panels
{

    public delegate void UserLoginEventHandler(object sender, UserInfoEventArgs e);
  
    public partial class LoginPanel : UserControl
    {
        public event UserLoginEventHandler OnUserLogin;
        public LoginPanel()
        {
            InitializeComponent();
        }

        private delegate void InvokeClearText();
        private void ClearText()
        {
            this.textBoxPassword.Text = "";
            this.textBoxUserID.Text = "";
        }

        public void ClearFormText()
        {
            this.BeginInvoke(new InvokeClearText(ClearText));
        }

        private delegate void InvokRefreshForm();
        private void RefreshForm()
        {
            if (textBoxUserID.Focused)
            {
                textBoxUserID.BackColor = Color.PowderBlue;
                textBoxUserID.SelectAll();
            }
            else
            {
                textBoxUserID.BackColor = Color.White;

            }
            if (textBoxPassword.Focused)
            {
                textBoxPassword.BackColor = Color.PowderBlue;
                textBoxPassword.SelectAll();
            }
            else
            {
                textBoxPassword.BackColor = Color.White;
            }
        }

        private delegate void InvokeKeyEnter(TextBox textBox);
        private void KeyEnter(TextBox textBox)
        {

            if (textBox.Equals(this.textBoxUserID))
            {
                if (textBox.Text.Trim() == "")
                {
                    this.textBoxUserID.Focus();
                    return;
                }
                else
                {
                    this.textBoxPassword.Focus();
                }
            }
            else if (textBox.Equals(this.textBoxPassword))
            {
                if (textBox.Text.Trim() == "")
                {
                    this.textBoxPassword.Focus();
                    return;
                }
            }

            if (this.textBoxUserID.Text.Trim() == "" || this.textBoxPassword.Text.Trim() == "")
            {
                return;
            }
            else
            {
                buttonLogin_Click(this, null);
            }

        }

        private void textBoxUserID_Enter(object sender, EventArgs e)
        {
            BeginInvoke(new InvokRefreshForm(RefreshForm));
        }

        private void textBoxUserID_Leave(object sender, EventArgs e)
        {
            BeginInvoke(new InvokRefreshForm(RefreshForm));
        }

        private void textBoxUserID_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode != Keys.Enter)
            {
                return;
            }
            TextBox textBox = sender as TextBox;
            if (textBox == null)
            {
                return;
            }
            object[] obj = new object[1];
            obj[0] = textBox;

            BeginInvoke(new InvokeKeyEnter(KeyEnter), obj);
        }

        private void buttonLogin_Click(object sender, EventArgs e)
        {
            if (OnUserLogin == null)
            {
                return;
            }
            OnUserLogin(this, new UserInfoEventArgs(this.textBoxUserID.Text, this.textBoxPassword.Text));
        }


    }
    public class UserInfoEventArgs : EventArgs
    {

        public UserInfoEventArgs(string id, string passwd)
        {
            this.UserId = id;
            this.password = passwd;
        }
        private string userId = "";
        public string UserId
        {
            get
            {
                return userId;
            }
            set
            {
                userId = value;
            }
        }
        private string password = "";
        public string Password
        {
            get
            {
                return password;
            }
            set
            {
                password = value;
            }
        }

    }
}
