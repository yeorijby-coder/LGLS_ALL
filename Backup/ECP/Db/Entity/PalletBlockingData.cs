using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class PalletBlockingData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
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
        private string blockingtype = "";

        public string Blockingtype
        {
            get { return blockingtype; }
            set { blockingtype = value; }
        }
        private string blockingreason = "";

        public string Blockingreason
        {
            get { return blockingreason; }
            set { blockingreason = value; }
        }
        private string starttime = "";

        public string Starttime
        {
            get { return starttime; }
            set { starttime = value; }
        }
        private string startman = "";

        public string Startman
        {
            get { return startman; }
            set { startman = value; }
        }
        private string endtime = "";

        public string Endtime
        {
            get { return endtime; }
            set { endtime = value; }
        }
        private string endman = "";

        public string Endman
        {
            get { return endman; }
            set { endman = value; }
        }

        public PalletBlockingData()
        {
        }

        //전체 반송 정보를 가져옴.
        public void getPalletBlockingDataWithPalletidStarttime(string palletid, string starttime)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += " WHERE plantcode = '" + GlobalConstant.PLANT_CODE + "' " +
                      "   AND palletid  = '" + palletid + "' " +
                      "   AND starttime = '" + starttime + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setPalletBlockingData(reader);
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

        //전체 반송 정보를 가져옴.
        public List<PalletBlockingData> getPalletBlockingDataList()
        {
            List<PalletBlockingData> list = new List<PalletBlockingData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletBlockingData data = new PalletBlockingData();

                    data.setPalletBlockingData(reader);

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

        //Pallet Meterial/Batch List 조회
        public List<PalletBlockingData> getPalletBlockingDataMeterialBatchList(string materialno, string batchno)
        {
            List<PalletBlockingData> list = new List<PalletBlockingData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "    AND materialno   LIKE '" + materialno + "'  " +
                      "    AND batchno      LIKE '" + batchno + "'  " +
                      "  ORDER BY materialno                                       ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletBlockingData pData = new PalletBlockingData();
                    pData.setPalletBlockingData(reader);

                    list.Add(pData);
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

        public void insertPalletBlockingData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_PALLETBLOCKING DATA update                                                    
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("BLOCKINGTYPE", Helper.PreventNull(this.Blockingtype));
                pss.SetVarChar("BLOCKINGREASON", Helper.PreventNull(this.Blockingreason));
                pss.SetVarChar("STARTTIME", Helper.PreventNull(this.Starttime));
                pss.SetVarChar("STARTMAN", Helper.PreventNull(this.Startman));
                pss.SetVarChar("ENDTIME", Helper.PreventNull(this.Endtime));
                pss.SetVarChar("ENDMAN", Helper.PreventNull(this.Endman));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertPalletBlockingDataBackup(theDB, "Insert");
                }

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updatePalletBlockingData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_PALLETBLOCKING DATA update                                                    
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("BLOCKINGTYPE", Helper.PreventNull(this.Blockingtype));
                pss.SetVarChar("BLOCKINGREASON", Helper.PreventNull(this.Blockingreason));
                pss.SetVarChar("STARTTIME", Helper.PreventNull(this.Starttime));
                pss.SetVarChar("STARTMAN", Helper.PreventNull(this.Startman));
                pss.SetVarChar("ENDTIME", Helper.PreventNull(this.Endtime));
                pss.SetVarChar("ENDMAN", Helper.PreventNull(this.Endman));
                pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID1", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("STARTTIME1", Helper.PreventNull(this.Starttime));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertPalletBlockingDataBackup(theDB, "Update");
                }

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deletePalletBlockingData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_PALLETBLOCKING DATA update                                                    
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("STARTTIME", Helper.PreventNull(this.Starttime));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertPalletBlockingDataBackup(theDB, "Delete");
                }

                theDB.ExecuteUpdate(strSql, pss);
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setPalletBlockingData(SqlDataReader reader)
        {
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Blockingtype = Helper.PreventNull((string)reader["BLOCKINGTYPE"]);
            this.Blockingreason = Helper.PreventNull((string)reader["BLOCKINGREASON"]);
            this.Starttime = Helper.PreventNull((string)reader["STARTTIME"]);
            this.Startman = Helper.PreventNull((string)reader["STARTMAN"]);
            this.Endtime = Helper.PreventNull((string)reader["ENDTIME"]);
            this.Endman = Helper.PreventNull((string)reader["ENDMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    "  SELECT  plantcode                                " +
                    "         ,palletid                                 " +
                    "         ,ISNULL(batchno       ,'')  batchno       " +
                    "         ,ISNULL(materialno    ,'')  materialno    " +
                    "         ,ISNULL(qty           ,'')  qty           " +
                    "         ,ISNULL(unit          ,'')  unit          " +
                    "         ,ISNULL(blockingtype  ,'')  blockingtype  " +
                    "         ,ISNULL(blockingreason,'')  blockingreason" +
                    "         ,ISNULL(starttime     ,'')  starttime     " +
                    "         ,ISNULL(startman      ,'')  startman      " +
                    "         ,ISNULL(endtime       ,'')  endtime       " +
                    "         ,ISNULL(endman        ,'')  endman        " +
                    "    FROM tb_palletblocking                         ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    "  INSERT INTO tb_palletblocking     " +
                    "        ( plantcode                 " +
                    "         ,palletid                  " +
                    "         ,batchno                   " +
                    "         ,materialno                " +
                    "         ,qty                       " +
                    "         ,unit                      " +
                    "         ,blockingtype              " +
                    "         ,blockingreason            " +
                    "         ,starttime                 " +
                    "         ,startman                  " +
                    "         ,endtime                   " +
                    "         ,endman                    " +
                    "        )                           " +
                    "  VALUES (  @PLANTCODE              " +
                    "           ,@PALLETID               " +
                    "           ,@BATCHNO                " +
                    "           ,@MATERIALNO             " +
                    "           ,@QTY                    " +
                    "           ,@UNIT                   " +
                    "           ,@BLOCKINGTYPE           " +
                    "           ,@BLOCKINGREASON         " +
                    "           ,@STARTTIME              " +
                    "           ,@STARTMAN               " +
                    "           ,@ENDTIME                " +
                    "           ,@ENDMAN                 " +
                    "         )                          ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    "  UPDATE tb_palletblocking                 " +
                    "     SET  plantcode      = @PLANTCODE      " +
                    "         ,palletid       = @PALLETID       " +
                    "         ,batchno        = @BATCHNO        " +
                    "         ,materialno     = @MATERIALNO     " +
                    "         ,qty            = @QTY            " +
                    "         ,unit           = @UNIT           " +
                    "         ,blockingtype   = @BLOCKINGTYPE   " +
                    "         ,blockingreason = @BLOCKINGREASON " +
                    "         ,starttime      = @STARTTIME      " +
                    "         ,startman       = @STARTMAN       " +
                    "         ,endtime        = @ENDTIME        " +
                    "         ,endman         = @ENDMAN         " +
                    "   WHERE  plantcode      = @PLANTCODE1     " +
                    "     AND  palletid       = @PALLETID1      " +
                    "     AND  starttime      = @STARTTIME1     ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    "  DELETE  tb_palletblocking               " +
                    "   WHERE  plantcode      = @PLANTCODE     " +
                    "     AND  palletid       = @PALLETID      " +
                    "     AND  starttime      = @STARTTIME     ";

            return strSql;
        }

        public void insertPalletBlockingDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                PalletBlockingData data = new PalletBlockingData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE plantcode  = '" + GlobalConstant.PLANT_CODE + "' " +
                          "   AND palletid   = '" + this.Palletid             + "' " +
                          "   AND starttime  = '" + this.Starttime            + "' ";

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
                    data.setPalletBlockingData(reader);
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
            ifValue +=  this.Plantcode + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Blockingtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Blockingreason + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Starttime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Startman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Endtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Endman;        

            return ifValue;
        }
    }
}
