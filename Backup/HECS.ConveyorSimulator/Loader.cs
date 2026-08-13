using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;
using System.Data.SqlClient;
using System.Configuration;
using HECS.ConveyorSimulator.Devices;
using ECP.Com.FieldBus;
using EzControl.Driver.MelsecNet;
using System.Threading;
using ECP.Device.Observe;
using ECP.Global.Device;
using HECS.ConveyorSimulator.PLCScenario;
using System.Windows.Forms;
using HECS.Device;

namespace HECS.ConveyorSimulator
{
    public class Loader
    {
        private static Thread observerThread = null;
        private static AbstractFieldBusDriver plcDriver = null;
        private static AbstractFieldBusDriver plcDriverSocket = null;
        private static bool isSimulatorMode = true;

        public static void Startup()
        {
            LoadApplicationSettings();
            LoadEquipment();
            LoadSubSystems();
            LoadLocation();

            ////isSimulatorMode = false; //PLC 적용시 주석해제 하세요
            LoadPLCDriver();
            LoadObservables();
            ChangeObseverbleAccessType();
            RegistObservables();
            //LoadAgv();
            //LoadMarkplate();
            //WritePLCData();
            if (isSimulatorMode == true)
            {
                plcDriver.Open();
            }
            else
            {
                plcDriver.Open(plcDriver.ConnectionInfoString);
            }

            plcDriver.StartScaning();
           
            

            //InitStocker();          //스토커 & 랙마스터 & 포트의 상태를 초기화 한다. 


            
            observerThread = new Thread(new ThreadStart(ObserverMonitor.DoScan));
            observerThread.IsBackground = true;
            observerThread.Start();
          

        }

        private static void LoadMarkplate()
        {
            
            //using (SqlConnection connection = new SqlConnection(Globals.connectionString))
            //{
            //    connection.Open();
            //    string sqlString = "Select  ROUTEID, FROMMARKPLATE, TOMARKPLATE, DISTANCE, ROUTETYPE from    TB_ITINERARYAGV";
            //    SqlCommand command = connection.CreateCommand();
            //    command.CommandText = sqlString;
            //    SqlDataReader reader = command.ExecuteReader();
            //    while (reader.Read())
            //    {
            //        string routeId = Convert.ToString(reader["ROUTEID"]);
            //        string fromMarkplate = Convert.ToString(reader["FROMMARKPLATE"]);
            //        string toMarkplate = Convert.ToString(reader["TOMARKPLATE"]);
            //        double distance = Convert.ToDouble (reader["DISTANCE"]);
            //        string routeType = Convert.ToString(reader["ROUTETYPE"]);
                    
            //        if(DeviceManager.ExistMarkplate(fromMarkplate) == false)
            //        {
            //            MarkPlate markplate = new MarkPlate();
            //            markplate.Markplateid = fromMarkplate;
            //            DeviceManager.Markplates.Add(markplate.Markplateid, markplate);
            //        }
            //        if (DeviceManager.ExistMarkplate(toMarkplate ) == false)
            //        {
            //            MarkPlate markplate = new MarkPlate();
            //            markplate.Markplateid = toMarkplate;
            //            DeviceManager.Markplates.Add(markplate.Markplateid, markplate);
            //        }

            //        AgvItinerary itinerary = new AgvItinerary();
            //        itinerary.RouteId = routeId;
            //        itinerary.FromMarkplate = DeviceManager.GetMarkplate(fromMarkplate);
            //        itinerary.ToMarkplate = DeviceManager.GetMarkplate(toMarkplate);
            //        itinerary.Distance = distance;
            //        DeviceManager.AgvItinerary.Add(itinerary.RouteId, itinerary);
            //    }
            //}
            
        }

        private static void LoadAgv()
        {
            
        }
        private static void RegistObservables()
        {

            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                conveyor.RegisterObservablesToFieldBusDrvier(plcDriver);
            }

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                vehicle.RegisterObservablesToFieldBusDrvier(plcDriver);
            }


        }
        private static void ChangeObseverbleAccessType()
        {
            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                foreach (Observable observer in conveyor.Observables.Values)
                {
                    observer.AccessType = AccessType.IN_OUT;
                    observer.IsMonitoring = true;
                }
            }

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                foreach (Observable observer in vehicle.Observables.Values)
                {
                    observer.AccessType = AccessType.IN_OUT;
                    observer.IsMonitoring = true;
                }
            }
        }
        private static void LoadObservables()
        {
            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                conveyor.LoadObservable(conveyor.DeviceId);
            }

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                vehicle.LoadObservable(vehicle.DeviceId);
            }
        }
        private static void LoadPLCDriver()
        {
            if (isSimulatorMode == true)
            {
                plcDriver = new AbstractFieldBusDriver();
                plcDriver.Load("ECS:1", 1);
            }
            else
            {
                plcDriver = new CMelsecNetDriver();
                plcDriver.Load("ECS:1", 1);
            }
        }

        private static void LoadApplicationSettings()
        {
            ConnectionStringSettings setting = ConfigurationManager.ConnectionStrings["DataBaseConnectionString"];
            if (setting != null)
            {
                Globals.connectionString = setting.ConnectionString;
            }
        }

        private static void LoadEquipment()
        {
            using (SqlConnection connection = new SqlConnection(Globals.connectionString))
            {
                connection.Open();

                string sqlString = "Select EQUIPMENTID, EQUIPMENTTYPE, HOSTID from TB_EQUIPMENT";
                SqlCommand command = connection.CreateCommand();
                command.CommandText = sqlString;
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read())
                {
                    string equipmentId = Convert.ToString(reader["EquipmentId"]);
                    string hostid = Convert.ToString(reader["HostId"]);
                    string equipmentType = Convert.ToString(reader["EquipmentType"]);
                    if (equipmentType != null && equipmentType == "ECS")
                    {
                        continue;
                    }
                    Component component = new Component();
                    component.EquipmentId = equipmentId;
                    component.HostId = hostid;
                    component.DeviceType = DEVICE_TYPE.COMPONENT;
                    DeviceManager.Component.Add(component.EquipmentId, component);

                }
            }
        }
        private static void LoadSubSystems()
        {
            using (SqlConnection connection = new SqlConnection(Globals.connectionString))
            {
                connection.Open();
                foreach (Component equipment in DeviceManager.Component.Values)
                {
                    string sqlString = "Select  SUBSYSTEMID, SUBSYSTEMTYPE, HOSTID,OWNERID from TB_SUBSYSTEM where ownerid='" + equipment.EquipmentId + "'";
                    SqlCommand command = connection.CreateCommand();
                    command.CommandText = sqlString;
                    SqlDataReader reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        string subsystemId = Convert.ToString(reader["SubSystemId"]);
                        string hostid = Convert.ToString(reader["HostId"]);
                        string subsystemType = Convert.ToString(reader["SubsystemType"]);
                        string ownerId = Convert.ToString(reader["OwnerId"]);

                        switch (subsystemType)
                        {
                            case "Conveyor":
                                Conveyor conveyor = new Conveyor();
                                conveyor.DeviceId = subsystemId;
                                conveyor.DeviceType = DEVICE_TYPE.CONVEYOR;
                                conveyor.HostId = hostid;
                                conveyor.Ownerid = ownerId;
                                equipment.Conveyors.Add(conveyor.DeviceId, conveyor);
                                DeviceManager.Conveyors.Add(conveyor.DeviceId, conveyor);
                                break;
                            case "Vehicle":
                                Vehicle vehicle = new Vehicle();
                                vehicle.DeviceId = subsystemId;
                                vehicle.DeviceType = DEVICE_TYPE.VEHICLE;
                                vehicle.HostId = hostid;
                                vehicle.Ownerid = ownerId;
                                equipment.Vehicles.Add(vehicle.DeviceId, vehicle);
                                DeviceManager.Vehicles.Add(vehicle.DeviceId, vehicle);
                                break;
                            case "Rack":
                                Rack rack = new Rack();
                                rack.DeviceId = subsystemId;
                                rack.DeviceType = DEVICE_TYPE.RACK ;
                                rack.HostId = hostid;
                                rack.Ownerid = ownerId;
                                equipment.Racks.Add(rack.DeviceId, rack);
                                DeviceManager.Racks.Add(rack.DeviceId, rack);
                                break;
                        }
                    }
                    reader.Close();
                }
            }
        }

        private static void LoadLocation()
        {
            using (SqlConnection connection = new SqlConnection(Globals.connectionString))
            {
                connection.Open();
                foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
                {
                    string sqlString = "Select   LOCATIONID FROM TB_LOCATION where ownerid='" + conveyor.DeviceId + "'";
                    SqlCommand command = connection.CreateCommand();
                    command.CommandText = sqlString;
                    SqlDataReader reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        string portId = Convert.ToString(reader["LOCATIONID"]);
                        string hostId = Convert.ToString(reader["LOCATIONID"]);
                        string plcId = Convert.ToString(reader["LOCATIONID"]);
                        Port port = new Port();
                        port.DeviceId = portId;
                        port.HostId = hostId;
                        port.PlcId = plcId;
                        port.DeviceType = DEVICE_TYPE.PORT;
                        conveyor.Ports.Add(port.DeviceId, port);
                        if (DeviceManager.Ports.ContainsKey(port.DeviceId) == false)
                        {
                            DeviceManager.Ports.Add(port.DeviceId, port);
                        }
                    }
                    reader.Close();
                }

                foreach (Rack rack in DeviceManager.Racks.Values)
                {
                    string sqlString = "Select   LOCATIONID FROM TB_LOCATION where ownerid='" + rack.DeviceId + "'";
                    SqlCommand command = connection.CreateCommand();
                    command.CommandText = sqlString;
                    SqlDataReader reader = command.ExecuteReader();
                    while (reader.Read())
                    {
                        string shelfId = Convert.ToString(reader["LOCATIONID"]);
                        string hostId = Convert.ToString(reader["LOCATIONID"]);
                        string plcId = Convert.ToString(reader["LOCATIONID"]);
                        Shelf shelf = new Shelf();
                        shelf.DeviceId = shelfId;
                        shelf.HostId = hostId;
                        shelf.PlcId = plcId;
                        shelf.DeviceType = DEVICE_TYPE.SHELF;
                        rack.Shelves.Add(shelf.DeviceId, shelf);
                    }
                    reader.Close();
                }
            }
        }


    }
}
