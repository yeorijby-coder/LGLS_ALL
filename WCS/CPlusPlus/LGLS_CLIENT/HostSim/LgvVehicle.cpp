// LgvVehicle.cpp: implementation of the CLgvVehicle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "LgvVehicle.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLgvVehicle::CLgvVehicle(int nVehicleID)
{
	m_nVehicleID = nVehicleID;

	m_nLuggNum			= 0;	
	m_nErrorCode		= 0;		
	m_nCurPosition		= 0;	
	m_ucVehicleMode		= 0;	
	m_ucVehicleStatus	= 0;	
//	m_ucBatteryCharge	= 0;	
	m_ucLoadStatus		= 0;	
//	m_ucActivity		= 0;
	
	m_pControl			= NULL;

	m_bInvoke			= FALSE;

	m_bModified			= FALSE;

	m_bFirstDoorCheck = TRUE;
}

CLgvVehicle::~CLgvVehicle()
{

}

//////////////////////////////////////////////////////////////////////////
void CLgvVehicle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nLuggNum;
		ar << m_nErrorCode;
		ar << m_nCurPosition;
		ar << m_ucVehicleMode;
		ar << m_ucVehicleStatus;
		ar << m_ucLoadStatus;
		ar << m_bInvoke;
	}
	else
	{
		ar >> m_nLuggNum;
		ar >> m_nErrorCode;
		ar >> m_nCurPosition;
		ar >> m_ucVehicleMode;
		ar >> m_ucVehicleStatus;
		ar >> m_ucLoadStatus;
		ar >> m_bInvoke;
	}
}

//////////////////////////////////////////////////////////////////////////

void CLgvVehicle::SetMode(BYTE ucMode)
{
	if (m_ucVehicleMode == ucMode)
		return;

	m_ucVehicleMode = ucMode;
	m_bModified = TRUE;
}

void CLgvVehicle::SetStatus(BYTE ucRun)
{
	if (m_ucVehicleStatus == ucRun)
		return;

	m_ucVehicleStatus = ucRun;
	m_bModified = TRUE;
}

void CLgvVehicle::SetProdLoad(BYTE ucContain)
{
	if (m_ucLoadStatus == ucContain)
		return;

	m_ucLoadStatus = ucContain;
	m_bModified = TRUE;
}

void CLgvVehicle::SetHorizontalPos(int nPos)
{
	if (m_nCurPosition == nPos)
		return;

	m_nCurPosition = nPos;
	m_bModified = TRUE;
}

//=====================================================================================================
//	// 경광등 관련 - 사용안함! CLgvInfo에서 체크함!
//=====================================================================================================	
void CLgvVehicle::SetLampInfo(BOOL bOn, CCvInfo* pCvInfo, int nRgvcNum)
{
	CEcsDoc* pDoc = pCvInfo->m_pEquipment->m_pDoc;
	if (pDoc == NULL)
		return;

	CCvFx* pCv = (CCvFx*)pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
	if (pCv == NULL)
		return;

	int nIndex = 0;

	switch(nRgvcNum) 
	{
	case 1:		nIndex = 6;		break;
	case 2:		nIndex = 7;		break;
	}

	CLib::SetBit(pCv->m_pInfo->m_wCurrLampStatus, nIndex, bOn);
}
//-----------------------------------------------------------------------------------------------------

void CLgvVehicle::SetErrorCode(int nECode, CCvInfo* pCvInfo, int nLgvNum)
{
//	if (m_nErrorCode == nECode)				// 항상 에러상태를 체크해서 경광등을 올려야 하므로...
//		return;

	if (nLgvNum == 2 && m_nVehicleID == 3)
		int a=0;

	// 에러가 처음 발생했을 경우
	if ((m_nErrorCode == 0) && (nECode != 0))
	{
		m_nErrorCode = nECode;
		m_bModified = TRUE;
		SetLampInfo(TRUE, pCvInfo, nLgvNum);
	}

	// 에러상태에서 다른 에러가 발생했을 경우
//	else 
	if ((m_nErrorCode != 0) && (nECode != 0) && (m_nErrorCode != nECode)) 
	{
		m_nErrorCode = nECode;
		m_bModified = TRUE;
	}

	// 정상으로 돌아왔을경우 
//	else 
	if ((m_nErrorCode != 0) && (nECode == 0))
	{
		SetLampInfo(FALSE, pCvInfo, nLgvNum);
	}
//	else
//	{
//		SetLampInfo(BOOL(nECode), pCvInfo, nLgvNum);
//	}

	// 프로그램이 처음 실행 되었을 때 
	if (m_bFirstDoorCheck == TRUE)
	{
		SetLampInfo(BOOL(nECode), pCvInfo, nLgvNum);
		m_bFirstDoorCheck = FALSE;
	}

	
	
	if (m_nErrorCode == nECode)				
		return;

	m_nErrorCode = nECode;
	m_bModified = TRUE;


//=====================================================================================================
//	// 경광등 관련
//=====================================================================================================	
//	BOOL WriteLampStatus(int nIndex, BOOL bOn) { return WriteWordValue(nIndex + 1, (WORD)0x0001 & bOn); }
	/*
	if (pCvInfo != NULL && nLgvNum != 0)
	{
		CCv* pCv = (CCv*)pCvInfo->m_pEquipment->m_pDoc->GetEquipment(CEquipment::enCV, 12);	// 12번이 경광등 임
		if (pCv == NULL)
			return;

		switch(nLgvNum) 
		{
		case 1:		pCv->WriteLampStatus(6, (m_nErrorCode!=0));		break;
		case 2:		pCv->WriteLampStatus(7, (m_nErrorCode!=0));		break;
		}
	}
	*/
//-----------------------------------------------------------------------------------------------------
}

void CLgvVehicle::SetLuggNum(int nLuggNo)
{
	if (m_nLuggNum == nLuggNo)
		return;

	m_nLuggNum = nLuggNo;
	m_bModified = TRUE;
}

//////////////////////////////////////////////////////////////////////////

CString CLgvVehicle::GetAgvString()
{
	CString strAgv;
	strAgv.Format(_T("RGV%d"), m_nVehicleID);
	return strAgv;
}

CString CLgvVehicle::GetModeString()
{
	switch (m_ucVehicleMode)
	{
	case enModeManual:		return _T("Manual");
	case enModeLocal:		return _T("Local");
	case enModeAuto:		return _T("Auto");
	}

	return _T("INVALID");
}

CString CLgvVehicle::GetVehicleStatusString()
{
	switch (m_ucVehicleStatus)
	{
	case enWait:			return _T("대기");
	case enMove:			return _T("이동");
	case enLoading:			return _T("로딩");
	case enUnloading:		return _T("언로딩");
	case enCarError:		return _T("Car이상");
	}

	return CConvert::ToString((int)m_ucVehicleStatus);
}

CString CLgvVehicle::GetLoadStatusString()
{
	switch (m_ucLoadStatus + 0x30)
	{
	case enUnload:		return _T("없음");
	case enLoad:		return _T("적재");
	}

	return _T("INVALID");
}

CString CLgvVehicle::GetErrorString()
{
	CString strErrorCode, strErrorMessage;
	strErrorCode.Format(_T("%d"), m_nErrorCode);
	strErrorMessage.Format(_T("등록되지 않은 에러코드 [CODE=%d]"), m_nErrorCode);

	TCHAR szTemp[_MAX_PATH] = {0};
	::GetPrivateProfileString(_T("RGV_ERROR_CODE"), strErrorCode, (LPCTSTR)strErrorMessage, szTemp, _MAX_PATH, ECS_CODE_FILE);
	strErrorMessage.Format(_T("%s"), szTemp);

	return strErrorMessage;
}

CString CLgvVehicle::GetXmlString()
{
	CString strXml;
	strXml.Format(_T("<RGV VID='%d' LUGG='%d' ERR='%d' POS='%d' MD='%d' VS='%d' PL='%d'/>"), 
		m_nVehicleID, m_nLuggNum, m_nErrorCode, m_nCurPosition, m_ucVehicleMode, m_ucVehicleStatus, m_ucLoadStatus	);
	return strXml;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLgvVehicles::CLgvVehicles()
{

}

CLgvVehicles::~CLgvVehicles()
{
	for (int i=0; i<m_nSize; ++i)
		delete m_pData[i];
	RemoveAll();
}

CLgvVehicle* CLgvVehicles::Find(int nVehicleID)
{
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];

		if (pLgvVehicle == NULL)
			continue;

		if (pLgvVehicle->m_nVehicleID == nVehicleID)
			return pLgvVehicle;
	}

	return NULL;
}

BOOL CLgvVehicles::IsAllRgvRestart()
{
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];

		if (pLgvVehicle == NULL)
			continue;

		if (pLgvVehicle->m_ucVehicleMode != CLgvVehicle::enModeAuto || 
			pLgvVehicle->m_ucVehicleStatus != CLgvVehicle::enMove )
			return FALSE;
	}

	return TRUE;
}

BOOL CLgvVehicles::IsAllRgvStop()
{
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];

		if (pLgvVehicle == NULL)
			continue;

		if (pLgvVehicle->m_ucVehicleMode != CLgvVehicle::enModeAuto || 
			pLgvVehicle->m_ucVehicleStatus != CLgvVehicle::enWait )
			return FALSE;
	}

	return TRUE;
}

CString CLgvVehicles::GetXmlString()
{
	CString strXml;
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];
		DEBUGER_ASSERT_VALID(pLgvVehicle != NULL);

		if (pLgvVehicle == NULL)
			continue;

		strXml += pLgvVehicle->GetXmlString();
	}

	return strXml;
}

BOOL CLgvVehicles::IsIdleJob(int nLuggNum)
{
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];

		if (pLgvVehicle == NULL)
			continue;

		if (pLgvVehicle->m_nLuggNum == nLuggNum)
			return FALSE;
	}

	return TRUE;
}

BOOL CLgvVehicles::IsErrorStatus()
{
	CLgvVehicle* pLgvVehicle = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pLgvVehicle = m_pData[i];

		if (pLgvVehicle == NULL)
			continue;

		if (pLgvVehicle->IsErrorStatus())
			return TRUE;
	}

	return FALSE;
}
