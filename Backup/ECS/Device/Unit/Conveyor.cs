using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;

using ECP.Device;
using ECP.Device.Observe;
using ECP.Service.Statistics;
using ECP.Db;
using ECP.Service.Logger;
using ECP.Interfaces.Unit;

using HECS.Statistics;
using System.Timers;
using System.Windows.Forms;
using ECP.Service.Movement;
using ECP.Util.Server;
using ECP.Db.Entity;
using HECS.Device.Dispatcher;
using ECP.Service.Device;
using ECP.Global;



namespace HECS.Device.Unit
{
    [Serializable]
    public partial class Conveyor : SubSystem
    {
        private const string CLASS = "Conveyor";
        System.Timers.Timer timerEventChecker = new System.Timers.Timer(1000);
        private bool dirtyFlag = true;
        private string[] palletArray = new string[10];
        public bool DirtyFlag
        {
            get
            {
                return dirtyFlag;
            }
            set
            {
                dirtyFlag = value;
            }
        }

        #region Constructor

        public Conveyor()
        {
            timerEventChecker.Elapsed += new ElapsedEventHandler(timerEventChecker_Elapsed);
        }

        void timerEventChecker_Elapsed(object sender, ElapsedEventArgs e)
        {
            Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:1");
            if (vehicle.VehicleState != Vehicle.VEHICLE_STATE.RUN)
            {
                if (this.IsWaitOut == true)
                {
                    timerEventChecker.Enabled = false;
                    OnWaitOut(this.IsWaitOut);
                }
            }
        }

        #endregion Constructor



        #region Attribute_Not_Persistance

        /// <summary>
        /// Conveyror가 가진 Port들
        /// </summary>
        private Dictionary<string, Port> ports = new Dictionary<string, Port>();

        public Dictionary<string, Port> Ports
        {
            get
            {
                return ports;
            }
            set
            {
                ports = value;
            }
        }

        public enum CONVEYOR_OPERATION_MODE
        {
            MANUAL = 0,
            AUTO = 1,
        }

        private CONVEYOR_OPERATION_MODE operationMode = CONVEYOR_OPERATION_MODE.MANUAL;

        public CONVEYOR_OPERATION_MODE OperationMode
        {
            get
            {
                return operationMode;
            }
            set
            {
                operationMode = value;
                OnPropertyChanged("OperationMode", value);
            }
        }

        public enum CONVEYOR_DIRECTION_MODE
        {
            RGVINGO = 0,
            RGVOUTGO = 1,
        }

        private CONVEYOR_DIRECTION_MODE directionMode = CONVEYOR_DIRECTION_MODE.RGVINGO;

        public CONVEYOR_DIRECTION_MODE DirectionMode
        {
            get
            {
                return directionMode;
            }
            set
            {
                directionMode = value;
                OnPropertyChanged("DirectionMode", value);
            }
        }

        public enum CONVEYOR_RUN_STATE
        {
            DOWN = 0,
            IDLE = 1,
            RUN = 2,
        }

        private CONVEYOR_RUN_STATE runState = CONVEYOR_RUN_STATE.IDLE;

        public CONVEYOR_RUN_STATE RunState
        {
            get
            {
                return runState;
            }
            set
            {
                runState = value;
                OnPropertyChanged("RunState", value);
            }
        }


        private bool isUnloadComplete = false;

        public bool IsUnloadComplete
        {
            get
            {
                return isUnloadComplete;
            }
            set
            {
                isUnloadComplete = value;
                OnPropertyChanged("IsUnloadComplete", value);
            }
        }
        private bool isLoadComplete = false;

        public bool IsLoadComplete
        {
            get
            {
                return isLoadComplete;
            }
            set
            {
                isLoadComplete = value;
                OnPropertyChanged("IsLoadComplete", value);
            }
        }
        private bool isUnloadComplete2 = false;

        public bool IsUnloadComplete2
        {
            get
            {
                return isUnloadComplete2;
            }
            set
            {
                isUnloadComplete2 = value;
                OnPropertyChanged("IsUnloadComplete2", value);
            }
        }
        private bool isLoadComplete2 = false;

        public bool IsLoadComplete2
        {
            get
            {
                return isLoadComplete2;
            }
            set
            {
                isLoadComplete2 = value;
                OnPropertyChanged("IsLoadComplete2", value);
            }
        }
        private bool isWaitIn = false;

        public bool IsWaitIn
        {
            get
            {
                return isWaitIn;
            }
            set
            {
                isWaitIn = value;
                OnPropertyChanged("IsWaitIn", value);
            }
        }
        private bool isWaitOut = false;

        public bool IsWaitOut
        {
            get
            {
                return isWaitOut;
            }
            set
            {
                isWaitOut = value;
                OnPropertyChanged("IsWaitOut", value);
            }
        }
        private bool inReady = false;

        public bool InReady
        {
            get
            {
                return inReady;
            }
            set
            {
                inReady = value;
                OnPropertyChanged("InReady", value);
            }
        }

        private bool isPalletExist01 = false;

        public bool IsPalletExist01
        {
            get
            {
                return isPalletExist01;
            }
            set
            {
                isPalletExist01 = value;
                OnPropertyChanged("IsPalletExist01", value);
            }
        }
        private bool isPalletExist02 = false;

        public bool IsPalletExist02
        {
            get
            {
                return isPalletExist02;
            }
            set
            {
                isPalletExist02 = value;
                OnPropertyChanged("IsPalletExist02", value);
            }
        }
        private bool isPalletExist03 = false;

        public bool IsPalletExist03
        {
            get
            {
                return isPalletExist03;
            }
            set
            {
                isPalletExist03 = value;
                OnPropertyChanged("IsPalletExist03", value);
            }
        }
        private bool isPalletExist04 = false;

        public bool IsPalletExist04
        {
            get
            {
                return isPalletExist04;
            }
            set
            {
                isPalletExist04 = value;
                OnPropertyChanged("IsPalletExist04", value);
            }
        }
        private bool isPalletExist05 = false;

        public bool IsPalletExist05
        {
            get
            {
                return isPalletExist05;
            }
            set
            {
                isPalletExist05 = value;
                OnPropertyChanged("IsPalletExist05", value);
            }
        }
        private bool isPalletExist06 = false;

        public bool IsPalletExist06
        {
            get
            {
                return isPalletExist06;
            }
            set
            {
                isPalletExist06 = value;
                OnPropertyChanged("IsPalletExist06", value);
            }
        }
        private bool isPalletExist07 = false;

        public bool IsPalletExist07
        {
            get
            {
                return isPalletExist07;
            }
            set
            {
                isPalletExist07 = value;
                OnPropertyChanged("IsPalletExist07", value);
            }
        }
        private bool isPalletExist08 = false;

        public bool IsPalletExist08
        {
            get
            {
                return isPalletExist08;
            }
            set
            {
                isPalletExist08 = value;
                OnPropertyChanged("IsPalletExist08", value);
            }
        }
        private bool isPalletExist09 = false;

        public bool IsPalletExist09
        {
            get
            {
                return isPalletExist09;
            }
            set
            {
                isPalletExist09 = value;
                OnPropertyChanged("IsPalletExist09", value);
            }
        }
        private bool isPalletExist10 = false;

        public bool IsPalletExist10
        {
            get
            {
                return isPalletExist10;
            }
            set
            {
                isPalletExist10 = value;
                OnPropertyChanged("IsPalletExist10", value);
            }
        }
        #endregion Attribute_Not_Persistance

        #region Manage_Persistent_Attribute

        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "CONVEYOR", "sql error \n " + ex.Message.ToString());
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

        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);
                this.Id = this.elementId;
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Delete", "Delete SQL Error", ex.ToString(), ex.StackTrace.ToString()));
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
                //if (reader != null) reader.Close();
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
                //ps.SetVarChar("ID", this.ID);

                //theDB.ExecuteUpdate("delete from PORT where ID =@ID", ps);
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "Port", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading Error", ex.ToString(), ex.StackTrace.ToString()));
                throw ex;
            }
            finally
            {

            }
        }

        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".save";

            try
            {
                base.Save(theDB);

                //2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //if (dirtyFlag)
                //{
                //    this.dirtyFlag = false;
                //    theDB.ExecuteUpdate("update CONVEYOR set\n" +
                //        "ID='" + this.iD + "', \n" +
                //        "HOSTID='" + this.hostID + "'\n" +
                //        "where ID = '" + this.iD + "'");
                //}
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Save", "Update SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "CONVEYOR", "sql error \n " + ex.Message.ToString());
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
        }

        #endregion Manage_Persistent_Attribute

        #region 초기화 및 기동 관련

        public override void Init()
        {
            base.Init();

            AddSubScribe();
        }

        public override void Startup()
        {
            base.Startup();
        }

        public override void Shutdown()
        {
            base.Shutdown();
        }
        #endregion 초기화 및 기동 관련

        #region Method

        /// <summary>
        /// 자기 Conveyor에 속한 Port의 Reference를 저장함.
        /// </summary>
        public void LoadPort()
        {
            SubSystem sub = null;

            foreach (string key in this.subsystems.Keys)
            {
                sub = this.subsystems[key];

                if (sub.SubSystemType == ECP.Global.Device.SubSystemType.PORT)
                {
                    ports.Add(key, (Port)sub);
                }
            }

        }

        /// <summary>
        /// Conveyor에 소속된 Port를 줌.
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Port> GetPorts()
        {
            return ports;
        }
        public string GetPalletFromPLC(Port port)
        {
            switch (port.PortOrder)
            {
                case "1":
                    palletArray[0] = this.IO_PALLET_EXIST01;
                    port.PalletId = palletArray[0];
                    return palletArray[0];
                case "2":
                    palletArray[1] = this.IO_PALLET_EXIST02;
                    port.PalletId = palletArray[1];
                    return palletArray[1];
                case "3":
                    palletArray[2] = this.IO_PALLET_EXIST03;
                    port.PalletId = palletArray[2];
                    return palletArray[2];
                case "4":
                    palletArray[3] = this.IO_PALLET_EXIST04;
                    port.PalletId = palletArray[3];
                    return palletArray[3];
                case "5":
                    palletArray[4] = this.IO_PALLET_EXIST05;
                    port.PalletId = palletArray[4];
                    return palletArray[4];

                //LGLS 주석 처리
                //case "6":
                //    palletArray[5] = this.IO_PALLET_EXIST06;
                //    port.PalletId = palletArray[5];
                //    return palletArray[5];
                //case "7":
                //    palletArray[6] = this.IO_PALLET_EXIST07;
                //    port.PalletId = palletArray[6];
                //    return palletArray[6];
                //case "8":
                //    palletArray[7] = this.IO_PALLET_EXIST08;
                //    port.PalletId = palletArray[7];
                //    return palletArray[7];
                //case "9":
                //    palletArray[8] = this.IO_PALLET_EXIST09;
                //    port.PalletId = palletArray[8];
                //    return palletArray[8];
                //case "10":
                //    palletArray[9] = this.IO_PALLET_EXIST10;
                //    port.PalletId = palletArray[9];
                //    return palletArray[9];
            }
            return null;
        }
        public string GetPallet(Port port)
        {
            
            string palletId = palletArray[int.Parse(port.PortOrder) - 1];
            if (palletId == null)
            {
                GetPalletFromPLC(port);
            }
            palletId = palletArray[int.Parse(port.PortOrder) - 1];
            if (palletId == null)
            {
                return "";
            }
            else
            {
                return palletId;
            }
        }
        public void SetPallet(Port port, string palletId)
        {
            switch (port.PortOrder)
            {
                case "1":
                    this.IO_PALLET_EXIST01 = palletId;
                    break;
                case "2":
                    this.IO_PALLET_EXIST02 = palletId;
                    break;
                case "3":
                    this.IO_PALLET_EXIST03 = palletId;
                    break;
                case "4":
                    this.IO_PALLET_EXIST04 = palletId;
                    break;
                case "5":
                    this.IO_PALLET_EXIST05 = palletId;
                    break;

                //LGLS 주석 처리
                //case "6":
                //    this.IO_PALLET_EXIST06 = palletId;
                //    break;
                //case "7":
                //    this.IO_PALLET_EXIST07 = palletId;
                //    break;
                //case "8":
                //    this.IO_PALLET_EXIST08 = palletId;
                //    break;
                //case "9":
                //    this.IO_PALLET_EXIST09 = palletId;
                //    break;
                //case "10":
                //    this.IO_PALLET_EXIST10 = palletId;
                //    break;
            }
            return;

        }
        /// <summary>
        /// Conveyor에 소속된 특정 Port를 가져 옴
        /// </summary>
        /// <param name="key">elementid or hostid</param>
        /// <returns>Port</returns>
        public Port GetPort(string key)
        {
            if (String.IsNullOrEmpty(key))
                return null;

            if (ports.ContainsKey(key))
                return ports[key];

            foreach (Port port in ports.Values)
            {
                if (string.IsNullOrEmpty(port.HostID) == false && port.HostID == key)
                {
                    return port;
                }
                else if (string.IsNullOrEmpty(port.ElementId) == false && port.ElementId == key)
                {
                    return port;
                }
                else if (string.IsNullOrEmpty(port.Id) == false && port.Id == key)
                {
                    return port;
                }
            }
            return null;
        }

        public Port GetPortByOrder(string order)
        {
            if (String.IsNullOrEmpty(order))
                return null;

            foreach (Port port in ports.Values)
            {
                if (string.IsNullOrEmpty(port.PortOrder) == false && port.PortOrder == order)
                {
                    return port;
                }

            }
            return null;
        }

        #endregion Mehod


        #region Event_Handler
        public void AddSubScribe()
        {
            if (this.observables.Count != 0)
            {

                this.__IO_IN_READY_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_IN_READY_02_ObservableValueChanged);
                this.__IO_LOAD_COMPLETE_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_LOAD_COMPLETE_01_ObservableValueChanged);
                this.__IO_LOAD_COMPLETE_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_LOAD_COMPLETE_02_ObservableValueChanged);
                this.__IO_OPERATION_MODE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_OPERATION_MODE_ObservableValueChanged);
                this.__IO_PALLET_EXIST01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST01_ObservableValueChanged);
                this.__IO_PALLET_EXIST02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST02_ObservableValueChanged);
                this.__IO_PALLET_EXIST03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST03_ObservableValueChanged);
                this.__IO_PALLET_EXIST04.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST04_ObservableValueChanged);
                this.__IO_PALLET_EXIST05.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST05_ObservableValueChanged);

                //LGLS 주석 처리(2009.12.28)
                //this.__IO_PALLET_EXIST06.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST06_ObservableValueChanged);
                //this.__IO_PALLET_EXIST07.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST07_ObservableValueChanged);
                //this.__IO_PALLET_EXIST08.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST08_ObservableValueChanged);
                //this.__IO_PALLET_EXIST09.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST09_ObservableValueChanged);
                //this.__IO_PALLET_EXIST10.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST10_ObservableValueChanged);

                this.__IO_PALLET_EXIST_FLAG_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_01_ObservableValueChanged);
                this.__IO_PALLET_EXIST_FLAG_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_02_ObservableValueChanged);
                this.__IO_PALLET_EXIST_FLAG_03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_03_ObservableValueChanged);
                this.__IO_PALLET_EXIST_FLAG_04.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_04_ObservableValueChanged);
                this.__IO_PALLET_EXIST_FLAG_05.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_05_ObservableValueChanged);

                //LGLS 주석 처리(2009.12.28)
                //this.__IO_PALLET_EXIST_FLAG_06.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_06_ObservableValueChanged);
                //this.__IO_PALLET_EXIST_FLAG_07.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_07_ObservableValueChanged);
                //this.__IO_PALLET_EXIST_FLAG_08.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_08_ObservableValueChanged);
                //this.__IO_PALLET_EXIST_FLAG_09.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_09_ObservableValueChanged);
                this.__IO_UNLOAD_COMPLETE_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_UNLOAD_COMPLETE_01_ObservableValueChanged);
                this.__IO_UNLOAD_COMPLETE_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_UNLOAD_COMPLETE_02_ObservableValueChanged);
                this.__IO_WAIT_IN.ObservableValueChanged += new DelegateObservableValueChanged(__IO_WAIT_IN_ObservableValueChanged);
                this.__IO_WAIT_OUT.ObservableValueChanged += new DelegateObservableValueChanged(__IO_WAIT_OUT_ObservableValueChanged);

                this.__IO_UNLOAD_REQUEST_ACK_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_UNLOAD_REQUEST_ACK_02_ObservableValueChanged);
                //LGLS 주석 처리
                //if (this.observables.ContainsKey("OVERLOAD_ALARM"))
                //{
                //    this.__IO_OVERLOAD_ALARM.ObservableValueChanged += new DelegateObservableValueChanged(__IO_OVERLOAD_ALARM_ObservableValueChanged);
                //}

                //if (this.observables.ContainsKey("OVERTIME_ALARM"))
                //{
                //    this.__IO_OVERTIME_ALARM.ObservableValueChanged += new DelegateObservableValueChanged(__IO_OVERTIME_ALARM_ObservableValueChanged);
                //}

                this.__IO_DIRECTION_MODE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_DIRECTION_MODE_ObservableValueChanged);

                foreach (string key in this.Observables.Keys)
                {
                    Observables[key].ObservableLogEvent += new delegateObservableLogEvent(Conveyor_ObservableLogEvent);
                }

            }
        }

        void __IO_OVERTIME_ALARM_ObservableValueChanged(object sender, object value)
        {
            List<short> valueList = (List<short>)value;
            OnOvertimeAlarm(valueList);
        }

        void __IO_OVERLOAD_ALARM_ObservableValueChanged(object sender, object value)
        {
            List<short> valueList = (List<short>)value;
            OnOverloadAlarm(valueList);
        }


        void Conveyor_ObservableLogEvent(object sender, ObservableStatisticsCollectorData collectionData)
        {
            try
            {
                collectionData.ownerId = this.HostID;
                StatisticsCollector.TriggerOccured(
                                                    ObservableStatisticsCollectorData.COLLECTOR_ID,
                                                    ObservableStatisticsCollectorData.TRIGGER_ID,
                                                    collectionData);
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                    RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                    RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                    new RunTimeExceptionStatisticsCollectorData(CLASS, "Conveyor_ObservableLogEvent", "", ex.ToString(), ex.StackTrace.ToString()));
            }
        }

        #endregion Event_Handler

        #region Event
        void __IO_IN_READY_02_ObservableValueChanged(object sender, object value)
        {
            OnInputReady("2", (bool)value);
        }



        void __IO_LOAD_COMPLETE_01_ObservableValueChanged(object sender, object value)
        {
            OnLoadComplete01((bool)value);
        }



        void __IO_LOAD_COMPLETE_02_ObservableValueChanged(object sender, object value)
        {
            OnLoadComplete02((bool)value);
        }

        void __IO_OPERATION_MODE_ObservableValueChanged(object sender, object value)
        {
            OnOperationMode((bool)value);
        }
        void __IO_PALLET_EXIST01_ObservableValueChanged(object sender, object value)
        {
            OnPallet("1", (string)value);
        }

        void __IO_PALLET_EXIST02_ObservableValueChanged(object sender, object value)
        {
            OnPallet("2", (string)value);
        }
        void __IO_PALLET_EXIST03_ObservableValueChanged(object sender, object value)
        {
            OnPallet("3", (string)value);
        }
        void __IO_PALLET_EXIST04_ObservableValueChanged(object sender, object value)
        {
            OnPallet("4", (string)value);
        }
        void __IO_PALLET_EXIST05_ObservableValueChanged(object sender, object value)
        {
            OnPallet("5", (string)value);
        }
        void __IO_PALLET_EXIST06_ObservableValueChanged(object sender, object value)
        {
            OnPallet("6", (string)value);
        }
        void __IO_PALLET_EXIST07_ObservableValueChanged(object sender, object value)
        {
            OnPallet("7", (string)value);
        }
        void __IO_PALLET_EXIST08_ObservableValueChanged(object sender, object value)
        {
            OnPallet("8", (string)value);
        }
        void __IO_PALLET_EXIST09_ObservableValueChanged(object sender, object value)
        {
            OnPallet("9", (string)value);
        }
        void __IO_PALLET_EXIST10_ObservableValueChanged(object sender, object value)
        {
            OnPallet("10", (string)value);
        }

        void __IO_PALLET_EXIST_FLAG_01_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("1", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_02_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("2", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_03_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("3", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_04_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("4", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_05_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("5", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_06_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("6", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_07_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("7", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_08_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("8", (bool)value);
        }

        void __IO_PALLET_EXIST_FLAG_09_ObservableValueChanged(object sender, object value)
        {
            OnPalletExist("9", (bool)value);
        }

        void __IO_UNLOAD_COMPLETE_01_ObservableValueChanged(object sender, object value)
        {
            OnUnloadComplete01((bool)value);
        }

        void __IO_UNLOAD_COMPLETE_02_ObservableValueChanged(object sender, object value)
        {
            OnUnloadComplete02((bool)value);
        }

        void __IO_WAIT_IN_ObservableValueChanged(object sender, object value)
        {
            OnWaitIn((bool)value);
        }
        void __IO_WAIT_OUT_ObservableValueChanged(object sender, object value)
        {
            OnWaitOut((bool)value);
        }
        void __IO_UNLOAD_REQUEST_ACK_02_ObservableValueChanged(object sender, object value)
        {
            OnUnloadRequestAck02((bool)value);
        }

        void __IO_DIRECTION_MODE_ObservableValueChanged(object sender, object value)
        {
            OnDirectionMode((string)value);
        }

        #endregion Event

        private void OnLoadComplete01(bool value)
        {
            this.IsLoadComplete = value;
            if (value == true)
            {
                this.observables["LOAD_COMPLETE_ACK_01"].Value = true;
            }
        }
        private void OnLoadComplete02(bool value)
        {
            this.IsLoadComplete2 = value;
            if (value == true)
            {
                this.observables["LOAD_COMPLETE_ACK_02"].Value = true;
            }
        }
        private void OnUnloadComplete01(bool value)
        {
            this.IsUnloadComplete = value;
            if (value == true)
            {
                this.observables["UNLOAD_COMPLETE_ACK_01"].Value = true;
            }
        }
        private void OnUnloadComplete02(bool value)
        {
            this.IsUnloadComplete2 = value;
            if (value == true)
            {
                this.observables["UNLOAD_COMPLETE_ACK_02"].Value = true;
            }
        }
        private void OnInputReady(string portNo, bool value)
        {
            this.InReady = value;
        }

        private void OnOperationMode(bool value)
        {
            if (value == true)
            {
                this.OperationMode = CONVEYOR_OPERATION_MODE.AUTO;
            }
            else
            {
                this.OperationMode = CONVEYOR_OPERATION_MODE.MANUAL;

            }
        }

        private void OnPalletExist(string portNo, bool value)
        {
            Port port = null;
            port = ECSDeviceManager.GetPortAtConveyor(this.elementId, portNo);
            if (port == null)
            {
                return;
            }
            port.IsPalletExist = value;
            OnPropertyChanged("OnPalletExist", value);
            if (port.IsPalletExist == false)
            {
               //Pallet Exist가 0이면 값을 지움.
               OnPallet(port.PortOrder.ToString(), "");
            }
            else
            {
                string palletid = GetPallet(port);

                //if (string.IsNullOrEmpty(palletid))
                //{

                //    if (this.ElementId == "CONVEYOR:11" || this.ElementId == "CONVEYOR:12" || this.ElementId == "CONVEYOR:15")
                //    {
                //        palletid = GetPalletFromPLC(port);
                //        if (string.IsNullOrEmpty(palletid))
                //        {
                //            SetPallet(port, "XXXX");
                //        }
                //    }
                //    return;
                //}

                TransferData transfer = ECSDispatcher.GetTransferDataByPalletId(palletid);
                if (transfer == null)
                {
                    //에러 로그 
                    return;
                }
                TransferDetailData transferDetail = ECSDispatcher.GetTransferDetailByPalletId(transfer.Commandid, palletid);
                if (transferDetail == null)
                {
                    return;
                }
                transferDetail.Palletid = palletid;
                transferDetail.Currentpalletlocation = port.ElementId;
            }
        }


        private void OnWaitIn(bool value)
        {
            this.IsWaitIn = value;
            if (this.IsWaitIn == false)
            {
                return;
            }
        }

        private void OnPallet(string index, string value)
        {
            Port port = null;
            port = ECSDeviceManager.GetPortAtConveyor(this.elementId, index);
            if (port == null)
            {
                return;
            }
            string palletid = GetPalletFromPLC(port);
            palletArray[int.Parse(index) - 1] = palletid;
            OnPropertyChanged("OnPallet", palletid);

            //OnPropertyChanged("OnPallet", value);
            //System.Console.WriteLine("Pallet ID Tracking : " + this.elementId + "->" + "PORT:" + index + "=" + value);
            //Pallet Id 트래킹 관련 로그를 남겨줘야 한다.

        }

        private void OnWaitOut(bool value)
        {
            this.IsWaitOut = value;

            if (this.isWaitOut == false)
            {
                return;
            }

            if (this.elementId == "CONVEYOR:11")
            {
                if (this.Observables["DIRECTION_MODE"].Value.ToString() == "1")
                {
                    return;
                }
            }

            if (this.elementId == "CONVEYOR:13")
            {
                return;
            }

            if (this.elementId == "CONVEYOR:14")
            {
                Port port1 = ECSDeviceManager.GetWaitOutPort(this.elementId);
                string palletId1 = GetPalletFromPLC(port1);
                if (string.IsNullOrEmpty(palletId1))
                {
                    // 에러처리 및 에러로그 
                    return;
                }
                //else
                //{
                //    this.observables["UNLOAD_REQUEST_02"].Value = true;
                //    return;
                //}
            }

            Port port = ECSDeviceManager.GetWaitOutPort(this.elementId);
            string palletId = GetPalletFromPLC(port);
            if (string.IsNullOrEmpty(palletId))
            {
                // 에러처리 및 에러로그 
                return;
            }
            TransferData transferData = ECSDispatcher.GetTransferDataByPalletId(palletId);
            if (transferData == null)
            {
                return;
            }
            TransferDetailData transferDetail = ECSDispatcher.GetTransferDetailByPalletId(transferData.Commandid, palletId);
            if (transferDetail == null)
            {
                // 에러처리 및 에러로그
                return;
            }
            else  
            {
                //두번 Wait Out으로 transferDetail이 다음 반송을 가져왔을 경우 Return
                ItineraryData itineraryCheck = new ItineraryData();
                itineraryCheck.getItineraryData(transferDetail.Pathid);

                //C/V의 Wait Out은 To Location이 현재 장비와 같아야 함.
                if (this.elementId != itineraryCheck.Tolocationid)
                {
                    return;
                }
            }
            TransferDetailData transferDetailNext = ECSDispatcher.GetNextTransferDetail(transferData.Commandid, transferDetail.Commandseq);

            if (transferDetailNext == null)
            {
                MovementManager manager = Naming.GetMovementManager();
                manager.chageTransferDetailComplete(transferData.Commandid, transferDetail.Commandseq);

                ItineraryData itinerary = new ItineraryData();
                itinerary.getItineraryData(transferDetail.Pathid);

                ////Pallet 정보 Update를 위해 추가
                PalletData palletData = new PalletData();
                palletData.getPalletData(transferData.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = itinerary.Todeviceid;
                palletData.Locationid = itinerary.Tolocationid;
                palletData.Sublocationid = itinerary.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                palletData.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transferData.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    palletData.updatePalletData(theDB, true);
                }
                catch (SqlException se)
                {
                    //Error Log
                }
                finally
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }
            else
            {
                MovementManager manager = Naming.GetMovementManager();
                transferDetailNext.Palletidtomove = palletId;
                transferDetailNext.updateTransferDetailData();

                //HISTORY Backup LGLS를 위해 추가함.
                try
                {
                    transferDetailNext.insertTransferDetailDataBackup();
                }
                catch (Exception ex)
                {
                    //throw ex;
                }

                manager.chageTransferDetailComplete(transferData.Commandid, transferDetail.Commandseq);


                ItineraryData itinerary = new ItineraryData();
                itinerary.getItineraryData(transferDetail.Pathid);

                ////Pallet 정보 Update를 위해 추가
                PalletData palletData = new PalletData();
                palletData.getPalletData(transferData.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = itinerary.Todeviceid;
                palletData.Locationid = itinerary.Tolocationid;
                palletData.Sublocationid = itinerary.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                palletData.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transferData.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    palletData.updatePalletData(theDB, true);
                }
                catch (SqlException se)
                {
                    //Error Log
                }
                finally
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            // 반송완료처리 
        }
        private void OnOvertimeAlarm(List<short> valueList)
        {
            System.Console.WriteLine("OverTimeAlarm이 발생했습니다.");
            int sum = 0;
            foreach (short val in valueList)
            {
                sum = sum + val;
            }
            if (sum == 0)
            {
                Util.Util.AlarmReset();
                //MessageBox.Show("Overtime Alarm이 해제되었습니다.", "확인", MessageBoxButtons.OK);
            }
            else
            {
                Util.Util.Alarm3();
                //MessageBox.Show("Overtime Alarm이 발생하였습니다..", "확인", MessageBoxButtons.OK, MessageBoxIcon.Error);

                ErrorHistData errorhistData = new ErrorHistData();
                errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Errortype = "Conveyor";
                errorhistData.Errorcode = "C0001";
                errorhistData.Errortext = "Conveyor: " + this.ElementId + "에서 C/V Overtime 알람이 발생했습니다.";

                try
                {
                    errorhistData.insertErrorHistData();
                }
                catch (Exception e)
                {
                    //No 처리
                }
            }
        }
        private void OnOverloadAlarm(List<short> valueList)
        {
            System.Console.WriteLine("OverloadAlarm이 발생했습니다.");
            int sum = 0;
            foreach (short val in valueList)
            {
                sum = sum + val;
            }
            if (sum == 0)
            {
                //MessageBox.Show("Overload Alarm이 해제되었습니다.", "확인", MessageBoxButtons.OK);
                Util.Util.AlarmReset();
            }
            else
            {
                Util.Util.Alarm3();

                ErrorHistData errorhistData = new ErrorHistData();
                errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Errortype = "Conveyor";
                errorhistData.Errorcode = "C0002";
                errorhistData.Errortext = "Conveyor: " + this.ElementId + "에서 C/V Overload 알람이 발생했습니다.";

                try
                {
                    errorhistData.insertErrorHistData();
                }
                catch (Exception e)
                {
                    //No 처리
                }

                //MessageBox.Show("Overload Alarm이 발생하였습니다..", "확인", MessageBoxButtons.OK,MessageBoxIcon.Error );
            }
        }

        private void OnUnloadRequestAck02(bool value)
        {
            //시뮬레이션을 위해서만 존재해야 함. 
            //삭제해야할 소스
            if (value == false)
            {
                return;
            }

            //시뮬레이션 모드일 경우만 처리함.
            FieldbusDriverData fbdriver = new FieldbusDriverData();
            string simulatorFlag = "N";

            try
            {
                fbdriver.getFieldbusDriverData("T");

                if (fbdriver.Drivertype == EntityEnumData.FIELDBUSDRIVERTYPE.SIMULATION.ToString())
                {
                    simulatorFlag = "Y";
                }
                else
                {
                    simulatorFlag = "N";
                }
            }
            catch (Exception ex)
            {
                simulatorFlag = "N";
            }

            if (simulatorFlag == "Y")
            {
                if (this.ElementId == "CONVEYOR:14")
                {
                    Conveyor conv = ECSDeviceManager.GetConveyor("CONVEYOR:15");

                    if (conv != null)
                    {
                        this.Observables["UNLOAD_REQUEST_ACK_02"].Value = false;
                        this.Observables["PALLET_EXIST_FLAG_03"].Value = false;
                        this.OnPalletExist("3", false);

                        conv.Observables["PALLET_EXIST_FLAG_03"].Value = true;
                        conv.OnPalletExist("3", true);
                    }
                }
            }
        }

        private void OnDirectionMode(string value) 
        {
            if (value == "0")
            {
                this.DirectionMode = CONVEYOR_DIRECTION_MODE.RGVINGO;
            }
            else
            {
                this.DirectionMode = CONVEYOR_DIRECTION_MODE.RGVOUTGO;

            }

            //Conveyor:11의 MODE 관리
            if (this.ElementId == "CONVEYOR:11")
            {
                GlobalConstant.CONVEYOR11_MODE = value;
            }
        }
    }
}
