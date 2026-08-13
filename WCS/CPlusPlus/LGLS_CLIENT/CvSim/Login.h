#if !defined(AFX_LOGIN_H__196ACF61_7682_4AA3_8FC6_3FF6D767DE8C__INCLUDED_)
#define AFX_LOGIN_H__196ACF61_7682_4AA3_8FC6_3FF6D767DE8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class CLogin : public CDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_LOGIN };
	CString	m_strUserID;
	CString	m_strPassword;
	//}}AFX_DATA

// Attributes
protected:
	CBitmap		m_bitmap;			// Background Bitmap
//	HBITMAP		m_hBitmap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__196ACF61_7682_4AA3_8FC6_3FF6D767DE8C__INCLUDED_)
