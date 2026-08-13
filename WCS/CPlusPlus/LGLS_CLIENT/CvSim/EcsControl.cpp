// EcsControl.cpp : implementation file
//

#include "stdafx.h"
#include "ecs.h"
#include "EcsControl.h"
#include "EcsDoc.h"
#include "ByteArrayEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEcsControl dialog

CEcsControl::CEcsControl(CWnd* pParent /*=NULL*/)
	: CDialog(CEcsControl::IDD, pParent)
	, m_bStartFlag(FALSE)
{
	//{{AFX_DATA_INIT(CEcsControl)
	m_bTwinCheck = FALSE;
	m_bHSTrack = FALSE;
	//}}AFX_DATA_INIT
}

void CEcsControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEcsControl)
	DDX_Control(pDX, IDC_COMBO_PLC, m_cbxPlcNo);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cbxDevice);
	DDX_Control(pDX, IDC_COMBO_CV_KIND, m_cbxCvKind);
	DDX_Control(pDX, IDC_LIST_REG_DATA, m_lstRegList);
	DDX_Control(pDX, IDC_LIST_TRACK_DATA, m_lstTrackData);
	DDX_Check(pDX, IDC_CHECK1, m_bBit01);
	DDX_Check(pDX, IDC_CHECK2, m_bBit02);
	DDX_Check(pDX, IDC_CHECK3, m_bBit03);
	DDX_Check(pDX, IDC_CHECK4, m_bBit04);
	DDX_Check(pDX, IDC_CHECK5, m_bBit05);
	DDX_Check(pDX, IDC_CHECK6, m_bBit06);
	DDX_Check(pDX, IDC_CHECK7, m_bBit07);
	DDX_Check(pDX, IDC_CHECK8, m_bBit08);
	DDX_Check(pDX, IDC_CHECK9, m_bBit09);
	DDX_Check(pDX, IDC_CHECK10, m_bBit10);
	DDX_Check(pDX, IDC_CHECK11, m_bBit11);
	DDX_Check(pDX, IDC_CHECK12, m_bBit12);
	DDX_Check(pDX, IDC_CHECK13, m_bBit13);
	DDX_Check(pDX, IDC_CHECK14, m_bBit14);
	DDX_Check(pDX, IDC_CHECK15, m_bBit15);
	DDX_Check(pDX, IDC_CHECK16, m_bBit16);
	DDX_Check(pDX, IDC_CHECK17, m_bBit17);
	DDX_Check(pDX, IDC_CHECK18, m_bBit18);
	DDX_Check(pDX, IDC_CHECK19, m_bBit19);
	DDX_Check(pDX, IDC_CHECK20, m_bBit20);
	DDX_Check(pDX, IDC_CHECK21, m_bBit21);
	DDX_Check(pDX, IDC_CHECK22, m_bBit22);
	DDX_Check(pDX, IDC_CHECK23, m_bBit23);
	DDX_Check(pDX, IDC_CHECK24, m_bBit24);
	DDX_Check(pDX, IDC_CHECK25, m_bBit25);
	DDX_Check(pDX, IDC_CHECK26, m_bBit26);
	DDX_Check(pDX, IDC_CHECK27, m_bBit27);
	DDX_Check(pDX, IDC_CHECK28, m_bBit28);
	DDX_Check(pDX, IDC_CHECK29, m_bBit29);
	DDX_Check(pDX, IDC_CHECK30, m_bBit30);
	DDX_Check(pDX, IDC_CHECK31, m_bBit31);
	DDX_Check(pDX, IDC_CHECK32, m_bBit32);
	DDX_Check(pDX, IDC_CHECK33, m_bBit33);
	DDX_Check(pDX, IDC_CHECK34, m_bBit34);
	DDX_Check(pDX, IDC_CHECK35, m_bBit35);
	DDX_Check(pDX, IDC_CHECK36, m_bBit36);
	DDX_Check(pDX, IDC_CHECK37, m_bBit37);
	DDX_Check(pDX, IDC_CHECK38, m_bBit38);
	DDX_Check(pDX, IDC_CHECK39, m_bBit39);
	DDX_Check(pDX, IDC_CHECK40, m_bBit40);
	DDX_Check(pDX, IDC_CHECK41, m_bBit41);
	DDX_Check(pDX, IDC_CHECK42, m_bBit42);
	DDX_Check(pDX, IDC_CHECK43, m_bBit43);
	DDX_Check(pDX, IDC_CHECK44, m_bBit44);
	DDX_Check(pDX, IDC_CHECK45, m_bBit45);
	DDX_Check(pDX, IDC_CHECK46, m_bBit46);
	DDX_Check(pDX, IDC_CHECK47, m_bBit47);
	DDX_Check(pDX, IDC_CHECK48, m_bBit48);
	DDX_Check(pDX, IDC_CHECK_NEXT_NOR, m_bNextNor);
	DDX_Check(pDX, IDC_CHECK_CENTER, m_bCenter);
	DDX_Check(pDX, IDC_CHECK_CENTER_NEXT, m_bCenterNext);
	DDX_Check(pDX, IDC_CHECK_CENTER_DEST, m_bCenterDest);
	DDX_Check(pDX, IDC_CHECK_CROSS, m_bCross);
	DDX_Check(pDX, IDC_CHECK_CROSS_TRACK, m_bCrossTrack);
	DDX_Check(pDX, IDC_CHECK_TWIN, m_bTwinCheck);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_START_FLAG, m_bStartFlag);
}

BEGIN_MESSAGE_MAP(CEcsControl, CDialog)
	//{{AFX_MSG_MAP(CEcsControl)
	ON_COMMAND(ID_VIEW_LAYOUT_INFO, OnViewLayoutInfo)
	ON_COMMAND(ID_UNSET_ALL_SUSPEND, OnUnSetAllSuspend)
	ON_COMMAND(ID_SET_ALL_SUSPEND, OnSetAllSuspend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_LAYOUT_TAB, OnSelchangeLayoutTab)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REG_DATA, OnClickListRegData)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_CBN_SELCHANGE(IDC_COMBO_CV_KIND, OnSelchangeComboCvKind)
	ON_BN_CLICKED(IDC_BUTTON22, OnBtnOpAuto)
	ON_BN_CLICKED(IDC_CHECK_NEXT_NOR, OnCheckNextNor)
	ON_BN_CLICKED(IDC_CHECK_CENTER_NEXT, OnCheckCenterNext)
	ON_BN_CLICKED(IDC_CHECK_CENTER_DEST, OnCheckCenterDest)
	ON_BN_CLICKED(IDC_CHECK_CROSS_TRACK, OnCheckCrossTrack)
	ON_BN_CLICKED(IDC_CHECK_CENTER, OnCheckCenter)
	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
	ON_BN_CLICKED(IDC_BUTTON23, OnBtnOpSuspend)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BTN_ALLAUTO, OnBtnAllauto)
	ON_BN_CLICKED(IDC_BTN_ALLAUTO2, OnBtnAllSuspend)
	ON_BN_CLICKED(IDC_CHECK_TWIN, OnCheckTwin)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
	//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRACK_DATA, &CEcsControl::OnLvnItemchangedListTrackData)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_TRACK_DATA, &CEcsControl::OnNMClickListTrackData)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRACK_DATA, &CEcsControl::OnNMDblclkListTrackData)
ON_BN_CLICKED(IDC_TIME_SET, &CEcsControl::OnBnClickedTimeSet)
ON_BN_CLICKED(IDC_CHK_START_FLAG, &CEcsControl::OnBnClickedChkStartFlag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEcsControl message handlers
void CEcsControl::UpdateList(int nPlcNo, int nDeviceNo)
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);
	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	CString strPlcNo, strDeviceNo;
	strPlcNo.Format(_T("%d"), nPlcNo);
	SET(IDC_COMBO_PLC, strPlcNo);
	strDeviceNo.Format(_T("%d"), nDeviceNo);
	SET(IDC_EDIT_REGNUM, strDeviceNo);

	if (nDeviceNo == 0)
		return;
	pListCtrl->DeleteAllItems();

	int nDeviceNoCnt = nDeviceNo + 100;

	for (int i = nDeviceNo; i < nDeviceNoCnt; i++)
	{
		int nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%.2d"), nPlcNo);
		lvitem.pszText = (LPWSTR)szBuff;
		lvitem.iImage = 0;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("D%04d"), i);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%04d"), m_pDoc->m_arrRegData[nPlcNo - 1][i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%02X %02X"), CLib::GetByteH(m_pDoc->m_arrRegData[nPlcNo - 1][i]), CLib::GetByteL(m_pDoc->m_arrRegData[nPlcNo - 1][i]));
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}
}

#define		GET_REG_INFO(name)									m_pDoc->GetAddrByName(m_nNumber - 1, nDevNum, name)

void CEcsControl::UpdateTrackData(int nPlcNo, int nDeviceNo)
{
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TRACK_DATA);
	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	if (nDeviceNo == 0)
		return;
	pListCtrl->DeleteAllItems();

	CString strType, strInOrder, strAddr;
	int		nValue;

	int m_nNumber = nPlcNo;
	int nDevNum = nDeviceNo;
	m_pDoc->m_nTempPlcNum = m_nNumber;
	m_pDoc->m_nTempDevNum = nDevNum;

	int nTrackNo = int(nDevNum / m_pDoc->m_nWordCnt) + (m_nNumber * 100);
	//if (m_nNumber == 1)
	//	nTrackNo = nDevNum / 2;

	int nCol = 0;
	CTrackProperty* pTrackProperty = NULL;


	CString strValue = _T("");
	
	//// 카운트를 가져와서 For문을 돌리장ㅎㅎㅎ
	//m_pDoc->m_pKeyWords.Lookup(_T("Count"), strValue);

	//int nKeyWordCnt = _ttoi(strValue);

	CString strName = _T("");
	//for (int i = 0; i < nKeyWordCnt; i++)
	//{
	//	strName = m_pDoc->m_pKeyWords[i];
	//}

	//for (CMapStringToString::CPair* pCurVal = m_pDoc->m_pKeyWords.PGetFirstAssoc(); pCurVal != NULL;)
	for (POSITION pos = m_pDoc->m_pKeyWords.GetStartPosition(); pos != NULL;)
	{
		//m_pDoc->m_pKeyWords.Lookup(pCurVal->key, strName);		// 아래거랑 순서 바뀌면 안됨(이미 맨 처음 KeyWord를 가져왔음)

		//pCurVal = m_pDoc->m_pKeyWords.PGetNextAssoc(pCurVal);	// 위에거랑 순서 바뀌면 안됨(맨처음 Keyword를 사용하고 난 다음에)

		m_pDoc->m_pKeyWords.GetNextAssoc(pos, strName, strValue);

		nValue = GET_REG_INFO(strName);

		if ((pTrackProperty = m_pDoc->GetTrackPropertyByKeyword(m_nNumber - 1, nTrackNo, strName)) == NULL)
			continue;

		strType = pTrackProperty->m_strType;
		strAddr = pTrackProperty->m_strAddr;
		strName = pTrackProperty->m_strName;

		nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), strType);
		lvitem.pszText = (LPWSTR)szBuff;
		lvitem.iImage = 0;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), strAddr);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), strName);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%d"), nValue);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}

	//int aaa = nLine;
	/*
	if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == TRUE)
	{
		//LPCTSTR strKeywords[] = { _T("StoStation"), _T("RetStation"), _T("ScStoHS"), _T("ScRetHS"), _T("RtvDepartHS"), _T("RtvArriveHS"),
		//	_T("Emergency"), _T("Auto"), _T("OverTime"), _T("OverLoad"), _T("ProductSensor") };
		LPCTSTR strKeywords[] = { _T("StoStation"), _T("RetStation"), _T("ScStoHS"), _T("ScRetHS"), _T("RtvDepartHS"), _T("RtvArriveHS"),
			_T("Emergency"), _T("OverTime"), _T("OverLoad") };

		int nRow = sizeof(strKeywords) / sizeof(strKeywords[0]);
		for (int j = 0; j < nRow; j++)
		{
			strName = strKeywords[j];

			nValue = GET_REG_INFO(strName);

			if ((pTrackProperty = m_pDoc->GetTrackPropertyByKeyword(m_nNumber - 1, nTrackNo, strName)) == NULL)
				continue;

			strType = pTrackProperty->m_strType;
			strAddr = pTrackProperty->m_strAddr;
			strName = pTrackProperty->m_strName;

			nCol = 0;
			lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem = nLine++;
			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), strType);
			lvitem.pszText = (LPWSTR)szBuff;
			lvitem.iImage = 0;
			pListCtrl->InsertItem(&lvitem);

			lvitem.mask = LVIF_TEXT;
			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), strAddr);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);

			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), strName);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);

			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%d"), nValue);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);
		}
	}
	//*/
	//int bbb = nLine;
}

void CEcsControl::UpdateBits(int nPlcNo, int nDeviceNo)
{
	int nLine = 0;

	CString strRegName;
	//strRegName.Format(_T("D%05d"),nDeviceNo); 기존
	strRegName.Format(_T("D%05d"), nDeviceNo); // G동
	SET(IDC_REG_NAME, strRegName);

	SET_INT(IDC_EDIT_REGNUM, nDeviceNo);

	if (nDeviceNo == 0)
		return;
	m_bBit01 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 0);
	m_bBit02 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 1);
	m_bBit03 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 2);
	m_bBit04 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 3);
	m_bBit05 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 4);
	m_bBit06 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 5);
	m_bBit07 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 6);
	m_bBit08 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 7);
	m_bBit09 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 8);
	m_bBit10 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 9);
	m_bBit11 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 10);
	m_bBit12 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 11);
	m_bBit13 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 12);
	m_bBit14 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 13);
	m_bBit15 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 14);
	m_bBit16 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo], 15);

	strRegName.Format(_T("%05d"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_DECIMAL, strRegName);
	strRegName.Format(_T("%04X"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_HEXA, strRegName);

	UpdateData(FALSE);
}

void CEcsControl::UpdateTrack(int nPlcNo, int nDeviceNo)
{
	int nLine = 0;

	int nTrackNum;

	nTrackNum = nDeviceNo;
	nDeviceNo = nTrackNum % 100 * m_pDoc->m_nWordCnt;
	CString strTrNum, strRegName;
	strTrNum.Format(_T("%03d"), nTrackNum);
	SET(IDC_TRACK_NO, strTrNum);
	strTrNum.Format(_T("TR%03d"), nTrackNum);
	SET(IDC_TRACK_NO2, strTrNum);

	CTrackInfo* pTrack = NULL;

	pTrack = m_pDoc->GetTrackInfo(nTrackNum, nPlcNo);
	if (pTrack != NULL)
	{
		m_cbxCvKind.SetCurSel(pTrack->m_nKind);

		if (pTrack->m_nKind == 0)
		{
			GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_TWIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);
			strTrNum.Format(_T("%03d"), pTrack->m_nNextCv);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			strTrNum.Format(_T("%03d"), pTrack->m_nCenterNext);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			strTrNum.Format(_T("%03d"), pTrack->m_nCenterDest);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			strTrNum.Format(_T("%03d"), pTrack->m_nCrossTrack);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			strTrNum.Format(_T("%03d"), pTrack->m_nHSTrack);
			SET(IDC_EDIT_HS_TRACK, strTrNum);
			m_bCenter = pTrack->m_bCenter;
			m_bCross = pTrack->m_bCross;
			m_bTwinCheck = pTrack->m_bTwinCheck;
			m_pDoc->m_nNextPlcNum = pTrack->m_nNextPlcNum;
			m_pDoc->m_nCenterNextPlcNum = pTrack->m_nCenterNextPlcNum;
			m_pDoc->m_nCrossPlcNum = pTrack->m_nCrossPlcNum;
		}
		else if (pTrack->m_nKind == 1)
		{
			GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TWIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON20)->EnableWindow(TRUE);
			strTrNum.Format(_T("%05d"), 0);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			SET(IDC_EDIT_HS_TRACK, strTrNum);
			m_bCenter = FALSE;
			m_bCross = FALSE;
		}
		else
		{
			GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_TWIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);
			strTrNum.Format(_T("%05d"), 0);
			SET(IDC_EDIT_NEXT_CV, strTrNum);
			SET(IDC_EDIT_CENTER_NEXT, strTrNum);
			SET(IDC_EDIT_CENTER_DEST, strTrNum);
			SET(IDC_EDIT_CROSS_TRACK, strTrNum);
			SET(IDC_EDIT_HS_TRACK, strTrNum);
			m_bCenter = FALSE;
			m_bCross = FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("해당 트랙이 존재하지 않습니다."));
		return;
	}

	CStationInfo* pStation = NULL;
	CTrackHS* pTrackHS = NULL;

	if (pStation = pTrack->GetStoStation())
		SET(IDC_EDIT_STATION, _T("입고대"));
	else if (pStation = pTrack->GetArvStation())
	{
		if (pStation->m_enKind == CStationInfo::enArvStation)
			SET(IDC_EDIT_STATION, _T("도착대"));
		if (pStation->m_enKind == CStationInfo::enArvStation2)
			SET(IDC_EDIT_STATION, _T("도착대2"));
	}
	else if (pStation = pTrack->GetRetStation())
		SET(IDC_EDIT_STATION, _T("출고대"));
	else
		SET(IDC_EDIT_STATION, _T(" "));

	if (pTrackHS = pTrack->GetScRetHS())
		SET(IDC_EDIT_HOME_ST, _T("출고HS"));
	else if (pTrackHS = pTrack->GetScStoHS())
		SET(IDC_EDIT_HOME_ST, _T("입고HS"));
	else if (pTrackHS = pTrack->GetRtvDepartHS())
		SET(IDC_EDIT_HOME_ST, _T("RTV출발 HS"));
	else if (pTrackHS = pTrack->GetRtvArriveHS())
		SET(IDC_EDIT_HOME_ST, _T("RTV도착 HS"));
	else if (pTrackHS = pTrack->GetLgvDepartHS())
		SET(IDC_EDIT_HOME_ST, _T("LGV출발 HS"));
	else if (pTrackHS = pTrack->GetLgvArriveHS())
		SET(IDC_EDIT_HOME_ST, _T("LGV도착 HS"));
	else
		SET(IDC_EDIT_HOME_ST, _T(" "));

	WORD asas = m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 5];

	//SK이노베이션용
	strRegName.Format(_T("%04d"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo]);
	SET(IDC_LUGG_NUM, strRegName);
	strRegName.Format(_T("%04d"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1]);
	SET(IDC_DEST_POS, strRegName);
	strRegName.Format(_T("%02d"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] & 0x000F);
	SET(IDC_JOB_TYPE, strRegName);
	strRegName.Format(_T("%02d"), (m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] >> 4) & 0x000F);
	SET(IDC_EDIT_TURN, strRegName);
	strRegName.Format(_T("%04d"), (m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] >> 8) & 0x00FF);
	SET(IDC_TRAY_TYPE, strRegName);
	strRegName.Format(_T("%04d"), (m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 3]) & 0x000F);
	SET(IDC_EDIT_SIZE, strRegName);
	//	strRegName.Format(_T("%04d"),(m_pDoc->m_arrRegData[nPlcNo-1][nDeviceNo+3] >> 4) & 0x000F);
	//	SET(IDC_EDIT_SIZE,strRegName);
	strRegName.Format(_T("%04d"), (m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4] >> 12) & 0x000F);
	SET(IDC_TRAY_ID, strRegName);
	strRegName.Format(_T("%01d"), (m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4] & 0x000F));
	SET(IDC_SUSPEND, strRegName);
	strRegName.Format(_T("%04d"), m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 6]);
	SET(IDC_ERR_CODE, strRegName);

	WORD wWord = m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] / 100;
	WORD wWord2 = wWord & 0x01;
	WORD wWord3 = wWord >> 1;
	DWORD wdWord = wWord2 << 16;
	DWORD wdWord2 = wdWord | m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4];

	WORD wWord233 = m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7];

	m_bBit17 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 0);
	m_bBit18 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 1);
	m_bBit19 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 2);
	m_bBit20 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 3);
	m_bBit21 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 4);
	m_bBit22 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 5);
	m_bBit23 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 6);
	m_bBit24 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 7);
	m_bBit25 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 8);
	m_bBit26 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 9);
	m_bBit27 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 10);
	m_bBit28 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 11);
	m_bBit29 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 12);
	m_bBit30 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 13);
	m_bBit31 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 14);
	m_bBit32 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7], 15);

	m_bBit33 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 0);
	m_bBit34 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 1);
	m_bBit35 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 2);
	m_bBit36 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 3);
	m_bBit37 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 4);
	m_bBit38 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 5);
	m_bBit39 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 6);
	m_bBit40 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 7);
	m_bBit41 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 8);
	m_bBit42 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 9);
	m_bBit43 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 10);
	m_bBit44 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 11);
	m_bBit45 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 12);
	m_bBit46 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 13);
	m_bBit47 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 14);
	m_bBit48 = CLib::IsSet(m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8], 15);

	UpdateData(FALSE);
}

void CEcsControl::Refresh()
{
	CStationInfo* pStation = NULL;
	CTrackHS* pTrackHS = NULL;
	CTrackInfo* pTrack = NULL;

	pTrack = m_pDoc->GetTrackInfo(m_nTrackNum, m_nCurrPlcNum);

	CString strTime = pTrack->m_tTime.Format(_T("%Y년 %m월 %d일 %H시 %M분 %S초"));
	SET(IDC_STATIC_TIME, strTime);

	if (pTrack != NULL)
		m_bStartFlag = pTrack->m_bStartFlag;

	UpdateData(FALSE);

//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	if (m_pDoc->m_pJobDlg != NULL)
	{
		if (m_bNextNor == FALSE && m_pDoc->m_pJobDlg->m_bNextCv == FALSE && m_pDoc->m_pJobDlg->m_bDestPos == FALSE && m_pDoc->m_pJobDlg->m_bNextCv2 == FALSE &&
			m_pDoc->m_pJobDlg->m_bCenterNext == FALSE && m_pDoc->m_pJobDlg->m_bCenterDest == FALSE && m_pDoc->m_pJobDlg->m_bCrossTrack == FALSE && m_pDoc->m_pJobDlg->m_bCrossDest == FALSE)
		{
			return;

			
			UpdateList(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateBits(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateTrackData(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateTrack(m_nCurrPlcNum, m_nTrackNum);
		}
		else if (m_bNextNor == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_NEXT_CV, strTemp);
			m_pDoc->m_nNextPlcNum = m_nCurrPlcNum;
		}
		else if (m_pDoc->m_pJobDlg->m_bNextCv == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDT_NEXT_TRACK, strTemp);
			m_pDoc->m_nNextPlcNum = m_nCurrPlcNum;

		}
		else if (m_pDoc->m_pJobDlg->m_bDestPos == TRUE)
		{
			CString strTemp;
			if (pStation = pTrack->GetArvStation())
			{
				strTemp.Format(_T("%s"), m_strID);
				m_pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, strTemp);
			}
			else
			{
				strTemp.Format(_T("%05d"), m_nTrackNum);
				m_pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, strTemp);

				//디버터 설정 창이 떠 있을때만 적용
				if (m_pDoc->m_pJobDlg != NULL)
				{
				//	CDestination* pDestination = NULL;
				//	CDestinationArray* pDestinations = NULL;
				//	if ((pDestinations = m_pDoc->GetDestinationsByDestNTrack(pTrack->m_nNumber, pTrack->m_nDestPos)) != NULL)
				//	{
				//		pDestination = pDestinations->GetDestinationByTrack(pTrack->m_nNumber);
				//
						if (/*(pDestination != NULL) && */(m_pDoc->m_pJobDlg->m_bDestPos == TRUE))
							m_pDoc->m_pJobDlg->SET(IDC_EDT_DEST_POS, CConvert::ToString(pTrack->m_nDestCode));
				//	}
				}
			}
		}
		else if (m_pDoc->m_pJobDlg->m_bNextCv2 == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDT_NEXT_TRACK2, strTemp);
			m_pDoc->m_nNextPlcNum2 = m_nCurrPlcNum;
		}
		else if (m_pDoc->m_pJobDlg->m_bCenterNext == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDIT_CENTER_NEXT, strTemp);
			m_pDoc->m_nCenterNextPlcNum = m_nCurrPlcNum;
		}
		else if (m_pDoc->m_pJobDlg->m_bCenterDest == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDIT_CENTER_DEST, strTemp);
		}
		else if (m_pDoc->m_pJobDlg->m_bCrossTrack == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDIT_CROSS_TRACK, strTemp);
			m_pDoc->m_nCrossPlcNum = m_nCurrPlcNum;
		}
		else if (m_pDoc->m_pJobDlg->m_bCrossDest == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDIT_CROSS_DEST, strTemp);
		}
		else if (m_pDoc->m_pJobDlg->m_bTwinCheck == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			m_pDoc->m_pJobDlg->SET(IDC_EDIT_HS_TRACK, strTemp);
		}
	}
	else
	{
		if (m_bNextNor == FALSE && m_bCenterNext == FALSE && m_bCenterDest == FALSE && m_bCrossTrack == FALSE && m_bHSTrack == FALSE)
		{
			UpdateList(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateBits(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateTrackData(m_nCurrPlcNum, (m_nTrackNum % 100) * m_pDoc->m_nWordCnt);
			UpdateTrack(m_nCurrPlcNum, m_nTrackNum);
		}
		else if (m_bNextNor == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_NEXT_CV, strTemp);
			m_pDoc->m_nNextPlcNum = m_nCurrPlcNum;
		}
		else if (m_bCenterNext == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_CENTER_NEXT, strTemp);
			m_pDoc->m_nCenterNextPlcNum = m_nCurrPlcNum;
		}
		else if (m_bCenterDest == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_CENTER_DEST, strTemp);
		}
		else if (m_bCrossTrack == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_CROSS_TRACK, strTemp);
			m_pDoc->m_nCrossPlcNum = m_nCurrPlcNum;
		}
		else if (m_bHSTrack == TRUE)
		{
			CString strTemp;
			strTemp.Format(_T("%05d"), m_nTrackNum);
			SET(IDC_EDIT_HS_TRACK, strTemp);
		}
	}
}

void CEcsControl::InitialRefresh()
{
	m_cbxPlcNo.AddString(_T("01"));
	m_cbxPlcNo.AddString(_T("02"));
	m_cbxPlcNo.AddString(_T("03"));
	m_cbxPlcNo.AddString(_T("04"));
	m_cbxPlcNo.AddString(_T("05"));
	m_cbxPlcNo.AddString(_T("06"));
	m_cbxPlcNo.AddString(_T("07"));
	m_cbxPlcNo.AddString(_T("08"));
	m_cbxPlcNo.AddString(_T("09"));
	m_cbxPlcNo.AddString(_T("10"));
	m_cbxPlcNo.AddString(_T("11"));
	m_cbxPlcNo.AddString(_T("12"));
	m_cbxPlcNo.AddString(_T("13"));
	m_cbxPlcNo.AddString(_T("14"));
	m_cbxPlcNo.AddString(_T("15"));
	m_cbxPlcNo.AddString(_T("16"));
	m_cbxPlcNo.AddString(_T("17"));
	m_cbxPlcNo.AddString(_T("18"));
	m_cbxPlcNo.AddString(_T("19"));
	m_cbxPlcNo.AddString(_T("20"));
	m_cbxPlcNo.SetCurSel(0);

	m_cbxDevice.AddString(_T("D"));
	m_cbxDevice.SetCurSel(0);

	m_cbxCvKind.AddString(_T("0:일반  -N"));
	m_cbxCvKind.AddString(_T("1:디버터-D"));
	m_cbxPlcNo.SetCurSel(0);

	CImageList imageList;
	CListCtrl* pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_REG_DATA);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl1->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 50, 50, 50, 50 };
		LPCTSTR lpszCol[] = { _T("PlcNO"), _T("RegNO"), _T("DEC"), _T("HEX") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	pListCtrl1 = (CListCtrl*)GetDlgItem(IDC_LIST_TRACK_DATA);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOARD));
	pListCtrl1->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl1->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 40, 78, 122, 75 };
		LPCTSTR lpszCol[] = { _T("TYPE"), _T("ADDR"), _T("NAME"), _T("VALUE") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl1->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}
}

void CEcsControl::OnViewLayoutInfo()
{
	m_pDoc->m_layout1.OnViewLayoutInfo(this);
}

void CEcsControl::OnSetAllSuspend()
{
	m_pDoc->SetAllSetSusspend();
}

void CEcsControl::OnUnSetAllSuspend()
{
	m_pDoc->SetAllUnSetSusspend();
}

void CEcsControl::OnButton1()
{
	CString strTemp;
	for (int i = 0; i < 10; ++i)
		strTemp.Format(_T("%d"), i);
	int a = 1;
}

//  조회
void CEcsControl::OnButton5()
{
	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);
	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nDeviceNo = _ttoi(strDeviceNo.Mid(2, 5));
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strRegNum;
	GET(IDC_EDIT_REGNUM, strRegNum);

	int nRegNum = _ttoi(strRegNum);
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	if (nRegNum != 0)
	{
		UpdateList(nPlcNo, nRegNum);
		UpdateBits(nPlcNo, nRegNum);
		UpdateTrackData(nPlcNo, nRegNum);
		UpdateTrack(nPlcNo, nDeviceNo);
		return;
	}

	UpdateList(nPlcNo, nDeviceNo % 100 * m_pDoc->m_nWordCnt);
	UpdateBits(nPlcNo, nDeviceNo % 100 * m_pDoc->m_nWordCnt);
	UpdateTrackData(nPlcNo, nDeviceNo % 100 * m_pDoc->m_nWordCnt);
	UpdateTrack(nPlcNo, nDeviceNo);
}

void CEcsControl::OnButton18()
{
	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo, nTrackNum;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	GET(IDC_REG_NAME, strDeviceNo);						// D값 적용시에는 입력정보의 값을 그대로 사용함!
	if (strDeviceNo.IsEmpty()) return;

	nTrackNum = _ttoi(strDeviceNo.Mid(2, 5));
	nDeviceNo = nTrackNum;// % 1000 * nWordCnt;
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strDecimal;

	int nDecimal;
	GET(IDC_DECIMAL, strDecimal);
	nDecimal = _ttoi(strDecimal);
	if (nDecimal < 0 || nDecimal > 65535)	return;

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = nDecimal;

	GET(IDC_EDIT_REGNUM, strDeviceNo);

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrackData(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, nTrackNum);
}

void CEcsControl::OnButton13()
{
	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo, nTrackNum;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	nTrackNum = _ttoi(strDeviceNo.Mid(2, 5));
	nDeviceNo = nTrackNum % 1000 * 10;
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	CString strHexa;

	GET(IDC_HEXA, strHexa);
	unsigned long nHexa;		//변경할 HEX값
	unsigned long nStartHexa;   //비교할 HEX값 : 0
	unsigned long nEndHexa;		//비교할 HEX값 : FFFF
	nHexa = _tcstoul(strHexa, NULL, 16);
	nStartHexa = _tcstoul(_T("0000"), NULL, 16);
	nEndHexa = _tcstoul(_T("FFFF"), NULL, 16);

	if (nHexa < nStartHexa || nHexa > nEndHexa)
		return;

	TCHAR szHexa[5];
	wsprintf(szHexa, _T("%4s"), strHexa);

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = CLib::HexStrToWORD(szHexa);

	GET(IDC_EDIT_REGNUM, strDeviceNo);

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrackData(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, nTrackNum);
}

void CEcsControl::OnButton12()
{
	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit01)
		nTemp += 0x01;
	if (m_bBit02)
		nTemp += 0x02;
	if (m_bBit03)
		nTemp += 0x04;
	if (m_bBit04)
		nTemp += 0x08;
	if (m_bBit05)
		nTemp += 0x10;
	if (m_bBit06)
		nTemp += 0x20;
	if (m_bBit07)
		nTemp += 0x40;
	if (m_bBit08)
		nTemp += 0x80;
	if (m_bBit09)
		nTemp += 0x100;
	if (m_bBit10)
		nTemp += 0x200;
	if (m_bBit11)
		nTemp += 0x400;
	if (m_bBit12)
		nTemp += 0x800;
	if (m_bBit13)
		nTemp += 0x1000;
	if (m_bBit14)
		nTemp += 0x2000;
	if (m_bBit15)
		nTemp += 0x4000;
	if (m_bBit16)
		nTemp += 0x8000;

	CString strPlcNo, strDeviceNo;

	int nPlcNo, nDeviceNo;
	GET(IDC_COMBO_PLC, strPlcNo);
	nPlcNo = _ttoi(strPlcNo);

	GET(IDC_TRACK_NO2, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	strDeviceNo = strDeviceNo.Mid(2, 5);
	nDeviceNo = GET_INT(IDC_EDIT_REGNUM);
	if (nDeviceNo < 0 || nDeviceNo > 9990)	return;

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = nTemp;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrackData(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, _ttoi(strDeviceNo));
}

void CEcsControl::OnButton17()
{
	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;

	nPlcNo = _ttoi(strDeviceNo.Mid(0, 2));

	nDeviceNo = _ttoi(strDeviceNo);
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	UpdateList(nPlcNo, (nDeviceNo % 100) * m_pDoc->m_nWordCnt);
	UpdateBits(nPlcNo, (nDeviceNo % 100) * m_pDoc->m_nWordCnt);
	UpdateTrackData(nPlcNo, (nDeviceNo % 100) * m_pDoc->m_nWordCnt);
	UpdateTrack(nPlcNo, nDeviceNo);
}

void CEcsControl::OnButton14()
{
	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = nDeviceNo % 100 * m_pDoc->m_nWordCnt;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	nPlcNo = _ttoi(strPlcNo);

	CString strLuggNum, strDestPos, strDeadLock, strJobType, strTrayType, strTrayId, strSuspend, strErrCode, strRgv, strPlc, strTurn, strSize;

	GET(IDC_LUGG_NUM, strLuggNum);
	GET(IDC_DEST_POS, strDestPos);
	GET(IDC_EDIT_SIZE, strSize);
	GET(IDC_JOB_TYPE, strJobType);
	GET(IDC_TRAY_TYPE, strTrayType);

	GET(IDC_TRAY_ID, strTrayId);
	GET(IDC_EDIT_TURN, strTurn);
	GET(IDC_SUSPEND, strSuspend);
	GET(IDC_ERR_CODE, strErrCode);
	GET(IDC_TRAY_ID_2, strRgv);
	GET(IDC_EDIT_LGV_AGREE, strPlc);

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = _ttoi(strLuggNum);

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1] = _ttoi(strDestPos);
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] = (_ttoi(strTrayType) << 8) + (_ttoi(strTurn) << 4) + _ttoi(strJobType);
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 3] = _ttoi(strSize);
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4] = (_ttoi(strTrayId) << 12) + _ttoi(strSuspend);
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 6] = _ttoi(strErrCode);

	int kkk = m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1];

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
}

void CEcsControl::OnButton15()
{
	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 100) * m_pDoc->m_nWordCnt;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	nPlcNo = _ttoi(strPlcNo);
	if (nPlcNo == 0)
		nPlcNo++;

	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit17)
		nTemp += 0x01;
	if (m_bBit18)
		nTemp += 0x02;
	if (m_bBit19)
		nTemp += 0x04;
	if (m_bBit20)
		nTemp += 0x08;
	if (m_bBit21)
		nTemp += 0x10;
	if (m_bBit22)
		nTemp += 0x20;
	if (m_bBit23)
		nTemp += 0x40;
	if (m_bBit24)
		nTemp += 0x80;
	if (m_bBit25)
		nTemp += 0x100;
	if (m_bBit26)
		nTemp += 0x200;
	if (m_bBit27)
		nTemp += 0x400;
	if (m_bBit28)
		nTemp += 0x800;
	if (m_bBit29)
		nTemp += 0x1000;
	if (m_bBit30)
		nTemp += 0x2000;
	if (m_bBit31)
		nTemp += 0x4000;
	if (m_bBit32)
		nTemp += 0x8000;

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] = nTemp;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
}

void CEcsControl::OnButton16()
{
	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 100) * m_pDoc->m_nWordCnt;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	nPlcNo = _ttoi(strPlcNo);

	int nTemp = 0;

	UpdateData(TRUE);

	if (m_bBit33)
		nTemp += 0x01;
	if (m_bBit34)
		nTemp += 0x02;
	if (m_bBit35)
		nTemp += 0x04;
	if (m_bBit36)
		nTemp += 0x08;
	if (m_bBit37)
		nTemp += 0x10;
	if (m_bBit38)
		nTemp += 0x20;
	if (m_bBit39)
		nTemp += 0x40;
	if (m_bBit40)
		nTemp += 0x80;
	if (m_bBit41)
		nTemp += 0x100;
	if (m_bBit42)
		nTemp += 0x200;
	if (m_bBit43)
		nTemp += 0x400;
	if (m_bBit44)
		nTemp += 0x800;
	if (m_bBit45)
		nTemp += 0x1000;
	if (m_bBit46)
		nTemp += 0x2000;
	if (m_bBit47)
		nTemp += 0x4000;
	if (m_bBit48)
		nTemp += 0x8000;

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8] = nTemp;

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
}

void CEcsControl::OnButton19()
{
	CString strPlcNo, strTrNum, strCvKind, strNextCv;
	CString strCenterNext, strCenterDest, strCrossTrack, strHSTrack;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;
	GET(IDC_COMBO_CV_KIND, strCvKind);
	if (strCvKind.IsEmpty()) return;
	GET(IDC_EDIT_NEXT_CV, strNextCv);
	int nNextCv = _ttoi(strNextCv);
	if (nNextCv <= 0) return;

	UpdateData(TRUE);

	int nCenterNext, nCenterDest, nCrossTrack, nCenterNextPlcNum, nCrossPlcNum, nHSTrack;

	if (m_bCenter)
	{
		GET(IDC_EDIT_CENTER_NEXT, strCenterNext);
		nCenterNext = _ttoi(strCenterNext);
		if (nCenterNext <= 0) return;
		GET(IDC_EDIT_CENTER_DEST, strCenterDest);
		nCenterDest = _ttoi(strCenterDest);
		if (nCenterDest <= 0) return;
		nCenterNextPlcNum = m_pDoc->m_nCenterNextPlcNum;
	}
	else
	{
		nCenterNext = 0;
		nCenterDest = 0;
		nCenterNextPlcNum = 0;
	}

	if (m_bCross)
	{
		GET(IDC_EDIT_CROSS_TRACK, strCrossTrack);
		nCrossTrack = _ttoi(strCrossTrack);
		if (nCrossTrack < 0) return;
		nCrossPlcNum = m_pDoc->m_nCrossPlcNum;
	}
	else
	{
		nCrossTrack = 0;
		nCrossPlcNum = 0;
	}

	if (m_bTwinCheck)
	{
		GET(IDC_EDIT_HS_TRACK, strHSTrack);
		nHSTrack = _ttoi(strHSTrack);
	}
	else nHSTrack = 0;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	CTrackInfo* pTrack = NULL;

	pTrack = m_pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack != NULL)
	{
		pTrack->m_nKind = _ttoi(strCvKind.Mid(0, 1));
		pTrack->m_nNextCv = nNextCv;
		pTrack->m_nNextPlcNum = m_pDoc->m_nNextPlcNum;
		pTrack->m_bCenter = m_bCenter;
		pTrack->m_nCenterNextPlcNum = nCenterNextPlcNum;
		pTrack->m_nCenterNext = nCenterNext;
		pTrack->m_nCenterDest = nCenterDest;
		pTrack->m_bCross = m_bCross;
		pTrack->m_nCrossTrack = nCrossTrack;
		pTrack->m_nCrossPlcNum = nCrossPlcNum;
		pTrack->m_bTwinCheck = m_bTwinCheck;
		pTrack->m_nHSTrack = nHSTrack;

		pTrack->SaveXML();
		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = FALSE;
		UpdateData(FALSE);
		pTrack->m_bModified = TRUE;
		pTrack->InvokeControl(pTrack->m_pTrackCtrl);
	}
}

void CEcsControl::OnButton20()
{
	CString strPlcNo, strTrNum;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	CTrackInfo* pTrack = NULL;

	pTrack = m_pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack == NULL)
		return;
	if (m_pDoc->m_pJobDlg != NULL)
	{
		m_pDoc->m_pJobDlg->ShowWindow(SW_SHOWNORMAL);
		m_pDoc->m_pJobDlg->BringWindowToTop();
	}
	else
	{
		m_pDoc->m_pJobDlg = new CJobDlg(m_pDoc, pTrack);
		m_pDoc->m_pJobDlg->Create(IDD_JOB);
		m_pDoc->m_pJobDlg->ShowWindow(SW_SHOW);
	}
}

void CEcsControl::OnSelchangeComboCvKind()
{
	CString strPlcNo, strTrNum, strCvKind;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	GET(IDC_COMBO_CV_KIND, strCvKind);

	int nKind = _ttoi(strCvKind.Mid(0, 1));
	int nSel = m_cbxCvKind.GetCurSel();

	CTrackInfo* pTrack = NULL;

	pTrack = m_pDoc->GetTrackInfo(nTrNum, nPlcNo);

	if (pTrack == NULL)
		return;
	if (nSel == 0)
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);

		m_bNextNor = TRUE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"), pTrack->m_nNextCv);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCenterNext);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCenterDest);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		strTrNum.Format(_T("%05d"), pTrack->m_nCrossTrack);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
	}
	else if (nSel == 1)
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(TRUE);

		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"), 0);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
	}
	else
	{
		GetDlgItem(IDC_EDIT_NEXT_CV)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON19)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CENTER_DEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CROSS_TRACK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON20)->EnableWindow(FALSE);

		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;

		UpdateData(FALSE);
		strTrNum.Format(_T("%05d"), 0);
		SET(IDC_EDIT_NEXT_CV, strTrNum);
		SET(IDC_EDIT_CENTER_NEXT, strTrNum);
		SET(IDC_EDIT_CENTER_DEST, strTrNum);
		SET(IDC_EDIT_CROSS_TRACK, strTrNum);
	}
}

void CEcsControl::OnClickListRegData(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_lstRegList.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstRegList.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstRegList.GetItemCount()))
		return;

	int nPlcNo = _ttoi(m_lstRegList.GetItemText(nIndex, 0));
	CString strDevNo = m_lstRegList.GetItemText(nIndex, 1);
	int nDeviceNo = _ttoi(strDevNo.Mid(1, 4));

	UpdateBits(nPlcNo, nDeviceNo);

	*pResult = 0;
}

void CEcsControl::OnCheckNextNor()
{
	UpdateData(TRUE);

	if (m_bNextNor)
	{
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnCheckCenterNext()
{
	UpdateData(TRUE);

	if (m_bCenterNext)
	{
		m_bNextNor = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnCheckCenterDest()
{
	UpdateData(TRUE);

	if (m_bCenterDest)
	{
		m_bCenterNext = FALSE;
		m_bNextNor = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnCheckCrossTrack()
{
	UpdateData(TRUE);

	if (m_bCrossTrack)
	{
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bNextNor = FALSE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnCheckCenter()
{
	UpdateData(TRUE);

	if (m_bCenter)
	{
		m_bNextNor = FALSE;
		m_bCenterNext = TRUE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnCheckCross()
{
	UpdateData(TRUE);

	if (m_bCross)
	{
		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = TRUE;
		m_bHSTrack = FALSE;
		m_bTwinCheck = FALSE;
	}

	UpdateData(FALSE);
}
void CEcsControl::OnButtonSave()
{
	for (int i = 1; i < 25; ++i)
	{
		CCv* pCv = (CCv*)m_pDoc->GetEquipment(CEquipment::enCV, i);

		if (pCv == NULL)
			continue;
		CTrackInfo* pTrack = NULL;
		for (int j = 0; j < pCv->m_pInfo->m_pTracks.GetSize(); ++j)
		{
			pTrack = pCv->m_pInfo->m_pTracks[j];

			pTrack->SaveXML();
		}
	}
}

void CEcsControl::OnButton6()
{
	m_pDoc->OnJobTestStop();
}

//전체자동
void CEcsControl::OnBtnAllauto()
{
	if (m_pDoc->m_nPlcCnt < 0)
		return;

	for (int nPlcNo = 1; nPlcNo <= m_pDoc->m_nPlcCnt; nPlcNo++)		// 1부터 시작하니까 < 가 아니라. <= 이어야함!
	{
		CCv* pCv = (CCv*)m_pDoc->GetEquipment(CEquipment::enCV, nPlcNo);
		CTrackInfo* pTrack = NULL;

		if (pCv == NULL)	return;

		int nTrackCnt = pCv->m_pInfo->m_pTracks.GetSize();
		for (int i = 0; i < nTrackCnt; ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * m_pDoc->m_nWordCnt;

			int nInOrder = -1;
			int nParent = 0;
			if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == TRUE)
			{
				//nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * 2;

				// 신호가 분리되어 있는 경우에 Parent 값을 가져오는 방법 이같이 사용해야할것
				//m_pDoc->m_bParent = TRUE;			
				nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), FALSE);	
				nInOrder = m_pDoc->GetBitInOrderByWord(nPlcNo - 1, nDeviceNo, _T("Auto"));
			}
			else
			{
				nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("StatusData"));
			}
			int nRessult = m_pDoc->SetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), nParent, 1);

			m_pDoc->m_bParent = TRUE;
			nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"));
		}
	}
}

//op자동버튼 
void CEcsControl::OnBtnOpAuto()
{
	CString strPlcNo, strTrNum, strCvKind, strNextCv;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);

	//nPlcNo = 7;
	CCv* pCv = (CCv*)m_pDoc->GetEquipment(CEquipment::enCV, nPlcNo);

	CTrackInfo* pTrack = NULL;

	if (pCv == NULL)	return;

	int nTrackCnt = pCv->m_pInfo->m_pTracks.GetSize();
	for (int i = 0; i < nTrackCnt; ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * m_pDoc->m_nWordCnt;

		int nInOrder = -1;
		int nParent = 0;
		if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == TRUE)
		{
			// 없어도 될듯!!
			//nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * m_pDoc->m_nWordCnt;

			// 신호가 분리되어 있는 경우에 Parent 값을 가져오는 방법 이같이 사용해야할것
			//m_pDoc->m_bParent = TRUE;			
			nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), FALSE);	

			nInOrder = m_pDoc->GetBitInOrderByWord(nPlcNo - 1, nDeviceNo, _T("Auto"));
		}
		else
		{
			nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("StatusData"));
		}
		int nRessult = m_pDoc->SetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), nParent, 1);

		m_pDoc->m_bParent = TRUE;
		nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"));
	}
}

void CEcsControl::OnBtnAllSuspend()
{
	if (m_pDoc->m_nPlcCnt < 0)
		return;

	for (int nPlcNo = 1; nPlcNo <= m_pDoc->m_nPlcCnt; nPlcNo++)		// 1부터 시작하니까 < 가 아니라. <= 이어야함!
	{
		CCv* pCv = (CCv*)m_pDoc->GetEquipment(CEquipment::enCV, nPlcNo);

		if (pCv == NULL)	return;

		CTrackInfo* pTrack = NULL;

		for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
		{
			pTrack = pCv->m_pInfo->m_pTracks[i];

			int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * m_pDoc->m_nWordCnt;

			int nParent = 0;
			if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == TRUE)
			{
				nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), TRUE);
			}
			else
			{
				nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("StatusData"), TRUE);
			}
			m_pDoc->SetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), nParent, 4);
		}
	}
}

//op수동버튼
void CEcsControl::OnBtnOpSuspend()
{
	CString strPlcNo, strTrNum, strCvKind, strNextCv;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;
	GET(IDC_TRACK_NO, strTrNum);
	if (strTrNum.IsEmpty()) return;

	int nPlcNo, nTrNum;

	nPlcNo = _ttoi(strPlcNo);
	nTrNum = _ttoi(strTrNum);
	CCv* pCv = (CCv*)m_pDoc->GetEquipment(CEquipment::enCV, nPlcNo);
	if (pCv == NULL)
		return;
	CTrackInfo* pTrack = NULL;
	for (int i = 0; i < pCv->m_pInfo->m_pTracks.GetSize(); ++i)
	{
		pTrack = pCv->m_pInfo->m_pTracks[i];

		int nDeviceNo = (pTrack->m_nNumber - pCv->m_nStTrNum + 1) * m_pDoc->m_nWordCnt;

		int nParent = 0;
		if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == TRUE)
		{
			nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), TRUE);
		}
		else
		{
			nParent = m_pDoc->GetAddrByName(nPlcNo - 1, nDeviceNo, _T("StatusData"), TRUE);
		}

		m_pDoc->SetAddrByName(nPlcNo - 1, nDeviceNo, _T("Auto"), nParent, 4);
	}
}

void CEcsControl::OnSelchangeLayoutTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	CRect rect;

	GetWindowRect(&rect);

	int h = rect.Height();
	int w = rect.Width();

	this->Invalidate();

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(w, h));

	*pResult = 0;
}

LRESULT CEcsControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_USER_REFRESH_NOTIFY:
	{

	}
	break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CEcsControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	this->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CEcsControl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == 'W')  // prevent close dialog
		{
			OnButton19();
			return true;
		}
		else if (pMsg->wParam == 'Q')
		{
			m_cbxCvKind.SetCurSel(0);
			GetDlgItem(IDC_CHECK_NEXT_NOR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON19)->EnableWindow(TRUE);
			m_bNextNor = TRUE;
		}
		else if (pMsg->wParam == 'D')
		{
			OnBtnDelete();
		}
		else if (pMsg->wParam == 'E')
		{
			OnButton14();
			return true;
		}
		else if (pMsg->wParam == 'R')
		{
			OnButton15();
			return true;
		}
		else if (pMsg->wParam == 'T')
		{
			OnButton16();
			return true;
		}
		else if (pMsg->wParam == 'S')
		{
			OnButton5();
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEcsControl::OnCheckTwin()
{
	UpdateData(TRUE);

	if (m_bTwinCheck)
	{
		m_bNextNor = FALSE;
		m_bCenterNext = FALSE;
		m_bCenterDest = FALSE;
		m_bCrossTrack = FALSE;
		m_bHSTrack = TRUE;
	}

	UpdateData(FALSE);
}

void CEcsControl::OnBtnDelete()
{
	CString strPlcNo, strDeviceNo;

	GET(IDC_TRACK_NO, strDeviceNo);
	if (strDeviceNo.IsEmpty()) return;

	GET(IDC_COMBO_PLC, strPlcNo);
	if (strPlcNo.IsEmpty()) return;

	int nPlcNo, nDeviceNo;
//	int nWordCnt = 2;
//	if (m_pDoc->m_pDeviceMaps[m_nCurrPlcNum - 1]->m_bUseSeparatelyETC == FALSE)
//	{
//		nWordCnt = 10;
//	}

	nDeviceNo = _ttoi(strDeviceNo);
	nDeviceNo = (nDeviceNo % 100) * m_pDoc->m_nWordCnt;
	if (nDeviceNo < 0 || nDeviceNo > 9999)	return;

	nPlcNo = _ttoi(strPlcNo);

	int nTemp = 0;

	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo] = 0;
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 1] = 0;
	m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 2] = 0;
	if (m_pDoc->m_pDeviceMaps[nPlcNo - 1]->m_bUseSeparatelyETC == FALSE)
	{
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 3] = 0;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 4] = 0;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 5] = 0;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 6] = 0;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 7] &= 0x0001;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 8] = 0;
		m_pDoc->m_arrRegData[nPlcNo - 1][nDeviceNo + 9] = 0;
	}

	UpdateList(nPlcNo, nDeviceNo);
	UpdateBits(nPlcNo, nDeviceNo);
	UpdateTrackData(nPlcNo, nDeviceNo);
	UpdateTrack(nPlcNo, _ttoi(strDeviceNo));
}

void CEcsControl::OnNMDblclkListTrackData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_lstTrackData.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstTrackData.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstTrackData.GetItemCount()))
		return;

	CString sType = m_lstTrackData.GetItemText(nIndex, 0);
	CString sAddr = m_lstTrackData.GetItemText(nIndex, 1);
	CString sName = m_lstTrackData.GetItemText(nIndex, 2);
	CString sValue = m_lstTrackData.GetItemText(nIndex, 3);

	m_pDoc->TrackData(sType, sAddr, sName, sValue);
}

void CEcsControl::OnBnClickedTimeSet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTrackInfo * pTrack = m_pDoc->GetTrackInfo(m_nTrackNum, m_nCurrPlcNum);

	if (pTrack != NULL)
		pTrack->m_tTime = COleDateTime::GetCurrentTime();

	Refresh();
}


void CEcsControl::OnBnClickedChkStartFlag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (UpdateData())
	{
		CTrackInfo* pTrack = m_pDoc->GetTrackInfo(m_nTrackNum, m_nCurrPlcNum);

		if (pTrack != NULL)
			pTrack->m_bStartFlag = m_bStartFlag;
	}

	Refresh();
}
