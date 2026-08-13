// Splash.h : header file
//

#pragma once


// CSplashWnd
//
class CSplashWnd : public CWnd
{
public:
	CSplashWnd();
	virtual ~CSplashWnd();

// Variables
public:
	CBitmap m_bitmap;
	static BOOL c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// Overrides
public:
	virtual void PostNcDestroy();

// Implementation
protected:
	BOOL Create(CWnd* pParentWnd = NULL);
	void HideSplashScreen();

public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);

protected:
	// Generated message map functions
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};


