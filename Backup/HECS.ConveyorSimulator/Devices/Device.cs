using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;
using ECP.Com.FieldBus;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public enum DEVICE_TYPE
    {
        UNKNOWN = 0,
        DEVICE = 1,
        COMPONENT = 2,
        CONVEYOR = 3,
        RACK = 4,
        VEHICLE = 5,
        PORT = 6,
        LIFT = 7,
        STOCKER = 8,
        SHELF = 9,
        
    }
    public enum RUN_STATE
    {
        DOWN = 0,
        IDLE = 1,
        RUN = 2
    }
   
    [Serializable]
    public class Device
    {

        private RUN_STATE runState = RUN_STATE.DOWN;
        protected RUN_STATE RunState
        {
            get
            {
                return runState;
            }
            set
            {
                runState = value;
            }
        }

        private string deviceId = "";
        public virtual string DeviceId
        {
            get
            {
                return deviceId;
            }
            set
            {
                if (value == null)
                {
                    deviceId = "";
                }
                else
                {
                    deviceId = value;
                }

            }
        }

        protected string hostId = "";
        public string HostId
        {
            get
            {
                return hostId;
            }
            set
            {
                if (value == null)
                {
                    hostId = "";
                }
                else
                {
                    hostId = value;
                }
            }
        }

        protected string plcId = "";
        public string PlcId
        {
            get
            {
                return plcId;
            }
            set
            {
                if (value == null)
                {
                    plcId = "";
                }
                else
                {
                    plcId = value;
                }
            }
        }

        protected string ownerid = "";

        public string Ownerid
        {
            get
            {
                return ownerid;
            }
            set
            {
                ownerid = value;
            }
        }
        private DEVICE_TYPE deviceType = DEVICE_TYPE.DEVICE;
        public DEVICE_TYPE DeviceType
        {
            get
            {
                return deviceType;
            }
            set
            {
                deviceType = value;
            }
        }

        private Dictionary<string, Conveyor> conveyors = new Dictionary<string, Conveyor>();
        public Dictionary<string, Conveyor> Conveyors
        {
            get
            {
                return conveyors;
            }
            set
            {
                conveyors = value;
            }
        }
        private Dictionary<string, Stocker> stocker = new Dictionary<string, Stocker>();
        public Dictionary<string, Stocker> Stockers
        {
            get
            {
                return stocker;
            }
            set
            {
                stocker = value;
            }
        }
        private Dictionary<string, Lift> lifts = new Dictionary<string, Lift>();
        public Dictionary<string, Lift> Lifts
        {
            get
            {
                return lifts;
            }
            set
            {
                lifts = value;
            }
        }
        private Dictionary<string, Port> ports = new Dictionary<string, Port>();
        public Dictionary<string, Port> Ports
        {
            get
            {
                return ports;
            }
            set
            {
                ports = value;
            }
        }
        private Dictionary<string, Vehicle> vehicles = new Dictionary<string, Vehicle>();
        public Dictionary<string, Vehicle> Vehicles
        {
            get
            {
                return vehicles;
            }
            set
            {
                vehicles = value;
            }
        }
        private Dictionary<string, Rack> racks = new Dictionary<string, Rack>();

        public Dictionary<string, Rack> Racks
        {
            get
            {
                return racks;
            }
            set
            {
                racks = value;
            }
        }

        private Dictionary<string, Shelf> shelves = new Dictionary<string, Shelf>();

        public Dictionary<string, Shelf> Shelves
        {
            get
            {
                return shelves;
            }
            set
            {
                shelves = value;
            }
        }

        public bool IsHaveDevice(string deviceId)
        {
            if (ports.ContainsKey(deviceId))
            {
                return true;
            }

            if (stocker.ContainsKey(deviceId))
            {
                return true;
            }
            if (conveyors.ContainsKey(deviceId))
            {
                return true;
            }
            if (vehicles.ContainsKey(deviceId))
            {
                return true;
            }

            if (lifts.ContainsKey(deviceId))
            {
                return true;
            }

            foreach (Port port in ports.Values)
            {
                if (port.deviceId == deviceId || port.plcId == deviceId || port.hostId == deviceId)
                {
                    return true;
                }
            }

            foreach (Lift lift in lifts.Values)
            {
                if (lift.deviceId == deviceId || lift.plcId == deviceId || lift.hostId == deviceId)
                {
                    return true;
                }
            }

            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (vehicle.deviceId == deviceId || vehicle.plcId == deviceId || vehicle.hostId == deviceId)
                {
                    return true;
                }

            }
            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (conveyor.plcId == deviceId || conveyor.hostId == deviceId || conveyor.deviceId == deviceId)
                {
                    return true;
                }
            }
            return false;
        }

        private Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

        public Dictionary<string, Observable> Observables
        {
            get
            {
                return observables;
            }
        }

        public void AddObservable(Observable obs)
        {
            Observables.Add(obs.ObservableId, obs);
        }
        public void RegisterObservablesToFieldBusDrvier(AbstractFieldBusDriver driver)
        {
            foreach (string key in Observables.Keys)
            {
                Observable obs = this.Observables[key];
                driver.RegisterObservable(this.Observables[key]);
            }
        }

        public void LoadObservable(string elementId)
        {
            this.observables = Observable.LoadObservables(elementId);
        }

        public virtual void Initialize()
        {
        }
    }
}
