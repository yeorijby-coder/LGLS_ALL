using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ECP.Gui.Config.Dialogs
{
    public partial class AbstractFieldBusDriverCreateOrEditForm : Form
    {
        AbstractFieldBusDriverCreateOrEditPanel abstractFieldBusDriverPanel = null;

        public AbstractFieldBusDriverCreateOrEditForm()
        {
            InitializeComponent();
            this.CenterToScreen();

            abstractFieldBusDriverPanel = new AbstractFieldBusDriverCreateOrEditPanel(this);
            abstractFieldBusDriverPanel.Dock = DockStyle.Fill;
        }

        private void AbstractFieldBusDriverCreateOrEditForm_Load(object sender, EventArgs e)
        {
            this.Controls.Add(abstractFieldBusDriverPanel);
        }
    }
}