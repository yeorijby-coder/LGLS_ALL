#pragma once

#include "ReportHeader.h"


// CReportCtrl
//
class CReportCtrl : public CListCtrl
{
public:
	CReportCtrl();
	virtual ~CReportCtrl();

public:
	CReportHeader m_ctlHeader;

// Overrides
protected:
	virtual void PreSubclassWindow();

// Implementation
public:
	void InsertColumn(LPCTSTR pszText, ...);
	void AddItem(LPARAM lParam, int nImage, CStringList *plistText);
	void LoadImage(int nIconID, ...);
	void ResizeColumn();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};


