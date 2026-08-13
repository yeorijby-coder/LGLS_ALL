using System;
using System.Collections.Generic;
using System.Text;


using System.Runtime.Remoting;

namespace ECP.Emulator.FieldBus
{
    public class LauncherForSharedMemory
    {
        ObservableSharedMemory observableSharedMemory = null;

        public LauncherForSharedMemory()
        {
            Initialize();
        }

        public void Initialize()
        {
            observableSharedMemory = new ObservableSharedMemory();
            RemotingServices.Marshal(this.observableSharedMemory, "ObservableSharedMemory");
        }

        public void ShowSharedMemoryForm()
        {
            SharedMemoryForm smf = new SharedMemoryForm(this.observableSharedMemory);
            smf.Show();
        }
    }
}
