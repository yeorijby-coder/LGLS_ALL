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
using ECP.Device.Observe;

namespace HECS.Device
{
    [Serializable]
    public class ConveyorSEM : TransportDevice
    {
        /// CLASS 명
        /// </summary>
        private const string CLASS = "ConveyorSEM";

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

        #endregion Attribute_Persistance;

        #region Attribute_Not_Persistance

        #endregion Attribute_Not_Persistance

        #region Constructor

        /// <summary>
        /// 객체 생성자
        /// </summary>
        public ConveyorSEM()
        {
        }

        /// <summary>
        /// 객체 생성자
        /// </summary>
        public ConveyorSEM(string stockerId)
            : base(stockerId)
        {
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

                //2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                //psparam.SetVarChar("ID", ((this.elementId == null) ? "" : this.elementId));

                //reader = theDB.ExecuteQuery("select * from SEM where ID =@ID", psparam);
                //if (reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "ConveyorSEM", this.iD + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                //    pss.SetVarChar("ID", ((this.elementId == null) ? "" : this.elementId));
                //    pss.SetVarChar("HOSTID", ((this.hostID==null)? "": this.iD));
                //    pss.SetVarChar("MAKER", ((this.maker==null) ?"": this.maker));

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
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "ConveyorSEM", this.iD + "doesnot exist");
                //}
                this.Id = this.elementId;
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StockerSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "ConveyorSEM", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StockerSem", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "ConveyorSEM", "error \n " + ex.Message.ToString());
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
                Log.log(5, METHOD, "error", Category.ERROR, "", "ConveyorSEM", "sql error \n [" + tSql + "\n]" + ex.Message.ToString());
            }
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련


        public override void Init()
        {
            base.Init();
            AddSubScribe();
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

                foreach (string key in this.Observables.Keys)
                {
                    Observables[key].ObservableLogEvent += new ECP.Device.Observe.delegateObservableLogEvent(ConveyorSEM_ObservableLogEvent);
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

        void ConveyorSEM_ObservableLogEvent(object sender, ECP.Device.Observe.ObservableStatisticsCollectorData collectionData)
        {
            try
            {
                collectionData.ownerId = this.HostID;
                StatisticsCollector.TriggerOccured(
                                                    ObservableStatisticsCollectorData.COLLECTOR_ID,
                                                    ObservableStatisticsCollectorData.TRIGGER_ID,
                                                    collectionData);
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                    RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                    RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                    new RunTimeExceptionStatisticsCollectorData(CLASS, "StockerSEM_ObservableLogEvent", "", ex.ToString(), ex.StackTrace.ToString()));
            }
        }
        #endregion Event_Handler

        #region Method

        #endregion Method


    }
}
