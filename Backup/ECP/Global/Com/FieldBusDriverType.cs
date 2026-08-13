using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Com
{
    public class FieldBusDriverType
    {
        private int _value;
        private static int _size = 6;


        private static FieldBusDriverType[] _array = new FieldBusDriverType[_size];
        public static string[] displayStrings = new String[_size];
        public static FieldBusDriverType[] dispalyValues = new FieldBusDriverType[_size];
                
        public const int _SIMULATION = 0;
        public static FieldBusDriverType SIMULATION = new FieldBusDriverType(_SIMULATION);

        public const int _MELSECNET = 1;
        public static FieldBusDriverType MELSECNET = new FieldBusDriverType(_MELSECNET);

        public const int _CCLINK = 2;
        public static FieldBusDriverType CCLINK = new FieldBusDriverType(_CCLINK);

        public const int _DEVICENET = 3;
        public static FieldBusDriverType DEVICENET = new FieldBusDriverType(_DEVICENET);

        public const int _FILE = 4;
        public static FieldBusDriverType FILE = new FieldBusDriverType(_FILE);

        public const int _SOCKET = 5;
        public static FieldBusDriverType SOCKET = new FieldBusDriverType(_SOCKET);

        public static FieldBusDriverType IT_ENUM_MAX = new FieldBusDriverType(int.MaxValue);

    
        static FieldBusDriverType()
        {
            displayStrings[0] = "SIMULATION";
            displayStrings[1] = "MELSECNET";
            displayStrings[2] = "CCLINK";
            displayStrings[3] = "DEVICENET";
            displayStrings[4] = "FILE";
            displayStrings[5] = "SOCKET";

            dispalyValues[0] = FieldBusDriverType.SIMULATION;
            dispalyValues[1] = FieldBusDriverType.MELSECNET;
            dispalyValues[2] = FieldBusDriverType.CCLINK;
            dispalyValues[3] = FieldBusDriverType.DEVICENET;
            dispalyValues[4] = FieldBusDriverType.FILE;
            dispalyValues[5] = FieldBusDriverType.SOCKET;
        }

        public static string ValueToString(FieldBusDriverType val)
        {
            switch (val.Value())
            {
                case FieldBusDriverType._SIMULATION:
                    return displayStrings[0];
                case FieldBusDriverType._MELSECNET:
                    return displayStrings[1];
                case FieldBusDriverType._CCLINK:
                    return displayStrings[2];
                case FieldBusDriverType._DEVICENET:
                    return displayStrings[3];
                case FieldBusDriverType._FILE:
                    return displayStrings[4];
                case FieldBusDriverType._SOCKET:
                    return displayStrings[5];
                default:
                    return "";  
            }
        }

        public static FieldBusDriverType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return FieldBusDriverType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if(val.Equals(displayStrings[i])) return dispalyValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static FieldBusDriverType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected FieldBusDriverType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(FieldBusDriverType otherEnum)
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
