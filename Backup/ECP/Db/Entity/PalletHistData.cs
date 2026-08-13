using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class PalletHistData : ECPObject 
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
        private string deviceid = "";

        public string Deviceid
        {
            get { return deviceid; }
            set { deviceid = value; }
        }
        private string locationid = "";

        public string Locationid
        {
            get { return locationid; }
            set { locationid = value; }
        }
        private string sublocationid = "";

        public string Sublocationid
        {
            get { return sublocationid; }
            set { sublocationid = value; }
        }
        private string installtime = "";

        public string Installtime
        {
            get { return installtime; }
            set { installtime = value; }
        }
        private string predeviceid = "";

        public string Predeviceid
        {
            get { return predeviceid; }
            set { predeviceid = value; }
        }
        private string prelocationid = "";

        public string Prelocationid
        {
            get { return prelocationid; }
            set { prelocationid = value; }
        }
        private string presublocationid = "";

        public string Presublocationid
        {
            get { return presublocationid; }
            set { presublocationid = value; }
        }
        private string preinstalltime = "";

        public string Preinstalltime
        {
            get { return preinstalltime; }
            set { preinstalltime = value; }
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
        private string pallettransferstatus = "";

        public string Pallettransferstatus
        {
            get { return pallettransferstatus; }
            set { pallettransferstatus = value; }
        }
        private string status = "";

        public string Status
        {
            get { return status; }
            set { status = value; }
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
        public PalletHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //Hirack Meterial/Batch List �ȸ
        public List<PalletHistData> getPalletHistDataMeterialBatchList(string materialno, string batchno)
        {
            List<PalletHistData> list = new List<PalletHistData>();
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
                    PalletHistData phData = new PalletHistData();
                    phData.setPalletHistData(reader);

                    list.Add(phData);
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
        public List<PalletHistData> getPalletHistDataList(string fromdate, string todate)
        {
            List<PalletHistData> list = new List<PalletHistData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE backuptime   BETWEEN '" + fromdate + "'  " +
                      "    AND                      '" + todate + "'  " +
                      "    AND plantcode        = '" + GlobalConstant.PLANT_CODE + "'  " +
                      "  ORDER BY backuptime                                           ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletHistData phData = new PalletHistData();
                    phData.setPalletHistData(reader);

                    list.Add(phData);
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

        public void insertPalletHistData()
        {
            DBConnection theDB = null;
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                //para.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                para.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //para.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                para.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                para.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                para.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                para.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                para.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                para.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid));
                //para.SetVarChar("INSTALLTIME", Helper.PreventNull(this.Installtime));
                //para.SetVarChar("PREDEVICEID", Helper.PreventNull(this.Predeviceid));
                //para.SetVarChar("PRELOCATIONID", Helper.PreventNull(this.Prelocationid));
                //para.SetVarChar("PRESUBLOCATIONID", Helper.PreventNull(this.Presublocationid));
                //para.SetVarChar("PREINSTALLTIME", Helper.PreventNull(this.Preinstalltime));
                para.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                para.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                para.SetVarChar("PALLETTRANSFERSTATUS", Helper.PreventNull(this.Pallettransferstatus));
                para.SetVarChar("STATUS", Helper.PreventNull(this.Status));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                para.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                para.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                theDB.ExecuteUpdate(strSql, para);

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

        public void insertPalletHistData(PalletData pData)
        {
            DBConnection theDB = null;
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", DateTime.Now.ToString("yyyyMMddHHmmssfff"));
                //para.SetVarChar("PLANTCODE", Helper.PreventNull(pData.Plantcode));
                para.SetVarChar("PALLETID", Helper.PreventNull(pData.Palletid));
                //para.SetVarChar("BATCHNO", Helper.PreventNull(pData.Batchno));
                para.SetVarChar("MATERIALNO", Helper.PreventNull(pData.Materialno));
                para.SetVarChar("QTY", Helper.PreventNull(pData.Qty));
                para.SetVarChar("UNIT", Helper.PreventNull(pData.Unit));
                para.SetVarChar("DEVICEID", Helper.PreventNull(pData.Deviceid));
                para.SetVarChar("LOCATIONID", Helper.PreventNull(pData.Locationid));
                para.SetVarChar("SUBLOCATIONID", Helper.PreventNull(pData.Sublocationid));
                //para.SetVarChar("INSTALLTIME", Helper.PreventNull(pData.Installtime));
                //para.SetVarChar("PREDEVICEID", Helper.PreventNull(pData.Predeviceid));
                //para.SetVarChar("PRELOCATIONID", Helper.PreventNull(pData.Prelocationid));
                //para.SetVarChar("PRESUBLOCATIONID", Helper.PreventNull(pData.Presublocationid));
                //para.SetVarChar("PREINSTALLTIME", Helper.PreventNull(pData.Preinstalltime));
                para.SetVarChar("COMMANDID", Helper.PreventNull(pData.Commandid));
                para.SetVarChar("COMMANDSEQ", Helper.PreventNull(pData.Commandseq));
                para.SetVarChar("PALLETTRANSFERSTATUS", Helper.PreventNull(pData.Pallettransferstatus));
                para.SetVarChar("STATUS", Helper.PreventNull(pData.Status));
                para.SetVarChar("CREATETIME", Helper.PreventNull(pData.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(pData.Createman));
                para.SetVarChar("UPDATETIME", Helper.PreventNull(pData.Updatetime));
                para.SetVarChar("UPDATEMAN", Helper.PreventNull(pData.Updateman));


                theDB.ExecuteUpdate(strSql, para);

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

        public void insertPalletHistData(DBConnection theDB)
        {
            PreparedSqlParameter para = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();
                para = theDB.GetPreparedSqlParameter();

                para.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                //para.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                para.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //para.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                para.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                para.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                para.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                para.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                para.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                para.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid));
                //para.SetVarChar("INSTALLTIME", Helper.PreventNull(this.Installtime));
                //para.SetVarChar("PREDEVICEID", Helper.PreventNull(this.Predeviceid));
                //para.SetVarChar("PRELOCATIONID", Helper.PreventNull(this.Prelocationid));
                //para.SetVarChar("PRESUBLOCATIONID", Helper.PreventNull(this.Presublocationid));
                //para.SetVarChar("PREINSTALLTIME", Helper.PreventNull(this.Preinstalltime));
                para.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                para.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                para.SetVarChar("PALLETTRANSFERSTATUS", Helper.PreventNull(this.Pallettransferstatus));
                para.SetVarChar("STATUS", Helper.PreventNull(this.Status));
                para.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                para.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                para.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                para.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                theDB.ExecuteUpdate(strSql, para);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setPalletHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Plantcode = Helper.PreventNull(list[0]);
            this.Palletid = Helper.PreventNull(list[1]);
            this.Batchno = Helper.PreventNull(list[2]);
            this.Materialno = Helper.PreventNull(list[3]);
            this.Qty = Helper.PreventNull(list[4]);
            this.Unit = Helper.PreventNull(list[5]);
            this.Deviceid = Helper.PreventNull(list[6]);
            this.Locationid = Helper.PreventNull(list[7]);
            this.Sublocationid = Helper.PreventNull(list[8]);
            this.Installtime = Helper.PreventNull(list[9]);
            this.Predeviceid = Helper.PreventNull(list[10]);
            this.Prelocationid = Helper.PreventNull(list[11]);
            this.Presublocationid = Helper.PreventNull(list[12]);
            this.Preinstalltime = Helper.PreventNull(list[13]);
            this.Commandid = Helper.PreventNull(list[14]);
            this.Commandseq = Helper.PreventNull(list[15]);
            this.Pallettransferstatus = Helper.PreventNull(list[16]);
            this.Status = Helper.PreventNull(list[17]);
            this.Createtime = Helper.PreventNull(list[18]);
            this.Createman = Helper.PreventNull(list[19]);
            this.Updatetime = Helper.PreventNull(list[20]);
            this.Updateman = Helper.PreventNull(list[21]);  
        }

        public PalletData setPalletData(string[] list)
        {
            PalletData data = new PalletData();

            data.Plantcode = Helper.PreventNull(list[0]);
            data.Palletid = Helper.PreventNull(list[1]);
            data.Batchno = Helper.PreventNull(list[2]);
            data.Materialno = Helper.PreventNull(list[3]);
            data.Qty = Helper.PreventNull(list[4]);
            data.Unit = Helper.PreventNull(list[5]);
            data.Deviceid = Helper.PreventNull(list[6]);
            data.Locationid = Helper.PreventNull(list[7]);
            data.Sublocationid = Helper.PreventNull(list[8]);
            data.Installtime = Helper.PreventNull(list[9]);
            data.Predeviceid = Helper.PreventNull(list[10]);
            data.Prelocationid = Helper.PreventNull(list[11]);
            data.Presublocationid = Helper.PreventNull(list[12]);
            data.Preinstalltime = Helper.PreventNull(list[13]);
            data.Commandid = Helper.PreventNull(list[14]);
            data.Commandseq = Helper.PreventNull(list[15]);
            data.Pallettransferstatus = Helper.PreventNull(list[16]);
            data.Status = Helper.PreventNull(list[17]);
            data.Createtime = Helper.PreventNull(list[18]);
            data.Createman = Helper.PreventNull(list[19]);
            data.Updatetime = Helper.PreventNull(list[20]);
            data.Updateman = Helper.PreventNull(list[21]);

            return data;
        }

        public void setPalletHistData(SqlDataReader reader)
        {
            this.Backuptime = Helper.PreventNull((string)reader["BACKUPTIME"]);
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Deviceid = Helper.PreventNull((string)reader["DEVICEID"]);
            this.Locationid = Helper.PreventNull((string)reader["LOCATIONID"]);
            this.Sublocationid = Helper.PreventNull((string)reader["SUBLOCATIONID"]);
            this.Installtime = Helper.PreventNull((string)reader["INSTALLTIME"]);
            this.Predeviceid = Helper.PreventNull((string)reader["PREDEVICEID"]);
            this.Prelocationid = Helper.PreventNull((string)reader["PRELOCATIONID"]);
            this.Presublocationid = Helper.PreventNull((string)reader["PRESUBLOCATIONID"]);
            this.Preinstalltime = Helper.PreventNull((string)reader["PREINSTALLTIME"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandseq = Helper.PreventNull((string)reader["COMMANDSEQ"]);
            this.Pallettransferstatus = Helper.PreventNull((string)reader["PALLETTRANSFERSTATUS"]);
            this.Status = Helper.PreventNull((string)reader["STATUS"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  backuptime                        backuptime           " +
                    "        ,plantcode                         plantcode            " +
                    "        ,palletid                          palletid             " +
                    "        ,ISNULL(batchno             ,'')   batchno              " +
                    "        ,ISNULL(materialno          ,'')   materialno           " +
                    "        ,ISNULL(qty                 ,'')   qty                  " +
                    "        ,ISNULL(unit                ,'')   unit                 " +
                    "        ,ISNULL(deviceid            ,'')   deviceid             " +
                    "        ,ISNULL(locationid          ,'')   locationid           " +
                    "        ,ISNULL(sublocationid       ,'')   sublocationid        " +
                    "        ,ISNULL(installtime         ,'')   installtime          " +
                    "        ,ISNULL(predeviceid         ,'')   predeviceid          " +
                    "        ,ISNULL(prelocationid       ,'')   prelocationid        " +
                    "        ,ISNULL(presublocationid    ,'')   presublocationid     " +
                    "        ,ISNULL(preinstalltime      ,'')   preinstalltime       " +
                    "        ,ISNULL(commandid           ,'')   commandid            " +
                    "        ,ISNULL(commandseq          ,'')   commandseq           " +
                    "        ,ISNULL(pallettransferstatus,'')   pallettransferstatus " +
                    "        ,ISNULL(status              ,'')   status               " +
                    "        ,ISNULL(createtime          ,'')   createtime           " +
                    "        ,ISNULL(createman           ,'')   createman            " +
                    "        ,ISNULL(updatetime          ,'')   updatetime           " +
                    "        ,ISNULL(updateman           ,'')   updateman            " +
                    "   FROM tb_pallethist                                           ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql ="" + 
                    " INSERT INTO tb_pallethist          " +
                    "            (backuptime             " +
                    "            ,palletid               " +
                    "            ,materialno             " +
                    "            ,qty                    " +
                    "            ,unit                   " +
                    "            ,deviceid               " +
                    "            ,locationid             " +
                    "            ,sublocationid          " +
                    "            ,commandid              " +
                    "            ,commandseq             " +
                    "            ,pallettransferstatus   " +
                    "            ,status                 " +
                    "            ,createtime             " +
                    "            ,createman              " +
                    "            ,updatetime             " +
                    "            ,updateman)             " +
                    "      VALUES                        " +
                    "            (@BACKUPTIME            " +
                    "            ,@PALLETID              " +
                    "            ,@MATERIALNO            " +
                    "            ,@QTY                   " +
                    "            ,@UNIT                  " +
                    "            ,@DEVICEID              " +
                    "            ,@LOCATIONID            " +
                    "            ,@SUBLOCATIONID         " +
                    "            ,@COMMANDID             " +
                    "            ,@COMMANDSEQ            " +
                    "            ,@PALLETTRANSFERSTATUS  " +
                    "            ,@STATUS                " +
                    "            ,@CREATETIME            " +
                    "            ,@CREATEMAN             " +
                    "            ,@UPDATETIME            " +
                    "            ,@UPDATEMAN)            ";

            return strSql;
        }
    }
}
