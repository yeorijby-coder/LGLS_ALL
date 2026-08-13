using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class RoleHistData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }

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

        private string dbConnectionString = "";
        public RoleHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertRoleHistData()
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

                //TB_Role 정보  insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled)); 

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

        public void insertRoleHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Role 정보  insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("ROLENAME", Helper.PreventNull(this.Rolename));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setRoleHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Roleid = Helper.PreventNull(list[0]);
            this.Rolename = Helper.PreventNull(list[1]);
            this.Createtime = Helper.PreventNull(list[2]);
            this.Createman = Helper.PreventNull(list[3]);
            this.Disabled = Helper.PreventNull(list[4]);
        }

        public RoleData setRoleData(string[] list)
        {
            RoleData data = new RoleData();

            data.Roleid = Helper.PreventNull(list[0]);
            data.Rolename = Helper.PreventNull(list[1]);
            data.Createtime = Helper.PreventNull(list[2]);
            data.Createman = Helper.PreventNull(list[3]);
            data.Disabled = Helper.PreventNull(list[4]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_rolehist  " +
                    "            ( backuptime  " +
                    "             ,roleid      " +
                    "             ,rolename    " +
                    "             ,createtime  " +
                    "             ,createman   " +
                    "             ,disabled    " +
                    "            )             " +
                    "      VALUES              " +
                    "            (@BACKUPTIME  " +
                    "             ,@ROLEID     " +
                    "             ,@ROLENAME   " +
                    "             ,@CREATETIME " +
                    "             ,@CREATEMAN  " +
                    "             ,@DISABLED   " +
                    "            )             ";

            return strSql;
        }
    }
}
