using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WmsUp
{
	class modDefAPI
	{
		///*********************************************************************************************
		/// INI파일에서 정수형 데이터를 읽어옴.
		///*********************************************************************************************
		[DllImport("kernel32", EntryPoint = "GetPrivateProfileIntA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
		public static extern int GetPrivateProfileInt(string lpApplicationName, string lpKeyName, int nDefault, string lpFileName);

		///*********************************************************************************************
		/// INI파일에서 문자형 데이터를 읽어옴.
		///*********************************************************************************************
		///
		//[DllImport("kernel32", EntryPoint = "GetPrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
		//public static extern int GetPrivateProfileString(string lpApplicationName, string lpKeyName, string lpDefault, string lpReturnedString, int nSize, string lpFileName);

		//*********************************************************************************************
		// INI파일에서 문자형 데이터를 씀.
		//*********************************************************************************************
		//*********************************************************************************************
		[DllImport("kernel32", EntryPoint = "WritePrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
		public static extern int WritePrivateProfileString(string lpApplicationName, string lpKeyName, string lpString, string lpFileName);


		// @@@.INI파일에서 문자형 데이터를 읽어옴.
		[DllImport("kernel32.dll")]
		public static extern uint GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);

	}
}
