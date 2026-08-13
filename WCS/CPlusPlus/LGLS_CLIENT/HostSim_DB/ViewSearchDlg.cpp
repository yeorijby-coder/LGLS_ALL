// ViewSearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "EcsDoc.h"
#include "ViewSearchDlg.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"

// CViewSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewSearchDlg, CSkinDialog)

CViewSearchDlg::CViewSearchDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewSearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewSearchDlg::CViewSearchDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewSearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CViewSearchDlg::~CViewSearchDlg()
{
	this->DestroyWindow();
}

void CViewSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edtSeach);
	DDX_Control(pDX, IDC_LBL_SEARCH_TYP, m_lblSearchTyp);
	DDX_Control(pDX, IDC_CMB_SEARCH_TYP, m_cmbSearchTyp);
	DDX_Control(pDX, ID_BTN_SEARCH, m_btnSeach);
	DDX_Control(pDX, IDC_GRP_SEARCH,	m_grpSearchBox);
}


BEGIN_MESSAGE_MAP(CViewSearchDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTN_SEARCH, &CViewSearchDlg::OnSetSearchCv)
END_MESSAGE_MAP()


BOOL CViewSearchDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		m_bInitialized = TRUE;		
		RelocationControls();
	}
	CLib::BindCombo(m_cmbSearchTyp, _T("SEARCH_TYP"), m_pDoc, (int)pEn);

	RedrawImage();
	RenameResource(pEn);
	Invalidate(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CViewSearchDlg::BindCombo(CComboBoxWrapper& cbx)
{
	cbx.ResetContent();

	cbx.SetItemDataEx(0, _T("0"));
	cbx.AddString(_T("TRACK_NO"));

	cbx.SetItemDataEx(1, _T("1"));
	cbx.AddString(_T("LUGG_NO"));

	//cbx.SetItemDataEx(2, _T("2"));
	//cbx.AddString(_T("TRAY_NO"));

	cbx.SetCurSel(0);
}


void CViewSearchDlg::OnClose()
{
	m_pDoc->m_pViewSearchDlg = NULL;
	CSkinDialog::OnClose();
}

void CViewSearchDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CViewSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewSearchDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CViewSearchDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;


	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeWideLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_WIDELARGE);

	CRect rc2;
	m_btnSeach.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnSeach.MoveWindow(rc.left, rc.top, sizeLarge.cx, sizeLarge.cy);
}


BOOL CViewSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)  
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			{
				break;
			}
		case VK_ESCAPE:
			{
				break;
			}
		}
	}
	return CSkinDialog::PreTranslateMessage(pMsg);
}

void CViewSearchDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_search\\"), _T("dlg_search"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(_T("찾기			Ecs - 2021.02.18 (V.01)"));

	//
	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_search\\"), _T("dlg_search"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("searchtyp"), (int)m_enLang);
	SetDlgItemText(IDC_LBL_SEARCH_TYP, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_search\\"), _T("dlg_search"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("search"), (int)m_enLang);
	SetDlgItemText(ID_BTN_SEARCH, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_search\\"), _T("dlg_search"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("select"), (int)m_enLang);
	SetDlgItemText(IDC_GRP_SEARCH, strValue);
}

void CViewSearchDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_search\\20x20\\");
	CString strExtension = _T(".png");

	m_btnSeach.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0 ,0);
	m_btnSeach.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("search"), strExtension)), NULL, 5, 5);
}

void CViewSearchDlg::OnSetSearchCv()
{
	CString strChkName, strTemp;
	CString strPreTrack, strTrack;
	CString strSearchTyp;
	CTrackInfo* pTrackInfo = NULL;
	
	strPreTrack = m_pDoc->m_strSeachTrack;

	m_edtSeach.GetWindowText(strTemp);

	strSearchTyp = m_cmbSearchTyp.GetItemKey(m_cmbSearchTyp.GetCurSel());

	strTrack = GetTrackNo(strTemp, strSearchTyp);

	if (strPreTrack != _T(""))
	{
		pTrackInfo = m_pDoc->GetTrackInfoNew(strPreTrack);
		pTrackInfo->SetSeachColor(pTrackInfo->m_pTrackCtrl, true);
	}

	if (strTrack == _T(""))
	{
		return;
	}

	pTrackInfo = m_pDoc->GetTrackInfoNew(strTrack);
	pTrackInfo->SetSeachColor(pTrackInfo->m_pTrackCtrl, false);
	
	CString strDciControl = _T("17110");
	int nMonitor = ID_MONITORING_NON;
	strDciControl = strDciControl + strTrack;
	int nLeft;
	int nRight;
	int nTop;
	int nBottom;

	//if (m_pDoc->m_layout.GetDciControl(strDciControl) != NULL)
	//{
	//	nMonitor += 1;
	//	nLeft	= m_pDoc->m_layout.GetDciControl(strDciControl)->m_rcControlL.left;
	//	nRight	= m_pDoc->m_layout.GetDciControl(strDciControl)->m_rcControlL.right;
	//	nTop	= m_pDoc->m_layout.GetDciControl(strDciControl)->m_rcControlL.top;
	//	nBottom = m_pDoc->m_layout.GetDciControl(strDciControl)->m_rcControlL.bottom;
	//}
	//else // 없는 트랙번호 일 때
	//{
	//	return;
	//}

	CEcsLayout* pLayout = m_pDoc->GetSelectedLayout();

	if (pLayout == NULL)
		return;

	if (pLayout->GetDciControl(strDciControl) != NULL)
	{
		nMonitor += 1;
		nLeft	= pLayout->GetDciControl(strDciControl)->m_rcControlL.left;
		nRight	= pLayout->GetDciControl(strDciControl)->m_rcControlL.right;
		nTop	= pLayout->GetDciControl(strDciControl)->m_rcControlL.top;
		nBottom = pLayout->GetDciControl(strDciControl)->m_rcControlL.bottom;
	}
	else // 없는 트랙번호 일 때
	{
		return;
	}


	m_pDoc->m_strSeachTrack = strTrack;

	
	m_pDoc->OnCommandRangeMainFrameMONITORING(nMonitor);
	

	//CEcsLayout* pLayout = m_pDoc->GetSelectedLayout();
	//if (pLayout == NULL)
	//	return;

	pLayout->Test(this, nLeft, nRight, nTop, nBottom);

	CString strLOG_LUGG_NO = _T("");
	if (strLOG_LUGG_NO == _T("")) { strLOG_LUGG_NO = _T("0"); }
	CString strLOG_BOTTOM_TRAY = _T("");
	CString strLOG_TOP_TRAY = _T("");
	CString strLOG_MSG = _T("SEARCH");
	m_pDoc->GetQueryInsertClientLog(_T("CViewSearchDlg"), strLOG_LUGG_NO, strLOG_BOTTOM_TRAY, strLOG_TOP_TRAY, strLOG_MSG);

}

CString CViewSearchDlg::GetTrackNo(CString pTemp, CString pChkName)
{
	int nRowCnt = 0;
	int nTemp = 0;
	CString strTemp;
	CString strMessage = _T("");
	CString strSql = _T("");
	CString strTrack = _T("");

	if (pChkName == _T("0")) //TRACK_NO
	{
		nTemp = CConvert::ToInt(pTemp);
		strTemp.Format(_T("%03d"), nTemp);
	
		strSql.Format(_T("		SELECT MC_NO							\n")
				      _T("		  FROM CV_DATA							\n")
					  _T("		 WHERE WH_TYP = '%s'					\n")
					  _T("		   AND MC_NO = '%s'					\n")
#if ORACLE
					  _T("         AND ROWNUM <= 1					\n"), m_pDoc->m_WH_TYP, strTemp);
#endif
#if POSTGRESQL
					  _T("		 LIMIT 1         						\n"), m_pDoc->m_WH_TYP, strTemp);
#endif
	}
	else if (pChkName == _T("1")) //LUGG_NO
	{
		nTemp = CConvert::ToInt(pTemp);
		strTemp.Format(_T("%04d"), nTemp);

		strSql.Format(_T("		SELECT MC_NO							\n")
				      _T("		  FROM CV_DATA CD 	 					\n")
					  _T("		 WHERE CD.WH_TYP = '%s'					\n")
					  _T("		   AND CD.LUGG_NO_RD = '%s'				\n")
#if ORACLE
					  _T("         AND ROWNUM <= 1					\n"), m_pDoc->m_WH_TYP, strTemp);
#endif
#if POSTGRESQL
					  _T("		 LIMIT 1								\n"), m_pDoc->m_WH_TYP, strTemp);
#endif
	}
	else if (pChkName == _T("2")) //TRAY_NO
	{
		if (pTemp.GetLength() != 6)
			return _T("");

		strTemp = pTemp;

		strSql.Format(_T("		SELECT MC_NO							\n")
				      _T("		  FROM CV_DATA CD JOIN JOB_MST JM 	 	\n")
					  _T("		    ON CD.LUGG_NO_RD = JM.LUGG_NO		\n")
					  _T("		   AND CD.WH_TYP = JM.WH_TYP			\n")
					  _T("		 WHERE CD.WH_TYP = '%s'					\n")
					  _T("		   AND (JM.BCR_BOTTOM = '%s'			\n")
					  _T("		    OR  JM.BCR_TOP = '%s')				\n")
#if ORACLE
					  _T("         AND ROWNUM <= 1					\n"), m_pDoc->m_WH_TYP, strTemp, strTemp);
#endif
#if POSTGRESQL
					  _T("		 LIMIT 1								\n"), m_pDoc->m_WH_TYP, strTemp, strTemp);
#endif
	}

	_RecordsetPtr pRsp = m_pDoc->GetSelectQryRecordsetPtr(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsp); 
	

	if (pRsw == NULL) { return 0; }

	pRsw->MoveFirst();
	
	for( int nIdxRow = 0; nIdxRow < nRowCnt; nIdxRow++ )
	{
		strTrack = pRsw->GetItem(_T("MC_NO"));
	
		pRsw->MoveNext();
	}

	if (pRsw != NULL) {	delete pRsw; }
	return strTrack;
}