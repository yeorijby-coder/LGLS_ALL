using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Device.Observe;
using ECP.Global;
using ECP.Db;
using ECP.Device;
using ECP.Interfaces;

using ECP.Global.Device;
using ECP.Com.FieldBus;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Service.Statistics;

namespace ECP.Gui.Config.Dialogs
{
    public partial class MasterObservableCreatePanel : UserControl
    {
        const int COLUMN_OWNER_ID = 0;
        const int COLUMN_OBSERVABLE_ID = 2;

        public string sqlString = null;

        public MasterObservableCreatePanel()
        {
            InitializeComponent();
            LoadMasterObservableData();
        }

        public void LoadMasterObservableData()
        {
            Dictionary<string, Observable> masterObservables = new Dictionary<string, Observable>();

            masterObservables = Observable.GetAllObservablesFromMasterDB();

            ListView.ListViewItemCollection listViewItems = this.listViewMasterObservable.Items;
            listViewItems.Clear();

            if (masterObservables.Count > 0)
            {
                foreach (Observable masterObservable in masterObservables.Values)
                {
                    ListViewItem listViewItem = new ListViewItem(masterObservable.OwnerId);
                    listViewItem.SubItems.Add(ElementType.ValueToString(masterObservable.OwnerType));
                    listViewItem.SubItems.Add(masterObservable.ObservableId);
                    listViewItem.SubItems.Add(masterObservable.ObservableGuid.ToString());
                    listViewItem.SubItems.Add(masterObservable.ObservablePath);
                    listViewItem.SubItems.Add(AccessType.ValueToString(masterObservable.AccessType));
                    listViewItem.SubItems.Add(DataType.ValueToString(masterObservable.DataType));
                    listViewItem.SubItems.Add(masterObservable.Min.ToString());
                    listViewItem.SubItems.Add(masterObservable.Max.ToString());
                    listViewItem.SubItems.Add(masterObservable.DriverNo.ToString());
                    listViewItem.SubItems.Add((masterObservable.IsMonitoring ? "T" : "F"));
                    listViewItem.SubItems.Add(masterObservable.ConnectionInfoString);
                    listViewItem.SubItems.Add(masterObservable.Description);
                    listViewItem.SubItems.Add((masterObservable.LogEnable ? "T" : "F"));
                    listViewItem.SubItems.Add(masterObservable.ValueInformation);
                    listViewItem.SubItems.Add(masterObservable.Remark);
                    listViewItems.Add(listViewItem);
                }
            }
        }

        public void LoadMasterObservableData(Dictionary<string, string> filteringCondition)
        {
            Dictionary<string, Observable> masterObservables = new Dictionary<string, Observable>();
            string sqlStatement = ObservableUtil.MakeObservableFilteringStatement(filteringCondition);

            masterObservables = Observable.GetObservablesBySpecificQueryFromMasterDB(sqlStatement);
            
            ListView.ListViewItemCollection listViewItems = this.listViewMasterObservable.Items;
            listViewItems.Clear();

            if (masterObservables.Count > 0)
            {
                foreach (Observable masterObservable in masterObservables.Values)
                {
                    ListViewItem listViewItem = new ListViewItem(masterObservable.OwnerId);
                    listViewItem.SubItems.Add(ElementType.ValueToString(masterObservable.OwnerType));
                    listViewItem.SubItems.Add(masterObservable.ObservableId);
                    listViewItem.SubItems.Add(masterObservable.ObservableGuid.ToString());
                    listViewItem.SubItems.Add(masterObservable.ObservablePath);
                    listViewItem.SubItems.Add(AccessType.ValueToString(masterObservable.AccessType));
                    listViewItem.SubItems.Add(DataType.ValueToString(masterObservable.DataType));
                    listViewItem.SubItems.Add(masterObservable.Min.ToString());
                    listViewItem.SubItems.Add(masterObservable.Max.ToString());
                    listViewItem.SubItems.Add(masterObservable.DriverNo.ToString());
                    listViewItem.SubItems.Add((masterObservable.IsMonitoring ? "T" : "F"));
                    listViewItem.SubItems.Add(masterObservable.ConnectionInfoString);
                    listViewItem.SubItems.Add(masterObservable.Description);
                    listViewItem.SubItems.Add((masterObservable.LogEnable ? "T" : "F"));
                    listViewItem.SubItems.Add(masterObservable.ValueInformation);
                    listViewItem.SubItems.Add(masterObservable.Remark);
                    listViewItems.Add(listViewItem);
                }
            }
            else
            {
                MessageBox.Show("No exist Data");
            }
        }

        public void ReloadMatserObservableData()
        {
            this.LoadMasterObservableData();
        }

        private void listViewMasterObservable_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Point loc = new Point(Parent.Location.X + e.X, Parent.Location.Y + e.Y);
                this.contextMenuStrip.Show(loc);
            }
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new MasterObservableWizardForm(this).Show();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedListViewItems = this.listViewMasterObservable.SelectedItems;

            if (selectedListViewItems.Count > 0)
            {
                string message = "Are you sure want to delete selected observables ?";

                string caption = "MasterObservable Delete";
                MessageBoxButtons buttons = MessageBoxButtons.OKCancel;
                DialogResult result;

                // Displays the MessageBox.

                result = MessageBox.Show(this, message, caption, buttons);

                if (result == DialogResult.OK)
                {
                    for (int inx = 0; inx < selectedListViewItems.Count; inx++)
                    {
                        Observable masterObservable =
                            new Observable(selectedListViewItems[inx].SubItems[COLUMN_OWNER_ID].Text,
                                                 selectedListViewItems[inx].SubItems[COLUMN_OBSERVABLE_ID].Text);
                        masterObservable.DeleteFromMasterDB();
                    }

                    this.ReloadMatserObservableData();
                }
            }
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ListView.SelectedListViewItemCollection selectedListViewItems = this.listViewMasterObservable.SelectedItems;

            MasterObservableWizardForm form =
                new MasterObservableWizardForm(this,
                selectedListViewItems[0].SubItems[COLUMN_OWNER_ID].Text,
                selectedListViewItems[0].SubItems[COLUMN_OBSERVABLE_ID].Text, true);

            form.Show();
        }

        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.ReloadMatserObservableData();
        }

        private void listViewMasterObservable_DoubleClick(object sender, EventArgs e)
        {
            this.editToolStripMenuItem_Click(sender, e);
        }

        private void filterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new MasterObservableFilterForm(this).Show();
        }
    }
}
