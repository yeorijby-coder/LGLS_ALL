
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
	CEcsDoc *m_pDoc;
	int m_nHostSeqNum;

// Operations
public:
	BOOL IsConnect() { return (m_enCommStatus == enStatusConnectOK); }
	void UpdateCommSv(char Status);
	void Parsing(char *pFrame);
	void Answer(BYTE ucMsgType, int nLuggNum, int nReasonCode);
	int  GetSeqNum();
	CString GetStartPos(int nStartPos);
	CString	GetDestPos(int nDestPos);


	void Answer(TCHAR chType, int nLuggNum, int nJobType, int nResultCode);
	void RedirectLocDirective(CJobItem& rJobItem);
	void RedirectWeightDirective(CJobItem& rJobItem);
	void JobOrderDirective(CJobItem& rJobItem);
	void ResendOrderDirective(CJobItem& rJobItem);



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
	void Arrived(int nLuggNum, int nStation, BOOL bManual = FALSE);
	BOOL Complete();
	BOOL CompleteAuto();
	BOOL Complete(int nLuggNum, int nScJobType = enCompleteAnyJob, int nStation = 0, BOOL bDelete = TRUE, BOOL bManual = FALSE);
	void Error(int nDevKind, int nDevNo, int nErrKind=0, WORD wErrCode=0, int nLuggNum=0, CString strLocation="", CString strWeight="", BOOL bManual = FALSE);
	BOOL ErrorScReport(CJobItem* pJobItem, EN_ERROR_GBN enErrorGbn);
	void Status(int nRegularCycle=0);
	BOOL BarcodeData(CString strStation, CString strBarcodeData, BOOL bManual = FALSE);
//	void JobRequest(int nStation, int nLuggNum);
	BOOL StatusReport(int nEquipGbn, CString& strStationNo, int nStatusGbn, BOOL bManual = FALSE );
	void JobRequest(int nStationNo, int JobKind, BOOL bManual = FALSE);
	void RetJobRequest(int nStationNo, int nLuggNum, BOOL bManual = FALSE);
	void StoJobRequest(int nStationNo, int nJobQty, CString strPalletID, BOOL bManual = FALSE);

//	CString GetStringReason(int nReasonCode);

	BOOL SendMsg(TCHAR* pTxBuff, int nSndMsgLen);
	void UpdateCommCl(char Status);

	int GetClientSeqNum();

public:
	int JobOrder(int nJobType, int n1stStn, int n2ndStn = 0, BOOL bManual = FALSE, LPCTSTR lpszLocation = NULL, CStringArray* pScList = NULL);
	int AlterLocation(BOOL bManual = FALSE);

	BOOL CalcurateLocation(BOOL bSto = TRUE, CStringArray* pScList = NULL);

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

	int m_nCurBank;
	int m_nCurBay;
	int m_nCurLevel;

	TCHAR m_JobOrderMsg[1024];
	TCHAR m_AlterLocMsg[1024];
	int m_nJobOrderSize;
	int m_nAlterLocSize;

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



