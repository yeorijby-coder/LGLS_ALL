using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace HECS.Device.Dispatcher
{
    /// <summary>
    /// Transfer Result Code
    /// Defined by MCS
    /// </summary>
    public class TransferResult : MarshalByRefObject
    {

        private int _value;
        private static int _size = 14;

        private static TransferResult[] _array = new TransferResult[_size];
        public static string[] displayStrings = new String[_size];
        public static TransferResult[] displayValues = new TransferResult[_size];
                

        public const int _TR_SUCCESS = 0;
        public static TransferResult TR_SUCCESS = new TransferResult(_TR_SUCCESS);

        public const int _TR_UNSUCCESSFUL = 1;
        public static TransferResult TR_UNSUCCESSFUL = new TransferResult(_TR_UNSUCCESSFUL);

        public const int _TR_ZONEFULL = 2;
        public static TransferResult TR_ZONEFULL = new TransferResult(_TR_ZONEFULL);

        public const int _TR_IDDUPLICATE = 3;
        public static TransferResult TR_IDDUPLICATE = new TransferResult(_TR_IDDUPLICATE);

        public const int _TR_IDMISMATCH = 4;
        public static TransferResult TR_IDMISMATCH = new TransferResult(_TR_IDMISMATCH);

        public const int _TR_IDFAILURE = 5;
        public static TransferResult TR_IDFAILURE = new TransferResult(_TR_IDFAILURE);

        public const int _TR_INTERLOCK = 6;
        public static TransferResult TR_INTERLOCK = new TransferResult(_TR_INTERLOCK);

        public const int _TR_TRANSFER_CANCEL = 7;
        public static TransferResult TR_TRANSFER_CANCEL = new TransferResult(_TR_TRANSFER_CANCEL);

        public const int _TR_TRANSFER_ABORT = 8;
        public static TransferResult TR_TRANSFER_ABORT = new TransferResult(_TR_TRANSFER_ABORT);

        public const int _TR_NOSPACE = 9;
        public static TransferResult TR_NOSPACE = new TransferResult(_TR_NOSPACE);

        public const int _TR_ULD_FAIL = 10;
        public static TransferResult TR_ULD_FAIL = new TransferResult(_TR_ULD_FAIL);

        public const int _TR_LD_FAIL = 11;
        public static TransferResult TR_LD_FAIL = new TransferResult(_TR_LD_FAIL);

        public const int _TR_SOURCEPORT_DOWN = 12;
        public static TransferResult TR_SOURCEPORT_DOWN = new TransferResult(_TR_SOURCEPORT_DOWN);

        public const int _TR_DOUBLE = 13;
        public static TransferResult TR_DOUBLE = new TransferResult(_TR_DOUBLE);

       

        public static TransferResult IT_ENUM_MAX = new TransferResult(int.MaxValue);

        static TransferResult()
        {

            displayStrings[0] = "TR_SUCCESS";
            displayStrings[1] = "TR_UNSUCCESSFUL";
            displayStrings[2] = "TR_ZONEFULL";
            displayStrings[3] = "TR_IDDUPLICATE";
            displayStrings[4] = "TR_IDMISMATCH";
            displayStrings[5] = "TR_IDFAILURE";
            displayStrings[6] = "TR_INTERLOCK";
            displayStrings[7] = "TR_TRANSFER_CANCEL";
            displayStrings[8] = "TR_TRANSFER_ABORT";
            displayStrings[9] = "TR_NOSPACE";
            displayStrings[10] = "TR_ULD_FAIL";
            displayStrings[11] = "TR_LD_FAIL";
            displayStrings[12] = "TR_SOURCEPORT_DOWN";
            displayStrings[13] = "TR_DOUBLE";


            displayValues[0] = TransferResult.TR_SUCCESS;
            displayValues[1] = TransferResult.TR_UNSUCCESSFUL;
            displayValues[2] = TransferResult.TR_ZONEFULL;
            displayValues[3] = TransferResult.TR_IDDUPLICATE;
            displayValues[4] = TransferResult.TR_IDMISMATCH;
            displayValues[5] = TransferResult.TR_IDFAILURE;
            displayValues[6] = TransferResult.TR_INTERLOCK;
            displayValues[7] = TransferResult.TR_TRANSFER_CANCEL;
            displayValues[8] = TransferResult.TR_TRANSFER_ABORT;
            displayValues[9] = TransferResult.TR_NOSPACE;
            displayValues[10] = TransferResult.TR_ULD_FAIL;
            displayValues[11] = TransferResult.TR_LD_FAIL;
            displayValues[12] = TransferResult.TR_SOURCEPORT_DOWN;
            displayValues[13] = TransferResult.TR_DOUBLE;
        }

        public static string ValueToString(TransferResult val)
        {
            switch (val.Value())
            {
                case TransferResult._TR_SUCCESS:
                    return displayStrings[0];
                case TransferResult._TR_UNSUCCESSFUL:
                    return displayStrings[1];
                case TransferResult._TR_ZONEFULL:
                    return displayStrings[2];
                case TransferResult._TR_IDDUPLICATE:
                    return displayStrings[3];
                case TransferResult._TR_IDMISMATCH:
                    return displayStrings[4];
                case TransferResult._TR_IDFAILURE:
                    return displayStrings[5];
                case TransferResult._TR_INTERLOCK:
                    return displayStrings[6];
                case TransferResult._TR_TRANSFER_CANCEL:
                    return displayStrings[7];
                case TransferResult._TR_TRANSFER_ABORT:
                    return displayStrings[8];
                case TransferResult._TR_NOSPACE:
                    return displayStrings[9];
                case TransferResult._TR_ULD_FAIL:
                    return displayStrings[10];
                case TransferResult._TR_LD_FAIL:
                    return displayStrings[11];
                case TransferResult._TR_SOURCEPORT_DOWN:
                    return displayStrings[12];
                case TransferResult._TR_DOUBLE:
                    return displayStrings[13];               
                default:
                    return "";  
            }
        }


        public static TransferResult StringToValue(string val)
        {
            if (Helper.IsNullOrEmpty(val))
            {
                return TransferResult.IT_ENUM_MAX;
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

        public static TransferResult From_Int(int value)
        {
            if (value >= 1 && value < _size)
                return _array[value];
            else
            {
                return IT_ENUM_MAX;
            }
        }

        protected TransferResult(int value)
        {
            _value = value;
            if (value >= 1 && value < _size)
            {
                _array[_value] = this;
            }
        }

        public bool Equals(TransferResult otherEnum)
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
