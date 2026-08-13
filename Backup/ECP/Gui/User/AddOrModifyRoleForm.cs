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
    public partial class AddOrModifyRoleForm : Form
    {
        RoleForm parent = null;

        bool IsModifiedMode = false;


        ListView lastChoosedListView = null;

        public AddOrModifyRoleForm()
        {
            InitializeComponent();
            this.CenterToParent();
        }
        public AddOrModifyRoleForm(RoleForm parent)
        {
            InitializeComponent();
            this.CenterToParent();

            this.parent = parent;

            LoadAllPermissionList();
        }
       

        public AddOrModifyRoleForm(RoleForm parent, string roleId, string description, bool modify)
        {
            InitializeComponent();
            this.CenterToParent();

            this.parent = parent;

            this.IsModifiedMode = modify;
            this.roleIDTextBox.Text = roleId;
            this.roleDescTextBox.Text  = description;

            LoadAvailablePermissionList(roleId);
            LoadAssignedPermissionList(roleId);


        }

        public void LoadAssignedPermissionList(string roleId)
        {
            UserManager manager = Naming.GetUserManager();
                   
            ListView.ListViewItemCollection listViewItems = this.assignedPermListView.Items;

            string[] permissionIdList = manager.GetPermissionIdListByRole(roleId);

            listViewItems.Clear();

            for (int i = 0; i < permissionIdList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(permissionIdList[i]);

                listViewItems.Add(viewItem);
            }

        }

        public void LoadAllPermissionList()
        {
            UserManager manager = Naming.GetUserManager();
        

            ListView.ListViewItemCollection listViewItems = this.availablePermissionListView.Items;

            string [] permissionList = manager.GetPermissionIdList();

            listViewItems.Clear();

            for (int i = 0; i < permissionList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(permissionList[i]);

                listViewItems.Add(viewItem);
            }

        }

        public void LoadAvailablePermissionList(string roleId)
        {
            UserManager manager = Naming.GetUserManager();

            ListView.ListViewItemCollection listViewItems = this.availablePermissionListView.Items;

            string[] permissionList = manager.GetAvailablePermissionIdByRole(roleId);

            listViewItems.Clear();

            for (int i = 0; i < permissionList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(permissionList[i]);

                listViewItems.Add(viewItem);
            }

        }

        private void okButton_Click(object sender, EventArgs e)
        {
            UserManager userManager = Naming.GetUserManager();
            bool found = false;

            if (!this.IsModifiedMode)
            {

                string[] roleIdList = userManager.GetRoleIdList();

                for (int i = 0; i < roleIdList.Length; i++)
                {
                    if (this.roleIDTextBox.Text.Equals(roleIdList[i]))
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    MessageBox.Show("Same Role Id Exist");
                    return;
                }
                else
                {
                    userManager.CreateRole(this.roleIDTextBox.Text, this.roleDescTextBox.Text);

                    string [] assignedPermList = new string[this.assignedPermListView.Items.Count];

                    int i =0;
                    foreach(ListViewItem item in this.assignedPermListView.Items)
                    {
                        assignedPermList[i] = item.Text;
                        i++;
                    }
                    userManager.SetRolePermissionIdList(this.roleIDTextBox.Text,
                        assignedPermList);
                    
                }
            }
            else
            {
                string [] assignedPermList = new string[this.assignedPermListView.Items.Count];

                int i =0;
                foreach(ListViewItem item in this.assignedPermListView.Items)
                {
                    assignedPermList[i] = item.Text;
                    i++;
                }


                userManager.ModifyRole(this.roleIDTextBox.Text, this.roleDescTextBox.Text);
                userManager.SetRolePermissionIdList(this.roleIDTextBox.Text, assignedPermList);

            }

            parent.ReLoadRoleData();
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            parent.ReLoadRoleData();
            this.Close();
        }
        

        private void leftButton_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedItems = 
                this.availablePermissionListView.SelectedItems;
          

            foreach (ListViewItem item in selectedItems)
            {
                this.availablePermissionListView.Items.Remove(item);
                this.assignedPermListView.Items.Add(item);                
            }
        }

        private void rightButton_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedItems =
                this.assignedPermListView.SelectedItems;

            foreach (ListViewItem item in selectedItems)
            {
                this.assignedPermListView.Items.Remove(item);
                this.availablePermissionListView.Items.Add(item);
            }
        }

        private void upButton_Click(object sender, EventArgs e)
        {

            ListView selectedView = lastChoosedListView;

            if (selectedView.SelectedItems.Count < 1)
            {
                return;
            }

            foreach(ListViewItem item in selectedView.SelectedItems)
            {
                int index = item.Index;

                if(item.Index != 0)
                {
                    selectedView.Items.Remove(item);
                    selectedView.Items.Insert(index-1,item);
                }
            }
        }

        private void assignedPermListView_MouseUp(object sender, MouseEventArgs e)
        {
            if (sender is ListView)
            {
                this.lastChoosedListView = (ListView)sender;
            }
        }

        private void downButton_Click(object sender, EventArgs e)
        {
            ListView selectedView = lastChoosedListView;

            if (selectedView.SelectedItems.Count < 1)
            {
                return;
            }

            foreach (ListViewItem item in selectedView.SelectedItems)
            {
                int index = item.Index;

                if (item.Index < selectedView.Items.Count-1)
                {
                    selectedView.Items.Remove(item);
                    selectedView.Items.Insert(index+1, item);
                }
            }

        }
    }
}