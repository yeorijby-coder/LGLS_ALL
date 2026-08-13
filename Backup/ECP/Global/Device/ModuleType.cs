using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class ModuleType
    {
        private int _value;
        private static int _size = 7;


        private static ModuleType[] _array = new ModuleType[_size];
        public static string[] displayStrings = new String[_size];
        public static ModuleType[] displayValues = new ModuleType[_size];
                
        public const int _DEVICE = 0;
        public static ModuleType DEVICE = new ModuleType(_DEVICE);

        public const int _STOCKER = 1;
        public static ModuleType STOCKER = new ModuleType(_STOCKER);

        public const int _LIFTER = 2;
        public static ModuleType LIFTER = new ModuleType(_LIFTER);

        public const int _OHS = 3;
        public static ModuleType OHS = new ModuleType(_OHS);

        public const int _OHT = 4;
        public static ModuleType OHT = new ModuleType(_OHT);

        public const int _AGV = 5;
        public static ModuleType AGV = new ModuleType(_AGV);

        public const int _PROCESS = 6;
        public static ModuleType PROCESS = new ModuleType(_PROCESS);

        public static ModuleType IT_ENUM_MAX = new ModuleType(int.MaxValue);

    
        static ModuleType()
        {
            displayStrings[0] = "Device";
            displayStrings[1] = "Stocker";
            displayStrings[2] = "Lifter";
            displayStrings[3] = "OHS";
            displayStrings[4] = "OHT";
            displayStrings[5] = "AGV ";
            displayStrings[6] = "PROCESS ";

            displayValues[0] = ModuleType.DEVICE;
            displayValues[1] = ModuleType.STOCKER;
            displayValues[2] = ModuleType.LIFTER;
            displayValues[3] = ModuleType.OHS;
            displayValues[4] = ModuleType.OHT;
            displayValues[5] = ModuleType.AGV;
            displayValues[6] = ModuleType.PROCESS;

        }

        public static string ValueToString(ModuleType val)
        {
            switch (val.Value())
            {
                case ModuleType._DEVICE:
                    return displayStrings[0];
                case ModuleType._STOCKER:
                    return displayStrings[1];
                case ModuleType._LIFTER:
                    return displayStrings[2];
                case ModuleType._OHS:
                    return displayStrings[3];
                case ModuleType._OHT:
                    return displayStrings[4];
                case ModuleType._AGV:
                    return displayStrings[5];
                case ModuleType._PROCESS:
                default:
                    return "";  
            }
        }

        public static ModuleType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return ModuleType.IT_ENUM_MAX;
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

        public static ModuleType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected ModuleType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(ModuleType otherEnum)
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
