using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class SubSystemType
    {
        private int _value;
        private static int _size = 11;


        private static SubSystemType[] _array = new SubSystemType[_size];
        public static string[] displayStrings = new String[_size];
        public static SubSystemType[] displayValues = new SubSystemType[_size];
                
        public const int _PORT = 0;
        public static SubSystemType PORT = new SubSystemType(_PORT);

        public const int _STOCKERPORT = 1;
        public static SubSystemType STOCKERPORT = new SubSystemType(_STOCKERPORT);

        public const int _PROCESSPORT = 2;
        public static SubSystemType PROCESSPORT = new SubSystemType(_PROCESSPORT);

        public const int _CLEANERPORT = 3;
        public static SubSystemType CLEANERPORT = new SubSystemType(_CLEANERPORT);

        public const int _SHELF = 4;
        public static SubSystemType SHELF = new SubSystemType(_SHELF);

        public const int _RACKMASTER = 5;
        public static SubSystemType RACKMASTER = new SubSystemType(_RACKMASTER);

        public const int _FFU = 6;
        public static SubSystemType FFU = new SubSystemType(_FFU);

        public const int _VEHICLE = 7;
        public static SubSystemType VEHICLE = new SubSystemType(_VEHICLE);

        public const int _LIFTERPORT = 8;
        public static SubSystemType LIFTERPORT = new SubSystemType(_LIFTERPORT);

        public const int _CONVEYOR = 9;
        public static SubSystemType CONVEYOR = new SubSystemType(_CONVEYOR);

        public const int _RACK = 10;
        public static SubSystemType RACK = new SubSystemType(_RACK);



        public static SubSystemType IT_ENUM_MAX = new SubSystemType(int.MaxValue);

    
        static SubSystemType()
        {
            displayStrings[0] = "Port";
            displayStrings[1] = "StockerPort";
            displayStrings[2] = "ProcessPort";
            displayStrings[3] = "CleanerPort";
            displayStrings[4] = "Shelf";
            displayStrings[5] = "RackMaster";
            displayStrings[6] = "FFU";
            displayStrings[7] = "Vehicle";
            displayStrings[8] = "LifterPort";
            displayStrings[9] = "Conveyor";
            displayStrings[10] = "Rack";

            displayValues[0] = SubSystemType.PORT;
            displayValues[1] = SubSystemType.STOCKERPORT;
            displayValues[2] = SubSystemType.PROCESSPORT;
            displayValues[3] = SubSystemType.CLEANERPORT;
            displayValues[4] = SubSystemType.SHELF;
            displayValues[5] = SubSystemType.RACKMASTER;
            displayValues[6] = SubSystemType.FFU;
            displayValues[7] = SubSystemType.VEHICLE;
            displayValues[8] = SubSystemType.LIFTERPORT;
            displayValues[9] = SubSystemType.CONVEYOR;
            displayValues[10] = SubSystemType.RACK;

        }

        public static string ValueToString(SubSystemType val)
        {
            switch (val.Value())
            {
                case SubSystemType._PORT:
                    return displayStrings[0];
                case SubSystemType._STOCKERPORT:
                    return displayStrings[1];
                case SubSystemType._PROCESSPORT:
                    return displayStrings[2];
                case SubSystemType._CLEANERPORT:
                    return displayStrings[3];
                case SubSystemType._SHELF:
                    return displayStrings[4];
                case SubSystemType._RACKMASTER:
                    return displayStrings[5];
                case SubSystemType._FFU:
                    return displayStrings[6];
                case SubSystemType._VEHICLE:
                    return displayStrings[7];
                case SubSystemType._LIFTERPORT:
                    return displayStrings[8];
                case SubSystemType._CONVEYOR:
                    return displayStrings[9];
                case SubSystemType._RACK:
                    return displayStrings[10];

                default:
                    return "";  
            }
        }

        public static SubSystemType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return SubSystemType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if(val.Equals(displayStrings[i])) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return _value;
        }

        public static SubSystemType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected SubSystemType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(SubSystemType otherEnum)
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
