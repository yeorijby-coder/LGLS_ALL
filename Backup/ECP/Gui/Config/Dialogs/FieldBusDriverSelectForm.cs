using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Com.FieldBus;
using ECP.Global.Com;

namespace ECP.Gui.Config.Dialogs
{
    public partial class FieldBusDriverSelectForm : Form
    {
        AbstractFieldBusDriverCreateOrEditPanel parent = null;

        public FieldBusDriverSelectForm(AbstractFieldBusDriverCreateOrEditPanel parent)
        {
            InitializeComponent();
            this.CenterToParent();
            this.parent = parent;
            this.comboBoxDriverType.Items.AddRange(FieldBusDriverType.displayStrings);
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Dispose();
            this.Close();
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            Dictionary<int, AbstractFieldBusDriver> fbDrivers =
                new Dictionary<int, AbstractFieldBusDriver>();

            fbDrivers = AbstractFieldBusDriver.LoadAbstractFieldBusDrivers();

            Dictionary<int, AbstractFieldBusDriver> newFBDrivers =
                new Dictionary<int, AbstractFieldBusDriver>();

            int inx = 2;
            foreach (AbstractFieldBusDriver fbDriver in fbDrivers.Values)
            {
                if (fbDriver.FieldBusDriverType ==
                    FieldBusDriverType.StringToValue(comboBoxDriverType.SelectedItem.ToString()))
                {   
                    newFBDrivers.Add(1, fbDriver);
                    fbDriver.Delete();

                    newFBDrivers[1].DriverNo = 1;
                    newFBDrivers[1].UseFlag = true;
                }
                else
                {
                    newFBDrivers.Add(inx, fbDriver);
                    fbDriver.Delete();

                    newFBDrivers[inx].DriverNo = inx;
                    newFBDrivers[inx].UseFlag = false;
                }
            }

            foreach (AbstractFieldBusDriver newFBDriver in newFBDrivers.Values)
            {
                newFBDriver.Add();
            }

            parent.ReloadAbstracFieldBusDriverData();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.buttonApply_Click(sender, e);
            this.Dispose();
            this.Close();
        }
    }
}