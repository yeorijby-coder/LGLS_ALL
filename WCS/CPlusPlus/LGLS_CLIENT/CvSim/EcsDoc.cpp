// EcsDoc.cpp : implementation of the CEcsDoc class
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "EcsView.h"
#include "MainFrm.h"

#include "ConfigPptSheet.h"
//#include "UserManagerDlg.h"
//#include "IfTransLogDlg.h"

#define DEF_LOGIN_SKIP

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc

IMPLEMENT_DYNCREATE(CEcsDoc, CDocument)

BEGIN_MESSAGE_MAP(CEcsDoc, CDocument)
	//{{AFX_MSG_MAP(CEcsDoc)
	ON_COMMAND(ID_SYSTEM_CONFIG1, OnSystemConfig1)
	ON_COMMAND(ID_SYSTEM_CONFIG2, OnSystemConfig2)
	ON_COMMAND(ID_SYSTEM_CONFIG3, OnSystemConfig3)
	ON_COMMAND(ID_SYS_USER_MANAGER, OnSysUserManager)
	ON_COMMAND(ID_JOB_MANAGER, OnJobManager)
	ON_COMMAND(ID_RELOAD_KEYWORD, OnReLoadKeyWord)
	ON_COMMAND(ID_RELOAD_DEVICEMAP, OnReLoadDeviceMap)
	ON_COMMAND(ID_RELOAD_LOGIC, OnReLoadLogic)
	ON_COMMAND(ID_JOB_OFFLINE, OnJobOffline)
	ON_COMMAND(ID_LOG_ALL, OnLogAll)
	ON_COMMAND(ID_WARNING, OnWarning)
	ON_COMMAND(ID_LOGIC_VALIDATION, OnLogicValid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc construction/destruction

CEcsDoc::CEcsDoc()
{
	m_pConfig	= NULL;
	m_pLog		= NULL;
	m_pJob		= NULL;

	m_hWndView		= NULL;
	m_pJobDlg		= NULL;
	m_pWarningDlg	= NULL;
	m_pTrackDataDlg = NULL;
	m_pLogicValidationDlg = NULL;
	m_pDataSettingDlg = NULL;

	m_bTesting		= FALSE;

	m_bLogPlayback  = FALSE;

	m_mapTempVals.RemoveAll();
	m_mapHumVals.RemoveAll();
	m_dtHeartBit = COleDateTime::GetCurrentTime();
	m_bHeartBit = FALSE;
	m_mapCEquipStatus.RemoveAll();
	m_nLoopTrayCount = 0;	// Update : 2014.06.08 - Loop track 실시간 물류 TRAY 수량
	m_nCurCapacity = 0;		// Update : 2014.06.08 - 현재 가동률
	m_bBackupChk = FALSE;

	for (int i=0; i< CV_PLC_CNT; i++)
	{
	//	m_arrRegData[i].SetSize(10000);
	//	for (int j=0; j<10000; j++)
	//		m_arrRegData[i][j] = 0x00;
		m_arrRegData[i].SetArray(0, 10000);
		m_arrRegData[i].InitArray();
	}

	//m_arrRegData[0][0] = 0x00;
	//int aaa = m_arrRegData[0][0] ^ 0x01;
	
	m_bMoveFlag = FALSE;
	m_layoutSelected = EN_Assemble;

	m_bParent = FALSE;
	m_nWordCnt = 10;
}

CEcsDoc::~CEcsDoc()
{
	if (m_pWarningDlg != NULL) delete m_pWarningDlg;
	if (m_pTrackDataDlg != NULL) delete m_pTrackDataDlg;
	if (m_pLogicValidationDlg != NULL) delete m_pLogicValidationDlg;
	if (m_pDataSettingDlg != NULL) delete m_pDataSettingDlg;
	//	if (m_pLogDlg != NULL) delete m_pLogDlg;
	if (m_pJobDlg != NULL) delete m_pJobDlg;

	if (m_pJob) delete m_pJob;
	if (m_pLog) delete m_pLog;
	if (m_pConfig) delete m_pConfig;
//	if (m_pLogPlayback) delete m_pLogPlayback;

	for (POSITION pos=m_MsgList.GetHeadPosition(); pos!=NULL; )
		delete (CStatusBarMsg*)m_MsgList.GetNext(pos);
	m_MsgList.RemoveAll();

	int i=0;
	for (i=0; i<m_pStationInfos.GetSize(); ++i)
		delete m_pStationInfos[i];
	m_pStationInfos.RemoveAll();

	for (i=0; i<m_pTrackInfos.GetSize(); ++i)
		delete m_pTrackInfos[i];
	m_pTrackInfos.RemoveAll();

	for (i=0; i<m_pEquipments.GetSize(); ++i)
		delete m_pEquipments[i];
	m_pEquipments.RemoveAll();

	for (i=0; i<m_pEcsLayOuts.GetSize(); ++i)
		delete m_pEcsLayOuts[i];
	m_pEcsLayOuts.RemoveAll();
	
	for (i = 0; i < m_pDeviceMaps.GetSize(); ++i)
	{
		if (m_pDeviceMaps[i] != NULL)
			delete m_pDeviceMaps[i];
	}
	m_pDeviceMaps.RemoveAll();
	for (int j = 0; j < CV_PLC_CNT; j++)
	{
		int nCount = m_pTrackProperties[j].GetSize();
		for (i = 0; i < nCount; ++i)
		{
			if (m_pTrackProperties[j][i] != NULL)
				delete m_pTrackProperties[j][i];
		}
		m_pTrackProperties[j].RemoveAll();
	}
	
	// 온습도계
	m_mapTempVals.RemoveAll();
	m_mapHumVals.RemoveAll();
}

BOOL CEcsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
 
	return Initialize();
}

BOOL CEcsDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if (AfxMessageBox(_T("종료하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return FALSE;

	for (int i = 0; i < m_pEquipments.GetSize(); ++i)
	{
		if (m_pEquipments[i] == NULL)
			continue;

		m_pEquipments[i]->Disconnect();
		
		//if (m_pEquipments[i]->m_enKind == CEquipment::enBCR)
		//{
		//	CBcr* pBcr = (CBcr*) m_pEquipments[i];
		//	if (pBcr == NULL)
		//		continue;

		//	int nSize = pBcr->m_pInfo.GetUpperBound();

		//	for (int i = 0; i <= nSize; i++)
		//	{
		//		pBcr->m_pInfo[i]->Backup2(i + 1);
		//	}
		//}
		//else
		//{
			m_pEquipments[i]->GetInfo()->Backup();
		//}
	}

	m_pJob->Backup();

	WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("ECS 종료... (Equipment Control System)"), _T("CEcsDoc::CanCloseFrame"));
	
	return CDocument::CanCloseFrame(pFrame);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc serialization

void CEcsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc diagnostics

#ifdef _DEBUG
void CEcsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEcsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CEcsDoc::Initialize()
{
	GetViewHandle();
	
	int i = 0;
//	char szPath[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	//다른 방법이 있을 테지만 머리가 안돌아가 @.@
	m_nPlcCnt = ::GetPrivateProfileInt(_T("COMMON"), _T("PlcCnt"), 1, ECS_INI_FILE);

	g_nLenStationID = ::GetPrivateProfileInt(_T("LENGTH"), _T("StationID"), 3, ECS_INI_FILE);
	g_nLenControlID = ::GetPrivateProfileInt(_T("LENGTH"), _T("ControlID"), 8, ECS_INI_FILE);
	g_nLenLocation = ::GetPrivateProfileInt(_T("LENGTH"), _T("Location"), 7, ECS_INI_FILE);
	g_nLenBarcode = ::GetPrivateProfileInt(_T("LENGTH"), _T("Barcode"), 31, ECS_INI_FILE);


	LoadKeyWord(m_nPlcCnt);
	LoadDeviceMap(m_nPlcCnt);
	LoadLogic(m_nPlcCnt);

	//// DeviceMap.Xml 읽어오기
	//CDeviceMap map(this);
	//if (!map.ParseXml())
	//{
	//	AfxMessageBox(map.GetErrorMessage());
	//	return FALSE;
	//}

	////int nWordPerOneTrack = m_arrOneTrack.GetSize();
	//if (m_n1TrackWord == 0)
	//{
	//	AfxMessageBox(_T("한 트랙당 Word 수에 대한 초기화를 하지 못했습니다.!"));
	//	return FALSE;
	//}

	int nTabCount = ::GetPrivateProfileInt(_T("COMMON"), _T("TabCount"), 1, ECS_INI_FILE);
	g_nPlcCount = 0;
	g_nTabCount = nTabCount;
	for (i = 0 ; i < nTabCount ; i++)
	{
		CEcsLayout* pEcsLayout = new CEcsLayout();

		CString strLayOutFilePath;
		strLayOutFilePath.Format(_T(".\\EcsLayout%d.xml"), i + 1);

		if (!pEcsLayout->LoadXml(strLayOutFilePath))
		{
			AfxMessageBox(pEcsLayout->m_strLog);
			continue;
		}

		m_pEcsLayOuts.Add(pEcsLayout);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	CEcsDefine define(this);
	if (!define.ParseXml())
	{
		AfxMessageBox(define.GetErrorMessage());
		return FALSE;
	}

	CTrackInfo* pTrack = NULL;
	for (i=0; i<m_pTrackInfos.GetSize(); ++i)
	{
		pTrack = m_pTrackInfos[i];
		DEBUGER_ASSERT_VALID(pTrack != NULL);

		if (pTrack->m_nNumber == 112)
			int aaa = 0;

		if (pTrack->LoadXML() != TRUE)
			pTrack->m_nKind = -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pConfig = new CConfig(this);
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	m_pConfig->InitializePath();					// 시작시 자동 경로 초기화

	m_pLog = new CLog(this);
	DEBUGER_ASSERT_VALID(m_pLog != NULL);

	m_pJob = new CJob(this);
	DEBUGER_ASSERT_VALID(m_pJob != NULL);

//	m_pLogPlayback = new CLogPlayback(this); 
//	DEBUGER_ASSERT_VALID(m_pLogPlayback != NULL);

	int nMode = 0;
	if (m_pJob->Restore(nMode) == FALSE)
	{
		nMode = 1;
		m_pJob->Restore(nMode);
	}

	WriteLog(LOG_TYPE_EVENT, LOG_POS_SYSTEM, _T("ECS 시작... (Equipment Control System)"), _T("CEcsDoc::Initialize"));

	return TRUE;
}

/*
BOOL CEcsDoc::GetUseByTrack(int nTrackNo)
{
	CDeviceMap* pDeviceMap = GetDeviceMapByTrack(nTrackNo);

	if (pDeviceMap == NULL)
		return FALSE;

	return pDeviceMap->m_bUseDestTable;
}

CDestinationArray* CEcsDoc::GetDestinationsByDestNTrack(int nTrack, int nDest)
{
	CDeviceMap* pDeviceMap = GetDeviceMapByTrack(nTrack);

	if (pDeviceMap == NULL)
		return NULL;

	return &pDeviceMap->m_oDestTable;
}
//*/

CDeviceMap* CEcsDoc::GetDeviceMapByTrack(int nTrack)
{
	CTrackInfo* pTrack = GetTrackInfo(nTrack);

	if (pTrack == NULL)
		return NULL;

	int nSize = m_pDeviceMaps.GetSize();

	CDeviceMap* pDeviceMap = NULL;

	for (int i = 0; i < nSize; i++)
	{
		pDeviceMap = m_pDeviceMaps.GetAt(i);

		if (pDeviceMap == NULL)
			continue;

		if (pTrack->m_nCvPlcNum == pDeviceMap->m_nPlc)
			return pDeviceMap;
	}
	return NULL;
}

// bMethod => 1:Self, 2:ScDest
BOOL CEcsDoc::IsDestination(CString strArgName, int nValue, int nTrNo, int nMethod)
{
	#pragma region 목적지 테이블을 변경해야 하는지에 대한 부분 체크 

	// 접근방법찾기
	BOOL bTemp1 = TRUE;	//GetUseByTrack(nTrNo);			// 트랙번호로 사용유무 찾기
	BOOL bTemp2 = FALSE;	// ((nTrNo == 0) && ((m_nScDestFrom > 0 && m_nScDestTo > 0) || (m_strDestList != _T(""))));
	if (bTemp1 == TRUE)
		bTemp2 = ((nTrNo == 0) && ((m_nScDestFrom > 0 && m_nScDestTo > 0) || (m_strDestList != _T(""))));
	else
	{
		if (nValue < 900)
			bTemp1 = TRUE;		// 이 경우 목적지를 트랙번호로 사용하므로 무조건 TRUE 대입 
		else 
			bTemp2 = ((nTrNo == 0) && ((m_nScDestFrom > 0 && m_nScDestTo > 0) || (m_strDestList != _T(""))));
	}

	BOOL bTemp = bTemp1 || bTemp2;

	if (bTemp == TRUE /* && strArgName == m_oDestTable.m_strKeyWord*/)
	{
		CTrackInfo* pTrack = GetTrackInfo(nTrNo);
		int nTrDestCode = (pTrack != NULL) ? pTrack->m_nDestCode : 0;
		// 목적지 테이블을 바꿀 필요가 있음!
		BOOL bIsSelfDest = (nValue == nTrDestCode);		// (nValue == nTrNo);
		BOOL bIsScDest1 = (nValue > m_nScDestFrom && nValue < m_nScDestTo);
		BOOL bIsScDest2 = (m_strDestList.Find(CConvert::ToString(nValue)) != -1);
		BOOL bIsScDest = bIsScDest1 || bIsScDest2;

		switch (nMethod)
		{
		case 1:		return bIsSelfDest;
		case 2:		return bIsScDest;
		}
	}
	#pragma endregion
	return FALSE;
}

BOOL CEcsDoc::LookupKeywordInfo(CString strKeyWord, CString& strType, CString& strAddr, CString& strInOrder)
{
	return TRUE;
}

CString CEcsDoc::GetParentWord(CString strKeyWord)
{
	CString strParent, strTemp = strKeyWord + _T("00");
	m_pKeyWords.Lookup(strTemp, strParent);

	if (strParent == _T(""))
	{
		return "";
	}
	return strParent;
}

BOOL CEcsDoc::IsStationKind(CTrackInfo* pTrack, CString strKeyWord)
{
	if (pTrack == NULL)
		return FALSE;

	CStationInfo* pStoStation	= pTrack->GetStoStation();
	CStationInfo* pRetStation	= pTrack->GetRetStation();
	CStationInfo* pArvStation	= pTrack->GetArvStation();
	CTrackHS* pScStoHS			= pTrack->GetScStoHS();
	CTrackHS* pScRetHS			= pTrack->GetScRetHS();
	CTrackHS* pRtvDepartHS		= pTrack->GetRtvDepartHS();
	CTrackHS* pRtvArriveHS		= pTrack->GetRtvArriveHS();
	CTrackHS* pLgvDepartHS		= pTrack->GetLgvDepartHS();
	CTrackHS* pLgvArriveHS		= pTrack->GetLgvArriveHS();
	CTrackHS* pLifterDepartHS	= pTrack->GetLifterDepartHS();
	CTrackHS* pLifterArriveHS	= pTrack->GetLifterArriveHS();

		 if (strKeyWord == _T("StoStation"))	{ CStationInfo* pStation = pTrack->GetStoStation();			return ((pStation != NULL) && (pStation->m_enKind == CStationInfo::enStoStation)); }
	else if (strKeyWord == _T("RetStation"))	{ CStationInfo* pStation = pTrack->GetRetStation();			return ((pStation != NULL) && (pStation->m_enKind == CStationInfo::enRetStation)); }
	else if (strKeyWord == _T("ArvStation"))	{ CStationInfo* pStation = pTrack->GetArvStation();			return ((pStation != NULL) && (pStation->m_enKind == CStationInfo::enArvStation)); }
	else if (strKeyWord == _T("ScStoHS"))		{ CTrackHS* pScHS = pTrack->GetScStoHS();				return (pScHS != NULL); }
	else if (strKeyWord == _T("ScRetHS"))		{ CTrackHS* pScHS = pTrack->GetScRetHS();				return (pScHS != NULL); }
	else if (strKeyWord == _T("RtvDepartHS"))	{ return pTrack->GetRtvDepartHSChk(); }
	else if (strKeyWord == _T("RtvArriveHS"))	{ return pTrack->GetRtvArriveHsChk(); }
	else if (strKeyWord == _T("LgvDepartHS"))	{ CTrackHS* pLgvHS = pTrack->GetLgvDepartHS();		return (pLgvHS != NULL); }
	else if (strKeyWord == _T("LgvArriveHS"))	{ CTrackHS* pLgvHS = pTrack->GetLgvArriveHS();		return (pLgvHS != NULL); }
	else if (strKeyWord == _T("LifterDepartHS")){ CTrackHS* pLifterHS = pTrack->GetLifterDepartHS();	return (pLifterHS != NULL); }
	else if (strKeyWord == _T("LifterArriveHS")){ CTrackHS* pLifterHS = pTrack->GetLifterArriveHS();	return (pLifterHS != NULL);}
	else if (strKeyWord == _T("In_Out_Stn"))	{ return (IsStationKind(pTrack, _T("StoStation")) && IsStationKind(pTrack, _T("RetStation"))); }
	else if (strKeyWord == _T("ScHS"))			{ return (IsStationKind(pTrack, _T("ScStoHS")) && IsStationKind(pTrack, _T("ScRetHS"))); }
	else if (strKeyWord == _T("RtvHS"))			{ return (IsStationKind(pTrack, _T("RtvDepartHS")) && IsStationKind(pTrack, _T("RtvArriveHS"))); }
	else if (strKeyWord == _T("LgvHS"))			{ return (IsStationKind(pTrack, _T("LgvDepartHS")) && IsStationKind(pTrack, _T("LgvArriveHS"))); }
	else if (strKeyWord == _T("LifterHS"))		{ return (IsStationKind(pTrack, _T("LifterDepartHS")) && IsStationKind(pTrack, _T("LifterArriveHS"))); }
//	else if (strKeyWord == _T("LgvInRequest"))	{ return (pStation->m_enKind == CStationInfo::enRetStation);}
//	else if (strKeyWord == _T("LgvOutRequest")) { return (pStation->m_enKind == CStationInfo::enRetStation);}
//	else if (strKeyWord == _T("CvCheckPos"))	{ return (pStation->m_enKind == CStationInfo::enRetStation);}

	return FALSE;
}

// Number이미 -1되어서 넘겨줌
#define		INVERSE_BIT(xArg, yArg, zArg)			m_arrRegData[xArg].InverseBit(yArg, zArg)

int CEcsDoc::GetAddrByName(int nNumber, int nDevNum, CString strArgName, BOOL bParent)
{
	CString strType, strInOrder;
	CString strAddr, strName, strTrackNo;
	
	#pragma region 원하는 키워드 찾기
	int nTrackNo = int(nDevNum / m_nWordCnt) + ((nNumber + 1) * 100);
	//if (nNumber == 0)						// 하드코딩
	//	nTrackNo = nDevNum / 2;

	CTrackProperty* pTrackProperty = NULL;
	if ((pTrackProperty = GetTrackPropertyByKeyword(nNumber, nTrackNo, strArgName, bParent)) == NULL)
		return 0;

	strType = pTrackProperty->m_strType;
	strAddr = pTrackProperty->m_strAddr;
	strName = pTrackProperty->m_strName;
	#pragma endregion

	int nRealDevNum;
	int nInOrder = pTrackProperty->m_nInOrder;
	if (nInOrder < 1)
		return -1;		// InOrder 값 이상(0)

	if (strAddr.GetLength() < 1)
		return -2;		// Addr 값 이상

	int nAddr = _ttoi(strAddr);
	int nWordOrder = _ttoi(strAddr.Right(1));
	if (pTrackProperty->m_bSeparatelyETC == TRUE)
		nRealDevNum = nAddr;
	else
		nRealDevNum = nDevNum + nWordOrder;

	//WORD wTempTemp = 0x0000;
	WORD wTemp = m_arrRegData[nNumber].GetWord(nRealDevNum);
	if (strType == _T("W") || bParent == TRUE)
		return wTemp;
	else
	{
		if (strType == _T("B"))
		{
			switch (nInOrder)
			{
			case 1:		wTemp = m_arrRegData[nNumber].GetByte(nRealDevNum, enByteL);	break;
			case 2:		wTemp = m_arrRegData[nNumber].GetByte(nRealDevNum, enByteH);	break;
			default:	return -3;	// InOrder 값 이상(Byte 일때 3이상 )
			}
			return wTemp;
		}
		if (strType == _T("N"))
		{
			switch (nInOrder)
			{
			case 1:		wTemp = m_arrRegData[nNumber].GetNibble(nRealDevNum, enNibble1);	break;
			case 2:		wTemp = m_arrRegData[nNumber].GetNibble(nRealDevNum, enNibble2);	break;
			case 3:		wTemp = m_arrRegData[nNumber].GetNibble(nRealDevNum, enNibble3);	break;
			case 4:		wTemp = m_arrRegData[nNumber].GetNibble(nRealDevNum, enNibble4);	break;
			default:	return -4;	// InOrder 값 이상(Nibble 일때 5이상 )
			}
			return wTemp;
		}
		if (strType == _T("b"))
		{
			switch (nInOrder)
			{
			case  1:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit11);			break;
			case  2:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit12);			break;
			case  3:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit13);			break;
			case  4:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit14);			break;
			case  5:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit15);			break;
			case  6:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit16);			break;
			case  7:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit17);			break;
			case  8:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit18);			break;
			case  9:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit21);			break;
			case 10:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit22);			break;
			case 11:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit23);			break;
			case 12:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit24);			break;
			case 13:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit25);			break;
			case 14:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit26);			break;
			case 15:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit27);			break;
			case 16:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit28);			break;
			default:	return -5;	// InOrder 값 이상(bit 일때 17이상 )
			}
			return wTemp;
		}
	}
	return -6;	// Type 이상
}

int CEcsDoc::GetBitInOrderByWord(int nNumber, int nDevNum, CString strArgName)
{
	CString strType, strInOrder;
	CString strAddr, strName, strTrackNo;

#pragma region 원하는 키워드 찾기
	int nTrackNo = int(nDevNum / m_nWordCnt) + ((nNumber + 1) * 100);

	CTrackProperty* pTrackProperty = NULL;
	if ((pTrackProperty = GetTrackPropertyByKeyword(nNumber, nTrackNo, strArgName)) == NULL)
		return -3;

	strType = pTrackProperty->m_strType;
	strAddr = pTrackProperty->m_strAddr;
	strName = pTrackProperty->m_strName;
#pragma endregion

	int nRealDevNum;
	int nInOrder = pTrackProperty->m_nInOrder;
	if (nInOrder < 1)
		return -1;		// InOrder 값 이상(0)

	if (strAddr.GetLength() < 1)
		return -2;		// Addr 값 이상

	int nAddr = _ttoi(strAddr);
	int nWordOrder = _ttoi(strAddr.Right(1));
	if (pTrackProperty->m_bSeparatelyETC == TRUE)
		nRealDevNum = nAddr;
	else
		nRealDevNum = nDevNum + nWordOrder;

	if (strType != _T("b"))
		return -6;		// Type 이상 (Bit이 아닐 때)


	//WORD wTempTemp = 0x0000;
	WORD wTemp = m_arrRegData[nNumber].GetWord(nRealDevNum);

	switch (nInOrder)
	{
	case  1:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit11);			break;
	case  2:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit12);			break;
	case  3:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit13);			break;
	case  4:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit14);			break;
	case  5:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit15);			break;
	case  6:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit16);			break;
	case  7:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit17);			break;
	case  8:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit18);			break;
	case  9:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit21);			break;
	case 10:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit22);			break;
	case 11:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit23);			break;
	case 12:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit24);			break;
	case 13:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit25);			break;
	case 14:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit26);			break;
	case 15:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit27);			break;
	case 16:	wTemp = m_arrRegData[nNumber].GetBit(nRealDevNum, enBit28);			break;
	default:	return -5;	// InOrder 값 이상(bit 일때 17이상 )
	}
	return nInOrder;
}

// Nibble 일때는 마지막에 Word DATA 가 0x0000 ~ 0x000F 밖에 없음
// Bit 일때 nOption에 대해 설명하면 : 0 이면 사용하지 않음,			wData가 0x0000, 0x0001밖에 없음
//									  1 이면 무조건 ON,				wData가 기존 값이어야함!
//									  2 이면 해당 비트만 빼고 OFF,	wData가 기존 값이어야함!
//									  3 이면 반전					wData가 기존 값이어야함!
//									  4 이면 특정 비트만 강제 OFF,	wData가 기존 값이어야함!
int CEcsDoc::SetAddrByName(int nNumber, int nDevNum, CString strArgName, WORD wData, int nOption)
{
	CString strType, strInOrder;
	CString strAddr, strName, strTrackNo;

	#pragma region 원하는 키워드 찾기
	//if (LookupKeywordInfo(nNumber, nDevNum, strArgName, strType, strAddr, strInOrder, strTrackNo) == FALSE)
	//	return 0;
	int nTrackNo = int(nDevNum / m_nWordCnt) + ((nNumber + 1) * 100);
	//if (nNumber == 0)						// 하드코딩
	//	nTrackNo = nDevNum / 2;

	CTrackProperty* pTrackProperty = NULL;
	if ((pTrackProperty = GetTrackPropertyByKeyword(nNumber, nTrackNo, strArgName)) == NULL)
		return 0;

	strType	= pTrackProperty->m_strType;
	strAddr = pTrackProperty->m_strAddr;
	strName = pTrackProperty->m_strName;
	#pragma endregion

	int nRealDevNum;
	int nInOrder = pTrackProperty->m_nInOrder;
	if (nInOrder < 1)
		return -1;		// InOrder 값 이상(0)

	if (strAddr.GetLength() < 1)
		return -2;		// Addr 값 이상

	int nAddr = _ttoi(strAddr);
	int nWordOrder = _ttoi(strAddr.Right(1));
	//	void CWordArrayEx::SetWord(int nWordAddr, WORD wWord)
	if (pTrackProperty->m_bSeparatelyETC == TRUE)
		nRealDevNum = nAddr;
	else
		nRealDevNum = nDevNum + nWordOrder;


	if (strType == _T("W"))
	{
		#pragma region WORD 일때 하는 값 설정하는 부분 
		m_arrRegData[nNumber].SetWord(nRealDevNum, wData);

		return nRealDevNum;
		#pragma endregion
	}
	else
	{
		if (strType == _T("B"))
		{
			#pragma region BYTE 일때 하는 값 설정하는 부분 
			switch (nInOrder)
			{
			case 1:		m_arrRegData[nNumber].SetByte(nRealDevNum, enByteL, wData);		return nRealDevNum;
			case 2:		m_arrRegData[nNumber].SetByte(nRealDevNum, enByteH, wData);		return nRealDevNum;
			default:	return -3;	// InOrder 값 이상(Byte 일때 3이상 )							
			}																						
			#pragma endregion
		}
		if (strType == _T("N"))																		
		{																							
			#pragma region Nibble 일때 하는 값 설정하는 부분 
			switch (nInOrder)
			{																						
			case 1:		m_arrRegData[nNumber].SetNibble(nRealDevNum, enNibble1, wData);	return nRealDevNum;
			case 2:		m_arrRegData[nNumber].SetNibble(nRealDevNum, enNibble2, wData);	return nRealDevNum;
			case 3:		m_arrRegData[nNumber].SetNibble(nRealDevNum, enNibble3, wData);	return nRealDevNum;
			case 4:		m_arrRegData[nNumber].SetNibble(nRealDevNum, enNibble4, wData);	return nRealDevNum;
			default:	return -4;	// InOrder 값 이상(Nibble 일때 5이상 )							
			}												
			#pragma endregion
		}
		if (strType == _T("b"))																		
		{	
			#pragma region Bit 일때 하는 값 설정하는 부분 
			if (nOption != 1 && nOption != 2 && nOption != 3 && nOption != 4)
				return -7;		// Bit 일때 Option값 이상 

			//WORD wPrevData = wData;
			WORD wTemp;
			WORD wPrevData = m_arrRegData[nNumber].GetWord(nRealDevNum);

			switch (nInOrder)
			{
			case  1:	wTemp = (nOption == 1) ? wPrevData |= enBit11 : ((nOption == 2) ? wPrevData &= enBit11 : (nOption == 3) ? wPrevData ^= enBit11 : wPrevData &= ~enBit11);		break;
			case  2:	wTemp = (nOption == 1) ? wPrevData |= enBit12 : ((nOption == 2) ? wPrevData &= enBit12 : (nOption == 3) ? wPrevData ^= enBit12 : wPrevData &= ~enBit12);		break;
			case  3:	wTemp = (nOption == 1) ? wPrevData |= enBit13 : ((nOption == 2) ? wPrevData &= enBit13 : (nOption == 3) ? wPrevData ^= enBit13 : wPrevData &= ~enBit13);		break;
			case  4:	wTemp = (nOption == 1) ? wPrevData |= enBit14 : ((nOption == 2) ? wPrevData &= enBit14 : (nOption == 3) ? wPrevData ^= enBit14 : wPrevData &= ~enBit14);		break;
			case  5:	wTemp = (nOption == 1) ? wPrevData |= enBit15 : ((nOption == 2) ? wPrevData &= enBit15 : (nOption == 3) ? wPrevData ^= enBit15 : wPrevData &= ~enBit15);		break;
			case  6:	wTemp = (nOption == 1) ? wPrevData |= enBit16 : ((nOption == 2) ? wPrevData &= enBit16 : (nOption == 3) ? wPrevData ^= enBit16 : wPrevData &= ~enBit16);		break;
			case  7:	wTemp = (nOption == 1) ? wPrevData |= enBit17 : ((nOption == 2) ? wPrevData &= enBit17 : (nOption == 3) ? wPrevData ^= enBit17 : wPrevData &= ~enBit17);		break;
			case  8:	wTemp = (nOption == 1) ? wPrevData |= enBit18 : ((nOption == 2) ? wPrevData &= enBit18 : (nOption == 3) ? wPrevData ^= enBit18 : wPrevData &= ~enBit18);		break;
			case  9:	wTemp = (nOption == 1) ? wPrevData |= enBit21 : ((nOption == 2) ? wPrevData &= enBit21 : (nOption == 3) ? wPrevData ^= enBit21 : wPrevData &= ~enBit21);		break;
			case 10:	wTemp = (nOption == 1) ? wPrevData |= enBit22 : ((nOption == 2) ? wPrevData &= enBit22 : (nOption == 3) ? wPrevData ^= enBit22 : wPrevData &= ~enBit22);		break;
			case 11:	wTemp = (nOption == 1) ? wPrevData |= enBit23 : ((nOption == 2) ? wPrevData &= enBit23 : (nOption == 3) ? wPrevData ^= enBit23 : wPrevData &= ~enBit23);		break;
			case 12:	wTemp = (nOption == 1) ? wPrevData |= enBit24 : ((nOption == 2) ? wPrevData &= enBit24 : (nOption == 3) ? wPrevData ^= enBit24 : wPrevData &= ~enBit24);		break;
			case 13:	wTemp = (nOption == 1) ? wPrevData |= enBit25 : ((nOption == 2) ? wPrevData &= enBit25 : (nOption == 3) ? wPrevData ^= enBit25 : wPrevData &= ~enBit25);		break;
			case 14:	wTemp = (nOption == 1) ? wPrevData |= enBit26 : ((nOption == 2) ? wPrevData &= enBit26 : (nOption == 3) ? wPrevData ^= enBit26 : wPrevData &= ~enBit26);		break;
			case 15:	wTemp = (nOption == 1) ? wPrevData |= enBit27 : ((nOption == 2) ? wPrevData &= enBit27 : (nOption == 3) ? wPrevData ^= enBit27 : wPrevData &= ~enBit27);		break;
			case 16:	wTemp = (nOption == 1) ? wPrevData |= enBit28 : ((nOption == 2) ? wPrevData &= enBit28 : (nOption == 3) ? wPrevData ^= enBit28 : wPrevData &= ~enBit28);		break;
			default:	return -5;	// InOrder 값 이상(bit 일때 17이상 )
			}
			m_arrRegData[nNumber].SetWord(nRealDevNum, wTemp);
			#pragma endregion
		}
		return nRealDevNum;
	}
	return -6;	// Type 이상
}
// -1이 가져오지 못했다는 뜻임!
int CEcsDoc::GetSignalIndex(CString strKeyWord)
{
	int nSelIndex = -1;
	int nCount = m_strEtcKeyWords.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (m_strEtcKeyWords[i] == strKeyWord)
		{
			nSelIndex = i;
			break;
		}
	}
	return nSelIndex;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BOOL CEcsDoc::ParsingDeviceMap()
{
	if (!CXmlDom::InitializeXmlDom())
		return FALSE;

	if (!CXmlDom::LoadXmlFile(ECS_DEFINE_FILE))
		return FALSE;


	CEquipment* pEquipment = NULL;
	CString strClassName, strDevice, strCID, strValue, strForkType, strThreadNo;
	int i, nCount = GetChildElmtCount();
	m_pDoc->m_pEquipments.SetSize(nCount);

	try
	{
		for (i = 0; i < nCount; ++i)
		{
			MoveRoot();
			MoveChild(i);

			GetElmtName(strValue);
			DEBUGER_ASSERT_VALID(strValue == _T("Equipment"));

			GetAttrValue(_T("class"), strClassName);
			GetAttrValue(_T("thread"), strThreadNo);
			GetAttrValue(_T("number"), strValue);
			GetAttrValue(_T("device"), strDevice);

			//	if (strClassName == "CThs") continue;
			//	if (strClassName == "CBcr") continue;
			//	if (strClassName == "CRtv") continue;

			m_pDoc->m_pEquipments[i] = pEquipment = CreateEquipment(strClassName, i, _ttoi(strValue), strDevice);

			if (strClassName == "CSc")
			{
				CString a = "";
			}

			if (pEquipment == NULL)
				continue;

			//DEBUGER_ASSERT_VALID(pEquipment != NULL);

			switch (pEquipment->m_enKind)
			{
			case CEquipment::enCV:
				if (!ParseCv(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseCv!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			case CEquipment::enSC:
				if (!ParseScPairNew(pEquipment))
				{
					DEBUGER_TRACE(_T("Fail to ParseScPairNew!"), _T("CEcsDefine::ParseXml"));
					return FALSE;
				}
				break;

			default:
				DEBUGER_TRACE(_T("등록되지 않은 설비구분!"), _T("CEcsDefine::ParseXml"));
				return FALSE;
			}
		}

		if (!ParseStation())
		{
			DEBUGER_TRACE(_T("Fail to ParseStation!"), _T("CEcsDefine::ParseXml"));
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		m_strErrorMessage = GetErrorMessage(e);
		DEBUGER_TRACE(m_strErrorMessage, _T("CEcsDefine::ParseXml"));
		return FALSE;
	}

	return TRUE;

}
//*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEcsDoc::StatusDisplay()
{
	if (m_MsgList.IsEmpty())
		return;

	CEcsView* pView = (CEcsView*)GetViewObject();
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	CStatusBarMsg* pMsg = (CStatusBarMsg*)m_MsgList.GetHead();
	DEBUGER_ASSERT_VALID(pMsg != NULL);

	pWnd->DisplayMessage(pMsg->m_strMsg);

	if (m_pConfig->m_bBeep && !pMsg->m_bNormalStatus)
		Beep(800, 300);

	m_MsgList.RemoveHead();
	delete pMsg;
}

void CEcsDoc::StatusDisplay(CString strMsg, BOOL bNormalStatus)
{
	//return;

	if (m_MsgList.GetCount() > 5)
		return;

	if (strMsg.IsEmpty())
		return;

	if (m_MsgList.GetCount() > 0)
	{
		CStatusBarMsg* pLastMsg = (CStatusBarMsg*)m_MsgList.GetTail();
		if (pLastMsg == NULL)	return;
		if ((pLastMsg != NULL) && (pLastMsg->m_strMsg.Compare(strMsg) == 0))
			return;
	}

	TRY
	{
		m_MsgList.AddTail(new CStatusBarMsg(strMsg, bNormalStatus));
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
	}
	END_CATCH
}

void CEcsDoc::GetViewHandle()
{
	CView* pView = GetViewObject();
	ASSERT(pView != NULL);
	if (pView == NULL)	return;

	m_hWndView = pView->m_hWnd;
}

CView* CEcsDoc::GetViewObject()
{
	POSITION pos = GetFirstViewPosition();
	ASSERT(pos != NULL);

	if(pos == NULL)
		return NULL;

	return GetNextView(pos);
}

/////////////////////////////////////////////////////////////////////////////
// CEcsDoc commands

void CEcsDoc::ExcuteClient(int nEquipKind, int nEquipNum)
{
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (pEquipment->IsConnect(0))
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->Disconnect();
	}
	else
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 연결하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->ConnectServer(0);
	}
}

void CEcsDoc::ExcuteServer(int nEquipKind, int nEquipNum)
{
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	DEBUGER_ASSERT_VALID(pEquipment != NULL);

	if (pEquipment->IsConnect(0))
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return;

		pEquipment->Disconnect();
	}
	else
	{
		// 18개의 소켓 객체중에 1개라도 널이면 서버를 실행하고, 모두 널이 아니면 서버를 종료한다. 
		BOOL bNotNULL = TRUE;
		for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{
			if (pEquipment->m_pListener[i] == NULL)
			{
				bNotNULL = FALSE;
			}
		}

		if (bNotNULL == TRUE)
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 종료하시겠습니까?"), MB_YESNO) != IDYES)
				return;

			for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
			{
				pEquipment->m_pListener[i]->ShutDown();
				delete pEquipment->m_pListener[i];
				pEquipment->m_pListener[i] = NULL;

				CString strLog;
				strLog.Format(_T("%s 서버 강제종료"), pEquipment->GetDeviceName());
				WriteLog(LOG_TYPE_EVENT, LOG_POS_LGV, strLog, _T("CEcsDoc::ExcuteServer"));
			}
		}
		else
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 실행하시겠습니까?"), MB_YESNO) != IDYES)
				return;

			for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
			{
				pEquipment->RunServer(i);
			}
		}
	}
}

//void CEcsDoc::OnComCv0101()
//{
//	CEquipment* pEquipment = GetEquipment(CEquipment::enCV, 1);
//	CString strTemp;
//	if(pEquipment == NULL)
//		return;
//	strTemp.Format(_T("포트번호 [%d]"),pEquipment->m_port.m_nPort[0]);
//	AfxMessageBox(strTemp);
//	return;
//}

BOOL CEcsDoc::OnComStatusClient(UINT nID)
{
	CString strID, strMsg;
	strID.Format(_T("%d"), nID);
	if ((strID.GetLength() != 5) || (strID[0] != '1'))
	{
		DEBUGER_ASSERT_VALID_LOG(FALSE, strID);
		return FALSE;
	}

	int nEquipKind = _ttoi(strID.Mid(1,2));
	int nEquipNum = _ttoi(strID.Mid(3,2));
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	if (pEquipment == NULL)
	{
		strMsg.Format(_T("해당 설비객체가 존재하지 않습니다! [ID=%d]"), nID);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (pEquipment->IsConnect(0))
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->Disconnect();
	}
	else
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 연결하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->ConnectServer(0);
	}

	return TRUE;
}

BOOL CEcsDoc::OnComStatusServer(UINT nID)
{
	CString strID, strMsg;
	strID.Format(_T("%d"), nID);
	if ((strID.GetLength() != 5) || (strID[0] != '1'))
	{
		DEBUGER_ASSERT_VALID_LOG(FALSE, strID);
		return FALSE;
	}

	int nEquipKind = _ttoi(strID.Mid(1,2));
	int nEquipNum = _ttoi(strID.Mid(3,2));
	CEquipment* pEquipment = GetEquipment(nEquipKind, nEquipNum);
	if (pEquipment == NULL)
	{
		strMsg.Format(_T("해당 설비객체가 존재하지 않습니다! [ID=%d]"), nID);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (pEquipment->IsConnect(0))
	{
		if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 통신을 중단하시겠습니까?"), MB_YESNO) != IDYES)
			return TRUE;

		pEquipment->Disconnect();
	}
	else
	{
		// 18개의 소켓 객체중에 1개라도 널이면 서버를 실행하고, 모두 널이 아니면 서버를 종료한다. 
		BOOL bNotNULL = TRUE;
		for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
		{
			if (pEquipment->m_pListener[i] == NULL)
			{
				bNotNULL = FALSE;
			}
		}

		if (bNotNULL == TRUE)
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 종료하시겠습니까?"), MB_YESNO) != IDYES)
				return FALSE;

			for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
			{
				pEquipment->m_pListener[i]->ShutDown();
				delete pEquipment->m_pListener[i];
				pEquipment->m_pListener[i] = NULL;

				CString strLog;
				strLog.Format(_T("%s 서버 강제종료"), pEquipment->GetDeviceName());
				WriteLog(LOG_TYPE_EVENT, LOG_POS_LGV, strLog, _T("CEcsDoc::ExcuteServer"));
			}
		}
		else
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 실행하시겠습니까?"), MB_YESNO) != IDYES)
				return FALSE;

			//pEquipment->RunServer1();
			for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
			{
				pEquipment->RunServer(i);
			}
		}
		/*
		if (pEquipment->m_pListener1)
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 종료하시겠습니까?"), MB_YESNO) != IDYES)
				return TRUE;

			pEquipment->m_pListener1->ShutDown();
			delete pEquipment->m_pListener1;
			pEquipment->m_pListener1 = NULL;

			CString strLog;
			strLog.Format(_T("%s 서버 강제종료"), pEquipment->GetDeviceName());
			WriteLog(LOG_TYPE_EVENT, LOG_POS_LGV, strLog, _T("CEcsDoc::ExcuteServer"));
		}
		else
		{
			if (AfxMessageBox(pEquipment->GetDeviceName() + _T(" 서버를 실행하시겠습니까?"), MB_YESNO) != IDYES)
				return TRUE;

			pEquipment->RunServer1();
		}
		//*/
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEcsDoc::OnJobTestStart() 
{
}

void CEcsDoc::OnJobTestStop() 
{
	// 출고 작업이 있는지 Check 
	CJobItem * pRetJobItem = m_pJob->FetchByTestJob(enJobPatternRet);
	CJobItem * pStoJobItem = m_pJob->FetchByTestJob(enJobPatternSto);

	if(pRetJobItem == NULL)
	{
		// 없으면 그냥 종료
		m_bTesting = FALSE;
		return;
	}
	else
	{
		// 있으면 입고 중인지 확인할 것!  
		if(pStoJobItem == NULL)
		{
			// 입고 중인 작업이 없으면 입고작업이 생성되면 출고작업을 삭제 하라는 메세지를 띄우고 종료
			if(pRetJobItem->m_nJobStatus == enJobStatusScInvoke)
			{
				AfxMessageBox(_T("현재 출고 중입니다. \n출고 중 일때는 중지할 수 없습니다. \n출고 HS에서 입고 작업이 실행되면... \n그때 다시 한번 중지해 주시시오"));
				return;
			}
//				m_bTesting = FALSE;
		}
		else
		{
			AfxMessageBox(_T("현재 입고 중인 작업이 완료되면 TEST모드가 끝납니다. \n기다려 주세요 \n출고 작업은 자동으로 삭제 됩니다."));
			m_pJob->Remove(pRetJobItem);
			m_bTesting = FALSE;
			return;
		}
	}

	m_bTesting = FALSE;
	return;
}

void CEcsDoc::WriteLog(int nLogType, int nLogPos, int nLuggNum, LPCTSTR lpszContent, LPCTSTR lpszPos, LPCTSTR lpszProdID, BOOL bAlarm)
{
	
	DEBUGER_ASSERT_VALID(m_pLog != NULL);
	DEBUGER_ASSERT_VALID(m_pConfig != NULL);

	CString strLog;
		strLog.Format(_T("%s"), lpszContent);

	if (m_pConfig->m_bLogTrace) 
		DEBUGER_TRACE(strLog, lpszPos);

	if (strLog.GetLength() > _MAX_PATH)
		strLog = strLog.Left(_MAX_PATH);

	switch (nLogType)
	{
	case LOG_TYPE_JOB:		if (m_pLog) m_pLog->Job(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);		break;
	case LOG_TYPE_EVENT:	if (m_pLog) m_pLog->Event(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_ERROR:	if (m_pLog) m_pLog->Error(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_ALARM:	if (m_pLog) m_pLog->Alarm(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	case LOG_TYPE_DEBUG:	if (m_pLog) m_pLog->Debug(nLogPos, nLuggNum, strLog, lpszProdID, bAlarm);	break;
	}
}

void CEcsDoc::WriteLog(int nLogType, int nLogPos, LPCTSTR lpszContent, LPCTSTR lpszPos, CJobItem* pJobItem, LPCTSTR lpszProdID, BOOL bAlarm)
{
	if (pJobItem == NULL) 
		WriteLog(nLogType, nLogPos, LOG_SYSTEM, lpszContent, lpszPos, lpszProdID, bAlarm);
	else 
		WriteLog(nLogType, nLogPos, pJobItem->m_nLuggNum, lpszContent, lpszPos, pJobItem->m_strBarcode, bAlarm);
}

void CEcsDoc::Alarm(int nLogPos, LPCTSTR lpszLog, CJobItem* pJobItem, LPCTSTR lpszBcr)
{
	if (m_pLog == NULL)
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return;
	}

	if (pJobItem == NULL) m_pLog->ShowAlarm(nLogPos, LOG_SYSTEM, lpszLog, lpszBcr);
	else m_pLog->ShowAlarm(nLogPos, pJobItem->m_nLuggNum, lpszLog, pJobItem->m_strBarcode);
}

void CEcsDoc::MulticastServerInfo(CMonitorServer* pMonitorSv, int nCommStatus)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><HOST><SERVER>%d</SERVER></HOST></ECS>%c"), 
		CMonitorServer::enSTX, nCommStatus, CMonitorServer::enETX);


}

void CEcsDoc::MulticastClientInfo(CMonitorServer* pMonitorSv, int nCommStatus)
{
	CString strSend;
	strSend.Format(_T("%c<ECS><HOST><CLIENT>%d</CLIENT></HOST></ECS>%c"), 
		CMonitorServer::enSTX, nCommStatus, CMonitorServer::enETX);
}

//==============================================================================
//	Desc	: MENU - 시스템 - 환경설정 (TOOLBAR - ECS 기본 정보)
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnSystemConfig1() 
{
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 0);
	ConfigSheet.DoModal();
}

//==============================================================================
//	Desc	: TOOLBAR - 경로 및 설비별 색상 정보
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnSystemConfig2() 
{
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 1);
	ConfigSheet.DoModal();
}

//==============================================================================
//	Desc	: TOOLBAR - ECS 클라이언트 정보
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnSystemConfig3() 
{
	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 2);
	ConfigSheet.DoModal();
}

//==============================================================================
//	Desc	: MENU - 시스템 - 사용자 관리
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnSysUserManager() 
{
//	CUserManagerDlg dlg;
//	dlg.DoModal();
}

//==============================================================================
//	Desc	: ReLoad - Logic
//	Date	: 2021.10.15
//	Update	: 
//==============================================================================
void CEcsDoc::LoadLogic(int nCount)
{
	m_bReLoading = TRUE;

	CLogicSystem logic(this);
	if (!logic.ParseXml())
	{
		AfxMessageBox(logic.GetErrorMessage());
		return;
	}

	m_bReLoading = FALSE;
}

//==============================================================================
//	Desc	: MENU - ReLoad - Logic
//	Date	: 2021.10.15
//	Update	:
//==============================================================================
void CEcsDoc::OnReLoadLogic()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	int nPlcCount = ::GetPrivateProfileInt(_T("COMMON"), _T("PlcCnt"), 1, ECS_INI_FILE);

	LoadLogic(nPlcCount);
}

//==============================================================================
//	Desc	: ReLoad - DeviceMap
//	Date	: 2021.10.15
//	Update	: 
//==============================================================================
void CEcsDoc::LoadDeviceMap(int nCount)
{
	m_bReLoading = TRUE;

	int i;
	for (i = 0; i < m_pDeviceMaps.GetSize(); ++i)
	{
		if (m_pDeviceMaps[i] != NULL)
			delete m_pDeviceMaps[i];
	}
	m_pDeviceMaps.RemoveAll();

	//m_strTrackingAreas.SetSize(nCount);

	for (i = 0; i < nCount; i++)
	{
		CDeviceMap* pDeviceMap = new CDeviceMap(this);

		CString strDeviceMapFilePath;
		strDeviceMapFilePath.Format(_T(".\\DeviceMap%02d.xml"), i + 1);

		if (!pDeviceMap->ParseXml(strDeviceMapFilePath, i))
		{
			AfxMessageBox(pDeviceMap->m_strLog);
			continue;
		}

		m_pDeviceMaps.Add(pDeviceMap);
	}

	m_bReLoading = FALSE;
}

//==============================================================================
//	Desc	: MENU - ReLoad - DeviceMap
//	Date	: 2021.10.15
//	Update	:
//==============================================================================
void CEcsDoc::OnReLoadDeviceMap()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	int nPlcCount = ::GetPrivateProfileInt(_T("COMMON"), _T("PlcCnt"), 1, ECS_INI_FILE);

	LoadDeviceMap(nPlcCount);
}

//==============================================================================
//	Desc	: ReLoad - KeyWord
//	Date	: 2021.10.15
//	Update	: 
//==============================================================================
void CEcsDoc::LoadKeyWord(int nCount)
{
	m_bReLoading = TRUE;

	CKeyWord keyword(this);
	if (!keyword.ParseXml())
	{
		AfxMessageBox(keyword.GetErrorMessage());
		return;
	}

	m_bReLoading = FALSE;
}

//==============================================================================
//	Desc	: MENU - ReLoad - KeyWord
//	Date	: 2021.10.15
//	Update	:
//==============================================================================
void CEcsDoc::OnReLoadKeyWord()
{
	TCHAR szPath[_MAX_PATH] = { 0 };
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

	int nPlcCount = ::GetPrivateProfileInt(_T("COMMON"), _T("PlcCnt"), 1, ECS_INI_FILE);

	LoadKeyWord(nPlcCount);
}

//==============================================================================
//	Desc	: MENU - 작업 - 작업정보 관리
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnJobManager() 
{
	if (m_pJobDlg != NULL)
	{
		m_pJobDlg->ShowWindow(SW_SHOWNORMAL);
		m_pJobDlg->BringWindowToTop();
	}
	else
	{
		m_pJobDlg = new CJobDlg(this, NULL);
		m_pJobDlg->Create(IDD_JOB);
		m_pJobDlg->ShowWindow(SW_SHOW);
	}
}

//==============================================================================
//	Desc	: MENU - 작업 - 반자동 작업
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnJobOffline() 
{
}

//==============================================================================
//	Desc	: MENU - 로그 - 로그 관리
//	Date	: 2013.09
//	Update	:
//==============================================================================

void CEcsDoc::OnLogAll() 
{
//	if (m_pLogDlg != NULL)
//	{
//		m_pLogDlg->ShowWindow(SW_SHOWNORMAL);
//		m_pLogDlg->BringWindowToTop();
//	}
//	else
//	{
//		m_pLogDlg = new CLogDlg(this, m_pLog);
//		m_pLogDlg->Create(IDD_LOG);
//		m_pLogDlg->ShowWindow(SW_SHOW);
//	}
	::ShellExecute(NULL, _T("open"), _T(".\\EcsLog.exe"), NULL, NULL, SW_SHOW);
}

//==============================================================================
//	Desc	: MENU - 로그 - 알람 관리
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::OnWarning() 
{
	if (m_pWarningDlg != NULL)
	{
		m_pWarningDlg->ShowWindow(SW_SHOW);
		if (m_pWarningDlg->GetSafeHwnd() != NULL)
			::PostMessage(m_pWarningDlg->GetSafeHwnd(), WM_USER_REFRESH_NOTIFY, 0, 0);
	}
	else
	{
		m_pWarningDlg = new CWarningDlg(this);
		m_pWarningDlg->Create(CWarningDlg::IDD);
		m_pWarningDlg->ShowWindow(TRUE);	
	}
}

void CEcsDoc::TrackData(CString Type, CString Addr, CString Name, CString Value)
{
	if (m_pTrackDataDlg != NULL)
	{
		m_pTrackDataDlg->ShowWindow(SW_SHOW);
		m_pTrackDataDlg->BringWindowToTop();
		if (m_pTrackDataDlg->GetSafeHwnd() != NULL)
			::PostMessage(m_pTrackDataDlg->GetSafeHwnd(), WM_USER_REFRESH_NOTIFY, 0, 0);
	}
	else
	{
		m_pTrackDataDlg = new CTrackDataDlg(this);
		m_pTrackDataDlg->Create(CTrackDataDlg::IDD);
		m_pTrackDataDlg->ShowWindow(TRUE);
	}
	m_pTrackDataDlg->ValueChange(Type, Addr, Name, Value);
}

void CEcsDoc::TrackConditionData(CString Value)
{
	if (m_pDataSettingDlg != NULL)
	{
		m_pDataSettingDlg->ShowWindow(SW_SHOW);
		m_pDataSettingDlg->BringWindowToTop();
		if (m_pDataSettingDlg->GetSafeHwnd() != NULL)
			::PostMessage(m_pDataSettingDlg->GetSafeHwnd(), WM_USER_REFRESH_NOTIFY, 0, 0);
	}
	else
	{
		m_pDataSettingDlg = new CDataSettingDlg(this);
		m_pDataSettingDlg->Create(CDataSettingDlg::IDD);
		m_pDataSettingDlg->ShowWindow(TRUE);
	}
	m_pDataSettingDlg->ValueChange(Value);
}

//==============================================================================
//	Desc	: MENU - 로그 - FMS 송수신 이력
//	Date	: 2013.09
//	Update	:
//==============================================================================
//void CEcsDoc::OnIfTrans() 
//{
//	CIfTransLogDlg dlg(this);
//	dlg.DoModal();
//}

//==============================================================================
//	Desc	: 설비상태통지 상태값 변경
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CEcsDoc::InvokeCenterEquipCtrl(CString strFrame) 
{
	CDciControl* pDciControl = NULL;
	CString strData = strFrame;
	CString strID, str;
	int nStatus = 0;

	for (int n = 0; n < strData.GetLength(); n += 4)
	{
		str = strData.Mid(n, 3);
		if (str == "   ")	continue;
		strID.Format(_T("%s00000"), str);
		pDciControl = GetDciControl(strID);
		nStatus = _ttoi(strData.Mid(n + 3, 1));
		// Update : 2014.05 - 중앙설비상태값 관리
		m_mapCEquipStatus.SetAt(_ttoi(str), nStatus);	// 중앙설비상태값 설정
		pDciControl->m_clrBgColor = GetColor(nStatus);
		pDciControl->InvalidateControl(m_hWndView, FALSE);
		// 중앙설비 C/V Color
		int nNo = 0;
		while (TRUE)
		{
			str.Format(_T("%d"), _ttoi(strID) + ++nNo);
			pDciControl = GetDciControl(str);
			if (pDciControl == NULL)	break;
			pDciControl->m_clrBgColor = GetColor(nStatus);
			pDciControl->InvalidateControl(m_hWndView, FALSE);
		}
	}
}

//==============================================================================
//	Desc	: 설비상태통지 Color
//	Date	: 2013.09
//	Update	:
//==============================================================================
COLORREF CEcsDoc::GetColor(int nStatus)
{
	switch (nStatus)
	{
	case enCEquipStatusOff :
		return WHITE;
	case enCEquipStatusWait :
		return OLIVE;
//	case enCEquipStatusAutoRun :
//		return PALE_GRAY;
	case enCEquipStatusAutoStop :
		return LIGHT_ORANGE;
	case enCEquipStatusManual :
		return DARK_GRAY;
	case enCEquipStatusError :
		return RED;
	}
	return PALE_GRAY;
}

//==============================================================================
//	Desc	: 실시간 물류 가동률 상태 업데이트
//	Date	: 
//	Update	:
//==============================================================================
void CEcsDoc::UpdateOperationRate() 
{
	// 총가동률
	CString strID = _T("99900001");
	CDciControl* pDciControl = GetDciControl(strID);
	pDciControl->m_strText.Format(_T("%s"), m_pConfig->m_strCapacity);
	pDciControl->InvalidateControl(m_hWndView, FALSE);
	// 현재 가동률
	strID = _T("99900002");
	pDciControl = GetDciControl(strID);
	pDciControl->m_strText.Format(_T("%d"), m_nCurCapacity);
	pDciControl->InvalidateControl(m_hWndView, FALSE);
	// 실시간 물류 TRAY 수량
	strID = _T("99900003");
	pDciControl = GetDciControl(strID);
	pDciControl->m_strText.Format(_T("%d"), m_nLoopTrayCount);
	pDciControl->InvalidateControl(m_hWndView, FALSE);
	// 클라이언트로 실시간 물류 정보 송신
}

CDciControl* CEcsDoc::GetDciControl(CString& strCID, int nTabNum)	
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	DEBUGER_ASSERT_VALID(pView != NULL);

	CDciControl* pDciControl = NULL;

	int nLayoutCnt = m_pEcsLayOuts.GetSize();
	for (int i=0;i<nLayoutCnt;i++)
	{
		CEcsLayout * pEcsLayout = m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		if (nTabNum != 0 && nTabNum != i + 1)
			continue;

		pDciControl = pEcsLayout->GetDciControl(strCID);
		if (pDciControl != NULL)
			return pDciControl;
	}
	return NULL;
}

CEcsLayout* CEcsDoc::GetSelectedLayout()
{
	switch(m_layoutSelected)
	{
	case EN_1F:
		{
			return &m_layout1;
		}
	case EN_2F:
		{
			return &m_layout2;
		}
	case EN_Assemble:
		{
			return &m_layoutAssemble;
		}
	}
	return NULL;
}

CEcsLayout* CEcsDoc::GetSelectedLayout(EN_LAYOUT pEnLayout)
{
	switch(pEnLayout)
	{
	case EN_1F:
		{
			return &m_layout1;
		}
	case EN_2F:
		{
			return &m_layout2;
		}
	case EN_Assemble:
		{
			return &m_layoutAssemble;
		}
	}
	
	return NULL;
}

//==============================================================================
//	Desc	: Logic 값 검증
//	Date	: 2022.02.08
//	Update	:
//==============================================================================
void CEcsDoc::OnLogicValid()
{
	if (m_pLogicValidationDlg != NULL)
	{
		m_pLogicValidationDlg->ShowWindow(SW_SHOWNORMAL);
		m_pLogicValidationDlg->BringWindowToTop();
	}
	else
	{
		m_pLogicValidationDlg = new CLogicValidationDlg(this, NULL);
		m_pLogicValidationDlg->Create(IDD_LOGIC_VALIDATION);
		m_pLogicValidationDlg->ShowWindow(SW_SHOW);
	}
}