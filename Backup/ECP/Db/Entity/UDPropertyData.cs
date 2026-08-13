using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class UDPropertyData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string entityid = "";

        public string Entityid
        {
            get { return entityid; }
            set { entityid = value; }
        }
        private string udpropertycd = "";

        public string Udpropertycd
        {
            get { return udpropertycd; }
            set { udpropertycd = value; }
        }
        private string value = "";

        public string Value
        {
            get { return this.value; }
            set { this.value = value; }
        }
        private string testvalue = "";

        public string Testvalue
        {
            get { return testvalue; }
            set { testvalue = value; }
        }

        public string[] getUDPropertyEntityId(string plantcode, string udpropertycd, string value)
        {
            List<string> list = new List<string>();
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = " SELECT *                                    " +
                            "   FROM tb_udproperty                        " +
                            "  WHERE plantcode = '" + plantcode + "'      " +
                            "    AND udpropertycd = '" + udpropertycd + "'  " +
                            "    AND value = '" + value + "'              ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    //UDPropertyData udpData = new UDPropertyData();

                    //udpData.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
                    //udpData.Entityid = Helper.PreventNull((string)reader["ENTITYID"]);
                    //udpData.Udpropertycd = Helper.PreventNull((string)reader["UDPROPERTYCD"]);
                    //udpData.Value = Helper.PreventNull((string)reader["VALUE"]);
                    //udpData.Testvalue = Helper.PreventNull((string)reader["TESTVALUE"]);

                    list.Add(Helper.PreventNull((string)reader["ENTITYID"]));
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

            return list.ToArray();
        }

        public int getUdpropertyValue(string plantcode, string entityid, string udpropertycd)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strSql = getSelectSQL();
            int iRow = 0;

            strSql += "  WHERE plantcode    = '" + plantcode    + "'  " +
                      "    AND entityid     = '" + entityid     + "'  " +
                      "    AND udpropertycd = '" + udpropertycd + "'  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);
                if (reader.Read())
                {
                    setUDPropertyData(reader);
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

        public void setUDPropertyData(SqlDataReader reader)
        {
            this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Entityid = Helper.PreventNull((string)reader["ENTITYID"]);
            this.Udpropertycd = Helper.PreventNull((string)reader["UDPROPERTYCD"]);
            this.Value = Helper.PreventNull((string)reader["VALUE"]);
            this.Testvalue = Helper.PreventNull((string)reader["TESTVALUE"]);             
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  plantcode                          " +
                    "        ,entityid                           " +
                    "        ,udpropertycd                       " +
                    "        ,ISNULL(value    ,'')   value       " +
                    "        ,ISNULL(testvalue,'')   testvalue   " +
                    "   FROM tb_udproperty                       ";

            return strSql;
        }
    }
}
