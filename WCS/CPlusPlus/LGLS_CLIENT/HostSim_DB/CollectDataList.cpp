#include "StdAfx.h"
#include "stdafx.h"
#include "CollectDataList.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "RecordSetWrap.h"
#include "CellInfo.h"
#include "JOB_MST.h"
#include "ViewUsageRackDlg.h"

#define CRLF _T("\n")


CCollectDataList::CCollectDataList(void)
{
}

CCollectDataList::CCollectDataList(CEcsDoc* pDoc)
{
	m_bThreadDoWork = FALSE;
	m_pDoc = pDoc;
	m_pThread = NULL;
	m_pDB_ACCESS = NULL;
}


CCollectDataList::~CCollectDataList(void)
{
	if(m_pDB_ACCESS != NULL){ delete m_pDB_ACCESS; m_pDB_ACCESS = NULL;}
	m_pDB_ACCESS = NULL;
}

BOOL CCollectDataList::IsDB_POSSIBLE()
{
// 	if(m_pDB == NULL)
// 	{
// 		m_pDB = NULL;
// 		m_pDB_ACCESS = NULL;
// 		m_pDB = new CAdoDB(m_pDoc);
// 		m_pDB_ACCESS->m_pAdoDB = new CURMDBAccess(m_pDoc, m_pDB);
// 		return m_pDB->ConnectDB();
// 	}
// 
// 	if(m_pDB->m_bConnected == FALSE)
// 	{
// 		m_pDB = NULL;
// 		m_pDB_ACCESS = NULL;
// 		m_pDB = new CAdoDB(m_pDoc);
// 		m_pDB_ACCESS = new CURMDBAccess(m_pDoc, m_pDB);
// 		return m_pDB->ConnectDB();
// 	}
// 	return m_pDB->m_bConnected;
	return TRUE;
}


BOOL CCollectDataList::StartDoWork()
{
	if(m_bThreadDoWork == TRUE)
	{
		return FALSE;
	}


	m_bThreadDoWork = TRUE;
	m_pThread = ::AfxBeginThread(DoWork, (LPVOID)this);
	if(m_pThread == NULL)
	{
		m_bThreadDoWork = FALSE;
		return FALSE;
	}

	return IsAllive();
}


BOOL CCollectDataList::IsAllive()
{
	return m_bThreadDoWork;
}

BOOL CCollectDataList::StopDoWork()
{
	m_bThreadDoWork = FALSE;
	::WaitForSingleObject(m_pThread, INFINITE);
	return TRUE;
}

UINT CCollectDataList::DoWork(LPVOID pParm)
{
	CCollectDataList* pThis = (CCollectDataList*)pParm;
	while(pThis->m_bThreadDoWork)
	{
		CEcsDoc* pDoc = pThis->m_pDoc;
		if(pDoc == NULL)
		{
			::Sleep(1000);
			continue;
		}

		if(pDoc->m_bExit == true)
			pThis->StopDoWork();

		if(pThis->IsDB_POSSIBLE() == FALSE)
		{
			::Sleep(1000);
			continue;
		}

		if(pThis->IsCollectCellInfo())
		{
			pThis->CallBackCellInfo();
		}

// 		if(pThis->IsCollectJOB_MST())
// 		{
// 			pThis->CallBackJOB_MST();
// 		}
		::Sleep(500); //1000
	}
	return 0;
}

bool CCollectDataList::IsCollectJOB_MST()
{
	if(m_pDoc == NULL){ return FALSE; }
	return m_pDoc->m_CollectJOB_MST.m_bRequest;
}

bool CCollectDataList::IsCollectCellInfo()
{
	if(m_pDoc == NULL){ return FALSE; }
	return m_pDoc->m_CollectCellInfo.m_bRequest;
}

void CCollectDataList::CallBackJOB_MST()
{
	if(m_pDoc == NULL) { return; };

	CString strSql = GetSelQuery_JOB_MST();
	if(strSql == _T("")){ return; }

	CString strMessage = _T("");
	int nRowCnt = -1;
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
 	CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMapJOB_MST = CreateJOB_MST(pRsw, nRowCnt);

	while(m_pDoc->m_pJob->ChgJobCollection(pMapJOB_MST) == FALSE)
	{
		::Sleep(10);
	}
	if(m_pDoc->m_pViewJobListDlg != NULL)
	{
		//::PostMessage(m_pDoc->m_pViewJobListDlg->m_hWnd, WM_USER_REFRESH_COLLECT_JOBINFO, (WPARAM)0, (LPARAM)0);	
	}
	delete pRsw;
}

void CCollectDataList::CallBackCellInfo()
{
	if(m_pDoc == NULL) { return; };
	if(IsWindow(m_pDoc->m_hWndViewRackDlg) == FALSE) { return; }

	CString strSql = GetSelQuery_CELL_MST();
	if(strSql == _T("")){ return; }

	CString strMessage = _T("");
	int nRowCnt = -1;
	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	if(nRowCnt <= 0){	return;	  }
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);
	CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>* pMapCellInfo = CreateCellInfo(pRsw, nRowCnt);
	delete pRsw;
	
	BOOL isSuccess = ::PostMessage(m_pDoc->m_pViewUsageRackDlg->m_hWnd, WM_USER_REFRESH_COLLECT_CELLINFO, (WPARAM)pMapCellInfo, (LPARAM)0);
	if(isSuccess == false)
	{
		return;
	}

	m_pDoc->m_CollectCellInfo.m_bRequest = FALSE;
}

CString CCollectDataList::GetSelQuery_CELL_MST()
{
	CString strSql = _T("");

	CString strWH_TYP = m_pDoc->m_CollectCellInfo.m_strWH_TYP;
	CString strAGING_TYP = m_pDoc->m_CollectCellInfo.m_strAGING_TYP;
 	CString strBANK = m_pDoc->m_CollectCellInfo.m_strBANK;

	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(CM.WH_TYP,			 '10')	as 	WH_TYP_M				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_SC_NO,				  '00000')	as 	CELL_SC_NO_M			\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_NO,				  '00-000-00')	as 	CELL_NO_M 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.BANK,					     '00')	as 	BANK_M 					\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.BAY,							 '00')	as 	BAY_M					\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.LEV,							  '0')	as 	LEV_M					\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.GDBOX_NO,				      '0') 	as 	GDBOX_NO_M				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.GDBOX_RACD,					  '0')	as 	GDBOX_RACD_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.FIRE_DETECTOR_NO,		     '0')	as 	FIRE_DETECTOR_NO_M		\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_FIRE_YN,			 	 'N')	as 	CELL_FIRE_YN_M			\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_STA,				 	 '0')	as 	CELL_STA_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_USE_DEF,			 	 '0')	as 	CELL_USE_DEF_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_USE_YN,				     'N')	as 	CELL_USE_YN_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.AGING_TYP,				 '00000')	as 	AGING_TYP_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_TYP,				 	 '0')	as 	CELL_TYP_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.BOTTOM_TRAY,				     'T')	as 	BOTTOM_TRAY_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.TOP_TRAY,				 	 'T')	as 	TOP_TRAY_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.UPD_DT ,					 ") + m_pDoc->SYSDATE + _T(")	as 	UPD_DT_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.USER_REMARKS,		   'NOTHING')	as 	USER_REMARKS_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.REMARKS,				   'NOTHING')	as 	REMARKS_M				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CM.CELL_STATUS,				     '0')	as 	CELL_STATUS_M			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.WH_TYP,					 '0')	as 	WH_TYP					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.CELL_NO,			 '00-000-00')	as 	CELL_NO					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.BANK,				        '00')	as 	BANK					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.BAY,				       '000')	as 	BAY						\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.LEV,					    '00')	as 	LEV						\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.AGING_START_DT,		 sysdate)	as 	AGING_START_DT			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.AGING_END_DT ,			 ") +m_pDoc->SYSDATE+ _T(")	as 	AGING_END_DT			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.USER_REMARKS,		   'NOTHING')	as 	USER_REMARKS			\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.REMARKS,			   'NOTHING')	as 	REMARKS					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.INS_DT,			     ") + m_pDoc->SYSDATE + _T(")	as 	INS_DT					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.INS_USER_ID ,		     'GUEST')	as 	INS_USER_ID				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.UPD_DT,			     ") + m_pDoc->SYSDATE + _T(")	as 	UPD_DT					\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.UPD_USER_ID,		     'GUEST')	as 	UPD_USER_ID				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.BOTTOM_TRAY,			 	 'T')	as 	BOTTOM_TRAY				\n ")	
				  _T("      , ") + m_pDoc->NVL + _T("(CDTL.TOP_TRAY,			  	     'T')	as 	TOP_TRAY				\n ")	
				  _T("   FROM CELL_MST CM LEFT OUTER JOIN CELL_DTL CDTL \n")
				  _T("                          ON CM.WH_TYP = '%s'		\n")
				  _T("                         AND CM.AGING_TYP like '%s__'  \n")
				  _T("                         AND CM.BANK = '%s'		\n")
				  _T("                         AND CM.WH_TYP = CDTL.WH_TYP	 \n")
				  _T("                         AND CM.CELL_NO = CDTL.CELL_NO \n")
				  _T("                         AND CM.CELL_SC_NO = CDTL.CELL_SC_NO \n")
				  _T(" WHERE CM.WH_TYP = '%s' \n")
				  _T("   AND CM.AGING_TYP like '%s__' \n")
				  _T("   AND CM.BANK = '%s' \n")
	, strWH_TYP, strAGING_TYP.Mid(0, 3), strBANK, strWH_TYP, strAGING_TYP.Mid(0,3), strBANK);
	
	return strSql;
}

CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>* CCollectDataList::CreateCellInfo( CRecordSetWrap* pRsw, int nRowCnt )
{
	CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>* pMapCellInfo = new CMap<CString, LPCTSTR, CCellInfo*, CCellInfo*>();
	pMapCellInfo->InitHashTable(nRowCnt);
	CString strWH_TYP, strUPD_DT, strCELL_SC_NO, strCELL_NO, strBANK; //5
	CString strBAY, strLEV, strGDBOX_NO, strGDBOX_RACD, strFIRE_DETECTOR_NO;  //5
	CString strCELL_FIRE_YN, strCELL_STA, strCELL_USE_DEF, strCELL_USE_YN, strAGING_TYP; //5     
	CString strCELL_TYP, strBOTTOM_TRAY, strTOP_TRAY, strREMARKS, strCELL_STATUS, strUSER_REMARKS; //7

	CString str2WH_TYP, str2CELL_NO, str2BANK, str2BAY, str2LEV;         
	CString str2INS_USER_ID, str2UPD_USER_ID, str2BOTTOM_TRAY, str2TOP_TRAY;
	CString str2USER_REMARKS, str2REMARKS;
	CString str2UPD_DT, str2AGING_START_DT, str2AGING_END_DT, str2INS_DT;
	
	pRsw->MoveFirst(); 
	for(int nIdxRow = 0; nIdxRow < nRowCnt; nIdxRow++)
	{
		strWH_TYP = pRsw->GetItem(_T("WH_TYP_M"));
		strCELL_SC_NO = pRsw->GetItem(_T("CELL_SC_NO_M")) ;
		strCELL_NO = pRsw->GetItem(_T("CELL_NO_M"));
		strBANK = pRsw->GetItem(_T("BANK_M"));
		strBAY = pRsw->GetItem(_T("BAY_M"));
		strLEV = pRsw->GetItem(_T("LEV_M"));
		strGDBOX_NO = pRsw->GetItem(_T("GDBOX_NO_M"));
		strGDBOX_RACD = pRsw->GetItem(_T("GDBOX_RACD_M"));
		strFIRE_DETECTOR_NO = pRsw->GetItem(_T("FIRE_DETECTOR_NO_M"));
		strCELL_FIRE_YN = pRsw->GetItem(_T("CELL_FIRE_YN_M"));
		strCELL_STA = pRsw->GetItem(_T("CELL_STA_M"));
		strCELL_USE_DEF = pRsw->GetItem(_T("CELL_USE_DEF_M"));
		strCELL_USE_YN = pRsw->GetItem(_T("CELL_USE_YN_M"));
		strAGING_TYP = pRsw->GetItem(_T("AGING_TYP_M"));
		strCELL_TYP = pRsw->GetItem(_T("CELL_TYP_M"));
		strBOTTOM_TRAY = pRsw->GetItem(_T("BOTTOM_TRAY_M"));
		strTOP_TRAY = pRsw->GetItem(_T("TOP_TRAY_M"));
		strUPD_DT = pRsw->GetItem(_T("UPD_DT_M"));
		strUSER_REMARKS = pRsw->GetItem(_T("USER_REMARKS_M"));
		strREMARKS = pRsw->GetItem(_T("REMARKS_M"));
		strCELL_STATUS = pRsw->GetItem(_T("CELL_STATUS_M"));

		str2WH_TYP  = pRsw->GetItem(_T("WH_TYP"));      
		str2CELL_NO = pRsw->GetItem(_T("CELL_NO"));      
		str2BANK = pRsw->GetItem(_T("BANK"));     
		str2BAY = pRsw->GetItem(_T("BAY"));     
		str2LEV = pRsw->GetItem(_T("LEV"));
		str2AGING_START_DT = pRsw->GetItem(_T("AGING_START_DT"));
		str2AGING_END_DT = pRsw->GetItem(_T("AGING_END_DT"));
		str2USER_REMARKS = pRsw->GetItem(_T("USER_REMARKS"));
		str2REMARKS = pRsw->GetItem(_T("REMARKS"));
		str2INS_DT = pRsw->GetItem(_T("INS_DT"));
		str2INS_USER_ID = pRsw->GetItem(_T("INS_USER_ID"));
		str2UPD_DT = pRsw->GetItem(_T("UPD_DT"));
		str2UPD_USER_ID = pRsw->GetItem(_T("UPD_USER_ID"));
		str2BOTTOM_TRAY = pRsw->GetItem(_T("BOTTOM_TRAY"));
		str2TOP_TRAY = pRsw->GetItem(_T("TOP_TRAY"));

		CString strKey = strCELL_SC_NO.Mid(0, 3) + CConvert::ToString(nIdxRow+1);
		CCellInfo* pCellInfo = NULL;
		if(pMapCellInfo->Lookup(strKey, pCellInfo) == FALSE){ pCellInfo = new CCellInfo(); }
		pCellInfo->WH_TYP			= strWH_TYP;					
		pCellInfo->CELL_SC_NO     	= strCELL_SC_NO;
		pCellInfo->CELL_NO    		= strCELL_NO;  
		pCellInfo->BANK				= strBANK;
		pCellInfo->BAY				= strBAY;
		pCellInfo->LEV				= strLEV;
		pCellInfo->GDBOX_NO			= strGDBOX_NO;
		pCellInfo->GDBOX_RACD		= strGDBOX_RACD;
		pCellInfo->FIRE_DETECTOR_NO	= strFIRE_DETECTOR_NO;
		pCellInfo->CELL_FIRE_YN		= strCELL_FIRE_YN;
		pCellInfo->CELL_STA			= strCELL_STA;
		pCellInfo->CELL_USE_DEF		= strCELL_USE_DEF;
		pCellInfo->CELL_USE_YN		= strCELL_USE_YN;
		pCellInfo->AGING_TYP		= strAGING_TYP;
		pCellInfo->CELL_TYP			= strCELL_TYP;
		pCellInfo->BOTTOM_TRAY		= strBOTTOM_TRAY;
		pCellInfo->TOP_TRAY			= strTOP_TRAY;
		pCellInfo->UPD_DT			= strUPD_DT;
		pCellInfo->USER_REMARKS		= strUSER_REMARKS;
		pCellInfo->REMARKS			= strREMARKS;
		pCellInfo->CELL_STATUS		= strCELL_STATUS;

		CCELL_DTL* pCELL_DTL = new CCELL_DTL();
		pCELL_DTL->WH_TYP			= str2WH_TYP;		
		pCELL_DTL->CELL_NO          = str2CELL_NO;
		pCELL_DTL->BANK				= str2BANK;
		pCELL_DTL->BAY				= str2BAY;
		pCELL_DTL->LEV				= str2LEV;
		pCELL_DTL->AGING_START_DT	= str2AGING_START_DT;
		pCELL_DTL->AGING_END_DT  	= str2AGING_END_DT;
		pCELL_DTL->USER_REMARKS  	= str2USER_REMARKS;
		pCELL_DTL->REMARKS      	= str2REMARKS;
		pCELL_DTL->INS_DT         	= str2INS_DT;
		pCELL_DTL->INS_USER_ID   	= str2INS_USER_ID;
		pCELL_DTL->UPD_DT      		= str2UPD_DT;
		pCELL_DTL->UPD_USER_ID 		= str2UPD_USER_ID;
		pCELL_DTL->BOTTOM_TRAY  	= str2BOTTOM_TRAY;
		pCELL_DTL->TOP_TRAY      	= str2TOP_TRAY;

		pMapCellInfo->SetAt(strKey, pCellInfo);
		pCellInfo->m_pCELL_DTL.AddTail(pCELL_DTL);
		pRsw->MoveNext(); 
	}

	return pMapCellInfo;
}

CString CCollectDataList::GetSelQuery_JOB_MST()
{
	CString strSql = _T("");
	CString strWH_TYP = m_pDoc->m_CollectJOB_MST.m_strWH_TYP;

	strSql.Format(_T(" SELECT ") + m_pDoc->NVL + _T("(WH_TYP,						 '10')	as 	WH_TYP					\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(LUGG_NO,						  '0')	as 	LUGG_NO				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(START_POS,					  '0')	as 	START_POS 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(START_LOCATION,		  '00-000-00')	as 	START_LOCATION 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(DEST_POS,						  '0')	as 	DEST_POS 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(DEST_LOCATION,		  '00-000-00')	as 	DEST_LOCATION 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(TURN,						      '0')	as 	TURN 				   \n ")	   
				  _T("      , ") + m_pDoc->NVL + _T("(JOB_TYP,						 '99')	as 	JOB_TYP 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(BCR_TOP,						  '0')	as 	BCR_TOP 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(BCR_BOTTOM,					  '0')	as 	BCR_BOTTOM 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(JOB_KIND,						 '99')	as 	JOB_KIND 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(JOB_STATUS,					 '99')	as 	JOB_STATUS 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(JOB_PRIORITY,					  '0')	as 	JOB_PRIORITY 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(MES_ERROR_CD,					  '0')	as 	MES_ERROR_CD 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(TRAY_TYP,						  '0')	as 	TRAY_TYP 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(TRAY_LEV,						  '0')	as 	TRAY_LEV 				\n ")	 
				  _T("      , ") + m_pDoc->NVL + _T("(DURATION_TIME,				  ") + m_pDoc->SYSDATE + _T(")	as 	DURATION_TIME 				\n ")	 
				  _T("   FROM JOB_MST \n")
				  _T("  WHERE WH_TYP = '%s' \n")
	, m_pDoc->m_CollectJOB_MST.m_strWH_TYP);
	

	return strSql;
}

CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* CCollectDataList::CreateJOB_MST( CRecordSetWrap* pRsw, int nRowCnt )
{
	CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>* pMapJOB_MST = new CMap<CString, LPCTSTR, CJOB_MST*, CJOB_MST*>();
	pMapJOB_MST->InitHashTable(nRowCnt);
	CString strWH_TYP, strLUGG_NO, strSTART_POS, strSTART_LOCATION, strDEST_POS;
	CString strDEST_LOCATION, strTURN, strJOB_TYP, strBCR_TOP, strBCR_BOTTOM;
	CString strJOB_KIND, strJOB_STATUS, strJOB_PRIORITY, strMES_ERROR_CD, strTRAY_TYP;
	CString strTRAY_LEV, strDURATION_TIME;

	pRsw->MoveFirst(); 
	for(int nIdxRow = 0; nIdxRow < nRowCnt; nIdxRow++)
	{
		strWH_TYP = pRsw->GetItem(_T("WH_TYP"));
		strLUGG_NO = pRsw->GetItem(_T("LUGG_NO"));
		if(strLUGG_NO == _T("9312"))
		{
			int a = 0;
		}
		strSTART_POS = pRsw->GetItem(_T("START_POS"));
		strSTART_LOCATION = pRsw->GetItem(_T("START_LOCATION"));
		strDEST_POS = pRsw->GetItem(_T("DEST_POS"));
		strDEST_LOCATION = pRsw->GetItem(_T("DEST_LOCATION"));
		strTURN = pRsw->GetItem(_T("TURN"));
		strJOB_TYP = pRsw->GetItem(_T("JOB_TYP"));
		strBCR_TOP = pRsw->GetItem(_T("BCR_TOP"));
		strBCR_BOTTOM = pRsw->GetItem(_T("BCR_BOTTOM"));
		strJOB_KIND = pRsw->GetItem(_T("JOB_KIND"));
		strJOB_STATUS = pRsw->GetItem(_T("JOB_STATUS"));
		strJOB_PRIORITY = pRsw->GetItem(_T("JOB_PRIORITY"));
		strMES_ERROR_CD = pRsw->GetItem(_T("MES_ERROR_CD"));
		strTRAY_TYP = pRsw->GetItem(_T("TRAY_TYP"));
		strTRAY_LEV = pRsw->GetItem(_T("TRAY_LEV"));
		strDURATION_TIME = pRsw->GetItem(_T("DURATION_TIME"));

		CJOB_MST* pJOB_MST = NULL;
		if(pMapJOB_MST->Lookup(strLUGG_NO, pJOB_MST) == FALSE){ pJOB_MST = new CJOB_MST(); }
		pJOB_MST->WH_TYP			= strWH_TYP;
		pJOB_MST->LUGG_NO			= strLUGG_NO;
		pJOB_MST->START_POS			 = strSTART_POS;
		pJOB_MST->START_LOCATION	 = strSTART_LOCATION;
		pJOB_MST->DEST_POS			 = strDEST_POS;
		pJOB_MST->DEST_LOCATION		 = strDEST_LOCATION;
		pJOB_MST->TURN				 = strTURN;
		pJOB_MST->JOB_TYP			 = strJOB_TYP;
		pJOB_MST->BCR_TOP			 = strBCR_TOP;
		pJOB_MST->BCR_BOTTOM		 = strBCR_BOTTOM;
		pJOB_MST->JOB_KIND			 = strJOB_KIND;
		pJOB_MST->JOB_STATUS		 = strJOB_STATUS;
		pJOB_MST->JOB_PRIORITY		 = strJOB_PRIORITY;
		pJOB_MST->MES_ERROR_CD		 = strMES_ERROR_CD;
		pJOB_MST->TRAY_TYP			 = strTRAY_TYP;
		pJOB_MST->TRAY_LEV			 = strTRAY_LEV;
		pJOB_MST->DURATION_TIME		 = strDURATION_TIME;

		pMapJOB_MST->SetAt(pJOB_MST->LUGG_NO, pJOB_MST);
		pRsw->MoveNext(); 
	}

	return pMapJOB_MST;
}




