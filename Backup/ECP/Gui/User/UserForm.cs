using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Util.Client;
using ECP.Service.User;
using ECP.Global.Service;
using ECP.Service.User.PermissionTarget;

namespace ECP.Gui.User
{
    public partial class UserForm : Form
    {
        //사용자 권환 문자열 상수
        const string PERMISSION_USER_EDIT = "Client.EditUser";
        const string CLASS = "UserForm";
        public UserForm()
        {
            InitializeComponent();
            InitializePermission();
            this.CenterToScreen();

            LoadUserData();
        }

        private void InitializePermission()
        {
            PermissionTarget target;
            target = new TargetTypeConstructor();

            List<IPermission> targets = new List<IPermission>();
            targets.Add(target.CreateType(PERMISSION_USER_EDIT, this.contextMenuStrip));

            UserPermission.AddTarget(CLASS,targets);
            UserPermission.GetInstance().DisableFuncationByUserPermission();

            if (UserPermission.GetInstance().HasPermission(PERMISSION_USER_EDIT))
            {
                this.listUserView.MouseDoubleClick += new MouseEventHandler(this.listUserView_MouseDoubleClick);
            }
        }

        public void LoadUserData()
        {
            UserManager manager = Naming.GetUserManager();
            ListView.ListViewItemCollection listViewItems = this.listUserView.Items;

            UserData[] data = manager.GetUserList();

            listViewItems.Clear();

            for (int i = 0; i < data.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(data[i].userId);
                viewItem.SubItems.Add(data[i].userName);
                viewItem.SubItems.Add(data[i].userDescription);

                listViewItems.Add(viewItem);
            }
        }

        public void ReLoadUserData()
        {
            LoadUserData();
        }



        private void addStripMenuItem_Click(object sender, EventArgs e)
        {
            AddOrModifyUserForm addForm = new AddOrModifyUserForm(this);
            addForm.Show();
        }

        private void deleteStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.listUserView.SelectedItems.Count > 0)
            {
                string message = "Are you sure want to delete selected User ?";

                string caption = "User Delete";
                MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                DialogResult result;


                if(this.listUserView.SelectedItems[0].Text.TrimEnd().Equals("Administrator"))
                {
                    MessageBox.Show("Administrator can not be deleted");
                    return;
                }
                else
                {
                    // Displays the MessageBox.

                    result = MessageBox.Show(this, message, caption, buttons);
                    if (result == DialogResult.OK)
                    {
                        Naming.GetUserManager().DeleteUser(this.listUserView.SelectedItems[0].Text.TrimEnd());

                        this.listUserView.Items.
                         Remove(this.listUserView.SelectedItems[0]);
                    }
                    else if (result == DialogResult.Cancel) /// we delete widget but not from system
                    {
                        // do nothing
                        return;
                    }
                }
            }

        }

        private void closeStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void listUserView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc =
                    new Point(this.Location.X + e.X, this.Location.Y + e.Y + 20);

                this.contextMenuStrip.Show(loc);
            }
        }

        private void listUserView_MouseDoubleClick(object sender, MouseEventArgs e)
        {


            if (this.listUserView.SelectedItems.Count > 0)
            {
                AddOrModifyUserForm addForm =
                    new AddOrModifyUserForm(this, this.listUserView.SelectedItems[0].Text,
                    this.listUserView.SelectedItems[0].SubItems[1].Text, this.listUserView.SelectedItems[0].SubItems[2].Text, true);
                addForm.Show();
            }

        }
    }
}