using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Util;

namespace ECP.Db.Entity
{
    [Serializable]
    public class LeadTimeData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string fromlocationid = "";

        public string Fromlocationid
        {
            get { return fromlocationid; }
            set { fromlocationid = value; }
        }
        private string fromsublocationid = "";

        public string Fromsublocationid
        {
            get { return fromsublocationid; }
            set { fromsublocationid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string tolocationid = "";

        public string Tolocationid
        {
            get { return tolocationid; }
            set { tolocationid = value; }
        }
        private string tosublocationid = "";

        public string Tosublocationid
        {
            get { return tosublocationid; }
            set { tosublocationid = value; }
        }
        private string leadtime = "";

        public string Leadtime
        {
            get { return leadtime; }
            set { leadtime = value; }
        }

        public LeadTimeData()
        {
        }

        public void getLeadTimeData(TransferData data)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string plantcode = GlobalConstant.PLANT_CODE;
            string fromposition = data.Fromposition;
            string fromDevice = data.Fromdeviceid;
            string fromLocation = data.Fromlocationid;
            string fromSubLocation = data.Fromsublocationid;

            string toposition = data.Toposition;
            string toDevice = data.Todeviceid;
            string toLocation = data.Tolocationid;
            string toSubLocation = data.Tosublocationid;

            string substrFromLocation = (data.Fromlocationid).Substring(0, 5);
            string substrToLocation = (data.Tolocationid).Substring(0, 5);

            int iCnt = 0;

            if (substrFromLocation == GlobalConstant.SUBSYSTEM_RACK)
            {
                fromSubLocation = "";
            }

            if (substrToLocation == GlobalConstant.SUBSYSTEM_RACK)
            {
                toSubLocation = "";
            }

            string strSql = getSelectSQL();
            strSql += " WHERE plantcode         = '" + plantcode + "' " +
                      "   AND fromposition      = '" + fromposition + "' " +
                      "   AND toposition        = '" + toposition + "' " +
                      "   AND fromdeviceid      = '" + fromDevice + "' " +
                      "   AND fromlocationid    = '" + fromLocation + "' " +
                      "   AND fromsublocationid = '" + fromSubLocation + "' " +
                      "   AND todeviceid        = '" + toDevice + "' " +
                      "   AND tolocationid      = '" + toLocation + "' " +
                      "   AND tosublocationid   = '" + toSubLocation + "' ";
            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setLeadTimeData(reader);
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

        public int getLeadTimeDataCnt()
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            int iCnt = 0;

            string strSql = getSelectCountSQL();
            strSql += " WHERE plantcode         = '" + this.plantcode + "' " +
                      "   AND fromposition      = '" + this.Fromposition + "' " +
                      "   AND toposition        = '" + this.Toposition + "' " +
                      "   AND fromdeviceid      = '" + this.Fromdeviceid + "' " +
                      "   AND fromlocationid    = '" + this.Fromlocationid + "' " +
                      "   AND fromsublocationid = '" + this.Fromsublocationid + "' " +
                      "   AND todeviceid        = '" + this.Todeviceid + "' " +
                      "   AND tolocationid      = '" + this.Tolocationid + "' " +
                      "   AND tosublocationid   = '" + this.Tosublocationid + "' ";
            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
                }
            }
            catch (SqlException se)
            {
                iCnt = 0;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return iCnt;
        }

        //Batch List 조회
        public List<LeadTimeData> getLeadTimeDataList()
        {
            List<LeadTimeData> list = new List<LeadTimeData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY plantcode, fromposition, toposition  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LeadTimeData lData = new LeadTimeData();
                    lData.setLeadTimeData(reader);

                    list.Add(lData);
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

        public void insertLeadTimeData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  inert                                                         
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("LEADTIME", Helper.PreventNull(this.Leadtime));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertLeadTimeDataBackup(theDB, "Insert");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updateLeadTimeData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  update                                                         
                pss.SetVarChar("LEADTIME", Helper.PreventNull(this.Leadtime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertLeadTimeDataBackup(theDB, "Update");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteLeadTimeData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  delete           
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertLeadTimeDataBackup(theDB, "Delete");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertLeadTimeData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  inert                                                         
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("LEADTIME", Helper.PreventNull(this.Leadtime));

                //DataBackup
                this.insertLeadTimeDataBackup(theDB, "Insert");

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

        public void updateLeadTimeData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  update                                                         
                pss.SetVarChar("LEADTIME", Helper.PreventNull(this.Leadtime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));

                //DataBackup
                this.insertLeadTimeDataBackup(theDB, "Update");

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

        public void deleteLeadTimehData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  update                                                         
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("TOPOSITION", Helper.PreventNull(this.Toposition));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));

                //DataBackup
                this.insertLeadTimeDataBackup(theDB, "Delete");

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

        public void setLeadTimeData(SqlDataReader reader)
        {
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Fromposition = Helper.PreventNull((string)reader["FROMPOSITION"]);
            this.Toposition = Helper.PreventNull((string)reader["TOPOSITION"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Fromlocationid = Helper.PreventNull((string)reader["FROMLOCATIONID"]);
            this.Fromsublocationid = Helper.PreventNull((string)reader["FROMSUBLOCATIONID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Tolocationid = Helper.PreventNull((string)reader["TOLOCATIONID"]);
            this.Tosublocationid = Helper.PreventNull((string)reader["TOSUBLOCATIONID"]);
            this.Leadtime = Helper.PreventNull((string)reader["LEADTIME"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT plantcode            " +
                    "       ,fromposition         " +
                    "       ,toposition           " +
                    "       ,fromdeviceid         " +
                    "       ,fromlocationid       " +
                    "       ,fromsublocationid    " +
                    "       ,todeviceid           " +
                    "       ,tolocationid         " +
                    "       ,tosublocationid      " +
                    "       ,leadtime             " +
                    "   FROM TB_LEADTIME          ";

            return strSql;
        }

        public string getSelectCountSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT COUNT(*) CNT         " +
                    "   FROM TB_LEADTIME          ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO TB_LEADTIME         " +
                    "            ( plantcode          " +
                    "             ,fromposition       " +
                    "             ,toposition         " +
                    "             ,fromdeviceid       " +
                    "             ,fromlocationid     " +
                    "             ,fromsublocationid  " +
                    "             ,todeviceid         " +
                    "             ,tolocationid       " +
                    "             ,tosublocationid    " +
                    "             ,leadtime           " +
                    "            )                    " +
                    "      VALUES                     " +
                    "            ( @PLANTCODE         " +
                    "             ,@FROMPOSITION      " +
                    "             ,@TOPOSITION        " +
                    "             ,@FROMDEVICEID      " +
                    "             ,@FROMLOCATIONID    " +
                    "             ,@FROMSUBLOCATIONID " +
                    "             ,@TODEVICEID        " +
                    "             ,@TOLOCATIONID      " +
                    "             ,@TOSUBLOCATIONID   " +
                    "             ,@LEADTIME          " +
                    "            )                    ";


            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  TB_LEADTIME                           " +
                    "    SET leadtime          = @LEADTIME          " +
                    "  WHERE plantcode         = @PLANTCODE         " +
                    "    AND fromposition      = @FROMPOSITION      " +
                    "    AND toposition        = @TOPOSITION        " +
                    "    AND fromdeviceid      = @FROMDEVICEID      " +
                    "    AND fromlocationid    = @FROMLOCATIONID    " +
                    "    AND fromsublocationid = @FROMSUBLOCATIONID " +
                    "    AND todeviceid        = @TODEVICEID        " +
                    "    AND tolocationid      = @TOLOCATIONID      " +
                    "    AND tosublocationid   = @TOSUBLOCATIONID   ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM TB_LEADTIME                       " +
                    "  WHERE plantcode         = @PLANTCODE         " +
                    "    AND fromposition      = @FROMPOSITION      " +
                    "    AND toposition        = @TOPOSITION        " +
                    "    AND fromdeviceid      = @FROMDEVICEID      " +
                    "    AND fromlocationid    = @FROMLOCATIONID    " +
                    "    AND fromsublocationid = @FROMSUBLOCATIONID " +
                    "    AND todeviceid        = @TODEVICEID        " +
                    "    AND tolocationid      = @TOLOCATIONID      " +
                    "    AND tosublocationid   = @TOSUBLOCATIONID   ";

            return strSql;
        }

        public void insertLeadTimeDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                LeadTimeData data = new LeadTimeData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE plantcode         = '" + this.Plantcode + "' " +
                          "   AND fromposition      = '" + this.Fromposition + "' " +
                          "   AND toposition        = '" + this.Toposition + "' " +
                          "   AND fromdeviceid      = '" + this.Fromdeviceid + "' " +
                          "   AND fromlocationid    = '" + this.Fromlocationid + "' " +
                          "   AND fromsublocationid = '" + this.Fromsublocationid + "' " +
                          "   AND todeviceid        = '" + this.Todeviceid + "' " +
                          "   AND tolocationid      = '" + this.Tolocationid + "' " +
                          "   AND tosublocationid   = '" + this.Tosublocationid + "' ";
                try
                {
                    reader = theDB.ExecuteQuery(strSql);
                }
                catch (SqlException se)
                {
                    if (reader != null) reader.Close();
                    throw se;
                }

                if (reader.Read())
                {
                    data.setLeadTimeData(reader);
                }
                else
                {
                    if (reader != null) reader.Close();
                    return;
                }

                if (reader != null) reader.Close();

                internalIfData.Internaliftype = data.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = data.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }
            else
            {
                internalIfData.Internaliftype = this.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = this.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }

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
            ifValue += this.Plantcode + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromposition + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Toposition + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromdeviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromlocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Fromsublocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Todeviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tolocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tosublocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Leadtime;

            return ifValue;
        }
    }
}
