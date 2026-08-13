using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.Device.Unit
{
    [Serializable]
    public class AGVPath : MarshalByRefObject
    {
        public readonly static char DEFAULT_PATH_DELIMITER = '|';
        private List<AgvItinerary> path = new List<AgvItinerary>();
        public AGVPath(string agvPathList)
        {
            if (string.IsNullOrEmpty(agvPathList))
            {
                return;
            }
            string[] tmp = agvPathList.Split(DEFAULT_PATH_DELIMITER);
            for (int i = 0; i < tmp.Length; i++)
            {
                if (string.IsNullOrEmpty(tmp[i]) || ECSDeviceManager.AgvItinerary.ContainsKey(tmp[i])==false)
                {
                    continue;
                }
                this.AddPath(ECSDeviceManager.AgvItinerary[tmp[i]]);
            }
        }


        public void AddPath(AgvItinerary itinerary)
        {
            path.Add(itinerary);
        }

        public double Weight
        {
            get
            {
                return GetWeight();
            }
        }

        private double GetWeight()
        {
            double weight = 0;
            foreach (AgvItinerary itinerary in path)
            {
                weight = weight + itinerary.Distance;
            }
            return weight;
        }

        public bool CanAddPath(string routeId)
        {
            if (string.IsNullOrEmpty(routeId))
            {
                return false;
            }
            if (IsExistItinerary(routeId))
            {
                return false;
            }
            return true;
        }

        public bool CanAddPath(AgvItinerary route)
        {
            if (route == null)
            {
                return false;
            }
            if (IsExistItinerary(route))
            {
                return false;
            }
            return true;
        }

        public bool IsExistItinerary(string routeId)
        {
            if (string.IsNullOrEmpty(routeId))
            {
                return false;
            }
            foreach (AgvItinerary itinerary in path)
            {
                if (itinerary.RouteId == routeId)
                {
                    return true;
                }
            }
            return false;
        }

        public bool IsExistItinerary(AgvItinerary route)
        {
            if (route == null)
            {
                return false;
            }
            foreach (AgvItinerary itinerary in path)
            {
                if (itinerary.RouteId == route.RouteId)
                {
                    return true;
                }
            }
            return false;
        }

        public override string ToString()
        {
            string pathTemp = "";
            foreach (AgvItinerary itinerary in path)
            {
                if (string.IsNullOrEmpty(pathTemp))
                {
                    pathTemp =  itinerary.FromMarkplate.Markplateid;
                }
                else
                {
                    pathTemp = pathTemp + DEFAULT_PATH_DELIMITER + itinerary.FromMarkplate.Markplateid;
                }
            }
            pathTemp = pathTemp + DEFAULT_PATH_DELIMITER + path[path.Count - 1].ToMarkplate.Markplateid;
            return pathTemp;
        }
    }
}
