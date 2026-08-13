using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Util;

namespace ECP.Db.Entity
{
    [Serializable]
    public class LeadTimeHistData : ECPObject 
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

        private string dbConnectionString = "";

        public LeadTimeHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertLeadTimeHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Leadtime 정보  inert                                                         
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
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

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertLeadTimeHistData()
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
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
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

        public void setLeadTimeHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Fromposition = Helper.PreventNull(list[1]);
            this.Toposition = Helper.PreventNull(list[2]);
            this.Fromdeviceid = Helper.PreventNull(list[3]);
            this.Fromlocationid = Helper.PreventNull(list[4]);
            this.Fromsublocationid = Helper.PreventNull(list[5]);
            this.Todeviceid = Helper.PreventNull(list[6]);
            this.Tolocationid = Helper.PreventNull(list[7]);
            this.Tosublocationid = Helper.PreventNull(list[8]);
            this.Leadtime = Helper.PreventNull(list[9]);            
        }

        public LeadTimeData setLeadTimeData(string[] list)
        {
            LeadTimeData data = new LeadTimeData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Fromposition = Helper.PreventNull(list[1]);
            data.Toposition = Helper.PreventNull(list[2]);
            data.Fromdeviceid = Helper.PreventNull(list[3]);
            data.Fromlocationid = Helper.PreventNull(list[4]);
            data.Fromsublocationid = Helper.PreventNull(list[5]);
            data.Todeviceid = Helper.PreventNull(list[6]);
            data.Tolocationid = Helper.PreventNull(list[7]);
            data.Tosublocationid = Helper.PreventNull(list[8]);
            data.Leadtime = Helper.PreventNull(list[9]);            

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO TB_LEADTIMEHIST     " +
                    "            ( backuptime         " +
                    "             ,plantcode          " +
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
                    "            ( @BACKUPTIME        " +
                    "             ,@PLANTCODE         " +
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
    }
}
