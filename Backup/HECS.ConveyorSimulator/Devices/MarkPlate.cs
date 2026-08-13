using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class MarkPlate
    {

        private string markplateid = "";
        public string Markplateid
        {
            get
            {
                return markplateid;
            }
            set
            {
               markplateid = value;
                switch (value)
                {
                    case "5":
                        this.portId = "PORT:63";
                        break;
                    case "6":
                        this.portId = "PORT:62";
                        break;
                    case "8":
                        this.portId = "PORT:11";
                        break;
                    case "16":
                        this.portId = "PORT:67";
                        break;
                    case "21":
                        this.portId = "PORT:70";
                        break;
                    case "39":
                        this.portId = "PORT:55";
                        break;
                    case "41":
                        this.portId = "PORT:54";
                        break;
                    case "40":
                        this.portId = "PORT:51";
                        break;
                    case "49":
                        this.portId = "PORT:50";
                        break;
                    case "48":
                        this.portId = "PORT:47";
                        break;
                    case "58":
                        this.portId = "PORT:41";
                        break;
                    case "59":
                        this.portId = "PORT:37";
                        break;
                    case "69":
                        this.portId = "PORT:36";
                        break;
                    case "70":
                        this.portId = "PORT:33";
                        break;
                }
            }
        }
        private string portId = "";

        public string PortId
        {
            get
            {
                return portId;
            }
            set
            {
                portId = value;
            }
        }

    }
}
