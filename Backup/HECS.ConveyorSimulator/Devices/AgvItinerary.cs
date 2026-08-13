using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class AgvItinerary
    {
        public enum ROUTE_TYPE
        {
            DIRECTION=1,
            ROUND= 2,
        }
        private string routeId = "";

        public string RouteId
        {
            get
            {
                return routeId;
            }
            set
            {
                routeId = value;
            }
        }
        private MarkPlate fromMarkplate;

        public MarkPlate FromMarkplate
        {
            get
            {
                return fromMarkplate;
            }
            set
            {
                fromMarkplate = value;
            }
        }
        private MarkPlate toMarkplate;

        public MarkPlate ToMarkplate
        {
            get
            {
                return toMarkplate;
            }
            set
            {
                toMarkplate = value;
            }
        }
        private double distance = 0.0F;

        public double  Distance
        {
            get
            {
                return distance;
            }
            set
            {
                distance = value;
            }
        }
        private ROUTE_TYPE routeType = ROUTE_TYPE.DIRECTION;

        public ROUTE_TYPE RouteType
        {
            get
            {
                return routeType;
            }
            set
            {
                routeType = value;
            }
        }
        
    }
}
