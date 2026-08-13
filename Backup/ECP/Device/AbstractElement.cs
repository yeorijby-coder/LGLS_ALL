using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Logger;
using ECP.Device.Location;
using ECP.Db;
using ECP.Util;
using ECP.Util.Client;
using ECP.Global.Device;
using ECP.Device.Observe;
using ECP.Global;
using ECP.Com.FieldBus;
using ECP.Event;

namespace ECP.Device
{
    /// <summary>
    /// AbstractElement는 Semi E98(OBEM)의 Equipment Object Model의 AbstractEquipementElement를 
    /// 나타내는 Class이다. AbstractEquipmentElement는  직접 구현할 수 없는 추상 객체이며,
    /// AbstractEquipment Subsystem과 AbstractEquipment I/O Device의 두개의 Subtype을 가진다
    /// 
    ///  
    /// AbstractElement는  Equipment Object Model 왼편의 반전된 Interface 상속 관계의 
    /// 최상위 Simple Interface이며, AbstractSubSystem 및 AbstractModuel등의 하위 Concreate한 
    /// Sub Type에 의하여 더욱 구체화 되어 질 것이다. 
    /// 
    /// AbstractEquipmentElement 은 IN SERVICE 또는 OUT OF SERVICE 두 상태 모델이 있다.
    /// 어떤 경우에는 User가 Operational State로 Set할 수 있다
    /// 
    /// 
    /// 또한 User Defined Property를 Functionality를 가지기 위하여 AbstractUDPElement를 상속하고
    /// 있다.
    /// 
    /// Observable이 이 Class의 Property로 있는 이유는 OBEM을 정확히 구현하는데 있어서
    /// 위배한 Case가 되는 것이나, IODevice에 있어야 할 Observable을 이곳에 옮겨 놓음으로 
    /// 하위 SubType들은 IODevice을 가지고 있지 않은 상태에서 직접적으로 장비의 Sensor나 
    /// Actuator의기능을 가질 수 있게 되어 프로젝트의 편의상 그렇게 한 것이다. 
    /// 
    /// Abstract Class인 AbstractElement에 Observable의 Container을 가지게 한 이유는
    /// SubSystem, Module,Equipment와 같은 Concreate Class는 상속관계를 가지고 있지 않음으로,
    /// 이곳에 Container을 제공함으로, SubSystem, Module, Equipment가 자동적으로
    /// Obaservable Capability를 가지게 하고자 함에 있다.
    ///
    /// 
    /// </summary>
    [Serializable]
    abstract public class AbstractElement : AbstractUDPElement
    {

        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "AbstractElement";

        #region Attribute_Not_Persistance

        /// <summary>
        /// Obserable의 Container, 이 Dictionary는 Obseraveble의 GUID String 또는 Obseravble ID을 Key
        /// 가질 수 있따.
        /// </summary>
        protected Dictionary<string, Observable> observables = new Dictionary<string, Observable>();

        /// <summary>
        /// Observable Access Member Public Method
        /// </summary>
        public Dictionary<string, Observable> Observables
        {
            get { return observables; }
            set { observables = value; }
        }

        /// <summary>
        /// 이 Property는 Static으로 선어 되어 있음에 주의 해야 함.
        /// Glabal Observable Property는 이 Program내의 모든 Sub System, Module, Equipement등의 
        /// Obseravble을 저장한다.
        /// </summary>
        private static Dictionary<string, Observable> global_Observables = new Dictionary<string, Observable>();

        public static Dictionary<string, Observable> Global_Observables
        {
            get { return AbstractElement.global_Observables; }
            set { AbstractElement.global_Observables = value; }
        }


        /// <summary>
        /// Element Object ID
        /// </summary>
        protected string elementId;

        public string ElementId
        {
            get { return elementId; }
            set { elementId = value; }
        }

        /// <summary>
        /// Element의 Type, Equipment, Module, SubSystem, IODevice가 될수 있다.
        /// </summary>
        protected ElementType elementType;

        public ElementType ElementType
        {
            get { return elementType; }
            set { elementType = value; }
        }

        /// <summary>
        /// 이 Element를 Contain하고 있는 Object의 ID
        /// </summary>
        protected string ownerId;

        public string OwnerId
        {
            get { return ownerId; }
            set { ownerId = value; }
        }


        /// <summary>
        /// Owner Element의 Type, Equipment, Module, SubSystem, IODevice가 될수 있다.
        /// </summary>
        protected ElementType ownerType;

        public ElementType OwnerType
        {
            get { return ownerType; }
            set { ownerType = value; }
        }


        /// <summary>
        /// AbstractElement가 Error가 없는 상태 또는 작업을 위하여 사용 가능한 상태일 경우 True
        /// </summary>
        protected bool inService = false;

        public bool InService
        {
            get { return inService; }
            set { inService = value; }
        }

        /// <summary>
        /// H/W 제조자나 공급자의 Id
        /// </summary>
        protected string supplier = null;

        public string Supplier
        {
            get { return supplier; }
            set { supplier = value; }
        }

        /// <summary>
        /// Element에 대한 상세 설명
        /// </summary>
        protected string description = null;

        public string Description
        {
            get { return description; }
            set { description = value; }
        }

        /// <summary>
        /// Install되거나, 최종 Reset된 이후로 부터의 Cycle 횟수 또는 시간
        /// </summary>
        protected ulong cycles;

        public ulong Cycles
        {
            get { return cycles; }
            set { cycles = value; }
        }
        #endregion Attribute_Not_Persistance

        #region Constructor

        public AbstractElement()
        {
        }
        #endregion Constructor

        #region Method

        /// <summary>
        /// Obaservabe을 Element에 Add
        /// </summary>
        /// <param name="observableId">추가할 Observable의 ID</param>
        /// <param name="observablePath">Observable의 Path, Observable의 유일성을 확보하기 위하여, Unique한 Path을 지정 </param>
        /// <param name="accessType">Observable의 Access Type Input/Output/InOutput</param>
        /// <param name="dataType">Observabe의 나타내는 Data의 Type</param>
        /// <param name="min">Observable이 허용 할 수 있는 최소 값</param>
        /// <param name="max">Observable이 허용 할 수 있는  최대 값</param>
        /// <param name="drivierNo">Observable 이 할당되어 있는 Field Bus Driver No</param>
        /// <param name="isMonitoring">Observable의 Monitoring 대상 여부, 만약 모니터링 대상 일 경우 Ture </param>
        /// <param name="connectionInfoString">Obseravable의 Driver Specific한 Connection string 명</param>
        /// <param name="description">Observabe에 대한 설명</param>
        /// <param name="logEnable">ture면 Logging</param>
        /// <param name="valueInfomation"></param>
        /// <param name="remark"></param>
        public void AddObservable(string observableId, string observablePath, AccessType accessType,
                                  DataType dataType, double min, double max, int drivierNo, bool isMonitoring, string connectionInfoString,
                                  string description, bool logEnable, string valueInfomation, string remark)
        {
            const string METHOD = CLASS + ".AddObservable";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding iodevice: " + observableId);

            Observable obs = null;
            obs = Observable.CreateObservable(this.elementId, this.elementType, observableId,
                observablePath, accessType, dataType, min, max, drivierNo, isMonitoring, connectionInfoString,description,logEnable,valueInfomation,remark);

            if (!this.observables.ContainsKey(observableId))
            {
                this.observables.Add(observableId, obs);
            }
            //else
            //{
            //    throw new ECPException(ECPExceptionType.DUPLICATE, "", "Duplicate ioDeviceId " + ownerId);
            //}

        }

        /// <summary>
        /// 해당 Obasevable을 Element에서 삭제
        /// </summary>
        /// <param name="observableId"></param>
        public void RemoveObservable(string observableId)
        {
            const string METHOD = CLASS + ".RemoveObservable";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing observable: " + observableId);

            Observable obs = null;

            if (this.observables.ContainsKey(observableId))
            {
                obs = this.observables[observableId];
                obs.Delete();
                this.observables.Remove(observableId);
            }
        }

        /// <summary>
        /// 이 Element에 할당된 Obseravable의 Reference을 Return
        /// </summary>
        /// <param name="observableId">Observabe의 Id</param>
        /// <returns></returns>
        public Observable GetObservable(string observableId)
        {
            Observable obs = null;
            if (this.observables.ContainsKey(observableId))
            {
                obs = this.observables[observableId];
            }
            return obs;
        }

        /// <summary>
        /// 이 Element에 할당된 모든 Observable의 Reference을 담고 있는 Container Return
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Observable> GetObservables()
        {
            return Observables;
        }
        
        /// <summary>
        /// 이 Element와 이 Element와 Associated된 모든 Element의 Observable을 지정 Field Bus Driver에 
        /// 등록함
        /// </summary>
        /// <param name="fieldbusDriver">지정된 Field bus Driver</param>
        abstract public void RegisterObservableToFiledBusDriver(AbstractFieldBusDriver fieldbusDriver);
        #endregion Method

        /// <summary>
        /// 이 Element가 ECP Event를 Publish하기 위한 Utility Method
        /// </summary>
        /// <param name="subject">subject Id</param>
        /// <param name="ecpEvent">ECP Event Reference</param>
        protected void PublishEvent(string subject, ECPEvent ecpEvent)
        {
            Naming.GetEventBrokerManager().Publish(subject, ecpEvent);
        }
    }
}
