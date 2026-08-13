#pragma once

#include "Resource.h"
#include "afxwin.h"

class CEcsDoc;
class CTrackInfo;
class CCvCmdMsg;


// CCvDlg 대화 상자입니다.
//
class CCvDlg : public CDialogEx
{
public:
	CCvDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCvDlg();

// Dialog Data
	enum { IDD = IDD_CV_CTRL1 };
	CButtonST	m_btnStatusStoStation;
	CButtonST	m_btnStatusScStoHS;
	CButtonST	m_btnStatusScRetHS;
	CButtonST	m_btnStatusRtvDepartHS;
	CButtonST	m_btnStatusRtvArriveHS;
	CButtonST	m_btnStatusRetStation;
	CButtonST	m_btnStatusLfDepartHS;
	CButtonST	m_btnStatusLfArriveHS;
	CButtonST	m_btnStatusLgvDepartHS;
	CButtonST	m_btnStatusLgvArriveHS;
	CButtonST	m_btnStatusLgvOutStatus;		// LgvInRequest인데.. 디스펜서 하단으로 사용	
	CButtonST	m_btnStatusLgvInRequest;		// LgvOutRequest인데.. 디스펜서 상단으로 사용
	CButtonST	m_btnStatusCvCheckPos;
	CButtonST	m_btnStatusMDEquipInit;
	CButtonST	m_btnStatusMDPlcInit;
	CButtonST	m_btnSensorProduct1;
	CButtonST	m_btnSensorScInterlock;
	CButtonST	m_btnSensorRtvInterlock;
	CButtonST	m_btnSensorLgvInterlock;
	CButtonST	m_btnSensorLfInterlock;
	CButtonST	m_btnSensorProduct2;
	CButtonST	m_btnSensorProduct3;
	CButtonST	m_btnStatusBcr;
	CComboBox	m_cbxJobType;
	CComboBox	m_cbxDestTrack;
	CComboBox	m_cbxGenCode;
	CComboBox	m_cbxGroup;
	BUTTON_TYPE m_btnWaitTime;
	BUTTON_TYPE	m_btnWrite;
	BUTTON_TYPE	m_btnStatusReport;
	BUTTON_TYPE	m_btnRefresh;
	BUTTON_TYPE	m_btnPaste;
	BUTTON_TYPE	m_btnDelete;
	BUTTON_TYPE	m_btnCut;
	BUTTON_TYPE	m_btnCopy;
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE	m_btnBcrWrite1;
	BUTTON_TYPE	m_btnBcrWrite2;
	BUTTON_TYPE	m_btnFMSReport;
	BUTTON_TYPE	m_btnArrive;
	BUTTON_TYPE m_btnLoadOrSplit;
	BUTTON_TYPE	m_btnLevel;
	BUTTON_TYPE m_btnMakeRetStation;
	BUTTON_TYPE m_btnMakeStoStation;
	BUTTON_TYPE m_btnTrayType;
	BUTTON_TYPE m_btnErrorRequest;
	BOOL		m_bSuspend;
	BOOL		m_bDeadLock;
	int			m_nDestTrack;
	CString		m_strInputInterBcr1;
	CString		m_strInputInterBcr2;
	CString		m_strInternalBcr1;
	CString		m_strInternalBcr2;
	

public:
	CEcsDoc* m_pDoc;
	CTrackInfo* m_pTrack;

//	CFont* m_pFont;

	int  m_nLuggNum;
	int  m_nJobType;
	int  m_nDestPos;
	int  m_nTrayType;
	int  m_nSize;
	BOOL m_bClone;

// Overrides
public:
	virtual BOOL DestroyWindow();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	void SetComboBox(CComboBox& rComboBox, DWORD dwItemData);
	void SetJobType(int nJobType) { SetComboBox(m_cbxJobType, nJobType); }
	void SetDestTrack(int nDestTrack) { SetComboBox(m_cbxDestTrack, nDestTrack); }
	void SetGenCode(BYTE ucGenCode) { SetComboBox(m_cbxGenCode, ucGenCode); }
	void WriteTrack(int nLuggNum, int nJobType, int nDestPos, BYTE ucGenCode, int nSize);
	void WriteWord(int nAddress, WORD wWord);
	BOOL UpdateTrackInfo(BOOL bSaveObject = TRUE);
	void UpdateStationInfo(int nWarehouse, CComboBox& cbxStation);

protected:
	// Generated message map functions
	afx_msg void OnButtonWrite();
	afx_msg void OnButtonCopy();
	afx_msg void OnButtonPaste();
	afx_msg void OnButtonCut();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonStatusReport();
	afx_msg void OnCheckSuspend();
	afx_msg void OnCheckDeadLock();
	afx_msg void OnSelchangeComboDestTrack();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonWritBcr1();
	afx_msg void OnBnClickedButtonWritBcr2();
	afx_msg void OnBnClickedButtonFmsReport();
	afx_msg void OnBnClickedButtonArrive();
	afx_msg void OnBnClickedButtonEquipOrder();
	afx_msg void OnCbnSelchangeComboGroup();
	afx_msg void OnBnClickedButtonWaitTime();
	int m_nWaitTime;
	afx_msg void OnBnClickedButtonLevel();
	afx_msg void OnBnClickedButtonRetStation();
	afx_msg void OnBnClickedButtonStoStation();
	
	afx_msg void OnBnClickedButtonErrRequest();
	BOOL m_bAutoRefresh;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
