using System;
using System.Collections.Generic;
using System.Text;
using HECS.ConveyorSimulator.Devices;
using ECP.Com.Tcp;

namespace HECS.ConveyorSimulator.PLCScenario
{
    public static class AGVScenario
    {

        public static void Initialize()
        {

            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                if (vehicle.Ownerid.Contains("AGVSEM"))
                {
                    vehicle.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.IDLE;
                    vehicle.AgvAlarmCode = "";
                }

            }
        }
        public static void DoArrivedAtDest(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            WMSCommand command = null;
            vehicle.AgvLocation = vehicle.AgvTo;

            MarkPlate markplate = DeviceManager.GetMarkplateByPortId("PORT:" + vehicle.AgvTo);
            if (markplate == null)
            {
                return;
            }


            //command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleArrivedPort_607);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //command.SAPCommandID = vehicle.AgvSapCommandId;
            //command.CommandSequence = vehicle.AgvSapSeq;
            //command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //command.ArrivedLocation = vehicle.AgvLocation.PadLeft(2, ' ');
            //DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOAD_START;
            //vehicle.AgvMarkPlate = markplate.Markplateid;
            //vehicle.AgvTravalDestinationStepCount = 0;
            // Dest 도착 완료 보고 
        }

        public static void DoArrivedAtSource(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;

            //vehicle.AgvLocation = vehicle.AgvFrom;
            //MarkPlate markplate = DeviceManager.GetMarkplateByPortId("PORT:" + vehicle.AgvFrom);
            //if (markplate == null)
            //{
            //    return;
            //}


            //command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleArrivedPort_607);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //command.SAPCommandID = vehicle.AgvSapCommandId;
            //command.CommandSequence = vehicle.AgvSapSeq;
            //command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //command.ArrivedLocation = vehicle.AgvLocation.PadLeft(2, ' ');
            //DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOAD_START;
            //vehicle.AgvMarkPlate = markplate.Markplateid;
            //vehicle.AgvTravelSourceStepCount = 0;

        }

        public static void DoLoadComplete(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;
            //if (vehicle.StepCount < 10)
            //{
            //    vehicle.StepCount++;
            //    return;
            //}

            //command = new WMSCommand(WMSCommand.AGV_COMMAND.PalletLoadComplete_609);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //command.SAPCommandID = vehicle.AgvSapCommandId;
            //command.CommandSequence = vehicle.AgvSapSeq;
            //command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //command.PalletId = vehicle.AgvPalletId.PadLeft(6, ' ');
            //command.LoadLocation = vehicle.AgvLocation.PadLeft(2, ' ');
            //DeviceManager.AgvSEMControl.SendServer(command.BinaryString);


            //vehicle.VehicleState = Vehicle.VEHICLE_STATE.START_TO_DEST;
            //vehicle.StepCount = 0;


            ////Pallet 적재 완료 보고 
        }

        public static void DoLoadStart(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            if (vehicle.StepCount < 10)
            {
                vehicle.StepCount++;
                return;
            }
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOADING;
            vehicle.StepCount = 0;
        }

        public static void DoLoading(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            if (vehicle.StepCount < 10)
            {
                vehicle.StepCount++;
                return;
            }



            //포트의 Pallet 감지 신호를 Off 해야 한다 .
            //반드시 Conveyor랑...

            Port port = DeviceManager.GetPort("PORT:" + (int.Parse(vehicle.AgvLocation).ToString()));
            Conveyor conveyor = DeviceManager.GetConveyorHasPort(port.DeviceId);
            if (port == null || conveyor == null)
            {
                return;
            }
            conveyor.Observables["WAIT_OUT"].Value = false;
            conveyor.Observables["UNLOAD_COMPLETE_02"].Value = false;
            conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
            conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = false;

            vehicle.VehicleState = Vehicle.VEHICLE_STATE.LOAD_COMPLETE;
            vehicle.StepCount = 0;

        }

        public static void DoReady(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {

        }

        public static void DoStartDest(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {

            //WMSCommand command = null;
            //AGVPath toPath = DeviceManager.GetShortestPath(vehicle.AgvMarkPlate, vehicle.AgvToMarkplate.Markplateid);
            //if (toPath == null)
            //{
            //    //NAK 처리하고 
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    return;
            //}
            //vehicle.AgvToPath = toPath.ToString();


            //command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStartPort_611);
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //command.SAPCommandID = vehicle.AgvSapCommandId;
            //command.CommandSequence = vehicle.AgvSapSeq;
            //command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //command.PalletId = vehicle.AgvPalletId.PadLeft(6, ' ');
            //command.LoadLocation = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //DeviceManager.AgvSEMControl.SendServer(command.BinaryString);



            //vehicle.VehicleState = Vehicle.VEHICLE_STATE.TRAVEL_TO_DEST;
            //vehicle.StepCount = 0;
            //vehicle.AgvTravalDestinationStepCount = 0;
            //Dest Port 출발보고
        }

        public static void DoStartToSource(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;
            //if (string.IsNullOrEmpty(vehicle.AgvMarkPlate))
            //{
            //    if (vehicle.DeviceId == "VEHICLE:1")
            //    {
            //        vehicle.AgvMarkPlate = "1";
            //    }
            //    else if (vehicle.DeviceId == "VEHICLE:2")
            //    {
            //        vehicle.AgvMarkPlate = "2";
            //    }
            //    else if (vehicle.DeviceId == "VEHICLE:3")
            //    {
            //        vehicle.AgvMarkPlate = "3";
            //    }
            //    else
            //    {
            //        vehicle.AgvMarkPlate = "4";
            //    }
            //}

            //if (vehicle.VehicleRunState == Vehicle.VEHICLE_RUN_STATE.IDLE)
            //{
            //    vehicle.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.RUN;
            //    //상태변경 보고 
            //    //반송시작보고 
            //    //Vehicle Assign 보고


            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);


            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleAssign_605);
            //    command.Sequence = vehicle.AgvCommandSeq;
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.SAPCommandID = vehicle.AgvSapCommandId;
            //    command.CommandSequence = vehicle.AgvSapSeq;
            //    command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);


            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStartPort_611);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.SAPCommandID = vehicle.AgvSapCommandId;
            //    command.CommandSequence = vehicle.AgvSapSeq;
            //    command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.PalletId = vehicle.AgvPalletId.PadLeft(6, ' ');
            //    command.LoadLocation = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //}


            //AGVPath fromPath = DeviceManager.GetShortestPath(vehicle.AgvMarkPlate, vehicle.AgvFromMarkplate.Markplateid);
            //if (fromPath == null)
            //{
            //    //NAK 처리하고 
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);
            //    return;
            //}
            //vehicle.AgvFromPath = fromPath.ToString();
            //vehicle.VehicleState = Vehicle.VEHICLE_STATE.TRAVEL_TO_SOURCE;
            //vehicle.StepCount = 0;
            //vehicle.AgvTravelSourceStepCount = 0;
            ////Source 포트 출발보고 
            //command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferStart_603);
            //command.Sequence = vehicle.AgvCommandSeq;
            //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //command.SAPCommandID = vehicle.AgvSapCommandId;
            //command.CommandSequence = vehicle.AgvSapSeq;
            //command.TransferStartTime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
            //DeviceManager.AgvSEMControl.SendServer(command.BinaryString);
        }

        public static void DoTravelToDest(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;
            //if (vehicle.StepCount < 10)
            //{
            //    vehicle.StepCount++;
            //    return;
            //}

            //string[] pathList = vehicle.AgvToPath.Split(AGVPath.DEFAULT_PATH_DELIMITER);
            //if (pathList == null || pathList.Length == 0)
            //{
            //    //NAK 보고 
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    vehicle.StepCount = 0;
            //    return;
            //}

            //if (pathList[vehicle.AgvTravalDestinationStepCount] == vehicle.AgvToMarkplate.Markplateid)
            //{
            //    //vehicle.AgvLocation = vehicle.AgvToMarkplate.PortId;
            //    vehicle.AgvLocation = vehicle.AgvTo;
            //    vehicle.AgvMarkPlate = vehicle.AgvToMarkplate.Markplateid;

            //    //620 상태보고 - Markplate 위치가 변경되어서 보고 
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.ARRIVED_AT_DEST;
            //    vehicle.StepCount = 0;
            //    return;
            //}


            //vehicle.AgvTravalDestinationStepCount++;
            //try
            //{
            //    vehicle.AgvMarkPlate = pathList[vehicle.AgvTravalDestinationStepCount];
            //    vehicle.StepCount = 0;

            //    //620 상태보고 - Markplate 위치가 변경되어서 보고 
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //    //LOCATION 이동 보고
            //}
            //catch (Exception)
            //{
            //    vehicle.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.DOWN;
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    vehicle.AgvTravalDestinationStepCount = 0;
            //    //NAK 보고 
            //}



            // 호스트 보고 
        }

        public static void DoTravelToSource(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;
            //if (vehicle.StepCount < 10)
            //{
            //    vehicle.StepCount++;
            //    return;
            //}

            //string[] pathList = vehicle.AgvFromPath.Split(AGVPath.DEFAULT_PATH_DELIMITER);
            //if (pathList == null || pathList.Length == 0)
            //{
            //    //NAK 보고 
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    vehicle.StepCount = 0;
            //    return;
            //}

            //if (pathList[vehicle.AgvTravelSourceStepCount] == vehicle.AgvFromMarkplate.Markplateid)
            //{
            //    vehicle.AgvLocation = vehicle.AgvFrom;
            //    vehicle.AgvMarkPlate = vehicle.AgvFromMarkplate.Markplateid;
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.ARRIVED_AT_SOURCE;
            //    vehicle.StepCount = 0;

            //    //620 상태보고 - Markplate 위치가 변경되어서 보고 
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //    return;
            //}


            //vehicle.AgvTravelSourceStepCount++;
            //try
            //{
            //    vehicle.AgvMarkPlate = pathList[vehicle.AgvTravelSourceStepCount];
            //    vehicle.StepCount = 0;
            //    //620 상태보고 - Markplate 위치가 변경되어서 보고 
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);


            //}
            //catch (Exception)
            //{
            //    vehicle.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.DOWN;
            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    vehicle.AgvTravelSourceStepCount = 0;
            //    //NAK 보고 
            //}



            //// 호스트 보고 
        }

        public static void DoUnloadComplete(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            //WMSCommand command = null;
            //if (vehicle.StepCount < 10)
            //{
            //    vehicle.StepCount++;
            //    return;
            //}
            //if (vehicle.StepCount == 10)
            //{
            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.PalletUnloadComplete_613);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.SAPCommandID = vehicle.AgvSapCommandId;
            //    command.CommandSequence = vehicle.AgvSapSeq;
            //    command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.PalletId = vehicle.AgvPalletId.PadLeft(6, ' ');
            //    command.LoadLocation = vehicle.AgvLocation.PadLeft(2, ' ');
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);



            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleReleased_615);
            //    command.Sequence = vehicle.AgvCommandSeq;
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.SAPCommandID = vehicle.AgvSapCommandId;
            //    command.CommandSequence = vehicle.AgvSapSeq;
            //    command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);


            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferComplete_617);
            //    command.Sequence = vehicle.AgvCommandSeq;
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.SAPCommandID = vehicle.AgvSapCommandId;
            //    command.CommandSequence = vehicle.AgvSapSeq;
            //    command.VehicleId = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.PalletId = vehicle.AgvPalletId.PadLeft(6, ' ');
            //    command.StartLocation = vehicle.AgvFrom.PadLeft(2, ' ');
            //    command.EndLocation = vehicle.AgvTo.PadLeft(2, ' ');
            //    command.CompletedTime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);
            //}

            //if (vehicle.StepCount == 20)
            //{
            //    Port port = DeviceManager.GetPort("PORT:" + (int.Parse(vehicle.AgvLocation).ToString()));
            //    Conveyor conveyor = DeviceManager.GetConveyorHasPort(port.DeviceId);
            //    if (port == null || conveyor == null)
            //    {
            //        return;
            //    }
            //    conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
            //    conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = true;

            //}
            //if (vehicle.StepCount == 25)
            //{



            //    vehicle.AgvSapCommandId = "";
            //    vehicle.AgvSapSeq = "";
            //    vehicle.AgvPriority = "";
            //    vehicle.AgvCommandId = "";
            //    vehicle.AgvCommandSeq = "";
            //    vehicle.AgvFrom = "";
            //    vehicle.AgvTo = "";
            //    vehicle.AgvFromPath = "";
            //    vehicle.AgvToPath = "";
            //    vehicle.AgvFromMarkplate = null;
            //    vehicle.AgvToMarkplate = null;
            //    vehicle.AgvPalletId = "";
            //    vehicle.VehicleRunState = Vehicle.VEHICLE_RUN_STATE.IDLE;


            //    command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
            //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //    command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
            //    command.VehicleIdForState = vehicle.DeviceId.Split(':')[1].PadLeft(2, ' ');
            //    command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
            //    command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
            //    command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
            //    command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
            //    DeviceManager.AgvSEMControl.SendServer(command.BinaryString);

            //    vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            //    vehicle.StepCount = 0;
            //}
            //vehicle.StepCount++;
            ////Pallet 이재(Unloading) 완료 보고
            ////Vehicle 할당 해제 보고 
            ////AGV 반송 완료보고 
            ////Vehicle 상태변경보고 

        }

        public static void DoUnloadStart(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            if (vehicle.StepCount < 10)
            {
                vehicle.StepCount++;
                return;
            }
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOADING;
            vehicle.StepCount = 0;
        }

        public static void DoUnloading(HECS.ConveyorSimulator.Devices.Vehicle vehicle)
        {
            if (vehicle.StepCount < 10)
            {
                vehicle.StepCount++;
                return;
            }
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.UNLOAD_COMPLETE;
            vehicle.StepCount = 0;
        }

        public static void OnTransferRequest(Vehicle vehicle)
        {
            if (string.IsNullOrEmpty(vehicle.AgvFrom))
            {
                //NAK 처리 
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                return;
            }
            if (string.IsNullOrEmpty(vehicle.AgvTo))
            {
                //NAK 처리 
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
            }

            Port fromPort = DeviceManager.GetPort("PORT:" + vehicle.AgvFrom);
            Port toPort = DeviceManager.GetPort("PORT:" + vehicle.AgvTo);
            if (fromPort == null || toPort == null)
            {
                //NAK 처리 
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                return;
            }


            MarkPlate fromMarkplate = DeviceManager.GetMarkplateByPortId("PORT:" + vehicle.AgvFrom);
            MarkPlate toMarkplate = DeviceManager.GetMarkplateByPortId("PORT:" + vehicle.AgvTo);
            if (fromMarkplate == null || toMarkplate == null)
            {
                //NAK 처리 
                vehicle.VehicleState = Vehicle.VEHICLE_STATE.READY;
                return;
            }

            vehicle.AgvFromMarkplate = fromMarkplate;
            vehicle.AgvToMarkplate = toMarkplate;
            vehicle.VehicleState = Vehicle.VEHICLE_STATE.START_TO_SOURCE;
            vehicle.StepCount = 0;

        }
    }
}
