using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Device.Observe;

namespace ECP.Gui.Config.Dialogs
{
    public partial class ObservableWizardForm : Form
    {
        protected bool isModifiedMode = false;

        public ObservableWizardForm()
        {
            InitializeComponent();
        }

        public ObservableWizardForm(bool modified)
        {
            InitializeComponent();
            isModifiedMode = modified;
        }

        protected virtual void LoadObservableData(bool modified, Observable seletecdObs)
        {
        }

        protected virtual void LoadObservableData(string ownerId, string observableId)
        {
        }

        protected virtual void textBoxOwnerID_Leave(object sender, EventArgs e)
        {
        }

        protected virtual void textBoxOwnerID_Enter(object sender, EventArgs e)
        {
        }

        protected virtual void HandleButtons(object sender, EventArgs e)
        {
            Button button = (Button)sender;

            if (button.Text.Equals("Apply"))
            {
                Apply();
            }
            else if (button.Text.Equals("Cancel"))
            {
                Cancel();
            }
            else if (button.Text.Equals("OK"))
            {
                Apply();
                Cancel();
            }
            else if (button.Text.Equals("New"))
            {
                CreateNewObservable();
            }
        }

        protected virtual void CreateNewObservable()
        {
        }

        protected virtual void Apply()
        {
        }

        protected virtual void Cancel()
        {
        }
    }
}