using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class MoveRuleHistData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string ruleid = "";

        public string Ruleid
        {
            get { return ruleid; }
            set { ruleid = value; }
        }
        private string rulename = "";

        public string Rulename
        {
            get { return rulename; }
            set { rulename = value; }
        }
        private string reftable = "";

        public string Reftable
        {
            get { return reftable; }
            set { reftable = value; }
        }

        private string ruleproperty = "";

        public string Ruleproperty
        {
            get { return ruleproperty; }
            set { ruleproperty = value; }
        }
        private string rulevalue = "";

        public string Rulevalue
        {
            get { return rulevalue; }
            set { rulevalue = value; }
        }

        private string ruledescription = "";

        public string Ruledescription
        {
            get { return ruledescription; }
            set { ruledescription = value; }
        }

        private string priority = "";

        public string Priority
        {
            get { return priority; }
            set { priority = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
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
        private string updatetime = "";

        public string Updatetime
        {
            get { return updatetime; }
            set { updatetime = value; }
        }
        private string updateman = "";

        public string Updateman
        {
            get { return updateman; }
            set { updateman = value; }
        }

        private string dbConnectionString = "";
        public MoveRuleHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertMoveRuleHistData()
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

                //TB_MOVERULE 정보  inert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

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

        public void insertMoveRuleHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_MOVERULE 정보  inert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setMoveRuleHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Ruleid = Helper.PreventNull(list[0]);
            this.Rulename = Helper.PreventNull(list[1]);
            this.Reftable = Helper.PreventNull(list[2]);
            this.Ruleproperty = Helper.PreventNull(list[3]);
            this.Rulevalue = Helper.PreventNull(list[4]);
            this.Ruledescription = Helper.PreventNull(list[5]);
            this.Priority = Helper.PreventNull(list[6]);
            this.Disabled = Helper.PreventNull(list[7]);
            this.Createtime = Helper.PreventNull(list[8]);
            this.Createman = Helper.PreventNull(list[9]);
            this.Updatetime = Helper.PreventNull(list[10]);
            this.Updateman = Helper.PreventNull(list[11]); 
        }

        public MoveRuleData setMoveRuleData(string[] list)
        {
            MoveRuleData data = new MoveRuleData();

            data.Ruleid = Helper.PreventNull(list[0]);
            data.Rulename = Helper.PreventNull(list[1]);
            data.Reftable = Helper.PreventNull(list[2]);
            data.Ruleproperty = Helper.PreventNull(list[3]);
            data.Rulevalue = Helper.PreventNull(list[4]);
            data.Ruledescription = Helper.PreventNull(list[5]);
            data.Priority = Helper.PreventNull(list[6]);
            data.Disabled = Helper.PreventNull(list[7]);
            data.Createtime = Helper.PreventNull(list[8]);
            data.Createman = Helper.PreventNull(list[9]);
            data.Updatetime = Helper.PreventNull(list[10]);
            data.Updateman = Helper.PreventNull(list[11]); 

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_moverulehist    " +
                    "            ( backuptime        " +
                    "             ,ruleid            " +
                    "             ,rulename          " +
                    "             ,reftable          " +
                    "             ,ruleproperty      " +
                    "             ,rulevalue         " +
                    "             ,ruledescription   " +
                    "             ,priority          " +
                    "             ,disabled          " +
                    "             ,createtime        " +
                    "             ,createman         " +
                    "             ,updatetime        " +
                    "             ,updateman         " +
                    "            )                   " +
                    "      VALUES                    " +
                    "            ( @BACKUPTIME       " +
                    "             ,@RULEID           " +
                    "             ,@RULENAME         " +
                    "             ,@REFTABLE         " +
                    "             ,@RULEPROPERTY     " +
                    "             ,@RULEVALUE        " +
                    "             ,@RULEDESCRIPTION  " +
                    "             ,@PRIORITY         " +
                    "             ,@DISABLED         " +
                    "             ,@CREATETIME       " +
                    "             ,@CREATEMAN        " +
                    "             ,@UPDATETIME       " +
                    "             ,@UPDATEMAN        " +
                    "            )                   ";

            return strSql;
        }

    }
}
