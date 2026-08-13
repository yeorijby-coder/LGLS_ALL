using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Data;
using System.Threading;
using System.Configuration;
using System.Collections.Specialized;


using System.Data.SqlClient;
using System.Reflection;
using System.Runtime.Remoting;


using ECP.Db;
using ECP.Util;
using ECP.Device.Observe;

using ECP.Interfaces.Emulator;
using ECP.Com.FieldBus;
using ECP.Global.Com;
using ECP.Global;
using ECP.Global.Device;

using ECP.Service.Logger;

namespace ECP.Com.FieldBus
{
    public delegate void delegateDriverConnectionStateChanged(AbstractFieldBusDriver driver, FBConnectionState connectionState);
    public delegate void delegateDriverErrorOccurred(AbstractFieldBusDriver driver, int iErrorCode);

    public class AbstractFieldBusDriver : Persistence
    {
        const string CLASS = "AbstractFieldBusDriver";

        private bool dirtyFlag = true;

        public string className = "ECP.Com.FieldBus.AbstractFieldBusDriver";

        public string assemblyName = "ECP";


        #region Attribute_Persistance

        protected string ownerId;

        public string OwnerId
        {
            get { return ownerId; }
            set { ownerId = value; }
        }

        protected ElementType ownerType;

        public ElementType OwnerType
        {
            get { return ownerType; }
            set { ownerType = value; }
        }

        private int driverNo;
        public int DriverNo
        {
            get { return this.driverNo; }
            set { this.driverNo = value; }
        }

        private FieldBusDriverType fieldBusDriverType;

        public FieldBusDriverType FieldBusDriverType
        {
            get { return fieldBusDriverType; }
            set { fieldBusDriverType = value; }
        }

        private int retryCount = 0;

        private int openRetrySec = -1;
        public int OpenRetrySec
        {
            get { return this.openRetrySec; }
            set { this.openRetrySec = value; }
        }

        private int timeOutSec = -1;
        public int TimeOutSec
        {
            get { return this.timeOutSec; }
            set { this.timeOutSec = value; }
        }

        public string ConnectionInfoString
        {
            get { return this.connectionInfo == null ? string.Empty : this.connectionInfo.ConnectionString; }
            set { InitConnectionString(value); }
        }

        protected DriverConnectionString connectionInfo = null;
        public DriverConnectionString ConnectionInfo
        {
            get { return connectionInfo; }
        }

        private bool useFlag;
        public bool UseFlag
        {
            get { return this.useFlag; }
            set { this.useFlag = value; }
        }

        private string description = string.Empty;
        public string Description
        {
            get { return this.description; }
            set { this.description = value; }
        }

        #endregion Attribute_Persistance

        #region Attribute_Not_Persistance

        private System.Timers.Timer timerOpenRetry = null;

        protected static string remotingURL = string.Empty;
        public static string RemotingURL
        {
            get { return remotingURL; }
        }

        private ISimulation simulation = null;
        public ISimulation Simulation
        {
            get
            {
                if (simulation == null)
                {
                    simulation = (ISimulation)Activator.GetObject(typeof(ISimulation), RemotingURL);
                }
                return simulation;
            }
        }

        protected ObservableCollection registeredObservableList = null;
        public ObservableCollection RegisteredObservable
        {
            get { return this.registeredObservableList; }
        }

        protected FBConnectionState connectionState = FBConnectionState.DISCONNECTED;
        public FBConnectionState ConnectionState
        {
            get { return this.connectionState; }
            set 
            {
                ChangeConnectionState(value);
            }
        }

        virtual protected void ChangeConnectionState(FBConnectionState connectionState)
        {
            const string METHOD = CLASS + ".ChangeConnectionState";

            this.connectionState = connectionState;

            try
            {
                if (ConnectionStateChanged != null)
                {
                    ConnectionStateChanged(this, connectionState);
                }

                switch ((int)connectionState.Value())
                {
                    case FBConnectionState._DISCONNECTED:
                        StopScaning();
                        break;
                    case FBConnectionState._CONNECTED:
                        //if (_iScanFlag != 0) StartScaning();
                        if(!this.enableScan)
                        this.StartScaning();
                        break;
                    case FBConnectionState._DISABLED:
                        break;
                    case FBConnectionState._ENABLED:
                        break;
                    case FBConnectionState._RETRY:
                        break;
                }
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "debug", Log.DEBUG, " ", ex.Message.ToString(),"Exception " + ex.StackTrace.ToString());
            }
        }

        public event delegateDriverConnectionStateChanged ConnectionStateChanged = null;
        public event delegateDriverErrorOccurred ErrorOccurred = null;
        protected void RaiseErrorOccurred(int iErrorCode)
        {
            if (ErrorOccurred != null)
                ErrorOccurred(this, iErrorCode);
        }

        #endregion Attribute_Not_Persistance

        #region Constructor

        public AbstractFieldBusDriver()
        {
            InitAllocation();
        }

        public AbstractFieldBusDriver(int driverNo, string ownerId)
        {
            this.driverNo = driverNo;
            this.ownerId = ownerId;
            InitAllocation();
        }
        #endregion

        #region 초기화및기동관련

        private void InitAllocation()
        {
            this.registeredObservableList = new ObservableCollection();
        }

        #endregion 초기화및기동관련

        #region Manage_Persistent_Attribute

        public void Load(string ownerId, int driverNo)
        {
            this.ownerId = ownerId;
            this.driverNo = driverNo;
            Load();
        }

        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetInt("DRIVERNO", this.driverNo);
                psparam.SetVarChar("OWNERID", this.ownerId);

                reader = theDB.ExecuteQuery("select * from TB_FIELDBUSDRIVER where DRIVERNO =@DRIVERNO and OWNERID =@OWNERID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "fieldbusdriver", Convert.ToString(this.driverNo) + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetInt("DRIVERNO", this.driverNo);
                    pss.SetVarChar("DRIVERTYPE", FieldBusDriverType.ValueToString(this.fieldBusDriverType));

                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString(this.ownerType));

                    pss.SetInt("OPENRETRYSEC", this.openRetrySec);
                    pss.SetInt("TIMEOUTSEC", this.timeOutSec);


                    pss.SetVarChar("CONNECTSTRING", this.ConnectionInfoString);
                    pss.SetInt("SCANINTERVAL", this.scanInterval);

                    pss.SetChar("USEFLAG", (this.useFlag == true) ? "T" : "F");
                    pss.SetVarChar("DESCRIPTION", this.description);

                    pss.SetVarChar("ASSEMBLYNAME", this.assemblyName);
                    pss.SetVarChar("CLASSNAME", this.className);


                    theDB.ExecuteUpdate("insert into TB_FIELDBUSDRIVER (DRIVERNO,DRIVERTYPE,OWNERID,OWNERTYPE,OPENRETRYSEC,TIMEOUTSEC,CONNECTSTRING,SCANINTERVAL,USEFLAG,DESCRIPTION,ASSEMBLYNAME,CLASSNAME) values (@DRIVERNO,@DRIVERTYPE,@OWNERID,@OWNERTYPE,@OPENRETRYSEC,@TIMEOUTSEC,@CONNECTSTRING,@SCANINTERVAL,@USEFLAG,@DESCRIPTION,@ASSEMBLYNAME,@CLASSNAME)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fbdriver", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                if(reader != null) reader.Close();
            }
        }

        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".delete";

            try
            {
                base.Delete(theDB);
                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetInt("DRIVERNO", this.driverNo);
                ps.SetVarChar("OWNERID", this.ownerId);


                theDB.ExecuteUpdate("delete from TB_FIELDBUSDRIVER where DRIVERNO =@DRIVERNO and OWNERID =@OWNERID", ps);

            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "sql error \n " + ex.Message.ToString());
                throw ex;
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

                psqlparam.SetInt("DRIVERNO", this.driverNo);
                psqlparam.SetVarChar("OWNERID", this.ownerId);

                reader = theDB.ExecuteQuery("select * from TB_FIELDBUSDRIVER where DRIVERNO =@DRIVERNO and OWNERID =@OWNERID", psqlparam);


                if (reader.Read())
                {
                    this.fieldBusDriverType = FieldBusDriverType.StringToValue((string)reader["DRIVERTYPE"]);
                    this.ownerType = ElementType.StringToValue((string)reader["OWNERTYPE"]);

                    this.openRetrySec = int.Parse((string)reader["OPENRETRYSEC"]);
                    this.timeOutSec = int.Parse((string)reader["TIMEOUTSEC"]);

                    this.ConnectionInfoString = (string)reader["CONNECTSTRING"];
                    this.scanInterval = int.Parse((string)reader["SCANINTERVAL"]);

                    this.useFlag = ((string)reader["USEFLAG"]).Equals("T");
                    this.description = (string)reader["DESCRIPTION"];

                    this.assemblyName = (string)reader["ASSEMBLYNAME"];
                    this.className = (string)reader["CLASSNAME"];

                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "fieldbusdriver", Convert.ToString(this.driverNo) + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "sql error \n " + ex.Message.ToString());
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
                    theDB.ExecuteUpdate("update TB_FIELDBUSDRIVER set\n" +
                        "DRIVERTYPE='" + FieldBusDriverType.ValueToString(this.fieldBusDriverType) + "',\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "',\n" +
                        "OPENRETRYSEC='" + this.openRetrySec + "',\n" +
                        "TIMEOUTSEC='" + this.timeOutSec + "',\n" +
                        "CONNECTSTRING='" + this.ConnectionInfoString + "',\n" +
                        "SCANINTERVAL='" + this.scanInterval + "',\n" +
                        "USEFLAG='" + ((this.useFlag == true) ? "T" : "F") + "',\n" +
                        "DESCRIPTION='" + this.description + "',\n" +
                        "OWNERID='" + this.ownerId + "',\n" +
                        "ASSEMBLYNAME='" + this.assemblyName + "',\n" +
                        "CLASSNAME='" + this.className + "'\n" +
                        "where DRIVERNO = '" + this.driverNo + "' and OWNERID = '" + this.ownerId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "sql error \n " + ex.Message.ToString());
            }
        }

        #endregion Manage_Persistent_Attribute

        #region DriverHandle_Method

        public bool Enabled
        {
            get { return !(this.connectionState == FBConnectionState.DISABLED); }
        }

        public bool Connected
        {
            get { return (this.connectionState == FBConnectionState.CONNECTED); }
        }

        public static Dictionary<int, AbstractFieldBusDriver> LoadAbstractFieldBusDrivers(string ownerId)
        {
            const string METHOD = CLASS + ".LoadAbstractFieldBusDrivers";

            Dictionary<int, AbstractFieldBusDriver> drivers = new Dictionary<int, AbstractFieldBusDriver>();

            int driverNo = -1;
            ObjectHandle objHandle = null;
            AbstractFieldBusDriver driver = null;

            DBConnection conn = DBControl.GetConnection();
            SqlDataReader reader;
            string sqlstring = "select DRIVERNO, ASSEMBLYNAME, CLASSNAME,USEFLAG from TB_FIELDBUSDRIVER where OWNERID ='" + ownerId + "' AND USEFLAG = 'T'";
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to load fieldbus dirvers Sql Error : " + ownerId +
                    "\n" + ex.Message.ToString());
                throw ex;
            }

            while (reader.Read())
            {

                try
                {
                    //if (!((string)reader["USEFLAG"]).Equals("T")) continue;
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    driver = (AbstractFieldBusDriver)objHandle.Unwrap();
                    driverNo = int.Parse((string)reader["DRIVERNO"]);
                    driver.Load(ownerId, driverNo);
                    drivers.Add(driverNo, driver);

                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to load fieldbus dirvers : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }

            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return drivers;
        }

        public static AbstractFieldBusDriver LoadAbstractFieldBusDriver(string ownerId, int driverNo)
        {
            const string METHOD = CLASS + ".LoadAbstractFieldBusDriver";

            ObjectHandle objHandle = null;
            AbstractFieldBusDriver driver = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select ASSEMBLYNAME, CLASSNAME from TB_FIELDBUSDRIVER where OWNERID ='" + ownerId +
                "' and DRIVERNO ='" + driverNo + "' AND USEFLAG = 'T'";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    driver = (AbstractFieldBusDriver)objHandle.Unwrap();
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to load driver No : " + Convert.ToString(driverNo) +
                        "\n" + ex.Message.ToString());
                }
                driver.Load(ownerId, driverNo);
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return driver;

        }

        public static AbstractFieldBusDriver CreateAbstractFieldBusDriver(
            string ownerId, ElementType ownerType, int driverNo, FieldBusDriverType driverType,
            int openRetrySec, int timeOutSec, string connectionInfoString,
            int scanInterval, bool useFlag, string description,
            string assemblyName, string className)
        {
            const string METHOD = CLASS + ".CreateAbstractFieldBusDriver";
            AbstractFieldBusDriver driver = null;
            ObjectHandle objHandle = null;
            try
            {
                objHandle = Activator.CreateInstance(assemblyName, className);
                driver = (AbstractFieldBusDriver)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to create field bus driver : " +
                    Convert.ToString(driverNo) + "\n" + ex.Message.ToString());
            }

            driver.driverNo = driverNo;
            driver.fieldBusDriverType = driverType;
            driver.ownerId = ownerId;
            driver.ownerType = ownerType;
            driver.openRetrySec = openRetrySec;
            driver.timeOutSec = timeOutSec;
            driver.ConnectionInfoString = connectionInfoString;
            driver.scanInterval = scanInterval;
            driver.useFlag = useFlag;
            driver.description = description;
            driver.assemblyName = assemblyName;
            driver.className = className;

            driver.Add();

            return driver;
        }

        public static Dictionary<int, AbstractFieldBusDriver> LoadAbstractFieldBusDrivers()
        {
            const string METHOD = CLASS + ".LoadAbstractFieldBusDrivers";

            Dictionary<int, AbstractFieldBusDriver> drivers = new Dictionary<int, AbstractFieldBusDriver>();

            int driverNo = -1;
            string ownerId = null;
            ObjectHandle objHandle = null;
            AbstractFieldBusDriver driver = null;

            DBConnection conn = DBControl.GetConnection();
            SqlDataReader reader;
            string sqlstring = "select * from TB_FIELDBUSDRIVER WHERE USEFLAG = 'T'";
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to load fieldbus dirvers Sql Error : " + ownerId +
                    "\n" + ex.Message.ToString());
                throw ex;
            }

            while (reader.Read())
            {

                try
                {
                    //if (!((string)reader["USEFLAG"]).Equals("T")) continue;
                    objHandle = Activator.CreateInstance((string)reader["ASSEMBLYNAME"], (string)reader["CLASSNAME"]);
                    driver = (AbstractFieldBusDriver)objHandle.Unwrap();
                    driverNo = int.Parse((string)reader["DRIVERNO"]);
                    ownerId = (string)reader["OWNERID"];
                    driver.Load(ownerId, driverNo);
                    drivers.Add(driverNo, driver);

                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver", "failed to load fieldbus dirvers : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }

            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);
            return drivers;
        }

        virtual protected void InitConnectionString(string cnnString)
        {
            this.connectionInfo = new DriverConnectionString();
            this.connectionInfo.Parse(cnnString);

            string sRemoteIP;
            string sPort;

            if (cnnString == string.Empty || cnnString == null ||
                !cnnString.Contains("IP_SIM") || !cnnString.Contains("PORT_SIM"))
            {
                NameValueCollection appSettings = ConfigurationManager.AppSettings;
                sRemoteIP = appSettings[ECPProperties.EmulationHost].ToString();
                sPort = appSettings[ECPProperties.EmulationPort].ToString();
            }
            else
            {
                sRemoteIP = this.connectionInfo["IP_SIM", "localhost"];
                sPort = this.connectionInfo["PORT_SIM", "5213"];
            }

            remotingURL = string.Format("tcp://{0}:{1}/ObservableSharedMemory", sRemoteIP, sPort);
        }



        //public virtual void InitDriver()
        //{            
        //}
        ////public void FBConnectionStateChange(FBConnectionState fbState)
        ////{
        ////    this.connectionState = fbState;
        ////}

        ////public void FBConnectionStateChanged()
        ////{
        ////    switch (this.connectionState.Value())
        ////    {
        ////        case FBConnectionState._DISABLED:
        ////            break;
        ////        case FBConnectionState._ENABLED:
        ////            break;
        ////        case FBConnectionState._DISCONNECTED:
        ////            StopScaning();
        ////            break;
        ////        case FBConnectionState._CONNECTED:
        ////            //StartScaning(); Song Scan 조정
        ////            break;
        ////    }
        ////}

        private string GetObservableKey(Observable observable)
        {
            return null;// return observable.ObservableId + observable.No.ToString();
        }

        public void RegisterObservable(Observable observable)
        {
            if (observable == null) return;

            if (observable.Driver != null)
            {
                observable.Driver.UnregisterObservable(observable);
            }

            observable.Driver = this;



            lock (this.registeredObservableList)
            {
                if (!this.registeredObservableList.ContainsKey(observable.ObservableGuid))
                {
                    this.registeredObservableList.Add(observable.ObservableGuid, observable);
                }
            }
        }

        public void UnregisterObservable(Observable observable)
        {
            if (observable == null) return;

            observable.Driver = null;


            lock (this.registeredObservableList)
            {
                this.registeredObservableList.Remove(observable.ObservableGuid);

                if (this.registeredObservableList.Count == 0)
                {

                }
            }
        }

        virtual public int BeginWrite(string sObservalbeCategory)
        {
            return 0;
        }


        virtual public int EndWrite(string sObservalbeCategory)
        {
            return 0;
        }

        virtual public int Open()
        {
            if (this.connectionInfo == null) this.InitConnectionString("Simulation");

            this.ConnectionState = FBConnectionState.CONNECTED;
            return 0;
        }

        virtual public int Open(string sCnnString)
        {
            InitConnectionString(sCnnString);
            this.ConnectionState = FBConnectionState.CONNECTED;
            return 0;
        }

        virtual public int Close()
        {
            this.ConnectionState = FBConnectionState.DISCONNECTED;
            return 0;
        }

        public void Enable()
        {
            Open();
        }

        public void Disable()
        {
            if (this.Connected)
            {
                Close();
            }

            this.ConnectionState = FBConnectionState.DISABLED;
        }

        public virtual int Read(Observable observable, out object value)
        {
            const string METHOD = CLASS + ".Read";

            int iRtn = 0;

            value = null;

            try
            {
                if (!this.Connected)
                {
                    return 1;
                }

                //ISimulation sim = (ISimulation)Activator.GetObject(typeof(ISimulation), RemotingURL);
                //iRtn = sim.GetValue(ObjectAttribute.UniqueID, out value);
                iRtn = Simulation.GetValue(observable.ObservableGuid.ToString(), out value);
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                //
                // 시뮬레이터를 안 띄우고 실행했을 경우 소켓이 끊기지 않게 수정함
                //
                if (this.fieldBusDriverType == FieldBusDriverType.SOCKET)
                {
                    ;
                }
                else
                {
                    //Close();
                    return 1;
                }
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver",
                                "Failed to read observable value \n" + ex.Message.ToString());
            }
            catch (Exception ex)
            {
                iRtn = 1003;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver",
                                "Failed to read observable value \n" + ex.Message.ToString());
            }

            return iRtn;
        }

        public virtual int Write(Observable observalbe, object value)
        {
            const string METHOD = CLASS + ".Write";

            int iRtn = 0;

            try
            {
                if (!this.Connected)
                {
                    return 1;
                }

                ////ISimulation sim = (ISimulation)Activator.GetObject(typeof(ISimulation), RemotingURL);
                ////iRtn = sim.SetValue(oObjectAttribute.UniqueID, value);
                iRtn = Simulation.SetValue(observalbe.ObservableGuid.ToString(), value);
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                //
                // 시뮬레이터를 안 띄우고 실행했을 경우 소켓이 끊기지 않게 수정함
                //
                if (this.fieldBusDriverType == FieldBusDriverType.SOCKET)
                {
                    ;
                }
                else
                {
                    //Close();
                    return 1;
                }
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver",
                                "Failed to write value to observable\n" + ex.Message.ToString());
            }
            catch (Exception ex)
            {
                iRtn = 1004;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbusdriver",
                                "Failed to write value to observable\n" + ex.Message.ToString());
            }

            return iRtn;
        }

        protected int ActiveOpen()
        {
            int iRtn = 0;

            //iRtn = Open();

            //if (iRtn != 0 && this.openRetrySec > 0)
            if (this.openRetrySec > 0)
            {
                if (this.timerOpenRetry == null)
                {
                    this.timerOpenRetry = new System.Timers.Timer(this.openRetrySec);
                    this.timerOpenRetry.Elapsed += new System.Timers.ElapsedEventHandler(OnOpenRetryElapsed);
                }

                this.retryCount = 0;
                this.timerOpenRetry.Start();
            }

            return iRtn;
        }

        /// <summary>
        /// Driver를 Retry 할 수 있도록 Timer를 동작 시키고 Retry Count를 시작함
        /// </summary>
        /// <param name="retryCountReset">True : Count Rest </param>
        /// <returns></returns>
        protected int ActiveOpen(bool retryCountReset)
        {
            int iRtn = 0;

            //iRtn = Open();

            //if (iRtn != 0 && this.openRetrySec > 0)
            if (this.openRetrySec > 0)
            {
                if (this.timerOpenRetry == null)
                {
                    this.timerOpenRetry = new System.Timers.Timer(this.openRetrySec*1000);
                    this.timerOpenRetry.Elapsed += new System.Timers.ElapsedEventHandler(OnOpenRetryElapsed);
                }

                if(retryCountReset)
                this.retryCount = 0;
                this.timerOpenRetry.Start();
            }

            return iRtn;
        }

        protected int PassiveOpen()
        {
            return 0;
        }

        /// <summary>
        /// 모니터링 영역을 Scan 함.
        /// ex) PLC physical 영역을 내부 Memory로 Scan
        /// </summary>
        virtual public void RefreshMonitorMemoryByObservable()
        {
            // Nothing
        }


        #endregion DriverHandle_Method

        #region Event Method

        private void OnOpenRetryElapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            System.Timers.Timer tmr = (System.Timers.Timer)sender;

            tmr.Stop();

            int iRtn = Open();

            if (iRtn != 0)
            {
                if (timeOutSec < this.openRetrySec * retryCount && timeOutSec != 0 )
                {
                    RaiseErrorOccurred(999);
                }
                else
                {
                    tmr.Start();
                    retryCount++;
                }
            }
        }

        #endregion

        #region Oservable Scanning Method

        private int scanInterval = 100;
        public int ScanInterval
        {
            get { return scanInterval; }
            set { scanInterval = value; }
        }


        private bool enableScan = false;
        protected Thread scanThread = null;

        protected virtual void DoScan()
        {
            while (this.enableScan)
            {
                ScanObservable();
                Thread.Sleep(this.scanInterval);
            }
        }

        public void StartFirstScan()
        {
            ScanObservable();
        }

        public int StartScaning() // SONG private Pulbic으로 변경
        {
            if (scanThread == null)
            {
            scanThread = new Thread(new ThreadStart(DoScan));
            scanThread.Priority = ThreadPriority.Highest;
            scanThread.IsBackground = true;
                scanThread.Start();
            }
                this.enableScan = true;

            return 0;
        }

        public int StopScaning()
        {
            if (scanThread != null)
            {
                this.enableScan = false;
                //scanThread.Join();
                scanThread = null;
            }
            
            return 0;
        }

        virtual public void ScanObservable()
        {
            if (this.registeredObservableList == null || this.registeredObservableList.Count == 0)
            {
                return;
            }

           // Log.log(5, "진입 ScanObservable"+ "드라이버 넘버" + this.driverNo.ToString(), "드라이버 넘버" + this.driverNo.ToString(), Category.INFO, "", "", "");
           // Console.WriteLine("진입 ScanObservable"+ "드라이버 넘버" + this.driverNo.ToString());
            this.RefreshMonitorMemoryByObservable();
           // Log.log(5, "해지 ScanObservable"+ "드라이버 넘버" + this.driverNo.ToString(), "드라이버 넘버" + this.driverNo.ToString(), Category.INFO, "", "", "");
            //Console.WriteLine("해지 ScanObservable"+ "드라이버 넘버" + this.driverNo.ToString());
            lock (this.registeredObservableList)
            {
                foreach (Observable oservable in this.registeredObservableList.Values)
                {
                    //oservable.IsMonitoring = true;
                    //short sCnnInfoStationNo = Convert.ToInt16(oservable.ConnectionInfo["STATION_NO", -1]);
                    //Console.WriteLine("["+DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff")+"]["+Thread.CurrentThread.ManagedThreadId.ToString("0000")+"] S="+ oservable.ObservablePath);
                    if (oservable.IsMonitoring && oservable.ReadValueSync(false) > 0)
                    {
                        // Leave log 
                    }

                    Thread.Sleep(0);
                }
            }
        }

        #endregion       
        virtual public GenericConnectionString GetObservalbeConnectionInfo(string strConnection)
        {
            return new GenericConnectionString(strConnection);
        }
    }    
}
