#pragma once


// CStaticTransparent

class CStaticTransparent : public CStatic
{
	DECLARE_DYNAMIC(CStaticTransparent)
public:
	CStaticTransparent();
	virtual ~CStaticTransparent();

private:
	BOOL m_bDraging;
protected:
	afx_msg LRESULT OnSetText(WPARAM,LPARAM);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_Bmp;
public:
	afx_msg void OnEnterSizeMove();
	afx_msg void OnExitSizeMove();
};


