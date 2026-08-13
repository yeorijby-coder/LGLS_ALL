#pragma once


// CReportHeader
//
class CReportHeader : public CHeaderCtrl
{
public:
	CReportHeader();
	virtual ~CReportHeader();

public:
	int m_nColumn;
	BOOL m_bAscending;

// Implementation
public:
	void SetSortArrow(const int iColumn);

protected:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()
};


