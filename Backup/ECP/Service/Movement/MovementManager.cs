using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Timers;

using ECP.Db;
using ECP.Util;
using ECP.Db.Entity;
using ECP.Global;
using ECP.BizRule;
using System.Runtime.Remoting;
using ECP.Service.Logger;

namespace ECP.Service.Movement
{
    /// <summary>
    /// Movement를 관리 함.
    /// </summary>
    
    [Serializable]
    public class MovementManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "MovementManager";

        //SAP I/F check : 기준 정보로 빼내야 함.
        Timer timerMovementIF = new Timer();

        //WMS I/F check : 기준 정보로 빼내야 함.
        Timer timerWmsIF = new Timer();

        //Transfer check : 기준 정보로 빼내야 함.
        Timer timerTransfer = new Timer();

        //Transfer Detail check : 기준 정보로 빼내야 함.
        Timer timerTransferDetail = new Timer();

        //Transfer Detail check : 기준 정보로 빼내야 함.
        Timer timerLeadTime = new Timer();

        protected static Boolean timerMovementIFFlag = false;
        protected static Boolean timerWmsIFFlag = false;
        protected static Boolean timerTransferFlag = false;
        protected static Boolean timerTransferDetailFlag = false;
        protected static Boolean timerLeadTimeFlag = false;

        /// <summary>
        /// 전체 반송 정보를 담고 있는 Container
        /// </summary>
        private static Dictionary<string, TransferData> transferDataList = new Dictionary<string, TransferData>();

        /// <summary>
        /// MovementManager 생성자
        /// 주어진 Owner Id와 Manager Id로 MovementManager를 생성한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public MovementManager(string ownerId, string id)
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
            setTransferDataList();
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
            this.Load();

           if(GlobalConstant.START_MODE != EntityEnumData.STARTMODE.Client.ToString() )
           {
                this.startBatchJob();
           }
        }

        /// <summary>
        /// Batch Job 구동 메소드
        ///   -> SAP I/F 반송 지시 Check timer 구동
        ///   -> 세부 반송 지시 Check timer 구동
        /// </summary>
        protected void startBatchJob()
        {
 
            //timerMovementIF.Interval = 5000;
            //timerMovementIF.Start();

            timerWmsIF.Interval = 5000;
            timerWmsIF.Start();


            timerTransfer.Interval = 5000;
            timerTransfer.Start();

            timerTransferDetail.Interval = 5000;
            timerTransferDetail.Start();

            //timerLeadTime.Interval = 3600000;
            //timerLeadTime.Start();

            //timerMovementIF.Elapsed += new ElapsedEventHandler(checkMovementIF);
            timerWmsIF.Elapsed += new ElapsedEventHandler(checkWmsIF);
            timerTransfer.Elapsed += new ElapsedEventHandler(checkTransfer);
            timerTransferDetail.Elapsed += new ElapsedEventHandler(checkTransferDetail);
            //timerLeadTime.Elapsed += new ElapsedEventHandler(checkLeadTime);
        }

        //1.SAP I/F Manager를 주기적으로 확인하면서 신규 반송을 생성하는 메소드
        //  :주기적으로 TB_EXTERNALIF table 확인
        //   -> 주기는 10초, I/F 완료되지 않은 반송지시 Data 확인
        //  :신규 반송 생성 메소드를 호출해서 반송 생성
        //   -> TB_TRANSFER table에 신규 반송 생성
        //   -> 신규 반송을 생성하면 TB_EXTERNALIF table update
        //  :반송 정보 Que에 반송 정보 등록

        protected void checkMovementIF(object sender, ElapsedEventArgs e)
        {
            DBConnection theDB = null;

            //timer Flag가 false 일 경우에만 진행함.
            if (!timerMovementIFFlag)
            {
                //timer 시작 Flag True
                timerMovementIF.Stop();
                timerMovementIFFlag = true;

                try
                {
                    MovementSapIfData movementSapIfData = new MovementSapIfData();
                    List<MovementSapIfData> list = movementSapIfData.getNotCompleteMovementSapIfDataList();

                    //공정이 존재하지 않는 경우 Pass함.
                    List<CodeMasterData> codeMasterList = new List<CodeMasterData>();
                    CodeMasterData codeMaster = new CodeMasterData();
                    codeMasterList = codeMaster.getPositionValueList();

                    foreach (MovementSapIfData moveData in list)
                    {
                        Boolean fromPositionCheck = false;
                        Boolean toPositionCheck = false;

                        try
                        {
                            //From 공정 확인
                            foreach (CodeMasterData codeTemp in codeMasterList)
                            {
                                if (codeTemp.Code == moveData.Frlgort)
                                {
                                    fromPositionCheck = true;
                                    break;
                                }
                            }

                            //To 공정 확인
                            foreach (CodeMasterData codeTemp in codeMasterList)
                            {
                                if (codeTemp.Code == moveData.Tolgort)
                                {
                                    toPositionCheck = true;
                                    break;
                                }
                            }

                            theDB = DBControl.GetConnection();

                            if ((fromPositionCheck == true) && (toPositionCheck == true))
                            {
                                TransferData transferData = new TransferData();
                                int iCnt = transferData.getTransferDataCnt(moveData);

                                TransferDivisionData transferDivisionData = new TransferDivisionData();
                                int iCnt1 = transferDivisionData.getTransferDivisionDataCnt(moveData);

                                if (iCnt == 0 && iCnt1 == 0)
                                {
                                    if (moveData.Frlgort != GlobalConstant.STORAGE_LOCATION_CODE)
                                    {
                                        if (moveData.Tolgort != GlobalConstant.STORAGE_LOCATION_CODE)
                                        {
                                            createSapMovementTransfer(moveData, EntityEnumData.MOVEMENTTYPE.SAP.ToString());
                                        }
                                        else
                                        {
                                            createIngoTransfer(moveData, EntityEnumData.MOVEMENTTYPE.SAP.ToString());
                                        }
                                    }
                                    else
                                    {
                                        createOutgoTransfer(moveData, EntityEnumData.MOVEMENTTYPE.SAP.ToString());
                                    }
                                }
                            }

                            moveData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                            moveData.updateMovementSapIfData(theDB, true);
                        }
                        catch (SqlException se)
                        {
                            //Error Log 관리
                            if (theDB != null) theDB.Rollback();
                            throw se;
                        }
                        catch (Exception ex)
                        {
                            moveData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                            moveData.updateMovementSapIfData(theDB, true);
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
                catch (Exception ex)
                {
                    ErrorHistData errorhistData = new ErrorHistData();
                    errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Errortype = "MCS";
                    errorhistData.Errorcode = "M0001";
                    errorhistData.Errortext = "입출고 오류 : "  + ex.Message;

                    theDB = null;

                    try
                    {
                        theDB = DBControl.GetConnection();
                        if (GlobalConstant.START_MODE == EntityEnumData.STARTMODE.Client.ToString())
                        {
                            errorhistData.insertErrorHistData(theDB);
                        }
                        else
                        {
                            errorhistData.insertErrorHistDataBackup(theDB);
                        }
                    }
                    catch (SqlException se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                finally
                {
                    //timer 종료 Flag false
                    timerMovementIFFlag = false;
                    timerMovementIF.Start();
                }
            }
        }

        protected void checkWmsIF(object sender, ElapsedEventArgs e)
        {
            DBConnection theDB = null;

            //timer Flag가 false 일 경우에만 진행함.
            if (!timerWmsIFFlag)
            {
                //timer 시작 Flag True
                timerWmsIF.Stop();
                timerWmsIFFlag = true;

                try
                {
                    ExternalIfData wmsIfData = new ExternalIfData();
                    List<ExternalIfData> list = wmsIfData.getNotCompleteWmsOrderIfDataList();

                    //공정이 존재하지 않는 경우 Pass함.
                    List<CodeMasterData> codeMasterList = new List<CodeMasterData>();
                    CodeMasterData codeMaster = new CodeMasterData();
                    codeMasterList = codeMaster.getPositionValueList();

                    foreach (ExternalIfData moveData in list)
                    {
                        Boolean fromPositionCheck = false;
                        Boolean toPositionCheck = false;

                        try
                        {
                            TransferData tfData = new TransferData();

                            tfData.setTransferExternalIfData(moveData);

                                                        //From 공정 확인
                            foreach (CodeMasterData codeTemp in codeMasterList)
                            {
                                if (codeTemp.Code == tfData.Fromposition)
                                {
                                    fromPositionCheck = true;
                                    break;
                                }
                            }

                            //To 공정 확인
                            foreach (CodeMasterData codeTemp in codeMasterList)
                            {
                                if (codeTemp.Code == tfData.Toposition)
                                {
                                    toPositionCheck = true;
                                    break;
                                }
                            }

                            if ((fromPositionCheck == true) && (toPositionCheck == true))
                            {
                                int iCnt = tfData.getTransferDataCnt();

                                if (iCnt == 0)
                                {
                                    if (tfData.Fromposition != GlobalConstant.STORAGE_LOCATION_CODE)
                                    {
                                        if (tfData.Toposition != GlobalConstant.STORAGE_LOCATION_CODE)
                                        {
                                            createMovementTransfer(tfData);
                                        }
                                        else
                                        {
                                            createIngoTransfer(tfData);
                                        }
                                    }
                                    else
                                    {
                                        createOutgoTransfer(tfData);
                                    }
                                }
                            }

                            //tfData.insertTransferData(theDB, true);

                            ////Hirack 상태 및 반송 상태 reserved
                            //HirackData hData = new HirackData();
                            //if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                            //{
                            //    hData.updateHirackIngoStatus(theDB, tfData);
                            //}
                            //else if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                            //{
                            //    hData.updateHirackOutgoStatus(theDB, tfData);
                            //}
                            //else
                            //{
                            //    if (tfData.Fromposition == GlobalConstant.STORAGE_LOCATION_CODE)
                            //    {
                            //        hData.updateHirackOutgoStatus(theDB, tfData);
                            //    }

                            //    if (tfData.Toposition == GlobalConstant.STORAGE_LOCATION_CODE)
                            //    {
                            //        hData.updateHirackIngoStatus(theDB, tfData);
                            //    }

                            //}

                            ////Pallet 상태 및 반송 reserved
                            //PalletData pData = new PalletData();
                            //pData.updatePalletStatus(theDB, tfData);

                            //theDB.Commit();

                            moveData.Transcompletetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                            moveData.updateExternalIfData();
                        }
                        catch (SqlException se)
                        {
                            //Error Log 관리
                            if (theDB != null) theDB.Rollback();
                            throw se;
                        }
                        catch (Exception ex)
                        {
                            moveData.Transcompletetime = GlobalConstant.SET_ERRIRTIME;
                            moveData.updateExternalIfData();
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
                catch (Exception ex)
                {
                    ErrorHistData errorhistData = new ErrorHistData();
                    errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Errortype = "MCS";
                    errorhistData.Errorcode = "M0001";
                    errorhistData.Errortext = "입출고 오류 : " + ex.Message;

                    theDB = null;

                    try
                    {
                        theDB = DBControl.GetConnection();
                        errorhistData.insertErrorHistData(theDB);
                    }
                    catch (SqlException se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                finally
                {
                    //timer 종료 Flag false
                    timerWmsIFFlag = false;
                    timerWmsIF.Start();
                }
            }
        }

        //2.TB_TRANSFER 주기적으로 확인하면서 QUE에 반송을 생성하는 메소드
        //  :Que에 반송이 누락될 경우를 방지하기 위함.

        void checkTransfer(object sender, ElapsedEventArgs e)
        {
            //timer Flag가 false 일 경우에만 진행함.
            if (!timerTransferFlag)
            {
                //timer 시작 Flag True
                timerTransfer.Stop();
                timerTransferFlag = true;
                try
                {
                    setTransferDataList();
                }
                catch (Exception ex)
                {
                    ErrorHistData errorhistData = new ErrorHistData();
                    errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Errortype = "MCS";
                    errorhistData.Errorcode = "M0002";
                    errorhistData.Errortext = "입출고 오류 : " + ex.Message;

                    DBConnection theDB = null;

                    try
                    {
                        Log.log(5, this.GetType().Name, "error", Category.ERROR, "", "Manager", ex.Message + " \n " + ex.StackTrace.ToString());

                        theDB = DBControl.GetConnection();
                        errorhistData.insertErrorHistData(theDB);
                    }
                    catch (SqlException se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    catch (Exception exc)
                    {
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                finally
                {

                    //timer 종료 Flag false
                    timerTransferFlag = false;
                    timerTransfer.Start();
                }

            }
        }

        //3. 주기적으로 세부 반송 생성 메소드
        //  :주기적으로 Que에 쪼개지지 않은 반송 확인
        //   ->주기는 10초 단위로 확인
        //   ->TB_MOVERULE의 전체 반송 최적화룰 확인을 통해서 세부 반송 생성
        //     :ROUTE MANAGER를 통해서 반송 경로 상태 확인
        //  :세부 반송 생성 메소드를 호출해서 세부 반송 생성
        //  :전체 반송 Que에 세부 반송 쪼갰는지 여부 등록

        protected void checkTransferDetail(object sender, ElapsedEventArgs e)
        {
            //timer Flag가 false 일 경우에만 진행함.
            if (!timerTransferDetailFlag)
            {
                //timer 시작 Flag True
                timerTransferDetail.Stop();
                timerTransferDetailFlag = true;

                try
                {

                JUMP1:
                    List<TransferData> utfDataList = new List<TransferData>();
                    try
                    {
                        foreach (TransferData tfData in transferDataList.Values)
                        {
                            if (tfData.Transferdetailflag == "N")
                            {
                                TransferDetailData tfdData = new TransferDetailData();

                                //TransferDetail 쪼개짐 여부
                                int iCnt = tfdData.getTransferDetailDataCnt(tfData);

                                if (iCnt != 0)
                                {
                                    tfData.Transferdetailflag = "Y";
                                    utfDataList.Add(tfData);
                                }
                                else
                                {
                                    //DB 존재 여부 Check
                                    try
                                    {
                                        int iTransferCnt = tfData.getTransferDataWaitCnt();

                                        if (iTransferCnt == 0)
                                        {
                                            continue;
                                        }
                                    }
                                    catch (Exception exe1)
                                    {
                                        throw exe1;
                                    }

                                    try
                                    {
                                        int iRst = tfdData.makeTransferDetailDataList(tfData);

                                        if (iRst == 0)
                                        {
                                            tfData.Transferdetailflag = "Y";
                                            utfDataList.Add(tfData);
                                        }
                                    }
                                    catch (Exception exe)
                                    {
                                        chageTransferCancel(tfData);
                                        throw exe;
                                    }
                                }
                            }
                        }
                    }
                    catch (InvalidOperationException ex)
                    {
                        goto JUMP1;
                    }

                    changeTransferDataList(utfDataList);
                }
                catch (Exception ex)
                {
                    ErrorHistData errorhistData = new ErrorHistData();
                    errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Errortype = "MCS";
                    errorhistData.Errorcode = "M0003";
                    errorhistData.Errortext = "입출고 오류 : " + ex.Message;

                    DBConnection theDB = null;

                    try
                    {
                        theDB = DBControl.GetConnection();

                        errorhistData.insertErrorHistData(theDB);
                    }
                    catch (SqlException se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
                finally
                {
                    //timer 종료 Flag false
                    timerTransferDetailFlag = false;
                    timerTransferDetail.Start();
                }
            }
        }

        //4. 주기적으로 LEAD TIME 생성 메소드
        //  :주기적으로 Transfer history를 통해 Lead Time을 가져옴.

        protected void checkLeadTime(object sender, ElapsedEventArgs e)
        {
            //timer Flag가 false 일 경우에만 진행함.
            if (!timerLeadTimeFlag)
            {
                //timer 시작 Flag True
                timerLeadTime.Stop();
                timerLeadTimeFlag = true;
                try
                {
                    setLeadTimeList();
                }
                catch (Exception ex)
                {
                    ErrorHistData errorhistData = new ErrorHistData();
                    errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    errorhistData.Errortype = "MCS";
                    errorhistData.Errorcode = "M0004";
                    errorhistData.Errortext = "Lead Time 오류 : " + ex.Message;

                    DBConnection theDB = null;

                    try
                    {
                        theDB = DBControl.GetConnection();
                        if (GlobalConstant.START_MODE == EntityEnumData.STARTMODE.Client.ToString())
                        {
                            errorhistData.insertErrorHistData(theDB);
                        }
                        else
                        {
                            errorhistData.insertErrorHistDataBackup(theDB);
                        }
                    }
                    catch (SqlException se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    catch (Exception exc)
                    {
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }

                //timer 종료 Flag false
                timerLeadTimeFlag = false;
                timerLeadTime.Start();

            }
        }


        //2. 신규 전체 반송 생성 메소드
        //  :반송 유형에 따라서 신규 반송 생성
        //   -> SAP 반송지시, ECS 반송지시, 이동반송 등에 따라서 반송 번호 다르게 생성
        //   -> TB_TRANSFER에 신규 반송을 insert함.
        public void createIngoTransfer(MovementSapIfData moveData, string movementtype)
        {
            DBConnection theDB = null;
            MovementSapIfData moveNewData = new MovementSapIfData(moveData);
            TransferData transferData = new TransferData();

            //Pallet Data Check
            CheckPalletInfo checkPallet = new CheckPalletInfo();
            string strResult = checkPallet.checkIngoPallet(moveNewData.Pallet, moveNewData.Charg, moveNewData.Matnr);

            if (strResult != "00")
            {
                if (strResult == "01")
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 가 ECS에 존재하지 않습니다.";
                }
                else if (strResult == "02")
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 가 사용 중입니다.";
                }
                else
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 확인 중 오류가 발생했습니다.";
                }

                throw new Exception(strResult);
            }

            try
            {
                transferData.setTransferIngoData(moveNewData, movementtype);

                theDB = DBControl.GetConnection();
                theDB.Start();

                transferData.insertTransferData(theDB, true);

                //Hirack 상태 및 반송 상태 reserved
                HirackData hData = new HirackData();
                hData.updateHirackIngoStatus(theDB, transferData);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, transferData);

                theDB.Commit();
                addTranferData(transferData);
            }
            catch (Exception se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void createIngoTransfer(TransferData tfData)
        {
            DBConnection theDB = null;
            TransferData transferNewData = new TransferData(tfData);

            try
            {

                //Cell 상태 Check
                HirackData checkHirack = new HirackData();
                checkHirack.getHirackData(transferNewData.Tosublocationid);

                if ((string.IsNullOrEmpty(checkHirack.Cellid))||
                    (checkHirack.Cellstatus != EntityEnumData.HIRACKSTATUS.Empty.ToString()))
                {
                    string strResult = "";
                    strResult = "(Cell 번호:" + transferNewData.Tosublocationid + ") 는 입고 할 수 없는 Location 입니다.";
                    throw new Exception(strResult);
                }

                theDB = DBControl.GetConnection();
                theDB.Start();

                transferNewData.insertTransferData(theDB, true);

                //Hirack 상태 및 반송 상태 reserved
                HirackData hData = new HirackData();
                hData.updateHirackIngoStatus(theDB, transferNewData);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, transferNewData);

                theDB.Commit();
                addTranferData(transferNewData);
            }
            catch (Exception se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void createOutgoTransfer(MovementSapIfData moveData, string movementtype)
        {
            MovementSapIfData moveNewData = new MovementSapIfData(moveData);
            List<FindPalletData> findPalletDataList = new List<FindPalletData>();
            FindPalletData findPalletData = new FindPalletData();
            Boolean newCommandIdFlag = false;
            Boolean orderCancelFlag = false;
            decimal totalQty = 0;

            findPalletDataList = findPalletData.getFindPalletDataList(moveNewData);

            //출고할 Data를 찾지 못한 경우 빠져나옴.(error 메시지 처리 필요함.)
            if (findPalletDataList == null)
            {
                Exception ex = new Exception("(요청번호:" + moveData.Reqno + ") 의 출고할 Data가 없습니다.");
                throw ex;
            }

            //New Command 생성 여부 파악
            if (findPalletDataList.Count > 1)
            {
                newCommandIdFlag = true;
            }
            else if (findPalletDataList.Count == 1)
            {
                if (decimal.Parse(findPalletDataList[0].Sapqty) != decimal.Parse(findPalletDataList[0].Qty))
                {
                    newCommandIdFlag = true;
                }
            }
            else //출고할 Data를 찾지 못한 경우 빠져나옴.(error 메시지 처리 필요함.)
            {
                Exception ex = new Exception("(요청번호:" + moveData.Reqno + ") 의 출고할 Data가 없습니다.");
                throw ex;
            }

            //반송 Cancel 여부 파악
            foreach (FindPalletData fpData in findPalletDataList)
            {
                totalQty += decimal.Parse(fpData.Qty);
            }

            //출고할 Data를 찾지 못한 경우 빠져나옴.(error 메시지 처리 필요함.)
            if (totalQty < decimal.Parse(moveNewData.Menge))
            {
                Exception ex = new Exception("(요청번호:" + moveData.Reqno + ") 의 요청 수량이 Hirack 재고 Pallet 수량보다 많습니다.");
                throw ex;
            }

            if (totalQty != decimal.Parse(moveNewData.Menge))
            {
                orderCancelFlag = true;
            }
            else
            {
                orderCancelFlag = false;
            }

            foreach (FindPalletData fpData in findPalletDataList)
            {
                DBConnection theDB = null;
                TransferData transferData = new TransferData();

                TransferDivisionData tfdData = new TransferDivisionData();
                tfdData = transferData.setTransferOutgoData(fpData, newCommandIdFlag, orderCancelFlag, movementtype);

                //출고의 경우 From/To Plant가 MovementSapIf를 통해 넣어줘야 함.
                transferData.Fromplant = moveNewData.Frwerks;

                try
                {
                    theDB = DBControl.GetConnection();
                    theDB.Start();

                    transferData.insertTransferData(theDB, true);

                    //Hirack 상태 및 반송 상태 reserved
                    HirackData hData = new HirackData();
                    hData.updateHirackOutgoStatus(theDB, transferData);

                    //Pallet 상태 및 반송 reserved
                    PalletData pData = new PalletData();
                    pData.updatePalletStatus(theDB, transferData);

                    //Pallet 상태 및 반송 reserved
                    if (newCommandIdFlag)
                    {
                        tfdData.insertTransferDivisionData(theDB, true);
                    }

                    theDB.Commit();
                    addTranferData(transferData);
                }
                catch (Exception se)
                {
                    throw se;
                }
                finally
                {
                    if (theDB != null)
                    {
                        DBControl.RemoveDBConection(theDB);
                    }
                }
            }

            //수량이 일치하지 않을 경우 반송을 Cancel 처리 해야 함.
            if ((movementtype == EntityEnumData.MOVEMENTTYPE.SAP.ToString()) &&
                 orderCancelFlag)
            {
                ExternalIfData externalifData = new ExternalIfData();
                externalifData.insertExternalIfData("02", moveNewData, 1, GlobalConstant.ORDER_CANCEL, GlobalConstant.ORDER_CANCEL_FLAG);
            }
        }

        public void createOutgoTransfer(TransferData tfData)
        {
            DBConnection theDB = null;
            TransferData transferNewData = new TransferData(tfData);

            CheckPalletInfo checkPallet = new CheckPalletInfo();
            string strResult = checkPallet.checkOutgoPallet(transferNewData.Palletidname);

            //출고할 Data를 찾지 못한 경우 빠져나옴.(error 메시지 처리 필요함.)
            if (strResult != "00")
            {
                Exception ex = new Exception("(요청번호:" + tfData.Commandid + ") 의 출고할 Data가 없습니다.");
                throw ex;
            }

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                transferNewData.insertTransferData(theDB, true);

                //Hirack 상태 및 반송 상태 reserved
                HirackData hData = new HirackData();
                hData.updateHirackOutgoStatus(theDB, transferNewData);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, transferNewData);

                theDB.Commit();
                addTranferData(transferNewData);
            }
            catch (Exception se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void createMovementTransfer(TransferData tfData)
        {
            DBConnection theDB = null;
            TransferData transferNewData = new TransferData(tfData);

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                transferNewData.insertTransferData(theDB, true);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, transferNewData);

                theDB.Commit();
                addTranferData(transferNewData);
            }
            catch (Exception se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void createSapMovementTransfer(MovementSapIfData moveData, string movementtype)
        {
            DBConnection theDB = null;
            MovementSapIfData moveNewData = new MovementSapIfData(moveData);
            TransferData transferData = new TransferData();

            //Pallet Data Check
            CheckPalletInfo checkPallet = new CheckPalletInfo();
            string strResult = checkPallet.checkIngoPallet(moveNewData.Pallet, moveNewData.Charg, moveNewData.Matnr);

            if (strResult != "00")
            {
                if (strResult == "01")
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 가 ECS에 존재하지 않습니다.";
                }
                else if (strResult == "02")
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 가 사용 중입니다.";
                }
                else
                {
                    strResult = "(팔렛 번호:" + moveNewData.Pallet + ") 확인 중 오류가 발생했습니다.";
                }

                throw new Exception(strResult);
            }

            try
            {
                transferData.setTransferSapMoveData(moveNewData, movementtype);

                theDB = DBControl.GetConnection();
                theDB.Start();

                transferData.insertTransferData(theDB, true);

                //Pallet 상태 및 반송 reserved
                PalletData pData = new PalletData();
                pData.updatePalletStatus(theDB, transferData);

                theDB.Commit();
                addTranferData(transferData);
            }
            catch (SqlException se)
            {
                throw se;
            }
            catch (Exception ex)
            {
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

        //4. 세부 반송 생성 메소드
        //  : ROUTE MANAGER를 통해서 반송 경로 및 전체 반송 경로 가져옴.
        //  : 각 SEM별 세부 반송을 TB_TRANFERDETAIL에 Insert함
        public void createTransferDetail(TransferData tfData)
        {
            TransferDetailData tfdData = new TransferDetailData();

            //TransferDetail 쪼개짐 여부
            int iCnt = tfdData.getTransferDetailDataCnt(tfData);

            if (iCnt != 0)
            {
                tfData.Transferdetailflag = "Y";
                changeTranferData(tfData);
            }
            else
            {
                int iRst = tfdData.makeTransferDetailDataList(tfData);

                if (iRst == 0)
                {
                    tfData.Transferdetailflag = "Y";
                    changeTranferData(tfData);
                }
            }
        }

        //5. SEM에서 받은 반송 상태 정보 Event 받는 메소드
        //  :SEM에서 반송 상태 Event를 받아서 Que정보 Update
        //   -> 반송 Que 생성/상태 수정하는 메소드 호출

        //6. 세부 반송 이력 조회하는 메소드
        //  :전체 반송 정보를 통해서 세부 반송 정보를 조회해 옴.
        public Dictionary<string, TransferDetailData> getTransferDetailDataList(string commandid)
        {
            Dictionary<string, TransferDetailData> list = new Dictionary<string, TransferDetailData>();

            TransferDetailData tranferDetailData = new TransferDetailData();
            list = tranferDetailData.getTransferDetailDataList(commandid);

            return list;
        }
        //7. 반송 Queue 생성/상태수정하는 메소드
        //  :세부 반송 이력을 조회하는 메소드를 호출하여 전체 완료 확인
        //   -> 세부 반송이 완료되면 TB_TRANSFER 반송 정보 update
        //   -> 전체 반송 Queue에서 반송 정보 삭제

        //반송 상태를 반송중으로 변경함. Que변경
        public void chageTransferStatus(string commandid)
        {
            DBConnection theDB = null;
            TransferData tfData = getTransferData(commandid);

            if (tfData != null)
            {
                tfData.Transferstatus = EntityEnumData.TRANSFERSTATUS.Transferring.ToString();
                tfData.Executetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

                try
                {
                    theDB = DBControl.GetConnection();
                    tfData.updateTransferData(theDB, true);

                    changeTranferData(tfData);
                }
                catch (SqlException se)
                {
                    throw se;
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

        //반송 상태를 반송중으로 변경함. Que변경
        public void chageTransferStatus(DBConnection theDB, TransferData tfData)
        {
            tfData.Transferstatus = EntityEnumData.TRANSFERSTATUS.Transferring.ToString();
            tfData.Executetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

            try
            {
                tfData.updateTransferData(theDB, true);

                changeTranferData(tfData);
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        //반송 상태를 반송 완료로 변경함. Que삭제
        public void chageTransferComplete(string commandid)
        {
            DBConnection theDB = null;
            TransferData tfData = getTransferData(commandid);

            if (tfData != null)
            {
                tfData.Transferstatus = EntityEnumData.TRANSFERSTATUS.Complete.ToString();
                tfData.Resultcode = Convert.ToInt32(EntityEnumData.TRANSFERRESULTCODE.SUCCESS).ToString();
                tfData.Completetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

                try
                {
                    theDB = DBControl.GetConnection();
                    tfData.updateTransferData(theDB, true);

                    tfData.deleteTransferData(theDB, false);

                    removeTranferData(tfData);

                    //WMS I/F 등록
                    try
                    {
                        MakeWmsIfString mwData = new MakeWmsIfString();
                        mwData.insertTransferComplete(tfData);
                    }
                    catch (Exception ex)
                    {
                        //Error
                    }

                    //TransferDivisionData transferDivisionData = new TransferDivisionData();
                    //int iCnt = transferDivisionData.getTransferDivisionData(tfData);

                    ////반송 완료 Transfer Division 정보 삭제
                    //if (iCnt > 0)
                    //{
                    //    transferDivisionData.deleteTransferDivisionData(theDB, true);
                    //}

                    //ECS 반송 or SAP 쪼개진 반송일 경우 DATA MOVE 요청
                    //SAP 반송일 경우 반송 완료 요청
                    //ExternalIfData externalifData = new ExternalIfData();
                    //if (tfData.Movementtype == EntityEnumData.MOVEMENTTYPE.SAP.ToString())
                    //{
                    //    if (iCnt > 0)
                    //    {
                    //        TransferDivisionData checkDivisionData = new TransferDivisionData();
                    //        int iCnt1 = checkDivisionData.getTransferDivisionSapDataCnt(theDB, transferDivisionData.Sapcommandid);

                    //        if (iCnt1 > 0)
                    //        {
                    //            if (transferDivisionData.Ordertype == GlobalConstant.ORDER_DATAMOVE)
                    //            {
                    //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_DATAMOVE_FLAG, transferDivisionData.Sapcommandid);
                    //            }
                    //            else
                    //            {
                    //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_DATAMOVE_FLAG, transferDivisionData.Sapcommandid);
                    //            }
                    //        }
                    //        else
                    //        {
                    //            if (transferDivisionData.Ordertype == GlobalConstant.ORDER_DATAMOVE)
                    //            {
                    //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_CONFIRMATION_FLAG, transferDivisionData.Sapcommandid);
                    //            }
                    //            else
                    //            {
                    //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_CONFIRMATION_FLAG, transferDivisionData.Sapcommandid);
                    //            }
                    //        }
                    //    }
                    //    else
                    //    {
                    //        externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_CONFIRMATION_FLAG);
                    //    }
                    //}
                    //else if (tfData.Movementtype == EntityEnumData.MOVEMENTTYPE.ECS.ToString())
                    //{
                    //    externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_DATAMOVE_FLAG);
                    //}
                }
                catch (Exception se)
                {
                    throw se;
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

        public void chageTransferComplete(DBConnection theDB, TransferData tfData)
        {
            tfData.Transferstatus = EntityEnumData.TRANSFERSTATUS.Complete.ToString();
            tfData.Resultcode = Convert.ToInt32(EntityEnumData.TRANSFERRESULTCODE.SUCCESS).ToString();
            tfData.Completetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

            try
            {
                tfData.updateTransferData(theDB, true);

                tfData.deleteTransferData(theDB, false);

                removeTranferData(tfData);

                //WMS I/F 등록
                try
                {
                    MakeWmsIfString mwData = new MakeWmsIfString();
                    mwData.insertTransferComplete(tfData);
                }
                catch (Exception ex)
                {
                    //Error
                }

                //TransferDivisionData transferDivisionData = new TransferDivisionData();
                //int iCnt = transferDivisionData.getTransferDivisionData(tfData);

                ////반송 완료 Transfer Division 정보 삭제
                //if (iCnt > 0)
                //{
                //    transferDivisionData.deleteTransferDivisionData(theDB, true);
                //}

                //ECS 반송 or SAP 쪼개진 반송일 경우 DATA MOVE 요청
                //SAP 반송일 경우 반송 완료 요청
                //ExternalIfData externalifData = new ExternalIfData();
                //if (tfData.Movementtype == EntityEnumData.MOVEMENTTYPE.SAP.ToString())
                //{
                //    if (iCnt > 0)
                //    {
                //        TransferDivisionData checkDivisionData = new TransferDivisionData();
                //        int iCnt1 = checkDivisionData.getTransferDivisionSapDataCnt(theDB, transferDivisionData.Sapcommandid);

                //        if (iCnt1 > 0)
                //        {
                //            if (transferDivisionData.Ordertype == GlobalConstant.ORDER_DATAMOVE)
                //            {
                //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_DATAMOVE_FLAG, transferDivisionData.Sapcommandid);
                //            } else
                //            {
                //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_DATAMOVE_FLAG, transferDivisionData.Sapcommandid);
                //            }
                //        } else
                //        {
                //            if (transferDivisionData.Ordertype == GlobalConstant.ORDER_DATAMOVE)
                //            {
                //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_CONFIRMATION_FLAG, transferDivisionData.Sapcommandid);
                //            } else
                //            {
                //                externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_CONFIRMATION_FLAG, transferDivisionData.Sapcommandid);
                //            }
                //        }
                //    }
                //    else
                //    {
                //        externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_CONFIRMATION, GlobalConstant.ORDER_CONFIRMATION_FLAG);
                //    }
                //}
                //else if (tfData.Movementtype == EntityEnumData.MOVEMENTTYPE.ECS.ToString())
                //{
                //    externalifData.insertExternalIfData("02", tfData, 0, GlobalConstant.ORDER_DATAMOVE, GlobalConstant.ORDER_DATAMOVE_FLAG);
                //}
            }
            catch (Exception se)
            {
                throw se;
            }
        }

        //반송 상태를 반송 취소로 변경함. Que삭제
        public void chageTransferCancel(TransferData tfData)
        {
            DBConnection theDB = null;
            TransferData transferNewData = new TransferData(tfData);

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                //전체 반송 완료 처리
                transferNewData.updateTransferData(theDB, true);

                //전체 반송 삭제 처리
                transferNewData.deleteTransferData(theDB, false);

                //TransferDetail이 존재할 경우 삭제
                TransferDetailData tfdData = new TransferDetailData();
                List<TransferDetailData> tfdDataList = null;
                //tfdData.Plantcode = tfData.Plantcode;
                tfdData.Commandid = tfData.Commandid;
                tfdData.Commandseq = GlobalConstant.ALL_CHARACTER;

                tfdDataList = tfdData.getTransferDetailDataList(transferNewData);

                if (tfdDataList.Count != 0)
                {
                    foreach (TransferDetailData tfdTemp in tfdDataList)
                    {
                        //완료된 세부 반송 삭제
                        tfdTemp.deleteTransferDetailData(theDB, true);
                    }
                }

                ////TransferDivision이 존재할 경우 삭제
                //TransferDivisionData tfdivsionData = new TransferDivisionData();

                //int iCnt = tfdivsionData.getTransferDivisionData(transferNewData);

                //if (iCnt > 0)
                //{
                //    tfdivsionData.deleteTransferDivisionData(theDB, true);
                //}

                //if (transferNewData.Commandtype != EntityEnumData.COMMANDTYPE.MOVE.ToString())
                //{
                    //Hirack 상태 및 반송 상태 cancel
                    HirackData hData = new HirackData();
                    hData.updateHirackCancelStatus(theDB, transferNewData);
                //}

                ////Pallet 상태 및 반송 cancel
                //PalletData pData = new PalletData();
                //pData.updatePalletCancelStatus(theDB, transferNewData);

                theDB.Commit();

                //전체 반송 Que 삭제
                removeTranferData(transferNewData);
            }
            catch (Exception se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            ////SAP 반송 명령의 취소의 경우에만 반송 취소 SAP I/F
            //if ((transferNewData.Movementtype == EntityEnumData.MOVEMENTTYPE.SAP.ToString()) &&
            //     (transferNewData.Commandid.Substring(0, 1) != GlobalConstant.SAP_PREFIX))
            //{
            //    ExternalIfData externalifData = new ExternalIfData();
            //    externalifData.insertExternalIfData("02", transferNewData, 0, GlobalConstant.ORDER_CANCEL, GlobalConstant.ORDER_CANCEL_FLAG);
            //}
        }

        //세부 반송 상태를 반송중으로 변경함.. Que변경
        //Hirack 및 Pallet의 상태는 SEM에서 변경해야 함.
        public void chageTransferDetailStatus(string commandid, string commandseq)
        {
            DBConnection theDB = null;
            TransferData tfData = null;
            TransferDetailData tfdData = new TransferDetailData();
            int iCnt = tfdData.getTransferDetailData(commandid, commandseq);

            if (iCnt != 0)
            {
                tfdData.Transferdetailstatus = EntityEnumData.TRANSFERDETAILSTATUS.Transferring.ToString();
                tfdData.Executetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

                try
                {
                    theDB = DBControl.GetConnection();
                    theDB.Start();

                    //세부 반송 상태 변경
                    tfdData.updateTransferDetailData(theDB, true);

                    //전체 반송 조회 및 상태 확인
                    tfData = getTransferData(commandid);

                    if (tfData != null)
                    {
                        //전체 반송이 Wait일때문 상태를 Transferring으로 변경함.
                        if (tfData.Transferstatus == EntityEnumData.TRANSFERSTATUS.Wait.ToString())
                        {
                            //전체 반송 상태 변경
                            chageTransferStatus(theDB, tfData);
                        }
                    }

                    theDB.Commit();
                }
                catch (Exception se)
                {
                    throw se;
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

        //세부 반송 상태를 반송 완료로 변경함. Que삭제
        //Hirack 및 Pallet의 상태는 SEM에서 변경해야 함.
        public void chageTransferDetailComplete(string commandid, string commandseq)
        {
            DBConnection theDB = null;
            TransferData tfData = null;
            TransferDetailData tfdData = new TransferDetailData();
            int iCnt = tfdData.getTransferDetailData(commandid, commandseq);

            if (iCnt != 0)
            {
                tfdData.Transferdetailstatus = EntityEnumData.TRANSFERDETAILSTATUS.Complete.ToString();
                tfdData.Completetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

                try
                {
                    theDB = DBControl.GetConnection();
                    theDB.Start();

                    //세부 반송 완료 처리
                    tfdData.updateTransferDetailData(theDB, true);
                    
                    //완료된 세부 반송 삭제
                    tfdData.deleteTransferDetailData(theDB, false);

                    //세부 반송 잔여 여부 확인 후, 전체 반송 상태 변경 및 삭제
                    //세부 반송 존재(상태만 변경), 세부 반송 미 존재(반송 완료 및 삭제

                    //WMS I/F 등록
                    try
                    {
                        MakeWmsIfString mwData = new MakeWmsIfString();
                        mwData.insertTransferDetailComplete(tfdData);
                    }
                    catch (Exception ex)
                    {
                        //Error
                    }

                    TransferDetailData tfdRestData = new TransferDetailData();
                    int iRest = tfdRestData.getTransferDetailDataCnt(commandid, theDB);


                    tfData = getTransferData(commandid);

                    if (tfData != null)
                    {
                        if (iRest > 0)  //전체 반송 상태 변경
                        {
                            if (tfData.Transferstatus == EntityEnumData.TRANSFERSTATUS.Wait.ToString())
                            {
                                chageTransferStatus(theDB, tfData);
                            }
                        }
                        else  //전체 반송 완료 처리 및 삭제
                        {
                            chageTransferComplete(theDB, tfData);
                        }
                    }

                    theDB.Commit();
                }
                catch (Exception se)
                {
                    throw se;
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


        //DB에서 반송을 조회해서 Que에 Setting함.
        public static void setTransferDataList()
        {
            Dictionary<string, TransferData> list = new Dictionary<string, TransferData>();
            List<TransferData> deleteList = new List<TransferData>();
            TransferData transferData = new TransferData();

            try
            {
                list = transferData.getTransferDataList();

                //Que에 쓰레기 반송 삭제 
                deleteList = getDeleteQueList(list);

                foreach (TransferData delTemp in deleteList)
                {
                    //삭제 Data 확인 시점에 DB insert 되었을 수도 있기때문에
                    //반송 존재 여부 한번 더 확인 해야 함.
                    int iCnt = 0;

                    try
                    {
                        iCnt = delTemp.getTransferDataCnt();
                    }
                    catch (Exception ex)
                    {
                        throw ex;
                    }

                    if (iCnt == 0)
                    {
                        removeTranferData(delTemp);

                        DBConnection theDB = null;

                        try
                        {
                            List<TransferDetailData> tfdDataList = null;
                            TransferDetailData tfdData = new TransferDetailData();
                            tfdDataList = tfdData.getTransferDetailDataList(delTemp);

                            theDB = DBControl.GetConnection();
                            theDB.Start();

                            if (tfdDataList.Count != 0)
                            {
                                foreach (TransferDetailData tfdTemp in tfdDataList)
                                {
                                    //완료된 세부 반송 삭제
                                    tfdTemp.deleteTransferDetailData(theDB, true);

                                    //HISTORY Backup LGLS를 위해 추가함.
                                    try
                                    {
                                        tfdTemp.insertTransferDetailDataBackup();
                                    }
                                    catch (Exception ex)
                                    {
                                        //throw ex;
                                    }
                                }
                            }

                            ////TransferDivision이 존재할 경우 삭제
                            //TransferDivisionData tfdivsionData = new TransferDivisionData();

                            //int iCnt1 = tfdivsionData.getTransferDivisionData(delTemp);

                            //if (iCnt1 > 0)
                            //{
                            //    tfdivsionData.deleteTransferDivisionData(theDB, true);
                            //}

                            theDB.Commit();
                        }
                        catch (Exception ex)
                        {
                            if (theDB != null) theDB.Rollback();
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

                foreach (TransferData tfDataTemp in list.Values)
                {
                    addTranferData(tfDataTemp);
                }
            }
            catch (Exception e)
            {
                throw e;
            }
        }

        //DB에서 반송을 조회해서 Que에 Setting함.
        public static void setLeadTimeList()
        {
            List<LeadTimeData> leadTimeList = new List<LeadTimeData>();
            List<string[]> list = new List<string[]>();
            TransferScheduleTime leadTimeData = new TransferScheduleTime();

            try
            {
                list = leadTimeData.getAverageScheduledTimeList();

                foreach (string[] strTemp in list)
                {
                    int iResult = 0;
                    LeadTimeData ltData = new LeadTimeData();
                    DBConnection theDB = null;

                    string plantcode = GlobalConstant.PLANT_CODE;
                    string subFromLocation = strTemp[1].Substring(0, 5);
                    string subToLocation = strTemp[4].Substring(0, 5);
                    string entityid = "";
                    string fromudppropertycd = "";
                    string toudppropertycd = "";

                    try
                    {
                        UDPropertyData FromudpData = new UDPropertyData();
                        if (subFromLocation == GlobalConstant.SUBSYSTEM_RACK)
                        {
                            entityid = strTemp[1];
                            fromudppropertycd = GlobalConstant.START_LOCATION_CD;
                            iResult += FromudpData.getUdpropertyValue(plantcode, entityid, fromudppropertycd);
                        }
                        else
                        {
                            entityid = strTemp[2];
                            fromudppropertycd = GlobalConstant.START_SUBLOCATION_CD;
                            iResult += FromudpData.getUdpropertyValue(plantcode, entityid, fromudppropertycd);
                        }

                        UDPropertyData ToudpData = new UDPropertyData();
                        if (subToLocation == GlobalConstant.SUBSYSTEM_RACK)
                        {
                            entityid = strTemp[4];
                            toudppropertycd = GlobalConstant.END_LOCATION_CD;
                            iResult += ToudpData.getUdpropertyValue(plantcode, entityid, toudppropertycd);
                        }
                        else
                        {
                            entityid = strTemp[5];
                            toudppropertycd = GlobalConstant.END_SUBLOCATION_CD;
                            iResult += ToudpData.getUdpropertyValue(plantcode, entityid, toudppropertycd);
                        }

                        if (iResult == 2)
                        {
                            ltData.Plantcode = GlobalConstant.PLANT_CODE;
                            ltData.Fromposition = FromudpData.Value;
                            ltData.Toposition = ToudpData.Value;
                            ltData.Fromdeviceid = strTemp[0];
                            ltData.Fromlocationid = strTemp[1];
                            ltData.Fromsublocationid = strTemp[2];
                            ltData.Todeviceid = strTemp[3];
                            ltData.Tolocationid = strTemp[4];
                            ltData.Tosublocationid = strTemp[5];
                            ltData.Leadtime = strTemp[6];

                            int iCnt = ltData.getLeadTimeDataCnt();

                            try
                            {
                                theDB = DBControl.GetConnection();

                                if (iCnt == 0)
                                {
                                    ltData.insertLeadTimeData(theDB, true);
                                }
                                else
                                {
                                    ltData.updateLeadTimeData(theDB, true);
                                }
                            }
                            catch (Exception ex)
                            {
                                if (theDB != null) theDB.Rollback();
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
                    catch (Exception ex)
                    {
                        throw ex;
                    }
                }
            }
            catch (Exception ex1)
            {
                throw ex1;
            }
        }

        //Que와 DB랑 불일치하는 Que 삭제 위해 Data Check
        public static List<TransferData> getDeleteQueList(Dictionary<string, TransferData> dic)
        {
            List<TransferData> list = new List<TransferData>();

            lock (transferDataList)
            {
                foreach (TransferData deleteData in transferDataList.Values)
                {
                    if(!(dic.ContainsKey(deleteData.Commandid))) 
                    {
                        list.Add(deleteData);
                    }
                }
            }

            return list;
        }

        //신규 반송을 반송 Que에 Add함.
        public static void addTranferData(TransferData tfData)
        {
            lock (transferDataList)
            {
                if (!(transferDataList.ContainsKey(tfData.Commandid)))
                {
                    transferDataList.Add(tfData.Commandid, tfData);
                }
            }
        }

        //반송 변경 상태를 Que에 저장함.
        public static void changeTranferData(TransferData tfData)
        {
            lock (transferDataList)
            {
                if (transferDataList.ContainsKey(tfData.Commandid))
                {
                    transferDataList[tfData.Commandid] = tfData;
                }
            }
        }

        //Que의 반송을 삭제함.
        public static void removeTranferData(TransferData tfData)
        {
            lock (transferDataList)
            {
                if (transferDataList.ContainsKey(tfData.Commandid))
                {
                    transferDataList.Remove(tfData.Commandid);
                }
            }
        }

        //Commandid를 통해서 반송 정보를 가져옴.
        public static TransferData getTransferData(string commandid)
        {
            TransferData tfData = null;
            lock (transferDataList)
            {
                if (transferDataList.ContainsKey(commandid))
                {
                    tfData = transferDataList[commandid];
                }
            }

            return tfData;
        }

        //반송 정보를 일괄로 변경함.
        public static void changeTransferDataList(List<TransferData> utfDataList)
        {
            lock (transferDataList)
            {
                foreach (TransferData tfDataTemp in utfDataList)
                {
                    if (transferDataList.ContainsKey(tfDataTemp.Commandid))
                    {
                        transferDataList[tfDataTemp.Commandid] = tfDataTemp;
                    }
                }
            }
        }

        /// <summary>
        ///8. 전체 반송 정보를 조회하는 메소드
        ///  :처음 manager가 생성될 경우에 반송 정도 등록
        ///   -> TB_TRANSFER 테이블에 미반송 정보를 읽어서 Que에 저장함.
        ///   -> TB_TRANSFERDETAIL을 확인하여 반송 쪼개기 여부 변경
        /// </summary>

        //QUE에서 반송 정보를 일괄로 가져옴
        public static Dictionary<string, TransferData> getTransferDataList()
        {
            Dictionary<string, TransferData> list = new Dictionary<string, TransferData>();

            lock (transferDataList)
            {
                foreach (TransferData tfDataTemp in transferDataList.Values)
                {
                    list.Add(tfDataTemp.Commandid, tfDataTemp);
                }
            }

            return list;
        }

        //Sorting된 반송 List를 던져줌.
        public static Dictionary<string, TransferData> getTransferDataPriorityList()
        {
            Dictionary<string, TransferData> list = new Dictionary<string, TransferData>();

            lock (transferDataList)
            {
                try
                {
                    CalculationMoveRule calculationMoveRule = new CalculationMoveRule();
                    list = calculationMoveRule.getTransferDataPriorityList(transferDataList);
                }
                catch (Exception e)
                {
                }
            }

            return list;
        }

        #endregion
    }
}
