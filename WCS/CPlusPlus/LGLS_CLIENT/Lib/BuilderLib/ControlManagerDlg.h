#if !defined(AFX_CONTROLMANAGERDLG_H__DD58D9E4_67E2_4EE8_9662_319F44DB7874__INCLUDED_)
#define AFX_CONTROLMANAGERDLG_H__DD58D9E4_67E2_4EE8_9662_319F44DB7874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlManagerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlManagerDlg dialog

#include "BuildManager.h"

class AFX_EXT_CLASS CControlManagerDlg : public CDialog
{
// Construction
public:
	CControlManagerDlg(CBuildManager* pBuilder, CWnd* pParent = NULL);   // standard constructor

protected:
	enum EN_GROUP_CMD { enGroupCreate, enGroupRemove, enGroupMoveUp, enGroupMoveDown, enGroupCmdSize };
	enum EN_CONTROL_CMD { enControlCreate, enControlRemove, enControlCmdSize };
	enum EN_OPERATOR { enOperatorSeq, enOperatorEqual, enOperatorPlus, enOperatorMinus, enOperatorMultiply, enOperatorDivision };

protected:
	CBuildManager* m_pBuilder;
	CDciControlList* m_pSelectCtrls;
	CDciControl* m_pSelectCtrl;

protected:
	void UpdateGroupSpread(int nSheet, BOOL bSaveObject = TRUE, BOOL bInitSpread = FALSE);
	void UpdateControlSpread(int nSheet, BOOL bSaveObject = TRUE, BOOL bInitSpread = FALSE);
	void UpdateCtrlItemSpread(int nSheet, BOOL bSaveObject = TRUE, BOOL bInitSpread = FALSE);

public:
// Dialog Data
	//{{AFX_DATA(CControlManagerDlg)
	enum { IDD = IDD_CONTROL_MANAGER };
	CComboBox	m_cbxOperator;
	CComboBox	m_cbxGroup;
	CComboBox	m_cbxControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlManagerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControlManagerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnCreateControl();
	afx_msg void OnChangeFpspreadGroup(long Col, long Row);
	afx_msg void OnClickFpspreadGroup(long Col, long Row);
	afx_msg void OnChangeFpspreadControl(long Col, long Row);
	afx_msg void OnClickFpspreadControl(long Col, long Row);
	afx_msg void OnSheetChangingFpspreadControl(short OldSheet, short NewSheet, VARIANT FAR* Cancel);
	afx_msg void OnChangeFpspreadItem(long Col, long Row);
	afx_msg void OnClickFpspreadItem(long Col, long Row);
	afx_msg void OnBtnGroupCommand();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnOperation();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLMANAGERDLG_H__DD58D9E4_67E2_4EE8_9662_319F44DB7874__INCLUDED_)
