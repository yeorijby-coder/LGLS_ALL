using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class SAPIFHistoryForm : FormBase
    {
        public SAPIFHistoryForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.SAPIFHistoryMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.SAPIFHistoryMainPanel";
            ViewPanel(this.viewPanelName);
        }

    }
}