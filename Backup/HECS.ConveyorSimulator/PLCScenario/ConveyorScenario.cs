using System;
using System.Collections.Generic;
using System.Text;
using HECS.ConveyorSimulator.Devices;
using ECP.Device.Observe;

namespace HECS.ConveyorSimulator.PLCScenario
{
    public static class ConveyorScenario
    {

        public static void Initialize()
        {

            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                //Operation Mode 초기화
                WriteObservable(conveyor, "OPERATION_MODE", true);
                //Pallet Exist Flag 초기화
                WriteObservable(conveyor, "PALLET_EXIST_FLAG_01", false);
                WriteObservable(conveyor, "PALLET_EXIST_FLAG_02", false);
                WriteObservable(conveyor, "PALLET_EXIST_FLAG_03", false);
                WriteObservable(conveyor, "PALLET_EXIST_FLAG_04", false);
                WriteObservable(conveyor, "PALLET_EXIST_FLAG_05", false);
                //반송 초기화
                WriteObservable(conveyor, "PALLET_EXIST01", "");
                WriteObservable(conveyor, "PALLET_EXIST02", "");
                WriteObservable(conveyor, "PALLET_EXIST03", "");
                WriteObservable(conveyor, "PALLET_EXIST04", "");
                WriteObservable(conveyor, "PALLET_EXIST05", "");

                WriteObservable(conveyor, "LOAD_COMPLETE_01", false);
                WriteObservable(conveyor, "LOAD_COMPLETE_02", false);
                WriteObservable(conveyor, "UNLOAD_COMPLETE_01", false);
                WriteObservable(conveyor, "UNLOAD_COMPLETE_02", false);

                WriteObservable(conveyor, "IN_READY_02", false);

                WriteObservable(conveyor, "WAIT_IN", false);
                WriteObservable(conveyor, "WAIT_OUT", false);
                WriteObservable(conveyor, "DIRECTION_MODE", 1);
               
            }

            string[] conveyorIndex = new string[] { "3", "5", "7", "9", "11", "12", "15" };
            foreach (string conveyorId in conveyorIndex)
            {
                Conveyor targetConveyor = DeviceManager.GetConveyor ("CONVEYOR:" + conveyorId);
                if (targetConveyor == null)
                {
                    continue;
                }
                WriteObservable(targetConveyor, "IN_READY_02", true);
                WriteObservable(targetConveyor, "DIRECTION_MODE", 0);
            }
        }
        private static void WriteObservable(Conveyor conveyor, string key, object value)
        {
            if (conveyor.Observables.ContainsKey(key))
            {
                conveyor.Observables[key].SetValue(value);
            }
        }
        public static void BranchTrigger(Conveyor conveyor, Observable obs)
        {
            switch (obs.ObservableId)
            {
                case "IN_READY_02":
                    OnInputReady(conveyor, obs);
                    break;
                case "LOAD_COMPLETE_01":
                case "LOAD_COMPLETE_02":
                    OnLoadComplete(conveyor, obs);
                    break;
                case "OPERATION_MODE":
                    OnOperationMode(conveyor, obs);
                    break;
                case "PALLET_EXIST_FLAG_01":
                case "PALLET_EXIST_FLAG_02":
                case "PALLET_EXIST_FLAG_03":
                case "PALLET_EXIST_FLAG_04":
                case "PALLET_EXIST_FLAG_05":
                    OnPalletExistFlag(conveyor, obs);
                    break;
                case "PALLET_EXIST01":
                case "PALLET_EXIST02":
                case "PALLET_EXIST03":
                case "PALLET_EXIST04":
                case "PALLET_EXIST05":
                    OnPalletExist(conveyor, obs);
                    break;
                case "UNLOAD_COMPLETE_01":
                case "UNLOAD_COMPLETE_02":
                    OnUnloadComplete(conveyor, obs);
                    break;
                case "WAIT_IN":
                    OnWaitIn(conveyor, obs);
                    break;
                case "WAIT_OUT":
                    OnWaitOut(conveyor, obs);
                    break;
                case "LOAD_COMPLETE_ACK_01" :
                    OnLoadCompleteACK1(conveyor, obs);
                    break;
                case "LOAD_COMPLETE_ACK_02":
                    OnLoadCompleteACK2(conveyor, obs);
                    break;
                case "UNLOAD_COMPLETE_ACK_01":
                    OnUnloadCompleteACK1(conveyor, obs);
                    break;
                case "UNLOAD_COMPLETE_ACK_02":
                    OnUnloadCompleteACK2(conveyor, obs);
                    break;
                case "UNLOAD_REQUEST_02":
                    OnUnloadRequest02(conveyor, obs);
                    break;
                case "UNLOAD_REQUEST_ACK_02":
                    OnUnloadRequestACK2(conveyor, obs);
                    break; 
                default :  
                    System.Console.WriteLine("OBS_ELSE:" + obs.ObservableId);
                    break;
            }


        }

        private static void OnUnloadCompleteACK2(Conveyor conveyor, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                conveyor.Observables["UNLOAD_COMPLETE_02"].Value = false;
                conveyor.Observables["UNLOAD_COMPLETE_ACK_02"].Value = false;
            }
            
        }

        private static void OnUnloadCompleteACK1(Conveyor conveyor, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                conveyor.Observables["UNLOAD_COMPLETE_01"].Value = false;
                conveyor.Observables["UNLOAD_COMPLETE_ACK_01"].Value = false;
            }

        }

        private static void OnLoadCompleteACK2(Conveyor conveyor, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                conveyor.Observables["LOAD_COMPLETE_02"].Value = false;
                conveyor.Observables["LOAD_COMPLETE_ACK_02"].Value = false;
            }
            
        }

        private static void OnLoadCompleteACK1(Conveyor conveyor, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                conveyor.Observables["LOAD_COMPLETE_01"].Value = false;
                conveyor.Observables["LOAD_COMPLETE_ACK_01"].Value = false;
            }
            
        }

        private static void OnInputReady(Conveyor conveyor, Observable obs)
        {
            // 구현할 필요 없음.
            return;
        }

        private static void OnLoadComplete(Conveyor conveyor, Observable obs)
        {
            //구현할 필요 없음.
        }

        private static void OnOperationMode(Conveyor conveyor, Observable obs)
        {
            //구현할 필요 없음.
        }

        private static void OnPalletExistFlag(Conveyor conveyor, Observable obs)
        {
            
            switch (conveyor.DeviceId)
            {
                case "CONVEYOR:2":
                case "CONVEYOR:11": 
                    if (conveyor.Observables["DIRECTION_MODE"].Value.ToString () == "0")
                    {
                        DoMoveToAGVLine(conveyor, obs);
                    }
                    else
                    {
                        DoMovedFromAGVLine(conveyor, obs);
                    }
                    break; 
                case "CONVEYOR:3":
                case "CONVEYOR:5":
                case "CONVEYOR:7":
                case "CONVEYOR:9":
                case "CONVEYOR:12":
                case "CONVEYOR:15":
                    DoMoveToAGVLine(conveyor, obs);
                    break;
                default:
                    DoMovedFromAGVLine(conveyor, obs);
                    break;
            }
        }


        private static void OnPalletExist(Conveyor conveyor, Observable obs)
        {
            switch (conveyor.DeviceId)
            {

                case "CONVEYOR:2":
                case "CONVEYOR:11":
                    if (conveyor.Observables["DIRECTION_MODE"].Value.ToString() == "0")
                    {
                        DoPalletMoveToAGV(conveyor, obs);
                    }
                    else
                    {
                        DoPalletMovedFromAGV(conveyor, obs);
                    }
                    break; 
                   
                case "CONVEYOR:3":
                case "CONVEYOR:5":
                case "CONVEYOR:7":
                case "CONVEYOR:9":
                case "CONVEYOR:12":
                case "CONVEYOR:15":
                    DoPalletMoveToAGV(conveyor, obs);
                    break;
                default:
                    DoPalletMovedFromAGV(conveyor, obs);
                    break;
            }
        }



        private static void OnUnloadComplete(Conveyor conveyor, Observable obs)
        {
        }

        private static void OnWaitIn(Conveyor conveyor, Observable obs)
        {

        }

        private static void OnWaitOut(Conveyor conveyor, Observable obs)
        {

        }

        private static void OnUnloadRequest02(Conveyor conveyor, Observable obs)
        {
            if ((bool)obs.Value == true)
            {
                conveyor.Observables["UNLOAD_REQUEST_02"].Value = false;
                conveyor.Observables["UNLOAD_REQUEST_ACK_02"].Value = true;
            }
        }

        private static void OnUnloadRequestACK2(Conveyor conveyor, Observable obs)
        {
        }

        private static bool IsInputConveyor(string conveyorId)
        {
            
            switch (conveyorId)
            {
                case "CONVEYOR:2":
                case "CONVEYOR:11":
                    Conveyor conveyor = DeviceManager.GetConveyor(conveyorId);
                    if (conveyor.Observables["DIRECTION_MODE"].Value.ToString()   == "0")
                    {
                        return true;
                    }
                    else
                    {
                        return false; 
                    }
                    
                case "CONVEYOR:3":
                case "CONVEYOR:5":
                case "CONVEYOR:7":
                case "CONVEYOR:9":
                case "CONVEYOR:12":
                case "CONVEYOR:15":
                    return true;
                default:
                    return false;
            }
        }

        private static Port GetNextPort(Conveyor conveyor, Port port)
        {
            int nextPortIndex = 0;
            int portIndex = int.Parse(port.ObjectOrder);
            Port nextPort = null;

            if (IsInputConveyor(conveyor.DeviceId))
            {
                nextPortIndex = int.Parse(port.ObjectOrder) - 1;
                if (nextPortIndex < 1)
                {
                    nextPortIndex = 1;
                }
            }
            else
            {
                nextPortIndex = int.Parse(port.ObjectOrder) + 1;
                if (nextPortIndex > conveyor.Ports.Count)
                {
                    nextPortIndex = conveyor.Ports.Count;
                }
            }

            if (portIndex == nextPortIndex)
            {
                nextPort = port;
            }
            else
            {
                nextPort = DeviceManager.GetPortInConveyorByIndex(conveyor.DeviceId, nextPortIndex.ToString());
            }
            return nextPort;
        }


        #region 제어 로직

        /**
         * Conveyor 제어 로직을 담고 있다. 
         **/

        //
        private static void DoMovedFromAGVLine(Conveyor conveyor, Observable obs)
        {
            int index = 0;
            int.TryParse(obs.ObservableId.Replace("PALLET_EXIST_FLAG_", ""), out index);
            Port port = DeviceManager.GetPortInConveyorByIndex(conveyor.DeviceId, index.ToString());
            if (port == null)
            {
                return;
            }
            // 마지막 버퍼
            if (index == 1)
            {
                if ((bool)obs.Value == true)
                {
                    conveyor.Observables["IN_READY_02"].Value = false;
                    conveyor.Observables["LOAD_COMPLETE_01"].Value = true;
                    //port.PortState = Port.PORT_STATE.LOAD_COMPLETE  ;
                    port.StepCount = 0;
                    port.PortState = Port.PORT_STATE.READY;
                }
                else
                {
                    conveyor.Observables["IN_READY_02"].Value = true;
                    conveyor.Observables["LOAD_COMPLETE_01"].Value = false;
                    port.StepCount = 0;
                    port.PortState = Port.PORT_STATE.READY;
                }
            }
            if((bool)obs.Value==false)
            {
                if (index == conveyor.Ports.Count && IsInputConveyor (conveyor.DeviceId ) == false)
                {
                    if((bool)conveyor.Observables["WAIT_OUT"].Value == true)
                    {
                        conveyor.Observables["WAIT_OUT"].Value = false;
                        conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)].Value = "";
                    }
                }
                
            }
            string palletId = (string)conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)].Value;
            if (string.IsNullOrEmpty(palletId) == false)
            {
                DoPalletMovedFromAGV(conveyor, conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)]);
            }
        }

        private static void DoMoveToAGVLine(Conveyor conveyor, Observable obs)
        {
            int index = 0;
            int.TryParse(obs.ObservableId.Replace("PALLET_EXIST_FLAG_", ""), out index);
            Port port = DeviceManager.GetPortInConveyorByIndex(conveyor.DeviceId, index.ToString());
            if (port == null)
            {
                return;
            }
            // 마지막 버퍼
            if (index == conveyor.Ports.Count)
            {
                if ((bool)obs.Value == true)
                {
                    conveyor.Observables["IN_READY_02"].Value = false;
                    conveyor.Observables["LOAD_COMPLETE_02"].Value = true;
                    //port.PortState = Port.PORT_STATE.LOAD_COMPLETE;
                    port.StepCount = 0;
                    port.PortState = Port.PORT_STATE.READY;
                }
                else
                {
                    conveyor.Observables["IN_READY_02"].Value = true;
                    conveyor.Observables["LOAD_COMPLETE_02"].Value = false;
                    port.StepCount = 0;
                    port.PortState = Port.PORT_STATE.READY;
                }
            }
            if ((bool)obs.Value == false)
            {
                if (index == 1 && IsInputConveyor(conveyor.DeviceId) == true)
                {
                    if ((bool)conveyor.Observables["WAIT_OUT"].Value == true)
                    {
                        conveyor.Observables["WAIT_OUT"].Value = false;
                        conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)].Value = "";
                    }
                }
            }
            string palletId = (string)conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)].Value;
            if (string.IsNullOrEmpty(palletId) == false)
            {
                DoPalletMoveToAGV(conveyor, conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", index)]);
            }
        }

        private static void DoPalletMovedFromAGV(Conveyor conveyor, Observable obs)
        {
            int index = 0;
            int.TryParse(obs.ObservableId.Replace("PALLET_EXIST", ""), out index);
            if (conveyor.Observables.ContainsKey("PALLET_EXIST_FLAG_" + string.Format("{0:0#}", index)) == false)
            {
                return;
            }
            if ((bool)conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", index)].Value == false)
            {
                return;
            }
            Port currentPort = DeviceManager.GetPortInConveyorByIndex(conveyor.DeviceId, index.ToString());
            Port nextPort = null;
            if (currentPort == null)
            {
                return;
            }


            if (string.IsNullOrEmpty((string)obs.Value) == false)
            {
                currentPort.PortState = Port.PORT_STATE.MOVE_START;
                currentPort.StepCount = 0;
            }
        }

        private static void DoPalletMoveToAGV(Conveyor conveyor, Observable obs)
        {
            int index = 0;
            int.TryParse(obs.ObservableId.Replace("PALLET_EXIST", ""), out index);
            if (conveyor.Observables.ContainsKey("PALLET_EXIST_FLAG_" + string.Format("{0:0#}", index)) == false)
            {
                return;
            }
            if ((bool)conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", index)].Value == false)
            {
                return;
            }
            Port currentPort = DeviceManager.GetPortInConveyorByIndex(conveyor.DeviceId, index.ToString());
            Port nextPort = null;
            if (currentPort == null)
            {
                return;
            }


            if (string.IsNullOrEmpty((string)obs.Value) == false)
            {
                currentPort.PortState = Port.PORT_STATE.MOVE_START;
                currentPort.StepCount = 0;
            }
        }
        public static void DoPortLoadComplete(Conveyor conveyor, Port port)
        {

        }
        public static void DoPortMoveStart(Conveyor conveyor, Port port)
        {

            Port nextPort = GetNextPort(conveyor, port);
            if (IsInputConveyor(conveyor.DeviceId))
            {
                if (int.Parse(port.ObjectOrder) == conveyor.Ports.Count)
                {
                    conveyor.Observables["LOAD_COMPLETE_02"].Value = false;
                    conveyor.Observables["WAIT_IN"].Value = false;
                }
            }
            else
            {

                if (int.Parse(port.ObjectOrder) == 1)
                {
                    conveyor.Observables["LOAD_COMPLETE_01"].Value = false;
                    conveyor.Observables["WAIT_IN"].Value = false;
                }
            }

            if (port == nextPort)
            {
                port.PortState = Port.PORT_STATE.UNLOAD_COMPLETE;
                port.StepCount = 0;
                if (IsInputConveyor(conveyor.DeviceId))
                {
                    if (int.Parse(port.ObjectOrder) == conveyor.Ports.Count)
                    {
                        conveyor.Observables["LOAD_COMPLETE_02"].Value = false;
                        conveyor.Observables["WAIT_IN"].Value = false;
                    }
                    //if (port.DeviceId == "PORT:15")
                    //{
                    //    string palletId = (string)conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value;
                    //    Conveyor tConveyor = DeviceManager.GetConveyor("CONVEYOR:27");
                    //    Port tPort = DeviceManager.GetPortInConveyorByIndex("CONVEYOR:27", "4");
                    //    tConveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(tPort.ObjectOrder))].Value = palletId;
                    //    tConveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(tPort.ObjectOrder))].Value = true;

                    //    conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
                    //    conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = false;
                    //    port.PortState = Port.PORT_STATE.READY; 
                    //    //
                    //}
                    //else
                    //{
                        conveyor.Observables["WAIT_OUT"].Value = true;
                        conveyor.Observables["UNLOAD_COMPLETE_01"].Value = true;
                    //}
                }
                else
                {

                    conveyor.Observables["WAIT_OUT"].Value = true;
                    conveyor.Observables["UNLOAD_COMPLETE_02"].Value = true;

                }
            }
            else
            {
                port.PortState = Port.PORT_STATE.MOVING;
                port.StepCount = 0;
            }
        }
        public static void DoPortMoving(Conveyor conveyor, Port port)
        {
            Port nextPort = GetNextPort(conveyor, port);
            if (conveyor.Observables.ContainsKey("PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(nextPort.ObjectOrder))) == false)
            {
                return;
            }
            if ((bool)conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(nextPort.ObjectOrder))].Value == false)
            {
                port.StepCount = port.StepCount + 1;
            }
            if (port.StepCount > 30)
            {
                port.PortState = Port.PORT_STATE.MOVE_END;
            }

        }
        public static void DoPortMoveEnd(Conveyor conveyor, Port port)
        {
            Port nextPort = GetNextPort(conveyor, port);
            string palletId = (string)conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value;


            port.PortState = Port.PORT_STATE.READY;
            port.StepCount = 0;


            conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(nextPort.ObjectOrder))].Value = palletId;
            conveyor.Observables["PALLET_EXIST" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = "";
            conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(nextPort.ObjectOrder))].Value = true;
            conveyor.Observables["PALLET_EXIST_FLAG_" + string.Format("{0:0#}", int.Parse(port.ObjectOrder))].Value = false;

            if (port == nextPort)
            {
                if (IsInputConveyor(conveyor.DeviceId))
                {
                    //conveyor.Observables["LOAD_COMPLETE_02"].Value = false;
                    //conveyor.Observables["WAIT_IN"].Value = false;
                    conveyor.Observables["WAIT_OUT"].Value = true;
                    conveyor.Observables["UNLOAD_COMPLETE_01"].Value = true;
                }
                else
                {
                    //conveyor.Observables["LOAD_COMPLETE_01"].Value = false;
                    //conveyor.Observables["WAIT_IN"].Value = false;                    

                    conveyor.Observables["WAIT_OUT"].Value = true;
                    conveyor.Observables["UNLOAD_COMPLETE_02"].Value = true;

                }
            }



        }
        #endregion 제어 로직


    }
}
