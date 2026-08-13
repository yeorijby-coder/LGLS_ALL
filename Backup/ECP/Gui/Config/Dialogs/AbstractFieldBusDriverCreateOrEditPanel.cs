using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Com.FieldBus;
using ECP.Global.Com;
using ECP.Global.Device;
using ECP.Service.User.PermissionTarget;
using ECP.Service.User;
using ECP.Util.Client;

namespace ECP.Gui.Config.Dialogs
{
    public partial class AbstractFieldBusDriverCreateOrEditPanel : UserControl
    {
        //사용자 권한 문자열 상수
        const string PERMISSION_FBDRIVER_EDIT = "Client.EditFBDriver";
        const string CLASS = "AbstractFieldBusDriverCreateOrEditPanel";

        AbstractFieldBusDriverCreateOrEditForm parent = null;

        const int COLUMN_DRIVER_NO = 0;
        const int COLUMN_OWNER_ID = 2;

        public AbstractFieldBusDriverCreateOrEditPanel()
        {
            InitializeComponent();
            InitializePermission();

            this.LoadAbstractFieldBusDriverData();
        }

        public AbstractFieldBusDriverCreateOrEditPanel(AbstractFieldBusDriverCreateOrEditForm parent)
        {
            InitializeComponent();
            InitializePermission();

            this.LoadAbstractFieldBusDriverData();
            this.parent = parent;
        }

        private void InitializePermission()
        {
            PermissionTarget target;
            target = new TargetTypeConstructor();

            List<IPermission> targets = new List<IPermission>();
            targets.Add(target.CreateType(PERMISSION_FBDRIVER_EDIT, this.contextMenuStrip));

            UserPermission.AddTarget(CLASS, targets);
            UserPermission.GetInstance().DisableFuncationByUserPermission();

            if (UserPermission.GetInstance().HasPermission(PERMISSION_FBDRIVER_EDIT))
            {
                this.listViewAbstractFieldBusDriver.DoubleClick += new EventHandler(this.listViewAbstractFieldBusDriver_DoubleClick);
            }
        }

        public void LoadAbstractFieldBusDriverData()
        {
            Dictionary<int, AbstractFieldBusDriver> fieldBusDrivers =
                new Dictionary<int, AbstractFieldBusDriver>();

            fieldBusDrivers = AbstractFieldBusDriver.LoadAbstractFieldBusDrivers();

            ListView.ListViewItemCollection listViewItems = this.listViewAbstractFieldBusDriver.Items;
            listViewItems.Clear();

            if (fieldBusDrivers.Count > 0)
            {
                foreach (AbstractFieldBusDriver fieldBusDriver in fieldBusDrivers.Values)
                {
                    ListViewItem listViewItem = new ListViewItem(fieldBusDriver.DriverNo.ToString());
                    listViewItem.SubItems.Add(FieldBusDriverType.ValueToString(fieldBusDriver.FieldBusDriverType));
                    listViewItem.SubItems.Add(fieldBusDriver.OwnerId);
                    listViewItem.SubItems.Add(ElementType.ValueToString(fieldBusDriver.OwnerType));
                    listViewItem.SubItems.Add(fieldBusDriver.OpenRetrySec.ToString());
                    listViewItem.SubItems.Add(fieldBusDriver.TimeOutSec.ToString());
                    listViewItem.SubItems.Add(fieldBusDriver.ConnectionInfoString);
                    listViewItem.SubItems.Add(fieldBusDriver.ScanInterval.ToString());
                    listViewItem.SubItems.Add(fieldBusDriver.UseFlag ? "T" : "F");
                    listViewItem.SubItems.Add(fieldBusDriver.Description);
                    listViewItem.SubItems.Add(fieldBusDriver.assemblyName);
                    listViewItem.SubItems.Add(fieldBusDriver.className);

                    listViewItems.Add(listViewItem);
                }
            }
        }

        public void ReloadAbstracFieldBusDriverData()
        {
            LoadAbstractFieldBusDriverData();
        }

        private void listViewAbstractFieldBusDriver_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc = new Point(this.parent.Location.X + e.X, this.parent.Location.Y + this.Location.Y + e.Y);
                this.contextMenuStrip.Show(loc);
            }
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new AbstractFieldBusDriverWizardForm(this).Show();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedListViewItems = 
                this.listViewAbstractFieldBusDriver.SelectedItems;

            if (selectedListViewItems.Count > 0)
            {
                string message = "Are you sure want to delete selected fieldbus drivers ?";

                string caption = "AbstractFieldBusDriver Delete";

                MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                DialogResult result;

                // Displays the MessageBox.

                result = MessageBox.Show(this, message, caption, buttons);

                if (result == DialogResult.OK)
                {
                    for (int inx = 0; inx < selectedListViewItems.Count; inx++)
                    {
                        AbstractFieldBusDriver fbDriver =
                            new AbstractFieldBusDriver(
                            Convert.ToInt32(selectedListViewItems[inx].SubItems[COLUMN_DRIVER_NO].Text),
                            selectedListViewItems[inx].SubItems[COLUMN_OWNER_ID].Text);

                        fbDriver.Delete();
                    }

                    this.ReloadAbstracFieldBusDriverData();
                }
            }
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedListViewItems =
                this.listViewAbstractFieldBusDriver.SelectedItems;

            if (selectedListViewItems.Count < 1)
                return;
            else
            {
                new AbstractFieldBusDriverWizardForm(this,
                    Convert.ToInt32(selectedListViewItems[0].SubItems[COLUMN_DRIVER_NO].Text),
                    selectedListViewItems[0].SubItems[COLUMN_OWNER_ID].Text, true).Show();
            }
        }

        private void selectDriverStripMenuItem_Click(object sender, EventArgs e)
        {
            new FieldBusDriverSelectForm(this).Show();
        }

        private void refreshlStripMenuItem_Click(object sender, EventArgs e)
        {
            this.ReloadAbstracFieldBusDriverData();
        }

        private void listViewAbstractFieldBusDriver_DoubleClick(object sender, EventArgs e)
        {
            this.editToolStripMenuItem_Click(sender, e);
        }
    }
}
