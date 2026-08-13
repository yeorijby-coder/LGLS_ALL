// DeviceMap.cpp: implementation of the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "DeviceMap.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define MYTRACE

void CTrackProperty::InitProperty()
{
	m_strAddr = _T("");
	m_strType = _T("");
	m_nInOrder = 1;
	//m_bParent = TRUE;
	m_strParent = _T("");
	m_strName = _T("");
	m_bSeparatelyETC = FALSE;
	m_bKeywordETC = FALSE;
	m_nTrackNo = 0;
	m_bEtcInfo.RemoveAll();
}

void CTrackProperty::SetProperty(CString strType, CString strName, CString strAddr, int nTrackNo, int nInOrder)
{
	m_strAddr = strAddr;
	m_strType = strType;
	m_nInOrder = nInOrder;

	m_strName = strName;

	m_nTrackNo = nTrackNo;
}

CTrackProperty* CTrackPropertyArray::GetTrackProperty(CString strKeyword, int nTrackNo, BOOL bParent)
{
	// 한국단자에서는 트랙번호를 101이런식으로 가져갔기 때문에 트랙번호가 101이런식으로 와야한다. 
	CTrackProperty* pTrackProperty = NULL;
	for (int i = 0; i < this->m_nSize; ++i)
	{
		pTrackProperty = this->m_pData[i];

		if (pTrackProperty == NULL)
			continue;

		if (pTrackProperty->m_bSeparatelyETC == TRUE)
		{
			if (bParent == TRUE)
			{
				if (pTrackProperty->m_strParent == strKeyword && pTrackProperty->m_nTrackNo == nTrackNo)
					return pTrackProperty;
			}
			else
			{
				if (pTrackProperty->m_strName == strKeyword && pTrackProperty->m_nTrackNo == nTrackNo)
					return pTrackProperty;
			}
		}
		else
		{
			if (bParent == TRUE)
			{
				if (pTrackProperty->m_strParent == strKeyword)
					return pTrackProperty;
			}
			else
			{
				if (pTrackProperty->m_strName == strKeyword)
					return pTrackProperty;
			}
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////

void CFlantProperty::InitProperty()
{
	m_strAddr = _T("");
	m_strType = _T("");
	m_nInOrder = 1;
	//m_bParent = TRUE;
	m_strParent = _T("");
	m_strName = _T("");
	m_bSeparatelyETC = FALSE;
	m_bKeywordETC = FALSE;
	m_nTrackNo = 0;
	m_bEtcInfo.RemoveAll();
}

void CFlantProperty::SetProperty(CString strType, CString strName, CString strAddr, int nTrackNo, int nInOrder)
{
	m_strAddr = strAddr;
	m_strType = strType;
	m_nInOrder = nInOrder;

	m_strName = strName;

	m_nTrackNo = nTrackNo;
}

CFlantProperty* CFlantPropertyArray::GetFlantProperty(CString strKeyword, int nTrackNo, BOOL bParent)
{
	CFlantProperty* pFlantProperty = NULL;
	for (int i = 0; i < this->m_nSize; ++i)
	{
		pFlantProperty = this->m_pData[i];

		if (pFlantProperty == NULL)
			continue;

		if (pFlantProperty->m_bSeparatelyETC == TRUE)
		{
			if (bParent == TRUE)
			{
				if (pFlantProperty->m_strParent == strKeyword && pFlantProperty->m_nTrackNo == nTrackNo)
					return pFlantProperty;
			}
			else
			{
				if (pFlantProperty->m_strName == strKeyword && pFlantProperty->m_nTrackNo == nTrackNo)
					return pFlantProperty;
			}
		}
		else
		{
			if (bParent == TRUE)
			{
				if (pFlantProperty->m_strParent == strKeyword)
					return pFlantProperty;
			}
			else
			{
				if (pFlantProperty->m_strName == strKeyword)
					return pFlantProperty;
			}
		}
	}

	return NULL;
}


/*
CDestination* CDestinationArray::GetDestinationByTrack(int nTrack)
{
	CDestination* pDestination = NULL;
	for (int i = 0; i < m_nSize; ++i)
	{
		pDestination = &m_pData[i];

		if (pDestination == NULL)
			continue;

		if (pDestination->m_nTrackNo == nTrack)
			return pDestination;
	}

	return NULL;
}

CDestination* CDestinationArray::GetDestinationByDest(int nDest)
{
	CDestination* pDestination = NULL;
	for (int i = 0; i < m_nSize; ++i)
	{
		pDestination = &m_pData[i];

		if (pDestination == NULL)
			continue;

		if (pDestination->m_nPos == nDest)
			return pDestination;
	}

	return NULL;
}
//*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeviceMap::CDeviceMap(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	m_bUseDestTable = FALSE;
	m_bUseSeparatelyETC = FALSE;
}

CDeviceMap::~CDeviceMap()
{
}
/*
BOOL CDeviceMap::GetUseByTrack(int nTrack)
{
	CDestination* pDestination = m_oDestTable.GetDestinationByTrack(nTrack);

	if (pDestination == NULL)
		return FALSE;

	// 해당 Pos 값이 존재하면 그 테이블에 정의된 사용유무를 반환하면 됨???
	return m_oDestTable.m_bUse;
}
//*/
BOOL CDeviceMap::ParseXml(CString strFilePath, int nId)
{
	CString strTemp, strLog, strFunction = __FUNCTION__;

#pragma region XmlDom 초기화 & Loading
	if (!CXmlDom::InitializeXmlDom())
	{
		strLog.Format(_T("%s ... XML 문서 초기화에 실패하였습니다. "), strFunction);
		AfxMessageBox(strLog);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
		return FALSE;
	}
	if (!CXmlDom::LoadXmlFile(strFilePath))
	{
		strLog.Format(_T("%s ... XML 문서 로딩에 실패하였습니다. "), strFunction);
		AfxMessageBox(strLog);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
		return FALSE;
	}
#pragma endregion 

	CString strClassName, strDevice, strCID, strValue, strForkType, strThreadNo, strTagItem;
	MoveRoot();

#pragma region 기타 디바이스 맵 정보 확인
	strTagItem = _T("./EquipType");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/EquipType
	{
		GetElmtValue(strValue);
		if (strValue != _T("CV"))
		{
			strLog.Format(_T("%s ... CV 시뮬레이터 입니다. \n %s이 CV가 아닐수는 없습니다. "), strFunction, strTagItem);
			AfxMessageBox(strLog);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
			return FALSE;
		}
		MoveParent();
	}

	strTagItem = _T("./CommMethod");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/CommMethod
	{
		GetElmtValue(strValue);
		int nCommMethod = _ttoi(strValue);

		switch (nCommMethod)
		{
		case enCommMethodTCP:
			m_enCommMethod = enCommMethodTCP;
			break;
		case enCommMethodUDP:
		case enCommMethodSerial:
		case enCommMethodXCom:
		case enCommMethodRFC:
		case enCommMethodNone:
			strLog.Format(_T("%s ... 현재 구성되지 않은 %s입니다.. \n 추후 구성해 나가겠습니다."), strFunction, strTagItem);
			AfxMessageBox(strLog);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
			return FALSE;		// 현재는 구성되지 않았음! 추후 구성해나갈 예정 
		}
		MoveParent();
	}

	strTagItem = _T("./CPU");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/CPU
	{
		GetElmtValue(strValue);
		int nCpuKind = _ttoi(strValue);

		switch (nCpuKind)
		{
		case enCpuKindMelsec:
			m_enCpuKind = enCpuKindMelsec;
			break;
		case enCpuKindAllenBradley:
		case enCpuKindSimens:
		case enCpuKindXgt:
		case enCpuKindFara:
		case enCpuKindNone:
			strLog.Format(_T("%s ... 현재 구성되지 않은 %s입니다.. \n 추후 구성해 나가겠습니다."), strFunction, strTagItem);
			AfxMessageBox(strLog);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
			return FALSE;		// 현재는 구성되지 않았음! 추후 구성해나갈 예정 
		}
		MoveParent();
	}

	strTagItem = _T("./AddressArea");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/AddressArea
	{
		GetElmtValue(strValue);
		int nAddressArea = _ttoi(strValue);

		switch (nAddressArea)
		{
		case enAddressAreaD:
			m_enAddressArea = enAddressAreaD;
			break;
		case enAddressAreaR:
		case enAddressAreaNone:
			strLog.Format(_T("%s ... 현재 구성되지 않은 %s입니다.. \n 추후 구성해 나가겠습니다."), strFunction, strTagItem);
			AfxMessageBox(strLog);
			m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
			return FALSE;		// 현재는 구성되지 않았음! 추후 구성해나갈 예정 
		}
		MoveParent();
	}
	/*
	strTagItem = _T("./UseDestTable");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/UseDestTable
	{
		GetAttrValue(_T("use"), strValue);
		m_oDestTable.m_bUse = _ttoi(strValue);

		if (m_oDestTable.m_bUse == 1)
		{
			strTagItem = _T("./DestTable");
			if (MoveXPath(strTagItem, FALSE))	//DeviceMap/UseDestTable/DestTable
			{
				// 실제 목적지를 체크할 키워드를 테이블에서 정의하여
				// 추후 목적지를 체크할때 해당 키워드가 왔고,
				// m_bUse가  TRUE 일때만 목적지 체크할 것!
				GetAttrValue(_T("keyword"), strValue);		m_oDestTable.m_strKeyWord = strValue;

				GetAttrValue(_T("plc"), strValue);			m_nPlc = _ttoi(strValue);

				int i, j, nIndex, nElmtCount = GetChildElmtCount();
				m_oDestTable.SetSize(nElmtCount);
				for (j = 0, nIndex = 0; j < nElmtCount; j++)
				{
					MoveChild(j);	//DeviceMap/UseDestTable/DestTable/Dest

					GetAttrValue(_T("pos"), strValue);		m_oDestTable[j].m_nPos = _ttoi(strValue);
					GetAttrValue(_T("tid"), strValue);		m_oDestTable[j].m_nTrackNo = _ttoi(strValue);
					GetAttrValue(_T("IsSc"), strValue);		m_oDestTable[j].m_bIsScDest = _ttoi(strValue);

					MoveParent();	//DeviceMap/UseDestTable/DestTable
				}
				MoveParent();
			}
		}
		MoveParent();
	}
	//*/

	strTagItem = _T("./UseScDest");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/UseScDest
	{
		GetAttrValue(_T("use"), strValue);

		if (strValue != _T("1"))
		{
			m_bUseScDest = FALSE;
			m_pDoc->m_nScDestFrom = 0;
			m_pDoc->m_nScDestTo = 0;
		}
		else
		{
			m_bUseScDest = TRUE;

			GetAttrValue(_T("from"), strValue);		m_pDoc->m_nScDestFrom = _ttoi(strValue);
			GetAttrValue(_T("to"), strValue);		m_pDoc->m_nScDestTo = _ttoi(strValue);

			GetAttrValue(_T("list"), strValue);		m_pDoc->m_strDestList = strValue;
		}
		MoveParent();
	}

#pragma endregion
#pragma region Tracking Data 관련 정보 설정
	CTrackProperty* pTrackProperty = NULL;
	CFlantProperty* pFlantProperty = NULL;
	int nPropIndex = 0;
	m_bUseSeparatelyETC = FALSE;
	CString strRealName;
	strTagItem = _T("./TrackDatas");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/TrackDatas
	{
		GetAttrValue(_T("Count"), strValue);
		int nCount = _ttoi(strValue);			// 트랙갯수만큼 Loop 해야 할것인지??

		int i, j, nIndex, nElmtCount = GetChildElmtCount();		// 여기서 하위에 엘리먼트가 없으면 안됨!
		CString strType, strAddr, strName, strElName, strTempTemp;
		int nKeywordCount = 0;

		m_pDoc->m_nWordCnt = nElmtCount;

		m_pDoc->m_n1TrackKeyWordCnt = 0;// nElmtCount;
		//m_pDoc->m_n1TrackWord = nElmtCount;

		CTrackProperty oTrackProperty;

		for (j = 0, nIndex = 0; j < nElmtCount; j++)
		{
			MoveChild(j);	//DeviceMap/TrackDatas/Word

			nPropIndex = 0;

			GetAttrValue(_T("type"), strType);
			GetAttrValue(_T("addr"), strAddr);
			GetAttrValue(_T("name"), strName);

#pragma region 각 항목 Validation Check
			if (strAddr == _T(""))
			{
				strLog.Format(_T("%s ... Address 영역이 비어있습니다.[index=%d]\nAddress 영역이 비어있을수는 없습니다."), strFunction, j);
				AfxMessageBox(strLog);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
				MoveParent();	//DeviceMap/TrackDatas
				return FALSE;
			}
			int nPropIndex = 0;

			WORD wTemp = 0x0000;
			int nTempCount = 0;
			if (strType == _T("W"))
			{
#pragma region Type이 워드일때 
				nTempCount = GetChildElmtCount();
				if (nTempCount != 0)
				{
					strLog.Format(_T("%s ... Word Type이 하위 엘리먼트가 있을수는 없습니다..[index=%d]"), strFunction, j);
					AfxMessageBox(strLog);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
					MoveParent();	//DeviceMap/TrackDatas
					return FALSE;
				}
				nIndex = _ttoi(strAddr.Right(1));

				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				pTrackProperty->SetProperty(strType, strName, strAddr);

				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
#pragma endregion
			}
			else if (strType == _T("B"))
			{
#pragma region Type이 바이트일때 
				nTempCount = GetChildElmtCount();
				if (nTempCount == 0)
				{
					strLog.Format(_T("%s ... Byte Type이 하위 엘리먼트가 없으면 안됩니다...[index=%d]"), strFunction, j);
					AfxMessageBox(strLog);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
					MoveParent();	//DeviceMap/TrackDatas
					return FALSE;
				}

				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				strRealName = strName;
				if (strName == _T(""))
					strRealName = strAddr;
				pTrackProperty->SetProperty(_T("W"), strRealName, strAddr);
				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

				// Byte 항목을 Word에 만들어 넣기 
				for (int k = 0; k < nTempCount; k++)
				{
					MoveChild(k);	//DeviceMap/TrackDatas/Word/Byte

					nPropIndex = 0;

					GetAttrValue(_T("name"), strElName);
					if (strElName == _T(""))
					{
						MoveParent();	//DeviceMap/TrackDatas/Word
						continue;
					}

					strTemp.Format(_T("%d"), k + 1);
					pTrackProperty = new CTrackProperty();
					pTrackProperty->InitProperty();
					pTrackProperty->SetProperty(strType, strElName, strAddr, 0, k + 1);
					m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
					MoveParent();	//DeviceMap/TrackDatas/Word

				}

#pragma endregion
			}
			else if (strType == _T("N"))
			{
#pragma region Type이 니블일때 
				nTempCount = GetChildElmtCount();
				if (nTempCount == 0)
				{
					strLog.Format(_T("%s ... Nibble Type이 하위 엘리먼트가 없으면 안됩니다...[index=%d]"), strFunction, j);
					AfxMessageBox(strLog);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
					MoveParent();	//DeviceMap/TrackDatas
					return FALSE;
				}

				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				strRealName = strName;
				if (strName == _T(""))
					strRealName = strAddr;
				pTrackProperty->SetProperty(_T("W"), strRealName, strAddr);
				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
				int k = 0;

				// Nibble 항목을 Word에 만들어 넣기 
				for (int k = 0; k < nTempCount; k++)
				{
					MoveChild(k);	//DeviceMap/TrackDatas/Word/Nibble

					nPropIndex = 0;

					GetAttrValue(_T("name"), strElName);
					if (strElName == _T(""))
					{
						MoveParent();	//DeviceMap/TrackDatas/Word
						continue;
					}

					strTemp.Format(_T("%d"), k + 1);
					pTrackProperty = new CTrackProperty();
					pTrackProperty->InitProperty();
					pTrackProperty->SetProperty(strType, strElName, strAddr, 0, k + 1);
					m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

					MoveParent();	//DeviceMap/TrackDatas/Word

				}
				// Parent 값 넣기 
#pragma endregion
			}
			else if (strType == _T("b"))
			{
#pragma region Type이 비트일때 
				nTempCount = GetChildElmtCount();
				if (nTempCount == 0)
				{
					strLog.Format(_T("%s ... bit Type이 하위 엘리먼트가 없으면 안됩니다...[index=%d]"), strFunction, j);
					AfxMessageBox(strLog);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
					MoveParent();	//DeviceMap/TrackDatas
					return FALSE;
				}

				// Parent 값 넣기 
				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				strRealName = strName;
				if (strName == _T(""))
					strRealName = strAddr;
				pTrackProperty->SetProperty(_T("W"), strRealName, strAddr);
				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

				// bit 항목을 Word에 만들어 넣기 
				for (int k = 0; k < nTempCount; k++)
				{
					MoveChild(k);	//DeviceMap/TrackDatas/Word/bit

					nPropIndex = 0;

					GetAttrValue(_T("name"), strElName);
					if (strElName == _T(""))
					{
						MoveParent();	//DeviceMap/TrackDatas/Word
						continue;
					}

					strTemp.Format(_T("%d"), k + 1);
					pTrackProperty = new CTrackProperty();
					pTrackProperty->InitProperty();
					pTrackProperty->SetProperty(strType, strElName, strAddr, 0, k + 1);
					m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

					MoveParent();	//DeviceMap/TrackDatas/Word
				}
#pragma endregion
			}
			else if (strType == _T("M"))
			{
#pragma region Type이 복합일때 
				nTempCount = GetChildElmtCount();
				if (nTempCount == 0)
				{
					strLog.Format(_T("%s ... Multi Type이 하위 엘리먼트가 없으면 안됩니다...[index=%d]"), strFunction, j);
					AfxMessageBox(strLog);
					m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
					MoveParent();	//DeviceMap/TrackDatas
					return FALSE;
				}

				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				strRealName = strName;
				if (strName == _T(""))
					strRealName = strAddr;
				pTrackProperty->SetProperty(_T("W"), strRealName, strAddr);
				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
				int k = 0;

				// Multi 항목을 Word에 만들어 넣기 
				for (int k = 0; k < nTempCount; k++)
				{
					CString strTempTypeHeader = _T("");
					CString strTempTypeTail = _T("");
					CString strTempType = _T("");
					MoveChild(k);	//DeviceMap/TrackDatas/Word/Multi

					nPropIndex = 0;

					GetElmtName(strValue);

					if (strValue == _T(""))
						continue;

					strTempTypeHeader = strValue.Left(1);
					strTempTypeTail = strValue.Right(1);
					if (strTempTypeHeader == _T("B"))
					{
						if (strTempTypeTail == _T("L") ||
							strTempTypeTail == _T("H"))
						{
							strTempType = strTempTypeHeader;	// _T("B");
						}
						else if(
							strTempTypeTail == _T("0") ||
							strTempTypeTail == _T("1") || 
							strTempTypeTail == _T("2") ||
							strTempTypeTail == _T("3") ||
							strTempTypeTail == _T("4") ||
							strTempTypeTail == _T("5") ||
							strTempTypeTail == _T("6") ||
							strTempTypeTail == _T("7") ||
							strTempTypeTail == _T("8") ||
							strTempTypeTail == _T("9") ||
							strTempTypeTail == _T("A") ||
							strTempTypeTail == _T("B") ||
							strTempTypeTail == _T("C") ||
							strTempTypeTail == _T("D") ||
							strTempTypeTail == _T("E") ||
							strTempTypeTail == _T("F") )
						{
							strTempType = _T("b");
						}
						else
						{
							strLog.Format(_T("%s ... Multi Type의 올바르지 않는 하위 엘리먼트의 Type 입니다....[index=%d][ElmtName=%s]"), strFunction, j, strValue);
							AfxMessageBox(strLog);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
							MoveParent();	//DeviceMap/TrackDatas
							return FALSE;
						}
					}
					else if (strTempTypeHeader == _T("N"))
					{
						if (strTempTypeTail == _T("0") ||
							strTempTypeTail == _T("1") ||
							strTempTypeTail == _T("2") ||
							strTempTypeTail == _T("3") )
						{
							strTempType = strTempTypeHeader;	// _T("N");
						}
						else
						{
							strLog.Format(_T("%s ... Multi Type의 올바르지 않는 하위 엘리먼트의 Type 입니다....[index=%d][ElmtName=%s]"), strFunction, j, strValue);
							AfxMessageBox(strLog);
							m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
							MoveParent();		//DeviceMap/TrackDatas
							return FALSE;
						}
					}
					else
					{
						strLog.Format(_T("%s ... Multi Type의 올바르지 않는 하위 엘리먼트의 Type 입니다....[index=%d][ElmtName=%s]"), strFunction, j, strValue);
						AfxMessageBox(strLog);
						m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
						MoveParent();		//DeviceMap/TrackDatas
						return FALSE;
					}

					GetAttrValue(_T("name"), strElName);
					if (strElName == _T(""))
					{
						MoveParent();	//DeviceMap/TrackDatas/Word
						continue;
					}

					strTemp.Format(_T("%d"), k + 1);
					pTrackProperty = new CTrackProperty();
					pTrackProperty->InitProperty();
					pTrackProperty->SetProperty(strTempType, strElName, strAddr, 0, k + 1);
					m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

					MoveParent();	//DeviceMap/TrackDatas/Word

				}
				// Parent 값 넣기 
#pragma endregion
			}
			else
			{
				strLog.Format(_T("%s ... 정의 되지 않은 타입입니다....[index=%d]"), strFunction, j);
				AfxMessageBox(strLog);
				m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
				MoveParent();	//DeviceMap/TrackDatas
				return FALSE;
			}

			if (strName == _T(""))
			{
				strName = strAddr;
			}
#pragma endregion 
			MoveParent();	//DeviceMap/TrackDatas
		}
		MoveParent();
	}

	strTagItem = _T("./SeparatelyETC");
	if (MoveXPath(strTagItem, FALSE))	//DeviceMap/SeparatelyETC
	{
		GetAttrValue(_T("use"), strValue);

		if (strValue != _T("1"))
		{
			m_bUseSeparatelyETC = FALSE;
			MoveParent();
		}

		m_bUseSeparatelyETC = TRUE;
		int i, j, nIndex, nElmtCount = GetChildElmtCount();		// 여기서 하위에 엘리먼트가 없으면 안됨!

		for (int i = 0; i < nElmtCount; i++)
		{
			CString strTempTemp;
			MoveChild(i);	//DeviceMap/SeparatelyETC/Area

			GetAttrValue(_T("type"), strValue);			CString strType = strValue;					// 변수 지정 여부???
			GetAttrValue(_T("seq"), strValue);			CString strSeq = strValue;					// 변수 지정 여부???
			GetAttrValue(_T("name"), strValue);			CString strName = strValue;					// 변수 지정 여부???
			GetAttrValue(_T("address"), strValue);		CString strAddr = strValue;					// 변수 지정 여부???

			int nSubElmtCount = GetChildElmtCount();		// 여기서 하위에 엘리먼트가 없으면 안됨!

			nPropIndex = 0;
			// Parent 값 넣기 
			pTrackProperty = new CTrackProperty();
			pTrackProperty->InitProperty();
			pTrackProperty->m_bSeparatelyETC = TRUE;
			pTrackProperty->m_strParent = strName + _T("s");
			pTrackProperty->SetProperty(_T("W"), strName, strAddr);
			m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);

			// strSeq에 대한 작업 필요 ??
			if (strSeq == _T("1"))
			{
			}

			for (int j = 0; j < nSubElmtCount; j++)
			{
				MoveChild(j);	//DeviceMap/SeparatelyETC/Area
				nPropIndex = 0;

				strTemp.Format(_T("%d"), j + 1);

				int nInOrder = _ttoi(strTemp);

				GetAttrValue(_T("tid"), strValue);		CString strTrackNo = strValue;		int nTrackNo = _ttoi(strTrackNo);

				pTrackProperty = new CTrackProperty();
				pTrackProperty->InitProperty();
				pTrackProperty->m_bSeparatelyETC = TRUE;

				if (strName != _T("ProductSensor"))
				{
					pTrackProperty->m_strParent = strName + _T("s");
				}

				pTrackProperty->SetProperty(strType, strName, strAddr, nTrackNo, nInOrder);
				m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
				m_pDoc->m_strEtcKeyWords.Add(strName);

				// nTrackNo가 0이면 OpBox임!
				if (nTrackNo == 0)
				{
					int nSubSubElmtCount = GetChildElmtCount();		// 여기서 하위에 엘리먼트가 없으면 안됨!

					if (nSubSubElmtCount == 0)
					{
						strLog.Format(_T("%s ... Tid가 0인데(OpBox에서) 하위 엘리먼트가 없으면 안됩니다...[index=%d]"), strFunction, j);
						AfxMessageBox(strLog);
						m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
						return FALSE;
					}

					for (int k = 0; k < nSubSubElmtCount; k++)
					{
						MoveChild(k);	//DeviceMap/SeparatelyETC/Area/Bit

						GetElmtValue(strValue);
						int nTrackNo = _ttoi(strValue);

						// 하드코딩함!
						if (strName == _T("Emergency"))
						{
							//strTempTemp.Format(_T("%s_OpEmg_Tid%04d"), strName, nTrackNo);
						}
						else if (strName == _T("Auto"))
						{
							//strTempTemp.Format(_T("%s_OpAuto_Tid%04d"), strName, nTrackNo);
						}
						else
						{
							MoveParent();	//DeviceMap/SeparatelyETC/Area
							continue;
						}

						pTrackProperty = new CTrackProperty();
						pTrackProperty->InitProperty();
						pTrackProperty->m_bSeparatelyETC = TRUE;
						pTrackProperty->SetProperty(strType, strName, strAddr, nTrackNo, j + 1);
						m_pDoc->m_pTrackProperties[nId].Add(pTrackProperty);
						m_pDoc->m_strEtcKeyWords.Add(strName);
						//void CTrackProperty::SetProperty(CString strType, CString strName, CString strAddr, int nTrackNo, int nInOrder)

						MoveParent();	//DeviceMap/SeparatelyETC/Area
					}
				}
				MoveParent();	//DeviceMap/SeparatelyETC/Area
			}
			MoveParent();	//DeviceMap/SeparatelyETC
		}
		MoveParent();
	}
#pragma endregion

#pragma region Data TrackingArea 관련 Keyword 설정
	strTagItem = _T("./TrackingArea");
	if (MoveXPath(strTagItem, FALSE))		//DeviceMap/TrackingArea
	{
		int nIndex, nIndex2, nElmtCount = GetChildElmtCount();
		CString strLength, strCount, strName, strElName, strElchild, strTempTemp;
		CStringArray* strTrackingAreas = new CStringArray();

		GetAttrValue(_T("keywords"), strValue);		//m_pDoc->m_strTrackingKeywordList = strValue;
		CString strBuff;
		m_pDoc->m_strTrackingKeywordList.RemoveAll();
		int nKeywordLength = strValue.GetLength();
		for (int i = 0; i < nKeywordLength; ++i)
		{
			if (i == 0)
			{
				strBuff.Empty();
				strBuff += strValue[i];
			}
			else if (strValue[i] == ',')
			{
				//strBuff += strValue[i];
				strBuff.TrimLeft();
				strBuff.TrimRight();
				m_pDoc->m_strTrackingKeywordList.AddTail(strBuff);
				strBuff.Empty();
			}
			else if (i == nKeywordLength - 1)
			{
				strBuff += strValue[i];
				strBuff.TrimLeft();
				strBuff.TrimRight();
				m_pDoc->m_strTrackingKeywordList.AddTail(strBuff);
			}
			else
			{
				strBuff += strValue[i];
			}
		}


		for (int i = 0, nIndex = 0; i < nElmtCount; i++)
		{
			MoveChild(i);	//DeviceMap/TrackingArea/Word

			GetElmtValue(strValue);
			strTrackingAreas->Add(strValue);

			MoveParent();	//DeviceMap/TrackingArea
		}
		m_pDoc->m_strTrackingAreas.Add(strTrackingAreas);
		MoveParent();
	}
#pragma endregion

#pragma region FlantEquip 관련 설정
	strTagItem = _T("./FlantEquip");
	if (MoveXPath(strTagItem, FALSE))		//DeviceMap/FlantEquip
	{
		GetAttrValue(_T("use"), strValue);

		if (strValue == _T("1"))
		{
			int i, j, k, l, nIndex, nIndex2, nSubFlant, nRobotCount = GetChildElmtCount();
			CString strLength, strCount, strName, strElName, strElchild, strTempTemp;

			for (int i = 0; i < nRobotCount; i++)
			{
				MoveChild(i);	//DeviceMap/FlantEquip/Robot

				GetAttrValue(_T("Sub"), strValue);
				nSubFlant = _ttoi(strValue);

				int nReadOrWriteCount = GetChildElmtCount();

				for (int j = 0; j < nReadOrWriteCount; j++)
				{
					MoveChild(j);	//DeviceMap/FlantEquip/Robot/Read or Write
					int nJobOrStatusCount = GetChildElmtCount();

					for (int k = 0; k < nJobOrStatusCount; k++)
					{
						MoveChild(k);	//DeviceMap/FlantEquip/Robot/Read or Write/Job or Status 
						int nAreaCount = GetChildElmtCount();

						for (int l = 0; l < nAreaCount; l++)
						{
							MoveChild(l);	//DeviceMap/FlantEquip/Robot/Read or Write/Job or Status/Area 

							GetAttrValue(_T("type"), strValue);			CString strType = strValue;
							GetAttrValue(_T("seq"), strValue);			CString strSeq = strValue;
							GetAttrValue(_T("name"), strValue);			CString strName = strValue;
							GetAttrValue(_T("address"), strValue);		CString strAddr = strValue;

							int nSubElmtCount = GetChildElmtCount();		// 여기서 하위에 엘리먼트가 있으면 안됨!

							// 여기서 하위에 엘리먼트가 있으면 안됨! - 전용설비에서는 무조건 0임 => 중외제약의 경우
							if (nSubElmtCount > 0)
							{
								MoveParent();	//DeviceMap/FlantEquip/Robot/Read or Write/Job or Status
								continue;
							}

							// strSeq가 0이 아니면 안됨 - 전용설비에서는 무조건 0으로 들어감 => 중외제약의 경우
							if (strSeq != _T("0"))
							{
								MoveParent();	//DeviceMap/FlantEquip/Robot/Read or Write/Job or Status
								continue;
							}
							
							pFlantProperty = new CFlantProperty();
							pFlantProperty->InitProperty();
							//pTrackProperty->m_bSeparatelyETC = TRUE;
							//pTrackProperty->m_strParent = strName + _T("s");		// Type이 W 이므로 Parent 값 필요 없음!
							pFlantProperty->SetProperty(_T("W"), strName, strAddr);
							m_pDoc->m_pFlantProperties[nId].Add(pFlantProperty);

							MoveParent();	//DeviceMap/FlantEquip/Robot/Read or Write/Job or Status
						}
						MoveParent();	//DeviceMap/FlantEquip/Robot/Read or Write
					}
					MoveParent();	//DeviceMap/FlantEquip/Robot
				}
				MoveParent();	//DeviceMap/FlantEquip
			}
		}
		MoveParent();
	}
#pragma endregion
	return TRUE;
}