using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Com.FieldBus;
using ECP.Global.Com;
using ECP.Global.Device;
using ECP.Util.Client;
using ECP.Util;

namespace ECP.Gui.Config.Dialogs
{
    public partial class AbstractFieldBusDriverWizardForm : Form
    {
        AbstractFieldBusDriverCreateOrEditPanel parent = null;

        int driverNo = 0;
        string ownerId = null;

        bool isModifiedMode = false;

        public AbstractFieldBusDriverWizardForm()
        {
            InitializeComponent();
            this.CenterToScreen();
        }

        public AbstractFieldBusDriverWizardForm(AbstractFieldBusDriverCreateOrEditPanel parent)
        {
            this.parent = parent;
            InitializeComponent();
            this.CenterToParent();
        }

        public AbstractFieldBusDriverWizardForm(
                    AbstractFieldBusDriverCreateOrEditPanel parent,
                    int driverNo,
                    string ownerId,
                    bool modifiedMode)
        {
            this.parent = parent;
            this.isModifiedMode = modifiedMode;
            InitializeComponent();
            this.CenterToParent();

            if (modifiedMode)
            {
                this.driverNo = driverNo;
                this.ownerId = ownerId;

                LoadAbstractFieldBusDriverData();

                this.textBoxOwnerId.Enabled = false;
                this.textBoxDriverNo.Enabled = false;
            }
        }

        public void LoadAbstractFieldBusDriverData()
        {
            AbstractFieldBusDriver fieldBusDriver =
                new AbstractFieldBusDriver(this.driverNo, this.ownerId);
            fieldBusDriver.Load();

            this.textBoxDriverNo.Text = fieldBusDriver.DriverNo.ToString();
            this.textBoxOwnerId.Text = fieldBusDriver.OwnerId;
            this.textBoxOpenRetSec.Text = fieldBusDriver.OpenRetrySec.ToString();
            this.textBoxScanInterval.Text = fieldBusDriver.ScanInterval.ToString();
            this.textBoxTimeOutSec.Text = fieldBusDriver.TimeOutSec.ToString();
            this.textBoxDescription.Text = fieldBusDriver.Description;
            this.textBoxConnectString.Text = fieldBusDriver.ConnectionInfoString;
            this.textBoxAssemblyName.Text = fieldBusDriver.assemblyName;
            this.textBoxClassName.Text = fieldBusDriver.className;

            this.checkBoxUseFlag.Checked = fieldBusDriver.UseFlag;

            foreach (object item in comboBoxDriverType.Items)
            {
                string driverType = (string)item;
                if (driverType.Equals(FieldBusDriverType.ValueToString(fieldBusDriver.FieldBusDriverType)))
                {
                    this.comboBoxDriverType.SelectedItem = item;
                    break;
                }
            }

            foreach(object item in comboBoxOwnerType.Items)
            {
                string ownerType = (string)item;
                if (ownerType.Equals(ElementType.ValueToString(fieldBusDriver.OwnerType)))
                {
                    this.comboBoxOwnerType.SelectedItem = item;
                    break;
                }
            }
        }
        private void buttonApply_Click(object sender, EventArgs e)
        {
            if (!Helper.IsNullOrEmpty(this.textBoxDriverNo.Text) &&
                !Helper.IsNullOrEmpty(this.textBoxOwnerId.Text))
            {
                if (isModifiedMode)
                {
                    AbstractFieldBusDriver fbDriver =
                        new AbstractFieldBusDriver(Convert.ToInt32(textBoxDriverNo.Text), textBoxOwnerId.Text);

                    if (fbDriver == null)
                    {
                        return;
                    }
                    else
                    {
                        fbDriver.FieldBusDriverType = FieldBusDriverType.StringToValue((string)this.comboBoxDriverType.SelectedItem);
                        fbDriver.OwnerType = ElementType.StringToValue((string)this.comboBoxOwnerType.SelectedItem);
                        fbDriver.OpenRetrySec = Convert.ToInt32(this.textBoxOpenRetSec.Text);
                        fbDriver.TimeOutSec = Convert.ToInt32(this.textBoxTimeOutSec.Text);
                        fbDriver.ConnectionInfoString = this.textBoxConnectString.Text;
                        fbDriver.ScanInterval = Convert.ToInt32(this.textBoxScanInterval.Text);
                        fbDriver.UseFlag = this.checkBoxUseFlag.Checked;
                        fbDriver.Description = this.textBoxDescription.Text;
                        fbDriver.assemblyName = this.textBoxAssemblyName.Text;
                        fbDriver.className = this.textBoxClassName.Text;

                        fbDriver.Save();
                    }
                }
                else
                {
                    AbstractFieldBusDriver fbDriver = new AbstractFieldBusDriver();

                    if (fbDriver == null)
                    {
                        return;
                    }
                    else
                    {
                        fbDriver.DriverNo = Convert.ToInt32(this.textBoxDriverNo.Text);
                        fbDriver.OwnerId = this.textBoxOwnerId.Text;
                        fbDriver.FieldBusDriverType = FieldBusDriverType.StringToValue((string)this.comboBoxDriverType.SelectedItem);
                        fbDriver.OwnerType = ElementType.StringToValue((string)this.comboBoxOwnerType.SelectedItem);
                        fbDriver.OpenRetrySec = Convert.ToInt32(this.textBoxOpenRetSec.Text);
                        fbDriver.TimeOutSec = Convert.ToInt32(this.textBoxTimeOutSec.Text);
                        fbDriver.ConnectionInfoString = this.textBoxConnectString.Text;
                        fbDriver.ScanInterval = Convert.ToInt32(this.textBoxScanInterval.Text);
                        fbDriver.UseFlag = this.checkBoxUseFlag.Checked;
                        fbDriver.Description = this.textBoxDescription.Text;
                        fbDriver.assemblyName = this.textBoxAssemblyName.Text;
                        fbDriver.className = this.textBoxClassName.Text;

                        fbDriver.Add();
                    }
                }
            }
            else
            {
                MessageBox.Show("Unavailable Information exist, Check again ");
            }

            if (parent != null)
            {
                parent.ReloadAbstracFieldBusDriverData();
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            buttonApply_Click(sender, e);

            this.Dispose();
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Dispose();
            this.Close();
        }


    }
}