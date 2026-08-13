// MessageDisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "MessageDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageDisplayDlg dialog


CMessageDisplayDlg::CMessageDisplayDlg(CString strData, CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDisplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageDisplayDlg)
	m_strMessage = _T("");
	//}}AFX_DATA_INIT
}


void CMessageDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageDisplayDlg)
	DDX_Text(pDX, IDC_EDIT_DISPLAY, m_strMessage);
	DDV_MaxChars(pDX, m_strMessage, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDisplayDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageDisplayDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDisplayDlg message handlers

BOOL CMessageDisplayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//LOGFONT m_logfont;
	//CFont font ;
	//CFont * pOldFont;

	//memset(&m_logfont, 0, sizeof(LOGFONT));
	//m_logfont.lfQuality = PROOF_QUALITY;
	//m_logfont.lfHeight = 20;
	//lstrcpy(m_logfont.lfFaceName, _T("System") );

	//font.CreateFontIndirect(&m_logfont);

	//GetDlgItem(IDC_EDIT_DISPLAY)->SetFont(&font);

	m_font.CreateFont( 80, // nHeight 
		20, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		1, // nWeight 
		0, // bItalic 
		0, // bUnderline 
		0, // cStrikeOut 
		0, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		0,                              // nClipPrecision 
		DEFAULT_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
		_T("±¼¸²") ); // lpszFacename 
	GetDlgItem(IDC_EDIT_DISPLAY)->SetFont(&m_font);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMessageDisplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
   
    switch(nCtlColor)
    {
    case CTLCOLOR_EDIT:
        {
			if(pWnd->GetDlgCtrlID() == IDC_EDIT_DISPLAY)
                pDC->SetTextColor(RGB(255, 0, 0));
                pDC->SetBkColor(RGB(255, 228, 0));
        }
    }
    // TODO: Return a different brush if the default is not desired
    return hbr;
}
