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
using HECS.Device.Dispatcher;
using ECP.Service.Movement;
using ECP.Util.Server;
using System.Timers;
using ECP.Db.Entity;
using ECP.BizRule;
using ECP.Global;

namespace HECS.Device.Unit
{

    public partial class Vehicle : SubSystem
    {

        private Timer timerClearCommand = new Timer();
        private int clearCommandCount = 0;
        public enum VEHICLE_STATE
        {
            DOWN = 0,
            IDLE = 1,
            RUN = 2,
        }
        public enum VEHICLE_OPERATION_MODE
        {
            MANUAL = 0,
            AUTO = 1,
        }

        private const string CLASS = "Vehicle";
        private bool dirtyFlag = true;

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




        #region Properties
        private bool isAlarmResetReport = false;

        public bool IsAlarmResetReport
        {
            get
            {
                return isAlarmResetReport;
            }
            set
            {
                isAlarmResetReport = value;
                OnPropertyChanged("IsAlarmResetReport", value);
            }
        }
        private bool isAlarmSetReport = false;

        public bool IsAlarmSetReport
        {
            get
            {
                return isAlarmSetReport;
            }
            set
            {
                isAlarmSetReport = value;
                OnPropertyChanged("IsAlarmSetReport", value);

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
        private bool isPalletExist = false;

        public bool IsPalletExist
        {
            get
            {
                return isPalletExist;
            }
            set
            {
                isPalletExist = value;
                OnPropertyChanged("IsPalletExist", value);

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
        private bool isTransferRequestAck = false;

        public bool IsTransferRequestAck
        {
            get
            {
                return isTransferRequestAck;
            }
            set
            {
                isTransferRequestAck = value;
                OnPropertyChanged("IsTransferRequestAck", value);

            }
        }
        private bool isTransferRequest = false;

        public bool IsTransferRequest
        {
            get
            {
                return isTransferRequest;
            }
            set
            {
                isTransferRequest = value;
                OnPropertyChanged("IsTransferRequest", value);

            }
        }
        private bool isLoadCompleteAck = false;

        public bool IsLoadCompleteAck
        {
            get
            {
                return isLoadCompleteAck;
            }
            set
            {
                isLoadCompleteAck = value;
                OnPropertyChanged("IsLoadCompleteAck", value);

            }
        }
        private bool isUnloadCompleteAck = false;

        public bool IsUnloadCompleteAck
        {
            get
            {
                return isUnloadCompleteAck;
            }
            set
            {
                isUnloadCompleteAck = value;
                OnPropertyChanged("IsUnloadCompleteAck", value);

            }
        }
        private bool isAlarmSetReportAck = false;

        public bool IsAlarmSetReportAck
        {
            get
            {
                return isAlarmSetReportAck;
            }
            set
            {
                isAlarmSetReportAck = value;
                OnPropertyChanged("IsAlarmSetReportAck", value);

            }
        }
        private bool isAlarmResetReportAck = false;

        public bool IsAlarmResetReportAck
        {
            get
            {
                return isAlarmResetReportAck;
            }
            set
            {
                isAlarmResetReportAck = value;
                OnPropertyChanged("IsAlarmResetReportAck", value);

            }
        }
        private short alarmResetCode = 0;

        public short AlarmResetCode
        {
            get
            {
                return alarmResetCode;
            }
            set
            {
                alarmResetCode = value;
                OnPropertyChanged("AlarmResetCode", value);

            }
        }
        private short alarmSetCode = 0;

        public short AlarmSetCode
        {
            get
            {
                return alarmSetCode;
            }
            set
            {
                alarmSetCode = value;
                OnPropertyChanged("AlarmSetCode", value);

            }
        }
        private VEHICLE_OPERATION_MODE operationMode = 0;

        public VEHICLE_OPERATION_MODE OperationMode
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
        private VEHICLE_STATE vehicleState = 0;

        public VEHICLE_STATE VehicleState
        {
            get
            {
                return vehicleState;
            }
            set
            {
                vehicleState = value;
                OnPropertyChanged("VehicleState", value);
            }
        }


        private string vehicleLocation01 = "";

        public string VehicleLocation01
        {
            get
            {
                return vehicleLocation01;
            }
            set
            {
                vehicleLocation01 = value;
                OnPropertyChanged("VehicleLocation01", value);

            }
        }
        private string vehicleLocation02 = "";

        public string VehicleLocation02
        {
            get
            {
                return vehicleLocation02;
            }
            set
            {
                vehicleLocation02 = value;
                OnPropertyChanged("VehicleLocation02", value);
            }
        }
        private string vehicleLocation03 = "";

        public string VehicleLocation03
        {
            get
            {
                return vehicleLocation03;
            }
            set
            {
                vehicleLocation03 = value;
                OnPropertyChanged("VehicleLocation03", value);
            }
        }
        private string from01 = "";

        public string From01
        {
            get
            {
                return from01;
            }
            set
            {
                from01 = value;
                OnPropertyChanged("From01", value);

            }
        }
        private string from02 = "";

        public string From02
        {
            get
            {
                return from02;
            }
            set
            {
                from02 = value;

            }
        }
        private string from03 = "";

        public string From03
        {
            get
            {
                return from03;
            }
            set
            {
                from03 = value;

            }
        }
        private string to01 = "";

        public string To01
        {
            get
            {
                return to01;
            }
            set
            {
                to01 = value;
                OnPropertyChanged("To01", value);

            }
        }
        private string to02 = "";

        public string To02
        {
            get
            {
                return to02;
            }
            set
            {
                to02 = value;

            }
        }
        private string to03 = "";

        public string To03
        {
            get
            {
                return to03;
            }
            set
            {
                to03 = value;

            }
        }
        private string palletIdForTransfer = "";

        public string PalletIdForTransfer
        {
            get
            {
                return palletIdForTransfer;
            }
            set
            {
                palletIdForTransfer = value;
                OnPropertyChanged("PalletIdForTransfer", value);

            }
        }
        private string palletIdOnVehicle = "";
        public string PalletIdOnVehicle
        {
            get
            {
                return palletIdOnVehicle;
            }
            set
            {
                palletIdOnVehicle = value;
                OnPropertyChanged("PalletIdOnVehicle", value);

            }
        }
        private string transferCompleteLocation01 = "";

        public string TransferCompleteLocation01
        {
            get
            {
                return transferCompleteLocation01;
            }
            set
            {
                transferCompleteLocation01 = value;
                OnPropertyChanged("TransferCompleteLocation01", value);

            }
        }
        private string transferCompleteLocation02 = "";

        public string TransferCompleteLocation02
        {
            get
            {
                return transferCompleteLocation02;
            }
            set
            {
                transferCompleteLocation02 = value;

            }
        }
        private string transferCompleteLocation03 = "";

        public string TransferCompleteLocation03
        {
            get
            {
                return transferCompleteLocation03;
            }
            set
            {
                transferCompleteLocation03 = value;
            }
        }
        private string reservedCommandId = "";

        public string ReservedCommandId
        {
            get
            {
                return reservedCommandId;
            }
            set
            {
                reservedCommandId = value;
            }
        }
        private string reservedCommandSeq = "";

        public string ReservedCommandSeq
        {
            get
            {
                return reservedCommandSeq;
            }
            set
            {
                reservedCommandSeq = value;
            }
        }


        private string commandid;

        public string Commandid
        {
            get
            {
                return commandid;
            }
            set
            {
                commandid = value;
            }
        }

        private string commandSeq;

        public string CommandSeq
        {
            get
            {
                return commandSeq;
            }
            set
            {
                commandSeq = value;
            }
        }
        #endregion properties
        #region Constructor

        public Vehicle()
        {
        }

        #endregion Constructor

        #region Attribute_Persistance

        /// <summary>
        /// elementID
        /// </summary>


        #endregion Attribute_Persistance

        #region Attribute_Not_Persistance

        #endregion Attribute_Not_Persistance

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

                //psparam.SetVarChar("ID", this.ID);

                //reader = theDB.ExecuteQuery("select * from VEHICLE where ID =@ID", psparam);
                //if (reader.Read())
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "VEHICLE", this.ID + "already Exist");
                //    //throw new ECPException
                //}
                //else
                //{
                //    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();

                //    pss.SetVarChar("ID", this.ID);
                //    pss.SetVarChar("HOSTID", this.hostID);

                //    theDB.ExecuteUpdate("insert into VEHICLE (ID,HOSTID) values" +
                //        " (@ID,@HOSTID)", pss);
                //}
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Add", "Insert SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "VEHICLE", "sql error \n " + ex.Message.ToString());
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

                //2009. 04.21 윤선화 주석처리 장비는 EQUIPMENT,SUBSYSTEM으로 통일
                //PreparedSqlParameter psqlparam = theDB.GetPreparedSqlParameter();

                //psqlparam.SetVarChar("ID", this.elementId);

                //reader = theDB.ExecuteQuery("select * from VEHICLE where ID =@ID", psqlparam);

                //if (reader.Read())
                //{
                //    this.ID = (string)reader["ID"];
                //    this.hostID = reader["HOSTID"].Equals(DBNull.Value) ? "" : (string)reader["HOSTID"];
                //}
                //else
                //{
                //    Log.log(5, METHOD, "debug", Category.DEBUG, "", "VEHICLE", this.ID + "doesnot exist");
                //}

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

                //theDB.ExecuteUpdate("delete from VEHICLE where ID =@ID", ps);
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                                                  RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                                                  RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                                                  new RunTimeExceptionStatisticsCollectorData(CLASS, "Load", "Loading SQL Error", ex.ToString(), ex.StackTrace.ToString()));
                Log.log(5, METHOD, "error", Category.ERROR, "", "VEHICLE", "sql error \n " + ex.Message.ToString());
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
                //    theDB.ExecuteUpdate("update VEHICLE set\n" +
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
                Log.log(5, METHOD, "error", Category.ERROR, "", "Port", "sql error \n " + ex.Message.ToString());
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
            timerClearCommand.Interval = 1000;
            timerClearCommand.Enabled = true;
            timerClearCommand.Stop();
            timerClearCommand.Elapsed += new ElapsedEventHandler(timerClearCommand_Elapsed);
        }

        void timerClearCommand_Elapsed(object sender, ElapsedEventArgs e)
        {
            clearCommandCount++;
            if (clearCommandCount >= 3)
            {
                clearCommandCount = 0;
                timerClearCommand.Stop();
                //if (this.vehicleState == VEHICLE_STATE.IDLE)
                //{
                //    if (string.IsNullOrEmpty(this.commandid) == false || string.IsNullOrEmpty(this.commandSeq) == false)
                //    {
                //        MovementManager manager = Naming.GetMovementManager();
                //        manager.chageTransferDetailComplete(this.commandid, this.commandSeq);
                //        this.commandid = "";
                //        this.commandSeq = "";
                //    }

                //}
            }
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

        #region IHostUnit 멤버

        //public string HostID
        //{
        //    get { return hostId; }
        //    set { hostId = value; }
        // }

        #endregion


        #region Event_Handler
        public void AddSubScribe()
        {
            if (this.observables.Count != 0)
            {
                this.__IO_ALARM_RESET_CODE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_RESET_CODE_ObservableValueChanged);
                this.__IO_ALARM_RESET_REPORT.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_RESET_REPORT_ObservableValueChanged);
                this.__IO_ALARM_SET_CODE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_SET_CODE_ObservableValueChanged);
                this.__IO_ALARM_SET_REPORT.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_SET_REPORT_ObservableValueChanged);
                this.__IO_LOAD_COMPLETE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_LOAD_COMPLETE_ObservableValueChanged);
                this.__IO_OPERATION_MODE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_OPERATION_MODE_ObservableValueChanged);
                this.__IO_PALLET_EXIST_FLAG.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_EXIST_FLAG_ObservableValueChanged);
                this.__IO_SUBSYSTEM_LOCATION_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_SUBSYSTEM_LOCATION_01_ObservableValueChanged);
                this.__IO_SUBSYSTEM_LOCATION_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_SUBSYSTEM_LOCATION_02_ObservableValueChanged);
                this.__IO_SUBSYSTEM_LOCATION_03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_SUBSYSTEM_LOCATION_03_ObservableValueChanged);

                this.__IO_SUBSYSTEM_STATUS.ObservableValueChanged += new DelegateObservableValueChanged(__IO_SUBSYSTEM_STATUS_ObservableValueChanged);
                this.__IO_TRANSFER_ACK.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TRANSFER_ACK_ObservableValueChanged);
                this.__IO_TRANSFER_COMPLETE_LOCATION_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TRANSFER_COMPLETE_LOCATION_01_ObservableValueChanged);
                this.__IO_TRANSFER_COMPLETE_LOCATION_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TRANSFER_COMPLETE_LOCATION_02_ObservableValueChanged);
                this.__IO_TRANSFER_COMPLETE_LOCATION_03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TRANSFER_COMPLETE_LOCATION_03_ObservableValueChanged);
                this.__IO_UNLOAD_COMPLETE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_UNLOAD_COMPLETE_ObservableValueChanged);
                this.__IO_FROM_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_FROM_01_ObservableValueChanged);
                this.__IO_FROM_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_FROM_02_ObservableValueChanged);
                this.__IO_FROM_03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_FROM_03_ObservableValueChanged);
                this.__IO_TO_01.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TO_01_ObservableValueChanged);

                this.__IO_TO_02.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TO_02_ObservableValueChanged);
                this.__IO_TO_03.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TO_03_ObservableValueChanged);
                this.__IO_PALLET_ID.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_ID_ObservableValueChanged);
                this.__IO_PALLET_ON_VEHICLE.ObservableValueChanged += new DelegateObservableValueChanged(__IO_PALLET_ON_VEHICLE_ObservableValueChanged);
                this.__IO_TRANSFER_REQUEST.ObservableValueChanged += new DelegateObservableValueChanged(__IO_TRANSFER_REQUEST_ObservableValueChanged);
                this.__IO_LOAD_COMPLETE_ACK.ObservableValueChanged += new DelegateObservableValueChanged(__IO_LOAD_COMPLETE_ACK_ObservableValueChanged);
                this.__IO_UNLOAD_COMPLETE_ACK.ObservableValueChanged += new DelegateObservableValueChanged(__IO_UNLOAD_COMPLETE_ACK_ObservableValueChanged);
                this.__IO_ALARM_SET_REPORT_ACK.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_SET_REPORT_ACK_ObservableValueChanged);
                this.__IO_ALARM_RESET_REPORT_ACK.ObservableValueChanged += new DelegateObservableValueChanged(__IO_ALARM_RESET_REPORT_ACK_ObservableValueChanged);
            }
            foreach (string key in this.Observables.Keys)
            {
                Observables[key].ObservableLogEvent += new delegateObservableLogEvent(Vehicle_ObservableLogEvent);
            }
        }

        void Vehicle_ObservableLogEvent(object sender, ObservableStatisticsCollectorData collectionData)
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
                    new RunTimeExceptionStatisticsCollectorData(CLASS, "Vehicle_ObservableLogEvent", "", ex.ToString(), ex.StackTrace.ToString()));
            }
        }


        #endregion Event_Handler

        #region Event
        void __IO_ALARM_RESET_REPORT_ACK_ObservableValueChanged(object sender, object value)
        {
            OnAlarmResetReportACK((bool)value);
        }

        void __IO_ALARM_SET_REPORT_ACK_ObservableValueChanged(object sender, object value)
        {
            OnAlarmSetReportACK((bool)value);
        }

        void __IO_UNLOAD_COMPLETE_ACK_ObservableValueChanged(object sender, object value)
        {
            OnUnloadCompleteACK((bool)value);
        }


        void __IO_LOAD_COMPLETE_ACK_ObservableValueChanged(object sender, object value)
        {
            OnLoadCompleteACK((bool)value);
        }

        void __IO_TRANSFER_REQUEST_ObservableValueChanged(object sender, object value)
        {
            OnTransferRequest((bool)value);
        }


        void __IO_PALLET_ON_VEHICLE_ObservableValueChanged(object sender, object value)
        {
            OnPalletOnVehicle((string)value);
        }


        void __IO_PALLET_ID_ObservableValueChanged(object sender, object value)
        {
            OnPalletIDForTransfer((string)value);
        }


        void __IO_TO_01_ObservableValueChanged(object sender, object value)
        {
            OnTo01((string)value);
        }

        void __IO_TO_02_ObservableValueChanged(object sender, object value)
        {
            OnTo02((string)value);
        }

        void __IO_TO_03_ObservableValueChanged(object sender, object value)
        {
            OnTo03((string)value);
        }

        void __IO_FROM_01_ObservableValueChanged(object sender, object value)
        {
            OnFrom01((string)value);
        }

        void __IO_FROM_02_ObservableValueChanged(object sender, object value)
        {
            OnFrom02((string)value);
        }

        void __IO_FROM_03_ObservableValueChanged(object sender, object value)
        {
            OnFrom03((string)value);
        }

        void __IO_ALARM_RESET_CODE_ObservableValueChanged(object sender, object value)
        {
            OnAlarmResetCode((short)value);
        }

        void __IO_ALARM_RESET_REPORT_ObservableValueChanged(object sender, object value)
        {
            OnAlarmResetReport((bool)value);
        }

        void __IO_ALARM_SET_CODE_ObservableValueChanged(object sender, object value)
        {
            OnAlarmSetCode((short)value);
        }

        void __IO_ALARM_SET_REPORT_ObservableValueChanged(object sender, object value)
        {
            OnAlarmSetReport((bool)value);
        }

        void __IO_LOAD_COMPLETE_ObservableValueChanged(object sender, object value)
        {
            OnLoadComplete((bool)value);
        }

        void __IO_OPERATION_MODE_ObservableValueChanged(object sender, object value)
        {
            OnOperationMode((short)value);
        }

        void __IO_PALLET_EXIST_FLAG_ObservableValueChanged(object sender, object value)
        {
            OnPalletExistFlag((bool)value);
        }

        void __IO_SUBSYSTEM_LOCATION_01_ObservableValueChanged(object sender, object value)
        {
            OnLocation01((string)value);
        }

        void __IO_SUBSYSTEM_LOCATION_02_ObservableValueChanged(object sender, object value)
        {
            OnLocation02((string)value);
        }

        void __IO_SUBSYSTEM_LOCATION_03_ObservableValueChanged(object sender, object value)
        {
            OnLocation03((string)value);
        }

        void __IO_SUBSYSTEM_STATUS_ObservableValueChanged(object sender, object value)
        {
            OnVehicleState((short)value);
        }

        void __IO_TRANSFER_ACK_ObservableValueChanged(object sender, object value)
        {
            OnTransferACK((bool)value);
        }

        void __IO_TRANSFER_COMPLETE_LOCATION_01_ObservableValueChanged(object sender, object value)
        {
            OnTransferCompleteLocation01((string)value);
        }

        void __IO_TRANSFER_COMPLETE_LOCATION_02_ObservableValueChanged(object sender, object value)
        {
            OnTransferCompleteLocation02((string)value);
        }

        void __IO_TRANSFER_COMPLETE_LOCATION_03_ObservableValueChanged(object sender, object value)
        {
            OnTransferCompleteLocation03((string)value);
        }

        void __IO_UNLOAD_COMPLETE_ObservableValueChanged(object sender, object value)
        {
            OnUnloadComplete((bool)value);
        }
        #endregion Event

        private void OnLoadComplete(bool value)
        {
            this.IsLoadComplete = value;
            if (value == true)
            {
                this.IO_LOAD_COMPLETE_ACK = true;
            }
        }
        private void OnUnloadComplete(bool value)
        {
            this.IsUnloadComplete = value;
            if (value == true)
            {
                this.IO_UNLOAD_COMPLETE_ACK = true;
                if (string.IsNullOrEmpty(this.commandid) == false || string.IsNullOrEmpty(this.commandSeq) == false)
                {
                    MovementManager manager = Naming.GetMovementManager();
                    manager.chageTransferDetailComplete(this.commandid, this.commandSeq);
                }
                this.commandid = "";
                this.commandSeq = "";
            }
        }
        private void OnAlarmResetReportACK(bool value)
        {
            this.IsAlarmResetReportAck = value;
        }

        private void OnAlarmSetReportACK(bool value)
        {
            this.IsAlarmSetReportAck = value;
        }
        private void OnUnloadCompleteACK(bool value)
        {
            this.isUnloadCompleteAck = value;
        }

        private void OnLoadCompleteACK(bool value)
        {
            this.IsLoadCompleteAck = value;
        }
        private void OnTransferRequest(bool value)
        {
            this.IsTransferRequest = value;
        }
        private void OnPalletOnVehicle(string value)
        {
            if (string.IsNullOrEmpty(value))
            {
                this.PalletIdOnVehicle = "";
            }
            else
            {
                this.PalletIdOnVehicle = this.IO_PALLET_ON_VEHICLE;

            }
        }
        private void OnPalletIDForTransfer(string value)
        {
            this.PalletIdForTransfer = value;
        }
        private void OnTo01(string value)
        {
            this.To01 = this.IO_TO_01;
        }
        private void OnTo02(string value)
        {

            this.To02 = this.IO_TO_02;

        }
        private void OnTo03(string value)
        {

            this.To03 = this.IO_TO_03;
        }
        private void OnFrom01(string value)
        {
            this.From01 = this.IO_FROM_01;

        }
        private void OnFrom02(string value)
        {
            this.From02 = this.IO_FROM_02;
        }
        private void OnFrom03(string value)
        {
            this.From03 = this.IO_FROM_03;
        }
        private void OnAlarmResetCode(short value)
        {
            this.AlarmResetCode = value;
        }
        private void OnAlarmResetReport(bool value)
        {
            this.IsAlarmResetReport = value;
            if (value == true)
            {
                this.IO_ALARM_RESET_REPORT_ACK = true;
            }

            this.AlarmResetCode = this.IO_ALARM_RESET_CODE;
            if (value == true)
            {

                System.Console.WriteLine("Vehicle에서 알람이 해제되었음:" + this.AlarmResetCode.ToString());
            }
        }
        private void OnAlarmSetCode(short value)
        {
            this.AlarmSetCode = value;
            if (value==0)
            {
                Util.Util.AlarmReset();
            }
        }
        private void OnAlarmSetReport(bool value)
        {
            this.IsAlarmSetReport = value;
            if (value == true)
            {
                this.IO_ALARM_SET_REPORT_ACK = true;
            }
            if (value == true)
            {
                this.AlarmSetCode = this.IO_ALARM_SET_CODE;
                //Util.Util.Alarm1();

                ErrorHistData errorhistData = new ErrorHistData();
                errorhistData.Backuptime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                errorhistData.Errortype = "Vehicle";
                errorhistData.Errorcode = AlarmSetCode.ToString();
                errorhistData.Errortext = "Vehicle: " + this.ElementId + "에서 알람이 발생했습니다.";

                try
                {
                    errorhistData.insertErrorHistData();
                }
                catch (Exception e)
                {
                    //No 처리
                }

                //알람 WMS I/F
                try
                {
                    MakeErrorString maError = new MakeErrorString(this.ElementId, this.AlarmSetCode.ToString());

                    string strTotal = maError.createErrorString();

                    if (!string.IsNullOrEmpty(strTotal))
                    {
                        MakeWmsIfString mwIf = new MakeWmsIfString();
                        mwIf.insertError(strTotal);
                    }
                }
                catch (Exception ex)
                {
                }
            }
        }

        private void OnOperationMode(short value)
        {
            if (value == 0)
            {
                this.OperationMode = VEHICLE_OPERATION_MODE.MANUAL;
            }
            else
            {
                this.OperationMode = VEHICLE_OPERATION_MODE.AUTO;
            }
        }
        private void OnPalletExistFlag(bool value)
        {
            this.IsPalletExist = value;
            if (value == true)
            {
                this.PalletIdOnVehicle = this.IO_PALLET_ON_VEHICLE;
                System.Console.WriteLine("Vehicle PalletID:" + this.IO_PALLET_ON_VEHICLE);
            }
        }
        private void OnLocation01(string value)
        {
            this.VehicleLocation01 = this.IO_SUBSYSTEM_LOCATION_01;

        }

        private void OnLocation02(string value)
        {

            this.VehicleLocation02 = this.IO_SUBSYSTEM_LOCATION_02;

        }

        private void OnLocation03(string value)
        {

            this.VehicleLocation03 = this.IO_SUBSYSTEM_LOCATION_03;
        }
        private void OnVehicleState(short value)
        {
            if (value == 0)
            {
                this.VehicleState = VEHICLE_STATE.DOWN;
            }
            else if (value == 1)
            {
                this.VehicleState = VEHICLE_STATE.IDLE;
                //if (string.IsNullOrEmpty(this.commandid) == false || string.IsNullOrEmpty(this.commandSeq) == false)
                //{
                //    this.timerClearCommand.Start();
                //}

            }
            else if (value == 2)
            {
                this.VehicleState = VEHICLE_STATE.RUN;
            }
        }

        private void OnTransferACK(bool value)
        {
            this.IsTransferRequestAck = value;
        }
        private void OnTransferCompleteLocation01(string value)
        {
            this.TransferCompleteLocation01 = this.IO_TRANSFER_COMPLETE_LOCATION_01;

        }
        private void OnTransferCompleteLocation02(string value)
        {

            this.TransferCompleteLocation02 = this.IO_TRANSFER_COMPLETE_LOCATION_02;

        }
        private void OnTransferCompleteLocation03(string value)
        {

            this.TransferCompleteLocation03 = this.IO_TRANSFER_COMPLETE_LOCATION_03;
        }

    }
}
