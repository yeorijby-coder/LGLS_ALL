using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Forms
{
    public partial class MovementRuleForm : FormBase
    {
        public MovementRuleForm()
        {
            InitializeComponent();
            this.defaultPanelName = "HECS.Gui.Monitor.Panels.MovementRuleInfoMainPanel";
            this.viewPanelName = "HECS.Gui.Monitor.Panels.MovementRuleInfoMainPanel";
            ViewPanel(this.viewPanelName);
        }

    }
}