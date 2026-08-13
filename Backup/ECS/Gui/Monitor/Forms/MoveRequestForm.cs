using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class MoveRequestForm : FormBase
    {
        public MoveRequestForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.MoveRequestMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.MoveRequestMainPanel";
            ViewPanel(this.viewPanelName);
        }
    }
}