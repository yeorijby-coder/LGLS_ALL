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
    public class EquipmentHistData : ECPObject 
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

        private string dbConnectionString = "";
        public EquipmentHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertEquipmentHistData()
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

                //TB_EquipmentHist 정보  inert                                               
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
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

        public void insertEquipmentHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_EquipmentHist 정보  inert                                               
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("PLANTCODE", Helper.PreventNull(this.Plantcode));
                pss.SetVarChar("EQUIPMENTID", Helper.PreventNull(this.Equipmentid));
                pss.SetVarChar("EQUIPMENTTYPE", Helper.PreventNull(this.Equipmenttype));
                pss.SetVarChar("HOSTID", Helper.PreventNull(this.Hostid));
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

        public void setEquipmentHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Equipmentid = Helper.PreventNull(list[0]);
            this.Equipmenttype = Helper.PreventNull(list[1]);
            this.Hostid = Helper.PreventNull(list[2]);
            this.Assemblyname = Helper.PreventNull(list[3]);
            this.Classname = Helper.PreventNull(list[4]);
            this.Available = Helper.PreventNull(list[5]);
        }

        public EquipmentData setEquipmentData(string[] list)
        {
            EquipmentData data = new EquipmentData();

            data.Equipmentid = Helper.PreventNull(list[0]);
            data.Equipmenttype = Helper.PreventNull(list[1]);
            data.Hostid = Helper.PreventNull(list[2]);
            data.Assemblyname = Helper.PreventNull(list[3]);
            data.Classname = Helper.PreventNull(list[4]);
            data.Available = Helper.PreventNull(list[5]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_equipmenthist " +
                    "            ( backuptime      " +
                    "             ,plantcode       " +
                    "             ,equipmentid     " +
                    "             ,equipmenttype   " +
                    "             ,hostid          " +
                    "             ,assemblyname    " +
                    "             ,classname       " +
                    "             ,available       " +
                    "            )                 " +
                    "      VALUES                  " +
                    "            ( @BACKUPTIME     " +
                    "             ,@PLANTCODE      " +
                    "             ,@EQUIPMENTID    " +
                    "             ,@EQUIPMENTTYPE  " +
                    "             ,@HOSTID         " +
                    "             ,@ASSEMBLYNAME   " +
                    "             ,@CLASSNAME      " +
                    "             ,@AVAILABLE      " +
                    "            )                 ";

            return strSql;
        }

    }
}
