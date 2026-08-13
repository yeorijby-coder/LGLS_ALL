using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class CodeMasterHistData : ECPObject
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string systemcd = "";

        public string Systemcd
        {
            get { return systemcd; }
            set { systemcd = value; }
        }
        private string codeclass = "";

        public string Codeclass
        {
            get { return codeclass; }
            set { codeclass = value; }
        }
        private string code = "";

        public string Code
        {
            get { return code; }
            set { code = value; }
        }
        private string value = "";

        public string Value
        {
            get { return this.value; }
            set { this.value = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
        }
        private string remark = "";

        public string Remark
        {
            get { return remark; }
            set { remark = value; }
        }
        private string attribute1 = "";

        public string Attribute1
        {
            get { return attribute1; }
            set { attribute1 = value; }
        }
        private string attribute2 = "";

        public string Attribute2
        {
            get { return attribute2; }
            set { attribute2 = value; }
        }
        private string attribute3 = "";

        public string Attribute3
        {
            get { return attribute3; }
            set { attribute3 = value; }
        }
        private string attribute4 = "";

        public string Attribute4
        {
            get { return attribute4; }
            set { attribute4 = value; }
        }
        private string attribute5 = "";

        public string Attribute5
        {
            get { return attribute5; }
            set { attribute5 = value; }
        }

        private string dbConnectionString = "";

        public CodeMasterHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertCodeMasterHistData()
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

                //TB_CodeMasterHist 정보  inert                                    
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("SYSTEMCD", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5)); 

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

        public void insertCodeMasterHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_CodeMasterHist 정보  inert                                    
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("SYSTEMCD", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5));

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setCodeMasterHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Systemcd = Helper.PreventNull(list[0]);
            this.Codeclass = Helper.PreventNull(list[1]);
            this.Code = Helper.PreventNull(list[2]);
            this.Value = Helper.PreventNull(list[3]);
            this.Disabled = Helper.PreventNull(list[4]);
            this.Remark = Helper.PreventNull(list[5]);
            this.Attribute1 = Helper.PreventNull(list[6]);
            this.Attribute2 = Helper.PreventNull(list[7]);
            this.Attribute3 = Helper.PreventNull(list[8]);
            this.Attribute4 = Helper.PreventNull(list[9]);
            this.Attribute5 = Helper.PreventNull(list[10]);
        }

        public CodeMasterData setCodeMasterData(string[] list)
        {
            CodeMasterData data = new CodeMasterData();

            data.Systemcd = Helper.PreventNull(list[0]);
            data.Codeclass = Helper.PreventNull(list[1]);
            data.Code = Helper.PreventNull(list[2]);
            data.Value = Helper.PreventNull(list[3]);
            data.Disabled = Helper.PreventNull(list[4]);
            data.Remark = Helper.PreventNull(list[5]);
            data.Attribute1 = Helper.PreventNull(list[6]);
            data.Attribute2 = Helper.PreventNull(list[7]);
            data.Attribute3 = Helper.PreventNull(list[8]);
            data.Attribute4 = Helper.PreventNull(list[9]);
            data.Attribute5 = Helper.PreventNull(list[10]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_codemasterhist    " +
                    "            ( backuptime          " +
                    "             ,systemcd            " +
                    "             ,codeclass           " +
                    "             ,code                " +
                    "             ,value               " +
                    "             ,disabled            " +
                    "             ,remark              " +
                    "             ,attribute1          " +
                    "             ,attribute2          " +
                    "             ,attribute3          " +
                    "             ,attribute4          " +
                    "             ,attribute5          " +
                    "            )                     " +
                    "      VALUES                      " +
                    "            ( @BACKUPTIME         " +
                    "             ,@SYSTEMCD           " +
                    "             ,@CODECLASS          " +
                    "             ,@CODE               " +
                    "             ,@VALUE              " +
                    "             ,@DISABLED           " +
                    "             ,@REMARK             " +
                    "             ,@ATTRIBUTE1         " +
                    "             ,@ATTRIBUTE2         " +
                    "             ,@ATTRIBUTE3         " +
                    "             ,@ATTRIBUTE4         " +
                    "             ,@ATTRIBUTE5         " +
                    "            )                     ";

            return strSql;
        }
    }
}
