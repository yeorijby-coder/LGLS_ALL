using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event;

namespace ECP.Event.Device
{

    [Serializable]
    public abstract class DeviceEvent : ECPEvent
    {
        public static readonly string SUBJECT = "device";

        protected string deviceId="";

        public string DeviceID
        {
            get { return this.deviceId; }
            set { this.deviceId = value; }
        }

        protected string deviceType = "";

        public string DeviceType
        {
            get { return this.deviceType; }
            set { this.deviceType = value; }
        }

     
        public DeviceEvent(string deviceId, string deviceType)
        {
            this.DeviceID = deviceId;
            this.DeviceType = deviceType;
        }

        protected string CreateSubject(string id)
        {
            return FormSubject(id);
        }

        public static string FormSubject(string id)
        {
            return SUBJECT + "." + id;
        }
        
    }
}
