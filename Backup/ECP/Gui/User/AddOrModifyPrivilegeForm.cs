using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Util.Client;
using ECP.Service.User;

namespace ECP.Gui.User
{
    public partial class AddOrModifyPrivilegeForm : Form
    {
        PrivilegeForm owner = null;

        bool IsModifiedMode = false;

        public AddOrModifyPrivilegeForm()
        {
            InitializeComponent();
            CenterToParent();
            this.privilegeTextBox.Focus();
        }

        public AddOrModifyPrivilegeForm(PrivilegeForm owner)
        {
            InitializeComponent();

            CenterToParent();

            this.owner = owner;
            this.privilegeTextBox.Focus();
        }

        public AddOrModifyPrivilegeForm(PrivilegeForm owner,string permissionId, 
            string description, bool modifiedMode)
        {
            InitializeComponent();

            CenterToParent();

            this.owner = owner;
            this.IsModifiedMode = modifiedMode;

            this.privilegeTextBox.Text = permissionId;
            this.privilegeDescTextBox.Text = description;
            this.privilegeTextBox.Focus();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            this.applyButton_Click(sender, e);
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            UserManager userManager = Naming.GetUserManager();
            bool found = false;

            if (!this.IsModifiedMode)
            {

                string[] permissionList = userManager.GetPermissionIdList();

                for (int i = 0; i < permissionList.Length; i++)
                {
                    if (this.privilegeTextBox.Text.Equals(permissionList[i]))
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    return;
                }
                else
                {
                    userManager.CreatePermission(this.privilegeTextBox.Text,
                        this.privilegeDescTextBox.Text);
                }
            }
            else
            {
                userManager.ModifyPermission(this.privilegeTextBox.Text,
                        this.privilegeDescTextBox.Text);
            }
            owner.ReloadPrivilegeData();
        }
    }
}