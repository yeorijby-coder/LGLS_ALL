using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Device.Observe;
using ECP.Db;
using ECP.Global.Device;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Com.FieldBus;


namespace ECP.Device
{
    [Serializable]
    public class IODevice :AbstractElement
    {
        const string CLASS = "IODevice";

        private bool dirtyFlag = true;

        public static string className = "ECP.Device.IODevice";

        public static string assemblyName = "ECP";

        #region Attribute_Persistance

        private Guid ioDeviceGuid;

        public Guid IoDeviceGuid
        {
            get { return ioDeviceGuid; }
            set { ioDeviceGuid = value; }
        }
        #endregion Attribute_Persistance

        #region Constructor

        public IODevice() : base()
        {
            this.elementType = ElementType.IODEVICE;
        }

        public IODevice(string ioDeviceId): base()
        {
            this.elementId = ioDeviceId;
            this.ElementType = ElementType.IODEVICE;
        }

        public IODevice(string ioDeviceId, Guid ioDeviceGuid, string ownerId, ElementType ownerType):base()
        {
            this.elementId = ioDeviceId;
            this.ElementType = ElementType.IODEVICE;
            this.ioDeviceGuid = ioDeviceGuid;
            this.ownerId = ownerId;
            this.ownerType = ownerType;
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

                psparam.SetVarChar("OWNERID", this.ownerId);
                psparam.SetVarChar("IODEVICEID", this.elementId);


                reader = theDB.ExecuteQuery("select * from IODEVICE where IODEVICEID =@IODEVICEID and OWNERID =@OWNERID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "iodevice", this.elementId + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                    pss.SetVarChar("IODEVICEID", this.elementId);
                    pss.SetUniqueIdentifier("IODEVICEGUID", this.ioDeviceGuid);
                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString( this.ownerType));
                    theDB.ExecuteUpdate("insert into IODEVICE (IODEVICEID,IODEVICEGUID, OWNERID,OWNERTYPE) values (@IODEVICEID,@IODEVICEGUID,@OWNERID,@OWNERTYPE)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "sql error \n " + ex.Message.ToString());
                throw ex;
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
                ps.SetVarChar("IODEVICEID", this.elementId);
                ps.SetVarChar("OWNERID", this.ownerId);

                theDB.ExecuteUpdate("delete from IODEVICE where IODEVICEID =@IODEVICEID and OWNERID =@OWNERID", ps);


                // remove observables
                foreach (string obsKey in this.observables.Keys)
                {
                    this.RemoveObservable(obsKey);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "sql error \n " + ex.Message.ToString());
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

                psqlparam.SetVarChar("IODEVICEID", this.elementId);
                psqlparam.SetVarChar("OWNERID", this.ownerId);


                reader = theDB.ExecuteQuery("select * from IODEVICE where IODEVICEID =@IODEVICEID and OWNERID =@OWNERID", psqlparam);


                if (reader.Read())
                {
                    this.ioDeviceGuid = (Guid)reader["IODEVICEGUID"];
                    this.ownerType = ElementType.StringToValue((string)reader["OWNERTYPE"]);

                    // Load All the objects blong to this
                    this.Observables = Observable.LoadObservables(this.elementId);

                    foreach (Observable obs in this.Observables.Values)
                    {
                        Global_Observables.Add(obs.ObservableGuid.ToString(), obs);
                    }

                   // this.LoadUserDefinedProperties();

                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "iodevice", this.elementId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }

        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Save";

            try
            {
                base.Save(theDB);
                if (dirtyFlag)
                {
                    this.dirtyFlag = false;
                    theDB.ExecuteUpdate("update IODEVICE set\n" +
                        "IODEVICEGUID='" + this.ioDeviceGuid + "',\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "'\n" +
                        "where IODEVICEID = '" + this.elementId + "' " + "and\n" +
                        "OWNERID = ' " + this.ownerId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
            }
        }

        public static Dictionary<string, IODevice> LoadIODevices(string ownerId)
        {
            const string METHOD = CLASS + ".LoadIODevices";

            Dictionary<string, IODevice> ioDevices = new Dictionary<string, IODevice>();
            string ioDeviceId = null;
            ObjectHandle objHandle = null;
            IODevice ioDev = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select IODEVICEID from IODEVICE where OWNERID ='" + ownerId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance(assemblyName, className);
                    ioDev = (IODevice)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", " load iodevice Sql Error : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;

                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "failed to load iodevice : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                ioDeviceId = Helper.PreventNull((string)reader["IODEVICEID"]);

                ioDev.Load(ownerId, ioDeviceId);
                ioDevices.Add(ioDeviceId, ioDev);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return ioDevices;
        }

        public static IODevice LoadIODevice(string ownerId, string ioDeviceId)
        {
            const string METHOD = CLASS + ".LoadIODevice";

            ObjectHandle objHandle = null;
            IODevice ioDev = null;
     
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                ioDev = (IODevice)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "failed to load iodevice : " + ioDeviceId +
                    "\n" + ex.Message.ToString());
            }
            ioDev.Load(ownerId, ioDeviceId);

            return ioDev;

        }

        public static IODevice CreateIODevice(string ownerId, ElementType ownerType, string ioDeviceId)
        {
            const string METHOD = CLASS + ".CreateIODevice";

            IODevice ioDev = null;
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                ioDev = (IODevice)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "iodevice", "failed to create iodevice : "
                    + ioDeviceId + "\n" + ex.Message.ToString());
            }



            ioDev.elementId = ioDeviceId;
            ioDev.elementType = ElementType.IODEVICE;
            ioDev.ioDeviceGuid = Guid.NewGuid();
            ioDev.ownerId = ownerId;
            ioDev.ownerType = ownerType;

            ioDev.Add();

            return ioDev;
        }


        public void Load(string ownerId, string ioDeviceId)
        {
            this.ownerId = ownerId;
            this.elementId = ioDeviceId;
            Load();
        }
        #endregion Manage_Persistent_Attribute

        #region Method
        public override void RegisterObservableToFiledBusDriver(AbstractFieldBusDriver fieldbusDriver)
        {
            // do my one
            foreach (string key in this.observables.Keys)
            {
                fieldbusDriver.RegisterObservable(this.observables[key]);
            }
        }


        public void AddObservable()
        {
        }

        public void RemoveObservable()
        {
        }

        public override string GetUserDefinedEntityClassName()
        {
            return CLASS.ToUpper();
        }

        public override string GetUserDefineEntityId()
        {
            return this.elementId;
        }


        public override bool IsAlive()
        {
            return true;
        }

        #endregion Method


    }
}
