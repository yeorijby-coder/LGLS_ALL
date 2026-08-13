using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using ItinerarayStatus = ECP.Service.Route.Itineraray.ItinerarayStatus;

namespace ECP.Service.Route
{
    /// <summary>
     /// Route를 관리 함.
    /// </summary>
    public class RouteManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "RouteManager";



        /// <summary>
        /// Manager가 Create하고 Control하고 있는 모든 Itineraray를 담고 있는 Container
        /// </summary>
        protected Dictionary<string, Itineraray> _itineraries = new Dictionary<string, Itineraray>();

        /// <summary>
        /// RouteManager 생성자
        /// 주어진 Owner Id와 Manager Id로 RouteManager를 생성한다.
        /// RouteManager생성시, RouteManager로 Configration되어진
        /// 모든 Itinerary를 Database로부터, Load한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public RouteManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.LoadItineraries();
        }

        /// <summary>
        /// DeviceManager가 생성한 모든 Itineraray를 Database로부터
        /// Load함
        /// </summary>
        public void LoadItineraries()
        {
           _itineraries = Itineraray.LoadItineraries();
        }


        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }


        /// <summary>
        /// Data base로 부터 Data를 Load함. 항상 Sub Class에 의하여 Override되어
        /// Implement되어져야 함 항상 base.Load()가 먼저 Call되어져야 함
        /// 
        /// SubClass가 Instance화 된 직후, 이 Method을 Call함으로 Configration되어있는
        /// 기준 정보에 의하여 Sub Object가 초기화 되어짐
        /// 
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection</param>
        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }


        /// <summary>
        /// Data base로 Data를 Save함
        /// Sub Class는 이 Method를 Override하여 Implement하여야 하며,
        /// 항상 base.Save() 먼저 Call되어 상속 계층에 있는 Parent Class가 먼저 
        /// 저장되도록 하여야 함
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }

        #region method

        /// <summary>
        /// 출발 Device와 연계된 Itineraray들을 알려 줌.
        /// </summary>
        /// <param name="orginDevice"></param>
        /// <returns></returns>
        public Dictionary<string, Itineraray> GetOriginItineraries(string orginDevice)
        {
            if (String.IsNullOrEmpty(orginDevice))
                return null;

             Dictionary<string, Itineraray> resultItineraries = null;

            foreach (Itineraray itineraray in _itineraries.Values)
            {
                if (itineraray.FromDeviceId.Equals(orginDevice))
                {
                    resultItineraries.Add(itineraray.PathId, itineraray);
                }
            }

            return resultItineraries;
        }

        /// <summary>
        /// 도착 Device와 연계된 Itineraray들을 알려 줌.
        /// </summary>
        /// <param name="orginDevice"></param>
        /// <returns></returns>
        public Dictionary<string, Itineraray> GetDestItineraries(string destDevice)
        {
            if (String.IsNullOrEmpty(destDevice))
                return null;

            Dictionary<string, Itineraray> resultItineraries = null;

            foreach (Itineraray itineraray in _itineraries.Values)
            {
                if (itineraray.ToDeviceId.Equals(destDevice))
                {
                    resultItineraries.Add(itineraray.PathId, itineraray);
                }
            }

            return resultItineraries;
        }

        /// <summary>
        /// itineraray의 상태를 변경
        /// </summary>
        /// <param name="devieID"></param>
        /// <param name="status"></param>
        public void SetItineararyStatus(string devieID, ItinerarayStatus status)
        {
            if (String.IsNullOrEmpty(devieID))
                return;

            //Orgion
            foreach (Itineraray itineraray in _itineraries.Values)
            {
                if (itineraray.FromDeviceId.Equals(devieID))
                {
                    itineraray.Status = status;
                }
            }

            //Dest
            foreach (Itineraray itineraray in _itineraries.Values)
            {
                if (itineraray.ToDeviceId.Equals(devieID))
                {
                    itineraray.Status = status;
                }
            }

        }

        /// <summary>
        /// 특정 Itineraray를 돌려 줌.
        /// </summary>
        /// <param name="id"></param>
        public Itineraray GetItineraray(string id)
        {
            if (String.IsNullOrEmpty(id))
                return null;

            if (_itineraries.ContainsKey(id))
            {
                return _itineraries[id];
            }
            else
            {
                return null;
            }
        }
/*
        /// <summary>
        /// 
        /// </summary>
        /// <param name="orginDevie"></param>
        /// <param name="destDevice"></param>
        /// <returns></returns>
        public Route GetRoute(string orginDevie, string destDevice, Route sourceRoute)
        {
            if (String.IsNullOrEmpty(orginDevie) || String.IsNullOrEmpty(destDevice))
                return null;

            Route nextRoute = new Route();

            if (sourceRoute == null)
            {
                sourceRoute = nextRoute;
            }
            else
            {
              //
            }

            nextRoute.CurrentDevice = orginDevie;

            //Dest
            foreach (Itineraray itineraray in _itineraries.Values)
            {
                Console.WriteLine("Current : " + nextRoute.CurrentDevice + "  itineraray org : " + itineraray.OriginDevice + " Dest org : " + itineraray.DestinationDevice);
                if (itineraray.OriginDevice.Equals(orginDevie) && sourceRoute.InvalidItineraray(itineraray.OriginDevice, itineraray.DestinationDevice))
                {
                    nextRoute.NextItineraray.Add(itineraray);

                    //최족 목적지가 아니라면 또 다음 itineraray 찾음
                    if (!itineraray.DestinationDevice.Equals(destDevice))
                    {
                        sourceRoute.NextRoute.Add(nextRoute);
                        sourceRoute = GetRoute(itineraray.DestinationDevice, destDevice, sourceRoute);
                        
                    }
                    else
                    {
                        sourceRoute.NextRoute.Add(nextRoute);
                    }
                }
            }
            return sourceRoute;
        }
*/
        #endregion
    }
    
}
