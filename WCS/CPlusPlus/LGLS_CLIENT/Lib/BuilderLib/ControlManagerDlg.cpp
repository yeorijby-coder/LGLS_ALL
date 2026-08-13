// ControlManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ControlManagerDlg.h"
#include "spreadsheet.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FPS_FONT_NAME		_T("tahoma")
#define FPS_FONT_SIZE		9.0
#define FPS_FONT_BOLD		FALSE
#define FPS_COL_WIDTH_GAP	1.0

/////////////////////////////////////////////////////////////////////////////
// CControlManagerDlg dialog


CControlManagerDlg::CControlManagerDlg(CBuildManager* pBuilder, CWnd* pParent /* = NULL */)
	: CDialog(CControlManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlManagerDlg)
	//}}AFX_DATA_INIT

	m_pBuilder = pBuilder;
	m_pSelectCtrls = NULL;
	m_pSelectCtrl = NULL;
}


void CControlManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlManagerDlg)
	DDX_Control(pDX, IDC_CBX_OPERATOR, m_cbxOperator);
	DDX_Control(pDX, IDC_CBX_GROUP, m_cbxGroup);
	DDX_Control(pDX, IDC_CBX_CONTROL, m_cbxControl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControlManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CControlManagerDlg)
	ON_BN_CLICKED(IDC_BTN_CREATE_CONTROL, OnBtnCreateControl)
	ON_BN_CLICKED(IDC_BTN_GROUP_COMMAND, OnBtnGroupCommand)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_OPERATION, OnBtnOperation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlManagerDlg message handlers

BOOL CControlManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int i = 0;
	CSpreadSheet* pSpread = NULL;
	
	// GROUP SPREAD
	UpdateGroupSpread(1, FALSE, TRUE);
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	// CONTROL SPREAD
	m_pSelectCtrls = m_pBuilder->m_pSelectCtrls;
	UpdateControlSpread(1, FALSE, TRUE);
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	// ITEM SPREAD
	m_pSelectCtrl = m_pSelectCtrls->GetCount() ? m_pSelectCtrls->GetHead() : NULL;
	UpdateCtrlItemSpread(1, FALSE, TRUE);
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	// GROUP COMBOBOX
	m_cbxGroup.AddString(_T("그룹추가"));
	m_cbxGroup.AddString(_T("그룹삭제"));
	m_cbxGroup.AddString(_T("그룹이동(UP)"));
	m_cbxGroup.AddString(_T("그룹이동(DOWN)"));

	// CONTROL COMBOBOX
	for (i=0; i<m_pBuilder->m_pDCI->GetDciClassSize(); ++i)
		m_cbxControl.AddString(CA2CT(m_pBuilder->m_pDCI->GetDciClass(i)->m_lpszClassName));

	m_cbxOperator.AddString(_T("Sequence"));
	m_cbxOperator.AddString(_T("Operator="));
	m_cbxOperator.AddString(_T("Operator+"));
	m_cbxOperator.AddString(_T("Operator-"));
	m_cbxOperator.AddString(_T("Operator*"));
	m_cbxOperator.AddString(_T("Operator/"));

	GetDlgItem(IDC_BTN_CREATE_CONTROL)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlManagerDlg::OnOK() 
{
	if (m_pSelectCtrl)
		m_pSelectCtrl->InvalidateControl(m_pParentWnd, FALSE);
	
	CDialog::OnOK();
}

BOOL CControlManagerDlg::DestroyWindow() 
{
	
	return CDialog::DestroyWindow();
}

void CControlManagerDlg::OnBtnApply() 
{
	m_pParentWnd->Invalidate();	
}

void CControlManagerDlg::OnBtnGroupCommand() 
{
	if (m_cbxGroup.GetCurSel() == CB_ERR)
		return;

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	int nSheet = pSpread->GetActiveSheet();
	int nRow = pSpread->GetActiveRow();
	int nCol = pSpread->GetActiveCol();
	CControlGroups& groups = m_pBuilder->m_ctrlGroups;

	switch (m_cbxGroup.GetCurSel())
	{
	case enGroupCreate:
		{
			CControlGroup group;
			group.m_strGroupName.Format(_T("Group#%d"), groups.GetSize());
			groups.SetSize(groups.GetSize() + 1);
			UpdateGroupSpread(nSheet, FALSE, FALSE);
			pSpread->SetActiveCell(nCol, groups.GetSize() + 1);
			OnClickFpspreadGroup(nCol, groups.GetSize() + 1);
		}
		break;

	case enGroupRemove:
		{
			m_pBuilder->m_ctrlGroups.RemoveAt(nRow-2);
			UpdateGroupSpread(nSheet, FALSE, FALSE);
//			OnClickFpspreadGroup(nCol, groups.GetSize() + 1);
		}
		break;
		
	case enGroupMoveUp:
		break;
		
	case enGroupMoveDown:
		break;
	}
}

void CControlManagerDlg::OnBtnCreateControl() 
{
	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	if (pSpread->GetActiveRow() == 1)
	{
		AfxMessageBox(_T("Selection Group에서는 컨트롤을 생성할 수 없습니다."));
		return;
	}

	if (m_cbxControl.GetCurSel() == -1)
	{
		AfxMessageBox(_T("생성할 컨트롤을 선택해주세요."));
		return;
	}
	
	CDciControlList* pCtrlList = (CDciControlList*)pSpread->GetRowItemData(pSpread->GetActiveRow());
	CDciControl* pDciControl = m_pBuilder->m_pDCI->CreateDciObject(m_cbxControl.GetCurSel());
	pDciControl->m_rcControlL = m_pBuilder->m_pDciGridCtrl->m_rcSelectL;
	pCtrlList->AddTail(pDciControl);
	ASSERT(pDciControl != NULL);

	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_CONTROL);
	UpdateControlSpread(1, FALSE, TRUE);
}

void CControlManagerDlg::OnBtnOperation() 
{
	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_CONTROL);
	ASSERT(pSpread != NULL);

	int nSelOperator  = m_cbxOperator.GetCurSel();
	int nSelBlockRow  = pSpread->GetSelBlockRow();
	int nSelBlockRow2 = pSpread->GetSelBlockRow2();
	int nSelBlockCol  = pSpread->GetSelBlockCol();
	int nSelBlockCol2 = pSpread->GetSelBlockCol2();

	if ((nSelBlockRow == -1) || (nSelBlockRow2 == -1))
	{
		nSelBlockRow = 1;
		nSelBlockRow2 = pSpread->GetMaxRows();
	}

	if ((nSelBlockCol == -1) || (nSelBlockCol != nSelBlockCol2))
	{
		AfxMessageBox(_T("Invalid Selected Column!"));
		return;
	}

	if (nSelOperator == CB_ERR)
	{
		AfxMessageBox(_T("연산자를 선택해주세요!"));
		return;
	}

	CString strValue;
	GetDlgItemText(IDC_EDT_VALUE, strValue);
	int nValue = CConvert::ToInt(strValue);

	pSpread->SetCol(nSelBlockCol);
	for (int i=nSelBlockRow; i<=nSelBlockRow2; ++i)
	{
		switch (nSelOperator)
		{
		case enOperatorSeq:
			{
				strValue.Format(_T("%d"), nValue++);
				pSpread->SetRow(i);
				pSpread->SetText(strValue);
			}
			break;

		case enOperatorEqual:
			{
				pSpread->SetRow(i);
				pSpread->SetText(strValue);
			}
			break;

		case enOperatorPlus:
			{
				pSpread->SetRow(i);
				strValue.Format(_T("%d"), CConvert::ToInt(pSpread->GetText()) + nValue);
				pSpread->SetText(strValue);
			}
			break;

		case enOperatorMinus:
			{
				pSpread->SetRow(i);
				strValue.Format(_T("%d"), CConvert::ToInt(pSpread->GetText()) - nValue);
				pSpread->SetText(strValue);
			}
			break;

		case enOperatorMultiply:
			{
				pSpread->SetRow(i);
				strValue.Format(_T("%d"), CConvert::ToInt(pSpread->GetText()) * nValue);
				pSpread->SetText(strValue);
			}
			break;

		case enOperatorDivision:
			{
				pSpread->SetRow(i);
				strValue.Format(_T("%d"), CConvert::ToInt(pSpread->GetText()) / nValue);
				pSpread->SetText(strValue);
			}
			break;

		default:
			return;
		}
	}

	UpdateControlSpread(pSpread->GetActiveSheet());
}

BEGIN_EVENTSINK_MAP(CControlManagerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CControlManagerDlg)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_GROUP, 4 /* Change */, OnChangeFpspreadGroup, VTS_I4 VTS_I4)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_GROUP, 5 /* Click */, OnClickFpspreadGroup, VTS_I4 VTS_I4)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_CONTROL, 4 /* Change */, OnChangeFpspreadControl, VTS_I4 VTS_I4)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_CONTROL, 5 /* Click */, OnClickFpspreadControl, VTS_I4 VTS_I4)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_CONTROL, 121 /* SheetChanging */, OnSheetChangingFpspreadControl, VTS_I2 VTS_I2 VTS_PVARIANT)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_ITEM, 4 /* Change */, OnChangeFpspreadItem, VTS_I4 VTS_I4)
	ON_EVENT(CControlManagerDlg, IDC_FPSPREAD_ITEM, 5 /* Click */, OnClickFpspreadItem, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CControlManagerDlg::UpdateGroupSpread(int nSheet, BOOL bSaveObject /* = TRUE */, BOOL bInitSpread /* = FALSE */)
{
	TRACE(_T("UpdateGroupSpread(int nSheet=%d, BOOL bSaveObject=%d, BOOL bInitSpread=%d)\n"), nSheet, bSaveObject, bInitSpread);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	if ((nSheet < 1) || (nSheet > pSpread->GetSheetCount()))
	{
//		pSpread->Reset();
		pSpread->SetMaxRows(0);
		pSpread->SetMaxCols(0);
		return;
	}

	int i, j;
	CDciPropertyArray properties;
	CControlGroups& groups = m_pBuilder->m_ctrlGroups;

	if (bInitSpread)
	{
		pSpread->Reset();
		pSpread->SetSheetCount(1);
		pSpread->SetSheet(1);
		pSpread->SetSheetName(_T("Groups"));
		pSpread->SetLockBackColor(RGB(128,128,128));
		pSpread->SetLockForeColor(RGB(255,255,255));
		pSpread->SetFontName(FPS_FONT_NAME);
		pSpread->SetFontSize(FPS_FONT_SIZE);
		pSpread->SetFontBold(FPS_FONT_BOLD);
	}

	pSpread->SetSheet(nSheet);

	if (bSaveObject)
	{
		for (i=0; i<groups.GetSize(); ++i)
		{
			CControlGroup& group = groups[i];
			group.UpdatePropNames(properties);

			pSpread->SetRow(i + 2);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				properties[j].m_strValue = pSpread->GetText();
			}

			group.UpdatePropValues(properties);
		}
	}
	else
	{
		CControlGroup select;
		select.m_strGroupID.Empty();
		select.m_strGroupName = _T("Selection");
		select.UpdatePropNames(properties);
		select.UpdatePropValues(properties, FALSE);

		pSpread->SetMaxRows(groups.GetSize() + 1);
		pSpread->SetMaxCols(properties.GetSize() + 1);

		// SELECTION GROUP
		{
			pSpread->SetRow(1);
			pSpread->SetRowItemData(1, (long)m_pBuilder->m_pSelectCtrls);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strValue);
				pSpread->SetLock(TRUE);
			}

			pSpread->SetCol(j + 2);
			pSpread->SetText(CConvert::ToString((int)m_pBuilder->m_pSelectCtrls->GetCount()));
		}

		for (i=0; i<groups.GetSize(); ++i)
		{
			CControlGroup& group = groups[i];
			group.UpdatePropNames(properties);
			group.UpdatePropValues(properties, FALSE);

			pSpread->SetRow(i + 2);
			pSpread->SetRowItemData(i + 2, (long)group.m_pGroupCtrls);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strValue);
			}

			pSpread->SetCol(j + 2);
			pSpread->SetText(CConvert::ToString((int)group.m_pGroupCtrls->GetCount()));
		}

		// PROPERTY NAME
		{
			pSpread->SetRow(0);
			pSpread->SetRowItemData(0, NULL);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strName);
				pSpread->SetColWidth(j + 1, pSpread->GetMaxTextColWidth(j + 1) + FPS_COL_WIDTH_GAP);
			}

			pSpread->SetCol(j + 2);
			pSpread->SetText(_T("count"));
		}
	}
}

void CControlManagerDlg::OnChangeFpspreadGroup(long Col, long Row) 
{
	TRACE("OnChangeFpspreadGroup(long Col=%d, long Row=%d)\n", Col, Row);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	UpdateGroupSpread(pSpread->GetActiveSheet(), TRUE, FALSE);
}

void CControlManagerDlg::OnClickFpspreadGroup(long Col, long Row) 
{
	TRACE("OnClickFpspreadGroup(long Col=%d, long Row=%d)\n", Col, Row);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_GROUP);
	ASSERT(pSpread != NULL);

	if ((Row < 1) || (Row > pSpread->GetMaxRows()))
	{
		m_pSelectCtrls = NULL;
		UpdateControlSpread(0, FALSE, FALSE);
		m_pSelectCtrl = NULL;
		UpdateCtrlItemSpread(0, FALSE, FALSE);
		return;
	}

	CDciControlList* pCtrlList = (CDciControlList*)pSpread->GetRowItemData(Row);
	if ((pCtrlList == NULL) || (pCtrlList == m_pSelectCtrls))
	{
		TRACE("OnClickFpspreadGroup(long Col=%d, long Row=%d): Same Group \n", Col, Row);
		return;
	}

	m_pSelectCtrls = pCtrlList;
	UpdateControlSpread(1, FALSE, TRUE);
	ASSERT(m_pSelectCtrls != NULL);

	if (m_pSelectCtrls->GetCount())
	{
		m_pSelectCtrl = m_pSelectCtrls->GetHead();
		UpdateCtrlItemSpread(1, FALSE, TRUE);
	}

	GetDlgItem(IDC_BTN_CREATE_CONTROL)->EnableWindow(Row > 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CControlManagerDlg::UpdateControlSpread(int nSheet, BOOL bSaveObject /* = TRUE */, BOOL bInitSpread /* = FALSE */)
{
	TRACE(_T("UpdateControlSpread(int nSheet=%d, BOOL bSaveObject=%d, BOOL bInitSpread=%d)\n"), nSheet, bSaveObject, bInitSpread);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_CONTROL);
	ASSERT(pSpread != NULL);

	if ((nSheet < 1) || (nSheet > pSpread->GetSheetCount()) || (m_pSelectCtrls == NULL))
	{
//		pSpread->Reset();
		pSpread->SetMaxRows(0);
		pSpread->SetMaxCols(0);
		return;
	}

	int i, j, nColSize = 0;
	const int ALL_CTRL_SHEET = 1;
	CDciPropertyArray properties;
	CDciControlList* pCtrlList = m_pSelectCtrls;
	CDciControl* pDciControl = NULL;
	ASSERT(pCtrlList != NULL);

	if (bInitSpread)
	{
		CStringArray strClasses;
		for (POSITION pos=pCtrlList->GetHeadPosition(); pos!=NULL; )
		{
			pDciControl = pCtrlList->GetNext(pos);
			ASSERT(pDciControl != NULL);

			if (strClasses.GetSize() == 0)
			{
				strClasses.Add(pDciControl->GetRuntimeClass()->m_lpszClassName);
				continue;
			}

			for (i=0; i<strClasses.GetSize(); ++i)
			{
				if (strClasses[i] == pDciControl->GetRuntimeClass()->m_lpszClassName)
					break;			
			}

			if (i == strClasses.GetSize())
				strClasses.Add(pDciControl->GetRuntimeClass()->m_lpszClassName);
		}

		pSpread->Reset();
		pSpread->SetSheetCount(1 + strClasses.GetSize());
		pSpread->SetSheet(1);
		pSpread->SetSheetName(_T("전체"));
		pSpread->SetFontName(FPS_FONT_NAME);
		pSpread->SetFontSize(FPS_FONT_SIZE);
		pSpread->SetFontBold(FPS_FONT_BOLD);
		
		for (i=0; i<strClasses.GetSize(); ++i)
		{
			pSpread->SetSheet(i + 2);
			pSpread->SetSheetName(strClasses[i]);
		}
	}

	pSpread->SetSheet(nSheet);

	if (bSaveObject)
	{
		for (i=0; i<pSpread->GetMaxRows(); ++i)
		{
			pDciControl = (CDciControl*)pSpread->GetRowItemData(i + 1);
			pDciControl->UpdatePropNames(properties);
			ASSERT(pDciControl != NULL);

			pSpread->SetRow(i + 1);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				properties[j].m_strValue = pSpread->GetText();
			}

			pDciControl->UpdatePropValues(properties);
		}
	}
	else
	{
		pSpread->DeleteRows(1, pSpread->GetMaxRows());

		CString strClassName = pSpread->GetSheetName();
		for (POSITION pos=pCtrlList->GetHeadPosition(); pos!=NULL; )
		{
			pDciControl = pCtrlList->GetNext(pos);
			ASSERT(pDciControl != NULL);

			if ((nSheet != ALL_CTRL_SHEET) && (strClassName != pDciControl->GetRuntimeClass()->m_lpszClassName))
				continue;

			pDciControl->UpdatePropNames(properties);
			pDciControl->UpdatePropValues(properties, FALSE);
			nColSize = ((nSheet != ALL_CTRL_SHEET) || (pSpread->GetSheetCount() == 2)) ? properties.GetSize() : CDciControl::enControlPropSize;

			pSpread->SetRow(pSpread->GetDataRowCnt() + 1);
			pSpread->SetRowItemData(pSpread->GetDataRowCnt() + 1, (long)pDciControl);
			for (j=0; j<nColSize; ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strValue);
			}

			if (nSheet == ALL_CTRL_SHEET)
			{
				pSpread->SetCol(0);
				pSpread->SetText(CA2CT(pDciControl->GetRuntimeClass()->m_lpszClassName));
			}
		}

		if (properties.GetSize())
		{
			pSpread->SetRow(0);
			nColSize = ((nSheet != ALL_CTRL_SHEET) || (pSpread->GetSheetCount() == 2)) ? properties.GetSize() : CDciControl::enControlPropSize;
			for (j=0; j<nColSize; ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strName);
				pSpread->SetColWidth(j + 1, pSpread->GetMaxTextColWidth(j + 1) + FPS_COL_WIDTH_GAP);
			}

			pSpread->SetColWidth(0, (nSheet == ALL_CTRL_SHEET) ? 15.0 : 4.0);
		}

		pSpread->SetMaxRows(pSpread->GetDataRowCnt());
		pSpread->SetMaxCols(nColSize);
	}
}

void CControlManagerDlg::OnChangeFpspreadControl(long Col, long Row) 
{
	TRACE(_T("OnChangeFpspreadControl(long Col=%d, long Row=%d)\n"), Col, Row);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_CONTROL);
	ASSERT(pSpread != NULL);

	UpdateControlSpread(pSpread->GetActiveSheet());	
}

void CControlManagerDlg::OnClickFpspreadControl(long Col, long Row) 
{
	TRACE(_T("OnClickFpspreadControl(long Col=%d, long Row=%d)\n"), Col, Row);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_CONTROL);
	ASSERT(pSpread != NULL);

	if ((Row < 1) && (Row > pSpread->GetMaxRows()))
	{
		m_pSelectCtrl = NULL;
		UpdateCtrlItemSpread(0, FALSE, FALSE);
		return;
	}

	CDciControl* pDciControl = (CDciControl*)pSpread->GetRowItemData(Row);
	if ((pDciControl == NULL) || (pDciControl == m_pSelectCtrl))
	{
		TRACE(_T("OnClickFpspreadControl(long Col=%d, long Row=%d): Same Control \n"), Col, Row);
		return;
	}

	m_pSelectCtrl = pDciControl;
	UpdateCtrlItemSpread(1, FALSE, TRUE);
	ASSERT(m_pSelectCtrl != NULL);
}

void CControlManagerDlg::OnSheetChangingFpspreadControl(short OldSheet, short NewSheet, VARIANT FAR* Cancel) 
{
	TRACE(_T("OnSheetChangingFpspreadControl(short OldSheet=%d, short NewSheet=%d, VARIANT FAR* Cancel=%d)\n"), OldSheet, NewSheet, *Cancel);

	UpdateControlSpread(NewSheet, FALSE, FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CControlManagerDlg::UpdateCtrlItemSpread(int nSheet, BOOL bSaveObject /* = TRUE */, BOOL bInitSpread /* = FALSE */)
{
	TRACE(_T("UpdateCtrlItemSpread(int nSheet=%d, BOOL bSaveObject=%d, BOOL bInitSpread=%d)\n"), nSheet, bSaveObject, bInitSpread);

	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_ITEM);
	ASSERT(pSpread != NULL);

	if ((nSheet < 1) || (nSheet > pSpread->GetSheetCount()) || (m_pSelectCtrl == NULL) || (m_pSelectCtrl->GetItemSize() == 0))
	{
//		pSpread->Reset();
		pSpread->SetMaxRows(0);
		pSpread->SetMaxCols(0);
		return;
	}

	int i, j;
	CDciPropertyArray properties;
	CDciControl* pDciControl = m_pSelectCtrl;
	pDciControl->UpdateItemNames(properties);
	ASSERT(pDciControl != NULL);

	if (bInitSpread)
	{
		pSpread->Reset();
		pSpread->SetSheetCount(1);
		pSpread->SetSheet(1);
		pSpread->SetSheetName(_T("Items"));
		pSpread->SetFontName(FPS_FONT_NAME);
		pSpread->SetFontSize(FPS_FONT_SIZE);
		pSpread->SetFontBold(FPS_FONT_BOLD);
	}
	
	if (bSaveObject)
	{
		for (i=0; i<pDciControl->GetItemSize(); ++i)
		{
			pSpread->SetRow(i + 1);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				properties[j].m_strValue = pSpread->GetText();
			}

			pDciControl->UpdateItemValues(i, properties);
		}
	}
	else
	{
		for (i=0; i<pDciControl->GetItemSize(); ++i)
		{
			pDciControl->UpdateItemValues(i, properties, FALSE);

			pSpread->SetRow(i + 1);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strValue);
			}
		}

		{
			pSpread->SetRow(0);
			for (j=0; j<properties.GetSize(); ++j)
			{
				pSpread->SetCol(j + 1);
				pSpread->SetText(properties[j].m_strName);
				pSpread->SetColWidth(j + 1, pSpread->GetMaxTextColWidth(j + 1) + FPS_COL_WIDTH_GAP);
			}
		}

		pSpread->SetMaxRows(pDciControl->GetItemSize());
		pSpread->SetMaxCols(properties.GetSize());
	}
}

void CControlManagerDlg::OnChangeFpspreadItem(long Col, long Row) 
{
	TRACE(_T("OnChangeFpspreadItem(long Col=%d, long Row=%d)\n"), Col, Row);
	
	CSpreadSheet* pSpread = NULL;
	pSpread = (CSpreadSheet*)GetDlgItem(IDC_FPSPREAD_ITEM);
	ASSERT(pSpread != NULL);

	UpdateCtrlItemSpread(pSpread->GetActiveSheet(), TRUE, FALSE);
}

void CControlManagerDlg::OnClickFpspreadItem(long Col, long Row) 
{
	TRACE(_T("OnClickFpspreadItem(long Col=%d, long Row=%d)\n"), Col, Row);
	
}
