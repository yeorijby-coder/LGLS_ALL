using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using ECP.Emulator.FieldBus;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Util.Client;
using System.Threading;

namespace StockerSharedMemory
{
    public class StockerSharedMemory
    {
        const string CLASS = "StockerSharedMemory";

        public void Start()
        {
            Log.Init(CLASS);
            Log.SetLocalOutput(false);

            SharedMemoryForm sharedMemoryForm = new SharedMemoryForm();
            Naming.Init(Environment.CurrentDirectory + @"\StockerSharedMemory.exe.config");
            sharedMemoryForm.Init();
            Application.Run(sharedMemoryForm);
        }
    }
}
