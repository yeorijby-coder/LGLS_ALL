using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class RoleProgramData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }

        private string roleid = "";

        public string Roleid
        {
            get { return roleid; }
            set { roleid = value; }
        }
        private string pgmid = "";

        public string Pgmid
        {
            get { return pgmid; }
            set { pgmid = value; }
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

        public RoleProgramData()
        {
        }

        //User List 조회
        public List<RoleProgramData> getRoleProgramDataList()
        {
            List<RoleProgramData> list = new List<RoleProgramData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY plantcode, roleid, pgmid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    RoleProgramData uData = new RoleProgramData();
                    uData.setRoleProgramData(reader);

                    list.Add(uData);
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

        //Hirack Meterial/Batch List 조회
        public List<RoleProgramData> getRoleProgramDataRoleProgramList(string roleid, string pgmid)
        {
            List<RoleProgramData> list = new List<RoleProgramData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode  = '"    + GlobalConstant.PLANT_CODE + "'  " +
                      "    AND roleid     LIKE '" + roleid                    + "'  " +
                      "    AND pgmid      LIKE '" + pgmid                     + "'  " +
                      "  ORDER BY plantcode, roleid, pgmid                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    RoleProgramData data = new RoleProgramData();
                    data.setRoleProgramData(reader);

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

        public void insertRoleProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_UserRole 정보  insert
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleProgramDataBackup(theDB, "Insert");
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

        public void updateRoleProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_UserRole 정보  update
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PGMID1", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("ROLEID1", Helper.PreventNull(this.Roleid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleProgramDataBackup(theDB, "Update");
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

        public void deleteRoleProgramData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_UserRole 정보  delete
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleProgramDataBackup(theDB, "Delete");
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

        public void insertRoleProgramData()
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

                //TB_UserRole 정보  insert
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                //DataBackup
                this.insertRoleProgramDataBackup(theDB, "Insert");

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

        public void updateRoleProgramData()
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

                //TB_UserRole 정보  update
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PGMID1", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("ROLEID1", Helper.PreventNull(this.Roleid));
                
                //DataBackup
                this.insertRoleProgramDataBackup(theDB, "Update");

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

        public void deleteRoleProgramData()
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

                //TB_UserRole 정보  delete
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                
                //DataBackup
                this.insertRoleProgramDataBackup(theDB, "Delete");

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

        public void setRoleProgramData(SqlDataReader reader)
        {
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Roleid = Helper.PreventNull((string)reader["ROLEID"]);
            this.Pgmid = Helper.PreventNull((string)reader["PGMID"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT plantcode                         plantcode   " +
                    "       ,roleid                            roleid      " +
                    "       ,pgmid                             pgmid       " +
                    "       ,ISNULL(createtime          ,'')   createtime  " +
                    "       ,ISNULL(createman           ,'')   createman   " +
                    "       ,ISNULL(disabled            ,'')   disabled    " +
                    "   FROM tb_roleprogram                                ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_roleprogram         " +
                    "            (plantcode              " +
                    "            ,roleid                 " +
                    "            ,pgmid                  " +
                    "            ,createtime             " +
                    "            ,createman              " +
                    "            ,disabled)              " +
                    "      VALUES                        " +
                    "            (@PLANTCODE             " +
                    "            ,@ROLEID                " +
                    "            ,@PGMID                 " +
                    "            ,@CREATETIME            " +
                    "            ,@CREATEMAN             " +
                    "            ,@DISABLED)             ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_roleprogram                                " +
                    "    SET  plantcode            = @PLANTCODE             " +
                    "        ,roleid               = @ROLEID                " +
                    "        ,pgmid                = @PGMID                 " +
                    "        ,createtime           = @CREATETIME            " +
                    "        ,createman            = @CREATEMAN             " +
                    "        ,disabled             = @DISABLED              " +
                    "  WHERE plantcode             = @PLANTCODE1            " +
                    "    AND roleid                = @ROLEID1               " +
                    "    AND pgmid                 = @PGMID1                ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_roleprogram                     " +
                    "  WHERE plantcode             = @PLANTCODE      " +
                    "    AND roleid                = @ROLEID         " +
                    "    AND pgmid                 = @PGMID          ";
                    
            return strSql;
        }

        public void insertRoleProgramDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                RoleProgramData data = new RoleProgramData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += "  WHERE plantcode = '" + this.Plantcode + "' " +
                          "    AND roleid    = '" + this.Roleid + "' " +
                          "    AND pgmid     = '" + this.Pgmid + "' ";

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
                    data.setRoleProgramData(reader);
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
                        this.Roleid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pgmid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled;                                            

            return ifValue;
        }
    }
}
