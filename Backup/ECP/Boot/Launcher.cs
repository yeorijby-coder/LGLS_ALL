using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

using ECP.Global;
using ECP.Device;

using ECP.Service;
using ECP.Service.Material;
using ECP.Service.Logger;
using ECP.Service.User;

using ECP.Service.EventBroker;
using ECP.Service.Device;
using ECP.Service.Gui;
using ECP.Service.Statistics;

using ECP.Service.Movement;
using ECP.Service.Interface;

using ECP.Util.Client;
using ECP.Service.Alarm;
using ECP.Service.Route;
using ECP.Db.Entity;
using ECP.Service.DataManagement;


namespace ECP.Boot
{
    /// <summary>
    /// Launcher Class는 모든 Manager들을 Load한다.
    /// Launcher Class는 시스템에 필요한 모든 서비스에 대하여
    /// 책임이 가진 Manager들을 Data Base로부터 Load한다.
    /// 
    /// Lancher Class들에 어떤 Manager들을 Configration을 통하여
    /// 할당할것인지에 문제는 미래의 확장성을 고려하여
    /// 설계에 반영 할 것이다.
    /// </summary>
    public class Launcher : ECPObject
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "Launcher";

        #region Attribute

        /// <summary>
        /// 이 Laucher에 할당된 Manager들을 담는 Heap Container
        /// </summary>
        private List<Manager> managers = new List<Manager>();

        #endregion Attribute

        #region Constructor

        /// <summary>
        /// Launcher의 생성자
        /// 모든 Manager을 Load함
        /// </summary>
        public Launcher()
        {
            LoadAllManagers();
        }

        #endregion Constructor

        #region Method

        /// <summary>
        /// 이 Lanucher에 할당된 모든 Manager을 Data base의 정보로 부터 Load함
        /// 모든 Device는 DeviceManager가 Load되어지면서 같이 Load되어짐
        /// </summary>
        public void LoadAllManagers()
        {
            // Logging 서비스를 담당하는 LogManager를 기동한다.
            managers.Add(new LogManager("launcher", "LogManager"));

            // User관리 서비스를 담당하는 UserManager을 기동한다.
            managers.Add(new UserManager("launcher", "UserManager"));

            // Event(Publish and Subscribe)을 담당하는 EventBrokerManager을 기동한다.
            managers.Add(new EventBrokerManager("launcher", "EventBrokerManager"));

            // Client GUI화면의 Widget정보를 관리한 GUIManager를 기동한다.
   //         managers.Add(new GUIManager("launcher", "GUIManager"));

            // 통계성 Data 서비스를 담당하는 StatisticsManager를 기동한다.

            if (GlobalConstant.START_MODE != EntityEnumData.STARTMODE.Client.ToString())
            {
                // Carrier(Material) 서비스를 담당하는 CarrierManger를 기동한다.
                managers.Add(new CarrierManager("launcher", "CarrierManager"));

                // 모든 장비를 관리하는 서비스를 담당하는 DeviceManger를 기동한다.
                managers.Add(new DeviceManager("launcher", "DeviceManager"));
 
//LGLS 주석 처리(2009.12.28)
                // External I/F를 관리하는 서비스를 담당하는 ExternalIFManager를 기동한다.
                managers.Add(new ExternalIFManager("launcher", "ExternalIFManager"));
                //LGLS 주석 처리(2009.12.28)
                // Internal I/F를 관리하는 서비스를 담당하는 InternalIFManager를 기동한다.
//                managers.Add(new InternalIFManager("launcher", "InternalIFManager"));

                // Data Purger를 관리하는 서비스를 담당하는 DataManager를 기동한다.
                  managers.Add(new DataManager("launcher", "DataManager"));
            }

            managers.Add(new StatisticsManager("launcher", "StatisticsManager"));


            //LGLS 주석 처리(2009.12.28)
            // Alarm을 관리하는 서비스를 담당하는 AlarmManager를 기동한다.
//            managers.Add(new AlarmManager("launcher", "AlarmManager"));

            // Route를 관리하는 서비스를 담당하는 RouteManager를 기동한다.
            managers.Add(new RouteManager("launcher", "RouteManager"));

            // Movement를 관리하는 서비스를 담당하는 MovementManager를 기동한다.
            managers.Add(new MovementManager("launcher", "MovementManager"));

        }

        /// <summary>
        /// 제공된 Manager Id에 해당하는 Manager의 Reference을 Return
        /// </summary>
        /// <param name="managerId"></param>
        /// <returns></returns>
       
        public Manager GetManager(string managerId)
        {
      
            foreach (Manager mgr in managers)
            {
                if(mgr.GetId().Equals(managerId))
                    return mgr;
            }
            return null;
        }
        public bool  SetManager(string managerId, Manager manager)
        {
            if (manager == null)
            {
                return false; 
            }
            if (string.IsNullOrEmpty(managerId))
            {
                return false;
            }

            int replaceIndex = -1;
            for (int i = 0; i < managers.Count; i++ )
            {
                if (managers[i].GetId() == managerId)
                {
                    replaceIndex = i;
                    break;
                }
            }
            if(replaceIndex == -1)
            {
                return false;
            }
            managers[replaceIndex] = manager;
            return true;
        }

        public override bool IsAlive()
        {
            return true;
        }
        #endregion Method
    }
}
