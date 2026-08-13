using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using ECP.Service.Movement;
using ECP.Util.Server;
using HECS.Device.Unit;
using ECP.Db;
using ECP.Com.Tcp;
using System.Data.SqlClient;
using System.Windows.Forms;
using ECP.BizRule;
using ECP.Global;

namespace HECS.Device.Dispatcher
{
    public static class ECSDispatcher
    {

        public static TransferData GetTransferDataByPalletId(string palletid)
        {
            if (Naming.GetMovementManager() == null)
            {
                return null;
            }
            Dictionary<string, TransferData> transfers = MovementManager.getTransferDataList();
            MovementManager manager = Naming.GetMovementManager();
            foreach (TransferData transfer in transfers.Values)
            {
                if (transfer.Palletid == palletid)
                {
                    return transfer;
                }
            }
            return null;
        }

        public static TransferDetailData GetTransferDetailByPalletId(string commandId, string palletId)
        {
            if (Naming.GetMovementManager() == null)
            {
                return null;
            }
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();
            transferDetails = manager.getTransferDetailDataList(commandId);
            foreach (TransferDetailData transferDetail in transferDetails.Values)
            {
                if (string.IsNullOrEmpty(transferDetail.Palletid))
                {
                    continue;
                }
                if (transferDetail.Palletid == palletId)
                {
                    return transferDetail;
                }
            }
            return null;
        }

        public static bool IsExecutableTransfer(TransferData transfer)
        {
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();
            transferDetails = manager.getTransferDetailDataList(transfer.Commandid);
            if (transferDetails == null || transferDetails.Count == 0)
            {
                return false;
            }
            foreach (TransferDetailData transferDetail in transferDetails.Values)
            {
                if (transferDetail.Transferdetailstatus != "Wait")
                {
                    return true;
                }
            }
            return true;
        }
        public static bool IsExecuteTransferDetail(TransferDetailData transferDetailData)
        {
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();
            transferDetails = manager.getTransferDetailDataList(transferDetailData.Commandid);
            if (transferDetails == null || transferDetails.Count == 0)
            {
                return false;
            }
            if (int.Parse(transferDetailData.Commandseq) == 1 && transferDetailData.Transferdetailstatus == "Wait")
            {
                return true;
            }

            foreach (TransferDetailData transferDetail in transferDetails.Values)
            {
                if (int.Parse(transferDetail.Commandseq) == int.Parse(transferDetailData.Commandseq) - 1)
                {
                    return false;
                }
            }
            return true;
        }
        public static bool IsTransferComplete(TransferData transfer)
        {
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();
            transferDetails = manager.getTransferDetailDataList(transfer.Commandid);
            if (transferDetails == null || transferDetails.Count == 0)
            {
                return true;
            }
            return false;
        }

        public static bool IsFirstTransferDetail(TransferDetailData transferDetail)
        {

            if (int.Parse(transferDetail.Commandseq) == 1 && transferDetail.Transferdetailstatus == "Wait" && string.IsNullOrEmpty(transferDetail.Palletidtomove))
            {
                return true;
            }
            return false;
        }


        public static TransferDetailData GetNextTransferDetail(string commandId, string currentDetailSeq)
        {
            if (Naming.GetMovementManager() == null)
            {
                return null;
            }
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();
            transferDetails = manager.getTransferDetailDataList(commandId);
            foreach (TransferDetailData transferDetail in transferDetails.Values)
            {
                if (int.Parse(transferDetail.Commandseq) == int.Parse(currentDetailSeq) + 1)
                {
                    return transferDetail;
                }
            }
            return null;
        }
        public static void DoWork()
        {

            if (ECP.Global.GlobalConstant.START_MODE != "ECS")
            {
                return;
            }

            try
            {
                CheckEquipmentStatus();
            }
            catch (Exception ex)
            {
                //
            }

            //Conveyor:11의 모드 변경
            try
            {
                Conveyor cnv = ECSDeviceManager.GetConveyor(GlobalConstant.DUAL_MODE_TO_LOCATION);
                if (cnv != null)
                {
                    if (cnv.Observables["DIRECTION_MODE"].Value.ToString() != GlobalConstant.CONVEYOR11_MODE)
                    {
                        cnv.Observables["DIRECTION_MODE"].AsString = GlobalConstant.CONVEYOR11_MODE;

                        if (GlobalConstant.CONVEYOR11_MODE == GlobalConstant.RGV_INGO)
                        {
                            cnv.DirectionMode = Conveyor.CONVEYOR_DIRECTION_MODE.RGVINGO;
                        }
                        else
                        {
                            cnv.DirectionMode = Conveyor.CONVEYOR_DIRECTION_MODE.RGVOUTGO;
                        }
                    }
                }
            }
            catch (Exception exe)
            {
                //
            }


            Dictionary<string, TransferData> transfers = MovementManager.getTransferDataPriorityList();
            if (transfers == null || transfers.Count == 0)
            {
                return;
            }
            DoWorkBoost(transfers);
            DoWorkOtherTransfers(transfers);

        }
        private static void DoWorkBoost(Dictionary<string, TransferData> transfers)
        {
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();

            Dictionary<string, Vehicle.VEHICLE_STATE> prevVehicleStatus = new Dictionary<string, Vehicle.VEHICLE_STATE>();
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                prevVehicleStatus.Add(vehicle.ElementId, vehicle.VehicleState);
            }

            foreach (TransferData transfer in transfers.Values)
            {

                //Holding
                if (transfer.IsBoost == false)
                {
                    continue;
                }
                //Vehicle의 상태가 변경되면 다음 Turn에 반송을 진행한다.
                //우선순위대로 반송을 진행하기 위해서이다.
                foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
                {
                    if (prevVehicleStatus[vehicle.ElementId] != vehicle.VehicleState)
                    {
                        return;
                    }
                }

                transferDetails = manager.getTransferDetailDataList(transfer.Commandid);
                foreach (TransferDetailData transferDetail in transferDetails.Values)
                {

                    if (IsFirstTransferDetail(transferDetail) == true)
                    {
                        transferDetail.Palletidtomove = transfer.Palletid;
                        transferDetail.updateTransferDetailData();

                        //HISTORY Backup LGLS를 위해 추가함.
                        try
                        {
                            transferDetail.insertTransferDetailDataBackup();
                        }
                        catch (Exception ex)
                        {
                            //throw ex;
                        }
                    }
                    if (IsExecuteTransferDetail(transferDetail) == false)
                    {
                        continue;
                    }
                    if (transferDetail.Transferdetailstatus == "Wait")
                    {
                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (ECSDeviceManager.IsConveyorSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsRGVSEM(itinerary.Fromdeviceid) || ECSDeviceManager.IsLiftSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteVehicleCommand(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsStockerSEM(itinerary.Fromdeviceid))
                        {
                            if (ECSDeviceManager.IsExistConveyor(itinerary.Fromlocationid) && ECSDeviceManager.IsExistConveyor(itinerary.Tolocationid))
                            {
                                ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                            }
                            else
                            {
                                ExecuteCommandStockerSEM(transfer, transferDetail, itinerary);
                            }
                        }
                        else if (ECSDeviceManager.IsAVGSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandAGVSEM(transfer, transferDetail, itinerary);
                        }
                    }
                }
            }
        }



        private static void DoWorkKR01(Dictionary<string, TransferData> transfers)
        {
            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();

            Dictionary<string, Vehicle.VEHICLE_STATE> prevVehicleStatus = new Dictionary<string, Vehicle.VEHICLE_STATE>();
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                prevVehicleStatus.Add(vehicle.ElementId, vehicle.VehicleState);
            }

            foreach (TransferData transfer in transfers.Values)
            {

                //Holding
                if (transfer.Tolocationid.Contains("RACK") == false)
                {
                    continue;
                }

                if (transfer.Priority == "0" && transfer.Transferstatus == "Wait")
                {
                    continue;
                }
                if (IsExecutableTransfer(transfer) == false)
                {
                    continue;
                }
                if (CheckOverSchedule(transfer) == false)
                {
                    continue;
                }

                //Vehicle의 상태가 변경되면 다음 Turn에 반송을 진행한다.
                //우선순위대로 반송을 진행하기 위해서이다.
                foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
                {
                    if (prevVehicleStatus[vehicle.ElementId] != vehicle.VehicleState)
                    {
                        return;
                    }
                }

                transferDetails = manager.getTransferDetailDataList(transfer.Commandid);
                foreach (TransferDetailData transferDetail in transferDetails.Values)
                {

                    if (IsFirstTransferDetail(transferDetail) == true)
                    {
                        transferDetail.Palletidtomove = transfer.Palletid;
                        transferDetail.updateTransferDetailData();

                        //HISTORY Backup LGLS를 위해 추가함.
                        try
                        {
                            transferDetail.insertTransferDetailDataBackup();
                        }
                        catch (Exception ex)
                        {
                            //throw ex;
                        }
                    }
                    if (IsExecuteTransferDetail(transferDetail) == false)
                    {
                        continue;
                    }
                    if (transferDetail.Transferdetailstatus == "Wait")
                    {
                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (ECSDeviceManager.IsConveyorSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsRGVSEM(itinerary.Fromdeviceid) || ECSDeviceManager.IsLiftSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteVehicleCommand(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsStockerSEM(itinerary.Fromdeviceid))
                        {
                            if (ECSDeviceManager.IsExistConveyor(itinerary.Fromlocationid) && ECSDeviceManager.IsExistConveyor(itinerary.Tolocationid))
                            {
                                ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                            }
                            else
                            {
                                ExecuteCommandStockerSEM(transfer, transferDetail, itinerary);
                            }
                        }
                        else if (ECSDeviceManager.IsAVGSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandAGVSEM(transfer, transferDetail, itinerary);
                        }
                    }
                }
            }
        }

        private static void DoWorkOtherTransfers(Dictionary<string, TransferData> transfers)
        {

            Dictionary<string, TransferDetailData> transferDetails = new Dictionary<string, TransferDetailData>();
            MovementManager manager = Naming.GetMovementManager();

            Dictionary<string, Vehicle.VEHICLE_STATE> prevVehicleStatus = new Dictionary<string, Vehicle.VEHICLE_STATE>();
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                prevVehicleStatus.Add(vehicle.ElementId, vehicle.VehicleState);
            }

            foreach (TransferData transfer in transfers.Values)
            {
                //Holding
                if (transfer.Priority == "0" && transfer.Transferstatus == "Wait")
                {
                    continue;
                }
                if (IsExecutableTransfer(transfer) == false)
                {
                    continue;
                }
                if (CheckOverSchedule(transfer) == false)
                {
                    continue;
                }

                //Vehicle의 상태가 변경되면 다음 Turn에 반송을 진행한다.
                //우선순위대로 반송을 진행하기 위해서이다.
                foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
                {
                    if (prevVehicleStatus[vehicle.ElementId] != vehicle.VehicleState)
                    {
                        return;
                    }
                }

                transferDetails = manager.getTransferDetailDataList(transfer.Commandid);
                foreach (TransferDetailData transferDetail in transferDetails.Values)
                {

                    if (IsFirstTransferDetail(transferDetail) == true)
                    {
                        transferDetail.Palletidtomove = transfer.Palletid;
                        transferDetail.updateTransferDetailData();

                        //HISTORY Backup LGLS를 위해 추가함.
                        try
                        {
                            transferDetail.insertTransferDetailDataBackup();
                        }
                        catch (Exception ex)
                        {
                            //throw ex;
                        }
                    }
                    if (IsExecuteTransferDetail(transferDetail) == false)
                    {
                        continue;
                    }
                    if (transferDetail.Transferdetailstatus == "Wait")
                    {
                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (ECSDeviceManager.IsConveyorSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsRGVSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteVehicleCommand(transfer, transferDetail, itinerary);
                        }
                        else if (ECSDeviceManager.IsStockerSEM(itinerary.Fromdeviceid))
                        {
                            if (ECSDeviceManager.IsExistConveyor(itinerary.Fromlocationid) && ECSDeviceManager.IsExistConveyor(itinerary.Tolocationid))
                            {
                                ExecuteCommandConveyor(transfer, transferDetail, itinerary);
                            }
                            else
                            {
                                ExecuteCommandStockerSEM(transfer, transferDetail, itinerary);
                            }
                        }
                        else if (ECSDeviceManager.IsAVGSEM(itinerary.Fromdeviceid))
                        {
                            ExecuteCommandAGVSEM(transfer, transferDetail, itinerary);
                        }
                    }
                }
            }
        }

        private static bool CheckOverSchedule(TransferData transfer)
        {
            return true;
        }
        private static void UpdatePalletInformation(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            // System.Console.WriteLine("[1]ExecuteCommandConveyorSEM");
            MovementManager manager = Naming.GetMovementManager();
            Conveyor conveyor = ECSDeviceManager.GetConveyor(itinerary.Fromlocationid);
            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);
            if (conveyor == null || fromPort == null)
            {
                return;
            }
            string palletId = conveyor.GetPallet(toPort);
            if (string.IsNullOrEmpty(palletId) == false)
            {
                //아래 구문은 Conveyor나 다른 쪽으로 빼는게 좋을 듯..
                manager.chageTransferDetailComplete(transferDetail.Commandid, transferDetail.Commandseq);

                //Pallet 정보 Update를 위해 추가
                PalletData palletData = new PalletData();
                palletData.getPalletData(transfer.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = itinerary.Todeviceid;
                palletData.Locationid = itinerary.Tolocationid;
                palletData.Sublocationid = itinerary.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                if ((transfer.Todeviceid == itinerary.Todeviceid) &&
                   (transfer.Tolocationid == itinerary.Tolocationid) &&
                   (transfer.Tosublocationid == itinerary.Tosublocationid)
                    )
                {
                    palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                    palletData.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();
                }
                else
                {
                    palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Transferring.ToString();
                    palletData.Status = EntityEnumData.PALLETSTATUS.Transferring.ToString();
                }

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transfer.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    palletData.updatePalletData(theDB, true);
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
        }
        private static void ExecuteCommandConveyor(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            // System.Console.WriteLine("[1]ExecuteCommandConveyorSEM");
            MovementManager manager = Naming.GetMovementManager();
            Conveyor conveyor = ECSDeviceManager.GetConveyor(itinerary.Fromlocationid);
            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);

            if (conveyor == null || fromPort == null)
            {
                return;
            }

            if (conveyor.IsAvailible == false)
            {
                return;
            }

            string palletId = conveyor.GetPallet(fromPort);
            if (palletId != transferDetail.Palletidtomove)
            {
                if (transfer.Palletid != transferDetail.Palletidtomove)
                {
                    return;
                }
            }

            if (fromPort.IsPalletExist == true)
            {
                if (transfer.Transferstatus == "Wait")
                {
                    if (fromPort.ElementId == "PORT:22")
                    {
                        if (GlobalConstant.CONVEYOR11_MODE != GlobalConstant.RGV_INGO)
                        {
                            return;
                        }
                    }

                    manager.chageTransferStatus(transfer.Commandid);
                }

                if (transferDetail.Transferdetailstatus == "Wait")
                {

                    conveyor.SetPallet(fromPort, transfer.Palletid);
                    transferDetail.Palletid = transfer.Palletid;
                    transferDetail.Palletidtomove = "";
                    transferDetail.updateTransferDetailData();

                    //HISTORY Backup LGLS를 위해 추가함.
                    try
                    {
                        transferDetail.insertTransferDetailDataBackup();
                    }
                    catch (Exception ex)
                    {
                        //throw ex;
                    }

                    manager.chageTransferDetailStatus(transfer.Commandid, transferDetail.Commandseq);

                    //최종 Conveyor는 첫번째 Conveyor에 Pallet이 놓여지는 순간 Complete 처리한다.
                    if (conveyor.ElementId == "CONVEYOR:11")
                    {
                        //Conveyor:11은 입출고 가능하므로 입고/이동 출발의 경우 제외
                        if (transfer.Fromlocationid != "CONVEYOR:11")
                        {
                            manager.chageTransferDetailComplete(transfer.Commandid, transferDetail.Commandseq);
                            PalletData palletData = new PalletData();
                            palletData.getPalletData(transfer.Palletidname);
                            palletData.Batchno = "";
                            palletData.Materialno = "";
                            palletData.Qty = "";
                            palletData.Unit = "";

                            palletData.Commandid = "";
                            palletData.Commandseq = "";

                            palletData.Predeviceid = "";
                            palletData.Prelocationid = "";
                            palletData.Presublocationid = "";
                            palletData.Preinstalltime = "";

                            palletData.Deviceid = "";
                            palletData.Locationid = "";
                            palletData.Sublocationid = "";
                            palletData.Installtime = "";

                            palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                            palletData.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();

                            palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                            palletData.Updateman = transfer.Createman;

                            DBConnection theDB = null;
                            try
                            {
                                theDB = DBControl.GetConnection();
                                palletData.updatePalletData(theDB, true);
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
                    }

                    // 최종 Conveyor는 첫번째 Conveyor에 Pallet이 놓여지는 순간 Complete 처리한다.
                    //2011년 4월 12일, 신승원 수정함
                    //Peaking Zone은 최종 Buffer 도착후, Completed 처리하도록 CONVEYOR:14 번은 주석처리함. 
                    //if (conveyor.ElementId == "CONVEYOR:13" || conveyor.ElementId == "CONVEYOR:14")
                    if (conveyor.ElementId == "CONVEYOR:13")
                    {
                        manager.chageTransferDetailComplete(transfer.Commandid, transferDetail.Commandseq);
                        PalletData palletData = new PalletData();
                        palletData.getPalletData(transfer.Palletidname);
                        palletData.Batchno = "";
                        palletData.Materialno = "";
                        palletData.Qty = "";
                        palletData.Unit = "";

                        palletData.Commandid = "";
                        palletData.Commandseq = "";

                        palletData.Predeviceid = "";
                        palletData.Prelocationid = "";
                        palletData.Presublocationid = "";
                        palletData.Preinstalltime = "";

                        palletData.Deviceid = "";
                        palletData.Locationid = "";
                        palletData.Sublocationid = "";
                        palletData.Installtime = "";

                        palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                        palletData.Status = EntityEnumData.PALLETSTATUS.Empty.ToString();

                        palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        palletData.Updateman = transfer.Createman;

                        DBConnection theDB = null;
                        try
                        {
                            theDB = DBControl.GetConnection();
                            palletData.updatePalletData(theDB, true);
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
                }

            }
        }
        private static void ExecuteCommandStockerSEM(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            MovementManager manager = Naming.GetMovementManager();
            if (ECSDeviceManager.IsExistRack(itinerary.Fromlocationid) && ECSDeviceManager.IsExistConveyor(itinerary.Tolocationid))
            {
                ExecuteCommandRackToConveyor(transfer, transferDetail, itinerary);
            }
            else if (ECSDeviceManager.IsExistConveyor(itinerary.Fromlocationid) && ECSDeviceManager.IsExistRack(itinerary.Tolocationid))
            {
                ExecuteCommandConveyorToRack(transfer, transferDetail, itinerary);
                //ExecuteVehicleCommand(transfer, transferDetail, itinerary);
            }


        }
        private static void ExecuteCommandRackToConveyor(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            MovementManager manager = Naming.GetMovementManager();
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);
            Conveyor toConveyor = ECSDeviceManager.GetConveyor(toPort.OwnerId);
            //Conveyor가 다운이거나, Pallet이 이미 존재하거나 . 
            if (toPort.IsPalletExist == true || toConveyor.RunState == Conveyor.CONVEYOR_RUN_STATE.DOWN)
            {
                return;
            }
            if (toConveyor.IsAvailible == false)
            {
                return;
            }
            Vehicle moveVehicle = null;
            Dictionary<string, Vehicle> vehicles = ECSDeviceManager.Vehicles;
            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (vehicle.OwnerId == itinerary.Fromdeviceid)
                {
                    moveVehicle = vehicle;
                    break;
                }
            }
            if (moveVehicle.VehicleState != Vehicle.VEHICLE_STATE.IDLE)
            {
                return;
            }

            // 이미 반송을 받았다면
            if (string.IsNullOrEmpty(moveVehicle.Commandid) == false || string.IsNullOrEmpty(moveVehicle.CommandSeq) == false)
            {
                return;
            }


            string palletId = transfer.Palletid;
            if (palletId != transferDetail.Palletidtomove)
            {
                //MessageBox.Show("Pallet 정보가 없어서 반송 불가2." + palletId);
                return;
            }

            //전체 세부 반송 중에서  C/V 2번 반송이 존재하면 반송 불가
            //STK 출고 반송은 RGV를 통해서 STK 1로 들어오는 반송이 존재하면 반송 불가
            if (moveVehicle.ElementId == "VEHICLE:11")
            {
                TransferDetail bizDetail = new TransferDetail();
                int iCheckCnt = 0;

                try
                {
                    iCheckCnt = bizDetail.getCheckInTransferToCNV2(transfer.Commandid);
                }
                catch (Exception cEx)
                {
                    return;
                }

                if (iCheckCnt > 0)
                {
                    return;
                }
            }

            if (transfer.Transferstatus == "Wait")
            {
                manager.chageTransferStatus(transfer.Commandid);
            }
            if (transferDetail.Transferdetailstatus == "Wait")
            {
                transferDetail.Palletid = palletId;
                transferDetail.Palletidtomove = "";
                transferDetail.updateTransferDetailData();

                //HISTORY Backup LGLS를 위해 추가함.
                try
                {
                    transferDetail.insertTransferDetailDataBackup();
                }
                catch (Exception ex)
                {
                    //throw ex;
                }

                manager.chageTransferDetailStatus(transferDetail.Commandid, transferDetail.Commandseq);


                moveVehicle.IO_PALLET_ID = palletId;
                //moveVehicle.IO_FROM_01 = transfer.Fromsublocationid.Substring(0, 2);
                string from01 = transfer.Fromsublocationid.Substring(0, 2);
                if (from01 == "01" || from01 == "03" || from01 == "05" || from01 == "07" || from01 == "09")
                {
                    from01 = "01";
                }
                else if (from01 == "02" || from01 == "04" || from01 == "06" || from01 == "08" || from01 == "10")
                {
                    from01 = "02";
                }
                else
                {
                    from01 = "00";
                }
                moveVehicle.IO_FROM_01 = from01;
                moveVehicle.IO_FROM_02 = transfer.Fromsublocationid.Substring(2, 2);
                moveVehicle.IO_FROM_03 = transfer.Fromsublocationid.Substring(4, 2);
                string is_to_03 = "";

                is_to_03 = toPort.ElementId.Split(':')[1];

                if (is_to_03.Length == 1)
                {
                    is_to_03 = "0" + is_to_03;
                }
                moveVehicle.IO_TO_01 = "00";
                moveVehicle.IO_TO_02 = "00";
                moveVehicle.IO_TO_03 = is_to_03;

                //양방향 Conveyor Direction MODE 전환
                if (toConveyor.ElementId == "CONVEYOR:2")
                {
                    if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "1")
                    {
                        toConveyor.Observables["DIRECTION_MODE"].AsString = "0";
                    }
                }

                moveVehicle.IO_TRANSFER_REQUEST = true;
                moveVehicle.CommandSeq = transferDetail.Commandseq;
                moveVehicle.Commandid = transferDetail.Commandid;

                //Hirack 정보 수정
                HirackData hirackData = new HirackData();
                List<HirackData> dataList = hirackData.getHirackDataCellList(transfer.Fromsublocationid.Substring(0, 2), transfer.Fromsublocationid.Substring(2, 2), transfer.Fromsublocationid.Substring(4, 2));
                dataList[0].Palletid = "";
                dataList[0].Batchno = "";
                dataList[0].Materialno = "";
                dataList[0].Qty = "";
                dataList[0].Unit = "";
                dataList[0].Fromplant = "";
                dataList[0].Fromposition = "";
                dataList[0].Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                dataList[0].Updateman = transfer.Createman;
                dataList[0].Createman = "";
                dataList[0].Createtime = "";
                dataList[0].Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Complete.ToString();
                dataList[0].Cellstatus = EntityEnumData.HIRACKSTATUS.Empty.ToString();



                PalletData palletData = new PalletData();
                palletData.getPalletData(transfer.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = itinerary.Todeviceid;
                palletData.Locationid = itinerary.Tolocationid;
                palletData.Sublocationid = itinerary.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Transferring.ToString();
                palletData.Status = EntityEnumData.PALLETSTATUS.Transferring.ToString();

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transfer.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    dataList[0].updateHirackData(theDB, true, true);
                    palletData.updatePalletData(theDB, true);
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
        }
        private static void ExecuteCommandConveyorToRack(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {

            MovementManager manager = Naming.GetMovementManager();
            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            if (fromPort == null)
            {
                return;
            }
            Conveyor fromConveyor = ECSDeviceManager.GetConveyor(fromPort.OwnerId);
            if (fromConveyor == null)
            {
                return;
            }

            if (fromConveyor.IsAvailible == false)
            {
                return;
            }
            Vehicle moveVehicle = null;
            Dictionary<string, Vehicle> vehicles = ECSDeviceManager.Vehicles;
            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (vehicle.OwnerId == itinerary.Fromdeviceid)
                {
                    moveVehicle = vehicle;
                    break;
                }
            }
            if (moveVehicle.VehicleState != Vehicle.VEHICLE_STATE.IDLE)
            {
                return;
            }

            //이미 반송을 받았다면
            if (string.IsNullOrEmpty(moveVehicle.Commandid) == false || string.IsNullOrEmpty(moveVehicle.CommandSeq) == false)
            {
                return;
            }

            string palletId = fromConveyor.GetPallet(fromPort);
            if (palletId != transferDetail.Palletidtomove)
            {
                return;
            }

            // if (fromConveyor.IsWaitOut == true)
            // {
            if (transfer.Transferstatus == "Wait")
            {
                manager.chageTransferStatus(transfer.Commandid);
            }
            if (transferDetail.Transferdetailstatus == "Wait")
            {
                transferDetail.Palletid = palletId;
                transferDetail.Palletidtomove = "";
                transferDetail.updateTransferDetailData();

                //HISTORY Backup LGLS를 위해 추가함.
                try
                {
                    transferDetail.insertTransferDetailDataBackup();
                }
                catch (Exception ex)
                {
                    //throw ex;
                }

                manager.chageTransferDetailStatus(transfer.Commandid, transferDetail.Commandseq);

                string is_from_03 = "";

                is_from_03 = fromPort.ElementId.Split(':')[1];

                if (is_from_03.Length == 1)
                {
                    is_from_03 = "0" + is_from_03;
                }


                moveVehicle.IO_PALLET_ID = palletId;
                moveVehicle.IO_FROM_01 = "00";
                moveVehicle.IO_FROM_02 = "00";
                moveVehicle.IO_FROM_03 = is_from_03;
                //moveVehicle.IO_TO_01 = transfer.Tosublocationid.Substring(0, 2);


                string to01 = transfer.Tosublocationid.Substring(0, 2);
                if (to01 == "01" || to01 == "03" || to01 == "05" || to01 == "07" || to01 == "09")
                {
                    to01 = "01";
                }
                else if (to01 == "02" || to01 == "04" || to01 == "06" || to01 == "08" || to01 == "10")
                {
                    to01 = "02";
                }
                else
                {
                    to01 = "00";
                }

                moveVehicle.IO_TO_01 = to01;
                moveVehicle.IO_TO_02 = transfer.Tosublocationid.Substring(2, 2);
                moveVehicle.IO_TO_03 = transfer.Tosublocationid.Substring(4, 2);
                moveVehicle.IO_TRANSFER_REQUEST = true;
                moveVehicle.CommandSeq = transferDetail.Commandseq;
                moveVehicle.Commandid = transferDetail.Commandid;

                Rack rack = ECSDeviceManager.GetRack(transfer.Tolocationid);
                HirackData hirackData = new HirackData();
                List<HirackData> dataList = hirackData.getHirackDataCellList(transfer.Tosublocationid.Substring(0, 2), transfer.Tosublocationid.Substring(2, 2), transfer.Tosublocationid.Substring(4, 2));
                dataList[0].Palletid = transfer.Palletidname;
                dataList[0].Batchno = transfer.Batchno;
                dataList[0].Materialno = transfer.Materialno;
                dataList[0].Qty = transfer.Qty;
                dataList[0].Unit = transfer.Unit;
                dataList[0].Fromplant = transfer.Fromplant;
                dataList[0].Fromposition = transfer.Fromposition;
                dataList[0].Createman = transfer.Createman;
                dataList[0].Createtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                dataList[0].Hiracktransferstatus = EntityEnumData.HIRACKSTRANSFERSTATUS.Complete.ToString();
                dataList[0].Cellstatus = EntityEnumData.HIRACKSTATUS.Full.ToString();

                ////Pallet 정보 Update를 위해 추가
                PalletData palletData = new PalletData();
                palletData.getPalletData(transfer.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = transfer.Todeviceid;
                palletData.Locationid = transfer.Tolocationid;
                palletData.Sublocationid = transfer.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Complete.ToString();
                palletData.Status = EntityEnumData.PALLETSTATUS.Assigned.ToString();

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transfer.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    dataList[0].updateHirackData(theDB, true, true);
                    palletData.updatePalletData(theDB, true);
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

            //}
        }

        private static void ExecuteVehicleCommand(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            int iCheckCnt = 0;
            MovementManager manager = Naming.GetMovementManager();
            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);
            if (fromPort == null || toPort == null)
            {
                return;
            }
            Conveyor fromConveyor = ECSDeviceManager.GetConveyor(fromPort.OwnerId);
            Conveyor toConveyor = ECSDeviceManager.GetConveyor(toPort.OwnerId);
            if (fromConveyor == null || toConveyor == null)
            {
                return;
            }
            if (fromConveyor.IsAvailible == false || toConveyor.IsAvailible == false)
            {
                return;
            }

            Vehicle moveVehicle = null;
            Dictionary<string, Vehicle> vehicles = ECSDeviceManager.Vehicles;
            foreach (Vehicle vehicle in vehicles.Values)
            {
                if (vehicle.OwnerId == itinerary.Fromdeviceid)
                {
                    moveVehicle = vehicle;
                    break;
                }
            }
            if (moveVehicle.VehicleState != Vehicle.VEHICLE_STATE.IDLE)
            {
                return;
            }

            // 이미 반송을 받았다면
            if (string.IsNullOrEmpty(moveVehicle.Commandid) == false || string.IsNullOrEmpty(moveVehicle.CommandSeq) == false)
            {
                return;
            }

            string palletId = fromConveyor.GetPallet(fromPort);
            if (palletId != transferDetail.Palletidtomove)
            {
                return;

            }
            if (toPort.IsPalletExist == true)
            {
                System.Console.WriteLine("Dest 위치에 Pallet이 존재하므로, 반송불가 :" + moveVehicle.ElementId);
                return;
            }

            //TO C/V에서 나오는 반송이 존재할 경우에 반송 명령을 주면안됨.
            if (toConveyor.ElementId == "CONVEYOR:2")
            {
                //출고모드이고, 하이랙측의 버퍼에 팔렛이 있으면 
                if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                {
                    if (toConveyor.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true
                        || toConveyor.Observables["PALLET_EXIST_FLAG_01"].AsBoolean == true
                        || (toConveyor.Observables["PALLET_EXIST01"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST01"].AsString ) == false)
                        || (toConveyor.Observables["PALLET_EXIST02"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST02"].AsString) == false)
                        )
                    {
                        return;
                    }
                }

                //전체 세부 반송 중에서  1번 Stocker 반송이 존재하면 반송 불가
                //RGV는 STK 반송이 존재하면 반송 불가, STK는 진행중인 RGV가 존재하면 반송불가
                TransferDetail bizDetail = new TransferDetail();
                try
                {
                    iCheckCnt = bizDetail.getCheckOutTransferFromSTK1(transfer.Commandid);
                }
                catch (Exception cEx)
                {
                    return;
                }

                if (iCheckCnt > 0)
                {
                    return;
                }
                else
                {
                    //S/C 반송 완료후 출고모드이고, 하이랙측의 버퍼에 팔렛이 있는지를 한번더 체크.
                    if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                    {
                        if (
                            toConveyor.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true
                            || toConveyor.Observables["PALLET_EXIST_FLAG_01"].AsBoolean == true
                            || (toConveyor.Observables["PALLET_EXIST01"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST01"].AsString ) == false)
                            || (toConveyor.Observables["PALLET_EXIST02"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST02"].AsString) == false)
                           )
                        {
                            return;
                        }
                    }
                }

                //전체 세부 반송 중에서  C/V 2번 출고 반송이 존재하면 반송 불가
                //int iCheckCnt1 = 0;

                //try
                //{
                //    iCheckCnt1 = bizDetail.getCheckOutTransferFromCNV2(transfer.Commandid);
                //}
                //catch (Exception cEx1)
                //{
                //    return;
                //}

                //if (iCheckCnt1 > 0)
                //{
                //    return;
                //}
            }
            //TO C/V에서 나오는 반송이 존재할 경우에 반송 명령을 주면안됨.
            if (toConveyor.ElementId == "CONVEYOR:11")
            {
                //입고모드이면서, 입고대측에 팔렛이 있으면 
                if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                {
                    if (toConveyor.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true
                        || toConveyor.Observables["PALLET_EXIST_FLAG_01"].AsBoolean == true
                        || (toConveyor.Observables["PALLET_EXIST01"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST01"].AsString) == false)
                        || (toConveyor.Observables["PALLET_EXIST02"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST02"].AsString) == false)
                       )
                    {
                        return;
                    }
                }

                //전체 세부 반송 중에서  1번 Stocker 반송이 존재하면 반송 불가
                //RGV는 STK 반송이 존재하면 반송 불가, STK는 진행중인 RGV가 존재하면 반송불가
                TransferDetail bizDetail = new TransferDetail();
                iCheckCnt = 0;

                try
                {
                    iCheckCnt = bizDetail.getCheckInTransferFromCNV11(transfer.Commandid);
                }
                catch (Exception cEx)
                {
                    return;
                }

                if (iCheckCnt > 0)
                {
                    return;
                }
                else
                {
                    //입고모드이면서, 입고대측에 팔렛이 있는지를 한번더 체크 
                    if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                    {
                        if (toConveyor.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true
                            || toConveyor.Observables["PALLET_EXIST_FLAG_01"].AsBoolean == true
                            || (toConveyor.Observables["PALLET_EXIST01"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST01"].AsString) == false)
                            || (toConveyor.Observables["PALLET_EXIST02"].Value != null && string.IsNullOrEmpty(toConveyor.Observables["PALLET_EXIST02"].AsString) == false)
                           )
                        {
                            return;
                        }
                    }
                }

            }

            // if (fromConveyor.IsWaitOut == true)
            // {
            if (transfer.Transferstatus == "Wait")
            {
                manager.chageTransferStatus(transfer.Commandid);
            }
            if (transferDetail.Transferdetailstatus == "Wait")
            {
                transferDetail.Palletid = palletId;
                transferDetail.Palletidtomove = "";
                transferDetail.updateTransferDetailData();

                //HISTORY Backup LGLS를 위해 추가함.
                try
                {
                    transferDetail.insertTransferDetailDataBackup();
                }
                catch (Exception ex)
                {
                    //throw ex;
                }

                manager.chageTransferDetailStatus(transfer.Commandid, transferDetail.Commandseq);

                string is_from_03 = "";
                string is_to_03 = "";

                is_from_03 = fromPort.ElementId.Split(':')[1];
                is_to_03 = toPort.ElementId.Split(':')[1];

                if (is_from_03.Length == 1)
                {
                    is_from_03 = "0" + is_from_03;
                }

                if (is_to_03.Length == 1)
                {
                    is_to_03 = "0" + is_to_03;
                }

                moveVehicle.IO_PALLET_ID = palletId;
                moveVehicle.IO_FROM_01 = "00";
                moveVehicle.IO_FROM_02 = "00";
                moveVehicle.IO_FROM_03 = is_from_03;
                moveVehicle.IO_TO_01 = "00";
                moveVehicle.IO_TO_02 = "00";
                moveVehicle.IO_TO_03 = is_to_03;

                //양방향 Conveyor Direction MODE 전환
                if (toConveyor.ElementId == "CONVEYOR:2" || toConveyor.ElementId == "CONVEYOR:11")
                {
                    if (toConveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                    {
                        toConveyor.Observables["DIRECTION_MODE"].AsString = "1";
                        GlobalConstant.CONVEYOR11_MODE = "1";
                    }
                }

                moveVehicle.IO_TRANSFER_REQUEST = true;
                moveVehicle.CommandSeq = transferDetail.Commandseq;
                moveVehicle.Commandid = transferDetail.Commandid;

                PalletData palletData = new PalletData();
                palletData.getPalletData(transfer.Palletidname);

                palletData.Commandid = transferDetail.Commandid;
                palletData.Commandseq = transferDetail.Commandseq;

                palletData.Predeviceid = palletData.Deviceid;
                palletData.Prelocationid = palletData.Locationid;
                palletData.Presublocationid = palletData.Sublocationid;
                palletData.Preinstalltime = palletData.Installtime;

                palletData.Deviceid = itinerary.Todeviceid;
                palletData.Locationid = itinerary.Tolocationid;
                palletData.Sublocationid = itinerary.Tosublocationid;
                palletData.Installtime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");

                palletData.Pallettransferstatus = EntityEnumData.PALLETTRANSFERSTATUS.Transferring.ToString();
                palletData.Status = EntityEnumData.PALLETSTATUS.Transferring.ToString();

                palletData.Updatetime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                palletData.Updateman = transfer.Createman;

                DBConnection theDB = null;
                try
                {
                    theDB = DBControl.GetConnection();
                    palletData.updatePalletData(theDB, true);
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


            //}
        }


        private static void ExecuteCommandAGVSEM(TransferData transfer, TransferDetailData transferDetail, ItineraryData itinerary)
        {
            MovementManager manager = Naming.GetMovementManager();
            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);
            if (fromPort == null || toPort == null)
            {
                return;
            }
            Conveyor fromConveyor = ECSDeviceManager.GetConveyor(fromPort.OwnerId);
            Conveyor toConveyor = ECSDeviceManager.GetConveyor(toPort.OwnerId);
            if (fromConveyor == null || toConveyor == null)
            {
                return;
            }

            if (fromConveyor.IsAvailible == false || toConveyor.IsAvailible == false)
            {
                return;
            }
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("AGV"))
                {
                    if (vehicle.CommandSeq == transferDetail.Commandseq && vehicle.Commandid == transferDetail.Commandid)
                    {
                        //이 반송은 이미 반송을 지시한 반송임.
                        return;
                    }
                    if (vehicle.ReservedCommandSeq == transferDetail.Commandseq && vehicle.ReservedCommandId == transferDetail.Commandid)
                    {
                        return;
                    }
                }
            }

            //Vehicle moveVehicle = ECSDeviceManager.GetVehicle("VEHICLE:1");
            Vehicle moveVehicle = null;
            double weight = 99999;
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("AGV"))
                {
                    if (vehicle.AgvVehicleState == Vehicle.AGV_RUN_STATE.IDLE)
                    {
                        if (string.IsNullOrEmpty(vehicle.Commandid) && string.IsNullOrEmpty(vehicle.CommandSeq))
                        {
                            string agvFrom = string.IsNullOrEmpty(vehicle.AgvMarkPlate) ? "1" : vehicle.AgvMarkPlate;
                            if (agvFrom == "0")
                            {
                                agvFrom = "1";
                            }
                            MarkPlate agvToMakrPlate = ECSDeviceManager.GetMarkplateByPortId(fromPort.ElementId);
                            if (agvToMakrPlate == null)
                            {
                                continue;
                            }
                            // string agvTo = ECSDeviceManager.GetMarkplateByPortId(agvToMakrPlate.Markplateid);
                            AGVPath agvPath = ECSDeviceManager.GetShortestPath(agvFrom, agvToMakrPlate.Markplateid);
                            if (agvPath.Weight <= weight)
                            {
                                weight = agvPath.Weight;
                                moveVehicle = vehicle;
                            }
                        }

                    }
                }
            }
            if (moveVehicle == null)
            {
                return;
            }
            if (moveVehicle.AgvVehicleState != Vehicle.AGV_RUN_STATE.IDLE)
            {
                return;
            }

            //이미 반송을 받았다면
            if (string.IsNullOrEmpty(moveVehicle.Commandid) == false || string.IsNullOrEmpty(moveVehicle.CommandSeq) == false)
            {
                return;
            }

            string palletId = fromConveyor.GetPallet(fromPort);
            if (palletId != transferDetail.Palletidtomove)
            {
                return;
            }

            //if (toPort.IsPalletExist == true)
            //{
            //    System.Console.WriteLine("Dest 위치에 Pallet이 존재하므로, 반송불가 :" + moveVehicle.ElementId);
            //    return;
            //}

            // 버퍼 체크해야함.



            // if (fromConveyor.IsWaitOut == true)
            // {
            if (transfer.Transferstatus == "Wait")
            {
                manager.chageTransferStatus(transfer.Commandid);
            }
            //AGV Vehicle On/Offline 상태 체크해야함.
            if (transferDetail.Transferdetailstatus == "Wait")
            {

                //manager.chageTransferDetailStatus(transfer.Commandid, transferDetail.Commandseq);

                //AgvSEM agvSem = ECSDeviceManager.GetAGVSEM("AGVSEM:1");
                //WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferRequest_601);
                //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
                //command.SAPCommandID = transfer.Commandid.PadLeft(12, ' ');
                //command.CommandSequence = transferDetail.Commandseq.PadLeft(4, ' ');
                //command.VehicleId = moveVehicle.ElementId.Split(':')[1].PadLeft(2, '0');
                //command.PalletId = transfer.Palletid.PadLeft(6, ' ');
                //command.From = fromPort.ElementId.Split(':')[1].PadLeft(2, ' ');
                //command.To = toPort.ElementId.Split(':')[1].PadLeft(2, ' ');
                //command.Priority = transfer.Priority.PadLeft(4, '0');
                //agvSem.SendClient(command.BinaryString);

                //moveVehicle.AgvBinaryCommandId = command.Sequence;
                //moveVehicle.CommandSeq = transferDetail.Commandseq;
                //moveVehicle.Commandid = transferDetail.Commandid;
            }
            //}
        }

        private static void CheckEquipmentStatus()
        {
            //Stacker Crane 상태 Check
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                SubSystemData subData = new SubSystemData();
                string oldAvailable = "";

                try
                {
                    subData.getSubSystemData(vehicle.ElementId);
                    oldAvailable = subData.Available;

                    if (vehicle.AlarmSetCode != 0)
                    {
                        subData.Available = "5";
                    }
                    else
                    {
                        if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
                        {
                            subData.Available = "4";
                        }
                        else if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.IDLE)
                        {
                            subData.Available = "0";
                        }
                        else
                        {
                            string strFrom = vehicle.__IO_FROM_01.ToString();
                            string strTo = vehicle.__IO_TO_01.ToString();

                            if (string.IsNullOrEmpty(strFrom) && string.IsNullOrEmpty(strTo))
                            {
                                subData.Available = "0";
                            }
                            else
                            {
                                if (strFrom != "00" &&
                                    (!string.IsNullOrEmpty(strFrom)) &&
                                    strTo != "00" &&
                                    (!string.IsNullOrEmpty(strTo)))
                                {
                                    subData.Available = "3";
                                }
                                else
                                {
                                    if (strFrom != "00" && (!string.IsNullOrEmpty(strFrom)))
                                    {
                                        subData.Available = "2";
                                    }
                                    else
                                    {
                                        if (strTo != "00" && (!string.IsNullOrEmpty(strTo)))
                                        {
                                            subData.Available = "1";
                                        }
                                        else
                                        {
                                            subData.Available = "0";
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (oldAvailable != subData.Available)
                    {
                        subData.updateSubSystemData();
                    }
                }
                catch (Exception ex)
                {
                    //
                }
            }

            //Conveyor 상태 Check
            foreach (Conveyor cnv in ECSDeviceManager.Conveyors.Values)
            {
                SubSystemData subData = new SubSystemData();
                string oldAvailable = "";

                try
                {
                    //Equipment 상태 보고
                    if (cnv.ElementId == "CONVEYOR:2" || cnv.ElementId == "CONVEYOR:11" ||
                        cnv.ElementId == "CONVEYOR:12" || cnv.ElementId == "CONVEYOR:13" ||
                        cnv.ElementId == "CONVEYOR:14" || cnv.ElementId == "CONVEYOR:15")
                    {
                        subData.getSubSystemData(cnv.ElementId);
                        oldAvailable = subData.Available;

                        if (cnv.OperationMode == Conveyor.CONVEYOR_OPERATION_MODE.AUTO)
                        {
                            subData.Available = "0";

                            //입고대, 피킹존, 생산동 입고대에 Pallet이 있을 경우만 반송 가능
                            if (cnv.ElementId == "CONVEYOR:11")
                            {
                                if (cnv.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true)
                                {
                                    subData.Available = "0";
                                }
                                else
                                {
                                    subData.Available = "1";
                                }
                            }

                            if (cnv.ElementId == "CONVEYOR:12")
                            {
                                if (cnv.Observables["PALLET_EXIST_FLAG_02"].AsBoolean == true)
                                {
                                    subData.Available = "0";
                                }
                                else
                                {
                                    subData.Available = "1";
                                }
                            }


                            if (cnv.ElementId == "CONVEYOR:15")
                            {
                                if (cnv.Observables["PALLET_EXIST_FLAG_03"].AsBoolean == true)
                                {
                                    subData.Available = "0";
                                }
                                else
                                {
                                    subData.Available = "1";
                                }
                            }
                        }
                        else
                        {
                            subData.Available = "1";
                        }

                        if (oldAvailable != subData.Available)
                        {
                            subData.updateSubSystemData();

                            //Down으로 상태가 변경되었을 경우만 처리함.
                            if (subData.Available == "1")
                            {
                                //알람 WMS I/F
                                try
                                {
                                    MakeErrorString maError = new MakeErrorString(cnv.ElementId, "0001");

                                    string strTotal = maError.createErrorString();

                                    if (!string.IsNullOrEmpty(strTotal))
                                    {
                                        MakeWmsIfString mwIf = new MakeWmsIfString();
                                        mwIf.insertError(strTotal);
                                    }
                                }
                                catch (Exception ex)
                                {
                                }
                            }

                        }

                        //MODE CHECK
                        if (cnv.ElementId == "CONVEYOR:2" || cnv.ElementId == "CONVEYOR:11")
                        {
                            SubSystemData subData1 = new SubSystemData();
                            subData1.getSubSystemData(cnv.ElementId + "R");
                            oldAvailable = subData1.Available;

                            if (cnv.DirectionMode == Conveyor.CONVEYOR_DIRECTION_MODE.RGVINGO)
                            {
                                if (cnv.ElementId == "CONVEYOR:2")
                                {
                                    subData1.Available = "1";
                                }
                                else
                                {
                                    subData1.Available = "0";
                                }
                            }
                            else
                            {
                                if (cnv.ElementId == "CONVEYOR:2")
                                {
                                    subData1.Available = "0";
                                }
                                else
                                {
                                    subData1.Available = "1";
                                }
                            }

                            if (oldAvailable != subData1.Available)
                            {
                                subData1.updateSubSystemData();
                            }
                        }
                    }
                }
                catch (Exception se)
                {
                    //
                }
            }


            //장비 상태 변경
            EquipmentData eqpData = new EquipmentData();
            Dictionary<string, EquipmentData> list = new Dictionary<string, EquipmentData>();

            list = eqpData.getEquipmentDataList();

            foreach (EquipmentData eqpTemp in list.Values)
            {
                int iStatus = 0;
                SubSystemData subData = new SubSystemData();
                string oldAvaiable = "";

                oldAvaiable = eqpTemp.Available;

                try
                {
                    if (eqpTemp.Equipmenttype == "Conveyor" || eqpTemp.Equipmenttype == "Stocker")
                    {
                        iStatus = subData.getStatusTotalByEqp(eqpTemp.Equipmentid);

                        if (iStatus != 0)
                        {
                            //Stocker의 경우 Down이 4 이상임.
                            if (eqpTemp.Equipmenttype == "Stocker" && iStatus < 4)
                            {
                                eqpTemp.Available = "0";
                            }
                            else
                            {
                                eqpTemp.Available = "1";
                            }
                        }
                        else
                        {
                            eqpTemp.Available = "0";
                        }

                        if (oldAvaiable != eqpTemp.Available)
                        {
                            eqpTemp.updateEquipmentData();
                        }
                    }
                }
                catch (Exception ex)
                {
                    //
                }
            }
        }
    }
}


