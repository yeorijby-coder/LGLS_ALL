using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Db;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferHistData : ECPObject 
    {
        const string CLASS = "TransferHistData";

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
        private string commandtype = "";

        public string Commandtype
        {
            get { return commandtype; }
            set { commandtype = value; }
        }
        private string movementtype = "";

        public string Movementtype
        {
            get { return movementtype; }
            set { movementtype = value; }
        }

        private string priority = "";

        public string Priority
        {
            get { return priority; }
            set { priority = value; }
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
        private string qty = "";

        public string Qty
        {
            get { return qty; }
            set { qty = value; }
        }
        private string unit = "";

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
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

        private string currentcommandseq = "";

        public string Currentcommandseq
        {
            get { return currentcommandseq; }
            set { currentcommandseq = value; }
        }
        private string transferstatus = "";

        public string Transferstatus
        {
            get { return transferstatus; }
            set { transferstatus = value; }
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
        private string resultcode = "";

        public string Resultcode
        {
            get { return resultcode; }
            set { resultcode = value; }
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
        private string executecount = "";

        public string Executecount
        {
            get { return executecount; }
            set { executecount = value; }
        }

        private string fromplant = "";

        public string Fromplant
        {
            get { return fromplant; }
            set { fromplant = value; }
        }

        private string palletidname = "";

        public string Palletidname
        {
            get { return palletidname; }
            set { palletidname = value; }
        }

        private string dbConnectionString = "";
        public TransferHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //Hirack Meterial/Batch List 조회
        public List<TransferHistData> getTransferHistDataMeterialBatchList(string materialno, string batchno)
        {
            List<TransferHistData> list = new List<TransferHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "    AND materialno   LIKE '" + materialno + "'  " +
                      "    AND batchno      LIKE '" + batchno + "'  " +
                      "  ORDER BY materialno                                       ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferHistData tfhData = new TransferHistData();
                    tfhData.setTransferHistData(reader);

                    list.Add(tfhData);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return list;
        }

        //Hirack Meterial/Batch List 조회
       public List<TransferHistData> getTransferHistDataList(string fromdate, string todate)
        {
            List<TransferHistData> list = new List<TransferHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE backuptime BETWEEN '" + fromdate + "'  " +
                      "    AND                    '" + todate   + "'  " +
                      "    AND plantcode        = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "  ORDER BY backuptime                                           ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferHistData tfhData = new TransferHistData();
                    tfhData.setTransferHistData(reader);

                    list.Add(tfhData);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
 
            return list;
        }

        public int getTotalQueScheduledTime(TransferData data)
        {
            int iCnt = 0;

            List<TransferData> list = new List<TransferData>();
            list = data.getTransferQueList();

            return iCnt;
        }

        public void insertTransferHistData()
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

                //TB_TRANSFER 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));

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

        public void insertTransferHistData(TransferData tfData)
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

                //TB_TRANSFER 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", DateTime.Now.ToString("yyyyMMddHHmmssfff"));
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(tfData.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(tfData.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(tfData.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(tfData.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(tfData.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(tfData.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(tfData.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(tfData.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(tfData.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(tfData.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(tfData.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(tfData.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(tfData.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(tfData.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(tfData.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(tfData.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(tfData.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(tfData.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(tfData.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(tfData.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(tfData.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(tfData.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(tfData.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(tfData.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(tfData.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(tfData.Palletidname));

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

        public void insertTransferHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDTYPE", Helper.PreventNull(this.Commandtype));
                pss.SetVarChar("MOVEMENTTYPE", Helper.PreventNull(this.Movementtype));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMDEVICEID", Helper.PreventNull(this.Fromdeviceid));
                pss.SetVarChar("FROMLOCATIONID", Helper.PreventNull(this.Fromlocationid));
                pss.SetVarChar("FROMSUBLOCATIONID", Helper.PreventNull(this.Fromsublocationid));
                pss.SetVarChar("TODEVICEID", Helper.PreventNull(this.Todeviceid));
                pss.SetVarChar("TOLOCATIONID", Helper.PreventNull(this.Tolocationid));
                pss.SetVarChar("TOSUBLOCATIONID", Helper.PreventNull(this.Tosublocationid));
                pss.SetVarChar("CURRENTCOMMANDSEQ", Helper.PreventNull(this.Currentcommandseq));
                pss.SetVarChar("TRANSFERSTATUS", Helper.PreventNull(this.Transferstatus));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                //pss.SetVarChar("RESULTCODE", Helper.PreventNull(this.Resultcode));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                //pss.SetVarChar("EXECUTECOUNT", Helper.PreventNull(this.Executecount));
                pss.SetVarChar("PALLETIDNAME", Helper.PreventNull(this.Palletidname));

                theDB.ExecuteUpdate(strSql, pss);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setTransferHistData(string[] list)
        { 
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Commandid = Helper.PreventNull(list[1]);
            this.Commandtype = Helper.PreventNull(list[2]);
            this.Movementtype = Helper.PreventNull(list[3]);
            this.Priority = Helper.PreventNull(list[4]);
            this.Palletid = Helper.PreventNull(list[5]);
            this.Batchno = Helper.PreventNull(list[6]);
            this.Materialno = Helper.PreventNull(list[7]);
            this.Qty = Helper.PreventNull(list[8]);
            this.Unit = Helper.PreventNull(list[9]);
            this.Fromplant = Helper.PreventNull(list[10]);
            this.Fromdeviceid = Helper.PreventNull(list[11]);
            this.Fromlocationid = Helper.PreventNull(list[12]);
            this.Fromsublocationid = Helper.PreventNull(list[13]);
            this.Todeviceid = Helper.PreventNull(list[14]);
            this.Tolocationid = Helper.PreventNull(list[15]);
            this.Tosublocationid = Helper.PreventNull(list[16]);
            this.Transferstatus = Helper.PreventNull(list[17]);
            this.Createtime = Helper.PreventNull(list[18]);
            this.Createman = Helper.PreventNull(list[19]);
            this.Resultcode = Helper.PreventNull(list[20]);
            this.Executetime = Helper.PreventNull(list[21]);
            this.Completetime = Helper.PreventNull(list[22]);
            this.Executecount = Helper.PreventNull(list[23]);
        }

        public TransferData setTransferData(string[] list)
        {
            TransferData data = new TransferData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Commandid = Helper.PreventNull(list[1]);
            data.Commandtype = Helper.PreventNull(list[2]);
            data.Movementtype = Helper.PreventNull(list[3]);
            data.Priority = Helper.PreventNull(list[4]);
            data.Palletid = Helper.PreventNull(list[5]);
            data.Batchno = Helper.PreventNull(list[6]);
            data.Materialno = Helper.PreventNull(list[7]);
            data.Qty = Helper.PreventNull(list[8]);
            data.Unit = Helper.PreventNull(list[9]);
            data.Fromplant = Helper.PreventNull(list[10]);
            data.Fromdeviceid = Helper.PreventNull(list[11]);
            data.Fromlocationid = Helper.PreventNull(list[12]);
            data.Fromsublocationid = Helper.PreventNull(list[13]);
            data.Todeviceid = Helper.PreventNull(list[14]);
            data.Tolocationid = Helper.PreventNull(list[15]);
            data.Tosublocationid = Helper.PreventNull(list[16]);
            data.Transferstatus = Helper.PreventNull(list[17]);
            data.Createtime = Helper.PreventNull(list[18]);
            data.Createman = Helper.PreventNull(list[19]);
            data.Resultcode = Helper.PreventNull(list[20]);
            data.Executetime = Helper.PreventNull(list[21]);
            data.Completetime = Helper.PreventNull(list[22]);
            data.Executecount = Helper.PreventNull(list[23]);

            return data;
        }

        public void setTransferHistData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandtype = Helper.PreventNull((string)reader["COMMANDTYPE"]);
            this.Movementtype = Helper.PreventNull((string)reader["MOVEMENTTYPE"]);
            this.Priority = Helper.PreventNull((string)reader["PRIORITY"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            //this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            //this.Fromplant = Helper.PreventNull((string)reader["FROMPLANT"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Fromlocationid = Helper.PreventNull((string)reader["FROMLOCATIONID"]);
            this.Fromsublocationid = Helper.PreventNull((string)reader["FROMSUBLOCATIONID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Tolocationid = Helper.PreventNull((string)reader["TOLOCATIONID"]);
            this.Tosublocationid = Helper.PreventNull((string)reader["TOSUBLOCATIONID"]);
            this.Currentcommandseq = Helper.PreventNull((string)reader["CURRENTCOMMANDSEQ"]);
            this.Transferstatus = Helper.PreventNull((string)reader["TRANSFERSTATUS"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            //this.Resultcode = Helper.PreventNull((string)reader["RESULTCODE"]);
            this.Executetime = Helper.PreventNull((string)reader["EXECUTETIME"]);
            this.Completetime = Helper.PreventNull((string)reader["COMPLETETIME"]);
            //this.Executecount = Helper.PreventNull((string)reader["EXECUTECOUNT"]);
            this.Palletidname = Helper.PreventNull((string)reader["PALLETIDNAME"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = " " +
                    "  SELECT backuptime                       backuptime         " +
                    "        ,commandid                        commandid          " +
                    "        ,ISNULL(commandtype       ,'')    commandtype        " +
                    "        ,ISNULL(movementtype      ,'')    movementtype       " +
                    "        ,ISNULL(priority          ,'')    priority           " +
                    "        ,ISNULL(palletid          ,'')    palletid           " +
                    "        ,ISNULL(materialno        ,'')    materialno         " +
                    "        ,ISNULL(qty               ,'')    qty                " +
                    "        ,ISNULL(unit              ,'')    unit               " +
                    "        ,ISNULL(fromdeviceid      ,'')    fromdeviceid       " +
                    "        ,ISNULL(fromlocationid    ,'')    fromlocationid     " +
                    "        ,ISNULL(fromsublocationid ,'')    fromsublocationid  " +
                    "        ,ISNULL(todeviceid        ,'')    todeviceid         " +
                    "        ,ISNULL(tolocationid      ,'')    tolocationid       " +
                    "        ,ISNULL(tosublocationid   ,'')    tosublocationid    " +
                    "        ,ISNULL(currentcommandseq ,'')    currentcommandseq  " +
                    "        ,ISNULL(transferstatus    ,'')    transferstatus     " +
                    "        ,ISNULL(createtime        ,'')    createtime         " +
                    "        ,ISNULL(createman         ,'')    createman          " +
                    "        ,ISNULL(executetime       ,'')    executetime        " +
                    "        ,ISNULL(completetime      ,'')    completetime       " +
                    "        ,ISNULL(palletidname      ,'')    palletidname       " +
                    "    FROM tb_transferhist                                     ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql =" " + 
                    "INSERT INTO tb_transferhist    " +
                    "           (backuptime         " +
                    "           ,commandid          " +
                    "           ,commandtype        " +
                    "           ,movementtype       " +
                    "           ,priority           " +
                    "           ,palletid           " +
                    "           ,materialno         " +
                    "           ,qty                " +
                    "           ,unit               " +
                    "           ,fromdeviceid       " +
                    "           ,fromlocationid     " +
                    "           ,fromsublocationid  " +
                    "           ,todeviceid         " +
                    "           ,tolocationid       " +
                    "           ,tosublocationid    " +
                    "           ,currentcommandseq  " +
                    "           ,transferstatus     " +
                    "           ,createtime         " +
                    "           ,createman          " +
                    "           ,executetime        " +
                    "           ,completetime       " +
                    "           ,palletidname)      " +
                    "     VALUES                    " +
                    "           (@BACKUPTIME        " +
                    "           ,@COMMANDID         " +
                    "           ,@COMMANDTYPE       " +
                    "           ,@MOVEMENTTYPE      " +
                    "           ,@PRIORITY          " +
                    "           ,@PALLETID          " +
                    "           ,@MATERIALNO        " +
                    "           ,@QTY               " +
                    "           ,@UNIT              " +
                    "           ,@FROMDEVICEID      " +
                    "           ,@FROMLOCATIONID    " +
                    "           ,@FROMSUBLOCATIONID " +
                    "           ,@TODEVICEID        " +
                    "           ,@TOLOCATIONID      " +
                    "           ,@TOSUBLOCATIONID   " +
                    "           ,@CURRENTCOMMANDSEQ " +
                    "           ,@TRANSFERSTATUS    " +
                    "           ,@CREATETIME        " +
                    "           ,@CREATEMAN         " +
                    "           ,@EXECUTETIME       " +
                    "           ,@COMPLETETIME      " +
                    "           ,@PALLETIDNAME)     ";
                            
            return strSql;
        }
    }
}
