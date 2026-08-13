using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;

using ECP.Service.Statistics;
using ECP.Device;
using ECP.Device.Observe;
using ECP.Db;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Global.Com;
using ECP.Com.FieldBus;
using ECP.Interfaces.Unit;

using HECS.Statistics;
using HECS.Global.Interface;
using HECS.Event.ECS;

namespace HECS.Device
{
    [Serializable]
    public class ECS : Equipment, IECS
    {
        /// <summary>
        /// CLASS 명
        /// </summary>
        private const string CLASS = "ECS";

        /// <summary>
        /// DB 저장 플래그
        /// </summary>
        private bool dirtyFlag = true;
        public bool DirtyFlag
        {
            get { return dirtyFlag; }
            set { dirtyFlag = value; }
        }

        #region Attribute_Persistance

        /// <summary>
        /// ELEMENT ID
        /// </summary>
        private string id;
        public string Id
        {
            get { return id; }
            set { id = value; this.dirtyFlag = true; }
        }

        /// <summary>
        /// HOST ID - MCS 장비 명
        /// </summary>
        private string hostID;

        /// <summary>
        /// STOCKER MAKER 정보. MAKER별로 관리되어지는 ALARM정보가 다름.
        /// </summary>
        private string maker;
        public string Maker
        {
            get { return maker; }
            set { maker = value; this.dirtyFlag = true; }
        }

        #endregion Attribute_Persistance;

        #region Attribute_Not_Persistance

        #endregion Attribute_Not_Persistance

        #region Constructor

        public ECS()
        { 

        }
        public ECS(string controllerID)
            : base(controllerID)
        {
        }

        #endregion Constructor

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

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                //psparam.SetVarChar("ID", ((this.elementId == null) ? "" : this.elementId));

                //reader = theDB.ExecuteQuery("select * from SEM where ID =@ID", psparam);
                //if (reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "stocker", this.iD + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                //    pss.SetVarChar("ID", ((this.elementId == null) ? "" : this.elementId));
                //    pss.SetVarChar("HOSTID", ((this.hostID == null) ? "" : this.iD));
                //    pss.SetVarChar("MAKER", ((this.maker == null) ? "" : this.maker));

                //    theDB.ExecuteUpdate(
                //        "insert into SEM " +
                //        "(ID,HOSTID,MAKER,SOFTWAREVERSION ) values " +
                //        "(@ID,@HOSTID,@MAKER)", pss);
                //}
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

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psqlparam = theDB.GetPreparedSqlParameter();

                //psqlparam.SetVarChar("ID", this.elementId);
                //reader = theDB.ExecuteQuery("select * from SEM where ID =@ID", psqlparam);

                //if (reader.Read())
                //{
                //    this.iD = (string)reader["ID"];
                //    this.hostID = (string)reader["HOSTID"];
                //    this.maker = (string)reader["MAKER"];
                //}
                //else
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "stocker", this.iD + "doesnot exist");
                //}

                this.id = this.elementId;
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("ECSSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "ECSSem", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("ECSSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "ECSSem", "error \n " + ex.Message.ToString());
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
                Log.log(5, METHOD, "error", Category.ERROR, "", "ECSSem", "sql error \n [" + tSql + "\n]" + ex.Message.ToString());
            }
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련


        public override void Init()
        {
            base.Init();
            AddSubScribe();

            StartDispatcher();
        }

        public override void Startup()
        {
            const string METHOD = CLASS + ".Startup";
            base.Startup();
            InitLogSet();

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
            //Driver Event Handling
            AbstractFieldBusDriverEventSubscribe();

        }

      

        #endregion Event_Handler

        #region Method


        /// <summary>
        /// Log 초기 설정값 SETUP
        /// </summary>
        private void InitLogSet()
        {
            // Observable LOG = PLC LOG
            StatisticsManager.SetTriggerInfo(ObservableStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 60, 100, 10000000));
            StatisticsManager.SetTriggerInfo(DebugStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 30, 100, 10000));

            // FAULT,ALARM LOG
            StatisticsManager.SetTriggerInfo(ErrorStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 30, 100, 10000));

            // 금지단 설정 Histrory LOG
            StatisticsManager.SetTriggerInfo(OperationStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 30, 100, 10000));

            // RUN TIME EXCEPTION LOG
            StatisticsManager.SetTriggerInfo(RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 30, 100, 10000));
            // Thread Lock Monitor
            StatisticsManager.SetTriggerInfo(ThreadLockMonitorStatisticsCollectorData.COLLECTOR_ID, new StatisticalLogInfo(true, 3, 10000, 10000));

        }

        void ECS_ErrorOccurred(AbstractFieldBusDriver driver, int iErrorCode)
        {
            System.Console.WriteLine("ECS Drvier Error : " + iErrorCode.ToString());
            //throw new Exception("The method or operation is not implemented.");
        }

        void ECS_ConnectionStateChanged(AbstractFieldBusDriver driver, FBConnectionState connectionState)
        {
            PublishEvent(DriverStatusEvent.FormSubject("*"),
                                                            new DriverStatusEvent(this.ElementId, String.Empty, connectionState));            
        }


        #endregion Method

        #region IECS 멤버

        public FBConnectionState DriverConnectionStatus
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public string SoftVersion
        {
            get
            {
                throw new Exception("The method or operation is not implemented.");
            }
            set
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }

        public void SetVersion(string strVersion)
        {
            throw new Exception("The method or operation is not implemented.");
        }


        public void ControllerStartUp()
        {
            this.Startup();
        }

        #endregion

        #region IHostUnit 멤버

        public string HostID
        {
            get
            {
                return hostID;
            }
            set
            {
                hostID = value;
            }
        }

        #endregion

        #region IRepresentativeEquipment 멤버

        /// <summary>
        /// 대표 Equipment가 Field Bus Driver를 로딩함
        /// </summary>
        new public void  LoadAbstractFieldBusDrivers()
        {
            base.LoadAbstractFieldBusDrivers();
        }

        /// <summary>
        /// fbdriver 상태 및 에러 발생 Event Handling
        /// </summary>
        public void AbstractFieldBusDriverEventSubscribe()
        {
            foreach (int key in this.fbdrivers.Keys)
            {
                if (this.fbdrivers[key].UseFlag)
                {
                    this.fbdrivers[key].ErrorOccurred += new delegateDriverErrorOccurred(ECS_ErrorOccurred);
                    this.fbdrivers[key].ConnectionStateChanged += new delegateDriverConnectionStateChanged(ECS_ConnectionStateChanged);
                }
            }
           
        }


        #endregion

        #region IRepresentativeEquipment 멤버



        #endregion
    }


}
