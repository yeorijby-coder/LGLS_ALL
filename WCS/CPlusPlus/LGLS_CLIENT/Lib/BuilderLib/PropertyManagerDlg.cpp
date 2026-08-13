// PropertyManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyManagerDlg.h"
#include "spreadsheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyManagerDlg dialog


CPropertyManagerDlg::CPropertyManagerDlg(CBuildManager* pBuilder, CWnd* pParent /* = NULL */)
	: CDialog(CPropertyManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropertyManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBuilder = pBuilder;
	ASSERT(m_pBuilder);
}


void CPropertyManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyManagerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CPropertyManagerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyManagerDlg message handlers

BOOL CPropertyManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CSpreadSheet* pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD1);
	CDciGridCtrl* pDciGridCtrl = m_pBuilder->m_pDciGridCtrl;
	CDciPropertyArray properties;
	pDciGridCtrl->UpdatePropNames(properties);
	pDciGridCtrl->UpdatePropValues(properties, FALSE);

	pSpread->SetColWidth(0, 10.0);
	pSpread->SetColWidth(1, 10.0);
	pSpread->SetMaxRows(properties.GetSize());
	pSpread->SetMaxCols(1);
	for (int i=0; i<properties.GetSize(); ++i)
	{
		pSpread->SetRow(i+1);
		pSpread->SetCol(0);
		pSpread->SetText(properties[i].m_strName);

		pSpread->SetRow(i+1);
		pSpread->SetCol(1);
		pSpread->SetText(properties[i].m_strValue);
	}
	pSpread->SetRow(0);
	pSpread->SetText(_T("Grid"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyManagerDlg::OnOK() 
{
	CSpreadSheet* pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD1);
	CDciGridCtrl* pDciGridCtrl = m_pBuilder->m_pDciGridCtrl;
	CDciPropertyArray properties;
	pDciGridCtrl->UpdatePropNames(properties);

	pSpread->SetCol(1);
	for (int i=0; i<properties.GetSize(); ++i)
	{
		pSpread->SetRow(i+1);
		properties[i].m_strValue = pSpread->GetText();
	}

	pDciGridCtrl->UpdatePropValues(properties);
	
	CDialog::OnOK();
}
