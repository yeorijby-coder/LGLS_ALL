using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class LogData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }

        private string createtime = "";

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string logtype = "";

        public string Logtype
        {
            get { return logtype; }
            set { logtype = value; }
        }
        private string logpath = "";

        public string Logpath
        {
            get { return logpath; }
            set { logpath = value; }
        }
        private string logtitle = "";

        public string Logtitle
        {
            get { return logtitle; }
            set { logtitle = value; }
        }
        private string logcontents = "";

        public string Logcontents
        {
            get { return logcontents; }
            set { logcontents = value; }
        }
        private string commandid = "";

        public string Commandid
        {
            get { return commandid; }
            set { commandid = value; }
        }
        private string commandseq = "";

        public string Commandseq
        {
            get { return commandseq; }
            set { commandseq = value; }
        }
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string batchno = "";

        public string Batchno
        {
            get { return batchno; }
            set { batchno = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string qty = "";

        public string Qty
        {
            get { return qty; }
            set { qty = value; }
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

        private string dbConnectionString = "";
        public LogData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //Log List 조회
        public List<LogData> getLogDataList(string fromdate, string todate)
        {
            List<LogData> list = new List<LogData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE SUBSTRING(backuptime,1,8)   BETWEEN '" + fromdate + "'  " +
                      "    AND                                     '" + todate + "'    " +
                      "  ORDER BY backuptime                                            ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LogData ehData = new LogData();
                    ehData.setLogData(reader);

                    list.Add(ehData);
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

        //Log List 조회
        public List<LogData> getLogDataListBackuptime(string fromdate, string todate)
        {
            List<LogData> list = new List<LogData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE SUBSTRING(backuptime,1,8)   BETWEEN '" + fromdate + "'  " +
                      "    AND                                     '" + todate + "'    " +
                      "  ORDER BY backuptime                                            ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LogData data = new LogData();
                    data.setLogData(reader);

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

        //Log List 조회
        public List<LogData> getLogDataListCreatetime(string fromdate, string todate)
        {
            List<LogData> list = new List<LogData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE SUBSTRING(createtime,1,8)   BETWEEN '" + fromdate + "'  " +
                      "    AND                                     '" + todate   + "'  " +
                      "  ORDER BY createtime                                           ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LogData data = new LogData();
                    data.setLogData(reader);

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

        //Log List 조회
        public List<LogData> getLogDataList(string plantcode
                                            , string logtype
                                            , string logpath
                                            , string logtitle
                                            , string logcontents
                                            , string commandid
                                            , string commandseq
                                            , string palletid
                                            , string batchno
                                            , string materialno
                                            , string qty
                                            , string deviceid
                                            , string locationid
                                            , string sublocationid)
        {
            List<LogData> list = new List<LogData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql +=   "  WHERE plantcode     LIKE '" + plantcode     + "' " +
                        "    AND logtype       LIKE '" + logtype       + "' " +
                        "    AND logpath       LIKE '" + logpath       + "' " +
                        "    AND logtitle      LIKE '" + logtitle      + "' " +
                        "    AND logcontents   LIKE '" + logcontents   + "' " +
                        "    AND commandid     LIKE '" + commandid     + "' " +
                        "    AND commandseq    LIKE '" + commandseq    + "' " +
                        "    AND palletid      LIKE '" + palletid      + "' " +
                        "    AND batchno       LIKE '" + batchno       + "' " +
                        "    AND materialno    LIKE '" + materialno    + "' " +
                        "    AND qty           LIKE '" + qty           + "' " +
                        "    AND deviceid      LIKE '" + deviceid      + "' " +
                        "    AND locationid    LIKE '" + locationid    + "' " +
                        "    AND sublocationid LIKE '" + sublocationid + "' " +
                        "  ORDER BY createtime                                ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LogData data = new LogData();
                    data.setLogData(reader);

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

        public void insertLogData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("LOGTYPE", Helper.PreventNull(this.Logtype));
                pss.SetVarChar("LOGPATH", Helper.PreventNull(this.Logpath));
                pss.SetVarChar("LOGTITLE", Helper.PreventNull(this.Logtitle));
                pss.SetVarChar("LOGCONTENTS", Helper.PreventNull(this.Logcontents));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid)); 

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
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

        public void insertLogData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("LOGTYPE", Helper.PreventNull(this.Logtype));
                pss.SetVarChar("LOGPATH", Helper.PreventNull(this.Logpath));
                pss.SetVarChar("LOGTITLE", Helper.PreventNull(this.Logtitle));
                pss.SetVarChar("LOGCONTENTS", Helper.PreventNull(this.Logcontents));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setLogData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Createtime = Helper.PreventNull(list[0]);
            this.Plantcode = Helper.PreventNull(list[1]);
            this.Logtype = Helper.PreventNull(list[2]);
            this.Logpath = Helper.PreventNull(list[3]);
            this.Logtitle = Helper.PreventNull(list[4]);
            this.Logcontents = Helper.PreventNull(list[5]);
            this.Commandid = Helper.PreventNull(list[6]);
            this.Commandseq = Helper.PreventNull(list[7]);
            this.Palletid = Helper.PreventNull(list[8]);
            this.Batchno = Helper.PreventNull(list[9]);
            this.Materialno = Helper.PreventNull(list[10]);
            this.Qty = Helper.PreventNull(list[11]);
            this.Deviceid = Helper.PreventNull(list[12]);
            this.Locationid = Helper.PreventNull(list[13]);
            this.Sublocationid = Helper.PreventNull(list[14]);
        }

        public void setLogData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Logtype = Helper.PreventNull((string)reader["LOGTYPE"]);
            this.Logpath = Helper.PreventNull((string)reader["LOGPATH"]);
            this.Logtitle = Helper.PreventNull((string)reader["LOGTITLE"]);
            this.Logcontents = Helper.PreventNull((string)reader["LOGCONTENTS"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandseq = Helper.PreventNull((string)reader["COMMANDSEQ"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Deviceid = Helper.PreventNull((string)reader["DEVICEID"]);
            this.Locationid = Helper.PreventNull((string)reader["LOCATIONID"]);
            this.Sublocationid = Helper.PreventNull((string)reader["SUBLOCATIONID"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  backuptime                  backuptime     " +
                    "        ,ISNULL(createtime   ,'')    createtime     " +
                    "        ,ISNULL(plantcode    ,'')    plantcode      " +
                    "        ,ISNULL(logtype      ,'')    logtype        " +
                    "        ,ISNULL(logpath      ,'')    logpath        " +
                    "        ,ISNULL(logtitle     ,'')    logtitle       " +
                    "        ,ISNULL(logcontents  ,'')    logcontents    " +
                    "        ,ISNULL(commandid    ,'')    commandid      " +
                    "        ,ISNULL(commandseq   ,'')    commandseq     " +
                    "        ,ISNULL(palletid     ,'')    palletid       " +
                    "        ,ISNULL(batchno      ,'')    batchno        " +
                    "        ,ISNULL(materialno   ,'')    materialno     " +
                    "        ,ISNULL(qty          ,'')    qty            " +
                    "        ,ISNULL(deviceid     ,'')    deviceid       " +
                    "        ,ISNULL(locationid   ,'')    locationid     " +
                    "        ,ISNULL(sublocationid,'')    sublocationid  " +
                    "   FROM tb_log                                      ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_log           " +
                    "            ( backuptime      " +
                    "             ,createtime      " +
                    "             ,plantcode       " +
                    "             ,logtype         " +
                    "             ,logpath         " +
                    "             ,logtitle        " +
                    "             ,logcontents     " +
                    "             ,commandid       " +
                    "             ,commandseq      " +
                    "             ,palletid        " +
                    "             ,batchno         " +
                    "             ,materialno      " +
                    "             ,qty             " +
                    "             ,deviceid        " +
                    "             ,locationid      " +
                    "             ,sublocationid   " +
                    "            )                 " +
                    "      VALUES                  " +
                    "            ( @BACKUPTIME     " +
                    "             ,@CREATETIME     " +
                    "             ,@PLANTCODE      " +
                    "             ,@LOGTYPE        " +
                    "             ,@LOGPATH        " +
                    "             ,@LOGTITLE       " +
                    "             ,@LOGCONTENTS    " +
                    "             ,@COMMANDID      " +
                    "             ,@COMMANDSEQ     " +
                    "             ,@PALLETID       " +
                    "             ,@BATCHNO        " +
                    "             ,@MATERIALNO     " +
                    "             ,@QTY            " +
                    "             ,@DEVICEID       " +
                    "             ,@LOCATIONID     " +
                    "             ,@SUBLOCATIONID  " +
                    "           )                  ";

            return strSql;
        }

        public void insertLogDataBackup(DBConnection theDB)
        {
            InternalIfData internalIfData = new InternalIfData();

            internalIfData.Internaliftype = this.GetType().Name;
            internalIfData.Transactiontype = "Insert";
            internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            internalIfData.Ifvalue = this.makeInternalIfValue();
            internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;

            try
            {
                internalIfData.insertInternalIfData(theDB);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public string makeInternalIfValue()
        {
            string ifValue = "";
            ifValue += this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Plantcode + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Logtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Logpath + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Logtitle + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Logcontents + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandseq + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Deviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Locationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Sublocationid;

            return ifValue;
        }
    }
}
