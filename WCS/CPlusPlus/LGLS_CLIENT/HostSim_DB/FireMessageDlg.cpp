

#include "stdafx.h"
#include "Ecs.h"
#include "FireMessageDlg.h"
#include "afxdialogex.h"
#include "JOB_MST.h"
#include "JobCollection.h"
#include "RecordSetWrap.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#define   C_index	0          
#define   C_type	0 
#define   D_not	      100      
#define   C_lampoff	   0 
#define   C_lampon	    1 
#define   C_lampblink	2 


////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllimport) void Usb_Qu_Open();
extern "C" __declspec(dllimport) void Usb_Qu_Close();
extern "C" __declspec(dllimport) int  Usb_Qu_Getstate(); 
extern "C" __declspec(dllimport) bool Usb_Qu_write(byte Qu_index, char Qu_type, char *pData);
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFireMessageDlg, CSkinDialog)

CFireMessageDlg::CFireMessageDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CFireMessageDlg::IDD, pParent)
{
	m_pDoc = pDoc;	
	m_mapView = new CMap<CString, LPCTSTR, CFireParms*, CFireParms*>();
	m_mapViewKey = new CMap<CString, LPCTSTR, CFireParms*, CFireParms*>();
	::AfxGetApp()->GetMainWnd()->GetWindowRect(&m_rect); 
	m_bFlag = FALSE;
	m_bBeep = FALSE;
	m_nLang = -1;
}


CFireMessageDlg::~CFireMessageDlg()
{
	m_pDoc->m_pFireMessageDlg = NULL;
	this->DestroyWindow();
}

BEGIN_MESSAGE_MAP(CFireMessageDlg, CSkinDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_STOP, &CFireMessageDlg::OnBnClickedBtnStop)
	ON_MESSAGE(WM_USER_FIREDIALOG_MESSAGE_STOP, &CFireMessageDlg::OnMessageStopStructure)
	ON_WM_MOVE()
	ON_WM_EXITSIZEMOVE()
	ON_WM_TIMER()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CFireMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX); 

	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edtMessage);
	DDX_Control(pDX, IDC_EDIT_REMARK, m_edtRemarks);
	DDX_Control(pDX, IDC_STATIC_REMARKS, m_lblRemark);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_FIRE_SPD, m_spd);
}

void CFireMessageDlg::KeySwap()
{
	CMap<CString, LPCTSTR, CFireParms*, CFireParms*>* pSwapTemp = NULL;
	pSwapTemp = m_mapViewKey;
	m_mapViewKey = m_mapView;
	m_mapView = pSwapTemp;
}


LRESULT CFireMessageDlg::OnMessageStopStructure(WPARAM wParam, LPARAM lParam)
{ 
	m_pDoc->m_nFireCollect = 2;
	if(m_pDoc == NULL)
	{
		m_pDoc->m_nFireCollect = 2;
		return 0;
	}

	if((int)lParam == 999)
	{
		m_spd.ClearRange(1, 0, 100, 100, TRUE);
		m_pDoc->m_nFireCollect = 2;
		BeepOff();
		return 0;
	}

	m_spd.ClearRange(1, 0, 100, 100, TRUE);

	if(m_nLang != m_pDoc->m_enLang)
	{
		m_nLang = (int)m_pDoc->m_enLang;
		ChangeFont();
	}

	CString strControlId = _T("");

	CList<CFireParms*>* oList = (CList<CFireParms*>*)wParam; 
	for( POSITION pos = oList->GetHeadPosition(); pos != NULL; )
	{
		CFireParms* pf = oList->GetNext(pos);
		CFireParms* pfTemp = NULL;
		if(pf == NULL){continue;}
		if(m_mapViewKey->Lookup(pf->GetKey(), pfTemp) == FALSE)
		{ 
			//first flag
			m_mapView->SetAt(pf->GetKey(), pf); 
			//1112029215
			if(pf->strWH_TYP == _T("10"))
			{
				strControlId.Format(_T("11120%05s"), pf->strEQP_NO);
			}
		}
		else
		{ 
			m_mapView->SetAt(pfTemp->GetKey(), pfTemp); 
		}
	}

	for( POSITION pos = m_mapViewKey->GetStartPosition(); pos != NULL; )
	{		
		CFireParms* pTemp = NULL;
		CFireParms* pViewTemp = NULL;
		CString strKeyTemp = _T("");
		m_mapViewKey->GetNextAssoc(pos, strKeyTemp, pTemp);
		m_mapView->Lookup(pTemp->GetKey(), pViewTemp);
		if(pViewTemp == NULL)
		{
			if(pTemp != NULL){ delete pTemp; };
		}
	}
	m_mapViewKey->RemoveAll();

	BOOL isShow = FALSE;
	int nIdxRow = 0;
	int nLang = (int)m_pDoc->m_enLang;
	m_spd.SetText(1,0,variant_t(GetColumnText(_T("no"), nLang)));
	m_spd.SetText(2,0,variant_t(GetColumnText(_T("whtyp"), nLang)));
	m_spd.SetText(3,0,variant_t(GetColumnText(_T("plcno"), nLang)));
	m_spd.SetText(4,0,variant_t(GetColumnText(_T("eqpno"), nLang)));
	m_spd.SetText(5,0,variant_t(GetColumnText(_T("cellno"), nLang)));
	m_spd.SetText(6,0,variant_t(GetColumnText(_T("message"), nLang)));
	m_spd.SetText(7,0,variant_t(GetColumnText(_T("priority"), nLang)));
	m_spd.SetText(8,0,variant_t(GetColumnText(_T("ok"), nLang)));

	for( POSITION pos = m_mapView->GetStartPosition(); pos != NULL; )
	{
		CFireParms* pFView = NULL;
		CString strKeyTemp2 = _T("");
		m_mapView->GetNextAssoc(pos, strKeyTemp2, pFView);
		if(pFView == NULL){ continue; }
		nIdxRow++;
		m_spd.SetText(1, nIdxRow, variant_t(pFView->GetKey()));
		m_spd.SetText(2, nIdxRow, variant_t(pFView->strWH_TYP_NM + _T("   ")));
		m_spd.SetText(3, nIdxRow, variant_t(pFView->strPLC_NO + _T("   ")));
		m_spd.SetText(4, nIdxRow, variant_t(pFView->strEQP_NO + _T("   ")));
		m_spd.SetText(5, nIdxRow, variant_t(pFView->strCELL_NO + _T("   ")));
		m_spd.SetText(6, nIdxRow, variant_t(pFView->strMESSAGE + _T("   ")));
		m_spd.SetText(7, nIdxRow, variant_t(pFView->strLEVEL));
		CString strTemp = pFView->IsStop() ? GetIniText(_T("ok"), nLang) : GetIniText(_T("nok"), nLang);
		strTemp += _T("    ");
		m_spd.SetText(8, nIdxRow, variant_t(strTemp));
		if(pFView->IsStop() == FALSE) 
		{
			isShow = TRUE; 
		};
	}
	m_spd.Invalidate(1);
	if(isShow == TRUE)
	{ 
		ShowDlg(); 
		BeepOn(strControlId);
	}

	//SWAP
	KeySwap();
	m_pDoc->m_nFireCollect =  2;
	m_spd.Invalidate();
	return 0;

}

void CFireMessageDlg::ShowDlg()
{
	if(IsWindowVisible() == FALSE)
	{
		CRect Rect;
		CRect PosRect;
		GetWindowRect(&Rect); 

		PosRect.left = ((m_rect.right  - m_rect.left) - Rect.Width())  / 2; 
		PosRect.top  = ((m_rect.bottom - m_rect.top)  - Rect.Height()) / 2; 
		::ShowWindow(wndTop, SW_SHOW); 
		SetWindowPos(&wndTop, PosRect.left, PosRect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW);
	}
}

void CFireMessageDlg::BeepOff()
{
	int nUsbConnected;
	nUsbConnected = Usb_Qu_Getstate(); 
	if(nUsbConnected  > 0)
	{
		Usb_Qu_Open();
		bool b_chk;
		char c_char[6];
		c_char[0] = C_lampoff;
		c_char[1] = C_lampoff;
		c_char[2] = C_lampoff;
		c_char[3] = C_lampoff;
		c_char[4] = C_lampoff;
		c_char[5] = 0;
		b_chk = Usb_Qu_write(C_index,C_type,c_char);
		Usb_Qu_Close();
	}

	if(m_bBeep == TRUE)
	{
		m_spd.SetBackColor(NORMAL);
		::KillTimer(m_hWnd, 1);
		m_bBeep = FALSE;
	}
}

void CFireMessageDlg::BeepOn(CString pstrControlId)
{
	//fireCell
	int nUsbConnected;
	nUsbConnected = Usb_Qu_Getstate(); 
	if(nUsbConnected  > 0)
	{
		Usb_Qu_Open();
		bool b_chk;
		char c_char[6];
		c_char[0] = C_lampon;
		c_char[1] = C_lampblink;
		c_char[2] = C_lampoff;
		c_char[3] = D_not;
		c_char[4] = C_lampoff;
		c_char[5] = 3;
		b_chk = Usb_Qu_write(C_index,C_type,c_char);
	}

	if(m_bBeep == FALSE)
	{
// 		//first FLAG
// 		CEcsLayout* pLayout = m_pDoc->GetSelectedLayout();
// 		if(pLayout == NULL)
// 			return;
// 
// 		pLayout->Test(this, nLeft, nRight, nTop, nBottom);
		if(pstrControlId != _T(""))
		{
			int nLayoutNo = -1;
			CDciControl* pControl = NULL;
			pControl = m_pDoc->GetDciControl_FindAllLayout(pstrControlId, nLayoutNo);
			if(pControl != NULL)
			{
				if(nLayoutNo > 0)
				{
					m_pDoc->OnCommandRangeMainFrameMONITORING(ID_MONITORING_NON + nLayoutNo);
					m_pDoc->m_enSelectedLayout = (EN_LAYOUT)nLayoutNo;
					CEcsLayout* pLayout = m_pDoc->GetSelectedLayout();
					if(pLayout != NULL)
					{
						pLayout->Test(NULL, 
									pControl->m_rcControlL.left, 
									pControl->m_rcControlL.right, 
									pControl->m_rcControlL.top, 
									pControl->m_rcControlL.bottom);
					}			
				}
			}
		}
		::SetTimer(m_hWnd, 1, 100, NULL);
		m_bBeep = TRUE;
	}
	
}

void CFireMessageDlg::HideDlg()
{
	CRect MainRect;
	CRect Rect;
	CRect PosRect;
	::AfxGetApp()->GetMainWnd()->GetWindowRect(&MainRect);   
	GetWindowRect(&Rect); 

	PosRect.left = ((MainRect.right  - MainRect.left) - Rect.Width())  / 2; 
	PosRect.top  = ((MainRect.bottom - MainRect.top)  - Rect.Height()) / 2; 
	SetWindowPos(&wndTop, PosRect.left, PosRect.top, Rect.Width(), Rect.Height(), SWP_SHOWWINDOW);
}

void CFireMessageDlg::OnClose()
{
	//m_pDoc->m_pFireMessageDlg = NULL;
	//CSkinDialog::OnClose();
}

void CFireMessageDlg::GetCellNo()
{
	int nRowCnt;
	CString strSql;
	CString strMessage;
	CString strCELL_SC_NO;
	CString strCELL_NO;
	CString strAGING_TYP;
	CString strFireMessage;

	strSql.Format(_T("	SELECT CELL_SC_NO, CELL_NO, SUBSTR(AGING_TYP,1,3) AS AGING_TYP  \n")
     			  _T("    FROM CELL_MST													\n")
				  _T("   WHERE WH_TYP = '%s'											\n")
				  _T("     AND FIRE_OD_RQ_YN = 'Y'										\n"), m_pDoc->m_WH_TYP);

	_RecordsetPtr  pRsptr  = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap( pRsptr); 

	pRsw->MoveFirst(); 

	for(int i = 0; i < nRowCnt; i++)
	{
		strCELL_SC_NO = pRsw->GetItem(_T("CELL_SC_NO"));
		strCELL_NO = pRsw->GetItem(_T("CELL_NO"));
		strAGING_TYP = pRsw->GetItem(_T("AGING_TYP"));

		switch ( CConvert::ToInt(strAGING_TYP) )
			{
			case	291:
				SetDlgItemText(IDC_BTN_AGING_TYP_1, strCELL_NO);
				break;

			case	292:
				SetDlgItemText(IDC_BTN_AGING_TYP_2, strCELL_NO);
				break;
				
			case	293:
				SetDlgItemText(IDC_BTN_AGING_TYP_3, strCELL_NO);
				break;
		
			case	294:
				SetDlgItemText(IDC_BTN_AGING_TYP_4, strCELL_NO);
				break;
			}


		pRsw->MoveNext();
	}
	delete pRsw;
	//return true;
}

void CFireMessageDlg::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_firemessage\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_firemessage\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnStop.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnStop.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("delete"), strExtension)), NULL, 5, 5);
	m_btnStop.GetWindowRect(&rc);
	ScreenToClient(&rc);

	m_spd.Invalidate(TRUE);
	RECT rc2;
	GetWindowRect(&rc2);
	m_btnStop.MoveWindow(/*rc2.right - rc.left - szLarge.cx - 20*/700, 680/*rc2.bottom + 10*/, szLarge.cx, szLarge.cy);
}



BOOL CFireMessageDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	UpdateData(0);
	m_spd.put_ColWidth(1, 0);
	m_spd.put_ColWidth(2, 25);
	m_spd.put_ColWidth(3, 0);
	m_spd.put_ColWidth(4, 25);
	m_spd.put_ColWidth(5, 35);
	m_spd.put_ColWidth(6, 0);
	m_spd.put_ColWidth(7, 0);
	m_spd.put_ColWidth(8, 10);
	m_spd.SetMaxCols(8);
	m_spd.SetMaxRows(9);
	m_spd.SetAutoSize(TRUE);
	m_spd.Invalidate(TRUE);
	
	if(m_nLang != m_pDoc->m_enLang)
	{
		m_nLang = (int)m_pDoc->m_enLang;
		ChangeFont();
	}

	//PlaySound(strSoundPath, AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFireMessageDlg::RenameResource( EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_firemessage\\"), _T("dlg_firemessage"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_firemessage\\"), _T("dlg_firemessage"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("ok"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_STOP, strValue);
}
void CFireMessageDlg::OnBnClickedBtnStop()
{
	variant_t val;
	int nActiveRow = m_spd.GetActiveRow();
	CString strValue = _T("");
	CFireParms* pParm = NULL;

	m_spd.GetText(1, nActiveRow, &val);
	strValue = (LPCTSTR)(_bstr_t)val; //key
	m_mapViewKey->Lookup(strValue, pParm);
	if(pParm == NULL){return;}
	pParm->Stop();
	BeepOff();
}


void CFireMessageDlg::OnMove(int x, int y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSkinDialog::OnMove(x, y);
}


void CFireMessageDlg::OnExitSizeMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	GetWindowRect(&m_rect);
	CSkinDialog::OnExitSizeMove(0, 0);
}

CString CFireMessageDlg::GetColumnText( CString pstrColName, int pnLang )
{
	CString strReturnValue = GetIniText(pstrColName, pnLang);
	if(strReturnValue == _T("")){ return pstrColName; };
	return strReturnValue + _T("  ");
}

CString CFireMessageDlg::GetIniText( CString pstrKey, int pnLang )
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_firemessage\\"), _T("dlg_firemessage"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, pstrKey, pnLang);

	return strValue;
}


void CFireMessageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int sCOUNTER = 0;
	if((sCOUNTER / 5) == 1)
	{
		if(nIDEvent == 1) // 깜빡임 이벤트 //100ms당 들어옴
		{
			CDC* dc;
			dc = GetDC();
			HDC hdc = *dc;
			if(m_bFlag == TRUE)
			{
				m_spd.SetBackColor(RED);
				m_bFlag = FALSE;
			}
			else
			{
				m_spd.SetBackColor(NORMAL);
				m_bFlag = TRUE;
			}
		}
		sCOUNTER = 0;
	}
	sCOUNTER++;
}

void CFireMessageDlg::ChangeFont()
{
	int nFontSize = (m_nLang == 0 ? 30 : 25);
	m_spd.SetFontName(Global.GetFontName(m_pDoc->m_enLang));
	m_spd.SetFontSize(nFontSize);

	RedrawImage();
	RenameResource(m_pDoc->m_enLang);
	m_spd.Invalidate(TRUE);

}


//BOOL CFireMessageDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//}
