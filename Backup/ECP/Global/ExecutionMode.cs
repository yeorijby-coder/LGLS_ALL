using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Global
{
    public class ExecutionMode
    {
        private int _value;
        private static int _size = 2;


        private static ExecutionMode[] _array = new ExecutionMode[_size];
        public static string[] displayStrings = new String[_size];
        public static ExecutionMode[] displayValues = new ExecutionMode[_size];
                
        public const int _REAL = 0;
        public static ExecutionMode REAL = new ExecutionMode(_REAL);

        public const int _SIMULATION = 1;
        public static ExecutionMode SIMULATION = new ExecutionMode(_SIMULATION);

 
        public static ExecutionMode IT_ENUM_MAX = new ExecutionMode(int.MaxValue);

    
        static ExecutionMode()
        {
            displayStrings[0] = "Real";
            displayStrings[1] = "Simulation";
    
            displayValues[0] = ExecutionMode.REAL;
            displayValues[1] = ExecutionMode.SIMULATION;

        }

        public static string ValueToString(ExecutionMode val)
        {
            switch (val.Value())
            {
                case ExecutionMode._REAL:
                    return displayStrings[0];
                case ExecutionMode._SIMULATION:
                    return displayStrings[1];
                default:
                    return "";  
            }
        }

        public static ExecutionMode StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return ExecutionMode.IT_ENUM_MAX;
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

        public static ExecutionMode From_Int(int value)
        {
            if (value >= 0 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected ExecutionMode(int value)
        {
            _value = value;
            if (value >= 0 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(ExecutionMode otherEnum)
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
