using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using ECP.Device.Observe;
using ECP.Global.Device;
using ECP.Global;
using HECS.ConveyorSimulator.Devices;

namespace HECS.ConveyorSimulator.PLCScenario
{

    public static class ObserverMonitor
    {
        public static void DoScan()
        {
            ConveyorScenario.Initialize();
            VehicleScenario.Initialize();
//            AGVScenario.Initialize();
            while (true)
            {
                CheckConveyorTrigger();
                CheckVehicleTrigger();
                CheckConveyorState();
                CheckVehicleState();
//                CheckAgvState();
                Thread.Sleep(Globals.Interval);

            }
        }

        private static void CheckAgvState()
        {
            //foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            //{
            //    if (vehicle.Ownerid.Contains("AGVSEM") == false)
            //    {
            //        continue;
            //    }

            //    if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.ARRIVED_AT_DEST)
            //    {
            //        AGVScenario.DoArrivedAtDest(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.ARRIVED_AT_SOURCE)
            //    {
            //        AGVScenario.DoArrivedAtSource(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOAD_COMPLETE)
            //    {
            //        AGVScenario.DoLoadComplete(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOAD_START)
            //    {
            //        AGVScenario.DoLoadStart(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOADING)
            //    {
            //        AGVScenario.DoLoading(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.READY)
            //    {
            //        AGVScenario.DoReady(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.START_TO_DEST)
            //    {
            //        AGVScenario.DoStartDest(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.START_TO_SOURCE)
            //    {
            //        AGVScenario.DoStartToSource(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.TRAVEL_TO_DEST)
            //    {
            //        AGVScenario.DoTravelToDest(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.TRAVEL_TO_SOURCE)
            //    {
            //        AGVScenario.DoTravelToSource(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOAD_COMPLETE)
            //    {
            //        AGVScenario.DoUnloadComplete(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOAD_START)
            //    {
            //        AGVScenario.DoUnloadStart(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOADING)
            //    {
            //        AGVScenario.DoUnloading(vehicle);
            //    }
            //    else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.REQUEST_TRANSFER)
            //    {
            //        AGVScenario.OnTransferRequest(vehicle);
            //    }
            //}
        }



        private static void CheckVehicleTrigger()
        {
            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                foreach (string key in vehicle.Observables.Keys)
                {
                    Observable obs = vehicle.Observables[key];
                    if (obs.PreviousValue == null && obs.Value != null)
                    {
                        obs.PreviousValue = obs.Value;
                        PLCScenario.VehicleScenario.BranchTrigger(vehicle, obs);

                    }
                    else if ((obs.PreviousValue != null) && (obs.PreviousValue != obs.Value))
                    {
                        obs.PreviousValue = obs.Value;
                        PLCScenario.VehicleScenario.BranchTrigger(vehicle, obs);
                    }
                }
            }
        }

        private static void CheckConveyorTrigger()
        {

            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                foreach (string key in conveyor.Observables.Keys)
                {
                    Observable obs = conveyor.Observables[key];
                    if (obs.PreviousValue == null && obs.Value != null)
                    {
                        obs.PreviousValue = obs.Value;
                        PLCScenario.ConveyorScenario.BranchTrigger(conveyor, obs);
                    }
                    else if ((obs.PreviousValue != null) && (obs.PreviousValue != obs.Value))
                    {
                        obs.PreviousValue = obs.Value;
                        PLCScenario.ConveyorScenario.BranchTrigger(conveyor, obs);
                    }
                }
            }
        }

        private static void CheckConveyorState()
        {
            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                foreach (Port port in conveyor.Ports.Values)
                {
                    if (port.PortState == Port.PORT_STATE.LOAD_COMPLETE)
                    {
                        port.StepCount = 0;
                        ConveyorScenario.DoPortLoadComplete(conveyor, port);
                    }
                    else if (port.PortState == Port.PORT_STATE.MOVE_START)
                    {
                        ConveyorScenario.DoPortMoveStart(conveyor, port);
                    }
                    else if (port.PortState == Port.PORT_STATE.MOVING)
                    {
                        ConveyorScenario.DoPortMoving(conveyor, port);
                    }
                    else if (port.PortState == Port.PORT_STATE.MOVE_END)
                    {
                        ConveyorScenario.DoPortMoveEnd(conveyor, port);

                    }
                    else if (port.PortState == Port.PORT_STATE.UNLOAD_COMPLETE)
                    {

                    }
                }
            }
        }

        private static void CheckVehicleState()
        {

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                if (vehicle.Ownerid.Contains("AGVSEM"))
                {
                    continue;
                }

                if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.ARRIVED_AT_DEST)
                {
                    VehicleScenario.DoArrivedAtDest(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.ARRIVED_AT_SOURCE)
                {
                    VehicleScenario.DoArrivedAtSource(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOAD_COMPLETE)
                {
                    VehicleScenario.DoLoadComplete(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOAD_START)
                {
                    VehicleScenario.DoLoadStart(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.LOADING)
                {
                    VehicleScenario.DoLoading(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.READY)
                {
                    VehicleScenario.DoReady(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.START_TO_DEST)
                {
                    VehicleScenario.DoStartDest(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.START_TO_SOURCE)
                {
                    VehicleScenario.DoStartToSource(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.TRAVEL_TO_DEST)
                {
                    VehicleScenario.DoTravelToDest(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.TRAVEL_TO_SOURCE)
                {
                    VehicleScenario.DoTravelToSource(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOAD_COMPLETE)
                {
                    VehicleScenario.DoUnloadComplete(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOAD_START)
                {
                    VehicleScenario.DoUnloadStart(vehicle);
                }
                else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.UNLOADING)
                {
                    VehicleScenario.DoUnloading(vehicle);
                }
            }
        }
    }

}
