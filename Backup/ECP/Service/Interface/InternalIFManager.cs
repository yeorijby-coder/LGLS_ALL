using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using ECP.Db.Entity;
using System.Timers;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Util;

namespace ECP.Service.Interface
{
    /// <summary>
    /// Internal I/F를 관리 함.
    /// </summary>
    public class InternalIFManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "InternalIFManager";

        //SAP I/F check : 기준 정보로 빼내야 함.
        Timer timerLocalIF = new Timer();


        //Transfer check : 기준 정보로 빼내야 함.
        Timer timerGlobalIF = new Timer();

        protected static Boolean timerLocalIFFlag = false;
        protected static Boolean timerGlobalIFFlag = false;

        /// <summary>
        /// InternalIFManager 생성자
        /// 주어진 Owner Id와 Manager Id로 InternalIFManager를 생성한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public InternalIFManager(string ownerId, string id)
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

            timerGlobalIF.Interval = 10000;
            timerGlobalIF.Start();

            timerLocalIF.Interval = 10000;
            timerLocalIF.Start();

            timerGlobalIF.Elapsed += new ElapsedEventHandler(checkGlobalIF);
            timerLocalIF.Elapsed += new ElapsedEventHandler(checkLocalIF);
        }

        //1.Global TB_INTERNALIF Data를 Local DB로 결과 전송
        void checkGlobalIF(object sender, ElapsedEventArgs e)
        {

            //timer Flag가 false 일 경우에만 진행함.
            if (!timerGlobalIFFlag)
            {
                //timer 시작 Flag True
                timerGlobalIF.Stop();
                timerGlobalIFFlag = true;

                try
                {
                    checkGlobalInternalIfDataList();
                }
                catch (Exception ex)
                {
                }
                finally
                {
                    CodeMasterData codeMaster = new CodeMasterData();

                    try
                    {
                        List<CodeMasterData> list = codeMaster.getInternalIfTimeValues("01");
                        timerGlobalIF.Interval = double.Parse(list[0].Value);
                    }
                    catch (Exception exe)
                    {
                        timerGlobalIF.Interval = 10000;
                    }

                    //timer 종료 Flag false
                    timerGlobalIFFlag = false;
                    timerGlobalIF.Start();
                }
            }
        }

        //2.내부TB_INTERNALIF Data를 Global DB로 결과 전송
        void checkLocalIF(object sender, ElapsedEventArgs e)
        {

            //timer Flag가 false 일 경우에만 진행함.
            if (!timerLocalIFFlag)
            {
                //timer 시작 Flag True
                timerLocalIF.Stop();
                timerLocalIFFlag = true;

                try
                {
                    checkLocalInternalIfDataList();
                }
                catch (Exception ex)
                {
                    throw ex;
                }
                finally
                {
                    CodeMasterData codeMaster = new CodeMasterData();

                    try
                    {
                        List<CodeMasterData> list = codeMaster.getInternalIfTimeValues("02");
                        timerLocalIF.Interval = double.Parse(list[0].Value);
                    }
                    catch (Exception exe)
                    {
                        timerLocalIF.Interval = 10000;
                    }

                    //timer 종료 Flag false
                    timerLocalIFFlag = false;
                    timerLocalIF.Start();
                }
            }
        }

        //DB에서 반송을 조회해서 Que에 Setting함.
        //Local에 있는 정보를 Global에 올리기 위한 메소드
        public void checkLocalInternalIfDataList()
        {
            List<InternalIfData> list = new List<InternalIfData>();
            InternalIfData internalIfData = new InternalIfData();

            //Transaction 시작 시간
            string strTransStartTime = "";
            string strTransCompleteTime = "";

            try
            {
                //Local 완료 Data 삭제
                internalIfData.deleteLocalInternalIfData();

                list = internalIfData.getLocalInternalIfDataList();

                foreach (InternalIfData internalIfDataTemp in list)
                {
                    try
                    {
                        //Transaction 시작시간
                        strTransStartTime = DateTime.Now.ToString("yyyyMMddHHmmss");

                        excuteLocalIfTransaction(internalIfDataTemp);

                        //Transaction 완료 시간
                        strTransCompleteTime = DateTime.Now.ToString("yyyyMMddHHmmss");

                        internalIfDataTemp.Transstarttime = strTransStartTime;
                        internalIfDataTemp.Transcompletetime = strTransCompleteTime;

                        internalIfDataTemp.updateLocalInternalIfData();
                    }
                    catch (Exception ex)
                    {
                        continue;
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //DB에서 반송을 조회해서 Que에 Setting함.
        //Global에 있는 정보를 Local에 내리기 위한 메소드
        public void checkGlobalInternalIfDataList()
        {
            List<InternalIfData> list = new List<InternalIfData>();
            InternalIfData internalIfData = new InternalIfData();
            
            //Transaction 시작 시간
            string strTransStartTime = "";
            string strTransCompleteTime = "";

            try
            {
                //Local 완료 Data 삭제
                internalIfData.deleteGlobalInternalIfData();

                list = internalIfData.getGlobalInternalIfDataList();

                foreach (InternalIfData internalIfDataTemp in list)
                {
                    //Transaction 시작시간
                    strTransStartTime = DateTime.Now.ToString("yyyyMMddHHmmss");

                    excuteGlobalIfTransaction(internalIfDataTemp);

                    //Transaction 완료 시간
                    strTransCompleteTime = DateTime.Now.ToString("yyyyMMddHHmmss");

                    internalIfDataTemp.Transstarttime = strTransStartTime;
                    internalIfDataTemp.Transcompletetime = strTransCompleteTime;

                    internalIfDataTemp.updateGlobalInternalIfData();
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        //Local에 있는 정보를 Global에 올리기 위한 메소드
        public void excuteLocalIfTransaction(InternalIfData data)
        {
            string[] list = null;

            DBConnection theGlobalDB = null;
            string GlobalDBConnectionString = "";
            DBConnectionValue dbValue = new DBConnectionValue();

            GlobalDBConnectionString = dbValue.getGlobalDBConnectionString();

            try
            {
                theGlobalDB = DBControl.GetConnection(GlobalDBConnectionString);

                switch (data.Internaliftype)
                {
                    case "TransferData":
                        TransferHistData transferHistData = new TransferHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferHistData.setTransferHistData(list);
                        transferHistData.insertTransferHistData(theGlobalDB);

                        TransferData transferData = null;
                        transferData = transferHistData.setTransferData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferData.insertTransferData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            transferData.updateTransferData(theGlobalDB, false);
                        }
                        else
                        {
                            transferData.deleteTransferData(theGlobalDB, false);
                        }

                        break;
                    case "TransferDetailData":
                        TransferDetailHistData transferDetailHistData = new TransferDetailHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferDetailHistData.setTransferDetailHistData(list);
                        transferDetailHistData.insertTransferDetailHistData(theGlobalDB);

                        TransferDetailData transferDetailData = null;
                        transferDetailData = transferDetailHistData.setTransferDetailData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferDetailData.insertTransferDetailData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            transferDetailData.updateTransferDetailData(theGlobalDB, false);
                        }
                        else
                        {
                            transferDetailData.deleteTransferDetailData(theGlobalDB, false);
                        }

                        break;
                    case "HirackData":
                        HirackHistData hirackHistData = new HirackHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        hirackHistData.setHirackHistData(list);
                        hirackHistData.insertHirackHistData(theGlobalDB);

                        HirackData hirackData = null;
                        hirackData = hirackHistData.setHirackData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            //Hirack 정보 동기화 Update로 DB Backup Flag false
                            //Hirack 정보 동기화 Update로 SAP I/F Flag false
                            hirackData.updateHirackData(theGlobalDB, false, false);
                        }

                        break;
                    case "PalletData":
                        PalletHistData palletHistData = new PalletHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        palletHistData.setPalletHistData(list);
                        palletHistData.insertPalletHistData(theGlobalDB);

                        PalletData palletData = null;
                        palletData = palletHistData.setPalletData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            palletData.insertPalletData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            palletData.updatePalletData(theGlobalDB, false);
                        }
                        else
                        {
                            palletData.deletePalletData(theGlobalDB, false);
                        }

                        break;

                    case "PalletBlockingData":
                        PalletBlockingHistData palletBlockingHistData = new PalletBlockingHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        palletBlockingHistData.setPalletBlockingHistData(list);
                        palletBlockingHistData.insertPalletBlockingHistData(theGlobalDB);

                        PalletBlockingData palletBlockingData = null;
                        palletBlockingData = palletBlockingHistData.setPalletBlockingData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            palletBlockingData.insertPalletBlockingData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            palletBlockingData.updatePalletBlockingData(theGlobalDB, false);
                        }
                        else
                        {
                            palletBlockingData.deletePalletBlockingData(theGlobalDB, false);
                        }

                        break;

                    case "TransferDivisionData":
                        TransferDivisionHistData transferDivisionHistData = new TransferDivisionHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferDivisionHistData.setTransferDivisionHistData(list);
                        transferDivisionHistData.insertTransferDivisionHistData(theGlobalDB);

                        TransferDivisionData transferDivisionData = null;
                        transferDivisionData = transferDivisionHistData.setTransferDivisionData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferDivisionData.insertTransferDivisionData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
                        {
                            transferDivisionData.deleteTransferDivisionData(theGlobalDB, false);
                        }

                        break;
                    case "ErrorHistData":
                        ErrorHistData errorHistData = new ErrorHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        errorHistData.setErrorHistData(list);
                        errorHistData.insertErrorHistData(theGlobalDB);
                        break;

                    case "LogData":
                        LogData logData = new LogData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        logData.setLogData(list);
                        logData.insertLogData(theGlobalDB);
                        break;

                    case "UserData":
                        UserHistData userHistData = new UserHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        userHistData.setUserHistData(list);
                        userHistData.insertUserHistData(theGlobalDB);

                        UserData userData = null;
                        userData = userHistData.setUserData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            userData.insertUserData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            userData.updateUserData(theGlobalDB, false);
                        }
                        else
                        {
                            userData.deleteUserData(theGlobalDB, false);
                        }

                        break;
                    case "ProgramData":
                        ProgramHistData programHistData = new ProgramHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        programHistData.setProgramHistData(list);
                        programHistData.insertProgramHistData(theGlobalDB);

                        ProgramData programData = null;
                        programData = programHistData.setProgramData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            programData.insertProgramData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            programData.updateProgramData(theGlobalDB, false);
                        }
                        else
                        {
                            programData.deleteProgramData(theGlobalDB, false);
                        }

                        break;
                    case "RoleData":
                        RoleHistData roleHistData = new RoleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        roleHistData.setRoleHistData(list);
                        roleHistData.insertRoleHistData(theGlobalDB);

                        RoleData roleData = null;
                        roleData = roleHistData.setRoleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            roleData.insertRoleData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            roleData.updateRoleData(theGlobalDB, false);
                        }
                        else
                        {
                            roleData.deleteRoleData(theGlobalDB, false);
                        }

                        break;
                    case "UserRoleData":
                        UserRoleHistData userRoleHistData = new UserRoleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        userRoleHistData.setUserRoleHistData(list);
                        userRoleHistData.insertUserRoleHistData(theGlobalDB);

                        UserRoleData userRoleData = null;
                        userRoleData = userRoleHistData.setUserRoleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            userRoleData.insertUserRoleData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            userRoleData.updateUserRoleData(theGlobalDB, false);
                        }
                        else
                        {
                            userRoleData.deleteUserRoleData(theGlobalDB, false);
                        }

                        break;
                    case "RoleProgramData":
                        RoleProgramHistData roleProgramHistData = new RoleProgramHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        roleProgramHistData.setRoleProgramHistData(list);
                        roleProgramHistData.insertRoleProgramHistData(theGlobalDB);

                        RoleProgramData roleProgramData = null;
                        roleProgramData = roleProgramHistData.setRoleProgramData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            roleProgramData.insertRoleProgramData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            roleProgramData.updateRoleProgramData(theGlobalDB, false);
                        }
                        else
                        {
                            roleProgramData.deleteRoleProgramData(theGlobalDB, false);
                        }

                        break;
                    case "MaterialData":
                        MaterialHistData materialHistData = new MaterialHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        materialHistData.setMaterialHistData(list);
                        materialHistData.insertMaterialHistData(theGlobalDB);

                        MaterialData materialData = null;
                        materialData = materialHistData.setMaterialData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            materialData.insertMaterialData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            materialData.updateMaterialData(theGlobalDB, false);
                        }
                        else
                        {
                            materialData.deleteMaterialData(theGlobalDB, false);
                        }

                        break;
                    case "BatchData":
                        BatchHistData batchHistData = new BatchHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        batchHistData.setBatchHistData(list);
                        batchHistData.insertBatchHistData(theGlobalDB);

                        BatchData batchData = null;
                        batchData = batchHistData.setBatchData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            batchData.insertBatchData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            batchData.updateBatchData(theGlobalDB, false);
                        }
                        else
                        {
                            batchData.deleteBatchData(theGlobalDB, false);
                        }

                        break;
                    case "EquipmentData":
                        EquipmentHistData equipmentHistData = new EquipmentHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        equipmentHistData.setEquipmentHistData(list);
                        equipmentHistData.insertEquipmentHistData(theGlobalDB);

                        EquipmentData equipmentData = null;
                        equipmentData = equipmentHistData.setEquipmentData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            equipmentData.insertEquipmentData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            equipmentData.updateEquipmentData(theGlobalDB, false);
                        }
                        else
                        {
                            equipmentData.deleteEquipmentData(theGlobalDB, false);
                        }

                        break;
                    case "CodeMasterData":
                        CodeMasterHistData codemasterHistData = new CodeMasterHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        codemasterHistData.setCodeMasterHistData(list);
                        codemasterHistData.insertCodeMasterHistData(theGlobalDB);

                        CodeMasterData codemasterData = null;
                        codemasterData = codemasterHistData.setCodeMasterData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            codemasterData.insertCodeMasterData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            codemasterData.updateCodeMasterData(theGlobalDB, false);
                        }
                        else
                        {
                            codemasterData.deleteCodeMasterData(theGlobalDB, false);
                        }

                        break;
                    case "MoveRuleData":
                        MoveRuleHistData moveruleHistData = new MoveRuleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        moveruleHistData.setMoveRuleHistData(list);
                        moveruleHistData.insertMoveRuleHistData(theGlobalDB);

                        MoveRuleData moveruleData = null;
                        moveruleData = moveruleHistData.setMoveRuleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            moveruleData.insertMoveRuleData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            moveruleData.updateMoveRuleData(theGlobalDB, false);
                        }
                        else
                        {
                            moveruleData.deleteMoveRuleData(theGlobalDB, false);
                        }

                        break;
                    case "MovementSapIfData":
                        MovementSapIfHistData movementSapIfHistData = new MovementSapIfHistData();

                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        movementSapIfHistData.setMovementSapIfHistData(list);
                        movementSapIfHistData.insertMovementSapIfHistData(theGlobalDB);

                        MovementSapIfData movementSapIfData = null;
                        movementSapIfData = movementSapIfHistData.setMovementSapIfData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            movementSapIfData.insertMovementSapIfData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            movementSapIfData.updateMovementSapIfData(theGlobalDB, false);
                        }

                        break;
                    case "LeadTimeData":
                        LeadTimeHistData leadTimeHistData = new LeadTimeHistData();

                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        leadTimeHistData.setLeadTimeHistData(list);
                        leadTimeHistData.insertLeadTimeHistData(theGlobalDB);

                        LeadTimeData leadtimeData = null;
                        leadtimeData = leadTimeHistData.setLeadTimeData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            leadtimeData.insertLeadTimeData(theGlobalDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            leadtimeData.updateLeadTimeData(theGlobalDB, false);
                        }

                        break;
                    default:
                        break;
                }
            }
            catch (Exception ex)
            {
                if (theGlobalDB != null) theGlobalDB.Rollback();
                throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(theGlobalDB);
            }
        }

        public void excuteGlobalIfTransaction(InternalIfData data)
        {
            string[] list = null;

            DBConnection theDB = null;
            
            try
            {
                theDB = DBControl.GetConnection();

                switch (data.Internaliftype)
                {
                    case "TransferData":
                        TransferHistData transferHistData = new TransferHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferHistData.setTransferHistData(list);
                        transferHistData.insertTransferHistData();

                        TransferData transferData = null;
                        transferData = transferHistData.setTransferData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferData.insertTransferData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            transferData.updateTransferData(theDB, false);
                        }
                        else
                        {
                            transferData.deleteTransferData(theDB, false);
                        }

                        break;
                    case "TransferDetailData":
                        TransferDetailHistData transferDetailHistData = new TransferDetailHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferDetailHistData.setTransferDetailHistData(list);
                        transferDetailHistData.insertTransferDetailHistData();

                        TransferDetailData transferDetailData = null;
                        transferDetailData = transferDetailHistData.setTransferDetailData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferDetailData.insertTransferDetailData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            transferDetailData.updateTransferDetailData(theDB, false);
                        }
                        else
                        {
                            transferDetailData.deleteTransferDetailData(theDB, false);
                        }

                        break;
                    case "HirackData":
                        HirackHistData hirackHistData = new HirackHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        hirackHistData.setHirackHistData(list);
                        hirackHistData.insertHirackHistData();

                        HirackData hirackData = null;
                        hirackData = hirackHistData.setHirackData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            //Hirack 정보 동기화 Update로 DB Backup Flag false
                            //Hirack 정보 동기화 Update로 SAP I/F Flag false
                            hirackData.updateHirackData(theDB, false, false);
                        }

                        break;
                    case "PalletData":
                        PalletHistData palletHistData = new PalletHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        palletHistData.setPalletHistData(list);
                        palletHistData.insertPalletHistData();

                        PalletData palletData = null;
                        palletData = palletHistData.setPalletData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            palletData.insertPalletData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            palletData.updatePalletData(theDB, false);
                        }
                        else
                        {
                            palletData.deletePalletData(theDB, false);
                        }

                        break;

                    case "PalletBlockingData":
                        PalletBlockingHistData palletBlockingHistData = new PalletBlockingHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        palletBlockingHistData.setPalletBlockingHistData(list);
                        palletBlockingHistData.insertPalletBlockingHistData();

                        PalletBlockingData palletBlockingData = null;
                        palletBlockingData = palletBlockingHistData.setPalletBlockingData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            palletBlockingData.insertPalletBlockingData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            palletBlockingData.updatePalletBlockingData(theDB, false);
                        }
                        else
                        {
                            palletBlockingData.deletePalletBlockingData(theDB, false);
                        }

                        break;

                    case "TransferDivisionData":
                        TransferDivisionHistData transferDivisionHistData = new TransferDivisionHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        transferDivisionHistData.setTransferDivisionHistData(list);
                        transferDivisionHistData.insertTransferDivisionHistData();

                        TransferDivisionData transferDivisionData = null;
                        transferDivisionData = transferDivisionHistData.setTransferDivisionData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            transferDivisionData.insertTransferDivisionData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
                        {
                            transferDivisionData.deleteTransferDivisionData(theDB, false);
                        }

                        break;
                    case "ErrorHistData":
                        ErrorHistData errorHistData = new ErrorHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        errorHistData.setErrorHistData(list);
                        errorHistData.insertErrorHistData();
                        break;

                    case "LogData":
                        LogData logData = new LogData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        logData.setLogData(list);
                        logData.insertLogData();
                        break;

                    case "UserData":
                        UserHistData userHistData = new UserHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        userHistData.setUserHistData(list);
                        userHistData.insertUserHistData();

                        UserData userData = null;
                        userData = userHistData.setUserData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            userData.insertUserData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            userData.updateUserData(theDB, false);
                        }
                        else
                        {
                            userData.deleteUserData(theDB, false);
                        }

                        break;
                    case "ProgramData":
                        ProgramHistData programHistData = new ProgramHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        programHistData.setProgramHistData(list);
                        programHistData.insertProgramHistData();

                        ProgramData programData = null;
                        programData = programHistData.setProgramData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            programData.insertProgramData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            programData.updateProgramData(theDB, false);
                        }
                        else
                        {
                            programData.deleteProgramData(theDB, false);
                        }

                        break;
                    case "RoleData":
                        RoleHistData roleHistData = new RoleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        roleHistData.setRoleHistData(list);
                        roleHistData.insertRoleHistData();

                        RoleData roleData = null;
                        roleData = roleHistData.setRoleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            roleData.insertRoleData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            roleData.updateRoleData(theDB, false);
                        }
                        else
                        {
                            roleData.deleteRoleData(theDB, false);
                        }

                        break;
                    case "UserRoleData":
                        UserRoleHistData userRoleHistData = new UserRoleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        userRoleHistData.setUserRoleHistData(list);
                        userRoleHistData.insertUserRoleHistData();

                        UserRoleData userRoleData = null;
                        userRoleData = userRoleHistData.setUserRoleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            userRoleData.insertUserRoleData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            userRoleData.updateUserRoleData(theDB, false);
                        }
                        else
                        {
                            userRoleData.deleteUserRoleData(theDB, false);
                        }

                        break;
                    case "RoleProgramData":
                        RoleProgramHistData roleProgramHistData = new RoleProgramHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        roleProgramHistData.setRoleProgramHistData(list);
                        roleProgramHistData.insertRoleProgramHistData();

                        RoleProgramData roleProgramData = null;
                        roleProgramData = roleProgramHistData.setRoleProgramData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            roleProgramData.insertRoleProgramData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            roleProgramData.updateRoleProgramData(theDB, false);
                        }
                        else
                        {
                            roleProgramData.deleteRoleProgramData(theDB, false);
                        }

                        break;
                    case "MaterialData":
                        MaterialHistData materialHistData = new MaterialHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        materialHistData.setMaterialHistData(list);
                        materialHistData.insertMaterialHistData();

                        MaterialData materialData = null;
                        materialData = materialHistData.setMaterialData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            materialData.insertMaterialData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            materialData.updateMaterialData(theDB, false);
                        }
                        else
                        {
                            materialData.deleteMaterialData(theDB, false);
                        }

                        break;
                    case "BatchData":
                        BatchHistData batchHistData = new BatchHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        batchHistData.setBatchHistData(list);
                        batchHistData.insertBatchHistData();

                        BatchData batchData = null;
                        batchData = batchHistData.setBatchData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            batchData.insertBatchData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            batchData.updateBatchData(theDB, false);
                        }
                        else
                        {
                            batchData.deleteBatchData(theDB, false);
                        }

                        break;
                    case "EquipmentData":
                        EquipmentHistData equipmentHistData = new EquipmentHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        equipmentHistData.setEquipmentHistData(list);
                        equipmentHistData.insertEquipmentHistData();

                        EquipmentData equipmentData = null;
                        equipmentData = equipmentHistData.setEquipmentData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            equipmentData.insertEquipmentData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            equipmentData.updateEquipmentData(theDB, false);
                        }
                        else
                        {
                            equipmentData.deleteEquipmentData(theDB, false);
                        }

                        break;
                    case "CodeMasterData":
                        CodeMasterHistData codemasterHistData = new CodeMasterHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        codemasterHistData.setCodeMasterHistData(list);
                        codemasterHistData.insertCodeMasterHistData();

                        CodeMasterData codemasterData = null;
                        codemasterData = codemasterHistData.setCodeMasterData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            codemasterData.insertCodeMasterData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            codemasterData.updateCodeMasterData(theDB, false);
                        }
                        else
                        {
                            codemasterData.deleteCodeMasterData(theDB, false);
                        }

                        break;
                    case "MoveRuleData":
                        MoveRuleHistData moveruleHistData = new MoveRuleHistData();
                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        moveruleHistData.setMoveRuleHistData(list);
                        moveruleHistData.insertMoveRuleHistData();

                        MoveRuleData moveruleData = null;
                        moveruleData = moveruleHistData.setMoveRuleData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            moveruleData.insertMoveRuleData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            moveruleData.updateMoveRuleData(theDB, false);
                        }
                        else
                        {
                            moveruleData.deleteMoveRuleData(theDB, false);
                        }

                        break;
                    case "MovementSapIfData":
                        MovementSapIfHistData movementSapIfHistData = new MovementSapIfHistData();

                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        movementSapIfHistData.setMovementSapIfHistData(list);
                        movementSapIfHistData.insertMovementSapIfHistData(theDB);

                        MovementSapIfData movementSapIfData = null;
                        movementSapIfData = movementSapIfHistData.setMovementSapIfData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            movementSapIfData.insertMovementSapIfData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            movementSapIfData.updateMovementSapIfData(theDB, false);
                        }

                        break;
                    case "LeadTimeData":
                        LeadTimeHistData leadTimeHistData = new LeadTimeHistData();

                        list = null;
                        list = data.Ifvalue.Split(GlobalConstant.MOVEMENT_DELIMITER);

                        leadTimeHistData.setLeadTimeHistData(list);
                        leadTimeHistData.insertLeadTimeHistData(theDB);

                        LeadTimeData leadtimeData = null;
                        leadtimeData = leadTimeHistData.setLeadTimeData(list);

                        if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Insert.ToString())
                        {
                            leadtimeData.insertLeadTimeData(theDB, false);
                        }
                        else if (data.Transactiontype == EntityEnumData.TRANSACTIONTYPE.Update.ToString())
                        {
                            leadtimeData.updateLeadTimeData(theDB, false);
                        }

                        break;
                    default:
                        break;
                }
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

        #endregion method
    }
}
