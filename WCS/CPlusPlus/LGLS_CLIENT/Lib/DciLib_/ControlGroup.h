// ControlGroup.h: interface for the CControlGroup class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "XmlDom.h"
//#include "DciControl.h"


class AFX_EXT_CLASS CControlGroup
{
public:
	CControlGroup(void);
	virtual ~CControlGroup(void);

public:
	CString	m_strGroupID;
	CString	m_strGroupName;
	BOOL	m_bVisible;
	CDciControlList* m_pGroupCtrls;

public:
	int UpdatePropNames(CDciPropertyArray& properties);
	int UpdatePropValues(CDciPropertyArray& proerties, BOOL bSaveObject = TRUE);

public:
	BOOL LoadXml(CXmlDom* pXML, CDciMaster* pDCI);
	BOOL SaveXml(CXmlDom* pXML);
};

class AFX_EXT_CLASS CControlGroups : public CArray<CControlGroup, CControlGroup>
{
public:
	CControlGroups() {}
	virtual ~CControlGroups() {}

public:
	CControlGroup* GetControlGroup(CDciControl* pDciControl);
	CControlGroup* GetControlGroup(CDciControlList* pDciCtrlList);
	CRect GetLayoutRectL(BOOL bOnlyShow);
};

//#undef AFX_DATA
//#define AFX_DATA

