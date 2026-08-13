using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Exception
{
    public class ECPExceptionType
    {
        private int _value;
        private static int _size = 5;


        private static ECPExceptionType[] _array = new ECPExceptionType[_size];
        public static string[] displayStrings = new String[_size];
        public static ECPExceptionType[] dispalyValues = new ECPExceptionType[_size];

        public const int _NOT_FOUND = 0;
        public static ECPExceptionType NOT_FOUND = new ECPExceptionType(_NOT_FOUND);

        public const int _NOT_SUPPORTED = 1;
        public static ECPExceptionType NOT_SUPPORTED = new ECPExceptionType(_NOT_SUPPORTED);

        public const int _DUPLICATE = 2;
        public static ECPExceptionType DUPLICATE = new ECPExceptionType(_DUPLICATE);

        public const int _INVALID_VALUE = 3;
        public static ECPExceptionType INVALID_VALUE = new ECPExceptionType(_INVALID_VALUE);

        public const int _OPERATION_ABORTED = 4;
        public static ECPExceptionType OPERATION_ABORTED = new ECPExceptionType(_OPERATION_ABORTED);


        public static ECPExceptionType IT_ENUM_MAX = new ECPExceptionType(int.MaxValue);

    
        static ECPExceptionType()
        {
            displayStrings[0] = "Not Found";
            displayStrings[1] = "Not Supported";
            displayStrings[2] = "Duplicate";
            displayStrings[3] = "Invalid Value";
            displayStrings[4] = "Operation Aborted";

            dispalyValues[0] = ECPExceptionType.NOT_FOUND;
            dispalyValues[1] = ECPExceptionType.NOT_SUPPORTED;
            dispalyValues[2] = ECPExceptionType.DUPLICATE;
            dispalyValues[3] = ECPExceptionType.INVALID_VALUE;
            dispalyValues[4] = ECPExceptionType.OPERATION_ABORTED;


        }

        public static string ValueToString(ECPExceptionType val)
        {
            switch (val.Value())
            {
                case ECPExceptionType._NOT_FOUND:
                    return displayStrings[0];
                case ECPExceptionType._NOT_SUPPORTED:
                    return displayStrings[1];
                case ECPExceptionType._DUPLICATE:
                    return displayStrings[2];
                case ECPExceptionType._INVALID_VALUE:
                    return displayStrings[3];
                case ECPExceptionType._OPERATION_ABORTED:
                    return displayStrings[4];
                default:
                    return "";  
            }
        }

        public static ECPExceptionType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return ECPExceptionType.IT_ENUM_MAX;
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

        public static ECPExceptionType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected ECPExceptionType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(ECPExceptionType otherEnum)
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
