using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class AccessType
    {
        private int _value;
        private static int _size = 3;


        private static AccessType[] _array = new AccessType[_size];
        public static string[] displayStrings = new String[_size];
        public static AccessType[] displayValues = new AccessType[_size];

        public const int _IN = 0;
        public static AccessType IN = new AccessType(_IN);

        public const int _OUT = 1;
        public static AccessType OUT = new AccessType(_OUT);

        public const int _IN_OUT = 2;
        public static AccessType IN_OUT = new AccessType(_IN_OUT);

        public static AccessType IT_ENUM_MAX = new AccessType(int.MaxValue);


        static AccessType()
        {
            displayStrings[0] = "In";
            displayStrings[1] = "Out";
            displayStrings[2] = "In_Out";

            displayValues[0] = AccessType.IN;
            displayValues[1] = AccessType.OUT;
            displayValues[2] = AccessType.IN_OUT;

        }

        public static string ValueToString(AccessType val)
        {
            switch (val.Value())
            {
                case AccessType._IN:
                    return displayStrings[0];
                case AccessType._OUT:
                    return displayStrings[1];
                case AccessType._IN_OUT:
                    return displayStrings[2];
                default:
                    return "";
            }
        }

        public static AccessType StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return AccessType.IT_ENUM_MAX;
            }

            for (int i = 0; i < displayStrings.Length; i++)
            {
                if ( val.Equals(displayStrings[i])  ||  val.Equals(displayStrings[i].ToUpper() ) ) return displayValues[i];
            }
            return null;
        }

        public int Value()
        {
            return this._value;
        }

        public static AccessType From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected AccessType(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(AccessType otherEnum)
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
