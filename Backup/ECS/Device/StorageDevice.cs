using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;

using ECP.Device;
using ECP.Global.Device;
using ECP.Db;
using ECP.Service.Logger;
using ECP.Util.Client;
using ECP.Service.Material;

using HECS.Device.Unit;
 

using HECS.Device.Service;
using ECP.Service.Statistics;
using HECS.Statistics;
using ECP.Service.User;
using ECP.Interfaces.Unit;

namespace HECS.Device
{

    [Serializable]
    public class StorageDevice : Equipment
    {
        const string CLASS = "StorageDevice";

        private bool dirtyFlag = true;

        //protected Dictionary<string, KeyInfomation> keyInfomations = new Dictionary<string, KeyInfomation>();

        /// <summary>
        /// RGV,S/C,LIFTER 등
        /// </summary>
        private Dictionary<string, Vehicle> _vehicles = new Dictionary<string, Vehicle>();

        public Dictionary<string, Vehicle> Vehicles
        {
            get { return _vehicles; }
            set { _vehicles = value; }
        }
        /// <summary>
        /// Stocker 적재 공간 racks
        /// </summary>
        private Dictionary<string, Rack> _racks = new Dictionary<string, Rack>();

        public Dictionary<string, Rack> Racks
        {
            get { return _racks; }
            set { _racks = value; }
        }
        /// <summary>
        /// Conveyor
        /// </summary>
        private Dictionary<string, Conveyor> _conveyors = new Dictionary<string, Conveyor>();

        public Dictionary<string, Conveyor> Conveyors
        {
            get { return _conveyors; }
            set { _conveyors = value; }
        }

        /// <summary>
        /// Ports Conveyor에 소속되어 있거나 AGV에 소속되어 있음.
        /// </summary>
        private Dictionary<string, Port> _ports = new Dictionary<string, Port>();

        public Dictionary<string, Port> Ports
        {
            get { return _ports; }
            set { _ports = value; }
        }

        /// <summary>
        /// 사용자 관리 
        /// </summary>
        protected Dictionary<string, UserManager> Users = new Dictionary<string, UserManager>();

        #region Constructor
        public StorageDevice()
            : base()
        {
        }

        public StorageDevice(string equipment)
            : base(equipment)
        {
        }

        public StorageDevice(string equipment, EquipmentType equipmentType,
            string assemblyName, string className)
            : base(equipment, equipmentType, assemblyName, className)
        {

        }
        #endregion

        #region Manage_Persistent_Attribute
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                //psparam.SetVarChar("EQUIPMENTID", this.elementId);

                //reader = theDB.ExecuteQuery("select * from STORAGEDEVICE where EQUIPMENTID =@EQUIPMENTID", psparam);

                //if (reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "stocker", this.elementId + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                //    pss.SetVarChar("EQUIPMENTID", this.elementId);

                //    theDB.ExecuteUpdate("insert into STORAGEDEVICE (EQUIPMENTID) values (@EQUIPMENTID)", pss);

                //}
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "stocker", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".delete";

            try
            {
                base.Delete(theDB);

//2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                //ps.SetVarChar("EQUIPMENTID", this.elementId);

                //theDB.ExecuteUpdate("delete from STORAGEDEVICE where EQUIPMENTID =@EQUIPMENTID", ps);

            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "stocker", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {

            }
        }

        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);
                PreparedSqlParameter psqlparam = theDB.GetPreparedSqlParameter();

                psqlparam.SetVarChar("EQUIPMENTID", this.elementId);


                reader = theDB.ExecuteQuery("select * from TB_EQUIPMENT where EQUIPMENTID =@EQUIPMENTID", psqlparam);


                if (reader.Read())
                {
                    // Equipment에 속한 Subsystem의 Reference를 모음.
                    this.LoadConveyors();
                    this.LoadRacks();
                    this.LoadVehicles();
                    this.UserLoad();
                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "stocker", this.elementId + "doesnot exist");
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StorageDevice", "Load", "Loading SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "stocker", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData("StorageDevice", "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "stocker", "error \n " + ex.Message.ToString());
                throw ex;
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
                    //theDB.ExecuteUpdate("update STOCKER set\n" +
                    //    "EQUIPMENTTYPE='" + EquipmentType.ValueToString(this.equipmentType) + "',\n" +
                    //    "ASSEMBLYNAME='" + this.assemblyName + "',\n" +
                    //    "CLASSNAME='" + this.className + "'\n" +
                    //    "where STOCKERID = '" + this.elementId + "'");
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "stocker", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {

            }
        }
        #endregion

        #region Method_Manage_Devices_Load

        /// <summary>
        /// Equipment에 소속된 Conveyor Reference를 사용하기 쉽도록 수집 함.
        /// </summary>
        private void LoadConveyors()
        {
            SubSystem sub = null;

            foreach (string key in this.subsystems.Keys)
            {
                sub = this.subsystems[key];

                if (sub.SubSystemType == SubSystemType.CONVEYOR)
                {
                    ((Conveyor)sub).LoadPort();
                    _conveyors.Add(key,(Conveyor)sub);
                }
            }
        }

        /// <summary>
        /// Equipment에 소속된 HiRack Reference를 사용하기 쉽도록 수집 함.
        /// </summary>
        private void LoadRacks()
        {
            SubSystem sub = null;

            foreach (string key in this.subsystems.Keys)
            {
                sub = this.subsystems[key];

                if (sub.SubSystemType == SubSystemType.RACK)
                {
                    _racks.Add(key, (Rack)sub);
                }
            }
        }

        /// <summary>
        /// Equipment에 소속된 Vehicle Reference를 사용하기 쉽도록 수집 함.
        /// </summary>
        private void LoadVehicles()
        {
            SubSystem sub = null;

            foreach (string key in this.subsystems.Keys)
            {
                sub = this.subsystems[key];

                if (sub.SubSystemType == SubSystemType.VEHICLE)
                {
                    _vehicles.Add(key, (Vehicle)sub);
                }
            }

        }


        private void UserLoad()
        {

        }


        /// <summary>
        /// 각 객체들의 Key를 닮는 Dictionary
        /// </summary>
        private void GenerateKeyInformation()
        {

        }

  
        #endregion


        #region 초기화및기동관련
        public override void Init()
        {
            base.Init();
        }
        public override void Startup()
        {
            base.Startup();
        }
        public override void Shutdown()
        {
            base.Shutdown();
        }
        #endregion



        #region Method

        /// <summary>
        /// Equipment의 모든 Conveyor를 리턴
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Conveyor> GetConveyors()
        {
            return _conveyors;
        }


        /// <summary>
        /// 특정 Conveyor를 가져 옴
        /// </summary>
        /// <param name="key">elementID or HostID</param>
        /// <returns>Conveyor</returns>
        public Conveyor GetConveyor(string key)
        {

            if (String.IsNullOrEmpty(key))
                return null;

            if (_conveyors.ContainsKey(key))
                return _conveyors[key];

            foreach (Conveyor cnv in _conveyors.Values)
            {
                if(string.IsNullOrEmpty (cnv.Id) == false && cnv.Id == key)
                {
                    return cnv ;
                }

                if (string.IsNullOrEmpty(cnv.HostID) == false && cnv.HostID == key)
                {
                    return cnv;
                }
                if (string.IsNullOrEmpty(cnv.ElementId) == false && cnv.ElementId == key)
                {
                    return cnv; 
                }
            }
            return null;

        }

        /// <summary>
        /// Equipment의 모든 Vehicles
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Vehicle> GetVehicles()
        {
            return _vehicles;
        }

        /// <summary>
        /// 특정 Vehicle를 가져 옴
        /// </summary>
        /// <param name="key">elementID or HostID</param>
        /// <returns>Vehicle</returns>
        public Vehicle GetVehicle(string key)
        {

            if (String.IsNullOrEmpty(key))
                return null;

            if (_vehicles.ContainsKey(key))
                return _vehicles[key];

            foreach (Vehicle vehicle in _vehicles.Values)
            {
                if(string.IsNullOrEmpty (vehicle.Id) == false && vehicle.Id == key)
                {
                    return vehicle;
                }
                if (string.IsNullOrEmpty(vehicle.HostID) == false && vehicle.HostID == key)
                {
                    return vehicle;
                }
                if (string.IsNullOrEmpty(vehicle.ElementId) == false && vehicle.ElementId == key)
                {
                    return vehicle;
                }
                return null; 
              
            }
            return null;

        }

        /// <summary>
        /// Equipment에 소속된 모든 Ports
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Port> GetPorts()
        {
            return _ports;
        }

        /// <summary>
        /// 특정 Port를 가져 옴, 그러나 일반 적인경우 Conveyor를 통해 Port로 접근하기 바랍
        /// </summary>
        /// <param name="key">elementID or HostID</param>
        /// <returns>Vehicle</returns>

        public Port GetPort(string key)
        {
            if (String.IsNullOrEmpty(key))
                return null;

            if (_ports.ContainsKey(key))
                return _ports[key];

            foreach (Port port in _ports.Values)
            {
                if(string.IsNullOrEmpty (port.Id) == false && port.Id == key)
                {
                    return port; 
                }
                else if (string.IsNullOrEmpty(port.HostID) == false && port.HostID == key)
                {
                    return port; 
                }
                else if (string.IsNullOrEmpty(port.ElementId) == false && port.ElementId == key)
                {
                    return port;
                }
            }
            return null;

        }

        #endregion Method
    }
}
