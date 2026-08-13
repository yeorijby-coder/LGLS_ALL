using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.ConveyorSimulator.Devices;
using System.Threading;

namespace HECS.ConveyorSimulator
{
    public partial class FormBase : Form
    {
        private string deviceId = "";
     
        public string DeviceId
        {
            get
            {
                return deviceId;
            }
            set
            {
                deviceId = value;
            }
        }
        public FormBase()
        {
            InitializeComponent();
        }

        public FormBase(string deviceId)
        {
            InitializeComponent();
            this.Text = deviceId;
            this.deviceId = deviceId;
        }



        private void FormBase_FormClosing(object sender, FormClosingEventArgs e)
        {
          //  e.Cancel = true; //don't close 
        }

       

       
    }
}