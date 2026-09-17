
using Microsoft.VisualBasic;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Windows.Forms;
//**************************************************
//
// Date, Time 관련 정의
//
//**************************************************

namespace TSK_HostCom
{
	public static class modDateTime
	{

#if ORACLE
		public const string SYSDATE = " SYSDATE ";
		public const string SYSDATE_TO_CDT = " TO_CHAR(SYSDATE, 'YYYYMMDD') ";
		public const string SYSDATE_TO_CTM = " TO_CHAR(SYSDATE, 'HH24MISS') ";
		public const string SYSDATETIME_TO_CDTM = " TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS') ";
		public const string SYSDATE_TO_CDTTM = " TO_CHAR(SYSDATE, 'YYYYMMDDHH24MISS') ";
#endif
#if SQL   
		public const string SYSDATE = " GETDATE() ";
		public const string SYSDATE_TO_CDT = " CONVERT(varchar,GETDATE(),112) ";
		public const string SYSDATE_TO_CTM = " REPLACE(CONVERT(varchar,GETDATE(),108),':','') ";
		public const string SYSDATETIME_TO_CDTM = " CONVERT(varchar,GETDATE(),120) ";
		public const string SYSDATE_TO_CDTTM = " CONVERT(varchar,GETDATE(),112) + REPLACE(CONVERT(varchar,GETDATE(),108),':','') ";
#endif
#if POSTGRESSQL
        public const string SYSDATE = " NOW() ";
        public const string SYSDATE_TO_CDT = " TO_CHAR(NOW(), 'YYYYMMDD') ";
        public const string SYSDATE_TO_CTM = " TO_CHAR(NOW(), 'HH24MISS') ";
        public const string SYSDATETIME_TO_CDTM = " TO_CHAR(NOW(), 'YYYY-MM-DD HH24:MI:SS') ";
        public const string SYSDATE_TO_CDTTM = " TO_CHAR(NOW(), 'YYYYMMDDHH24MISS') ";
#endif


		public static string GetDateCode(DateTime aDate)
		{
			return aDate.ToString("yyyyMMdd");
		}
		public static string GetDateCode(DateTimePicker dtp)
		{
			return GetDateCode(dtp.Value);
		}
		public static string GetTimeCode(DateTime aTime)
		{
			return aTime.ToString("HHmmss");
		}
		public static string GetTimeCode(DateTimePicker dtp)
		{
			return GetTimeCode(dtp.Value);
		}
		public static Boolean IsNumeric(string stringToTest)
		{
			int result;
			return int.TryParse(stringToTest, out result);
		}
	}

}


