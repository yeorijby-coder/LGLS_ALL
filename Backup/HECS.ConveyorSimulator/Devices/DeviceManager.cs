using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;
using System.Text.RegularExpressions;
using HECS.ConveyorSimulator.Controls;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public static class DeviceManager
    {
        private static AGVSEMControl agvSEMControl = null;

        public static AGVSEMControl AgvSEMControl
        {
            get
            {
                return agvSEMControl;
            }
            set
            {
                agvSEMControl = value;
            }
        } 
        private static Dictionary<string, Port> ports = new Dictionary<string, Port>();

        public static Dictionary<string, Port> Ports
        {
            get
            {
                return DeviceManager.ports;
            }
            set
            {
                DeviceManager.ports = value;
            }
        }
        private static Dictionary<string, Conveyor> conveyors = new Dictionary<string, Conveyor>();

        public static Dictionary<string, Conveyor> Conveyors
        {
            get
            {
                return DeviceManager.conveyors;
            }
            set
            {
                DeviceManager.conveyors = value;
            }
        }

        private static Dictionary<string, MarkPlate> markplates = new Dictionary<string, MarkPlate>();

        public static Dictionary<string, MarkPlate> Markplates
        {
            get
            {
                return DeviceManager.markplates;
            }
            set
            {
                DeviceManager.markplates = value;
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
        public static bool PortHasMarkplate(string portid)
        {
            foreach (MarkPlate markplate in markplates.Values)
            {
                if (string.IsNullOrEmpty(markplate.PortId))
                {
                    continue;
                }
                if (markplate.PortId == portid)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool ExistConveyor(string conveyorId)
        {
            if (conveyors.ContainsKey(conveyorId))
            {
                return true;
            }
            return false;
        }


        public static Conveyor GetConveyor(string conveyorId)
        {
            if (ExistConveyor(conveyorId))
            {
                return conveyors[conveyorId];
            }
            return null;
        }



        public static Dictionary<string, Conveyor> GetConverByOwnerID(string ownerId)
        {
            Dictionary<string, Conveyor> conveyorDictionary = new Dictionary<string, Conveyor>();
            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (conveyor.Ownerid == ownerId)
                {
                    conveyorDictionary.Add(conveyor.DeviceId, conveyor);
                }
            }
            return conveyorDictionary;

        }
        public static bool ExistPort(string portId)
        {
            if (ports.ContainsKey(portId))
            {
                return true;
            }
            return false;
        }

        public static Port GetPort(string portId)
        {
            if (ExistPort(portId))
            {
                return ports[portId];
            }
            return null;
        }

        public static Port GetPortInConveyorByIndex(string conveyorId, string portIndex)
        {
            if (ExistConveyor(conveyorId) == false)
            {
                return null;
            }

            Conveyor conveyor = GetConveyor(conveyorId);

            foreach (Port port in conveyor.Ports.Values)
            {
                if (port.ObjectOrder == portIndex)
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
                    if (port.ObjectOrder == portIndex)
                    {
                        return port;
                    }
                }
            }
            return null;
        }
        public static bool ExistLift(string liftId)
        {
            if (lifts.ContainsKey(liftId))
            {
                return true;
            }
            return false;
        }

        public static Lift GetLift(string liftId)
        {
            if (ExistLift(liftId))
            {
                return lifts[liftId];
            }
            return null;
        }

        public static bool ExistRack(string rackId)
        {
            if (racks.ContainsKey(rackId))
            {
                return true;
            }
            return false;
        }
        public static Rack GetRack(string rackId)
        {
            if (ExistRack(rackId))
            {
                return racks[rackId];
            }
            return null;
        }

        public static bool ExistVehicle(string vehicleId)
        {
            if (vehicles.ContainsKey(vehicleId))
            {
                return true;
            }
            return false;
        }

        public static Vehicle GetVehicle(string vehicleId)
        {
            if (ExistVehicle(vehicleId))
            {
                return vehicles[vehicleId];
            }
            return null;
        }


        public static bool ExistDevice(string deviceId)
        {
            if (ExistConveyor(deviceId))
            {
                return true;
            }
            else if (ExistVehicle(deviceId))
            {
                return true;
            }
            else if (ExistRack(deviceId))
            {
                return true;
            }
            else if (ExistPort(deviceId))
            {
                return true;
            }
            else if (ExistLift(deviceId))
            {
                return true;
            }
            return false;
        }

        public static DEVICE_TYPE TellDeviceType(string deviceId)
        {
            if (ExistConveyor(deviceId))
            {
                return DEVICE_TYPE.CONVEYOR;
            }
            else if (ExistVehicle(deviceId))
            {
                return DEVICE_TYPE.VEHICLE;
            }
            else if (ExistRack(deviceId))
            {
                return DEVICE_TYPE.RACK;
            }
            else if (ExistPort(deviceId))
            {
                return DEVICE_TYPE.PORT;
            }
            else if (ExistLift(deviceId))
            {
                return DEVICE_TYPE.LIFT;
            }
            return DEVICE_TYPE.UNKNOWN;
        }

        private static Dictionary<string, AgvItinerary> agvItinerary = new Dictionary<string, AgvItinerary>();

        public static Dictionary<string, AgvItinerary> AgvItinerary
        {
            get
            {
                return DeviceManager.agvItinerary;
            }
            set
            {
                DeviceManager.agvItinerary = value;
            }
        }
        public static List<AGVPath> GetRoute(string from, string to)
        {
            if (DeviceManager.ExistMarkplate(from) == false)
            {
                return null;
            }
            if (DeviceManager.ExistMarkplate(to) == false)
            {
                return null;
            }
            List<AGVPath> route = new List<AGVPath>();
            route = FindRoute(from, to, "", route);
            //for (int i = 0; i < route.Count; i++)
            //{
            //    System.Console.WriteLine("가중치:" + route[i].Weight + "//"+route[i].ToString());
            //}
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
                            AgvItinerary tempItinerary = DeviceManager.AgvItinerary[temp[i]];
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

        //private static string FindRoute(string source, string dest, string path)
        //{
        //    string foundPath = path;
        //    foreach (AgvItinerary itinerary in agvItinerary.Values)
        //    {
        //        if (itinerary.FromMarkplate.Markplateid == source)
        //        {
        //            string[] temp = foundPath.Split('|');
        //            if (temp == null || temp.Length > 0)
        //            {
        //                for (int i = 0; i < temp.Length - 1; i++)
        //                {
        //                    if (temp[i] == source)
        //                    {
        //                        return foundPath;
        //                    }
        //                }
        //            }


        //            //path = path + "|" + itinerary.ToMarkplate.Markplateid;
        //            if (itinerary.ToMarkplate.Markplateid == dest)
        //            {
        //                System.Console.WriteLine("찾음:" + foundPath);
        //                return path;
        //                //break;
        //            }
        //            FindRoute(itinerary.ToMarkplate.Markplateid, dest, foundPath + "|" + itinerary.ToMarkplate.Markplateid);
        //        }
        //    }
        //    return path;
        //}

        private static Dictionary<string, Lift> lifts = new Dictionary<string, Lift>();
        public static Dictionary<string, Lift> Lifts
        {
            get
            {
                return DeviceManager.lifts;
            }
            set
            {
                DeviceManager.lifts = value;
            }
        }
        private static Dictionary<string, Vehicle> vehicles = new Dictionary<string, Vehicle>();

        public static Dictionary<string, Vehicle> Vehicles
        {
            get
            {
                return DeviceManager.vehicles;
            }
            set
            {
                DeviceManager.vehicles = value;
            }
        }
        private static Dictionary<string, Stocker> stockers = new Dictionary<string, Stocker>();

        public static Dictionary<string, Stocker> Stockers
        {
            get
            {
                return DeviceManager.stockers;
            }
            set
            {
                DeviceManager.stockers = value;
            }
        }

        private static Dictionary<string, Rack> racks = new Dictionary<string, Rack>();

        public static Dictionary<string, Rack> Racks
        {
            get
            {
                return DeviceManager.racks;
            }
            set
            {
                DeviceManager.racks = value;
            }
        }

        private static Dictionary<string, Component> component = new Dictionary<string, Component>();

        public static Dictionary<string, Component> Component
        {
            get
            {
                return DeviceManager.component;
            }
            set
            {
                DeviceManager.component = value;
            }
        }

        private static void SetPortOwner(Port port)
        {
            foreach (Conveyor conveyor in conveyors.Values)
            {
                if (conveyor.Ports.ContainsKey(port.DeviceId))
                {
                    port.Ownerid = conveyor.DeviceId;   
                    return;
                }
            }

        }
        public static Conveyor GetConveyorHasPort(string portId)
        {
            if (ports.ContainsKey(portId) == false)
            {
                return null;
            }
            Port port = ports[portId];

            if (string.IsNullOrEmpty(port.Ownerid))
            {
                SetPortOwner(port);
            }
            string ownerId = port.Ownerid;
            if (port.Ownerid.EndsWith("R") || port.Ownerid.EndsWith("L") ||  port.Ownerid.EndsWith("S") || port.Ownerid.EndsWith("A"))
            {
                ownerId = ownerId.Substring(0, ownerId.Length - 1);
            }

            if (string.IsNullOrEmpty(ownerId))
            {
                return null;
            }
            if (conveyors.ContainsKey(ownerId))
            {
                return conveyors[ownerId];
            }
            return null;
        }

    }
}
