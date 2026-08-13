#if !defined(AFX_TGROUPBOX_H__FC985894_7DBF_11D3_AE2E_000000000000__INCLUDED_)
#define AFX_TGROUPBOX_H__FC985894_7DBF_11D3_AE2E_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TGroupBox.h : header file
//

#define OFS_X	10 // distance from left/right side to beginning/end of text

/////////////////////////////////////////////////////////////////////////////
// CTGroupBox window

class CTGroupBox : public CButton
{
	// Construction
public:
	CTGroupBox();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTGroupBox)
public:
	void SetFontSubclass(CFont* pFont);

protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CTGroupBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTGroupBox)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGROUPBOX_H__FC985894_7DBF_11D3_AE2E_000000000000__INCLUDED_)
