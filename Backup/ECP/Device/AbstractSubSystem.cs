using System;
using System.Collections.Generic;
using System.Text;

using ECP.Device.Location;
using ECP.Service.Logger;
using ECP.Device;
using ECP.Global.Device;
using ECP.Global.Exception;

namespace ECP.Device
{
    /// <summary>
    /// AbstractEquipmentSubsystem 객체는 AbstractEquipmentElement의 Subtype이며 
    /// Supertype의 모든 속성과 서비스를 상속 받는다. 그리고, Material을 적재하기 위하여 
    /// Material Location과 Association 될 수 있다.
    /// 
    /// AbstractEquipmentSubsystem은 Material을 가질 수도 그렇지 않을 수도 있다. 
    /// Material을 가지는 AbstractEquipmentSubsystem은 어떤 주어진 시점에 Material을 가진다.
    /// 그리고, 그것은 Material의 Type이나, Material이 존재하는지의 여부, 
    /// Material의 인식자(ID)을 알 수 있다. 어떤 경우에는 Material을 여러 개 가질수 있으며,
    /// 한 개의 Type이상도 가질 수 있다
    /// 
    /// 세가지 종류의 중요한 Material 이 있다 : Consumables(소비재), Durables(재 사용 가능한
    /// 재료), Substrate( 낱장 재료 ), Durables의 중요한 Subtype으로는 Carrier와 Process 
    /// Durables이 있다
    /// 
    /// Material Location은 Material을 가질 수 있는 장소이다. Material Location은
    /// Tracking Location과 연관된 물리적인 요소와 관련 없이, Material의 추적이 가능하게 
    /// 하는 Abstraction(추상 개념) 이다. 예를 들어 Substrate 뭉치는 그것의 표면에 한 개의
    /// Substrate을 가질 수 있는 subsystem이다. 전체 Subsystem은 움직일 수 있거나 
    /// 그렇지 않은 Stage로 구성된다. Stage는 Substrate을 가지기 위하여 Vacuum을 사용 할 
    /// 수 있다. 그리고 그것은 추가적인 메커니즘이나, 센서를 가질 수 있다. Material Location의
    /// Location 뭉치의 표면을 나타낸다. 이런 이유로, EquipmentSubSystem은 집합적인 Material의
    /// Location이 아니라. Material Location을 제공 한다고 할 수 있다.
    /// 
    /// Abstract Class인 AbstractSubSystem에 Location에 대한 Container을 제공하는 이유는
    /// Concrete Class 간에는 상속관계를 유지 하지 않으므로, 여기에 Location에 대한 Container를
    /// 제공함으로 Module, Equipment가 Code의 추가 없이, Location Container을 상속하게함에 있다.
    /// 
    /// 
    /// Subsystem 가질 수 있는 Location은 제품(Material)를 적재할 있는 Material Location과
    /// Material를 담고 있는 Carrier를 적재할 수 있는 Carrier Location이 있다.
    /// 
    /// AbstractEquipmentSubsystem은 AbstractEquipmentModule에 의하여 상속되어지며,
    /// EquipmentSubSystem에 의하여 구현되어진다.
    /// 
    /// 또한 다른 SubSystem이나, IO Device을 가질수 있으며, Module이나, Equipment에 
    /// 의하여 Associated될수 있다.
    /// 
    /// User는 장비에 특정 타입의 AbstractEquipmentElement을 추가 하도록 지시 할 수 있다
    /// User는 장비에게 기존에 추가된 AbstractEquipmentElement을 삭제 하도록 지시 할 수 있다
    ///  
    /// </summary>

    [Serializable]
    abstract public class AbstractSubSystem : AbstractElement
    {

        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "AbstractSubSystem";

        /// <summary>
        /// SubSystem내에 Associated되어지는 SubSystem의 Container
        /// </summary>
        public Dictionary<string, SubSystem> subsystems = new Dictionary<string,SubSystem>();

        /// <summary>
        /// SubSystem내에 Associated되어지는 IODevice(Sensor 또는 Actuator)의 Container
        /// </summary>
        public Dictionary<string, IODevice> ioDevices = new Dictionary<string,IODevice>();

        /// <summary>
        /// SubSysem의 Material 놓여 질 수 있는 Location(즉 Wafer Location 또는 Glass Location)
        /// </summary>
        public Dictionary<string, MaterialLocation> locations = new Dictionary<string, MaterialLocation>();

        /// <summary>
        /// SubSysem의 Material 놓여 질 수 있는 Carrier(Durable) Location
        /// </summary>
        public Dictionary<string, CarrierLocation> carrierLocations = new Dictionary<string, CarrierLocation>();


        #region Constructor

        /// <summary>
        /// AbstractSubSystem 생성자
        /// </summary>
        public AbstractSubSystem() :base()
        {
        }
        #endregion Constructor

        #region Manage_SubSystem_Method

        /// <summary>
        /// SubSystem에 SubSystem을 Add함
        /// SubSystem은 SubSystem을 가질 수 있음.
        /// </summary>
        /// <param name="subSystemId">저장할 SubSystem의 Id</param>
        /// <param name="subSystemType">저장할 SubSystem의 Type</param>
        /// <param name="assemblyName">저장할 SubSysem의 Type</param>
        /// <param name="className">저장할 SubSystem의 Type Name</param>
        public void AddSubSystem(string subSystemId, SubSystemType subSystemType, 
            string assemblyName, string className)
        {
            const string METHOD = CLASS + ".AddSubSystem";
            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding subsystem: " + subSystemId + ", subSystem type: " + SubSystemType.ValueToString(subSystemType));

            SubSystem sub = null;
            sub = SubSystem.CreateSubSystem(this.elementId, this.elementType,
                subSystemId, subSystemType, assemblyName, className);

            this.subsystems.Add(subSystemId, sub);

        }

        /// <summary>
        /// SubSystem을  SubSystem으로 부터 제거함.
        /// </summary>
        /// <param name="subSystemId">제거하고자 하는 SubSystme Id</param>
        public void RemoveSubSystem(string subSystemId)
        {
            const string METHOD = CLASS + ".RemoveSubSystem";
            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing subsystem: " + subSystemId);

            SubSystem sub = null;

            if (this.subsystems.ContainsKey(subSystemId))
            {
                sub = this.subsystems[subSystemId];
                sub.Delete();
                this.subsystems.Remove(subSystemId);
            }
        }

        /// <summary>
        /// SubSystem이 가지고 있는 SubSystem의 Reference을 Return
        /// </summary>
        /// <param name="subSystemId">찾고자 하는 SubSystem Id</param>
        /// <returns>발견된 SubSystem Reference</returns>
        public SubSystem GetSubSystem(string subSystemId)
        {
            SubSystem sub = null;
            if (this.subsystems.ContainsKey(subSystemId))
            {
                sub = this.subsystems[subSystemId];
            }            
            return sub;
        }

        #endregion Manage_SubSystem_Method

        #region Manage_Device_Method

        /// <summary>
        /// SubSystem에 IODevice(Sensor, Actuator)를 Add함
        /// </summary>
        /// <param name="ioDeviceId">추가하고자 하는 IODevcie</param>
        public void AddIODevice(string ioDeviceId)
        {
            const string METHOD = CLASS + ".AddIODevice";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding iodevice: " + ioDeviceId);

            IODevice ioDev = null;
            ioDev = IODevice.CreateIODevice(this.elementId, this.elementType, ioDeviceId);

            if (!this.ioDevices.ContainsKey(ioDeviceId))
            {
                this.ioDevices.Add(ioDeviceId, ioDev);
            }
            //else
            //{
            //    throw new ECPException(ECPExceptionType.DUPLICATE, "", "Duplicate ioDeviceId " + ownerId);
            //}

        }

        /// <summary>
        /// SubSystem에서 IODevice을 제거
        /// </summary>
        /// <param name="ioDeviceId">제거하고자 하는 IODevice의 Id</param>
        public void RemoveIODevice(string ioDeviceId)
        {
            const string METHOD = CLASS + ".RemoveIODevice";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing io device: " + ioDeviceId);

            IODevice ioDev = null;

            if (this.ioDevices.ContainsKey(ioDeviceId))
            {
                ioDev = this.ioDevices[ioDeviceId];
                ioDev.Delete();
                this.ioDevices.Remove(ioDeviceId);
            }
        }

        /// <summary>
        /// SubSystem이 가지고 있는 IO Device의 Reference을 Return
        /// </summary>
        /// <param name="ioDeviceId">찾고자하는 IO Device</param>
        /// <returns>발견된 IODevice Reference, 없다면, Null Reference Return</returns>
        public IODevice GetIODevice(string ioDeviceId)
        {
            IODevice ioDev = null;
            if (this.ioDevices.ContainsKey(ioDeviceId))
            {
                ioDev = this.ioDevices[ioDeviceId];
            }
            return ioDev;
        }

        #endregion Manage_Device_Method

        #region Manage_Material_Method

        /// <summary>
        /// SubSystem에 Material Location을 추가
        /// </summary>
        /// <param name="locationId">추가할 Material Location Id</param>
        /// <param name="locationType">추가할 Material Location Type</param>
        /// <param name="logicalState">추가할 Material Location의 논리적인 상태, Offline 또는 Online</param>
        public void AddMaterialLocation(string locationId,
            LocationType locationType, LocationLogicalState logicalState)
        {
            const string METHOD = CLASS + ".AddMaterialLocation";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding material location: " + locationId + ", location type: " +
              LocationType.ValueToString(locationType));

            MaterialLocation location = null;
            location = MaterialLocation.CreateMaterialLocation(
                this.elementId, this.elementType, locationId, locationType, logicalState);

            this.locations.Add(locationId, location);
        }

        /// <summary>
        /// SubSystem에 연속적인 Nubmer을 가진 Material Location을 추가
        /// </summary>
        /// <param name="theNumberOfcreate">추가하고자 하는 Material Loation의 갯수</param>
        /// <param name="locationType">추가하고자 하는 Location Type</param>
        /// <param name="logicalState">추가하고자 하는 Location의 논리적인 상태 Offline, Online</param>
        public void AddMaterialLocations(int theNumberOfcreate, LocationType locationType,
            LocationLogicalState logicalState)
        {
            for (int i = 0; i < theNumberOfcreate; i++)
            {
                this.AddMaterialLocation(i.ToString(), locationType, logicalState);
            }

        }

        /// <summary>
        /// Material Location을 SubSystem에서 삭제
        /// </summary>
        /// <param name="locationId">삭제하고자하는 Location의 Id</param>
        public void RemoveMaterialLocation(string locationId)
        {
            const string METHOD = CLASS + ".RemoveMaterialLocation";
            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing material location: " + locationId);

            MaterialLocation location = null;

            if (this.locations.ContainsKey(locationId))
            {
                location = this.locations[locationId];
                location.Delete();
                this.locations.Remove(locationId);
            }
        }

        /// <summary>
        /// SubSystem에 속한 Material Location의 Reference을 Return
        /// </summary>
        /// <param name="locationId">찾고자하는 Material Location Id</param>
        /// <returns>발견된 Location의 Reference</returns>
        public MaterialLocation GetMaterialLocation(string locationId)
        {
            MaterialLocation location = null;
            if (this.locations.ContainsKey(locationId))
            {
                location = this.locations[locationId];
            }
            return location;
        }

        /// <summary>
        /// SubSystem에 Carrier Location 추가
        /// </summary>
        /// <param name="locationId">추가하고자하는 Carrier Location Id</param>
        /// <param name="logicalState">추가하고자 하는 Carrier Location의 논리적인 상태</param>
        public void AddCarrierLocation(string locationId,
                 LocationLogicalState logicalState)
        {
            const string METHOD = CLASS + ".AddCarrierLocation";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "adding carrier location: " + locationId);

            CarrierLocation location = null;

            location = CarrierLocation.CreateCarrierLocation(
                this.elementId, this.elementType, locationId, logicalState);

            this.locations.Add(locationId, location);
        }

        /// <summary>
        /// SubSystem에서 Carrier Location의 삭제
        /// 
        /// </summary>
        /// <param name="locationId">삭제하고자 하는 Carrier Location Id</param>
        public void RemoveCarrierLocation(string locationId)
        {
            const string METHOD = CLASS + ".RemoveCarrierLocation";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing carrier location: " + locationId);

            CarrierLocation location = null;

            if (this.locations.ContainsKey(locationId))
            {
                location = (CarrierLocation)this.locations[locationId];
                location.Delete();
                this.locations.Remove(locationId);
            }
        }

        /// <summary>
        /// SubSystem의 모든 Carrier Location의 삭제
        /// </summary>
        public void RemoveAllCarrierLocation()
        {
            const string METHOD = CLASS + ".RemoveAllCarrierLocationByElementId";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.elementId, "",
              "removing carrier location all: " + elementId);

            if (this.locations == null) return;
            foreach (CarrierLocation location in locations.Values)
            {
                location.Delete();
            }
            this.locations.Clear();
        }

        /// <summary>
        /// SubSystem의 CarrierLocation의 Reference Return
        /// </summary>
        /// <param name="locationId">찾고자하는 Carrier Location Id</param>
        /// <returns>발견된 Carrier Location Reference</returns>
        public CarrierLocation GetCarrierLocation(string locationId)
        {
            CarrierLocation location = null;
            if (this.locations.ContainsKey(locationId))
            {
                location = (CarrierLocation)this.locations[locationId];
            }
            return location;
        }

        
        /// <summary>
        /// SubSystem 이 Database로 부터 Load되어 Instance화 되어 질때,
        /// 이 SubSystem가지고 있는 모든 Location으로부터, CarrierLocation Type의 
        /// Location을 carrierLocation Container로 Load함
        /// </summary>
        public void LoadCarrierLocations()
        {
            MaterialLocation mLocation;
            foreach (string key in this.locations.Keys)
            {
                mLocation = locations[key];
                if (mLocation.LocationType == LocationType.CarrierLocation)
                {
                    this.carrierLocations.Add(key, (CarrierLocation)mLocation);
                }
            }
        }


        /// <summary>
        /// Carrier를 SubSystem의 Carrier Location에 추가
        /// 
        /// </summary>
        /// <param name="locationId">Carrier를 적재할 Location Id</param>
        /// <param name="carrierId">적재할 Carrier Id</param>
        public void AddCarrier(string locationId, string carrierId)
        {
            CarrierLocation location = this.GetCarrierLocation(locationId);
            location.LocationState = LocationState.OCCUFIED;
            location.AddCarrier(carrierId);
        }

        /// <summary>
        /// Carrier를 SubSystem의 Location으로부터 삭제
        /// </summary>
        /// <param name="locationId">삭제할 Carrier를 가지고 있는 Location Id</param>
        public void RemoveCarrier(string locationId)
        {
            CarrierLocation location = this.GetCarrierLocation(locationId);
            location.LocationState = LocationState.EMPTY;
            location.RemoveCarrier();
        }

        /// <summary>
        /// SubSystem의 Location에 적재된 Carrier Id을 Return
        /// </summary>
        /// <param name="locationId">찾고자 하는 Carrier Id가 놓인 Location Id</param>
        /// <returns>발견된 Carrier Id, 만약 발견하지 못했다면, null Return</returns>
        public string GetCarrier(string locationId)
        {
            CarrierLocation location = this.GetCarrierLocation(locationId);
            if (location == null) return null;

            return location.GetCarrier();
        }
        public string GetCarrier()
        {
            CarrierLocation location = this.GetCarrierLocation("1");
            if (location == null)
            {
                return null;
            }
            return location.GetCarrier();
        }
        /// <summary>
        /// Material을 주어진 LocationId에 해당하는 Location에 추가함
        /// </summary>
        /// <param name="locationId">추가할 Location Id</param>
        /// <param name="materialId">추가하고자 하는 Material Id</param>
        /// <returns></returns>
        public bool AddMaterial(string locationId, string materialId)
        {
            MaterialLocation materialLocation;
            locations.TryGetValue(locationId, out materialLocation);
            if (materialLocation == null)
            {
                return false;
            }
            else
            {
                materialLocation.AddMaterial(materialId);
                return true;
            }
        }

        /// <summary>
        /// Material Id가 "0"인 Location으로 부터, Material을 삭제함
        /// </summary>
        public void RemoveMaterial()
        {
            MaterialLocation location = this.GetMaterialLocation("0");

            location.RemoveMaterial();

        }

        /// <summary>
        /// SubSystem이 가지고 있는 모든 Location이 가지고 있는 모든 Material의 List을 Return.
        /// </summary>
        /// <returns>SubSystem이 가지고 있는 모든 Location이 가지고 있는 모든 Material</returns>
        public string[] GetMaterialList()
        {
            string[] materialList = new string[locations.Keys.Count];
            locations.Keys.CopyTo(materialList, 0);
            return materialList;

        }

        /// <summary>
        /// 주어진 Location Id에 해당하는 Location이 가지고 있는 Material Id을 Return
        /// </summary>
        /// <param name="locationId">Material을 찾고자하는 Location Id</param>
        /// <returns>주어진 Location Id에 해당하는 Location이 가지고 있는 Material Id</returns>
        public string GetMaterial(string locationId)
        {
            MaterialLocation location;
            locations.TryGetValue(locationId, out location);
            if (location == null)
            {
                return "";
            }
            else
            {
                return location.MaterialId;
            }
        }

        /// <summary>
        /// Material Location "0"가 가지고 있는 Material Id을 Return.
        /// </summary>
        /// <returns>Material Location "0"가 가지고 있는 Material Id</returns>
        public string GetMaterial()
        {
            MaterialLocation location;

            location = this.GetMaterialLocation("0");

            return location.MaterialId;
        }


        /// <summary>
        /// 주어진 Location Id에 해당하는 Location으로부터 Material을 제거함.
        /// </summary>
        /// <param name="locationId">제거하고자하는 Material을 가지고 있는 Location</param>
        public void RemoveMaterial(string locationId)
        {
            MaterialLocation location = this.GetMaterialLocation(locationId);

            location.RemoveMaterial();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="materialId"></param>
        /// <returns></returns>
        public bool RemoveMaterialByCarrierID(string materialId)
        {
            foreach (KeyValuePair<string, MaterialLocation> materialLocation in locations)
            {
                if (materialLocation.Value.MaterialId.Equals(materialId))
                {
                    materialLocation.Value.RemoveMaterial();
                    return true;
                }
            }
            return false;
        }


        /// <summary>
        /// SubSystem이 가지고 있는 모든 Material을 Location들로 부터 삭제
        /// </summary>
        public void RemoveAllMaterial()
        {
            foreach (KeyValuePair<string, MaterialLocation> materialLocation in locations)
            {
                materialLocation.Value.RemoveMaterial();
            }
        }


        /// <summary>
        /// SubSystem이 가지 있는 Location의 Count
        /// </summary>
        /// <returns></returns>
        public int GetMaterialLocationSize()
        {
            return locations.Values.Count;
        }

        #endregion Manage_Material_Method

        #region 초기화및기동관련

        /// <summary>
        /// 장비가 Data base로 부터 Load되어지거나, Data base에 추가 된 직후
        /// 초기화 작업 수행함
        /// 
        /// </summary>
        public abstract void Init();

        /// <summary>
        /// 장비가 Start하거나, 통신이 시작된 직후 호출되어지는 Method
        /// </summary>
        public abstract void Startup();

        /// <summary>
        /// 장비가 Stop되거나, 통신이 Stop된 직후 호출되어지는 Method
        /// </summary>
        public abstract void Shutdown();

        /// <summary>
        /// 장비로부터,SubSystem이 Unload 된 직후 호출되어지는 Method
        /// </summary>
        public abstract void Exit();
        #endregion 초기화및기동관련


    }
}
