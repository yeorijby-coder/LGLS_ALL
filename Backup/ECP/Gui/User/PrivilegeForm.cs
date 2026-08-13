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
    public partial class PrivilegeForm : Form
    {  
        //사용자 권환 문자열 상수
        const string PERMISSION_USER_EDIT = "Client.EditUser";
        const string CLASS = "PrivilegeForm";

        public PrivilegeForm()
        {
            InitializeComponent();
            InitializePermission();
            this.CenterToScreen();

            LoadPrivilegeData();

            this.DisableFuncationalityBaseOneUserPrivilege();
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
                this.listPrvilegeView.MouseDoubleClick += new MouseEventHandler(this.listPrvilegeView_MouseDoubleClick);
            }
        }

        public void LoadPrivilegeData()
        {
            UserManager manager = Naming.GetUserManager();
            ListView.ListViewItemCollection listViewItems = this.listPrvilegeView.Items;

            PermissionData[] data = manager.GetPermissionList();

            listViewItems.Clear();

            for (int i = 0; i < data.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(data[i].permissionId);
                viewItem.SubItems.Add(data[i].permissionDescription);

                listViewItems.Add(viewItem);
            }
        }

        public void ReloadPrivilegeData()
        {
            LoadPrivilegeData();
        }

        private void DisableFuncationalityBaseOneUserPrivilege()
        {
            if (!UserPermission.GetInstance().HasPermission(PERMISSION_USER_EDIT))
            {
                this.contextMenuStrip.Visible = false;
            }
        }

        private void listPrvilegeView_MouseUp(object sender, MouseEventArgs e)        
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc =
                    new Point(this.Location.X+e.X, this.Location.Y+e.Y+20);

                this.contextMenuStrip.Show(loc);
            }
        }

        private void deleteStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.listPrvilegeView.SelectedItems.Count > 0)
            {
                string message = "Are you sure want to delete selected privilege ?";

                string caption = "Prvilege Delete";
                MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                DialogResult result;

                // Displays the MessageBox.
                
                result = MessageBox.Show(this, message, caption, buttons);

                if (result == DialogResult.OK)
                {
                    Naming.GetUserManager().DeletePermission(this.listPrvilegeView.SelectedItems[0].Text.TrimEnd());

                    this.listPrvilegeView.Items.
                     Remove(this.listPrvilegeView.SelectedItems[0]);
                }
                else if (result == DialogResult.Cancel) /// we delete widget but not from system
                {
                    // do nothing
                    return;
                }
            }
        }

        private void closeStripMenuItem_Click(object sender, EventArgs e)
        {
            this.contextMenuStrip.Hide();
        }

        private void addStripMenuItem_Click(object sender, EventArgs e)
        {
            AddOrModifyPrivilegeForm addForm =  new AddOrModifyPrivilegeForm(this);
            addForm.Show();
        }

        private void listPrvilegeView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (this.listPrvilegeView.SelectedItems.Count > 0)
            {
                AddOrModifyPrivilegeForm addForm =
                    new AddOrModifyPrivilegeForm(this,this.listPrvilegeView.SelectedItems[0].Text,
                    this.listPrvilegeView.SelectedItems[0].SubItems[1].Text,true);
                addForm.Show();
            }

        }
    }
}