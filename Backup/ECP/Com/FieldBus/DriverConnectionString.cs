/// <summary>
/// 
/// </summary>
/// <filename></filename>
/// <version>1.0.0.0</version>
/// <modifications>
/// 	1.  ver 1.0.0.0     2006-00-00 00:00:00     Jongseok Song   -
///         : Created.
/// </modifications>
/// <copyright>Copyright (c) 2005~2007. EzControl, LG CNS All rights reserved.</copyright>
/// 

using System;
using System.Collections.Generic;
using System.Text;

using ECP.Com.FieldBus;

namespace ECP.Com.FieldBus
{
    public class DriverConnectionString : GenericConnectionString
    {
        public const string RETRY_COUNT = "RETRY_COUNT";
        public const string TIMEOUT_SECOND = "TIMEOUT_SECOND";

        public const string IP_SIM = "IP_SIM";
        public const string PORT_SIM = "PORT_SIM";
        public const string HOST = "HOST";
        public const string PORT = "PORT";
        public const string HEADER_SIZE = "HEADER_SIZE";

        #region Method
        protected override object GetValue(string sName, string sValue)
        {
            object newValue = null;

            switch (sName)
            {
                case RETRY_COUNT:
                    newValue = Convert.ToInt32(sValue);
                    break;
                case TIMEOUT_SECOND:
                    break;
                case IP_SIM:
                    newValue = sValue.Trim();
                    break;
                case PORT_SIM:
                    newValue = sValue.Trim();
                    break;
                case HOST:
                    newValue = sValue.Trim();
                    break;
                case PORT:
                    newValue = sValue.Trim();
                    break;
                case HEADER_SIZE:
                    newValue = sValue.Trim();
                    break;
            }

            return newValue;
        }

        #endregion Method
    }
}
