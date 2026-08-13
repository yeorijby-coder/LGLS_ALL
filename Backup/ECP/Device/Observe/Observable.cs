using System;
using System.Collections.Generic;
using System.Text;

using System.Reflection;
using System.Runtime.Remoting;

using System.Data;
using System.Data.SqlClient;

using ECP.Global;
using ECP.Db;
using ECP.Device;
using ECP.Interfaces;

using ECP.Global.Device;
using ECP.Com.FieldBus;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Service.Statistics;
//using ECP.Global.Com;


namespace ECP.Device.Observe
{
    /// <summary>
    /// Subsystem으로 Log data를 
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="collectionData"></param>
    public delegate void delegateObservableLogEvent(object sender, ObservableStatisticsCollectorData collectionData);

    public class Observable : Persistence, IValue
    {
        const string CLASS = "Observable";

        private bool dirtyFlag = true;

        #region Attribute_Persistance
        private string observableId = null;

        public string ObservableId
        {
            get
            {
                return observableId;
            }
            set
            {
                observableId = value;
            }
        }



        private Guid observableGuid = Guid.Empty;
        public Guid ObservableGuid
        {
            get
            {
                return observableGuid;
            }
            set
            {
                observableGuid = value;
            }
        }


        private string ownerId = null;

        public string OwnerId
        {
            get
            {
                return ownerId;
            }
            set
            {
                ownerId = value;
            }
        }

        private string observablePath = null;

        public string ObservablePath
        {
            get
            {
                return this.observablePath;
            }
            set
            {
                this.observablePath = value;
            }
        }

        private AccessType accessType = AccessType.IN_OUT;

        public AccessType AccessType
        {
            get
            {
                return this.accessType;
            }
            set
            {
                this.accessType = value;
            }
        }

        private DataType dataType = DataType.INTEGER;

        public DataType DataType
        {
            get
            {
                return this.dataType;
            }
            set
            {
                this.dataType = value;
            }
        }


        private double max = 0;

        public double Max
        {
            get
            {
                return this.max;
            }
            set
            {
                this.max = value;
            }
        }

        private double min = 0;

        public double Min
        {
            get
            {
                return this.min;
            }
            set
            {
                this.min = value;
            }
        }

        private ElementType ownerType;

        public ElementType OwnerType
        {
            get
            {
                return ownerType;
            }
            set
            {
                ownerType = value;
            }
        }


        public int driverNo = -1;

        public int DriverNo
        {
            get
            {
                return (this.Driver == null) ? 1 : this.driverNo;
            }
            set
            {
                this.driverNo = value;
            }

        }

        private bool isMonitoring = false;
        public bool IsMonitoring
        {
            get
            {
                return this.isMonitoring;
            }
            set
            {
                this.isMonitoring = value;
            }
        }

        public string ConnectionInfoString
        {
            get
            {
                return this.connectionInfo.ConnectionString;
            }
            set
            {
                this.connectionInfo.Parse(value);
                //Save();
            }
        }

        private string description = string.Empty;

        public string Description
        {
            get
            {
                return this.description;
            }
            set
            {
                this.description = value;
            }
        }

        /// <summary>
        /// Observable Logging 여부 True : log 남김 , False 남기지 않음
        /// </summary>
        private bool logEnable;
        public bool LogEnable
        {
            get
            {
                return logEnable;
            }
            set
            {
                logEnable = value;
            }
        }

        /// <summary>
        /// Observable Value의 의미를 전달 할 수 있는 Data 보관
        /// </summary>
        private string valueInformation;
        public string ValueInformation
        {
            get
            {
                return valueInformation;
            }
            set
            {
                valueInformation = value;
            }
        }

        /// <summary>
        /// Observale 설명
        /// </summary>
        private string remark;
        public string Remark
        {
            get
            {
                return remark;
            }
            set
            {
                remark = value;
            }
        }



        #endregion Attribute_Persistance

        #region Attribute_Not_Persistance

        private AbstractFieldBusDriver driver = null;

        public AbstractFieldBusDriver Driver
        {
            get
            {
                return this.driver;
            }
            set
            {
                this.driver = value;
            }
        }


        public event DelegateObservableValueChanged ObservableValueChanged = null;

        public event DelegateObservableEvent ObservableOn = null;

        public event DelegateObservableEvent ObservableOff = null;

        private GenericConnectionString connectionInfo = new GenericConnectionString();

        public GenericConnectionString ConnectionInfo
        {
            get
            {
                return this.connectionInfo;
            }
        }

        private string defaultValue = string.Empty;

        public string DefaultValue
        {
            get
            {
                return this.defaultValue;
            }
            set
            {
                this.defaultValue = value;
            }
        }

        private int driverNoPreDef = -1;

        public int DriverNoPreDef
        {
            get
            {
                return this.driverNoPreDef;
            }
            set
            {
                this.driverNoPreDef = value;
            }
        }

        private int categoryNoPreDef = -1;

        public int CategoryNoPreDef
        {
            get
            {
                return this.categoryNoPreDef;
            }
        }

        private object requestedValue = null;

        public object RequestedValue
        {
            get
            {
                return this.requestedValue;
            }
        }

        private object previousValue = null;

        public object PreviousValue
        {
            get
            {
                return this.previousValue;
            }
            set
            {
                this.previousValue = value;
            }
        }

        private object tmppreviousValue = null;
        public object TmppreviousValue
        {
            get
            {
                return this.tmppreviousValue;
            }
            set
            {
                this.tmppreviousValue = value;
            }
        }



        private object currentValue = null;

        public object Value
        {
            get
            {
                if (this.dataType == DataType.BOOL_ARRAY)
                {
                    List<bool> sValue = new List<bool>();
                    CloneList<bool>(((List<bool>)this.currentValue), sValue);
                    return sValue;
                }
                else
                {
                    //return this.GetValueFromDriver();
                    return this.currentValue; //this.GetValueFromDriver() 사용시 시뮬레이터 리소스 상승됨
                }
                // this.currentValue;
            }
            set
            {
                this.SetObservableValue(value);
            }
        }


        private object monitor = new object();
        private int listSize;
        public int ListSize
        {
            get
            {
                return listSize;
            }
            set
            {
                listSize = value;
            }
        }
        /// <summary>
        /// Observable Log Event
        /// </summary>
        public event delegateObservableLogEvent ObservableLogEvent = null;

        #endregion Attribute_Not_Persistance

        #region Constructor
        public Observable()
        {

        }

        public Observable(string ownerId, string observableId)
        {
            this.ownerId = ownerId;
            this.observableId = observableId;
            this.currentValue = this.GetInitValue();
            this.previousValue = null;
            this.requestedValue = null;

        }

        public Observable(string observableId, string ownerId, ElementType ownerType, string ownerPath,
            AccessType accessType, DataType dataType, int min, int max, int driverNo,
            bool isMonitoring, string cnnString, string description, bool logEnable, string valueInformation, string remark)
        {

            this.observableId = observableId;
            this.observableGuid = Guid.NewGuid();

            this.ownerId = ownerId;
            this.ownerType = ownerType;

            this.observablePath = ownerPath;

            this.accessType = accessType;
            this.dataType = dataType;
            this.min = min;
            this.max = max;
            this.DriverNo = driverNo;
            this.IsMonitoring = isMonitoring;
            this.ConnectionInfoString = cnnString;
            this.description = description;
            this.logEnable = logEnable;
            this.valueInformation = valueInformation;
            this.remark = remark;
        }

        #endregion Constructor

        #region DriverReadWriting_Method
        public int WriteValueSync(bool bForceSync)
        {
            const string METHOD = CLASS + ".WriteValueSync";

            switch (this.dataType.Value())
            {
                case DataType._SHORT_ARRAY:
                    if (!bForceSync && CompareList<short>((List<short>)this.RequestedValue, (List<short>)this.currentValue) == 1)
                    {
                        return 0;
                    }
                    break;
                case DataType._BOOL_ARRAY:
                    if (!bForceSync && CompareList<bool>((List<bool>)this.RequestedValue, (List<bool>)this.currentValue) == 1)
                    {
                        return 0;
                    }
                    break;
                default:
                    if (!bForceSync && this.RequestedValue.Equals(this.currentValue))
                    {
                        return 0;
                    }
                    break;
            }


            int iRtn = 0;

            try
            {
                lock (monitor)
                {
                    if (this.Driver != null)
                    {
                        if (this.Driver.Connected)
                        {
                            iRtn = this.Driver.Write(this, this.requestedValue);
                        }
                        else
                        {
                            iRtn = 1;
                        }
                    }



                    if (this.driver == null || iRtn == 0 && this.accessType == AccessType.OUT)
                    {

                        switch (this.dataType.Value())
                        {
                            case DataType._SHORT_ARRAY:
                                this.previousValue = this.currentValue;
                                CopyList<short>((List<short>)this.requestedValue, (List<short>)this.currentValue);
                                break;
                            case DataType._BOOL_ARRAY:
                                this.previousValue = this.currentValue;
                                CopyList<bool>((List<bool>)this.requestedValue, (List<bool>)this.currentValue);
                                break;
                            default:
                                this.previousValue = this.currentValue;
                                this.currentValue = this.requestedValue;
                                break;
                        }

                        if (bForceSync)
                            this.WriteObservbleLog(true);

                        BroadcastObservableValueChangedEvent();
                        BroadcastValueChangedEvent();
                        BroadcastBooleanEvent();
                    }
                    else if (this.accessType == AccessType.IN_OUT)
                    {
                        switch (this.dataType.Value())
                        {
                            case DataType._SHORT_ARRAY:
                                this.previousValue = this.currentValue;
                                CopyList<short>((List<short>)this.requestedValue, (List<short>)this.currentValue);
                                break;
                            case DataType._BOOL_ARRAY:
                                this.previousValue = this.currentValue;
                                CopyList<bool>((List<bool>)this.requestedValue, (List<bool>)this.currentValue);
                                break;
                            default:
                                this.previousValue = this.currentValue;
                                this.currentValue = this.requestedValue;
                                break;
                        }

                        if (bForceSync)
                            this.WriteObservbleLog(true);
                    }
                    //========================================



                }
            }
            catch (Exception ex)
            {
                iRtn = 1004;
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", ex.Message.ToString());
            }

            return iRtn;
        }


        public int ReadValueSync(bool logWrite)
        {
            const string METHOD = CLASS + ".ReadValueSync";

            int iRtn = 0;

            object newValue = null;
            bool isChangeValue = false;
            try
            {
                lock (monitor)
                {
                    if (this.Driver != null)
                    {
                        if (this.Driver.Connected)
                        {
                            iRtn = this.Driver.Read(this, out newValue);
                        }
                        else
                        {
                            iRtn = 1;
                        }
                    }
                    else
                    {
                        if (isArrayType(this.dataType))
                        {
                            //Arry Type Value 초기값 설정
                            SetInitialValueForArryType(out newValue);
                        }
                        else
                        {
                            newValue = this.currentValue;
                        }
                    }


                    if (iRtn == 0)
                    {
                        // bool isChangeValue = false;
                        newValue = ConvertValue(newValue);

                        switch (this.dataType.Value())
                        {
                            case DataType._SHORT_ARRAY:
                                {
                                    if (this.currentValue != null && CompareList<short>((List<short>)this.currentValue, (List<short>)newValue) == 0)
                                    {
                                        isChangeValue = true;
                                        this.previousValue = this.currentValue;
                                        CopyList<short>((List<short>)newValue, (List<short>)currentValue);
                                    }
                                }
                                break;
                            case DataType._BOOL_ARRAY:
                                {
                                    if (this.currentValue != null && CompareList<bool>((List<bool>)this.currentValue, (List<bool>)newValue) == 0)
                                    {
                                        isChangeValue = true;
                                        this.previousValue = this.currentValue;
                                        CopyList<bool>((List<bool>)newValue, (List<bool>)currentValue);
                                    }
                                }
                                break;

                            default:
                                {
                                    if (this.currentValue != null && !this.currentValue.Equals(newValue))
                                    {
                                        isChangeValue = true;
                                        this.previousValue = this.currentValue;
                                        this.currentValue = newValue;
                                    }
                                }
                                break;
                        }

                        // Log Write
                        //if (isChangeValue || logWrite)
                        //{                        
                        if (isChangeValue )
                        {
                            this.WriteObservbleLog(false);
                        }

                        if (isChangeValue)
                        {
                            BroadcastObservableValueChangedEvent();
                            BroadcastValueChangedEvent();
                            BroadcastBooleanEvent();
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                iRtn = 1003;
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", ex.Message.ToString());
            }

            return iRtn;
        }


        public bool AsBoolean
        {
            get
            {
                return (bool)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public byte AsByte
        {
            get
            {
                return (byte)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public short AsShort
        {
            get
            {
                return (short)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public int AsInteger
        {
            get
            {
                return (int)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public long AsLong
        {
            get
            {
                return (long)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public double AsDouble
        {
            get
            {
                return (double)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public string AsString
        {
            get
            {
                return (string)GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public Object AsObject
        {
            get
            {
                return GetValueFromDriver();
            }
            set
            {
                SetValueToDriver(value);
            }
        }

        public List<short> AsShortArray
        {
            get
            {
                List<short> sValue = new List<short>();
                CloneList<short>(((List<short>)GetValueFromDriver()), sValue);
                return sValue;
            }
            set
            {
                SetValueToDriver(value);
            }
        }
        public List<bool> AsBoolArray
        {
            get
            {
                List<bool> sValue = new List<bool>();
                CloneList<bool>(((List<bool>)GetValueFromDriver()), sValue);
                return sValue;
            }
            set
            {
                SetValueToDriver(value);
            }
        }


        public override string ToString()
        {
            string str = String.Empty;

            switch (dataType.Value())
            {
                case DataType._SHORT_ARRAY:
                    for (int i = 0; i < listSize; i++)
                    {
                        str += string.Format("[{0}]:{1} ", i, ((List<short>)this.currentValue)[i]);
                    }
                    break;
                case DataType._BOOL_ARRAY:
                    for (int i = 0; i < listSize; i++)
                    {
                        str += string.Format("[{0}]:{1} ", i, ((List<bool>)this.currentValue)[i]);
                    }
                    break;

                case DataType._STRING:
                    str = (string)currentValue;
                    break;
                default:
                    str = currentValue.ToString();
                    break;

            }
            return str;
        }

        public void InitCurrentValueAsDefault()
        {
            SetValueToDriver(this.defaultValue);
        }

        public object GetInitValue()
        {
            object oRtn = null;
            switch (this.DataType.Value())
            {
                case DataType._BOOLEAN:
                    oRtn = false;
                    break;
                case DataType._BYTE:
                    oRtn = (byte)0;
                    break;
                case DataType._DOUBLE:
                    oRtn = (double)0;
                    break;
                case DataType._INTEGER:
                    oRtn = (int)0;
                    break;
                case DataType._LONG:
                    oRtn = (long)0;
                    break;
                case DataType._SHORT:
                    oRtn = (short)0;
                    break;
                case DataType._STRING:
                    oRtn = string.Empty;
                    break;
                case DataType._SHORT_ARRAY:
                    {
                        oRtn = new List<short>();
                        for (int i = 0; i < this.listSize; i++)
                        {
                            ((List<short>)oRtn).Add(0);
                        }

                    }
                    break;
                case DataType._BOOL_ARRAY:
                    {
                        oRtn = new List<bool>();
                        for (int i = 0; i < this.listSize; i++)
                        {
                            ((List<bool>)oRtn).Add(false);
                        }

                    }
                    break;
            }
            return oRtn;
        }


        private object ConvertValue(object value)
        {
            const string METHOD = CLASS + ".ConvertValue";

            object oRtn = null;
            try
            {
                switch (this.DataType.Value())
                {
                    case DataType._BOOLEAN:
                        oRtn = Convert.ToBoolean(value);
                        break;
                    case DataType._BYTE:
                        oRtn = Convert.ToByte(value);
                        break;
                    case DataType._DOUBLE:
                        oRtn = Convert.ToDouble(value);
                        break;
                    case DataType._INTEGER:
                        oRtn = Convert.ToInt32(value);
                        break;
                    case DataType._LONG:
                        oRtn = Convert.ToInt64(value);
                        break;
                    case DataType._SHORT:
                        oRtn = Convert.ToInt16(value);
                        break;
                    case DataType._STRING:
                        oRtn = Convert.ToString(value);
                        break;
                    case DataType._SHORT_ARRAY:
                        oRtn = (List<short>)value;
                        break;
                    case DataType._BOOL_ARRAY:
                        oRtn = (List<bool>)value;
                        break;
                }
            }
            catch (Exception ex)
            {
                oRtn = GetInitValue();
                Log.log(5, METHOD, "error", Category.ERROR, "", "observer",
                                "Failed to convert value to appropriate type \n" + ex.Message.ToString());
            }

            return oRtn;
        }

        private object GetValueFromDriver()
        {
            ReadValueSync(true);
            return this.currentValue;
        }


        private int SetValueToDriver(object value)
        {
            this.requestedValue = ConvertValue(value);

            return WriteValueSync(true);
        }

        private void SetObservableValue(object oValue)
        {
            this.requestedValue = ConvertValue(oValue);
            WriteValueSync(true);

        }

        #endregion DriverReadWriting_Method

        #region ObserbleEventRaiseMethod

        private void BroadcastObservableValueChangedEvent()
        {
            const string METHOD = CLASS + ".BroadcastObservableValueChangedEvent";

            //Write Log 
            //this.WriteObservbleLog(valueChangedByWrite);

            if (this.ObservableValueChanged != null)
            {
                DelegateObservableValueChanged delValueChanged = null;
                Delegate[] invocationList = null;

                try
                {
                    invocationList = this.ObservableValueChanged.GetInvocationList();
                }
                catch (MemberAccessException ex)
                {
                    throw ex;
                }

                if (invocationList != null)
                {
                    lock (monitor)
                    {
                        foreach (Delegate del in invocationList)
                        {
                            try
                            {
                                delValueChanged = (DelegateObservableValueChanged)del;
                                // 
                                // delegateValueChanged(this, this.Value, null, null);
                                // ! Asynchronous Invocation
                                //

                                //delValueChanged.BeginInvoke(this, this.Value, null, null);
                                delValueChanged.Invoke(this, this.Value);

                            }
                            catch (Exception ex)
                            {
                                //this.ObservableValueChanged -= delValueChanged;
                                Log.log(5, METHOD, "error", Category.ERROR, "", "observer",
                                "Failed to send observableValueChanged to subscriber\nID:" + this.observableId + ex.Message.ToString());
                            }
                        }
                    }
                }
            }

        }

        private void BroadcastBooleanEvent()
        {
            if (this.DataType == DataType.BOOLEAN)
            {
                if ((bool)this.currentValue)
                    BroadcastOnEvent();
                else
                    BroadcastOffEvent();
            }
        }

        private void BroadcastOnEvent()
        {
            const string METHOD = CLASS + ".BroadcastOnEvent";
            if (this.ObservableOn != null)
            {
                DelegateObservableEvent delObservableOn = null;
                Delegate[] invocationList = null;

                try
                {
                    invocationList = this.ObservableOn.GetInvocationList();
                }
                catch (MemberAccessException ex)
                {
                    throw ex;
                }

                if (invocationList != null)
                {
                    lock (monitor)
                    {
                        foreach (Delegate del in invocationList)
                        {
                            try
                            {
                                delObservableOn = (DelegateObservableEvent)del;

                                //delObservableOn.BeginInvoke(this, null, null);
                                delObservableOn.Invoke(this);
                            }
                            catch (Exception ex)
                            {
                                this.ObservableOn -= delObservableOn;

                                Log.log(5, METHOD, "error", Category.ERROR, "", "observer",
                                "Failed to send observableOn to subscriber\n" + ex.Message.ToString());
                            }
                        }
                    }
                }
            }
        }

        private void BroadcastOffEvent()
        {
            const string METHOD = CLASS + ".BroadcastOffEvent";

            if (this.ObservableOff != null)
            {
                DelegateObservableEvent delObservableOff = null;
                Delegate[] invocationList = null;

                try
                {
                    invocationList = this.ObservableOff.GetInvocationList();
                }
                catch (MemberAccessException ex)
                {
                    throw ex;
                }

                if (invocationList != null)
                {
                    lock (monitor)
                    {
                        foreach (Delegate del in invocationList)
                        {
                            try
                            {
                                delObservableOff = (DelegateObservableEvent)del;
                                //delObservableOff.BeginInvoke(this, null, null);
                                delObservableOff.Invoke(this);
                            }
                            catch (Exception ex)
                            {
                                this.ObservableOff -= delObservableOff;

                                Log.log(5, METHOD, "error", Category.ERROR, "", "observer",
                                "Failed to send observableOff event to subscriber\n" + ex.Message.ToString());
                            }
                        }
                    }
                }
            }
        }

        #endregion ObserbleEventRaiseMethod

        #region IValue

        public event DelegateValueChanged OnValueChanged = null;

        public int SetValue(object oValue)
        {
            return SetValueToDriver(oValue);
        }

        private void BroadcastValueChangedEvent()
        {
            const string METHOD = CLASS + ".BroadcastValueChangedEvent";

            if (OnValueChanged != null)
            {
                DelegateValueChanged delValueChanged = null;
                Delegate[] invocationList = null;

                try
                {
                    invocationList = OnValueChanged.GetInvocationList();
                }
                catch (MemberAccessException ex)
                {
                    throw ex;
                }

                if (invocationList != null)
                {
                    lock (monitor)
                    {
                        foreach (Delegate del in invocationList)
                        {
                            try
                            {
                                delValueChanged = (DelegateValueChanged)del;
                                //
                                //delegateValueChanged(this, this.m_oCurrentValue);
                                // ! Asynchronous Invocation
                                //


                                //delValueChanged.BeginInvoke(this, this.currentValue, null, null);
                                delValueChanged.Invoke(this, this.currentValue);
                            }
                            catch (Exception ex)
                            {
                                //OnValueChanged -= delValueChanged;

                                Log.log(5, METHOD, "error", Category.ERROR, "", "observer",
                                "Failed to send OnValueChanged event to subscriber\n" + ex.Message.ToString());

                            }
                        }
                    }
                }
            }
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
                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetVarChar("OWNERID", this.ownerId);
                psparam.SetVarChar("OBSERVABLEID", this.ObservableId);

                reader = theDB.ExecuteQuery("select * from tb_Observable where OWNERID =@OWNERID and OBSERVABLEID =@OBSERVABLEID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "observable", this.ownerId + ":" + this.ObservableId + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString(this.ownerType));
                    pss.SetVarChar("OBSERVABLEID", this.ObservableId);
                    pss.SetUniqueIdentifier("OBSERVABLEGUID", this.observableGuid);
                    pss.SetVarChar("OBSERVABLEPATH", this.observablePath);
                    pss.SetVarChar("ACCESSTYPE", AccessType.ValueToString(this.accessType));
                    pss.SetVarChar("DATATYPE", DataType.ValueToString(this.dataType));
                    pss.SetInt("MAXVALUE", Convert.ToInt32(this.max));
                    pss.SetInt("MINVALUE", Convert.ToInt32(this.min));
                    pss.SetInt("DRIVERNO", Convert.ToInt32(this.driverNo));
                    pss.SetChar("MONFLAG", this.isMonitoring ? "T" : "F");
                    pss.SetVarChar("CONNECTSTRING", this.ConnectionInfoString);
                    pss.SetVarChar("DESCRIPTION", this.description);
                    pss.SetChar("LOGENABLE", this.logEnable ? "T" : "F");
                    pss.SetVarChar("VALUEINFORMATION", this.valueInformation);
                    pss.SetVarChar("REMARK", this.remark);
                    theDB.ExecuteUpdate("insert into tb_Observable (OWNERID,OWNERTYPE, OBSERVABLEID,OBSERVABLEGUID,OBSERVABLEPATH,ACCESSTYPE,DATATYPE,MINVALUE,MAXVALUE,DRIVERNO,MONFLAG,CONNECTSTRING,DESCRIPTION,LOGENABLE,VALUEINFORMATION,REMARK) values (@OWNERID,@OWNERTYPE,@OBSERVABLEID,@OBSERVABLEGUID,@OBSERVABLEPATH,@ACCESSTYPE,@DATATYPE,@MINVALUE,@MAXVALUE,@DRIVERNO,@MONFLAG,@CONNECTSTRING,@DESCRIPTION,@LOGENABLE,@VALUEINFORMATION,@REMARK)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

        public void load(string ownerId, string observableId)
        {
            this.ownerId = ownerId;
            this.ObservableId = observableId;
            Load();
        }

        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".delete";

            try
            {
                base.Delete(theDB);
                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("OWNERID", this.ownerId);
                ps.SetVarChar("OBSERVABLEID", this.ObservableId);

                theDB.ExecuteUpdate("delete from tb_Observable where OWNERID =@OWNERID and OBSERVABLEID =@OBSERVABLEID", ps);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
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

                psqlparam.SetVarChar("OWNERID", this.ownerId);
                psqlparam.SetVarChar("OBSERVABLEID", this.ObservableId);


                reader = theDB.ExecuteQuery("select * from tb_Observable where OWNERID =@OWNERID and OBSERVABLEID =@OBSERVABLEID", psqlparam);


                if (reader.Read())
                {
                    this.ownerType = ElementType.StringToValue((string)reader["OWNERTYPE"]);
                    //                    this.observableGuid = (Guid)reader["OBSERVABLEGUID"];
                    this.observableGuid = new Guid((string)reader["OBSERVABLEGUID"]);
                    this.observablePath = (string)reader["OBSERVABLEPATH"];


                    this.accessType = AccessType.StringToValue((string)reader["ACCESSTYPE"]);
                    this.dataType = DataType.StringToValue((string)reader["DATATYPE"]);

                    this.min = int.Parse((string)reader["MINVALUE"]);
                    this.max = int.Parse((string)reader["MAXVALUE"]);

                    this.driverNo = int.Parse((string)reader["DRIVERNO"]);
                    this.IsMonitoring = ((string)reader["MONFLAG"]).Equals("T");

                    this.ConnectionInfoString = (string)reader["CONNECTSTRING"];
                    this.description = (string)reader["DESCRIPTION"];
                    //Array Type의 Length
                    if (isArrayType(this.dataType))
                        this.listSize = (int)this.connectionInfo["LENGTH", 0];

                    this.logEnable = ((string)reader["LOGENABLE"]).Equals("T");
                    this.valueInformation = (string)reader["VALUEINFORMATION"];
                    this.remark = (string)reader["REMARK"];

                }
                else
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "observable", this.ObservableId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
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
                    theDB.ExecuteUpdate("update tb_Observable set\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "',\n" +
                        "OBSERVABLEGUID='" + this.observableGuid + "',\n" +
                        "OBSERVABLEPATH='" + this.observablePath + "',\n" +
                        "ACCESSTYPE='" + AccessType.ValueToString(this.accessType) + "',\n" +
                        "DATATYPE='" + DataType.ValueToString(this.dataType) + "',\n" +
                        "MINVALUE='" + Convert.ToString(this.min) + "',\n" +
                        "MAXVALUE='" + Convert.ToString(this.Max) + "',\n" +
                        "DRIVERNO='" + Convert.ToString(this.driverNo) + "',\n" +
                        "MONFLAG='" + (this.IsMonitoring ? "T" : "F") + "',\n" +
                        "CONNECTSTRING='" + this.ConnectionInfoString + "',\n" +
                        "DESCRIPTION='" + this.Description + "',\n" +
                        "LOGENABLE='" + (this.logEnable ? "T" : "F") + "',\n" +
                        "VALUEINFORMATION='" + this.valueInformation + "',\n" +
                        "REMARK='" + this.remark + "'\n" +
                        "where OWNERID = '" + this.ownerId + "' " + "and\n" +
                        "OBSERVABLEID = ' " + this.ObservableId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
            }
        }
        #endregion Manage_Persistent_Attribute

        #region Method

        /// <summary>
        /// ObservableLog Write
        /// </summary>
        /// <param name="isWriteLog">True: Wite Log type ,False : Read Log  type</param>
        private void WriteObservbleLog(bool isWriteLog)
        {
            //            string preValue = this.PreviousValue == null ? "" : this.PreviousValue.ToString();
            //            string curValue = this.Value == null ? "" : this.Value.ToString();
            string defaultValue = this.DefaultValue == null ? "" : this.DefaultValue.ToString();
            string valueInformtion = this.valueInformation == null ? "" : this.valueInformation.ToString();


            ObservableStatisticsCollectorData collectorData = new ObservableStatisticsCollectorData(
                                                              this.ObservableId, this.OwnerId, DataType.ValueToString(this.DataType),
                                                              this.previousValue, this.currentValue, Convert.ToString(this.DriverNo),
                                                              this.ConnectionInfoString, isWriteLog, valueInformtion);
            if (this.logEnable || isWriteLog)
            {
                if (ObservableLogEvent != null)
                {
                    ObservableLogEvent(this, collectorData);
                }
                else
                {

                    StatisticsCollector.TriggerOccured(
                                                        ObservableStatisticsCollectorData.COLLECTOR_ID,
                                                        ObservableStatisticsCollectorData.TRIGGER_ID,
                                                        collectorData);
                }
            }

        }


        /// <summary>
        /// Arrry Type의 초기값 부여
        /// </summary>
        /// <param name="newValue"></param>
        private void SetInitialValueForArryType(out object newValue)
        {
            newValue = GetInitValue();

            switch (this.dataType.Value())
            {
                case DataType._SHORT_ARRAY:

                    CopyList<short>((List<short>)this.currentValue, (List<short>)newValue);
                    break;
                case DataType._BOOL_ARRAY:
                    CopyList<bool>((List<bool>)this.currentValue, (List<bool>)newValue);
                    break;
            }
        }

        /// <summary>
        /// Data Type이 Arry Type인지 
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        private bool isArrayType(DataType type)
        {
            return (type == DataType.BOOL_ARRAY || type == DataType.SHORT_ARRAY);

        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        private int CompareList<T>(List<T> a, List<T> b)
        {
            if (a.Count != b.Count)
                return 1;

            for (int i = 0; i < a.Count; i++)
            {
                if (!a[i].Equals(b[i]))
                    return 0;
            }

            return 1;
        }

        private int CopyList<T>(List<T> source, List<T> target)
        {
            if (source == null || source.Count > this.listSize)
                return 1;

            for (int i = 0; i < source.Count; i++)
            {
                target[i] = source[i];
            }

            return 0;

        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="source"></param>
        /// <param name="target"></param>
        /// <returns></returns>
        private int CloneList<T>(List<T> source, List<T> target)
        {
            if (source == null || source.Count > this.listSize)
                return 1;

            target.Clear();
            for (int i = 0; i < source.Count; i++)
            {
                target.Add(source[i]);
            }

            return 0;
        }

        public static Dictionary<string, Observable> LoadObservables(string ownerId)
        {
            const string METHOD = CLASS + ".LoadObservables";

            Dictionary<string, Observable> observables = new Dictionary<string, Observable>();
            string observableId = null;
            ObjectHandle objHandle = null;
            Observable obs = null;

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select OBSERVABLEID from TB_OBSERVABLE where OWNERID ='" + ownerId + "' order by [description]";

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {

                try
                {
                    objHandle = Activator.CreateInstance("ECP", "ECP.Device.Observe.Observable");
                    obs = (Observable)objHandle.Unwrap();
                }
                catch (SqlException ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "load observables Sql Error : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                catch (Exception ex)
                {
                    Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "failed to load observables : " + ownerId +
                        "\n" + ex.Message.ToString());
                    throw ex;
                }
                observableId = Helper.PreventNull((string)reader["OBSERVABLEID"]);

                obs.Load(ownerId, observableId);
                observables.Add(observableId, obs);
            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return observables;
        }

        public static Observable LoadObservable(string ownerId, string observableId)
        {
            const string METHOD = CLASS + ".LoadObservable";

            ObjectHandle objHandle = null;
            Observable obs = null;

            try
            {
                objHandle = Activator.CreateInstance("ECP", "ECP.Device.Observe.Observable");
                obs = (Observable)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "failed to load observable : " + observableId +
                    "\n" + ex.Message.ToString());
            }
            obs.Load(ownerId, observableId);

            return obs;

        }

        public void Load(string ownerId, string observableId)
        {
            this.ownerId = ownerId;
            this.ObservableId = observableId;
            Load();
            this.currentValue = this.GetInitValue();
            this.previousValue = null;
            this.requestedValue = null;
        }

        public static Observable CreateObservable(string ownerId, ElementType ownerType, string observableId,
            string observablePath, AccessType accessType, DataType dataType, double min, double max, int drvierNo,
            bool isMonitoring, string connectionInfoString, string description, bool logEnable, string valueInformation, string remark)
        {

            const string METHOD = CLASS + ".createObservable";

            ObjectHandle objHandle = null;
            Observable obs = null;
            try
            {
                objHandle = Activator.CreateInstance("ECP", "ECP.Device.Observe.Observable");
                obs = (Observable)objHandle.Unwrap();
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable",
                    "failed to create observable Sql Error : " + observableId +
                    "\n" + ex.Message.ToString());
                throw ex;

            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable",
                    "failed to create observable : " + observableId +
                    "\n" + ex.Message.ToString());
                throw ex;
            }

            try
            {
                obs.ownerId = ownerId;
                obs.ownerType = ownerType;
                obs.ObservableId = observableId;
                obs.observableGuid = Guid.NewGuid();
                obs.observablePath = observablePath;

                obs.accessType = accessType;
                obs.dataType = dataType;
                obs.min = min;
                obs.max = max;
                obs.driverNo = drvierNo;
                obs.isMonitoring = isMonitoring;
                obs.ConnectionInfoString = connectionInfoString;
                obs.logEnable = logEnable;
                obs.Description = description;
                obs.dirtyFlag = true;
                obs.valueInformation = valueInformation;
                obs.remark = remark;
                obs.Add();
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "observable Sql error \n " + ex.Message.ToString());
                throw ex;
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "observable error \n " + ex.Message.ToString());
                throw ex;
            }

            return obs;

        }

        public static Dictionary<string, Observable> getAllObservable()
        {
            Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select * from TB_OBSERVABLE order by OWNERID,CONNECTSTRING ";
            SqlDataReader reader;
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "getAllObservable", "error", Category.ERROR, "", "Allobservable", "observable Sql error \n " + ex.Message.ToString());
                throw ex;

            }

            Observable obs = null;

            try
            {

                while (reader.Read())
                {
                    obs = LoadObservable((string)reader["OWNERID"], (string)reader["OBSERVABLEID"]);
                    observables.Add(obs.ObservableGuid.ToString(), obs);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "getAllObservable", "error", Category.ERROR, "", "Allobservable", "observable error \n " + ex.Message.ToString());
                throw ex;

            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return observables;
        }

        public override bool IsAlive()
        {
            return base.IsAlive();
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
        #endregion Method

        #region Manage_MasterObservable_Data_Method
        public void AddToMasterDB()
        {
            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();

                theDB.Start();
                AddToMasterDB(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                throw ex;
                //theDB.rollback();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }

        public void AddToMasterDB(DBConnection theDB)
        {
            const string METHOD = CLASS + ".AddToMasterDB";

            SqlDataReader reader = null;
            try
            {
                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetVarChar("OWNERID", this.ownerId);
                psparam.SetVarChar("OBSERVABLEID", this.ObservableId);

                reader = theDB.ExecuteQuery("select * from ZCONFIG_OBSERVABLE_PROTOTYPE where OWNERID =@OWNERID and OBSERVABLEID =@OBSERVABLEID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "masterObservable", this.ownerId + ":" + this.ObservableId + "already Exist");
                    //throw new ECPException
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("OWNERID", this.ownerId);
                    pss.SetVarChar("OWNERTYPE", ElementType.ValueToString(this.ownerType));
                    pss.SetVarChar("OBSERVABLEID", this.ObservableId);
                    pss.SetUniqueIdentifier("OBSERVABLEGUID", this.observableGuid);
                    pss.SetVarChar("OBSERVABLEPATH", this.observablePath);
                    pss.SetVarChar("ACCESSTYPE", AccessType.ValueToString(this.accessType));
                    pss.SetVarChar("DATATYPE", DataType.ValueToString(this.dataType));
                    pss.SetInt("MAXVALUE", Convert.ToInt32(this.max));
                    pss.SetInt("MINVALUE", Convert.ToInt32(this.min));
                    pss.SetInt("DRIVERNO", Convert.ToInt32(this.driverNo));
                    pss.SetChar("MONFLAG", this.isMonitoring ? "T" : "F");
                    pss.SetVarChar("CONNECTSTRING", this.ConnectionInfoString);
                    pss.SetVarChar("DESCRIPTION", this.description);
                    pss.SetChar("LOGENABLE", this.logEnable ? "T" : "F");
                    pss.SetChar("VALUEINFORMATION", this.valueInformation);
                    pss.SetChar("REMARK", this.remark);
                    theDB.ExecuteUpdate("insert into ZCONFIG_OBSERVABLE_PROTOTYPE (OWNERID,OWNERTYPE, OBSERVABLEID,OBSERVABLEGUID,OBSERVABLEPATH,ACCESSTYPE,DATATYPE,MINVALUE,MAXVALUE,DRIVERNO,MONFLAG,CONNECTSTRING,DESCRIPTION,LOGENABLE,VALUEINFORMATION,REMARK) values (@OWNERID,@OWNERTYPE,@OBSERVABLEID,@OBSERVABLEGUID,@OBSERVABLEPATH,@ACCESSTYPE,@DATATYPE,@MINVALUE,@MAXVALUE,@DRIVERNO,@MONFLAG,@CONNECTSTRING,@DESCRIPTION,@LOGENABLE,@VALUEINFORMATION,@REMARK)", pss);
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

        public void SaveToMasterDB()
        {
            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();

                theDB.Start();
                SaveToMasterDB(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                throw ex;
                //theDB.rollback();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }

        public void SaveToMasterDB(DBConnection theDB)
        {
            const string METHOD = CLASS + ".SaveToMasterDB";

            this.dirtyFlag = true;
            try
            {
                if (dirtyFlag)
                {
                    this.dirtyFlag = false;
                    theDB.ExecuteUpdate("update ZCONFIG_OBSERVABLE_PROTOTYPE set\n" +
                        "OWNERTYPE='" + ElementType.ValueToString(this.ownerType) + "',\n" +
                        "OBSERVABLEGUID='" + this.observableGuid.ToString() + "',\n" +
                        "ACCESSTYPE='" + AccessType.ValueToString(this.accessType) + "',\n" +
                        "DATATYPE='" + DataType.ValueToString(this.dataType) + "',\n" +
                        "MINVALUE='" + Convert.ToString(this.min) + "',\n" +
                        "MAXVALUE='" + Convert.ToString(this.Max) + "',\n" +
                        "DRIVERNO='" + Convert.ToString(this.driverNo) + "',\n" +
                        "MONFLAG='" + (this.IsMonitoring ? "T" : "F") + "',\n" +
                        "CONNECTSTRING='" + this.ConnectionInfoString + "',\n" +
                        "DESCRIPTION='" + this.description + "',\n" +
                        "LOGENABLE='" + (this.logEnable ? "T" : "F") + "',\n" +
                        "VALUEINFORMATION='" + this.valueInformation + "',\n" +
                        "REMARK='" + this.remark + "'\n" +
                        "where OWNERID = '" + this.ownerId + "' " + " and\n" +
                        "OBSERVABLEID = '" + this.observableId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "observable", "sql error \n " + ex.Message.ToString());
            }
        }

        public void DeleteFromMasterDB()
        {
            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();

                theDB.Start();
                DeleteFromMasterDB(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                throw ex;
                //theDB.rollback();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }

        public void DeleteFromMasterDB(DBConnection theDB)
        {
            const string METHOD = CLASS + ".DeleteFromMasterDB";

            try
            {
                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();
                psparam.SetVarChar("OWNERID", this.ownerId);
                psparam.SetVarChar("OBSERVABLEID", this.ObservableId);

                theDB.ExecuteUpdate("delete from ZCONFIG_OBSERVABLE_PROTOTYPE where OWNERID =@OWNERID and OBSERVABLEID =@OBSERVABLEID", psparam);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "masterObservable", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
        }



        public static Observable GetObservableFromMasterDB(string ownerId, string observableId)
        {
            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select * from ZCONFIG_OBSERVABLE_PROTOTYPE \n" +
                               "where OWNERID ='" + ownerId.Trim() + "'\n" +
                               "and OBSERVABLEID = '" + observableId.Trim() + "'";
            SqlDataReader reader;
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "GetObservableFromMasterDB", "error", Category.ERROR, "", "GetObservableFromMasterDB", "observable Sql error \n " + ex.Message.ToString());
                throw ex;

            }

            Observable obs = null;

            try
            {
                while (reader.Read())
                {
                    obs = new Observable(
                        (string)reader["OBSERVABLEID"],
                        (string)reader["OWNERID"],
                        ElementType.StringToValue((string)reader["OWNERTYPE"]),
                        (string)reader["OBSERVABLEPATH"],
                        AccessType.StringToValue((string)reader["ACCESSTYPE"]),
                        DataType.StringToValue((string)reader["DATATYPE"]),

                        (int)reader["MINVALUE"],
                        (int)reader["MAXVALUE"],
                        (int)reader["DRIVERNO"],
                        ((string)reader["MONFLAG"]).Equals("T"),
                        (string)reader["CONNECTSTRING"],
                        (string)reader["DESCRIPTION"],
                        ((string)reader["LOGENABLE"]).Equals("T"),
                        (string)reader["VALUEINFORMATION"],
                        (string)reader["REMARK"]);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "GetObservableFromMasterDB", "error", Category.ERROR, "", "GetObservableFromMasterDB", "observable error \n " + ex.Message.ToString());
                throw ex;

            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return obs;
        }

        public static Dictionary<string, Observable> GetAllObservablesFromMasterDB()
        {
            Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select * from ZCONFIG_OBSERVABLE_PROTOTYPE";
            SqlDataReader reader;
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "GetAllObservablesFromMasterDB", "error", Category.ERROR, "", "GetAllObservablesFromMasterDB", "observable Sql error \n " + ex.Message.ToString());
                throw ex;

            }

            Observable obs = null;

            try
            {
                while (reader.Read())
                {
                    obs = new Observable(
                        (string)reader["OBSERVABLEID"],
                        (string)reader["OWNERID"],
                        ElementType.StringToValue((string)reader["OWNERTYPE"]),
                        (string)reader["OBSERVABLEPATH"],
                        AccessType.StringToValue((string)reader["ACCESSTYPE"]),
                        DataType.StringToValue((string)reader["DATATYPE"]),

                        (int)reader["MINVALUE"],
                        (int)reader["MAXVALUE"],
                        (int)reader["DRIVERNO"],
                        ((string)reader["MONFLAG"]).Equals("T"),
                        (string)reader["CONNECTSTRING"],
                        (string)reader["DESCRIPTION"],
                        ((string)reader["LOGENABLE"]).Equals("T"),
                        (string)reader["VALUEINFORMATION"],
                        (string)reader["REMARK"]);
                    observables.Add(obs.ObservableGuid.ToString(), obs);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "GetAllObservablesFromMasterDB", "error", Category.ERROR, "", "GetAllObservablesFromMasterDB", "observable error \n " + ex.Message.ToString());
                throw ex;

            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return observables;
        }

        public static Dictionary<string, Observable> GetObservablesOfSpecificOwnerIdFromMasterDB(string ownerId)
        {
            Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select * from ZCONFIG_OBSERVABLE_PROTOTYPE where ownerId ='" + ownerId.Trim() + "'";
            SqlDataReader reader;
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "GetObservablesOfSpecificOwnerIdFromMasterDB", "error", Category.ERROR, "", "GetObservablesOfSpecificOwnerIdFromMasterDB", "observable Sql error \n " + ex.Message.ToString());
                throw ex;

            }

            Observable obs = null;

            try
            {
                while (reader.Read())
                {
                    obs = new Observable(
                        (string)reader["OBSERVABLEID"],
                        (string)reader["OWNERID"],
                        ElementType.StringToValue((string)reader["OWNERTYPE"]),
                        (string)reader["OBSERVABLEPATH"],
                        AccessType.StringToValue((string)reader["ACCESSTYPE"]),
                        DataType.StringToValue((string)reader["DATATYPE"]),

                        (int)reader["MINVALUE"],
                        (int)reader["MAXVALUE"],
                        (int)reader["DRIVERNO"],
                        ((string)reader["MONFLAG"]).Equals("T"),
                        (string)reader["CONNECTSTRING"],
                        (string)reader["DESCRIPTION"],
                        ((string)reader["LOGENABLE"]).Equals("T"),
                        (string)reader["VALUEINFORMATION"],
                        (string)reader["REMARK"]);
                    observables.Add(obs.ObservableGuid.ToString(), obs);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "GetObservablesOfSpecificOwnerIdFromMasterDB", "error", Category.ERROR, "", "GetObservablesOfSpecificOwnerIdFromMasterDB", "observable error \n " + ex.Message.ToString());
                throw ex;

            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return observables;
        }

        public static Dictionary<string, Observable> GetObservablesBySpecificQueryFromMasterDB(string sqlStatement)
        {
            Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

            DBConnection conn = DBControl.GetConnection();

            string sqlstring = "select * from ZCONFIG_OBSERVABLE_PROTOTYPE\n" + sqlStatement;
            SqlDataReader reader;
            try
            {
                reader = conn.ExecuteQuery(sqlstring);
            }
            catch (SqlException ex)
            {
                Log.log(5, "GetObservablesBySpecificQueryFromMasterDB", "error", Category.ERROR, "", "GetObservablesBySpecificQueryFromMasterDB", "observable Sql error \n " + ex.Message.ToString());
                throw ex;

            }

            Observable obs = null;

            try
            {
                while (reader.Read())
                {
                    obs = new Observable(
                        (string)reader["OBSERVABLEID"],
                        (string)reader["OWNERID"],
                        ElementType.StringToValue((string)reader["OWNERTYPE"]),
                        (string)reader["OBSERVABLEPATH"],
                        AccessType.StringToValue((string)reader["ACCESSTYPE"]),
                        DataType.StringToValue((string)reader["DATATYPE"]),

                        (int)reader["MINVALUE"],
                        (int)reader["MAXVALUE"],
                        (int)reader["DRIVERNO"],
                        ((string)reader["MONFLAG"]).Equals("T"),
                        (string)reader["CONNECTSTRING"],
                        (string)reader["DESCRIPTION"],
                        ((string)reader["LOGENABLE"]).Equals("T"),
                        (string)reader["VALUEINFORMATION"],
                        (string)reader["REMARK"]);
                    observables.Add(obs.ObservableGuid.ToString(), obs);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, "GetObservablesBySpecificQueryFromMasterDB", "error", Category.ERROR, "", "GetObservablesBySpecificQueryFromMasterDB", "observable error \n " + ex.Message.ToString());
                throw ex;

            }

            if (reader != null)
                reader.Close();

            DBControl.RemoveDBConection(conn);
            return observables;
        }
        #endregion
    }

    public sealed class ObservableCollection : Dictionary<Guid, Observable>
    {
        public void Add(Observable observable)
        {
            if (observable == null)
                return;

            Add(observable.ObservableGuid, observable);
        }
    }
}
