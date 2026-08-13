using System;
using System.Collections.Generic;
using System.Text;

using ECP.Db;
using ECP.Device;
using ECP.Global.Device;
using ECP.Service.Logger;
using ECP.Interfaces.Unit;


namespace ECP.Service.Device
{
    /// <summary>
    /// Manager관점에서 모든 Device(장비)를 유지하기 위한 서비스를 제공하는 Class
    /// 모든 장비의 생성/삭제/Load등의 서비스를 제공함 
    /// </summary>
    public class DeviceManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "DeviceManager";

        //private bool dirtyFlag = true;

        /// <summary>
        /// Manager가 Create하고 Control하고 있는 모든 Equipment를 담고 있는 Container
        /// </summary>
        protected Dictionary<string, Equipment> devices = new Dictionary<string, Equipment>();

        /// <summary>
        /// Device Manager 생성자
        /// 주어진 Owner Id와 Manager Id로 DeviceManager를 생성한다.
        /// DeviceManager생성시, DeviceManager로 Configration되어진
        /// 모든 Device을 Database로부터, Load한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public DeviceManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.loadDevices();
        }

        /// <summary>
        /// DeviceManager가 생성한 모든 Device을 Database로부터
        /// Load함
        /// </summary>
        public void loadDevices()
        {
            this.devices = Equipment.LoadEquipments();
        }

        /// <summary>
        /// 주어진 DeviceId의 Device을 Database로부터 Load
        /// </summary>
        /// <param name="deviceId">장비 Id</param>
        public void loadDevice(string deviceId)
        {
            Equipment eqp =  Equipment.LoadEquipment(deviceId);

            if (!this.devices.ContainsKey(deviceId))
            {
                this.devices.Add(deviceId, eqp);
            }
        }

        /// <summary>
        /// 주어진 Device Id에 해당하는 장비를 Unload함
        /// </summary>
        /// <param name="deviceId">Device Id</param>
        public void UnloadDevice(string deviceId)
        {
            Equipment eqp = null;
            if(this.devices.ContainsKey(deviceId))
            {
                eqp = this.devices[deviceId];
                eqp.Exit();

                // Let Gabage Collector Handle this
                this.devices.Remove(deviceId);
            }          
        }

        /// <summary>
        /// 장비를 생성함
        /// </summary>
        /// <param name="equipmentId">생성하고자하는 장비 Id</param>
        /// <param name="equipmentType">생성하고자 하는 장비 타입</param>
        /// <param name="assemblyName">생성하고자하는 장비를 담고 있는 Assembley Name</param>
        /// <param name="className">생성하고자하는 장비 Class명</param>
        public void CreateDevice(
            string equipmentId,
            EquipmentType equipmentType, 
            string assemblyName,
            string className)
        {
            const string METHOD = CLASS + ".CreateDevice";

            Log.log( 5, METHOD, "debug", Log.DEBUG, this.ManagerId, "",
              "adding device: " + equipmentId + ", device type: " + EquipmentType.ValueToString(equipmentType));

            Equipment eqp = null;
            eqp = Equipment.CreateEquipment(equipmentId, equipmentType,assemblyName,className);            

            this.devices.Add(equipmentId, eqp);

        }

        /// <summary>
        /// 장비를 Database와 Memory로부터 삭제
        /// </summary>
        /// <param name="equipmentId">삭제할 장비 Id</param>
        public void DeleteDevice(string equipmentId)
        {
            const string METHOD = CLASS + ".DeleteDevice";

            Log.log(5, METHOD, "debug", Log.DEBUG, this.ManagerId, "",
              "removing device: " + equipmentId);

            Equipment eqp = null;

            if (this.devices.ContainsKey(equipmentId))
            {
                eqp = this.devices[equipmentId];
                eqp.Delete();
                this.devices.Remove(equipmentId);
            }
        }


        /// <summary>
        /// 주어진 장비명에 해당하는 장비의 Reference Return
        /// </summary>
        /// <param name="deviceID">Devie를 찾을 수 있는 장비명 ID(HOSTID)</param>
        /// <returns>Device</returns>
        public Equipment GetDevice(string deviceID)
        {
            if (String.IsNullOrEmpty(deviceID))
                return null;

            if (this.devices.ContainsKey(deviceID))
            {
                return this.devices[deviceID];
            }
            foreach (Equipment equipment in this.devices.Values)
            {
                if (string.IsNullOrEmpty(equipment.HostID) == false && equipment.HostID == deviceID) 
                {
                    return equipment ; 
                }
                else if (string.IsNullOrEmpty(equipment.ElementId) == false && equipment.ElementId == deviceID)
                {
                    return equipment ;
                }
            }
            return null;

          
            //foreach (string key in this.devices.Keys)
            //{
            //    if (this.devices[key] is IHostUnit)
            //    {
            //        IHostUnit device = devices[key] as IHostUnit;
            //        if (device.HostID.Equals(deviceID))
            //            return devices[key];
            //    }
            //}

            //return null;
        }

        /// <summary>
        /// 모든 Device를 줌
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, Equipment> GetDeviceAll()
        {
            return devices;
        }
    
        /// <summary>
        /// 주어진 장비명에 해당하는 Devcie가 존재하는지에 대한 검사
        /// </summary>
        /// <param name="deviceId">장비 Id</param>
        /// <returns>장비명이 존재할 경우 true</returns>
        public bool ExistDevice(string deviceId)
        {
            return this.devices.ContainsKey(deviceId);
        }


        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }


        /// <summary>
        /// Data base로 부터 Data를 Load함. 항상 Sub Class에 의하여 Override되어
        /// Implement되어져야 함 항상 base.Load()가 먼저 Call되어져야 함
        /// 
        /// SubClass가 Instance화 된 직후, 이 Method을 Call함으로 Configration되어있는
        /// 기준 정보에 의하여 Sub Object가 초기화 되어짐
        /// 
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection</param>
        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }


        /// <summary>
        /// Data base로 Data를 Save함
        /// Sub Class는 이 Method를 Override하여 Implement하여야 하며,
        /// 항상 base.Save() 먼저 Call되어 상속 계층에 있는 Parent Class가 먼저 
        /// 저장되도록 하여야 함
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }
    }
}
