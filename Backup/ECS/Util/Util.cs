using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;
using ECP.Global.Device;
using ECP.Util.Client;
using ECP.Service.Logger;
using ECP.Service.Material;
using HECS.Device;
using HECS.Device.Unit;
using ECP.Device.Observe;

using ECP.Service.Statistics;
using HECS.Statistics;
using ECP.Db;
using System.Data.SqlClient;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;
using HECS.Gui.Monitor.Popups;
using System.Net;



namespace HECS.Util
{


    public class Util
    {
        public struct SYSTEMTIME
        {
            public ushort wYear;
            public ushort wMonth;
            public ushort wDayOfWeek;
            public ushort wDay;
            public ushort wHour;
            public ushort wMinute;
            public ushort wSecond;
            public ushort wMilliseconds;

        }
        Util()
        {
        }

        public const byte EMPTY = 32;

        [StructLayout(LayoutKind.Sequential)]
        public struct COORD
        {
            public short x;
            public short y;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SMALL_RECT
        {
            public short Left;
            public short Top;
            public short Right;
            public short Bottom;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct CONSOLE_SCREEN_BUFFER_INFO
        {
            public COORD dwSize;
            public COORD dwCursorPosition;
            public int wAttributes;
            public SMALL_RECT srWindow;
            public COORD dwMaximumWindowSize;
        }


        //화면 저장기능을 위해 사용
        [DllImport("GDI32.dll")]
        public static extern bool BitBlt(int hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, int hdcSrc, int nXSrc, int nYSrc, int dwRop);
        [DllImport("GDI32.dll")]
        public static extern int CreateCompatibleBitmap(int hdc, int nWidth, int nHeight);
        [DllImport("GDI32.dll")]
        public static extern int CreateCompatibleDC(int hdc);
        [DllImport("GDI32.dll")]
        public static extern bool DeleteDC(int hdc);
        [DllImport("GDI32.dll")]
        public static extern bool DeleteObject(int hObject);
        [DllImport("GDI32.dll")]
        public static extern int GetDeviceCaps(int hdc, int nIndex);
        [DllImport("GDI32.dll")]
        public static extern int SelectObject(int hdc, int hgdiobj);

        [DllImport("User32.dll")]
        public static extern int GetDesktopWindow();
        [DllImport("User32.dll")]
        public static extern int GetWindowDC(int hWnd);
        [DllImport("User32.dll")]
        public static extern int ReleaseDC(int hWnd, int hDC);



        [DllImport("Kernel32.dll")]
        public static extern bool SetLocalTime(ref SYSTEMTIME Time);
        #region DATA_STRUCTURE
        /// <summary>
        /// UNK,DUP,DBL,HST,PHB,EQP 등의
        /// 생성시 상위 Rule의 자리수를 말함
        /// </summary>
        public const int ABNORMAL_CARRIERID_RULE_LENGTH = 3;

        /// <summary>
        /// Carrier id Rule의 길이 정의
        /// </summary>
        public const int P8_CARRID_LENGTH = 14;

        /// <summary>
        /// P8 HOST EQP NAME 길이 정의
        /// </summary>
        public const int P8_SKTC_HOST_NAME_LENGTH = 8;

        /// <summary>
        /// 설정값을 변경한 Histroy를 남기는 Type 정의
        /// </summary>
        public enum CONFIG_CHANGE_TYPE : int
        {
            SHELF_INHIBIT_CHANGE = 0,
            SENSER_CHANGE = 1
        }
        /// <summary>
        /// CONFIG 변경 SET, 해지 : RESET
        /// </summary>
        public enum CONFIG_SET_TYPE : int
        {
            SET = 1,
            RESET = 2
        }

        /// <summary>
        /// Config Change 이력 정보 Data 구조
        /// </summary>
        public struct CONFIG_CHANGE_INFO
        {
            public string day;
            public string userName;
            public string configId;
            public string configChangeType;
            public string configSetType;
            public string configChangeContents;
        }

        public enum dateTimeFormat
        {
            FULL,
            YYYYMMDD,
            YYMMDD,
            HHMMSS,
            YYYY_MM_DD,
            YY_MM_DD,
            HH_MM_SS,
            YYMMDDHHMMSS,
            YYYYMMDDHHMMSS,
            YYYYMMDDHHMMSS_COLRON,
            UNIQTIME
        };
        public enum DateFormat
        {
            YEAR,
            MONTH,
            DAY,
            HOUR,
            MIN,
            SECOND
        };

        #endregion DATA_STRUCTURE

        /// <summary>
        /// txt에 ASCII or 숫자 값만 들어 있는지 체크 함
        /// </summary>
        /// <param name="txt">체크하고자하는 문자열</param>
        /// <returns>true : 숫자 또는 아스키 문자열만 존재, false : 잡문자 포함됨</returns>
        public static bool CheckCharacterForCstIdOrLotId(string txt)
        {
            if (txt != null && txt.Length > 0)
            {
                char[] str = txt.ToCharArray();

                foreach (char s in str)
                {
                    int codeValue = Convert.ToInt16(s);
                    // 영문자 대문                                         영문자 소문                         숫자
                    if (!((codeValue >= 65 && codeValue <= 90) || (codeValue >= 97 && codeValue <= 122) || (codeValue >= 48 && codeValue <= 57)))
                    {
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
            return true;
        }
        public static string HexToBinary(string value, int length)
        {
            string retValue = "";
            retValue = Convert.ToString(Convert.ToInt32(value, 16), 2);
            if (retValue.Length > length)
            {
                retValue = retValue.Substring(0, length);
            }
            retValue = retValue.PadLeft(length, '0');
            return retValue;
        }

        /// <summary>
        /// int 값을 Binary로 변경 1 -> 01
        /// </summary>
        /// <param name="value">변경하고자 하는 int 값</param>
        /// <param name="length">binary length</param>
        /// <returns>binary 값</returns>
        public static string IntToBinary(int value, int length)
        {
            return HexToBinary(IntToHex(value), length);
        }

        public static string BinaryToHex(string value)
        {
            string retValue = "";
            retValue = (Convert.ToInt32(value, 2)).ToString("X");
            return retValue;
        }

        /// <summary>
        /// CONFIG 변경 이력 저장
        /// </summary>
        /// <param name="userName">변경자명</param>
        /// <param name="configId">변경대상 ID 예 SHELFID, SENSERID</param>
        /// <param name="changeType">변경 CONFIG TYPE</param>
        /// <param name="set">설정 또는 해지</param>
        /// <param name="contents">변경 사유</param>
        public static void SaveConfigChangeHistory(string userName, string configId, CONFIG_CHANGE_TYPE changeType, CONFIG_SET_TYPE set, string contents)
        {
            DBConnection conn = DBControl.GetConnection();
            PreparedSqlParameter pss = conn.GetPreparedSqlParameter();

            try
            {
                pss.SetVarChar("DATE", GetNowDateTimeString(dateTimeFormat.FULL));
                //   pss.SetVarChar("DATE", DateFormat.);
                pss.SetVarChar("USER_NAME", userName);
                pss.SetVarChar("CONFIG_ID", configId);
                pss.SetVarChar("CONFIG_TYPE", Enum.GetName(typeof(CONFIG_CHANGE_TYPE), changeType));
                pss.SetVarChar("CONFIG_SET_TYPE", Enum.GetName(typeof(CONFIG_SET_TYPE), set));
                pss.SetVarChar("CHANGING_CONTENTS", contents);

                conn.ExecuteUpdate("insert into CONFIG_CHANGE_HISTORY (DATE,USER_NAME,CONFIG_ID,CONFIG_TYPE,CONFIG_SET_TYPE,CHANGING_CONTENTS) values (@DATE,@USER_NAME,@CONFIG_ID,@CONFIG_TYPE,@CONFIG_SET_TYPE,@CHANGING_CONTENTS)", pss);
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("Util", "SaveConfigChangeHistory", configId, ex.ToString(), ex.StackTrace.ToString()));
            }
            DBControl.RemoveDBConection(conn);

        }

        /// <summary>
        /// Config 또는 금지단 설정의 History를 조회
        /// </summary>
        /// <param name="configId">Shelf ID key임</param>
        /// <param name="changeType">금지단 설정인지 , Config  변경이지</param>
        /// <returns></returns>
        public static Dictionary<string, CONFIG_CHANGE_INFO> SerchChangeHistoryOfConfig(string configId, CONFIG_CHANGE_TYPE changeType)
        {
            DBConnection conn = DBControl.GetConnection();
            PreparedSqlParameter psparam = conn.GetPreparedSqlParameter();
            SqlDataReader reader = null;
            CONFIG_CHANGE_INFO configChangeInfo;
            Dictionary<string, CONFIG_CHANGE_INFO> configChangeInfoes = new Dictionary<string, CONFIG_CHANGE_INFO>();
            try
            {
                psparam.SetVarChar("CONFIG_ID", configId.ToString());
                psparam.SetVarChar("CONFIG_TYPE", changeType.ToString());

                reader = conn.ExecuteQuery("select * from CONFIG_CHANGE_HISTORY where CONFIG_ID =@CONFIG_ID and CONFIG_TYPE =@CONFIG_TYPE  ORDER BY DATE DESC ", psparam);
                //SELECT * FROM CONFIG_CHANGE_HISTORY WHERE DATE IN(SELECT MAX(DATE)FROM CONFIG_CHANGE_HISTORY WHERE CONFIG_ID =@CONFIG_ID and CONFIG_TYPE =@CONFIG_TYPE and CONFIG_SET_TYPE =@CONFIG_SET_TYPE" )
                while (reader.Read())
                {
                    configChangeInfo = new CONFIG_CHANGE_INFO();
                    configChangeInfo.day = ((DateTime)reader["DATE"]).ToString();
                    configChangeInfo.userName = (string)reader["USER_NAME"];
                    configChangeInfo.configId = (string)reader["CONFIG_ID"];
                    configChangeInfo.configChangeType = (string)reader["CONFIG_TYPE"];
                    configChangeInfo.configSetType = (string)reader["CONFIG_SET_TYPE"];
                    configChangeInfo.configChangeContents = (string)reader["CHANGING_CONTENTS"];
                    configChangeInfoes.Add(configChangeInfo.day, configChangeInfo);
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("Util", "SerchChangeHistoryOfConfig", configId, ex.ToString(), ex.StackTrace.ToString()));
            }
            DBControl.RemoveDBConection(conn);
            return configChangeInfoes;
        }
        /// <summary>
        /// CONFIG를 변경 DATA를 조회
        /// </summary>
        /// <param name="configId">변경대상 ID 예 SHELFID, SENSERID</param>
        /// <param name="changeType">변경 CONFIG TYPE</param>
        /// <param name="set">설정 또는 해지</param>
        /// <returns>CONFIG CHANGE DATA INFOMATION</returns>
        public static CONFIG_CHANGE_INFO LoadConfigChangeHistory(string configId, CONFIG_CHANGE_TYPE changeType, CONFIG_SET_TYPE set)
        {
            DBConnection conn = DBControl.GetConnection();
            PreparedSqlParameter psparam = conn.GetPreparedSqlParameter();
            SqlDataReader reader = null;
            CONFIG_CHANGE_INFO configChangeInfo = new CONFIG_CHANGE_INFO();

            try
            {
                psparam.SetVarChar("CONFIG_ID", configId.ToString());
                psparam.SetVarChar("CONFIG_TYPE", changeType.ToString());
                psparam.SetVarChar("CONFIG_SET_TYPE", set.ToString());

                reader = conn.ExecuteQuery("select * from CONFIG_CHANGE_HISTORY where CONFIG_ID =@CONFIG_ID and CONFIG_TYPE =@CONFIG_TYPE and CONFIG_SET_TYPE =@CONFIG_SET_TYPE ORDER BY DATE DESC ", psparam);
                //SELECT * FROM CONFIG_CHANGE_HISTORY WHERE DATE IN(SELECT MAX(DATE)FROM CONFIG_CHANGE_HISTORY WHERE CONFIG_ID =@CONFIG_ID and CONFIG_TYPE =@CONFIG_TYPE and CONFIG_SET_TYPE =@CONFIG_SET_TYPE" )
                if (reader.Read())
                {
                    configChangeInfo.day = ((DateTime)reader["DATE"]).ToString();
                    configChangeInfo.userName = (string)reader["USER_NAME"];
                    configChangeInfo.configId = (string)reader["CONFIG_ID"];
                    configChangeInfo.configChangeType = (string)reader["CONFIG_TYPE"];
                    configChangeInfo.configSetType = (string)reader["CONFIG_SET_TYPE"];
                    configChangeInfo.configChangeContents = (string)reader["CHANGING_CONTENTS"];
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("Util", "LoadConfigChangeHistory", configId, ex.ToString(), ex.StackTrace.ToString()));
            }
            DBControl.RemoveDBConection(conn);
            return configChangeInfo;
        }

        /// <summary>
        /// CONGIG를 저장한 1달치의 모든 데이터를 가져 옴.
        /// </summary>
        /// <returns>CONFIG CHANGE DATA INFOMATIONS</returns>
        public static Dictionary<string, CONFIG_CHANGE_INFO> LoadConfigChangeHistoryAll()
        {
            DBConnection conn = DBControl.GetConnection();
            PreparedSqlParameter psparam = conn.GetPreparedSqlParameter();
            SqlDataReader reader = null;
            Dictionary<string, CONFIG_CHANGE_INFO> configChangeInfoes = new Dictionary<string, CONFIG_CHANGE_INFO>();
            try
            {
                CONFIG_CHANGE_INFO configChangeInfo;

                psparam.SetDateTime("BEFORE_DATE", DateTime.Now.AddMonths(-1));

                reader = conn.ExecuteQuery("select * from CONFIG_CHANGE_HISTORY where DATE > @BEFORE_DATE", psparam);

                while (reader.Read())
                {
                    configChangeInfo = new CONFIG_CHANGE_INFO();
                    configChangeInfo.day = ((DateTime)reader["DATE"]).ToString();
                    configChangeInfo.userName = (string)reader["USER_NAME"];
                    configChangeInfo.configId = (string)reader["CONFIG_ID"];
                    configChangeInfo.configChangeType = (string)reader["CONFIG_TYPE"];
                    configChangeInfo.configSetType = (string)reader["CONFIG_SET_TYPE"];
                    configChangeInfo.configChangeContents = (string)reader["CHANGING_CONTENTS"];
                    configChangeInfoes.Add(configChangeInfo.day, configChangeInfo);
                }
            }
            catch (SqlException ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_CORE_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("Util", "LoadConfigChangeHistory", "", ex.ToString(), ex.StackTrace.ToString()));
            }

            DBControl.RemoveDBConection(conn);
            return configChangeInfoes;
        }


        public static string StringToHex(string s)
        {
            string retValue = "";
            string temp = "";
            if (s.Equals(""))
            {
                return "0000";
            }
            int index = 0;
            for (index = 0; index < s.Length; index++)
            {
                if (index % 2 == 0)
                {
                    temp = StringFormat(s[index]);
                }
                else
                {
                    temp = temp.Substring(0, 2) + StringFormat(s[index]).Substring(0, 2);
                    retValue = retValue.Insert(retValue.Length, temp);
                }
            }
            if ((index % 2) != 0)
            {
                retValue = retValue.Insert(retValue.Length, temp);
            }
            return retValue;
        }
        private static string StringFormat(char s)
        {
            string retValue = "";
            string temp = "";
            temp = ((int)s).ToString("X");
            temp = temp.PadRight(4, '0');
            retValue = retValue.Insert(retValue.Length, temp);

            return retValue;
        }


        public static string HexToDec(string s)
        {
            const string METHOD = "HexToDec";
            string HexToDec = "";
            try
            {
                HexToDec = Convert.ToInt64(s, 16).ToString();
            }
            catch (Exception ex)
            {

                HexToDec = ex.Message.ToString();
                Log.log(5, METHOD, "HexToDec", Category.DEBUG, "UTIL", "", HexToDec = ex.Message.ToString());
            }
            return HexToDec;
        }
        public static string IntToHex(int s)
        {
            const string METHOD = "IntToHex";
            string DecToHex = "";
            try
            {
                ushort t;
                t = (ushort)s;
                DecToHex = t.ToString("X");
                DecToHex = DecToHex.PadLeft(4, '0');
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "IntToHex", Category.DEBUG, "UTIL", "", DecToHex = ex.Message.ToString());
            }
            return DecToHex;
        }
        public static string DecToHex(string s)
        {
            string DecToHex = "";
            string Result = "";
            try
            {
                long decs = Int64.Parse(s);
                DecToHex = decs.ToString("X");
                if (DecToHex.Length < 4)
                    for (int i = 0; i < (4 - DecToHex.Length); i++)
                        Result += "0";
                Result += DecToHex;
            }
            catch (Exception ex)
            {
                Result = ex.Message.ToString();
            }
            return Result;
        }
        public static string HexToAscii(string _value)
        {
            string retValue = "";
            for (int index = 0; index < _value.Length; index += 4)
            {
                string _str = "";
                string word = "";

                word = _value.Substring(index, 4);

                for (int i = 0; i < word.Length; i = i + 2)
                {
                    _str = (Convert.ToChar(Convert.ToInt32(word.Substring(i, 2), 16))).ToString() + _str;
                }
                retValue += _str;
            }
            return retValue;
        }
        public static string HexToString(string _value)
        {
            string _str = "";
            if (_value.Length < 2)
            {
                return "";
            }
            for (int i = 0; i < _value.Length; i = i + 2)
            {
                _str = _str + (Convert.ToChar(Convert.ToInt32(_value.Substring(i, 2), 16))).ToString();
            }
            return _str;
        }
        public static string AsciiToHex(string _value)
        {
            string _str = "";
            int index = 0;
            for (index = 0; index < _value.Length; index = index + 2)
            {
                string temp = _value.Substring(index, 2);
                char[] chr = temp.ToCharArray();

                temp = "";
                temp += Convert.ToString(chr[1], 16);
                temp = temp.PadLeft(2, '0');
                _str = _str.Insert(_str.Length, temp);

                temp = "";
                temp = Convert.ToString(chr[0], 16);
                temp = temp.PadLeft(2, '0');
                _str = _str.Insert(_str.Length, temp);
            }
            return _str;
        }

        public static string StringReverse(string source)
        {
            const string METHOD = "StringReverse";
            string ret = "";
            try
            {
                for (int i = 0; i < source.Length; i++)
                {
                    ret = ret + source.Substring(source.Length - (i + 1), 1);
                }
            }
            catch (Exception ex)
            {
                Log.log(5, METHOD, "IntToHex", Category.DEBUG, "UTIL", "", ex.Message.ToString());
            }
            return ret;


        }
        /// <summary>
        /// 날자까지만 비교함.
        /// </summary>
        /// <param name="dayA"></param>
        /// <param name="dayB"></param>
        /// <returns></returns>
        public static int DayCompare(DateTime dayA, DateTime dayB)
        {
            int reasult = 0;

            if (dayA.Year == dayB.Year)
            {
                reasult = 0;
                if (dayA.Month == dayB.Month)
                {
                    reasult = 0;
                    if (dayA.Day == dayB.Day)
                    {
                        reasult = 0;
                    }
                    else if (dayA.Day > dayB.Day)
                    {
                        reasult = -1;
                    }
                    else
                    {
                        reasult = 1;
                    }
                }
                else if (dayA.Month > dayB.Month)
                {
                    reasult = -1;
                }
                else
                {
                    reasult = 1;
                }
            }
            else if (dayA.Year > dayB.Year)
            {
                reasult = -1;
            }
            else
            {
                reasult = 1;
            }
            return reasult;
        }

        //////////////////////////////////////////////////////////////////////////////////////
        public static string StringToReverseHex(string s)
        {
            string retValue = "";
            string temp = "";
            int index = 0;
            for (index = 0; index < s.Length; index++)
            {
                if (index % 2 == 0)
                {
                    temp = StringFormat(s[index]);
                }
                else
                {
                    temp = StringFormat(s[index]).Substring(0, 2) + temp.Substring(0, 2);
                    retValue = retValue.Insert(retValue.Length, temp);
                }
            }
            if ((index % 2) != 0)
            {
                temp = temp.Substring(2, 2) + temp.Substring(0, 2);
                retValue = retValue.Insert(retValue.Length, temp);
            }
            return retValue;
        }


        //public static void CreateCarrier(string carrierID, string locationXXX, string locationId, string sublocationid,
        //    CarrierState carrierstate, CarrierType carriertype, CarrierHold carrierhold, CarrierProhibit carrierprohibit)
        //{
        //    string install_time = DateTime.Now.ToString("yyyy-MM-dd hh:mm:ss");
        //    string id64 = CreateCarrierID64(carrierID, locationXXX);

        //    Naming.GetCarrierManager().CreateCarrier(carrierID, id64, locationId, sublocationid, install_time, carrierstate, carriertype, carrierhold, carrierprohibit);
        //}




        public static string GetNowDateTimeString(dateTimeFormat format)
        {
            string ret = null;

            switch (format)
            {
                case dateTimeFormat.FULL:
                    ret = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
                    break;
                case dateTimeFormat.YYYYMMDD:
                    ret = DateTime.Now.ToString("yyyyMMdd");
                    break;
                case dateTimeFormat.YYMMDD:
                    ret = DateTime.Now.ToString("yyMMdd");
                    break;
                case dateTimeFormat.HHMMSS:
                    ret = DateTime.Now.ToString("HHmmss");
                    break;
                case dateTimeFormat.YYYY_MM_DD:
                    ret = DateTime.Now.ToString("yyyy-MM-dd");
                    break;
                case dateTimeFormat.YY_MM_DD:
                    ret = DateTime.Now.ToString("yy-MM-dd");
                    break;
                case dateTimeFormat.HH_MM_SS:
                    ret = DateTime.Now.ToString("HH:mm:ss");
                    break;
                case dateTimeFormat.YYMMDDHHMMSS:
                    ret = DateTime.Now.ToString("yyMMddHHmmss");
                    break;
                case dateTimeFormat.YYYYMMDDHHMMSS:
                    ret = DateTime.Now.ToString("yyyyMMddHHmmss");
                    break;
                case dateTimeFormat.YYYYMMDDHHMMSS_COLRON:
                    ret = DateTime.Now.ToString("yyyy:MM:dd:HH:mm:ss");
                    break;
                case dateTimeFormat.UNIQTIME:
                    ret = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                    break;
            }
            return ret;
        }

        /// <summary>
        /// CarrierID 생성시 STKC 호기명에 기본으로 하는 8자리 Identity key 생성 
        /// ex) UNKP8STK01001 과같이 P8STK01을 생성해 줌.
        /// </summary>
        /// <returns></returns>
        private static string GetIdentityKey()
        {
            string Identitykey = "";
            StockerSEM stockerSem = (StockerSEM)Naming.GetDeviceManager().GetDevice("");
            Identitykey = stockerSem.HostID;
            if (Identitykey != null)
            {
                if (Identitykey.Length > P8_SKTC_HOST_NAME_LENGTH)
                {
                    Identitykey = Identitykey.Substring(Identitykey.Length - P8_SKTC_HOST_NAME_LENGTH, P8_SKTC_HOST_NAME_LENGTH);
                }
                else
                {
                    Identitykey = Identitykey.PadLeft(P8_SKTC_HOST_NAME_LENGTH, '0');
                }
            }
            else
            {
                //DB에 호기명이 들어 있지 않을 경우 Default
                return "P8STKXX";
            }
            return Identitykey;
        }


        /// <summary>
        /// 새로운 Carrier ID를 생성
        /// ex) DUPP8STK01001
        ///     DUP + XXXXXXXXXXXXXXX = 14자리
        /// </summary>
        /// <param name="header"></param>
        /// <returns></returns>
        public static string GetNewCarrierID(string header)
        {
            string newCarrierId = "";

            Dictionary<string, string> keyTable = new Dictionary<string, string>();
            ;


            if (header != null)
            {
                if (header.Length > ABNORMAL_CARRIERID_RULE_LENGTH)
                {
                    header = header.Substring(0, ABNORMAL_CARRIERID_RULE_LENGTH);
                }
                else if (header.Length == ABNORMAL_CARRIERID_RULE_LENGTH)
                {
                    // Nothing
                }
                else
                {
                    // Error 처리 후 Defalut Carrier ID 생성
                    LogFormat.AddDebug("ERROR", "STOKCER", "Carrier ID 생성시 Header 에러.", "ex) DUP 등의 길이가 틀림");
                    header = "ERR";
                }

            }
            else
            {
                // Error 처리 후 Defalut Carrier ID 생성
                header = "ERR";
                LogFormat.AddDebug("ERROR", "STOCKER", "Carrier ID 생성시 Header 에러.", " ex) DUP 등이 않옴");
            }

            newCarrierId = GenerateCstIdKey(header);

            return newCarrierId;

        }


        /// <summary>
        /// CSTID 생성시 Unique 한 ID 생성을 위해 11자리의 Number를 생성 함.
        /// Rule Header 3자리 + Unique 11자리 = 14자리
        /// Unique = DateTime.Now.Ticks를 이용 18자리의 카운터(100나노초) 서기 1년 1월 12일 부터 카운터 되는 윈도우의 Tick 이용
        /// 총 13자리 중 앞 3자리와 나노 및 마이크로초를 제외한 11자리 생성
        /// </summary>
        /// <param name="header">DUP등의 3자리 해더 값</param>
        /// <returns>신규 CSTID </returns>
        private static string GenerateCstIdKey(string header)
        {
            int previoustime;
            string cstIdKey = "";
            int keyLength = 11;
            int normalTicksLength = 18;
            string newCstId = "";

            previoustime = DateTime.Now.Millisecond;

            try
            {
                //시간 Base의 세로운 Key 생성을 위해 시간을 흘려 버림.
                while (previoustime == DateTime.Now.Millisecond)
                {
                    // Nothing
                }

                String time = DateTime.Now.Ticks.ToString();

                if (time.Length == normalTicksLength) // Normal
                {

                    cstIdKey = time.Substring(3, keyLength);
                }
                else if (time.Length > normalTicksLength)
                {
                    cstIdKey = time.Substring(time.Length - keyLength - 1, 11);
                }
                else
                {
                    cstIdKey = time.PadLeft(normalTicksLength, '0').Substring(3, normalTicksLength);
                }
            }
            catch (Exception ex)
            {
                LogFormat.AddDebug("ERROR", "STOKCER", "정상적인 CARRIER ID 생성에 실패.DEFAULT로 생성 함.", "");
                string key = header + GetNowDateTimeString(dateTimeFormat.YYMMDDHHMMSS);
                cstIdKey = key.Substring(0, 14);
            }

            newCstId = header + cstIdKey;

            if (Naming.GetCarrierManager().isExistCarrerId(newCstId) == false)
            {
                // Nothing
            }
            else
            {
                LogFormat.AddDebug("ERROR", "STOKCER", "정상적인 CARRIER ID 생성에 실패.DEFAULT로 생성 함.", "");
                string key = header + GetNowDateTimeString(dateTimeFormat.YYMMDDHHMMSS);
                cstIdKey = key.Substring(0, 14);
                newCstId = header + cstIdKey;
            }
            return newCstId;
        }


        /// <summary>
        /// Key Table과 중복되지 않는 3자리의 Key를 생성해 줌
        /// </summary>
        /// <param name="carrierKeyTable"></param>
        /// <returns></returns>
        private static string GetNewKey(Dictionary<string, string> carrierKeyTable)
        {
            if (carrierKeyTable != null)
            {

                for (int i = 0; i < 1000; i++)
                {
                    string newKey = "";

                    if (i < 10)
                        newKey = "00" + i.ToString();
                    else if (i < 100)
                        newKey = "0" + i.ToString();
                    else
                        newKey = i.ToString();

                    if (!carrierKeyTable.ContainsKey(newKey))
                    {
                        return newKey;
                    }
                }


                LogFormat.AddDebug("ERROR", "STOKCER", "new Key 생성 Error", "");

                return "XXX";
            }
            else // Key Table 없다는 것은 Stocker에 Carrier가 하나도 없다는 뜻으로 최초 하나 생성 해줌
            {
                return "001";
            }
        }


        public static string CreateCarrierID64(string carrierid, string location)
        {
            string install_time = GetNowDateTimeString(dateTimeFormat.YYMMDDHHMMSS);
            return carrierid + "-" + location + "-" + install_time;
        }

        public static int GetGapTime1toNow(DateFormat df, string time1)
        {
            int gap = 0;
            try
            {
                DateTime nowtime = DateTime.Now;
                DateTime qTime = Convert.ToDateTime(time1);
                TimeSpan tGqp = nowtime.Subtract(qTime);
                gap = GetValueTimeSpan(df, tGqp);
            }
            catch
            {

            }
            return gap;
        }

        public static int GetGapTime1toNow(DateFormat df, DateTime time1)
        {
            int gap = 0;
            try
            {
                DateTime nowtime = DateTime.Now;
                TimeSpan tGqp = nowtime.Subtract(time1);
                gap = GetValueTimeSpan(df, tGqp);
            }
            catch
            {

            }
            return gap;
        }

        public static int GetValueTimeSpan(DateFormat df, TimeSpan ts)
        {
            int ret = 0;
            switch (df)
            {
                case DateFormat.YEAR:
                    ret = (int)(ts.TotalDays / 365);
                    break;
                case DateFormat.MONTH:
                    ret = (int)(ts.TotalDays / 30);
                    break;
                case DateFormat.DAY:
                    ret = (int)ts.TotalDays;
                    break;
                case DateFormat.HOUR:
                    ret = (int)ts.TotalHours;
                    break;
                case DateFormat.MIN:
                    ret = (int)ts.TotalMinutes;
                    break;
                case DateFormat.SECOND:
                    ret = (int)ts.TotalSeconds;
                    break;
            }
            return ret;
        }



        //SHINKO
        public static void WriteExectionLog(string eID, string funcPath, Exception ex)
        {
            StatisticsCollector.TriggerOccured(
                    DebugStatisticsCollectorData.COLLECTOR_ID,
                    DebugStatisticsCollectorData.TRIGGER_EVT_ID,
                    new DebugStatisticsCollectorData("STKC", eID, funcPath,
                       string.Format("Occuer Exception. Message:{0} Source:{1} Stack:{2}", ex.Message, ex.Source, ex.StackTrace)));
        }


        public static bool isNumeric(string Arg)
        {
            try
            {
                int tem = Convert.ToInt16(Arg);
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
            return false;
        }

        public static string ReadCSTID(string cstID)
        {
            if (cstID == null)
                return "";
            return cstID.Trim().ToUpper();
        }
        public static string ConvertLocation(string location)
        {
            string returnValue = "";

            switch (location)
            {
                case "CONVEYOR:2":
                    returnValue = "SC1, 입출고대";
                    break;
                case "CONVEYOR:3":
                    returnValue = "SC2, 출고대";
                    break;
                case "CONVEYOR:4":
                    returnValue = "SC2, 입고대";
                    break;
                case "CONVEYOR:5":
                    returnValue = "SC3, 출고대";
                    break;
                case "CONVEYOR:6":
                    returnValue = "SC3, 입고대";
                    break;
                case "CONVEYOR:7":
                    returnValue = "SC4, 출고대";
                    break;
                case "CONVEYOR:8":
                    returnValue = "SC4, 입고대";
                    break;
                case "CONVEYOR:9":
                    returnValue = "SC5, 출고대";
                    break;
                case "CONVEYOR:10":
                    returnValue = "SC5, 입고대";
                    break;
                case "CONVEYOR:11":
                    returnValue = "KR01,입고존";
                    break;
                case "CONVEYOR:12":
                    returnValue = "KR02,출고존";
                    break;
                case "CONVEYOR:13":
                    returnValue = "KR02,출고존";
                    break;
                case "CONVEYOR:14":
                    returnValue = "KR03,피킹존";
                    break;
                case "CONVEYOR:15":
                    returnValue = "KR03,피킹존";
                    break;
                case "RACK:1":
                    returnValue = "KR01,하이랙1";
                    break;
                case "RACK:2":
                    returnValue = "KR01,하이랙2";
                    break;
                case "RACK:3":
                    returnValue = "KR01,하이랙3";
                    break;
                case "RACK:4":
                    returnValue = "KR01,하이랙4";
                    break;
                case "RACK:5":
                    returnValue = "KR01,하이랙5";
                    break;
                case "RACK:6":
                    returnValue = "KR01,하이랙6";
                    break;
                case "RACK:7":
                    returnValue = "KR01,하이랙7";
                    break;
                case "RACK:8":
                    returnValue = "KR01,하이랙8";
                    break;
                case "RACK:9":
                    returnValue = "KR01,하이랙9";
                    break;
                case "RACK:10":
                    returnValue = "KR01,하이랙10";
                    break;
                case "VEHICLE:1":
                    returnValue = "RGV";
                    break;
                case "VEHICLE:11":
                    returnValue = "SC1";
                    break;
                case "VEHICLE:12":
                    returnValue = "SC2";
                    break;
                case "VEHICLE:13":
                    returnValue = "SC3";
                    break;
                case "VEHICLE:14":
                    returnValue = "SC4";
                    break;
                case "VEHICLE:15":
                    returnValue = "SC5";
                    break;

                default:
                    returnValue = location;
                    break;
            }

            return returnValue;
        }
        public static void ShowMesagePopup(string message)
        {
            MessageDialog messageDialog = (MessageDialog)SimpleCache.GetObject("Popup", "MessageDialog");
            if (messageDialog != null)
            {
                messageDialog.DisplayMessage = message;
            }
            else
            {
                messageDialog = new MessageDialog();
                SimpleCache.AddObject("Popup", "MessageDialog", messageDialog);
                messageDialog.DisplayMessage = message;
            }
            messageDialog.Show();
        }


        public static int CaptureScreen()
        {
            int hdcSrc = GetWindowDC(GetDesktopWindow()),
            hdcDest = CreateCompatibleDC(hdcSrc),
            hBitmap = CreateCompatibleBitmap(hdcSrc,
            GetDeviceCaps(hdcSrc, 8), GetDeviceCaps(hdcSrc, 10));
            SelectObject(hdcDest, hBitmap);
            BitBlt(hdcDest, 0, 0, GetDeviceCaps(hdcSrc, 8),
            GetDeviceCaps(hdcSrc, 10), hdcSrc, 0, 0, 0x00CC0020);
            Cleanup(hdcSrc, hdcDest);
            return hBitmap;
        }

        public static void CaptureScreen(string fileName, ImageFormat imageFormat)
        {
            int hdcSrc = GetWindowDC(GetDesktopWindow()),
            hdcDest = CreateCompatibleDC(hdcSrc),
            hBitmap = CreateCompatibleBitmap(hdcSrc,
            GetDeviceCaps(hdcSrc, 8), GetDeviceCaps(hdcSrc, 10));
            SelectObject(hdcDest, hBitmap);
            BitBlt(hdcDest, 0, 0, GetDeviceCaps(hdcSrc, 8),
            GetDeviceCaps(hdcSrc, 10), hdcSrc, 0, 0, 0x00CC0020);
            SaveImageAs(hBitmap, fileName, imageFormat);
            Cleanup(hBitmap, hdcSrc, hdcDest);
        }
        private static void Cleanup(int hBitmap, int hdcSrc, int hdcDest)
        {
            ReleaseDC(GetDesktopWindow(), hdcSrc);
            DeleteDC(hdcDest);
            DeleteObject(hBitmap);
        }
        public static void Cleanup(int hBitmap)
        {
            DeleteObject(hBitmap);
        }

        private static void Cleanup(int hdcSrc, int hdcDest)
        {
            ReleaseDC(GetDesktopWindow(), hdcSrc);
            DeleteDC(hdcDest);
        }

        public static void SaveImageAs(int hBitmap, string fileName, ImageFormat imageFormat)
        {
            Bitmap image =
            new Bitmap(Image.FromHbitmap(new IntPtr(hBitmap)),
            Image.FromHbitmap(new IntPtr(hBitmap)).Width,
            Image.FromHbitmap(new IntPtr(hBitmap)).Height);
            image.Save(fileName, imageFormat);
        }

        public static void Alarm1()
        {
            try
            {
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?S1=OFF");
                HttpWRequest.UserAgent = "Handok Towner Lamp Control";
                HttpWRequest.KeepAlive = true; //this is the default
                HttpWRequest.Headers.Set("Pragma", "no-cache");
                HttpWRequest.Timeout = 300;
                HttpWRequest.Method = "GET";
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();

                HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?LF1=1");
                HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();
            }

            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }

        public static void Alarm2()
        {
            try
            {
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?S2=ON");
                HttpWRequest.UserAgent = "Handok Towner Lamp Control";
                HttpWRequest.KeepAlive = true; //this is the default
                HttpWRequest.Headers.Set("Pragma", "no-cache");
                HttpWRequest.Timeout = 300;
                HttpWRequest.Method = "GET";
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();

                HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?LF2=1");
                HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();
            }

            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }
        public static void Alarm3()
        {
            try
            {
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?S3=OFF");
                HttpWRequest.UserAgent = "Handok Towner Lamp Control";
                HttpWRequest.KeepAlive = true; //this is the default
                HttpWRequest.Headers.Set("Pragma", "no-cache");
                HttpWRequest.Timeout = 300;
                HttpWRequest.Method = "GET";
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();

                HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?LF3=1");
                HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();
            }

            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }

        public static void AlarmReset()
        {
            try
            {
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?RESET=RESET");
                HttpWRequest.UserAgent = "Handok Towner Lamp Control";
                HttpWRequest.KeepAlive = true; //this is the default
                HttpWRequest.Headers.Set("Pragma", "no-cache");
                HttpWRequest.Timeout = 300;
                HttpWRequest.Method = "GET";
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();
            }

            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }

        public static void AlarmAGVReset()
        {
            try
            {
                HttpWebRequest HttpWRequest = (HttpWebRequest)WebRequest.Create(@"http://166.118.231.73/ET.CGI?LF2=0");
                HttpWRequest.UserAgent = "Handok Towner Lamp Control";
                HttpWRequest.KeepAlive = true; //this is the default
                HttpWRequest.Headers.Set("Pragma", "no-cache");
                HttpWRequest.Timeout = 300;
                HttpWRequest.Method = "GET";
                HttpWebResponse HttpWResponse = (HttpWebResponse)HttpWRequest.GetResponse();
                HttpWResponse.Close();
            }

            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
        }
    }
    public class listSortCompare : IComparer<ListViewItem>
    {

        public int Compare(ListViewItem x, ListViewItem y)
        {
            if (int.Parse(x.SubItems[1].Text) < int.Parse(y.SubItems[1].Text))
            {
                return 1;
            }
            else if (int.Parse(x.SubItems[1].Text) > int.Parse(y.SubItems[1].Text))
            {
                return -1;
            }
            else
            {
                {
                    DateTime xTime = Convert.ToDateTime(x.SubItems[3].Text);
                    DateTime yTime = Convert.ToDateTime(y.SubItems[3].Text);
                    if (xTime < yTime)
                    {
                        return -1;
                    }
                    else if (xTime > yTime)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                    return 0;
                }
            }
        }
    }


}
