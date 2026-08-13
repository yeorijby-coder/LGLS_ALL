using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class RoleData : ECPObject 
    {
        private string roleid = "";

        public string Roleid
        {
            get { return roleid; }
            set { roleid = value; }
        }
        private string rolename = "";

        public string Rolename
        {
            get { return rolename; }
            set { rolename = value; }
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

        public RoleData()
        {
        }

        //User List 조회
        public Dictionary<string, RoleData> getRoleDataList()
        {
            Dictionary<string, RoleData> list = new Dictionary<string, RoleData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY roleid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    RoleData rData = new RoleData();
                    rData.setRoleData(reader);

                    list.Add(rData.Roleid, rData);
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

        public void insertRoleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Role 정보  insert
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleDataBackup(theDB, "Insert");
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

        public void updateRoleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Role 정보  update
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("ROLEID1", Helper.PreventNull(this.Roleid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleDataBackup(theDB, "Update");
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

        public void deleteRoleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_User 정보  update
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertRoleDataBackup(theDB, "Delete");
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

        public void insertRoleData()
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

                //TB_Role 정보  insert
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled)); 

                //DataBackup
                this.insertRoleDataBackup(theDB, "Insert");

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

        public void updateRoleData()
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

                //TB_Role 정보  update
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("ROLEID1", Helper.PreventNull(this.Roleid));
                
                //DataBackup
                this.insertRoleDataBackup(theDB, "Update");

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

        public void deleteRoleData()
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

                //TB_User 정보  update
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));

                //DataBackup
                this.insertRoleDataBackup(theDB, "Delete");

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

        public void setRoleData(SqlDataReader reader)
        {
            this.Roleid = Helper.PreventNull((string)reader["ROLEID"]);
            this.Rolename = Helper.PreventNull((string)reader["ROLENAME"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);    
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT roleid                   roleid     " +
                    "       ,ISNULL(rolename   ,'')   rolename   " +
                    "       ,ISNULL(createtime ,'')   createtime " +
                    "       ,ISNULL(createman  ,'')   createman  " +
                    "       ,ISNULL(disabled   ,'')   disabled   " +
                    "   FROM tb_role                             ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_role     " +
                    "            (roleid      " +
                    "            ,rolename    " +
                    "            ,createtime  " +
                    "            ,createman   " +
                    "            ,disabled    " +
                    "            )            " +
                    "      VALUES             " +
                    "            (@ROLEID     " +
                    "            ,@ROLENAME   " +
                    "            ,@CREATETIME " +
                    "            ,@CREATEMAN  " +
                    "            ,@DISABLED   " +
                    "            )            ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_role                   " +
                    "    SET  roleid      = @ROLEID     " +
                    "        ,rolename    = @ROLENAME   " +
                    "        ,createtime  = @CREATETIME " +
                    "        ,createman   = @CREATEMAN  " +
                    "        ,disabled    = @DISABLED   " +
                    "  WHERE roleid       = @ROLEID1    "; 

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_role     " +
                    "  WHERE roleid = @ROLEID "; 

            return strSql;
        }

        public void insertRoleDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                RoleData data = new RoleData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE roleid  = '" + this.Roleid + "' ";

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
                    data.setRoleData(reader);
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
            ifValue += this.Roleid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Rolename + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled;

            return ifValue;
        }
    }
}
