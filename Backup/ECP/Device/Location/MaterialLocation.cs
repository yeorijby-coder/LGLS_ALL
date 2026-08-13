using System;
using System.Collections.Generic;
using System.Text;

using System.Data.SqlClient;


using System.Reflection;
using System.Runtime.Remoting;


using ECP.Global;
using ECP.Db;
using ECP.Global.Device;
using ECP.Service.Logger;
using ECP.Util;

namespace ECP.Device.Location
{
    abstract public  class MaterialLocation : Persistence
    {
       private const string CLASS = "MaterialLocation";
        private bool dirtyFlag = true;
        public static string className = "ECP.Device.Location.MaterialLocation";
        public static string assemblyName = "ECP";

        #region Attribute_Persistance

        protected string locationId = null;
        public string LocationId
        {
            get { return locationId; }
            set { locationId = value; }
        }

        protected string ownerId = "";

        public string OwnerID
        {
            get { return ownerId; }
            //set { ownerId = value; }
        }

        protected LocationType locationType = LocationType.CarrierLocation;

        public LocationType LocationType
        {
            get { return locationType; }
            set { locationType = value; }
        }

        protected LocationState locationState;

        public LocationState LocationState
        {
            get { return locationState; }
            set { locationState = value; }
        }


        protected LocationLogicalState logicalState;

        protected LocationLogicalState LogicalState
        {
            get { return logicalState; }
            set { logicalState = value; }
        }


        #endregion Attribute_Persistance

        #region Attribute_Not_Persistance

        protected string materialId = "";
        public string MaterialId
        {
            get { return materialId; }
            set { materialId = value; }
        }

        #endregion Attribute_Not_Persistance

        #region Constructor

        public MaterialLocation()
        {
        }

        public MaterialLocation(string ownerId, string locationId, 
            string materiald,  LocationState locationState, LocationLogicalState logicalState)
        {
            this.LocationId = locationId;
            this.MaterialId = materiald;    
            this.LocationState = locationState;
            this.ownerId = ownerId;
            this.LogicalState = logicalState;
        }

        #endregion Constructor

        #region Manage_Persistent_Attribute
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetVarChar("LOCATIONID", this.locationId);
                psparam.SetVarChar("OWNERID", this.ownerId);

                reader = theDB.ExecuteQuery("select * from TB_LOCATION where LOCATIONID =@LOCATIONID and OWNERID =@OWNERID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "MaterialLocation", this.locationId + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("OWNERID", this.OwnerID);
                    pss.SetVarChar("LOCATIONID", this.locationId);
                    pss.SetVarChar("LOCATIONTYPE", LocationType.ValueToString(this.locationType));
//                    pss.SetVarChar("LOGICALSTATE", LocationLogicalState.ValueToString(this.LogicalState));

                    pss.SetVarChar("ASSEMBLYNAME", assemblyName);
                    pss.SetVarChar("CLASSNAME", "ECP.Device.Location." + LocationType.ValueToString(this.locationType));

                    theDB.ExecuteUpdate("insert into TB_LOCATION (OWNERID,LOCATIONID,LOCATIONTYPE,ASSEMBLYNAME,CLASSNAME) values (@OWNERID,@LOCATIONID,@LOCATIONTYPE,@ASSEMBLYNAME,@CLASSNAME)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "MaterialLocation", "sql error \n " + ex.Message.ToString());
                //throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }
        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Delete";

            try
            {
                base.Delete(theDB);
                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("LOCATIONID", this.locationId);
                ps.SetVarChar("OWNERID", this.ownerId);
                theDB.ExecuteUpdate("delete from TB_LOCATION where LOCATIONID =@LOCATIONID and  OWNERID =@OWNERID", ps);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "MaterialLocation", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
        }

        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);
                PreparedSqlParameter psqlparam = theDB.GetPreparedSqlParameter();
                psqlparam.SetVarChar("LOCATIONID", this.locationId);
                psqlparam.SetVarChar("OWNERID", this.ownerId);


                reader = theDB.ExecuteQuery("select * from TB_LOCATION where LOCATIONID =@LOCATIONID and  OWNERID =@OWNERID", psqlparam);


                if (reader.Read())
                {
                    //this.logicalState = LocationLogicalState.StringToValue((string)reader["LOGICALSTATE"]);
                    this.locationType = LocationType.StringToValue((string)reader["LOCATIONTYPE"]);
                    assemblyName = (string)reader["ASSEMBLYNAME"];
                    className = (string)reader["CLASSNAME"];
                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "MaterialLocation", this.locationId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "MaterialLocation", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }
        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".save";


            try
            {
                base.Save(theDB);
                if (dirtyFlag)
                {
                    this.dirtyFlag = false;
                    theDB.ExecuteUpdate("update TB_LOCATION set\n" +
                        "LOCATIONTYPE='" + LocationType.ValueToString(this.locationType) + "'\n" +
                        //"LOGICALSTATE='" + LocationLogicalState.ValueToString(this.logicalState) + "'\n" +
                        "ASSEMBLYNAME='" + assemblyName + "',\n" +
                        "CLASSNAME='" + className + "'\n" +
                        "where OWNERID = '" + this.ownerId + "' and LOCATIONID = '" + this.locationId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "sql error \n " + ex.Message.ToString());
            }
        }


        public static Dictionary<string, MaterialLocation> LoadMaterialLocations(string ownerId)
        {
            const string METHOD = CLASS + ".LoadMaterialLocations";

            Dictionary<string, MaterialLocation> locations = new Dictionary<string, MaterialLocation>();
            string locationId = null;
            ObjectHandle objHandle = null;
            MaterialLocation location = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select LOCATIONID,ASSEMBLYNAME, CLASSNAME from TB_LOCATION where OWNERID ='" + ownerId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    location = (MaterialLocation)objHandle.Unwrap();
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "material location", "failed to load material locations : " + ownerId +
                        "\n" + ex.Message.ToString());
                }
                locationId = Helper.PreventNull((string)reader["LOCATIONID"]);
                location.Load(ownerId, locationId);
                locations.Add(locationId, location);
            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return locations;
        }

        public static MaterialLocation LoadMaterialLocation(string ownerId, string locationId)
        {
            const string METHOD = CLASS + ".LoadMaterialLocation";

            ObjectHandle objHandle = null;
            MaterialLocation location = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select ASSEMBLYNAME, CLASSNAME from TB_LOCATION where OWNERID ='" + ownerId +
                "' and LOCATIONID ='" + locationId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    location = (MaterialLocation)objHandle.Unwrap();
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "material location", "failed to load material location : " + locationId +
                        "\n" + ex.Message.ToString());
                }
                location.Load(ownerId, locationId);
            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return location;

        }

        public static MaterialLocation CreateMaterialLocation(string ownerId, ElementType ownerType, string locationId,
            LocationType locationType, LocationLogicalState logicalState)
        {
            const string METHOD = CLASS + ".CreateMaterialLocation";
            MaterialLocation location = null;
            ObjectHandle objHandle = null;
            try
            {
                string className = "ECP.Device.Location." + LocationType.ValueToString(locationType);

                objHandle = Activator.CreateInstance(assemblyName, className);
                location = (MaterialLocation)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "material location", "failed to create material location : " + locationId +
                    "\n" + ex.Message.ToString());
            }

            location.locationId = locationId;
            location.logicalState = logicalState;
            location.locationType = locationType;
            location.ownerId = ownerId;

            location.Add();

            return location;
        }

        public void DeleteMaterialLocationByElementID(string elementId)
        {
            const string METHOD = CLASS + ".DeleteMaterialLocationByElementID";

            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();

                theDB.Start();

                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("OWNERID", elementId);
                theDB.ExecuteUpdate("delete from TB_LOCATION where OWNERID =@OWNERID", ps);
                
                theDB.Commit();

                
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "MaterialLocation", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
        }
        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련
        public void Load(string ownerId, string locationid)
        {
            this.ownerId = ownerId;
            this.locationId = locationid;
            Load();
        }
        #endregion 초기화및기동관련

        #region Method

        public void RemoveMaterial()
        {
            this.materialId = "";
        }

        public bool AddMaterial(string materialId)
        {
            //ECP.Util.Helper.PreventNull()
            if (materialId == null || materialId.Trim().Equals(""))
            {
                return false;
            }

            this.materialId = materialId;
            return true;
        }
     
        public bool IsReserved()
        {
            if (this.locationState == LocationState.RESERVED)
            {
                return true;
            }
            return false;
        }

        public bool IsMaterialExist()
        {
            if (this.locationState == LocationState.OCCUFIED)
            {
                return true;
            }
            return false;
        }
        #endregion Method

    }
}
