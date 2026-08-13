using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class SystemInfoForm : FormBase
    {
        public SystemInfoForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.SystemInfoMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.SystemInfoMainPanel";
            ViewPanel(this.viewPanelName);
        }

    }
}