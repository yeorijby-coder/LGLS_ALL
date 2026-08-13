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
    public class BatchHistData : ECPObject
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string batchno = "";

        public string Batchno
        {
            get { return batchno; }
            set { batchno = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string validexpireddate = "";

        public string Validexpireddate
        {
            get { return validexpireddate; }
            set { validexpireddate = value; }
        }
        private string productiondate = "";

        public string Productiondate
        {
            get { return productiondate; }
            set { productiondate = value; }
        }
        private string vendorbatchno = "";

        public string Vendorbatchno
        {
            get { return vendorbatchno; }
            set { vendorbatchno = value; }
        }
        private string lastgoodsreceiptdate = "";

        public string Lastgoodsreceiptdate
        {
            get { return lastgoodsreceiptdate; }
            set { lastgoodsreceiptdate = value; }
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

        public BatchHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertBatchHistData()
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

                //TB_Batchhist 정보  inert                                                         
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(this.Validexpireddate));
                pss.SetVarChar("PRODUCTIONDATE", Helper.PreventNull(this.Productiondate));
                pss.SetVarChar("VENDORBATCHNO", Helper.PreventNull(this.Vendorbatchno));
                pss.SetVarChar("LASTGOODSRECEIPTDATE", Helper.PreventNull(this.Lastgoodsreceiptdate));
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

        public void insertBatchHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Batchhist 정보  inert                                                         
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(this.Validexpireddate));
                pss.SetVarChar("PRODUCTIONDATE", Helper.PreventNull(this.Productiondate));
                pss.SetVarChar("VENDORBATCHNO", Helper.PreventNull(this.Vendorbatchno));
                pss.SetVarChar("LASTGOODSRECEIPTDATE", Helper.PreventNull(this.Lastgoodsreceiptdate));
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

        public void setBatchHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Batchno = Helper.PreventNull(list[0]);
            this.Materialno = Helper.PreventNull(list[1]);
            this.Validexpireddate = Helper.PreventNull(list[2]);
            this.Productiondate = Helper.PreventNull(list[3]);
            this.Vendorbatchno = Helper.PreventNull(list[4]);
            this.Lastgoodsreceiptdate = Helper.PreventNull(list[5]);
            this.Createtime = Helper.PreventNull(list[6]);
            this.Createman = Helper.PreventNull(list[7]);
            this.Updatetime = Helper.PreventNull(list[8]);
            this.Updateman = Helper.PreventNull(list[9]);
        }

        public BatchData setBatchData(string[] list)
        {
            BatchData data = new BatchData();

            data.Batchno = Helper.PreventNull(list[0]);
            data.Materialno = Helper.PreventNull(list[1]);
            data.Validexpireddate = Helper.PreventNull(list[2]);
            data.Productiondate = Helper.PreventNull(list[3]);
            data.Vendorbatchno = Helper.PreventNull(list[4]);
            data.Lastgoodsreceiptdate = Helper.PreventNull(list[5]);
            data.Createtime = Helper.PreventNull(list[6]);
            data.Createman = Helper.PreventNull(list[7]);
            data.Updatetime = Helper.PreventNull(list[8]);
            data.Updateman = Helper.PreventNull(list[9]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_batchhist           " +
                    "            ( backuptime            " +
                    "             ,batchno               " +
                    "             ,materialno            " +
                    "             ,validexpireddate      " +
                    "             ,productiondate        " +
                    "             ,vendorbatchno         " +
                    "             ,lastgoodsreceiptdate  " +
                    "             ,createtime            " +
                    "             ,createman             " +
                    "             ,updatetime            " +
                    "             ,updateman             " +
                    "            )                       " +
                    "      VALUES                        " +
                    "            ( @BACKUPTIME           " +
                    "             ,@BATCHNO              " +
                    "             ,@MATERIALNO           " +
                    "             ,@VALIDEXPIREDDATE     " +
                    "             ,@PRODUCTIONDATE       " +
                    "             ,@VENDORBATCHNO        " +
                    "             ,@LASTGOODSRECEIPTDATE " +
                    "             ,@CREATETIME           " +
                    "             ,@CREATEMAN            " +
                    "             ,@UPDATETIME           " +
                    "             ,@UPDATEMAN            " +
                    "            )                       ";                    

            return strSql;
        }
    }
}
