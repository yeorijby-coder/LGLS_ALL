// UserManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "Ecs.h"
#include "MainFrm.h"
#include "EcsDoc.h"
#include "UserManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg dialog


CUserManagerDlg::CUserManagerDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CUserManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserManagerDlg)
	m_strUserID = _T("");
	m_strUserName = _T("");
	m_strPwd = _T("");
	m_strPwdConf = _T("");
	//}}AFX_DATA_INIT
	m_bModified = FALSE;
}


void CUserManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserManagerDlg)
	DDX_Control(pDX, IDC_CBX_LEVEL, m_cbxLevel);
	DDX_Text(pDX, IDC_USER_ID, m_strUserID);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_PASSWORD, m_strPwd);
	DDX_Text(pDX, IDC_PWD_CONFIRM, m_strPwdConf);
	DDX_Control(pDX, IDC_FPSPREAD1, m_fpSpread);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserManagerDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CUserManagerDlg)
	ON_BN_CLICKED(IDC_BTN_QUERY, OnQueryClicked)
	ON_BN_CLICKED(IDC_BTN_INSERT, OnInsertClicked)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnUpdateClicked)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnDeleteClicked)
	ON_BN_CLICKED(IDC_BTN_EXCEL, OnExcelClicked)
	ON_BN_CLICKED(IDC_USER_SAVE, OnClickedSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CUserManagerDlg, CResizableDialog)
    //{{AFX_EVENTSINK_MAP(CUserManagerDlg)
	ON_EVENT(CUserManagerDlg, IDC_FPSPREAD1, 5 /* Click */, OnClickFpspread1, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserManagerDlg message handlers

BOOL CUserManagerDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	// Spread
	m_fpSpread.SetUserColAction(1);
	m_fpSpread.SetMaxRows(0);
	m_fpSpread.SetFontSize(10);
	
	// Preset resizable control layout
	CResizableDialog::AddAnchor(IDC_GROUP, TOP_LEFT, TOP_RIGHT);
	CResizableDialog::AddAnchor(m_fpSpread, TOP_LEFT, BOTTOM_RIGHT);

	OnQueryClicked();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//==============================================================================
//	Desc	: [조회] 버튼
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnQueryClicked() 
{
	CWaitCursor wait;

	CString str, strCap;
	CWnd::GetWindowText(strCap);

	TRY 
	{
		if (_taccess(ECS_USER_FILE, 0) != 0)
		{
			CWnd::MessageBox("조회하고자 하는 내용이 없습니다.", strCap, MB_OK | MB_ICONINFORMATION);
			return;
		}

		int nRow = 0, nCol = 0;
		CStringArray strArray;
		CString str;

		CFile f(ECS_USER_FILE, CFile::modeRead);
		CArchive ar(&f, CArchive::load);

		m_fpSpread.DeleteRows(1, m_fpSpread.GetMaxRows());
		m_fpSpread.SetReDraw(FALSE);

		while (ar.ReadString(str))
		{
			CLib::Split(str, strArray, _T(","));
			if (strArray.GetAt(0).IsEmpty()) continue;
			m_fpSpread.SetMaxRows(++nRow);
			m_fpSpread.SetText(++nCol, nRow, variant_t(strArray.GetAt(0)));
			m_fpSpread.SetText(++nCol, nRow, variant_t(strArray.GetAt(1)));
			m_fpSpread.SetText(++nCol, nRow, variant_t(strArray.GetAt(2)));
			m_cbxLevel.GetLBText(atoi(strArray.GetAt(3)), str);
			m_fpSpread.SetText(++nCol, nRow, variant_t(str));
			m_fpSpread.SetText(++nCol, nRow, variant_t(strArray.GetAt(4)));
			nCol = 0;
		}

		m_fpSpread.SetReDraw(TRUE);
		ar.Close();
		f.Close();
	}
	CATCH (CException, e)
	{
		CWnd::MessageBox(CLib::GetExceptionString(e), strCap, MB_OK | MB_ICONERROR);
	}
	END_CATCH

	wait.Restore();
}

//==============================================================================
//	Desc	: [입력] 버튼 클릭시
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnInsertClicked() 
{
	if (UpdateData(TRUE) == FALSE)
		return;

	CString strCap;
	CWnd::GetWindowText(strCap);

	if (m_strUserID == _T("ADMIN"))
	{
		CWnd::MessageBox("ADMIN으로는 계정을 생성할 수 없습니다.", strCap, MB_OK | MB_ICONWARNING);
		return;
	}

	if (!CheckList()) return;

	if (CWnd::MessageBox(m_strUserID + " 사용자 정보를 입력하시겠습니까?", strCap, MB_YESNO) != IDYES)
		return;

	CWaitCursor wait;
	m_fpSpread.SetReDraw(FALSE);
	m_fpSpread.SetMaxRows(m_fpSpread.GetMaxRows() + 1);

	CString str = _T("");
	int nCol = 0;
	int nRow = m_fpSpread.GetMaxRows();
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strUserID));
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strUserName));
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strPwd));
	m_cbxLevel.GetLBText(m_cbxLevel.GetCurSel(), str);
	m_fpSpread.SetText(++nCol, nRow, variant_t(str));
	COleDateTime dt = COleDateTime::GetCurrentTime();
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
	m_fpSpread.SetText(++nCol, nRow, variant_t(str));
	
	m_fpSpread.SetReDraw(TRUE);
	m_bModified = TRUE;
	wait.Restore();
}

//==============================================================================
//	Desc	: [수정] 버튼 클릭시
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnUpdateClicked() 
{
	CString strCap;
	CWnd::GetWindowText(strCap);

	if (m_fpSpread.GetMaxRows() == 0)
	{
		CWnd::MessageBox("수정하고자 하는 내용이 없습니다.", strCap, MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (UpdateData(TRUE) == FALSE)
		return;

	CString strPrevName = m_strUserName, strPrevPwd = m_strPwd;

	variant_t val;
	m_fpSpread.GetText(1, m_fpSpread.GetActiveRow(), &val);
	CString strPrevID = (LPCTSTR)(_bstr_t)val;

	if (strPrevID != m_strUserID)
	{
		CWnd::MessageBox("수정하고자 하는 사용자 ID는 동일해야 합니다.", strCap, MB_OK | MB_ICONWARNING);
		(GetDlgItem(IDC_USER_ID))->SetFocus();
		return;
	}

	if (!CheckList()) return;

	if (AfxMessageBox(m_strUserID + " 사용자 정보를 수정하시겠습니까?", MB_YESNO) != IDYES)
		return;

	CWaitCursor wait;
	m_fpSpread.SetReDraw(FALSE);

	CString str = _T("");
	int nCol = 0;
	int nRow = m_fpSpread.GetActiveRow();
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strUserID));
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strUserName));
	m_fpSpread.SetText(++nCol, nRow, variant_t(m_strPwd));
	m_cbxLevel.GetLBText(m_cbxLevel.GetCurSel(), str);
	m_fpSpread.SetText(++nCol, nRow, variant_t(str));
	COleDateTime dt = COleDateTime::GetCurrentTime();
	str.Format("%04d-%02d-%02d %02d:%02d:%02d", dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
	m_fpSpread.SetText(++nCol, nRow, variant_t(str));
	
	m_fpSpread.SetReDraw(TRUE);
	m_bModified = TRUE;
	wait.Restore();
}

//==============================================================================
//	Desc	: [삭제] 버튼 클릭시
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnDeleteClicked() 
{
	CString strCap;
	CWnd::GetWindowText(strCap);

	if (m_fpSpread.GetMaxRows() == 0)
	{
		CWnd::MessageBox("삭제하고자 하는 내용이 없습니다.", strCap, MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (UpdateData(TRUE) == FALSE)
		return;

	int nRow = m_fpSpread.GetActiveRow();
	int nMax = m_fpSpread.GetMaxRows();
	if ((nRow <= 0) || (nRow > nMax) || (m_strUserID.GetLength() < 1))
	{
		CWnd::MessageBox("삭제하고자 하는 사용자를 선택하십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (m_strUserID == _T("ADMIN"))
	{
		CWnd::MessageBox("ADMIN 계정은 삭제할 수 없습니다.", strCap, MB_OK | MB_ICONWARNING);
		return;
	}

	if (CWnd::MessageBox(m_strUserID + " 사용자 정보를 삭제하시겠습니까?", strCap, MB_YESNO) != IDYES)
		return;

	m_fpSpread.SetReDraw(FALSE);
	m_fpSpread.DeleteRows(nRow, 1);
	m_fpSpread.SetMaxRows(nMax - 1);
	m_fpSpread.SetReDraw(TRUE);
	m_bModified = TRUE;
}

//==============================================================================
//	Desc	: [저장] 버튼 클릭시
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnClickedSave() 
{
	WriteFiles();	
}

//==============================================================================
//	Desc	: [엑셀] 버튼 클릭시
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnExcelClicked() 
{
	CString strCap;
	CWnd::GetWindowText(strCap);

	if (m_fpSpread.GetMaxRows() == 0)
	{
		CWnd::MessageBox("저장하고자 하는 내용이 없습니다.", strCap, MB_OK | MB_ICONINFORMATION);
		return;
	}

	char szFilter[] = "Excel 통합 문서(.xls)|*.xls|";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	
	dlg.m_ofn.lpstrTitle = _T("파일 저장");
	dlg.m_ofn.lpstrInitialDir = _T(".\\");

	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strPathName = dlg.GetPathName();
	CString strFileName = dlg.GetFileName();

	if (strFileName.IsEmpty())
		return;

	// Excel 통합 문서
	if (dlg.GetFileExt().IsEmpty() || strFileName.Right(4) != _T(".xls"))
		strPathName += _T(".xls");

	// FarPoint Spread 7.0 버전은 Excel 파일로 저장시 Column명이 저장되지 않으므로 
	// 첫번째 Row에 Column명을 추가하고 파일을 저장한 후 첫번째 Row를 삭제한다.
	m_fpSpread.SetReDraw(FALSE);
	m_fpSpread.SetMaxRows(m_fpSpread.GetMaxRows() + 1);
	m_fpSpread.InsertRows(1, 1);

	variant_t val;
	for (int n = 0; n <= m_fpSpread.GetMaxCols(); n++)
	{
		m_fpSpread.GetText(n, 0, &val);
		m_fpSpread.SetText(n, 1, val);
	}

//	m_fpSpread.ExportToExcel(strPathName, _T("Sheet1"), "");
	m_fpSpread.ExportExcelBook(strPathName, "");
	m_fpSpread.DeleteRows(1, 1);
	m_fpSpread.SetMaxRows(m_fpSpread.GetMaxRows() - 1);
	m_fpSpread.SetReDraw(TRUE);
}

//==============================================================================
//	Desc	: Spread clicked
//	Date	: 2013.09
//	Update	:
//==============================================================================
void CUserManagerDlg::OnClickFpspread1(long Col, long Row) 
{
	if ((Row > 0) && (Row <= m_fpSpread.GetMaxRows()))
	{
		m_fpSpread.SetRow(Row);
		m_fpSpread.SetCol(1); m_strUserID = m_fpSpread.GetText();
		m_fpSpread.SetCol(2); m_strUserName = m_fpSpread.GetText();
		m_fpSpread.SetCol(3); m_strPwd = m_strPwdConf = m_fpSpread.GetText();
		m_fpSpread.SetCol(4); m_cbxLevel.SelectString(0, m_fpSpread.GetText());
		m_fpSpread.SetCol(5); 
	}
	else
	{
		m_strUserID.Empty();
		m_strUserName.Empty();
		m_strPwd.Empty();
		m_strPwdConf.Empty();
		m_cbxLevel.SetCurSel(-1);
	}

	UpdateData(FALSE);
}

//==============================================================================
//	Desc	: Check input data
//	Date	: 2013.09
//	Update	:
//==============================================================================
BOOL CUserManagerDlg::CheckList()
{
	CString strCap;
	CWnd::GetWindowText(strCap);

	if (m_strUserID.GetLength() < 1)
	{
		CWnd::MessageBox("사용자 ID를 입력해주십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		(GetDlgItem(IDC_USER_ID))->SetFocus();
		return FALSE;
	}

	if (m_strUserName.GetLength() < 1)
	{
		CWnd::MessageBox("사용자 이름을 입력해주십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		(GetDlgItem(IDC_USER_NAME))->SetFocus();
		return FALSE;
	}

	if ((m_strPwd.GetLength() < 1) || (m_strPwdConf.GetLength() < 1))
	{
		CWnd::MessageBox("비밀번호를 입력해주십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		(GetDlgItem(IDC_PASSWORD))->SetFocus();
		return FALSE;
	}

	if (m_strPwd != m_strPwdConf)
	{
		CWnd::MessageBox("비밀번호가 일치하지 않습니다. 재입력해 주십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		(GetDlgItem(IDC_PWD_CONFIRM))->SetFocus();
		return FALSE;
	}

	if (m_cbxLevel.GetCurSel() == CB_ERR)
	{
		CWnd::MessageBox("사용자 권한을 선택해 주십시오.", strCap, MB_OK | MB_ICONINFORMATION);
		(GetDlgItem(IDC_CBX_LEVEL))->SetFocus();
		return FALSE;
	}

	return TRUE;
}

//==============================================================================
//	Desc	: 
//	Date	: 2013.09
//	Update	:
//==============================================================================
BOOL CUserManagerDlg::WriteFiles()
{
	if (m_fpSpread.GetMaxRows() == 0)	return TRUE;

	CWaitCursor wait;

	CString str, strCap;
	CWnd::GetWindowText(strCap);

	// Backup 파일이 존재할 경우 삭제
	TRY
	{
		if (_taccess(ECS_USER_BACK_FILE, 0) == 0)
		{
			CFile::Remove(ECS_USER_BACK_FILE);
		}
	}
	CATCH (CFileException, e)
	{
		CWnd::MessageBox("백업파일 삭제 실패! 재시도해 주십시오.", strCap, MB_OK | MB_ICONERROR);
//		str.Format(_T("\n백업파일 삭제 실패! (CODE=%d)"), e->m_cause);
//		TRACE(str);
	}
	END_CATCH

	// Data 파일 백업
	TRY
	{
		CFile::Rename(ECS_USER_FILE, ECS_USER_BACK_FILE);
	}
	CATCH (CFileException, e)
	{
		CWnd::MessageBox("데이터 파일 백업 실패! 재시도해 주십시오.", strCap, MB_OK | MB_ICONERROR);
//		str.Format(_T("\n데이터 파일 백업 실패! (CODE=%d)"), e->m_cause);
//		TRACE(str);
	}
	END_CATCH

	// Data 파일 저장
	TRY
	{
		CFile f(ECS_USER_FILE, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		f.SeekToEnd();

		CArchive ar(&f, CArchive::store);

		CEcsDoc* pDoc = (CEcsDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		CStringArray strArray;
		variant_t val;
		int nCol = 0, nCurSel = 0;
		CString strXml = _T("");
		
		m_fpSpread.SetReDraw(FALSE);
		for (int nRow = 1; nRow <= m_fpSpread.GetMaxRows(); nRow++)
		{
			strArray.RemoveAll();
			m_fpSpread.GetText(++nCol, nRow, &val);
			strArray.Add((LPCTSTR)(_bstr_t)val);
			m_fpSpread.GetText(++nCol, nRow, &val);
			strArray.Add((LPCTSTR)(_bstr_t)val);
			m_fpSpread.GetText(++nCol, nRow, &val);
			strArray.Add((LPCTSTR)(_bstr_t)val);
			m_fpSpread.GetText(++nCol, nRow, &val);
			nCurSel = m_cbxLevel.FindString(0, (LPCTSTR)(_bstr_t)val);
			str.Format(_T("%d"), nCurSel);
			strArray.Add(str);
			m_fpSpread.GetText(++nCol, nRow, &val);
			strArray.Add((LPCTSTR)(_bstr_t)val);

			str.Format(_T("%s,%s,%s,%s,%s\r\n"), 
						strArray.GetAt(0), strArray.GetAt(1), strArray.GetAt(2), strArray.GetAt(3), strArray.GetAt(4));
			ar.WriteString(str);
			// 클라이언트 송신용
			str.Format(_T("<INFO DATA='%s,%s,%s,%s,%s'/>"), 
						strArray.GetAt(0), strArray.GetAt(1), strArray.GetAt(2), strArray.GetAt(3), strArray.GetAt(4));
			strXml += str;
			nCol = 0;
		}

		// 수정사항이 있을 경우 클라이언트 송신
		if (m_bModified == TRUE)
		{
			CString strSend = _T("");
			strSend.Format(_T("%c<ECS><SYS><USER>%s</USER></SYS></ECS>%c"), 
							CMonitorServer::enSTX, strXml, CMonitorServer::enETX);
			if (pDoc->m_pMonitorListener && pDoc->m_pMonitorListener->GetClientCount())
				pDoc->m_pMonitorListener->SendToClients(strSend);
		}

		m_fpSpread.SetReDraw(TRUE);
		ar.Close();
		f.Close();
	}
	CATCH (CException, e)
	{
		CWnd::MessageBox("파일 저장 실패! 재시도해 주십시오.", strCap, MB_OK | MB_ICONERROR);
//		CWnd::MessageBox(CLib::GetExceptionString(e), strCap, MB_OK | MB_ICONERROR);
	}
	END_CATCH

	wait.Restore();

	return TRUE;
}

void CUserManagerDlg::OnCancel() 
{
	WriteFiles();	
	
	CResizableDialog::OnCancel();
}
