using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class CarrierHold
    {
        private int _value;
        private static int _size = 2;


        private static CarrierHold[] _array = new CarrierHold[_size];
        public static string[] displayStrings = new String[_size];
        public static CarrierHold[] displayValues = new CarrierHold[_size];

        public const int _RESET = 0;
        public static CarrierHold RESET = new CarrierHold(_RESET);

        public const int _SET = 1;
        public static CarrierHold SET = new CarrierHold(_SET);

        public static CarrierHold IT_ENUM_MAX = new CarrierHold(int.MaxValue);

        
        static CarrierHold()
        {
            displayStrings[0] = "RESET";
            displayStrings[1] = "SET";
            

            displayValues[0] = CarrierHold.RESET;
            displayValues[1] = CarrierHold.SET;
            

        }

        public static string ValueToString(CarrierHold val)
        {
            switch (val.Value())
            {
                case CarrierHold._RESET:
                    return displayStrings[0];
                case CarrierHold._SET:
                    return displayStrings[1];
          
                default:
                    return "";
            }
        }

        public static CarrierHold StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return CarrierHold.IT_ENUM_MAX;
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

        public static CarrierHold From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected CarrierHold(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(CarrierHold otherEnum)
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
