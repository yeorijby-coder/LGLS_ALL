using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using ECP.Util;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class ItineraryAgvData : ECPObject 
    {
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

        public ItineraryAgvData()
        {
        }

        //ItineraryAgv List 조회
        public Dictionary<string, ItineraryAgvData> getItineraryAgvDataDictionary()
        {
            Dictionary<string, ItineraryAgvData> list = new Dictionary<string, ItineraryAgvData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY routeid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ItineraryAgvData data = new ItineraryAgvData();
                    data.setItineraryAgvData(reader);

                    list.Add(data.Routeid, data);
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

        //ItineraryAgv List 조회
        public List<ItineraryAgvData> getItineraryAgvDataList()
        {
            List<ItineraryAgvData> list = new List<ItineraryAgvData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY routeid  ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ItineraryAgvData data = new ItineraryAgvData();
                    data.setItineraryAgvData(reader);

                    list.Add(data);
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

        //ItineraryAgv List 조회
        public void getItineraryAgvData(string routeid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  Where routeid = '" + routeid + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    this.setItineraryAgvData(reader);
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

        //ItineraryAgv List 조회
        public Dictionary<string, ItineraryAgvData> getItineraryAgvDataDictionary(string frommarkplate)
        {
            Dictionary<string, ItineraryAgvData> list = new Dictionary<string, ItineraryAgvData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += " WHERE frommarkplate = '" + frommarkplate + "' " +
                      "  ORDER BY routeid                             ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ItineraryAgvData data = new ItineraryAgvData();
                    data.setItineraryAgvData(reader);

                    list.Add(data.Routeid, data);
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

        //ItineraryAgv List 조회
        public List<ItineraryAgvData> getItineraryAgvDataList(string frommarkplate)
        {
            List<ItineraryAgvData> list = new List<ItineraryAgvData>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += " WHERE frommarkplate = '" + frommarkplate + "' " +
                      "  ORDER BY routeid                             ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    ItineraryAgvData data = new ItineraryAgvData();
                    data.setItineraryAgvData(reader);

                    list.Add(data);
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

        public void insertItineraryAgvData()
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

                //TB_Itinerary 정보  insert                                             
                pss.SetVarChar("ROUTEID", Helper.PreventNull(this.Routeid));
                pss.SetVarChar("FROMMARKPLATE", Helper.PreventNull(this.Frommarkplate));
                pss.SetVarChar("TOMARKPLATE", Helper.PreventNull(this.Tomarkplate));
                pss.SetVarChar("DISTANCE", Helper.PreventNull(this.Distance));
                pss.SetVarChar("ROUTETYPE", Helper.PreventNull(this.Routetype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));    

                //DataBackup
                this.insertItineraryAgvDataBackup(theDB, "Insert");

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

        public void updateItineraryAgvData()
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

                //TB_Itinerary 정보  update                                           
                pss.SetVarChar("ROUTEID", Helper.PreventNull(this.Routeid));
                pss.SetVarChar("FROMMARKPLATE", Helper.PreventNull(this.Frommarkplate));
                pss.SetVarChar("TOMARKPLATE", Helper.PreventNull(this.Tomarkplate));
                pss.SetVarChar("DISTANCE", Helper.PreventNull(this.Distance));
                pss.SetVarChar("ROUTETYPE", Helper.PreventNull(this.Routetype));
                pss.SetVarChar("CREATETIME", Helper.PreventNull(this.Createtime));
                pss.SetVarChar("CREATEMAN", Helper.PreventNull(this.Createman));
                pss.SetVarChar("UPDATETIME", Helper.PreventNull(this.Updatetime));
                pss.SetVarChar("UPDATEMAN", Helper.PreventNull(this.Updateman));
                pss.SetVarChar("ROUTEID1", Helper.PreventNull(this.Routeid));
                
                //DataBackup
                this.insertItineraryAgvDataBackup(theDB, "Update");

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

        public void deleteItineraryAgvData()
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

                //TB_Itinerary 정보  delete                                        
                pss.SetVarChar("ROUTEID", Helper.PreventNull(this.Routeid));

                //DataBackup
                this.insertItineraryAgvDataBackup(theDB, "Delete");

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

        public void setItineraryAgvData(SqlDataReader reader)
        {
            this.Routeid = Helper.PreventNull((string)reader["ROUTEID"]);
            this.Frommarkplate = Helper.PreventNull((string)reader["FROMMARKPLATE"]);
            this.Tomarkplate = Helper.PreventNull((string)reader["TOMARKPLATE"]);
            this.Distance = Helper.PreventNull((string)reader["DISTANCE"]);
            this.Routetype = Helper.PreventNull((string)reader["ROUTETYPE"]);
            this.Createtime = Helper.PreventNull((string)reader["CREATETIME"]);
            this.Createman = Helper.PreventNull((string)reader["CREATEMAN"]);
            this.Updatetime = Helper.PreventNull((string)reader["UPDATETIME"]);
            this.Updateman = Helper.PreventNull((string)reader["UPDATEMAN"]);
        }

        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT routeid                     routeid       " +
                    "       ,ISNULL(frommarkplate ,'')   frommarkplate " +
                    "       ,ISNULL(tomarkplate   ,'')   tomarkplate   " +
                    "       ,ISNULL(distance      ,'')   distance      " +
                    "       ,ISNULL(routetype     ,'')   routetype     " +
                    "       ,ISNULL(createtime    ,'')   createtime    " +
                    "       ,ISNULL(createman     ,'')   createman     " +
                    "       ,ISNULL(updatetime    ,'')   updatetime    " +
                    "       ,ISNULL(updateman     ,'')   updateman     " +
                    "   FROM tb_itineraryagv                             ";     

            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_itineraryagv  " +
                    "            (routeid          " +
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
                    "            (@ROUTEID         " +
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

        public string getUpdateSQL()
        {
            string strSql = "";
            strSql = "" +
                    " UPDATE  tb_itineraryagv                " +
                    "    SET  routeid       = @ROUTEID       " +
                    "        ,frommarkplate = @FROMMARKPLATE " +
                    "        ,tomarkplate   = @TOMARKPLATE   " +
                    "        ,distance      = @DISTANCE      " +
                    "        ,routetype     = @ROUTETYPE     " +
                    "        ,createtime    = @CREATETIME    " +
                    "        ,createman     = @CREATEMAN     " +
                    "        ,updatetime    = @UPDATETIME    " +
                    "        ,updateman     = @UPDATEMAN     " +
                    "  WHERE routeid        = @ROUTEID1      ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_itineraryagv    " +
                    "  WHERE routeid = @ROUTEID      "; 

            return strSql;
        }

        public void insertItineraryAgvDataBackup(DBConnection theDB, string transactiontype)
        {
            InternalIfData internalIfData = new InternalIfData();

            if (transactiontype == EntityEnumData.TRANSACTIONTYPE.Delete.ToString())
            {
                ItineraryAgvData data = new ItineraryAgvData();
                SqlDataReader reader = null;

                string strSql = getSelectSQL();
                strSql += " WHERE routeid  = '" + this.Routeid + "' ";

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
                    data.setItineraryAgvData(reader);
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
            ifValue += this.Routeid + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Frommarkplate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Tomarkplate + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Distance + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Routetype + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createtime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Createman + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updatetime + GlobalConstant.MOVEMENT_DELIMITER +
                        this.Updateman;

            return ifValue;
        }
    }
}
