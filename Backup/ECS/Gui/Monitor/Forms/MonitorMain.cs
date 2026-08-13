using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Remoting;

namespace HECS.Gui.Monitor.Forms
{
    public partial class MonitorMain : FormBase
    {
        
        public MonitorMain()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.MonitorMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.MonitorMainPanel";
            ViewPanel(this.viewPanelName);
        }

        private void buttonMain_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorMainPanel");
        }

        private void buttonHiRack_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorHiRackMainPanel");
        }

        private void buttonConveyor_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorConveyorMainPanel");
        }

        private void buttonAGV_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorAGVMainPanel");

        }

        private void buttonRGV_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorRGVMainPanel");
        }

        private void buttonTransfers_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorTransferMainPanel");
        }

        private void buttonMaterial_Click(object sender, EventArgs e)
        {
            ViewPanel("HECS.Gui.Monitor.Panels.MonitorMaterialMainPanel");

        }


        
    }
}