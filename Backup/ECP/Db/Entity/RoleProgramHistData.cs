using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class RoleProgramHistData : ECPObject 
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

        private string dbConnectionString = "";
        public RoleProgramHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertRoleProgramHistData()
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
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
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

        public void insertRoleProgramHistData(DBConnection theDB)
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
                pss.SetVarChar("ROLEID", Helper.PreventNull(this.Roleid));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
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

        public void setRoleProgramHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Roleid = Helper.PreventNull(list[1]);
            this.Pgmid = Helper.PreventNull(list[2]);
            this.Createtime = Helper.PreventNull(list[3]);
            this.Createman = Helper.PreventNull(list[4]);
            this.Disabled = Helper.PreventNull(list[5]);
        }

        public RoleProgramData setRoleProgramData(string[] list)
        {
            RoleProgramData data = new RoleProgramData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Roleid = Helper.PreventNull(list[1]);
            data.Pgmid = Helper.PreventNull(list[2]);
            data.Createtime = Helper.PreventNull(list[3]);
            data.Createman = Helper.PreventNull(list[4]);
            data.Disabled = Helper.PreventNull(list[5]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_roleprogramhist     " +
                    "            (backuptime             " +
                    "            ,plantcode              " +
                    "            ,roleid                 " +
                    "            ,pgmid                  " +
                    "            ,createtime             " +
                    "            ,createman              " +
                    "            ,disabled)              " +
                    "      VALUES                        " +
                    "            (@BACKUPTIME            " +
                    "            ,@PLANTCODE             " +
                    "            ,@ROLEID                " +
                    "            ,@PGMID                 " +
                    "            ,@CREATETIME            " +
                    "            ,@CREATEMAN             " +
                    "            ,@DISABLED)             ";

            return strSql;
        }
    }
}
