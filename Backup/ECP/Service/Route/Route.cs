using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Service.Route
{
    /// <summary>
    /// Route 
    /// </summary>
    public class Route
    {
        public Route()
        { 
        }

        /// <summary>
        /// 현재 Device
        /// </summary>
        private string _currentDevice;

        public string CurrentDevice
        {
            get { return _currentDevice; }
            set { _currentDevice = value; }
        }

        private List<Route> _nextRoute = new List<Route>();

        public List<Route> NextRoute
        {
            get { return _nextRoute; }
            set { _nextRoute = value; }
        }

        /// <summary>
        /// 현재 Devie에서 갈수 있는 다음 Itineraray
        /// </summary>
        private List<Itineraray> _nextItineraray = new List<Itineraray>();

        public List<Itineraray> NextItineraray
        {
            get { return _nextItineraray; }
            set { _nextItineraray = value; }
        }

        /// <summary>
        /// 역방향 Itineraray가 존재하는가?
        /// </summary>
        /// <param name="orgDevice"></param>
        /// <param name="destDeivce"></param>
        /// <returns></returns>
        public bool HasReverseItineraray(string orgDevice, string destDeivce)
        {
            for (int i = 0; i < _nextItineraray.Count; i++)
            {
                if (_nextItineraray[i].FromDeviceId.Equals(destDeivce) && _nextItineraray[i].ToDeviceId.Equals(orgDevice))
                {
                    return true;
                }
            }

            for (int i = 0; i < _nextRoute.Count; i++)
            {
                if (_nextRoute[i].HasReverseItineraray(orgDevice, destDeivce))
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// 중복된 Itineraray
        /// </summary>
        /// <param name="orgDevice"></param>
        /// <param name="destDeivce"></param>
        /// <returns></returns>
        public bool HasExistItineraray(string orgDevice, string destDeivce)
        {
            for (int i = 0; i < _nextItineraray.Count; i++)
            {
                if (_nextItineraray[i].FromDeviceId.Equals(orgDevice) && _nextItineraray[i].ToDeviceId.Equals(destDeivce))
                {
                    return true;
                }
            }

            for (int i = 0; i < _nextRoute.Count; i++)
            {
                if (_nextRoute[i].HasExistItineraray(orgDevice, destDeivce))
                {
                    return true;
                }
            }

            return false;
 
        }

        public bool InvalidItineraray(string orgDevice, string destDeivce)
        {
            if (HasReverseItineraray(orgDevice, destDeivce) || HasExistItineraray(orgDevice, destDeivce))
            {
                return false;
            }
            else
            {
                return true;
            }

        }
    
       
    }
}
