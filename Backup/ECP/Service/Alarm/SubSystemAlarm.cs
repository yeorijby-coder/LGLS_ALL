using System;
using System.Collections.Generic;
using System.Text;

using ECP.Db;
using ECP.Global.Device;
using ECP.Service.Logger;
using System.Data.SqlClient;
using System.Runtime.Remoting;
using ECP.Util;

namespace ECP.Service.Alarm
{
    /// <summary>
    /// 각 SubSystem Alarm을 Manager에서 관리 할 수 있도록 메모리에 적재 및 관리
    /// 단 Alarm의 Rule은 각 Subsystem별로 다를 수 있으므로 LoadAlarms()을 구현해야 함.
    /// </summary>
    public class SubSystemAlarm : Persistence
    {
        private  const string CLASS = "SubSystemAlarm";

        public  string className = "ECP.Service.Alarm.SubSystemAlarm";

        public  string assemblyName = "ECP";


        private EquipmentType equipmentType;

        public EquipmentType EquipmentType
        {
            get { return equipmentType; }
            set { equipmentType = value; }
        }

        public SubSystemAlarm()
        {
        }


        #region Manage_Persistance_Attribute

        /// <summary>
        /// SubSystem 별로 구체화된 Alarm을 Loading 할 수 있도록 Subsystem의 Alarm 개체를
        /// Activation하여 Loading 해줌
        /// </summary>
        /// <returns>각 SubSystem에서 Loading된 Alarm정보</returns>
        public static Dictionary<long, EcpAlarm> LoadSubSystemAlarms()
        {
            const string METHOD = CLASS + ".LoadSubSystemAlarms";

            Dictionary<long, EcpAlarm> alarms = new Dictionary<long, EcpAlarm>();
              
//2009. 04.21 윤선화 주석처리 SubSystemAlarm으로 Alarm을 통일함.
            //ObjectHandle objHandle = null;
            //SubSystemAlarm alarm = null;

            //DBConnection conn = DBControl.GetConnection();

            //string sqlstring = "SELECT S.ASSEMBLYNAME , S.CLASSNAME from TB_EQUIPMENT AS E , TB_SUBSYSTEM_ALARM AS S WHERE E.EQUIPMENTTYPE = S.EQUIPMENTTYPE";

            //SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            //if(reader.Read())
            //{
            //    try
            //    {
            //        objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
            //        alarm = (SubSystemAlarm)objHandle.Unwrap();
            //    }
            //    catch (Exception ex)
            //    {
            //        Log.log(5, METHOD, "error", Category.ERROR, "", "carrier", "failed to load carriers" + ex.Message.ToString());
            //    }

            //    alarms= alarm.LoadAlarms();
            //}

            //if (reader != null) reader.Close();

            //DBControl.RemoveDBConection(conn);


//2009. 04.21 윤선화 주석처리 SubSystemAlarm으로 Alarm을 구현함.
            EcpAlarm alarm;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            try
            {
                theDB = DBControl.GetConnection();

                String sql = "SELECT * from TB_ALARM ";

                reader = theDB.ExecuteQuery(sql);

                while (reader.Read())
                {
                    alarm = new EcpAlarm();
                    alarm.ALARMID = Convert.ToInt32((string)reader["ALARMID"]);
                    alarm.ALARMCD = Convert.ToByte((string)reader["ALARMCD"]);
                    alarm.ALARMTEXT = (string)reader["ALARMTEXT"];
                    alarm.EQUIPMENTTYPE = EquipmentType.StringToValue((string)reader["EQUIPMENTTYPE"]);
                    alarm.LEVEL = (string)reader["LEVEL"];
                    alarms.Add(alarm.ALARMID, alarm);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "TB_ALARM", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                if (reader != null) reader.Close();

                DBControl.RemoveDBConection(theDB);
            }

//////////////////////////////////////////////////////////////////////////////////////

            return alarms;
        }

        /// <summary>
        /// Alarm을 SubSystem 별로 구체화하여 Loading Rule을 구현 함.
        /// Alarm ID 부여 Text Rule 부여
        /// </summary>
        /// <returns>알람 정보가 담긴 Alarm List를 반환</returns>
        virtual public Dictionary<long, EcpAlarm> LoadAlarms()
        {
            return null;
        }

        /// <summary>
        /// Data를 data base에 Add함
        /// <see cref="Add(DBConnection theDB)"/>
        /// </summary>
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetVarChar("EQUIPMENTTYPE", this.equipmentType.ToString());

                reader = theDB.ExecuteQuery("select * from TB_SUBSYSTEM_ALARM where EQUIPMENTTYPE =@EQUIPMENTTYPE", psparam);
                if (reader.Read())
                {
                    //Log.log(5, METHOD, "debug", Category.DEBUG, "", "CARRIER", this.carrierID + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("EQUIPMENTTYPE",this.equipmentType.ToString());
                    pss.SetVarChar("ASSEMBLYNAME", this.assemblyName);
                    pss.SetVarChar("CLASSNAME", this.className);

                    theDB.ExecuteUpdate("insert into TB_SUBSYSTEM_ALARM (EQUIPMENTTYPE, ASSEMBLYNAME, CLASSNAME) " +
                        " values (@EQUIPMENTTYPE, @ASSEMBLYNAME, @CLASSNAME)", pss);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "SUBSYSTEM_ALARM", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if(reader != null) reader.Close();
            }

        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".delete";

            try
            {
                base.Delete(theDB);
                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("EQUIPMENTTYPE", this.equipmentType.ToString());

                theDB.ExecuteUpdate("delete from TB_SUBSYSTEM_ALARM where EQUIPMENTTYPE =@EQUIPMENTTYPE", ps);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "SUBSYSTEM_ALARM", "sql error \n " + ex.Message.ToString());
                throw ex;
            }

        }

        #endregion Manage_Persistance_Attribute

    }
}
