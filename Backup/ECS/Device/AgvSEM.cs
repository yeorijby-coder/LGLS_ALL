using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Statistics;
using HECS.Statistics;
using System.Windows.Forms;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Service.Logger;
using ECP.Interfaces.Unit;
using ECP.Com.Tcp;
using System.IO;
using System.Collections.Specialized;
using System.Configuration;
using ECP.Global;
using HECS.Device.Unit;
using System.ComponentModel;
using ECP.Service.Movement;
using ECP.Util.Server;
using HECS.Device.Dispatcher;
using System.Threading;
using ECP.Db.Entity;

namespace HECS.Device
{
    [Serializable]
    public class AgvSEM : TransportDevice
    {
        /// CLASS 명
        /// </summary>
        private const string CLASS = "AgvSEM";

        /// <summary>
        /// DB 저장 플래그
        /// </summary>
        private bool dirtyFlag = true;
        public bool DirtyFlag
        {
            get
            {
                return dirtyFlag;
            }
            set
            {
                dirtyFlag = value;
            }
        }

        #region Attribute_Persistance


        /// <summary>
        /// STOCKER MAKER 정보. MAKER별로 관리되어지는 ALARM정보가 다름.
        /// </summary>
        private string maker;
        public string Maker
        {
            get
            {
                return maker;
            }
            set
            {
                maker = value;
                this.dirtyFlag = true;
            }
        }
        private WMSSocketDriver agvDriver = null;

        public WMSSocketDriver AgvDriver
        {
            get
            {
                return agvDriver;
            }

        }
        #endregion Attribute_Persistance;

        #region Attribute_Not_Persistance

        #endregion Attribute_Not_Persistance
        TcpServer server = null;
        //StreamWriter logFile = null;
        private int tcpPort = 9900;

        public int TcpPort
        {
            get
            {
                return tcpPort;
            }
            set
            {
                tcpPort = value;
            }
        }
        System.Windows.Forms.Timer linkTimer = new System.Windows.Forms.Timer();
        #region Constructor

        /// <summary>
        /// 객체 생성자
        /// </summary>
        public AgvSEM()
        {
            //logFile = new StreamWriter(@"C:\AGV_LOG_" + System.DateTime.Now.ToString("yyyyMMdd") + ".log", true, Encoding.Default, 1);

        }

        /// <summary>
        /// 객체 생성자
        /// </summary>
        public AgvSEM(string stockerId)
            : base(stockerId)
        {
            //logFile =  new StreamWriter(@"C:\AGV_LOG_" + System.DateTime.Now.ToString("yyyyMMdd") + ".log", true, Encoding.Default, 1);
        }
        #endregion

        #region Manage_Persistent_Attribute

        /// <summary>
        /// DB에 객체 정보 Insert
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);


            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, METHOD, "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, METHOD, "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

        /// <summary>
        /// 객체 정보 DB에서 LOAD
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);

                this.Id = this.elementId;
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StockerSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "AgvSEM", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StockerSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "AgvSEM", "error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }

        /// <summary>
        /// 객체 정보를 DB에 update
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Save";
            string tSql = "";
            try
            {
                base.Save(theDB);

                //2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //if (dirtyFlag)
                //{
                //    this.dirtyFlag = false;

                //    tSql = " UPDATE SEM SET   HOSTID='" + this.hostID + "'," +
                //                    "MAKER='" + this.maker + "'," +
                //                    "Where ID='" + this.iD + "'";
                //    theDB.ExecuteUpdate(tSql);
                //}
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "AgvSEM", "sql error \n [" + tSql + "\n]" + ex.Message.ToString());
            }
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련

        public void StartSocketDriver()
        {

            server.Start();
        }
        public override void Init()
        {
            base.Init();
            AddSubScribe();
            linkTimer.Tick += new EventHandler(linkTimer_Tick);
            linkTimer.Interval = 60000;
            linkTimer.Enabled = true;
            linkTimer.Start();

        }

        void linkTimer_Tick(object sender, EventArgs e)
        {
            //WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.HostCommunicationState_621);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
            //command.ACK = "0";
            //SendClient(command.BinaryString);

            //for (int i = 1; i <= 3; i++)
            //{
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReport_619);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
            //    command.VehicleIdForState = "0" + i.ToString();
            //    command.ACK = "0";
            //    SendClient(command.BinaryString);
            //    Thread.Sleep(200);
            //}
        }

        public override void Startup()
        {
            const string METHOD = CLASS + ".Startup";
            base.Startup();
        }

        /// <summary>
        /// 프로그램 종료.
        /// </summary>
        public override void Shutdown()
        {
            base.Shutdown();
        }

        #endregion 초기화및기동관련

        #region Event_Handler
        /// <summary>
        /// Observable에 변경 이벤트 함수들을 연결.
        /// </summary>
        public void AddSubScribe()
        {
            try
            {
                ////////////////////   SAFTY SENSOR USED  Event Handling         ////////////////////
                //this.__I_HOME_SAFETY_SENSOR_USED.ObservableValueChanged += new DelegateObservableValueChanged(I_HOME_SAFETY_SENSOR_USED_ObservableValueChanged);

                ////////////////////   Observable Log Event Handling             ////////////////////

                //foreach (string key in this.Observables.Keys)
                //{
                //    Observables[key].ObservableLogEvent += new delegateObservableLogEvent(StockerSEM_ObservableLogEvent);
                //}

                if (agvDriver == null)
                {
                    NameValueCollection appSettings = ConfigurationManager.AppSettings;
                    if (int.TryParse(appSettings[ECPProperties.AGV_PORT], out tcpPort) == false)
                    {
                        tcpPort = 9900;
                    }

                    server = new TcpServer(tcpPort);

                    server.OnConnect += new TcpServer.ConnectionDelegate(server_OnConnect);
                    server.OnDisconnect += new TcpServer.ConnectionDelegate(server_OnDisconnect);
                    server.OnError += new TcpServer.ErrorDelegate(server_OnError);
                    server.OnListen += new TcpServer.ListenDelegate(server_OnListen);
                    server.OnRead += new TcpServer.ReadDelegate(server_OnRead);
                    //server.OnSendFile += new TcpServer.ConnectionDelegate(server_OnSendFile);
                    server.OnWrite += new TcpServer.ConnectionDelegate(server_OnWrite);
                }
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData(CLASS, "AddSubScribe", "", ex.ToString(), ex.StackTrace.ToString()));
                MessageBox.Show("Observable이 등록되지 않았습니다." + ex.ToString());
            }
        }

        void server_OnWrite(System.Net.Sockets.Socket soc)
        {

        }



        //TransferRequest_601 = 601,
        //TransferRequestAck_602 = 602,
        //TransferStart_603 = 603,
        //TransferStartAck_604 = 604,
        //VehicleAssign_605 = 605,
        //VehicleAssignAck_606 = 606,
        //VehicleArrivedPort_607 = 607,
        //VehicleArrivedPortAck_608 = 608,
        //PalletLoadComplete_609 = 609,
        //PalletLoadCompleteAck_610 = 610,
        //VehicleStartPort_611 = 611,
        //VehicleStartPortAck_612 = 612,
        //PalletUnloadComplete_613 = 613,
        //PalletUnloadCompleteAck_614 = 614,
        //VehicleReleased_615 = 615,
        //VehicleReleasedAck_616 = 616,
        //TransferComplete_617 = 617,
        //TransferCompleteAck_618 = 618,
        //VehicleStateReport_619 = 619,
        //VehicleStateReportAck_620 = 620,
        //HostCommunicationState_621 = 621,
        //HostCommunicationAck_622 = 622,
        //AlarmSetReport_623 = 623,
        //AlarmSetReportAck_624 = 624,
        //AlarmResetReport_625 = 625,
        //AlarmResetReportAck_626 = 626,
        //DateTimeSetReport_627 = 627,
        //DateTimeSetReportAck_628 = 628,
        //IllegalMessage_629 = 629,


        void server_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            string recv = string.Copy(receivedString);

            string[] message = null;
            message = recv.Split((char)2);
            for (int i = 0; i < message.Length; i++)
            {
                if (message[i].IndexOf((char)3, 0) < 0)
                {
                    continue;
                }
                WMSCommand recvCommand = new WMSCommand();
                recvCommand.BinaryString = ((char)2 + message[i]).PadRight(76, ' ');
                if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
                {
                    //WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
                    //errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
                    //errorCommand.ACK = "0";
                    //System.Console.WriteLine("Checksum Error:" + recvCommand.CommandId);
                    //SendClient(errorCommand.BinaryString);
                }
                else if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.SIZE)
                {
                    //if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
                    //{
                    //    WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
                    //    errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
                    //    errorCommand.ACK = "1";
                    //    //      logFile.WriteLine("Size Error:" + recvCommand.CommandId);
                    //    SendClient(errorCommand.BinaryString);

                    //}
                }
                HandleCommand(recvCommand);
                //System.Console.Flush();

            }

        }


        //void server_OnRead(System.Net.Sockets.Socket soc)
        //{
        //    string recv = string.Copy(server.ReceivedText);

        //    string[] message = null;
        //    message = recv.Split((char)2);
        //    for (int i = 0; i < message.Length; i++)
        //    {
        //        if (message[i].IndexOf((char)3, 0) < 0)
        //        {
        //            continue;
        //        }
        //        WMSCommand recvCommand = new WMSCommand();
        //        recvCommand.BinaryString = ((char)2 + message[i]).PadRight(76, ' ');
        //        if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
        //        {
        //            //WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
        //            //errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //            //errorCommand.ACK = "0";
        //            //System.Console.WriteLine("Checksum Error:" + recvCommand.CommandId);
        //            //SendClient(errorCommand.BinaryString);
        //        }
        //        else if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.SIZE)
        //        {
        //            //if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
        //            //{
        //            //    WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
        //            //    errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //            //    errorCommand.ACK = "1";
        //            //    //      logFile.WriteLine("Size Error:" + recvCommand.CommandId);
        //            //    SendClient(errorCommand.BinaryString);

        //            //}
        //        }
        //        HandleCommand(recvCommand);
        //        //System.Console.Flush();

        //    }

        //}

        void server_OnListen()
        {


        }

        void server_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {

        }

        void server_OnDisconnect(System.Net.Sockets.Socket soc)
        {

        }

        void server_OnConnect(System.Net.Sockets.Socket soc)
        {
            //WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.DateTimeSetReport_627);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
            //command.SAPCommandID = "0001";
            //command.CommandSequence = "0002";
            //command.DateTime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
            //SendClient(command.BinaryString);

            //foreach (Vehicle vehicle in Vehicles.Values)
            //{
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReport_619);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
            //    command.VehicleIdForState = vehicle.ElementId.Split(':')[1].PadLeft(2, '0');
            //    SendClient(command.BinaryString);
            //}

        }

        public void SendClient(string sendText)
        {
            for (int i = 0; i < server.ActiveConnections; i++)
            {
                server.SendText(sendText, i);
            }
        }

        private void HandleCommand(WMSCommand recvCommand)
        {

            //switch (recvCommand.CommandId)
            //{
            //    case "603":
            //        HandleCommand603(recvCommand);
            //        break;
            //    case "605":
            //        HandleCommand605(recvCommand);
            //        break;
            //    case "607":
            //        HandleCommand607(recvCommand);
            //        break;
            //    case "609":
            //        HandleCommand609(recvCommand);
            //        break;
            //    case "611":
            //        HandleCommand611(recvCommand);
            //        break;
            //    case "613":
            //        HandleCommand613(recvCommand);
            //        break;
            //    case "615":
            //        HandleCommand615(recvCommand);
            //        break;
            //    case "617":
            //        HandleCommand617(recvCommand);
            //        break;
            //    case "623":
            //        HandleCommand623(recvCommand);
            //        break;
            //    case "625":
            //        HandleCommand625(recvCommand);
            //        break;
            //    case "602":
            //        HandleCommand602(recvCommand);
            //        break;
            //    case "632":
            //        HandleCommand632(recvCommand);
            //        break;
            //    case "604":
            //    case "606":
            //    case "608":
            //    case "610":
            //    case "612":
            //    case "614":
            //    case "616":
            //    case "618":
            //    case "626":
            //        System.Console.WriteLine("===========================================================");
            //        System.Console.WriteLine("CommandId:" + recvCommand.CommandId);
            //        System.Console.WriteLine("Direction:" + recvCommand.Direction);
            //        System.Console.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
            //        System.Console.WriteLine("Sequence:" + recvCommand.Sequence);
            //        System.Console.WriteLine("Received From AGVC:" + recvCommand.CommandId + "=" + (recvCommand.ACK == "0" ? "[0]SUCCESS" : "[" + recvCommand.ACK + "]NAK"));
            //        System.Console.WriteLine("Message Binary:" + recvCommand.BinaryString);
            //        System.Console.WriteLine("Header:" + recvCommand.Header);
            //        System.Console.WriteLine("Body:" + recvCommand.Body);
            //        System.Console.WriteLine("===========================================================");
            //        //System.Console.Flush();

            //        break;
            //    case "620":
            //        HandleCommand620(recvCommand);
            //        break;
            //    case "622":
            //        System.Console.WriteLine("===========================================================");
            //        System.Console.WriteLine("CommunicationState");
            //        System.Console.WriteLine("CommandId:" + recvCommand.CommandId);
            //        System.Console.WriteLine("Direction:" + recvCommand.Direction);
            //        System.Console.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
            //        System.Console.WriteLine("Sequence:" + recvCommand.Sequence);
            //        System.Console.WriteLine("");
            //        System.Console.WriteLine("" + recvCommand.CommuncationState);
            //        System.Console.WriteLine("Message Binary:" + recvCommand.BinaryString);
            //        System.Console.WriteLine("Header:" + recvCommand.Header);
            //        System.Console.WriteLine("Body:" + recvCommand.Body);
            //        System.Console.WriteLine("===========================================================");
            //        //System.Console.Flush();

            //        break;
            //    case "628":
            //        System.Console.WriteLine("===========================================================");
            //        System.Console.WriteLine("DateTimeSet Request");
            //        System.Console.WriteLine("CommandId:" + recvCommand.CommandId);
            //        System.Console.WriteLine("Direction:" + recvCommand.Direction);
            //        System.Console.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
            //        System.Console.WriteLine("Sequence:" + recvCommand.Sequence);
            //        System.Console.WriteLine("");
            //        System.Console.WriteLine("" + recvCommand.ACK);
            //        System.Console.WriteLine("Message Binary:" + recvCommand.BinaryString);
            //        System.Console.WriteLine("Header:" + recvCommand.Header);
            //        System.Console.WriteLine("Body:" + recvCommand.Body);
            //        System.Console.WriteLine("===========================================================");
            //        //System.Console.Flush();

            //        break;
            //    case "629":
            //        System.Console.WriteLine("===========================================================");
            //        //System.Console.Flush();

            //        System.Console.WriteLine("Message Format Exception(Illegal Data Format)");
            //        System.Console.WriteLine("Direction:" + recvCommand.Direction);
            //        System.Console.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
            //        System.Console.WriteLine("Sequence:" + recvCommand.Sequence);
            //        System.Console.WriteLine("");
            //        System.Console.WriteLine("" + recvCommand.ACK + "=>" + (recvCommand.ACK == "0" ? "CheckSumError" : "SizeError"));
            //        System.Console.WriteLine("Message Binary:" + recvCommand.BinaryString);
            //        System.Console.WriteLine("Header:" + recvCommand.Header);
            //        System.Console.WriteLine("Body:" + recvCommand.Body);
            //        System.Console.WriteLine("===========================================================");
            //        //System.Console.Flush();

            //        break;
            //    default:
            //        System.Console.WriteLine("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
            //        System.Console.WriteLine("알수없는 Command : " + recvCommand.CommandId);
            //        System.Console.WriteLine("Direction:" + recvCommand.Direction);
            //        System.Console.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
            //        System.Console.WriteLine("Sequence:" + recvCommand.Sequence);
            //        System.Console.WriteLine("Message Binary:" + recvCommand.BinaryString);
            //        System.Console.WriteLine("Header:" + recvCommand.Header);
            //        System.Console.WriteLine("Body:" + recvCommand.Body);
            //        System.Console.WriteLine("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
            //        //System.Console.Flush();

            //        break;
            //}
            ////System.Console.Flush();
        }
       

        #endregion Method

    }
}
