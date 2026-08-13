using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class InternalIfData : ECPObject 
    {
        private string internaliftype = "";

        public string Internaliftype
        {
            get { return internaliftype; }
            set { internaliftype = value; }
        }

        private string transactiontype = "";

        public string Transactiontype
        {
            get { return transactiontype; }
            set { transactiontype = value; }
        }

        private string createtime = "";

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string ifvalue = "";

        public string Ifvalue
        {
            get { return ifvalue; }
            set { ifvalue = value; }
        }
        private string transstarttime = "";

        public string Transstarttime
        {
            get { return transstarttime; }
            set { transstarttime = value; }
        }
        private string transcompletetime = "";

        public string Transcompletetime
        {
            get { return transcompletetime; }
            set { transcompletetime = value; }
        }
        private string ifcnt = "";

        public string Ifcnt
        {
            get { return ifcnt; }
            set { ifcnt = value; }
        }

        private string dbConnectionString = "";

        public InternalIfData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public List<InternalIfData> getGlobalInternalIfDataList()
        {
            List<InternalIfData> list = new List<InternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE transcompletetime = '" + GlobalConstant.SET_MAXTIME + "'    " +
                      "    ORDER BY createtime                                             ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    InternalIfData internalIfData = new InternalIfData();

                    internalIfData.setInternalIfData(reader);

                    list.Add(internalIfData);
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

        public List<InternalIfData> getLocalInternalIfDataList()
        {
            List<InternalIfData> list = new List<InternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE transcompletetime = '" + GlobalConstant.SET_MAXTIME + "'    " +
                      "    ORDER BY createtime                                             ";
            try

            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    InternalIfData internalIfData = new InternalIfData();

                    internalIfData.setInternalIfData(reader);

                    list.Add(internalIfData);
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

        public void insertInternalIfData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = getInsertSQL();

            try
            {
                theDB = DBControl.GetConnection();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("INTERNALIFTYPE", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void insertInternalIfData(DBConnection theDB)
        {
            string strSql = "";
            strSql = getInsertSQL();

            try
            {
                PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("INTERNALIFTYPE", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        //TB_INTERNALIF에 Glboal로 등록한 정보를 I/F 완료 처리함.
        public void updateLocalInternalIfData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            strSql = getUpdateSQL();

            try
            {
                theDB = DBControl.GetConnection();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("INTERNALIFTYPE", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));
                pss.SetVarChar("INTERNALIFTYPE1", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE1", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME1", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE1", Helper.PreventNull(this.Ifvalue));

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException se)
            {
                if(theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        //Global TB_INTERNALIF에 Local로 등록한 정보를 I/F 완료 처리함.
        public void updateGlobalInternalIfData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            strSql = getUpdateSQL();

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("INTERNALIFTYPE", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));
                pss.SetVarChar("INTERNALIFTYPE1", Helper.PreventNull(this.Internaliftype));
                pss.SetVarChar("TRANSACTIONTYPE1", Helper.PreventNull(this.Transactiontype));
                pss.SetVarChar("CREATETIME1", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE1", Helper.PreventNull(this.Ifvalue));


                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        //TB_INTERNALIF에 Glboal로 등록한 정보를 삭제 처리함.
        public void deleteLocalInternalIfData()
        {
            DBConnection theDB = null;
            string strSql = "";
            strSql = getDeleteSQL();

            try
            {
                theDB = DBControl.GetConnection();
                theDB.ExecuteUpdate(strSql);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        //Global TB_INTERNALIF에 Local로 등록한 정보를 삭제 처리함.
        public void deleteGlobalInternalIfData()
        {
            DBConnection theDB = null;
            string strSql = "";
            strSql = getDeleteSQL();

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                theDB.ExecuteUpdate(strSql);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void setInternalIfData(SqlDataReader reader)
        {
            this.Internaliftype = Helper.PreventNull((string)reader["INTERNALIFTYPE"]);
            this.Transactiontype = Helper.PreventNull((string)reader["TRANSACTIONTYPE"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Ifvalue = Helper.PreventNull((string)reader["IFVALUE"]);
            this.Transstarttime = Helper.PreventNull((string)reader["TRANSSTARTTIME"]);
            this.Transcompletetime = Helper.PreventNull((string)reader["TRANSCOMPLETETIME"]);
            this.Ifcnt = Helper.PreventNull((string)reader["IFCNT"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  internaliftype                                     " +
                    "        ,transactiontype                                    " +
                    "        ,createtime                                         " +
                    "        ,ifvalue                                            " +
                    "        ,ISNULL(transstarttime, '')     transstarttime      " +
                    "        ,ISNULL(transcompletetime, '')  transcompletetime   " +
                    "        ,ISNULL(ifcnt, '')          ifcnt                   " +
                    "   FROM tb_internalif                                       ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql =" " +
                    "INSERT INTO tb_internalif      " +
                    "           (internaliftype     " +
                    "           ,transactiontype    " +
                    "           ,createtime         " +
                    "           ,ifvalue            " +
                    "           ,transstarttime     " +
                    "           ,transcompletetime  " +
                    "           ,ifcnt)             " +
                    "     VALUES                    " +
                    "           (@INTERNALIFTYPE    " +
                    "           ,@TRANSACTIONTYPE   " +
                    "           ,@CREATETIME        " +
                    "           ,@IFVALUE           " +
                    "           ,@TRANSSTARTTIME    " +
                    "           ,@TRANSCOMPLETETIME " +
                    "           ,@IFCNT)            ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";

            strSql = " " +
                    " UPDATE tb_internalif                            " +
                    "    SET  internaliftype     = @INTERNALIFTYPE    " +
                    "        ,transactiontype    = @TRANSACTIONTYPE   " +
                    "        ,createtime         = @CREATETIME        " +
                    "        ,ifvalue            = @IFVALUE           " +
                    "        ,transstarttime     = @TRANSSTARTTIME    " +
                    "        ,transcompletetime  = @TRANSCOMPLETETIME " +
                    "        ,ifcnt              = @IFCNT             " +
                    "  WHERE internaliftype    = @INTERNALIFTYPE1     " +
                    "    AND transactiontype   = @TRANSACTIONTYPE1    " +
                    "    AND createtime        = @CREATETIME1         " +
                    "    AND ifvalue           = @IFVALUE1            " +
                    "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "'  ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";

            strSql = " " +
                    " DELETE tb_internalif                            " +
                    "  WHERE TRANSCOMPLETETIME <> '" + GlobalConstant.SET_MAXTIME + "' ";

            return strSql;
        }
    }
}
