using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace TSK_HostCom
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
		//[DllImport("kernel32", EntryPoint = "GetPrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
		//public static extern int GetPrivateProfileString(string lpApplicationName, string lpKeyName, string lpDefault, string lpReturnedString, int nSize, string lpFileName);

		// @@@.INI파일에서 문자형 데이터를 읽어옴.
		[DllImport("kernel32.dll")]
		public static extern uint GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);

		///*********************************************************************************************
		/// [LGLS] INI파일의 섹션 전체를 읽어옴 (항목간 NULL 구분).
		///        스테이션 매핑표([StationMap_xxx])처럼 키 개수가 가변인 섹션을 읽는 데 사용.
		///*********************************************************************************************
		[DllImport("kernel32.dll", EntryPoint = "GetPrivateProfileSectionA", CharSet = CharSet.Ansi, SetLastError = true)]
		public static extern int GetPrivateProfileSection(string lpAppName, byte[] lpReturnedString, int nSize, string lpFileName);

		//--------------------------------------------
		// RYU 2013.02.05 Sleep을 분리하도록 수정
		//--------------------------------------------
		public static void SleepA(int p_iToTalMillSec, int p_iSleepMillSec = 100)
		{
			int iToTalMillSec = p_iToTalMillSec;

			while (true)
			{
				if (iToTalMillSec <= p_iSleepMillSec)
				{
					System.Threading.Thread.Sleep(iToTalMillSec);
					iToTalMillSec = 0;
					break; // TODO: might not be correct. Was : Exit While
				}
				else
				{
					System.Threading.Thread.Sleep(p_iSleepMillSec);
					iToTalMillSec = iToTalMillSec - p_iSleepMillSec;
				}
				Application.DoEvents();

			}

		}

	}
}
