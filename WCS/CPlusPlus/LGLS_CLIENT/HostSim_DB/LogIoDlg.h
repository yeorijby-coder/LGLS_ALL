#pragma once


// CLogIoDlg 대화 상자입니다.

class CLogIoDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogIoDlg)

public:
	CLogIoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogIoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOG_IO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
