using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class CarrierProhibit
    {
        private int _value;
        private static int _size = 2;


        private static CarrierProhibit[] _array = new CarrierProhibit[_size];
        public static string[] displayStrings = new String[_size];
        public static CarrierProhibit[] displayValues = new CarrierProhibit[_size];

        public const int _SET =0;
        public static CarrierProhibit SET = new CarrierProhibit(_SET);

        public const int _RESET = 1;
        public static CarrierProhibit RESET = new CarrierProhibit(_RESET);

        public static CarrierProhibit IT_ENUM_MAX = new CarrierProhibit(int.MaxValue);



        static CarrierProhibit()
        {
            displayStrings[0] = "SET";
            displayStrings[1] = "RESET";
            

            displayValues[0] = CarrierProhibit.SET;
            displayValues[1] = CarrierProhibit.RESET;
            

        }

        public static string ValueToString(CarrierProhibit val)
        {
            switch (val.Value())
            {
                case CarrierProhibit._SET:
                    return displayStrings[0];
                case CarrierProhibit._RESET:
                    return displayStrings[1];
          
                default:
                    return "";
            }
        }

        public static CarrierProhibit StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return CarrierProhibit.IT_ENUM_MAX;
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

        public static CarrierProhibit From_Int(int value)
        {
            if (value >= 1 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected CarrierProhibit(int value)
        {
            _value = value;
            if (value >= 1 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(CarrierProhibit otherEnum)
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
