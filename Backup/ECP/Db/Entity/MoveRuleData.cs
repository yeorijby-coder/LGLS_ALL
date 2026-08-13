using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class MoveRuleData : ECPObject 
    {
        private string ruleid = "";

        public string Ruleid
        {
            get { return ruleid; }
            set { ruleid = value; }
        }
        private string rulename = "";

        public string Rulename
        {
            get { return rulename; }
            set { rulename = value; }
        }
        private string reftable = "";

        public string Reftable
        {
            get { return reftable; }
            set { reftable = value; }
        }
        private string ruleproperty = "";

        public string Ruleproperty
        {
            get { return ruleproperty; }
            set { ruleproperty = value; }
        }
        private string rulevalue = "";

        public string Rulevalue
        {
            get { return rulevalue; }
            set { rulevalue = value; }
        }

        private string ruledescription = "";

        public string Ruledescription
        {
            get { return ruledescription; }
            set { ruledescription = value; }
        }

        private string priority = "";

        public string Priority
        {
            get { return priority; }
            set { priority = value; }
        }
        private string disabled = "";

        public string Disabled
        {
            get { return disabled; }
            set { disabled = value; }
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

        public MoveRuleData()
        {
        }

        //Program List 조회
        public Dictionary<string, MoveRuleData> getMoveRuleDataList()
        {
            Dictionary<string, MoveRuleData> list = new Dictionary<string, MoveRuleData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY ruleid   ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    MoveRuleData data = new MoveRuleData();
                    data.setMoveRuleData(reader);

                    list.Add(data.ruleid, data);
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
        public Dictionary<string, MoveRuleData> getMoveRuleDataListOrderPriority()
        {
            Dictionary<string, MoveRuleData> list = new Dictionary<string, MoveRuleData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY priority   ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    MoveRuleData data = new MoveRuleData();
                    data.setMoveRuleData(reader);

                    list.Add(data.ruleid, data);
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

        public void insertMoveRuleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_MOVERULE 정보  inert
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertMoveRuleDataBackup(theDB, "Insert");
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

        public void updateMoveRuleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_MoveRule 정보  update
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));

                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertMoveRuleDataBackup(theDB, "Update");
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

        public void deleteMoveRuleData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_MoveRule 정보  delete
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));

                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertMoveRuleDataBackup(theDB, "Delete");
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

        public void insertMoveRuleData()
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

                //TB_MOVERULE 정보  inert
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));


                ////DataBackup
                //this.insertMoveRuleDataBackup(theDB, "Insert");

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

        public void updateMoveRuleData()
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

                //TB_MoveRule 정보  update
                pss.SetVarChar("RULENAME", Helper.PreventNull(this.Rulename));
                pss.SetVarChar("REFTABLE", Helper.PreventNull(this.Reftable));
                pss.SetVarChar("RULEPROPERTY", Helper.PreventNull(this.Ruleproperty));
                pss.SetVarChar("RULEVALUE", Helper.PreventNull(this.Rulevalue));
                pss.SetVarChar("RULEDESCRIPTION", Helper.PreventNull(this.Ruledescription));
                pss.SetVarChar("PRIORITY", Helper.PreventNull(this.Priority));
                pss.SetVarChar("DISABLED", Helper.PreventNull(this.Disabled));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));
                
                ////DataBackup
                //this.insertMoveRuleDataBackup(theDB, "Update");

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

        public void deleteMoveRuleData()
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

                //TB_MoveRule 정보  delete
                pss.SetVarChar("RULEID", Helper.PreventNull(this.Ruleid));

                ////DataBackup
                //this.insertMoveRuleDataBackup(theDB, "Delete");

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

        public void setMoveRuleData(SqlDataReader reader)
        {
            this.Ruleid = Helper.PreventNull((string)reader["RULEID"]);
            this.Rulename = Helper.PreventNull((string)reader["RULENAME"]);
            this.Reftable = Helper.PreventNull((string)reader["REFTABLE"]);
            this.Ruleproperty = Helper.PreventNull((string)reader["RULEPROPERTY"]);
            this.Rulevalue = Helper.PreventNull((string)reader["RULEVALUE"]);
            this.Ruledescription = Helper.PreventNull((string)reader["RULEDESCRIPTION"]);
            this.Priority = Helper.PreventNull((string)reader["PRIORITY"]);
            this.Disabled = Helper.PreventNull((string)reader["DISABLED"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);     
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT ruleid                       ruleid          " +
                    "       ,ISNULL(rulename       ,'')   rulename        " +
                    "       ,ISNULL(reftable       ,'')   reftable        " +
                    "       ,ISNULL(ruleproperty   ,'')   ruleproperty    " +
                    "       ,ISNULL(rulevalue      ,'')   rulevalue       " +
                    "       ,ISNULL(ruledescription,'')   ruledescription " +
                    "       ,ISNULL(priority       ,'')   priority        " +
                    "       ,ISNULL(disabled       ,'')   disabled        " +
                    "       ,ISNULL(createtime     ,'')   createtime      " +
                    "       ,ISNULL(createman      ,'')   createman       " +
                    "       ,ISNULL(updatetime     ,'')   updatetime      " +
                    "       ,ISNULL(updateman      ,'')   updateman       " +
                    "   FROM tb_moverule                                  "; 

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_moverule        " +
                    "            ( ruleid            " +
                    "             ,rulename          " +
                    "             ,reftable          " +
                    "             ,ruleproperty      " +
                    "             ,rulevalue         " +
                    "             ,ruledescription   " +
                    "             ,priority          " +
                    "             ,disabled          " +
                    "             ,createtime        " +
                    "             ,createman         " +
                    "             ,updatetime        " +
                    "             ,updateman         " +
                    "            )                   " +
                    "      VALUES                    " +
                    "            ( @RULEID           " +
                    "             ,@RULENAME         " +
                    "             ,@REFTABLE         " +
                    "             ,@RULEPROPERTY     " +
                    "             ,@RULEVALUE        " +
                    "             ,@RULEDESCRIPTION  " +
                    "             ,@PRIORITY         " +
                    "             ,@DISABLED         " +
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
                    " UPDATE  tb_moverule                        " +
                    "    SET  rulename        = @RULENAME        " +
                    "        ,reftable        = @REFTABLE        " +
                    "        ,ruleproperty    = @RULEPROPERTY    " +
                    "        ,rulevalue       = @RULEVALUE       " +
                    "        ,ruledescription = @RULEDESCRIPTION " +
                    "        ,priority        = @PRIORITY        " +
                    "        ,disabled        = @DISABLED        " +
                    "        ,createtime      = @CREATETIME      " +
                    "        ,createman       = @CREATEMAN       " +
                    "        ,updatetime      = @UPDATETIME      " +
                    "        ,updateman       = @UPDATEMAN       " +
                    "  WHERE ruleid           = @RULEID          ";        

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_moverule   " +
                    "  WHERE ruleid = @RULEID   "; 

            return strSql;
        }

        public void insertMoveRuleDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                MoveRuleData data = new MoveRuleData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE ruleid  = '" + this.Ruleid + "' ";

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
                    data.setMoveRuleData(reader);
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
            ifValue += this.Ruleid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Rulename + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Reftable + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Ruleproperty + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Rulevalue + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Ruledescription + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Priority + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Disabled + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updateman;

            return ifValue;
        }
    }
}
