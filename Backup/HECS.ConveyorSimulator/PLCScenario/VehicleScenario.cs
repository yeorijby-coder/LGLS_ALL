using System;
using System.Collections.Generic;
using System.Text;
using HECS.ConveyorSimulator.Devices;
using ECP.Device.Observe;

namespace HECS.ConveyorSimulator.PLCScenario
{
    public static class VehicleScenario
    {
        /**
        ALARM_RESET_CODE
        ALARM_RESET_REPORT
        ALARM_SET_CODE
        ALARM_SET_REPORT
        LOAD_COMPLETE
        OPERATION_MODE
        PALLET_EXIST_FLAG
        SUBSYSTEM_LOCATION_01
        SUBSYSTEM_LOCATION_02
        SUBSYSTEM_LOCATION_03
        SUBSYSTEM_STATUS
        TRANSFER_ACK
        TRANSFER_COMPLETE_LOCATION_01
        TRANSFER_COMPLETE_LOCATION_02
        TRANSFER_COMPLETE_LOCATION_03
        UNLOAD_COMPLETE
        FROM_01
        FROM_02
        FROM_03
        TO_01
        TO_02
        TO_03
        PALLET_ID
        PALLET_ON_VEHICLE
        TRANSFER_REQUEST
        LOAD_COMPLETE_ACK
        UNLOAD_COMPLETE_ACK
        ALARM_SET_REPORT_ACK
        ALARM_RESET_REPORT_ACK
        */
        public static void BranchTrigger(Vehicle vehicle, Observable obs)
        {
            switch (obs.ObservableId)
            {
                case "ALARM_RESET_CODE":
                    break;
                case "ALARM_RESET_REPORT":
                    break;
                case "ALARM_SET_CODE":
                    break;
                case "ALARM_SET_REPORT":
                    break;
                case "LOAD_COMPLETE":
                    break;
                case "OPERATION_MODE":
                    break;
                case "UNLOAD_COMPLETE":
                    break;
                case "PALLET_ID":
                    break;
                case "PALLET_ON_VEHICLE":
                    break;
                case "TRANSFER_REQUEST":
                    OnTransferRequest(vehicle, obs);
                    break;
                case "TRANSFER_ACK":
                    OnTransferRequestACK(vehicle, obs);
                    break;
                case "LOAD_COMPLETE_ACK":
                    OnLoadCompleteACK(vehicle, obs);
                    break;
                case "UNLOAD_COMPLETE_ACK":
                    OnUnloadCompleteACK(vehicle, obs);
                    break; 
            }
        }














        /* -----------------------------------------------------------------------
         * 여기서부터 제어 시나리오가 들어갑니다.
         *-----------------------------------------------------------------------*/
        private static void OnTransferRequest(Vehicle vehicle, Observable obs)
        {
            if ((bool)obs.Value == false)
            {
                return;
            }

            //SOURCE & DEST에 대한 Vailidation을 해야한다.
            //vehicle이 DOWN 상태에서 반송이 지시되었으면, 무시한다. 

            vehicle.VehicleState = Vehicle.VEHICLE_STATE.START_TO_SOURCE;
            vehicle.StepCount = 0;


            //vehicle이 다른 반송을 수행하고 있는 도중 반송이 지시되면 DOWN 되고 
            //반송을 Clear 한다.
        }
        private static void OnTransferRequestACK(Vehicle vehicle, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                vehicle.Observables["TRANSFER_REQUEST"].Value = false;
                vehicle.Observables["TRANSFER_ACK"].Value = false;
            }
        }
        private static void OnUnloadCompleteACK(Vehicle vehicle, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                vehicle.Observables["UNLOAD_COMPLETE"].Value = false;
                vehicle.Observables["UNLOAD_COMPLETE_ACK"].Value = false;
            }
        }
        private static void OnLoadCompleteACK(Vehicle vehicle, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                vehicle.Observables["LOAD_COMPLETE"].Value = false;
                vehicle.Observables["LOAD_COMPLETE_ACK"].Value = false;
            }
        }
        public static void DoStartToSource(Vehicle vehicle)
        {
            //Source 위치가 유효한지 확인한다.
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.TRAVEL_TO_SOURCE;
            vehicle.StepCount = 0;
            vehicle.Observables["TRANSFER_REQUEST"].Value = false;
            vehicle.Observables["SUBSYSTEM_STATUS"].Value = 2;

        }

        public static void DoUnloading(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 20)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOAD_COMPLETE;

            }
        }

        public static void DoUnloadStart(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 5)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOADING;

            }
        }

        public static void DoUnloadComplete(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 5)
            {
                string to1 = (string)vehicle.Observables["TO_01"].Value;
                string to2 = (string)vehicle.Observables["TO_02"].Value;
                string to3 = (string)vehicle.Observables["TO_03"].Value;


                if (to1 == "00" && to2 == "00")          //Source가 HIRACK
                {
                    Port port = DeviceManager.GetPort("PORT:" + (int.Parse(to3)).ToString());
                    if (port != null)
                    {
                        Conveyor conveyor = DeviceManager.GetConveyorHasPort(port.DeviceId);
                        conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = true;
                        conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
                    }
                    vehicle.Observables["SUBSYSTEM_STATUS"].Value = 1;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_01"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_01"].Value;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_02"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_02"].Value;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_03"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_03"].Value;
                    vehicle.Observables["PALLET_EXIST_FLAG"].Value = false;
                    vehicle.Observables["PALLET_ON_VEHICLE"].Value = "";
                    vehicle.Observables["UNLOAD_COMPLETE"].Value = true;
                    vehicle.StepCount = 0;
                    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                }
                else
                {
                    vehicle.Observables["SUBSYSTEM_STATUS"].Value = 1;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_01"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_01"].Value;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_02"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_02"].Value;
                    vehicle.Observables["TRANSFER_COMPLETE_LOCATION_03"].Value = vehicle.Observables["SUBSYSTEM_LOCATION_03"].Value;
                    vehicle.Observables["PALLET_EXIST_FLAG"].Value = false;
                    vehicle.Observables["PALLET_ON_VEHICLE"].Value = "";
                    vehicle.Observables["UNLOAD_COMPLETE"].Value = true;
                    vehicle.StepCount = 0;
                    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                    // file Register 영역(Location)의 값을 삭제해야 한다.
                }
            }
        }

        public static void DoTravelToSource(Vehicle vehicle)
        {
            //현재 위치로 부터 Source위치가찌 이동을 시작한다.
            //StepCount가 매 20회가 될때마다 한번씩 체크해서 옮겨준다.

            vehicle.StepCount++;
            if (vehicle.StepCount > 40)
            {
                vehicle.Observables["SUBSYSTEM_LOCATION_01"].Value = vehicle.Observables["FROM_01"].Value;
                vehicle.Observables["SUBSYSTEM_LOCATION_02"].Value = vehicle.Observables["FROM_02"].Value;
                vehicle.Observables["SUBSYSTEM_LOCATION_03"].Value = vehicle.Observables["FROM_03"].Value;
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.ARRIVED_AT_SOURCE;
            }
        }

        public static void DoTravelToDest(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 40)
            {
                vehicle.Observables["SUBSYSTEM_LOCATION_01"].Value = vehicle.Observables["TO_01"].Value;
                vehicle.Observables["SUBSYSTEM_LOCATION_02"].Value = vehicle.Observables["TO_02"].Value;
                vehicle.Observables["SUBSYSTEM_LOCATION_03"].Value = vehicle.Observables["TO_03"].Value;
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.ARRIVED_AT_DEST;
            }

        }

        public static void DoReady(Vehicle vehicle)
        {
        }

        public static void DoLoading(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 20)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOAD_COMPLETE;

            }
        }

        public static void DoLoadStart(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 5)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOADING;

            }
        }

        public static void DoLoadComplete(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 5)
            {
                string from1 = (string)vehicle.Observables["FROM_01"].Value;
                string from2 = (string)vehicle.Observables["FROM_02"].Value;
                string from3 = (string)vehicle.Observables["FROM_03"].Value;

                vehicle.Observables["PALLET_EXIST_FLAG"].Value = true;
                vehicle.Observables["PALLET_ON_VEHICLE"].Value = vehicle.Observables["PALLET_ID"].Value;
                vehicle.Observables["LOAD_COMPLETE"].Value = true;
                if (from1 == "00" && from2 == "00")          //Source가 HIRACK
                {
                    Port port = DeviceManager.GetPort("PORT:" + (int.Parse(from3)).ToString());
                    if (port == null)
                    {

                        vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                        vehicle.Observables["PALLET_EXIST_FLAG"].Value = false;
                        vehicle.Observables["PALLET_ON_VEHICLE"].Value = "";
                    }
                    else
                    {
                        Conveyor conveyor = DeviceManager.GetConveyorHasPort(port.DeviceId);
                        conveyor.Observables["WAIT_OUT"].Value = false;
                        conveyor.Observables["UNLOAD_COMPLETE_02"].Value = false;
                        conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
                        conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = false;
                    }
                    vehicle.StepCount = 0;
                    vehicle.VehicleState = Vehicle.VEHICLE_STATE.START_TO_DEST;
                }
                else
                {
                    vehicle.StepCount = 0;
                    vehicle.VehicleState = Vehicle.VEHICLE_STATE.START_TO_DEST;
                    // file Register 영역(Location)의 값을 삭제해야 한다.
                }
            }
        }

        public static void DoArrivedAtSource(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 3)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOAD_START;
            }


        }

        public static void DoArrivedAtDest(Vehicle vehicle)
        {
            vehicle.StepCount++;
            if (vehicle.StepCount > 3)
            {
                vehicle.StepCount = 0;
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOAD_START;
            }
        }

        public static void DoStartDest(Vehicle vehicle)
        {
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.TRAVEL_TO_DEST;
            vehicle.StepCount = 0;
        }

         public static void Initialize()
        {

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                //Operation Mode 초기화
                WriteObservable(vehicle, "SUBSYSTEM_STATUS", 1);
                WriteObservable(vehicle, "OPERATION_MODE", 1);
            }
        }
        private static void WriteObservable(Vehicle vehicle, string key, object value)
        {
            if (vehicle.Observables.ContainsKey(key))
            {
                vehicle.Observables[key].SetValue(value);
            }
        }
    }
}
