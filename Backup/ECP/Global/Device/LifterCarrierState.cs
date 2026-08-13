using System;
using System.Collections.Generic;
using System.Text;
using ECP.Util;
namespace ECP.Global.Device
{
    public class LifterCarrierState
    {
        private int _value;
        private static int _size = 4;


        private static LifterCarrierState[] _array = new LifterCarrierState[_size];
        public static string[] displayStrings = new String[_size];
        public static LifterCarrierState[] displayValues = new LifterCarrierState[_size];

        public const int _NONE = 0;
        public static LifterCarrierState NONE = new LifterCarrierState(_NONE);

        public const int _WAITIN = 1;
        public static LifterCarrierState WAITIN = new LifterCarrierState(_WAITIN);

        public const int _ONVEHICLE = 2;
        public static LifterCarrierState ONVEHICLE = new LifterCarrierState(_ONVEHICLE);

        public const int _WAITOUT = 3;
        public static LifterCarrierState WAITOUT = new LifterCarrierState(_WAITOUT);        

      
        public static LifterCarrierState IT_ENUM_MAX = new LifterCarrierState(int.MaxValue);

    
        static LifterCarrierState()
        {

            displayStrings[0] = "NONE";
            displayStrings[1] = "WAITIN";
            displayStrings[2] = "ONVEHICLE";
            displayStrings[3] = "WAITOUT";
     
            displayValues[0] = LifterCarrierState.NONE;
            displayValues[1] = LifterCarrierState.WAITIN;
            displayValues[2] = LifterCarrierState.ONVEHICLE;
            displayValues[3] = LifterCarrierState.WAITOUT;              

        }

        public static string ValueToString(LifterCarrierState val)
        {
            switch (val.Value())
            {

                case LifterCarrierState._NONE:
                    return displayStrings[0];
                case LifterCarrierState._WAITIN:
                    return displayStrings[1];
                case LifterCarrierState._ONVEHICLE:
                    return displayStrings[2];
                case LifterCarrierState._WAITOUT:
                    return displayStrings[3];            
                default:
                    return "";  
            }
        }

        public static LifterCarrierState StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return LifterCarrierState.IT_ENUM_MAX;
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

        public static LifterCarrierState From_Int(int value)
        {
            if (value >= 1 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected LifterCarrierState(int value)
        {
            _value = value;
            if (value >= 1 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(LifterCarrierState otherEnum)
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
