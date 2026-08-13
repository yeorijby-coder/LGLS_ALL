using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class CarrierElevType
    {
        private int _value;
        private static int _size = 4;


        private static CarrierElevType[] _array = new CarrierElevType[_size];
        public static string[] displayStrings = new String[_size];
        public static CarrierElevType[] displayValues = new CarrierElevType[_size];

    
        public const int _ELEV0 = 0;
        public static CarrierElevType ELEV0 = new CarrierElevType(_ELEV0);

        public const int _WIRE_H = 1;
        public static CarrierElevType WIRE_H = new CarrierElevType(_WIRE_H);

        public const int _WIRE_N = 2;
        public static CarrierElevType WIRE_N = new CarrierElevType(_WIRE_N);

        public const int _NORMAL = 3;
        public static CarrierElevType NORMAL = new CarrierElevType(_NORMAL);

        public static CarrierElevType IT_ENUM_MAX = new CarrierElevType(int.MaxValue);

        static CarrierElevType()
        {
            displayStrings[0] = "ELEV_0";
            displayStrings[1] = "WIRE_H";
            displayStrings[2] = "WIRE_N";
            displayStrings[3] = "NORMAL";



            displayValues[0] = CarrierElevType.ELEV0;
            displayValues[1] = CarrierElevType.WIRE_H;
            displayValues[2] = CarrierElevType.WIRE_N;
            displayValues[3] = CarrierElevType.NORMAL;
           
            

        }

        public static string ValueToString(CarrierElevType val)
        {
            switch (val.Value())
            {
                case CarrierElevType._ELEV0:
                    return displayStrings[0];
                case CarrierElevType._WIRE_H:
                    return displayStrings[1];
                case CarrierElevType._WIRE_N:
                    return displayStrings[2];
                case CarrierElevType._NORMAL:
                    return displayStrings[3];              
                default:
                    return "";
            }
        }

        public static CarrierElevType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return CarrierElevType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if (val.Equals(displayStrings[i])) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static CarrierElevType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected CarrierElevType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(CarrierElevType otherEnum)
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
