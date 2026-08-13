
#pragma once

#include "ReportHeader.h"

class CReportCtrl : public CListCtrl
{

public:
	CReportCtrl();

public:
	CReportHeader m_ctlHeader;

public:
	void InsertColumn(LPCTSTR pszText, ...);
	void AddItem(LPARAM lParam, int nImage, CStringList *plistText);

	void LoadImage(int nIconID, ...);

	void ResizeColumn();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CReportCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

