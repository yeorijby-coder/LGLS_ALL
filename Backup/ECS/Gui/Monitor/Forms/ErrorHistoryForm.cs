using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class ErrorHistoryForm : FormBase
    {
        public ErrorHistoryForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.ErrorHistoryMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.ErrorHistoryMainPanel";
            ViewPanel(this.viewPanelName);
        }

    }
}