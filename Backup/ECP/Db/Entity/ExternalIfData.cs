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
    public class ExternalIfData : ECPObject 
    {
        private string externaliftype = "";

        public string Externaliftype
        {
            get { return externaliftype; }
            set { externaliftype = value; }
        }
        private string createtime = ""; 

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string ifvalue = "";

        public string Ifvalue
        {
            get { return ifvalue; }
            set { ifvalue = value; }
        }
        private string transstarttime = "";

        public string Transstarttime
        {
            get { return transstarttime; }
            set { transstarttime = value; }
        }
        private string transcompletetime = "";

        public string Transcompletetime
        {
            get { return transcompletetime; }
            set { transcompletetime = value; }
        }
        private string ifcnt = "";

        public string Ifcnt
        {
            get { return ifcnt; }
            set { ifcnt = value; }
        }

        //반송지시 정보 List 조회
        public List<ExternalIfData> getNotCompleteWmsOrderIfDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'O'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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

        //반송 재작업 지시 정보 List 조회
        public List<ExternalIfData> getNotCompleteWmsReorderIfDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'R'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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

        //Mode Change 정보 List 조회
        public List<ExternalIfData> getNotCompleteWmsModeChangeDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'M'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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

        //장비 상태 정보 List 조회
        public List<ExternalIfData> getNotCompleteStatusDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'S'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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

        //장비 Error 정보 List 조회
        public List<ExternalIfData> getNotCompleteErrorDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'E'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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

        //완료 보고 정보 List 조회
        public List<ExternalIfData> getNotCompleteTransferDataList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = 'F'                                  " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData data = new ExternalIfData();

                    data.setExternalIfData(reader);

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


        //TB_EXTERNALIF에서 TB_TRANSFER로 등록되지 않은 반송지시 정보 List 조회
        public List<ExternalIfData> getSAPNotCompleteOrderList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = '01'                                 " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";
            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData externalIfData = new ExternalIfData();

                    externalIfData.setExternalIfData(reader);

                    list.Add(externalIfData);
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

        //TB_EXTERNALIF에서 SAP에 I/F 되지 않은 Order Result List 조회하기
        public List<ExternalIfData> getNotReceiveOrderResultList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = '02'                                 " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData externalIfData = new ExternalIfData();

                    externalIfData.setExternalIfData(reader);

                    list.Add(externalIfData);
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

        //TB_EXTERNALIF에서 TB_TRANSFER로 등록되지 않은 반송지시 정보 List 조회
        public List<ExternalIfData> getNotReceiveHirackList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = '03'                                 " +
                      "    AND transcompletetime = '" + GlobalConstant.SET_MAXTIME + "' " +
                      "    ORDER BY createtime                                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData externalIfData = new ExternalIfData();

                    externalIfData.setExternalIfData(reader);

                    list.Add(externalIfData);
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

        //TB_EXTERNALIF에서 SAP 반송 지시 정보 List 조회
        public List<ExternalIfData> getExternalIfDataTransferList()
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype    = '01' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData externalIfData = new ExternalIfData();

                    externalIfData.setExternalIfData(reader);

                    list.Add(externalIfData);
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

        //TB_EXTERNALIF에서 SAP 반송 지시 정보 List 조회
        public List<ExternalIfData> getExternalIfDataTransferList(string fromdate, string todate)
        {
            List<ExternalIfData> list = new List<ExternalIfData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE createtime   BETWEEN '" + fromdate + "'  " +
                      "    AND                      '" + todate   + "'  " +
                      "  ORDER BY createtime                                           ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ExternalIfData externalIfData = new ExternalIfData();

                    externalIfData.setExternalIfData(reader);

                    list.Add(externalIfData);
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

        //TB_EXTERNALIF에서 SAP 반송 지시 정보 List 조회
        public int getExternalIfDataCount(string externaliftype, string ifvalue)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            int iRow = 0;
            
            string strSql = getSelectSQL();
            strSql += "  WHERE externaliftype = '" + externaliftype + "' " +
                      "    AND ifvalue        = '" + ifvalue        + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    iRow++;
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

            return iRow;
        }

        //TB_EXTERNALIF에 TB_TRANSFER로 등록한 정보를 I/F 완료 처리함.
        public void insertExternalIfData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;

            try
            {
                string strSql = getInsertSQL();

                theDB = DBControl.GetConnection();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_EXTERNALIF 정보  insert
                pss.SetVarChar("EXTERNALIFTYPE", Helper.PreventNull(this.Externaliftype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));

                theDB.ExecuteUpdate(strSql, pss);
                theDB.Commit();
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void insertExternalIfData(string externaliftype, Object value, int iTag, string movementtype, string movementflag, string sapCommandid)
        {
            try
            {
                if (string.IsNullOrEmpty(sapCommandid))
                {
                    insertExternalIfData(externaliftype, value, iTag, movementtype, movementflag);
                } else 
                {
                    TransferData tfTempData = (TransferData)value;
                    tfTempData.Commandid = sapCommandid;

                    insertExternalIfData(externaliftype, tfTempData, iTag, movementtype, movementflag);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void insertExternalIfData(string externaliftype, Object value, int iTag, string movementtype, string movementflag)
        {
            if (externaliftype == "02")
            {
                ExternalIfData externalIfData = new ExternalIfData();

                externalIfData.Externaliftype = externaliftype;
                externalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

                if (iTag == 0)
                {
                    TransferData tfData = (TransferData)value;
                    externalIfData.Ifvalue = makeIfValueReceiveOrderResult(tfData, movementtype, movementflag);
                }
                else
                {
                    MovementSapIfData moveData = (MovementSapIfData)value;
                    externalIfData.Ifvalue = makeIfValueReceiveOrderResult(moveData, movementtype, movementflag);
                }

                externalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;

                try
                {
                    externalIfData.insertExternalIfData();
                }
                catch (SqlException se)
                {
                    throw se;
                }
            }
            else
            {
                try
                {
                    insertExternalIfData(externaliftype, value);
                }
                catch (SqlException se)
                {
                    throw se;
                }
            }

        }

        public void insertExternalIfData(string externaliftype, Object value)
        {
            ExternalIfData externalIfData = new ExternalIfData();

            externalIfData.Externaliftype = externaliftype;
            externalIfData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");

            if (externaliftype == "01")
            {
                MovementSapIfData moveData = (MovementSapIfData)value;
                externalIfData.Ifvalue = makeIfValueSendOrder(moveData);
            }
            else if (externaliftype == "03")
            {
                HirackData hData = (HirackData)value;
                externalIfData.Ifvalue = makeIfValueReceiveHiRack(hData);
            }
            else if (externaliftype == "04")
            {
                HirackData hData = (HirackData)value;
                externalIfData.Ifvalue = makeIfValueSendHiRack(hData);
            }
            else if (externaliftype == "05")
            {
                BatchData batchData = (BatchData)value;
                externalIfData.Ifvalue = makeIfValueSendBatch(batchData);
            }
            else if (externaliftype == "06")
            {
                MaterialData materialData = (MaterialData)value;
                externalIfData.Ifvalue = makeIfValueSendMaterial(materialData);
            }
            else
            {
            }

            externalIfData.Transcompletetime = GlobalConstant.SET_MAXTIME;

            try
            {
                externalIfData.insertExternalIfData();
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        //TB_EXTERNALIF에 TB_TRANSFER로 등록한 정보를 I/F 완료 처리함.
        public void updateExternalIfData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter pss = null;

            try
            {
                string strSql = getUpdateSQL();

                theDB = DBControl.GetConnection();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_EXTERNALIF 정보  update
                pss.SetVarChar("EXTERNALIFTYPE", Helper.PreventNull(this.Externaliftype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE", Helper.PreventNull(this.Ifvalue));
                pss.SetVarChar("TRANSSTARTTIME", Helper.PreventNull(this.Transstarttime));
                pss.SetVarChar("TRANSCOMPLETETIME", Helper.PreventNull(this.Transcompletetime));
                pss.SetVarChar("IFCNT", Helper.PreventNull(this.Ifcnt));
                pss.SetVarChar("EXTERNALIFTYPE1", Helper.PreventNull(this.Externaliftype));
                pss.SetVarChar("CREATETIME1", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("IFVALUE1", Helper.PreventNull(this.Ifvalue));

                theDB.ExecuteUpdate(strSql, pss);
                theDB.Commit();
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
        }

        public void setExternalIfData(SqlDataReader reader)
        {
            this.Externaliftype = Helper.PreventNull((string)reader["EXTERNALIFTYPE"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Ifvalue = Helper.PreventNull((string)reader["IFVALUE"]);
            this.Transstarttime = Helper.PreventNull((string)reader["TRANSSTARTTIME"]);
            this.Transcompletetime = Helper.PreventNull((string)reader["TRANSCOMPLETETIME"]);
            this.Ifcnt = Helper.PreventNull((string)reader["IFCNT"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  externaliftype               externaliftype    " +
                    "        ,ISNULL(createtime       ,'') createtime        " +
                    "        ,ISNULL(ifvalue          ,'') ifvalue           " +
                    "        ,ISNULL(transstarttime   ,'') transstarttime    " +
                    "        ,ISNULL(transcompletetime,'') transcompletetime " +
                    "        ,ISNULL(ifcnt            ,'') ifcnt             " +
                    "   FROM tb_externalif                                   ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  INSERT INTO tb_externalif       " +
                    "             (externaliftype      " +
                    "             ,createtime          " +
                    "             ,ifvalue             " +
                    "             ,transstarttime      " +
                    "             ,transcompletetime   " +
                    "             ,ifcnt)              " +
                    "       VALUES                     " +
                    "             (@EXTERNALIFTYPE     " +
                    "             ,@CREATETIME         " +
                    "             ,@IFVALUE            " +
                    "             ,@TRANSSTARTTIME     " +
                    "             ,@TRANSCOMPLETETIME  " +
                    "             ,@IFCNT)             ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE tb_externalif                             " +
                    "    SET  externaliftype     = @EXTERNALIFTYPE     " +
                    "        ,createtime         = @CREATETIME         " +
                    "        ,ifvalue            = @IFVALUE            " +
                    "        ,transstarttime     = @TRANSSTARTTIME     " +
                    "        ,transcompletetime  = @TRANSCOMPLETETIME  " +
                    "        ,ifcnt              = @IFCNT              " +
                    "  WHERE  externaliftype     = @EXTERNALIFTYPE1    " +
                    "    AND  createtime         = @CREATETIME1        " +
                    "    AND  ifvalue            = @IFVALUE1           ";

            return strSql;
        }

        public string makeIfValueSendOrder(MovementSapIfData moveData)
        {
            string ifValue = "";
            ifValue += moveData.Reqno + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Crdat + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Crtim + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Matnr + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Charg + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Pallet + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Menge + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Frwerks + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Frlgort + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Towerks + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Tolgort + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Rqnam + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Sampleflag;

            return ifValue;
        }

        public string makeIfValueReceiveOrderResult(MovementSapIfData moveData, string movementtype, string movementflag)
        {
            string ifValue = "";
            ifValue +=  movementtype + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Reqno + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Matnr + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Charg + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Pallet + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Menge + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Frwerks + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Frlgort + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Towerks + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Tolgort + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Rqnam + GlobalConstant.MOVEMENT_DELIMITER +
                        moveData.Sampleflag + GlobalConstant.MOVEMENT_DELIMITER +
                        movementflag;

            return ifValue;
        }

        public string makeIfValueReceiveOrderResult(TransferData tfData, string movementtype, string movementflag)
        {
            string ifValue = "";
            ifValue +=  movementtype + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Commandid + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Fromplant + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Fromposition + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Fromplant + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Toposition + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        tfData.Sampleflag + GlobalConstant.MOVEMENT_DELIMITER +
                        movementflag;

            return ifValue;
        }

        public string makeIfValueReceiveHiRack(HirackData hData)
        {
            string ifValue = "";
            string createdate = "";
            string palletid = "";
            string cellstatus = "";

            if (string.IsNullOrEmpty(hData.Createtime))
            {
                createdate = "";
            }
            else
            {
                //Empty로 상태 변경시에는 Pallet id를 Space 처리함.
                if (hData.Cellstatus == EntityEnumData.HIRACKSTATUS.Empty.ToString())
                {
                    createdate = "";
                }
                else
                {
                    createdate = hData.Createtime.Substring(0, 8);
                }
            }

            //Empty로 상태 변경시에는 Pallet id를 Space 처리함.
            //Cell Status가 Empty일 경우에 Blank로 보냄
            if (hData.Cellstatus == EntityEnumData.HIRACKSTATUS.Empty.ToString())
            {
                cellstatus = EntityEnumData.HIRACKSTATUS.Blank.ToString();
                palletid = " ";
            }
            else
            {
                cellstatus = hData.Cellstatus;
                palletid = hData.Palletid;
            }

            //CELL STATUS = 'Empty'일경우 'Blank'로 변경함.
            ifValue += GlobalConstant.CLIENT_CODE   + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Cellid.Substring(0, 2) + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Cellid.Substring(2, 2) + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Cellid.Substring(4, 2) + GlobalConstant.MOVEMENT_DELIMITER +
                       palletid + GlobalConstant.MOVEMENT_DELIMITER +
                       cellstatus + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Fromplant + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Fromposition + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                       createdate + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Validexpireddate;

            return ifValue;
        }

        public string makeIfValueSendHiRack(HirackData hData)
        {
            string ifValue = "";
            ifValue += " ";
            ifValue += hData.Cellid + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                       hData.Unit ;

            return ifValue;
        }

        public string makeIfValueSendBatch(BatchData batchData)
        {
            string ifValue = "";
            ifValue += " ";
            ifValue += batchData.Batchno + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Validexpireddate + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Productiondate + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Vendorbatchno + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Lastgoodsreceiptdate + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                       batchData.Updateman;

            return ifValue;
        }

        public string makeIfValueSendMaterial(MaterialData materialData)
        {
            string ifValue = "";

            ifValue += " ";
            ifValue += materialData.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Materialname + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Materialtype + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Tempconditionflg + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Valueconfig + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Tempmin + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Tempmax + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                       materialData.Updateman;

            return ifValue;
        }
    }
}
