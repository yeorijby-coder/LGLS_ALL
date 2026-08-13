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
    public class SubSystemData : ECPObject 
    {
        private string subsystemid = "";

        public string Subsystemid
        {
            get { return subsystemid; }
            set { subsystemid = value; }
        }
        private string subsystemtype = "";

        public string Subsystemtype
        {
            get { return subsystemtype; }
            set { subsystemtype = value; }
        }
        private string hostid = "";

        public string Hostid
        {
            get { return hostid; }
            set { hostid = value; }
        }
        private string ownerid = "";

        public string Ownerid
        {
            get { return ownerid; }
            set { ownerid = value; }
        }
        private string ownertype = "";

        public string Ownertype
        {
            get { return ownertype; }
            set { ownertype = value; }
        }
        private string assemblyname = "";

        public string Assemblyname
        {
            get { return assemblyname; }
            set { assemblyname = value; }
        }
        private string classname = "";

        public string Classname
        {
            get { return classname; }
            set { classname = value; }
        }
        private string available = "";

        public string Available
        {
            get { return available; }
            set { available = value; }
        }

        public SubSystemData()
        {
        }

        public void getSubSystemData(string subsystemid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE subsystemid = '" + subsystemid + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setSubSystemData(reader);
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

        public int getStatusTotalByEqp(string equipmentid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            
            int iCnt = 0;
            string strSql = "";
            strSql += " SELECT SUM(CONVERT(int, (ISNULL(AVAILABLE, '0'))))  CNT " +
                      "   FROM tb_subsystem                                     " +
                      "  WHERE ownerid = '" + equipmentid + "'                  ";

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

        //Program List 조회
        public Dictionary<string, SubSystemData> getSubSystemDataList()
        {
            Dictionary<string, SubSystemData> list = new Dictionary<string, SubSystemData>();
            DBConnection theDB = DBControl.GetConnection();
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY subsystemid  ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    SubSystemData sData = new SubSystemData();
                    sData.setSubSystemData(reader);

                    list.Add(sData.Subsystemid, sData);
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

        public void insertSubSystemData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_SUBSYSTEM 정보
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));
                pss.SetVarChar("SUBSYSTEMTYPE", Helper.PreventNull(this.Subsystemtype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("OWNERTYPE", Helper.PreventNull(this.Ownertype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void updateSubSystemData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_SUBSYSTEM 정보  update
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));
                pss.SetVarChar("SUBSYSTEMTYPE", Helper.PreventNull(this.Subsystemtype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("OWNERTYPE", Helper.PreventNull(this.Ownertype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));
                pss.SetVarChar("SUBSYSTEMID1", Helper.PreventNull(this.Subsystemid));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteSubSystemData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Equipment 정보  delete
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertSubSystemData()
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

                //TB_SUBSYSTEM 정보  inert                                               
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));
                pss.SetVarChar("SUBSYSTEMTYPE", Helper.PreventNull(this.Subsystemtype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("OWNERTYPE", Helper.PreventNull(this.Ownertype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));

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

        public void updateSubSystemData()
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

                //TB_SUBSYSTEM 정보  update
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));
                pss.SetVarChar("SUBSYSTEMTYPE", Helper.PreventNull(this.Subsystemtype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("OWNERTYPE", Helper.PreventNull(this.Ownertype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));
                pss.SetVarChar("SUBSYSTEMID1", Helper.PreventNull(this.Subsystemid));

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

        public void deleteSubSystemData()
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

                //TB_subsystem 정보  delete
                pss.SetVarChar("SUBSYSTEMID", Helper.PreventNull(this.Subsystemid));

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

        public void setSubSystemData(SqlDataReader reader)
        {
            this.Subsystemid = Helper.PreventNull((string)reader["SUBSYSTEMID"]);
            this.Subsystemtype = Helper.PreventNull((string)reader["SUBSYSTEMTYPE"]);
            this.Hostid = Helper.PreventNull((string)reader["HOSTID"]);
            this.Ownerid = Helper.PreventNull((string)reader["OWNERID"]);
            this.Ownertype = Helper.PreventNull((string)reader["OWNERTYPE"]);
            this.Assemblyname = Helper.PreventNull((string)reader["ASSEMBLYNAME"]);
            this.Classname = Helper.PreventNull((string)reader["CLASSNAME"]);
            this.Available = Helper.PreventNull((string)reader["AVAILABLE"]); 
        }
        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  subsystemid                  subsystemid    " +
                    "        ,ISNULL(subsystemtype , '')   subsystemtype  " +
                    "        ,ISNULL(hostid        , '')   hostid         " +
                    "        ,ISNULL(ownerid       , '')   ownerid        " +
                    "        ,ISNULL(ownertype     , '')   ownertype      " +
                    "        ,ISNULL(assemblyname  , '')   assemblyname   " +
                    "        ,ISNULL(classname     , '')   classname      " +
                    "        ,ISNULL(available     , '')   available      " +
                    "   FROM tb_subsystem                                 ";   


            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_subsystem    " +
                    "            ( subsystemid    " +
                    "             ,subsystemtype  " +
                    "             ,hostid         " +
                    "             ,ownerid        " +
                    "             ,ownertype      " +
                    "             ,assemblyname   " +
                    "             ,classname      " +
                    "             ,available      " +
                    "            )                " +
                    "      VALUES                 " +
                    "            ( @SUBSYSTEMID   " +
                    "             ,@SUBSYSTEMTYPE " +
                    "             ,@HOSTID        " +
                    "             ,@OWNERID       " +
                    "             ,@OWNERTYPE     " +
                    "             ,@ASSEMBLYNAME  " +
                    "             ,@CLASSNAME     " +
                    "             ,@AVAILABLE     " +
                    "            )                ";
            return strSql;
        }

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_subsystem                     " +
                    "    SET  subsystemid     = @SUBSYSTEMID   " +
                    "        ,subsystemtype   = @SUBSYSTEMTYPE " +
                    "        ,hostid          = @HOSTID        " +
                    "        ,ownerid         = @OWNERID       " +
                    "        ,ownertype       = @OWNERTYPE     " +
                    "        ,assemblyname    = @ASSEMBLYNAME  " +
                    "        ,classname       = @CLASSNAME     " +
                    "        ,available       = @AVAILABLE     " +
                    "  WHERE   subsystemid    = @SUBSYSTEMID1  ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_subsystem              " +
                    "  WHERE subsystemid    = @SUBSYSTEMID  ";

            return strSql;
        }
    }
}
