// ViewSemiJobDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Ecs.h"
#include "ViewSemiJobDlg.h"
#include "afxdialogex.h"


// CViewSemiJobDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewSemiJobDlg, CSkinDialog)

CViewSemiJobDlg::CViewSemiJobDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewSemiJobDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CViewSemiJobDlg::CViewSemiJobDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CSkinDialog(CViewSemiJobDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
}

CViewSemiJobDlg::~CViewSemiJobDlg()
{
	this->DestroyWindow();
}

void CViewSemiJobDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_VIEW_SEMIJOB_OK, m_btnOk);
	DDX_Control(pDX, ID_VIEW_SEMIJOB_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_SEMIJOB_LIST, m_pSpdSemiJob);
}


BEGIN_MESSAGE_MAP(CViewSemiJobDlg, CSkinDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_VIEW_SEMIJOB_OK, &CViewSemiJobDlg::OnBnClickedViewSemijobOk)
	ON_BN_CLICKED(ID_VIEW_SEMIJOB_CANCEL, &CViewSemiJobDlg::OnBnClickedViewSemijobCancel)
END_MESSAGE_MAP()


BOOL CViewSemiJobDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();
	if( !m_bInitialized )
	{
		m_bInitialized = TRUE;

		m_btnOk.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		m_btnOk.SetIcon(Global.GetIcon(IDX_ICON_OK, ICON16), NULL, 5);

		m_btnCancel.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE), Global.GetRGB(IDX_RGB_MASK));
		m_btnCancel.SetIcon(Global.GetIcon(IDX_ICON_CANCEL, ICON16), NULL, 5);


		//IDC_LOGIN_PICTUREBOX
		HBITMAP hbitBg;
		hbitBg = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_SPLASH_ECS));
		//m_bgPic.SetBitmap(hbitBg);
		RelocationControls();
	}
	Invalidate(TRUE);
	InitializeUSER_MST();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CViewSemiJobDlg::OnSize(UINT nType, int cx, int cy)
{
	CSkinDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if( m_bInitialized )
		RelocationControls();
}


HCURSOR CViewSemiJobDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewSemiJobDlg::OnPaint()
{
	CSkinDialog::OnPaint();
}

void CViewSemiJobDlg::RelocationControls()
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


BOOL CViewSemiJobDlg::PreTranslateMessage(MSG* pMsg)
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

// CViewSemiJobDlg 메시지 처리기입니다.


void CViewSemiJobDlg::OnBnClickedViewSemijobOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CViewSemiJobDlg::OnBnClickedViewSemijobCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
	CSkinDialog::OnCancel();
}

void CViewSemiJobDlg::InitializeUSER_MST()
{
	CStringList strList;
	int nSizeRow = GetListSEMI_JOB(strList);
	int nSizeCol = strList.GetCount() / nSizeRow;

	m_pSpdSemiJob.SetMaxCols(nSizeCol);
	m_pSpdSemiJob.SetMaxRows(nSizeRow);
	POSITION pos = strList.GetHeadPosition();
	for(int nIdxRow = 0; nIdxRow < nSizeRow; nIdxRow++)
	{
		for(int nIdxCol = 0; nIdxCol < nSizeCol; nIdxCol++)
		{
			CString strTemp = strList.GetNext(pos);
			CString StrFieldName = strTemp.Mid(0, strTemp.Find(';'));
			if(nIdxRow == 0) { FillSpreadColumn(nIdxCol, StrFieldName); };

			CString strValue = strTemp.Mid(strTemp.Find(';')+1);
			FillSpreadRow(nIdxCol, nIdxRow, strValue);
		}
	}
}

int CViewSemiJobDlg::GetListSEMI_JOB(CStringList &strListUSER_GRP)
{
	CString strSql = _T("");

	strSql.Format(_T(" SELECT WH_TYP as 창고구분, LUGG_NO as 작업번호, START_POS as 출발지, START_LOCATION as 출발랙, DEST_POS as 도착지, DEST_LOCATION as 도착랙, TURN as 턴여부, PRODUCT_ID as 제품번호, PRODUCT_SIZE as 제품크기, JOB_TYP as 작업구분, BCR_DATA0 as 바코드, BCR_DATA1 as 바코드1, BCR_DATA2 as 바코드2, BCR_DATA3 as 바코드3, MES_WRITE_YN as MES보고여부, JOB_KIND as 작업구분, JOB_STATUS as 작업상태, JOB_PRIORITY as 우선순위, MES_ERROR_CD as MES에러, OD_LAST_PAGE as 지시프로그램, OD_LAST_USER as 지시자, JOB_START_DT as 최초구동시간, UPD_DT as 수정시간, UPD_USER_ID as 수정자, INS_DT as 생성시간, INS_USER_ID as 생성자")
				_T("   FROM JOB_MST"));

	int nSqlResult = 0;// m_pDB->SelectQry(strSql, strListUSER_GRP);

	return nSqlResult;
}

void CViewSemiJobDlg::FillSpreadColumn(int nColIdx, CString strColumnName)
{
	m_pSpdSemiJob.SetText( nColIdx + 1, 0, variant_t(m_pDoc->m_pLang->GetLangValue(strColumnName, m_pDoc->m_enLang)));
}

void CViewSemiJobDlg::FillSpreadRow(int nColIdx, int nRowIdx, CString strValue)
{
	m_pSpdSemiJob.SetText(nColIdx + 1, nRowIdx + 1, variant_t(strValue));
}
