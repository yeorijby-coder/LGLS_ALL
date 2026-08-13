using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Device.Location;
using ECP.Db;
using ECP.Util;
using ECP.Global.Device;
using ECP.Device.Observe;
using ECP.Com.FieldBus;
using ECP.Global.Exception;
using System.ComponentModel;
using System.Windows.Forms;

namespace ECP.Device
{
    [Serializable]
    public class SubSystem : AbstractSubSystem
    {
        private const string CLASS = "SubSystem";

        private bool dirtyFlag = true;

        public bool DirtyFlag
        {
            get { return dirtyFlag; }
            set { dirtyFlag = value; }
        }


        public string className = "ECP.Device.SubSystem";

        public string assemblyName = "ECP";

        #region Attribute_Persistance

        private SubSystemType subSystemType;

        public SubSystemType SubSystemType
        {
            get { return subSystemType; }
            set { subSystemType = value; this.dirtyFlag = true; Save(); }
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

        private bool isAvailible = true;

        public bool IsAvailible
        {
            get
            {
                return isAvailible;
            }
            set
            {
                isAvailible = value;
                OnPropertyChanged("IsAvailible", value);
            }
        }

        private bool isProhibited = false;

        public bool IsProhibited
        {
            get
            {
                return isProhibited;
            }
            set
            {
                isProhibited = value;
                OnPropertyChanged("IsProhibited", value);
            }
        }
        #endregion Attribute_Persistance

        #region Constructor

        public SubSystem()
            : base()
        {
            this.ElementType = ElementType.SUBSYSTEM;
        }

        public SubSystem(string subSystemId)
            : base()
        {
            this.elementId = subSystemId;
            this.ElementType = ElementType.SUBSYSTEM;
        }

        public SubSystem(string subsystemId, SubSystemType subsystemType,
        string ownerId, ElementType ownerType, string assemblyName, string className)
            : base()
        {
            this.elementId = subsystemId;
            this.elementType = ElementType.SUBSYSTEM;
            this.subSystemType = subsystemType;
            this.ownerId = ownerId;
            this.ownerType = ownerType;
            this.assemblyName = assemblyName;
            this.className = className;
        }

        public SubSystem(string subsystemId, SubSystemType subsystemType, string hostId,
            string ownerId, ElementType ownerType, string assemblyName, string className)
            : base()
        {
            this.elementId = subsystemId;
            this.elementType = ElementType.SUBSYSTEM;
            this.subSystemType = subsystemType;
            this.hostId = hostId;
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

                psparam.SetVarChar("SUBSYSTEMID", this.elementId);

                reader = theDB.ExecuteQuery("select * from TB_SUBSYSTEM where SUBSYSTEMID =@SUBSYSTEMID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "subsystem", this.elementId + "already Exist");
                    // throw new ECPException(ECPExceptionType.DUPLICATE,"",""subsystem", this.elementId + "already Exist");
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("SUBSYSTEMID", this.elementId);
                    pss.SetVarChar("SUBSYSTEMTYPE", SubSystemType.ValueToString(this.subSystemType));

                    pss.SetVarChar("HOSTID", this.hostId);
                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString(this.ownerType));
                    pss.SetVarChar("ASSEMBLYNAME", this.assemblyName);
                    pss.SetVarChar("CLASSNAME", this.className);


                    theDB.ExecuteUpdate("insert into TB_SUBSYSTEM (SUBSYSTEMID,SUBSYSTEMTYPE,HOSTID,OWNERID,OWNERTYPE,ASSEMBLYNAME,CLASSNAME) values (@SUBSYSTEMID,@SUBSYSTEMTYPE,@HOSTID,@OWNERID,@OWNERTYPE,@ASSEMBLYNAME,@CLASSNAME)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "sql error \n " + ex.Message.ToString());
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
                ps.SetVarChar("SUBSYSTEMID", this.elementId);

                theDB.ExecuteUpdate("delete from TB_SUBSYSTEM where SUBSYSTEMID =@SUBSYSTEMID", ps);

                // remove All ojbects that belongs to this

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

                // 임시.. 추후 삭제 
                if (!ownerId.Equals("LIFTER"))
                {

                AGAIN:
                    // remove observables
                    foreach (string obsKey in this.observables.Keys)
                    {
                        this.RemoveObservable(obsKey);
                        goto AGAIN;
                    }
                }

                this.DeleteAllUserDefinedProperties(CLASS);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "sql error \n " + ex.Message.ToString());
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

                psqlparam.SetVarChar("SUBSYSTEMID", this.elementId);

                reader = theDB.ExecuteQuery("select * from TB_SUBSYSTEM where SUBSYSTEMID =@SUBSYSTEMID", psqlparam);


                if (reader.Read())
                {
                    this.ElementType = ElementType.SUBSYSTEM;

                    this.subSystemType = SubSystemType.StringToValue((string)reader["SUBSYSTEMTYPE"]);
                    this.hostId = (string)reader["HOSTID"];
                    this.ownerId = (string)reader["OWNERID"];
                    this.ownerType = ElementType.StringToValue((string)reader["OWNERTYPE"]);
                    this.assemblyName = (string)reader["ASSEMBLYNAME"];
                    this.className = (string)reader["CLASSNAME"];


                    // Load All the objects blong to this
                    this.subsystems = SubSystem.LoadSubSystems(this.elementId);

                    //IO Device Load 부분 주석 처리 2009.04.20 윤선화 : 사용 안함.                    
                    //                    this.ioDevices = IODevice.LoadIODevices(this.elementId);
                    this.Observables = Observable.LoadObservables(this.elementId);


                    //2009년 05월 22일, 신승원 변경함.
                    foreach (Observable obs in this.Observables.Values)
                    {
                        //if (Global_Observables.ContainsKey(obs.ObservableGuid.ToString()) == false)
                        //{
                        //    Global_Observables.Add(obs.ObservableGuid.ToString(), obs);
                        //}
                        //else
                        //{
                        //    Global_Observables[obs.ObservableGuid.ToString()]=  obs;
                        //}
                        Global_Observables.Add(obs.ObservableGuid.ToString(), obs);
                    }

                    this.locations = MaterialLocation.LoadMaterialLocations(this.elementId);
                    this.LoadCarrierLocations();


                    this.LoadUserDefinedProperties();



                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "subsystem", this.elementId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }


        [field: NonSerialized]
        public event PropertyChangedEventHandler PropertyChanged;
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
                    try
                    {
                        PropertyChanged(this, new PropertyChangedEventArgs(propertyName + "=" + value.ToString()));
                    }
                    catch (Exception ex)
                    {
                        System.Console.WriteLine("SubSystem:OnPropertyChanged->" + ex.StackTrace);
                    }
                }
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
                    theDB.ExecuteUpdate("update TB_SUBSYSTEM set\n" +
                        "OWNERID='" + this.ownerId + "',\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "',\n" +
                        "SUBSYSTEMTYPE='" + SubSystemType.ValueToString(this.subSystemType) + "',\n" +
                        "ASSEMBLYNAME='" + this.assemblyName + "',\n" +
                        "CLASSNAME='" + this.className + "'\n" +
                        "where SUBSYSTEMID = '" + this.elementId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "sql error \n " + ex.Message.ToString());
            }
        }



        public static Dictionary<string, SubSystem> LoadSubSystems(string ownerId)
        {
            const string METHOD = CLASS + ".LoadSubSystems";

            Dictionary<string, SubSystem> subsystems = new Dictionary<string, SubSystem>();
            string subsystemId = null;
            ObjectHandle objHandle = null;
            SubSystem sub = null;

            DBConnection conn = DBControl.GetConnection();
            SqlDataReader reader;
            string sqlstring = "select SUBSYSTEMID, ASSEMBLYNAME, CLASSNAME from TB_SUBSYSTEM where OWNERID ='" + ownerId + "'";

            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "LoadSubSystems", "error", Category.ERROR, "", "subsystem", " Load Sql Error : " + ownerId + "\n" + ex.Message.ToString());
                throw ex;

            }

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    sub = (SubSystem)objHandle.Unwrap();
                    subsystemId = Helper.PreventNull((string)reader["SUBSYSTEMID"]);
                    sub.Load(ownerId, subsystemId);
                    subsystems.Add(subsystemId, sub);
                }
                catch (SqlException ex)
                {
                    Log.log(5, "LoadSubSystems", "error", Category.ERROR, "", "subsystem", "failed to load subsystems Sql Error : " + ownerId + "\n" + ex.Message.ToString());
                    throw ex;

                }
                catch (Exception ex)
                {
                    Log.log(5, "LoadSubSystems", "error", Category.ERROR, "", "subsystem", "failed to load subsystems : " + ownerId + "\n" + ex.Message.ToString());
                    throw ex;
                }

                //Console.WriteLine(subsystemId + "     " + sub.ToString() + "          " + ownerId);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return subsystems;
        }

        public static SubSystem LoadSubSystem(string ownerId, string subsystemId)
        {
            const string METHOD = CLASS + ".LoadSubSystem";

            ObjectHandle objHandle = null;
            SubSystem sub = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select ASSEMBLYNAME, CLASSNAME from TB_SUBSYSTEM where OWNERID ='" + ownerId +
                "' and SUBSYSTEMID ='" + subsystemId + "'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    sub = (SubSystem)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", " load subsystem Sql Error : " + subsystemId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "failed to load subsystem : " + subsystemId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                sub.Load(ownerId, subsystemId);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return sub;

        }

        public static SubSystem CreateSubSystem(string ownerId, ElementType ownerType, string subsystemId,
            SubSystemType subSystemType, string assemblyName, string className)
        {
            const string METHOD = CLASS + ".CreateSubSystem";
            SubSystem sub = null;
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                sub = (SubSystem)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "subsystem", "failed to create subsystem : " + subsystemId +
                    "\n" + ex.Message.ToString());
            }

            sub.elementId = subsystemId;
            sub.elementType = ElementType.SUBSYSTEM;
            sub.subSystemType = subSystemType;
            sub.ownerId = ownerId;
            sub.ownerType = ownerType;
            sub.assemblyName = assemblyName;
            sub.className = className;

            sub.Add();

            return sub;
        }

        public void Load(string ownerId, string subsystemId)
        {
            this.ownerId = ownerId;
            this.elementId = subsystemId;
            Load();
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화및기동관련
        public override void Init()
        {
            InitContainerObjects();
        }


        private void InitContainerObjects()
        {
            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].Init();
            }
        }

        public override void Startup()
        {
            this.StartupContainerObjects();
        }
        public void StartupContainerObjects()
        {
            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].Startup();
            }
        }

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
            foreach (string key in this.subsystems.Keys)
            {
                this.subsystems[key].RegisterObservableToFiledBusDriver(fieldbusDriver);
            }
        }


        public override void Shutdown()
        {
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
        #endregion 초기화및기동관련
    }
}