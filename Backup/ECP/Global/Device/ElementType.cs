using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;

namespace ECP.Global.Device
{
    public class ElementType
    {

        private int _value;
        private static int _size = 4;


        private static ElementType[] _array = new ElementType[_size];
        public static string[] displayStrings = new String[_size];
        public static ElementType[] displayValues = new ElementType[_size];
                
        public const int _EQUIPMENT = 0;
        public static ElementType EQUIPMENT = new ElementType(_EQUIPMENT);

        public const int _MODULE = 1;
        public static ElementType MODULE = new ElementType(_MODULE);

        public const int _SUBSYSTEM = 2;
        public static ElementType SUBSYSTEM = new ElementType(_SUBSYSTEM);

        public const int _IODEVICE = 3;
        public static ElementType IODEVICE = new ElementType(_IODEVICE);

 
        public static ElementType IT_ENUM_MAX = new ElementType(int.MaxValue);

    
        static ElementType()
        {
            displayStrings[0] = "Equipment";
            displayStrings[1] = "Module";
            displayStrings[2] = "SubSystem";
            displayStrings[3] = "IODevice";

            displayValues[0] = ElementType.EQUIPMENT;
            displayValues[1] = ElementType.MODULE;
            displayValues[2] = ElementType.SUBSYSTEM;
            displayValues[3] = ElementType.IODEVICE;
        }

        public static string ValueToString(ElementType val)
        {
            switch (val.Value())
            {
                case ElementType._EQUIPMENT:
                    return displayStrings[0];
                case ElementType._MODULE:
                    return displayStrings[1];
                case ElementType._SUBSYSTEM:
                    return displayStrings[2];
                case ElementType._IODEVICE:
                    return displayStrings[3];
                default:
                    return "";  
            }
        }

        public static ElementType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return ElementType.IT_ENUM_MAX;
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

        public static ElementType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected ElementType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(ElementType otherEnum)
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
