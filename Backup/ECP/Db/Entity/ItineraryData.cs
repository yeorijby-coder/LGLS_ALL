using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;
using System.Data.SqlClient;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ItineraryData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string pathid = "";

        public string Pathid
        {
            get { return pathid; }
            set { pathid = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string fromlocationid = "";

        public string Fromlocationid
        {
            get { return fromlocationid; }
            set { fromlocationid = value; }
        }
        private string fromsublocationid = "";

        public string Fromsublocationid
        {
            get { return fromsublocationid; }
            set { fromsublocationid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string tolocationid = "";

        public string Tolocationid
        {
            get { return tolocationid; }
            set { tolocationid = value; }
        }
        private string tosublocationid = "";

        public string Tosublocationid
        {
            get { return tosublocationid; }
            set { tosublocationid = value; }
        }
        private string priority = "";

        public string Priority
        {
            get { return priority; }
            set { priority = value; }
        }
        private string status = "";

        public string Status
        {
            get { return status; }
            set { status = value; }
        }
        private string startflag = "";

        public string Startflag
        {
            get { return startflag; }
            set { startflag = value; }
        }

        public ItineraryData()
        {
        }

        public void getItineraryData(string pathid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL(pathid);

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setItineraryData(reader);
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

        public string getSelectSQL(string pathid)
        {
            string strSql = getSelectSQL();
            strSql +="" +
                     " WHERE pathid    = '" + pathid                    + "' ";

            return strSql;

        }

        public void setItineraryData(SqlDataReader reader)
        {
            //this.Plantcode = Helper.PreventNull((string)reader["PLANTCODE"]);
            this.Pathid = Helper.PreventNull((string)reader["PATHID"]);
            this.Fromdeviceid = Helper.PreventNull((string)reader["FROMDEVICEID"]);
            this.Fromlocationid = Helper.PreventNull((string)reader["FROMLOCATIONID"]);
            this.Fromsublocationid = Helper.PreventNull((string)reader["FROMSUBLOCATIONID"]);
            this.Todeviceid = Helper.PreventNull((string)reader["TODEVICEID"]);
            this.Tolocationid = Helper.PreventNull((string)reader["TOLOCATIONID"]);
            this.Tosublocationid = Helper.PreventNull((string)reader["TOSUBLOCATIONID"]);
            //this.Priority = Helper.PreventNull((string)reader["PRIORITY"]);
            //this.Status = Helper.PreventNull((string)reader["STATUS"]);
            this.Startflag = Helper.PreventNull((string)reader["STARTFLAG"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";
            strSql ="" +
                    " SELECT  pathid                       pathid             " +
                    "        ,fromdeviceid                 fromdeviceid       " +
                    "        ,fromlocationid               fromlocationid     " +
                    "        ,ISNULL(fromsublocationid,'') fromsublocationid  " +
                    "        ,todeviceid                   todeviceid         " +
                    "        ,tolocationid                 tolocationid       " +
                    "        ,ISNULL(tosublocationid,'')   tosublocationid    " +
                    "        ,startflag                    startflag          " +
                    "   FROM tb_itinerary                                     ";

            return strSql;
        }
    }
}
