using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Global;
using ECP.Util;

namespace ECP.BizRule
{
    [Serializable]
    public class TransferScheduleTime : ECPObject 
    {
        private string dbConnectionString = "";
        public TransferScheduleTime()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public int getAverageScheduledTime(TransferData data)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string fromDevice = data.Fromdeviceid;
            string fromLocation = data.Fromlocationid;
            string fromSubLocation = data.Fromsublocationid;

            string toDevice = data.Todeviceid;
            string toLocation = data.Tolocationid;
            string toSubLocation = data.Tosublocationid;

            string substrFromLocation = (data.Fromlocationid).Substring(0, 5);
            string substrToLocation = (data.Tolocationid).Substring(0, 5);

            int iCnt = 0;

            if (substrFromLocation == GlobalConstant.SUBSYSTEM_RACK)
            {
                fromSubLocation = "";
            }

            if (substrToLocation == GlobalConstant.SUBSYSTEM_RACK)
            {
                toSubLocation = "";
            }

            string strSql = getTransferScheduledSQL();
            strSql += " WHERE fromdeviceid      = '" + fromDevice + "' " +
                      "   AND fromlocationid    = '" + fromLocation + "' " +
                      "   AND fromsublocationid = '" + fromSubLocation + "' " +
                      "   AND todeviceid        = '" + toDevice + "' " +
                      "   AND tolocationid      = '" + toLocation + "' " +
                      "   AND tosublocationid   = '" + toSubLocation + "' ";

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["SCHEDULEDTIME"];
                }
            }
            catch (SqlException se)
            {
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

        public List<string[]> getAverageScheduledTimeList()
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            List<string[]> list = new List<string[]>();

            string strSql = getTransferScheduledSQL();

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    int iCnt = 0;
                    string[] strList = new string[7];

                    strList[0] = (string)reader["FROMDEVICEID"];
                    strList[1] = (string)reader["FROMLOCATIONID"];
                    strList[2] = (string)reader["FROMSUBLOCATIONID"];
                    strList[3] = (string)reader["TODEVICEID"];
                    strList[4] = (string)reader["TOLOCATIONID"];
                    strList[5] = (string)reader["TOSUBLOCATIONID"];
                    
                    iCnt = (int)reader["scheduledtime"];
                    strList[6] = iCnt.ToString();

                    list.Add(strList);
                }
            }
            catch (SqlException se)
            {
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

        public string getTransferScheduledSQL()
        {
            string strSql = "";

            TimeSpan avgTime = TimeSpan.FromDays(double.Parse(GlobalConstant.TACKTIME_INTERVAL_DAY));
            DateTime nowTime = DateTime.Now;
            DateTime preTime = nowTime.Subtract(avgTime);

            string strPreTime = preTime.ToString("yyyyMMddHHmmssfff");
            string strNowTime = nowTime.ToString("yyyyMMddHHmmssfff");

            strSql = " " +
                    "  SELECT  fromdeviceid                                                                       " +
                    "         ,fromlocationid                                                                     " +
                    "         ,fromsublocationid                                                                  " +
                    "         ,todeviceid                                                                         " +
                    "         ,tolocationid                                                                       " +
                    "         ,tosublocationid                                                                    " +
                    "         ,scheduledtime                                                                      " +
                    "    FROM                                                                                     " +
                    "        (SELECT  fromdeviceid                                                                " +
                    "                ,fromlocationid                                                              " +
                    "                ,fromsublocationid                                                           " +
                    "                ,todeviceid                                                                  " +
                    "                ,tolocationid                                                                " +
                    "                ,tosublocationid                                                             " +
                    "                ,AVG(scheduledtime) scheduledtime                                            " +
                    "           FROM                                                                              " +
                    "               (SELECT  fromdeviceid                                                         " +
                    "                       ,fromlocationid                                                       " +
                    "                       ,fromsublocationid                                                    " +
                    "                       ,todeviceid                                                           " +
                    "                       ,tolocationid                                                         " +
                    "                       ,tosublocationid                                                      " +
                    "                       ,DATEDIFF(second                                                      " +
                    "                                 ,CAST(calexecutetime AS DATETIME)                           " +
                    "                                 ,CAST(calcompletetime AS DATETIME)                          " +
                    "                                ) scheduledtime                                              " +
                    "                  FROM                                                                       " +
                    "                       (SELECT  fromdeviceid                                                 " +
                    "                               ,fromlocationid                                               " +
                    "                               ,CASE                                                         " +
                    "                                     WHEN SUBSTRING(fromlocationid,1,4) = 'RACK' THEN ''     " +
                    "                                     ELSE fromsublocationid                                  " +
                    "                                END                          fromsublocationid               " +
                    "                               ,todeviceid                                                   " +
                    "                               ,tolocationid                                                 " +
                    "                               ,CASE                                                         " +
                    "                                     WHEN SUBSTRING(tolocationid,1,4) = 'RACK' THEN ''       " +
                    "                                     ELSE tosublocationid                                    " +
                    "                                END                          tosublocationid                 " +
                    "                               ,SUBSTRING(MAX(executetime),1,4)  + '-' +                     " +
                    "                                SUBSTRING(MAX(executetime),5,2)  + '-' +                     " +
                    "                                SUBSTRING(MAX(executetime),7,2)  + ' ' +                     " +
                    "                                SUBSTRING(MAX(executetime),9,2)  + ':' +                     " +
                    "                                SUBSTRING(MAX(executetime),11,2) + ':' +                     " +
                    "                                SUBSTRING(MAX(executetime),13,2)                             " +
                    "                                calexecutetime                                               " +
                    "                               ,SUBSTRING(MAX(completetime),1,4)  + '-' +                    " +
                    "                                SUBSTRING(MAX(completetime),5,2)  + '-' +                    " +
                    "                                SUBSTRING(MAX(completetime),7,2)  + ' ' +                    " +
                    "                                SUBSTRING(MAX(completetime),9,2)  + ':' +                    " +
                    "                                SUBSTRING(MAX(completetime),11,2) + ':' +                    " +
                    "                                SUBSTRING(MAX(completetime),13,2)                            " +
                    "                                calcompletetime                                              " +
                    "                          FROM tb_transferhist                                               " +
                    "                         WHERE (executetime <> '' AND executetime is not null)               " +
                    "                           AND (completetime <> '' AND completetime is not null)             " +
                    "                           AND (executetime BETWEEN '" + preTime + "' AND '" + nowTime + "') " +
                    "                          GROUP BY  fromdeviceid                                             " +
                    "                                   ,fromlocationid                                           " +
                    "                                   ,fromsublocationid                                        " +
                    "                                   ,todeviceid                                               " +
                    "                                   ,tolocationid                                             " +
                    "                                   ,tosublocationid                                          " +
                    "                       ) a                                                                   " +
                    "               ) b                                                                           " +
                    "         WHERE scheduledtime > 0                                                             " +
                    "         GROUP BY  fromdeviceid                                                              " +
                    "                  ,fromlocationid                                                            " +
                    "                  ,fromsublocationid                                                         " +
                    "                  ,todeviceid                                                                " +
                    "                  ,tolocationid                                                              " +
                    "                  ,tosublocationid                                                           " +
                    "        ) c                                                                                  ";

            return strSql;
        }
    }
}
