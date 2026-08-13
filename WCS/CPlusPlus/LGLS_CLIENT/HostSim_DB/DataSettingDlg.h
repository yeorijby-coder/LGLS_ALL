#pragma once


// CDataSettingDlg 대화 상자입니다.
//
class CDataSettingDlg : public CDialogEx
{
public:
	CDataSettingDlg(CString strData, CString strCaption, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDataSettingDlg();

// Dialog Data
	enum { IDD = IDD_DATA_SETTING };
	BUTTON_TYPE	m_btnOK;
	BUTTON_TYPE m_btnCancel;

public:
	CString m_strData;
	CString m_strCaption;

// Overrides
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnOK();

// Implementation
protected:
	// Generated message map functions
	DECLARE_MESSAGE_MAP()
};
