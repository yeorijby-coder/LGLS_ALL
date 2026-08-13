using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferDetailData : ECPObject 
    {
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

        public TransferDetailData()
        {
        }

        public int getTransferDetailDataCnt(TransferData tData)
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT COUNT(*)  CNT                                    " +
                            "   FROM tb_transferdetail                                " +
                            "  WHERE commandid = '" + tData.Commandid           + "'  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
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

            return iCnt;
        }

        public int getTransferDetailDataCnt(string commandid)
        {
            int iCnt = 0;

            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();
                iCnt = getTransferDetailDataCnt(commandid, theDB);
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
            
            return iCnt;
        }

        public int getTransferDetailDataCnt(string commandid, DBConnection theDB)
        {
            int iCnt = 0;

            SqlDataReader reader = null;

            string strSql = " SELECT COUNT(*)  CNT                                    " +
                            "   FROM tb_transferdetail                                " +
                            "  WHERE commandid = '" + commandid + "'  ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);
                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
            }
            
            return iCnt;
        }

        public int getTransferDetailData(string commandid, string commandseq)
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE commandid = '" + commandid + "'                  " +
                      "    AND commandseq = '" + commandseq + "'                ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setTransferDetailData(reader);
                    iCnt = 1;
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
            
            return iCnt;
        }

        public void getTransferDetailInGoLastData(string commandid, string tolocationid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE commandid = '" + commandid + "'                  " +
                      "    AND pathid in (SELECT pathid                         " +
                      "                     FROM tb_itinerary                   " +
                      "                    WHERE tolocationid = '" + tolocationid + "' " +
                      "                  )";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setTransferDetailData(reader);
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
        }

        public List<TransferDetailData> getTransferDetailDataList(TransferData tdData)
        {
            List<TransferDetailData> list = new List<TransferDetailData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;


            string strSql = getSelectSQL();
            strSql += "  WHERE commandid = '" + tdData.Commandid + "'           ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDetailData data = new TransferDetailData();

                    data.setTransferDetailData(reader);

                    list.Add(data);
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

        public int makeTransferDetailDataList(TransferData tfData)
        {
            int iResult = 0;
            DBConnection theDB = null;

            TotalPathData tpData = new TotalPathData();
            try
            {
                tpData.getTotalPathData(tfData);

                if (tpData == null || string.IsNullOrEmpty(tpData.Pathtotal))
                {
                    Exception newEx = new Exception("반송번호(" + tfData.Commandid + "): 반송 경로가 없습니다.");
                    throw newEx;
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            
            //this.Plantcode = tfData.Plantcode;
            this.Commandid = tfData.Commandid;
            //this.Pathtotal = tpData.Pathtotal;
            this.Transferdetailstatus = EntityEnumData.TRANSFERDETAILSTATUS.Wait.ToString();
            this.Executetime = "";
            this.Completetime = "";
            this.Palletid = "";
            this.Currentpalletlocation = "";
            this.Palletidtomove = "";

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                char[] separator = { ';' };
                string[] strPathidList = (tpData.Pathtotal).Split(separator,StringSplitOptions.RemoveEmptyEntries);

                int iCnt = 0;
                foreach (string strPathid in strPathidList)
                {
                    iCnt += 1;
                    this.Pathid = strPathid;
                    this.Commandseq = iCnt.ToString("0000");

                    this.insertTransferDetailData(theDB, true);
                }

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                iResult = -1;
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

            return iResult;
        }

        public void insertTransferDetailData(DBConnection theDB, Boolean bkFlag) 
        {
            PreparedSqlParameter pss = null;
            string strSql = getInsertSQL();

            try
            {
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFERDETAIL 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                //pss.SetVarChar("PATHTOTAL", Helper.PreventNull(this.Pathtotal));
                pss.SetVarChar("PATHID", Helper.PreventNull(this.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(this.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(this.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(this.Palletidtomove));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDetailDataBackup(theDB, "Insert");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        TransferDetailHistData tdhData = new TransferDetailHistData();
                        tdhData.insertTransferDetailHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public int updateTransferDetailData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            int iResult = 0;

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getUpdateSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFERDETAIL 반송 정보 update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                //pss.SetVarChar("PATHTOTAL", Helper.PreventNull(this.Pathtotal));
                pss.SetVarChar("PATHID", Helper.PreventNull(this.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(this.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(this.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(this.Palletidtomove));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID1", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ1", Helper.PreventNull(this.Commandseq));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDetailDataBackup(theDB, "Update");

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferDetailHistData tdhData = new TransferDetailHistData();
                    tdhData.insertTransferDetailHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
            }
            catch (SqlException ex)
            {
                iResult = -1;
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

            return iResult;
        }

        public void updateTransferDetailData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";
            
            try
            {
                strSql = getUpdateSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFERDETAIL 반송 정보 update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                //pss.SetVarChar("PATHTOTAL", Helper.PreventNull(this.Pathtotal));
                pss.SetVarChar("PATHID", Helper.PreventNull(this.Pathid));
                pss.SetVarChar("TRANSFERDETAILSTATUS", Helper.PreventNull(this.Transferdetailstatus));
                pss.SetVarChar("EXECUTETIME", Helper.PreventNull(this.Executetime));
                pss.SetVarChar("COMPLETETIME", Helper.PreventNull(this.Completetime));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("CURRENTPALLETLOCATION", Helper.PreventNull(this.Currentpalletlocation));
                pss.SetVarChar("PALLETIDTOMOVE", Helper.PreventNull(this.Palletidtomove));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID1", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ1", Helper.PreventNull(this.Commandseq));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDetailDataBackup(theDB, "Update");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        TransferDetailHistData tdhData = new TransferDetailHistData();
                        tdhData.insertTransferDetailHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public int deleteTransferDetailData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";
            int iResult = 0;

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getDeleteSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));


                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertTransferDetailDataBackup(theDB, "Delete");

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();

                //DataBackup
                try
                {
                    TransferDetailHistData tdhData = new TransferDetailHistData();
                    tdhData.insertTransferDetailHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
            }
            catch (SqlException ex)
            {
                iResult = -1;
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

            return iResult;
        }

        public void deleteTransferDetailData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_TRANSFER 반송 정보 Insert
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertTransferDetailDataBackup(theDB, "Delete");
                //}


                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    //this.insertTransferDetailDataBackup(theDB, "Delete");
                    try
                    {
                        TransferDetailHistData tdhData = new TransferDetailHistData();
                        tdhData.insertTransferDetailHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public Dictionary<string, TransferDetailData> getTransferDetailDataList(string commandid)
        {
            Dictionary<string, TransferDetailData> list = new Dictionary<string, TransferDetailData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WITH (UPDLOCK)                                       " +
                      " WHERE commandid = '" + commandid                 + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDetailData data = new TransferDetailData();

                    data.setTransferDetailData(reader);

                    list.Add(data.Commandseq, data);
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

        public List<TransferDetailData> getNewTransferDetailDataList(string fromdeviceid)
        {
            List<TransferDetailData> list = new List<TransferDetailData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += " WHERE pathid in ( SELECT pathid                                          " +
                      "                     FROM tb_itinerary                                    " +
                      "                    WHERE fromdeviceid = '" + fromdeviceid + "' )         " +
                      "   AND executetime is null                                                ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDetailData data = new TransferDetailData();

                    data.setTransferDetailData(reader);

                    list.Add(data);
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

        public List<TransferDetailData> getNotCompletedTransferDetailDataList(string fromdeviceid)
        {
            List<TransferDetailData> list = new List<TransferDetailData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += " WHERE pathid in ( SELECT pathid                                          " +
                      "                     FROM tb_itinerary                                    " +
                      "                    WHERE fromdeviceid = '" + fromdeviceid + "' )         " +
                      "   AND completetime is null                                               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDetailData data = new TransferDetailData();

                    data.setTransferDetailData(reader);

                    list.Add(data);
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

        public void setTransferDetailData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandseq = Helper.PreventNull((string)reader["COMMANDSEQ"]);
            //this.Pathtotal = Helper.PreventNull((string)reader["PATHTOTAL"]);
            this.Pathid = Helper.PreventNull((string)reader["PATHID"]);
            this.Transferdetailstatus = Helper.PreventNull((string)reader["TRANSFERDETAILSTATUS"]);
            this.Executetime = Helper.PreventNull((string)reader["EXECUTETIME"]);
            this.Completetime = Helper.PreventNull((string)reader["COMPLETETIME"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Currentpalletlocation = Helper.PreventNull((string)reader["CURRENTPALLETLOCATION"]);
            this.Palletidtomove = Helper.PreventNull((string)reader["PALLETIDTOMOVE"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql ="" +
                    " SELECT  commandid                         commandid             " +
                    "        ,commandseq                        commandseq            " +
                    "        ,ISNULL(pathid                ,'') pathid                " +
                    "        ,ISNULL(transferdetailstatus  ,'') transferdetailstatus  " +
                    "        ,ISNULL(executetime           ,'') executetime           " +
                    "        ,ISNULL(completetime          ,'') completetime          " +
                    "        ,ISNULL(palletid              ,'') palletid              " +
                    "        ,ISNULL(currentpalletlocation ,'') currentpalletlocation " +
                    "        ,ISNULL(palletidtomove        ,'') palletidtomove        " +
                    "   FROM tb_transferdetail                                        ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql ="" +
                    "  INSERT INTO tb_transferdetail         " +
                    "             (commandid                 " +
                    "             ,commandseq                " +
                    "             ,pathid                    " +
                    "             ,transferdetailstatus      " +
                    "             ,executetime               " +
                    "             ,completetime              " +
                    "             ,palletid                  " +
                    "             ,currentpalletlocation     " +
                    "             ,palletidtomove)           " +
                    "       VALUES                           " +
                    "             (@COMMANDID                " +
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

        public string getUpdateSQL()
        {
            string strSql = "";

            strSql = " " +
                    " UPDATE tb_transferdetail                                " +
                    "    SET  commandid             = @COMMANDID              " +
                    "        ,commandseq            = @COMMANDSEQ             " +
                    "        ,pathid                = @PATHID                 " +
                    "        ,transferdetailstatus  = @TRANSFERDETAILSTATUS   " +
                    "        ,executetime           = @EXECUTETIME            " +
                    "        ,completetime          = @COMPLETETIME           " +
                    "        ,palletid              = @PALLETID               " +
                    "        ,currentpalletlocation = @CURRENTPALLETLOCATION  " +
                    "        ,palletidtomove        = @PALLETIDTOMOVE         " +
                    " WHERE  commandid  = @COMMANDID1                         " +
                    "   AND  commandseq = @COMMANDSEQ1                        ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";

            strSql = " " +
                    " DELETE tb_transferdetail             " +
                    " WHERE  commandid = @COMMANDID        " +
                    "   AND  commandseq = @COMMANDSEQ      ";

            return strSql;
        }

        public void insertTransferDetailDataBackup()
        {
            TransferDetailHistData tdhData = new TransferDetailHistData();
            try
            {
                tdhData.insertTransferDetailHistData(this);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void insertTransferDetailDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                TransferDetailData data = new TransferDetailData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE commandid  = '" + this.Commandid   + "' " +
                          "   AND commandseq = '" + this.Commandseq  + "' ";

                try
                {
                    reader = theDB.ExecuteQuery(strSql);
                }
                catch (SqlException se)
                {
                    if (reader != null) reader.Close();
                    throw se;
                }
                
                if (reader.Read())
                {
                    data.setTransferDetailData(reader);
                }
                else
                {
                    if (reader != null) reader.Close();
                    return;
                }

                if (reader != null) reader.Close();

                internalIfData.Internaliftype = data.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = data.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }
            else
            {
                internalIfData.Internaliftype = this.GetType().Name;
                internalIfData.Transactiontype = transactiontype;
                internalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                internalIfData.Ifvalue = this.makeInternalIfValue();
                internalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;
            }

            try
            {
                internalIfData.insertInternalIfData(theDB);     
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public string makeInternalIfValue()
        {
            string ifValue = "";
            ifValue += this.Commandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandseq + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pathid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Transferdetailstatus + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Executetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Completetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Currentpalletlocation + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletidtomove;

            return ifValue;
        }
    }
}
