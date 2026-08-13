using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.Device.Unit
{
    [Serializable]
    public partial class Vehicle
    {
        public enum AGV_RUN_STATE
        {
            IDLE = 0,
            RUN = 1,
            CHARGE = 2,
            DOWN = 3,
            MANUAL = 4,
            EXIT_MAP = 5,
            OFFLINE = 6

        }
        public enum AGV_TRANSFER_STATE
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
        #region AGV Property
        private string agvBinaryCommandId = "";

        public string AgvBinaryCommandId
        {
            get
            {
                return agvBinaryCommandId;
            }
            set
            {
                agvBinaryCommandId = value;
            }
        }
        private AGV_RUN_STATE agvVehicleState = AGV_RUN_STATE.DOWN;

        public AGV_RUN_STATE AgvVehicleState
        {
            get
            {
                return agvVehicleState;
            }
            set
            {
                agvVehicleState = value;
                OnPropertyChanged("AgvVehicleState", value);
            }
        }
        private int agvCommandResetCount = 0;

        public int AgvCommandResetCount
        {
            get
            {
                return agvCommandResetCount;
            }
            set
            {
                agvCommandResetCount = value;
            }
        }
        private AGV_TRANSFER_STATE agvTransferState = AGV_TRANSFER_STATE.READY;

        public AGV_TRANSFER_STATE AgvTransferState
        {
            get
            {
                return agvTransferState;
            }
            set
            {
                agvTransferState = value;
                OnPropertyChanged("AgvTransferState", value);
            }
        }
       
        private string agvAlarmCode = "";
        public string AgvAlarmCode
        {
            get
            {
                return agvAlarmCode.Trim();
            }
            set
            {
                agvAlarmCode = value == null ? null : value.Trim();
                OnPropertyChanged("AgvAlarmCode", value);
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
                OnPropertyChanged("IsAgvAlarmSet", value);
            }
        }
        private string agvCommandId = "";
        public string AgvCommandId
        {
            get
            {
                return agvCommandId.Trim();
                
            }
            set
            {
                agvCommandId = value == null ? null : value.Trim();
                OnPropertyChanged("AgvCommandId", value);
            }
        }
        private string agvCommandSeq = "";

        public string AgvCommandSeq
        {
            get
            {
                return agvCommandSeq.Trim();
            }
            set
            {
                agvCommandSeq = value == null ? null : value.Trim();
                OnPropertyChanged("AgvCommandSeq", value);
            }
        }
        private string agvCommandDirection = "";

        public string AgvCommandDirection
        {
            get
            {
                return agvCommandDirection.Trim();
            }
            set
            {
                agvCommandDirection = value;
                OnPropertyChanged("AgvCommandDirection", value);
            }
        }
        private string agvCommandRequestReply = "";

        public string AgvCommandRequestReply
        {
            get
            {
                return agvCommandRequestReply.Trim();
            }
            set
            {
                agvCommandRequestReply = value;
                OnPropertyChanged("AgvCommandRequestReply", value);
            }
        }

        private string agvSapCommandId = "";

        public string AgvSapCommandId
        {
            get
            {
                return agvSapCommandId.Trim();
            }
            set
            {
                agvSapCommandId = value == null ? null : value.Trim();
                OnPropertyChanged("AgvSapCommandId", value);
            }
        }
        private string agvSapSeq = "";

        public string AgvSapSeq
        {
            get
            {
                return agvSapSeq.Trim();
            }
            set
            {
                agvSapSeq = value == null ? null : value.Trim();
                OnPropertyChanged("AgvSapSeq", value);
            }
        }
        private string agvVehicleId = "";

        public string AgvVehicleId
        {
            get
            {
                return agvVehicleId.Trim();
            }
            set
            {
                agvVehicleId = value == null ? null : value.Trim();
                OnPropertyChanged("AgvVehicleId", value);
            }
        }
        private string agvPalletId = "";

        public string AgvPalletId
        {
            get
            {
                return agvPalletId.Trim();
            }
            set
            {
                agvPalletId = value == null ? null : value.Trim();
                OnPropertyChanged("AgvPalletId", value);
            }
        }

        private string agvPalletIdForTransfer;

        public string AgvPalletIdForTransfer
        {
            get
            {
                return agvPalletIdForTransfer;
            }
            set
            {
                agvPalletIdForTransfer = value == null ? null : value.Trim();
                OnPropertyChanged("AgvPalletIdForTransfer", value);
            }
        }


        private string agvFrom = "";

        public string AgvFrom
        {
            get
            {
                return agvFrom.Trim();
            }
            set
            {
                agvFrom = value == null ? null : value.Trim();
                OnPropertyChanged("AgvFrom", value);
            }
        }
        private string agvTo = "";

        public string AgvTo
        {
            get
            {
                return agvTo.Trim();
            }
            set
            {
                agvTo = value == null ? null : value.Trim();
                OnPropertyChanged("AgvTo", value);
            }
        }
        private string agvPriority = "";

        public string AgvPriority
        {
            get
            {
                return agvPriority.Trim();
            }
            set
            {
                agvPriority = value == null ? null : value.Trim();
                OnPropertyChanged("AgvPriority", value);
            }
        }

        private string agvLocation = "";

        public string AgvLocation
        {
            get
            {
                return agvLocation.Trim();
            }
            set
            {
                agvLocation = value == null ? null : value.Trim();
                OnPropertyChanged("AgvLocation", value);
            }
        }
        private string agvPrevLocation = "";

        public string AgvPrevLocation
        {
            get
            {
                return agvPrevLocation.Trim();
            }
            set
            {
                agvPrevLocation = value == null ? null : value.Trim();
                OnPropertyChanged("AgvPrevLocation", value);
            }
        }

        private string agvMarkPlate = "";

        public string AgvMarkPlate
        {
            get
            {
                return agvMarkPlate.Trim();
            }
            set
            {

                agvMarkPlate = value == null ? null : value.Trim();
             
                OnPropertyChanged("AgvMarkPlate", value);
            }
        }

        private string agvPrevMarkPlate = "";
        public string AgvPrevMarkPlate
        {
            get
            {
                return agvPrevMarkPlate.Trim();
            }
            set
            {
                agvPrevMarkPlate = value == null ? null : value.Trim();
                OnPropertyChanged("AgvPrevMarkPlate", value);
            }
        }

        private string agvFromPath = "";

        public string AgvFromPath
        {
            get
            {
                return agvFromPath.Trim();
            }
            set
            {
                agvFromPath = value == null ? null : value.Trim();
                OnPropertyChanged("AgvFromPath", value);
            }
        }
        private string agvToPath = "";

        public string AgvToPath
        {
            get
            {
                return agvToPath.Trim();
            }
            set
            {
                agvToPath = value == null ? null : value.Trim();
                OnPropertyChanged("AgvToPath", value);
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
                OnPropertyChanged("AgvFromMarkplate", value);
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
                OnPropertyChanged("AgvToMarkplate", value);
            }
        }

       
        #endregion
            
    }
}
