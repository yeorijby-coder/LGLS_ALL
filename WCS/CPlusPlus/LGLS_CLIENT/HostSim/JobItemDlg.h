#if !defined(AFX_JOBITEMDLG_H__870E272C_CF50_43EB_8892_A682ECDD1E17__INCLUDED_)
#define AFX_JOBITEMDLG_H__870E272C_CF50_43EB_8892_A682ECDD1E17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobItemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJobItemDlg dialog

class CJobItem;
class CJobItemDlg : public CDialog
{
// Construction
public:
	enum EN_KIND { enModify, enComplete, enArrive, enCancel };
	CJobItemDlg(CJobItem* pJobItem, EN_KIND enKind, CWnd* pParent = NULL);   // standard constructor

protected:
	CJobItem* m_pJobItem;
	EN_KIND m_enKind;

public:
	void DrawBitmap();

public:
// Dialog Data
	//{{AFX_DATA(CJobItemDlg)
	enum { IDD = IDD_JOB_ITEM };
	CComboBox	m_cbxJobStatus;
	BUTTON_TYPE	m_btnCancel;
	BUTTON_TYPE	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJobItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBITEMDLG_H__870E272C_CF50_43EB_8892_A682ECDD1E17__INCLUDED_)
