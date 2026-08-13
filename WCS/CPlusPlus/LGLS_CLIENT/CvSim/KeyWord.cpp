// KeyWord.cpp: implementation of the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "KeyWord.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MYTRACE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyWord::CKeyWord(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	m_bUseDestTable = FALSE;
}

CKeyWord::~CKeyWord()
{
		
}

BOOL CKeyWord::ParseXml()
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
	if (!CXmlDom::LoadXmlFile(KEY_WORD_FILE))
	{
		strLog.Format(_T("%s ... XML 문서 로딩에 실패하였습니다. "), strFunction);
		AfxMessageBox(strLog);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
		return FALSE;
	}
	#pragma endregion 

	CString strClassName, strDevice, strCID, strValue, strForkType, strThreadNo, strTagItem;
	MoveRoot();

	#pragma region KeyWord System 구축 

//	strTagItem = _T("./KeyWords");
//	if (MoveXPath(strTagItem, FALSE))	//KeyWord/KeyWords
//	{
		int nElmtCount = GetChildElmtCount();
		CString strName, strParent;

		// 초기화
//		m_pDoc->m_strKeyWords.RemoveAll();
		m_pDoc->m_pKeyWords.RemoveAll();


		m_pDoc->m_pKeyWords.SetAt(_T("Count"), CConvert::ToString(nElmtCount));

		for (int n = 0; n < nElmtCount; n++)
		{
			MoveChild(n);	//KeyWord/KeyWords/KeyWord

			GetAttrValue(_T("name"), strName);

			GetAttrValue(_T("parent"), strParent);

			int nSubElmtCount = GetChildElmtCount();

			int nMultiple = 2;	// 배수
			int nSubCount = nSubElmtCount * nMultiple;
			strTemp = CConvert::ToString(nSubCount);
			m_pDoc->m_pKeyWords.SetAt(strName, strTemp);
//			m_pDoc->m_strKeyWords.Add(strName);

			strTemp = strName + _T("00");
			m_pDoc->m_pKeyWords.SetAt(strTemp, strParent);

			if (nSubElmtCount == 0)
			{
				MoveParent();	//KeyWord/KeyWords
				continue;
			}

			for (int m = 0; m < nSubElmtCount; m++)
			{
				MoveChild(m);	//KeyWord/KeyWords/KeyWord/Condition
				int nPropNum = (m * nMultiple) + 1;

				strTemp.Format(_T("%s%02d"), strName, nPropNum++);	
				GetAttrValue(_T("content"), strValue);	
				m_pDoc->m_pKeyWords.SetAt(strTemp, strValue);

				strTemp.Format(_T("%s%02d"), strName, nPropNum++);	
				GetElmtValue(strValue);					
				m_pDoc->m_pKeyWords.SetAt(strTemp, strValue);
				MoveParent();	//KeyWord/KeyWords/KeyWord
			}
			MoveParent();	//KeyWord/KeyWords
		}
		MoveParent();
//	}
	#pragma endregion 
	return TRUE;
}