using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Device.Dispatch
{
    public class WorkState
    {
        #region Attribute
        public static String[] displayStrings = new String[14];

        public static int _REQUESTED = 0;
        public static WorkState REQUESTED = new WorkState(_REQUESTED);

        public static int _READY = 1;
        public static WorkState READY = new WorkState(_READY);

        public static int _REMOTE_COMMAND_SENT = 2;
        public static WorkState REMOTE_COMMAND_SENT = new WorkState(_REMOTE_COMMAND_SENT);

        public static int _PROCESSING = 3;
        public static WorkState PROCESSING = new WorkState(_PROCESSING);

        public static int _PROCESSED = 4;
        public static WorkState PROCESSED = new WorkState(_PROCESSED);

        public static int _IT_ENUM_MAX = 5;
        public static WorkState IT_ENUM_MAX = new WorkState(_IT_ENUM_MAX);
        #endregion Attribute

        #region Constructor
        static WorkState()
        {
            displayStrings[0] = "Requested";
            displayStrings[1] = "Ready";
            displayStrings[2] = "RemoteCommandSend";
            displayStrings[3] = "Processing";
            displayStrings[4] = "Processed";

        }
        #endregion Constructor

        #region Method
        public int Value()
        {
            return ___value;
        }

        public override string ToString()
        {
            return displayStrings[___value];
        }

        private WorkState(int value)
        {
            ___value = value;
        }

        private int ___value;

        public bool Equals(WorkState otherEnum)
        {
            if (otherEnum.Value() == ___value)
            {
                return true;
            }
            else
            {
                return false;
            }

        }
        #endregion Method
    }
}
