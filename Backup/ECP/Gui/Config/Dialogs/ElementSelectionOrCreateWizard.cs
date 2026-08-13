using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Gui.Widget;

namespace ECP.Gui.Config.Dialogs
{
    public partial class ElementSelectionOrCreateWizard : Form
    {

        protected BaseWidget widget = null;


        public ElementSelectionOrCreateWizard()
        {
            InitializeComponent();
        }
        public ElementSelectionOrCreateWizard(BaseWidget widget)
        {
            InitializeComponent();
            this.widget = widget;
        }

        protected virtual void buttonCreateElement_Click(object sender, EventArgs e)
        {

        }

        protected virtual void buttonOk_Click(object sender, EventArgs e)
        {

        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Dispose();
            this.Close();
        }
    }
}