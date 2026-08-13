using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global
{
    public class DataType
    {
        private int _value;
        private static int _size = 12;


        private static DataType[] _array = new DataType[_size];
        public static string[] displayStrings = new String[_size];
        public static DataType[] displayValues = new DataType[_size];
                
        public const int _BOOLEAN = 0;
        public static DataType BOOLEAN = new DataType(_BOOLEAN);

        public const int _BYTE = 1;
        public static DataType BYTE = new DataType(_BYTE);

        public const int _SHORT = 2;
        public static DataType SHORT = new DataType(_SHORT);

        public const int _INTEGER = 3;
        public static DataType INTEGER = new DataType(_INTEGER);

        public const int _LONG = 4;
        public static DataType LONG = new DataType(_LONG);

        public const int _DOUBLE = 5;
        public static DataType DOUBLE = new DataType(_DOUBLE);

        public const int _STRING = 6;
        public static DataType STRING = new DataType(_STRING);

        public const int _SHORT_ARRAY = 7;
        public static DataType SHORT_ARRAY = new DataType(_SHORT_ARRAY);

        public const int _INTEGER_ARRAY = 8;
        public static DataType INTEGER_ARRAY = new DataType(_INTEGER_ARRAY);

        public const int _LONG_ARRAY = 9;
        public static DataType LONG_ARRAY = new DataType(_LONG_ARRAY);

        public const int _DOUBLE_ARRAY = 10;
        public static DataType DOUBLE_ARRAY = new DataType(_DOUBLE_ARRAY);

        public const int _BOOL_ARRAY = 11;
        public static DataType BOOL_ARRAY = new DataType(_BOOL_ARRAY);


        public static DataType IT_ENUM_MAX = new DataType(int.MaxValue);

    
        static DataType()
        {
            displayStrings[0] = "Boolean";
            displayStrings[1] = "Byte";
            displayStrings[2] = "Short";
            displayStrings[3] = "Integer";
            displayStrings[4] = "Long";
            displayStrings[5] = "Double";
            displayStrings[6] = "String";
            displayStrings[7] = "Short Array";
            displayStrings[8] = "Integer Array";
            displayStrings[9] = "Long Array";
            displayStrings[10] = "Double Array";
            displayStrings[11] = "Bool Array";

            displayValues[0] = DataType.BOOLEAN;
            displayValues[1] = DataType.BYTE;
            displayValues[2] = DataType.SHORT;
            displayValues[3] = DataType.INTEGER;
            displayValues[4] = DataType.LONG;
            displayValues[5] = DataType.DOUBLE;
            displayValues[6] = DataType.STRING;
            displayValues[7] = DataType.SHORT_ARRAY;
            displayValues[8] = DataType.INTEGER_ARRAY;
            displayValues[9] = DataType.LONG_ARRAY;
            displayValues[10] = DataType.DOUBLE_ARRAY;
            displayValues[11] = DataType.BOOL_ARRAY;


        }

        public static string ValueToString(DataType val)
        {
            switch (val.Value())
            {
                case DataType._BOOLEAN:
                    return displayStrings[0];
                case DataType._BYTE:
                    return displayStrings[1];
                case DataType._SHORT:
                    return displayStrings[2];
                case DataType._INTEGER:
                    return displayStrings[3];
                case DataType._LONG:
                    return displayStrings[4];
                case DataType._DOUBLE:
                    return displayStrings[5];
                case DataType._STRING:
                    return displayStrings[6];
                case DataType._SHORT_ARRAY:
                    return displayStrings[7];
                case DataType._INTEGER_ARRAY:
                    return displayStrings[8];
                case DataType._LONG_ARRAY:
                    return displayStrings[9];
                case DataType._DOUBLE_ARRAY:
                    return displayStrings[10];
                case DataType._BOOL_ARRAY:
                    return displayStrings[11];

                default:
                    return "";  
            }
        }

        public static DataType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return DataType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if ( val.ToUpper().Equals(displayStrings[i].ToUpper())  ) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static DataType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected DataType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(DataType otherEnum)
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
