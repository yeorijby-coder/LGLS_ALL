// Config.h: interface for the CConfig class.
//
#pragma once


class CEcsDoc;


// CConfig
//
class CConfig : public CObject
{
protected:
	DECLARE_DYNCREATE(CConfig)

public:
	CConfig();
	CConfig(CEcsDoc* pDoc);
	virtual ~CConfig();

public:
	CEcsDoc*		m_pDoc;

public:
	CString m_strWH_WH_TYP;

public:
	CString m_strDATABASE_SERVER;
	CString m_strDATABASE_USERID;
	CString m_strDATABASE_USERPASSWORD;
	CString m_strDATABASE_DATABASE;
	CString m_strDATABASE_DB;
	CString m_strDATABASE_DRIVER;
	CString m_strDB_TYPE;
	
public: 
	int m_nUSER_JOB_COUNT;
	int m_nUSER_LAST_LANG;
	int m_nUSER_LAST_TAB_INDEX;
	COLORREF m_clrUSER_COLOR_STO; 
	COLORREF m_clrUSER_COLOR_RET;
	COLORREF m_clrUSER_COLOR_MOVE;
	COLORREF m_clrUSER_COLOR_ATA;
	COLORREF m_clrUSER_COLOR_WTW;
	COLORREF m_clrUSER_COLOR_RTR;
	COLORREF m_clrUSER_COLOR_STN_STO;
	COLORREF m_clrUSER_COLOR_STN_RET;
	COLORREF m_clrUSER_COLOR_HS_STO;
	COLORREF m_clrUSER_COLOR_HS_RET;
	COLORREF m_clrUSER_COLOR_SUSPEND;
	COLORREF m_clrUSER_COLOR_ERROR;
	COLORREF m_clrUSER_COLOR_MANUAL;
	COLORREF m_clrUSER_COLOR_DISCONNECT;
	COLORREF m_clrUSER_COLOR_WC_PASS;
	COLORREF m_clrUSER_COLOR_CV_SEARCH;
	COLORREF m_clrUSER_COLOR_STO_SUSPEND;
	COLORREF m_clrUSER_COLOR_RET_SUSPEND;
	COLORREF m_clrUSER_COLOR_ALL_SUSPEND;
	COLORREF m_clrUSER_COLOR_RAIL_ERROR;
	COLORREF m_clrUSER_COLOR_SC_INVK;
	COLORREF m_clrSPREAD_COLOR;

//	COLORREF m_clrMES_ERROR_CD;
	COLORREF m_clrUSER_COLOR_READY_OFF;


	// Ãß°¡ »ö±ò

public:		
	void 			InitializeConfigWH();
	void 			InitializeConfigDATABASE();
	void 			InitializeConfigUSER();
		
public:		
	void 			SaveConfigWH();
	void 			SaveConfigUSER();
	void 			SaveConfigDATABASE();
	void			SaveConfigAll();

	void 			LoadConfigWH();
	void			LoadConfigDBTYPE();
	void			SaveConfigMONITOR();
	void			SaveConfigLANG();
	void 			LoadConfigDATABASE();
	void 			LoadConfigUSER();
	void			LoadConfigKiosk();
	CString			GetIpAddress();
	void			LoadConfigSound();


public:
	BOOL			MakeSurePathExists(CString strPath);
	BOOL			MakeSureFileExists(CString strFile);

public:
	void GetPC_IP();
};
