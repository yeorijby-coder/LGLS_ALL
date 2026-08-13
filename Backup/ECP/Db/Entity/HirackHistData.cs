using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class HirackHistData : ECPObject 
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

        private string dbConnectionString = "";
        public HirackHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //Hirack List �ȸ
        public List<HirackHistData> getHirackHistDataList()
        {
            List<HirackHistData> list = new List<HirackHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY backuptime, cellid  ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackHistData hhData = new HirackHistData();
                    hhData.setHirackHistData(reader);

                    list.Add(hhData);
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

        //Hirack Meterial/Batch List �ȸ
        public List<HirackHistData> getHirackHistDataList(string fromdate, string todate)
        {
            List<HirackHistData> list = new List<HirackHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "    WHERE backuptime   BETWEEN '" + fromdate + "'  " +
                      "    AND                                     '" + todate + "'  " +
                      "    AND plantcode        = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "  ORDER BY backuptime                                           ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackHistData hhData = new HirackHistData();
                    hhData.setHirackHistData(reader);

                    list.Add(hhData);
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


        //Hirack Meterial/Batch List �ȸ
        public List<HirackHistData> getHirackHistDataMeterialBatchList(string materialno, string batchno)
        {
            List<HirackHistData> list = new List<HirackHistData>();
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
                    HirackHistData hhData = new HirackHistData();
                    hhData.setHirackHistData(reader);

                    list.Add(hhData);
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

        //Hirack Meterial/Batch List �ȸ
        public List<HirackHistData> getHirackHistDataCellList(string bankno, string bayno, string levelno)
        {
            List<HirackHistData> list = new List<HirackHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE plantcode             = '" + GlobalConstant.PLANT_CODE + "' " +
                      "    AND SUBSTRING(cellid,1,2) LIKE '" + bankno + "' " +
                      "    AND SUBSTRING(cellid,3,2) LIKE '" + bayno + "' " +
                      "    AND SUBSTRING(cellid,5,2) LIKE '" + levelno + "' " +
                      " ORDER BY cellid                                                    ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    HirackHistData hhData = new HirackHistData();
                    hhData.setHirackHistData(reader);

                    list.Add(hhData);
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

        public void insertHirackHistData()
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

                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("CELLID", Helper.PreventNull(this.Cellid));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                pss.SetVarChar("HIRACKTRANSFERSTATUS", Helper.PreventNull(this.Hiracktransferstatus));
                pss.SetVarChar("CELLSTATUS", Helper.PreventNull(this.Cellstatus));
                pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("FROMPLANT", Helper.PreventNull(this.Fromplant));
                pss.SetVarChar("FROMPOSITION", Helper.PreventNull(this.Fromposition));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(this.Validexpireddate));
                pss.SetVarChar("SAPPALLETID", Helper.PreventNull(this.Sappalletid));
                pss.SetVarChar("SAPBATCHNO", Helper.PreventNull(this.Sapbatchno));
                pss.SetVarChar("SAPITEMNO", Helper.PreventNull(this.Sapitemno));
                pss.SetVarChar("SAPQTY", Helper.PreventNull(this.Sapqty));
                pss.SetVarChar("SAPUNIT", Helper.PreventNull(this.Sapunit));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

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

        public void insertHirackHistData(HirackData hData)
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

                pss.SetVarChar("BACKUPTIME", DateTime.Now.ToString("yyyyMMddHHmmssfff"));
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(hData.Plantcode));
                pss.SetVarChar("CELLID", Helper.PreventNull(hData.Cellid));
                pss.SetVarChar("PALLETID", Helper.PreventNull(hData.Palletid));
                pss.SetVarChar("HIRACKTRANSFERSTATUS", Helper.PreventNull(hData.Hiracktransferstatus));
                pss.SetVarChar("CELLSTATUS", Helper.PreventNull(hData.Cellstatus));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(hData.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(hData.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(hData.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(hData.Unit));
                //pss.SetVarChar("FROMPLANT", Helper.PreventNull(hData.Fromplant));
                //pss.SetVarChar("FROMPOSITION", Helper.PreventNull(hData.Fromposition));
                pss.SetVarChar("VALIDEXPIREDDATE", Helper.PreventNull(hData.Validexpireddate));
                //pss.SetVarChar("SAPPALLETID", Helper.PreventNull(hData.Sappalletid));
                //pss.SetVarChar("SAPBATCHNO", Helper.PreventNull(hData.Sapbatchno));
                //pss.SetVarChar("SAPITEMNO", Helper.PreventNull(hData.Sapitemno));
                //pss.SetVarChar("SAPQTY", Helper.PreventNull(hData.Sapqty));
                //pss.SetVarChar("SAPUNIT", Helper.PreventNull(hData.Sapunit));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(hData.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(hData.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(hData.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(hData.Updateman));

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

        public void insertHirackHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                pss = theDB.GetPreparedSqlParameter();

                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
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

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setHirackHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Cellid = Helper.PreventNull(list[1]);
            this.Palletid = Helper.PreventNull(list[2]);
            this.Hiracktransferstatus = Helper.PreventNull(list[3]);
            this.Cellstatus = Helper.PreventNull(list[4]);
            this.Batchno = Helper.PreventNull(list[5]);
            this.Materialno = Helper.PreventNull(list[6]);
            this.Qty = Helper.PreventNull(list[7]);
            this.Unit = Helper.PreventNull(list[8]);
            this.Fromplant = Helper.PreventNull(list[9]);
            this.Fromposition = Helper.PreventNull(list[10]);
            this.Validexpireddate = Helper.PreventNull(list[11]);
            this.Sappalletid = Helper.PreventNull(list[12]);
            this.Sapbatchno = Helper.PreventNull(list[13]);
            this.Sapitemno = Helper.PreventNull(list[14]);
            this.Sapqty = Helper.PreventNull(list[15]);
            this.Sapunit = Helper.PreventNull(list[16]);
            this.Createtime = Helper.PreventNull(list[17]);
            this.Createman = Helper.PreventNull(list[18]);
            this.Updatetime = Helper.PreventNull(list[19]);
            this.Updateman = Helper.PreventNull(list[20]);
        }

        public HirackData setHirackData(string[] list)
        {
            HirackData data = new HirackData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Cellid = Helper.PreventNull(list[1]);
            data.Palletid = Helper.PreventNull(list[2]);
            data.Hiracktransferstatus = Helper.PreventNull(list[3]);
            data.Cellstatus = Helper.PreventNull(list[4]);
            data.Batchno = Helper.PreventNull(list[5]);
            data.Materialno = Helper.PreventNull(list[6]);
            data.Qty = Helper.PreventNull(list[7]);
            data.Unit = Helper.PreventNull(list[8]);
            data.Fromplant = Helper.PreventNull(list[9]);
            data.Fromposition = Helper.PreventNull(list[10]);
            data.Validexpireddate = Helper.PreventNull(list[11]);
            data.Sappalletid = Helper.PreventNull(list[12]);
            data.Sapbatchno = Helper.PreventNull(list[13]);
            data.Sapitemno = Helper.PreventNull(list[14]);
            data.Sapqty = Helper.PreventNull(list[15]);
            data.Sapunit = Helper.PreventNull(list[16]);
            data.Createtime = Helper.PreventNull(list[17]);
            data.Createman = Helper.PreventNull(list[18]);
            data.Updatetime = Helper.PreventNull(list[19]);
            data.Updateman = Helper.PreventNull(list[20]);

            return data;
        }

        public void setHirackHistData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Cellid = Helper.PreventNull((string)reader["CELLID"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Hiracktransferstatus = Helper.PreventNull((string)reader["HIRACKTRANSFERSTATUS"]);
            this.Cellstatus = Helper.PreventNull((string)reader["CELLSTATUS"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Fromplant = Helper.PreventNull((string)reader["FROMPLANT"]);
            this.Fromposition = Helper.PreventNull((string)reader["FROMPOSITION"]);
            this.Validexpireddate = Helper.PreventNull((string)reader["VALIDEXPIREDDATE"]);
            this.Sappalletid = Helper.PreventNull((string)reader["SAPPALLETID"]);
            this.Sapbatchno = Helper.PreventNull((string)reader["SAPBATCHNO"]);
            this.Sapitemno = Helper.PreventNull((string)reader["SAPITEMNO"]);
            this.Sapqty = Helper.PreventNull((string)reader["SAPQTY"]);
            this.Sapunit = Helper.PreventNull((string)reader["SAPUNIT"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  backuptime                      backuptime            " +
                    "        ,plantcode                       plantcode             " +
                    "        ,cellid                          cellid                " +
                    "        ,ISNULL(palletid            ,'') palletid              " +
                    "        ,ISNULL(hiracktransferstatus,'') hiracktransferstatus  " +
                    "        ,ISNULL(cellstatus          ,'') cellstatus            " +
                    "        ,ISNULL(batchno             ,'') batchno               " +
                    "        ,ISNULL(materialno          ,'') materialno            " +
                    "        ,ISNULL(qty                 ,'') qty                   " +
                    "        ,ISNULL(unit                ,'') unit                  " +
                    "        ,ISNULL(fromplant           ,'') fromplant             " +
                    "        ,ISNULL(fromposition        ,'') fromposition          " +
                    "        ,ISNULL(validexpireddate    ,'') validexpireddate      " +
                    "        ,ISNULL(sappalletid         ,'') sappalletid           " +
                    "        ,ISNULL(sapbatchno          ,'') sapbatchno            " +
                    "        ,ISNULL(sapitemno           ,'') sapitemno             " +
                    "        ,ISNULL(sapqty              ,'') sapqty                " +
                    "        ,ISNULL(sapunit             ,'') sapunit               " +
                    "        ,ISNULL(createtime          ,'') createtime            " +
                    "        ,ISNULL(createman           ,'') createman             " +
                    "        ,ISNULL(updatetime          ,'') updatetime            " +
                    "        ,ISNULL(updateman           ,'') updateman             " +
                    "   FROM tb_hirackhist                                          ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_hirackhist            " +
                    "            ( backuptime              " +
                    "             ,cellid                  " +
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
                    "            ( @BACKUPTIME             " +
                    "             ,@CELLID                 " +
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
    }
}
