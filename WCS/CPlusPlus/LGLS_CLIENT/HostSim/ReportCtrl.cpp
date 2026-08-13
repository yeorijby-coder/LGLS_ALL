

#include "stdafx.h"
#include "Ecs.h"

#include "ReportCtrl.h"
#include "EcsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportCtrl

CReportCtrl::CReportCtrl()
{

}

CReportCtrl::~CReportCtrl()
{

}


BEGIN_MESSAGE_MAP(CReportCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CReportCtrl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportCtrl message handlers

void CReportCtrl::PreSubclassWindow() 
{
//	DWORD dwStyle = GetWindowLongPtr(GetSafeHwnd(), GWL_STYLE); 
//	SetWindowLong(GetSafeHwnd(), GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

	ModifyStyle( LVS_TYPEMASK, (GetStyle() & ~LVS_TYPEMASK ) | LVS_REPORT );

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CListCtrl::PreSubclassWindow();

	CHeaderCtrl *pHead = this->GetHeaderCtrl();
	HWND hwnd = pHead->GetSafeHwnd();
	m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() );
}

int CReportCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CReportCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();	
}

void CReportCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	const int iColumn = pNMListView->iSubItem;
	m_ctlHeader.SetSortArrow( iColumn );

	*pResult = 0;
}

void CReportCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    *pResult = CDRF_DODEFAULT;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		LVITEM lvitem;
		lvitem.mask = LVIF_PARAM;
		lvitem.iItem = pLVCD->nmcd.dwItemSpec;
		lvitem.iSubItem = 1;

		if ( GetItem(&lvitem) )
		{
			//CFont font;
			//font.CreateFont (8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));//_T("MS Sans serif") );
			//CDC *pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			//pDC->SelectObject(&font);

			switch ( lvitem.lParam )
			{
			case	LOG_TYPE_ERROR:
				pLVCD->clrText = RED;
				break;

			case	LOG_TYPE_ALARM:
				pLVCD->clrText = ORANGE;
				break;

			case	LOG_TYPE_DEBUG:
				pLVCD->clrText = BLUE;
				break;
			}

	        // Tell Windows to paint the control itself.
		    *pResult = CDRF_DODEFAULT; //CDRF_NEWFONT;
		}
	}
}

void CReportCtrl::InsertColumn(LPCTSTR pszText, ...)
{
	if ( pszText == NULL )
		return;

	LV_COLUMN		lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;
	lvcolumn.iSubItem = 0;
	lvcolumn.pszText = (LPTSTR)pszText;
	lvcolumn.cx = GetStringWidth(lvcolumn.pszText)+15;
	CListCtrl::InsertColumn(lvcolumn.iSubItem++, &lvcolumn);

 	va_list list;
	va_start( list, pszText );
	
	LPCTSTR pszTemp = NULL;
	for ( ; ; )
	{
		pszTemp = va_arg( list, LPCTSTR );
		CString strTemp = pszTemp;
		if ( strTemp == "ETX" )
			break;

		if ( pszTemp == NULL )
			break;

		lvcolumn.iSubItem++;
		lvcolumn.pszText = (LPTSTR)pszTemp;
		lvcolumn.cx = GetStringWidth(lvcolumn.pszText)+15;
		CListCtrl::InsertColumn(lvcolumn.iSubItem, &lvcolumn);
	}
	va_end( list );

//	CHeaderCtrl *pHead = GetHeaderCtrl();
//	HWND hwnd = pHead->GetSafeHwnd();
//	ASSERT( m_ctlHeader.SubclassWindow( hwnd ) );
}

void CReportCtrl::AddItem(LPARAM lParam, int nImage, CStringList *plistText)
{
	if ( plistText->IsEmpty() )
		return;

	POSITION pos = plistText->GetHeadPosition();
	CString strText = plistText->GetNext(pos);

	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvitem.iItem = GetItemCount();
	lvitem.iSubItem = 0;
	lvitem.iImage = nImage;
	lvitem.lParam = lParam;
	lvitem.pszText = (LPTSTR)(LPCTSTR)strText;
	CListCtrl::InsertItem(&lvitem);

	lvitem.mask &= (~LVIF_PARAM & ~LVIF_IMAGE);
	for ( ; pos != NULL; )
	{
		strText = plistText->GetNext(pos);

		lvitem.iSubItem++;
		lvitem.pszText = (LPTSTR)(LPCTSTR)strText;
		CListCtrl::SetItem(&lvitem);
	}
}

void CReportCtrl::LoadImage(int nIconID, ...)
{
	CEcsApp* pApp = (CEcsApp *)AfxGetApp();
	CImageList imageList;
	imageList.Create(16, 16, TRUE/*bMask*/,	1/*nInitial*/, 1/*nGrow*/);

	HICON hIcon = pApp->LoadIcon(nIconID);
	if ( hIcon == NULL )
		return;
	imageList.Add(hIcon);

 	va_list list;
	va_start( list, nIconID );

	for ( ; ; )
	{
		int nID = va_arg( list, int );
		if ( nID == -1 )
			break;

		hIcon = pApp->LoadIcon(nID);
		if ( hIcon == NULL )
			break;

		imageList.Add(hIcon);
	}
	va_end( list );

	SetImageList(&imageList, LVSIL_SMALL);

	imageList.Detach();
}

void CReportCtrl::ResizeColumn()
{
	if ( GetItemCount() > 0 )
	{
		CHeaderCtrl* pHD = GetHeaderCtrl();		//(CHeaderCtrl*)GetDlgItem(0); 
		int nCount = pHD->GetItemCount();
		for(int i =0; i < nCount; i++)
			SetColumnWidth(i, LVSCW_AUTOSIZE);	// 컬럼의 폭 설정
	}
}

