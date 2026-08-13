using System;
using System.Collections.Generic;
using System.Text;
using ECP.Com.Tcp;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class Vehicle : Device
    {

        public enum VEHICLE_RUN_STATE
        {
            IDLE = 0,
            RUN = 1,
            CHARGE = 2,
            DOWN = 3,
            MANUAL = 4,
            EXIT_MAP = 5,
            OFFLINE = 6

        }
        public enum VEHICLE_STATE
        {
            READY = 1,
            REQUEST_TRANSFER, 
            START_TO_SOURCE,
            TRAVEL_TO_SOURCE,
            ARRIVED_AT_SOURCE,
            LOAD_START,
            LOADING,
            LOAD_COMPLETE,
            START_TO_DEST,
            TRAVEL_TO_DEST,
            ARRIVED_AT_DEST,
            UNLOAD_START,
            UNLOADING,
            UNLOAD_COMPLETE,
        }




        #region AGV 관련 속성

        private TcpClient clientSocket = null;

        public TcpClient ClientSocket
        {
            get
            {
                return clientSocket;
            }
            set
            {
                clientSocket = value;
            }
        }
        private VEHICLE_RUN_STATE vehicleRunState = VEHICLE_RUN_STATE.DOWN;

        public VEHICLE_RUN_STATE VehicleRunState
        {
            get
            {
                return vehicleRunState;
            }
            set
            {
                vehicleRunState = value;
            }
        }
        private void SendServer(string value)
        {
            if (clientSocket == null)
            {
                return;
            }
            if (clientSocket.Connected == false)
            {
                return;
            }
            this.clientSocket.SendText(value);
        }


        private string agvAlarmCode = "";
        public string AgvAlarmCode
        {
            get
            {
                return agvAlarmCode;
            }
            set
            {

                try
                {
                    if (string.IsNullOrEmpty(value))
                    {
                        ////Alarm Reset                        
                        //WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.AlarmResetReport_625);
                        //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
                        //command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
                        //command.AlarmSetTime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        //command.AlarmVehicleId = (DeviceId.Split(':')[1]).PadLeft(2, ' ');
                        //command.AlarmCode = this.agvAlarmCode.PadLeft(4, ' ');
                        //SendServer(command.BinaryString);
                    }
                    else
                    {
                        ////Alarm Set
                        //WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.AlarmSetReport_623);
                        //command.IsRequestReply = WMSCommand.REQUEST_REPLY.NEED_REPLY;
                        //command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
                        //command.AlarmSetTime = System.DateTime.Now.ToString("yyyyMMddHHmmssfff");
                        //command.AlarmVehicleId = (DeviceId.Split(':')[1]).PadLeft(2, ' ');
                        //command.AlarmCode = value.PadLeft(4, ' ');
                        //SendServer(command.BinaryString);
                    }
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("XXXXXXXXXX" + ex.StackTrace);
                }
                agvAlarmCode = value;
            }
        }
        private bool isAgvAlarmSet = false;
        public bool IsAgvAlarmSet
        {
            get
            {
                return isAgvAlarmSet;
            }
            set
            {
                isAgvAlarmSet = value;
            }
        }
        private string agvCommandId = "";
        public string AgvCommandId
        {
            get
            {
                return agvCommandId;
            }
            set
            {
                agvCommandId = value;
            }
        }
        private string agvCommandSeq = "";

        public string AgvCommandSeq
        {
            get
            {
                return agvCommandSeq;
            }
            set
            {
                agvCommandSeq = value;
            }
        }
        private string agvCommandDirection = "";

        public string AgvCommandDirection
        {
            get
            {
                return agvCommandDirection;
            }
            set
            {
                agvCommandDirection = value;
            }
        }
        private string agvCommandRequestReply = "";

        public string AgvCommandRequestReply
        {
            get
            {
                return agvCommandRequestReply;
            }
            set
            {
                agvCommandRequestReply = value;
            }
        }

        private string agvSapCommandId = "";

        public string AgvSapCommandId
        {
            get
            {
                return agvSapCommandId;
            }
            set
            {
                agvSapCommandId = value;
            }
        }
        private string agvSapSeq = "";

        public string AgvSapSeq
        {
            get
            {
                return agvSapSeq;
            }
            set
            {
                agvSapSeq = value;
            }
        }
        private string agvVehicleId = "";

        public string AgvVehicleId
        {
            get
            {
                return agvVehicleId;
            }
            set
            {
                agvVehicleId = value;
            }
        }
        private string agvPalletId = "";

        public string AgvPalletId
        {
            get
            {
                return agvPalletId;
            }
            set
            {
                agvPalletId = value;
            }
        }
        private string agvFrom = "";

        public string AgvFrom
        {
            get
            {
                return agvFrom;
            }
            set
            {
                agvFrom = value;
            }
        }
        private string agvTo = "";

        public string AgvTo
        {
            get
            {
                return agvTo;
            }
            set
            {
                agvTo = value;
            }
        }
        private string agvPriority = "";

        public string AgvPriority
        {
            get
            {
                return agvPriority;
            }
            set
            {
                agvPriority = value;
            }
        }

        private string agvLocation = "";

        public string AgvLocation
        {
            get
            {
                return agvLocation;
            }
            set
            {
                agvLocation = value;
            }
        }
        private string agvPrevLocation = "";

        public string AgvPrevLocation
        {
            get
            {
                return agvPrevLocation;
            }
            set
            {
                agvPrevLocation = value;
            }
        }

        private string agvMarkPlate = "";

        public string AgvMarkPlate
        {
            get
            {
                return agvMarkPlate;
            }
            set
            {
                agvMarkPlate = value;
            }
        }

        private string agvPrevMarkPlate = "";

        public string AgvPrevMarkPlate
        {
            get
            {
                return agvPrevMarkPlate;
            }
            set
            {
                agvPrevMarkPlate = value;
            }
        }

        private string agvFromPath = "";

        public string AgvFromPath
        {
            get
            {
                return agvFromPath;
            }
            set
            {
                agvFromPath = value;
            }
        }
        private string agvToPath = "";

        public string AgvToPath
        {
            get
            {
                return agvToPath;
            }
            set
            {
                agvToPath = value;
            }
        }


        private MarkPlate agvFromMarkplate = null;

        public MarkPlate AgvFromMarkplate
        {
            get
            {
                return agvFromMarkplate;
            }
            set
            {
                agvFromMarkplate = value;
            }
        }
        private MarkPlate agvToMarkplate = null;
        public MarkPlate AgvToMarkplate
        {
            get
            {
                return agvToMarkplate;
            }
            set
            {
                agvToMarkplate = value;
            }
        }

        private int agvTravelSourceStepCount = 0;

        public int AgvTravelSourceStepCount
        {
            get
            {
                return agvTravelSourceStepCount;
            }
            set
            {
                agvTravelSourceStepCount = value;
            }
        }
        private int agvTravalDestinationStepCount = 0;

        public int AgvTravalDestinationStepCount
        {
            get
            {
                return agvTravalDestinationStepCount;
            }
            set
            {
                agvTravalDestinationStepCount = value;
            }
        }

        #endregion

        private VEHICLE_STATE vehicleState = VEHICLE_STATE.READY;
        public VEHICLE_STATE VehicleState
        {
            get
            {
                return vehicleState;
            }
            set
            {
                vehicleState = value;
                if (value == VEHICLE_STATE.READY)
                {
                    this.stepCount = 0;
                }
            }
        }

        private int stepCount = 0;
        public int StepCount
        {
            get
            {
                return stepCount;
            }
            set
            {
                stepCount = value;
            }
        }

        public override void Initialize()
        {
            base.Initialize();
            if (this.ownerid.Contains("AGVSEM"))
            {
                this.vehicleRunState = VEHICLE_RUN_STATE.IDLE;
                this.agvAlarmCode = "";
            }
            else
            {
                if (this.Observables.ContainsKey("SUBSYSTEM_STATUS"))
                {
                    this.Observables["SUBSYSTEM_STATUS"].Value = 1;
                }
                if (this.Observables.ContainsKey("SUBSYSTEM_LOCATION_01"))
                {
                    this.Observables["SUBSYSTEM_LOCATION_01"].Value = GetDefaultCurrentLocation("01");
                    this.Observables["SUBSYSTEM_LOCATION_01"].Value = GetDefaultCurrentLocation("02");
                    this.Observables["SUBSYSTEM_LOCATION_01"].Value = GetDefaultCurrentLocation("03");
                }
            }
        }

        private string GetDefaultCurrentLocation(string locationIndex)
        {
            switch (locationIndex)
            {
                case "01":
                    if (ownerid.Contains("LFTSEM") || ownerid.Contains("RGVSEM"))
                    {
                        return "00";
                    }
                    break;
                case "02":
                    if (ownerid.Contains("LFTSEM") || ownerid.Contains("RGVSEM"))
                    {
                        return "00";
                    }
                    break;
                case "03":
                    if (ownerid.Contains("STK"))
                    {
                        return "00";
                    }
                    break;
            }
            return "00";
        }

    }
}
