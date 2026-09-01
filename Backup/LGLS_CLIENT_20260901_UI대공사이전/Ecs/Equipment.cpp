// Equipment.cpp: implementation of the CEquipment class.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "Equipment.h"
#include "RecordSetWrap.h"

#define DEVICE m_port.m_strDevice

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CEquipment
//
IMPLEMENT_DYNAMIC(CEquipment, CObject)

CEquipment::CEquipment(CEcsDoc* pDoc, CString pstrThreadNo, int nIndex)
{
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	m_strThreadNo = pstrThreadNo;
	m_pDoc = pDoc;
	m_pRsw = NULL;
	//m_nNumber = nNumber;
	m_nIndex = nIndex;
	m_enKind = enNone;
	m_hWndInfo = NULL;
	m_pEquipDlg = NULL;
	m_pThread = NULL;
	m_hEventArray[enEventKill] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventArray[enEventSend] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_pfThreadProc = CEquipment::ThreadProc;
	m_pThread = ::AfxBeginThread(m_pfThreadProc, this);
}

CEquipment::~CEquipment()
{
	::CloseHandle(m_hEventArray[enEventSend]);
	::CloseHandle(m_hEventArray[enEventKill]);
}

CString CEquipment::GetEquipmentKey(EN_KIND pEN_KIND)
{
	CString strKey;
	strKey.Format(_T("%02d%02s"), pEN_KIND, m_strThreadNo);
	return strKey;
}

CString CEquipment::GetEquipmentKey()
{
	CString strKey;
	strKey.Format(_T("%02d%02s"), m_enKind, m_strThreadNo);
	return strKey;
}

BOOL CEquipment::IsCollect()
{
	return TRUE;
}

void CEquipment::CallAutoRunProc()
{
	AutoRunProc();
	return;
}

UINT CEquipment::ThreadProc(LPVOID pParam)
{
	CEquipment* pThis = (CEquipment*)pParam;
	DEBUGER_ASSERT_VALID(pThis != NULL);

	while (true)
	{
		switch (::WaitForMultipleObjects(enEventSize, pThis->m_hEventArray, FALSE, 500))
		{
		case enEventKill:
			return 0;

		case enEventSend:
			{
				try
				{
					pThis->AutoRunProc(); //20190522kdh
					if(pThis->m_pRsw != NULL)
					{
						delete pThis->m_pRsw;
						pThis->m_pRsw = NULL;
					}
				}
				catch(CException* e)
				{
					CString strTemp = _T("");
					
					switch(pThis->m_enKind)
					{
					case CEquipment::enCV:	strTemp.Format(_T("CV Time:%s"), COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));		AfxMessageBox(strTemp);		break;
					case CEquipment::enSC:	strTemp.Format(_T("SC Time:%s"), COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));		AfxMessageBox(strTemp);		break;
					case CEquipment::enWC:	strTemp.Format(_T("WC Time:%s"), COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S")));		AfxMessageBox(strTemp);		break;
					}
					
				}
				break;
			}
		}
		::Sleep(100); //1000
	}
	return 0;	
}

CEquipment* CEquipmentArray::GetEquipmentSC(int nEquipKind, int nEquipNum)
{
	int nEquipRealNum = nEquipNum;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		if(pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind != nEquipKind)
			continue;

		if (pEquipment->m_nEQP_NO == nEquipNum)
			return pEquipment;
	}

	return NULL;
}

CEquipment* CEquipmentArray::GetEquipmentRTV(int nEquipKind, int nEquipNum)
{
	int nEquipRealNum = nEquipNum;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		if(pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind != nEquipKind)
			continue;

		if (pEquipment->m_nEQP_NO == nEquipNum)
			return pEquipment;
	}

	return NULL;
}

CEquipment* CEquipmentArray::GetEquipment(int nEquipKind, int nEquipNum)
{
	int nEquipRealNum = nEquipNum;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		if(pEquipment == NULL)
			continue;

		if (pEquipment->m_enKind != nEquipKind)
			continue;

		if (pEquipment->m_nNumber == nEquipNum)
			return pEquipment;
	}

	return NULL;
}


CEquipment* CEquipmentArray::IsExist(CEquipment::EN_KIND pEqpKind)
{
	BOOL bReturn = TRUE;
	for(int nIdxEqp = 0; nIdxEqp < GetCount(); nIdxEqp++)
	{
		CEquipment* pEqpTmp = (CEquipment*)GetAt(nIdxEqp);
		if(pEqpTmp != NULL &&  
			pEqpTmp->m_enKind == pEqpKind)
		{
			return pEqpTmp;
		}
	}
	return NULL;
}

void CEquipmentArray::InvokeControl(BOOL bManual)
{
	CInfo* pInfo = NULL;
	CEquipment* pEquipment = NULL;
	for (int i=0; i<m_nSize; ++i)
	{
		pEquipment = m_pData[i];
		DEBUGER_ASSERT_VALID(pEquipment != NULL);

		pInfo = pEquipment->GetInfo();
		DEBUGER_ASSERT_VALID(pInfo != NULL);

		if (bManual)
			pInfo->m_bModified = TRUE;

		if (pEquipment->m_enKind == CEquipment::enCV)
			pInfo->m_bManualModified = TRUE;

		//pInfo->InvokeControl();
		//pEquipment->CallAutoRunProc();
	}
}



