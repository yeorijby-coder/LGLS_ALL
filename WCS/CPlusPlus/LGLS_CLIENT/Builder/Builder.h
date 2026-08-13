// Builder.h : main header file for the BUILDER application
//

#if !defined(AFX_BUILDER_H__85903979_E6BC_4A6A_90E4_A8DF77705070__INCLUDED_)
#define AFX_BUILDER_H__85903979_E6BC_4A6A_90E4_A8DF77705070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBuilderApp:
// See Builder.cpp for the implementation of this class
//

class CBuilderApp : public CWinApp
{
public:
	CBuilderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuilderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBuilderApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDER_H__85903979_E6BC_4A6A_90E4_A8DF77705070__INCLUDED_)
