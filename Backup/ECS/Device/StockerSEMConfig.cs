using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;

namespace HECS.Device
{
  
    public partial class StockerSEM
    {

        #region Properties for Observable Access
        public Observable @__O_HOME_SAFETY_SENSOR_NOT_USED_REQ
        {
            get
            {
                return this.Observables["HOME_SAFETY_SENSOR_NOT_USED_REQ"];
            }
        }
        public Observable @__I_HOME_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["HOME_SAFETY_SENSOR_USED"];
            }
        }
        public Observable @__O_HOME_SAFETY_SENSOR_USED_REQ
        {
            get
            {
                return this.Observables["HOME_SAFETY_SENSOR_USED_REQ"];
            }
        }
        public Observable @__O_INLINE_SAFETY_SENSOR_NOT_USED_REQ
        {
            get
            {
                return this.Observables["INLINE_SAFETY_SENSOR_NOT_USED_REQ"];
            }
        }
        public Observable @__I_INLINE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["INLINE_SAFETY_SENSOR_USED"];
            }
        }
        public Observable @__O_INLINE_SAFETY_SENSOR_USED_REQ
        {
            get
            {
                return this.Observables["INLINE_SAFETY_SENSOR_USED_REQ"];
            }
        }
        public Observable @__O_MIDDLE_SAFETY_SENSOR_NOT_USED_REQ
        {
            get
            {
                return this.Observables["MIDDLE_SAFETY_SENSOR_NOT_USED_REQ"];
            }
        }
        public Observable @__I_MIDDLE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["MIDDLE_SAFETY_SENSOR_USED"];
            }
        }
        public Observable @__O_MIDDLE_SAFETY_SENSOR_USED_REQ
        {
            get
            {
                return this.Observables["MIDDLE_SAFETY_SENSOR_USED_REQ"];
            }
        }
        public Observable @__O_ONLIN_OFFLINE
        {
            get
            {
                return this.Observables["ONLIN_OFFLINE"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGE_ENABLE_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T1"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGE_ENABLE_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T2"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGE_ENABLE_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T3"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGE_ENABLE_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T4"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGING_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T1"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGING_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T2"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGING_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T3"];
            }
        }
        public Observable @__I_OPERATION_MODE_CHANGING_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T4"];
            }
        }
        public Observable @__I_OPERATION_MODE_NOT_CHANGED_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T1"];
            }
        }
        public Observable @__I_OPERATION_MODE_NOT_CHANGED_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T2"];
            }
        }
        public Observable @__I_OPERATION_MODE_NOT_CHANGED_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T3"];
            }
        }
        public Observable @__I_OPERATION_MODE_NOT_CHANGED_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T4"];
            }
        }
        public Observable @__O_OPPOSITE_SAFETY_SENSOR_NOT_USED_REQ
        {
            get
            {
                return this.Observables["OPPOSITE_SAFETY_SENSOR_NOT_USED_REQ"];
            }
        }
        public Observable @__I_OPPOSITE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["OPPOSITE_SAFETY_SENSOR_USED"];
            }
        }
        public Observable @__O_OPPOSITE_SAFETY_SENSOR_USED_REQ
        {
            get
            {
                return this.Observables["OPPOSITE_SAFETY_SENSOR_USED_REQ"];
            }
        }
        public Observable @__O_PORT_SAFETY_SENSOR_NOT_USED_REQ
        {
            get
            {
                return this.Observables["PORT_SAFETY_SENSOR_NOT_USED_REQ"];
            }
        }
        public Observable @__I_PORT_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["PORT_SAFETY_SENSOR_USED"];
            }
        }
        public Observable @__O_PORT_SAFETY_SENSOR_USED_REQ
        {
            get
            {
                return this.Observables["PORT_SAFETY_SENSOR_USED_REQ"];
            }
        }
        public Observable @__O_PORT1_OPERATION_MODE_CHANGE_REQ
        {
            get
            {
                return this.Observables["PORT1_OPERATION_MODE_CHANGE_REQ"];
            }
        }
        public Observable @__O_PORT1_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT1_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__O_PORT2_OPERATION_MODE_CHANGE_REQ
        {
            get
            {
                return this.Observables["PORT2_OPERATION_MODE_CHANGE_REQ"];
            }
        }
        public Observable @__O_PORT2_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT2_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__O_PORT3_OPERATION_MODE_CHANGE_REQ
        {
            get
            {
                return this.Observables["PORT3_OPERATION_MODE_CHANGE_REQ"];
            }
        }
        public Observable @__O_PORT3_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT3_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__O_PORT4_OPERATION_MODE_CHANGE_REQ
        {
            get
            {
                return this.Observables["PORT4_OPERATION_MODE_CHANGE_REQ"];
            }
        }
        public Observable @__O_PORT4_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT4_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__O_STOCKER_FULL
        {
            get
            {
                return this.Observables["STOCKER_FULL"];
            }
        }
        public Observable @__O_XMOVE_CONTROL
        {
            get
            {
                return this.Observables["XMOVE_CONTROL"];
            }
        }

        public Observable @__O_AROUND_PORT_DOWN_CONTROL
        {
            get
            {
                return this.Observables["AROUND_PORT_DOWN_CONTROL"];
            }
        }

        public Observable @__I_PORT1_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT1_CURRENT_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__I_PORT2_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT2_CURRENT_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__I_PORT3_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT3_CURRENT_OPERATION_MODE_STATE"];
            }
        }
        public Observable @__I_PORT4_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT4_CURRENT_OPERATION_MODE_STATE"];
            }
        }

        public Observable @__I_T_TYPE_PORT1_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT1_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT2_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT3_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT4_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT1_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT2_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT3_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT4_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT1_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT2_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT3_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT4_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT1_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT2_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT3_DIRECTION_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT4_DIRECTION_STATE"];
            }
        }

        public Observable @__I_T_TYPE_PORT1_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT1_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT2_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT3_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT1_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT4_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT1_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT2_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT3_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT2_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT4_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT1_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT2_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT3_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT3_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT4_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT1_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT2_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT3_RUN_STATE"];
            }
        }
        public Observable @__I_T_TYPE_PORT4_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT4_RUN_STATE"];
            }
        }
        #endregion

        #region Properties for Observable Access By Value

        public bool O_HOME_SAFETY_SENSOR_NOT_USED_REQ
        {
            set
            {
                this.Observables["HOME_SAFETY_SENSOR_NOT_USED_REQ"].AsBoolean = value;
            }
        }
        public bool I_HOME_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["HOME_SAFETY_SENSOR_USED"].AsBoolean;
            }
        }
        public bool O_HOME_SAFETY_SENSOR_USED_REQ
        {
            set
            {
                this.Observables["HOME_SAFETY_SENSOR_USED_REQ"].AsBoolean = value;
            }
        }
        public bool O_INLINE_SAFETY_SENSOR_NOT_USED_REQ
        {
            set
            {
                this.Observables["INLINE_SAFETY_SENSOR_NOT_USED_REQ"].AsBoolean = value;
            }
        }
        public bool I_INLINE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["INLINE_SAFETY_SENSOR_USED"].AsBoolean;
            }
        }
        public bool O_INLINE_SAFETY_SENSOR_USED_REQ
        {
            set
            {
                this.Observables["INLINE_SAFETY_SENSOR_USED_REQ"].AsBoolean = value;
            }
        }
        public bool O_MIDDLE_SAFETY_SENSOR_NOT_USED_REQ
        {
            set
            {
                this.Observables["MIDDLE_SAFETY_SENSOR_NOT_USED_REQ"].AsBoolean = value;
            }
        }
        public bool I_MIDDLE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["MIDDLE_SAFETY_SENSOR_USED"].AsBoolean;
            }
        }
        public bool O_MIDDLE_SAFETY_SENSOR_USED_REQ
        {
            set
            {
                this.Observables["MIDDLE_SAFETY_SENSOR_USED_REQ"].AsBoolean = value;
            }
        }
        public bool O_ONLIN_OFFLINE
        {
            set
            {
                this.Observables["ONLIN_OFFLINE"].AsBoolean = value;
            }
        }
        public bool I_OPERATION_MODE_CHANGE_ENABLE_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T1"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGE_ENABLE_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T2"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGE_ENABLE_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T3"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGE_ENABLE_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGE_ENABLE_T4"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGING_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T1"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGING_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T2"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGING_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T3"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_CHANGING_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_CHANGING_T4"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_NOT_CHANGED_T1
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T1"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_NOT_CHANGED_T2
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T2"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_NOT_CHANGED_T3
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T3"].AsBoolean;
            }
        }
        public bool I_OPERATION_MODE_NOT_CHANGED_T4
        {
            get
            {
                return this.Observables["OPERATION_MODE_NOT_CHANGED_T4"].AsBoolean;
            }
        }
        public bool O_OPPOSITE_SAFETY_SENSOR_NOT_USED_REQ
        {
            set
            {
                this.Observables["OPPOSITE_SAFETY_SENSOR_NOT_USED_REQ"].AsBoolean = value;
            }
        }
        public bool I_OPPOSITE_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["OPPOSITE_SAFETY_SENSOR_USED"].AsBoolean;
            }
        }
        public bool O_OPPOSITE_SAFETY_SENSOR_USED_REQ
        {
            set
            {
                this.Observables["OPPOSITE_SAFETY_SENSOR_USED_REQ"].AsBoolean = value;
            }
        }
        public bool O_PORT_SAFETY_SENSOR_NOT_USED_REQ
        {
            set
            {
                this.Observables["PORT_SAFETY_SENSOR_NOT_USED_REQ"].AsBoolean = value;
            }
        }
        public bool I_PORT_SAFETY_SENSOR_USED
        {
            get
            {
                return this.Observables["PORT_SAFETY_SENSOR_USED"].AsBoolean;
            }
        }
        public bool O_PORT_SAFETY_SENSOR_USED_REQ
        {
            set
            {
                this.Observables["PORT_SAFETY_SENSOR_USED_REQ"].AsBoolean = value;
            }
        }
        public bool O_PORT1_OPERATION_MODE_CHANGE_REQ
        {
            set
            {
                this.Observables["PORT1_OPERATION_MODE_CHANGE_REQ"].AsBoolean = value;
            }
        }
        public short O_PORT1_OPERATION_MODE_STATE
        {
            set
            {
                this.Observables["PORT1_OPERATION_MODE_STATE"].AsShort = value;
            }
        }
        public bool O_PORT2_OPERATION_MODE_CHANGE_REQ
        {
            set
            {
                this.Observables["PORT2_OPERATION_MODE_CHANGE_REQ"].AsBoolean = value;
            }
        }
        public short O_PORT2_OPERATION_MODE_STATE
        {
            set
            {
                this.Observables["PORT2_OPERATION_MODE_STATE"].AsShort = value;
            }
        }
        public bool O_PORT3_OPERATION_MODE_CHANGE_REQ
        {
            set
            {
                this.Observables["PORT3_OPERATION_MODE_CHANGE_REQ"].AsBoolean = value;
            }
        }
        public short O_PORT3_OPERATION_MODE_STATE
        {
            set
            {
                this.Observables["PORT3_OPERATION_MODE_STATE"].AsShort = value;
            }
        }
        public bool O_PORT4_OPERATION_MODE_CHANGE_REQ
        {
            set
            {
                this.Observables["PORT4_OPERATION_MODE_CHANGE_REQ"].AsBoolean = value;
            }
        }
        public short O_PORT4_OPERATION_MODE_STATE
        {
            set
            {
                this.Observables["PORT4_OPERATION_MODE_STATE"].AsShort = value;
            }
        }
        public bool O_STOCKER_FULL
        {
            set
            {
                this.Observables["STOCKER_FULL"].AsBoolean = value;
            }
        }
        public bool O_XMOVE_CONTROL
        {
            set
            {
                this.Observables["XMOVE_CONTROL"].AsBoolean = value;
            }
        }

        public bool O_AROUND_PORT_DOWN_CONTROL
        {
            set
            {
                this.Observables["AROUND_PORT_DOWN_CONTROL"].AsBoolean = value;
            }
        }

        public short I_PORT1_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT1_CURRENT_OPERATION_MODE_STATE"].AsShort;
            }
        }
        public short I_PORT2_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT2_CURRENT_OPERATION_MODE_STATE"].AsShort;
            }
        }
        public short I_PORT3_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT3_CURRENT_OPERATION_MODE_STATE"].AsShort;
            }
        }
        public short I_PORT4_CURRENT_OPERATION_MODE_STATE
        {
            get
            {
                return this.Observables["PORT4_CURRENT_OPERATION_MODE_STATE"].AsShort;
            }
        }

        public short I_T_TYPE_PORT1_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT1_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT2_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT3_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT4_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT1_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT2_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT3_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT4_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT1_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT2_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT3_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT4_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT1_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT1_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT2_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT2_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT3_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT3_DIRECTION_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT4_DIRECTION_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT4_DIRECTION_STATE"].AsShort;
            }
        }

        public short I_T_TYPE_PORT1_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT1_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT2_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT3_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT1_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT1_NEXT4_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT1_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT2_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT3_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT2_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT2_NEXT4_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT1_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT2_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT3_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT3_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT3_NEXT4_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT1_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT1_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT2_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT2_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT3_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT3_RUN_STATE"].AsShort;
            }
        }
        public short I_T_TYPE_PORT4_NEXT4_RUN_STATE
        {
            get
            {
                return this.Observables["T_TYPE_PORT4_NEXT4_RUN_STATE"].AsShort;
            }
        }
        #endregion
    }
}
