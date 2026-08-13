using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using ECP.Interfaces.Emulator;
using ECP.Device.Observe;

namespace ECP.Emulator.FieldBus
{
    public class ObservableSharedMemory : MarshalByRefObject, ISimulation
    {
        public ObservableSharedMemory()
        {          
            observables = new Dictionary<string, Observable>();
            Init();
        }

        private Dictionary<string, Observable> observables = null;


        public Dictionary<string, Observable> Observables
        {
            get { return this.observables; }
        }

        private void Init()
        {      
            observables = Observable.getAllObservable();
        }

        public int GetValue(string sUniqueID, out object newValue)
        {
            newValue = null;

            if (!this.observables.ContainsKey(sUniqueID))
            {
                return 1;
            }

            newValue = this.observables[sUniqueID].AsObject;

            return 0;
        }

        private object monitor = new object();
        public int SetValue(string sUniqueID, object newValue)
        {
            if (!this.observables.ContainsKey(sUniqueID))
            {
                return 1;
            }

            lock (monitor)
            {
                this.observables[sUniqueID].AsObject = newValue;
            }

            return 0;
        }
    }
}
