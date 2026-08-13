#pragma once

#include "PortObject.h"

class CEcsDoc;
class CConfig : public CObject
{
protected:
	DECLARE_DYNCREATE(CConfig)

// constructor & destructor
public:
	CConfig();
	CConfig(CEcsDoc* pDoc);
	virtual ~CConfig();

protected:
	CEcsDoc*		m_pDoc;

public:
	CPortEthernet	m_portHost;
	int				m_nEcsPort;
public:
	int				m_nMonPort;
	BOOL			m_bMonLog;
	CString			m_strPassword;

public:
	BOOL			m_bJob;
	BOOL			m_bEvent;
	BOOL			m_bError;
	BOOL			m_bAlarm;
	BOOL			m_bDebug;
	BOOL			m_bBeep;

public:
	CString			m_strEcsPath;
	CString			m_strJobPath;
	CString			m_strLogPath;
	CString			m_strStcPath;
	CString			m_strInfoPath;

	CString			m_strPlaybackPath;

public:
	COLORREF 		m_clrAutoSto;
	COLORREF 		m_clrAutoRet;
	COLORREF 		m_clrAutoMove;
	COLORREF 		m_clrAutoR2R;
	COLORREF 		m_clrAutoW2W;
	COLORREF 		m_clrSemiSto;
	COLORREF 		m_clrSemiRet;
	COLORREF 		m_clrSemiMove;
	COLORREF 		m_clrSemiR2R;
	COLORREF 		m_clrSemiW2W;
	COLORREF 		m_clrStoST;
	COLORREF 		m_clrRetST;
	COLORREF 		m_clrArvST;
	COLORREF 		m_clrDepart;
	COLORREF 		m_clrArrive;
	COLORREF 		m_clrDisconnect;
	COLORREF 		m_clrSuspend;
	COLORREF 		m_clrManual;
	COLORREF		m_clrAlarm;
	COLORREF		m_clrError;
	COLORREF		m_clrScStoError;
	COLORREF		m_clrScRetError;

public:				// INI FILE
	DWORD 			m_dwResendLimit;
	DWORD 			m_dwRecvTimeout;
	DWORD 			m_dwExitTimeout;
	BOOL  			m_bLogTrace;

	BOOL  			m_bViewLampControler;
	BOOL  			m_bViewProductInfoInputer;

public:				
	int				m_nWeightValue;
	int				m_nFontSize;
	int				m_nCompleteCnt;

public:				
	int				m_nBank;
	int				m_nBay;
	int				m_nLevel;

	int				m_nRetBufferCount;
	BOOL			m_bNoCheckScRetHs;

	BOOL			m_bNotCheckRetHsSc1;
	BOOL			m_bNotCheckRetHsSc2;
	BOOL			m_bNotCheckRetHsSc3;
	BOOL			m_bNotCheckRetHsSc4;
	BOOL			m_bNotCheckRetHsSc5;
	BOOL			m_bNotCheckRetHsSc6;

	BOOL			m_bNotBackupNetwork;

public:
	CStringArray	m_strDisableRacks;


public:
	int				m_nJobCnt;
	int				m_nScCnt;
	int				m_nLogicGroupCnt;
	int				m_nStoSeperate;
	int				m_nRetSeperate;



public:		
	void			Load(CReportCtrl* pReportCtrl);

public:		
	void	 		InitializeConfig();
	void 			InitializePath();
	void 			InitializeColor();
//	void 			InitializePort();
		
public:		
	void 			LoadConfig1();
	void 			SaveConfig1();
	void	 		LoadConfig2();
	void 			SaveConfig2();
	void 			LoadConfig3();
	void	 		SaveConfig3();
	void 			LoadConfig4();
	void	 		SaveConfig4();
	void 			LoadConfig5();
	void	 		SaveConfig5();
	void			LoadConfig6();
//	void			SaveConfig6();
//	BOOL			LoadXML();
//	BOOL			SaveXML();

public:
	CString			GetJobDirectoryPath();
	CString			GetLogDirectoryPath();
	CString			GetInfoDirectoryPath();

	BOOL			MakeSurePathExists(CString strPath);
	BOOL			MakeSureFileExists(CString strFile);
};


