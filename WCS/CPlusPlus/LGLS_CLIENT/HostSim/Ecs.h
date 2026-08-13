// Ecs.h : main header file for the ECS application
//

#if !defined(AFX_ECS_H__86DBFDA8_3F4E_41C3_AFF6_3AB23D879BA7__INCLUDED_)
#define AFX_ECS_H__86DBFDA8_3F4E_41C3_AFF6_3AB23D879BA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEcsApp:
// See Ecs.cpp for the implementation of this class
//

class CEcsApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEcsApp();
public:
	CString m_strCurID;
	CString m_strCurPwd;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEcsApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEcsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECS_H__86DBFDA8_3F4E_41C3_AFF6_3AB23D879BA7__INCLUDED_)
