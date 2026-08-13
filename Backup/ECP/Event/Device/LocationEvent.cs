using System;
using System.Collections.Generic;
using System.Text;
using ECP.Event;

namespace ECP.Event.Device
{

    [Serializable]
    public abstract class LocationEvent : ECPEvent
    {
        public static readonly string SUBJECT = "location";

        private string locationId="";

        protected string LocationId
        {
          get { return locationId; }
          set { locationId = value; }
        }

     
        public LocationEvent(string locationId)
        {
            this.locationId = locationId;
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
