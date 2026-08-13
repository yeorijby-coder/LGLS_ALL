#if !defined(AFX_LISTENERSK_H__95CACDBA_CA9F_4859_AAAF_060A3520F2F0__INCLUDED_)
#define AFX_LISTENERSK_H__95CACDBA_CA9F_4859_AAAF_060A3520F2F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenerSk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListenerSk command target

class CEquipment;
class CListenerSk : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CListenerSk() {}
	CListenerSk(CEquipment* pEquipment, int nIndex);
	virtual ~CListenerSk();

protected:
	CEquipment* m_pEquipment;

public:
	int m_nIndex;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenerSk)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenerSk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENERSK_H__95CACDBA_CA9F_4859_AAAF_060A3520F2F0__INCLUDED_)
