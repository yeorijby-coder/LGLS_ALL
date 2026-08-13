using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace HECS.Device.Dispatcher
{
    public class TransferState : MarshalByRefObject
    {
        /// <summary>
        /// Transfer State Model Define.
        /// </summary>
        private int value;
        private static int _size = 14;

        private static TransferState[] _array = new TransferState[_size];
        public static string[] displayStrings = new String[_size];
        public static TransferState[] displayValues = new TransferState[_size];


        public const int _NONE = 0;
        public static TransferState NONE = new TransferState(_NONE);

        public const int _QUEUED = 1;
        public static TransferState QUEUED = new TransferState(_QUEUED);

        public const int _READY = 2;
        public static TransferState READY = new TransferState(_READY);

        public const int _NOTREADY = 3;
        public static TransferState NOTREADY = new TransferState(_NOTREADY);

        public const int _MOVE_REQ_SENT = 4;
        public static TransferState MOVE_REQ_SENT = new TransferState(_MOVE_REQ_SENT);

        public const int _TRANSFERRING = 5;
        public static TransferState TRANSFERRING = new TransferState(_TRANSFERRING);

        public const int _PAUSED = 6;
        public static TransferState PAUSED = new TransferState(_PAUSED);

        public const int _ALT_DEST = 7;
        public static TransferState ALT_DEST = new TransferState(_ALT_DEST);

        public const int _ALT_HOFF = 8;
        public static TransferState ALT_HOFF = new TransferState(_ALT_HOFF);
        
        public const int _CANCELING = 9;
        public static TransferState CANCELING = new TransferState(_CANCELING);

        public const int _ABORTING = 10;
        public static TransferState ABORTING = new TransferState(_ABORTING);

        public const int _COMPLETED = 11;
        public static TransferState COMPLETED = new TransferState(_COMPLETED);

        public const int _ABORTCOMPLETE = 12;
        public static TransferState ABORTCOMPLETE = new TransferState(_ABORTCOMPLETE);

        public const int _ABORTFAIL = 13;
        public static TransferState ABORTFAIL = new TransferState(_ABORTFAIL);

        public static TransferState IT_ENUM_MAX = new TransferState(int.MaxValue);

        /// <summary>
        /// 정적 필드 초기화
        /// </summary>
        static TransferState()
        {
            displayStrings[0] = "NONE";
            displayStrings[1] = "QUEUED";
            displayStrings[2] = "READY";
            displayStrings[3] = "NOTREADY";
            displayStrings[4] = "MOVE_REQ_SENT";
            displayStrings[5] = "TRANSFERRING ";
            displayStrings[6] = "PAUSED ";
            displayStrings[7] = "ALT_DEST ";
            displayStrings[8] = "ALT_HOFF ";
            displayStrings[9] = "CANCELING ";
            displayStrings[10] = "ABORTING ";
            displayStrings[11] = "COMPLETED ";
            displayStrings[12] = "ABORTCOMPLETE ";
            displayStrings[13] = "ABORTFAIL ";

            displayValues[0] = TransferState.NONE;
            displayValues[1] = TransferState.QUEUED;
            displayValues[2] = TransferState.READY;
            displayValues[3] = TransferState.NOTREADY;
            displayValues[4] = TransferState.MOVE_REQ_SENT;
            displayValues[5] = TransferState.TRANSFERRING;
            displayValues[6] = TransferState.PAUSED;
            displayValues[7] = TransferState.ALT_DEST;
            displayValues[8] = TransferState.ALT_HOFF;
            displayValues[9] = TransferState.CANCELING;
            displayValues[10] = TransferState.ABORTING;
            displayValues[11] = TransferState.COMPLETED;
            displayValues[12] = TransferState.ABORTCOMPLETE ;
            displayValues[13] = TransferState.ABORTFAIL ;
        }

        public static string ValueToString(TransferState val)
        {
            switch (val.Value())
            {
                case TransferState._NONE:
                    return displayStrings[0];
                case TransferState._QUEUED:
                    return displayStrings[1];
                case TransferState._READY:
                    return displayStrings[2];
                case TransferState._NOTREADY:
                    return displayStrings[3];
                case TransferState._MOVE_REQ_SENT:
                    return displayStrings[4];
                case TransferState._TRANSFERRING:
                    return displayStrings[5];
                case TransferState._PAUSED:
                    return displayStrings[6];
                case TransferState._ALT_DEST:
                    return displayStrings[7];
                case TransferState._ALT_HOFF:
                    return displayStrings[8];
                case TransferState._CANCELING:
                    return displayStrings[9];
                case TransferState._ABORTING:
                    return displayStrings[10];
                case TransferState._COMPLETED:
                    return displayStrings[11];
                case TransferState._ABORTCOMPLETE:
                    return displayStrings[12];
                case TransferState._ABORTFAIL:
                    return displayStrings[13];
                default:
                    return "";
            }
        }

        public static TransferState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return TransferState.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if (val.Equals(displayStrings[i])) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this.value;
        }

        public static TransferState From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected TransferState(int value)
        {
            this.value = value;
            if (value >= 0 && value < _size)
            {
                _array[value] = this;
            }
        }

        public bool Equals(TransferState otherEnum)
        {
            if (otherEnum.Value() == value)
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        public override string ToString()
        {
            return displayStrings[value];
        }

    }

}
