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
    public class EquipmentData : ECPObject 
    {
        private string plantcode = "";
        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }

        private string equipmentid = "";

        public string Equipmentid
        {
            get { return equipmentid; }
            set { equipmentid = value; }
        }
        private string equipmenttype = "";

        public string Equipmenttype
        {
            get { return equipmenttype; }
            set { equipmenttype = value; }
        }
        private string hostid = "";

        public string Hostid
        {
            get { return hostid; }
            set { hostid = value; }
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

        public EquipmentData()
        {
        }

        public void getEquipmentData(string equipmentid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE equipmentid = '" + equipmentid               + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setEquipmentData(reader);
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

        //Program List 炼雀
        public Dictionary<string, EquipmentData> getEquipmentDataList()
        {
            Dictionary<string, EquipmentData> list = new Dictionary<string, EquipmentData>();
            DBConnection theDB = DBControl.GetConnection();
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY equipmentid  ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    EquipmentData eData = new EquipmentData();
                    eData.setEquipmentData(reader);

                    list.Add(eData.Equipmentid, eData);
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

        public void insertEquipmentData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Equipment 沥焊  inert                                               
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));

                //LGLS 林籍 贸府(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertEquipmentDataBackup(theDB, "Insert");
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

        public void updateEquipmentData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Equipment 沥焊  update
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available)); 
                pss.SetVarChar("EQUIPMENTID1", Helper.PreventNull(this.Equipmentid));

                //LGLS 林籍 贸府(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertEquipmentDataBackup(theDB, "Update");
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

        public void deleteEquipmentData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Equipment 沥焊  delete
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));

                //LGLS 林籍 贸府(2009.12.28)
                //if (bkFlag)
                //{
                //    //DataBackup
                //    this.insertEquipmentDataBackup(theDB, "Delete");
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

        public void insertEquipmentData()
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

                //TB_Equipment 沥焊  inert                                               
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));

                //LGLS 林籍 贸府(2009.12.28)
                ////DataBackup
                //this.insertEquipmentDataBackup(theDB, "Insert");

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

        public void updateEquipmentData()
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

                //TB_Equipment 沥焊  update
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available)); 
                pss.SetVarChar("EQUIPMENTID1", Helper.PreventNull(this.Equipmentid));

                //LGLS 林籍 贸府(2009.12.28)
                ////DataBackup
                //this.insertEquipmentDataBackup(theDB, "Update");

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

        public void deleteEquipmentData()
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

                //TB_Equipment 沥焊  delete
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));

                //LGLS 林籍 贸府(2009.12.28)
                ////DataBackup
                //this.insertEquipmentDataBackup(theDB, "Delete");

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

        public void setEquipmentData(SqlDataReader reader)
        {
            this.Equipmentid = Helper.PreventNull((string)reader["EQUIPMENTID"]);
            this.Equipmenttype = Helper.PreventNull((string)reader["EQUIPMENTTYPE"]);
            this.Hostid = Helper.PreventNull((string)reader["HOSTID"]);
            this.Assemblyname = Helper.PreventNull((string)reader["ASSEMBLYNAME"]);
            this.Classname = Helper.PreventNull((string)reader["CLASSNAME"]);
            this.Available = Helper.PreventNull((string)reader["AVAILABLE"]);     
        }
        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  equipmentid                  equipmentid   " +
                    "        ,ISNULL(equipmenttype, '')    equipmenttype " +
                    "        ,ISNULL(hostid       , '')    hostid        " +
                    "        ,ISNULL(assemblyname , '')    assemblyname  " +
                    "        ,ISNULL(classname    , '')    classname     " +
                    "        ,ISNULL(available    , '')    available     " +
                    "   FROM tb_equipment                                ";   

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_equipment    " +
                    "            ( equipmentid    " +
                    "             ,equipmenttype  " +
                    "             ,hostid         " +
                    "             ,assemblyname   " +
                    "             ,classname      " +
                    "             ,available      " +
                    "            )                " +
                    "      VALUES                 " +
                    "            ( @EQUIPMENTID   " +
                    "             ,@EQUIPMENTTYPE " +
                    "             ,@HOSTID        " +
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
                    " UPDATE  tb_equipment                   " +
                    "    SET  equipmentid   = @EQUIPMENTID   " +
                    "        ,equipmenttype = @EQUIPMENTTYPE " +
                    "        ,hostid        = @HOSTID        " +
                    "        ,assemblyname  = @ASSEMBLYNAME  " +
                    "        ,classname     = @CLASSNAME     " +
                    "        ,available     = @AVAILABLE     " +
                    "  WHERE equipmentid    = @EQUIPMENTID1  ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_equipment              " +
                    "  WHERE equipmentid    = @EQUIPMENTID  ";

            return strSql;
        }

        public void insertEquipmentDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                EquipmentData data = new EquipmentData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE equipmentid  = '" + this.Equipmentid + "' ";

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
                    data.setEquipmentData(reader);
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
            ifValue += this.Equipmentid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Equipmenttype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Hostid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Assemblyname + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Classname + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Available;                                    

            return ifValue;
        }
    }
}
