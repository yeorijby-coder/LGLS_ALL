

using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Device;
using ECP.Db;
using ECP.Global.Device;

using HECS.Statistics;
using ECP.Service.Statistics;
using ECP.Interfaces.Unit;

namespace HECS.Device.Unit
{
    /// <summary>
    /// Port는 LocationUnit에서 상속을 받아 LocationUnit이 가지고 있는 
    /// Attribute와 Port만의 Attribute를 사용하고 Persistence class에서 
    /// 선언된 Add(), Delete(), Load(), Save()를 구현한다.
    ///
    /// Port 역시 Port의 속성을 가지고 있는 SubClass를 통하여 더욱 구체화
    /// 될 수 있다. (ex. ProcessPort, StockerPort 등)
    /// 실질적인 장비의 BizLogic 구현은 Port를 상속받아 구현하는 class에서
    /// 이루어지며 Port class는 다양한 Port의 common attribute를 DB 접근을
    /// 통하여 관리한다.
    /// </summary>
    [Serializable]
    public class Port : SubSystem
    {
        private const string CLASS = "Port";
        private bool dirtyFlag = true;

        public bool DirtyFlag
        {
            get { return dirtyFlag; }
            set { dirtyFlag = value; }
        }

        private string portOrder = "";

        public string PortOrder
        {
            get
            {
                return portOrder;
            }
            set
            {
                portOrder = value;
              
            }
        }

        private bool isPalletExist = false;

        public bool IsPalletExist
        {
            get
            {
                return isPalletExist;
            }
            set
            {
                isPalletExist = value;
                OnPropertyChanged("IsPalletExist", value);
            }
        }
        private string palletId = "";

        public string PalletId
        {
            get
            {
                return palletId;
            }
            set
            {
                palletId = value;
                OnPropertyChanged("PalletId", value);
            }
        }


        #region Constructor

        public Port()
        {
        }

        #endregion Constructor



        #region Attribute_Not_Persistance

        #endregion Attribute_Not_Persistance

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

                //reader = theDB.ExecuteQuery("select * from PORT where ID =@ID", psparam);
                //if (reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "Port", this.ID + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                //    pss.SetVarChar("ID", this.ID);
                //    pss.SetVarChar("HOSTID", this.hostID);

                //    theDB.ExecuteUpdate("insert into PORT (ID,HOSTID) values" +
                //        " (@ID,@HOSTID)", pss);
                //}
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "Port", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
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

                //reader = theDB.ExecuteQuery("select * from PORT where ID =@ID", psqlparam);

                //if (reader.Read())
                //{
                //    this.ID = (string)reader["ID"];
                //    this.hostID = reader["HOSTID"].Equals(DBNull.Value) ? "" : (string)reader["HOSTID"];
                //}
                //else
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "Port", this.ID + "doesnot exist");
                //}

                this.Id = this.elementId;


                //2009년 5월 25일, 신승원 
                //여기서 에러남. 수정요망
                // 이 부분은 UD_PROPERTY 테이블이 수정되었으므로, 사용법을 확인할것.
                //string order = GetUserDefinedPropertyValue("PORT_ORDER", "", true);
                //if (string.IsNullOrEmpty(order))
                //{
                //    order = GetDefaultPortOrder(this.elementId);
                //}
                //SetUserDefinedPropertyValue("PORT_ORDER", order, "Conveyor내의 버퍼순서");
                string order = GetDefaultPortOrder(this.elementId);
                portOrder = order;
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }



        private string GetDefaultPortOrder(string elementId)
        {
            string order = "";
            switch (elementId)
            {
                case "PORT:1":
                    order = "1";
                    break;
                case "PORT:2":
                    order = "2";
                    break;
                case "PORT:3":
                    order = "1";
                    break;
                case "PORT:4":
                    order = "2";
                    break;
                case "PORT:5":
                    order = "1";
                    break;
                case "PORT:6":
                    order = "2";
                    break;
                case "PORT:7":
                    order = "1";
                    break;
                case "PORT:8":
                    order = "2";
                    break;
                case "PORT:9":
                    order = "1";
                    break;
                case "PORT:10":
                    order = "2";
                    break;
                case "PORT:11":
                    order = "1";
                    break;
                case "PORT:12":
                    order = "2";
                    break;
                case "PORT:13":
                    order = "1";
                    break;
                case "PORT:14":
                    order = "2";
                    break;
                case "PORT:15":
                    order = "1";
                    break;
                case "PORT:16":
                    order = "2";
                    break;
                case "PORT:17":
                    order = "1";
                    break;
                case "PORT:18":
                    order = "2";
                    break;
                case "PORT:19":
                    order = "1";
                    break;
                case "PORT:20":
                    order = "2";
                    break;
                case "PORT:21":
                    order = "1";
                    break;
                case "PORT:22":
                    order = "2";
                    break;
                case "PORT:23":
                    order = "1";
                    break;
                case "PORT:24":
                    order = "2";
                    break;
                case "PORT:25":
                    order = "1";
                    break;
                case "PORT:26":
                    order = "2";
                    break;
                case "PORT:27":
                    order = "1";
                    break;
                case "PORT:28":
                    order = "2";
                    break;
                case "PORT:29":
                    order = "3";
                    break;
                case "PORT:30":
                    order = "3";
                    break;
                case "PORT:31":
                    order = "1";
                    break;
                case "PORT:32":
                    order = "2";
                    break;
                case "PORT:33":
                    order = "1";
                    break;
                case "PORT:34":
                    order = "";
                    break;
                case "PORT:35":
                    order = "";
                    break;
                case "PORT:36":
                    order = "";
                    break;
                case "PORT:37":
                    order = "";
                    break;
                case "PORT:38":
                    order = "";
                    break;
                case "PORT:39":
                    order = "";
                    break;
                case "PORT:40":
                    order = "";
                    break;
                case "PORT:41":
                    order = "";
                    break;
                case "PORT:42":
                    order = "";
                    break;
                case "PORT:43":
                    order = "";
                    break;
                case "PORT:44":
                    order = "";
                    break;
                case "PORT:45":
                    order = "";
                    break;
                case "PORT:46":
                    order = "";
                    break;
                case "PORT:47":
                    order = "";
                    break;
                case "PORT:48":
                    order = "";
                    break;
                case "PORT:49":
                    order = "";
                    break;
                case "PORT:50":
                    order = "";
                    break;
                case "PORT:51":
                    order = "";
                    break;
                case "PORT:52":
                    order = "";
                    break;
                case "PORT:53":
                    order = "";
                    break;
                case "PORT:54":
                    order = "";
                    break;
                case "PORT:55":
                    order = "";
                    break;
                case "PORT:56":
                    order = "";
                    break;
                case "PORT:57":
                    order = "";
                    break;
                case "PORT:58":
                    order = "";
                    break;
                case "PORT:59":
                    order = "";
                    break;
                case "PORT:60":
                    order = "";
                    break;
                case "PORT:61":
                    order = "";
                    break;
                case "PORT:62":
                    order = "";
                    break;
                case "PORT:63":
                    order = "";
                    break;
                case "PORT:64":
                    order = "";
                    break;
                case "PORT:65":
                    order = "";
                    break;
                case "PORT:66":
                    order = "";
                    break;
                case "PORT:67":
                    order = "";
                    break;
                case "PORT:68":
                    order = "";
                    break;
                case "PORT:69":
                    order = "";
                    break;
                case "PORT:70":
                    order = "";
                    break;
                case "PORT:71":
                    order = "";
                    break;
                case "PORT:72":
                    order = "";
                    break;
                case "PORT:73":
                    order = "";
                    break;
                case "PORT:74":
                    order = "";
                    break;
                case "PORT:75":
                    order = "";
                    break;
                case "PORT:76":
                    order = "";
                    break;
                case "PORT:77":
                    order = "";
                    break;
                case "PORT:78":
                    order = "";
                    break;
                case "PORT:79":
                    order = "";
                    break;
                case "PORT:80":
                    order = "";
                    break;
                case "PORT:81":
                    order = "";
                    break;
                case "PORT:82":
                    order = "";
                    break;
                case "PORT:83":
                    order = "";
                    break;
                case "PORT:84":
                    order = "";
                    break;
                case "PORT:85":
                    order = "";
                    break;
                case "PORT:86":
                    order = "";
                    break;
                case "PORT:87":
                    order = "";
                    break;
                case "PORT:88":
                    order = "";
                    break;
                case "PORT:89":
                    order = "";
                    break;
            }

            return order;
        }

        public void SetDefaultPortOrder()
        {
            this.portOrder  = GetDefaultPortOrder(this.elementId);
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

                //theDB.ExecuteUpdate("delete from PORT where ID =@ID", ps);
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "Port", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
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
                //if (dirtyFlag)
                //{
                //    this.dirtyFlag = false;
                //    theDB.ExecuteUpdate("update PORT set\n" +
                //        "ID='" + this.iD + "', \n" +
                //        "HOSTID='" + this.hostID + "'\n" +
                //        "where ID = '" + this.iD + "'");
                //}
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "Port", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화 및 기동 관련

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
        #endregion 초기화 및 기동 관련


    }
}
