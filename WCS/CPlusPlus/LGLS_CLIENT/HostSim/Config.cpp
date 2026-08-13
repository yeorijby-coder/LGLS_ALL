// Config.cpp : implementation of the CConfig class
//

#include "stdafx.h"
#include "Ecs.h"
#include "Config.h"

#include "EcsDoc.h"

CString g_strEcsPath;

///////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CConfig, CObject)

CConfig::CConfig()
{
	m_pDoc = NULL;
	InitializeConfig();
	m_strPlaybackPath = "";
}

CConfig::CConfig(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	ASSERT(m_pDoc != NULL);

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
	LoadConfig5();
	LoadConfig6();

	m_dwResendLimit = ::GetPrivateProfileInt(_T("COMMON"), _T("SocketResendLimit"), 5, ECS_INI_FILE);
	m_dwRecvTimeout = ::GetPrivateProfileInt(_T("COMMON"), _T("SocketRecvTimeout"), 3000, ECS_INI_FILE);
	m_dwExitTimeout = ::GetPrivateProfileInt(_T("COMMON"), _T("ThreadExitTimeout"), 3000, ECS_INI_FILE);
	m_bLogTrace		= ::GetPrivateProfileInt(_T("COMMON"), _T("LogTrace"), FALSE, ECS_INI_FILE);
}

void CConfig::InitializePath()
{
	char szPath[_MAX_PATH] = {0};
	DWORD dwTemp1 = ::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	//AfxMessageBox((LPWSTR)szPath);

	//DWORD dwTemp2 = GetLastError();

	//CString strTemp = CLib::GetSystemErrMsg(dwTemp2);

	g_strEcsPath.Format(_T("%s"), szPath);
	m_strEcsPath.Format(_T("%s"), szPath);

	//g_strEcsPath = (LPWSTR)szPath;
	//m_strEcsPath = (LPWSTR)szPath;
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
	m_clrScStoError	= RED;
	m_clrScRetError	= RED;
}

void CConfig::LoadConfig1()
{
	TCHAR szTemp[_MAX_PATH] = {0};
	m_portHost.m_strDevice = _T("HOST");
	::GetPrivateProfileString(m_portHost.m_strDevice, _T("IP"), _T("127.0.0.1"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_portHost.m_strIP.Format(_T("%s"), szTemp);

	m_portHost.m_nPort			= ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Port"), 7000, ECS_INI_FILE);
	m_portHost.m_bConnect		= ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Connect"), 1, ECS_INI_FILE);
	m_portHost.m_bRetry			= ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("Retry"), 1, ECS_INI_FILE);
	m_portHost.m_bWriteLog		= ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("WriteLog"), 0, ECS_INI_FILE);
	m_nEcsPort					= ::GetPrivateProfileInt(m_portHost.m_strDevice, _T("ServerPort"), 8000, ECS_INI_FILE);

	::GetPrivateProfileString(_T("LOGIN"), _T("LoginPW"), _T("SLI35"), szTemp, _MAX_PATH, ECS_INI_FILE);
	m_strPassword.Format(_T("%s"), szTemp);

	 
	m_bViewLampControler		= ::GetPrivateProfileInt(_T("VIEWLAMPCONTROLER"), _T("ViewLampControler"), 1, ECS_INI_FILE);
	m_bViewProductInfoInputer	= ::GetPrivateProfileInt(_T("VIEWPRODUCTINFOINPUTER"), _T("ViewProductInfoInputer"), 1, ECS_INI_FILE);
	m_nWeightValue				= ::GetPrivateProfileInt(_T("SETWEIGHTVALUE"), _T("WeightValue"), 1, ECS_INI_FILE);
	m_nFontSize					= ::GetPrivateProfileInt(_T("SETFONTSIZE"), _T("FontSize"), 1, ECS_INI_FILE);

	m_nRetBufferCount			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("BufferLimit"), 1, ECS_INI_FILE);
	m_bNoCheckScRetHs			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHS"), 1, ECS_INI_FILE);

	m_bNotCheckRetHsSc1			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc1"), 1, ECS_INI_FILE);
	m_bNotCheckRetHsSc2			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc2"), 1, ECS_INI_FILE);
	m_bNotCheckRetHsSc3			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc3"), 1, ECS_INI_FILE);
	m_bNotCheckRetHsSc4			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc4"), 1, ECS_INI_FILE);
	m_bNotCheckRetHsSc5			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc5"), 1, ECS_INI_FILE);
	m_bNotCheckRetHsSc6			= ::GetPrivateProfileInt(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc6"), 1, ECS_INI_FILE);

	m_bNotBackupNetwork			= ::GetPrivateProfileInt(_T("NOT_BACKUP_NETWORK"), _T("NotBackupNetwork"), 1, ECS_INI_FILE);

	m_nCompleteCnt				= ::GetPrivateProfileInt(_T("COMPLETE_CHECK"), _T("COMP_CHK_CNT"), 5, ECS_INI_FILE);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = m_pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		CPortEthernet& port = pEquipment->m_port;
		::GetPrivateProfileString(port.m_strDevice, _T("IP"), _T("127.0.0.1"), szTemp, _MAX_PATH, ECS_INI_FILE);
		port.m_strIP.Format(_T("%s"), szTemp);

		port.m_nPort		 = ::GetPrivateProfileInt(port.m_strDevice, _T("Port"), 8192, ECS_INI_FILE);
		port.m_bConnect		 = ::GetPrivateProfileInt(port.m_strDevice, _T("Connect"), 1, ECS_INI_FILE);
		port.m_bRetry		 = ::GetPrivateProfileInt(port.m_strDevice, _T("Retry"), 1, ECS_INI_FILE);
		port.m_bWriteLog	 = ::GetPrivateProfileInt(port.m_strDevice, _T("WriteLog"), 0, ECS_INI_FILE);
		port.m_dwRecvTimeout = ::GetPrivateProfileInt(port.m_strDevice, _T("RecvTimeout"), 3000, ECS_INI_FILE);

		if (pEquipment->m_pSocket)
			pEquipment->m_pSocket->m_bWriteLog = port.m_bWriteLog;

		if (pEquipment->m_pThreadSocket)
			pEquipment->m_pThreadSocket->m_bWriteLog = port.m_bWriteLog;
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
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Port"), CConvert::ToString(m_portHost.m_nPort), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Connect"), CConvert::ToString((int)m_portHost.m_bConnect), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("Retry"), CConvert::ToString((int)m_portHost.m_bRetry), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("WriteLog"), CConvert::ToString((int)m_portHost.m_bWriteLog), ECS_INI_FILE);
	::WritePrivateProfileString(m_portHost.m_strDevice, _T("ServerPort"), CConvert::ToString(m_nEcsPort), ECS_INI_FILE);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	::WritePrivateProfileString(_T("VIEWLAMPCONTROLER"), _T("ViewLampControler"), CConvert::ToString(m_bViewLampControler), ECS_INI_FILE);
	::WritePrivateProfileString(_T("VIEWPRODUCTINFOINPUTER"), _T("ViewProductInfoInputer"), CConvert::ToString(m_bViewProductInfoInputer), ECS_INI_FILE);
	::WritePrivateProfileString(_T("SETWEIGHTVALUE"), _T("WeightValue"), CConvert::ToString(m_nWeightValue), ECS_INI_FILE);
	::WritePrivateProfileString(_T("SETFONTSIZE"), _T("FontSize"), CConvert::ToString(m_nFontSize), ECS_INI_FILE);

	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("BufferLimit"), CConvert::ToString(m_nRetBufferCount), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHS"), CConvert::ToString(m_bNoCheckScRetHs), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc1"), CConvert::ToString(m_bNotCheckRetHsSc1), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc2"), CConvert::ToString(m_bNotCheckRetHsSc2), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc3"), CConvert::ToString(m_bNotCheckRetHsSc3), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc4"), CConvert::ToString(m_bNotCheckRetHsSc4), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc5"), CConvert::ToString(m_bNotCheckRetHsSc5), ECS_INI_FILE);
	::WritePrivateProfileString(_T("NOT_CHECK_RET_HS"), _T("NotCheckRetHsSc6"), CConvert::ToString(m_bNotCheckRetHsSc6), ECS_INI_FILE);

	::WritePrivateProfileString(_T("NOT_BACKUP_NETWORK"), _T("NotBackupNetwork"), CConvert::ToString(m_bNotBackupNetwork), ECS_INI_FILE);

	::WritePrivateProfileString(_T("COMPLETE_CHECK"), _T("COMP_CHK_CNT"), CConvert::ToString(m_nCompleteCnt), ECS_INI_FILE);

	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_pDoc->m_pEquipments.GetSize(); ++i)
	{
		pEquipment = m_pDoc->m_pEquipments[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		if (pEquipment->m_pSocket)
			pEquipment->m_pSocket->m_bWriteLog = pEquipment->m_port.m_bWriteLog;

		if (pEquipment->m_pThreadSocket)
			pEquipment->m_pThreadSocket->m_bWriteLog = pEquipment->m_port.m_bWriteLog;

		CPortEthernet& port = pEquipment->m_port;
		::WritePrivateProfileString(port.m_strDevice, _T("IP"), port.m_strIP, ECS_INI_FILE);
		::WritePrivateProfileString(port.m_strDevice, _T("Port"), CConvert::ToString(port.m_nPort), ECS_INI_FILE);
		::WritePrivateProfileString(port.m_strDevice, _T("Connect"), CConvert::ToString((int)port.m_bConnect), ECS_INI_FILE);
		::WritePrivateProfileString(port.m_strDevice, _T("Retry"), CConvert::ToString((int)port.m_bRetry), ECS_INI_FILE);
		::WritePrivateProfileString(port.m_strDevice, _T("WriteLog"), CConvert::ToString((int)port.m_bWriteLog), ECS_INI_FILE);
		::WritePrivateProfileString(port.m_strDevice, _T("RecvTimeout"), CConvert::ToString((int)port.m_dwRecvTimeout), ECS_INI_FILE);
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

	::GetPrivateProfileString(_T("COLOR"), _T("Disconnect"), CConvert::ToString(m_clrDisconnect),	szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrDisconnect = CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),    _T("Suspend"), CConvert::ToString(m_clrSuspend),		szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrSuspend	= CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),     _T("Manual"), CConvert::ToString(m_clrManual),		szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrManual		= CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"),      _T("Alarm"), CConvert::ToString(m_clrAlarm),		szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrAlarm		= CConvert::ToColor(CString(szTemp));

	::GetPrivateProfileString(_T("COLOR"),      _T("Error"), CConvert::ToString(m_clrError),		szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrError		= CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("ScStoError"), CConvert::ToString(m_clrScStoError),	szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrScStoError	= CConvert::ToColor(CString(szTemp));
	::GetPrivateProfileString(_T("COLOR"), _T("ScRetError"), CConvert::ToString(m_clrScRetError),	szTemp, _MAX_PATH, ECS_INI_FILE);	m_clrScRetError	= CConvert::ToColor(CString(szTemp));
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
	::WritePrivateProfileString(_T("COLOR"),      _T("Alarm"),      CConvert::ToString(m_clrAlarm), ECS_INI_FILE);

	::WritePrivateProfileString(_T("COLOR"),      _T("Error"),      CConvert::ToString(m_clrError), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("ScStoError"), CConvert::ToString(m_clrScStoError), ECS_INI_FILE);
	::WritePrivateProfileString(_T("COLOR"), _T("ScRetError"), CConvert::ToString(m_clrScRetError), ECS_INI_FILE);
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
	m_nBank		= ::GetPrivateProfileInt(_T("RACK"), _T("Bank"),	10, ECS_INI_FILE);
	m_nBay		= ::GetPrivateProfileInt(_T("RACK"), _T("Bay"),		10, ECS_INI_FILE);
	m_nLevel	= ::GetPrivateProfileInt(_T("RACK"), _T("Level"),	10, ECS_INI_FILE);
}

void CConfig::SaveConfig4()
{
	::WritePrivateProfileString(_T("RACK"), _T("Bank"),		CConvert::ToString(m_nBank),	ECS_INI_FILE);
	::WritePrivateProfileString(_T("RACK"), _T("Bay"),		CConvert::ToString(m_nBay),		ECS_INI_FILE);
	::WritePrivateProfileString(_T("RACK"), _T("Level"),	CConvert::ToString(m_nLevel),	ECS_INI_FILE);
}

void CConfig::LoadConfig5()
{
//	m_nBank		= ::GetPrivateProfileInt(_T("RACK"), _T("Bank"),	10, ECS_INI_FILE);
//	m_nBay		= ::GetPrivateProfileInt(_T("RACK"), _T("Bay"),		10, ECS_INI_FILE);
//	m_nLevel	= ::GetPrivateProfileInt(_T("RACK"), _T("Level"),	10, ECS_INI_FILE);

	TCHAR szTemp[9] = {0};
	int nCount	= ::GetPrivateProfileInt(_T("DISABLE_RACK"), _T("Disable_Count"),	1, ECS_INI_FILE);		//
	m_strDisableRacks.RemoveAll();

	CString strTemp;
	for (int i=0; i<nCount; ++i)
	{
		strTemp.Format(_T("Disable%02d"), i + 1);
		::GetPrivateProfileString(_T("DISABLE_RACK"), strTemp, _T(""), szTemp, _MAX_PATH, ECS_INI_FILE);
		
		m_strDisableRacks.Add(CString(szTemp));
	}
}

void CConfig::SaveConfig5()
{
	CString strTemp;
	for (int i=0; i<m_strDisableRacks.GetSize(); ++i)
	{
		strTemp.Format(_T("Disable%02d"), i + 1);
		::WritePrivateProfileString(_T("DISABLE_RACK"), strTemp, m_strDisableRacks[i],	ECS_INI_FILE);
	}
	int nDisableRackCnt = m_strDisableRacks.GetSize();
	::WritePrivateProfileString(_T("DISABLE_RACK"), _T("Disable_Count"), (LPCWSTR)CConvert::ToString(nDisableRackCnt),	ECS_INI_FILE);
}

//typedef struct SJobInvokeInfo
//{
//	CString m_strStoStation;
//	CString m_strRetStation;
//
//	CString m_strLocation;
//
//	int		m_nWorkingLuggNum;	// 현재 작업중인 작업번호 
//	BOOL	m_bCompleteStore;	// 입고작업완료 
//	int		m_nPrevLuggNum;		// PRE 작업번호 
//	CString	m_strAlterLocation;	// (이중입고 에러시)재지정 할 Location;
//	int		m_nDualStoScNo;
//	CString	m_strFromPos;		// 기타 작업할 포지션 
//	CString	m_strToPos;			// 기타 작업할 포지션 
//
//
//
//	//	int		m_nScCount;			// 해당 로직에서 작업중인 크레인 수
//	CStringArray m_strStoStations;		// 해당 로직에서 작업중인 입고스테이션들
//	CStringArray m_strViaStations;		// 해당 로직에서 작업중인 경유스테이션들
//	CStringArray m_strRetStations;		// 해당 로직에서 작업중인 출고스테이션들
//	CStringArray m_strScs;				// 해당 로직에서 작업중인 SC들
//
//} SJobInvokeInfo;


void CConfig::LoadConfig6()
{
	TCHAR szTemp[_MAX_PATH] = { 0 };
	m_nJobCnt = ::GetPrivateProfileInt(_T("LOGIC_CONTROL"), _T("JobCount"), 1, ECS_INI_FILE);
	m_nScCnt = ::GetPrivateProfileInt(_T("LOGIC_CONTROL"), _T("ScCount"), 1, ECS_INI_FILE);
	m_nLogicGroupCnt = ::GetPrivateProfileInt(_T("LOGIC_CONTROL"), _T("LogicGroup"), 1, ECS_INI_FILE);
	m_nStoSeperate = ::GetPrivateProfileInt(_T("LOGIC_CONTROL"), _T("StoSeperate"), 0, ECS_INI_FILE);
	m_nRetSeperate = ::GetPrivateProfileInt(_T("LOGIC_CONTROL"), _T("RetSeperate"), 0, ECS_INI_FILE);

	int nIndex = 0;
	CString strToken;
	CString strSection, strTemp;
	for (int i = 0; i < m_nLogicGroupCnt; ++i)
	{
		SLogicGorupInfo* pLogicGorupInfo = new SLogicGorupInfo();
		pLogicGorupInfo->m_bStart = FALSE;

		strSection.Format(_T("LOGIC_GROUP%02d"), i + 1);
		//===========================================================================================================
		// 각 스테이션 및 SC 리스트들을 CStringArray로 변환하여 저장
		// AfxExtractSubString를 사용해 콤마(,) 기준으로 분리
		//===========================================================================================================
		::GetPrivateProfileString(strSection, _T("StoStn"), _T("No Info"), szTemp, _MAX_PATH, ECS_INI_FILE);
		strTemp.Format(_T("%s"), szTemp);		
		nIndex = 0;

		SJobInvokeInfo* pJobInvokeInfo = NULL;
//		CArray<SJobInvokeInfo*, SJobInvokeInfo*> pJobInvokeInfos;		// 임시데이터임!
		while (AfxExtractSubString(strToken, strTemp, nIndex, _T(',')))
		{
			pJobInvokeInfo = new SJobInvokeInfo();
			pLogicGorupInfo->m_strStoStations.Add(strToken);
			//nIndex++;
			pJobInvokeInfo->m_nWorkingLuggNum = 0;		// 초기 작업중인 러그 수는 0으로 설정
			pJobInvokeInfo->m_bCompleteStore = FALSE;		// 초기 작업 완료 상태는 FALSE로 설정	
			pJobInvokeInfo->m_bCompleteMove = FALSE;		// 초기 이동 완료 상태는 FALSE로 설정	
			CString strTime = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
			pJobInvokeInfo->m_strTime = strTime;
//			pJobInvokeInfos.Add(pJobInvokeInfo);
			pLogicGorupInfo->m_pJobInvokeInfos.Add(pJobInvokeInfo);
			pLogicGorupInfo->m_nStoStnCnt = nIndex++ + 1;
		}

		BOOL bExist = TRUE;
		::GetPrivateProfileString(strSection, _T("ViaStn"), _T("No Info"), szTemp, _MAX_PATH, ECS_INI_FILE);
		strTemp.Format(_T("%s"), szTemp);
		nIndex = 0;
		int nJobInfoCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
		while (AfxExtractSubString(strToken, strTemp, nIndex, _T(',')))
		{
			if (nJobInfoCount == 0)		// pJobInvokeInfo가 NULL 인 상태임!!
			{
				pJobInvokeInfo = new SJobInvokeInfo();
				bExist = FALSE;
			}
			else
			{
				pJobInvokeInfo = pLogicGorupInfo->m_pJobInvokeInfos[nIndex];
			}

			pLogicGorupInfo->m_strViaStations.Add(strToken);
			nIndex++;

			if (bExist == FALSE)
			{
				pLogicGorupInfo->m_pJobInvokeInfos.Add(pJobInvokeInfo);
				pLogicGorupInfo->m_nStoStnCnt = nIndex + 1;
			}
		}

		::GetPrivateProfileString(strSection, _T("RetStn"), _T("No Info"), szTemp, _MAX_PATH, ECS_INI_FILE);
		strTemp.Format(_T("%s"), szTemp);		
		nIndex = 0;
		bExist = TRUE;
		nJobInfoCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
		while (AfxExtractSubString(strToken, strTemp, nIndex, _T(',')))
		{
			//int nJobInfoCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
			if (nJobInfoCount == 0)		// pJobInvokeInfo가 NULL 인 상태임!!
			{
				pJobInvokeInfo = new SJobInvokeInfo();
				bExist = FALSE;
			}
			else
			{
				int nInvokeJobCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
				if (nInvokeJobCount <= nIndex)
				{
					nIndex++;
					continue;	// pJobInvokeInfo = new SJobInvokeInfo();
				}

				pJobInvokeInfo = pLogicGorupInfo->m_pJobInvokeInfos[nIndex];
			}

			pLogicGorupInfo->m_strRetStations.Add(strToken);
			nIndex++;

			if (bExist == FALSE)
			{
				pLogicGorupInfo->m_pJobInvokeInfos.Add(pJobInvokeInfo);
				pLogicGorupInfo->m_nStoStnCnt = nIndex + 1;
			}
		}

		::GetPrivateProfileString(strSection, _T("ScNum"), _T("No Info"), szTemp, _MAX_PATH, ECS_INI_FILE);
		strTemp.Format(_T("%s"), szTemp);		
		nIndex = 0;
		bExist = TRUE;
		nJobInfoCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
		while (AfxExtractSubString(strToken, strTemp, nIndex, _T(',')))
		{
			//int nJobInfoCount = pLogicGorupInfo->m_pJobInvokeInfos.GetSize();
			//if (nJobInfoCount == 0)		// pJobInvokeInfo가 NULL 인 상태임!!
			//{
			//	pJobInvokeInfo = new SJobInvokeInfo();
			//	bExist = FALSE;
			//}
			//else
			//{
			//	pJobInvokeInfo = pLogicGorupInfo->m_pJobInvokeInfos[nIndex];
			//}
			pLogicGorupInfo->m_strScs.Add(strToken);
			nIndex++;

			//if (bExist == FALSE)
			//{
			//	pLogicGorupInfo->m_pJobInvokeInfos.Add(pJobInvokeInfo);
			//	pLogicGorupInfo->m_nStoStnCnt = nIndex + 1;
			//}
		}
		//-----------------------------------------------------------------------------------------------------------

		m_pDoc->m_pLogicGorupInfos.Add(pLogicGorupInfo);
	}
}
/*
void CConfig::SaveConfig6()
{
	TCHAR szTemp[_MAX_PATH] = { 0 };

	::WritePrivateProfileString(_T("LOGIC_CONTROL"), _T("JobCount"), CConvert::ToString(m_nJobCnt), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOGIC_CONTROL"), _T("ScCount"), CConvert::ToString(m_nScCnt), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOGIC_CONTROL"), _T("LogicGroup"), CConvert::ToString(m_nLogicGroupCnt), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOGIC_CONTROL"), _T("StoSeperate"), CConvert::ToString(m_nStoSeperate), ECS_INI_FILE);
	::WritePrivateProfileString(_T("LOGIC_CONTROL"), _T("RetSeperate"), CConvert::ToString(m_nRetSeperate), ECS_INI_FILE);

	int nIndex = 0;
	CString strToken;
	CString strSection, strTemp;
	SJobInvokeInfo sJobInvokeInfo;
	for (int i = 0; i < m_nLogicGroupCnt; ++i)
	{
		strSection.Format(_T("LogicGroup%02d"), i + 1);

		// 이부분을 [CStringArray를 콤마로 구분된 CString 으로 변환]하는 부분으로 적용해야 함!
		//::GetPrivateProfileString(strSection, _T("StoStn"), strTemp, szTemp, _MAX_PATH, ECS_INI_FILE);		strTemp.Format(_T("%s"), szTemp);		nIndex = 0;
		//while (AfxExtractSubString(strToken, strTemp, nIndex, _T(',')))
		//{
		//	sJobInvokeInfo.m_strStoStations.Add(strToken);
		//	nIndex++;
		//}

	}
}
//*/

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

void CConfig::Load(CReportCtrl* pReportCtrl)
{
//	CWaitCursor SomeWait;
	CMonitorServer* pMoniterSv = NULL;

	pReportCtrl->DeleteAllItems();

	CString strTemp;
	for (int i=0;i<m_nBank;i++)
	{
		for (int j=0;j<m_nBay;j++)
		{
			for (int k=0;k<m_nLevel;k++)
			{
				CStringList listTemp;
				strTemp.Format(_T("%02d-%03d-%02d"), i+1, j+1, k+1);
				listTemp.AddTail(strTemp);
//				listTemp.AddTail(pMoniterSv->m_tAccept.Format(_T("%Y-%m-%d %H:%M:%S")));
//				listTemp.AddTail(pMoniterSv->m_strPeerIP);
//				listTemp.AddTail(CConvert::ToString(pMoniterSv->m_nPeerPort));
				BOOL bTemp = FALSE;
				for (int i = 0 ; i < m_strDisableRacks.GetSize() ; ++i)
				{
					if (bTemp == TRUE)
						break;

					bTemp |= (BOOL)(m_strDisableRacks[i] == strTemp);
				}
				listTemp.AddTail(bTemp ? _T("O") : _T("X"));
				pReportCtrl->AddItem(0, 0, &listTemp);
				listTemp.RemoveAll();
				
			}
		}	
	}

	pReportCtrl->ResizeColumn();
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