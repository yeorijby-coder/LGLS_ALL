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

	int				m_nMonPort;
	BOOL			m_bMonLog;

	BOOL			m_bJob;
	BOOL			m_bEvent;
	BOOL			m_bError;
	BOOL			m_bAlarm;
	BOOL			m_bDebug;
	BOOL			m_bBeep;

	CString			m_strEcsPath;
	CString			m_strJobPath;
	CString			m_strLogPath;
	CString			m_strStcPath;
	CString			m_strInfoPath;
	CString			m_strPlaybackPath;

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
	// INI FILE
	DWORD 			m_dwResendLimit;
	DWORD 			m_dwRecvTimeout;
	DWORD 			m_dwExitTimeout;
	BOOL  			m_bLogTrace;
	// 온습도 정보
	CString			m_strTempMax;
	CString			m_strTempMin;
	CString			m_strHumMax;
	CString			m_strHumMin;
// Update : 2014.06.27 - 입고포트 이동제어
//	CString			m_strStartTime;
// Update : 2014.06.08 - 물류흐름이 원활하지 않은 경우 상온 Aging, 충방전에서 출고된 Tray를 물류 Loop line으로 투입하지 않음.
	// 물류흐름제어 정보
	CString			m_strLoopTrackCount;	// 메인 라인 트랙 수량
	CString			m_strCapacity;			// 가동률(%)
	CString			m_strMode;				// 모드구분 ( 1 : Auto, 0 : Manual )
	CString			m_strAging01;			// 상온 Aging 1호기 출고 포트
	CString			m_strAging02;			// 상온 Aging 2호기 출고 포트
	CString			m_strAging03;			// 상온 Aging 3호기 출고 포트
	CString			m_strAging04;			// 상온 Aging 4호기 출고 포트
	CString			m_strAging05;			// 상온 Aging 5호기 출고 포트
	CString			m_strAging06;			// 상온 Aging 6호기 출고 포트
	CString			m_strCde01;				// 충방전 1호기 출고 포트
	CString			m_strCde02;				// 충방전 2호기 출고 포트
	BIT_FIELD		m_tagManualBits;
	int				m_nTabCount;
//----------------------------------------------------------------------------------------------

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
//	BOOL			LoadXML();
//	BOOL			SaveXML();

public:
	CString			GetJobDirectoryPath();
	CString			GetLogDirectoryPath();
	CString			GetInfoDirectoryPath();

	BOOL			MakeSurePathExists(CString strPath);
	BOOL			MakeSureFileExists(CString strFile);
};


