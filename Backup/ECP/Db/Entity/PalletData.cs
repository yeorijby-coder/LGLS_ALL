using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class PalletData:ECPObject 
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

        public PalletData()
        {
        }

        //Hirack List 조회
        public Dictionary<string, PalletData> getPalletDataListWithPalletBatchMaterial(string palletid, string materialno, string batchno)
        {
            Dictionary<string, PalletData> list = new Dictionary<string, PalletData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE palletid   LIKE '" + palletid + "'  " +
                      "    AND materialno   LIKE '" + materialno + "'  " +
                      "    AND batchno      LIKE '" + batchno + "'  " +
                      "  ORDER BY palletid                                       ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletData data = new PalletData();
                    data.setPalletData(reader);

                    list.Add(data.Palletid, data);
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
        public Dictionary<string, PalletData> getPalletDataList()
        {
            Dictionary<string, PalletData> list = new Dictionary<string, PalletData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY palletid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletData data = new PalletData();
                    data.setPalletData(reader);

                    list.Add(data.Palletid, data);
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
        public List<PalletData> getPalletDataMeterialBatchList(string materialno, string batchno)
        {
            List<PalletData> list = new List<PalletData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE materialno   LIKE '" + materialno + "'  " +
                      "    AND batchno      LIKE '" + batchno + "'  " +
                      "  ORDER BY materialno                                       ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    PalletData pData = new PalletData();
                    pData.setPalletData(reader);

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

        public void getPalletData(string palletid)
        {
            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();
                this.getPalletData(theDB, palletid);
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
        }

        public void getPalletData(DBConnection theDB, string palletid)
        {
            string strSql = "";
            SqlDataReader reader = null;

            strSql = getSelectSQL(palletid);

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setPalletData(reader);
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
        }

        public void updatePalletStatus(DBConnection theDB, TransferData trData)
        {
            string strSql = "";
            SqlDataReader reader = null;

            strSql = getSelectSQL();
            strSql += "  WHERE palletid  = '" + trData.Palletidname           + "'  ";
            try
            {
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setPalletData(reader);
                }

                if (reader != null) reader.Close();

                this.Commandid = trData.Commandid;

                this.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Wait.ToString();
                this.Status = EntityEnumData.PALLETSTATUS.Reserved.ToString();

                if (trData.Commandtype != EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    //this.Batchno = trData.Batchno;
                    this.Materialno = trData.Materialno;
                    this.Qty = trData.Qty;
                    this.Unit = trData.Unit;
                }

                //this.Predeviceid = this.Deviceid;
                //this.Prelocationid = this.Locationid;
                //this.Presublocationid = this.Sublocationid;

                this.Deviceid = trData.Fromdeviceid;
                this.Locationid = trData.Fromlocationid;
                this.Sublocationid = trData.Fromsublocationid;

                this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                this.Updateman = trData.Createman;

                int iRow = this.updatePalletData(theDB, true);

                if (iRow == 0)
                {
                    insertPalletData(theDB, trData);
                }

            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void updatePalletCancelStatus(DBConnection theDB, TransferData trData)
        {
            string strSql = "";
            SqlDataReader reader = null;

            strSql = getSelectSQL();
            strSql += "  WHERE palletid  = '" + trData.Palletid + "'  ";
            try
            {
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setPalletData(reader);
                }

                if (reader != null) reader.Close();

                this.Commandid = "";
                this.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();

                //this.Deviceid = this.Predeviceid;
                //this.Locationid = this.Prelocationid;
                //this.Sublocationid = this.Presublocationid;

                //this.Predeviceid = "";
                //this.Prelocationid = "";
                //this.Presublocationid = "";

                if (trData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    this.Status = EntityEnumData.PALLETSTATUS.Assigned.ToString();
                }
                else
                {
                    this.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();

                    //this.Batchno = "";
                    this.Materialno = "";
                    this.Qty = "";
                    this.Unit = "";
                }

                this.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                this.Updateman = trData.Createman;

                int iRow = this.updatePalletData(theDB, true);

                if (iRow == 0)
                {
                    insertPalletData(theDB, trData);
                }

            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public int updatePalletData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";
            int iResult = 0;

            try
            {
                strSql = getUpdateSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Pallet  정보  update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid));
                //pss.SetVarChar("INSTALLTIME", Helper.PreventNull(this.Installtime));
                //pss.SetVarChar("PREDEVICEID", Helper.PreventNull(this.Predeviceid));
                //pss.SetVarChar("PRELOCATIONID", Helper.PreventNull(this.Prelocationid));
                //pss.SetVarChar("PRESUBLOCATIONID", Helper.PreventNull(this.Presublocationid));
                //pss.SetVarChar("PREINSTALLTIME", Helper.PreventNull(this.Preinstalltime));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PALLETTRANSFERSTATUS", Helper.PreventNull(this.Pallettransferstatus));
                pss.SetVarChar("STATUS", Helper.PreventNull(this.Status));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID1", Helper.PreventNull(this.Palletid));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertPalletDataBackup(theDB, "Update");
                //}

                iResult = theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        PalletHistData phData = new PalletHistData();
                        phData.insertPalletHistData(this);
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

            return iResult;
        }

        public void updatePalletData()
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

                //TB_Pallet  정보  update
                //pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));
                //pss.SetVarChar("BATCHNO", Helper.PreventNull(this.Batchno));
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("QTY", Helper.PreventNull(this.Qty));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("DEVICEID", Helper.PreventNull(this.Deviceid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("SUBLOCATIONID", Helper.PreventNull(this.Sublocationid));
                //pss.SetVarChar("INSTALLTIME", Helper.PreventNull(this.Installtime));
                //pss.SetVarChar("PREDEVICEID", Helper.PreventNull(this.Predeviceid));
                //pss.SetVarChar("PRELOCATIONID", Helper.PreventNull(this.Prelocationid));
                //pss.SetVarChar("PRESUBLOCATIONID", Helper.PreventNull(this.Presublocationid));
                //pss.SetVarChar("PREINSTALLTIME", Helper.PreventNull(this.Preinstalltime));
                pss.SetVarChar("COMMANDID", Helper.PreventNull(this.Commandid));
                pss.SetVarChar("COMMANDSEQ", Helper.PreventNull(this.Commandseq));
                pss.SetVarChar("PALLETTRANSFERSTATUS", Helper.PreventNull(this.Pallettransferstatus));
                pss.SetVarChar("STATUS", Helper.PreventNull(this.Status));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                //pss.SetVarChar("PLANTCODE1", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("PALLETID1", Helper.PreventNull(this.Palletid));

                theDB.ExecuteUpdate(strSql, pss);
                theDB.Commit();

                //DataBackup
                try
                {
                    PalletHistData phData = new PalletHistData();
                    phData.insertPalletHistData(this);
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

        public void insertPalletData(DBConnection theDB, TransferData trData)
        {
            //this.Plantcode = Helper.PreventNull(trData.Plantcode);
            this.Palletid = Helper.PreventNull(trData.Palletidname);
            //this.Batchno = Helper.PreventNull(trData.Batchno);
            this.Materialno = Helper.PreventNull(trData.Materialno);
            this.Qty = Helper.PreventNull(trData.Qty);
            this.Unit = Helper.PreventNull(trData.Unit);
            this.Deviceid = Helper.PreventNull(trData.Fromdeviceid);
            this.Locationid = Helper.PreventNull(trData.Fromlocationid);
            this.Sublocationid = Helper.PreventNull(trData.Fromsublocationid);
            //this.Installtime = Helper.PreventNull("");
            //this.Predeviceid = Helper.PreventNull("");
            //this.Prelocationid = Helper.PreventNull("");
            //this.Presublocationid = Helper.PreventNull("");
            //this.Preinstalltime = Helper.PreventNull("");
            this.Commandid = Helper.PreventNull(trData.Commandid);
            this.Commandseq = Helper.PreventNull("");
            this.Pallettransferstatus = Helper.PreventNull(trData.Transferstatus);
            this.Status = Helper.PreventNull(EntityEnumData.PALLETSTATUS.Reserved.ToString());
            this.Createtime = Helper.PreventNull(DateTime.Now.ToString("yyyyMMddHHmmssfff"));
            this.Createman = Helper.PreventNull(trData.Createman);
            this.Updatetime = Helper.PreventNull("");
            this.Updateman = Helper.PreventNull(""); 

            try
            {
                this.insertPalletData(theDB, true);
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void insertPalletData(TransferData trData)
        {
            DBConnection theDB = null;

            //this.Plantcode = Helper.PreventNull(trData.Plantcode);
            this.Palletid = Helper.PreventNull(trData.Palletidname);
            //this.Batchno = Helper.PreventNull(trData.Batchno);
            this.Materialno = Helper.PreventNull(trData.Materialno);
            this.Qty = Helper.PreventNull(trData.Qty);
            this.Unit = Helper.PreventNull(trData.Unit);
            this.Deviceid = Helper.PreventNull(trData.Fromdeviceid);
            this.Locationid = Helper.PreventNull(trData.Fromlocationid);
            this.Sublocationid = Helper.PreventNull(trData.Fromsublocationid);
            //this.Installtime = Helper.PreventNull("");
            //this.Predeviceid = Helper.PreventNull("");
            //this.Prelocationid = Helper.PreventNull("");
            //this.Presublocationid = Helper.PreventNull("");
            //this.Preinstalltime = Helper.PreventNull("");
            this.Commandid = Helper.PreventNull(trData.Commandid);
            this.Commandseq = Helper.PreventNull("");
            this.Pallettransferstatus = Helper.PreventNull(trData.Transferstatus);
            this.Status = Helper.PreventNull(EntityEnumData.PALLETSTATUS.Reserved.ToString());
            this.Createtime = Helper.PreventNull(DateTime.Now.ToString("yyyyMMddHHmmssfff"));
            this.Createman = Helper.PreventNull(trData.Createman);
            this.Updatetime = Helper.PreventNull("");
            this.Updateman = Helper.PreventNull("");

            try
            {
                theDB = DBControl.GetConnection();

                theDB.Start();
                this.insertPalletData(theDB, true);

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

        public void insertPalletData(DBConnection theDB, Boolean bkFlag)
        {
            string strSql = "";
            strSql = getInsertSQL();

            PreparedSqlParameter para = null;

            try
            {
                para = theDB.GetPreparedSqlParameter();

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

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertPalletDataBackup(theDB, "Insert");
                //}

                theDB.ExecuteUpdate(strSql, para);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        PalletHistData phData = new PalletHistData();
                        phData.insertPalletHistData(this);
                    }
                    catch (Exception exe)
                    {
                        //History Backup Error
                    }
                }
            }
            catch (SqlException se)
            {
                if (theDB != null) theDB.Rollback();
                throw se;
            }
        }

        public void deletePalletData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Pallet  정보  update
                pss.SetVarChar("PALLETID", Helper.PreventNull(this.Palletid));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertPalletDataBackup(theDB, "Delete");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                if (bkFlag)
                {
                    //DataBackup
                    try
                    {
                        PalletHistData phData = new PalletHistData();
                        phData.insertPalletHistData(this);
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

        public void setPalletData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Palletid = Helper.PreventNull((string)reader["PALLETID"]);
            //this.Batchno = Helper.PreventNull((string)reader["BATCHNO"]);
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Qty = Helper.PreventNull((string)reader["QTY"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Deviceid = Helper.PreventNull((string)reader["DEVICEID"]);
            this.Locationid = Helper.PreventNull((string)reader["LOCATIONID"]);
            this.Sublocationid = Helper.PreventNull((string)reader["SUBLOCATIONID"]);
            //this.Installtime = Helper.PreventNull((string)reader["INSTALLTIME"]);
            //this.Predeviceid = Helper.PreventNull((string)reader["PREDEVICEID"]);
            //this.Prelocationid = Helper.PreventNull((string)reader["PRELOCATIONID"]);
            //this.Presublocationid = Helper.PreventNull((string)reader["PRESUBLOCATIONID"]);
            //this.Preinstalltime = Helper.PreventNull((string)reader["PREINSTALLTIME"]);
            this.Commandid = Helper.PreventNull((string)reader["COMMANDID"]);
            this.Commandseq = Helper.PreventNull((string)reader["COMMANDSEQ"]);
            this.Pallettransferstatus = Helper.PreventNull((string)reader["PALLETTRANSFERSTATUS"]);
            this.Status = Helper.PreventNull((string)reader["STATUS"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);  
        }

        public string getInsertSQL()
        {
            string strSql = "";

            strSql ="" + 
                    " INSERT INTO tb_pallet              " +
                    "            (palletid               " +
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
                    "            (@PALLETID              " +
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

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE tb_pallet                                        " +
                    "    SET  palletid              = @PALLETID             " +
                    "        ,materialno            = @MATERIALNO           " +
                    "        ,qty                   = @QTY                  " +
                    "        ,unit                  = @UNIT                 " +
                    "        ,deviceid              = @DEVICEID             " +
                    "        ,locationid            = @LOCATIONID           " +
                    "        ,sublocationid         = @SUBLOCATIONID        " +
                    "        ,commandid             = @COMMANDID            " +
                    "        ,commandseq            = @COMMANDSEQ           " +
                    "        ,pallettransferstatus  = @PALLETTRANSFERSTATUS " +
                    "        ,status                = @STATUS               " +
                    "        ,createtime            = @CREATETIME           " +
                    "        ,createman             = @CREATEMAN            " +
                    "        ,updatetime            = @UPDATETIME           " +
                    "        ,updateman             = @UPDATEMAN            " +
                    "  WHERE  palletid              = @PALLETID1            ";     

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE tb_pallet                                      " +
                    "  WHERE  palletid              = @PALLETID             ";

            return strSql;
        }

        public string getSelectSQL(string palletid)
        {
            string strSql = getSelectSQL();

            if (string.IsNullOrEmpty(palletid))
            {
                return strSql;
            }
            else
            {
                strSql += " WHERE palletid  = '" + palletid                   + "' ";
            }
            return strSql;
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  palletid                                              " +
                    "        ,ISNULL(materialno          ,'')  materialno           " +
                    "        ,ISNULL(qty                 ,'')  qty                  " +
                    "        ,ISNULL(unit                ,'')  unit                 " +
                    "        ,ISNULL(deviceid            ,'')  deviceid             " +
                    "        ,ISNULL(locationid          ,'')  locationid           " +
                    "        ,ISNULL(sublocationid       ,'')  sublocationid        " +
                    "        ,ISNULL(commandid           ,'')  commandid            " +
                    "        ,ISNULL(commandseq          ,'')  commandseq           " +
                    "        ,ISNULL(pallettransferstatus,'')  pallettransferstatus " +
                    "        ,ISNULL(status              ,'')  status               " +
                    "        ,ISNULL(createtime          ,'')  createtime           " +
                    "        ,ISNULL(createman           ,'')  createman            " +
                    "        ,ISNULL(updatetime          ,'')  updatetime           " +
                    "        ,ISNULL(updateman           ,'')  updateman            " +
                    "  FROM  tb_pallet                                              ";

            return strSql;
        }

        public void insertPalletDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                PalletData data = new PalletData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE palletid   = '" + this.Palletid             + "' ";

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
                    data.setPalletData(reader);
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
            ifValue += this.Palletid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Qty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Deviceid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Locationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Sublocationid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Commandseq + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Pallettransferstatus + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Status + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updateman;

            return ifValue;
        }
    }
}
