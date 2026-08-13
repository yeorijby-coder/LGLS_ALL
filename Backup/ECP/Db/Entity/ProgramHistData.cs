using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ProgramHistData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
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

        private string dbConnectionString = "";
        public ProgramHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertProgramHistData()
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

                //TB_Program 정보  inert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime)); 
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
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

        public void insertProgramHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  inert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PGMID", Helper.PreventNull(this.Pgmid));
                pss.SetVarChar("PGMNAME", Helper.PreventNull(this.Pgmname));
                pss.SetVarChar("PGMTYPE", Helper.PreventNull(this.Pgmtype));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("TOPID", Helper.PreventNull(this.Topid));
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

        public void setProgramHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Pgmid = Helper.PreventNull(list[0]);
            this.Pgmname = Helper.PreventNull(list[1]);
            this.Pgmtype = Helper.PreventNull(list[2]);
            this.Ownerid = Helper.PreventNull(list[3]);
            this.Topid = Helper.PreventNull(list[4]);
            this.Createtime = Helper.PreventNull(list[5]);
            this.Createman = Helper.PreventNull(list[6]);
            this.Disabled = Helper.PreventNull(list[7]);
        }

        public ProgramData setProgramData(string[] list)
        {
            ProgramData data = new ProgramData();

            data.Pgmid = Helper.PreventNull(list[0]);
            data.Pgmname = Helper.PreventNull(list[1]);
            data.Pgmtype = Helper.PreventNull(list[2]);
            data.Ownerid = Helper.PreventNull(list[3]);
            data.Topid = Helper.PreventNull(list[4]);
            data.Createtime = Helper.PreventNull(list[5]);
            data.Createman = Helper.PreventNull(list[6]);
            data.Disabled = Helper.PreventNull(list[7]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_programhist   " +
                    "            ( backuptime      " +
                    "             ,pgmid           " +
                    "             ,pgmname         " +
                    "             ,pgmtype         " +
                    "             ,ownerid         " +
                    "             ,topid           " +
                    "             ,createtime      " +
                    "             ,createman       " +
                    "             ,disabled        " +
                    "            )                 " +
                    "      VALUES                  " +
                    "            ( @BACKUPTIME     " +
                    "             ,@PGMID          " +
                    "             ,@PGMNAME        " +
                    "             ,@PGMTYPE        " +
                    "             ,@OWNERID        " +
                    "             ,@TOPID          " +
                    "             ,@CREATETIME     " +
                    "             ,@CREATEMAN      " +
                    "             ,@DISABLED       " +
                    "            )                 ";

            return strSql;
        }
    }
}
