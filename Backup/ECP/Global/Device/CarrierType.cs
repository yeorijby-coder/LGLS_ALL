using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class CarrierType
    {

        private int _value;
        private static int _size = 10;


        private static CarrierType[] _array = new CarrierType[_size];
        public static string[] displayStrings = new String[_size];
        public static CarrierType[] displayValues = new CarrierType[_size];
 

        public const int _CA_AGVPORT = 0;
        public static CarrierType CA_AGVPORT = new CarrierType(_CA_AGVPORT);

        public const int _CA_FULL = 1;
        public static CarrierType CA_FULL = new CarrierType(_CA_FULL);

        public const int _CA_EMPTY = 2;
        public static CarrierType CA_EMPTY = new CarrierType(_CA_EMPTY);

        public const int _CA_CLEANREQUEST = 3;
        public static CarrierType CA_CLEANREQUEST = new CarrierType(_CA_CLEANREQUEST);

        public const int _CA_ERROR = 4;
        public static CarrierType CA_ERROR = new CarrierType(_CA_ERROR);

        public const int _CA_RESERV1 = 5;
        public static CarrierType CA_RESERV1 = new CarrierType(_CA_RESERV1);

        public const int _CA_RESERV2 = 6;
        public static CarrierType CA_RESERV2 = new CarrierType(_CA_RESERV2);

        public const int _CA_RESERV3 = 7;
        public static CarrierType CA_RESERV3 = new CarrierType(_CA_RESERV3);

        public const int _CA_IPS = 8;
        public static CarrierType CA_IPS = new CarrierType(_CA_IPS);

        public const int _CA_TN = 9;
        public static CarrierType CA_TN = new CarrierType(_CA_TN);

        public static CarrierType IT_ENUM_MAX = new CarrierType(int.MaxValue);


        static CarrierType()
        {
            displayStrings[0] = "AGV";
            displayStrings[1] = "FULL";
            displayStrings[2] = "EMPTY";
            displayStrings[3] = "CLEAN";
            displayStrings[4] = "ERROR";
            displayStrings[5] = "RESERV1";
            displayStrings[6] = "RESERV2";
            displayStrings[7] = "RESERV3";
            displayStrings[8] = "IPS";
            displayStrings[9] = "TN";
           

            displayValues[0] = CarrierType.CA_AGVPORT;
            displayValues[1] = CarrierType.CA_FULL;
            displayValues[2] = CarrierType.CA_EMPTY;
            displayValues[3] = CarrierType.CA_CLEANREQUEST;
            displayValues[4] = CarrierType.CA_ERROR;
            displayValues[5] = CarrierType.CA_RESERV1;
            displayValues[6] = CarrierType.CA_RESERV2;
            displayValues[7] = CarrierType.CA_RESERV3;
            displayValues[8] = CarrierType.CA_IPS;
            displayValues[9] = CarrierType.CA_TN;
        }

        public static string ValueToString(CarrierType val)
        {
            if (val == null)
            {
                return "";
            }
            switch (val.Value())
            {
                case CarrierType._CA_AGVPORT:
                    return displayStrings[0];
                case CarrierType._CA_FULL:
                    return displayStrings[1];
                case CarrierType._CA_EMPTY:
                    return displayStrings[2];
                case CarrierType._CA_CLEANREQUEST:
                    return displayStrings[3];
                case CarrierType._CA_ERROR:
                    return displayStrings[4];
                case CarrierType._CA_RESERV1 :
                    return displayStrings[5];
                case CarrierType._CA_RESERV2:
                    return displayStrings[6];
                case CarrierType._CA_RESERV3:
                    return displayStrings[7];
                case CarrierType._CA_IPS :
                    return displayStrings[8];
                case CarrierType._CA_TN:
                    return displayStrings[9];
                default:
                    return "";
            }
        }

        public static CarrierType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return CarrierType.IT_ENUM_MAX;
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

        public static CarrierType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected CarrierType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(CarrierType otherEnum)
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
