using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Device;
using ECP.Device;
using HECS.Device.Unit;
using ECP.Device.Location;
using System.Data.SqlClient;
using ECP.Db;
using ECP.Global;

namespace HECS.Device
{
    [Serializable]
    public static class ECSDeviceManager
    {
        private static DeviceManager hostDeviceManager = null;
        private static Dictionary<string, SubSystem> subSystems = new Dictionary<string, SubSystem>();
        private static Dictionary<string, Port> ports = new Dictionary<string, Port>();
        private static Dictionary<string, Conveyor> conveyors = new Dictionary<string, Conveyor>();
        private static Dictionary<string, Rack> racks = new Dictionary<string, Rack>();
        private static Dictionary<string, Vehicle> vehicles = new Dictionary<string, Vehicle>();
        private static Dictionary<string, CarrierLocation> locations = new Dictionary<string, CarrierLocation>();
        private static Dictionary<string, StockerSEM> stockerSEMS = new Dictionary<string, StockerSEM>();
        private static Dictionary<string, RgvSEM> rgvSEMS = new Dictionary<string, RgvSEM>();
        private static Dictionary<string, AgvSEM> agvSEMS = new Dictionary<string, AgvSEM>();
        private static Dictionary<string, ConveyorSEM> conveyorSEMS = new Dictionary<string, ConveyorSEM>();
        private static Dictionary<string, LiftSEM> liftSEMS = new Dictionary<string, LiftSEM>();
        private static Dictionary<string, AgvItinerary> agvItinerary = new Dictionary<string, AgvItinerary>();
        private static Dictionary<string, MarkPlate> markplates = new Dictionary<string, MarkPlate>();



        private static object LockObject = new Object();

        public static DeviceManager HostDeviceManager
        {
            get
            {
                return ECSDeviceManager.hostDeviceManager;
            }
            set
            {
                ECSDeviceManager.hostDeviceManager = value;
                CacheObject();

                //LGLS 주석 처리(2009.12.28)
                //LoadMarkplate();
            }
        }
        private static void LoadMarkplate()
        {

            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();

                string sqlString = "Select  ROUTEID, FROMMARKPLATE, TOMARKPLATE, DISTANCE, ROUTETYPE from    TB_ITINERARYAGV";

                SqlDataReader reader = theDB.ExecuteQuery(sqlString);
                while (reader.Read())
                {
                    string routeId = Convert.ToString(reader["ROUTEID"]);
                    string fromMarkplate = Convert.ToString(reader["FROMMARKPLATE"]);
                    string toMarkplate = Convert.ToString(reader["TOMARKPLATE"]);
                    double distance = Convert.ToDouble(reader["DISTANCE"]);
                    string routeType = Convert.ToString(reader["ROUTETYPE"]);

                    if (ECSDeviceManager.ExistMarkplate(fromMarkplate) == false)
                    {
                        MarkPlate markplate = new MarkPlate();
                        markplate.Markplateid = fromMarkplate;
                        ECSDeviceManager.Markplates.Add(markplate.Markplateid, markplate);
                    }
                    if (ECSDeviceManager.ExistMarkplate(toMarkplate) == false)
                    {
                        MarkPlate markplate = new MarkPlate();
                        markplate.Markplateid = toMarkplate;
                        ECSDeviceManager.Markplates.Add(markplate.Markplateid, markplate);
                    }

                    AgvItinerary itinerary = new AgvItinerary();
                    itinerary.RouteId = routeId;
                    itinerary.FromMarkplate = ECSDeviceManager.GetMarkplate(fromMarkplate);
                    itinerary.ToMarkplate = ECSDeviceManager.GetMarkplate(toMarkplate);
                    itinerary.Distance = distance;
                    ECSDeviceManager.AgvItinerary.Add(itinerary.RouteId, itinerary);
                }

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
        public static MarkPlate GetMarkplate(string markplateid)
        {
            if (ExistMarkplate(markplateid))
            {
                return markplates[markplateid];
            }
            return null;
        }

        public static MarkPlate GetMarkplateByPortId(string portid)
        {
            foreach (MarkPlate markplate in markplates.Values)
            {
                if (string.IsNullOrEmpty(markplate.PortId))
                {
                    continue;
                }
                if (markplate.PortId == portid)
                {
                    return markplate;
                }
            }
            return null;
        }
        private static void CacheObject()
        {
            Dictionary<string, Equipment> equipments = hostDeviceManager.GetDeviceAll();
            if (equipments == null)
            {
                return;
            }

            foreach (Equipment equip in equipments.Values)
            {
                if (equip is StockerSEM)
                {
                    StockerSEM stockerSEM = equip as StockerSEM;
                    stockerSEMS.Add(stockerSEM.ElementId, stockerSEM);
                }
                else if (equip is ConveyorSEM)
                {
                    ConveyorSEM conveyorSEM = equip as ConveyorSEM;
                    conveyorSEMS.Add(conveyorSEM.ElementId, conveyorSEM);

                }
                else if (equip is RgvSEM)
                {
                    RgvSEM rgvSEM = equip as RgvSEM;
                    rgvSEMS.Add(rgvSEM.ElementId, rgvSEM);
                }
                else if (equip is AgvSEM)
                {
                    AgvSEM agvSEM = equip as AgvSEM;
                    agvSEMS.Add(agvSEM.ElementId, agvSEM);
                }
                else if (equip is LiftSEM)
                {
                    LiftSEM liftSEM = equip as LiftSEM;
                    liftSEMS.Add(liftSEM.ElementId, liftSEM);
                }
                CacheSubSystem(equip.subsystems);

            }

        }
        public static Dictionary<string, MarkPlate> Markplates
        {
            get
            {
                return ECSDeviceManager.markplates;
            }
            set
            {
                ECSDeviceManager.markplates = value;
            }
        }

        public static bool ExistMarkplate(string markplateid)
        {
            if (markplates.ContainsKey(markplateid))
            {
                return true;
            }
            return false;
        }
        public static Dictionary<string, AgvItinerary> AgvItinerary
        {
            get
            {
                return ECSDeviceManager.agvItinerary;
            }
            set
            {
                ECSDeviceManager.agvItinerary = value;
            }
        }
        public static List<AGVPath> GetRoute(string from, string to)
        {
            if (ECSDeviceManager.ExistMarkplate(from) == false)
            {
                return null;
            }
            if (ECSDeviceManager.ExistMarkplate(to) == false)
            {
                return null;
            }
            List<AGVPath> route = new List<AGVPath>();
            route = FindRoute(from, to, "", route);

            return route;
        }

        public static AGVPath GetShortestPath(List<AGVPath> pathList)
        {
            double maxWeight = double.MaxValue;
            AGVPath choicePath = null;
            foreach (AGVPath path in pathList)
            {
                if (path.Weight <= maxWeight)
                {
                    maxWeight = path.Weight;
                    choicePath = path;
                }
            }
            return choicePath;
        }
        public static AGVPath GetShortestPath(string from, string to)
        {
            List<AGVPath> pathList = GetRoute(from, to);
            if (pathList == null)
            {
                return null;
            }
            return GetShortestPath(pathList);
        }
        private static List<AGVPath> FindRoute(string source, string dest, string path, List<AGVPath> pathResult)
        {
            string foundPath = path;
            if (pathResult == null)
            {
                pathResult = new List<AGVPath>();
            }

            foreach (AgvItinerary itinerary in agvItinerary.Values)
            {
                if (itinerary.FromMarkplate.Markplateid == source)
                {
                    string[] temp = foundPath.Split(AGVPath.DEFAULT_PATH_DELIMITER);
                    if (string.IsNullOrEmpty(foundPath) == false && (temp == null || temp.Length > 0))
                    {
                        for (int i = 0; i < temp.Length; i++)
                        {
                            AgvItinerary tempItinerary = ECSDeviceManager.AgvItinerary[temp[i]];
                            if (tempItinerary.FromMarkplate.Markplateid == source)
                            {
                                return pathResult;
                            }
                        }
                    }


                    //path = path + "|" + itinerary.ToMarkplate.Markplateid;
                    if (itinerary.ToMarkplate.Markplateid == dest)
                    {
                        //System.Console.WriteLine("찾음:" + foundPath + "|" + itinerary.RouteId);
                        AGVPath agvPath = new AGVPath(foundPath + AGVPath.DEFAULT_PATH_DELIMITER + itinerary.RouteId);
                        if (agvPath != null)
                        {
                            pathResult.Add(agvPath);
                        }
                        return pathResult;
                        //break;
                    }
                    if (string.IsNullOrEmpty(foundPath))
                    {
                        FindRoute(itinerary.ToMarkplate.Markplateid, dest, itinerary.RouteId, pathResult);
                    }
                    else
                    {
                        FindRoute(itinerary.ToMarkplate.Markplateid, dest, foundPath + AGVPath.DEFAULT_PATH_DELIMITER + itinerary.RouteId, pathResult);
                    }
                }
            }
            return pathResult;
        }
        private static void CacheLocation(Dictionary<string, MaterialLocation> value)
        {
            foreach (CarrierLocation location in value.Values)
            {
                Port port = new Port();
                port.OwnerId = location.OwnerID;
                port.HostID = location.LocationId;
                port.PlcId = location.LocationId;
                port.ElementId = location.LocationId;
                CachePort(port);
                if (string.IsNullOrEmpty(location.LocationId) || locations.ContainsKey(location.LocationId))
                {
                    continue;
                }
                else
                {
                    locations.Add(location.LocationId, location);
                }

            }

        }


        private static void CacheSubSystem(Dictionary<string, SubSystem> value)
        {
            if (value == null || value.Count == 0)
            {
                return;
            }
            foreach (SubSystem subsystem in value.Values)
            {
                if (subSystems.ContainsKey(subsystem.ElementId))
                {
                    continue;
                }
                subSystems.Add(subsystem.ElementId, subsystem);


                if (subsystem is Port)
                {
                    CachePort(subsystem as Port);
                }
                else if (subsystem is Conveyor)
                {
                    Conveyor conveyor = subsystem as Conveyor;

                    CacheConveyor(conveyor);
                    if (conveyor.ElementId == "CONVEYOR:19")
                    {
                        CacheLocation(conveyor.locations);
                    }
                    else
                    {
                        CacheLocation(conveyor.locations);
                    }
                    CacheSubSystem(conveyor.subsystems);
                }
                else if (subsystem is Rack)
                {
                    CacheRack(subsystem as Rack);
                }
                else if (subsystem is Vehicle)
                {
                    CacheVehicle(subsystem as Vehicle);
                }

            }
        }

        private static void CacheVehicle(Vehicle vehicle)
        {
            if (vehicle == null)
            {
                return;
            }
            if (string.IsNullOrEmpty(vehicle.ElementId))
            {
                return;
            }
            if (vehicles.ContainsKey(vehicle.ElementId))
            {
                return;
            }

            vehicles.Add(vehicle.ElementId, vehicle);

        }

        private static void CacheRack(Rack rack)
        {
            if (rack == null)
            {
                return;
            }
            if (string.IsNullOrEmpty(rack.ElementId))
            {
                return;
            }
            if (racks.ContainsKey(rack.ElementId))
            {
                return;
            }
            racks.Add(rack.ElementId, rack);
        }

        private static void CacheConveyor(Conveyor conveyor)
        {
            if (conveyor == null)
            {
                return;
            }
            if (string.IsNullOrEmpty(conveyor.ElementId))
            {
                return;
            }
            if (conveyors.ContainsKey(conveyor.ElementId))
            {
                return;
            }
            conveyors.Add(conveyor.ElementId, conveyor);
        }

        private static void CachePort(Port port)
        {
            if (port == null)
            {
                return;
            }

            if (string.IsNullOrEmpty(port.ElementId))
            {
                return;
            }

            if (ports.ContainsKey(port.ElementId))
            {
                //프트 객체를 여러 Device에서 참조하는 관계로 
                //생성한 객체 아이디를 사용하는 객체가 있으면 
                //바꿔치기 한다. 
                if (port.OwnerId.Split(':').Length == 2)
                {
                    bool convertResult;
                    int convertValue;
                    convertResult = int.TryParse(port.OwnerId.Split(':')[1], out convertValue);
                    if (convertResult == true)
                    {
                        port.SetDefaultPortOrder();
                        ports[port.ElementId] = port;
                    }
                }
                //return;
            }
            else
            {
                port.SetDefaultPortOrder();
                ports.Add(port.ElementId, port);
            }
            Conveyor conveyor = GetConveyor(port.OwnerId);
            if (conveyor != null)
            {
                if (conveyor.Ports.ContainsKey(port.ElementId) == false)
                {
                    conveyor.Ports.Add(port.ElementId, port);
                }
            }
        }

        public static bool IsExistDevice(string deviceId)
        {
            if (hostDeviceManager == null)
            {
                return false;
            }

            if (subSystems.ContainsKey(deviceId))
            {
                return true;
            }

            foreach (SubSystem subsystem in subSystems.Values)
            {
                if (subsystem is Conveyor)
                {
                    Conveyor conveyor = subsystem as Conveyor;
                    if (string.IsNullOrEmpty(conveyor.ElementId) == false && conveyor.ElementId == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(conveyor.HostID) == false && conveyor.HostID == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(conveyor.PlcId) == false && conveyor.PlcId == deviceId)
                    {
                        return true;
                    }

                }
                else if (subsystem is Vehicle)
                {
                    Vehicle vehicle = subsystem as Vehicle;
                    if (string.IsNullOrEmpty(vehicle.ElementId) == false && vehicle.ElementId == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(vehicle.HostID) == false && vehicle.HostID == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(vehicle.PlcId) == false && vehicle.PlcId == deviceId)
                    {
                        return true;
                    }
                }
                else if (subsystem is Rack)
                {
                    Rack rack = subsystem as Rack;
                    if (string.IsNullOrEmpty(rack.ElementId) == false && rack.ElementId == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(rack.HostID) == false && rack.HostID == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(rack.PlcId) == false && rack.PlcId == deviceId)
                    {
                        return true;
                    }
                }
                else if (subsystem is Port)
                {
                    Port port = subsystem as Port;
                    if (string.IsNullOrEmpty(port.ElementId) == false && port.ElementId == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(port.HostID) == false && port.HostID == deviceId)
                    {
                        return true;
                    }
                    else if (string.IsNullOrEmpty(port.PlcId) == false && port.PlcId == deviceId)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public static bool IsExistPort(string portId)
        {
            if (string.IsNullOrEmpty(portId))
            {
                return true;
            }

            if (ports.ContainsKey(portId))
            {
                return true;
            }

            foreach (Port port in ports.Values)
            {
                if (string.IsNullOrEmpty(port.HostID) == false && port.HostID == portId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(port.PlcId) == false && port.PlcId == portId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(port.ElementId) == false && port.ElementId == portId)
                {
                    return true;
                }
            }
            return false;
        }

        public static Dictionary<string, Conveyor> GetConverByOwnerID(string ownerId)
        {
            Dictionary<string, Conveyor> conveyorDictionary = new Dictionary<string, Conveyor>();
            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (conveyor.OwnerId == ownerId)
                {
                    conveyorDictionary.Add(conveyor.ElementId, conveyor);
                }
            }
            return conveyorDictionary;

        }
        public static Port GetWaitOutPort(string conveyorId)
        {
            if (IsInputConveyor(conveyorId) == true)
            {
                Port port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, "1");
                return port;
            }
            else
            {
                Conveyor conveyor = ECSDeviceManager.GetConveyor(conveyorId);

                if (conveyor == null)
                {
                    return null;
                }

                Port port = null;
                
                //모드 방향이 고정이 아닌 C/V 방향에 따라 Wait out 위치 확인 해줘야 함.
                if (conveyorId == "CONVEYOR:2")
                {
                    if(conveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                    {
                        port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, "1");
                        return port;
                    }
                }

                port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, conveyor.Ports.Count.ToString());
                return port;

            }
        }
        public static Port GetWaitInPort(string conveyorId)
        {
            if (IsInputConveyor(conveyorId) == true)
            {
                Conveyor conveyor = ECSDeviceManager.GetConveyor(conveyorId);
                if (conveyor == null)
                {
                    return null;
                }

                Port port = null;

                //모드 방향이 고정이 아닌 C/V 방향에 따라 Wait out 위치 확인 해줘야 함.
                if (conveyorId == "CONVEYOR:11")
                {
                    if (conveyor.Observables["DIRECTION_MODE"].Value.ToString() == "1")
                    {
                        port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, "1");
                        return port;
                    }
                }
                
                port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, conveyor.Ports.Count.ToString());
                
                return port;
            }
            else
            {
                Port port = ECSDeviceManager.GetPortInConveyorByIndex(conveyorId, "1");
                return port;


            }
        }

        public static bool IsInputConveyor(string conveyorId)
        {
            switch (conveyorId)
            {
                case "CONVEYOR:3":
                case "CONVEYOR:5":
                case "CONVEYOR:7":
                case "CONVEYOR:9":
                case "CONVEYOR:11":
                case "CONVEYOR:12":
                case "CONVEYOR:15":
                    return true;
                default:
                    return false;
            }
        }
        public static Port GetPortInConveyorByIndex(string conveyorId, string portIndex)
        {

            Conveyor conveyor = GetConveyor(conveyorId);

            foreach (Port port in conveyor.Ports.Values)
            {
                if (port.PortOrder == portIndex)
                {
                    return port;
                }
            }
            return null;
        }
        public static Port GetPortInConveyorByIndex(string portIndex)
        {
            foreach (Conveyor conveyor in conveyors.Values)
            {

                foreach (Port port in conveyor.Ports.Values)
                {
                    if (port.PortOrder == portIndex)
                    {
                        return port;
                    }
                }
            }
            return null;
        }


        public static bool IsExistRack(string rackId)
        {
            if (string.IsNullOrEmpty(rackId))
            {
                return false;
            }
            if (racks.ContainsKey(rackId))
            {
                return true;
            }
            foreach (Rack rack in racks.Values)
            {
                if (string.IsNullOrEmpty(rack.HostID) == false && rack.HostID == rackId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(rack.PlcId) == false && rack.PlcId == rackId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(rack.ElementId) == false && rack.ElementId == rackId)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool IsExistVehicle(string vehicleId)
        {
            if (string.IsNullOrEmpty(vehicleId))
            {
                return false;
            }
            if (vehicles.ContainsKey(vehicleId))
            {
                return true;
            }

            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (string.IsNullOrEmpty(vehicle.HostID) == false && vehicle.HostID == vehicleId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(vehicle.PlcId) == false && vehicle.PlcId == vehicleId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(vehicle.ElementId) == false && vehicle.ElementId == vehicleId)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool IsConveyorSEM(string conveyorsemId)
        {
            if (string.IsNullOrEmpty(conveyorsemId))
            {
                return false;
            }
            if (conveyorSEMS.ContainsKey(conveyorsemId))
            {
                return true;
            }
            foreach (ConveyorSEM conveyorSEM in conveyorSEMS.Values)
            {
                if (string.IsNullOrEmpty(conveyorSEM.HostID) == false && conveyorSEM.HostID == conveyorsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(conveyorSEM.PlcId) == false && conveyorSEM.PlcId == conveyorsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(conveyorSEM.ElementId) == false && conveyorSEM.ElementId == conveyorsemId)
                {
                    return true;
                }
            }
            return false;
        }
        public static bool IsExistConveyor(string conveyorId)
        {
            if (string.IsNullOrEmpty(conveyorId))
            {
                return false;
            }
            if (conveyors.ContainsKey(conveyorId))
            {
                return true;
            }

            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (string.IsNullOrEmpty(conveyor.HostID) == false && conveyor.HostID == conveyorId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(conveyor.PlcId) == false && conveyor.PlcId == conveyorId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(conveyor.ElementId) == false && conveyor.ElementId == conveyorId)
                {
                    return true;
                }
            }

            return false;
        }

        public static Port GetPort(string portId)
        {

            if (string.IsNullOrEmpty(portId))
            {
                return null;
            }

            if (ports.ContainsKey(portId))
            {
                return ports[portId];
            }

            foreach (Port port in ports.Values)
            {
                if (string.IsNullOrEmpty(port.HostID) == false && port.HostID == portId)
                {
                    return port;
                }
                else if (string.IsNullOrEmpty(port.PlcId) == false && port.PlcId == portId)
                {
                    return port;
                }
                else if (string.IsNullOrEmpty(port.ElementId) == false && port.ElementId == portId)
                {
                    return port;
                }
            }
            return null;
        }

        public static Rack GetRack(string rackId)
        {
            if (string.IsNullOrEmpty(rackId))
            {
                return null;
            }
            if (racks.ContainsKey(rackId))
            {
                return racks[rackId];
            }
            foreach (Rack rack in racks.Values)
            {
                if (string.IsNullOrEmpty(rack.HostID) == false && rack.HostID == rackId)
                {
                    return rack;
                }
                else if (string.IsNullOrEmpty(rack.PlcId) == false && rack.PlcId == rackId)
                {
                    return rack;
                }
                else if (string.IsNullOrEmpty(rack.ElementId) == false && rack.ElementId == rackId)
                {
                    return rack;
                }
            }
            return null;
        }

        public static Conveyor GetConveyor(string conveyorId)
        {
            if (string.IsNullOrEmpty(conveyorId))
            {
                return null;
            }
            if (conveyors.ContainsKey(conveyorId))
            {
                return conveyors[conveyorId];
            }

            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (string.IsNullOrEmpty(conveyor.HostID) == false && conveyor.HostID == conveyorId)
                {
                    return conveyor;
                }
                else if (string.IsNullOrEmpty(conveyor.PlcId) == false && conveyor.PlcId == conveyorId)
                {
                    return conveyor;
                }
                else if (string.IsNullOrEmpty(conveyor.ElementId) == false && conveyor.ElementId == conveyorId)
                {
                    return conveyor;
                }
            }
            return null;
        }

        public static Vehicle GetVehicle(string vehicleId)
        {
            if (string.IsNullOrEmpty(vehicleId))
            {
                return null;
            }
            if (vehicles.ContainsKey(vehicleId))
            {
                return vehicles[vehicleId];
            }

            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (string.IsNullOrEmpty(vehicle.HostID) == false && vehicle.HostID == vehicleId)
                {
                    return vehicle;
                }
                else if (string.IsNullOrEmpty(vehicle.PlcId) == false && vehicle.PlcId == vehicleId)
                {
                    return vehicle;
                }
                else if (string.IsNullOrEmpty(vehicle.ElementId) == false && vehicle.ElementId == vehicleId)
                {
                    return vehicle;
                }
            }
            return null;
        }

        public static bool IsStockerSEM(string stockersemId)
        {
            if (string.IsNullOrEmpty(stockersemId))
            {
                return false;
            }
            if (stockerSEMS.ContainsKey(stockersemId))
            {
                return true;
            }
            foreach (StockerSEM stockerSEM in stockerSEMS.Values)
            {
                if (string.IsNullOrEmpty(stockerSEM.HostID) == false && stockerSEM.HostID == stockersemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(stockerSEM.PlcId) == false && stockerSEM.PlcId == stockersemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(stockerSEM.ElementId) == false && stockerSEM.ElementId == stockersemId)
                {
                    return true;
                }
            }
            return false;
        }
        public static StockerSEM GetStockerSEM(string stockersemId)
        {
            if (string.IsNullOrEmpty(stockersemId))
            {
                return null;
            }
            if (stockerSEMS.ContainsKey(stockersemId))
            {
                return stockerSEMS[stockersemId];
            }
            foreach (StockerSEM stockerSEM in stockerSEMS.Values)
            {
                if (string.IsNullOrEmpty(stockerSEM.HostID) == false && stockerSEM.HostID == stockersemId)
                {
                    return stockerSEM;
                }
                else if (string.IsNullOrEmpty(stockerSEM.PlcId) == false && stockerSEM.PlcId == stockersemId)
                {
                    return stockerSEM;
                }
                else if (string.IsNullOrEmpty(stockerSEM.ElementId) == false && stockerSEM.ElementId == stockersemId)
                {
                    return stockerSEM;
                }
            }
            return null;
        }
        public static bool IsAVGSEM(string agvsemId)
        {
            if (string.IsNullOrEmpty(agvsemId))
            {
                return false;
            }
            if (agvSEMS.ContainsKey(agvsemId))
            {
                return true;
            }
            foreach (AgvSEM agvSEM in agvSEMS.Values)
            {
                if (string.IsNullOrEmpty(agvSEM.HostID) == false && agvSEM.HostID == agvsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(agvSEM.PlcId) == false && agvSEM.PlcId == agvsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(agvSEM.ElementId) == false && agvSEM.ElementId == agvsemId)
                {
                    return true;
                }
            }
            return false;
        }

        public static AgvSEM GetAGVSEM(string agvsemId)
        {
            if (string.IsNullOrEmpty(agvsemId))
            {
                return null;
            }
            if (agvSEMS.ContainsKey(agvsemId))
            {
                return agvSEMS[agvsemId];
            }
            foreach (AgvSEM agvSEM in agvSEMS.Values)
            {
                if (string.IsNullOrEmpty(agvSEM.HostID) == false && agvSEM.HostID == agvsemId)
                {
                    return agvSEM;
                }
                else if (string.IsNullOrEmpty(agvSEM.PlcId) == false && agvSEM.PlcId == agvsemId)
                {
                    return agvSEM;
                }
                else if (string.IsNullOrEmpty(agvSEM.ElementId) == false && agvSEM.ElementId == agvsemId)
                {
                    return agvSEM;
                }
            }
            return null;
        }
        public static RgvSEM GetRGVSEM(string rgvsemId)
        {
            if (string.IsNullOrEmpty(rgvsemId))
            {
                return null;
            }
            if (rgvSEMS.ContainsKey(rgvsemId))
            {
                return rgvSEMS[rgvsemId];
            }
            foreach (RgvSEM rgvSEM in rgvSEMS.Values)
            {
                if (string.IsNullOrEmpty(rgvSEM.HostID) == false && rgvSEM.HostID == rgvsemId)
                {
                    return rgvSEM;
                }
                else if (string.IsNullOrEmpty(rgvSEM.PlcId) == false && rgvSEM.PlcId == rgvsemId)
                {
                    return rgvSEM;
                }
                else if (string.IsNullOrEmpty(rgvSEM.ElementId) == false && rgvSEM.ElementId == rgvsemId)
                {
                    return rgvSEM;
                }
            }
            return null;
        }
        public static bool IsRGVSEM(string rgvsemId)
        {
            if (string.IsNullOrEmpty(rgvsemId))
            {
                return false;
            }
            if (rgvSEMS.ContainsKey(rgvsemId))
            {
                return true;
            }
            foreach (RgvSEM rgvSEM in rgvSEMS.Values)
            {
                if (string.IsNullOrEmpty(rgvSEM.HostID) == false && rgvSEM.HostID == rgvsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(rgvSEM.PlcId) == false && rgvSEM.PlcId == rgvsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(rgvSEM.ElementId) == false && rgvSEM.ElementId == rgvsemId)
                {
                    return true;
                }
            }
            return false;
        }
        public static bool IsLiftSEM(string liftsemId)
        {
            if (string.IsNullOrEmpty(liftsemId))
            {
                return false;
            }
            if (liftSEMS.ContainsKey(liftsemId))
            {
                return true;
            }
            foreach (LiftSEM liftSEM in liftSEMS.Values)
            {
                if (string.IsNullOrEmpty(liftSEM.HostID) == false && liftSEM.HostID == liftsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(liftSEM.PlcId) == false && liftSEM.PlcId == liftsemId)
                {
                    return true;
                }
                else if (string.IsNullOrEmpty(liftSEM.ElementId) == false && liftSEM.ElementId == liftsemId)
                {
                    return true;
                }
            }
            return false;
        }

        public static LiftSEM GetLiftSEM(string liftsemId)
        {
            if (string.IsNullOrEmpty(liftsemId))
            {
                return null;
            }
            if (liftSEMS.ContainsKey(liftsemId))
            {
                return liftSEMS[liftsemId];
            }
            foreach (LiftSEM liftSEM in liftSEMS.Values)
            {
                if (string.IsNullOrEmpty(liftSEM.HostID) == false && liftSEM.HostID == liftsemId)
                {
                    return liftSEM;
                }
                else if (string.IsNullOrEmpty(liftSEM.PlcId) == false && liftSEM.PlcId == liftsemId)
                {
                    return liftSEM;
                }
                else if (string.IsNullOrEmpty(liftSEM.ElementId) == false && liftSEM.ElementId == liftsemId)
                {
                    return liftSEM;
                }
            }
            return null;
        }

        public static ConveyorSEM GetConveyorSEM(string conveyorsemId)
        {
            if (string.IsNullOrEmpty(conveyorsemId))
            {
                return null;
            }
            if (conveyorSEMS.ContainsKey(conveyorsemId))
            {
                return conveyorSEMS[conveyorsemId];
            }
            foreach (ConveyorSEM conveyorSEM in conveyorSEMS.Values)
            {
                if (string.IsNullOrEmpty(conveyorSEM.HostID) == false && conveyorSEM.HostID == conveyorsemId)
                {
                    return conveyorSEM;
                }
                else if (string.IsNullOrEmpty(conveyorSEM.PlcId) == false && conveyorSEM.PlcId == conveyorsemId)
                {
                    return conveyorSEM;
                }
                else if (string.IsNullOrEmpty(conveyorSEM.ElementId) == false && conveyorSEM.ElementId == conveyorsemId)
                {
                    return conveyorSEM;
                }
            }
            return null;
        }

        public static Dictionary<string, Port> Ports
        {
            get
            {
                return ports;
            }
        }

        public static Dictionary<string, Rack> Racks
        {
            get
            {
                return racks;
            }

        }

        public static Dictionary<string, Conveyor> Conveyors
        {
            get
            {
                return conveyors;
            }

        }

        public static Dictionary<string, Vehicle> Vehicles
        {
            get
            {
                return vehicles;
            }
        }

        public static Dictionary<string, Port> GetPortAllAtConveyor(string conveyorId)
        {
            Dictionary<string, Port> retValues = new Dictionary<string, Port>();
            if (IsExistConveyor(conveyorId) == false)
            {
                return retValues;
            }
            Conveyor conveyor = GetConveyor(conveyorId);
            return conveyor.GetPorts();
        }
        public static Port GetPortAtConveyor(string conveyorId, string index)
        {
            if (IsExistConveyor(conveyorId) == false)
            {
                return null;
            }
            int idx = 0;
            bool isConverted = int.TryParse(index, out idx);
            if (isConverted == false)
            {
                return null;
            }

            Conveyor conveyor = GetConveyor(conveyorId);
            foreach (Port port in conveyor.GetPorts().Values)
            {
                if (string.IsNullOrEmpty(port.PortOrder) == false)
                {
                    int portOrder = 0;
                    isConverted = int.TryParse(port.PortOrder, out portOrder);
                    if (isConverted == true && portOrder == idx)
                    {
                        return port;
                    }
                }
            }
            return null;
        }
        public static Rack[] GetRackAtStockerSEM(string stockersemId)
        {
            return null;
        }

        public static Vehicle[] GetVehicleAtDevice(string deviceId)
        {
            return null;
        }

        public static Vehicle GetVehicleByCommandSequence(string commandId)
        {
            foreach (Vehicle agvVehicle in vehicles.Values)
            {
                if (agvVehicle.OwnerId.Contains("AGV"))
                {
                    if (agvVehicle.AgvBinaryCommandId == commandId)
                    {
                        return agvVehicle;
                    }
                }
            }
            return null;
        }
    }
}
