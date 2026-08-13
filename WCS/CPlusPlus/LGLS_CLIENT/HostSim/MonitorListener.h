#if !defined(AFX_MONITORLISTENER_H__BC5A4FBE_2073_439A_A2E7_F3079D2EE3D6__INCLUDED_)
#define AFX_MONITORLISTENER_H__BC5A4FBE_2073_439A_A2E7_F3079D2EE3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonitorListener.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMonitorListener command target

class CEcsDoc;
class CMonitorServer;
class CMonitorListener : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CMonitorListener(CEcsDoc* pDoc);
	virtual ~CMonitorListener();

protected:
	CEcsDoc* m_pDoc;
	CObList m_pSocketList;
	CCriticalSection m_csSyncSend;

public:
	CMonitorServer* GetClient(CString& strIP);
	void RemoveClient(CMonitorServer* pMonitorSv);
	void SendToClients(CString& strSend);
	void Load(CReportCtrl* pReportCtrl);
	int GetClientCount() { return m_pSocketList.GetCount();	}

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonitorListener)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMonitorListener)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONITORLISTENER_H__BC5A4FBE_2073_439A_A2E7_F3079D2EE3D6__INCLUDED_)
