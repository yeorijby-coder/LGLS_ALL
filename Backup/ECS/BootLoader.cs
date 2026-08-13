using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using ECP.Boot;
using ECP.Db;
using ECP.Util.Client;
using ECP.Service.Logger;
using ECP.Global.Device;

using HECS.Global;
using HECS.Util;
using HECS.Gui.Monitor;
using HECS.Device;

using ECP.Util;
using ECP.Service.Statistics;
using HECS.Statistics;
using System.Data.SqlClient;
using HECS.Global.Interface;
using ECP.Device;
using ECP.Com.FieldBus;
using System.Configuration;

namespace HECS
{
    public class BootLoader
    {

        const string CLASS = "BootLoader";

        private Server server = null;

        private MonitorBuilder guiBuilder = null;


        public BootLoader()
        {
            Log.Init(CLASS);
            Log.SetLocalOutput(false);
            Load();
        }

        private void Load()
        {
            const string METHOD = CLASS + ".Load";

            try
            {
                // Lauch Process Objects
                if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                {
                    server = new Server();
                    Splasher.Status = "Stocker Service 시작합니다.";
                    Thread.Sleep(50);

                    //Remote Server로 Naming 객체 초기화
                    //server.Init(Environment.CurrentDirectory + @"\Stocker.exe.config");
                    //server.Init(Environment.CurrentDirectory + @"\HECS.exe.config");
                    //Local로 Naming 객체 초기화
                    server.Init();
                    Splasher.Status = "ECS를 초기화를 완료하였습니다.";
                    Thread.Sleep(50);
                    ECSDeviceManager.HostDeviceManager = Naming.GetDeviceManager();
                    // UI Build
                    //StockerMainForm form = new StockerMainForm();
                    //ECSMainForm form = new ECSMainForm();
                    //SimpleCache.AddObject("FormMain", "ECSMainForm", form);
                    //Splasher.Status = "Main Form을 생성을 시작합니다.";
                    //form.Show();
                }
                else
                {
                    //ECP.Util.Client.Naming.Init(Environment.CurrentDirectory + @"\HECS.exe.config");
                    server = new Server();
                    server.Init();
                    //server.Init(Environment.CurrentDirectory + @"\HECS.exe.config");
                    //ECSDeviceManager.HostDeviceManager = Naming.GetDeviceManager();
                    
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, METHOD, "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                MessageBox.Show("프로그램 기동에 실패 했습니다. \n\r 내용 : " + ex.StackTrace.ToString(), "확인요망", MessageBoxButtons.OK);
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                MessageBox.Show("프로그램 기동에 실패 했습니다. \n\r 내용 : " + ex.StackTrace.ToString(), "확인요망", MessageBoxButtons.OK);
                throw ex;
            }

        }


        public void EquipmentStartup()
        {
            Splasher.Status = "==================================";
            Splasher.Status = "Eqpipment Loading을 시작합니다.";
            Thread.Sleep(50);
            if (ECP.Global.GlobalConstant.START_MODE == "ECS")
            {

                Dictionary<string, Equipment> devices = Naming.GetDeviceManager().GetDeviceAll();

//한독 Data 주석 처리 (2009. 12. 28 )
//                AgvSEM agvSem = ECSDeviceManager.GetAGVSEM("AGVSEM:1");
//                agvSem.StartSocketDriver();

                Thread.Sleep(50);
                Splasher.Status = "==================================";
                Thread.Sleep(50);
                Splasher.Status = "ECS를 시작합니다.";
                Thread.Sleep(50);
                Splasher.Status = "==================================";
                Thread.Sleep(500);
                Splasher.Close();

                StatisticsCollector.TriggerOccured(
                                       OperationStatisticsCollectorData.COLLECTOR_ID,
                                       OperationStatisticsCollectorData.TRIGGER_APP_ID,
                                       new OperationStatisticsCollectorData("ECS", "START", "APP / START"));

                Thread.Sleep(3000);

                Dictionary<int, AbstractFieldBusDriver> fbdrivers = null;


                // ECS 부터 Startup

                foreach (string key in devices.Keys)
                {
                    Equipment eqp = (Equipment)devices[key];
                    if (eqp is IECS)
                    {
                        IECS ecs = eqp as IECS;
                        if (ecs != null)
                        {
                            ecs.ControllerStartUp();
                            fbdrivers = eqp.GetFieldBusDrivers();
                            eqp.FBdriverOpen();

                        }
                    }

                }


                // 일반 Equipments를 Startup
                foreach (string key in devices.Keys)
                {
                    Equipment eqp = (Equipment)devices[key];
                    if (!(eqp is IECS))
                    {
                        eqp.SetFieldBusDrivers(fbdrivers);
                        eqp.Startup();
                    }
                }

            }
        }

    }
}
