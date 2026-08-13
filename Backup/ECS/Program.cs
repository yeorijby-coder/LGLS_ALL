using System;
using System.Collections.Generic;
using System.Windows.Forms;
using HECS.Global;
using HECS.Util;
using HECS.Gui.Monitor;
using System.Data.SqlClient;
using ECP.Service.Statistics;
using HECS.Statistics;
using System.Threading;
using ECP.Util.Server;
using System.Configuration;
using ECP.Boot;
using ECP.Global;

namespace HECS
{
    static class Program
    {

        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                LoadConfig();

                bool createAsnew = false;

                // 프로그램 중복 실행체크
                System.Threading.Mutex mutex = new Mutex(true, "ECS", out createAsnew);
                //SAPBridge.ConnectionInfo.SAPConnectString = ConfigurationManager.AppSettings["SAPConnectString"];
                //"CLIENT=100 USER=RCPCOM PASSWD=ECSCOM ASHOST=166.118.165.23 SYSNR=0";
                if (createAsnew)
                {
                    Application.ThreadException += new ThreadExceptionEventHandler(Application_ThreadException);
                    Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
                    AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
                    if (ECP.Global.GlobalConstant.START_MODE != "ECS")
                    {
                        //ECSClientMainForm form = new ECSClientMainForm();
                        DoStartupClient();
                    }
                    else
                    {
                        Splasher.Show();
                        //Naming.Init(Environment.CurrentDirectory + @"\HECS.exe.config");
                        DoStartup();

                        //                    Splasher.Close();
                    }
                    mutex.ReleaseMutex();
                }
                else
                {
                    MessageBox.Show("ECS가 이미 실행중입니다.", "ECS", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    Application.Exit();
                }
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("Program", "Main()", "[중요] Main Execption 발생 ", ex.ToString(), ex.StackTrace.ToString()));
            }

        }

        private static void LoadConfig()
        {
            string startMode = ConfigurationManager.AppSettings["StartMode"];
            //SAPBridge.ConnectionInfo.SAPConnectString = ConfigurationManager.AppSettings["SAPConnectString"];
            if (!string.IsNullOrEmpty(startMode))
            {
                if (startMode.Equals("ECS"))
                {
                    GlobalConstant.START_MODE = "ECS";
                }
                else
                {
                    GlobalConstant.START_MODE = "Client";
                }
            };

            string clientMode = ConfigurationManager.AppSettings["ClientType"];
            if (string.IsNullOrEmpty(clientMode) == false)
            {
                GlobalConstant.CLIENT_TYPE = ConfigurationManager.AppSettings["ClientType"];
            }
            else
            {
                GlobalConstant.CLIENT_TYPE = "";
            }

            string wmsHost = ConfigurationManager.AppSettings["WMS_HOST"];
            if (string.IsNullOrEmpty(wmsHost) == false)
            {
                GlobalConstant.WMS_HOST = ConfigurationManager.AppSettings["WMS_HOST"];
            }
            else
            {
                GlobalConstant.WMS_HOST = "127.0.0.1";
            }

            string wmsPort = ConfigurationManager.AppSettings["WMS_PORT"];
            if (string.IsNullOrEmpty(wmsHost) == false)
            {
                GlobalConstant.WMS_PORT = ConfigurationManager.AppSettings["WMS_PORT"];
            }
            else
            {
                GlobalConstant.WMS_PORT = "9910";
            }

            string ecsPort = ConfigurationManager.AppSettings["ECS_PORT"];
            if (string.IsNullOrEmpty(ecsPort) == false)
            {
                GlobalConstant.ECS_PORT = ConfigurationManager.AppSettings["ECS_PORT"];
            }
            else
            {
                GlobalConstant.ECS_PORT = "9911";
            }

            string plcIp = ConfigurationManager.AppSettings["PLC_IP"];
            if (string.IsNullOrEmpty(plcIp) == false)
            {
                GlobalConstant.PLC_IP = plcIp;
            }

            string plcPort = ConfigurationManager.AppSettings["PLC_PORT"];
            int plcPortValue;
            if (string.IsNullOrEmpty(plcPort) == false && int.TryParse(plcPort, out plcPortValue))
            {
                GlobalConstant.PLC_PORT = plcPortValue;
            }
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Console.WriteLine("[중요] Domain Application CurrentDomain_UnhandledException" + ((Exception)e.ExceptionObject).ToString());
            StatisticsCollector.TriggerOccured(
                                              RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                              RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                              new RunTimeExceptionStatisticsCollectorData("Program", "CurrentDomain_UnhandledException", "[중요] Domain Application CurrentDomain_UnhandledException", ((Exception)e.ExceptionObject).ToString(), ((Exception)e.ExceptionObject).StackTrace));

        }

        static void Application_ThreadException(object sender, ThreadExceptionEventArgs ex)
        {
            Console.WriteLine("[중요]Main Thread Exception 발생" + ex.Exception.ToString());
            StatisticsCollector.TriggerOccured(
                                              RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                              RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                              new RunTimeExceptionStatisticsCollectorData("Program", "Application_ThreadException", "[중요] Main Thread Exception 발생", ex.Exception.ToString(), ex.Exception.StackTrace));

        }

        static void DoStartup()
        {
            bool bootSuccess = false;
            BootLoader loader;

            try
            {
                loader = new BootLoader();
                loader.EquipmentStartup();
                bootSuccess = true;
            }
            catch (SqlException ex)
            {
                bootSuccess = false;
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("Main", "Load", "Loading SQL Error로 인해 Application을 종료 합니다.", ex.ToString(), ex.StackTrace.ToString()));
            }
            catch (Exception ex)
            {
                bootSuccess = false;
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("Main", "Load", "Loading Error로 인해 Application을 종료 합니다.", ex.ToString(), ex.StackTrace.ToString()));
            }



            if (bootSuccess)
            {
                Splasher.Status = "Main Form을 시작합니다.";
                Splasher.Status = "==================================";
                Thread.Sleep(50);
                ECSMainForm form = new ECSMainForm();
                SimpleCache.AddObject("Form", "MainForm", form);
                form.WindowState = FormWindowState.Maximized;
                try
                {
                    Application.Run(form);
                }
                catch (Exception ex)
                {
                    StatisticsCollector.TriggerOccured(
                                                      RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                      RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                      new RunTimeExceptionStatisticsCollectorData("Main", "ERROR", "MainForm Open에 심각한 문제가 발생하여 종료합니다.", ex.ToString(), ex.StackTrace.ToString()));
                }
            }

        }

        static void DoStartupClient()
        {
            bool bootSuccess = false;
            BootLoader loader;

            try
            {
                 loader = new BootLoader();
                //loader.EquipmentStartup();
                //ECP.Util.Client.Naming.Init(Environment.CurrentDirectory + @"\HECS.exe.config");
                //Server server = new Server();
                //server.Init();
                bootSuccess = true;
            }
            catch (SqlException ex)
            {
                bootSuccess = false;
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("Main", "Load", "Loading SQL Error로 인해 Application을 종료 합니다.", ex.ToString(), ex.StackTrace.ToString()));
            }
            catch (Exception ex)
            {
                bootSuccess = false;
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("Main", "Load", "Loading Error로 인해 Application을 종료 합니다.", ex.ToString(), ex.StackTrace.ToString()));
            }



            if (bootSuccess)
            {
                ECSClientMainForm form = new ECSClientMainForm();

                try
                {
                    Application.Run(form);
                }
                catch (Exception ex)
                {
                    StatisticsCollector.TriggerOccured(
                                                      RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                      RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                      new RunTimeExceptionStatisticsCollectorData("Main", "ERROR", "MainForm Open에 심각한 문제가 발생하여 종료합니다.", ex.ToString(), ex.StackTrace.ToString()));
                }
            }

        }
    }
}