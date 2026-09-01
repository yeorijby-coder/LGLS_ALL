#pragma once
class CManyLang
{
public:
	CManyLang(CWnd* pWnd, int pnLang);
	~CManyLang(void);

public:
	int m_nLang;
	CFont* m_pFont;

protected:
	CWnd* m_pWnd;
	void SetFont(CWnd* pWnd = NULL, int pnLang = 1);
};

