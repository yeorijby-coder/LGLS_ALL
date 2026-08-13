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
//	ON_COMMAND(ID_SYSTEM_CONFIG3, OnSystemConfig3)
	ON_COMMAND(ID_SYS_USER_MANAGER, OnSysUserManager)
	ON_COMMAND(ID_JOB_MANAGER, OnJobManager)
	ON_COMMAND(ID_JOB_OFFLINE, OnJobOffline)
	ON_COMMAND(ID_LOG_ALL, OnLogAll)
	ON_COMMAND(ID_WARNING, OnWarning)
	//ON_COMMAND(ID_COM_CV01_01, OnComCv0101)
	//ON_COMMAND(ID_COM_CV01_02, OnComCv0102)
	//ON_COMMAND(ID_COM_CV01_03, OnComCv0103)
	//ON_COMMAND(ID_COM_CV01_04, OnComCv0104)
	//ON_COMMAND(ID_COM_CV01_05, OnComCv0105)
	//}}AFX_MSG_MAP
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
//	m_pLogDlg		= NULL;
	m_pWarningDlg	= NULL;

	m_bTesting		= FALSE;

	m_bMoveFlag = FALSE;
	m_bNextDualStore = FALSE;
	m_bNextEmptyRetrieve = FALSE;
	m_bScInterLock = FALSE;
	m_layoutSelected = EN_1F;
	m_nCenterNextPlcNum = 0;
	m_nCrossPlcNum = 0;
	m_nNextPlcNum = 0;
	m_nNextPlcNum2 = 0;
	for (int i = 0; i < 30; ++i) m_nStTrNum[i] = 0;
	for (int i = 0; i < 100; ++i) m_nCurrLoadingHsNum[i] = 0;

	/*	기존 코드
	m_wSc1Status910 = 0x0000;
	m_wSc1Status911 = 0x0000;
	m_wSc1Status912 = 0x0000;
	m_wSc2Status910 = 0x0000;
	m_wSc2Status911 = 0x0000;
	m_wSc1Status920 = 0x0000;
	m_wSc2Status921 = 0x0000;
	m_wSc3Status922 = 0x0000;
	*/

	m_bLogPlayback  = FALSE;

	m_mapTempVals.RemoveAll();
	m_mapHumVals.RemoveAll();
	m_dtHeartBit = COleDateTime::GetCurrentTime();
	m_bHeartBit = FALSE;
	m_mapCEquipStatus.RemoveAll();
	m_nLoopTrayCount = 0;	// Update : 2014.06.08 - Loop track 실시간 물류 TRAY 수량
	m_nCurCapacity = 0;		// Update : 2014.06.08 - 현재 가동률
	m_bBackupChk = FALSE;

	for (int i=0; i<100; i++)
	{
		m_bLoadingTrackDelete[i] = FALSE;
		m_bLoadingTrackDelete2[i] = FALSE;
		m_bUnLoadingTrackDelete[i] = FALSE;
		m_bUnLoadingTrackDelete2[i] = FALSE;
		m_nCurrLoadingPlcNum[i]=0;
		m_bUnLoadComplete[i] = FALSE;
		m_nCompleteFork[i] = 0;
		m_nCurrLoadingTrackNum[i] = 0;
		m_nCurrLoadingTrackNum2[i] = 0;

		m_nCurrUnLoadingTrackNum[i] = 0;
		m_nCurrUnLoadingTrackNum2[i] = 0;
		m_nCurrUnLoadingPlcNum[i] = 0;

		m_arrRegData[i].SetSize(10000);
		for (int j=0; j<10000; j++)
			m_arrRegData[i][j] = 0x00;
	}
	
}

CEcsDoc::~CEcsDoc()
{
	if (m_pWarningDlg != NULL) delete m_pWarningDlg;
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
		m_pEquipments[i]->Disconnect();
		
		m_pEquipments[i]->GetInfo()->Backup();

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
void CEcsDoc::DeleteData(int nPlcNum, int nFork) 
{
	int nCount = 8;
	
	BOOL bTemp = FALSE;
	int nRegAddr = 0;
	
	switch(nFork)
	{
	case 1:	
		DeleteData1(nPlcNum);
		// 데이타가 2번 포크에도 없으면 작업구분을 없앤다.
		nRegAddr = 123;
		break;
	case 2:		
		DeleteData2(nPlcNum);							
		// 데이타가 1번 포크에도 없으면 작업구분을 없앤다.
		nRegAddr = 113;
		break;
	case 3:		
		m_arrRegData[nPlcNum][111] = 0x00;	
		DeleteData1(nPlcNum);		
		DeleteData2(nPlcNum);		
		break;		
	}
	
	// 반대편 포크에 데이타 있는지 확인한다.
	if (nRegAddr != 0)
	{
		for (int i=0;i<nCount;i++)
		{
			int nTemp = m_arrRegData[nPlcNum][nRegAddr + i];
			if (nTemp != 0x00)
				bTemp = TRUE;
		}
	}
	
	// 반대편 포크에도 데이터가 없으면... 작업구분을 없애준다.
	if (bTemp == FALSE)
	{
		m_arrRegData[nPlcNum][111] = 0x00;		
	}
}

void CEcsDoc::DeleteData1(int nPlcNum) 
{
	int nCount = 8;
	
	for (int i = 0 ; i < nCount ; i++)
	{
		m_arrRegData[nPlcNum][113 + i] = 0x00;
	}
}

void CEcsDoc::DeleteData2(int nPlcNum) 
{
	int nCount = 8;
	
	m_arrRegData[nPlcNum][121] = 0x00;
	
	for (int i = 0 ; i < nCount ; i++)
	{
		m_arrRegData[nPlcNum][123 + i] = 0x00;
	}
}


CEquipment* CEcsDoc::CreateEquipment(CString& strClassName, int nIndex, int nNumber, LPCTSTR lpszDevice)
{
	if (strClassName.IsEmpty())
	{
		DEBUGER_ASSERT_VALID(FALSE);
		return NULL;
	}
	
	if (strClassName == RUNTIME_CLASS(CCv)->m_lpszClassName) return new CCv(this, nIndex, nNumber, lpszDevice);
	else if (strClassName == RUNTIME_CLASS(CSc)->m_lpszClassName) return new CSc(this, nIndex, nNumber, lpszDevice);
	else if (strClassName == RUNTIME_CLASS(CScPair)->m_lpszClassName) return new CScPair(this, nIndex, nNumber, lpszDevice);
//	else if (strClassName == RUNTIME_CLASS(CRtv)->m_lpszClassName) return new CRtv(this, nIndex, nNumber, lpszDevice);
	else DEBUGER_ASSERT_VALID_LOG(FALSE, strClassName);
	
	return NULL;	
}


BOOL CEcsDoc::LoadXML()
{
	CString strXmlFile, strValue, strName;	
	strXmlFile.Format(_T(".\\SC.xml"));
	
	CXmlDom dom;
	try
	{
		dom.InitializeXmlDom();
		if (dom.LoadXmlFile(strXmlFile) == FALSE)
		{
			return FALSE;
		}
		int i, nCount = dom.GetChildElmtCount();

		m_pEquipments.SetSize(nCount);
		
		for(i=0; i<nCount; ++i)
		{
			dom.MoveRoot();
			dom.MoveChild(i);
			
			dom.GetAttrValue(_T("TYPE"), strValue);
			int nForkType = _ttoi(strValue);
			
			dom.GetAttrValue(_T("class"), strValue);
			CString strClass = strValue;
			dom.GetAttrValue(_T("number"), strValue);
			int nNumber = _ttoi(strValue);
			dom.GetAttrValue(_T("device"), strValue);
			CString strDevice = strValue;
			dom.GetAttrValue(_T("cid"), strValue);
			CString strCID = strValue;
			dom.GetAttrValue(_T("com"), strValue);
			CString strCommMethod = strValue;

			CString strClassName = strClass;
			if (strClass == "CRtv")
				strClassName = "CSc";
			else if (strClass == "CRtvPair")
				strClassName = "CScPair";

			CEquipment* pEquipment = NULL;
			
			m_pEquipments[i] = pEquipment = CreateEquipment(strClassName, i, nNumber, strDevice);
			
			if (pEquipment == NULL)
				return FALSE;
			
			CScInfo* pInfo = NULL;
			
			if (strClassName == "CSc")
			{
				CSc* pSc = (CSc*)pEquipment;
				
				if (pSc == NULL )
					return FALSE;
				
				pInfo = (CScInfo*) pSc->GetInfo();
				
				if (pInfo == NULL )
					return FALSE;
				
				pInfo->m_nForkType = nForkType;

				if (strClass == "CRtv")
					pInfo->m_bRtv = TRUE;

				if (strCommMethod == "S")
					pInfo->m_bEtherNet = FALSE;

				pInfo->m_pControl = (CDciRvCtrl*)GetDciControl(strCID);
				if(pInfo->m_pControl == NULL)
				{
					int a = 0;
				}
				DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
			}
			else if (strClassName == "CScPair")
			{
				CScPair* pSc = (CScPair*)pEquipment;
				
				if (pSc == NULL )
					return FALSE;
				
				pInfo = (CScInfo*) pSc->GetInfo();
				
				if (pInfo == NULL )
					return FALSE;
				
				pInfo->m_nForkType = nForkType;

				if (strClass == "CRtvPair")
					pInfo->m_bRtv = TRUE;

				if (strCommMethod == "S")
					pInfo->m_bEtherNet = FALSE;

				pInfo->m_pControl = (CDciRvCtrl*)GetDciControl(strCID);
				DEBUGER_ASSERT_VALID(pInfo->m_pControl != NULL);			// @@
			}			
			else
			{
				if (pEquipment->m_enKind == CEquipment::enCV)
				{
					CCv* pCv = (CCv*)pEquipment;
					CCvInfo* pInfo = pCv->m_pInfo;
					DEBUGER_ASSERT_VALID(pCv != NULL);
					DEBUGER_ASSERT_VALID(pInfo != NULL);
					
					if (strCommMethod == "S")
						pInfo->m_bEtherNet = FALSE;

					if (dom.MoveXPath(_T("./Tracks"), FALSE))	//FALSE 	//Equipment/Tracks
					{
						CString strCID, strTrackCID, strTabNum;
						dom.GetAttrValue(_T("cid"), strCID);
						DEBUGER_ASSERT_VALID_LOG(CLib::IsValidControlID(strCID), strCID);
						
						CTrackInfo* pTrack = NULL;
						int j, nCvCount = dom.GetChildElmtCount();
						pInfo->m_pTracks.SetSize(nCvCount);
						
						for (j=0; j<nCvCount; ++j)
						{
							dom.MoveChild(j);	//Equipment/Tracks/Track
							
							pInfo->m_pTracks[j] = pTrack = new CTrackInfo(pEquipment);
							DEBUGER_ASSERT_VALID(pTrack != NULL);
							
							dom.GetAttrValue(_T("number"), strValue);
							pTrack->m_nCvPlcNum = nNumber;		//PLC 넘버 
							//pTrack->m_nNumber = (nNumber * 100) + _ttoi(strValue) ;						// !!!!!!!!
							pTrack->m_nNumber = _ttoi(strValue);
							if(pTrack->m_nNumber >= 40)
							{
								int a = 0;
							}
							
							strTrackCID.Format(_T("%s%05d"), strCID.Left(5), pTrack->m_nNumber);		// !!!!!!!!
							
							pTrack->m_pTrackCtrl = (CDciTrackCtrl*)GetDciControl(strTrackCID);
							DEBUGER_ASSERT_VALID(CLib::IsValidControlID(strTrackCID));
							
							if (dom.MoveXPath(_T("./Status"), FALSE))	//Equipment/Tracks/Track/Status
							{
								int k;
								for (k=0; k<dom.GetChildElmtCount(); ++k)
								{
									dom.GetChildElmtName(k, strValue);
									
									WORD wStatus = 0;
									if (strValue == _T("StoStation")) wStatus = CTrackInfo::enStatusStoSTReady;
									else if (strValue == _T("RetStation")) wStatus = CTrackInfo::enStatusRetSTReady;
									else if (strValue == _T("ArvStation")) wStatus = CTrackInfo::enStatusArvSTReady;
									else if (strValue == _T("ScStoHS")) wStatus = CTrackInfo::enStatusScStoHSReady;
									else if (strValue == _T("ScRetHS")) wStatus = CTrackInfo::enStatusScRetHSReady;
									else if (strValue == _T("RtvDepartHS")) wStatus = CTrackInfo::enStatusRtvDepartHSReady;
									else if (strValue == _T("RtvArriveHS")) wStatus = CTrackInfo::enStatusRtvArriveHSReady;
									else if (strValue == _T("LgvDepartHS")) wStatus = CTrackInfo::enStatusLgvDepartHSReady;
									else if (strValue == _T("LgvArriveHS")) wStatus = CTrackInfo::enStatusLgvArriveHSReady;
									else if (strValue == _T("LfDepartHS")) wStatus = CTrackInfo::enStatusLfDepartHSReady;
									else if (strValue == _T("LfArriveHS")) wStatus = CTrackInfo::enStatusLfArriveHSReady;
									else if (strValue == _T("CvCheckPos")) wStatus = CTrackInfo::enStatusCvCheckReady;
									else DEBUGER_ASSERT_VALID_LOG(FALSE, strValue);
									
									pTrack->m_mapStatusInfos.SetAt(wStatus, NULL);
									pTrack->m_arrayStatus.Add(wStatus);
								}
								
								dom.MoveParent();	//Equipment/Tracks/Track
							}
							
							dom.MoveParent();	//Equipment/Tracks
						}
						
						dom.MoveParent();	//Equipment
					}
					
					if (pInfo->m_pTracks.GetSize())
						m_pTrackInfos.Append(pInfo->m_pTracks);		
				}
			}
			
			if (dom.MoveXPath(_T("./HS_GROUP"), FALSE))	//SC/HS_GROUP
			{
				if (dom.MoveXPath(_T("./STO_HS"), FALSE))	//SC/HS_GROUP/STO_HS
				{
					CTrackHS* pTrackHS = NULL;
					int j, k, nHsCount = dom.GetChildElmtCount();
					pInfo->m_pStosHS.SetArraySize(nHsCount);
									
					for (k=0; k<nHsCount; ++k)
					{
						pInfo->m_pStosHS[k] = pTrackHS = new CTrackHS(pEquipment);
						DEBUGER_ASSERT_VALID(pTrackHS != NULL);
						
						dom.MoveChild(k);	//SC/HS_GROUP/STO_HS/HS
						
						dom.GetAttrValue(_T("NO"), strValue);
						pTrackHS->m_nPos = _ttoi(strValue);
						
						dom.GetAttrValue(_T("PAIR"), strValue);
						pTrackHS->m_nPair = _ttoi(strValue);
						
						dom.GetAttrValue(_T("CV_PLC_NO"), strValue);
						pTrackHS->m_nCvPlcNo = _ttoi(strValue);
						
						dom.GetAttrValue(_T("BANK"), strValue);
						pTrackHS->m_nBank = _ttoi(strValue);
						dom.GetAttrValue(_T("BAY"), strValue);
						pTrackHS->m_nBay = _ttoi(strValue);
						dom.GetAttrValue(_T("LEVEL"), strValue);
						pTrackHS->m_nLevel = _ttoi(strValue);
						
						// 입고 H/S : SC 로딩 완료시 지울 CV 작업대 주소영역 (선택 속성)
						strValue = dom.GetAttrValue(CString(_T("DEL_ADDR")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nDelAddr = _ttoi(strValue);
						strValue = dom.GetAttrValue(CString(_T("DEL_CNT")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nDelCnt = _ttoi(strValue);
						strValue = dom.GetAttrValue(CString(_T("PROD_ADDR")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nProdAddr = _ttoi(strValue);
						// PAIR 트랙(포크#2)용 속성
						strValue = dom.GetAttrValue(CString(_T("DEL_ADDR2")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nDelAddr2 = _ttoi(strValue);
						strValue = dom.GetAttrValue(CString(_T("DEL_CNT2")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nDelCnt2 = _ttoi(strValue);
						strValue = dom.GetAttrValue(CString(_T("PROD_ADDR2")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nProdAddr2 = _ttoi(strValue);

						dom.GetAttrValue(_T("TID"), strValue);
						//int nTrackNumber = (pTrackHS->m_nCvPlcNo * 100) + _ttoi(strValue);			//!!!!!!!
						int nTrackNumber = /*(pTrackHS->m_nCvPlcNo * 100) + */_ttoi(strValue);			//!!!!!!!
						pTrackHS->m_pTrack = GetTrackInfo(nTrackNumber, pTrackHS->m_nCvPlcNo);
						if(pTrackHS->m_pTrack == NULL)
						{
							int a =0;
						}
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);
						
						j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
						pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScStoHSReady, pTrackHS);
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScStoHSReady] == pTrackHS);
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);
						
						dom.MoveParent();	//SC/HS_GROUP/STO_HS/HS
					}
					
					dom.MoveParent();	//SC/HS_GROUP/STO_HS
					
				}
				
				if (dom.MoveXPath(_T("./RET_HS"), FALSE))	//SC/HS_GROUP/RET_HS
				{
					CTrackHS* pTrackHS = NULL;
					int j, k, nHsCount = dom.GetChildElmtCount();
					
					pInfo->m_pRetsHS.SetArraySize(nHsCount);
					for (k=0; k<nHsCount; ++k)
					{
						pInfo->m_pRetsHS[k] = pTrackHS = new CTrackHS(pEquipment);
						DEBUGER_ASSERT_VALID(pTrackHS != NULL);
						
						dom.MoveChild(k);	//SC/HS_GROUP/RET_HS/HS
						
						dom.GetAttrValue(_T("NO"), strValue);
						pTrackHS->m_nPos = _ttoi(strValue);
						
						dom.GetAttrValue(_T("PAIR"), strValue);
						pTrackHS->m_nPair = _ttoi(strValue);
						
						dom.GetAttrValue(_T("CV_PLC_NO"), strValue);
						pTrackHS->m_nCvPlcNo = _ttoi(strValue);
						
						dom.GetAttrValue(_T("BANK"), strValue);
						pTrackHS->m_nBank = _ttoi(strValue);
						dom.GetAttrValue(_T("BAY"), strValue);
						pTrackHS->m_nBay = _ttoi(strValue);
						dom.GetAttrValue(_T("LEVEL"), strValue);
						pTrackHS->m_nLevel = _ttoi(strValue);
						
						// 출고 H/S : SC 언로딩 완료시 화물감지를 쓸 CV 주소영역 (선택 속성)
						strValue = dom.GetAttrValue(CString(_T("PROD_ADDR")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nProdAddr = _ttoi(strValue);
						// PAIR 트랙(포크#2)용 속성
						strValue = dom.GetAttrValue(CString(_T("PROD_ADDR2")));
						if (strValue.IsEmpty() == FALSE)
							pTrackHS->m_nProdAddr2 = _ttoi(strValue);

						dom.GetAttrValue(_T("TID"), strValue);
						//int nTrackNumber = (pTrackHS->m_nCvPlcNo * 100) + _ttoi(strValue);		//!!!!!!!
						int nTrackNumber = /*(pTrackHS->m_nCvPlcNo * 100) + */_ttoi(strValue);			//!!!!!!!
						pTrackHS->m_pTrack = GetTrackInfo(nTrackNumber, pTrackHS->m_nCvPlcNo);
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack != NULL);
						
						j = pTrackHS->m_pTrack->m_mapStatusInfos.GetCount();
						pTrackHS->m_pTrack->m_mapStatusInfos.SetAt(CTrackInfo::enStatusScRetHSReady, pTrackHS);
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos[CTrackInfo::enStatusScRetHSReady] == pTrackHS);
						DEBUGER_ASSERT_VALID(pTrackHS->m_pTrack->m_mapStatusInfos.GetCount() == j);
						
						dom.MoveParent();	//SC/HS_GROUP/STO_HS/HS
					}
					
					dom.MoveParent();	//SC/HS_GROUP/STO_HS
					
				}
				
				dom.MoveParent();	//SC/HS_GROUP
			}

			if (pInfo != NULL && pInfo->m_nForkType == CScInfo::enMCS)
			{
				if (dom.MoveXPath(_T("./RACK_INFO"), FALSE))	//SC/RACK_INFO
				{
					// 랙정보를 세팅하는 부분 
				//	dom.GetAttrValue(_T("LEVEL_START"), strValue);
					pInfo->m_nStartLevel = 5;
					
				//	dom.GetAttrValue(_T("LEVEL_START"), strValue);
					pInfo->m_nEndLevel = 5;
					
					dom.MoveParent();	//SC/RACK_INFO
				}
			}
		}
	}
	catch ( _com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CEcsDoc::SaveXML()
{
	CXmlDom dom;
	
	CString str;
	
	CString strXmlFile, strValue, strName;
	strXmlFile.Format(_T(".\\SC.xml"));
	
	try
	{
		if (dom.InitializeXmlDom() == FALSE)
		{
			AfxMessageBox(dom.GetErrorMessage());
			return FALSE;
		}
		
		dom.MoveDocument();
		dom.AddChildProcessingInstruction(_T("xml"), _T("version='1.0' encoding='euc-kr'"));
		dom.AddChildComment(_T("SC Simualtor"));
		dom.AddChildElement(_T("SC"));

		strXmlFile.Format(_T(".\\SC1.xml"));
		
		if (dom.SaveXmlFile(strXmlFile) == FALSE)
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

BOOL CEcsDoc::Initialize()
{
	GetViewHandle();

	//m_oControlDlg.m_pDoc = this;
	//m_oControlDlg.Create(IDD_ECS_CONTROL);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
//	if (!m_layout1.LoadXml(ECS_LAYOUT_FILE_LAOUT_1))
//	{
//		AfxMessageBox(m_layout1.m_strLog);
//		return FALSE;
//	}
	
	int i = 0;
//	char szPath[_MAX_PATH] = {0};
	TCHAR szPath[_MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(szPath), (LPWSTR)szPath);

	g_strEcsPath = szPath;

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
	/*
	CEcsDefine define(this);
	if (!define.ParseXml())
	{
	AfxMessageBox(define.GetErrorMessage());
	return FALSE;
	}
	//*/
	
	if (!LoadXML())
	{
		AfxMessageBox(_T("SIM 설정 파일 로드 실패!"));
		return FALSE;
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

void CEcsDoc::ReconnectServer()
{
	//	if ((m_pHostCl == NULL) && (m_pConfig->m_portHost.m_bRetry == TRUE))
	//		ConnectHostServer();
	
	m_pEquipments.ReconnectServers();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CEcsDoc::StatusDisplay()
{
	//return;

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
void CEcsDoc::WriteCVPLCWord(int nPLCNum, int nAddress, WORD wData)
{
	CCv* pCv = (CCv*) GetEquipment(CEquipment::enCV, nPLCNum);
	DEBUGER_ASSERT_VALID(pCv != NULL);
	
	pCv->WriteWordValue(nAddress, wData);
}

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
		// 2개의 소켓 객체중에 1개라도 널이면 서버를 실행하고, 모두 널이 아니면 서버를 종료한다. 
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

			//pEquipment->RunServer1();
			for (int i = 0 ; i < PLC_CONN_PORT_CNT ; i++)
			{
				pEquipment->RunServer(i);
			}
		}
	}
}

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
		// 2개의 소켓 객체중에 1개라도 널이면 서버를 실행하고, 모두 널이 아니면 서버를 종료한다. 
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
	//*/
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
//void CEcsDoc::OnSystemConfig3() 
//{
//	CConfigPptSheet ConfigSheet(this, m_pConfig, _T("환경설정"), NULL, 2);
//	ConfigSheet.DoModal();
//}

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

BOOL CEcsDoc::GetLocationByHs(int nScPlcNum, int nType, int nHsNum, int& nBank, int& nBay, int& nLevel, int& nTrackNum, int& nPairTrackNum, int& nPlcNum)	
{
	//	int nPlcNum;	// 필요하면 나중에 인자로 집어넣어라.
	
	CSc *pSc = (CSc*) GetEquipment(CEquipment::enSC, nScPlcNum+1);
	CScInfo *pInfo = (CScInfo *) pSc->GetInfo();

	CTrackHS * pTrackHS = NULL;
	if (nType == 1)
	{
		pTrackHS = pInfo->m_pStosHS.FindByPos(nHsNum);
	}
	else if (nType == 2)
	{
		pTrackHS = pInfo->m_pRetsHS.FindByPos(nHsNum);
	}
	
	if (pTrackHS == NULL)
		return FALSE;
	
	nBank = pTrackHS->m_nBank;
	nBay = pTrackHS->m_nBay;
	nLevel = pTrackHS->m_nLevel;
	
	nPlcNum = pTrackHS->m_nCvPlcNo;
	nTrackNum = pTrackHS->m_pTrack->m_nNumber;
	nPairTrackNum = pTrackHS->m_nPair;
	
	return TRUE;
}

//=============================================================================
//	Desc	: CV PLC번호 + 트랙번호로 SC.xml HS_GROUP 엔트리 검색
//			  nType => 1:입고HS(로딩), 2:출고HS(언로딩)
//			  SC.xml <HS>의 DEL_ADDR/DEL_CNT/PROD_ADDR 속성 참조용
//=============================================================================
CTrackHS* CEcsDoc::FindHsByCvTrack(int nCvPlcNum, int nTrackNum, int nType)
{
	int nEquipCnt = m_pEquipments.GetSize();
	for (int i=0; i<nEquipCnt; ++i)
	{
		CEquipment* pEquipment = m_pEquipments[i];
		if (pEquipment == NULL || pEquipment->m_enKind != CEquipment::enSC)
			continue;

		CScInfo* pInfo = (CScInfo*)((CSc*)pEquipment)->GetInfo();
		if (pInfo == NULL)
			continue;

		CTrackHSArray* pHsArray = (nType == 1) ? &pInfo->m_pStosHS : &pInfo->m_pRetsHS;
		int nHsCnt = pHsArray->GetSize();
		for (int k=0; k<nHsCnt; ++k)
		{
			CTrackHS* pTrackHS = pHsArray->GetAt(k);
			if (pTrackHS == NULL || pTrackHS->m_pTrack == NULL)
				continue;

			if (pTrackHS->m_nCvPlcNo != nCvPlcNum)
				continue;

			// 메인 트랙(TID) 또는 PAIR 트랙 일치 확인
			if (pTrackHS->m_pTrack->m_nNumber == nTrackNum ||
				(pTrackHS->m_nPair != 0 && pTrackHS->m_nPair == nTrackNum))
				return pTrackHS;
		}
	}

	return NULL;
}

CDciControl* CEcsDoc::GetDciControl(CString& strCID, int nTabNum)	
{
	CEcsView* pView = (CEcsView*)GetViewObject();
	DEBUGER_ASSERT_VALID(pView != NULL);

	int nLayoutCnt = m_pEcsLayOuts.GetSize();
	for (int i=0;i<nLayoutCnt;i++)
	{
		CEcsLayout * pEcsLayout = m_pEcsLayOuts[i];

		if (pEcsLayout == NULL)
			continue;

		if (pEcsLayout->GetDciControl(strCID) != NULL)
			return pEcsLayout->GetDciControl(strCID);
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
