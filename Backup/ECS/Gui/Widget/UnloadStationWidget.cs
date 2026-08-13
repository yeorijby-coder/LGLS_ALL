using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Widget
{
    public partial class UnloadStationWidget : HECSBaseWidget
    {
        public UnloadStationWidget()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.ContainerControl, false);
        }
    }
}
