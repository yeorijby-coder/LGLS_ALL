using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event;
using ECP.Event.Device;

namespace HECS.Event.ECS
{
    [Serializable]
    public class MasterConveyorEvent : DeviceEvent
    {
        public static readonly string CLASS = "MasterConveyorEvent";

        public static readonly string SPECIFIC_SUBJECT = "conveyor";

        new public static readonly string SUBJECT = DeviceEvent.SUBJECT + "." + SPECIFIC_SUBJECT;

        private string conveyorId;

        public string ConveyorId
        {
            get { return conveyorId; }
            set { conveyorId = value; }
        }

        private ConveyorEventType conveyorevent;

        public ConveyorEventType Conveyorevent
        {
            get { return conveyorevent; }
            set { conveyorevent = value; }
        }



        public enum ConveyorEventType : int
        {
            CARRIER_EXIST = 1,
            HAS_TRANSFER = 2,
            ALARM_RAISED = 3,
            STATE_CHANGE = 4,
            INHIBIT = 7,
            IM_HERE = 9,
        }

        public MasterConveyorEvent(string deviceId, string deviceType, string conveyorId, ConveyorEventType conveyorevent)
            : base(deviceId, deviceType)
        {
            this.conveyorId = conveyorId;
            this.conveyorevent = conveyorevent;

            this.SetSubject(this.CreateSubject(conveyorId));
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
