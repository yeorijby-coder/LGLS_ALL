using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class CodeMasterData : ECPObject
    {
        private string systemcd = "";

        public string Systemcd
        {
            get { return systemcd; }
            set { systemcd = value; }
        }
        private string codeclass = "";

        public string Codeclass
        {
            get { return codeclass; }
            set { codeclass = value; }
        }
        private string code = "";

        public string Code
        {
            get { return code; }
            set { code = value; }
        }
        private string value = "";

        public string Value
        {
            get { return this.value; }
            set { this.value = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
        }
        private string remark = "";

        public string Remark
        {
            get { return remark; }
            set { remark = value; }
        }
        private string attribute1 = "";

        public string Attribute1
        {
            get { return attribute1; }
            set { attribute1 = value; }
        }
        private string attribute2 = "";

        public string Attribute2
        {
            get { return attribute2; }
            set { attribute2 = value; }
        }
        private string attribute3 = "";

        public string Attribute3
        {
            get { return attribute3; }
            set { attribute3 = value; }
        }
        private string attribute4 = "";

        public string Attribute4
        {
            get { return attribute4; }
            set { attribute4 = value; }
        }
        private string attribute5 = "";

        public string Attribute5
        {
            get { return attribute5; }
            set { attribute5 = value; }
        }

        private string attribute6 = "";

        public string Attribute6
        {
            get { return attribute6; }
            set { attribute6 = value; }
        }

        private string attribute7 = "";

        public string Attribute7
        {
            get { return attribute7; }
            set { attribute7 = value; }
        }

        private string attribute8 = "";

        public string Attribute8
        {
            get { return attribute8; }
            set { attribute8 = value; }
        }

        public CodeMasterData()
        {
        }

        public void getCodeMasterData(string systemcd, string codeclass, string code)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE batchno   = '" + systemcd  + "' " + 
                      "    AND codeclass = '" + codeclass + "' " +
                      "    AND code      = '" + code      + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
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
        public List<CodeMasterData> getCodeMasterDataList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY systemcd, codeclass, code  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        public void insertCodeMasterData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Codemaster 정보  inert                                       
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE     ", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED  ", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK    ", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5));
                pss.SetVarChar("ATTRIBUTE6", Helper.PreventNull(this.Attribute6));
                pss.SetVarChar("ATTRIBUTE7", Helper.PreventNull(this.Attribute7));
                pss.SetVarChar("ATTRIBUTE8", Helper.PreventNull(this.Attribute8));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertCodeMasterDataBackup(theDB, "Insert");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updateCodeMasterData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_CodeMaser 정보  update
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE     ", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED  ", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK    ", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5));
                pss.SetVarChar("ATTRIBUTE6", Helper.PreventNull(this.Attribute6));
                pss.SetVarChar("ATTRIBUTE7", Helper.PreventNull(this.Attribute7));
                pss.SetVarChar("ATTRIBUTE8", Helper.PreventNull(this.Attribute8));
                pss.SetVarChar("SYSTEMCD1  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS1 ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE1      ", Helper.PreventNull(this.Code));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertCodeMasterDataBackup(theDB, "Update");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteCodeMasterData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_CodeMaster정보  delete
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));

                //LGLS 주석 처리(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertCodeMasterDataBackup(theDB, "Delete");
                //}

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertCodeMasterData()
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

                //TB_Codemaster 정보  inert                                       
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE     ", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED  ", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK    ", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5));
                pss.SetVarChar("ATTRIBUTE6", Helper.PreventNull(this.Attribute6));
                pss.SetVarChar("ATTRIBUTE7", Helper.PreventNull(this.Attribute7));
                pss.SetVarChar("ATTRIBUTE8", Helper.PreventNull(this.Attribute8));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertCodeMasterDataBackup(theDB, "Insert");

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

        public void updateCodeMasterData()
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

                //TB_CodeMaser 정보  update
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));
                pss.SetVarChar("VALUE     ", Helper.PreventNull(this.Value));
                pss.SetVarChar("DISABLED  ", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("REMARK    ", Helper.PreventNull(this.Remark));
                pss.SetVarChar("ATTRIBUTE1", Helper.PreventNull(this.Attribute1));
                pss.SetVarChar("ATTRIBUTE2", Helper.PreventNull(this.Attribute2));
                pss.SetVarChar("ATTRIBUTE3", Helper.PreventNull(this.Attribute3));
                pss.SetVarChar("ATTRIBUTE4", Helper.PreventNull(this.Attribute4));
                pss.SetVarChar("ATTRIBUTE5", Helper.PreventNull(this.Attribute5));
                pss.SetVarChar("ATTRIBUTE6", Helper.PreventNull(this.Attribute6));
                pss.SetVarChar("ATTRIBUTE7", Helper.PreventNull(this.Attribute7));
                pss.SetVarChar("ATTRIBUTE8", Helper.PreventNull(this.Attribute8));
                pss.SetVarChar("SYSTEMCD1  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS1 ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE1      ", Helper.PreventNull(this.Code));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertCodeMasterDataBackup(theDB, "Update");

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

        public void deleteCodeMasterData()
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

                //TB_CodeMaster정보  delete
                pss.SetVarChar("SYSTEMCD  ", Helper.PreventNull(this.Systemcd));
                pss.SetVarChar("CODECLASS ", Helper.PreventNull(this.Codeclass));
                pss.SetVarChar("CODE      ", Helper.PreventNull(this.Code));

                //LGLS 주석 처리(2009.12.28)
                ////DataBackup
                //this.insertCodeMasterDataBackup(theDB, "Delete");

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


        //System Info List 조회
        public List<CodeMasterData> getSystemInfoValues()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'   " +
                      "    AND code      = 'ZZ'  " +
                      "    AND disabled  = 'N'   " +
                      "  ORDER BY codeclass      ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //입출고 저장위치
        public List<CodeMasterData> getPositionValues(string fromtoFlag)
        {
            string fromFlag = "";
            string toFlag = "";

            if (fromtoFlag == GlobalConstant.FROM_TEXT)
            {
                fromFlag = "Y";
                toFlag = GlobalConstant.ALL_CHARACTER;
            }
            else
            {
                toFlag = "Y";
                fromFlag = GlobalConstant.ALL_CHARACTER;
            }

            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'                    " +
                      "    AND codeclass = '001'                  " +
                      "    AND code      <> 'ZZ'                  " +
                      "    AND attribute1 LIKE '" + fromFlag + "' " +
                      "    AND attribute2 LIKE '" + toFlag + "' " +
                      "    AND disabled  = 'N'                    ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //입출고 저장위치
        public List<CodeMasterData> getPositionValueList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'                    " +
                      "    AND codeclass = '001'                  " +
                      "    AND code      <> 'ZZ'                  " +
                       "    AND disabled  = 'N'                    ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //입출고 저장위치
        public void getPositionPriorityValue(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'                    " +
                      "    AND codeclass = '001'                  " +
                      "    AND code      <> 'ZZ'                  " +
                      "    AND code      = '" + code         + "' " +
                       "    AND disabled  = 'N'                   ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setCodeMasterData(reader);
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

        //Pallet 상태
        public List<CodeMasterData> getPalletStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '002'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Hirack 상태
        public List<CodeMasterData> getHirackStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '003'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //입출고 유형
        public List<CodeMasterData> getCommandTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            //DBConnection theDB = null;
            //SqlDataReader reader = null;
            //string strSql = getSelectSQL();
            //strSql += "  WHERE systemcd  = 'C'               " +
            //          "    AND codeclass = '015'             " +
            //          "    AND code      LIKE '" + code + "' " +
            //          "    AND code      <> 'ZZ'             " +
            //          "    AND disabled  = 'N'                 ";

            //try
            //{
            //    theDB = DBControl.GetConnection();
            //    reader = theDB.ExecuteQuery(strSql);

            //    while (reader.Read())
            //    {
            //        CodeMasterData data = new CodeMasterData();
            //        data.setCodeMasterData(reader);

            //        list.Add(data);
            //    }
            //}
            //catch (SqlException se)
            //{
            //    throw se;
            //}
            //finally
            //{
            //    if (reader != null) reader.Close();
            //    if (theDB != null)
            //    {
            //        DBControl.RemoveDBConection(theDB);
            //    }
            //}

            return list;
        }

        //측정 단위
        public List<CodeMasterData> getUnitValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            //DBConnection theDB = null;
            //SqlDataReader reader = null;
            //string strSql = getSelectSQL();
            //strSql += "  WHERE systemcd  = 'C'               " +
            //          "    AND codeclass = '033'             " +
            //          "    AND code      LIKE '" + code + "' " +
            //          "    AND code      <> 'ZZ'             " +
            //          "    AND disabled  = 'N'                 ";

            //try
            //{
            //    theDB = DBControl.GetConnection();
            //    reader = theDB.ExecuteQuery(strSql);

            //    while (reader.Read())
            //    {
            //        CodeMasterData data = new CodeMasterData();
            //        data.setCodeMasterData(reader);

            //        list.Add(data);
            //    }
            //}
            //catch (SqlException se)
            //{
            //    throw se;
            //}
            //finally
            //{
            //    if (reader != null) reader.Close();
            //    if (theDB != null)
            //    {
            //        DBControl.RemoveDBConection(theDB);
            //    }
            //}

            return list;
        }

        //장비 유형
        public List<CodeMasterData> getEquipmentTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '004'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //반송 유형
        public List<CodeMasterData> getTransferTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '005'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Material Type을 가져오는 메소드
        public void getMaterialTypeCode(string code)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'            " +
                      "    AND codeclass = '006'          " +
                      "    AND code      = '" + code + "' " +
                      "    AND disabled  = 'N'            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
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

        //Material Type List
        public List<CodeMasterData> getMaterialTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '006'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Value Config List
        public List<CodeMasterData> getValueConfigValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '007'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //SAP I/F Type List
        public List<CodeMasterData> getSAPIFTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '008'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Pallet Blocking 상태
        public List<CodeMasterData> getBlockStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '009'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Pallet 반송 상태
        public List<CodeMasterData> getPalletTransferStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '010'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Hirack 반송 상태
        public List<CodeMasterData> getHirackTransferStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '011'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //반송 상태
        public List<CodeMasterData> getTransferStatusValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '012'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Class 별 Random Value 값을 가져오는 메소드
        public void getRandomValueCode(string code)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'            " +
                      "    AND codeclass = '013'          " +
                      "    AND code      = '" + code + "' " +
                      "    AND disabled  = 'N'            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
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

        //Material Type을 가져오는 메소드
        public void getLocationConditionGroupCode(string code)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'            " +
                      "    AND codeclass = '014'          " +
                      "    AND code      = '" + code + "' " +
                      "    AND disabled  = 'N'            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
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

        //Material Type을 가져오는 메소드
        public void getLocationConditionMaxGroupCode()
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'                      " +
                      "    AND codeclass = '014'                    " +
                      "    AND disabled  = 'N'                      " +
                      "    AND code      = (SELECT MAX(code)        " +
                      "                      FROM tb_codemaster     " +
                      "                     WHERE systemcd = 'C'    " +
                      "                       AND codeclass = '014' " +
                      "                       AND code <> 'ZZ'      " +
                      "                       AND disabled = 'N')   ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
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

        //Material Type을 가져오는 메소드
        public int getLocationConditionCode(string code)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();

            int iRow = 0;

            strSql += "  WHERE systemcd  = 'C'            " +
                      "    AND codeclass = '015'          " +
                      "    AND code      = '" + code + "' " +
                      "    AND disabled  = 'N'            ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setCodeMasterData(reader);
                    iRow = 1;
                }
                else
                {
                    iRow = 0;
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

        public List<CodeMasterData> getCompanyTypeValues()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            //DBConnection theDB = null;
            //SqlDataReader reader = null;
            //string strSql = getSelectSQL();
            //strSql += "  WHERE systemcd  = 'C'               " +
            //          "    AND codeclass = '064'             " +
            //          "    AND code      <> 'ZZ'             " +
            //          "    AND disabled  = 'N'                 ";

            //try
            //{
            //    theDB = DBControl.GetConnection();
            //    reader = theDB.ExecuteQuery(strSql);

            //    while (reader.Read())
            //    {
            //        CodeMasterData data = new CodeMasterData();
            //        data.setCodeMasterData(reader);

            //        list.Add(data);
            //    }
            //}
            //catch (SqlException se)
            //{
            //    throw se;
            //}
            //finally
            //{
            //    if (reader != null) reader.Close();
            //    if (theDB != null)
            //    {
            //        DBControl.RemoveDBConection(theDB);
            //    }
            //}

            return list;
        }

        //Pallet Blocking 유형
        public List<CodeMasterData> getPalletBlockingTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '016'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //오류 유형
        public List<CodeMasterData> getErrorTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '017'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //오류 유형
        public List<CodeMasterData> getProgramTypeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '018'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //내부 I/F 시간
        public List<CodeMasterData> getInternalIfTimeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '019'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //External I/F 시간
        public List<CodeMasterData> getExternalIfTimeValues(string code)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '020'             " +
                      "    AND code      LIKE '" + code + "' " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //크레인 작업 상태 확인
        public List<CodeMasterData> getNormalCraneInfo()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '021'             " +
                      "    AND attribute1 = 'Y'              " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Station 관리
        public List<CodeMasterData> getPositionInfoList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '022'             " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //Purge Table 관리
        public List<CodeMasterData> getPurgeTableList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '023'             " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'               ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        //장비 상태 보고 정보
        public List<CodeMasterData> getEQPStatusInfoList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '024'             " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'               " +
                      "  ORDER BY attribute5                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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


        //에러 리스트
        public List<CodeMasterData> getErrorInfoList()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            strSql += "  WHERE systemcd  = 'C'               " +
                      "    AND codeclass = '025'             " +
                      "    AND code      <> 'ZZ'             " +
                      "    AND disabled  = 'N'               " +
                      "  ORDER BY attribute5                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    CodeMasterData data = new CodeMasterData();
                    data.setCodeMasterData(reader);

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

        public void getErrorInfo(string errorcode)
        {
            List<CodeMasterData> list = new List<CodeMasterData>();

            try
            {
                list = getErrorInfoList();

                foreach (CodeMasterData codeTemp in list)
                {
                    if (codeTemp.Code == errorcode)
                    {
                        this.setCodeMasterData(codeTemp);
                    }
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
        }

        public void setCodeMasterData(SqlDataReader reader)
        {
            this.Systemcd = Helper.PreventNull((string)reader["SYSTEMCD"]);
            this.Codeclass = Helper.PreventNull((string)reader["CODECLASS"]);
            this.Code = Helper.PreventNull((string)reader["CODE"]);
            this.Value = Helper.PreventNull((string)reader["VALUE"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);
            this.Remark = Helper.PreventNull((string)reader["REMARK"]);
            this.Attribute1 = Helper.PreventNull((string)reader["ATTRIBUTE1"]);
            this.Attribute2 = Helper.PreventNull((string)reader["ATTRIBUTE2"]);
            this.Attribute3 = Helper.PreventNull((string)reader["ATTRIBUTE3"]);
            this.Attribute4 = Helper.PreventNull((string)reader["ATTRIBUTE4"]);
            this.Attribute5 = Helper.PreventNull((string)reader["ATTRIBUTE5"]);
            this.Attribute6 = Helper.PreventNull((string)reader["ATTRIBUTE6"]);
            this.Attribute7 = Helper.PreventNull((string)reader["ATTRIBUTE7"]);
            this.Attribute8 = Helper.PreventNull((string)reader["ATTRIBUTE8"]);
        }

        public void setCodeMasterData(CodeMasterData code)
        {
            this.Systemcd = Helper.PreventNull(code.Systemcd);
            this.Codeclass = Helper.PreventNull(code.Codeclass);
            this.Code = Helper.PreventNull(code.Code);
            this.Value = Helper.PreventNull(code.Value);
            this.Disabled = Helper.PreventNull(code.Disabled);
            this.Remark = Helper.PreventNull(code.Remark);
            this.Attribute1 = Helper.PreventNull(code.Attribute1);
            this.Attribute2 = Helper.PreventNull(code.Attribute2);
            this.Attribute3 = Helper.PreventNull(code.Attribute3);
            this.Attribute4 = Helper.PreventNull(code.Attribute4);
            this.Attribute5 = Helper.PreventNull(code.Attribute5);
            this.Attribute6 = Helper.PreventNull(code.Attribute6);
            this.Attribute7 = Helper.PreventNull(code.Attribute7);
            this.Attribute8 = Helper.PreventNull(code.Attribute8);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  systemcd                           " +
                    "        ,codeclass                          " +
                    "        ,code                               " +
                    "        ,ISNULL(value     ,'')   value      " +
                    "        ,ISNULL(disabled  ,'')   disabled   " +
                    "        ,ISNULL(remark    ,'')   remark     " +
                    "        ,ISNULL(attribute1,'')   attribute1 " +
                    "        ,ISNULL(attribute2,'')   attribute2 " +
                    "        ,ISNULL(attribute3,'')   attribute3 " +
                    "        ,ISNULL(attribute4,'')   attribute4 " +
                    "        ,ISNULL(attribute5,'')   attribute5 " +
                    "        ,ISNULL(attribute6,'')   attribute6 " +
                    "        ,ISNULL(attribute7,'')   attribute7 " +
                    "        ,ISNULL(attribute8,'')   attribute8 " +
                    "   FROM tb_codemaster                       ";

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_codemaster " +
                    "            ( systemcd     " +
                    "             ,codeclass    " +
                    "             ,code         " +
                    "             ,value        " +
                    "             ,disabled     " +
                    "             ,remark       " +
                    "             ,attribute1   " +
                    "             ,attribute2   " +
                    "             ,attribute3   " +
                    "             ,attribute4   " +
                    "             ,attribute5   " +
                    "             ,attribute6   " +
                    "             ,attribute7   " +
                    "             ,attribute8   " +
                    "            )              " +
                    "      VALUES               " +
                    "            ( @SYSTEMCD    " +
                    "             ,@CODECLASS   " +
                    "             ,@CODE        " +
                    "             ,@VALUE       " +
                    "             ,@DISABLED    " +
                    "             ,@REMARK      " +
                    "             ,@ATTRIBUTE1  " +
                    "             ,@ATTRIBUTE2  " +
                    "             ,@ATTRIBUTE3  " +
                    "             ,@ATTRIBUTE4  " +
                    "             ,@ATTRIBUTE5  " +
                    "             ,@ATTRIBUTE6  " +
                    "             ,@ATTRIBUTE7  " +
                    "             ,@ATTRIBUTE8  " +
                    "            )              ";

            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE tb_codemaster              " +
                    "    SET  systemcd   = @SYSTEMCD    " +
                    "        ,codeclass  = @CODECLASS   " +
                    "        ,code       = @CODE        " +
                    "        ,value      = @VALUE       " +
                    "        ,disabled   = @DISABLED    " +
                    "        ,remark     = @REMARK      " +
                    "        ,attribute1 = @ATTRIBUTE1  " +
                    "        ,attribute2 = @ATTRIBUTE2  " +
                    "        ,attribute3 = @ATTRIBUTE3  " +
                    "        ,attribute4 = @ATTRIBUTE4  " +
                    "        ,attribute5 = @ATTRIBUTE5  " +
                    "        ,attribute6 = @ATTRIBUTE6  " +
                    "        ,attribute7 = @ATTRIBUTE7  " +
                    "        ,attribute8 = @ATTRIBUTE8  " +
                    "  WHERE systemcd    = @SYSTEMCD1   " +
                    "    AND codeclass   = @CODECLASS1  " +
                    "    AND code        = @CODE1       ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_codemaster        " +
                    "  WHERE systemcd    = @SYSTEMCD   " +
                    "    AND codeclass   = @CODECLASS  " +
                    "    AND code        = @CODE       ";

            return strSql;
        }

        public void insertCodeMasterDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                CodeMasterData data = new CodeMasterData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += "  WHERE systemcd    = '" + this.Systemcd  + "'  " +
                          "    AND codeclass   = '" + this.Codeclass + "'  " +
                          "    AND code        = '" + this.Code      + "'  ";

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
                    data.setCodeMasterData(reader);
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
            ifValue += this.Systemcd + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Codeclass + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Code + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Value + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Remark + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute1 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute2 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute3 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute4 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute5 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute6 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute7 + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Attribute8;

            return ifValue;
        }
    }
}
