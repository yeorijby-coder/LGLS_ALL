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
    public class BatchData : ECPObject 
    {
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

        public BatchData()
        {
        }

        public void getBatchData(string batchno, string materialno)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE batchno    = '" + batchno    + "' " + 
                      "    AND materialno = '" + materialno + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setBatchData(reader);
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

        //Batch List 조회
        public List<BatchData> getBatchDataList()
        {
            List<BatchData> list = new List<BatchData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY batchno, materialno  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    BatchData bData = new BatchData();
                    bData.setBatchData(reader);

                    list.Add(bData);
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

        //Batch List 조회
        public List<BatchData> getBatchDataList(String dbConnectString)
        {
            List<BatchData> list = new List<BatchData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY batchno, materialno  ";

            try
            {
                if(string.IsNullOrEmpty(dbConnectString)) 
                {
                    theDB = DBControl.GetConnection();
                } else
                {
                    theDB = DBControl.GetConnection(dbConnectString);
                }

                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    BatchData bData = new BatchData();
                    bData.setBatchData(reader);

                    list.Add(bData);
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

        //Batch List 조회
        public List<BatchData> getBatchDataList(DBConnection theDB)
        {
            List<BatchData> list = new List<BatchData>();
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY batchno, materialno  ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    BatchData bData = new BatchData();
                    bData.setBatchData(reader);

                    list.Add(bData);
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

            return list;
        }

        //Batch Meterial/Batch List 조회
        public List<BatchData> getBatchDataMeterialBatchList(string materialno, string batchno)
        {
            List<BatchData> list = new List<BatchData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE batchno     LIKE '" + batchno    + "'  " +
                      "    AND materialno  LIKE '" + materialno + "'  " +
                      "  ORDER BY batchno                              ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    BatchData bData = new BatchData();
                    bData.setBatchData(reader);

                    list.Add(bData);
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

        public void insertBatchData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Batch 정보  inert                                                         
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

                if (bkFlag)
                {
                    //DataBackup
                    this.insertBatchDataBackup(theDB, "Insert");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updateBatchData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Material 정보  update
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
                pss.SetVarChar("BATCHNO1", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO1", Helper.PreventNull(this.Materialno));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertBatchDataBackup(theDB, "Update");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteBatchData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  delete
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertBatchDataBackup(theDB, "Delete");
                }

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertBatchData()
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

                //TB_Batch 정보  inert                                                         
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

                //DataBackup
                this.insertBatchDataBackup(theDB, "Insert");

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

        public void updateBatchData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Material 정보  update
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
                pss.SetVarChar("BATCHNO1", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO1", Helper.PreventNull(this.Materialno));
                                
                //DataBackup
                this.insertBatchDataBackup(theDB, "Update");

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

        public void deleteBatchData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection();
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  delete
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                
                //DataBackup
                this.insertBatchDataBackup(theDB, "Delete");

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

//배치 Update 시작
        public void deleteAllBatchData(String dbConnectionString)
        {
            DBConnection theDB = null;
            string strSql = "";

            try
            {
                if (string.IsNullOrEmpty(dbConnectionString))
                {
                    theDB = DBControl.GetConnection();
                }
                else
                {
                    theDB = DBControl.GetConnection(dbConnectionString);
                }

                strSql = "" +
                    " DELETE FROM tb_batch            ";

                theDB.ExecuteUpdate(strSql);

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
                };
            }
        }


        public void insertAllBatchData(String dbConnectionString)
        {
            DBConnection theDB = null;
            string strSql = "";

            try
            {
                if (string.IsNullOrEmpty(dbConnectionString))
                {
                    theDB = DBControl.GetConnection();
                }
                else
                {
                    theDB = DBControl.GetConnection(dbConnectionString);
                }

                strSql = "" +
                    " INSERT INTO tb_batch                                           " +
                    " ( materialno, batchno, validexpireddate, productiondate,       " +
                    "   vendorbatchno, lastgoodsreceiptdate, createtime,             " +
                    "   createman, updatetime, updateman )                           " +
                    "        SELECT DISTINCT materialno, batchno,                    " +
                    "                        '', '', '', '',                         " +
                    "'" + DateTime.Now.ToString("yyyyMMddHHmmssfff") + "', 'SYSTEM', " +
                    "                        '', ''                                  " +
                    "          FROM  tb_hirack                                       " +
                    "         WHERE  materialno <> ''                                " +
                    "           AND  materialno IS NOT NULL                          " +
                    "           AND  batchno <> ''                                   " +
                    "           AND  batchno IS NOT NULL                             " ;

                theDB.ExecuteUpdate(strSql);

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
                };
            }


        }

        public void updateAllBatchDataToHirack(String dbConnectionString)
        {
            DBConnection theDB = null;
            string strSql = "";

            try
            {
                if (string.IsNullOrEmpty(dbConnectionString))
                {
                    theDB = DBControl.GetConnection();
                }
                else
                {
                    theDB = DBControl.GetConnection(dbConnectionString);
                }

                strSql = "" +
                        " UPDATE c                                      " +
                        "    SET validexpireddate = b.validexpireddate  " +
                        "   FROM (SELECT *                              " +
                        "           FROM tb_hirack a                    " +
                        "          WHERE a.materialno IS NOT NULL       " +
                        "            AND a.batchno IS NOT NULL          " +
                        "            AND a.batchno <> ''                " +
                        "            AND a.materialno <> ''             " +
                        "         ) as c                                " +
                        "   JOIN tb_batch b                             " +
                        "     ON  c.batchno = b.batchno                 " +
                        "     AND c.materialno = b.materialno           ";

                theDB.ExecuteUpdate(strSql);

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
                };
            }
        }
//종료
        public void setBatchData(SqlDataReader reader)
        {
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Validexpireddate = Helper.PreventNull((string)reader["VALIDEXPIREDDATE"]);
            this.Productiondate = Helper.PreventNull((string)reader["PRODUCTIONDATE"]);
            this.Vendorbatchno = Helper.PreventNull((string)reader["VENDORBATCHNO"]);
            this.Lastgoodsreceiptdate = Helper.PreventNull((string)reader["LASTGOODSRECEIPTDATE"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  batchno                          batchno              " +
                    "        ,ISNULL(materialno          , '') materialno           " +
                    "        ,ISNULL(validexpireddate    , '') validexpireddate     " +
                    "        ,ISNULL(productiondate      , '') productiondate       " +
                    "        ,ISNULL(vendorbatchno       , '') vendorbatchno        " +
                    "        ,ISNULL(lastgoodsreceiptdate, '') lastgoodsreceiptdate " +
                    "        ,ISNULL(createtime          , '') createtime           " +
                    "        ,ISNULL(createman           , '') createman            " +
                    "        ,ISNULL(updatetime          , '') updatetime           " +
                    "        ,ISNULL(updateman           , '') updateman            " +
                    "   FROM tb_batch                                               "; 

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_batch               " +
                    "            ( batchno               " +
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
                    "            ( @BATCHNO              " +
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

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_batch                                     " +
                    "    SET  batchno              = @BATCHNO              " +
                    "        ,materialno           = @MATERIALNO           " +
                    "        ,validexpireddate     = @VALIDEXPIREDDATE     " +
                    "        ,productiondate       = @PRODUCTIONDATE       " +
                    "        ,vendorbatchno        = @VENDORBATCHNO        " +
                    "        ,lastgoodsreceiptdate = @LASTGOODSRECEIPTDATE " +
                    "        ,createtime           = @CREATETIME           " +
                    "        ,createman            = @CREATEMAN            " +
                    "        ,updatetime           = @UPDATETIME           " +
                    "        ,updateman            = @UPDATEMAN            " +
                    "  WHERE batchno               = @BATCHNO1             " +
                    "    AND materialno            = @MATERIALNO1          ";       

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_batch            " +
                    "  WHERE batchno    = @BATCHNO    " +
                    "    AND materialno = @MATERIALNO "; 

            return strSql;
        }

        public void insertBatchDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                BatchData data = new BatchData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE batchno    = '" + this.Batchno    + "' " +
                          "   AND materialno = '" + this.Materialno + "' ";

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
                    data.setBatchData(reader);
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
            ifValue += this.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Validexpireddate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Productiondate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Vendorbatchno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Lastgoodsreceiptdate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.updateman;


            return ifValue;
        }


        //************************************************************************************
        // SAP I/F Batch TEST를 위해서 항목을 추가함.
        // public get
        //***********************************************************************
        public string getTempSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  batchno                          batchno              " +
                    "        ,ISNULL(materialno          , '') materialno           " +
                    "        ,ISNULL(validexpireddate    , '') validexpireddate     " +
                    "        ,ISNULL(productiondate      , '') productiondate       " +
                    "        ,ISNULL(vendorbatchno       , '') vendorbatchno        " +
                    "        ,ISNULL(lastgoodsreceiptdate, '') lastgoodsreceiptdate " +
                    "        ,ISNULL(createtime          , '') createtime           " +
                    "        ,ISNULL(createman           , '') createman            " +
                    "        ,ISNULL(updatetime          , '') updatetime           " +
                    "        ,ISNULL(updateman           , '') updateman            " +
                    "   FROM tb_batchtemp                                               "; 

            return strSql;
        }

        //Hirack List 조회
        public void getBatchTempData(string batchno, string materialno)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getTempSelectSQL();
            strSql += "  WHERE batchno    = '" + batchno + "' " +
                      "    AND materialno    = '" + materialno + "' " +
                      "  ORDER BY batchno                                        ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setBatchData(reader);
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

    }
}
