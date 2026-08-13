// WarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "WarningDlg.h"

#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////
// CWarningDlg dialog


CWarningDlg::CWarningDlg(CEcsDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialogResize(CWarningDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bExpand = true;
	m_bShow = true;
	//{{AFX_DATA_INIT(CWarningDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_TIP, m_ctlTip);
	DDX_Control(pDX, IDC_LIST_WARNING, m_ctlReport);
	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_btnFirst);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_SHOW, m_btnShow);
}

BEGIN_MESSAGE_MAP(CWarningDlg, CDialogResize)
	//{{AFX_MSG_MAP(CWarningDlg)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, OnButtonShow)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_USER_REFRESH_NOTIFY, OnRefreshNotify)

END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CWarningDlg)
	DLGRESIZE_CONTROL(IDC_STATIC_PANE, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_DELETE, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_FIRST, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_LAST, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_PREV, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_NEXT, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_BUTTON_SHOW, DLSZ_MOVE_X)

	DLGRESIZE_CONTROL(IDC_STATIC_TIP, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_LIST_WARNING, DLSZ_SIZE_X | DLSZ_SIZE_Y)
END_DLGRESIZE_MAP()


////////////////////////////////////////////////
// CWarningDlg message handlers

BOOL CWarningDlg::OnInitDialog() 
{
	CDialogResize::OnInitDialog();

	SetSxButton();

	InitResizing(FALSE);

	m_ctlTip.SetIcon(IDI_ICON_LIGHTBULB);

	m_ctlReport.LoadImage(IDI_ICON_LOG_INFO,
						  IDI_ICON_LOG_ERROR,
						  IDI_ICON_LOG_WARNING,
						  IDI_ICON_LOG_QUESTION, -1);
	m_ctlReport.InsertColumn(_T("작업번호"), _T("TRAY ID"), _T("구분"), _T("내용"), _T("ETX"));

	Refresh();

	return TRUE;
}

void CWarningDlg::SetSxButton()
{
	m_btnShow.setType (CFooButton::checkBox);
	m_btnShow.check (!m_bShow);
	m_btnShow.setTextColor(RGB(51, 102, 255));

	m_btnFirst.setGradient (true);
	m_btnFirst.setType (CFooButton::hotPushButton);

	m_btnLast.setGradient (true);
	m_btnLast.setType (CFooButton::hotPushButton);

	m_btnNext.setGradient (true);
	m_btnNext.setType (CFooButton::hotPushButton);

	m_btnPrev.setGradient (true);
	m_btnPrev.setType (CFooButton::hotPushButton);

	m_btnDelete.setGradient (true);
	m_btnDelete.setType (CFooButton::hotPushButton);

	m_btnExit.setGradient (true);
	m_btnExit.setGradientBottomColor(RGB(212, 124, 168));
	m_btnExit.setType (CFooButton::hotPushButton);
}

LRESULT CWarningDlg::OnRefreshNotify(WPARAM wParam, LPARAM lParam) 
{
	Refresh();

	return 0;
}

void CWarningDlg::Refresh()
{
	m_pDoc->m_pLog->DeleteMaxOverAlarm();
	m_pDoc->m_pLog->LoadAlarmList(&m_ctlReport);
	m_pDoc->m_pLog->LoadAlarmLast(&m_ctlTip);
}

void CWarningDlg::OnButtonFirst() 
{
	m_pDoc->m_pLog->LoadAlarmFirst(&m_ctlTip);	
}

void CWarningDlg::OnButtonLast() 
{
	m_pDoc->m_pLog->LoadAlarmLast(&m_ctlTip);	
}

void CWarningDlg::OnButtonPrev() 
{
	m_pDoc->m_pLog->LoadAlarmPrev(&m_ctlTip);	
}

void CWarningDlg::OnButtonNext() 
{
	m_pDoc->m_pLog->LoadAlarmNext(&m_ctlTip);	
}

void CWarningDlg::OnButtonDelete() 
{
	m_pDoc->m_pLog->DeleteAllAlarm();
	m_ctlReport.DeleteAllItems();
	m_ctlTip.ResetTipText();
}

void CWarningDlg::OnButtonShow() 
{
	m_bShow = !m_bShow;
	Refresh();
	ShowWindow(m_bShow);
//	if (m_bShow )
//		::PostMessage(m_pDoc->m_hWndView, WM_USER_ALARM_NOTIFY, 0, 0);
}

void CWarningDlg::OnButtonExpand() 
{
	m_bExpand = !m_bExpand;
	if ( m_bExpand )
	{
		GetDlgItem(IDC_STATIC_TIP)->ShowWindow(false);
		GetDlgItem(IDC_LIST_WARNING)->ShowWindow(true);

		ENABLE_WND(IDC_BUTTON_FIRST, false);
		ENABLE_WND(IDC_BUTTON_LAST, false);
		ENABLE_WND(IDC_BUTTON_PREV, false);
		ENABLE_WND(IDC_BUTTON_NEXT, false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TIP)->ShowWindow(true);
		GetDlgItem(IDC_LIST_WARNING)->ShowWindow(false);

		ENABLE_WND(IDC_BUTTON_FIRST, true);
		ENABLE_WND(IDC_BUTTON_LAST, true);
		ENABLE_WND(IDC_BUTTON_PREV, true);
		ENABLE_WND(IDC_BUTTON_NEXT, true);
	}
}

BOOL CWarningDlg::OnEraseBkgnd(CDC* pDC) 
{
	CDialogResize::OnEraseBkgnd(pDC);

	CRect rect;
    GetClientRect(rect);
    
    int r1 = 255, g1 = 0, b1 = 0;
    int r2 = 255, g2 = 255, b2 = 0;
    
    for(int i=0; i<rect.right; i++) 
    { 
        int r = r1 + (i * (r2-r1) / rect.right); 
        int g = g1 + (i * (g2-g1) / rect.right);
        int b = b1 + (i * (b2-b1) / rect.right);
        
		CBrush brush;
		brush.CreateSolidBrush(RGB(r, g, b));

        pDC->FillRect(CRect(i, 0, i+1, rect.bottom), &brush);
    }

	return true;
}

void CWarningDlg::ExpandShow(UINT nMarkID, BOOL bExpand)
{
	CRect rcLarge;
	CRect rcSmall;

	CRect rcMark;
	CWnd* pMark = GetDlgItem(nMarkID);
	ASSERT(pMark);

	GetWindowRect(rcLarge);
	pMark->GetWindowRect(rcMark);

	rcSmall = rcLarge;
	rcSmall.bottom = rcMark.top;

	if(bExpand)
		SetWindowPos(NULL, 0,0, rcLarge.Width(), rcLarge.Height(), SWP_NOZORDER|SWP_NOMOVE);
	else
		SetWindowPos(NULL, 0,0, rcSmall.Width(), rcSmall.Height(), SWP_NOZORDER|SWP_NOMOVE);
}

LRESULT CWarningDlg::OnNcHitTest(CPoint point) 
{
	//	return CDialogResize::OnNcHitTest(point);
	UINT hit = CDialogResize::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CWarningDlg::PostNcDestroy() 
{
	m_pDoc->m_pWarningDlg = NULL;
	delete this;

	CDialogResize::PostNcDestroy();
}

void CWarningDlg::OnOK() 
{
//	CDialogResize::OnOK();
//	DestroyWindow();
	OnButtonDelete();
	ShowWindow(false);
}


BOOL CWarningDlg::PreTranslateMessage(MSG* pMsg) 
{
    if( pMsg->message == WM_KEYDOWN )
    {
        if( pMsg->wParam == VK_ESCAPE )  // prevent close dialog
		{
			OnOK();
            return true;
		}
    }
	
	return CDialogResize::PreTranslateMessage(pMsg);
}
