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
    public class LocationData : ECPObject 
    {

        private string ownerid = "";

        public string Ownerid
        {
            get { return ownerid; }
            set { ownerid = value; }
        }
        private string locationid = "";

        public string Locationid
        {
            get { return locationid; }
            set { locationid = value; }
        }
        private string locationtype = "";

        public string Locationtype
        {
            get { return locationtype; }
            set { locationtype = value; }
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

        public LocationData()
        {
        }

        public void getLocationData(string ownerid, string locationid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  WHERE ownerid = '" + ownerid + "' " +
                      "    AND locationid = '" + locationid + "' ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    setLocationData(reader);
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

        public int getLocationStatusBySubsysem(string ownerid)
        {
            DBConnection theDB = null;
            SqlDataReader reader = null;
            
            int iCnt = 0;
            string strSql = "";
            strSql += " SELECT COUNT(*)      CNT               " +
                      "   FROM tb_location                     " +
                      "  WHERE ownerid = '" + ownerid + "' " +
                      "    AND available <> '0'                " +
                      "    AND available is not null           " +
                      "    AND available <> ''                 ";

            try
            {
                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(strSql);

                if (reader.Read())
                {
                    iCnt = (int)reader["CNT"];
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

            return iCnt;
        }

        //Program List 조회
        public List<LocationData> getLocationDataList()
        {
            List<LocationData> list = new List<LocationData>();
            DBConnection theDB = DBControl.GetConnection();
            SqlDataReader reader = null;

            string strSql = getSelectSQL();
            strSql += "  ORDER BY ownerid, locationid  ";

            try
            {
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    LocationData lData = new LocationData();
                    lData.setLocationData(reader);

                    list.Add(lData);
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

        public void insertLocationData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getInsertSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_LOCATION 정보
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("LOCATIONTYPE", Helper.PreventNull(this.Locationtype));
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

        public void updateLocationData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getUpdateSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_LOCATION 정보  update
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("LOCATIONTYPE", Helper.PreventNull(this.Locationtype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));
                pss.SetVarChar("OWNERID1", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID1", Helper.PreventNull(this.Locationid));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void deleteLocationData(DBConnection theDB, Boolean bkFlag)
        {
            PreparedSqlParameter pss = null;
            string strSql = "";

            try
            {
                strSql = getDeleteSQL();

                theDB.Start();

                pss = theDB.GetPreparedSqlParameter();

                //TB_Equipment 정보  delete
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));

                theDB.ExecuteUpdate(strSql, pss);

                theDB.Commit();
            }
            catch (SqlException ex)
            {
                if (theDB != null) theDB.Rollback();
                throw ex;
            }
        }

        public void insertLocationData()
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

                //TB_LOCATION 정보  inert                                               
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("LOCATIONTYPE", Helper.PreventNull(this.Locationtype));
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

        public void updateLocationData()
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

                //TB_LOCATION 정보  update
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));
                pss.SetVarChar("LOCATIONTYPE", Helper.PreventNull(this.Locationtype));
                pss.SetVarChar("ASSEMBLYNAME", Helper.PreventNull(this.Assemblyname));
                pss.SetVarChar("CLASSNAME", Helper.PreventNull(this.Classname));
                pss.SetVarChar("AVAILABLE", Helper.PreventNull(this.Available));
                pss.SetVarChar("OWNERID1", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID1", Helper.PreventNull(this.Locationid));

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

        public void deleteLocationData()
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

                //TB_Location 정보  delete
                pss.SetVarChar("OWNERID", Helper.PreventNull(this.Ownerid));
                pss.SetVarChar("LOCATIONID", Helper.PreventNull(this.Locationid));

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

        public void setLocationData(SqlDataReader reader)
        {
            this.Ownerid = Helper.PreventNull((string)reader["OWNERID"]);
            this.Locationid = Helper.PreventNull((string)reader["LOCATIONID"]);
            this.Locationtype = Helper.PreventNull((string)reader["LOCATIONTYPE"]);
            this.Assemblyname = Helper.PreventNull((string)reader["ASSEMBLYNAME"]);
            this.Classname = Helper.PreventNull((string)reader["CLASSNAME"]);
            this.Available = Helper.PreventNull((string)reader["AVAILABLE"]); 
        }
        public string getSelectSQL()
        {
            string strSql = "";

            strSql = "" +
                    " SELECT  ownerid                      ownerid        " +
                    "        ,ISNULL(locationid    , '')   Locationid     " +
                    "        ,ISNULL(locationtype  , '')   locationtype   " +
                    "        ,ISNULL(assemblyname  , '')   assemblyname   " +
                    "        ,ISNULL(classname     , '')   classname      " +
                    "        ,ISNULL(available     , '')   available      " +
                    "   FROM tb_location                                   ";   


            return strSql;
        }

        public string getInsertSQL()
        {
            string strSql = "";
            strSql = "" +
                    " INSERT INTO tb_location     " +
                    "            ( ownerid        " +
                    "             ,locationid     " +
                    "             ,locationtype   " +
                    "             ,assemblyname   " +
                    "             ,classname      " +
                    "             ,available      " +
                    "            )                " +
                    "      VALUES                 " +
                    "            ( @OWNERID       " +
                    "             ,@LOCATIONID    " +
                    "             ,@LOCATIONTYPE  " +
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
                    " UPDATE  tb_location                   " +
                    "    SET  ownerid      = @OWNERID       " +
                    "        ,locationid   = @LOCATIONID    " +
                    "        ,locationtype = @LOCATIONTYPE  " +
                    "        ,assemblyname = @ASSEMBLYNAME  " +
                    "        ,classname    = @CLASSNAME     " +
                    "        ,available    = @AVAILABLE     " +
                    "  WHERE  ownerid      = @OWNERID1      " +
                    "    AND  locationid   = @LOCATIONID1   ";

            return strSql;
        }

        public string getDeleteSQL()
        {
            string strSql = "";
            strSql = "" +
                    " DELETE FROM tb_location               " +
                    "  WHERE  ownerid      = @OWNERID1      " +
                    "    AND  locationid   = @LOCATIONID1   ";

            return strSql;
        }
    }
}
