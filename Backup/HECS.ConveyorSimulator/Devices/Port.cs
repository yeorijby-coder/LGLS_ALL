using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class Port : Device
    {
        public enum PORT_STATE
        {
            READY = 1,
            LOAD_COMPLETE,
            UNLOAD_COMPLETE,
            MOVE_START,
            MOVING,
            MOVE_END,
        }

        private PORT_STATE portState = PORT_STATE.READY;

        public PORT_STATE PortState
        {
            get
            {
                return portState;
            }
            set
            {
                portState = value;
            }
        }
        private int stepCount = 0;

        public int StepCount
        {
            get
            {
                return stepCount;
            }
            set
            {
                stepCount = value;
            }
        }

        private string objectOrder = "";

        public string ObjectOrder
        {
            get
            {
                return objectOrder;
            }
            set
            {
                objectOrder = value;
            }
        }
        public override string DeviceId
        {
            get
            {
                return base.DeviceId;
            }
            set
            {
                if (value == null)
                {
                    base.DeviceId = "";
                }
                else
                {
                    base.DeviceId = value;
                    this.ObjectOrder = GetDefaultPortOrder(value);
                }
            }
        }
        private string GetDefaultPortOrder(string elementId)
        {
            string order = "";
            switch (elementId)
            {
                case "PORT:1":
                case "PORT:3":
                case "PORT:5":
                case "PORT:7":
                case "PORT:9":
                case "PORT:11":
                case "PORT:13":
                case "PORT:15":
                case "PORT:17":
                case "PORT:19":
                case "PORT:21":
                case "PORT:23":
                case "PORT:25":
                case "PORT:27":
                case "PORT:31":
                    order = "1";
                    break;
                case "PORT:2":
                case "PORT:4":
                case "PORT:6":
                case "PORT:8":
                case "PORT:10":
                case "PORT:12":
                case "PORT:14":
                case "PORT:16":
                case "PORT:18":
                case "PORT:20":
                case "PORT:22":
                case "PORT:24":
                case "PORT:26":
                case "PORT:28":
                case "PORT:32":
                    order = "2";
                    break;
                case "PORT:30":
                case "PORT:29":
                    order = "3";
                    break; 
            }

            return order;
        }
    }
}
