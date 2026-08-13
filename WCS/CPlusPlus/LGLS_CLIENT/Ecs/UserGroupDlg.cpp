// UserGroupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "UserGroupDlg.h"
#include "afxdialogex.h"


// CUserGroupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserGroupDlg, CSkinDialog)

CUserGroupDlg::CUserGroupDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserGroupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CUserGroupDlg::CUserGroupDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CUserGroupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CUserGroupDlg::~CUserGroupDlg()
{
	this->DestroyWindow();
}


void CUserGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FPSPREAD1, m_pUSER_GRP);
	DDX_Control(pDX, ID_USER_GROUP_OK, m_btnOk);
	DDX_Control(pDX, ID_USER_GROUP_CANCEL, m_btnCancel);
	DDX_Control(pDX, ID_BTN_USER_GROUP_SEARCH, m_btnSearch);
	DDX_Control(pDX, ID_BTN_USER_GROUP_DELETE, m_btnDelete);


	DDX_Control(pDX, IDC_LBL_GRP_CD,   m_lblGrpNm);
	DDX_Control(pDX, IDC_LBL_URER_GROUP_USER_DESC, m_lblGroupDesc);
	DDX_Control(pDX, IDC_LBL_REMARKS,  m_lblRemarks);
	DDX_Control(pDX, IDC_LBL_USER_GROUP_RESULT, m_lblResult);
	DDX_Control(pDX, IDC_LBL_USER_GROUP_CNT,  m_lblCnt);


	DDX_Control(pDX, IDC_CBX_GRP_CD,   m_cbxGrpCd);
	DDX_Control(pDX, IDC_EDT_GRP_DESC, m_edtGrpDesc);
	DDX_Control(pDX, IDC_EDT_REMARKS,  m_edtRemarks);

	DDX_Control(pDX, IDC_CHECK_ALL,	   m_chkCheckAll);
}

BEGIN_MESSAGE_MAP(CUserGroupDlg, CSkinDialog)
	ON_MESSAGE(SSM_CLICK, &CUserGroupDlg::OnLglsHdrSort)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_ALL, &CUserGroupDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(ID_BTN_USER_GROUP_DELETE, &CUserGroupDlg::OnBnClickedBtnDelete2)
	ON_BN_CLICKED(ID_BTN_USER_GROUP_SEARCH, &CUserGroupDlg::OnBnClickedBtnSearch2)
END_MESSAGE_MAP()

// [LGLS] 그리드 헤더 클릭 정렬
LRESULT CUserGroupDlg::OnLglsHdrSort(WPARAM wParam, LPARAM lParam)
{
	LPSS_CELLCOORD pCell = (LPSS_CELLCOORD)lParam;
	if(pCell != NULL && pCell->Row == 0 && pCell->Col >= 1)
		m_pUSER_GRP.SortByHeaderColumn(pCell->Col);
	return 0;
}

BOOL CUserGroupDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	if( !m_bInitialized )
	{
		m_bInitialized = TRUE;

		//m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		//m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

		//m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		//m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);

		m_btnSearch.SetIcon(Global.GetIcon(Global.ICO_SEARCH));
		m_btnDelete.SetIcon(Global.GetIcon(Global.ICO_JOBDELETE));


		//IDC_LOGIN_PICTUREBOX
		HBITMAP hbitBg;
		hbitBg = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SPLASH_ECS));
		//m_bgPic.SetBitmap(hbitBg);
		RelocationControls();
	}
	FillComboBoxGrpCd();
	InitializeControlLanguage();
	Invalidate(TRUE);
	InitializeUSER_GRP(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUserGroupDlg::InitializeControlLanguage()
{
	m_lblGrpNm.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹코드"), m_pDoc->m_enLang));
	m_lblGroupDesc.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("그룹설명"), m_pDoc->m_enLang));
	m_lblRemarks.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("비고"), m_pDoc->m_enLang));
	m_lblResult.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회건수:"), m_pDoc->m_enLang));


	m_btnDelete.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("삭제"), m_pDoc->m_enLang));
	m_btnSearch.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("조회"), m_pDoc->m_enLang));
	m_btnOk.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("확인"), m_pDoc->m_enLang));
	m_btnCancel.SetWindowText(m_pDoc->m_pLang->GetLangValue(_T("취소"), m_pDoc->m_enLang));
}



void CUserGroupDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CUserGroupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserGroupDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CUserGroupDlg::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);

	x = rc.Width() - 20 - size.cx;
	y = rc.Height() - 10 - size.cy;
	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);

	x -= size.cx + 10;
	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
	m_btnOk.Invalidate();
}


BOOL CUserGroupDlg::PreTranslateMessage(MSG* pMsg)
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




int CUserGroupDlg::GetListUSER_GRP(CStringList &strListUSER_GRP, BOOL bSearch )
{
	CString strSql = _T("");

	strSql.Format(_T(" SELECT GRP_CD as 그룹코드 \n")
				  _T("      , GRP_DESC as 그룹설명 \n")
				  _T("      , REMARKS  as 비고     \n")
				  _T("      , INS_USER_ID as 생성자  \n")
				  _T("      , INS_DT as 생성일자 \n")
				  _T("      , UPD_USER_ID as 수정자 \n")
				  _T("      , UPD_DT as 수정일자 \n")
				  _T("   FROM USER_GRP \n")
				  _T("  WHERE 1 = 1  \n"));

	CString strTemp = _T("");

	m_cbxGrpCd.GetWindowText(strTemp);
	if (strTemp != "")
	{
		strSql += _T("AND GRP_CD = '") + strTemp + _T("' \n");
	}
	m_edtGrpDesc.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND GRP_DESC LIKE  '%") + strTemp + _T("%' \n");	
	}
	m_edtRemarks.GetWindowText(strTemp);
	if (strTemp != "")
	{		
		strSql += _T("AND REMARSKS LIKE '%") + strTemp + _T("%' \n");
	}

	strSql += _T("   ORDER BY GRP_CD  \n");


	int nSqlResult = 0;//m_pDB->SelectQry(strSql, strListUSER_GRP);
	m_lblCnt.SetWindowText(CConvert::ToString(nSqlResult));
	return nSqlResult;
}

void CUserGroupDlg::FillComboBoxGrpCd()
{
	CString strSql = _T("");
	CString strJobTyp = _T("GRP_CD");

	CStringList strUserGrpList;

	strSql.Format(_T(" SELECT GRP_CD \n")
		_T("	 FROM USER_GRP  \n")
		_T(" ORDER BY GRP_CD"));

	int nCntSel = 0;//m_pDB->SelectQry(strSql, strUserGrpList);
	if(nCntSel < 1)
	{
		AfxMessageBox(_T("DEBUG :: 공통코드가 수집되지 않았습니다"));
		return;
	}

	int nLoopCnt = -1;
	for (POSITION pos = strUserGrpList.GetHeadPosition(); pos != NULL;)
	{
		nLoopCnt++;
		CString strTemp = strUserGrpList.GetNext(pos);
		CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
		CString strValue = strTemp.Mid(strTemp.Find(';')+1);

		if ( strTemp.IsEmpty() ) continue;
		if (StrFieldName == "GRP_CD")		
		{
			m_cbxGrpCd.InsertString(nLoopCnt, strValue);
		}		
	}	
}


void CUserGroupDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	m_pUSER_GRP.SetText( nColIdx + 1, 0, variant_t(m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang)));
}

void CUserGroupDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	CString aaa = _T("");
	aaa.Format(_T("%s\r"), strValue);
	m_pUSER_GRP.SetText(nColIdx + 1, nRowIdx + 1, variant_t(aaa));
}

void CUserGroupDlg::InitializeUSER_GRP(BOOL bSearch)
{
	CStringList strList;
	int nSizeRow = GetListUSER_GRP(strList, bSearch);
	int nSizeCol = 0;

	if(nSizeRow == 0)
	{
		CString arrColName[] ={ _T("그룹코드"), _T("그룹설명"), _T("비고"), _T("생성자"), _T("생성일자"), _T("수정자"), _T("수정일자")};

		int nColSize = (sizeof(arrColName)/sizeof(*arrColName));

		m_pUSER_GRP.SetMaxCols(nColSize);
		m_pUSER_GRP.SetMaxRows(0);

		for(int nIdxCol = 1; nIdxCol < nColSize+1; nIdxCol++)
		{
			FillSpreadColumn(nIdxCol, arrColName[nIdxCol-1]);
		}
		return;
	}

	nSizeCol = strList.GetCount() / nSizeRow  +1;

	m_pUSER_GRP.SetMaxCols(nSizeCol);
	m_pUSER_GRP.SetMaxRows(nSizeRow);
	POSITION pos = strList.GetHeadPosition();


	for(int nIdxRow = 0; nIdxRow < nSizeRow; nIdxRow++)
	{
		for(int nIdxCol = 1; nIdxCol < nSizeCol; nIdxCol++)
		{
			CString strTemp = strList.GetNext(pos);
			CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
			if(nIdxRow == 0) { FillSpreadColumn(nIdxCol, StrFieldName); };

			CString strValue = strTemp.Mid(strTemp.Find(';')+1);
			FillSpreadRow(nIdxCol, nIdxRow, strValue);
		}
	}
}

void CUserGroupDlg::OnBnClickedCheckAll()
{
	if (m_chkCheckAll.GetCheck() == TRUE)
	{
		for(int nIdxRow = 1; nIdxRow < m_pUSER_GRP.GetDataRowCnt() + 1; nIdxRow++)
		{
			m_pUSER_GRP.SetText(1, nIdxRow, variant_t(_T("1")));
		}
	}
	else
	{
		for(int nIdxRow = 1; nIdxRow < m_pUSER_GRP.GetDataRowCnt() + 1; nIdxRow++)
		{
			m_pUSER_GRP.SetText(1, nIdxRow, variant_t(_T("0")));
		}
	}
}


void CUserGroupDlg::OnBnClickedBtnDelete2()
{
	DeleteUserMst();
}

void CUserGroupDlg::DeleteUserMst()
{
	CString strSql = _T("");
	CStringList strCvDataList;

	if(IDYES == AfxMessageBox(_T("해당 유저를 삭제 하시겠습니까?"), MB_YESNO))
	{

		variant_t val;

		for (int i = 1; i <= m_pUSER_GRP.GetMaxRows(); i++)
		{
			m_pUSER_GRP.GetText(1, i, &val);
			CString strCheck = (LPCTSTR)(_bstr_t)val;

			if (strCheck == "1")
			{	
				m_pUSER_GRP.GetText(4, i, &val);

				CStringList strListUserGrp;
				CString strUserId = (LPCTSTR)(_bstr_t)val;

				strSql = _T("");
				strSql.Format(_T("DELETE FROM USER_GRP WHERE GRP_CD = '%s' "), strUserId);

				BOOL isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

				if(isSuccess == TRUE)
				{
					AfxMessageBox(m_pDoc->GetMsgLangDef(_T("성공")));
					GetListUSER_GRP(strListUserGrp, TRUE);
					return;
				}
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("실패")));
				return;
			}
		}	
	}
}


void CUserGroupDlg::OnBnClickedBtnSearch2()
{
	InitializeUSER_GRP(TRUE);
}
