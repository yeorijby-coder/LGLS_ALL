//TrackDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "TrackDataDlg.h"

#include "EcsDoc.h"
#include "EcsControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////
// CTrackDataDlg dialog


CTrackDataDlg::CTrackDataDlg(CEcsDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialogResize(CTrackDataDlg::IDD, pParent)
{
	m_pDoc = pDoc;
}

void CTrackDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrackDataDlg, CDialogResize)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTN_APPLY, &CTrackDataDlg::OnBnClickedBtnApply)
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CTrackDataDlg)
	DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
END_DLGRESIZE_MAP()


////////////////////////////////////////////////
// CTrackDataDlg message handlers

BOOL CTrackDataDlg::OnInitDialog()
{
	CDialogResize::OnInitDialog();

	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(&wndTopMost, 300, 300, rectWnd.Width(), rectWnd.Height(), SWP_NOMOVE | SWP_NOACTIVATE);
	return TRUE;
}

void CTrackDataDlg::ValueChange(CString Type, CString Addr, CString Name, CString Value)
{
	SET(IDC_KEYWORD_TYPE, Type);
	SET(IDC_KEYWORD_ADDR, Addr);
	SET(IDC_KEYWORD_NAME, Name);
	SET(IDC_KEYWORD_VALUE, Value);
}

void CTrackDataDlg::PostNcDestroy()
{
	m_pDoc->m_pTrackDataDlg = NULL;
	delete this;

	CDialogResize::PostNcDestroy();
}

void CTrackDataDlg::OnOK()
{
	ShowWindow(false);
}

#define		SET_REG_INFO(name, data, option)					m_pDoc->SetAddrByName(m_nNumber - 1, nDevNum, name, data, option)
#define		GET_REG_INFO(name)									m_pDoc->GetAddrByName(m_nNumber - 1, nDevNum, name)


void CTrackDataDlg::OnBnClickedBtnApply()
{
	CString Type, Name, Value;

	int m_nNumber = m_pDoc->m_nTempPlcNum;
	int nDevNum = m_pDoc->m_nTempDevNum;

	GET(IDC_KEYWORD_TYPE, Type);
	GET(IDC_KEYWORD_NAME, Name);
	GET(IDC_KEYWORD_VALUE, Value);

	int nValue = _ttoi(Value);

	// Vallidation

	if (Type == "b")
	{
		CString strParent = m_pDoc->GetParentWord(Name);
		int nParent;
		nParent = GET_REG_INFO(strParent);

		SET_REG_INFO(Name, nParent, nValue ? 1 : 4);
	}
	else
		SET_REG_INFO(Name, _ttoi(Value), 0);

	m_pDoc->m_oControlDlg.UpdateList(m_nNumber, nDevNum);
	m_pDoc->m_oControlDlg.UpdateBits(m_nNumber, nDevNum);
	m_pDoc->m_oControlDlg.UpdateTrackData(m_nNumber, nDevNum);
	m_pDoc->m_oControlDlg.UpdateTrack(m_nNumber, (m_nNumber * 100) + (nDevNum / m_pDoc->m_nWordCnt));
//	if (m_nNumber == 1)
//		m_pDoc->m_oControlDlg.UpdateTrack(m_nNumber, nDevNum / 2);
//	else
//		m_pDoc->m_oControlDlg.UpdateTrack(m_nNumber, (m_nNumber - 1) * 100 + nDevNum / 10);
	
	ShowWindow(false);
}
