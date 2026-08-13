

using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;

namespace ECP.Global.Device
{
    public class LocationState
    {
        private int _value;
        private static int _size = 3;

        private static LocationState[] _array = new LocationState[_size];
        public static string[] displayStrings = new String[_size];
        public static LocationState[] displayValues = new LocationState[_size];

        public const int _EMPTY = 0;
        public static LocationState EMPTY = new LocationState(_EMPTY);

        public const int _RESERVED = 1;
        public static LocationState RESERVED = new LocationState(_RESERVED);

        public const int _OCCUFIED = 2;
        public static LocationState OCCUFIED = new LocationState(_OCCUFIED);

        public static LocationState IT_ENUM_MAX = new LocationState(int.MaxValue);


        static LocationState()
        {
            displayStrings[0] = "EMPTY";
            displayStrings[1] = "RESERVED";
            displayStrings[2] = "OCCUFIED";

            displayValues[0] = LocationState.EMPTY;
            displayValues[1] = LocationState.RESERVED;
            displayValues[2] = LocationState.OCCUFIED;
        }

        public static string ValueToString(LocationState val)
        {
            switch (val.Value())
            {
                case LocationState._EMPTY:
                    return displayStrings[0];
                case LocationState._RESERVED:
                    return displayStrings[1];
                case LocationState._OCCUFIED:
                    return displayStrings[2];
                default:
                    return "";
            }
        }

        public static LocationState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return LocationState.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if (val.Equals(displayStrings[i]))
                    return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static LocationState From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected LocationState(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(LocationState otherEnum)
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

        public override string ToString()
        {
            return displayStrings[_value];
        }

    }
}
