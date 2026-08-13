using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Global;
using ECP.Db;
using ECP.Device.Dispatch;
using ECP.Global.Device;
using ECP.Device.Observe;
using ECP.Util;
using ECP.Com.FieldBus;
using ECP.Interfaces.Unit;
using System.ComponentModel;

namespace ECP.Device
{
    [Serializable]
    public class Equipment : AbstractModule
    {
        const string CLASS = "Equipment";

        private bool dirtyFlag = true;

        public bool DirtyFlag
        {
            get { return dirtyFlag; }
            set { dirtyFlag = value; }
        }

        public string className = "ECP.Device.Equipment";

        public string assemblyName = "ECP";

        #region Attribute_Persistance

        private EquipmentType equipmentType;

        public EquipmentType EquipmentType
        {
            get { return this.equipmentType; }
            set { this.equipmentType = value; }
        }

        private string hostId = "";

        public string HostID
        {
            get { return hostId; }
            set { this.hostId = value; }
        }

        private string plcId = "";

        public string PlcId
        {
            get { return plcId; }
            set { plcId = value; }
        }

        private string id = "";
        public string Id
        {
            get { return id; }
            set { id = value; }
        }

        #endregion Attribute_Persistance
        public event PropertyChangedEventHandler PropertyChanged;
        #region Constructor

        public Equipment()
        {
            this.ElementType = ElementType.EQUIPMENT;
        }

        public Equipment(string equipmentId)
        {
            this.elementId = equipmentId;
            this.ElementType = ElementType.EQUIPMENT;
        }

        public Equipment(string equipmentId, EquipmentType equipmentType,
            string assemblyName, string className)
        {
            this.elementId = equipmentId;
            this.ElementType = ElementType.EQUIPMENT;
            this.equipmentType = equipmentType;
            this.assemblyName = assemblyName;
            this.className = className;
        }

        public Equipment(string equipmentId, EquipmentType equipmentType,
            string hostId, string assemblyName, string className)
        {
            this.elementId = equipmentId;
            this.ElementType = ElementType.EQUIPMENT;
            this.equipmentType = equipmentType;
            this.hostId = hostId;
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

                psparam.SetVarChar("EQUIPMENTID", this.elementId);

                reader = theDB.ExecuteQuery("select * from TB_EQUIPMENT where EQUIPMENTID =@EQUIPMENTID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "equipment", this.elementId + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("EQUIPMENTID", this.elementId);
                    pss.SetVarChar("EQUIPMENTTYPE", EquipmentType.ValueToString(this.equipmentType));
                    pss.SetVarChar("HOSTID", this.hostId);
                    pss.SetVarChar("ASSEMBLYNAME", this.assemblyName);
                    pss.SetVarChar("CLASSNAME", this.className);

                    theDB.ExecuteUpdate("insert into TB_EQUIPMENT (EQUIPMENTID,EQUIPMENTTYPE,HOSTID,ASSEMBLYNAME,CLASSNAME) values (@EQUIPMENTID,@EQUIPMENTTYPE,@HOSTID,@ASSEMBLYNAME,@CLASSNAME)", pss);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "sql error \n " + ex.Message.ToString());
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
                ps.SetVarChar("EQUIPMENTID", this.elementId);

                theDB.ExecuteUpdate("delete from TB_EQUIPMENT where EQUIPMENTID =@EQUIPMENTID", ps);

                // remove All ojbects that belongs to this

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

                this.DeleteAllUserDefinedProperties(CLASS);


            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "sql error \n " + ex.Message.ToString());
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

                psqlparam.SetVarChar("EQUIPMENTID", this.elementId);
                reader = theDB.ExecuteQuery("select * from TB_EQUIPMENT where EQUIPMENTID =@EQUIPMENTID", psqlparam);

                if (reader.Read())
                {
                    this.equipmentType = EquipmentType.StringToValue((string)reader["EQUIPMENTTYPE"]);
                    this.hostId = (string)reader["HOSTID"];
                    this.assemblyName = (string)reader["ASSEMBLYNAME"];
                    this.className = (string)reader["CLASSNAME"];

                    // Load All the objects blong to equipment
                    this.subsystems = SubSystem.LoadSubSystems(this.elementId);

                    //Module, IO Device Load 부분 주석 처리 2009.04.20 윤선화 : 사용 안함.                    
                    //this.modules = Module.LoadModules(this.elementId);
                    //this.ioDevices = IODevice.LoadIODevices(this.elementId);
                    this.Observables = Observable.LoadObservables(this.elementId);

                    foreach (Observable obs in this.Observables.Values)
                    {
                        Global_Observables.Add(obs.ObservableGuid.ToString(), obs);
                    }


                    if (this is IRepresentativeEquipment)
                    {
                        IRepresentativeEquipment irepresentativeEquipment = this as IRepresentativeEquipment;
                        if (irepresentativeEquipment != null)
                        {
                            irepresentativeEquipment.LoadAbstractFieldBusDrivers();
                        }
                    }
                    this.LoadUserDefinedProperties();

                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "equipment", this.elementId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "Load error \n " + ex.Message.ToString());
                throw ex;
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
                    theDB.ExecuteUpdate("update TB_EQUIPMENT set\n" +
                        "EQUIPMENTTYPE='" + EquipmentType.ValueToString(this.equipmentType) + "',\n" +
                        "ASSEMBLYNAME='" + this.assemblyName + "',\n" +
                        "CLASSNAME='" + this.className + "'\n" +
                        "where EQUIPMENTID = '" + this.elementId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "sql error \n " + ex.Message.ToString());
            }
        }


        public static Dictionary<string, Equipment> LoadEquipments()
        {
            const string METHOD = CLASS + ".LoadEquipments";

            Dictionary<string, Equipment> equipments = new Dictionary<string, Equipment>();
            string equipmentId = null;
            ObjectHandle objHandle = null;
            Equipment eqp = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select EQUIPMENTID, ASSEMBLYNAME, CLASSNAME from TB_EQUIPMENT";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    eqp = (Equipment)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "quipment", "failed to load equipments because SQL Error : \n" + ex.Message.ToString());
                    throw ex;
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "quipment", "failed to load equipments : \n" + ex.Message.ToString());
                    throw ex;
                }
                equipmentId = Helper.PreventNull((string)reader["EQUIPMENTID"]);
                eqp.Load(equipmentId);

                eqp.Init();

                equipments.Add(equipmentId, eqp);

            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return equipments;
        }

        public static Equipment LoadEquipment(string equipmentId)
        {
            const string METHOD = CLASS + ".LoadEquipment";

            ObjectHandle objHandle = null;
            Equipment eqp = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select ASSEMBLYNAME, CLASSNAME from TB_EQUIPMENT where EQUIPMENTID ='" + equipmentId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    eqp = (Equipment)objHandle.Unwrap();
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "failed to load equipment : " + equipmentId +
                        "\n" + ex.Message.ToString());
                }
                eqp.Load(equipmentId);
                eqp.Init();
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return eqp;

        }

        public static Equipment CreateEquipment(string equipmentId,
            EquipmentType equipmentType, string assemblyName, string className)
        {
            const string METHOD = CLASS + ".CreateEquipment";
            Equipment eqp = null;
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                eqp = (Equipment)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "equipment", "failed to create equipment : " + equipmentId +
                    "\n" + ex.Message.ToString());
            }

            eqp.elementId = equipmentId;
            eqp.ElementType = ElementType.EQUIPMENT;
            eqp.equipmentType = equipmentType;
            eqp.assemblyName = assemblyName;
            eqp.className = className;

            eqp.Add();
            //   eqp.Init();

            return eqp;
        }

        public void Load(string equipmentId)
        {
            this.elementId = equipmentId;
            Load();
        }

        public void AddEquipment(string deviceId)
        {
            this.elementId = deviceId;
            Add();
        }

        public void RemoveEquipment()
        {
            Delete();
        }
        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련

        public override void Init()
        {
            InitContainerObjects();

            if (this.Dispatcher == null)
                this.Dispatcher = new Dispatcher(this);

        }

        private void InitContainerObjects()
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

        public void StartDispatcher()
        {
            this.Dispatcher.Start();
        }

        private void FBdriverRegister()
        {
            foreach (int key in this.fbdrivers.Keys)
            {
                this.RegisterObservableToFiledBusDriver(fbdrivers[key]);
            }

        }

        public void LoadAbstractFieldBusDrivers()
        {
            this.fbdrivers = AbstractFieldBusDriver.LoadAbstractFieldBusDrivers(this.elementId);
        }

        public void FBdriverOpen()
        {
            foreach (int key in this.fbdrivers.Keys)
            {
                if (fbdrivers[key].UseFlag == true)
                    fbdrivers[key].Open();
            }
        }


        public override void Startup()
        {
            StartupContainerObject();
            FBdriverRegister();
        }

        private void StartupContainerObject()
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
            foreach (int key in this.fbdrivers.Keys)
            {
                if (fbdrivers[key].UseFlag == true)
                    fbdrivers[key].Close();
            }


            this.Dispatcher.Stop();
        }
        protected void OnPropertyChanged(string propertyName, object value)
        {
            if (PropertyChanged != null)
            {
                if (value == null)
                {

                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName + "=null"));
                }
                else
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName + "=" + value.ToString()));
                }

            }
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

        public void StartFirstScanObservables()
        {
            foreach (int key in this.fbdrivers.Keys)
            {
                if (this.fbdrivers[key].UseFlag)
                {
                    this.fbdrivers[key].StartFirstScan();
                }
            }
        }

        public void StartScanObservables()
        {
            foreach (int key in this.fbdrivers.Keys)
            {
                if (this.fbdrivers[key].UseFlag)
                {
                    this.fbdrivers[key].StartScaning();
                }
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
