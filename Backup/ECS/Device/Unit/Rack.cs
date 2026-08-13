#define LOCK_DEGUB
using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Device;
using ECP.Db;
using ECP.Global.Device;
using ECP.Service.Material;
using ECP.Util.Client;
using ECP.Util;
using ECP.Service.Statistics;
using HECS.Statistics;



namespace HECS.Device.Unit
{
    
    public partial class Rack : SubSystem
    {
        private const string CLASS = "Rack";

        private bool dirtyFlag = true;

        #region Attribute_Not_Persistance
    
        #endregion Attribute_Not_Persistance

        #region Constructor
        public Rack()
        {
        }
        #endregion Constructor

        #region Manage_Persistent_Attribute
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                //psparam.SetVarChar("ID", this.ID);

                //reader = theDB.ExecuteQuery("select * from RACK where ID =@ID", psparam);
                //if(reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "RACK", this.ID + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                //    pss.SetVarChar("ID", this.ID);
                //    string tSql = "insert into RACK (ID)" +
                //        " values (@ID)";
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "RACK", "sql : " + tSql);
                //    theDB.ExecuteUpdate(tSql, pss);
                //}
            }
            catch(SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "RACK", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch(Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

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

                //reader = theDB.ExecuteQuery("select * from RACK where ID =@ID", psqlparam);

                //if(reader.Read())
                //{
                //    this.ID = (string)reader["ID"];
                //}
                //else
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "RACK", this.ID + "doesnot exist");
                //}

                this.Id  = this.elementId;
            }
            catch(SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;

            }
            catch(Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }

        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".delete";

            try
            {
                base.Delete(theDB);

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                //ps.SetVarChar("ID", this.ID);
                //theDB.ExecuteUpdate("delete from RACK where ID =@ID", ps);
            }
            catch(SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "RACK", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch(Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {

            }
        }

        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".save";

            try
            {
                base.Save(theDB);

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //if(dirtyFlag)
                //{
                //}
            }
            catch(SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "RACK", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch(Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {

            }
        }
        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련
        public override void Init()
        {
            base.Init();
        }

        public override void Startup()
        {
            base.Startup();
        }

        public override void Shutdown()
        {
            base.Shutdown();
        }

        #endregion 초기화및기동관련

   
    }
}

