// ControlGroup.cpp: implementation of the CControlGroup class.
//

#include "StdAfx.h"
#include "ControlGroup.h"
#include <atlbase.h>


// CControlGroup
CControlGroup::CControlGroup(void)
{
	m_strGroupID.Empty();
	m_strGroupName.Empty();
	m_bVisible = TRUE;

	m_pGroupCtrls = new CDciControlList;
	ASSERT(m_pGroupCtrls != NULL);
}

CControlGroup::~CControlGroup(void)
{
	delete m_pGroupCtrls;
}

// CConvert ¸â¹ö ÇÔ¼ö
int CControlGroup::UpdatePropNames(CDciPropertyArray& properties)
{
	int i = 0;

	properties.SetSize(3);
	properties[i++].SetProperty(CDciProperty::PT_STRING, _T("gname"));
	properties[i++].SetProperty(CDciProperty::PT_DEC, _T("gid"));
	properties[i++].SetProperty(CDciProperty::PT_BOOL, _T("show"));
	ASSERT(properties.GetSize() == i);

	return i;
}

int CControlGroup::UpdatePropValues(CDciPropertyArray& proerties, BOOL bSaveObject /* = TRUE */)
{
	int i = 0;

	if (bSaveObject)
	{
		m_strGroupName	= proerties[i++].m_strValue;
		m_strGroupID	= proerties[i++].m_strValue;
		m_bVisible		= CConvert::ToInt(proerties[i++].m_strValue);
	}
	else
	{
		proerties[i++].m_strValue.Format(_T("%s"), m_strGroupName);
		proerties[i++].m_strValue.Format(_T("%s"), m_strGroupID);
		proerties[i++].m_strValue.Format(_T("%d"), m_bVisible);
	}

	return i;
}


BOOL CControlGroup::LoadXml(CXmlDom* pXML, CDciMaster* pDCI)
{
	if (pXML == NULL)
		return FALSE;

	int i, j, k;
	CString str;
	CDciPropertyArray properties;

	UpdatePropNames(properties);
	for (i=0; i<properties.GetSize(); ++i)
	{
		try
		{
			pXML->GetAttrValue(properties[i].m_strName, properties[i].m_strValue);
		}
		catch (_com_error& e)
		{
			TRACE(_T("\n [%s][%d][CControlGroup::LoadXml][1] GetAttrValue(%s, %s) _com_error_exception(%s) \n"), 
				__FILE__, __LINE__, properties[i].m_strName, properties[i].m_strValue, e.ErrorMessage());
		}
	}
	UpdatePropValues(properties);

	if (pXML->SelectElements(_T("./*/Control"), FALSE))
	{
		CString strClassName;
		CDciControl* pDciControl = NULL;
		m_pGroupCtrls->Clear();
		
		for (i=0; i<pXML->GetSelectCount(); ++i)
		{
			pXML->MoveSelect(i);
			pXML->MoveParent();
			pXML->GetElmtName(strClassName);
			pXML->MoveSelect(i);

			if (pDciControl = pDCI->CreateDciObject(strClassName))
			{
				pDciControl->UpdatePropNames(properties);
				for (j=0; j<properties.GetSize(); ++j)
				{
					try
					{
						pXML->GetAttrValue(properties[j].m_strName, properties[j].m_strValue);
					}
					catch (_com_error& e)
					{
						TRACE(_T("\n [%s][%d][CControlGroup::LoadXml][2] %s GetAttrValue(%s, %s) _com_error_exception(%s) \n"), __FILE__, __LINE__,
							pDciControl->GetRuntimeClass()->m_lpszClassName, properties[j].m_strName, properties[j].m_strValue, e.ErrorMessage());
					}
				}
				pDciControl->UpdatePropValues(properties);

				if (pDciControl->GetItemSize())
				{
					pDciControl->UpdateItemNames(properties);

					for (j=0; j<pDciControl->GetItemSize(); ++j)
					{
						pXML->MoveChild(j);
						
						for (k=0; k<properties.GetSize(); ++k)
						{
							try
							{
								pXML->GetAttrValue(properties[k].m_strName, properties[k].m_strValue);
							}
							catch (_com_error& e)
							{
								TRACE(_T("\n [%s][%d][CControlGroup::LoadXml][3] %s GetAttrValue(%s, %s) _com_error_exception(%s) \n"), __FILE__, __LINE__,
									pDciControl->GetRuntimeClass()->m_lpszClassName, properties[j].m_strName, properties[j].m_strValue, e.ErrorMessage());
							}
						}
						pDciControl->UpdateItemValues(j, properties);

						pXML->MoveParent();
					}

				}

				m_pGroupCtrls->AddTail(pDciControl);
			}
		}
	}
	
	return TRUE;
}

BOOL CControlGroup::SaveXml(CXmlDom* pXML)
{
	if (pXML == NULL)
		return FALSE;

	int i, j, k;
	CString str;
	CDciPropertyArray properties;

	UpdatePropNames(properties);
	UpdatePropValues(properties, FALSE);
	for (i=0; i<properties.GetSize(); ++i)
		pXML->AddChildAttribute(properties[i].m_strName, properties[i].m_strValue);

	CDciControl* pDciControl = NULL;
	for (POSITION pos=m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
	{
		pDciControl = m_pGroupCtrls->GetNext(pos);
		ASSERT(pDciControl != NULL);

		str = pDciControl->GetRuntimeClass()->m_lpszClassName;
		//str.Format(_T("./%s"), CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName));		// Ver. VS2010
		//str.Format(_T("./%s"), LPCWSTR(pDciControl->GetRuntimeClass()->m_lpszClassName));		// Ver. VS2019
		if (!pXML->MoveXPath(str, FALSE))
		{
			//pXML->AddChildElement(CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName));	// Ver. VS2010
			//pXML->AddChildElement(LPCWSTR(pDciControl->GetRuntimeClass()->m_lpszClassName));	// Ver. VS2019
			pXML->AddChildElement(str);	// Ver. VS2019
			pXML->MoveChildLast();
		}

		pXML->AddChildElement(_T("Control"));
		pXML->MoveChildLast();

		pDciControl->UpdatePropNames(properties);
		pDciControl->UpdatePropValues(properties, FALSE);
		for (i=0; i<properties.GetSize(); ++i)
			pXML->AddChildAttribute(properties[i].m_strName, properties[i].m_strValue);

		if (pDciControl->GetItemSize())
		{
			pDciControl->UpdateItemNames(properties);

			for (j=0; j<pDciControl->GetItemSize(); ++j)
			{
				pXML->AddChildElement(_T("Item"));
				pXML->MoveChildLast();

				pDciControl->UpdateItemValues(j, properties, FALSE);
				for (k=0; k<properties.GetSize(); ++k)
					pXML->AddChildAttribute(properties[k].m_strName, properties[k].m_strValue);

				pXML->MoveParent();
			}
		}

		pXML->MoveParent();
		pXML->MoveParent();
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CControlGroup* CControlGroups::GetControlGroup(CDciControl* pDciControl)
{
	if (pDciControl == NULL)
		return NULL;

	for (int i=0; i<m_nSize; ++i)
	{
		CControlGroup& group = m_pData[i];
		for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
		{
			if (group.m_pGroupCtrls->GetNext(pos) == pDciControl)
				return &group;
		}
	}

	return NULL;
}

CControlGroup* CControlGroups::GetControlGroup(CDciControlList* pDciCtrlList)
{
	if (pDciCtrlList == NULL)
		return NULL;

	for (int i=0; i<m_nSize; ++i)
	{
		CControlGroup& group = m_pData[i];
		if (group.m_pGroupCtrls == pDciCtrlList)
			return &group;
	}

	return NULL;
}

CRect CControlGroups::GetLayoutRectL(BOOL bOnlyShow)
{
	CRect rcLayoutL(0, 0, 0, 0);
	for (int i=0; i<m_nSize; ++i)
	{
		CControlGroup& group = m_pData[i];
		if ((bOnlyShow == TRUE) && (group.m_bVisible == FALSE))
			continue;

		CDciControl* pDciControl = NULL;
		for (POSITION pos=group.m_pGroupCtrls->GetHeadPosition(); pos!=NULL; )
		{
			pDciControl = group.m_pGroupCtrls->GetNext(pos);
			ASSERT(pDciControl != NULL);

			if (rcLayoutL.IsRectNull())
			{
				rcLayoutL = pDciControl->m_rcControlL;
				continue;
			}

			rcLayoutL.left = (rcLayoutL.left > pDciControl->m_rcControlL.left) ? pDciControl->m_rcControlL.left : rcLayoutL.left;
			rcLayoutL.bottom = (rcLayoutL.bottom > pDciControl->m_rcControlL.bottom) ? pDciControl->m_rcControlL.bottom : rcLayoutL.bottom;
			rcLayoutL.right = (rcLayoutL.right < pDciControl->m_rcControlL.right) ? pDciControl->m_rcControlL.right : rcLayoutL.right;
			rcLayoutL.top = (rcLayoutL.top < pDciControl->m_rcControlL.top) ? pDciControl->m_rcControlL.top : rcLayoutL.top;
		}
	}

	return rcLayoutL;
}