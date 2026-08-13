

using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;

namespace ECP.Global.Device
{
    public class LocationType
    {
        private int _value;
        private static int _size = 2;

        private static LocationType[] _array = new LocationType[_size];
        public static string[] displayStrings = new String[_size];
        public static LocationType[] displayValues = new LocationType[_size];

        public const int _CarrierLocation = 0;
        public static LocationType CarrierLocation = new LocationType(_CarrierLocation);

        public const int _SubstrateLocation = 1;
        public static LocationType SubstrateLocation = new LocationType(_SubstrateLocation);

       public static LocationType IT_ENUM_MAX = new LocationType(int.MaxValue);


        static LocationType()
        {
            displayStrings[0] = "CarrierLocation";
            displayStrings[1] = "SubstrateLocation";
 
            displayValues[0] = LocationType.CarrierLocation;
            displayValues[1] = LocationType.SubstrateLocation;
        }

        public static string ValueToString(LocationType val)
        {
            switch (val.Value())
            {
                case LocationType._CarrierLocation:
                    return displayStrings[0];
                case LocationType._SubstrateLocation:
                    return displayStrings[1];
                default:
                    return "";
            }
        }

        public static LocationType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return LocationType.IT_ENUM_MAX;
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

        public static LocationType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected LocationType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(LocationType otherEnum)
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
