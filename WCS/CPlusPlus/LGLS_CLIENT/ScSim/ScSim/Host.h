
#ifndef __ECSSOCK_H__
#define __ECSSOCK_H__

#include "EcsDef.h"
//#include <EcsEnv.h>
#include "AsyncSocketEx.h"

class CEcsDoc;
class CJobItem;

class CListenSk : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CListenSk);

// Attributes
public:
	CEcsDoc *m_pDoc;
	BOOL m_bConnected;
	BOOL m_bInkJetConnected;

// Operations
public:
	CListenSk();
	CListenSk(CEcsDoc *pDoc);
//	CListenSk(LPCTSTR lpszName, BOOL bWriteLog);
	virtual ~CListenSk();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSk)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

class CHostSv : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CHostSv);

// Construction
public:
	CHostSv(CEcsDoc* pDoc);
//	CHostSv(LPCTSTR lpszName, BOOL bWriteLog);
	virtual ~CHostSv();

// Attributes
public:
	enum EN_COMPLETE_GBN {	enCompleteAnyJob,					//  0
							enCompleteStore,					//  1
							enCompleteRetrieve,					//  2
							enCompleteHsToHs,					//  3
							enCompleteRackToRack,				//  4
							enCompleteCallToHome,				//  5
							enCompleteSiteToSite,				//  6
							enCompleteAisleToAisle,				//  7
							enCompleteAisleToAisleSto,			//  8
							enCompletePicking,					//  9
							enCompleteManual };					// 10

	CEcsDoc *m_pDoc;
	int m_nHostSeqNum;
//	BOOL m_bArriveReport;
//	CStringArray m_saStation, m_saBcr;

// Operations
public:
	BOOL IsConnect() { return (m_enCommStatus == enStatusConnectOK); }
	void UpdateCommSv(char Status);
	void TokenRxFrame(char *pFrame);
	void Parsing(CString pFrame);
	void Answer(BYTE ucMsgType, int nLuggNum, int nReasonCode);
	int  GetSeqNum();
	void WriteData();
	CString GetStartPos(int nStartPos);
	CString	GetDestPos(int nDestPos);

	void Answer(TCHAR chType, int nLuggNum, int nJobType, int nResultCode);
	void RedirectLocDirective(CJobItem& rJobItem);
	void JobOrderDirective(CJobItem& rJobItem);
	void ResendOrderDirective(CJobItem& rJobItem);

	BOOL SendMsg(char* pTxBuff, int nSndMsgLen);
	void Arrived(int nLuggNum, BOOL bManual = FALSE);
	BOOL AgingStoArrived(int nLuggNum, CString strStation, BOOL bManual = FALSE);
	BOOL ResendArrivedReport();
	BOOL Complete();
	BOOL ResendCompleteReport();
	BOOL Complete(int nLuggNum, int nScJobType = enCompleteAnyJob, int nStation = 0, BOOL bDelete = TRUE, BOOL bManual = FALSE);
	void Error(int nDevKind, int nRelease, int nDevNo, int nErrKind = 0, WORD wErrCode = 0, CString strErrMsg = "", int nLuggNum = 0, CString strLocation = "", BOOL bManual = FALSE);
	BOOL ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn, CString strErrMsg);
	void Status(int nRegularCycle=0);
	BOOL BarcodeData(CString strStation, CString strBcr, BOOL bManual = FALSE);
	BOOL SendThsInfo(BOOL bManual = FALSE);
	BOOL StatusReport(int nEquipGbn, CString& strStationNo, int nStatusGbn, BOOL bManual = FALSE );
	BOOL SendHeartBit(BOOL bManual = FALSE);
	// Update : 2014.07.09 - 동간 TRAY 대기 수량 보고
	BOOL SendEmptyTrayWaitCount();
	// Update : 2014.08 - DCIR TRAY 대기 버퍼 추가 (대기 버퍼의 TRAY 수량 전송)
	BOOL SendDcirTrayWaitCount();
	void JobRequest(int nStationNo, int JobKind, BOOL bManual = FALSE);



// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostSv)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CHostSv)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

//#define	SC_JOB_TYPE_ANY					 0		 
//#define SC_JOB_TYPE_STORE					 1		 
//#define SC_JOB_TYPE_RETRIEVE				 2		 
//#define SC_JOB_TYPE_HS_TO_HS				 3		 
//#define SC_JOB_TYPE_RACK_TO_RACK			 4		 
//#define SC_JOB_TYPE_CALL_TO_HOME			 5		 
//#define SC_JOB_TYPE_SITE_TO_SITE			 6			 
//#define SC_JOB_TYPE_AISLE_TO_AISLE		 7		 
//#define SC_JOB_TYPE_AISLE_TO_AISLE_STO	 8		 
//#define SC_JOB_TYPE_PICKING				 9		 
//#define SC_JOB_TYPE_MANUAL				10			 

class CHostCl : public CAsyncSocketEx
{
	DECLARE_DYNAMIC(CHostCl);

public:
	CEcsDoc *m_pDoc;

public:
	CHostCl(CEcsDoc* pDoc);
//	CHostCl(LPCTSTR lpszName, BOOL bWriteLog);
	virtual ~CHostCl();

public:
	enum EN_COMPLETE_GBN {	enCompleteAnyJob,					//  0
							enCompleteStore,					//  1
							enCompleteRetrieve,					//  2
							enCompleteHsToHs,					//  3
							enCompleteRackToRack,				//  4
							enCompleteCallToHome,				//  5
							enCompleteSiteToSite,				//  6
							enCompleteAisleToAisle,				//  7
							enCompleteAisleToAisleSto,			//  8
							enCompletePicking,					//  9
							enCompleteManual};					// 10
	
	BOOL IsConnect() { return (m_enCommStatus == enStatusConnectOK); }
	void Parsing(char *pFrame);
	void Arrived(int nLuggNum, BOOL bManual = FALSE);
	BOOL Complete();
	BOOL CompleteAuto();
	BOOL Complete(int nLuggNum, int nScJobType = enCompleteAnyJob, int nStation = 0, BOOL bDelete = TRUE, BOOL bManual = FALSE);
	void Error(int nDevKind, int nDevNo, int nErrKind=0, WORD wErrCode=0, int nLuggNum=0, CString strLocation="", BOOL bManual = FALSE);
	BOOL ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn);
	void Status(int nRegularCycle=0);
	BOOL BarcodeData(CString strStation, CString strBarcodeData, BOOL bManual = FALSE);
	BOOL ThsData(CString strStation, CString strThsData, BOOL bManual = FALSE);
//	void JobRequest(int nStation, int nLuggNum);
	BOOL StatusReport(int nEquipGbn, CString& strStationNo, int nStatusGbn, BOOL bManual = FALSE );
	void JobRequest(int nStationNo, int JobKind, BOOL bManual = FALSE);

	CString GetStringReason(int nReasonCode);

	BOOL SendMsg(char* pTxBuff, int nSndMsgLen);
	void UpdateCommCl(char Status);

	int GetClientSeqNum();

public:
//	int nHogi; 
	int nStackerStatus;
	int nLuggNum;
//	int nSto101;
//	int nSto102;
//	int nRet201;
//	int nRet202;
	int nStoRetMode;

	int m_nClientSeqNum;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHostCl)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CHostCl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};


#endif



