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

namespace EzControl.Driver.MelsecNet
{
    public class CMelsecNetConnection : DriverConnectionString
    {
        protected override object GetValue(string sName, string sValue)
        {
            object value = null;
            
            switch (sName)
            {
                case "CHANNEL_NO" :
                case "STATION_NO" :
                case "MODE":
                    value = Convert.ToInt32(sValue);
                    break;
                case "MONITOR_INTERVAL" :
                    value = Convert.ToInt32(sValue);
                    break;
                case "X_START_ADDR" :
                case "X_END_ADDR" :
                case "Y_START_ADDR" :
                case "Y_END_ADDR" :
                case "SB_START_ADDR" :
                case "SB_END_ADDR" :
                case "SW_START_ADDR" :
                case "SW_END_ADDR" :
                case "B_START_ADDR" :
                case "B_END_ADDR" :
                case "W_START_ADDR" :
                case "W_END_ADDR" :
                case "WW_START_ADDR":
                case "WW_END_ADDR":
                case "WR_START_ADDR":
                case "WR_END_ADDR":
                case "R_START_ADDR":
                case "R_END_ADDR":
                    value = Convert.ToInt32(sValue);
                    break;
            }

            return value;
        }
    }
}
