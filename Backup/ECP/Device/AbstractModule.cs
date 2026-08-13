using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Logger;
using ECP.Device;
using ECP.Global.Device;
using ECP.Global.Exception;
using ECP.Com.FieldBus;
using ECP.Device.Dispatch;
using ECP.Global.Com;

namespace ECP.Device
{

    /// <summary>
    /// AbstractEquipmentModule은 보다 높은 레벨의 복잡도 와 Factory에 보다 크게 중요한
    /// AbstractEquipmentSubsytem의 type이다. 그것은 주로 Process 모듈을 표현하기 위하여 
    /// 사용 되어지나, AbstractEquipmentModule을 위한 요구사항을 지원할 수 있는 
    /// 주요 Intelligent한 Subsystem을 위하여서도 사용되어진다. 어떤 경우에 있어서는
    /// 물리적인 모듈을 장비로부터 독립적으로 작동하게 하는 것도 가능하다.
    /// 
    /// AbstractEquipmentModule은 두 개의 Subtype을 가진다 그것은  Equipment와 EquipmentModule이다. 
    /// 
    /// AbstractEquipmentModule은 AbstractEquipmentElement와 AbstractEquipmentSubsystem의 
    /// 모든 속성, 상태모델, 서비스를 상속 받는다. AbstractEquipmentSubsystem로서 
    /// AbstractEquipmentModule은 한 개나, 그 이상의  Material을 가질 수 있다
    /// 
    /// AbstractEquipmentModule은 Process Chamber와 같은 주요 subsystem 모듈을 표현하다,
    /// 그것은 기본 Operation Command을 지원한다 : Start, Stop, Pause, Resume, Abort
    /// 
    /// SEMI 58을 구현함에, AbstractEquipmentModule은 SEMI 58과 부합해야 한다. 
    /// AbstractEquipmentModule은 SEMI 58(ARAMS)가 유지 되어야 하는 가장 작은 장비의 요소이다
    /// 
    /// AbstractEquipmentModule이 장비로부터 분리되어 전원 Off가 불가능할 경우, 
    /// 전원 Down에 대한 추정과, 마지막 전원Down시간을 제공 할 필요가 없다
    /// 
    /// SEMI E10에 일관성 있는 ARAMS을 구현 하기 위하여 AbstractEquipmentModule은 
    /// 그것이 Uptime일 때마다, INSERVICE이다, 다시 말하면, 모듈이 생산을 위하여 
    /// Scheduling되지 못 할 경우 그것은 OUTOFSERVICE상태가 된다AbstractEquipmentModule의 
    /// Service 상태를 변경 하기 위하여 User는 그것은 SEMI E10(RAM)상태를 변경해야 한다
    /// 
    /// Equipment에 독립적으로 작동하는 모듈의 경우 (CPU을 가진 모듈) AbstractEquipmentModule은 
    /// 개별적인 Clock을 가진다. 다 수개의 내부 Clock을 일치 시키기 위한 것은 Equipment의 책임이다
    /// 
    /// AbstractEquipmentModule은 그것은 주요 기능을 다음중의 하나로 Process Type을 가질 수 있다. 
    /// : Process(프로세싱), Measurement(측정), Transport(반송), Storage(저장), 
    /// 이것은 특별히 더 필요한대로, Text로 기술 될 수 있다
    /// 
    /// AbstractEquipmentModule은 한 개 또는 그 이상 ProcessCapabilities을 가진다. 
    /// Process Type과 ProcessCapabilities는 높은 레벨과 자세한 Process특징을 위해 사용 되어진다.
    /// User는 ProcessCapability Description을 AddProcessCapability와 RemoveProcessCapability를
    /// 통하여 추가, 삭제 할 수 있다
    /// 
    /// AbstractEquipmentModule은 Recipe ExecutionService와 Recipe을 받고,저장하고,검증하며, 
    /// 선택 할 수 있으며, Execution Recipe(SEMI E42)을 실행 할 수 있는 능력을 제공한다. 
    /// 이 능력은 측정 또는 생산(Process처리) 장비 AbstractEquipmentModule에게 요구되어 진다. 
    /// 제공받은 Recipe와 Recipe Execution은 SEMI E42(RMS)에 규정된 처럼. 
    /// Execution Recipe와 Recipe Executor을 위한 기본 요구사항에 부합하여야 한다
    /// 
    /// AbstractEquipmentModule은 Mechanical Dry Run을 할 수 있는 능력을 제공해야 한다
    /// 이것은 실제 Process작업이 이루어지거나, 소재가 사용됨 없이, Material Handling 
    /// Subsystem이나 Software 기능이 연습되어지고, 테스트 되어져야 한다. 
    /// 이것은 특별히 시간 Setting을 하거나, 온도, 가스, 플라즈마, 용수를 위한 Setting을
    /// 사용하지 않는 Recipe를 통하여 이루어 진다. 어떤 경우 그것은 특별한 구분 케이스인
    /// /DRYRUN/과 같은 Recipe을 요구한다
    /// 
    /// </summary>
    [Serializable]
    abstract public class AbstractModule : AbstractSubSystem
    {
        private const string CLASS = "AbstractModule";

        #region Attribute

        /// <summary>
        /// Module은 내부에 Module을 가질 수있다, Module이 가지고 있는 Moudle에 대한 Container
        /// Moudle은 또한 SubSystem,IODevice, Observable을 가질 수 있으며, 이것은
        /// Moudle이 Abstract SubSystem을 상속함으로 자연스럽게 능력이 승계되어진다.
        /// </summary>
        protected Dictionary<string, Module> modules = new Dictionary<string,Module>();

        /// <summary>
        /// Module이 가질 수 있는 Field Bus Driver을 담고 있는 Container
        /// </summary>
        protected Dictionary<int, AbstractFieldBusDriver> fbdrivers = new Dictionary<int, AbstractFieldBusDriver>();

        /// <summary>
        /// Module이 가질 수 있는 Carrier또는 Material Dispatching을 위한 Dispatcher
        /// </summary>
        private Dispatcher dispatcher = null;

        protected Dispatcher Dispatcher
        {
            get { return dispatcher; }
            set { dispatcher = value; }
        }

        #endregion Attribute

        #region Constructor

        public AbstractModule()
            : base()
        {
        }
        #endregion Constructor

        #region Method

        /// <summary>
        /// Module을 Module에 추가
        /// </summary>
        /// <param name="moduleId">추가할 Module Id</param>
        /// <param name="moduleType">추가할 Module의 Type</param>
        /// <param name="assemblyName">추가할 Module이 있는 Dll명</param>
        /// <param name="className">추가할 모듈의 Class명</param>
        public void AddModule(string moduleId, ModuleType moduleType, string assemblyName, string className)
        {
            const string METHOD = CLASS + ".AddModule";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding module: " + moduleId + ", module type: " + ModuleType.ValueToString(moduleType));

            Module mod = null;

            mod = Module.CreateModule(this.elementId, this.elementType,
                moduleId, moduleType, assemblyName, className);

            this.modules.Add(moduleId, mod);

        }

        /// <summary>
        /// Module을 Module로 부터 삭제
        /// </summary>
        /// <param name="moduleId">삭제할 Module Id</param>
        public void RemoveModule(string moduleId)
        {
            const string METHOD = CLASS + ".RemoveModule";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing module: " + moduleId);

            Module mod = null;

            if (this.modules.ContainsKey(moduleId))
            {
                mod = this.modules[moduleId];
                mod.Delete();
                this.modules.Remove(moduleId);
            }
        }

        /// <summary>
        /// Module이 가지고 있는 Module의 Reference을 Return
        /// </summary>
        /// <param name="moduleId">찾고자하는 Module Id</param>
        /// <returns>발견된 Module Reference, 만약 발견되지 않을 겨우, Null Return</returns>
        public Module GetModule(string moduleId)
        {
            Module mod = null;
            if (this.modules.ContainsKey(moduleId))
            {
                mod = this.modules[moduleId];
            }
            return mod;
        }

        /// <summary>
        ///  Module에 Level 1 Layer와 통신 하기 위한 통신 Driver 추가
        /// </summary>
        /// <param name="driverNo">추가할 Driver의 No</param>
        /// <param name="driverType">추가할 Driver의 Type</param>
        /// <param name="openRetrySec">추가할 Driver가 최초 통신 실패시, Retry할 횟수</param>
        /// <param name="timeOutSec">추가할 Driver의 Comm Timeout</param>
        /// <param name="connectionInfoString">추가할 Driver가 Level 통신 모듈과 접속하기 위한 Connection 정보</param>
        /// <param name="scanInterval">Scan할 Sensor들에 대하나 Scan Interval Time</param>
        /// <param name="useFlag">Dirver의 사용 여부, 사용시, True</param>
        /// <param name="description">Driver에 대한 설명</param>
        /// <param name="assemblyName">Drvier가 위치한 Runtime Library 명</param>
        /// <param name="className">Driver의 Class명</param>
        public void AddFieldBusDriver(
            int driverNo, FieldBusDriverType driverType,
            int openRetrySec, int timeOutSec, string connectionInfoString,
            int scanInterval, bool useFlag, string description,
            string assemblyName, string className)
        {
            const string METHOD = CLASS + ".AddFieldBusDriver";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding fbdriver: " + Convert.ToString(driverNo) + ", driver type: " + 
              FieldBusDriverType.ValueToString(driverType));

            AbstractFieldBusDriver driver = null;

            driver = AbstractFieldBusDriver.CreateAbstractFieldBusDriver(
                    this.elementId, this.elementType,
                    driverNo, driverType,
                    openRetrySec,  timeOutSec,  connectionInfoString,
                    scanInterval,  useFlag,  description,
                    assemblyName,  className );

            this.fbdrivers.Add(driverNo, driver);

        }

        /// <summary>
        /// Drvier을 Module로 부터 삭제
        /// </summary>
        /// <param name="driverNo">삭제하고자 하는 Driver No</param>
        /// <param name="ownerId">삭제할 Driver을 소유하고 있는 소유자명 </param>
        public void RemoveFieldBusDriver(int driverNo, string ownerId)
        {
            const string METHOD = CLASS + ".RemoveFieldBusDriver";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing fbdriver driver no: " + Convert.ToString( driverNo));

            AbstractFieldBusDriver driver = null;

            if (this.fbdrivers.ContainsKey(driverNo))
            {
                driver = this.fbdrivers[driverNo];
                driver.Delete();
                this.fbdrivers.Remove(driverNo);
            }
        }

        /// <summary>
        /// 주어진 No에 해당 하는 Driver Reference을 Return
        /// </summary>
        /// <param name="driverNo">찾고자 하는 Driver 명</param>
        /// <returns>발견된 Driver Reference</returns>
        public AbstractFieldBusDriver GetFieldBusDriver(int driverNo)
        {
            AbstractFieldBusDriver driver = null;
            if (this.fbdrivers.ContainsKey(driverNo))
            {
                driver = this.fbdrivers[driverNo];
            }
            return driver;
        }

        public Dictionary<int, AbstractFieldBusDriver> GetFieldBusDrivers()
        {
            return fbdrivers;
        }

        public void SetFieldBusDrivers(Dictionary<int, AbstractFieldBusDriver> drivers)
        {
            fbdrivers = drivers;
        }

        #endregion Method

    }
}
