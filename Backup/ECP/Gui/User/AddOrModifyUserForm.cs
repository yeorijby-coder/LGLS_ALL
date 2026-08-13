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

using ECP.Util;

namespace ECP.Gui.User
{
    public partial class AddOrModifyUserForm : Form
    {

        UserForm parent=null;

        string originalPassworld = null;

        bool IsModifiedMode = false;

        ListView lastChoosedListView = null;

        public AddOrModifyUserForm()
        {
            InitializeComponent();
            this.CenterToScreen();
        }


        public AddOrModifyUserForm(UserForm parent)
        {
            InitializeComponent();
            this.CenterToParent();

            this.parent = parent;

            LoadAllRoleList();
        }

        public AddOrModifyUserForm(UserForm parent, string userId, string userName,
            string description, bool modify)
        {
            InitializeComponent();
            this.CenterToParent();

            this.parent = parent;

            this.IsModifiedMode = modify;

            this.textBoxUserID.Text = userId;
            this.textBoxUserName.Text = userName;
            this.textBoxDesc.Text = description;

            ECP.Service.User.Person user = Naming.GetUserManager().GetUser(userId);

            this.maskedTextBoxPassworld.Text = user.ThePassword;
            this.maskedTextBoxPasswordConfirm.Text = user.ThePassword;

            this.originalPassworld = user.ThePassword;

            if (user.ThePasswordMaxAge == -1)
            {
                this.checkBoxPassworldExpire.Checked = true;
                this.numericUpDownPassworldDuration.Enabled = false;
            }
            else
            {
                this.checkBoxPassworldExpire.Checked = false;
                this.numericUpDownPassworldDuration.Value = Convert.ToDecimal(user.ThePasswordMaxAge);
            }

            if (modify == true)
            {
                this.textBoxUserID.Enabled = false;
               // this.labelUserID.Visible = false;
            }
            else
            {
                this.textBoxUserID.Visible = true;
                this.labelUserID.Visible = true;

            }


      
            LoadAvailableRoleList(userId);
            LoadAssignedRoleList(userId);

        }

        public void LoadAssignedRoleList(string userId)
        {
            UserManager manager = Naming.GetUserManager();

            ListView.ListViewItemCollection listViewItems = 
                this.assignedListView.Items;

            ECP.Service.User.Person user = manager.GetUser(userId);

            string[] assignedRoleList = user.TheRoleIdList;

            listViewItems.Clear();

            for (int i = 0; i < assignedRoleList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(assignedRoleList[i]);

                listViewItems.Add(viewItem);
            }

        }

        public void LoadAvailableRoleList(string userId)
        {
            UserManager manager = Naming.GetUserManager();

            ListView.ListViewItemCollection listViewItems = this.availableListView.Items;

            ECP.Service.User.Person user = manager.GetUser(userId);
            string[] roleIdList = user.GetAvailableRoleIdList();


            listViewItems.Clear();

            for (int i = 0; i < roleIdList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(roleIdList[i]);

                listViewItems.Add(viewItem);
            }

        }

        public void LoadAllRoleList()
        {
            UserManager manager = Naming.GetUserManager();


            ListView.ListViewItemCollection listViewItems = this.availableListView.Items;

            string[] roleIdList = manager.GetRoleIdList();

            listViewItems.Clear();

            for (int i = 0; i < roleIdList.Length; i++)
            {
                ListViewItem viewItem = new ListViewItem(roleIdList[i]);

                listViewItems.Add(viewItem);
            }
        }

        private void leftButton_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedItems =
                this.availableListView.SelectedItems;


            foreach (ListViewItem item in selectedItems)
            {
                this.availableListView.Items.Remove(item);
                this.assignedListView.Items.Add(item);
            }
        }

        private void rightButton_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedItems =
               this.assignedListView.SelectedItems;

            foreach (ListViewItem item in selectedItems)
            {
                this.assignedListView.Items.Remove(item);
                this.availableListView.Items.Add(item);
            }
        }

        private void upButton_Click(object sender, EventArgs e)
        {
            ListView selectedView = lastChoosedListView;

            if (selectedView.SelectedItems.Count < 1)
            {
                return;
            }

            foreach (ListViewItem item in selectedView.SelectedItems)
            {
                int index = item.Index;

                if (item.Index != 0)
                {
                    selectedView.Items.Remove(item);
                    selectedView.Items.Insert(index - 1, item);
                }
            }
        }

        private void assignedListView_MouseUp(object sender, MouseEventArgs e)
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

                if (item.Index < selectedView.Items.Count - 1)
                {
                    selectedView.Items.Remove(item);
                    selectedView.Items.Insert(index + 1, item);
                }
            }

        }

        private void okButton_Click(object sender, EventArgs e)
        {
            UserManager userManager = Naming.GetUserManager();

            bool found = false;

            if (!maskedTextBoxPassworld.Text.Equals(maskedTextBoxPasswordConfirm.Text))
            {
                MessageBox.Show("Password Cofirm Failed");
                return;
            }


            if (!this.IsModifiedMode)
            {

                string[] userIdList = userManager.GetUserIds();

                for (int i = 0; i < userIdList.Length; i++)
                {
                    if (this.textBoxUserID.Text.Equals(userIdList[i]))
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    MessageBox.Show("Same User Id Exist");
                    return;
                }
                else
                {
                    int passwordAge = -1;


                    if(checkBoxPassworldExpire.Checked == true)
                    {
                        passwordAge = -1;
                    }
                    else
                    {
                        passwordAge = Convert.ToInt32( numericUpDownPassworldDuration.Value);

                    }

                    string encodedPassworld =  CryptorEngine.Encrypt(this.maskedTextBoxPassworld.Text,true);


                    ECP.Service.User.Person user = userManager.CreateUser(this.textBoxUserID.Text,
                       encodedPassworld, this.textBoxUserName.Text,
                       this.textBoxDesc.Text,passwordAge, checkBoxUserDiable.Checked);


                    string[] assignedRoleList = new string[this.assignedListView.Items.Count];

                    int i = 0;
                    foreach (ListViewItem item in this.assignedListView.Items)
                    {
                        assignedRoleList[i] = item.Text;
                        i++;
                    }


                    user.SetInformation(new UserData(user.TheUserId, encodedPassworld, user.TheUserName,
                        user.TheUserDrescription, user.ThePasswordCreateTime,user.ThePasswordMaxAge), assignedRoleList);
                }
            }
            else
            {
                int passwordAge = -1;
                string encodedPassworld = null;


                if(checkBoxPassworldExpire.Checked == true)
                {
                    passwordAge = -1;
                }
                else
                {
                    passwordAge = Convert.ToInt32( numericUpDownPassworldDuration.Value);

                }

                string[] assignedRoleList = new string[this.assignedListView.Items.Count];

                int i = 0;
                foreach (ListViewItem item in this.assignedListView.Items)
                {
                    assignedRoleList[i] = item.Text;
                    i++;
                }

                if (!this.originalPassworld.Equals(this.maskedTextBoxPassworld.Text))
                {
                    encodedPassworld = CryptorEngine.Encrypt(this.maskedTextBoxPassworld.Text, true);
                }
                else
                {
                    encodedPassworld = originalPassworld;
                }

                ECP.Service.User.Person user = userManager.GetUser(this.textBoxUserID.Text);

                user.SetInformation(new UserData(user.TheUserId,
                    encodedPassworld,
                    textBoxUserName.Text,textBoxDesc.Text,user.ThePasswordCreateTime, passwordAge), 
                        assignedRoleList);
            }

            parent.ReLoadUserData();
            this.Close();
        }

        private void cancleButton_Click(object sender, EventArgs e)
        {
            parent.ReLoadUserData();
            this.Close();
        }

        private void checkBoxPassworldExpire_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBoxPassworldExpire.Checked == true)
            {
                this.numericUpDownPassworldDuration.Enabled = false;
            }
            else
            {
                this.numericUpDownPassworldDuration.Enabled = true;
            }
        }
    }
}