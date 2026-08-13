using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace ECP.Gui.Config.Dialogs
{
    public partial class MasterObservableCreateForm : Form
    {
        MasterObservableCreatePanel masterObservableCreatePanel = null;

        public MasterObservableCreateForm()
        {
            InitializeComponent();
            this.CenterToScreen();

            masterObservableCreatePanel = new MasterObservableCreatePanel();
            masterObservableCreatePanel.Dock = DockStyle.Fill;
        }

        private void MasterObservableCreateForm_Load(object sender, EventArgs e)
        {
            this.Controls.Add(masterObservableCreatePanel);
        }
    }
}