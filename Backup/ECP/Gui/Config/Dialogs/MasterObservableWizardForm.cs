using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Device.Observe;
using ECP.Global.Device;
using ECP.Global;
using ECP.Util;

namespace ECP.Gui.Config.Dialogs
{
    public partial class MasterObservableWizardForm : ECP.Gui.Config.Dialogs.ObservableWizardForm
    {
        MasterObservableCreatePanel parent = null;
        bool isModifiedMode = false;

        public MasterObservableWizardForm(MasterObservableCreatePanel parent)
        {
            InitializeComponent();
            this.CenterToParent();
            this.parent = parent;
        }

        public MasterObservableWizardForm(MasterObservableCreatePanel parent, string ownerId, string observableId, bool modified)
        {
            InitializeComponent();
            this.CenterToParent();
            this.parent = parent;

            if (modified)
            {
                this.isModifiedMode = modified;
                this.LoadObservableData(ownerId, observableId);
            }
        }

        protected override void LoadObservableData(string ownerId, string observableId)
        {
            base.LoadObservableData(ownerId, observableId);

            Observable masterObservable = Observable.GetObservableFromMasterDB(ownerId, observableId);

            this.textBoxDescription.Text = masterObservable.Description;
            this.textBoxDriverNo.Text = masterObservable.DriverNo.ToString();
            this.textBoxMax.Text = masterObservable.Max.ToString();
            this.textBoxMin.Text = masterObservable.Min.ToString();
            this.textBoxObservableID.Text = masterObservable.ObservableId;
            this.textBoxObservablePath.Text = masterObservable.ObservablePath;
            this.textBoxOwnerID.Text = masterObservable.OwnerId;

            this.checkBoxLogEnable.Checked = masterObservable.LogEnable;
            this.checkBoxMonFlag.Checked = masterObservable.IsMonitoring;

            this.comboBoxAccessType.SelectedItem = AccessType.ValueToString(masterObservable.AccessType);
            this.comboBoxDataType.SelectedItem = DataType.ValueToString(masterObservable.DataType);
            this.comboBoxOwnerType.SelectedItem = ElementType.ValueToString(masterObservable.OwnerType);

            if (masterObservable.ConnectionInfo.ContainsKey("DEVICE_TYPE"))
            {
                this.comboBoxDeviceType.SelectedItem = (string)masterObservable.ConnectionInfo["DEVICE_TYPE"];
            }

            if (masterObservable.ConnectionInfo.ContainsKey("ADDRESS_NO"))
            {
                this.textBoxAddressNo.Text = (string)masterObservable.ConnectionInfo["ADDRESS_NO"];
            }

            if (masterObservable.ConnectionInfo.ContainsKey("BIT_INDEX"))
            {
                this.textBoxBitIndex.Text = (string)masterObservable.ConnectionInfo["BIT_INDEX"];
            }

            if (masterObservable.ConnectionInfo.ContainsKey("BIT_LENGTH"))
            {
                this.textBoxLength.Text = (string)masterObservable.ConnectionInfo["BIT_LENGTH"];
            }

            this.textBoxOwnerID.Enabled = false;
            this.textBoxObservableID.Enabled = false;
            this.textBoxObservablePath.Enabled = false;
        }

        protected override void Apply()
        {
            //base.Apply();

            if (!Helper.IsNullOrEmpty(this.textBoxOwnerID.Text) &&
                !Helper.IsNullOrEmpty(this.textBoxObservableID.Text) &&
                !Helper.IsNullOrEmpty(this.textBoxObservablePath.Text))
            {
                Observable masterObservable = 
                    new Observable(this.textBoxOwnerID.Text, this.textBoxObservableID.Text);
                
                masterObservable.AccessType = AccessType.StringToValue((string)this.comboBoxAccessType.SelectedItem);
                masterObservable.OwnerType = ElementType.StringToValue((string)this.comboBoxOwnerType.SelectedItem);
                masterObservable.DataType = DataType.StringToValue((string)this.comboBoxDataType.SelectedItem);
                masterObservable.ConnectionInfoString = "DEVICE_TYPE=" + this.comboBoxDeviceType.SelectedItem;

                masterObservable.ObservablePath = this.textBoxObservablePath.Text;
                masterObservable.Max = Convert.ToInt32(this.textBoxMax.Text);
                masterObservable.Min = Convert.ToInt32(this.textBoxMin.Text);
                masterObservable.DriverNo = Convert.ToInt32(this.textBoxDriverNo.Text);
                masterObservable.Description = this.textBoxDescription.Text;

                masterObservable.LogEnable = this.checkBoxLogEnable.Checked;
                masterObservable.IsMonitoring = this.checkBoxMonFlag.Checked;

                if (this.textBoxAddressNo.Text.Trim() != "")
                {
                    masterObservable.ConnectionInfoString = masterObservable.ConnectionInfoString + ",ADDRESS_NO=" + this.textBoxAddressNo.Text;
                }
                if (this.textBoxBitIndex.Text.Trim() != "")
                {
                    masterObservable.ConnectionInfoString = masterObservable.ConnectionInfoString + ",BIT_INDEX=" + this.textBoxBitIndex.Text;
                }
                if (this.textBoxLength.Text.Trim() != "")
                {
                    masterObservable.ConnectionInfoString = masterObservable.ConnectionInfoString + ",BIT_LENGTH=" + this.textBoxLength.Text;
                }

                if (isModifiedMode)
                {
                    masterObservable.SaveToMasterDB();
                }
                else
                {
                    masterObservable.AddToMasterDB();
                }

                parent.ReloadMatserObservableData();
            }
            else
            {
                MessageBox.Show("Unavailable data exist. Check Again ");
            }
        }

        protected override void Cancel()
        {
            //base.Cancel();

            this.Dispose();
            this.Close();
        }

        protected override void CreateNewObservable()
        {
            base.CreateNewObservable();

            isModifiedMode = false;

            this.textBoxOwnerID.Text = string.Empty;
            this.textBoxOwnerID.Enabled = true;
            this.textBoxObservableID.Text = string.Empty;
            this.textBoxObservableID.Enabled = true;
            this.textBoxObservablePath.Text = string.Empty;
            this.textBoxObservablePath.Enabled = true;
            this.textBoxOwnerID.Focus();
        }
    }
}

