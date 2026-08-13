using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class HiRackStatusForm : FormBase
    {
        public HiRackStatusForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.HiRackStatusMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.HiRackStatusMainPanel";
            ViewPanel(this.viewPanelName);
        }
    }
}