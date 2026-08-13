using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;
using ECP.BizRule;

namespace ECP.Db.Entity
{
    [Serializable]
    public class HirackData:ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string cellid = "";

        public string Cellid
        {
            get { return cellid; }
            set { cellid = value; }
        }
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string hiracktransferstatus = "";

        public string Hiracktransferstatus
        {
            get { return hiracktransferstatus; }
            set { hiracktransferstatus = value; }
        }
        private string cellstatus = "";

        public string Cellstatus
        {
            get { return cellstatus; }
            set { cellstatus = value; }
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
        private string fromplant = "";

        public string Fromplant
        {
            get { return fromplant; }
            set { fromplant = value; }
        }
        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string validexpireddate = "";

        public string Validexpireddate
        {
            get { return validexpireddate; }
            set { validexpireddate = value; }
        }
        private string sappalletid = "";

        public string Sappalletid
        {
            get { return sappalletid; }
            set { sappalletid = value; }
        }
        private string sapbatchno = "";

        public string Sapbatchno
        {
            get { return sapbatchno; }
            set { sapbatchno = value; }
        }
        private string sapitemno = "";

        public string Sapitemno
        {
            get { return sapitemno; }
            set { sapitemno = value; }
        }
        private string sapqty = "";

        public string Sapqty
        {
            get { return sapqty; }
            set { sapqty = value; }
        }
        private string sapunit = "";

        public string Sapunit
        {
            get { return sapunit; }
            set { sapunit = value; }
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

        public HirackData()
        {
        }

        //출고 Pallet Data 조회
        public int getHirackDataCountWithPallet(string palletid)
        {
            DBConnection theDB = null;
            int iCnt = 0;
            try
            {
                theDB = DBControl.GetConnection();
                iCnt = getHirackDataCountWithPallet(palletid, theDB);
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

        //출고 Pallet Data 조회
        public int getHirackDataCountWithPallet(string palletid, DBConnection theDB)
        {
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = getSelectRowSQL();
            strSql += "  WHERE palletid   = '" + palletid + "'                                     ";

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
                setHirackData(reader);
                iCnt = 1;
            }

            if (reader != null) reader.Close();

            return iCnt;
        }

        //출고 Pallet Data 조회
        public int getHirackDataCellCount()
        {
            DBConnection theDB = null;
            int iCnt = 0;
            string cellstatus = "%";

            try
            {
                theDB = DBControl.GetConnection();
                iCnt = getHirackDataCountByCellStatus(cellstatus, theDB);
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

        //출고 Pallet Data 조회
        public int getHirackDataCountByCellStatus(string cellstatus)
        {
            DBConnection theDB = null;
            int iCnt = 0;
            try
            {
                theDB = DBControl.GetConnection();
                iCnt = getHirackDataCountByCellStatus(cellstatus, theDB);
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

        //출고 Pallet Data 조회
        public int getHirackDataCountByCellStatus(string cellstatus, DBConnection theDB)
        {
            SqlDataReader reader = null;
            int iCnt = 0;

            string strSql = getSelectCountSQL();
            strSql += "  WHERE cellstatus like '" + cellstatus + "'              ";

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
                iCnt = (int) reader["CNT"];
            }

            if (reader != null) reader.Close();

            return iCnt;
        }

        //출고 Pallet Data 조회
        public int getHirackDataCountByRackAndCellStatus(string bankno, string cellstatus)
        {
            DBConnection theDB = null;
            string banknoReplace = "";
            int iCnt = 0;

            try
            {
                banknoReplace = bankno.Replace(GlobalConstant.SUBSYSTEM_RACK, "0");

                if(banknoReplace.Length > 2) {
                    banknoReplace = banknoReplace.Substring(1);
                }

                theDB = DBControl.GetConnection();
                iCnt = getHirackDataCountByRackAndCellStatus(banknoReplace, cellstatus, theDB);
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

        //출고 Pallet Data 조회
        public int getHirackDataCountByRackAndCellStatus(string bankno, string cellstatus, DBConnection theDB)
        {
            SqlDataReader reader = null;
            int iCnt = 0;
            string banknoReplace = "";
            banknoReplace = bankno.Replace(GlobalConstant.SUBSYSTEM_RACK, "0");
            if (banknoReplace.Length > 2)
            {
                banknoReplace = banknoReplace.Substring(1);
            }

            string strSql = getSelectCountSQL();
            strSql += "  WHERE SUBSTRING(cellid,1,2) like '" + banknoReplace + "'  " +
                      "    AND cellstatus like            '"   + cellstatus                + "'  ";

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
                iCnt = (int)reader["CNT"];
            }

            if (reader != null) reader.Close();

            return iCnt;
        }

        //Hirack List 조회
        public void getHirackData(string cellid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE cellid    = '" + cellid                    + "' " +
                      "  ORDER BY cellid                                         ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setHirackData(reader);
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

        //Hirack List 조회
        public Dictionary<string, HirackData> getHirackDataList()
        {
            Dictionary<string, HirackData> list = new Dictionary<string, HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY cellid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData.Cellid, hData);
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

        //Hirack List 조회
        public Dictionary<string, HirackData> getHirackDataListWithBatchMaterial(string materialno, string batchno)
        {
            Dictionary<string, HirackData> list = new Dictionary<string, HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE materialno LIKE  '" + materialno + "' " +
                      "    AND batchno    LIKE  '" + batchno    + "' " +
                      "  ORDER BY cellid                               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData.Cellid, hData);
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

        //Hirack List 조회
        public List<HirackData> getHirackCellStatusFullDataList()
        {
            List<HirackData> list = new List<HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE cellstatus = 'Full' " +
                      "  ORDER BY cellid           ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData);
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

        //Hirack List 조회
        public List<HirackData> getHirackDataListByCellStatus(string cellstatus)
        {
            List<HirackData> list = new List<HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE cellstatus = '" + cellstatus + "' " +
                      "  ORDER BY cellid           ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData);
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
        public List<HirackData> getHirackDataMeterialBatchList(string materialno, string batchno)
        {
            List<HirackData> list = new List<HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE materialno   LIKE '" + materialno             + "'  " +
                      "    AND batchno      LIKE '" + batchno                + "'  " +
                      "  ORDER BY materialno                                       ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData);
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
        public List<HirackData> getHirackDataCellList(string bankno, string bayno, string levelno)
        {
            List<HirackData> list = new List<HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE SUBSTRING(cellid,1,2) LIKE '" + bankno                 + "' " +
                      "    AND SUBSTRING(cellid,3,2) LIKE '" + bayno                  + "' " +
                      "    AND SUBSTRING(cellid,5,2) LIKE '" + levelno                + "' " +
                      " ORDER BY cellid                                                    ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData);
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

        //적치 위치 온도 조건에 따른 조회(08%07 Cell은 08%06Cell과 병합됨.)
        public void getHirackIngoLocationConditionData(PositionData pd, CodeMasterData codeMasterData, CodeMasterData cmData)
        {
            string strValue = "";
            string bankno = "0";

            strValue = pd.Locationid;
            bankno += strValue.Substring(strValue.Length - 1);

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectRowSQL();

            strSql += "  WHERE cellid LIKE '" + bankno + "%'                                        " +
                      "    AND cellid NOT LIKE '08%06'                                              " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Empty.ToString() + "'  " +
                      "    AND SUBSTRING(cellid,5,2) " +  codeMasterData.Attribute1 + " '" + codeMasterData.Attribute2 + "'         " +
                      "    AND SUBSTRING(cellid,3,2) BETWEEN '" + cmData.Attribute1 + "'            " +
                      "                                  AND '" + cmData.Attribute2 + "'            " +
                      "  ORDER BY cellid                                                            ";
            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
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

        //일반 자재 적치 위치 조회(08%07 Cell은 08%06Cell과 병합됨.)
        public void getHirackIngoData(PositionData pd, CodeMasterData cmData)
        {
            string strValue = "";
            string bankno = "0";

            strValue = pd.Locationid;
            bankno += strValue.Substring(strValue.Length - 1);

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectRowSQL();
            strSql += "  WHERE cellid LIKE '" + bankno + "%'                                        " +
                      "    AND cellid NOT LIKE '08%06'                                              " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Empty.ToString() + "'  " +
                      "    AND SUBSTRING(cellid,3,2) BETWEEN '" + cmData.Attribute1 + "'            " +
                      "                                  AND '" + cmData.Attribute2 + "'            " +
                      "  ORDER BY cellid                                                            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
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

        //Big Cell 자재 적치 위치 조회(우선 순위 그룹 포함)
        public void getHirackIngoLargeData(CodeMasterData cmData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectRowSQL();
            strSql += "  WHERE cellid     LIKE '08%06'                                              " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Empty.ToString() + "'  " +
                      "    AND SUBSTRING(cellid,3,2) BETWEEN '" + cmData.Attribute1 + "'            " +
                      "                                  AND '" + cmData.Attribute2 + "'            " +
                      "  ORDER BY cellid                                                            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
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


        //출고 자재 List 조회
        public List<HirackData> getHirackOutgoDataList(MovementSapIfData moveData)
        {
            List<HirackData> list = new List<HirackData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE batchno      = '" + moveData.Charg            + "'                  " +
                      "    AND materialno   = '" + moveData.Matnr            + "'                  " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Full.ToString() + "'  " +
                      "    AND fromplant    = '" + moveData.Frwerks + "'                           " +
                      "  ORDER BY createtime desc, CAST(qty as FLOAT) asc                          ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackData hData = new HirackData();
                    hData.setHirackData(reader);

                    list.Add(hData);
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

        //출고 Pallet Data 조회
        public void getHirackOutgoPalletData(string palletid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectRowSQL();
            strSql += "  WHERE palletid   = '" + palletid + "'                                     " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Full.ToString() + "'  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
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

        //출고 Pallet Data 조회
        public void getHirackOutgoPalletData(string palletid, string fromplant)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectRowSQL();
            strSql += "  WHERE palletid   = '" + palletid + "'                                     " +
                      "    AND fromplant   = '" + fromplant + "'                                   " +
                      "    AND cellstatus = '" + EntityEnumData.HIRACKSTATUS.Full.ToString() + "'  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
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

        public void setHirackData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Cellid = Helper.PreventNull((string)reader["CELLID"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Hiracktransferstatus = Helper.PreventNull((string)reader["HIRACKTRANSFERSTATUS"]);
            this.Cellstatus = Helper.PreventNull((string)reader["CELLSTATUS"]);
            //this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            //this.Fromplant = Helper.PreventNull((string)reader["FROMPLANT"]);
            //this.Fromposition = Helper.PreventNull((string)reader["FROMPOSITION"]);
            this.Validexpireddate = Helper.PreventNull((string)reader["VALIDEXPIREDDATE"]);
            //this.Sappalletid = Helper.PreventNull((string)reader["SAPPALLETID"]);
            //this.Sapbatchno = Helper.PreventNull((string)reader["SAPBATCHNO"]);
            //this.Sapitemno = Helper.PreventNull((string)reader["SAPITEMNO"]);
            //this.Sapqty = Helper.PreventNull((string)reader["SAPQTY"]);
            //this.Sapunit = Helper.PreventNull((string)reader["SAPUNIT"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }

        public void updateHirackIngoStatus(DBConnection theDB, TransferData trData)
        {
            string strSql = "";
            string cellid = "";

            cellid = trData.Tosublocationid;
            
            SqlDataReader reader = null;
            strSql = getSelectRowSQL();
            strSql += "  WHERE cellid = '" + cellid + "'                        ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
                }

                if (reader != null) reader.Close();

                this.Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Wait.ToString();
                this.Cellstatus = EntityEnumData.HIRACKSTATUS.Reserved.ToString();

                this.Palletid = trData.Palletidname;
                //this.Batchno = trData.Batchno;
                this.Materialno = trData.Materialno;
                this.Qty = trData.Qty;
                this.Unit = trData.Unit;

                //this.Fromplant = trData.Fromplant;
                //this.Fromposition = trData.Fromposition;
                
                this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                this.Updateman = trData.Createman;

                this.updateHirackData(theDB, true, true);
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void updateHirackOutgoStatus(DBConnection theDB, TransferData trData)
        {
            string strSql = "";
            string cellid = "";

            cellid = trData.Fromsublocationid;
            
            SqlDataReader reader = null;
            strSql = getSelectRowSQL();
            strSql += "  WHERE cellid = '" + cellid + "'                        ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setHirackData(reader);
                }

                if (reader != null) reader.Close();

                this.Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Wait.ToString(); 
                this.Cellstatus = EntityEnumData.HIRACKSTATUS.Reserved.ToString();

                this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                this.Updateman = trData.Createman;

                this.updateHirackData(theDB, true, true);
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void updateHirackCancelStatus(DBConnection theDB, TransferData trData)
        {
            string strFromSql = "";
            string strToSql = "";
            string fromcellid = "";
            string tocellid = "";

            Boolean fromFlag = false;
            Boolean toFlag = false;

            fromcellid = trData.Fromsublocationid;
            tocellid = trData.Tosublocationid;

            SqlDataReader fromreader = null;
            SqlDataReader toreader = null;

            strFromSql = getSelectRowSQL();
            strFromSql += "  WHERE cellid = '" + fromcellid + "'                        ";

            strToSql = getSelectRowSQL();
            strToSql += "  WHERE cellid = '" + tocellid + "'                        ";

            try
            {
                //From Location 원복
                fromreader = theDB.ExecuteQuery(strFromSql);

                if (fromreader.Read())
                {
                    setHirackData(fromreader);
                    fromFlag = true;
                }

                if (fromreader != null) fromreader.Close();

                if (fromFlag)
                {
                    this.Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Complete.ToString();
                    this.Cellstatus = EntityEnumData.HIRACKSTATUS.Full.ToString();

                    this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    this.Updateman = trData.Createman;

                    this.updateHirackData(theDB, true, true);
                }

                //To Location 원복
                toreader = theDB.ExecuteQuery(strToSql);

                if (toreader.Read())
                {
                    setHirackData(toreader);
                    toFlag = true;
                }

                if (toreader != null) toreader.Close();

                if (toFlag)
                {
                    this.Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Complete.ToString();
                    this.Cellstatus = EntityEnumData.HIRACKSTATUS.Empty.ToString();

                    this.Palletid = "";
                    this.Batchno = "";
                    this.Materialno = "";
                    this.Qty = "";
                    this.Unit = "";

                    this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    this.Updateman = trData.Createman;

                    this.updateHirackData(theDB, true, true);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void updateHirackData(Boolean sapIfFlag)
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

                //TB_HIRACK 정보  update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("CELLID", Helper.PreventNull(this.Cellid));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("HIRACKTRANSFERSTATUS", Helper.PreventNull(this.Hiracktransferstatus));
                pss.SetVarChar("CELLSTATUS", Helper.PreventNull(this.Cellstatus));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                //pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(this.Validexpireddate));
                //pss.SetVarChar("SAPPALLETID", Helper.PreventNull(this.Sappalletid));
                //pss.SetVarChar("SAPBATCHNO", Helper.PreventNull(this.Sapbatchno));
                //pss.SetVarChar("SAPITEMNO", Helper.PreventNull(this.Sapitemno));
                //pss.SetVarChar("SAPQTY", Helper.PreventNull(this.Sapqty));
                //pss.SetVarChar("SAPUNIT", Helper.PreventNull(this.Sapunit));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("CELLID1", Helper.PreventNull(this.Cellid));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertHirackDataBackup(theDB, "Update");

                ////SAP I/F 시에 Hi-Rack 정보에 Pallet id 지울 경우는 기존 값을 넣어줘야 함.
                //HirackData tempHirack = new HirackData();
                //try {
                //    tempHirack.getHirackData(this.Cellid);
                //} catch(SqlException se) 
                //{
                //}

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();

                //LGLS 주석 처리(2009.12.28)
                //if (sapIfFlag) 
                //{
                //    ////HiRack Data SAP I/F
                //    //if (string.IsNullOrEmpty(this.Palletid))
                //    //{
                //    //    if (!string.IsNullOrEmpty(tempHirack.Palletid))
                //    //    {
                //    //        this.Palletid = tempHirack.Palletid;
                //    //        ExternalIfData externalifData = new ExternalIfData();
                //    //        externalifData.insertExternalIfData("03", this);
                //    //    }
                //    //}
                //    //else
                //    //{
                //        ExternalIfData externalifData = new ExternalIfData();
                //        externalifData.insertExternalIfData("03", this);
                //    //}
                //}

                //DataBackup
                try
                {
                    HirackHistData hhData = new HirackHistData();
                    hhData.insertHirackHistData(this);
                }
                catch (Exception exe)
                {
                    //History Backup Error
                }
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

        public void updateHirackData(DBConnection theDB, Boolean bkFlag, Boolean sapIfFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_HIRACK 정보  update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("CELLID", Helper.PreventNull(this.Cellid));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("HIRACKTRANSFERSTATUS", Helper.PreventNull(this.Hiracktransferstatus));
                pss.SetVarChar("CELLSTATUS", Helper.PreventNull(this.Cellstatus));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                //pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(this.Validexpireddate));
                //pss.SetVarChar("SAPPALLETID", Helper.PreventNull(this.Sappalletid));
                //pss.SetVarChar("SAPBATCHNO", Helper.PreventNull(this.Sapbatchno));
                //pss.SetVarChar("SAPITEMNO", Helper.PreventNull(this.Sapitemno));
                //pss.SetVarChar("SAPQTY", Helper.PreventNull(this.Sapqty));
                //pss.SetVarChar("SAPUNIT", Helper.PreventNull(this.Sapunit));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("CELLID1", Helper.PreventNull(this.Cellid));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertHirackDataBackup(theDB, "Update");
                //}

                ////SAP I/F 시에 Hi-Rack 정보에 Pallet id 지울 경우는 기존 값을 넣어줘야 함.
                //HirackData tempHirack = new HirackData();
                //try
                //{
                //    tempHirack.getHirackData(this.Cellid);
                //}
                //catch (SqlException se)
                //{
                //}

                theDB.ExecuteUpdate(strSql, pss);

                //LGLS 주석 처리(2009.12.28)
                //if (sapIfFlag)
                //{
                //    ////HiRack Data SAP I/F
                //    //if (string.IsNullOrEmpty(this.Palletid))
                //    //{
                //    //    if (!string.IsNullOrEmpty(tempHirack.Palletid))
                //    //    {
                //    //        this.Palletid = tempHirack.Palletid;
                //    //        ExternalIfData externalifData = new ExternalIfData();
                //    //        externalifData.insertExternalIfData("03", this);
                //    //    }
                //    //}
                //    //else
                //    //{
                //        ExternalIfData externalifData = new ExternalIfData();
                //        externalifData.insertExternalIfData("03", this);
                //    //}
                //}

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        HirackHistData hhData = new HirackHistData();
                        hhData.insertHirackHistData(this);
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

        public string getSelectSQL()
        {
            string strSql = "";
                
            strSql = "" +
                    " SELECT  cellid                                                 " +
                    "        ,ISNULL(palletid            ,'')   palletid             " +
                    "        ,ISNULL(hiracktransferstatus,'')   hiracktransferstatus " +
                    "        ,ISNULL(cellstatus          ,'')   cellstatus           " +
                    "        ,ISNULL(materialno          ,'')   materialno           " +
                    "        ,ISNULL(qty                 ,'')   qty                  " +
                    "        ,ISNULL(unit                ,'')   unit                 " +
                    "        ,ISNULL(validexpireddate    ,'')   validexpireddate     " +
                    "        ,ISNULL(createtime          ,'')   createtime           " +
                    "        ,ISNULL(createman           ,'')   createman            " +
                    "        ,ISNULL(updatetime          ,'')   updatetime           " +
                    "        ,ISNULL(updateman           ,'')   updateman            " +
                    "   FROM tb_hirack                                               ";
            return strSql;
        }

        public string getSelectRowSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  TOP 1                                                  " +
                    "         cellid                                                 " +
                    "        ,ISNULL(palletid            ,'')   palletid             " +
                    "        ,ISNULL(hiracktransferstatus,'')   hiracktransferstatus " +
                    "        ,ISNULL(cellstatus          ,'')   cellstatus           " +
                    "        ,ISNULL(materialno          ,'')   materialno           " +
                    "        ,ISNULL(qty                 ,'')   qty                  " +
                    "        ,ISNULL(unit                ,'')   unit                 " +
                    "        ,ISNULL(validexpireddate    ,'')   validexpireddate     " +
                    "        ,ISNULL(createtime          ,'')   createtime           " +
                    "        ,ISNULL(createman           ,'')   createman            " +
                    "        ,ISNULL(updatetime          ,'')   updatetime           " +
                    "        ,ISNULL(updateman           ,'')   updateman            " +
                    "   FROM tb_hirack                                               ";
            return strSql;                 
        }

        public string getSelectCountSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  COUNT(*) CNT  " +
                    "   FROM tb_hirack      ";
            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_hirack                " +
                    "            ( cellid                  " +
                    "             ,palletid                " +
                    "             ,hiracktransferstatus    " +
                    "             ,cellstatus              " +
                    "             ,materialno              " +
                    "             ,qty                     " +
                    "             ,unit                    " +
                    "             ,validexpireddate        " +
                    "             ,createtime              " +
                    "             ,createman               " +
                    "             ,updatetime              " +
                    "             ,updateman)              " +
                    "      VALUES                          " +
                    "            ( @CELLID                 " +
                    "             ,@PALLETID               " +
                    "             ,@HIRACKTRANSFERSTATUS   " +
                    "             ,@CELLSTATUS             " +
                    "             ,@MATERIALNO             " +
                    "             ,@QTY                    " +
                    "             ,@UNIT                   " +
                    "             ,@VALIDEXPIREDDATE       " +
                    "             ,@CREATETIME             " +
                    "             ,@CREATEMAN              " +
                    "             ,@UPDATETIME             " +
                    "             ,@UPDATEMAN)             "; 
            
            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE tb_hirack                                          " +
                    "    SET  cellid                  = @CELLID                 " +
                    "        ,palletid                = @PALLETID               " +
                    "        ,hiracktransferstatus    = @HIRACKTRANSFERSTATUS   " +
                    "        ,cellstatus              = @CELLSTATUS             " +
                    "        ,materialno              = @MATERIALNO             " +
                    "        ,qty                     = @QTY                    " +
                    "        ,unit                    = @UNIT                   " +
                    "        ,validexpireddate        = @VALIDEXPIREDDATE       " +
                    "        ,createtime              = @CREATETIME             " +
                    "        ,createman               = @CREATEMAN              " +
                    "        ,updatetime              = @UPDATETIME             " +
                    "        ,updateman               = @UPDATEMAN              " +
                    "  WHERE  cellid                  = @CELLID1                ";

            return strSql;
        }

        public void insertHirackDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                HirackData data = new HirackData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE cellid  = '" + this.Cellid + "' ";

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
                    data.setHirackData(reader);
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
                internalIfData.Ifvalue= data.makeInternalIfValue();
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
            catch(SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public string makeInternalIfValue()
        {
            string ifValue = "";
            ifValue += this.Cellid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Hiracktransferstatus + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Cellstatus + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Validexpireddate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updateman;

            return ifValue;
        }
    }
}
