using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class UserRoleHistData :ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }

        private string userid = "";

        public string Userid
        {
            get { return userid; }
            set { userid = value; }
        }
        private string roleid = "";

        public string Roleid
        {
            get { return roleid; }
            set { roleid = value; }
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
        public UserRoleHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertUserRoleHistData()
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

                //TB_UserRoleHist 정보  insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
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

        public void insertUserRoleHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_UserRoleHist 정보  insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("USERID", Helper.PreventNull(this.Userid));
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
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

        public void setUserRoleHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Userid = Helper.PreventNull(list[1]);
            this.Roleid = Helper.PreventNull(list[2]);
            this.Createtime = Helper.PreventNull(list[3]);
            this.Createman = Helper.PreventNull(list[4]);
            this.Disabled = Helper.PreventNull(list[5]);
        }

        public UserRoleData setUserRoleData(string[] list)
        {
            UserRoleData data = new UserRoleData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Userid = Helper.PreventNull(list[1]);
            data.Roleid = Helper.PreventNull(list[2]);
            data.Createtime = Helper.PreventNull(list[3]);
            data.Createman = Helper.PreventNull(list[4]);
            data.Disabled = Helper.PreventNull(list[5]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_userrolehist        " +
                    "            (backuptime             " +
                    "            ,plantcode              " +
                    "            ,userid                 " +
                    "            ,roleid                 " +
                    "            ,createtime             " +
                    "            ,createman              " +
                    "            ,disabled)              " +
                    "      VALUES                        " +
                    "            (@BACKUPTIME            " +
                    "            ,@PLANTCODE             " +
                    "            ,@USERID                " +
                    "            ,@ROLEID                " +
                    "            ,@CREATETIME            " +
                    "            ,@CREATEMAN             " +
                    "            ,@DISABLED)             ";

            return strSql;
        }
    }
}
