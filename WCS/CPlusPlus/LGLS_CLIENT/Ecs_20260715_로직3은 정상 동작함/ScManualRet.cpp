// ScManualRet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EcsDoc.h"
#include "Ecs.h"
#include "ScManualRet.h"
#include "afxdialogex.h"
#include "RecordSetWrap.h"


// CScManualRet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScManualRet, CDialogEx)

CScManualRet::CScManualRet(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CScManualRet::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
}

CScManualRet::CScManualRet(CEcsDoc* pDoc, CWnd* pParent)
	: CSkinDialog(CScManualRet::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitialized = FALSE;
	m_pDoc = pDoc;
	m_nLang = m_pDoc->m_enLang;
	m_pSC_DATA = NULL;

}
CScManualRet::~CScManualRet()
{
	m_pDoc->m_pScManualRet = NULL;
}

void CScManualRet::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RAD_DEST_148, m_radStoHs);
	DDX_Control(pDX, IDC_BTN_SC_MANUAL_RET, m_btnScManualRet);
	DDX_Control(pDX, IDC_GRP_POSITION, m_grpPosition);
	//DDX_Control(pDX, IDC_CBX_DESET_POS, m_cbxDestPos);
	//DDX_Control(pDX, IDC_LBL_DEST_POS, m_lblDestPos);
}


BEGIN_MESSAGE_MAP(CScManualRet, CSkinDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SC_MANUAL_RET, &CScManualRet::OnBnClickedBtnScManualRet)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RAD_DEST_148, IDC_RAD_STO, CScManualRet::OnRadioButton)
END_MESSAGE_MAP()


// CScManualRet 메시지 처리기입니다.


BOOL CScManualRet::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	EN_LANG pEn = (m_pDoc == NULL) ? EN_ENG : m_pDoc->m_enLang;
	InitializeFontManager(this);
	SetFontNation((int)pEn);
	CSkinDialog::SetFont(this->GetFont());
	if( !m_bInitialized )
	{		
		RelocationControls();
		m_bInitialized = TRUE;		
	}

	InitializeResource(pEn);
	return TRUE;  
}

void CScManualRet::OnRadioButton(UINT value)
{
	UpdateData(TRUE);

}

void CScManualRet::RelocationControls()
{
	CRect rc;
	GetClientRect(&rc);

	int x=0, y=0;

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	SIZE sizeLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);

	CRect rc2;
	m_btnScManualRet.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	m_btnScManualRet.MoveWindow(rc2.left, rc2.top, sizeLarge.cx, sizeLarge.cy);

}

void CScManualRet::RenameResource(EN_LANG m_enLang)
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath = _T("");
	strAppPath.Format(_T("%s"),chrFileName);
	CString strExtension = _T(".ini");

	CString strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanualret\\"), _T("dlg_scmanualret"), strExtension);
	CString strValue = CLib::GetIniStringFromPath(strFullPath, _T("dlgname"), (int)m_enLang);
	SetWindowText(strValue);

	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanualret\\"), _T("dlg_scmanualret"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("grpposition"), (int)m_enLang);
	//SetDlgItemText(IDC_GRP_POSITION, strValue);

	strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanualret\\"), _T("dlg_scmanualret"), strExtension);
	strValue = CLib::GetIniStringFromPath(strFullPath, _T("scmanualret"), (int)m_enLang);
	SetDlgItemText(IDC_BTN_SC_MANUAL_RET, strValue);

	//strFullPath = Global.GetConcatPath(strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanualret\\"), _T("dlg_scmanualret"), strExtension);
	//strValue = CLib::GetIniStringFromPath(strFullPath, _T("destpos"), (int)m_enLang);
	//SetDlgItemText(IDC_LBL_DEST_POS, strValue);
		
}

void CScManualRet::InitializeResource(int pnLang)
{	
	RenameResource((EN_LANG)pnLang);
	RedrawImage();
}


void CScManualRet::RedrawImage()
{
	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, MAX_PATH);
	CString strAppPath;
	CString strAppPath2;
	CString strExtension;
	strAppPath.Format(_T("%s"),chrFileName);
	strAppPath2.Format(_T("%s"),chrFileName);

	strAppPath = strAppPath.Left(strAppPath.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\20x20\\");
	strAppPath2 = strAppPath2.Left(strAppPath2.ReverseFind('\\')) + _T("\\rc_resource\\dlg_scmanual\\16x16\\");
	strExtension = _T(".png");

	SIZE szLarge = Global.GetBitmapSize(IDX_BMP_BTN_BASE_LARGE);
	SIZE szSmall = Global.GetBitmapSize(IDX_BMP_BTN_BASE);
	RECT rc;

	m_btnScManualRet.SetBitmaps(Global.GetBitmap(IDX_BMP_BTN_BASE_LARGE), Global.GetRGB(IDX_RGB_MASK), 0, 0);
	m_btnScManualRet.SetIcon(Global.HICONFromPATH(Global.GetConcatPath(strAppPath, _T("read"), strExtension)), NULL, 5, 5);
	m_btnScManualRet.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnScManualRet.MoveWindow(rc.left, rc.top, szLarge.cx, szLarge.cy);	
}


void CScManualRet::OnClose()
{
	m_pDoc->m_pScManualRet = NULL;
	delete this;
}


void CScManualRet::OnBnClickedBtnScManualRet()
{
	m_strWhTyp = m_pSC_DATA->K_WH_TYP;
	m_strPlcNo = m_pSC_DATA->K_PLC_NO;
	m_strScNo = m_pSC_DATA->K_SC_NO;

	CString strLuggNo = _T("");
	CString strHsMcNo = _T("");
	CString strSensor0DataRd = _T("");
	CString strItnLuggNo = m_pSC_DATA->V_ITN_LUGG_FK1; //진행중인 작업번호 미리 담기.
	CString strSensorFkRd = m_pSC_DATA->V_SENSOR_FK_RD;
	CString strForkPosRd = m_pSC_DATA->V_FORKPOS_FK1_RD;
	CString strProductSize = _T("");
	CString strLOG_MSG = _T("");
	CString strDEST_POS = _T("");

	switch(m_radStoHs) //라디오 버튼
	{
		case 0: // 148
			strDEST_POS = _T("148");
			break;
		case 1: // 107
			strDEST_POS = _T("107");
			break;
		case 2: // 재입고
			strDEST_POS = m_strScNo;
			break;
		default :
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("도착지를 선택 해주세요.")));
			return;
	}

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출은 비상 시 사용합니다. 하시겠습니까?\n도착지 : ") + strDEST_POS), MB_YESNO) != IDYES)	
		return;	

	if (AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS에 화물이 있습니까? ")), MB_YESNO) != IDYES)	
		return;	


	//화물감지유무 확인.
	if (RetHsDataSelect(strHsMcNo, strSensor0DataRd) == FALSE)
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS 화물유무감지 조회중 실패")));
		return;
	}

	if (strSensor0DataRd == _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고HS에 화물이 없습니다. 센서를 확인해주세요.")));
		return;
	}

	//SC 화물 없어야됨.
	if (strSensorFkRd == _T("1") || strSensorFkRd == _T("3"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC에 화물이 있어서 강제배출을 할 수 없습니다.")));
		return;
	}

	//Sc 포크 센터여야 함.
	if (strForkPosRd != _T("0"))
	{
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 포크가 센터가 아니어서 강제배출을 할 수 없습니다.")));
		return;
	}

	m_pDoc->BeginTrans_DLG();

	//SC DATA 삭제
	if (ScDataDelete() == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("SC 데이터 삭제중 실패")));
		return;
	}

	//재입고인 경우
	if(m_radStoHs == 2)
	{
		//재입고인 경우에는 작업상태만 'CV 구동중'으로 변경
		if (JobMstUpdate(strItnLuggNo, strProductSize) == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업은 삭제된 작업이므로 재입고를 할 수 없습니다.\n다른 출고대로 진행하세요.")));
			return;
		}
		strLuggNo = strItnLuggNo;
	}
	else //아닌경우
	{
		//해당 작업정보 조회 및 삭제(없으면 삭제 안하고 진행)
		if (JobMstDelete(strItnLuggNo, strProductSize) == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("작업정보 삭제중 실패")));
			return;
		}

		//반자동작업 생성
		if (JobMstInsert(strHsMcNo, strLuggNo, strProductSize, strDEST_POS) == FALSE)
		{
			m_pDoc->RollbackTrans_DLG();
			AfxMessageBox(m_pDoc->GetMsgLangDef(_T("반자동 작업 생성중 실패")));
			return;
		}
	}

	//출고HS DATA 쓰기
	if (CvDataUpdate(strHsMcNo, strLuggNo, strProductSize, strDEST_POS) == FALSE)
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("출고 HS DATA 쓰기중 실패")));
		return;
	}

	strLOG_MSG.Format(_T("SC 강제 배출 BUTTON -> SC NO : %s , 삭제 작업번호 : %s, 생성 작업번호 : %s"), m_pSC_DATA->K_SC_NO, strItnLuggNo, strLuggNo);
	if (!m_pDoc->GetQueryInsertClientLog(_T("CScSkinDlg"), _T(""), _T(""), _T(""), strLOG_MSG))
	{
		m_pDoc->RollbackTrans_DLG();
		AfxMessageBox(m_pDoc->GetMsgLangDef(_T("LOG 입력중 실패")));
		return;
	}

	m_pDoc->CommitTrans_DLG();
	AfxMessageBox(m_pDoc->GetMsgLangDef(_T("강제배출 성공")));
	return;
}

BOOL CScManualRet::CvDataUpdate(CString strMcNo, CString strLuggNo, CString strPulpSensor, CString strDestPos)
{
	CString strSql = _T("");

	//입고 HS 트랙 데이터 삭제
	strSql.Format(_T("  UPDATE CV_DATA																		  \n")
		            _T("   SET LUGG_NO_OD = '%s'															  \n")
		            _T("     , DEST_POS_OD = '%s'															  \n")
		            _T("     , JOB_TYP_OD = (CASE WHEN '%s' IN ('148','107') THEN '6' ELSE '1' END)			  \n")
					_T("     , PULP_SENSOR_OD = '%s'														  \n")
					_T("     , WAIT_SC_RET_JOB_OD = '0'														  \n")
					_T("     , OD_RQ_YN = 'Y'																  \n")
		            _T(" WHERE WH_TYP = '%s'																  \n")
		            _T("   AND MC_NO = '%s'		       "), strLuggNo, strDestPos, strDestPos, strPulpSensor, m_pSC_DATA->K_WH_TYP, strMcNo);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScManualRet::RetHsDataSelect(CString& strHS_MC_NO, CString& strSENSOR0_DATA_RD)
{
	CString strHS_NO = _T("");
	strHS_MC_NO = _T("");
	strSENSOR0_DATA_RD = _T("");

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//해당 SC의 출고 HS 구하기
	strSql.Format(_T("  SELECT SHD.HS_MC_NO, CD.SENSOR0_DATA_RD	")
				  _T("	  FROM SC_HS_DEF SHD					")
				  _T("   INNER JOIN CV_DATA CD				    ")
				  _T("           ON SHD.HS_MC_NO = CD.MC_NO 	")
				  _T("   WHERE SHD.WH_TYP = '%s' 				")
				  _T("     AND SHD.HS_NO = '02' 				")
				  _T("     AND SHD.SC_NO = '%s'				    "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_SC_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//출고 HS 번호, 화물감지 유무
	strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));
	strSENSOR0_DATA_RD = pRsw->GetItem(_T("SENSOR0_DATA_RD"));

	//pRsw->MoveNext();
	delete pRsw;

	if (nRowCnt <= 0)
		return FALSE;

	return TRUE;

}

BOOL CScManualRet::StoHsDataDelete(CString& strLuggNo, CString& strPulpSensorRd, CString& strHS_MC_NO)
{
	CString strHS_NO = _T("");
    strHS_MC_NO = _T("");

	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//해당 SC의 입고 hs 구하기
	strSql.Format(_T("  SELECT HS_NO						")
				  _T("       , HS_MC_NO						") 
				  _T("	  FROM SC_HS_DEF					")
				  _T("   WHERE WH_TYP = '%s'				")
				  _T("     AND SC_NO = '%s'				    ")
				  _T("     AND HS_NO = '01'				    "), m_strWhTyp, m_strScNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst(); 
	strHS_NO = pRsw->GetItem(_T("HS_NO"));
	strHS_MC_NO = pRsw->GetItem(_T("HS_MC_NO"));

	//pRsw->MoveNext();
	delete pRsw;

	//입고 HS의 작업번호, PULP 단수 구하기
	strSql.Format(_T("  SELECT LUGG_NO_RD, PULP_SENSOR_RD   ")
				  _T("	  FROM CV_DATA						")
				  _T("   WHERE WH_TYP = '%s'				")
				  _T("     AND MC_NO = '%s'				    "), m_strWhTyp, strHS_MC_NO);

	pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst(); 
	//작업번호, PULP 단수 변수에 담기
	strLuggNo = pRsw->GetItem(_T("LUGG_NO_RD"));
	strPulpSensorRd = pRsw->GetItem(_T("PULP_SENSOR_RD"));

	if (strLuggNo == _T("0000"))
	{
		return FALSE;
	}

	//pRsw->MoveNext();
	delete pRsw;

	//입고 HS 트랙 데이터 삭제
	strSql.Format(_T("  UPDATE CV_DATA					 \n")
		            _T("   SET LUGG_NO_OD = '0000'		 \n")
		            _T("     , DEST_POS_OD = '000'		 \n")
		            _T("     , JOB_TYP_OD = '0'			 \n")
					_T("     , OD_RQ_YN = 'Y'			 \n")
		            _T(" WHERE WH_TYP = '%s'			 \n")
		            _T("   AND MC_NO = '%s'		       "), m_strWhTyp, strHS_MC_NO);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScManualRet::ScDataDelete()
{
	CString strSql = _T("");
	
	//해당 SC DATA 삭제
	strSql.Format(_T("UPDATE SC_DATA 				\n")
                  _T("	 SET CMD_RQ_ID = 'DELFK1'   \n")
                  _T("	   , CMD_RQ_YN = 'Y'		 \n")
				  _T("    WHERE WH_TYP = '%s'		\n")
		          _T("      AND PLC_NO = '%s'		\n")
		          _T("      AND SC_NO = '%s'		  "), m_pSC_DATA->K_WH_TYP, m_pSC_DATA->K_PLC_NO, m_pSC_DATA->K_SC_NO);
	
	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);
	
	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScManualRet::JobMstDelete(CString pLuggNo, CString& strProductSize)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	strProductSize = _T("");

	//SC의 작업 작업번호가 작업정보에 있는지 확인
	strSql.Format(_T("  SELECT *						")
				  _T("	  FROM JOB_MST					")
				  _T("   WHERE LUGG_NO = '%s'		    "), pLuggNo);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt == 1)
	{
		pRsw->MoveFirst(); 
		strProductSize =  pRsw->GetItem(_T("PRODUCT_SIZE"));

		delete pRsw;

		//해당 작업정보 삭제
		strSql.Format(_T("DELETE FROM JOB_MST 	 \n")
					  _T(" WHERE LUGG_NO = '%s'	 \n"), pLuggNo);

		int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

		if(isSuccess == TRUE)
		{
			//자동 작업인 경우 메시지 보여주기
			if (pLuggNo.Left(1) != _T("9"))
			{
				AfxMessageBox(m_pDoc->GetMsgLangDef(_T("해당 작업을 WMS에서도 삭제해주세요.")));
			}
			return TRUE;
		}
		return FALSE;
	}
	strProductSize = _T("0");
	return TRUE;
}

BOOL CScManualRet::JobMstInsert(CString pHsMcNo, CString& strNextVal, CString& strProductSize, CString strDestPos)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");
	strNextVal = _T("");

	//UpdateData(TRUE);
	//CString strDestPos = _T("");
	//m_cbxDestPos.GetWindowText(strDestPos);

	//해당 SC의 출고 HS 구하기
	strSql.Format(_T("  SELECT RIGHT('0000' + CAST(ISNULL(MAX(CAST(LUGG_NO AS INT)), 8999) + 1 AS VARCHAR), 4) AS nextval FROM JOB_MST WHERE LUGG_NO LIKE '9[0-9][0-9][0-9]' ")); // [LGLS] SQL2008 has no sequences: MANUAL_SEQ -> MAX+1 in 9000 range

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;

	pRsw->MoveFirst();
	//seq에서 작업에 사용할 번호 가져오기
	strNextVal = pRsw->GetItem(_T("nextval"));

	//pRsw->MoveNext();
	delete pRsw;

	//작업대이동 반자동작업 생성
	strSql.Format(_T("INSERT INTO JOB_MST (WH_TYP					\n")
				  _T("					  ,LUGG_NO					\n")
       			  _T("					  ,START_POS 				\n")
       			  _T("					  ,START_LOCATION 			\n")
       			  _T("					  ,DEST_POS 				\n")
       			  _T("					  ,DEST_LOCATION 			\n")
       			  _T("					  ,PRODUCT_SIZE 			\n")
       			  _T("					  ,JOB_TYP 					\n")
       			  _T("					  ,JOB_STATUS 				\n")
       			  _T("					  ,JOB_PRIORITY 			\n")
				  _T("					  ,HS_TRACK_NO 				\n")
				  _T("					  ,SC_NO		 			\n")
       			  _T("					  ,INS_DT 					\n")
       			  _T("					  ,INS_USER_ID)				\n")
				  _T("			   VALUES ('%s'						\n")
				  _T("			          ,'%s'						\n") // 작업번호
				  _T("			          ,'%s'						\n") // 출발지 해당 SC
				  _T("			          ,'00-000-00'				\n") // 출발 로케이션 '00-000-00'
				  _T("			          ,'%s'						\n") // 도착지
				  _T("			          ,'00-000-00'				\n") // 도착 로케이션 '00-000-00'
				  _T("			          ,'0'						\n") // 펄프 단수 '0'
				  _T("			          ,'6'						\n") // 작업구분 '6'(작업대이동)
				  _T("			          ,'11'				    	\n") // 작업상태 '11' (CV구동중)
				  _T("			          ,'100'					\n") // 우선순위 '100'
				  _T("			          ,'%s'						\n") // 출고 HS
				  _T("			          ,'%s'						\n") // SC 번호
				  _T("			          ,") + m_pDoc->SYSDATE + _T("\n")
				  _T("			          ,'%s')					 \n"), m_pSC_DATA->K_WH_TYP, strNextVal, m_pSC_DATA->K_SC_NO, strDestPos, pHsMcNo, m_pSC_DATA->K_SC_NO, m_pDoc->m_strId);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CScManualRet::JobMstUpdate(CString pLUGG_NO, CString& strProductSize)
{
	CString strSql = _T("");
	int nRowCnt = 0;
	CString strMessage = _T("");

	//CV의 작업번호가 작업정보에 있는지 확인
	strSql.Format(_T("  SELECT *						")
				  _T("	  FROM JOB_MST					")
				  _T("   WHERE LUGG_NO = '%s'		    "), pLUGG_NO);

	_RecordsetPtr pRsptr = m_pDoc->GetSelectQryRecordsetPtr_DLG(strSql, nRowCnt, strMessage);
	CRecordSetWrap* pRsw = new CRecordSetWrap(pRsptr);

	if (nRowCnt <= 0)
		return FALSE;
	
	pRsw->MoveFirst(); 
	strProductSize =  pRsw->GetItem(_T("PRODUCT_SIZE"));

	delete pRsw;

	//해당 작업정보 변경
	strSql.Format(_T(" UPDATE JOB_MST 				 \n")
				  _T("    SET JOB_STATUS = '11' 	 \n")
				  _T(" WHERE LUGG_NO = '%s'	 \n"), pLUGG_NO);

	int isSuccess = m_pDoc->ExcuteQueryString_DLG(strSql);

	if(isSuccess == TRUE)
	{
		return TRUE;
	}

	return FALSE;
	
	
}