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
    public class FieldbusDriverData : ECPObject
    {
        private string driverno = "";

        public string Driverno
        {
            get { return driverno; }
            set { driverno = value; }
        }
        private string drivertype = "";

        public string Drivertype
        {
            get { return drivertype; }
            set { drivertype = value; }
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
        private string openretrysec = "";

        public string Openretrysec
        {
            get { return openretrysec; }
            set { openretrysec = value; }
        }
        private string timeoutsec = "";

        public string Timeoutsec
        {
            get { return timeoutsec; }
            set { timeoutsec = value; }
        }
        private string connectstring = "";

        public string Connectstring
        {
            get { return connectstring; }
            set { connectstring = value; }
        }
        private string scaninterval = "";

        public string Scaninterval
        {
            get { return scaninterval; }
            set { scaninterval = value; }
        }
        private string useflag = "";

        public string Useflag
        {
            get { return useflag; }
            set { useflag = value; }
        }
        private string description = "";

        public string Description
        {
            get { return description; }
            set { description = value; }
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

        private string dbConnectionString = "";

        public FieldbusDriverData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void getFieldbusDriverData(string useflag)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE useflag    = '" + useflag + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setFieldbusDriverData(reader);
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

        public void setFieldbusDriverData(SqlDataReader reader)
        {
            this.Driverno = Helper.PreventNull((string)reader["DRIVERNO"]);
            this.Drivertype = Helper.PreventNull((string)reader["DRIVERTYPE"]);
            this.Ownerid = Helper.PreventNull((string)reader["OWNERID"]);
            this.Ownertype = Helper.PreventNull((string)reader["OWNERTYPE"]);
            this.Openretrysec = Helper.PreventNull((string)reader["OPENRETRYSEC"]);
            this.Timeoutsec = Helper.PreventNull((string)reader["TIMEOUTSEC"]);
            this.Connectstring = Helper.PreventNull((string)reader["CONNECTSTRING"]);
            this.Scaninterval = Helper.PreventNull((string)reader["SCANINTERVAL"]);
            this.Useflag = Helper.PreventNull((string)reader["USEFLAG"]);
            this.Description = Helper.PreventNull((string)reader["DESCRIPTION"]);
            this.Assemblyname = Helper.PreventNull((string)reader["ASSEMBLYNAME"]);
            this.Classname = Helper.PreventNull((string)reader["CLASSNAME"]);  
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  ISNULL(driverno     , '') driverno      " +
                    "        ,ISNULL(drivertype   , '') drivertype    " +
                    "        ,ISNULL(ownerid      , '') ownerid       " +
                    "        ,ISNULL(ownertype    , '') ownertype     " +
                    "        ,ISNULL(openretrysec , '') openretrysec  " +
                    "        ,ISNULL(timeoutsec   , '') timeoutsec    " +
                    "        ,ISNULL(connectstring, '') connectstring " +
                    "        ,ISNULL(scaninterval , '') scaninterval  " +
                    "        ,ISNULL(useflag      , '') useflag       " +
                    "        ,ISNULL(description  , '') description   " +
                    "        ,ISNULL(assemblyname , '') assemblyname  " +
                    "        ,ISNULL(classname    , '') classname     " +
                    "   FROM tb_fieldbusdriver                        "; 
     
            return strSql;
        }
    }
}
