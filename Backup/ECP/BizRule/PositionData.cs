using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;
using ECP.Db;

namespace ECP.BizRule
{
    [Serializable]
    public class PositionData : ECPObject 
    {
        private string position = "";

        public string Position
        {
            get { return position; }
            set { position = value; }
        }
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string deviceid = "";

        public string Deviceid
        {
            get { return deviceid; }
            set { deviceid = value; }
        }
        private string locationid = "";

        public string Locationid
        {
            get { return locationid; }
            set { locationid = value; }
        }
        private string sublocationid = "";

        public string Sublocationid
        {
            get { return sublocationid; }
            set { sublocationid = value; }
        }
        private string sapposition = "";

        public string Sapposition
        {
            get { return sapposition; }
            set { sapposition = value; }
        }

        private string sampleflag = "";

        public string Sampleflag
        {
            get { return sampleflag; }
            set { sampleflag = value; }
        }

        public PositionData()
        {
        }

        public List<PositionData> getPositionDataList()
        {
            List<PositionData> list = new List<PositionData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getPositionSQL();
            strSql += " ORDER BY deviceid, locationid, sublocationid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PositionData data = new PositionData();

                    data.setPositionData(reader);

                    list.Add(data);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
            
            return list;
        }

        public List<PositionData> getPositionDataList(string strPosition, string strSapposition,
                                                      string strSampleflag)
        {
            List<PositionData> list = new List<PositionData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            //sample Flag 값이 없어면 "N"
            if (string.IsNullOrEmpty(strSampleflag))
            {
                strSampleflag = "N";
            }

            string strSql = getPositionSQL(strPosition, strSapposition, strSampleflag);

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PositionData data = new PositionData();

                    data.setPositionData(reader);

                    list.Add(data);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
            
            return list;
        }

        public void getPositionData(PositionData pData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            //Rack의 경우 sublocation 정보 없어야 경로 가져옴.
            if (pData.Locationid.Substring(0, 5) == GlobalConstant.SUBSYSTEM_RACK)
            {
                pData.Sublocationid = "";
            }

            string strSql = getPositionSQL(pData.Deviceid, pData.Locationid,
                                           pData.Sublocationid, pData.Position);

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);
                if (reader.Read())
                {
                    setPositionData(reader);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public string getPositionSQL(string strPosition, string strSapposition,
                                   string strSampleflag)
        {
            string strSql = getPositionSQL();
            strSql += "  WHERE new.position    = '" + strPosition    + "'                             " +
                      "    AND new.sapposition = '" + strSapposition + "'                             " +
                      "    AND new.sampleflag  = '" + strSampleflag  + "'                             ";

            return strSql;
        }

        public string getPositionSQL(string strDeviceid, string strLocationid,
                                     string strSublocationid, string strPosition)
        {
            string strSql = getPositionSQL();
            strSql += "  WHERE new.deviceid = '" + strDeviceid + "'                             " +
                      "    AND new.locationid  = '" + strLocationid + "'                        " +
                      "    AND new.sublocationid  = '" + strSublocationid + "'                  " +
                      "    AND new.position  = '" + strPosition + "'                            ";

            return strSql;
        }


        public void setPositionData(SqlDataReader reader)
        {
            this.Position = Helper.PreventNull((string)reader["POSITION"]);
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Deviceid = Helper.PreventNull((string)reader["DEVICEID"]);
            this.Locationid = Helper.PreventNull((string)reader["LOCATIONID"]);
            this.Sublocationid = Helper.PreventNull((string)reader["SUBLOCATIONID"]);
            this.Sapposition = Helper.PreventNull((string)reader["SAPPOSITION"]);
            this.Sampleflag = Helper.PreventNull((string)reader["SAMPLEFLAG"]);
        }

        public string getPositionSQL()
        {
            string strSql = "";
            strSql += "" + 
                    "  SELECT  new.position       position                                             " +
                    "         ,new.deviceid       deviceid                                             " +
                    "         ,new.locationid     locationid                                           " +
                    "         ,new.sublocationid  sublocationid                                        " +
                    "         ,new.sapposition    sapposition                                          " +
                    "         ,new.sampleflag     sampleflag                                           " +
                    "    FROM                                                                          " +
                    "         (SELECT 'FROM'                                            position       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_subsystem                                            " +
                    "                    WHERE subsystemid =                                           " +
                    "                          (SELECT ownerid                                         " +
                    "                             FROM tb_location                                     " +
                    "                            WHERE locationid = a.entityid                         " +
                    "                              AND RIGHT(ownerid,1) NOT IN ('A','R','L')           " +
                    "                          )                                                       " +
                    "                   )                                               deviceid       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_location                                             " +
                    "                    WHERE locationid = a.entityid                                 " +
                    "                      AND RIGHT(ownerid,1) NOT IN ('A','R','L'))   locationid     " +
                    "                 ,a.entityid                                       sublocationid  " +
                    "                 ,a.value                                          sapposition    " +
                    "                ,(SELECT ISNULL(MAX(value),'N')                                   " +
                    "                    FROM tb_udproperty                                            " +
                    "                   WHERE udpropertycd = 'SAMPLE_FLAG'                             " +
                    "                     AND entityid = a.entityid                                    " +
                    "                  )                                                sampleflag     " +
                    "            FROM tb_udproperty a                                                  " +
                    "           WHERE udpropertycd LIKE 'START_SUBLOCATION%'                           " +
                    "          union all                                                               " +
                    "          SELECT  'FROM'                                           position       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_subsystem                                            " +
                    "                    WHERE subsystemid = a.entityid)                deviceid       " +
                    "                 ,a.entityid                                       locationid     " +
                    "                 ,''                                               sublocationid  " +
                    "                 ,a.value                                          sapposition    " +
                    "                ,(SELECT ISNULL(MAX(value),'N')                                   " +
                    "                    FROM tb_udproperty                                            " +
                    "                   WHERE udpropertycd = 'SAMPLE_FLAG'                             " +
                    "                     AND entityid = a.entityid                                    " +
                    "                  )                                                sampleflag     " +
                    "            FROM tb_udproperty a                                                  " +
                    "           WHERE udpropertycd LIKE 'START_LOCATION%'                              " +
                    "          UNION ALL                                                               " +
                    "          SELECT 'TO'                                              position       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_subsystem                                            " +
                    "                    WHERE subsystemid =                                           " +
                    "                          (SELECT ownerid                                         " +
                    "                             FROM tb_location                                     " +
                    "                            WHERE locationid = a.entityid                         " +
                    "                              AND RIGHT(ownerid,1) NOT IN ('A','R','L')           " +
                    "                          )                                                       " +
                    "                   )                                               deviceid       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_location                                             " +
                    "                    WHERE locationid = a.entityid                                 " +
                    "                      AND RIGHT(ownerid,1) NOT IN ('A','R','L'))   locationid     " +
                    "                 ,a.entityid                                       sublocationid  " +
                    "                 ,a.value                                          sapposition    " +
                    "                ,(SELECT ISNULL(MAX(value),'N')                                   " +
                    "                    FROM tb_udproperty                                            " +
                    "                   WHERE udpropertycd = 'SAMPLE_FLAG'                             " +
                    "                     AND entityid = a.entityid                                    " +
                    "                  )                                                sampleflag     " +
                    "            FROM tb_udproperty a                                                  " +
                    "           WHERE udpropertycd LIKE 'END_SUBLOCATION%'                             " +
                    "          UNION ALL                                                               " +
                    "          SELECT  'TO'                                             position       " +
                    "                 ,(SELECT ownerid                                                 " +
                    "                     FROM tb_subsystem                                            " +
                    "                    WHERE subsystemid = a.entityid)                deviceid       " +
                    "                 ,a.entityid                                       locationid     " +
                    "                 ,''                                               sublocationid  " +
                    "                 ,a.value                                          sapposition    " +
                    "                 ,(SELECT ISNULL(MAX(value),'N')                                  " +
                    "                     FROM tb_udproperty                                           " +
                    "                    WHERE udpropertycd = 'SAMPLE_FLAG'                            " +
                    "                      AND entityid = a.entityid                                   " +
                    "                   )                                                sampleflag    " +
                    "             FROM tb_udproperty a                                                 " +
                    "            WHERE udpropertycd LIKE 'END_LOCATION%'                               " +
                    "         ) new                                                                    ";

            return strSql;
        }
    }
}
