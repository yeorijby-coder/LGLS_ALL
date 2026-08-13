

#pragma once

class CReportHeader : public CHeaderCtrl
{
public:
	CReportHeader();

public:
	int m_nColumn;
	BOOL m_bAscending;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportHeader)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReportHeader();

	void SetSortArrow( const int iColumn );

	// Generated message map functions
protected:
	void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	//{{AFX_MSG(CReportHeader)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

