using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Event;
using ECP.Util.Client;
using HECS.Device;
using HECS.Device.Unit;
using ECP.Service.Statistics;
using HECS.Statistics;
using HECS.Global;

namespace HECS.Gui.Monitor.Controls
{

    public partial class PortDirectionChange : UserControl
    {

        private string elementId;

        public string ElementId
        {
            get { return elementId; }
            set
            {
                elementId = value;

            }
        }

        public PortDirectionChange()
        {
            InitializeComponent();
        }

    }
}
