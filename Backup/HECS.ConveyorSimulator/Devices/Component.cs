using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class Component : Device 
    {
        private string equipmentId = "";

        public string EquipmentId
        {
            get
            {
                return equipmentId;
            }
            set
            {
                if (value == null)
                {
                    equipmentId = "";
                    this.DeviceId = equipmentId;
                }
                else
                {
                    equipmentId = value;
                    this.DeviceId = equipmentId;
                }

            }
        }

        public Component()
        {
            this.DeviceType = DEVICE_TYPE.COMPONENT;
        }
    }
}
