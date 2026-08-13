using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class PalletBlockingHistData : ECPObject 
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

        private string dbConnectionString = "";
        public PalletBlockingHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //전체 반송 정보를 가져옴.
        public List<PalletBlockingHistData> getPalletBlockingHistDataList()
        {
            List<PalletBlockingHistData> list = new List<PalletBlockingHistData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletBlockingHistData data = new PalletBlockingHistData();

                    data.setPalletBlockingHistData(reader);

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

        public List<PalletBlockingHistData> getPalletBlockingHistDataList(string fromdate, string todate)
        {
            List<PalletBlockingHistData> list = new List<PalletBlockingHistData>();

            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE backuptime  BETWEEN '" + fromdate + "'  " +
                      "    AND                     '" + todate + "'  " +
                      "    AND plantcode        = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "  ORDER BY backuptime                                           ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletBlockingHistData data = new PalletBlockingHistData();

                    data.setPalletBlockingHistData(reader);

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
        public List<PalletBlockingHistData> getPalletBlockingHistDataMeterialBatchList(string materialno, string batchno)
        {
            List<PalletBlockingHistData> list = new List<PalletBlockingHistData>();
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
                    PalletBlockingHistData pData = new PalletBlockingHistData();
                    pData.setPalletBlockingHistData(reader);

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

        public void insertPalletBlockingHistData()
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

                //TB_PALLETBLOCKINGHIST DATA update                                                    
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
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

        public void insertPalletBlockingHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                //TB_PALLETBLOCKINGHIST DATA update                                                    
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
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

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setPalletBlockingHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Palletid = Helper.PreventNull(list[1]);
            this.Batchno = Helper.PreventNull(list[2]);
            this.Materialno = Helper.PreventNull(list[3]);
            this.Qty = Helper.PreventNull(list[4]);
            this.Unit = Helper.PreventNull(list[5]);
            this.Blockingtype = Helper.PreventNull(list[6]);
            this.Blockingreason = Helper.PreventNull(list[7]);
            this.Starttime = Helper.PreventNull(list[8]);
            this.Startman = Helper.PreventNull(list[9]);
            this.Endtime = Helper.PreventNull(list[10]);
            this.Endman = Helper.PreventNull(list[11]);
        }

        public PalletBlockingData setPalletBlockingData(string[] list)
        {
            PalletBlockingData data = new PalletBlockingData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Palletid = Helper.PreventNull(list[1]);
            data.Batchno = Helper.PreventNull(list[2]);
            data.Materialno = Helper.PreventNull(list[3]);
            data.Qty = Helper.PreventNull(list[4]);
            data.Unit = Helper.PreventNull(list[5]);
            data.Blockingtype = Helper.PreventNull(list[6]);
            data.Blockingreason = Helper.PreventNull(list[7]);
            data.Starttime = Helper.PreventNull(list[8]);
            data.Startman = Helper.PreventNull(list[9]);
            data.Endtime = Helper.PreventNull(list[10]);
            data.Endman = Helper.PreventNull(list[11]);

            return data;
        }

        public void setPalletBlockingHistData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
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
                    "  SELECT  backuptime                               " +
                    "         ,plantcode                                " +
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
                    "    FROM tb_palletblockinghist                     ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql = "" +
                    "  INSERT INTO tb_palletblockinghist " +
                    "        ( backuptime                " +
                    "         ,plantcode                 " +
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
                    "  VALUES (  @BACKUPTIME             " +
                    "           ,@PLANTCODE              " +
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
    }
}
