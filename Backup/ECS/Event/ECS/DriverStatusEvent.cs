using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event.Device;
using ECP.Global.Com;

namespace HECS.Event.ECS
{
    [Serializable]
    public class DriverStatusEvent : DeviceEvent
    {
        public static readonly string CLASS = "DriverStatusEvent";

        public static readonly string SPECIFIC_SUBJECT = "ECS";

        new public static readonly string SUBJECT = DeviceEvent.SUBJECT + "." + SPECIFIC_SUBJECT;

        private FBConnectionState _drvStatus;

        public FBConnectionState DrvStatus
        {
            get { return _drvStatus; }
            set { _drvStatus = value; }
        }

        public DriverStatusEvent(string deviceId, string deviceType, FBConnectionState connectionState)
            :base(deviceId,deviceType)
        {
            _drvStatus = connectionState;
            this.SetSubject(this.CreateSubject(deviceId));
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
