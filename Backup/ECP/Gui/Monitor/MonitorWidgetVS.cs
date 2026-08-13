using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;

namespace ECP.Gui.Monitor
{
    public class MonitorWidgetVS : Panel
    {
        public void SubscribeWidgets()
        {
            foreach (Control control in this.Controls)
            {
                if (control is BaseWidget)
                {
                    BaseWidget baseWidget = (BaseWidget)control;
                    baseWidget.Subscribe();
                }
            }
        }
    }
}
