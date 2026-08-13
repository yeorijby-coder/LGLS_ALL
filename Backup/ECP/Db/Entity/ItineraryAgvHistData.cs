using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ItineraryAgvHistData : ECPObject 
    {
        private string backuptime = "";

        public string Backuptime
        {
            get { return backuptime; }
            set { backuptime = value; }
        }
        private string routeid = "";

        public string Routeid
        {
            get { return routeid; }
            set { routeid = value; }
        }
        private string frommarkplate = "";

        public string Frommarkplate
        {
            get { return frommarkplate; }
            set { frommarkplate = value; }
        }
        private string tomarkplate = "";

        public string Tomarkplate
        {
            get { return tomarkplate; }
            set { tomarkplate = value; }
        }
        private string distance = "";

        public string Distance
        {
            get { return distance; }
            set { distance = value; }
        }
        private string routetype = "";

        public string Routetype
        {
            get { return routetype; }
            set { routetype = value; }
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

        private string dbConnectionString = "";
        public ItineraryAgvHistData()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        public void insertItineraryAgvHistData()
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

                //TB_Itinerary 정보  insert                                             
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("ROUTEID", Helper.PreventNull(this.Routeid));
                pss.SetVarChar("FROMMARKPLATE", Helper.PreventNull(this.Frommarkplate));
                pss.SetVarChar("TOMARKPLATE", Helper.PreventNull(this.Tomarkplate));
                pss.SetVarChar("DISTANCE", Helper.PreventNull(this.Distance));
                pss.SetVarChar("ROUTETYPE", Helper.PreventNull(this.Routetype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));    

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

        public void insertItineraryAgvHistData(DBConnection theDB)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Itinerary 정보  insert                                             
                pss.SetVarChar("BACKUPTIME", Helper.PreventNull(this.Backuptime));
                pss.SetVarChar("ROUTEID", Helper.PreventNull(this.Routeid));
                pss.SetVarChar("FROMMARKPLATE", Helper.PreventNull(this.Frommarkplate));
                pss.SetVarChar("TOMARKPLATE", Helper.PreventNull(this.Tomarkplate));
                pss.SetVarChar("DISTANCE", Helper.PreventNull(this.Distance));
                pss.SetVarChar("ROUTETYPE", Helper.PreventNull(this.Routetype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void setItineraryAgvHistData(string[] list)
        {
            this.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            this.Routeid = Helper.PreventNull(list[0]);
            this.Frommarkplate = Helper.PreventNull(list[1]);
            this.Tomarkplate = Helper.PreventNull(list[2]);
            this.Distance = Helper.PreventNull(list[3]);
            this.Routetype = Helper.PreventNull(list[4]);
            this.Createtime = Helper.PreventNull(list[5]);
            this.Createman = Helper.PreventNull(list[6]);
            this.Updatetime = Helper.PreventNull(list[7]);
            this.Updateman = Helper.PreventNull(list[8]);
        }

        public ItineraryAgvData setItineraryAgvData(string[] list)
        {
            ItineraryAgvData data = new ItineraryAgvData();

            data.Routeid = Helper.PreventNull(list[0]);
            data.Frommarkplate = Helper.PreventNull(list[1]);
            data.Tomarkplate = Helper.PreventNull(list[2]);
            data.Distance = Helper.PreventNull(list[3]);
            data.Routetype = Helper.PreventNull(list[4]);
            data.Createtime = Helper.PreventNull(list[5]);
            data.Createman = Helper.PreventNull(list[6]);
            data.Updatetime = Helper.PreventNull(list[7]);
            data.Updateman = Helper.PreventNull(list[8]);

            return data;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_itineraryagv  " +
                    "            (backuptime       " +
                    "            ,routeid          " +
                    "            ,frommarkplate    " +
                    "            ,tomarkplate      " +
                    "            ,distance         " +
                    "            ,routetype        " +
                    "            ,createtime       " +
                    "            ,createman        " +
                    "            ,updatetime       " +
                    "            ,updateman        " +
                    "            )                 " +
                    "      VALUES                  " +
                    "            (@BACKUPTIME      " +
                    "            ,@ROUTEID         " +
                    "            ,@FROMMARKPLATE   " +
                    "            ,@TOMARKPLATE     " +
                    "            ,@DISTANCE        " +
                    "            ,@ROUTETYPE       " +
                    "            ,@CREATETIME      " +
                    "            ,@CREATEMAN       " +
                    "            ,@UPDATETIME      " +
                    "            ,@UPDATEMAN       " +
                    "            )                 ";    

            return strSql;
        }
    }
}
