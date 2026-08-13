using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;

namespace HECS.Device.Unit
{
    
    public partial class Vehicle
    {
        #region Properties for Observable Access
        public Observable @__IO_ALARM_RESET_CODE
        {
            get
            {
                return this.Observables["ALARM_RESET_CODE"];
            }
        }
        public Observable @__IO_ALARM_RESET_REPORT
        {
            get
            {
                return this.Observables["ALARM_RESET_REPORT"];
            }
        }
        public Observable @__IO_ALARM_SET_CODE
        {
            get
            {
                return this.Observables["ALARM_SET_CODE"];
            }
        }
        public Observable @__IO_ALARM_SET_REPORT
        {
            get
            {
                return this.Observables["ALARM_SET_REPORT"];
            }
        }
        public Observable @__IO_LOAD_COMPLETE
        {
            get
            {
                return this.Observables["LOAD_COMPLETE"];
            }
        }
        public Observable @__IO_OPERATION_MODE
        {
            get
            {
                return this.Observables["OPERATION_MODE"];
            }
        }
        public Observable @__IO_PALLET_EXIST_FLAG
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG"];
            }
        }
        public Observable @__IO_SUBSYSTEM_LOCATION_01
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_01"];
            }
        }
        public Observable @__IO_SUBSYSTEM_LOCATION_02
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_02"];
            }
        }
        public Observable @__IO_SUBSYSTEM_LOCATION_03
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_03"];
            }
        }
        public Observable @__IO_SUBSYSTEM_STATUS
        {
            get
            {
                return this.Observables["SUBSYSTEM_STATUS"];
            }
        }
        public Observable @__IO_TRANSFER_ACK
        {
            get
            {
                return this.Observables["TRANSFER_ACK"];
            }
        }
        public Observable @__IO_TRANSFER_COMPLETE_LOCATION_01
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_01"];
            }
        }
        public Observable @__IO_TRANSFER_COMPLETE_LOCATION_02
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_02"];
            }
        }
        public Observable @__IO_TRANSFER_COMPLETE_LOCATION_03
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_03"];
            }
        }
        public Observable @__IO_UNLOAD_COMPLETE
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE"];
            }
        }
        public Observable @__IO_FROM_01
        {
            get
            {
                return this.Observables["FROM_01"];
            }
        }
        public Observable @__IO_FROM_02
        {
            get
            {
                return this.Observables["FROM_02"];
            }
        }
        public Observable @__IO_FROM_03
        {
            get
            {
                return this.Observables["FROM_03"];
            }
        }
        public Observable @__IO_TO_01
        {
            get
            {
                return this.Observables["TO_01"];
            }
        }
        public Observable @__IO_TO_02
        {
            get
            {
                return this.Observables["TO_02"];
            }
        }
        public Observable @__IO_TO_03
        {
            get
            {
                return this.Observables["TO_03"];
            }
        }
        public Observable @__IO_PALLET_ID
        {
            get
            {
                return this.Observables["PALLET_ID"];
            }
        }
        public Observable @__IO_PALLET_ON_VEHICLE
        {
            get
            {
                return this.Observables["PALLET_ON_VEHICLE"];
            }
        }
        public Observable @__IO_TRANSFER_REQUEST
        {
            get
            {
                return this.Observables["TRANSFER_REQUEST"];
            }
        }
        public Observable @__IO_LOAD_COMPLETE_ACK
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_ACK"];
            }
        }
        public Observable @__IO_UNLOAD_COMPLETE_ACK
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_ACK"];
            }
        }
        public Observable @__IO_ALARM_SET_REPORT_ACK
        {
            get
            {
                return this.Observables["ALARM_SET_REPORT_ACK"];
            }
        }
        public Observable @__IO_ALARM_RESET_REPORT_ACK
        {
            get
            {
                return this.Observables["ALARM_RESET_REPORT_ACK"];
            }
        }

        #endregion

        #region Properties for Observable Access By Value
        public short IO_ALARM_RESET_CODE
        {
            get
            {
                return this.Observables["ALARM_RESET_CODE"].AsShort ;
            }
            set
            {
                this.Observables["ALARM_RESET_CODE"].AsShort = value;
            }
        }
        public bool IO_ALARM_RESET_REPORT
        {
            get
            {
                return this.Observables["ALARM_RESET_REPORT"].AsBoolean ;
            }
            set
            {
                this.Observables["ALARM_RESET_REPORT"].AsBoolean  = value;
            }
        }
        public short IO_ALARM_SET_CODE
        {
            get
            {
                return this.Observables["ALARM_SET_CODE"].AsShort ;
            }
            set
            {
                this.Observables["ALARM_SET_CODE"].AsShort = value;
            }
        }
        public bool IO_ALARM_SET_REPORT
        {
            get
            {
                return this.Observables["ALARM_SET_REPORT"].AsBoolean ;
            }
            set
            {
                this.Observables["ALARM_SET_REPORT"].AsBoolean = value;
            }
        }
        public bool IO_LOAD_COMPLETE
        {
            get
            {
                return this.Observables["LOAD_COMPLETE"].AsBoolean ;
            }
            set
            {
                this.Observables["LOAD_COMPLETE"].AsBoolean = value;
            }
        }
        public short IO_OPERATION_MODE
        {
            get
            {
                return this.Observables["OPERATION_MODE"].AsShort ;
            }
            set
            {
                this.Observables["OPERATION_MODE"].AsShort = value;
            }
        }
        public bool IO_PALLET_EXIST_FLAG
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG"].AsBoolean ;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG"].AsBoolean = value;
            }
        }
        public string IO_SUBSYSTEM_LOCATION_01
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_01"].AsString;
            }
            set
            {
                this.Observables["SUBSYSTEM_LOCATION_01"].AsString = value;
            }
        }
        public string IO_SUBSYSTEM_LOCATION_02
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_02"].AsString;
            }
            set
            {
                this.Observables["SUBSYSTEM_LOCATION_02"].AsString = value;
            }
        }
        public string IO_SUBSYSTEM_LOCATION_03
        {
            get
            {
                return this.Observables["SUBSYSTEM_LOCATION_03"].AsString;
            }
            set
            {
                this.Observables["SUBSYSTEM_LOCATION_03"].AsString = value;
            }
        }
        public short  IO_SUBSYSTEM_STATUS
        {
            get
            {
                return this.Observables["SUBSYSTEM_STATUS"].AsShort ;
            }
            set
            {
                this.Observables["SUBSYSTEM_STATUS"].AsShort = value;
            }
        }
        public bool IO_TRANSFER_ACK
        {
            get
            {
                return this.Observables["TRANSFER_ACK"].AsBoolean ;
            }
            set
            {
                this.Observables["TRANSFER_ACK"].AsBoolean = value;
            }
        }
        public string IO_TRANSFER_COMPLETE_LOCATION_01
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_01"].AsString;
            }
            set
            {
                this.Observables["TRANSFER_COMPLETE_LOCATION_01"].AsString = value;
            }
        }
        public string IO_TRANSFER_COMPLETE_LOCATION_02
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_02"].AsString;
            }
            set
            {
                this.Observables["TRANSFER_COMPLETE_LOCATION_02"].AsString = value;
            }
        }
        public string IO_TRANSFER_COMPLETE_LOCATION_03
        {
            get
            {
                return this.Observables["TRANSFER_COMPLETE_LOCATION_03"].AsString;
            }
            set
            {
                this.Observables["TRANSFER_COMPLETE_LOCATION_03"].AsString = value;
            }
        }
        public bool IO_UNLOAD_COMPLETE
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE"].AsBoolean ;
            }
            set
            {
                this.Observables["UNLOAD_COMPLETE"].AsBoolean = value;
            }
        }

        public string IO_FROM_01
        {
            get
            {
                return this.Observables["FROM_01"].AsString;
            }
            set
            {
                this.Observables["FROM_01"].AsString = value;
            }
        }
        public string IO_FROM_02
        {
            get
            {
                return this.Observables["FROM_02"].AsString;
            }
            set
            {
                this.Observables["FROM_02"].AsString = value;
            }
        }
        public string IO_FROM_03
        {
            get
            {
                return this.Observables["FROM_03"].AsString;
            }
            set
            {
                this.Observables["FROM_03"].AsString = value;
            }
        }
        public string IO_TO_01
        {
            get
            {
                return this.Observables["TO_01"].AsString;
            }
            set
            {
                this.Observables["TO_01"].AsString = value;
            }
        }
        public string IO_TO_02
        {
            get
            {
                return this.Observables["TO_02"].AsString;
            }
            set
            {
                this.Observables["TO_02"].AsString = value;
            }
        }
        public string IO_TO_03
        {
            get
            {
                return this.Observables["TO_03"].AsString;
            }
            set
            {
                this.Observables["TO_03"].AsString = value;
            }
        }
        public string IO_PALLET_ID
        {
            get
            {
                return this.Observables["PALLET_ID"].AsString;
            }
            set
            {
                this.Observables["PALLET_ID"].AsString = value;
            }
        }
        public string IO_PALLET_ON_VEHICLE
        {
            get
            {
                return this.Observables["PALLET_ON_VEHICLE"].AsString;
            }
            set
            {
                this.Observables["PALLET_ON_VEHICLE"].AsString = value;
            }
        }
        public bool IO_TRANSFER_REQUEST
        {
            get
            {
                return this.Observables["TRANSFER_REQUEST"].AsBoolean ;
            }
            set
            {
                this.Observables["TRANSFER_REQUEST"].AsBoolean = value;
            }
        }
        public bool IO_LOAD_COMPLETE_ACK
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_ACK"].AsBoolean;
            }
            set
            {
                this.Observables["LOAD_COMPLETE_ACK"].AsBoolean = value;
            }
        }
        public bool  IO_UNLOAD_COMPLETE_ACK
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_ACK"].AsBoolean;
            }
            set
            {
                this.Observables["UNLOAD_COMPLETE_ACK"].AsBoolean = value;
            }
        }
        public bool IO_ALARM_SET_REPORT_ACK
        {
            get
            {
                return this.Observables["ALARM_SET_REPORT_ACK"].AsBoolean;
            }
            set
            {
                this.Observables["ALARM_SET_REPORT_ACK"].AsBoolean = value;
            }
        }
        public bool IO_ALARM_RESET_REPORT_ACK
        {
            get
            {
                return this.Observables["ALARM_RESET_REPORT_ACK"].AsBoolean;
            }
            set
            {
                this.Observables["ALARM_RESET_REPORT_ACK"].AsBoolean = value;
            }
        }

        #endregion                                         

    }
}
