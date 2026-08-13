// ListenerSk.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "ListenerSk.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenerSk

CListenerSk::CListenerSk(CEquipment* pEquipment)
{
	m_pEquipment = pEquipment;
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);
}

CListenerSk::~CListenerSk()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenerSk, CAsyncSocketEx)
	//{{AFX_MSG_MAP(CListenerSk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenerSk member functions

void CListenerSk::OnAccept(int nErrorCode) 
{
	DEBUGER_ASSERT_VALID(m_pEquipment != NULL);

	m_pEquipment->OnAcceptSocket(nErrorCode);
}
