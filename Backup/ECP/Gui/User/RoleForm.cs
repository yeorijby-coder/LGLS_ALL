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
    public partial class RoleForm : Form
    {
        //사용자 권환 문자열 상수
        const string PERMISSION_USER_EDIT = "Client.EditUser";
        const string CLASS = "RoleForm";
        public RoleForm()
        {
            InitializeComponent();
            InitializePermission();
            this.CenterToScreen();

            LoadRoleData();
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
                this.listRoleView.MouseDoubleClick += new MouseEventHandler(this.listRoleView_MouseDoubleClick);
            }
        }

        public void LoadRoleData()
        {
            UserManager manager = Naming.GetUserManager();
            ListView.ListViewItemCollection listViewItems = this.listRoleView.Items;

            RoleData[] data = manager.GetRoleList();

            listViewItems.Clear();

            for (int i = 0; i < data.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(data[i].roleId);
                viewItem.SubItems.Add(data[i].roleDescription);

                listViewItems.Add(viewItem);
            }
        }

        public void ReLoadRoleData()
        {
            LoadRoleData();
        }



        private void listRoleView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (this.listRoleView.SelectedItems.Count > 0)
            {
                AddOrModifyRoleForm addForm =
                    new AddOrModifyRoleForm(this, this.listRoleView.SelectedItems[0].Text,
                    this.listRoleView.SelectedItems[0].SubItems[1].Text, true);
                addForm.Show();
            }

        }

        private void listRoleView_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc =
                    new Point(this.Location.X + e.X, this.Location.Y + e.Y + 20);

                this.contextMenuStrip.Show(loc);
            }
        }

        private void addStripMenuItem_Click(object sender, EventArgs e)
        {
            AddOrModifyRoleForm addForm = new AddOrModifyRoleForm(this);

            addForm.Show();

        }

        private void deleteStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.listRoleView.SelectedItems.Count > 0)
            {
                string message = "Are you sure want to delete selected Role ?";

                string caption = "Role Delete";
                MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                DialogResult result;

                // Displays the MessageBox.

                result = MessageBox.Show(this, message, caption, buttons);

                if (result == DialogResult.OK)
                {
                    Naming.GetUserManager().DeleteRole(this.listRoleView.SelectedItems[0].Text.TrimEnd());

                    this.listRoleView.Items.
                     Remove(this.listRoleView.SelectedItems[0]);
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

        private void RoleForm_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc =
                    new Point(this.Location.X + e.X, this.Location.Y + e.Y + 20);

                this.contextMenuStrip.Show(loc);
            }
        }
    }
}