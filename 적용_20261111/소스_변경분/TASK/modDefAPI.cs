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
				// [LGLS 2026-11-11] 작업 스레드(접속 수락 · 송신 · 수신)가 이 함수로 쉬면서 화면 이벤트를 처리하던 것을 막는다.
				//   DoEvents 는 화면 스레드에서만 의미가 있고, 다른 스레드에서 부르면 재진입·크로스 스레드 문제의 원인이 된다.
				{
					System.Windows.Forms.Form f = modDefApp.g_frmForm;
					if (f == null || !f.IsHandleCreated || !f.InvokeRequired)
						Application.DoEvents();
				}

			}

		}

	}
}
