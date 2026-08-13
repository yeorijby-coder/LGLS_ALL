// Config.cpp : implementation of the CConfig class
//

#include "stdafx.h"
#include "Ecs.h"
#include "Config.h"

#include "EcsDoc.h"

CString g_strEcsPath;
int g_nTabCount;

int g_nPlcCount;


///////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CConfig, CObject)

CConfig::CConfig()
{
	m_pDoc = NULL;
	InitializeConfig();
//	m_strStartTime = _T("0");
	m_strPlaybackPath = "";
}

CConfig::CConfig(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	ASSERT(m_pDoc != NULL);

//	m_strStartTime = _T("0");
	InitializeConfig();
}

CConfig::~CConfig()
{

}

void CConfig::InitializeConfig()
{
	InitializePath();
	InitializeColor();

	LoadConfig1();
	LoadConfig2();
	LoadConfig3();
	LoadConfig4();

	m_dwResendLimit = ::GetPrivateProfileInt(_T("COMMON"), _T("SocketResendLimit"), 5, ECS_INI_FILE); //소켓통신 재수신 한계 5번
	m_dwRecvTimeout = ::GetPrivateProfileInt(_T("COMMON"), _T("SocketRecvTimeout"), 3000, ECS_INI_FILE); //소켓 수신 초과 시간 3초
	m_dwExitTimeout = ::GetPrivateProfileInt(_T("COMMON"), _T("ThreadExitTimeout"), 3000, ECS_INI_FILE); //쓰레드 종료 시간 3초
	m_bLogTrace		= ::GetPrivateProfileInt(_T("COMMON"), _T("LogTrace"), FALSE, ECS_INI_FILE);
}

void CConfig::InitializePath()
{
	TCHAR szPath[_MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath.Format(_T("%s"),szPath);
	m_strEcsPath.Format(_T("%s"),szPath);
	MakeSurePathExists(m_strEcsPath);
	m_strJobPath.Format(_T("%s\\Job"), m_strEcsPath);
	m_strLogPath.Format(_T("%s\\Log"), m_strEcsPath);
	m_strStcPath.Format(_T("%s\\Stc"), m_strEcsPath);
	m_strInfoPath.Format(_T("%s\\Temp"), m_strEcsPath);
	m_strPlaybackPath.Format(_T("%s\\PlayBack"), m_strEcsPath);
}

void CConfig::InitializeColor()
{
	m_clrAutoSto	= GREEN;
	m_clrAutoRet	= BLUE;
	m_clrAutoMove	= CYAN;
	m_clrAutoR2R	= MAGENTA;
	m_clrAutoW2W	= SITE;
	m_clrSemiSto	= DARK_GREEN;
	m_clrSemiRet	= DARK_BLUE;
	m_clrSemiMove	= DARK_CYAN;
	m_clrSemiR2R	= DARK_MAGENTA;
	m_clrSemiW2W	= DARK_SITE;
	m_clrStoST		= 0xFF00FF;
	m_clrRetST		= 0xFF8000;
	m_clrArvST		= 0xFF8000;
	m_clrDepart		= 0x4080FF;
	m_clrArrive		= 0x808000;
	m_clrDisconnect = 0xFFFFFF;
	m_clrSuspend	= 0x8000ff;
	m_clrManual		= YELLOW;
	m_clrAlarm		= BLACK;	//ORANGE;	// DeadLock으로 사용
	m_clrError		= RED;
}

void CConfig::LoadConfig1()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	m_portHost.m_strDevice = _T("HOST");
	::GetPrivateProfileString(m_portHost.m_strDevice, _T("IP"), _T("127.0.0.1"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_portHost.m_strIP.Format(_T("%s"), szTemp);

	m_portHost.m_nPort[0]		 = ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Port"), 7000, ECS_INI_FILE);
	m_portHost.m_bConnect	 = ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Connect"), 1, ECS_INI_FILE);
	m_portHost.m_bRetry		 = ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Retry"), 1, ECS_INI_FILE);
	m_portHost.m_bWriteLog	 = ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("WriteLog"), 0, ECS_INI_FILE);
	m_nEcsPort = ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("ServerPort"), 8000, ECS_INI_FILE);
	m_nTabCount				= ::GetPrivateProfileInt(_T("COMMON"), _T("TabCount"), 0, ECS_INI_FILE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strTemp;
	CEquipment* pEquipment = NULL;
	int nEquipCnt = m_pDoc->m_pEquipments.GetSize();
	for (int i=0; i<nEquipCnt; ++i)
	{
		pEquipment = m_pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		CInfo* pInfo = pEquipment->GetInfo();

		if (pInfo != NULL && pInfo->m_bEtherNet == FALSE)
		{
			CPortSerial& portSerial = pEquipment->m_portSerial;

			::GetPrivateProfileString(portSerial.m_strDevice, _T("ComName"), _T("COM6"), szTemp, _MAX_PATH, ECS_INI_FILE);
			portSerial.m_strComName.Format(_T("%s"), szTemp);

			portSerial.m_nBaudRate = ::GetPrivateProfileInt(portSerial.m_strDevice, _T("BaudRate"), 9600, ECS_INI_FILE);
			portSerial.m_nParity = ::GetPrivateProfileInt(portSerial.m_strDevice, _T("Parity"), 0, ECS_INI_FILE);
			portSerial.m_nStopBits = ::GetPrivateProfileInt(portSerial.m_strDevice, _T("StopBits"), 0, ECS_INI_FILE);
			portSerial.m_nBitsPerChar = ::GetPrivateProfileInt(portSerial.m_strDevice, _T("BitsPerChar"), 8, ECS_INI_FILE);
			portSerial.m_dwReadTimeOut = ::GetPrivateProfileInt(portSerial.m_strDevice, _T("ReadTimeOut"), 2000, ECS_INI_FILE);
		}
		else
		{
			CPortEthernet& port = pEquipment->m_port;
			::GetPrivateProfileString(port.m_strDevice, _T("IP"), _T("127.0.0.1"), szTemp, _MAX_PATH, ECS_INI_FILE);
			port.m_strIP.Format(_T("%s"), szTemp);

			port.m_bConnect = ::GetPrivateProfileInt(port.m_strDevice, _T("Connect"), 1, ECS_INI_FILE);
			port.m_bRetry = ::GetPrivateProfileInt(port.m_strDevice, _T("Retry"), 1, ECS_INI_FILE);
			port.m_bWriteLog = ::GetPrivateProfileInt(port.m_strDevice, _T("WriteLog"), 0, ECS_INI_FILE);
			port.m_dwRecvTimeout = ::GetPrivateProfileInt(port.m_strDevice, _T("RecvTimeout"), 3000, ECS_INI_FILE);
			port.m_dwReadTime = ::GetPrivateProfileInt(port.m_strDevice, _T("ReadTime"), 300, ECS_INI_FILE);

			for (int j = 0; j < PLC_CONN_PORT_CNT; j++)
			{
				strTemp.Format(_T("Port%d"), j + 1);
				port.m_nPort[j] = ::GetPrivateProfileInt(port.m_strDevice, strTemp, 0, ECS_INI_FILE);

				if (pEquipment->m_pSocket[j])
					pEquipment->m_pSocket[j]->m_bWriteLog = port.m_bWriteLog;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_bJob		= ::GetPrivateProfileInt(_T("LOG"), _T("Job"), 1, ECS_INI_FILE);
	m_bEvent	= ::GetPrivateProfileInt(_T("LOG"), _T("Event"), 1, ECS_INI_FILE);
	m_bError	= ::GetPrivateProfileInt(_T("LOG"), _T("Error"), 1, ECS_INI_FILE);
	m_bAlarm	= ::GetPrivateProfileInt(_T("LOG"), _T("Alarm"), 1, ECS_INI_FILE);
	m_bDebug	= ::GetPrivateProfileInt(_T("LOG"), _T("Debug"), 1, ECS_INI_FILE);
	m_bBeep		= ::GetPrivateProfileInt(_T("LOG"), _T("Beep"), 1, ECS_INI_FILE);
}

void CConfig::SaveConfig1()
{
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("IP"), m_portHost.m_strIP, ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Port"), CConvert::ToString(m_portHost.m_nPort[0]), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Connect"), CConvert::ToString((int)m_portHost.m_bConnect), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Retry"), CConvert::ToString((int)m_portHost.m_bRetry), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("WriteLog"), CConvert::ToString((int)m_portHost.m_bWriteLog), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("ServerPort"), CConvert::ToString(m_nEcsPort), ECS_INI_FILE);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString strTemp;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = m_pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		CInfo* pInfo = pEquipment->GetInfo();

		if (pInfo != NULL && pInfo->m_bEtherNet == FALSE)
		{
			CPortSerial& portSerial = pEquipment->m_portSerial;
			::WritePrivateProfileString(portSerial.m_strDevice, _T("ComName"), portSerial.m_strComName, ECS_INI_FILE);
			::WritePrivateProfileString(portSerial.m_strDevice, _T("BaudRate"), CConvert::ToString((int)portSerial.m_nBaudRate), ECS_INI_FILE);
			::WritePrivateProfileString(portSerial.m_strDevice, _T("Parity"), CConvert::ToString((int)portSerial.m_nParity), ECS_INI_FILE);
			::WritePrivateProfileString(portSerial.m_strDevice, _T("StopBits"), CConvert::ToString((int)portSerial.m_nStopBits), ECS_INI_FILE);
			::WritePrivateProfileString(portSerial.m_strDevice, _T("BitsPerChar"), CConvert::ToString((int)portSerial.m_nBitsPerChar), ECS_INI_FILE);
			::WritePrivateProfileString(portSerial.m_strDevice, _T("ReadTimeOut"), CConvert::ToString((int)portSerial.m_dwReadTimeOut), ECS_INI_FILE);
		}
		else
		{
			CPortEthernet& port = pEquipment->m_port;
			::WritePrivateProfileString(port.m_strDevice, _T("IP"), port.m_strIP, ECS_INI_FILE);
			::WritePrivateProfileString(port.m_strDevice, _T("Connect"), CConvert::ToString((int)port.m_bConnect), ECS_INI_FILE);
			::WritePrivateProfileString(port.m_strDevice, _T("Retry"), CConvert::ToString((int)port.m_bRetry), ECS_INI_FILE);
			::WritePrivateProfileString(port.m_strDevice, _T("WriteLog"), CConvert::ToString((int)port.m_bWriteLog), ECS_INI_FILE);
			::WritePrivateProfileString(port.m_strDevice, _T("RecvTimeout"), CConvert::ToString((int)port.m_dwRecvTimeout), ECS_INI_FILE);

			for (int j = 0; j < PLC_CONN_PORT_CNT; j++)
			{
				strTemp.Format(_T("Port%d"), j + 1);
				::WritePrivateProfileString(port.m_strDevice, strTemp, CConvert::ToString(port.m_nPort[j]), ECS_INI_FILE);
				if (pEquipment->m_pSocket[j])
					pEquipment->m_pSocket[j]->m_bWriteLog = port.m_bWriteLog;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	::WritePrivateProfileString(_T("LOG"), _T("Job"), CConvert::ToString((int)m_bJob), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOG"), _T("Event"), CConvert::ToString((int)m_bEvent), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOG"), _T("Error"), CConvert::ToString((int)m_bError), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOG"), _T("Alarm"), CConvert::ToString((int)m_bAlarm), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOG"), _T("Debug"), CConvert::ToString((int)m_bDebug), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOG"), _T("Beep"), CConvert::ToString((int)m_bBeep), ECS_INI_FILE);
}

void CConfig::LoadConfig2()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("PATH"), _T("Ecs"),		m_strEcsPath,		szTemp, _MAX_PATH, ECS_INI_FILE);	m_strEcsPath.Format(_T("%s"),		szTemp);
	::GetPrivateProfileString(_T("PATH"), _T("Job"),		m_strJobPath,		szTemp, _MAX_PATH, ECS_INI_FILE);	m_strJobPath.Format(_T("%s"),		szTemp);
	::GetPrivateProfileString(_T("PATH"), _T("Log"),		m_strLogPath,		szTemp, _MAX_PATH, ECS_INI_FILE);	m_strLogPath.Format(_T("%s"),		szTemp);
	::GetPrivateProfileString(_T("PATH"), _T("Stc"),		m_strStcPath,		szTemp, _MAX_PATH, ECS_INI_FILE);	m_strStcPath.Format(_T("%s"),		szTemp);
	::GetPrivateProfileString(_T("PATH"), _T("Temp"),		m_strInfoPath,		szTemp, _MAX_PATH, ECS_INI_FILE);	m_strInfoPath.Format(_T("%s"),		szTemp);
	::GetPrivateProfileString(_T("PATH"), _T("PlayBack"),	m_strPlaybackPath,	szTemp, _MAX_PATH, ECS_INI_FILE);	m_strPlaybackPath.Format(_T("%s"),	szTemp);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	::GetPrivateProfileString(_T("COLOR"),  _T("AutoSto"),  CConvert::ToString(m_clrAutoSto), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrAutoSto = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("AutoRet"),  CConvert::ToString(m_clrAutoRet), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrAutoRet = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("AutoMove"), CConvert::ToString(m_clrAutoMove), szTemp, _MAX_PATH, ECS_INI_FILE);	 m_clrAutoMove = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("AutoRtR"),  CConvert::ToString(m_clrAutoR2R), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrAutoR2R = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("AutoWtW"),  CConvert::ToString(m_clrAutoW2W), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrAutoW2W = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("COLOR"),  _T("SemiSto"),  CConvert::ToString(m_clrSemiSto), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrSemiSto = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("SemiRet"),  CConvert::ToString(m_clrSemiRet), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrSemiRet = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("SemiMove"), CConvert::ToString(m_clrSemiMove), szTemp, _MAX_PATH, ECS_INI_FILE);	 m_clrSemiMove = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("SemiRtR"),  CConvert::ToString(m_clrSemiR2R), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrSemiR2R = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("SemiWtW"),  CConvert::ToString(m_clrSemiW2W), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrSemiW2W = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("COLOR"),  _T("StoST"),  CConvert::ToString(m_clrStoST), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrStoST = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("RetST"),  CConvert::ToString(m_clrRetST), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrRetST = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),  _T("ArvST"),  CConvert::ToString(m_clrArvST), szTemp, _MAX_PATH, ECS_INI_FILE);	  m_clrArvST = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("Depart"), CConvert::ToString(m_clrDepart), szTemp, _MAX_PATH, ECS_INI_FILE);	 m_clrDepart = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("Arrive"), CConvert::ToString(m_clrArrive), szTemp, _MAX_PATH, ECS_INI_FILE);	 m_clrArrive = CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("COLOR"), _T("Disconnect"), CConvert::ToString(m_clrDisconnect), szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrDisconnect = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),    _T("Suspend"), CConvert::ToString(m_clrSuspend), szTemp, _MAX_PATH, ECS_INI_FILE);	   m_clrSuspend = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),     _T("Manual"), CConvert::ToString(m_clrManual), szTemp, _MAX_PATH, ECS_INI_FILE);		    m_clrManual = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),      _T("Error"), CConvert::ToString(m_clrError), szTemp, _MAX_PATH, ECS_INI_FILE);		     m_clrError = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),      _T("Alarm"), CConvert::ToString(m_clrAlarm), szTemp, _MAX_PATH, ECS_INI_FILE);		     m_clrAlarm = CConvert::ToColor(CString(szTemp));
}

void CConfig::SaveConfig2()
{
	::WritePrivateProfileString(_T("PATH"),  _T("Ecs"),  m_strEcsPath, ECS_INI_FILE);
	::WritePrivateProfileString(_T("PATH"),  _T("Job"),  m_strJobPath, ECS_INI_FILE);
	::WritePrivateProfileString(_T("PATH"),  _T("Log"),  m_strLogPath, ECS_INI_FILE);
	::WritePrivateProfileString(_T("PATH"),  _T("Stc"),  m_strStcPath, ECS_INI_FILE);
	::WritePrivateProfileString(_T("PATH"), _T("Temp"), m_strInfoPath, ECS_INI_FILE);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	::WritePrivateProfileString(_T("COLOR"),  _T("AutoSto"),  CConvert::ToString(m_clrAutoSto), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("AutoRet"),  CConvert::ToString(m_clrAutoRet), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("AutoMove"), CConvert::ToString(m_clrAutoMove), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("AutoRtR"),  CConvert::ToString(m_clrAutoR2R), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("AutoWtW"),  CConvert::ToString(m_clrAutoW2W), ECS_INI_FILE);

	::WritePrivateProfileString(_T("COLOR"),  _T("SemiSto"),  CConvert::ToString(m_clrSemiSto), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("SemiRet"),  CConvert::ToString(m_clrSemiRet), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("SemiMove"), CConvert::ToString(m_clrSemiMove), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("SemiRtR"),  CConvert::ToString(m_clrSemiR2R), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("SemiWtW"),  CConvert::ToString(m_clrSemiW2W), ECS_INI_FILE);

	::WritePrivateProfileString(_T("COLOR"),  _T("StoST"),  CConvert::ToString(m_clrStoST), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("RetST"),  CConvert::ToString(m_clrRetST), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),  _T("ArvST"),  CConvert::ToString(m_clrArvST), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("Depart"), CConvert::ToString(m_clrDepart), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("Arrive"), CConvert::ToString(m_clrArrive), ECS_INI_FILE);

	::WritePrivateProfileString(_T("COLOR"), _T("Disconnect"), CConvert::ToString(m_clrDisconnect), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),    _T("Suspend"),    CConvert::ToString(m_clrSuspend), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),     _T("Manual"),     CConvert::ToString(m_clrManual), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),      _T("Error"),      CConvert::ToString(m_clrError), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"),      _T("Alarm"),      CConvert::ToString(m_clrAlarm), ECS_INI_FILE);
}

void CConfig::LoadConfig3()
{
	m_nMonPort = ::GetPrivateProfileInt(_T("MONITOR"), _T("ServerPort"), 9000, ECS_INI_FILE);
	m_bMonLog  = ::GetPrivateProfileInt(_T("MONITOR"), _T("WriteLog"), FALSE, ECS_INI_FILE);
}

void CConfig::SaveConfig3()
{
	::WritePrivateProfileString(_T("MONITOR"), _T("ServerPort"), CConvert::ToString(m_nMonPort), ECS_INI_FILE);
	::WritePrivateProfileString(_T("MONITOR"), _T("WriteLog"), CConvert::ToString(m_bMonLog), ECS_INI_FILE);
}

void CConfig::LoadConfig4()
{
	// 온습도 정보
	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("THS"), _T("TempMax"), m_strTempMax, szTemp, _MAX_PATH, ECS_INI_FILE);	m_strTempMax.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("THS"), _T("TempMin"), m_strTempMin, szTemp, _MAX_PATH, ECS_INI_FILE);	m_strTempMin.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("THS"), _T("HumMax"), m_strHumMax, szTemp, _MAX_PATH, ECS_INI_FILE);	m_strHumMax.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("THS"), _T("HumMin"), m_strHumMin, szTemp, _MAX_PATH, ECS_INI_FILE);	m_strHumMin.Format(_T("%s"), szTemp);
// Update : 2014.06.27 - 입고포트 이동제어
//	::GetPrivateProfileString(_T("IN_PORT"), _T("StartTime"), m_strStartTime, szTemp, _MAX_PATH, ECS_INI_FILE);	m_strStartTime.Format(_T("%s"), szTemp);
// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
	// 물류흐름 제어 정보
	::GetPrivateProfileString(_T("TRAY"), _T("LoopTrackCount"), _T("174"), szTemp, _MAX_PATH, ECS_INI_FILE);	m_strLoopTrackCount.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("Capacity"), _T("50"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strCapacity.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("Mode"), _T("1"), szTemp, _MAX_PATH, ECS_INI_FILE);				m_strMode.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort1"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging01.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort2"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging02.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort3"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging03.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort4"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging04.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort5"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging05.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("AgingPort6"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strAging06.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("CdePort1"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strCde01.Format(_T("%s"), szTemp);
	::GetPrivateProfileString(_T("TRAY"), _T("CdePort2"), _T("0"), szTemp, _MAX_PATH, ECS_INI_FILE);			m_strCde02.Format(_T("%s"), szTemp);
	m_tagManualBits.tagBit.Bit1 = _ttoi(m_strAging01);
	m_tagManualBits.tagBit.Bit2 = _ttoi(m_strAging02);
	m_tagManualBits.tagBit.Bit3 = _ttoi(m_strAging03);
	m_tagManualBits.tagBit.Bit4 = _ttoi(m_strAging04);
	m_tagManualBits.tagBit.Bit5 = _ttoi(m_strAging05);
	m_tagManualBits.tagBit.Bit6 = _ttoi(m_strAging06);
	m_tagManualBits.tagBit.Bit7 = _ttoi(m_strCde01);
	m_tagManualBits.tagBit.Bit8 = _ttoi(m_strCde02);
//----------------------------------------------------------------------------------------------------------------------------------------------------
}

void CConfig::SaveConfig4()
{
	// 온습도 정보
	::WritePrivateProfileString(_T("THS"), _T("TempMax"), m_strTempMax, ECS_INI_FILE);
	::WritePrivateProfileString(_T("THS"), _T("TempMin"), m_strTempMin, ECS_INI_FILE);
	::WritePrivateProfileString(_T("THS"), _T("HumMax"), m_strHumMax, ECS_INI_FILE);
	::WritePrivateProfileString(_T("THS"), _T("HumMin"), m_strHumMin, ECS_INI_FILE);
	// 클라이언트로 온습도 설정값 송신
	CString strSend = _T(""), strXml = _T("");
	strXml.Format(_T("<CONFIG DATA='%04s%04s%04s%04s'/>"), m_strTempMax, m_strTempMin, m_strHumMax, m_strHumMin);
	strSend.Format(_T("%c<ECS><SYS><THS>%s</THS></SYS></ECS>%c"), CMonitorServer::enSTX, strXml, CMonitorServer::enETX);
	
// Update : 2014.06.27 - 입고포트 이동제어
//	::WritePrivateProfileString(_T("IN_PORT"), _T("StartTime"), m_strStartTime, ECS_INI_FILE);
// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
	// 물류흐름 제어 정보
	::WritePrivateProfileString(_T("TRAY"), _T("LoopTrackCount"), m_strLoopTrackCount, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("Capacity"), m_strCapacity, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("Mode"), m_strMode, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort1"), m_strAging01, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort2"), m_strAging02, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort3"), m_strAging03, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort4"), m_strAging04, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort5"), m_strAging05, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("AgingPort6"), m_strAging06, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("CdePort1"), m_strCde01, ECS_INI_FILE);
	::WritePrivateProfileString(_T("TRAY"), _T("CdePort2"), m_strCde02, ECS_INI_FILE);
	m_tagManualBits.tagBit.Bit1 = _ttoi(m_strAging01);
	m_tagManualBits.tagBit.Bit2 = _ttoi(m_strAging02);
	m_tagManualBits.tagBit.Bit3 = _ttoi(m_strAging03);
	m_tagManualBits.tagBit.Bit4 = _ttoi(m_strAging04);
	m_tagManualBits.tagBit.Bit5 = _ttoi(m_strAging05);
	m_tagManualBits.tagBit.Bit6 = _ttoi(m_strAging06);
	m_tagManualBits.tagBit.Bit7 = _ttoi(m_strCde01);
	m_tagManualBits.tagBit.Bit8 = _ttoi(m_strCde02);
	// 클라이언트로 출고 물류흐름 제어 설정값 송신
	strXml.Format(_T("<CONFIG DATA='%03s%03s'/>"), m_strLoopTrackCount, m_strCapacity);
	strSend.Format(_T("%c<ECS><SYS><TRAY>%s</TRAY></SYS></ECS>%c"), CMonitorServer::enSTX, strXml, CMonitorServer::enETX);
	
//----------------------------------------------------------------------------------------------
}

CString CConfig::GetJobDirectoryPath()
{
	int nLen = m_strJobPath.GetLength()-1;
	if ( m_strJobPath[nLen] != '\\' )
		return m_strJobPath + _T("\\");

	return m_strJobPath;
}

CString CConfig::GetLogDirectoryPath()
{
	int nLen = m_strLogPath.GetLength()-1;
	if ( m_strLogPath[nLen] != '\\' )
		return m_strLogPath + _T("\\");

	return m_strLogPath;
}

CString CConfig::GetInfoDirectoryPath()
{
	int nLen = m_strInfoPath.GetLength()-1;
	if ( m_strInfoPath[nLen] != '\\' )
		return m_strInfoPath + _T("\\");

	return m_strInfoPath;
}

BOOL CConfig::MakeSurePathExists(CString strPath)
{
	if (strPath.IsEmpty())		
		return FALSE;

	if (strPath.Right(1) == _T("\\"))
		strPath = strPath.Left(strPath.GetLength()-1); 
	
	if (GetFileAttributes(strPath) != 0xFFFFFFFF)
		return TRUE;
	
	int nPos = strPath.ReverseFind('\\');
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

/*
BOOL CConfig::LoadXML()
{
	CXmlDom dom;
	CString str;
//	int i, nCount;

	try
	{
		if (dom.InitizeXmlDom() == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}

		if (dom.LoadXmlFile(ECS_CFG_FILE) == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}

		if (dom.MoveXPath(_T("//Host")))
		{
			dom.GetChildElmtValue(_T("ImsIP"),				m_portHost.m_strIP);
			dom.GetChildElmtValue(_T("ImsPort"), str);		m_portHost.m_nPort = _ttoi(str);
			dom.GetChildElmtValue(_T("ImsConnect"), str);	m_portHost.m_bConnect = _ttoi(str);
			dom.GetChildElmtValue(_T("ImsRetry"), str);		m_portHost.m_bRetry = _ttoi(str);
			dom.GetChildElmtValue(_T("EcsPort"), str);		m_nEcsPort = _ttoi(str);
			dom.GetChildElmtValue(_T("MonPort"), str);		m_nMonPort = _ttoi(str);
		}

		if (dom.MoveXPath(_T("//Option")))
		{
			dom.GetChildElmtValue(_T("Job"), str);		m_bJob = _ttoi(str);
			dom.GetChildElmtValue(_T("Event"), str);	m_bEvent = _ttoi(str);
			dom.GetChildElmtValue(_T("Error"), str);	m_bError = _ttoi(str);
			dom.GetChildElmtValue(_T("Debug"), str);	m_bDebug = _ttoi(str);
			dom.GetChildElmtValue(_T("Beep"), str);		m_bBeep = _ttoi(str);
		}

		if (dom.SelectElements(_T("//Connect")))
		{
			nCount = dom.GetSelectCount();
			DEBUGER_ASSERT_VALID(nCount == m_pDoc->m_pEquipments.GetSize());

			for (i=0; i<nCount; ++i)
			{
				CPortEthernet& port = m_pDoc->m_pEquipments[i]->m_port;

				dom.MoveSelect(i);
				dom.GetAttrValue(_T("device"),			port.m_strDevice);
				dom.GetAttrValue(_T("ip"),				port.m_strIP);
				dom.GetAttrValue(_T("port"), str);		port.m_nPort = _ttoi(str);
				dom.GetAttrValue(_T("connect"), str);	port.m_bConnect = _ttoi(str);
				dom.GetAttrValue(_T("retry"), str);		port.m_bRetry = _ttoi(str);
				dom.GetAttrValue(_T("log"), str);		port.m_bWriteLog = _ttoi(str);
				dom.GetAttrValue(_T("timeout"), str);	port.m_dwRecvTimeout = _ttoi(str);
			}
		}

		if (dom.MoveXPath(_T("//Paths")))
		{
			dom.GetChildElmtValue(_T("Path[@name='ecs']"), m_strEcsPath);
			dom.GetChildElmtValue(_T("Path[@name='job']"), m_strJobPath);
			dom.GetChildElmtValue(_T("Path[@name='log']"), m_strLogPath);
			dom.GetChildElmtValue(_T("Path[@name='stc']"), m_strStcPath);
			dom.GetChildElmtValue(_T("Path[@name='info']"), m_strInfoPath);
		}

		if (dom.MoveXPath(_T("//Colors")))
		{
			dom.GetChildElmtValue(_T("Color[@type='auto' and @mode='sto']"), str);		m_clrAutoSto  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='auto' and @mode='ret']"), str);		m_clrAutoRet  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='auto' and @mode='move']"), str);		m_clrAutoMove = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='auto' and @mode='rtr']"), str);		m_clrAutoR2R  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='auto' and @mode='wtw']"), str);		m_clrAutoW2W  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='semi' and @mode='sto']"), str);		m_clrSemiSto  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='semi' and @mode='ret']"), str);		m_clrSemiRet  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='semi' and @mode='move']"), str);		m_clrSemiMove = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='semi' and @mode='rtr']"), str);		m_clrSemiR2R  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='semi' and @mode='wtw']"), str);		m_clrSemiW2W  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='equip' and @mode='manual']"), str);	m_clrManual	  = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='equip' and @mode='alarm']"), str);	m_clrAlarm    = CConvert::ToColor(str);
 			dom.GetChildElmtValue(_T("Color[@type='equip' and @mode='error']"), str);	m_clrError    = CConvert::ToColor(str);
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}

	return TRUE;
}

BOOL CConfig::SaveXML()
{
	CXmlDom dom;
	CString str;
	int i, nCount;

	try
	{
		if (dom.InitizeXmlDom() == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}

		dom.MoveDocument();
		dom.AddChildProcessingInstruction(_T("xml"), _T("version='1.0' encoding='euc-kr'"));
		dom.AddChildComment(_T("ECS Configuration (SCP2)"));
		dom.AddChildElement(_T("Config"));

		dom.MoveRoot();
		dom.AddChildElement(_T("Host"));
		dom.AddChildElement(_T("Option"));
//		dom.AddChildElement(_T("Connects"));
		dom.AddChildElement(_T("Paths"));
		dom.AddChildElement(_T("Colors"));

		if (dom.MoveXPath(_T("/Config/Host")))
		{
			dom.AddChildElement(_T("ImsIP"), m_portHost.m_strIP);
			dom.AddChildElement(_T("ImsPort"), CConvert::ToString(m_portHost.m_nPort));
			dom.AddChildElement(_T("ImsConnect"), CConvert::ToString(m_portHost.m_bConnect));
			dom.AddChildElement(_T("ImsRetry"),CConvert::ToString(m_portHost.m_bRetry));
			dom.AddChildElement(_T("EcsPort"), CConvert::ToString(m_nEcsPort));
			dom.AddChildElement(_T("MonPort"), CConvert::ToString(m_nMonPort));
		}

		if (dom.MoveXPath(_T("/Config/Option")))
		{
			dom.AddChildElement(_T("Job"), CConvert::ToString(m_bJob));
			dom.AddChildElement(_T("Event"), CConvert::ToString(m_bEvent));
			dom.AddChildElement(_T("Error"), CConvert::ToString(m_bError));
			dom.AddChildElement(_T("Debug"), CConvert::ToString(m_bDebug));
			dom.AddChildElement(_T("Beep"), CConvert::ToString(m_bBeep));
		}

		if (dom.MoveXPath(_T("/Config/Connects")))
		{
			for (i=0; i<m_pDoc->m_pEquipments.GetSize(); ++i)
			{
				CPortEthernet& port = m_pDoc->m_pEquipments[i]->m_port;
				dom.AddChildElement(_T("Connect"));

				dom.MoveChildLast();
				dom.AddChildAttribute(_T("device"), port.m_strDevice);
				dom.AddChildAttribute(_T("ip"),	port.m_strIP);
				dom.AddChildAttribute(_T("port"), CConvert::ToString(port.m_nPort));
				dom.AddChildAttribute(_T("connect"), CConvert::ToString(port.m_bConnect));
				dom.AddChildAttribute(_T("retry"), CConvert::ToString(port.m_bRetry));
				dom.AddChildAttribute(_T("log"), CConvert::ToString(port.m_bWriteLog));
				dom.AddChildAttribute(_T("timeout"), CConvert::ToString((int)port.m_dwRecvTimeout));
				dom.MoveParent();
			}
		}

		if (dom.MoveXPath(_T("/Config/Paths")))
		{
			LPCTSTR lpszValues[][2] = {
				{ m_strEcsPath,  _T("ecs")},
				{ m_strJobPath,  _T("job")},
				{ m_strLogPath,  _T("log")},
				{ m_strStcPath,  _T("stc")},
				{m_strInfoPath, _T("info")}
			};
			nCount = sizeof(lpszValues) / sizeof(lpszValues[0]);
			for (i=0; i<nCount; ++i)
			{
				dom.AddChildElement(_T("Path"), lpszValues[i][0]);

				dom.MoveChildLast();
				dom.AddChildAttribute(_T("name"), lpszValues[i][1]);
				dom.MoveParent();
			}
		}

		if (dom.MoveXPath(_T("/Config/Colors")))
		{
			LPCTSTR lpszType[][2] = {
				{ _T("auto"),    _T("sto")},
				{ _T("auto"),    _T("ret")},
				{ _T("auto"),   _T("move")},
				{ _T("auto"),    _T("rtr")},
				{ _T("auto"),    _T("wtw")},
				{ _T("semi"),    _T("sto")},
				{ _T("semi"),    _T("ret")},
				{ _T("semi"),   _T("move")},
				{ _T("semi"),    _T("rtr")},
				{ _T("semi"),    _T("wtw")},
				{_T("equip"), _T("manual")},
				{_T("equip"),  _T("alarm")},
				{_T("equip"),  _T("error")}
			};
			COLORREF clrColor[] = {
				m_clrAutoSto, m_clrAutoRet, m_clrAutoMove, m_clrAutoR2R, m_clrAutoW2W,
				m_clrSemiSto, m_clrSemiRet, m_clrSemiMove, m_clrSemiR2R, m_clrSemiW2W,
				m_clrManual, m_clrAlarm, m_clrError
			};
			nCount = sizeof(lpszType) / sizeof(lpszType[0]);
			for (i=0; i<nCount; ++i)
			{
				dom.AddChildElement(_T("Color"), CConvert::ToString(clrColor[i]));
				
				dom.MoveChildLast();
				dom.AddChildAttribute(_T("type"), lpszType[i][0]);
				dom.AddChildAttribute(_T("mode"), lpszType[i][1]);
				dom.MoveParent();
			}
		}

		if (dom.SaveXmlFile(ECS_CFG_FILE) == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}

	return TRUE;
}

void CConfig::InitializePort()
{
	LPCTSTR lpszValues[][7] = {
		{   _T("LGV#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("C/V#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("C/V#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("C/V#3"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("C/V#4"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("C/V#5"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("S/C#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("S/C#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("S/C#3"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("RTV#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("RTV#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("RTV#3"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{_T("LIFTER#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{_T("LIFTER#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{  _T("RFID#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{  _T("RFID#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{  _T("RFID#3"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{  _T("RFID#4"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("BCR#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("BCR#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("BCR#3"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("BCR#4"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{   _T("BCR#5"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{_T("전광판#1"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")},
		{_T("전광판#2"), _T("127.0.0.1"), _T("8192"), _T("1"), _T("1"), _T("0"), _T("3000")}
	};
	int nCount = sizeof(lpszValues) / sizeof(lpszValues[0]);
	m_arrayPort.SetSize(nCount);

	for (int i=0; i<nCount; ++i)
	{
		CPortEthernet& port  = m_arrayPort[i];
		port.m_strDevice	 = lpszValues[i][0];
		port.m_strIP		 = lpszValues[i][1];
		port.m_nPort		 = _ttoi(lpszValues[i][2]);
		port.m_bConnect		 = _ttoi(lpszValues[i][3]);
		port.m_bRetry		 = _ttoi(lpszValues[i][4]);
		port.m_bWriteLog	 = _ttoi(lpszValues[i][5]);
		port.m_dwRecvTimeout = _ttoi(lpszValues[i][6]);
	}
}
*/