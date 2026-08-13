using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global.Device
{
    public class CarrierState
    {
        private int _value;
        private static int _size = 8;


        private static CarrierState[] _array = new CarrierState[_size];
        public static string[] displayStrings = new String[_size];
        public static CarrierState[] displayValues = new CarrierState[_size];

        public const int _NONE = 0;
        public static CarrierState NONE = new CarrierState(_NONE);

        public const int _WAITIN = 1;
        public static CarrierState WAITIN = new CarrierState(_WAITIN);

        public const int _TRANSFERRING = 2;
        public static CarrierState TRANSFERRING = new CarrierState(_TRANSFERRING);

        public const int _COMPLETE = 3;
        public static CarrierState COMPLETE = new CarrierState(_COMPLETE);

        public const int _ALTERNATIVE = 4;
        public static CarrierState ALTERNATIVE = new CarrierState(_ALTERNATIVE);

        public const int _WAITOUT = 5;
        public static CarrierState WAITOUT = new CarrierState(_WAITOUT);

        public const int _INSTALLED = 6;
        public static CarrierState INSTALLED = new CarrierState(_INSTALLED);

        public const int _ONVEHICLE = 7;
        public static CarrierState ONVEHICLE = new CarrierState(_ONVEHICLE);
      
        public static CarrierState IT_ENUM_MAX = new CarrierState(int.MaxValue);

    
        static CarrierState()
        {

            displayStrings[0] = "NONE";
            displayStrings[1] = "WAITIN";
            displayStrings[2] = "TRANSFERRING";
            displayStrings[3] = "COMPLETE";
            displayStrings[4] = "ALTERNATIVE";
            displayStrings[5] = "WAITOUT ";
            displayStrings[6] = "INSTALLED ";
            displayStrings[7] = "ONVEHICLE ";

            displayValues[0] = CarrierState.NONE;
            displayValues[1] = CarrierState.WAITIN;
            displayValues[2] = CarrierState.TRANSFERRING;
            displayValues[3] = CarrierState.COMPLETE;
            displayValues[4] = CarrierState.ALTERNATIVE;
            displayValues[5] = CarrierState.WAITOUT;
            displayValues[6] = CarrierState.INSTALLED;
            displayValues[7] = CarrierState.ONVEHICLE;

        }

        public static string ValueToString(CarrierState val)
        {
            switch (val.Value())
            {

                case CarrierState._NONE:
                    return displayStrings[0];
                case CarrierState._WAITIN:
                    return displayStrings[1];
                case CarrierState._TRANSFERRING:
                    return displayStrings[2];
                case CarrierState._COMPLETE:
                    return displayStrings[3];
                case CarrierState._ALTERNATIVE:
                    return displayStrings[4];
                case CarrierState._WAITOUT:
                    return displayStrings[5];
                case CarrierState._INSTALLED:
                    return displayStrings[6];
                case CarrierState._ONVEHICLE:
                    return displayStrings[7];
                default:
                    return "";  
            }
        }

        public static CarrierState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return CarrierState.IT_ENUM_MAX;
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

        public static CarrierState From_Int(int value)
        {
            if (value >= 1 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected CarrierState(int value)
        {
            _value = value;
            if (value >= 1 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(CarrierState otherEnum)
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
