using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


using ECP.Util;
using ECP.Gui.Config;

namespace ECP.Gui.Config.Dialogs
{
    public partial class GridSizeDialog : Form
    {
        ECPConfigPanel owner = null;

        public GridSizeDialog()
        {
            InitializeComponent();
        }

        public GridSizeDialog(ECPConfigPanel owner)
        {
            InitializeComponent();
            this.owner = owner;
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            string size = this.gridSizeTextBox.Text;

            foreach (char c in size)
            {
                if(!char.IsDigit(c)) return;
            }

            if (size != string.Empty)
            {
                this.owner.GridSize = Convert.ToInt32(size);
                this.owner.Invalidate();
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            string size = this.gridSizeTextBox.Text;

            foreach (char c in size)
            {
                if (!char.IsDigit(c)) return;
            }

            if (size != string.Empty)
            {
                this.owner.GridSize = Convert.ToInt32(size);
                this.owner.Invalidate();
            }
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}