using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Configuration;
using System.Collections.Specialized;

using ECP.Service;
using ECP.Service.Logger;
using ECP.Service.Material;
using ECP.Service.EventBroker;
using ECP.Service.Device;
using ECP.Service.Gui;
using ECP.Service.Statistics;
using ECP.Service.User;
using ECP.Service.Alarm;
using ECP.Service.Route;
using ECP.Service.Movement;

using ECP.Boot;
using ECP.Global;
using ECP.Event;
using ECP.Service.Interface;
using System.ComponentModel;
using ECP.Service.DataManagement;


namespace ECP.Util.Client
{
    /// <summary>
    /// ECP Platform의 Naming Service Class
    /// </summary>
    public class Naming
    {
        /// <summary>
        /// Naming Class의 Instance
        /// </summary>
        public static Naming instance = new Naming();

        /// <summary>
        /// Carrier Manager의 Id
        /// </summary>
        public static readonly string CarrierManager_ID = "CarrierManager";

        /// <summary>
        /// User Manager의 Id
        /// </summary>
        public static readonly string UserManager_ID = "UserManager";

        /// <summary>
        ///  Log Manager의 Id
        /// </summary>
        public static readonly string LogManager_ID = "LogManager";

        /// <summary>
        /// EventBroker Manger의 Id
        /// </summary>
        public static readonly string EventBrokerManager_ID = "EventBrokerManager";

        /// <summary>
        /// GUIManger의 Id
        /// </summary>
        public static readonly string GUIManager_ID = "GUIManager";

        /// <summary>
        /// StatisticsManager의 Id
        /// </summary>
        public static readonly string StatisticsManager_ID = "StatisticsManager";

        /// <summary>
        /// Device Manager의 Id
        /// </summary>
        public static readonly string DeviceManager_ID = "DeviceManager";

        /// <summary>
        /// Alarm Manager의 Id
        /// </summary>
        public static readonly string AlarmManager_ID = "AlarmManager";


        /// <summary>
        /// Alarm Manager의 Id
        /// </summary>
        public static readonly string RouteManager_ID = "RouteManager";

        /// <summary>
        /// Movement Manager의 Id
        /// </summary>
        public static readonly string MovementManager_ID = "MovementManager";

        /// <summary>
        /// External If Manager의 Id
        /// </summary>
        public static readonly string ExternalIFManager_ID = "ExternalIFManager";

        /// <summary>
        /// Internal If Manager의 Id
        /// </summary>
        public static readonly string InternalIFManager_ID = "InternalIFManager";

        /// <summary>
        /// Data Manager의 Id
        /// </summary>
        public static readonly string DataManager_ID = "DataManager";


        /// <summary>
        /// Launcher의 Instance
        /// </summary>
        public static Launcher launcher = null;

        /// <summary>
        /// Naming Service의 Instance을 Return
        /// </summary>
        /// <returns></returns>
        public static Naming GetInstance()
        {
            return instance;
        }

        /// <summary>
        /// Naming의 초기화
        /// </summary>
        public static void Init()
        {
        }

        /// <summary>
        /// 주어진 환경 파일로 Naming의 초기화
        /// Laucher의 Remote Instance Reference을 가져와서 Local Reference에 초기화
        /// </summary>
        /// <param name="remoteConfigFileName"></param>
        public static void Init(string remoteConfigFileName)
        {
            RemotingConfiguration.Configure(remoteConfigFileName, false);
            //RemotingConfiguration.RegisterWellKnownServiceType(typeof(PropertyChangedEventHandler), "RemoteEvent", WellKnownObjectMode.Singleton); 

            NameValueCollection appSettings =  ConfigurationManager.AppSettings;

            launcher = (Launcher)Activator.GetObject(typeof(Launcher),
                "tcp://" + appSettings[ECPProperties.NamingHost].ToString() + ":" +
                appSettings[ECPProperties.NamingPort].ToString() + "/Launcher");
        }

        /// <summary>
        /// Naming의 Launcher객체를 Remote가 아닌 Local 객체로 초기와 함
        /// </summary>
        /// <param name="localLauncher"></param>
        public static void Init(Launcher localLauncher)
        {
            launcher = localLauncher;
        }

        /// <summary>
        /// CarrierManger의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static CarrierManager GetCarrierManager()
        {
            return (CarrierManager)launcher.GetManager(CarrierManager_ID);
        }

        /// <summary>
        /// LogManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static LogManager GetLogManager()
        {
            return (LogManager)launcher.GetManager(LogManager_ID);
        }

        /// <summary>
        /// UserManager의 Reference를 Return
        /// </summary>
        /// <returns></returns>
        public static UserManager GetUserManager()
        {
            return (UserManager)launcher.GetManager(UserManager_ID);
        }

        /// <summary>
        /// EventBrokerManger의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static EventBrokerManager GetEventBrokerManager()
        {
            return (EventBrokerManager)launcher.GetManager(EventBrokerManager_ID);
        }

        /// <summary>
        /// GUIManger의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static GUIManager GetGUIManager()
        {
            return (GUIManager)launcher.GetManager(GUIManager_ID);
        }

        /// <summary>
        /// StatisticsManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static StatisticsManager GetStatisticsManager()
        {
            return (StatisticsManager)launcher.GetManager(StatisticsManager_ID);
        }

        /// <summary>
        /// DeviceManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static DeviceManager GetDeviceManager()
        {
            return (DeviceManager)launcher.GetManager(DeviceManager_ID);
        }

        /// <summary>
        /// AlarmManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static AlarmManager GetAlarmManager()
        {
            return (AlarmManager)launcher.GetManager(AlarmManager_ID);
        }

        /// <summary>
        /// RouteManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static RouteManager GetRouteManager()
        {
            return (RouteManager)launcher.GetManager(RouteManager_ID);
        }
        public static bool SetDeviceManager(DeviceManager manager)
        {
            return launcher.SetManager(DeviceManager_ID, manager);
        }

        /// <summary>
        /// MovementManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static MovementManager GetMovementManager()
        {
            return (MovementManager)launcher.GetManager(MovementManager_ID);
        }

        /// <summary>
        /// External If Manager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static ExternalIFManager GetExternalIFManager()
        {
            return (ExternalIFManager)launcher.GetManager(ExternalIFManager_ID);
        }

        /// <summary>
        /// InternalIFManager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static InternalIFManager GetInternalIFManager()
        {
            return (InternalIFManager)launcher.GetManager(InternalIFManager_ID);
        }

        /// <summary>
        /// Data Manager의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static DataManager GetDataManager()
        {
            return (DataManager)launcher.GetManager(DataManager_ID);
        }

        /// <summary>
        /// Launcher의 Reference을 Return
        /// </summary>
        /// <returns></returns>
        public static Launcher GetLauncher()
        {
            return launcher;
        }

        /// <summary>
        /// 주어진 subjectId의 ECPEvent를 Publish
        /// </summary>
        /// <param name="subject">subject Id</param>
        /// <param name="ecpEvent">ECPEvent Reference</param>
        public static void PublishEvent(string subject, ECPEvent ecpEvent)
        {
            GetEventBrokerManager().Publish(subject, ecpEvent);
        }

    }
}
