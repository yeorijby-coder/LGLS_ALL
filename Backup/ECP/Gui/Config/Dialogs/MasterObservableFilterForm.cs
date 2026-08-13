using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Global;
using ECP.Global.Device;

using ECP.Device.Observe;

namespace ECP.Gui.Config.Dialogs
{
    public partial class MasterObservableFilterForm : ECP.Gui.Config.Dialogs.ObservableWizardForm
    {
        MasterObservableCreatePanel parent = null;

        public MasterObservableFilterForm()
        {
            InitializeComponent();
        }

        public MasterObservableFilterForm(MasterObservableCreatePanel parent)
        {
            InitializeComponent();
            this.CenterToParent();
            this.buttonNew.Visible = false;
            this.parent = parent;
        }

        protected override void HandleButtons(object sender, EventArgs e)
        {
            base.HandleButtons(sender, e);
        }

        protected override void Apply()
        {
            Dictionary<string, string> filteringCondition = new Dictionary<string, string>();

            if (!this.textBoxOwnerID.Text.Equals("*"))
            {
                filteringCondition.Add("OWNERID", this.textBoxOwnerID.Text);
            }

            if (!(this.comboBoxOwnerType.SelectedItem == null))
            {
                filteringCondition.Add("OWNERTYPE", this.comboBoxOwnerType.SelectedItem.ToString());
            }

            if (!this.textBoxObservableID.Text.Equals("*"))
            {
                filteringCondition.Add("OBSERVABLEID", this.textBoxObservableID.Text);
            }

            if (!this.textBoxObservablePath.Text.Equals("*"))
            {
                filteringCondition.Add("OBSERVABLEPATH", this.textBoxObservablePath.Text);
            }

            if (!(this.comboBoxAccessType.SelectedItem == null))
            {
                filteringCondition.Add("ACCESSTYPE", this.comboBoxAccessType.SelectedItem.ToString());
            }

            if (!(this.comboBoxDataType.SelectedItem == null))
            {
                filteringCondition.Add("DATATYPE", this.comboBoxDataType.SelectedItem.ToString());
            }

            if (!this.textBoxMin.Text.Equals("*"))
            {
                filteringCondition.Add("MIN", this.textBoxMin.Text);
            }

            if (!this.textBoxMax.Text.Equals("*"))
            {
                filteringCondition.Add("MAX", this.textBoxMax.Text);
            }

            if (!this.textBoxDriverNo.Text.Equals("*"))
            {
                filteringCondition.Add("DRIVERNO", this.textBoxDriverNo.Text);
            }

            if (!(this.comboBoxDeviceType.SelectedItem == null))
            {
                filteringCondition.Add("CONNECTSTRING", "DEVICE_TYPE=" + this.comboBoxDeviceType.SelectedItem.ToString());
            }

            if (!this.textBoxAddressNo.Text.Equals("*"))
            {
                if (filteringCondition.ContainsKey("CONNECTSTRING"))
                {
                    filteringCondition["CONNECTSTRING"] =
                        filteringCondition["CONNECTSTRING"] + "\n and CONNECTSTRING LIKE '%" + this.textBoxAddressNo.Text;
                }
                else
                {
                    filteringCondition.Add("CONNECTSTRING", this.textBoxAddressNo.Text);
                }
            }

            if (!this.textBoxBitIndex.Text.Equals("*"))
            {
                if (filteringCondition.ContainsKey("CONNECTSTRING"))
                {
                    filteringCondition["CONNECTSTRING"] =
                        filteringCondition["CONNECTSTRING"] + "\n and CONNECTSTRING LIKE '%BIT_INDEX=" + this.textBoxBitIndex.Text;
                }
                else
                {
                    filteringCondition.Add("CONNECTSTRING", "BIT_INDEX=" + this.textBoxBitIndex.Text);
                }
            }

            if (!this.textBoxLength.Text.Equals("*"))
            {
                if (filteringCondition.ContainsKey("CONNECTSTRING"))
                {
                    filteringCondition["CONNECTSTRING"] =
                        filteringCondition["CONNECTSTRING"] + "\n and CONNECTSTRING LIKE '%LENGTH=" + this.textBoxLength.Text;
                }
                else
                {
                    filteringCondition.Add("CONNECTSTRING", "LENGTH=" + this.textBoxLength.Text);
                }
            }

            filteringCondition.Add("MONFLAG", this.checkBoxMonFlag.Checked ? "T" : "F");
            filteringCondition.Add("LOG_ENABLE", this.checkBoxLogEnable.Checked ? "T" : "F");

            parent.LoadMasterObservableData(filteringCondition);
        }

        protected override void Cancel()
        {
            base.Cancel();
        }
    }
}

