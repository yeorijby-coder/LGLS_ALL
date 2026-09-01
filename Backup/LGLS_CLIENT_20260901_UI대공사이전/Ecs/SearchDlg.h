#pragma once
#include "DialogResize.h"

// CSearchDlg 대화 상자입니다.

class CEcsView;
class CSearchDlg : public CDialogResize
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CEcsView* pView, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSearchDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SEARCH_DLG };

public:
	CEcsView* m_pView;

private:
	CFont *m_pDlgFont;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_chxLuggSearch;
	CButton m_chxStationSearch;
	CButton m_chxTrackSearch;
	CButton m_chxTraySearch;
	afx_msg void OnCheckLuggnumSearch();
	afx_msg void OnCheckTrayidSearch();
	afx_msg void OnCheckStationSearch();
	afx_msg void OnBnClickedButtonSize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCheckTrackSearch();
};
