using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event.Device;

namespace ECP.Event.Device
{

    [Serializable]
    public  class DeviceStateEvent : DeviceEvent
    {

        public static readonly string CLASS ="ECP.Emulator.Event.Device.DeviceStateEvent";

        public static readonly string SPECIFIC_SUBJECT = "state";

        new public static readonly string SUBJECT = DeviceEvent.SUBJECT+"."+SPECIFIC_SUBJECT;


        public string state = "";

        public string State 
        {
            get { return this.state; }
            set { this.state = value; }
        }

        public DeviceStateEvent(string deviceId, string deviceType, string deviceState)
            : base(deviceId,deviceType)
        {
            this.State = deviceState;
           
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
