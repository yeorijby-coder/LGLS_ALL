using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Db.Entity;
using ECP.Util;
using ECP.Db;

namespace ECP.BizRule
{
    [Serializable]
    public class TransferDetail : ECPObject 
    {
        private string reqno = "";

        public string Reqno
        {
            get { return reqno; }
            set { reqno = value; }
        }
        private string ordertype = "";

        public string Ordertype
        {
            get { return ordertype; }
            set { ordertype = value; }
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
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string batchno = "";

        public string Batchno
        {
            get { return batchno; }
            set { batchno = value; }
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
        private string unit = "";

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
        }
        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string fromsublocationid = "";

        public string Fromsublocationid
        {
            get { return fromsublocationid; }
            set { fromsublocationid = value; }
        }
        private string tosublocationid = "";

        public string Tosublocationid
        {
            get { return tosublocationid; }
            set { tosublocationid = value; }
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

        private string dbConnectionString = "";
        public TransferDetail()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //반송 이력 조회
        public List<TransferDetail> getTransferDetailData(string dateString, string fromPos, string toPos, string status)
        {
            List<TransferDetail> list = new List<TransferDetail>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strDate = "%" + dateString.Substring(1, 7) + "%";
            string strSql = getTransferDetailSQL(strDate, fromPos, toPos, status);

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferDetail transferDetailData = new TransferDetail();
                    transferDetailData.setTransferDetailData(reader);

                    list.Add(transferDetailData);
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

        //dual c/v의 반송 확인
        public int getCheckOutTransferFromSTK1(string commandid)
        { 
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                            " +
                            "   FROM  TB_TRANSFERDETAIL                                          " +
                            "  WHERE  PATHID IN (SELECT PATHID                                   " +
                            "                      FROM TB_ITINERARY                             " +
                            "                     WHERE FROMDEVICEID = '"      + GlobalConstant.DUAL_MODE_STKSEM   + "'    " +
                            "                       AND FROMLOCATIONID LIKE '" + GlobalConstant.SUBSYSTEM_RACK     + "%' ) " +
                            "    AND  TRANSFERDETAILSTATUS = '" + EntityEnumData.TRANSFERDETAILSTATUS.Transferring.ToString() + "' " +
                            "    AND  COMMANDID <> '" + commandid + "'                                                     ";

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

        //dual c/v의 반송 확인
        public int getCheckOutTransferFromCNV2(string commandid)
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                            " +
                            "   FROM  TB_TRANSFERDETAIL                                          " +
                            "  WHERE  PATHID IN (SELECT PATHID                                   " +
                            "                      FROM TB_ITINERARY                             " +
                            "                     WHERE FROMLOCATIONID    LIKE '" + GlobalConstant.DUAL_MODE_FROM_CNVLOCATION + "%'   " +
                            "                       AND FROMSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_FROM_CNVPORT + "' )     " +
                            "    AND  COMMANDID <> '" + commandid + "'                                                             ";

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

        //dual c/v의 반송 확인
        public int getCheckInTransferFromCNV11(string commandid)
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                            " +
                            "   FROM  TB_TRANSFERDETAIL                                          " +
                            "  WHERE  PATHID IN (SELECT PATHID                                   " +
                            "                      FROM TB_ITINERARY                             " +
                            "                     WHERE FROMLOCATIONID    LIKE '" + GlobalConstant.DUAL_MODE_FROM_LOCATION + "%'   " +
                            "                       AND FROMSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_FROM_PORT     + "' )     " +
                            "    AND  COMMANDID <> '" + commandid + "'                                                             ";

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

        //dual c/v의 반송 확인
        public int getCheckOutTransferToCNV11()
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                            " +
                            "   FROM  TB_TRANSFERDETAIL                                          " +
                            "  WHERE  PATHID IN (SELECT PATHID                                   " +
                            "                      FROM TB_ITINERARY                             " +
                            "                     WHERE TODEVICEID      = '" + GlobalConstant.DUAL_MODE_TO_CNVSEM + "' " +
                            "                       AND TOLOCATIONID    = '" + GlobalConstant.DUAL_MODE_TO_LOCATION + "'   " +
                            "                       AND TOSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_TO_PORT + "' ) ";

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

        //dual c/v의 반송 확인
        public int getCheckInTransferFromCNV11()
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                            " +
                            "   FROM  TB_TRANSFERDETAIL                                          " +
                            "  WHERE  PATHID IN (SELECT PATHID                                   " +
                            "                      FROM TB_ITINERARY                             " +
                            "                     WHERE FROMDEVICEID      = '" + GlobalConstant.DUAL_MODE_FROM_CNVSEM + "' " +
                            "                       AND FROMLOCATIONID    = '" + GlobalConstant.DUAL_MODE_FROM_LOCATION + "'   " +
                            "                       AND FROMSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_FROM_PORT + "' ) ";

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

        //STK1번을 위한 dual c/v의 반송 확인
        public int getCheckInTransferToCNV2(string commandid)
        {
            int iCnt = 0;

            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = " SELECT  COUNT(*) AS CNT                                                        " +
                            "   FROM  TB_TRANSFERDETAIL                                                      " +
                            "  WHERE  PATHID IN (SELECT PATHID                                               " +
                            "                      FROM TB_ITINERARY                                         " +
                            "                     WHERE TODEVICEID      = '" + GlobalConstant.DUAL_MODE_STKSEM + "' " +
                            "                       AND TOLOCATIONID    = '" + GlobalConstant.DUAL_MODE_TO_CNVLOCATION + "' " +
                            "                       AND TOSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_TO_CNVPORT + "' " +
                            "                       AND  COMMANDID <> '" + commandid + "' " +
                            "                     UNION ALL                                                  " +
                            "                    SELECT PATHID                                               " +
                            "                      FROM TB_ITINERARY                                         " +
                            "                     WHERE TODEVICEID      = '" + GlobalConstant.DUAL_MODE_TO_RGVSEM + "' " +
                            "                       AND TOLOCATIONID    = '" + GlobalConstant.DUAL_MODE_TO_RGVLOCATION + "' " +
                            "                       AND TOSUBLOCATIONID = '" + GlobalConstant.DUAL_MODE_TO_RGVPORT + "' " +
                            "                       AND TRANSFERDETAILSTATUS = '" + EntityEnumData.TRANSFERDETAILSTATUS.Transferring.ToString() + "' " +
                            "                       AND  COMMANDID <> '" + commandid + "' " +
                            "                   )                                                              " ;
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

        public void setTransferDetailData(SqlDataReader reader)
        {
            this.Reqno = Helper.PreventNull((string)reader["REQNO"]);
            this.Ordertype = Helper.PreventNull((string)reader["ORDERTYPE"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandtype = Helper.PreventNull((string)reader["COMMANDTYPE"]);
            this.Movementtype = Helper.PreventNull((string)reader["MOVEMENTTYPE"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Sapqty = Helper.PreventNull((string)reader["SAPQTY"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Fromposition = Helper.PreventNull((string)reader["FROMPOSITION"]);
            this.Toposition = Helper.PreventNull((string)reader["TOPOSITION"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Fromsublocationid = Helper.PreventNull((string)reader["FROMSUBLOCATIONID"]);
            this.Tosublocationid = Helper.PreventNull((string)reader["TOSUBLOCATIONID"]);
            this.Transferstatus = Helper.PreventNull((string)reader["TRANSFERSTATUS"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
        }

        public string getTransferDetailSQL(string strDate, string fromPos, string toPos, string status)
        {
            string strSql = "";

            strSql += "" +
                    " SELECT  DISTINCT reqno                                                 " +
                    "        ,ordertype                                                      " +
                    "        ,commandid                                                      " +
                    "        ,commandtype                                                    " +
                    "        ,movementtype                                                   " +
                    "        ,palletid                                                       " +
                    "        ,materialno                                                     " +
                    "        ,batchno                                                        " +
                    "        ,sapqty                                                         " +
                    "        ,qty                                                            " +
                    "        ,unit                                                           " +
                    "        ,fromposition                                                   " +
                    "        ,toposition                                                     " +
                    "        ,fromdeviceid                                                   " +
                    "        ,todeviceid                                                     " +
                    "        ,fromsublocationid                                              " +
                    "        ,tosublocationid                                                " +
                    "        ,transferstatus                                                 " +
                    "        ,createtime                                                     " +
                    "   FROM (                                                               " +
                    " 				SELECT  ISNULL(B.sapcommandid, A.commandid) AS reqno           " +
                    " 				       ,ISNULL(B.ordertype,'') AS ordertype                    " +
                    " 				       ,A.commandid As commandid                               " +
                    " 				       ,A.commandtype As commandtype                           " +
                    " 				       ,A.movementtype As movementtype                         " +
                    " 				       ,A.palletid As palletid                                 " +
                    " 				       ,ISNULL(A.materialno,'') AS materialno                  " +
                    " 				       ,ISNULL(A.batchno,'') AS batchno                        " +
                    " 				       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty           " +
                    " 				       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                 " +
                    " 				       ,ISNULL(A.unit,'') AS unit                              " +
                    " 				       ,(SELECT value                                          " +
                    " 				           FROM TB_UDPROPERTY                                  " +
                    " 				          WHERE entityid = A.fromdeviceid                      " +
                    " 				            AND udpropertycd = 'START_DEVICE_01'               " +
                    " 				         ) fromposition                                        " +
                    " 				       ,A.fromdeviceid                                         " +
                    " 				       ,A.fromsublocationid As fromsublocationid               " +
                    " 				       ,(SELECT value                                          " +
                    " 				           FROM TB_UDPROPERTY                                  " +
                    " 				          WHERE entityid = A.todeviceid                        " +
                    " 				            AND udpropertycd = 'END_DEVICE_01'                 " +
                    " 				         ) toposition                                          " +
                    " 				       ,A.todeviceid                                           " +
                    " 				       ,A.tosublocationid As tosublocationid                   " +
                    " 				       ,A.transferstatus AS transferstatus                     " +
                    " 				       ,A.createtime AS createtime                             " +
                    " 				FROM   TB_TRANSFER AS A                                        " +
                    " 				       LEFT OUTER JOIN                                         " +
                    " 				       TB_TRANSFERDIVISION AS B                                " +
                    " 				       ON A.commandid = B.ecscommandid                         " +
                    "              WHERE   A.commandid LIKE '" + strDate + "'                      " +
                    " 				UNION ALL                                                      " +
                    " 				SELECT  ISNULL(B.sapcommandid, A.commandid) AS reqno           " +
                    " 				       ,ISNULL(B.ordertype,'') AS ordertype                    " +
                    " 				       ,A.commandid As commandid                               " +
                    " 				       ,A.commandtype As commandtype                           " +
                    " 				       ,A.movementtype As movementtype                         " +
                    " 				       ,A.palletid As palletid                                 " +
                    " 				       ,ISNULL(A.materialno,'') AS materialno                  " +
                    " 				       ,ISNULL(A.batchno,'') AS batchno                        " +
                    " 				       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty           " +
                    " 				       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                 " +
                    " 				       ,ISNULL(A.unit,'') AS unit                              " +
                    " 				       ,(SELECT value                                          " +
                    " 				           FROM TB_UDPROPERTY                                  " +
                    " 				          WHERE entityid = A.fromdeviceid                      " +
                    " 				            AND udpropertycd = 'START_DEVICE_01'               " +
                    " 				         ) fromposition                                        " +
                    " 				       ,A.fromdeviceid                                         " +
                    " 				       ,A.fromsublocationid As fromsublocationid               " +
                    " 				       ,(SELECT value                                          " +
                    " 				           FROM TB_UDPROPERTY                                  " +
                    " 				          WHERE entityid = A.todeviceid                        " +
                    " 				            AND udpropertycd = 'END_DEVICE_01'                 " +
                    " 				         ) toposition                                          " +
                    " 				       ,A.todeviceid                                           " +
                    " 				       ,A.tosublocationid As fromsublocationid                 " +
                    " 				       ,'Complete' AS transferstatus                           " +
                    " 				       ,A.createtime AS createtime                             " +
                    " 				FROM   TB_TRANSFERHIST AS A                                    " +
                    " 				       LEFT OUTER JOIN                                         " +
                    " 				       TB_TRANSFERDIVISIONHIST AS B                            " +
                    " 				       ON A.commandid = B.ecscommandid                         " +
                    " 				WHERE A.executetime <> ''                                      " +
                    " 				  AND A.executetime IS NOT NULL                                " +
                    " 				  AND A.completetime <> ''                                     " +
                    " 				  AND A.completetime IS NOT NULL                               " +
                    " 				  AND A.commandid LIKE '" + strDate + "'                       " +
                    "        ) as inner_1                                                          " +
                    "  WHERE fromposition = '" + fromPos + "'                                      " +
                    "    AND toposition = '" + toPos + "'                                          " +
                    "    AND transferstatus = '" + status + "'                                     " +
                    " ORDER BY createtime, reqno, commandid                                        ";

            return strSql;
        }
    }
}
