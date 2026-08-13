using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class TotalPathData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string totalpathid = "";

        public string Totalpathid
        {
            get { return totalpathid; }
            set { totalpathid = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string pathtotal = "";

        public string Pathtotal
        {
            get { return pathtotal; }
            set { pathtotal = value; }
        }

        public TotalPathData()
        {
        }

        public void getTotalPathData(TransferData tfData)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL(tfData);

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TotalPathData totalTemp = new TotalPathData();
                    totalTemp.setTotalPathData(reader);
                    
                    //전체 Path에서 From/To Path 가져오기
                    char[] separator = { ';' };
                    string[] strPathidList = (totalTemp.Pathtotal).Split(separator, StringSplitOptions.RemoveEmptyEntries);

                    string strFromPathid = strPathidList[0];
                    string strToPathid = strPathidList[strPathidList.Length - 1];

                    //FROM/TO path의 세부 Device/Location/SubLocation 가져오기
                    ItineraryData fromItData = new ItineraryData();
                    fromItData.getItineraryData(strFromPathid);

                    ItineraryData toItData = new ItineraryData();
                    toItData.getItineraryData(strToPathid);


                    if (tfData.Fromdeviceid == fromItData.Fromdeviceid &&
                        tfData.Todeviceid == toItData.Todeviceid)
                    {
                        if (tfData.Fromlocationid == fromItData.Fromlocationid &&
                        tfData.Tolocationid == toItData.Tolocationid)
                        {
                            //RACK TO RACK의 경우
                            if (string.IsNullOrEmpty(fromItData.Fromsublocationid) &&
                                string.IsNullOrEmpty(toItData.Tosublocationid))
                            {
                                this.setTotalPathData(reader);
                                break;
                            }


                            if (string.IsNullOrEmpty(fromItData.Fromsublocationid))
                            {
                                if (tfData.Tosublocationid == toItData.Tosublocationid)
                                {
                                    this.setTotalPathData(reader);
                                    break;
                                }
                            }
                            else
                            {
                                if (string.IsNullOrEmpty(toItData.Tosublocationid))
                                {
                                    if (tfData.Fromsublocationid == fromItData.Fromsublocationid)
                                    {
                                        this.setTotalPathData(reader);
                                        break;
                                    }
                                }
                                else
                                {
                                    if (tfData.Fromsublocationid == fromItData.Fromsublocationid &&
                                        tfData.Tosublocationid == toItData.Tosublocationid)
                                    {
                                        this.setTotalPathData(reader);
                                        break;
                                    }
                                }
                            }
                        }
                    }
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

        public void setTotalPathData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Totalpathid = Helper.PreventNull((string)reader["TOTALPATHID"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Pathtotal = Helper.PreventNull((string)reader["PATHTOTAL"]);
        }

        public string getSelectSQL(TransferData tfData)
        {
            string strSql = "";
            strSql = getSelectSQL();

            if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
            {
                strSql += " WHERE fromdeviceid = '" + tfData.Fromdeviceid       + "' " +
                          "   AND todeviceid   = '" + tfData.Todeviceid         + "' " + 
                          "   AND totalpathid LIKE 'TI%'                             ";
            }
            else if (tfData.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
            {
                strSql += " WHERE fromdeviceid = '" + tfData.Fromdeviceid       + "' " +
                          "   AND todeviceid   = '" + tfData.Todeviceid         + "'   " +
                          "   AND totalpathid LIKE 'TO%'                       ";
            }
            else
            {
                strSql += " WHERE fromdeviceid = '" + tfData.Fromdeviceid       + "' " +
                          "   AND todeviceid   = '" + tfData.Todeviceid         + "' " +
                          "   AND totalpathid LIKE 'TM%'                             ";
            }

            return strSql;
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql = "" +
                    " SELECT  totalpathid  " +
                    "        ,fromdeviceid " +
                    "        ,todeviceid   " +
                    "        ,pathtotal    " +
                    "   FROM tb_totalpath  ";

            return strSql;
        }
    }
}
