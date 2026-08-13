#pragma once
class CFontManagerDialog
{
public:
	CFontManagerDialog(void);
	CFontManagerDialog(CWnd* pWnd);
	~CFontManagerDialog(void);


public:
	//kdh
	//static const int nFontSizeDefaultWidth_KOR  = 12;
	//static const int nFontSizeDefaultWidth_ENG  = 10;
	//static const int nFontSizeDefaultWidth_HUN  = 10;//7
	//static const int nFontSizeDefaultWidth_CHIN = 8;

	//static const int nFontSizeDefaultHeight_KOR  = 14;
	//static const int nFontSizeDefaultHeight_ENG  = 12;
	//static const int nFontSizeDefaultHeight_HUN  = 12;//14
	//static const int nFontSizeDefaultHeight_CHIN = 16;

	//cksrb
	static const int nFontSizeDefaultWidth_KOR  = 7;
	static const int nFontSizeDefaultWidth_ENG  = 7;
	static const int nFontSizeDefaultWidth_HUN  = 9;//7
	static const int nFontSizeDefaultWidth_CHIN = 7;

	static const int nFontSizeDefaultHeight_KOR  = 14;
	static const int nFontSizeDefaultHeight_ENG  = 12;
	static const int nFontSizeDefaultHeight_HUN  = 12;//14
	static const int nFontSizeDefaultHeight_CHIN = 13;
protected:
	CFont* m_pFont;
	CWnd* m_pParentDlgWnd;

	int qwer;

protected:
	BOOL InitializeFontManager();
	BOOL InitializeFontManager(CWnd* pWnd);
	BOOL SetFontNation(int pnLang = 1, int pnWidth = 0, int pnHeight = 0,  BOOL pbRedraw = TRUE, BOOL pbMessage = TRUE, CWnd* pWnd = NULL);

private:
	int GetLangDefaultWidth(int pnLang);
	int GetLangDefaultHeight(int pnLang);
};

