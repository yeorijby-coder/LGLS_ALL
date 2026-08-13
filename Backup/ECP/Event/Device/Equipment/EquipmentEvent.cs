using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event.Device;

namespace ECP.Event.Device.Equipment
{

    [Serializable]
    public abstract class EquipmentEvent : DeviceEvent
    {

        public static readonly string SPECIFIC_SUBJECT = "equipment";

        new public static readonly string SUBJECT = DeviceEvent.SUBJECT+"."+SPECIFIC_SUBJECT;

  
        protected EquipmentEvent(string deviceId, string deviceType)
            : base(deviceId,deviceType)
        {
           
        }

        new protected string CreateSubject(string id)
        {
            return FormSubject(id);
        }

        new public static string FormSubject(string id)
        {
            return SUBJECT + "." + id;
        }
        
    }
}
