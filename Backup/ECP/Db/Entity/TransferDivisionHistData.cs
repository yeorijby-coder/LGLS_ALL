using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferDivisionHistData : ECPObject 
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
        private string sapcommandid = "";

        public string Sapcommandid
        {
            get { return sapcommandid; }
            set { sapcommandid = value; }
        }
        private string ecscommandid = "";

        public string Ecscommandid
        {
            get { return ecscommandid; }
            set { ecscommandid = value; }
        }
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
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
        private string sapqty = "";

        public string Sapqty
        {
            get { return sapqty; }
            set { sapqty = value; }
        }
        private string qty = "";

        public string Qty
        {
            get { return qty; }
            set { qty = value; }
        }
        private string ordertype = "";

        public string Ordertype
        {
            get { return ordertype; }
            set { ordertype = value; }
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

        private string dbConnectionString = "";
        public TransferDivisionHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertTransferDivisionHistData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                para.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                para.SetVarChar("SAPCOMMANDID", Helper.PreventNull(this.Sapcommandid));
                para.SetVarChar("ECSCOMMANDID", Helper.PreventNull(this.Ecscommandid));
                para.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                para.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                para.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                para.SetVarChar("SAPQTY", Helper.PreventNull(this.Sapqty));
                para.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                para.SetVarChar("ORDERTYPE", Helper.PreventNull(this.Ordertype));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman)); 

                theDB.ExecuteUpdate(strSql, para);

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

        public void insertTransferDivisionHistData(DBConnection theDB)
        {
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                para.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                para.SetVarChar("SAPCOMMANDID", Helper.PreventNull(this.Sapcommandid));
                para.SetVarChar("ECSCOMMANDID", Helper.PreventNull(this.Ecscommandid));
                para.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                para.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                para.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                para.SetVarChar("SAPQTY", Helper.PreventNull(this.Sapqty));
                para.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                para.SetVarChar("ORDERTYPE", Helper.PreventNull(this.Ordertype));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));

                theDB.ExecuteUpdate(strSql, para);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setTransferDivisionHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Sapcommandid = Helper.PreventNull(list[1]);
            this.Ecscommandid = Helper.PreventNull(list[2]);
            this.Palletid = Helper.PreventNull(list[3]);
            this.Batchno = Helper.PreventNull(list[4]);
            this.Materialno = Helper.PreventNull(list[5]);
            this.Sapqty = Helper.PreventNull(list[6]);
            this.Qty = Helper.PreventNull(list[7]);
            this.Ordertype = Helper.PreventNull(list[8]);
            this.Createtime = Helper.PreventNull(list[9]);
            this.Createman = Helper.PreventNull(list[10]);
        }

        public TransferDivisionData setTransferDivisionData(string[] list)
        {
            TransferDivisionData data = new TransferDivisionData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Sapcommandid = Helper.PreventNull(list[1]);
            data.Ecscommandid = Helper.PreventNull(list[2]);
            data.Palletid = Helper.PreventNull(list[3]);
            data.Batchno = Helper.PreventNull(list[4]);
            data.Materialno = Helper.PreventNull(list[5]);
            data.Sapqty = Helper.PreventNull(list[6]);
            data.Qty = Helper.PreventNull(list[7]);
            data.Ordertype = Helper.PreventNull(list[8]);
            data.Createtime = Helper.PreventNull(list[9]);
            data.Createman = Helper.PreventNull(list[10]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  INSERT INTO tb_transferdivisionhist  " +
                    "            ( backuptime               " +
                    "             ,plantcode                " +
                    "             ,sapcommandid             " +
                    "             ,ecscommandid             " +
                    "             ,palletid                 " +
                    "             ,batchno                  " +
                    "             ,materialno               " +
                    "             ,sapqty                   " +
                    "             ,qty                      " +
                    "             ,ordertype                " +
                    "             ,createtime               " +
                    "             ,createman)               " +
                    "       VALUES                          " +
                    "            ( @BACKUPTIME              " +
                    "             ,@PLANTCODE               " +
                    "             ,@SAPCOMMANDID            " +
                    "             ,@ECSCOMMANDID            " +
                    "             ,@PALLETID                " +
                    "             ,@BATCHNO                 " +
                    "             ,@MATERIALNO              " +
                    "             ,@SAPQTY                  " +
                    "             ,@QTY                     " +
                    "             ,@ORDERTYPE               " +
                    "             ,@CREATETIME              " +
                    "             ,@CREATEMAN)              ";

            return strSql;
        }
    }
}
