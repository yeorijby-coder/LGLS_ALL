using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ProgramData : ECPObject 
    {
        private string pgmid = "";

        public string Pgmid
        {
            get { return pgmid; }
            set { pgmid = value; }
        }
        private string pgmname = "";

        public string Pgmname
        {
            get { return pgmname; }
            set { pgmname = value; }
        }
        private string pgmtype = "";

        public string Pgmtype
        {
            get { return pgmtype; }
            set { pgmtype = value; }
        }
        private string ownerid = "";

        public string Ownerid
        {
            get { return ownerid; }
            set { ownerid = value; }
        }
        private string topid = "";

        public string Topid
        {
            get { return topid; }
            set { topid = value; }
        }
        private string createtime = "";

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string createman = "";

        public string Createman
        {
            get { return createman; }
            set { createman = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
        }

        public ProgramData()
        {
        }

        //Program List 조회
        public Dictionary<string, ProgramData> getProgramDataList()
        {
            Dictionary<string, ProgramData> list = new Dictionary<string, ProgramData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY pgmid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ProgramData pData = new ProgramData();
                    pData.setProgramData(reader);

                    list.Add(pData.Pgmid, pData);
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

        public void insertProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  inert
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertProgramDataBackup(theDB, "Insert");
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

        public void updateProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  update
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("PGMID1", Helper.PreventNull(this.Pgmid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertProgramDataBackup(theDB, "Update");
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

        public void deleteProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  delete
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertProgramDataBackup(theDB, "Delete");
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

        public void insertProgramData()
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

                //TB_Program 정보  inert
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                //DataBackup
                this.insertProgramDataBackup(theDB, "Insert");

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

        public void updateProgramData()
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

                //TB_Program 정보  update
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));              
                pss.SetVarChar("PGMID1", Helper.PreventNull(this.Pgmid));
                
                //DataBackup
                this.insertProgramDataBackup(theDB, "Update");

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

        public void deleteProgramData()
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

                //TB_Program 정보  delete
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));

                //DataBackup
                this.insertProgramDataBackup(theDB, "Delete");

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

        public void setProgramData(SqlDataReader reader)
        {
            this.Pgmid = Helper.PreventNull((string)reader["PGMID"]);
            this.Pgmname = Helper.PreventNull((string)reader["PGMNAME"]);
            this.Pgmtype = Helper.PreventNull((string)reader["PGMTYPE"]);
            this.Ownerid = Helper.PreventNull((string)reader["OWNERID"]);
            this.Topid = Helper.PreventNull((string)reader["TOPID"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);    
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT pgmid                    pgmid      " +
                    "       ,ISNULL(pgmname    ,'')   pgmname    " +
                    "       ,ISNULL(pgmtype    ,'')   pgmtype    " +
                    "       ,ISNULL(ownerid    ,'')   ownerid    " +
                    "       ,ISNULL(topid      ,'')   topid      " +
                    "       ,ISNULL(createtime ,'')   createtime " +
                    "       ,ISNULL(createman  ,'')   createman  " +
                    "       ,ISNULL(disabled   ,'')   disabled   " +
                    "   FROM tb_program                          "; 

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_program   " +
                    "            ( pgmid       " +
                    "             ,pgmname     " +
                    "             ,pgmtype     " +
                    "             ,ownerid     " +
                    "             ,topid       " +
                    "             ,createtime  " +
                    "             ,createman   " +
                    "             ,disabled    " +
                    "            )             " +
                    "      VALUES              " +
                    "            ( @PGMID      " +
                    "             ,@PGMNAME    " +
                    "             ,@PGMTYPE    " +
                    "             ,@OWNERID    " +
                    "             ,@TOPID      " +
                    "             ,@CREATETIME " +
                    "             ,@CREATEMAN  " +
                    "             ,@DISABLED   " +
                    "            )             ";
            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_program                 " +
                    "    SET  pgmid       = @PGMID       " +
                    "        ,pgmname     = @PGMNAME     " +
                    "        ,pgmtype     = @PGMTYPE     " +
                    "        ,ownerid     = @OWNERID     " +
                    "        ,topid       = @TOPID       " +
                    "        ,createtime  = @CREATETIME  " +
                    "        ,createman   = @CREATEMAN   " +
                    "        ,disabled    = @DISABLED    " +
                    "  WHERE pgmid        = @PGMID1      "; 

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_program  " +
                    "  WHERE pgmid = @PGMID   "; 

            return strSql;
        }

        public void insertProgramDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                ProgramData data = new ProgramData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE pgmid  = '" + this.Pgmid + "' ";

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
                    data.setProgramData(reader);
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
            ifValue += this.Pgmid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pgmname + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pgmtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Ownerid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Topid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled;      

            return ifValue;
        }
    }
}
