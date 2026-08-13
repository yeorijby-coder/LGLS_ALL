using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Com
{
    public class FBConnectionState
    {
        private int _value;
        private static int _size = 5;


        private static FBConnectionState[] _array = new FBConnectionState[_size];
        public static string[] displayStrings = new String[_size];
        public static FBConnectionState[] dispalyValues = new FBConnectionState[_size];

        public const int _DISABLED = 0;
        public static FBConnectionState DISABLED = new FBConnectionState(_DISABLED);

        public const int _ENABLED = 1;
        public static FBConnectionState ENABLED = new FBConnectionState(_ENABLED);

        public const int _DISCONNECTED = 2;
        public static FBConnectionState DISCONNECTED = new FBConnectionState(_DISCONNECTED);

        public const int _CONNECTED = 3;
        public static FBConnectionState CONNECTED = new FBConnectionState(_CONNECTED);

        public const int _RETRY = 4;
        public static FBConnectionState RETRY = new FBConnectionState(_RETRY);


        public static FBConnectionState IT_ENUM_MAX = new FBConnectionState(int.MaxValue);


        static FBConnectionState()
        {
            displayStrings[0] = "Disabled";
            displayStrings[1] = "Enabled";
            displayStrings[2] = "Disconnected";
            displayStrings[3] = "Connected";
            displayStrings[4] = "Retry";

            dispalyValues[0] = FBConnectionState.DISABLED;
            dispalyValues[1] = FBConnectionState.ENABLED;
            dispalyValues[2] = FBConnectionState.DISCONNECTED;
            dispalyValues[3] = FBConnectionState.CONNECTED;
            dispalyValues[4] = FBConnectionState.RETRY;
        }

        public static string ValueToString(FBConnectionState val)
        {
            switch (val.Value())
            {
                case FBConnectionState._DISABLED:
                    return displayStrings[0];
                case FBConnectionState._ENABLED:
                    return displayStrings[1];
                case FBConnectionState._DISCONNECTED:
                    return displayStrings[2];
                case FBConnectionState._CONNECTED:
                    return displayStrings[3];
                case FBConnectionState._RETRY:
                    return displayStrings[4];
                default:
                    return "";
            }
        }

        public static FBConnectionState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return FBConnectionState.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if (val.Equals(displayStrings[i])) return dispalyValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static FBConnectionState From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected FBConnectionState(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(FBConnectionState otherEnum)
        {
            if (otherEnum.Value() == _value)
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        public override  string ToString()
        {
            return displayStrings[_value];
        }


    }
}
