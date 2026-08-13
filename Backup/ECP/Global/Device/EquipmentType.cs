using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class EquipmentType
    {
        private int _value;
        private static int _size = 10;


        private static EquipmentType[] _array = new EquipmentType[_size];
        public static string[] displayStrings = new String[_size];
        public static EquipmentType[] displayValues = new EquipmentType[_size];
                
        public const int _DEVICE = 0;
        public static EquipmentType DEVICE = new EquipmentType(_DEVICE);

        public const int _STOCKER = 1;
        public static EquipmentType STOCKER = new EquipmentType(_STOCKER);

        public const int _LIFTER = 2;
        public static EquipmentType LIFTER = new EquipmentType(_LIFTER);

        public const int _OHS = 3;
        public static EquipmentType OHS = new EquipmentType(_OHS);

        public const int _OHT = 4;
        public static EquipmentType OHT = new EquipmentType(_OHT);

        public const int _AGV = 5;
        public static EquipmentType AGV = new EquipmentType(_AGV);

        public const int _PROCESS = 6;
        public static EquipmentType PROCESS = new EquipmentType(_PROCESS);

        public const int _CONVEYOR = 7;
        public static EquipmentType CONVEYOR = new EquipmentType(_CONVEYOR);

        public const int _RGV = 8;
        public static EquipmentType RGV = new EquipmentType(_RGV);

        public const int _ECS = 9;
        public static EquipmentType ECS = new EquipmentType(_ECS);


        public static EquipmentType IT_ENUM_MAX = new EquipmentType(int.MaxValue);

    
        static EquipmentType()
        {
            displayStrings[0] = "Device";
            displayStrings[1] = "Stocker";
            displayStrings[2] = "Lifter";
            displayStrings[3] = "OHS";
            displayStrings[4] = "OHT";
            displayStrings[5] = "AGV ";
            displayStrings[6] = "PROCESS ";
            displayStrings[7] = "Conveyor";
            displayStrings[8] = "RGV";
            displayStrings[9] = "ECS";


            displayValues[0] = EquipmentType.DEVICE;
            displayValues[1] = EquipmentType.STOCKER;
            displayValues[2] = EquipmentType.LIFTER;
            displayValues[3] = EquipmentType.OHS;
            displayValues[4] = EquipmentType.OHT;
            displayValues[5] = EquipmentType.AGV;
            displayValues[6] = EquipmentType.PROCESS;
            displayValues[7] = EquipmentType.CONVEYOR;
            displayValues[8] = EquipmentType.RGV;
            displayValues[9] = EquipmentType.ECS;

        }

        public static string ValueToString(EquipmentType val)
        {
            switch (val.Value())
            {
                case EquipmentType._DEVICE:
                    return displayStrings[0];
                case EquipmentType._STOCKER:
                    return displayStrings[1];
                case EquipmentType._LIFTER:
                    return displayStrings[2];
                case EquipmentType._OHS:
                    return displayStrings[3];
                case EquipmentType._OHT:
                    return displayStrings[4];
                case EquipmentType._AGV:
                    return displayStrings[5];
                case EquipmentType._PROCESS:
                    return displayStrings[6];
                case EquipmentType._CONVEYOR:
                    return displayStrings[7];
                case EquipmentType._RGV:
                    return displayStrings[8];
                case EquipmentType._ECS:
                    return displayStrings[9];

                default:
                    return "";  
            }
        }

        public static EquipmentType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return EquipmentType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if(val.Equals(displayStrings[i])) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static EquipmentType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected EquipmentType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(EquipmentType otherEnum)
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
