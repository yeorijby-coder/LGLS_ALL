using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;

namespace ECP.Global.Device
{
    public class LocationLogicalState
    {
        private int _value;
        private static int _size = 2;

        private static LocationLogicalState[] _array = new LocationLogicalState[_size];
        public static string[] displayStrings = new String[_size];
        public static LocationLogicalState[] displayValues = new LocationLogicalState[_size];

        public const int _AVAILABLE = 0;
        public static LocationLogicalState AVAILABLE = new LocationLogicalState(_AVAILABLE);

        public const int _UNAVAILABLE = 1;
        public static LocationLogicalState UNAVAILABLE = new LocationLogicalState(_UNAVAILABLE);

        public static LocationLogicalState IT_ENUM_MAX = new LocationLogicalState(int.MaxValue);


        static LocationLogicalState()
        {
            displayStrings[0] = "AVAILABLE";
            displayStrings[1] = "UNAVAILABLE";

            displayValues[0] = LocationLogicalState.AVAILABLE;
            displayValues[1] = LocationLogicalState.UNAVAILABLE;
        }

        public static string ValueToString(LocationLogicalState val)
        {
            switch (val.Value())
            {
                case LocationLogicalState._AVAILABLE:
                    return displayStrings[0];
                case LocationLogicalState._UNAVAILABLE:
                    return displayStrings[1];
                default:
                    return "";
            }
        }

        public static LocationLogicalState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return LocationLogicalState.IT_ENUM_MAX;
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

        public static LocationLogicalState From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected LocationLogicalState(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(LocationLogicalState otherEnum)
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
