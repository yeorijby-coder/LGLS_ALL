using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TransferDivisionData : ECPObject 
    {
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

        public TransferDivisionData()
        {
        }

        public int getTransferDivisionDataCnt(MovementSapIfData msData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transferdivision                                " +
                            "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "' " +
                            "    AND sapcommandid = '" + msData.Reqno + "'              ";

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

        public int getTransferDivisionDataCnt(TransferData tfData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transferdivision                                " +
                            "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "' " +
                            "    AND sapcommandid = '" + tfData.Commandid + "'              ";

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

        public int getTransferDivisionSapDataCnt(DBConnection theDB, string sapcommandid)
        {
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = " SELECT  COUNT(*)             CNT                          " +
                            "   FROM tb_transferdivision                                " +
                            "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "' " +
                            "    AND sapcommandid = '" + sapcommandid + "'              ";

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

        public int getTransferDivisionData(TransferData data)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "' " +
                      "    AND ecscommandid = '" + data.Commandid            + "' " +
                      "    AND palletid     = '" + data.Palletid             + "' " +
                      "    AND batchno      = '" + data.Batchno              + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setTransferDivisionData(reader);
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

        public List<TransferDivisionData> getTransferDivisionDataList(TransferData data)
        {
            List<TransferDivisionData> list = new List<TransferDivisionData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode    = '" + GlobalConstant.PLANT_CODE + "' " +
                      "    AND ecscommandid = '" + data.Commandid + "' " +
                      "    AND palletid     = '" + data.Palletid + "' " +
                      "    AND batchno      = '" + data.Batchno + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDivisionData tdData = new TransferDivisionData();
                    tdData.setTransferDivisionData(reader);

                    list.Add(tdData);
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

        public void insertTransferDivisionData(DBConnection theDB, Boolean bkFlag)
        {
            string strSql = "";
            strSql = getInsertSQL();

            PreparedSqlParameter para = null;
            try
            {
                para = theDB.GetPreparedSqlParameter();

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

                if (bkFlag)
                {
                    //DataBackup
                    this.insertTransferDivisionHistDataBackup(theDB, "Insert");
                }

                theDB.ExecuteUpdate(strSql, para);

            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public void deleteTransferDivisionData(DBConnection theDB, Boolean bkFlag)
        {
            string strSql = "";
            strSql = getDeleteSQL();

            PreparedSqlParameter para = null;
            try
            {
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                para.SetVarChar("SAPCOMMANDID", Helper.PreventNull(this.Sapcommandid));
                para.SetVarChar("ECSCOMMANDID", Helper.PreventNull(this.Ecscommandid));
                para.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                para.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertTransferDivisionHistDataBackup(theDB, "Delete");
                }

                theDB.ExecuteUpdate(strSql, para);
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback(); 
                throw se;
            }
        }

        public void setTransferDivisionData(SqlDataReader reader)
        {
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Sapcommandid = Helper.PreventNull((string)reader["SAPCOMMANDID"]);
            this.Ecscommandid = Helper.PreventNull((string)reader["ECSCOMMANDID"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Sapqty = Helper.PreventNull((string)reader["SAPQTY"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Ordertype = Helper.PreventNull((string)reader["ORDERTYPE"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  SELECT  plantcode            " +
                    "         ,sapcommandid         " +
                    "         ,ecscommandid         " +
                    "         ,palletid             " +
                    "         ,batchno              " +
                    "         ,materialno           " +
                    "         ,sapqty               " +
                    "         ,qty                  " +
                    "         ,ordertype            " +
                    "         ,createtime           " +
                    "         ,createman            " +
                    "    FROM tb_transferdivision   ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  INSERT INTO tb_transferdivision  " +
                    "             (plantcode            " +
                    "             ,sapcommandid         " +
                    "             ,ecscommandid         " +
                    "             ,palletid             " +
                    "             ,batchno              " +
                    "             ,materialno           " +
                    "             ,sapqty               " +
                    "             ,qty                  " +
                    "             ,ordertype            " +
                    "             ,createtime           " +
                    "             ,createman)           " +
                    "       VALUES                      " +
                    "             (@PLANTCODE           " +
                    "             ,@SAPCOMMANDID        " +
                    "             ,@ECSCOMMANDID        " +
                    "             ,@PALLETID            " +
                    "             ,@BATCHNO             " +
                    "             ,@MATERIALNO          " +
                    "             ,@SAPQTY              " +
                    "             ,@QTY                 " +
                    "             ,@ORDERTYPE           " +
                    "             ,@CREATETIME          " +
                    "             ,@CREATEMAN)          ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  DELETE tb_transferdivision           " +
                    "   WHERE plantcode     = @PLANTCODE    " +
                    "     AND sapcommandid  = @SAPCOMMANDID " +
                    "     AND ecscommandid  = @ECSCOMMANDID " +
                    "     AND palletid      = @PALLETID     " +
                    "     AND batchno       = @BATCHNO      ";

            return strSql;
        }

        public void insertTransferDivisionHistDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                TransferDivisionData data = new TransferDivisionData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE plantcode  = '" + GlobalConstant.PLANT_CODE + "' " +
                          "   AND sapcommandid  = '" + this.Sapcommandid      + "' " +
                          "   AND ecscommandid  = '" + this.Ecscommandid      + "' " +
                          "   AND palletid      = '" + this.Palletid          + "' " +
                          "   AND batchno       = '" + this.Batchno           + "' ";

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
                    data.setTransferDivisionData(reader);
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
            ifValue += this.Plantcode + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Sapcommandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Ecscommandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Sapqty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Ordertype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman;

            return ifValue;
        }
    }
}
