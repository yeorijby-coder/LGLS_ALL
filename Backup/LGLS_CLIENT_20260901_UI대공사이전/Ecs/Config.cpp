// Config.cpp : implementation of the CConfig class
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Config.h"
#include <WinSock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CConfig
//
IMPLEMENT_DYNCREATE(CConfig, CObject)

CConfig::CConfig()
{
}
CConfig::CConfig(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	ASSERT(m_pDoc != NULL);

	GetPC_IP();

	//InitializeConfigWH();
	//InitializeConfigDATABASE();
	InitializeConfigUSER();

	LoadConfigWH(); //창고구분 가져오기
	LoadConfigDBTYPE(); //DB 타입 가져오기

	LoadConfigDATABASE(); //DB 타입별로 DB정보 가져오기
	LoadConfigUSER();
	
	//LoadConfigKiosk();
	//LoadConfigSound();
}

CConfig::~CConfig()
{
}

void CConfig::SaveConfigAll()
{
	SaveConfigWH();
	SaveConfigUSER();
	SaveConfigDATABASE();
}

void CConfig::LoadConfigWH()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("WH"), _T("WH_TYP"), _T("10"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_strWH_WH_TYP.Format(_T("%s"), szTemp);
}

void CConfig::LoadConfigDBTYPE()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("DB_TYPE"), _T("TYPE"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_strDB_TYPE.Format(_T("%s"), szTemp);
}

void CConfig::LoadConfigDATABASE()
{
	TCHAR szTemp[_MAX_PATH] = {0};

	//0:None, 1:Oracle, 2:PostgreSql, 3:MS_SQL, 4:MY_SQL
#if ORACLE
		::GetPrivateProfileString(_T("DB_1"), _T("DRIVER"), _T("Oracle in OraClient11g_home1") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_DRIVER.Format(_T("%s"), szTemp);
		::GetPrivateProfileString(_T("DB_1"), _T("SERVER"), _T("LOCALHOST") , szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_SERVER.Format(_T("%s"), szTemp); 
		::GetPrivateProfileString(_T("DB_1"), _T("USERID"), _T("LFC") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_USERID.Format(_T("%s"), szTemp); 
		::GetPrivateProfileString(_T("DB_1"), _T("USERPASSWORD"), _T("LFC") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_USERPASSWORD.Format(_T("%s"), szTemp);
#elif POSTGRESQL || MSSQL
		::GetPrivateProfileString(_T("DB_2"), _T("DRIVER"), _T("PostgreSQL Unicode") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_DRIVER.Format(_T("%s"), szTemp);
		::GetPrivateProfileString(_T("DB_2"), _T("SERVER"), _T("LOCALHOST") , szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_SERVER.Format(_T("%s"), szTemp); 
		::GetPrivateProfileString(_T("DB_2"), _T("USERID"), _T("LFC") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_USERID.Format(_T("%s"), szTemp); 
		::GetPrivateProfileString(_T("DB_2"), _T("USERPASSWORD"), _T("LFC") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_USERPASSWORD.Format(_T("%s"), szTemp); 
		::GetPrivateProfileString(_T("DB_2"), _T("DATABASE"), _T("LFC") ,  szTemp, _MAX_PATH, ECS_INI_FILE);
		m_strDATABASE_DATABASE.Format(_T("%s"), szTemp);
#endif
}

void CConfig::LoadConfigUSER()
{
	// [LGLS 2026-07-19] [ETC] ViewRetCnt : SC별 출고 잔여 수 표시(1=표시, 0=숨김)
	m_nETC_ViewRetCnt = ::GetPrivateProfileInt(_T("ETC"), _T("ViewRetCnt"), 0, ECS_INI_FILE);
	m_nUSER_LAST_LANG = ::GetPrivateProfileInt(_T("USER"), _T("LAST_LANG"), 1, ECS_INI_FILE);
	m_nUSER_LAST_TAB_INDEX = ::GetPrivateProfileInt(_T("USER"), _T("LAST_TAB_INDEX"), 1, ECS_INI_FILE);

	if (m_strWH_WH_TYP == _T("40"))
	{
		m_nUSER_LAST_TAB_INDEX = 1;
	}

	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_STO"),	CConvert::ToString(m_clrUSER_COLOR_STO),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_STO = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_RET"),	CConvert::ToString(m_clrUSER_COLOR_RET),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_RET = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_MOVE"),	CConvert::ToString(m_clrUSER_COLOR_MOVE),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_MOVE = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_RTR"),	CConvert::ToString(m_clrUSER_COLOR_RTR),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_RTR = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_WTW"),	CConvert::ToString(m_clrUSER_COLOR_WTW),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_WTW = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_ATA"),	CConvert::ToString(m_clrUSER_COLOR_ATA),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_WTW = CConvert::ToColor(CString(szTemp));

	// [LGLS 2026-07-19] 반자동 색상
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_STO"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_STO),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_STO = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_RET"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_RET),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_RET = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_MOVE"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_MOVE),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_MOVE = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_RTR"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_RTR),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_RTR = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_ATA"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_ATA),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_ATA = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_PR"),	CConvert::ToString(m_clrUSER_COLOR_SEMI_PR),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SEMI_PR = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_STN_STO"),	CConvert::ToString(m_clrUSER_COLOR_STN_STO),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_STN_STO = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_STN_RET"),	CConvert::ToString(m_clrUSER_COLOR_STN_RET),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_STN_RET = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_HS_STO"),	CConvert::ToString(m_clrUSER_COLOR_HS_STO),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_HS_STO = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_HS_RET"),	CConvert::ToString(m_clrUSER_COLOR_HS_RET),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_HS_RET = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SUSPEND"),	CConvert::ToString(m_clrUSER_COLOR_SUSPEND),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_SUSPEND = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_RET_SUSPEND"),	CConvert::ToString(m_clrUSER_COLOR_RET_SUSPEND),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_RET_SUSPEND = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_STO_SUSPEND"),	CConvert::ToString(m_clrUSER_COLOR_STO_SUSPEND),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_STO_SUSPEND = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_STORET_SUSPEND"),	CConvert::ToString(m_clrUSER_COLOR_ALL_SUSPEND),	szTemp, _MAX_PATH,ECS_INI_FILE);	
	m_clrUSER_COLOR_ALL_SUSPEND = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_RAIL_ERROR"),	CConvert::ToString(m_clrUSER_COLOR_RAIL_ERROR),	szTemp, _MAX_PATH,ECS_INI_FILE);	
	m_clrUSER_COLOR_RAIL_ERROR = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_MANUAL"),	CConvert::ToString(m_clrUSER_COLOR_MANUAL),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_MANUAL = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_DISCONNECT"),	CConvert::ToString(m_clrUSER_COLOR_DISCONNECT),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_DISCONNECT = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_ERROR"),	CConvert::ToString(m_clrUSER_COLOR_ERROR),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_ERROR = CConvert::ToColor(CString(szTemp));


	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_WC_PASS"),	CConvert::ToString(m_clrUSER_COLOR_WC_PASS),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_WC_PASS = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_CV_SEARCH"),	CConvert::ToString(m_clrUSER_COLOR_CV_SEARCH),	szTemp, _MAX_PATH, ECS_INI_FILE);
	m_clrUSER_COLOR_CV_SEARCH = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("USER"), _T("USER_COLOR_SC_INVK"),	CConvert::ToString(m_clrUSER_COLOR_SC_INVK),	szTemp, _MAX_PATH, ECS_INI_FILE);	
	m_clrUSER_COLOR_SC_INVK = CConvert::ToColor(CString(szTemp));
		
}

void CConfig::InitializeConfigWH()
{
	m_strWH_WH_TYP = _T("10");
}

void CConfig::InitializeConfigDATABASE()
{
	m_strDATABASE_SERVER = _T("LOCALHOST");
	m_strDATABASE_USERID = _T("LFC");
	m_strDATABASE_USERPASSWORD = _T("LFC");
	m_strDATABASE_DATABASE = _T("LFC");
}

void CConfig::InitializeConfigUSER()
{
	m_nUSER_LAST_LANG = 1; 
	m_nUSER_LAST_TAB_INDEX = 1;

	//스프레드 화면 색상
	m_clrSPREAD_COLOR = RGB(237,239,255);

	m_clrUSER_COLOR_STO = GREEN;			// 
	m_clrUSER_COLOR_RET = BLUE;				//
	m_clrUSER_COLOR_MOVE = CYAN;			//
	m_clrUSER_COLOR_ATA = MAGENTA;		
	m_clrUSER_COLOR_WTW = SITE;				// 현재는 사용안함!
	m_clrUSER_COLOR_RTR = MAGENTA;		

	// [LGLS 2026-07-19] 반자동 = 자동의 다크 색 (입고 GREEN->DARK GREEN, 출고 BLUE->DARK BLUE ...)
	m_clrUSER_COLOR_SEMI_STO  = RGB(0,128,0);
	m_clrUSER_COLOR_SEMI_RET  = RGB(0,0,139);
	m_clrUSER_COLOR_SEMI_MOVE = RGB(0,139,139);
	m_clrUSER_COLOR_SEMI_RTR  = RGB(139,0,139);
	m_clrUSER_COLOR_SEMI_ATA  = RGB(139,0,98);
	m_clrUSER_COLOR_SEMI_PR   = RGB(25,25,112);

	m_clrUSER_COLOR_STN_STO = GREEN;
	m_clrUSER_COLOR_STN_RET = BLUE;
	// [LGLS 2026-08-22] 핸드셰이크 색 규칙 : 싣는 곳(Pick)=오렌지 / 내려놓는 곳(Drop)=진한 초록
	m_clrUSER_COLOR_HS_STO	= RGB(255,140,0);
	m_clrUSER_COLOR_HS_RET	= RGB(0,128,0);
	m_clrUSER_COLOR_SUSPEND = RED;

	m_clrUSER_COLOR_ERROR = RED;			//
	m_clrUSER_COLOR_MANUAL = YELLOW;		// 
	m_clrUSER_COLOR_DISCONNECT = WHITE;
	m_clrUSER_COLOR_WC_PASS = SITE;			
	m_clrUSER_COLOR_CV_SEARCH = PINK;

	m_clrUSER_COLOR_STO_SUSPEND = GREEN;	//
	m_clrUSER_COLOR_RET_SUSPEND = BLUE;		//
	m_clrUSER_COLOR_ALL_SUSPEND = RED;		//
	m_clrUSER_COLOR_RAIL_ERROR = RED;		
	m_clrUSER_COLOR_SC_INVK = PINK;			

	m_clrUSER_COLOR_READY_OFF = DARK_GRAY;
//	m_clrMES_ERROR_CD = RED;
}

void CConfig::SaveConfigWH()
{
	::WritePrivateProfileString(_T("WH"), _T("WHTYP"), m_strWH_WH_TYP, ECS_INI_FILE);
}

void CConfig::SaveConfigLANG()
{
	::WritePrivateProfileString(_T("USER"), _T("LAST_LANG"), CConvert::ToString(m_nUSER_LAST_LANG), ECS_INI_FILE);
}

void CConfig::SaveConfigMONITOR()
{
	::WritePrivateProfileString(_T("USER"), _T("LAST_TAB_INDEX"), CConvert::ToString(m_nUSER_LAST_TAB_INDEX), ECS_INI_FILE);
}

void CConfig::SaveConfigDATABASE()
{
	//0:None, 1:Oracle, 2:PostgreSql, 3:MS_SQL, 4:MY_SQL
#if ORACLE
		::WritePrivateProfileString(_T("DB_1"), _T("DRIVER"), m_strDATABASE_DRIVER, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_1"), _T("SERVER"), m_strDATABASE_SERVER, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_1"), _T("USERID"), m_strDATABASE_USERID, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_1"), _T("USERPASSWORD"), m_strDATABASE_USERPASSWORD, ECS_INI_FILE);
#elif POSTGRESQL || MSSQL
		::WritePrivateProfileString(_T("DB_2"), _T("DRIVER"), m_strDATABASE_DRIVER, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_2"), _T("SERVER"), m_strDATABASE_SERVER, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_2"), _T("USERID"), m_strDATABASE_USERID, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_2"), _T("USERPASSWORD"), m_strDATABASE_USERPASSWORD, ECS_INI_FILE);
		::WritePrivateProfileString(_T("DB_2"), _T("DATABASE"), m_strDATABASE_DATABASE, ECS_INI_FILE);
#endif
	
}

void CConfig::SaveConfigUSER()
{
	::WritePrivateProfileString(_T("USER"), _T("LAST_LANG"), CConvert::ToString(m_nUSER_LAST_LANG), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("LAST_TAB_INDEX"), CConvert::ToString(m_nUSER_LAST_TAB_INDEX), ECS_INI_FILE);

	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_STO"),    CConvert::ToString(m_clrUSER_COLOR_STO), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_RET"),    CConvert::ToString(m_clrUSER_COLOR_RET), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_MOVE"),   CConvert::ToString(m_clrUSER_COLOR_MOVE), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_RTR"),    CConvert::ToString(m_clrUSER_COLOR_RTR), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_WTW"),    CConvert::ToString(m_clrUSER_COLOR_WTW), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_ATA"),    CConvert::ToString(m_clrUSER_COLOR_ATA), ECS_INI_FILE);

	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_STN_STO"),    CConvert::ToString(m_clrUSER_COLOR_STN_STO), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_STN_RET"),    CConvert::ToString(m_clrUSER_COLOR_STN_RET), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_HS_STO"),    CConvert::ToString(m_clrUSER_COLOR_HS_STO), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_HS_RET"),    CConvert::ToString(m_clrUSER_COLOR_HS_RET), ECS_INI_FILE);
	// [LGLS 2026-07-19] 반자동 색상
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_STO"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_STO), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_RET"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_RET), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_MOVE"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_MOVE), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_RTR"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_RTR), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_ATA"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_ATA), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SEMI_PR"),    CConvert::ToString(m_clrUSER_COLOR_SEMI_PR), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SUSPEND"),    CConvert::ToString(m_clrUSER_COLOR_SUSPEND), ECS_INI_FILE);

	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_STO_SUSPEND"),    CConvert::ToString(m_clrUSER_COLOR_STO_SUSPEND), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_RET_SUSPEND"),    CConvert::ToString(m_clrUSER_COLOR_RET_SUSPEND), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_STORET_SUSPEND"),    CConvert::ToString(m_clrUSER_COLOR_ALL_SUSPEND), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_RAIL_ERROR"),    CConvert::ToString(m_clrUSER_COLOR_RAIL_ERROR), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_MANUAL"),    CConvert::ToString(m_clrUSER_COLOR_MANUAL), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_DISCONNECT"),    CConvert::ToString(m_clrUSER_COLOR_DISCONNECT), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_ERROR"),    CConvert::ToString(m_clrUSER_COLOR_ERROR), ECS_INI_FILE);

	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_WC_PASS"),    CConvert::ToString(m_clrUSER_COLOR_WC_PASS), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_CV_SEARCH"),    CConvert::ToString(m_clrUSER_COLOR_CV_SEARCH), ECS_INI_FILE);
	::WritePrivateProfileString(_T("USER"), _T("USER_COLOR_SC_INVK"),    CConvert::ToString(m_clrUSER_COLOR_SC_INVK), ECS_INI_FILE);

	return;
}
	


BOOL CConfig::MakeSurePathExists(CString strPath)
{
	if (strPath.IsEmpty())		
		return FALSE;

	if (strPath.Right(1) == _T("\\"))
		strPath = strPath.Left(strPath.GetLength()-1); 
	
	if (GetFileAttributes(strPath) != 0xFFFFFFFF)
		return TRUE;
	
	int nPos = strPath.ReverseFind(_T('\\'));	
	if (MakeSurePathExists(strPath.Left(nPos)) == FALSE)
		return FALSE;
	
	return CreateDirectory(strPath, NULL); 
}

BOOL CConfig::MakeSureFileExists(CString strFile)
{
	if (strFile.IsEmpty())		
		return FALSE;

	if (strFile.Right(1) == _T("\\"))
		return FALSE;

	if (GetFileAttributes(strFile) == 0xFFFFFFFF)
		return FALSE;

	return TRUE;
}

void CConfig::LoadConfigKiosk()
{
	CString strKioskIp = GetIpAddress();
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("KIOSK"), strKioskIp, _T("0"), szTemp, _MAX_PATH, IP_INI_FILE);
	m_pDoc->m_strKioskNo.Format(_T("%s"), szTemp); 
}

void CConfig::LoadConfigSound()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("SIREN"), _T("SOUND"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_pDoc->m_strSound.Format(_T("%s"), szTemp); 
}

CString CConfig::GetIpAddress()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    PHOSTENT hostinfo;
    CString strIpAddress = _T("");
    wVersionRequested = MAKEWORD(2, 0);

    if(WSAStartup(wVersionRequested, &wsaData) == 0)
    {
        if(gethostname(name, sizeof(name)) == 0)
        {
            if((hostinfo = gethostbyname(name)) != NULL)
                strIpAddress = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
        }
        WSACleanup();
    }

    //AfxMessageBox(strIpAddress);

    return strIpAddress;
}

void CConfig::GetPC_IP()
{
	char name[255];
	CString strLocalIP;
	PHOSTENT hostinfo;
	WSADATA wsaData;
	WORD wVersjon = MAKEWORD(1,1);
	if(WSAStartup(wVersjon, &wsaData) == 0)
	{
		if(!::gethostname(name,255))
		{
			if((hostinfo=gethostbyname(name)) != NULL)
			{
				strLocalIP = inet_ntoa(*(struct in_addr*)* hostinfo->h_addr_list);
			}
		}
		WSACleanup();
		m_pDoc->m_PC_IP = strLocalIP;
	}
}