using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferDetailHistData : ECPObject 
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
        private string commandid = "";

        public string Commandid
        {
            get { return commandid; }
            set { commandid = value; }
        }
        private string commandseq = "";

        public string Commandseq
        {
            get { return commandseq; }
            set { commandseq = value; }
        }
        private string pathtotal = "";

        public string Pathtotal
        {
            get { return pathtotal; }
            set { pathtotal = value; }
        }
        private string pathid = "";

        public string Pathid
        {
            get { return pathid; }
            set { pathid = value; }
        }
        private string transferdetailstatus = "";

        public string Transferdetailstatus
        {
            get { return transferdetailstatus; }
            set { transferdetailstatus = value; }
        }
        private string executetime = "";

        public string Executetime
        {
            get { return executetime; }
            set { executetime = value; }
        }
        private string completetime = "";

        public string Completetime
        {
            get { return completetime; }
            set { completetime = value; }
        }

        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string currentpalletlocation = "";

        public string Currentpalletlocation
        {
            get { return currentpalletlocation; }
            set { currentpalletlocation = value; }
        }
        private string palletidtomove = "";

        public string Palletidtomove
        {
            get { return palletidtomove; }
            set { palletidtomove = value; }
        }

        private string dbConnectionString = "";
        public TransferDetailHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertTransferDetailHistData() 
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

                //TB_TRANSFERDETAIL 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PATHID", Helper.PreventNull(this.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(this.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(this.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(this.Palletidtomove));

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

        public void insertTransferDetailHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFERDETAIL 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PATHID", Helper.PreventNull(this.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(this.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(this.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(this.Palletidtomove));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertTransferDetailHistData(TransferDetailData tdData)
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

                //TB_TRANSFERDETAIL 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", DateTime.Now.ToString("yyyyMMddHHmmssfff"));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(tdData.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(tdData.Commandseq));
                pss.SetVarChar("PATHID", Helper.PreventNull(tdData.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(tdData.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(tdData.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(tdData.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(tdData.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(tdData.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(tdData.Palletidtomove));

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

        public void setTransferDetailHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Commandid = Helper.PreventNull(list[1]);
            this.Commandseq = Helper.PreventNull(list[2]);
            this.Pathtotal = Helper.PreventNull(list[3]);
            this.Pathid = Helper.PreventNull(list[4]);
            this.Transferdetailstatus = Helper.PreventNull(list[5]);
            this.Executetime = Helper.PreventNull(list[6]);
            this.Completetime = Helper.PreventNull(list[7]);
            this.Palletid = Helper.PreventNull(list[8]);
            this.Currentpalletlocation = Helper.PreventNull(list[9]);
            this.Palletidtomove = Helper.PreventNull(list[10]);

        }

        public TransferDetailData setTransferDetailData(string[] list)
        {
            TransferDetailData data = new TransferDetailData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Commandid = Helper.PreventNull(list[1]);
            data.Commandseq = Helper.PreventNull(list[2]);
            data.Pathtotal = Helper.PreventNull(list[3]);
            data.Pathid = Helper.PreventNull(list[4]);
            data.Transferdetailstatus = Helper.PreventNull(list[5]);
            data.Executetime = Helper.PreventNull(list[6]);
            data.Completetime = Helper.PreventNull(list[7]);
            data.Palletid = Helper.PreventNull(list[8]);
            data.Currentpalletlocation = Helper.PreventNull(list[9]);
            data.Palletidtomove = Helper.PreventNull(list[10]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql ="" +
                    "  INSERT INTO tb_transferdetailhist     " +
                    "             (backuptime                " +
                    "             ,commandid                 " +
                    "             ,commandseq                " +
                    "             ,pathid                    " +
                    "             ,transferdetailstatus      " +
                    "             ,executetime               " +
                    "             ,completetime              " +
                    "             ,palletid                  " +
                    "             ,currentpalletlocation     " +
                    "             ,palletidtomove)           " +
                    "       VALUES                           " +
                    "             (@BACKUPTIME               " +
                    "             ,@COMMANDID                " +
                    "             ,@COMMANDSEQ               " +
                    "             ,@PATHID                   " +
                    "             ,@TRANSFERDETAILSTATUS     " +
                    "             ,@EXECUTETIME              " +
                    "             ,@COMPLETETIME             " +
                    "             ,@PALLETID                 " +
                    "             ,@CURRENTPALLETLOCATION    " +
                    "             ,@PALLETIDTOMOVE)          ";

            return strSql;
        }
    }
}
