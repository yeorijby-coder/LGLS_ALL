// SpreadTest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "SpreadTest.h"
#include "afxdialogex.h"


// SpreadTest 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpreadTest, CDialog)

CSpreadTest::CSpreadTest(CWnd* pParent /*=NULL*/)
	: CDialog(CSpreadTest::IDD, pParent)
{
	m_pDoc = NULL;

	m_SpreadSheet.m_nSorting = SS_USERCOLACTION_DEFAULT;
	//m_SpreadSheet.m_bAutoResizeCol = true;
	m_SpreadSheet.m_wGridType = SS_GRID_HORIZONTAL|SS_GRID_VERTICAL|SS_GRID_SOLID;
	m_SpreadSheet.m_rectMargin = CRect(0, 35, 0, 0);

	HFONT hFontH = CreateFont(20,0,0,0,700,0,0,0,0,0,0,0,0,_T("Tahoma"));
	HFONT hFontB = CreateFont(12,0,0,0,FW_HEAVY,0,0,0,0,0,0,0,0,_T("굴림"));

	m_SpreadSheet.AddSheet(_T(""), this, hFontH, hFontB);
	m_SpreadSheet.AddColHead(_T("항  목"), 14);
	m_SpreadSheet.AddColHead(_T("설  정"), 20);
	m_SpreadSheet.AddColHead(_T("선  택"), 10);
	m_SpreadSheet.AddColHead(_T("확  인"), 12);
	m_SpreadSheet.AddColHead(_T("기  타"), 300);



}

CSpreadTest::~CSpreadTest()
{
}

void CSpreadTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
}


BEGIN_MESSAGE_MAP(CSpreadTest, CDialog)
END_MESSAGE_MAP()


// SpreadTest 메시지 처리기입니다.


BOOL CSpreadTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//m_pDoc->m_hPanelInfo = m_hWnd;
	BOOL bResult = m_SpreadSheet.Create();
	if (bResult == TRUE)
	{
		m_SpreadSheet.SetStatic(1, SS_ALLROWS);
		m_SpreadSheet.SetColMerge(1);
	
		m_staticTitle.SetWindowText(_T("DETAIL INFORMATION"));
		m_staticTitle.SetTransparent(TRUE);
		m_staticTitle.SetFont(_T("Arial Black"));
		m_staticTitle.SetTextColor(RGB(255, 102, 0));
		m_staticTitle.SetFontSize(30, TRUE);
	}
	else 
	{
		AfxMessageBox(_T("생성에 실패했습니다. "));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
