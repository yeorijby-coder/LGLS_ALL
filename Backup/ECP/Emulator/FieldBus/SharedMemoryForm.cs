using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Remoting;

using ECP.Util.Server;
using ECP.Gui.Device.Observe;
using ECP.Device.Observe;
using ECP.Service.Logger;


namespace ECP.Emulator.FieldBus
{
    public partial class SharedMemoryForm : Form
    {
        const string CLASS = "SharedMemoryForm";

        ObservableSharedMemory observableSharedMemory = null;

        public SharedMemoryForm()
        {
            InitializeComponent();
        }

        public SharedMemoryForm(ObservableSharedMemory memory)
        {
            InitializeComponent();
            this.observableSharedMemory = memory;
            this.observableView.AccessMode = ObservableViewAccessMode.Simulation;
            this.observableView.Observables = observableSharedMemory.Observables;
        }

        public void Init()
        {
            observableSharedMemory = new ObservableSharedMemory();

            this.observableView.AccessMode = ObservableViewAccessMode.Simulation;
            this.observableView.Observables = observableSharedMemory.Observables;
            //RemotingServices.Marshal(this.observableSharedMemory, "ObservableSharedMemory");    
        }

        private void SharedMemoryForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.observableView.Unsubscribe();
        }    
    }
}