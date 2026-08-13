using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class MaterialHistData : ECPObject
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string materialname = "";

        public string Materialname
        {
            get { return materialname; }
            set { materialname = value; }
        }
        private string materialtype = "";

        public string Materialtype
        {
            get { return materialtype; }
            set { materialtype = value; }
        }
        private string unit = "";

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
        }
        private string tempconditionflg = "";

        public string Tempconditionflg
        {
            get { return tempconditionflg; }
            set { tempconditionflg = value; }
        }
        private string valueconfig = "";

        public string Valueconfig
        {
            get { return valueconfig; }
            set { valueconfig = value; }
        }
        private string tempmin = "";

        public string Tempmin
        {
            get { return tempmin; }
            set { tempmin = value; }
        }
        private string tempmax = "";

        public string Tempmax
        {
            get { return tempmax; }
            set { tempmax = value; }
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
        public MaterialHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertMaterialHistData()
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

                //TB_Material 정보  inert                                                      
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.updateman));

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

        public void insertMaterialHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Material 정보  inert                                                      
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.updateman));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setMaterialHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Materialno = Helper.PreventNull(list[0]);
            this.Materialname = Helper.PreventNull(list[1]);
            this.Materialtype = Helper.PreventNull(list[2]);
            this.Unit = Helper.PreventNull(list[3]);
            this.Tempconditionflg = Helper.PreventNull(list[4]);
            this.Valueconfig = Helper.PreventNull(list[5]);
            this.Tempmin = Helper.PreventNull(list[6]);
            this.Tempmax = Helper.PreventNull(list[7]);
            this.Createtime = Helper.PreventNull(list[8]);
            this.Createman = Helper.PreventNull(list[9]);
            this.Updatetime = Helper.PreventNull(list[10]);
            this.Updateman = Helper.PreventNull(list[11]);
        }

        public MaterialData setMaterialData(string[] list)
        {
            MaterialData data = new MaterialData();

            data.Materialno = Helper.PreventNull(list[0]);
            data.Materialname = Helper.PreventNull(list[1]);
            data.Materialtype = Helper.PreventNull(list[2]);
            data.Unit = Helper.PreventNull(list[3]);
            data.Tempconditionflg = Helper.PreventNull(list[4]);
            data.Valueconfig = Helper.PreventNull(list[5]);
            data.Tempmin = Helper.PreventNull(list[6]);
            data.Tempmax = Helper.PreventNull(list[7]);
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
                    " INSERT INTO tb_materialhist    " +
                    "            ( backuptime        " +
                    "             ,materialno        " +
                    "             ,materialname      " +
                    "             ,materialtype      " +
                    "             ,unit              " +
                    "             ,tempconditionflg  " +
                    "             ,valueconfig       " +
                    "             ,tempmin           " +
                    "             ,tempmax           " +
                    "             ,createtime        " +
                    "             ,createman         " +
                    "             ,updatetime        " +
                    "             ,updateman         " +
                    "            )                   " +
                    "      VALUES                    " +
                    "            ( @BACKUPTIME       " +
                    "             ,@MATERIALNO       " +
                    "             ,@MATERIALNAME     " +
                    "             ,@MATERIALTYPE     " +
                    "             ,@UNIT             " +
                    "             ,@TEMPCONDITIONFLG " +
                    "             ,@VALUECONFIG      " +
                    "             ,@TEMPMIN          " +
                    "             ,@TEMPMAX          " +
                    "             ,@CREATETIME       " +
                    "             ,@CREATEMAN        " +
                    "             ,@UPDATETIME       " +
                    "             ,@UPDATEMAN        " +
                    "            )                   ";

            return strSql;
        }
    }
}
