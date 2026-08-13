// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_)
#define AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------
#define IDX_BMP_CAPTION_LEFT				0		//
#define IDX_BMP_CAPTION_CENTER				1		//
#define IDX_BMP_CAPTION_RIGHT				2		//
#define IDX_BMP_FRAME_LEFT					3		//
#define IDX_BMP_FRAME_LEFT_BOTTOM			4		//
#define IDX_BMP_FRAME_BOTTOM				5		//
#define IDX_BMP_FRAME_RIGHT_BOTTOM			6		//
#define IDX_BMP_FRAME_RIGHT					7		//
#define IDX_BMP_BOX_MINIMIZE				8		//
#define IDX_BMP_BOX_MAXMIZE					9		//
#define IDX_BMP_BOX_RESTORE					10		//
#define IDX_BMP_BOX_EXIT					11		//
#define IDX_BMP_BOX_MINIMIZE_ON				12		//
#define IDX_BMP_BOX_MAXMIZE_ON				13		//
#define IDX_BMP_BOX_RESTORE_ON				14		//
#define IDX_BMP_BOX_EXIT_ON					15		//
#define IDX_BMP_BOX_MINIMIZE_OVER			16		//
#define IDX_BMP_BOX_MAXMIZE_OVER			17		//
#define IDX_BMP_BOX_RESTORE_OVER			18		//
#define IDX_BMP_BOX_EXIT_OVER				19		//
#define IDX_BMP_BTN_BASE					20		//	
#define IDX_BMP_BTN_BASE_LARGE				21		//
#define IDX_BMP_BTN_BASE_WIDELARGE			22		//
#define IDX_BMP_MAX							23		//

//--------------------------------------------------------
#define IDX_ICON_SKIN						0		//
#define IDX_ICON_SKIN_INACTIVE				1		//
#define IDX_ICON_OK							2		//
#define IDX_ICON_CANCEL						3		//
#define IDX_ICON_WCS						4		//	JBY 추가 - 20210218
#define IDX_ICON_MAX						5		//

//--------------------------------------------------------
#define ICON16								0		//
#define ICON24								1		//
#define ICON32								2		//

//--------------------------------------------------------
#define IDX_FONT_SMALL						0		//
#define IDX_FONT_MEDIUM						1		//
#define IDX_FONT_LARGE						2		//
#define IDX_FONT_MAX						3		//

//--------------------------------------------------------
#define IDX_RGB_MASK						0		//
#define IDX_RGB_BACKGROUND					1		//
#define IDX_RGB_FONT						2		//
#define IDX_RGB_FONT_CAPTION				3		//
#define IDX_RGB_FONT_CAPTION_INACTIVE		4		//
#define IDX_RGB_MAX							5		//


//
#define KEYDOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)		((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//===========================================================================
// function pointer definitions for SetLayeredWindowAttributes in User32.dll
//===========================================================================
typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

typedef struct tagBITMAP_INFO
{
	HBITMAP hBitmap;
	BITMAP	bitmap;
} BITMAP_INFO;


//-------------------------------------------------------enum match
//이미지 관련
#define  IDX_ICON_CV_ON _T(".\\ecs_resource\\ico\\on.ico")
#define  IDX_ICON_CV_OFF _T(".\\ecs_resource\\ico\\off.ico")
#define  IDX_ICON_WRITE _T(".\\ecs_resource\\ico\\write.ico")
#define  IDX_ICON_COPY _T(".\\ecs_resource\\ico\\copy.ico")
#define  IDX_ICON_CUT _T(".\\ecs_resource\\ico\\cut.ico")
#define  IDX_ICON_DELETE _T(".\\ecs_resource\\ico\\delete.ico")
#define  IDX_ICON_PASTE _T(".\\ecs_resource\\ico\\paste.ico")
#define  IDX_ICON_ACTIVE _T(".\\ecs_resource\\ico\\active.ico")
#define  IDX_ICON_CALL_TO_HOME _T(".\\ecs_resource\\ico\\calltohome.ico")
#define  IDX_ICON_COMPLETE _T(".\\ecs_resource\\ico\\complete.ico")
#define  IDX_ICON_EMGSTOP _T(".\\ecs_resource\\ico\\emgstop.ico")
#define  IDX_ICON_ERROR_RESET _T(".\\ecs_resource\\ico\\errreset.ico")
#define  IDX_ICON_JOB_DELETE _T(".\\ecs_resource\\ico\\jobdelete.ico")
#define  IDX_ICON_MANUAL _T(".\\ecs_resource\\ico\\manual.ico")
#define  IDX_ICON_RESET _T(".\\ecs_resource\\ico\\reset.ico")
#define  IDX_ICON_STOP _T(".\\ecs_resource\\ico\\stop.ico")
#define  IDX_ICON_SAVE _T(".\\ecs_resource\\ico\\save.ico")
#define  IDX_ICON_LEFT _T(".\\ecs_resource\\ico\\left.ico")
#define  IDX_ICON_RIGHT _T(".\\ecs_resource\\ico\\right.ico")
#define  IDX_ICON_SEARCH _T(".\\ecs_resource\\ico\\search.ico")	
#define  IDX_ICON_SEMIJOB _T(".\\ecs_resource\\ico\\semijob.ico")
#define  IDX_ICON_MENU_EQP_SUSPEND _T(".\\ecs_resource\\ico\\eqp_uspend.ico")
#define  IDX_ICON_RETIREVE _T(".\\ecs_resource\\ico\\retireve.ico")
#define  IDX_ICON_CLOSE _T(".\\ecs_resource\\ico\\exit.ico")
#define  IDX_ICON_READ _T(".\\ecs_resource\\ico\\read.ico")
#define  IDX_ICON_REPORT _T(".\\ecs_resource\\ico\\report.ico")
#define  IDX_ICON_REQUESTRESET _T(".\\ecs_resource\\ico\\requestreset.ico")
#define  IDX_ICON_LED_ON _T(".\\LedOn.ico")
#define  IDX_ICON_LED_OFF _T(".\\LedOff.ico")









class CGlobal
{
public:
	CGlobal();
	virtual ~CGlobal();


public:


	//이미지 관련
	enum EN_ICON_IDX { ICO_CV_ON = 0
		,ICO_CV_OFF 
		,ICO_WRITE
		,ICO_COPY
		,ICO_CUT
		,ICO_DELETE
		,ICO_PASTE
		,ICO_ACTIVE
		,ICO_CALLTOHOME
		,ICO_COMPLETE
		,ICO_EMGSTOP
		,ICO_ERRORRESET
		,ICO_JOBDELETE
		,ICO_MANUAL
		,ICO_RESET
		,ICO_STOP
		,ICO_SAVE
		,ICO_LEFT
		,ICO_RIGHT
		,ICO_SEARCH
		,ICO_SEMIJOB
		,ICO_MENU_EQP_SUSPEND
		,ICO_RETIREVE
		,ICO_CLOSE
		,ICO_READ
		,ICO_REPORT
		,ICO_REQUEST_RESET
		,ICO_LED_ON
		,ICO_LED_OFF
		,END_IDX }; //DEFINE MATCH
	HICON hIconBank[END_IDX]; ////이미지 관련 아이콘 카운트 상수로 박을것

	void InitializeIcon();
	HICON GetIcon(EN_ICON_IDX enIconIdx);

	// Attributes
public:
	CFont			*m_pFont[IDX_FONT_MAX];
	BITMAP_INFO		m_infoBitmap[IDX_BMP_MAX];
	HICON			m_hIcon[IDX_ICON_MAX][3];
	COLORREF		m_cf[IDX_RGB_MAX];

	// Operations
public:
	lpfn			SetLayeredWindowAttributes;
	
	void			DeleteBitmaps();	
	void			LoadBitmaps();	
	HBITMAP			GetBitmap(UINT nBitmapID);
	SIZE			GetBitmapSize(UINT nBitmapID);
	CString			GetBitmapFileName(UINT nBitmapID);

	CString			GetModulePath();

	void			DeleteIcons();
	void			LoadIcons();
	CString			GetIconFileName(UINT nIconID);
	HICON			GetIcon(UINT nIconID, UINT nType);

	void			SetRGB(UINT nRGBID, COLORREF cf);
	COLORREF		GetRGB(UINT nRGBID);

	void			DeleteFonts();
	CFont	*		SetFont(UINT nFontID, CString sFaceName, UINT nFontSize, BOOL bBold=FALSE);
	CFont	*		GetFont(UINT nFontID);

	CSize			FormatText(CString& sText, CDC* pDC, int nWidth);

	COLORREF		Darker(COLORREF crBase, float fFactor);
	COLORREF		Lighter(COLORREF crBase, float fFactor);

public:
	CString GetConcatPath(CString pstrAppPath, CString pstrFileName, CString pstrExtension);
	HICON HICONFromPATH(CString pstrPath);

public:
	BOOL RegistryFont(CString pstrFullPath, BOOL bMessage = FALSE);
	CString GetFontName( int pnLang );
	HBITMAP BITMAPFromPATH( CString pstrFullPath );
};

#endif // !defined(AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_)
