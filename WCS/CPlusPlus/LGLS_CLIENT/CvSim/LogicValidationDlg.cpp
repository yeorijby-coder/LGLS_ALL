//CogicValidationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ecs.h"
#include "LogicValidationDlg.h"

#include "EcsDoc.h"
#include "EcsControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////
// CLogicValidationDlg dialog


CLogicValidationDlg::CLogicValidationDlg(CEcsDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogResize(CTrackDataDlg::IDD, pParent)
{
	m_pDoc = pDoc;
}

void CLogicValidationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_LOGIC_GROUP, m_lstLogicGroup);
	DDX_Control(pDX, IDC_LIST_LOGIC_DESC, m_lstLogicDesc);
	DDX_Control(pDX, IDC_LIST_LOGIC_CONDITION, m_lstConditionData);
}

BEGIN_MESSAGE_MAP(CLogicValidationDlg, CDialogResize)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOGIC_GROUP, &CLogicValidationDlg::OnNMDblclkListLogicGroup)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOGIC_DESC, &CLogicValidationDlg::OnNMDblclkListLogicDesc)
	ON_EN_CHANGE(IDC_TRACK_NUMBER, &CLogicValidationDlg::OnEnChangeTrackNumber)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOGIC_CONDITION, &CLogicValidationDlg::OnNMDblclkListLogicCondition)
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CLogicValidationDlg)
	DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
END_DLGRESIZE_MAP()


////////////////////////////////////////////////
// CLogicValidationDlg message handlers

BOOL CLogicValidationDlg::OnInitDialog()
{
	CDialogResize::OnInitDialog();

	CImageList imageList;
	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_GROUP);

#pragma region Ç¥ »ý¼º °ü·Ã
	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BALOON));
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 217, 90, 90, 70 };
		LPCTSTR lpszCol[] = { _T("LOGIC NAME"), _T("ADDR"), _T("INORDER"), _T("VALUE") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_DESC);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_BOX4));
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 40, 427 };
		LPCTSTR lpszCol[] = { _T("ID"), _T("DESC") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_CONDITION);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_HALLOWEEN1));
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 40, 150 ,137, 70, 70 };
		LPCTSTR lpszCol[] = { _T("ID"), _T("CONDITION"), _T("KEYWORD"), _T("VALUE"), _T("RESULT") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}

	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_ACTION);

	imageList.Create(DEF_LVITEM_IMG_SIZE, DEF_LVITEM_IMG_SIZE, ILC_MASK/*bMask*/, 1/*nInitial*/, 1/*nGrow*/);
	imageList.Add(AfxGetApp()->LoadIcon(IDI_HALLOWEEN2));
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	pListCtrl->SetImageList(&imageList, LVSIL_SMALL);
	imageList.Detach();
	{
		UINT uMask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		int nFmt = LVCFMT_LEFT;
		int cx[] = { 40, 150, 137, 70, 70 };
		LPCTSTR lpszCol[] = { _T("ID"), _T("ACTION"), _T("KEYWORD"), _T("VALUE"), _T("RESULT") };
		int nCol = sizeof(lpszCol) / sizeof(lpszCol[0]);

		for (int i = 0; i < nCol; ++i)
			pListCtrl->InsertColumn(i, lpszCol[i], nFmt, cx[i]);
	}
#pragma endregion

	sSelectedGroup = "";

	return TRUE;
}

void CLogicValidationDlg::PostNcDestroy()
{
	m_pDoc->m_pLogicValidationDlg = NULL;
	delete this;

	CDialogResize::PostNcDestroy();
}

void CLogicValidationDlg::OnOK()
{
	ShowWindow(false);
}

#define		SET_REG_INFO(name, data, option)					m_pDoc->SetAddrByName(m_nNumber - 1, nDevNum, name, data, option)
#define		GET_REG_INFO(name)									m_pDoc->GetAddrByName(m_nNumber - 1, nDevNum, name)

void CLogicValidationDlg::OnNMDblclkListLogicGroup(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nId[100];
	CString LogicDesc[100];
	int nLogicGroupCnt = m_pDoc->m_arrLogicGroups.GetSize();

	POSITION pos = m_lstLogicGroup.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstLogicGroup.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstLogicGroup.GetItemCount()))
		return;

	CString sLogicName = m_lstLogicGroup.GetItemText(nIndex, 0);
	sSelectedGroup = sLogicName;

	CLogicArray* pLogicArray = NULL;
	for (int n = 0; n < nLogicGroupCnt; n++)
	{
		pLogicArray = &m_pDoc->m_arrLogicGroups[n];

		if (pLogicArray == NULL)
			continue;

		int nLogicSize = pLogicArray->GetSize();

		CLogic* pLogic = NULL;

		if (pLogicArray->m_strName == sLogicName)
		{
			for (int l = 0; l < nLogicSize; l++)
			{
				pLogic = &pLogicArray->GetAt(l);

				if (pLogic == NULL)	continue;

				nId[l] = pLogic->m_nId;
				LogicDesc[l] = pLogic->m_strDesc;
			}
		}
	}

	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_ACTION);
	pListCtrl->DeleteAllItems();
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_CONDITION);
	pListCtrl->DeleteAllItems();
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_DESC);
	pListCtrl->DeleteAllItems();

	int z = 0;
	for (z = 0; z < 100; z++)
		if (LogicDesc[z] == "")
			break;

	for (int i = 0; i < z; i++)
	{
		int nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%d"), nId[i]);
		lvitem.iImage = 0;
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), LogicDesc[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}
}

#define		GET_REG_INFO(name)									m_pDoc->GetAddrByName(m_nNumber - 1, nDevNum, name)

void CLogicValidationDlg::OnNMDblclkListLogicDesc(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nId[100];
	CString LogicDesc[100];
	int nLogicGroupCnt = m_pDoc->m_arrLogicGroups.GetSize();

	CString strTrackNum;
	GET(IDC_TRACK_NUMBER, strTrackNum);

	int m_nNumber;
	int nDevNum;

	if (strTrackNum.GetLength() == 3)
	{
		m_nNumber = _ttoi(strTrackNum.Mid(0, 1)) + 1;
		nDevNum = _ttoi(strTrackNum.Mid(1, 2)) * m_pDoc->m_nWordCnt;// 10;
	}
	else
	{
		m_nNumber = 1;
		nDevNum = _ttoi(strTrackNum) * m_pDoc->m_nWordCnt;//2;
	}

	POSITION pos = m_lstLogicDesc.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstLogicDesc.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstLogicDesc.GetItemCount()))
		return;

	CString sLogicDesc = m_lstLogicDesc.GetItemText(nIndex, 1);

	int		nCondition[20];
	CString sConditionContent[20];
	CString sConditionKeyArray[20];
	int		nConditionValue[20];

	int		nAction[20];
	CString sActionContent[20];
	CString sActionKeyArray[20];
	int		nActionValue[20];
	int		nActionActualValue[20];

	CLogicArray* pLogicArray = NULL;
	for (int n = 0; n < nLogicGroupCnt; n++)
	{
		pLogicArray = &m_pDoc->m_arrLogicGroups[n];

		if (pLogicArray == NULL)
			continue;

		int nLogicSize = pLogicArray->GetSize();

		CLogic* pLogic = NULL;

		if (pLogicArray->m_strName == sSelectedGroup)
		{
			for (int l = 0; l < nLogicSize; l++)
			{
				pLogic = &pLogicArray->GetAt(l);

				if (pLogic == NULL)	continue;

				if (pLogic->m_strDesc == sLogicDesc)
				{
					int nLogicConditionSize = pLogic->m_arrConditions.GetSize();
					for (int x = 0; x < nLogicConditionSize; x++)
					{
						nCondition[x] = pLogic->m_arrConditions.GetAt(x).m_nType;
						sConditionContent[x] = ContentConvert(nCondition[x]);
						sConditionKeyArray[x] = pLogic->m_arrConditions.GetAt(x).m_strKeyWord;
						nConditionValue[x] = GET_REG_INFO(sConditionKeyArray[x]);

						nConditionValue[x] = GetTrackLogicData(sConditionKeyArray[x], nConditionValue[x], _ttoi(strTrackNum) - 1, pLogic->m_arrConditions.GetAt(x).m_enType, x);
					}
					SET(IDC_CONDITION_DESC, pLogic->m_arrConditions.m_strDesc);

					int nLogicActionSize = pLogic->m_arrActions.GetSize();

					for (int x = 0; x < nLogicActionSize; x++)
					{
						nAction[x] = pLogic->m_arrActions.GetAt(x).m_nType;
						sActionContent[x] = ContentConvert(nAction[x]);
						sActionKeyArray[x] = pLogic->m_arrActions.GetAt(x).m_strKeyWord;
						nActionValue[x] = GET_REG_INFO(sActionKeyArray[x]);

						if (nActionValue[x] == 1) 
							 sActionResult[x] = "TRUE";
						else sActionResult[x] = "FALSE";
					}
					SET(IDC_ACTION_DESC, pLogic->m_arrActions.m_strDesc);
				}
			}
		}
	}

	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_CONDITION);
	pListCtrl->DeleteAllItems();

	int z = 0;
	for (z = 0; z < 100; z++)
		if (sConditionKeyArray[z] == "")
			break;

	for (int i = 0; i < z; i++)
	{
		int nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		lvitem.iImage = 0;
		wsprintf(szBuff, _T("%d"), nCondition[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sConditionContent[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sConditionKeyArray[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%d"), nConditionValue[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sConditionResult[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}
	nLine = 0;

	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_ACTION);
	pListCtrl->DeleteAllItems();

	for (z = 0; z < 100; z++)
		if (sActionKeyArray[z] == "")
			break;

	for (int i = 0; i < z; i++)
	{
		int nCol = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iItem = nLine++;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%d"), nAction[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->InsertItem(&lvitem);

		lvitem.mask = LVIF_TEXT;
		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sActionContent[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sActionKeyArray[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%d"), nActionValue[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);

		lvitem.iSubItem = nCol++;
		wsprintf(szBuff, _T("%s"), sActionResult[i]);
		lvitem.pszText = (LPWSTR)szBuff;
		pListCtrl->SetItem(&lvitem);
	}
}

CString CLogicValidationDlg::ContentConvert(int nNumber)
{
	CString sTemp;

	switch (nNumber)
	{
	case 0:			sTemp = "BIT_OFF";	break;
	case 1:			sTemp = "BIT_ON";	break;
	case 2:			sTemp = "NOT_0";	break;
	case 3:			sTemp = "IS_0";	break;
	case 4:			sTemp = "Ùí";	break;
	case 5:			sTemp = "êó";	break;
	case 6:			sTemp = "IsStation";	break;
	case 7:			sTemp = "á³(Less)";	break;
	case 8:			sTemp = "ÓÞ(Greater)";	break;
	case 9:			sTemp = "á³ÔÒ(Less than or Equal)";	break;
	case 10:		sTemp = "ÓÞÔÒ(Greater than or Equal)";	break;
	case 11:		sTemp = "EqualSelf";	break;
	case 12:		sTemp = "NotEqualSelf";	break;
	case 13:		sTemp = "IsScDestNum";	break;
	case 14:		sTemp = "except";	break;
	case 15:		sTemp = "TimeElapse";	break;
	case 16:		sTemp = "NotScDestNum";	break;

	case 51:		sTemp = "BitOn";	break;
	case 52:		sTemp = "BitOff";	break;
	case 53:		sTemp = "Inverse";	break;
	case 54:		sTemp = "StartFlagOn";	break;
	case 55:		sTemp = "StartFlagOff";	break;
	case 56:		sTemp = "TimeReset";	break;
	case 57:		sTemp = "WORD_Reset";	break;
	case 58:		sTemp = "BitForceOff";	break;
	}

	return sTemp;
}

void CLogicValidationDlg::OnEnChangeTrackNumber()
{
	CString strTrackNum;
	CString strAddr;
	int		nInOrder;
	int		nValue;
	GET(IDC_TRACK_NUMBER, strTrackNum);

	LV_ITEM lvitem;
	TCHAR szBuff[256];
	int nLine = 0;

	int nLogicGroupCnt = m_pDoc->m_arrLogicGroups.GetSize();

	CListCtrl* pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_DESC);
	pListCtrl->DeleteAllItems();
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_ACTION);
	pListCtrl->DeleteAllItems();
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_CONDITION);
	pListCtrl->DeleteAllItems();
	pListCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_LOGIC_GROUP);
	pListCtrl->DeleteAllItems();

	int z;
	for (z = 0; z < 100; z++)
		if (m_pDoc->m_sEcsDefineLoad[_ttoi(strTrackNum)][z] == "")
			break;

	int m_nNumber;
	int nDevNum;

	if (strTrackNum.GetLength() == 3)
	{
		m_nNumber = _ttoi(strTrackNum.Mid(0, 1)) + 1;
		nDevNum = _ttoi(strTrackNum.Mid(1, 2)) * m_pDoc->m_nWordCnt;//10;
	}
	else
	{
		m_nNumber = 1;
		nDevNum = _ttoi(strTrackNum) * m_pDoc->m_nWordCnt;//2;
	}

	for (int i = 0; i < z; i++)
	{
		CTrackProperty* pTrackProperty = NULL;
		int nTrackNum = _ttoi(strTrackNum);
		CString sTemp = m_pDoc->m_sEcsDefineLoad[_ttoi(strTrackNum)][i];
		if ((pTrackProperty = m_pDoc->GetTrackPropertyByKeyword(m_nNumber - 1, nTrackNum, sTemp)) == NULL)
		{
			int nCol = 0;
			lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem = nLine++;
			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), m_pDoc->m_sEcsDefineLoad[_ttoi(strTrackNum)][i]);
			lvitem.iImage = 0;
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->InsertItem(&lvitem);
		}
		else
		{
			strAddr = pTrackProperty->m_strAddr;
			nInOrder = pTrackProperty->m_nInOrder;
			nValue = GET_REG_INFO(sTemp);

			int nCol = 0;
			lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvitem.iItem = nLine++;
			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), m_pDoc->m_sEcsDefineLoad[_ttoi(strTrackNum)][i]);
			lvitem.iImage = 0;
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->InsertItem(&lvitem);

			lvitem.mask = LVIF_TEXT;
			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%s"), strAddr);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);

			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%d"), nInOrder);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);

			lvitem.iSubItem = nCol++;
			wsprintf(szBuff, _T("%d"), nValue);
			lvitem.pszText = (LPWSTR)szBuff;
			pListCtrl->SetItem(&lvitem);
		}

	}
}

int CLogicValidationDlg::GetTrackLogicData(CString strKeyWord, int nGetData, int nTrackNum, int enType, int nCount)
{
	int j = int(nTrackNum/100);
	CString strDevice;
	//if (nTrackNum > 99)	j = 2;
	//else j = 1;
	if (j < 1)
		return 0;

	CCv* pCv = NULL;
	strDevice.Format(_T("CV_E%0.2d"), j);
	CCv* pCvTemp = (CCv*)m_pDoc->m_pEquipments.GetEquipment(strDevice);
	pCv = pCvTemp;

	CTrackInfo* pTrack = NULL;

	int nTrackNumTemp = nTrackNum - (j * 100);
	//if (nTrackNum > 99)
	//	pTrack = pCv->m_pInfo->m_pTracks[nTrackNum - 100];
	//else
		pTrack = pCv->m_pInfo->m_pTracks[nTrackNumTemp];
	COleDateTimeSpan tElapseTime = COleDateTime::GetCurrentTime() - pTrack->m_tTime;

	int nSec = _ttoi(strKeyWord);
	int nTotalSec = tElapseTime.GetTotalSeconds();
	BOOL bIsDestSelf = m_pDoc->IsDestination(strKeyWord, nGetData, pTrack->m_nNumber, 1);
	BOOL bIsScDest = m_pDoc->IsDestination(strKeyWord, nGetData, 0, 2);
	BOOL bIsStation = m_pDoc->IsStationKind(pTrack, strKeyWord);
	BOOL bTimeElapse = (pTrack->m_bStartFlag) && (nTotalSec > nSec);

	BOOL bTemp = FALSE;
	BOOL bConditionResult = TRUE;

	// Condition Check
	switch (enType)
	{
	case BT_SIGN_BIT_OFF:			bTemp = (nGetData == FALSE);	break;		// OFF
	case BT_SIGN_BIT_ON:			bTemp = (nGetData == TRUE);		break;		// ON
	case BT_SIGN_NOT_ZERO:			bTemp = (nGetData != 0);		break;		// NOT 0
	case BT_SIGN_IS_ZERO:			bTemp = (nGetData == 0);		break;		// IS 0
	//case BT_SIGN_NO:				bTemp = (nGetData );			break;		// NO 
	//case BT_SIGN_EXIST:			bTemp = (nGetData );			break;		// EXIST 
	case BT_SIGN_IS_STATION:		bTemp = bIsStation;				break;		// IsStation 
	//case BT_SIGN_LESS:			bTemp = (nGetData );			break;		// LESS <
	//case BT_SIGN_GREATER:			bTemp = (nGetData );			break;		// GREATER > 
	//case BT_SIGN_LTOE:			bTemp = (nGetData );			break;		// LTOE <=
	//case BT_SIGN_GTOE:			bTemp = (nGetData );			break;		// GTOE >=
	case BT_SIGN_EQUAL_SELF:		bTemp = bIsDestSelf;			break;		// EQUAL SELF
	case BT_SIGN_NOT_EQUAL_SELF:	bTemp = !bIsDestSelf;			break;		// NOT EQUAL SELF
	case BT_SIGN_IS_SC_DEST:		bTemp = bIsScDest;				break;		// IS SC DEST NUM
	//case BT_SIGN_EXCEPT:			bTemp = (nGetData );			break;		// EXCEPT
	case BT_SIGN_TIME_ELAPSE:		bTemp = bTimeElapse;			break;		// TIME ELAPSE(5ÃÊ)
	case BT_SIGN_NOT_SC_DEST:		bTemp = !bIsScDest;				break;		// NOT SC DEST NUM
	default:						bTemp = FALSE;					break;		// Á¤ÀÇµÇÁö ¾ÊÀ¸¸é ¹«Á¶°Ç FALSE
	}

	bConditionResult = bConditionResult & bTemp;

	if (bConditionResult == FALSE)
		sConditionResult[nCount] = "FALSE";
	else sConditionResult[nCount] = "TRUE";

	if (enType == BT_SIGN_BIT_OFF || enType == BT_SIGN_BIT_ON || enType == BT_SIGN_NOT_ZERO || enType == BT_SIGN_IS_ZERO)
		return nGetData;
	else
	{
		if (bTemp)	return 1;
	}

	return 0;
}

void CLogicValidationDlg::OnNMDblclkListLogicCondition(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	POSITION pos = m_lstConditionData.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int nIndex = m_lstConditionData.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex >= m_lstConditionData.GetItemCount()))
		return;

	CString sValue = m_lstConditionData.GetItemText(nIndex, 3);

	m_pDoc->TrackConditionData(sValue);
}
