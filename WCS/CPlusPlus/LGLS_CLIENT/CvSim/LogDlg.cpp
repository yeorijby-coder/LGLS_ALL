// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"

#include "Log.h"

#include "LogDlg.h"
#include "EcsDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

#define ID_INDICATOR_TOTAL 1

static UINT auIDStatusBar[] = 
{
	ID_SEPARATOR,
	ID_INDICATOR_TOTAL
};

CLogDlg::CLogDlg(CEcsDoc *pDoc, CLog *pLog, CWnd* pParent /*=NULL*/)
	: CDialogResize(CLogDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pLog = pLog;

	//{{AFX_DATA_INIT(CLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_CONTEXT, m_edtContext);
	DDX_Control(pDX, IDC_BUTTON_SORT, m_btnSort);
	DDX_Control(pDX, IDC_LIST_LOG, m_ctlReport);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialogResize)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION, OnSelchangeComboPosition)
	ON_COMMAND(ID_LOG_FILE, OnLogFile)
	ON_COMMAND(ID_LOG_EXIT, OnLogExit)
	ON_COMMAND(ID_LOG_SAVEAS, OnLogSaveas)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTLOOK, OnClickListOutlook)
	ON_COMMAND(ID_REFRESH_LOG, OnRefreshLog)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_OUTLOOK, OnKeydownListOutlook)
	ON_BN_CLICKED(IDC_BUTTON_SORT, OnButtonSort)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CLogDlg)
	DLGRESIZE_CONTROL(AFX_IDW_STATUS_BAR, DLSZ_MOVE_Y | DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(AFX_IDW_TOOLBAR, DLSZ_SIZE_X)

	DLGRESIZE_CONTROL(IDC_BUTTON_SORT, DLSZ_MOVE_X)
	DLGRESIZE_CONTROL(IDC_EDIT_CONTEXT, DLSZ_SIZE_X)

	DLGRESIZE_CONTROL(IDC_LIST_OUTLOOK, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_LIST_LOG, DLSZ_SIZE_X | DLSZ_SIZE_Y)

	DLGRESIZE_CONTROL(IDC_STATIC_EDGE, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_STATIC_TOP, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_STATIC_BOTTOM, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_STATIC_RIGHT, DLSZ_MOVE_X)
END_DLGRESIZE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

BOOL CLogDlg::OnInitDialog() 
{
	SetIcon(AfxGetApp()->LoadIcon(IDI_LOG), TRUE);
	SetIcon(AfxGetApp()->LoadIcon(IDI_LOG), FALSE);

	CDialogResize::OnInitDialog();
	CDialog::CenterWindow();
	
	SetSxButton();

	SetupOutlookBar();
	CreateBarWindow();
	InitResizing(FALSE);

	m_ctlReport.LoadImage(IDI_LOG_JOB, IDI_LOG_EVENT, IDI_LOG_ERROR, IDI_LOG_ALARM, IDI_LOG_DEBUG, -1);
	m_ctlReport.InsertColumn(_T("LOG"), _T("시간"), _T("작업번호"), _T("구분"), _T("TrayID"), _T("내용"), _T("ETX"));

	CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_POSITION);

	CLogEvent LogEvent;
	for (int i=0; i < LOG_POS_CNT; i++)
	{
		pComboBox->InsertString(i, LogEvent.GetPosition(i));
	}
	pComboBox->SetCurSel(0);

	SetRange();

	COleDateTime time = COleDateTime::GetCurrentTime();
	Context.m_strFileName.Format(_T("%s\\%s%.2d.elg"), m_pDoc->m_pConfig->m_strLogPath, m_pLog->m_strLogName, time.GetDay());
	Loading();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogDlg::SetRange()
{
	COleDateTime theTime = COleDateTime::GetCurrentTime();
	Context.m_nFrom = theTime.GetHour();
	Context.m_nTo = Context.m_nFrom + 1;

	CSpinButtonCtrl* pSpinButtonCtrlFrom = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FROM);
	pSpinButtonCtrlFrom->SetRange(0, 23);
	pSpinButtonCtrlFrom->SetPos(Context.m_nFrom);

	CSpinButtonCtrl* pSpinButtonCtrlTo = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TO);
	pSpinButtonCtrlTo->SetRange(1, 24);
	pSpinButtonCtrlTo->SetPos(Context.m_nTo);

	SET_INT(IDC_EDIT_FROM, Context.m_nFrom);
	SET_INT(IDC_EDIT_TO, Context.m_nTo);
}

void CLogDlg::SetSxButton()
{
	m_edtContext.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	m_edtContext.SetTextColor(RGB(255, 0, 0));
	SET_TEXT(IDC_EDIT_CONTEXT, _T("파일 메뉴에서 Logging 파일을 선택 하세요"));

	m_btnSort.setBitmapId(IDB_SEARCH);
	m_btnSort.setGradient(true);
	m_btnSort.setType(CFooButton::Type::pushButtonMulti/*hotPushButtonMulti*/);
}

void CLogDlg::Loading() 
{
	Context.m_nFrom = GET_INT(IDC_EDIT_FROM);
	Context.m_nTo = GET_INT(IDC_EDIT_TO);

	GET_TEXT(IDC_EDIT_CONTEXT, Context.m_strContext);

	if ( !Context.IsValidContext() ) return;

	m_pLog->Load(&m_ctlReport, &Context);

	SetStatusBarPane();
}

void CLogDlg::SetStatusBarPane()
{
	CString strText;
	CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_LOG);
	strText.Format(_T("총 %d건"), pListCtrl->GetItemCount());

	CClientDC dc(this);
	CSize size = dc.GetTextExtent(strText);

	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_TOTAL);

	m_wndStatusBar.SetPaneText(0, Context.m_strFileName);
	m_wndStatusBar.SetPaneText(nIndex, strText, TRUE);
	m_wndStatusBar.SetPaneInfo(nIndex, ID_INDICATOR_TOTAL, SBPS_NORMAL, size.cx);
}

void CLogDlg::OnSelchangeComboPosition() 
{
	CComboBox* pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_POSITION);
	Context.m_nPos = pComboBox->GetCurSel();
	Loading();	
}

void CLogDlg::OnClickListOutlook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_OUTLOOK);
	int nIndex = pListCtrl->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;
	
	Context.m_nType = nIndex;

	Loading();

	*pResult = 0;
}

void CLogDlg::OnKeydownListOutlook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_OUTLOOK);
	int nIndex = pListCtrl->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;

	if (pLVKeyDow->wVKey == VK_DOWN)
	{
		if ( (pListCtrl->GetItemCount()-1) > nIndex)
			Context.m_nType = nIndex+1;
	}
	else
	if (pLVKeyDow->wVKey == VK_UP)
	{
		if (nIndex > 0)
			Context.m_nType = nIndex-1;
	}
	else
	if ((pLVKeyDow->wVKey == VK_NEXT) || (pLVKeyDow->wVKey == VK_END))
	{
		Context.m_nType = pListCtrl->GetItemCount()-1;
	}
	else
	if ((pLVKeyDow->wVKey == VK_PRIOR) || (pLVKeyDow->wVKey == VK_HOME))
	{
		Context.m_nType = 0;
	}

	Loading();
}

void CLogDlg::OnRefreshLog() 
{
	Loading();	
}

void CLogDlg::OnButtonSort() 
{
	CString strMenu, strTemp;

	if (m_btnSort.isMultiClicked())
	{
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, LOG_SORT_NONE, _T("ALL-검색"));
		menu.AppendMenu(MF_STRING, LOG_SORT_LUGGID, _T("작업번호별 검색"));
		menu.AppendMenu(MF_STRING, LOG_SORT_PRODID, _T("TrayID별 검색"));
		menu.AppendMenu(MF_STRING, LOG_SORT_STR, _T("문자열 검색"));
		menu.CheckMenuItem(Context.m_nSort, MF_BYPOSITION|MF_CHECKED);
		Context.m_nSort = m_btnSort.displayPopupMenu(&menu);

		menu.GetMenuString(Context.m_nSort, strMenu, MF_BYPOSITION);
		strTemp.Format(_T("%s"), strMenu);
		
		SET_TEXT(IDC_BUTTON_SORT, strMenu);

		if ( Context.m_nSort == LOG_SORT_NONE )
			m_edtContext.SetBkColor(::GetSysColor(COLOR_BTNFACE));
		else
			m_edtContext.SetBkColor(WHITE);

		ENABLE_WND(IDC_EDIT_CONTEXT, Context.m_nSort);

		//if (0 != nResult)
		//	PostMessage (WM_COMMAND, nResult);
		
        // Remember to clear the button's multi-click property!
        m_btnSort.clearMultiClick();
    }
	else 
	{
		Loading();	
	}
}

void CLogDlg::OnLogFile() 
{
	CFileDialog FileDlg(TRUE, NULL, _T("*.elg"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T(" ECS Log Files (*.elg) | *.elg ||"));
	FileDlg.m_ofn.lpstrInitialDir = m_pDoc->m_pConfig->m_strLogPath;
	FileDlg.m_ofn.lpstrTitle = _T("ECS 작업기록 파일");
	FileDlg.m_ofn.Flags |= OFN_ALLOWMULTISELECT;

	if (FileDlg.DoModal() == IDOK) 
	{
		Context.m_strFileName = FileDlg.GetPathName();
		Loading();
	}
}

void CLogDlg::OnLogSaveas() 
{
	if (!Context.IsValidContext()) 
		return;

	char szFilter[] = "CSV Files(.csv)|*.csv|Text Files(.txt)|*.txt|";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)szFilter);
	
	dlg.m_ofn.lpstrTitle = _T("ECS 작업기록 저장");
	dlg.m_ofn.lpstrInitialDir = _T(".\\");

	if (dlg.DoModal() == IDCANCEL)
		return;

	DWORD dwFilterIndex = dlg.m_ofn.nFilterIndex;
	CString strPathName = dlg.GetPathName();
	CString strFileName = dlg.GetFileName();

	if (strFileName.IsEmpty())
		return;

	if (dlg.GetFileExt().IsEmpty())
	{
		if (dwFilterIndex == 1 && strFileName.Right(4) != _T(".csv"))
			strPathName += _T(".csv");
		else if (dwFilterIndex == 2 && strFileName.Right(4) != _T(".txt"))
			strPathName += _T(".txt");
	}

	CFile* pFile;
	CString str;
	TRY
	{
		//const char* pFileName = strPathName;
		pFile = new CFile((LPCTSTR)strPathName, CFile::modeCreate | CFile::modeWrite);
	}
	CATCH (CFileException, e)
	{
		str.Format(_T("ECS 작업기록 저장중 오류가 발생했습니다. 재시도해 주십시오. (%d)"), e->m_cause);
		AfxMessageBox(str);
		return;
	}
	END_CATCH

	// CSV
	if (dwFilterIndex == 1)
	{
		TCHAR szBuffer[1024];

		::memset(&szBuffer, 0x00, sizeof(szBuffer));
		wsprintf(szBuffer, _T("%s,%s,%s,%s,%s\r\n"), _T("시간"), _T("작업번호"), _T("구분"), _T("TRAY ID"), _T("내용"));
		pFile->Write(szBuffer, wcslen(szBuffer));

		CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_LOG);

		for (int i = 0; i < pListCtrl->GetItemCount(); i++)
		{
			wsprintf(szBuffer, _T("%s,%s,%s,%s,%s\r\n"), 
				pListCtrl->GetItemText(i, 1), 
				pListCtrl->GetItemText(i, 2), 
				pListCtrl->GetItemText(i, 3), 
				pListCtrl->GetItemText(i, 4),
				pListCtrl->GetItemText(i, 5));
			pFile->Write(szBuffer, wcslen(szBuffer));
		}
	}
	// TXT
	else if (dwFilterIndex == 2)
	{
		TCHAR szBuffer[1024];

		wsprintf(szBuffer, _T("Logging file dated %s........\r\n\r\n"), Context.m_strFileName);
		pFile->Write(szBuffer, wcslen(szBuffer));
		wsprintf(szBuffer, _T("  시간   작업번호   구분      TRAY ID    내용\r\n"));
		pFile->Write(szBuffer, wcslen(szBuffer));
		wsprintf(szBuffer, _T("======== ======== ======== ============= ====================================================================================>>>\r\n"));
		pFile->Write(szBuffer, wcslen(szBuffer));

		CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_LOG);

		for (int i = 0; i < pListCtrl->GetItemCount(); i++)
		{
			wsprintf(szBuffer, _T("%s %-8s %-8s %-13s %s\r\n"), 
				pListCtrl->GetItemText(i, 1), 
				pListCtrl->GetItemText(i, 2), 
				pListCtrl->GetItemText(i, 3), 
				pListCtrl->GetItemText(i, 4),
				pListCtrl->GetItemText(i, 5));
			pFile->Write(szBuffer, wcslen(szBuffer));
		}
	}
	delete pFile;
}

void CLogDlg::PrintTitle(CDC* pDC, int nPageNum)
{
	TCHAR Buff[1024];

	wsprintf(Buff, _T("Time     Lugg Pos   Content........................................< Page %d >"), nPageNum);
	pDC->TextOut(10, 300, (LPCTSTR)Buff, wcslen(Buff));
	wsprintf(Buff, _T("======== ==== ===== =======================================================>>>"));

	pDC->TextOut(10, 400, (LPCTSTR)Buff, wcslen(Buff));
}

// void CLogDlg::OnLogPrint() 
// {
// 	if ( !Context.IsValidContext() ) return;
// 
// 	CPrintDialog PrintDlg(TRUE);
// 
// 	if (PrintDlg.DoModal() != IDOK)
// 		return;
// 
// 	HDC hdc = PrintDlg.CreatePrinterDC();
// 	if (hdc == NULL)
// 	{
// 		AfxMessageBox(_T("프린터 드라이버가 정상적으로 작동하지 않습니다..! ");
// 		return;
// 	}
// 
// 	CDC* pDC = CDC::FromHandle(hdc);
// 
// 	DOCINFO lpdi;
// 	lpdi.cbSize = sizeof(DOCINFO);
//     lpdi.lpszDocName = "EcsLog";
//     lpdi.lpszOutput = NULL;
// 
// 	pDC->StartDoc(&lpdi);
// 	pDC->StartPage();
// 
//     CString strTemp;
// 	char Buff[1024];
// 
// 	swprintf(Buff, "Logging file dated %s........", Context.m_strFileName);
// 	pDC->TextOut(10, 100, Buff, strlen(Buff));
// 
// 	CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_LOG);
// 
// 	for (int i = 0, nLine, nPageNum = 1; i < pListCtrl->GetItemCount(); i++)
// 	{
// 		if (i % 62 == 0)
// 		{
// 			nLine = 0;
// 			if (nPageNum != 1)
// 			{
// 				pDC->EndPage();
// 				pDC->StartPage();
// 			}
// 			PrintTitle(pDC, nPageNum);
// 			nPageNum++;
// 		}
// 
// 		wsprintf(Buff, _T("%s %-4s %-4s %s"), 
// 			pListCtrl->GetItemText(i, 1), 
// 			pListCtrl->GetItemText(i, 2), 
// 			pListCtrl->GetItemText(i, 3), 
// 			pListCtrl->GetItemText(i, 4));
// 		pDC->TextOut(10, 500 + nLine++ * 100, Buff, strlen(Buff));
// 	}
// 
// 	pDC->EndPage();
// 	pDC->EndDoc();
// 	DeleteDC(hdc);
// 
// 	AfxMessageBox(_T("로그파일 프린트 완료.."));
// }

BOOL CLogDlg::CreateBarWindow()
{
	// Create the Toolbar and attach the resource
	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_LOGGINGBAR))
	{
		TRACE0("Failed to Create Dialog Toolbar\n");
		return FALSE;	
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |CBRS_LEFT | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_GRIPPER);
	m_wndToolBar.ModifyStyle(0, m_wndToolBar.GetStyle()|TBSTYLE_FLAT);

	// Create status bar at the bottom of the dialog window
	if (m_wndStatusBar.Create(this))
	{
		m_wndStatusBar.SetIndicators(auIDStatusBar, sizeof(auIDStatusBar)/sizeof(UINT));
		
		m_wndStatusBar.SetPaneText(0, _T("Ecs Log Viewer"), TRUE);
		// Make a sunken or recessed border around the first pane
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL );

//		SetOnlineLed(FALSE);
//		SetOfflineLed(TRUE);

//		m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_ONLINELED), ID_INDICATOR_ONLINELED, SBPS_NOBORDERS, 14);
//		m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINELED), ID_INDICATOR_OFFLINELED, SBPS_NOBORDERS, 14);
	}

    // We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

    // Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top); 

	CRect  rcChild;					
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{                               
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    return TRUE;
}

void CLogDlg::SetupOutlookBar()
{

	CEcsApp* pApp = (CEcsApp *)AfxGetApp();
	CImageList imageList;
	CListCtrl* pListCtrl = (CListCtrl *)GetDlgItem(IDC_LIST_OUTLOOK);
	
	// 리소스를 줄이기 위해서 
	imageList.Create(32, 32, ILC_COLOR16|ILC_MASK, 1, 4);
	imageList.Add(pApp->LoadIcon(IDI_LOG_ALL));
	imageList.Add(pApp->LoadIcon(IDI_LOG_JOB));
	imageList.Add(pApp->LoadIcon(IDI_LOG_EVENT));
	imageList.Add(pApp->LoadIcon(IDI_LOG_ERROR));
	imageList.Add(pApp->LoadIcon(IDI_LOG_ALARM));
	imageList.Add(pApp->LoadIcon(IDI_LOG_DEBUG));
	pListCtrl->SetImageList(&imageList, LVSIL_NORMAL);
	imageList.Detach();

	CRect rc;
	pListCtrl->GetClientRect(rc);
	
	pListCtrl->SetIconSpacing(rc.Width(), 64);

	pListCtrl->SetTextColor(RGB(255,255,255));
	pListCtrl->SetTextBkColor(RGB(128,128,128));
	pListCtrl->SetBkColor(RGB(128,128,128));	

	pListCtrl->InsertColumn(0, _T("OutlookBar"));

	pListCtrl->InsertItem(0, _T("전체"), 0);
	pListCtrl->InsertItem(1, _T("작업"), 1);
	pListCtrl->InsertItem(2, _T("이벤트"), 2);
	pListCtrl->InsertItem(3, _T("에러"), 3);
	pListCtrl->InsertItem(4, _T("알람"), 4);
	pListCtrl->InsertItem(5, _T("디버그"), 5);
}

/////////////////////////////////////////////////////////////////////////////////
// <MODALESS DIALOG> 구현시 반드시 구현해야함

void CLogDlg::PostNcDestroy() 
{
//	m_pDoc->m_pLogDlg = NULL;
	delete this;
}

void CLogDlg::OnLogExit() 
{
	DestroyWindow();
//	CDialogResize::OnCancel();
}

void CLogDlg::OnClose() 
{
	DestroyWindow();
//	CDialogResize::OnClose();
}

void CLogDlg::OnCancel()
{
	DestroyWindow();
//	CDialog::OnCancel();
}

void CLogDlg::OnOK()
{
	DestroyWindow();
//	CDialog::OnOK();
}
