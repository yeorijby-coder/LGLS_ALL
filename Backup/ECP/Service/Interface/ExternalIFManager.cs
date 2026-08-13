using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using System.Timers;
using ECP.Db.Entity;
using System.Data.SqlClient;
using ECP.Global;
using ECP.Util;
using ECP.Com.Tcp;
using ECP.BizRule;
using ECP.Service.Movement;

namespace ECP.Service.Interface
{
    /// <summary>
    /// External I/F를 관리 함.
    /// </summary>
    public class ExternalIFManager : Manager
    { 
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "ExternalIFManager";

        private static TcpServer tcpServer = null;
        private static TcpClient tcpClient = null;

        //SAP I/F check
        Timer timerServer = new Timer();
        Timer timerReorder = new Timer();
        Timer timerMode = new Timer();
        Timer timerStatus = new Timer();
        Timer timerRegStatus = new Timer();
        Timer timerError = new Timer();
        Timer timerComplete = new Timer();

        protected static Boolean timerReorderFlag = false;
        protected static Boolean timerModeFlag = false;
        protected static Boolean timerServerFlag = false;
        protected static Boolean timerStatusFlag = false;
        protected static Boolean timerRegStatusFlag = false;
        protected static Boolean timerErrorFlag = false;
        protected static Boolean timerCompleteFlag = false;

        //장비 상태 정보 관리를 위한 변수
        protected static string checkStatusString = "0100000020000003000000400000050000000000";

        /// <summary>
        /// 전체 Socket Client 정보를 담고 있는 Container
        /// </summary>
        private static List<TcpClient> clientList = new List<TcpClient>();


        /// <summary>
        /// ExternalIFManager 생성자
        /// 주어진 Owner Id와 Manager Id로 ExternalIFManager를 생성한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public ExternalIFManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.init();
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
        ///0. 초기 Initialize 메소드
        ///  :처음 manager가 생성될 경우에 반송 정도 등록
        ///   -> TB_TRANSFER 테이블에 미반송 정보를 읽어서 Que에 저장함.
        ///   -> TB_TRANSFERDETAIL을 확인하여 반송 쪼개기 여부 변경
        ///  :Batch Job 구동
        ///   -> SAP I/F 반송 지시 Check timer 구동
        ///   -> 세부 반송 지시 Check timer 구동
        /// </summary>

        protected void init()
        {
            this.startBatchJob();
        }

        /// <summary>
        /// Batch Job 구동 메소드
        ///   -> SAP I/F 반송 지시 Check timer 구동
        ///   -> 세부 반송 지시 Check timer 구동
        /// </summary>
        protected void startBatchJob()
        {
            //Socket 통신 연결
            try
            {
                startSocketServer();
                startSocketClient();
            }
            catch (Exception ex)
            {
                //Pass
            }

            timerReorder.Interval = 5000;
            timerMode.Interval = 1000;
            timerServer.Interval = 5000;
            timerStatus.Interval = 5000;
            timerRegStatus.Interval = 1000;
            timerError.Interval = 5000;
            timerComplete.Interval = 5000;

            timerReorder.Start();
            timerMode.Start();
            timerServer.Start();
            timerStatus.Start();
            timerRegStatus.Start();
            timerError.Start();
            timerComplete.Start();

            timerReorder.Elapsed += new ElapsedEventHandler(timerReorder_Elapsed);
            timerMode.Elapsed += new ElapsedEventHandler(timerMode_Elapsed);
            timerServer.Elapsed += new ElapsedEventHandler(timerServer_Elapsed);
            timerStatus.Elapsed += new ElapsedEventHandler(timerStatus_Elapsed);
            timerRegStatus.Elapsed += new ElapsedEventHandler(timerRegStatus_Elapsed);
            timerError.Elapsed += new ElapsedEventHandler(timerError_Elapsed);
            timerComplete.Elapsed += new ElapsedEventHandler(timerComplete_Elapsed);
        }

        void timerReorder_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerReorderFlag)
            {
                timerReorder.Stop();
                timerReorderFlag = true;

                try
                {
                    checkReorder();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerReorderFlag = false;
                    timerReorder.Start();
                }
            }
        }

        void timerMode_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerModeFlag)
            {
                timerMode.Stop();
                timerModeFlag = true;

                try
                {
                    checkMode();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerModeFlag = false;
                    timerMode.Start();
                }
            }
        }

        void timerServer_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerServerFlag)
            {
                timerServer.Stop();
                timerServerFlag = true;

                try
                {
                    checkServerConnecting();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerServerFlag = false;
                    timerServer.Start();
                }
            }
        }

        void timerStatus_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerStatusFlag)
            {
                timerStatus.Stop();
                timerStatusFlag = true;

                try
                {
                    //장비 상태 I/F
                    checkStatus();
               }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerStatusFlag = false;
                    timerStatus.Start();
                }
            }
        }

        void timerRegStatus_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerRegStatusFlag)
            {
                timerRegStatus.Stop();
                timerRegStatusFlag = true;

                try
                {
                    //장비 상태 등록
                    regEqpStatus();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerRegStatusFlag = false;
                    timerRegStatus.Start();
                }
            }
        }

        void timerError_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerErrorFlag)
            {
                timerError.Stop();
                timerErrorFlag = true;

                try
                {
                    checkError();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerErrorFlag = false;
                    timerError.Start();
                }
            }
        }

        void timerComplete_Elapsed(object sender, ElapsedEventArgs e)
        {
            if (!timerCompleteFlag)
            {
                timerComplete.Stop();
                timerCompleteFlag = true;

                try
                {
                    checkComplete();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerCompleteFlag = false;
                    timerComplete.Start();
                }
            }
        }

        //start Server
        public void startSocketServer()
        {
            if (tcpServer != null)
            {
                if (tcpServer.MListening)
                {
                    tcpServer.Stop();
                }

                tcpServer = null;
            }

            int TcpServerPort = int.Parse(GlobalConstant.ECS_PORT);

            tcpServer = new TcpServer(TcpServerPort);

            tcpServer.OnConnect += new TcpServer.ConnectionDelegate(tcpServer_OnConnect);
            tcpServer.OnDisconnect += new TcpServer.ConnectionDelegate(tcpServer_OnDisconnect);
            tcpServer.OnError += new TcpServer.ErrorDelegate(tcpServer_OnError);
            tcpServer.OnListen += new TcpServer.ListenDelegate(tcpServer_OnListen);
            tcpServer.OnRead += new TcpServer.ReadDelegate(tcpServer_OnRead);
            tcpServer.OnSendFile += new TcpServer.ConnectionDelegate(tcpServer_OnSendFile);
            tcpServer.OnWrite += new TcpServer.ConnectionDelegate(tcpServer_OnWrite);

            tcpServer.Start();
        }

        void tcpServer_OnConnect(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnDisconnect(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnListen()
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            //ETX일 경우 Pass
            if (receivedString.Length == 1)
            {
                if (char.Parse(receivedString) == (char)3)
                {
                    return;
                }
            }

            string strReceived = string.Copy(receivedString);
            string strDisplay = "";
            WMSCommand rtnCommand = null;
            WMSCommand recvCommand = null;

            if (!string.IsNullOrEmpty(strReceived))
            {
                strDisplay = strReceived.Replace((char)2, GlobalConstant.START_CHAR);
                strDisplay = strDisplay.Replace((char)3, GlobalConstant.END_CHAR);
            }

            string[] message = null;
            message = strReceived.Split((char)2);

            if (message.Length != 2)   //message 사이즈는 2가 되어야 함.
            {
                recvCommand = new WMSCommand();
                rtnCommand = recvCommand.errorReplyMessage();
            }
            else
            {
                //Check message Data
                recvCommand = new WMSCommand(message);
                recvCommand.BinaryString = strReceived;

                rtnCommand = recvCommand.checkMessage();
            }

            if (tcpServer != null)
            {
                int iCnt = tcpServer.ActiveConnections;

                if (iCnt > 0)
                {
                    for (int i = 0; i < iCnt; i++)
                    {
                        tcpServer.SendText(rtnCommand.BinaryString, i);
                    }
                }
            }

            //Make Data ACK Data에 대해서만 Data를 만들어야 함.
            if (rtnCommand.ReplyACK == "A")
            {
                ExternalIfData ifData = new ExternalIfData();
                ifData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                ifData.Externaliftype = recvCommand.MessageType.ToString();
                ifData.Ifvalue = strDisplay;
                ifData.Transstarttime = ifData.Createtime;

                ifData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                int iCnt = 0;
                iCnt = ifData.getExternalIfDataCount(ifData.Externaliftype, ifData.Ifvalue);

                if (iCnt < 1)
                {
                    ifData.insertExternalIfData();
                }
                else
                {
                    if (ifData.Externaliftype == "M")
                    {
                        ifData.insertExternalIfData();
                    }
                }
            }
        }

        //void tcpServer_OnRead(System.Net.Sockets.Socket soc)
        //{
        //    string strReceived = string.Copy(tcpServer.ReceivedText);
        //    string strDisplay = "";

        //    if (!string.IsNullOrEmpty(strReceived))
        //    {
        //        strDisplay = strReceived.Replace((char)2, GlobalConstant.START_CHAR);
        //        strDisplay = strDisplay.Replace((char)3, GlobalConstant.END_CHAR);

        //        strDisplay = strDisplay.Remove(strDisplay.Length - 1);
        //        strReceived = strReceived.Remove(strReceived.Length - 1);
        //    }

        //    string[] message = null;
        //    message = strReceived.Split((char)2);

        //    //Check message Data
        //    WMSCommand recvCommand = new WMSCommand(message);
        //    recvCommand.BinaryString = strReceived;
            
        //    WMSCommand rtnCommand = null;
        //    rtnCommand = recvCommand.checkMessage();


        //    if (tcpServer != null)
        //    {
        //        int iCnt = tcpServer.ActiveConnections;

        //        if (iCnt > 0)
        //        {
        //            for (int i = 0; i < iCnt; i++)
        //            {
        //                tcpServer.SendText(rtnCommand.BinaryString, i);
        //            }
        //        }
        //    }

        //    //Make Data ACK Data에 대해서만 Data를 만들어야 함.
        //    if (rtnCommand.ReplyACK == "A")
        //    {
        //        ExternalIfData ifData = new ExternalIfData();
        //        ifData.Createtime = DateTime.Now.ToString("yyyymmddhhmmssfff");
        //        ifData.Externaliftype = recvCommand.MessageType.ToString();
        //        ifData.Ifvalue = strDisplay;
        //        ifData.Transstarttime = ifData.Createtime;

        //        ifData.Transcompletetime = GlobalConstant.SET_MAXTIME;

        //        int iCnt = 0;
        //        iCnt = ifData.getExternalIfDataCount(ifData.Externaliftype, ifData.Ifvalue);

        //        if (iCnt < 1)
        //        {
        //            ifData.insertExternalIfData();
        //        }
        //    }
        //}

        void tcpServer_OnSendFile(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnWrite(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }


        //Check Server Connecting
        public void checkServerConnecting()
        {
            if (tcpServer != null)
            {
                if (!tcpServer.MListening)
                {
                    tcpServer = null;
                    startSocketServer();
                }
                else
                {
                    return;
                }
            }
            else
            {
                startSocketServer();
            }
        }


        //start Server
        public void startSocketClient()
        {
            if (tcpClient == null)
            {
                string TcpClientIp = GlobalConstant.WMS_HOST;
                int TcpClientPort = int.Parse(GlobalConstant.WMS_PORT);

                tcpClient = new TcpClient(TcpClientIp, TcpClientPort);

                tcpClient.OnConnect += new TcpClient.ConnectionDelegate(tcpClient_OnConnect);
                tcpClient.OnDisconnect += new TcpClient.ConnectionDelegate(tcpClient_OnDisconnect);
                tcpClient.OnError += new TcpClient.ErrorDelegate(tcpClient_OnError);
                tcpClient.OnRead += new TcpClient.ReadDelegate(tcpClient_OnRead);
                tcpClient.OnWrite += new TcpClient.ConnectionDelegate(tcpClient_OnWrite);
                tcpClient.Connect();
            }
            else
            {
                if (!tcpClient.Connected)
                {
                    tcpClient.Connect();
                }
            }
            
        }


        void tcpClient_OnConnect(System.Net.Sockets.Socket soc)
        {
            //
        }

        void tcpClient_OnDisconnect(System.Net.Sockets.Socket soc)
        {
            //
        }

        void tcpClient_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {
            //
            startSocketClient();
        }

        void tcpClient_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            //
        }

        void tcpClient_OnWrite(System.Net.Sockets.Socket soc)
        {
            //
        }

        //Check Reorder
        public void checkReorder()
        {
            ExternalIfData wmsIfData = new ExternalIfData();
            List<ExternalIfData> list = wmsIfData.getNotCompleteWmsReorderIfDataList();

            foreach (ExternalIfData reorderData in list)
            {
                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();

                    string jobno = reorderData.Ifvalue.Substring(18, 4);
                    string bankno = reorderData.Ifvalue.Substring(35, 2);
                    string bayno = reorderData.Ifvalue.Substring(37, 3);
                    string levelno = reorderData.Ifvalue.Substring(40, 2);
                    string reordertype = reorderData.Ifvalue.Substring(42, 1);


                    //재입고의 경우만 처리함.
                    if (reordertype == "1")
                    {
                        TransferDetailData tfdData = new TransferDetailData();

                        try
                        {
                            string now = DateTime.Now.ToString("yyyyMMdd");

                            //반송 명령의 To Sub Location 수정
                            Dictionary<string, TransferData> transfers = MovementManager.getTransferDataList();
                            TransferData tfNewData = new TransferData(transfers[now + jobno]);

                            //반송 명령이 존재해야 함.
                            if (!string.IsNullOrEmpty(tfNewData.Commandid))
                            {
                                tfdData.getTransferDetailInGoLastData(now + jobno, tfNewData.Tolocationid);

                                //Detail 반송 명령이 존재해야 함.
                                if (!string.IsNullOrEmpty(tfdData.Commandid))
                                { 
                                    theDB.Start();

                                    tfNewData.Tosublocationid = bankno + bayno.Substring(1, 2) + levelno;
                                    tfNewData.updateTransferData(theDB, true);

                                    //반송 Detail 명령 상태 수정
                                    tfdData.Transferdetailstatus = EntityEnumData.TRANSFERDETAILSTATUS.Wait.ToString();
                                    tfdData.Executetime = "";
                                    tfdData.Palletidtomove = jobno;

                                    tfdData.updateTransferDetailData(theDB, true);

                                    theDB.Commit();

                                    //Daemon 수정
                                    MovementManager.changeTranferData(tfNewData);
                                }
                            }
                        }
                        catch (Exception exe)
                        {
                            //Error Log 관리
                            if (theDB != null) theDB.Rollback();
                            throw exe;
                        }
                    }

                    reorderData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    reorderData.updateExternalIfData();
                }
                catch (SqlException se)
                {
                    //Error Log 관리
                    if (theDB != null) theDB.Rollback();
                }
                catch (Exception ex)
                {
                    reorderData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                    reorderData.updateExternalIfData();
                    throw ex;
                }
                finally
                {
                    if (theDB != null)
                    {
                        DBControl.RemoveDBConection(theDB);
                    }
                }
            }
        }

        //Check Mode
        public void checkMode()
        {
            ExternalIfData wmsIfData = new ExternalIfData();
            List<ExternalIfData> list = wmsIfData.getNotCompleteWmsModeChangeDataList();

            foreach (ExternalIfData modeData in list)
            {
                DBConnection theDB = null;
                string conveyorNo = "";
                string conveyorMode = "";
                int iCnt = 0;

                conveyorNo = modeData.Ifvalue.Substring(17, 3);
                conveyorMode = modeData.Ifvalue.Substring(20, 1);

                if (conveyorNo == GlobalConstant.CONVEYOR11_WMS_CODE)
                {
                    if (GlobalConstant.CONVEYOR11_MODE != conveyorMode)
                    {
                        TransferDetail tfDetail = new TransferDetail();
                        if (conveyorMode == GlobalConstant.RGV_INGO)
                        {
                            iCnt = tfDetail.getCheckOutTransferToCNV11();
                        }
                        else
                        {
                            iCnt = tfDetail.getCheckInTransferFromCNV11();
                        }

                        if (iCnt == 0)
                        {
                            GlobalConstant.CONVEYOR11_MODE = conveyorMode;
                        }
                    }
                }

                try
                {
                    theDB = DBControl.GetConnection();

                    modeData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    modeData.updateExternalIfData();
                }
                catch (SqlException se)
                {
                    //Error Log 관리
                    if (theDB != null) theDB.Rollback();
                }
                catch (Exception ex)
                {
                    modeData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                    modeData.updateExternalIfData();
                    throw ex;
                }
                finally
                {
                    if (theDB != null)
                    {
                        DBControl.RemoveDBConection(theDB);
                    }
                }
            }
        }

        //Check Status
        public void checkStatus()
        {
            ExternalIfData wmsIfData = new ExternalIfData();
            List<ExternalIfData> list = wmsIfData.getNotCompleteStatusDataList();

            foreach (ExternalIfData statusData in list)
            { 
                bool bFlag = false;
                DBConnection theDB = null;
                int iCnt = 0;

            JUMP1:

                iCnt += 1;
                bFlag = sendMessageToWms(statusData.Ifvalue);

                if (bFlag)
                {
                    try
                    {
                        theDB = DBControl.GetConnection();

                        statusData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        statusData.updateExternalIfData();
                    }
                    catch (SqlException se)
                    {
                        //Error Log 관리
                        if (theDB != null) theDB.Rollback();
                    }
                    catch (Exception ex)
                    {
                        statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                        statusData.updateExternalIfData();
                        throw ex;
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                else 
                {
                    if (iCnt > 3)
                    {
                        try
                        {
                            statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                            statusData.updateExternalIfData();
                        }
                        catch (Exception ex)
                        {
                          //
                        }

                        continue;
                    }
                    else
                    {
                        goto JUMP1;
                    }
                }
            }
        }

        //Check Status
        public void checkError()
        {
            ExternalIfData wmsIfData = new ExternalIfData();
            List<ExternalIfData> list = wmsIfData.getNotCompleteErrorDataList();

            foreach (ExternalIfData statusData in list)
            {
                bool bFlag = false;
                DBConnection theDB = null;
                int iCnt = 0;

            JUMP1:

                iCnt += 1;
                bFlag = sendMessageToWms(statusData.Ifvalue);

                if (bFlag)
                {
                    try
                    {
                        theDB = DBControl.GetConnection();

                        statusData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        statusData.updateExternalIfData();
                    }
                    catch (SqlException se)
                    {
                        //Error Log 관리
                        if (theDB != null) theDB.Rollback();
                    }
                    catch (Exception ex)
                    {
                        statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                        statusData.updateExternalIfData();
                        throw ex;
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                else
                {
                    if (iCnt > 3)
                    {
                        try
                        {
                            statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                            statusData.updateExternalIfData();
                        }
                        catch (Exception ex)
                        {
                            //
                        }
                        continue;
                    }
                    else
                    {
                        goto JUMP1;
                    }
                }
            }
        }

        //Check Status
        public void checkComplete()
        {
            ExternalIfData wmsIfData = new ExternalIfData();
            List<ExternalIfData> list = wmsIfData.getNotCompleteTransferDataList();

            foreach (ExternalIfData statusData in list)
            {
                bool bFlag = false;
                DBConnection theDB = null;
                int iCnt = 0;

            JUMP1:

                iCnt += 1;
                bFlag = sendMessageToWms(statusData.Ifvalue);

                if (bFlag)
                {
                    try
                    {
                        theDB = DBControl.GetConnection();

                        statusData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        statusData.updateExternalIfData();
                    }
                    catch (SqlException se)
                    {
                        //Error Log 관리
                        if (theDB != null) theDB.Rollback();
                    }
                    catch (Exception ex)
                    {
                        statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                        statusData.updateExternalIfData();
                        throw ex;
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                else
                {
                    if (iCnt > 3)
                    {
                        try
                        {
                            statusData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                            statusData.updateExternalIfData();
                        }
                        catch (Exception ex)
                        {
                            //
                        }
                        continue;
                    }
                    else
                    {
                        goto JUMP1;
                    }
                }
            }
        }

        //장비 상태 External If 등록
        public void regEqpStatus()
        {
            string strStatus = "";
            CheckEquipmentStatus checkEqp = new CheckEquipmentStatus();

            try
            {
                strStatus = checkEqp.createWmsStatusInfo();

                if (strStatus != checkStatusString)
                {
                    checkStatusString = strStatus;

                    MakeWmsIfString makeIf = new MakeWmsIfString();
                    makeIf.insertStatus(checkStatusString);
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public bool sendMessageToWms(string sendMessage)
        {
            string sendText = "";
            bool bFlag = false;

            sendText = sendMessage;
            sendText = sendText.Replace(GlobalConstant.START_CHAR, (char)2);
            sendText = sendText.Replace(GlobalConstant.END_CHAR, (char)3);

            if (tcpClient == null)
            {
                try
                {
                    startSocketClient();
                }
                catch (Exception ex)
                {
                    bFlag = false;
                }
            }
            else
            {
                try
                {
                    lock (tcpClient)
                    {
                        try
                        {
                            bFlag = tcpClient.SendText(sendText);
                        }
                        catch (Exception ex)
                        {
                            bFlag = false;
                        }
                    }
                }
                catch (Exception ex1)
                {
                    bFlag = false;

                    if (tcpClient != null)
                    {
                        if (tcpClient.Connected)
                        {
                            tcpClient.Disconnect();
                        }
                        
                        tcpClient = null;
                    }
                }
            }

            return bFlag;
        }


        //신규 Client Que를 Add함.
        public static void addClient(TcpClient client)
        {
            lock (clientList)
            {
                clientList.Add(client);
            }
        }

        //Client Que를 삭제함.
        public static void removeClient(TcpClient client)
        {
            lock (clientList)
            {
                foreach(TcpClient clientTemp in clientList) 
                {
                    if (clientTemp.Equals(client))
                    {
                        clientList.Remove(clientTemp);
                    }
                }
            }
        }
       #endregion
    }
}
