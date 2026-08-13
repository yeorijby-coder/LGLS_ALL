using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ErrorHistData : ECPObject 
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
        private string errortype = "";

        public string Errortype
        {
            get { return errortype; }
            set { errortype = value; }
        }
        private string errorcode = "";

        public string Errorcode
        {
            get { return errorcode; }
            set { errorcode = value; }
        }
        private string errortext = "";

        public string Errortext
        {
            get { return errortext; }
            set { errortext = value; }
        }

        private string dbConnectionString = "";
        public ErrorHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //Error Hist List Á¶È¸
        public List<ErrorHistData> getErrorHistDataListBackuptime(string fromdate, string todate)
        {
            List<ErrorHistData> list = new List<ErrorHistData>();
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
                    ErrorHistData ehData = new ErrorHistData();
                    ehData.setErrorHistData(reader);

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

        public List<ErrorHistData> getErrorHistDataListCreatetime(string fromdate, string todate)
        {
            List<ErrorHistData> list = new List<ErrorHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE createtime   BETWEEN '" + fromdate + "'  " +
                      "    AND                                     '" + todate + "'    " +
                      "  ORDER BY createtime                                            ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ErrorHistData ehData = new ErrorHistData();
                    ehData.setErrorHistData(reader);

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

        public void insertErrorHistData()
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
                pss.SetVarChar("ERRORTYPE", Helper.PreventNull(this.Errortype));
                pss.SetVarChar("ERRORCODE", Helper.PreventNull(this.Errorcode));
                pss.SetVarChar("ERRORTEXT", Helper.PreventNull(this.Errortext));

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

        public void insertErrorHistData(DBConnection theDB)
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
                pss.SetVarChar("ERRORTYPE", Helper.PreventNull(this.Errortype));
                pss.SetVarChar("ERRORCODE", Helper.PreventNull(this.Errorcode));
                pss.SetVarChar("ERRORTEXT", Helper.PreventNull(this.Errortext));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setErrorHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Createtime = Helper.PreventNull(list[0]);
            this.Errortype = Helper.PreventNull(list[1]);
            this.Errorcode = Helper.PreventNull(list[2]);
            this.Errortext = Helper.PreventNull(list[3]);
        }

        public void setErrorHistData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Errortype = Helper.PreventNull((string)reader["ERRORTYPE"]);
            this.Errorcode = Helper.PreventNull((string)reader["ERRORCODE"]);
            this.Errortext = Helper.PreventNull((string)reader["ERRORTEXT"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  backuptime                 backuptime            " +
                    "        ,createtime                 createtime            " +
                    "        ,errortype                  errortype             " +
                    "        ,errorcode                  errorcode             " +
                    "        ,ISNULL(errortext    ,'')   errortext             " +
                    "   FROM tb_errorhist                                      ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_errorhist             " +
                    "            ( backuptime              " +
                    "             ,createtime              " +
                    "             ,errortype               " +
                    "             ,errorcode               " +
                    "             ,errortext)              " +
                    "      VALUES                          " +
                    "            ( @BACKUPTIME             " +
                    "             ,@CREATETIME             " +
                    "             ,@ERRORTYPE              " +
                    "             ,@ERRORCODE              " +
                    "             ,@ERRORTEXT)             ";

            return strSql;
        }

        public void insertErrorHistDataBackup(DBConnection theDB)
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
                        this.Errortype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Errorcode + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Errortext;

            return ifValue;
        }
    }
}
