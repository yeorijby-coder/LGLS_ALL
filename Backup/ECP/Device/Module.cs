using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;

using ECP.Device.Dispatch;
using ECP.Db;
using ECP.Global.Device;
using ECP.Util;
using ECP.Device.Observe;
using ECP.Com.FieldBus;

namespace ECP.Device
{
    [Serializable]
    public class Module : AbstractModule
    {
        const string CLASS = "Module";

        private bool dirtyFlag = true;

        public string className = "ECP.Device.Module";

        public string assemblyName = "ECP";


        #region Attribute_Persistance

        private ModuleType moduleType;

        public ModuleType ModuleType
        {
            get { return moduleType; }
            set { moduleType = value; }
        }
        #endregion Attribute_Persistance

        #region Constructor
        public Module() :base()
        {
            this.ElementType = ElementType.MODULE;
        }

        public Module(string moduleId) : base()
        {
            this.elementId = moduleId;
            this.ElementType = ElementType.MODULE;
        }

        public Module(string moduleId, ModuleType moduleType,
            string ownerId, ElementType ownerType, string assemblyName, string className)
            : base()
        {
            this.elementId = moduleId;
            this.ElementType = ElementType.MODULE;
            this.moduleType = moduleType;
            this.ownerId = ownerId;
            this.ownerType = ownerType;
            this.assemblyName = assemblyName;
            this.className = className;
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

                psparam.SetVarChar("MODULEID", this.elementId);
         
                reader = theDB.ExecuteQuery("select * from MODULE where MODULEID =@MODULEID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "module", this.elementId+ "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("MODULEID", this.elementId);
                    pss.SetVarChar("MODULETYPE", ModuleType.ValueToString(this.moduleType));

                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString(this.ownerType));

                    pss.SetVarChar("ASSEMBLYNAME", this.assemblyName);
                    pss.SetVarChar("CLASSNAME", this.className);


                    theDB.ExecuteUpdate("insert into MODULE (MODULEID,MODULETYPE,OWNERID,OWNERTYPE,ASSEMBLYNAME,CLASSNAME) values (@MODULEID,@MODULETYPE,@OWNERID,@OWNERTYPE,@ASSEMBLYNAME,@CLASSNAME)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "module", "sql error \n " + ex.Message.ToString());
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
                ps.SetVarChar("MODULEID", this.elementId);
             
                theDB.ExecuteUpdate("delete from MODULE where MODULEID =@MODULEID", ps);

                // remove modules
                foreach (string modkey in this.modules.Keys)
                {
                    this.RemoveModule(modkey);
                }

                // remove subsystems
                foreach (string subkey in this.subsystems.Keys)
                {
                    this.RemoveSubSystem(subkey);
                }

                // remove ioDevices
                foreach (string ioKey in this.ioDevices.Keys)
                {
                    this.RemoveIODevice(ioKey);
                }

                // remove observables
                foreach (string obsKey in this.observables.Keys)
                {
                    this.RemoveObservable(obsKey);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "module", "sql error \n " + ex.Message.ToString());
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

                psqlparam.SetVarChar("MODULEID", this.elementId);
        
                reader = theDB.ExecuteQuery("select * from MODULE where MODULEID =@MODULEID", psqlparam);


                if (reader.Read())
                {
                    this.moduleType = ModuleType.StringToValue((string)reader["MODULETYPE"]);

                    this.ownerId = (string)reader["OWNERID"];
                    this.ownerType = ElementType.StringToValue((string)reader["OWNERTYPE"]);

                    this.assemblyName = (string)reader["ASSEMBLYNAME"];
                    this.className = (string)reader["CLASSNAME"];

                    // Load All the objects blong to this
                    this.subsystems = SubSystem.LoadSubSystems(this.elementId);
                    this.modules = Module.LoadModules(this.elementId);
                    this.ioDevices = IODevice.LoadIODevices(this.elementId);
                    this.Observables = Observable.LoadObservables(this.elementId);

                    foreach (Observable obs in this.Observables.Values)
                    {
                        Global_Observables.Add(obs.ObservableGuid.ToString(), obs);
                    }

                    this.fbdrivers = AbstractFieldBusDriver.LoadAbstractFieldBusDrivers(this.elementId);

                   // this.LoadUserDefinedProperties();

                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "module", this.elementId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "module", "sql error \n " + ex.Message.ToString());
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
                    theDB.ExecuteUpdate("update MODULE set\n" +
                        "OWNERID='" + this.ownerId + "',\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "',\n" +
                        "MODULETYPE='" + ModuleType.ValueToString(this.moduleType) + "',\n" +
                        "ASSEMBLYNAME='" + this.assemblyName + "',\n" +
                        "CLASSNAME='" + this.className + "'\n" + 
                        "where MODULEID = '" + this.ElementId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "module", "sql error \n " + ex.Message.ToString());
            }
        }


        public static Dictionary<string, Module> LoadModules(string ownerId)
        {
            const string METHOD = CLASS + ".LoadModules";

            Dictionary<string, Module> modules = new Dictionary<string, Module>();
            string moduleId = null;
            ObjectHandle objHandle = null;
            Module mod = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select MODULEID, ASSEMBLYNAME, CLASSNAME from MODULE where OWNERID ='" + ownerId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    mod = (Module)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "module", " load modules Sql Error : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;

                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "module", "failed to load modules : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                moduleId = Helper.PreventNull((string)reader["MODULEID"]);
                mod.Load(ownerId, moduleId);
                modules.Add(moduleId, mod);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return modules;
        }

        public static Module LoadModule(string ownerId, string moduleId)
        {
            const string METHOD = CLASS + ".LoadModule";

            ObjectHandle objHandle = null;
            Module mod = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select ASSEMBLYNAME, CLASSNAME from MODULE where OWNERID ='" + ownerId +
                "' and MODULEID ='" + moduleId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    mod = (Module)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "module", " load module Sql Error : " + moduleId +
                        "\n" + ex.Message.ToString());
                    throw ex;

                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "module", "failed to load module : " + moduleId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                mod.Load(ownerId, moduleId);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return mod;

        }

        public static Module CreateModule(string ownerId, ElementType ownerType, string moduleId,
            ModuleType moduleType, string assemblyName, string className)
        {
            const string METHOD = CLASS + ".CreateModule";
            Module mod = null;
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                mod = (Module)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "module", "failed to create module : " + moduleId +
                    "\n" + ex.Message.ToString());
            }

            mod.elementId = moduleId;
            mod.elementType = ElementType.MODULE;
            mod.moduleType = moduleType;
            mod.ownerId = ownerId;
            mod.ownerType = ownerType;
            mod.assemblyName = assemblyName;
            mod.className = className;

            mod.Add();

            return mod;
        }

        public void Load(string ownerId, string moduleId)
        {
            this.ownerId = ownerId;
            this.elementId = moduleId;
            Load();
        }
        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련

        public override void  Init()
        {
            foreach (string key in this.modules.Keys)
            {
                this.modules[key].Init();
            }

            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].Init();
            }
        }


        public void InitContainerObjects()
        {
            foreach (string key in this.modules.Keys)
            {
                this.modules[key].Init();
            }

            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].Init();
            }
        }


        public override void Startup()
        {   
             StartupContainerObjects();

         }

         public void StartupContainerObjects()
         {
             foreach (string key in this.modules.Keys)
             {
                 this.modules[key].Startup();
             }

             foreach (string key in this.subsystems.Keys)
             {
                 this.subsystems[key].Startup();
             }
         }



         public override void Shutdown()
         {
             ShutdownContainerObject();
         }

         private void ShutdownContainerObject()
         {
             foreach (string key in this.modules.Keys)
             {
                 this.modules[key].Shutdown();
             }

             foreach (string key in this.subsystems.Keys)
             {
                 this.subsystems[key].Shutdown();
             }
         }

         public override void Exit()
         {
             this.Shutdown();

             // Unsubscribe 
         }

         #endregion 초기화및기동관련

        #region Method

         public override void RegisterObservableToFiledBusDriver(AbstractFieldBusDriver fieldbusDriver)
        {
            // do my one
            foreach (string key in this.observables.Keys)
            {
                if (this.observables[key].driverNo == fieldbusDriver.DriverNo)
                    fieldbusDriver.RegisterObservable(this.observables[key]);
                else
                {
                    //do nothing
                }
            } 
            // do my associate objects
            foreach (string key in this.modules.Keys)
            {
                this.modules[key].RegisterObservableToFiledBusDriver(fieldbusDriver);
            }
            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].RegisterObservableToFiledBusDriver(fieldbusDriver);
            }

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
