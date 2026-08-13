using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ECP.Device.Observe;


namespace ECP.Gui.Device.Oserve
{
    public partial class ObservableSpyForm : Form
    {
        public ObservableSpyForm()
        {
            InitializeComponent();            
        }

        public ObservableSpyForm(Dictionary<string, Observable> observables)
        {
            InitializeComponent();     
            this.observableSpy.AccessMode = ECP.Gui.Device.Observe.ObservableViewAccessMode.Spy;
//            this.observableSpy.Observables = observables;
            this.CreateHandle();
        }

        private void ObservableSpyForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();
            e.Cancel = true;
        }
        
    }
}