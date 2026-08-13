// Logic.cpp: implementation of the CEcsDefine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ecs.h"
#include "Logic.h"
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

CLogicSystem::CLogicSystem(CEcsDoc* pDoc)
{
	m_pDoc = pDoc;
	DEBUGER_ASSERT_VALID(m_pDoc != NULL);
	m_bUseDestTable = FALSE;
}

CLogicSystem::~CLogicSystem()
{
		
}

BOOL CLogicSystem::ParseXml()
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
	if (!CXmlDom::LoadXmlFile(LOGIC_FILE))
	{
		strLog.Format(_T("%s ... XML 문서 로딩에 실패하였습니다. "), strFunction);
		AfxMessageBox(strLog);
		m_pDoc->WriteLog(LOG_TYPE_ERROR, LOG_POS_SYSTEM, strLog, strFunction);
		return FALSE;
	}
	#pragma endregion 

	CString strClassName, strDevice, strCID, strValue, strForkType, strThreadNo, strTagItem;
	MoveRoot();

	#pragma region Logic System 구축 
	CString strNum, strKeyWord;
	int nElmtCount = GetChildElmtCount();					// LogicGroup 갯수 
	CString strName, strParent, strLogicDesc, strConditionDesc, strActionDesc, strId;

	// TEST - TEMP
	m_pDoc->m_arrLogicGroups.RemoveAll();
	m_pDoc->m_arrLogicGroups.SetSize(nElmtCount);
	for (int n = 0; n < nElmtCount; n++)
	{
		MoveChild(n);	//Logic/LogicDatas/LogicGroup
		int nSubElmtCount = GetChildElmtCount();			// Logic 갯수 

		m_pDoc->m_arrLogicGroups[n].SetSize(nSubElmtCount);
			
		GetAttrValue(_T("name"), strName);
		m_pDoc->m_arrLogicGroups[n].m_strName = strName;
			
		for (int nn = 0; nn < nSubElmtCount; nn++)
		{
			MoveChild(nn);	//Logic/LogicDatas/LogicGroup/Logic

			GetAttrValue(_T("desc"), strLogicDesc);			
			m_pDoc->m_arrLogicGroups[n][nn].m_strDesc = strLogicDesc;

			GetAttrValue(_T("id"), strId);
			m_pDoc->m_arrLogicGroups[n][nn].m_nId = _ttoi(strId);

			if (MoveXPath(_T("conditions"), FALSE))	//Logic/LogicDatas/LogicGroup/Logic/conditions
			{
				int nSubSubElmtCount = GetChildElmtCount();

				//m_pDoc->m_arrLogicGroups[n].m_pConditions->SetSize(nSubSubElmtCount);
				m_pDoc->m_arrLogicGroups[n][nn].m_arrConditions.SetSize(nSubSubElmtCount);

				GetAttrValue(_T("desc"), strConditionDesc);
				m_pDoc->m_arrLogicGroups[n][nn].m_arrConditions.m_strDesc = strConditionDesc;

				CCondition  oCondition;
				for (int nnn = 0; nnn < nSubSubElmtCount; nnn++)
				{
					MoveChild(nnn);	//Logic/LogicDatas/LogicGroup/Logic/conditions/condition
						
					GetAttrValue(_T("CNum"), strNum);
					int nConditionNum = _ttoi(strNum);
						
					GetAttrValue(_T("CKeyWord"), strKeyWord);
					oCondition.m_enType = (EN_CONDITION_TYPE)nConditionNum;
					oCondition.m_nType = nConditionNum;
					oCondition.m_strKeyWord = strKeyWord;

					m_pDoc->m_arrLogicGroups[n][nn].m_arrConditions[nnn] = oCondition;
					MoveParent();	//Logic/LogicDatas/LogicGroup/Logic/conditions
				}
				MoveParent();		//Logic/LogicDatas/LogicGroup/Logic/
			}
			if (MoveXPath(_T("actions"), FALSE))	//Logic/LogicDatas/LogicGroup/Logic/actions
			{
				int nSubSubElmtCount = GetChildElmtCount();

				m_pDoc->m_arrLogicGroups[n][nn].m_arrActions.SetSize(nSubSubElmtCount);

				GetAttrValue(_T("desc"), strActionDesc);
				m_pDoc->m_arrLogicGroups[n][nn].m_arrActions.m_strDesc = strActionDesc;

				CAction oAction;
				for (int nnn = 0; nnn < nSubSubElmtCount; nnn++)
				{
					MoveChild(nnn);	//Logic/LogicDatas/LogicGroup/Logic/actions/action
					GetAttrValue(_T("ANum"), strNum);
					int nActionNum = _ttoi(strNum);

					GetAttrValue(_T("AKeyWord"), strKeyWord);
					oAction.m_enType = (EN_ACTION_TYPE)nActionNum;
					oAction.m_nType = nActionNum;
					oAction.m_strKeyWord = strKeyWord;

					m_pDoc->m_arrLogicGroups[n][nn].m_arrActions[nnn] = oAction;
					MoveParent();	//Logic/LogicDatas/LogicGroup/Logic/actions
				}
				MoveParent();		//Logic/LogicDatas/LogicGroup/Logic/
			}
			MoveParent();	//Logic/LogicDatas/LogicGroup
		}
		MoveParent();	//Logic/LogicDatas
	}
	MoveParent();
	#pragma endregion 

	return TRUE;
}