#pragma once
#include "afxwin.h"
#include "ExtLabel.h"
#include "EcsDoc.h"

// SpreadTest 대화 상자입니다.

class CSpreadTest : public CDialog
{
	DECLARE_DYNAMIC(CSpreadTest)

public:
	CSpreadTest(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpreadTest();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PANEL_EXTRA };

	CEcsDoc*		m_pDoc;
	CSpreadSheet m_SpreadSheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CExtLabel m_staticTitle;
};
