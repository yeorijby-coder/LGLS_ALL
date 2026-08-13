using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using System.Data.SqlClient;
using ECP.Global;
using ECP.Db;
using ECP.Util;

namespace ECP.BizRule
{
    [Serializable]
    public class CheckEquipmentStatus : ECPObject
    {
        public CheckEquipmentStatus()
        {
        }

        public string createWmsStatusInfo()
        {
            List<CodeMasterData> list = new List<CodeMasterData>();

            string totalStatus = "";
            
            CodeMasterData codeData = new CodeMasterData();
            list = codeData.getEQPStatusInfoList();

            foreach (CodeMasterData codeTemp in list)
            {
                string strTemp = "";

                strTemp = getEQPStatus(codeTemp);

                if (string.IsNullOrEmpty(strTemp))
                {
                    strTemp = "0";
                }

                if (codeTemp.Attribute4 == "Y")
                {
                    totalStatus += codeTemp.Code + strTemp + "0000";
                }
                else
                {
                    totalStatus += strTemp;
                }
            }

            return totalStatus;
        }

        public string getEQPStatus(CodeMasterData cdData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            string strStatus = "";
            string strSql = "";
            strSql += "  SELECT available " +
                      "    FROM " + cdData.Attribute1 + " " +
                      "   WHERE " + cdData.Attribute2 + " = '" + cdData.Attribute3 + "' "; 

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    strStatus = Helper.PreventNull((string)reader["AVAILABLE"]);
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

            return strStatus;
        }
    }
}
