using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Util.Client;
using ECP.Gui.Widget;
using ECP.Device;



namespace ECP.Gui.Config.Dialogs
{
    public partial class ElementWizardForm : Form
    {
        string elementId = "";

        BaseWidget baseWidget = null;

        public ElementWizardForm()
        {
            InitializeComponent();
        }

        public ElementWizardForm(BaseWidget baseWidget, string elementId)
        {
            InitializeComponent();
            this.elementId = elementId;
            this.baseWidget = baseWidget;
        }

        private void ElementWizardForm_Load(object sender, EventArgs e)
        {
            /*
            Equipment equipment = (Equipment)Naming.GetDeviceManager().GetDevice();

            SubSystem sub = equipment.GetSubSystem(this.elementId);
            if (sub != null)
             {
                 this.elemenIdText.Text = sub.ElementId;
             }
             else
             {
                 
             }
             */
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (this.elemenIdText.Text != string.Empty)
            {
                this.elementId = this.elemenIdText.Text;
                Equipment equipment = (Equipment)Naming.GetDeviceManager().GetDevice("");
                SubSystem sub = equipment.GetSubSystem(this.elementId);

                if (sub != null)
                {
                    this.baseWidget.ElementId = this.elemenIdText.Text;
                }
                else
                {
                    MessageBox.Show("Please Put the Correct Element Id!");
                }
            }
            else
            {
                //
            }
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}