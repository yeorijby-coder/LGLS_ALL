// BuilderDoc.h : interface of the CBuilderDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUILDERDOC_H__90700B5B_74E7_4F9C_B22B_A37534BC790A__INCLUDED_)
#define AFX_BUILDERDOC_H__90700B5B_74E7_4F9C_B22B_A37534BC790A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BuildManager.h"

class CBuilderDoc : public CDocument
{
protected: // create from serialization only
	CBuilderDoc();
	DECLARE_DYNCREATE(CBuilderDoc)

// Attributes
public:
	CBuildManager m_buildManager;

public:
	BOOL		m_bMoveFlag;
	CPoint		m_PrevPoint;
	BOOL		m_bStoReReport;
	CTime		m_tChecktime;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuilderDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBuilderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBuilderDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDERDOC_H__90700B5B_74E7_4F9C_B22B_A37534BC790A__INCLUDED_)
