using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global
{
    public class GlobalConstant
    {
        public const char SAP_DELIMITER = '@';
        public const char MOVEMENT_DELIMITER = '^';
        public const char PALLET_DELIMITER = '/';

        public const string START_DEVICE_CD = "START_DEVICE_01";
        public const string START_LOCATION_CD = "START_LOCATION_01";
        public const string START_SUBLOCATION_CD = "START_SUBLOCATION_01";

        public const string END_DEVICE_CD = "END_DEVICE_01";
        public const string END_LOCATION_CD = "END_LOCATION_01";
        public const string END_SUBLOCATION_CD = "END_SUBLOCATION_01";

        public const string STORAGE_LOCATION_CODE = "KR01";
        public const string INGO_LOCATION_CODE = "KR00";
        
        public const string SUBSYSTEM_RACK = "RACK:";
        public const string BIG_SIZE_RACK = "RACK:8";

        public const string BIG_SIZE_LOCATION = "BIG_SIZE_LOCATION";
        public const string PRIORITY_LOCATION = "PRIORITY_LOCATION";

        public const string SUMMER_TIME_CODE = "TIME_GAP";
        public const string TACKTIME_INTERVAL_DAY = "5";
        public const string DEFAULT_TACKTIME = "300";

        public const string PLANT_CODE = "KR01";
        public const string CLIENT_CODE = "100";

        public const string FROM_TEXT = "FROM";
        public const string TO_TEXT = "TO";

        public const string ALL_CHARACTER = "%";
        public const string ALL_TEXT = "ÀüÃ¼";

        public const string ORDER_DATAMOVE = "311";
        public const string ORDER_CANCEL = "314";
        public const string ORDER_CONFIRMATION = "315";

        public const string ORDER_DATAMOVE_FLAG = "M";
        public const string ORDER_CANCEL_FLAG = "C";
        public const string ORDER_CONFIRMATION_FLAG = "F";

        public const string SET_MAXTIME = "99991231235959999";
        public const string SET_ERRIRTIME = "99991231235959XXX";
        public const string TRANSFER_PRIORITY_DEFAULT = "50";

        public const string SAP_PREFIX = "9";
        public const string ECS_PREFIX = "8";
        public const string MOVE_PREFIX = "7";


        public static string START_MODE = "ECS";
        public static string START_RACK = "0";
        public static string CLIENT_TYPE = "";

        public const string PRIORITY_RULE = "R001";
        public const string LOCATION_RULE = "R002";
        public const string DELAY_RULE = "R005";

        public const string DUAL_MODE_STKSEM = "STKSEM:1";
        public const string DUAL_MODE_FROM_RGVSEM = "RGVSEM:1";
        public const string DUAL_MODE_FROM_CNVSEM = "CNVSEM:1";
        public const string DUAL_MODE_FROM_LOCATION = "CONVEYOR:11";
        public const string DUAL_MODE_FROM_PORT = "PORT:21";

        public const string DUAL_MODE_TO_CNVSEM = "CNVSEM:1";
        public const string DUAL_MODE_TO_LOCATION = "CONVEYOR:11";
        public const string DUAL_MODE_TO_PORT = "PORT:22";

        public const string DUAL_MODE_TO_RGVSEM = "RGVSEM:1";
        public const string DUAL_MODE_TO_RGVLOCATION = "CONVEYOR:2R";
        public const string DUAL_MODE_TO_RGVPORT = "PORT:3";
        public const string DUAL_MODE_TO_CNVLOCATION = "CONVEYOR:2";
        public const string DUAL_MODE_TO_CNVPORT = "PORT:4";
        public const string DUAL_MODE_FROM_CNVLOCATION = "CONVEYOR:2";
        public const string DUAL_MODE_FROM_CNVPORT = "PORT:4";

        public static string WMS_HOST = "192.100.1.142";
        public static string WMS_PORT = "9910";
        public static string ECS_PORT = "9911";
        public static string PLC_IP = "192.100.1.110";
        public static int PLC_PORT = 2004;

        public const char NULL_CHAR = '@';
        public const char START_CHAR = '^';
        public const char END_CHAR = '#';

        public static string CONVEYOR11_MODE = "0";
        public const string CONVEYOR11_WMS_CODE = "101";
        public const string RGV_INGO = "0";
        public const string RGV_OUTGO = "1";

        public const string RGV_ERR_PREFIX = "10";
        public const string SC_ERR_PREFIX = "20";
    }
}
