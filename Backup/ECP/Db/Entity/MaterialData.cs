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
    public class MaterialData : ECPObject 
    {
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string materialname = "";

        public string Materialname
        {
            get { return materialname; }
            set { materialname = value; }
        }
        private string materialtype = "";

        public string Materialtype
        {
            get { return materialtype; }
            set { materialtype = value; }
        }
        private string unit = "";

        public string Unit
        {
            get { return unit; }
            set { unit = value; }
        }
        private string tempconditionflg = "";

        public string Tempconditionflg
        {
            get { return tempconditionflg; }
            set { tempconditionflg = value; }
        }
        private string valueconfig = "";

        public string Valueconfig
        {
            get { return valueconfig; }
            set { valueconfig = value; }
        }
        private string tempmin = "";

        public string Tempmin
        {
            get { return tempmin; }
            set { tempmin = value; }
        }
        private string tempmax = "";

        public string Tempmax
        {
            get { return tempmax; }
            set { tempmax = value; }
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

        public MaterialData()
        {
        }

        public void getMaterialData(string materialno)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE materialno = '" + materialno + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setMaterialData(reader);
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

        //Program List 조회
        public Dictionary<string, MaterialData> getMaterialDataListWithNoName(string mno, string mnm)
        {
            Dictionary<string, MaterialData> list = new Dictionary<string, MaterialData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE materialno LIKE '" + mno + "' " +
                      "    AND materialname LIKE '" + mnm + "' " +
                      "   ORDER BY materialno  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    MaterialData mData = new MaterialData();
                    mData.setMaterialData(reader);

                    list.Add(mData.Materialno, mData);
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

        //Program List 조회
        public Dictionary<string, MaterialData> getMaterialDataList()
        {
            Dictionary<string, MaterialData> list = new Dictionary<string, MaterialData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY materialno  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    MaterialData mData = new MaterialData();
                    mData.setMaterialData(reader);

                    list.Add(mData.Materialno, mData);
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

        public void insertMaterialData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Material 정보  inert                                                      
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.Materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.Materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.Tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.Valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.Tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.Tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertMaterialDataBackup(theDB, "Insert");
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

        public void updateMaterialData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Material 정보  update
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.Materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.Materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.Tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.Valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.Tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.Tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                pss.SetVarChar("MATERIALNO1", Helper.PreventNull(this.Materialno));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertMaterialDataBackup(theDB, "Update");
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

        public void deleteMaterialData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Program 정보  delete
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));

                if (bkFlag)
                {
                    //DataBackup
                    this.insertMaterialDataBackup(theDB, "Delete");
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

        public void insertMaterialData()
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

                //TB_Material 정보  inert                                                      
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.Materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.Materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.Tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.Valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.Tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.Tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));        

                //DataBackup
                this.insertMaterialDataBackup(theDB, "Insert");

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

        public void updateMaterialData()
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
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));
                pss.SetVarChar("MATERIALNAME", Helper.PreventNull(this.Materialname));
                pss.SetVarChar("MATERIALTYPE", Helper.PreventNull(this.Materialtype));
                pss.SetVarChar("UNIT", Helper.PreventNull(this.Unit));
                pss.SetVarChar("TEMPCONDITIONFLG", Helper.PreventNull(this.Tempconditionflg));
                pss.SetVarChar("VALUECONFIG", Helper.PreventNull(this.Valueconfig));
                pss.SetVarChar("TEMPMIN", Helper.PreventNull(this.Tempmin));
                pss.SetVarChar("TEMPMAX", Helper.PreventNull(this.Tempmax));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                pss.SetVarChar("MATERIALNO1", Helper.PreventNull(this.Materialno));
                
                //DataBackup
                this.insertMaterialDataBackup(theDB, "Update");

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

        public void deleteMaterialData()
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
                pss.SetVarChar("MATERIALNO", Helper.PreventNull(this.Materialno));

                //DataBackup
                this.insertMaterialDataBackup(theDB, "Delete");

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

        public void setMaterialData(SqlDataReader reader)
        {
            this.Materialno = Helper.PreventNull((string)reader["MATERIALNO"]);
            this.Materialname = Helper.PreventNull((string)reader["MATERIALNAME"]);
            this.Materialtype = Helper.PreventNull((string)reader["MATERIALTYPE"]);
            this.Unit = Helper.PreventNull((string)reader["UNIT"]);
            this.Tempconditionflg = Helper.PreventNull((string)reader["TEMPCONDITIONFLG"]);
            this.Valueconfig = Helper.PreventNull((string)reader["VALUECONFIG"]);
            this.Tempmin = Helper.PreventNull((string)reader["TEMPMIN"]);
            this.Tempmax = Helper.PreventNull((string)reader["TEMPMAX"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }
        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  materialno                   materialno       " +
                    "        ,ISNULL(materialname    , '') materialname     " +
                    "        ,ISNULL(materialtype    , '') materialtype     " +
                    "        ,ISNULL(unit            , '') unit             " +
                    "        ,ISNULL(tempconditionflg, '') tempconditionflg " +
                    "        ,ISNULL(valueconfig     , '') valueconfig      " +
                    "        ,ISNULL(tempmin         , '') tempmin          " +
                    "        ,ISNULL(tempmax         , '') tempmax          " +
                    "        ,ISNULL(createtime      , '') createtime       " +
                    "        ,ISNULL(createman       , '') createman        " +
                    "        ,ISNULL(updatetime      , '') updatetime       " +
                    "        ,ISNULL(updateman       , '') updateman        " +
                    "   FROM tb_material                                    ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_material        " +
                    "            ( materialno        " +
                    "             ,materialname      " +
                    "             ,materialtype      " +
                    "             ,unit              " +
                    "             ,tempconditionflg  " +
                    "             ,valueconfig       " +
                    "             ,tempmin           " +
                    "             ,tempmax           " +
                    "             ,createtime        " +
                    "             ,createman         " +
                    "             ,updatetime        " +
                    "             ,updateman         " +
                    "            )                   " +
                    "      VALUES                    " +
                    "            ( @MATERIALNO       " +
                    "             ,@MATERIALNAME     " +
                    "             ,@MATERIALTYPE     " +
                    "             ,@UNIT             " +
                    "             ,@TEMPCONDITIONFLG " +
                    "             ,@VALUECONFIG      " +
                    "             ,@TEMPMIN          " +
                    "             ,@TEMPMAX          " +
                    "             ,@CREATETIME       " +
                    "             ,@CREATEMAN        " +
                    "             ,@UPDATETIME       " +
                    "             ,@UPDATEMAN        " +
                    "            )                   ";                    

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_material                          " +
                    "    SET  materialno       = @MATERIALNO       " +
                    "        ,materialname     = @MATERIALNAME     " +
                    "        ,materialtype     = @MATERIALTYPE     " +
                    "        ,unit             = @UNIT             " +
                    "        ,tempconditionflg = @TEMPCONDITIONFLG " +
                    "        ,valueconfig      = @VALUECONFIG      " +
                    "        ,tempmin          = @TEMPMIN          " +
                    "        ,tempmax          = @TEMPMAX          " +
                    "        ,createtime       = @CREATETIME       " +
                    "        ,createman        = @CREATEMAN        " +
                    "        ,updatetime       = @UPDATETIME       " +
                    "        ,updateman        = @UPDATEMAN        " +
                    "  WHERE materialno        = @MATERIALNO1      ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_material           " +
                    "  WHERE materialno = @MATERIALNO   ";

            return strSql;
        }

        public void insertMaterialDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                MaterialData data = new MaterialData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE materialno  = '" + this.Materialno + "' ";

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
                    data.setMaterialData(reader);
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
            ifValue += this.Materialno + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialname + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Materialtype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Unit + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tempconditionflg + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Valueconfig + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tempmin + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tempmax + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updateman;

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
                    " SELECT  TOP 5                                         " +
                    "         materialno                   materialno       " +
                    "        ,ISNULL(materialname    , '') materialname     " +
                    "        ,ISNULL(materialtype    , '') materialtype     " +
                    "        ,ISNULL(unit            , '') unit             " +
                    "        ,ISNULL(tempconditionflg, '') tempconditionflg " +
                    "        ,ISNULL(valueconfig     , '') valueconfig      " +
                    "        ,ISNULL(tempmin         , '') tempmin          " +
                    "        ,ISNULL(tempmax         , '') tempmax          " +
                    "        ,ISNULL(createtime      , '') createtime       " +
                    "        ,ISNULL(createman       , '') createman        " +
                    "        ,ISNULL(updatetime      , '') updatetime       " +
                    "        ,ISNULL(updateman       , '') updateman        " +
                    "   FROM tb_materialtemp                                 ";

            return strSql;
        }

        //Hirack List 조회
        public List<MaterialData> getMaterialTempData()
        {
            List<MaterialData> list = new List<MaterialData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getTempSelectSQL();
            strSql += "  ORDER BY materialno                                        ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    MaterialData data = new MaterialData();
                    data.setMaterialData(reader);

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

    }
}
