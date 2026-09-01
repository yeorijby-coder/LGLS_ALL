
#pragma once
#include "ssdllmfc.h"

//#pragma comment ( lib, "EcsLibD.lib" )

//class CXmlDom;
class CSpreadCell
{
public:
	CSpreadCell();
	CSpreadCell(int, int, UINT nLong=0);

public:
	int	m_nCol;
	int	m_nRow;
	UINT m_nLong;
};

typedef struct tagAUXPROPERTY
{
//public:
	CString strUnitID;
	BYTE* pStartAddr;
	BYTE* pSheetAddr;
	int	  nMaxByteLen;
	tagAUXPROPERTY() : strUnitID(""), pStartAddr(NULL), pSheetAddr(0), nMaxByteLen(0) {}
	tagAUXPROPERTY(CString str) : strUnitID(str), pStartAddr(NULL), pSheetAddr(0), nMaxByteLen(0) {}

} AUXPROPERTY, *PAUXPROPERTY;

class CSheet : public CObject
{
public:
	CSheet();
	CSheet(CString str, HFONT hFontHeader=NULL, HFONT hFontBody=NULL);
	~CSheet();

public:
	void AddColHead(int nNumber);
	void AddColHead(CString strText);
	void AddColHead(CString strText, int nColWidth, CString strColID);

	void RemoveAllColHead();

public:
	CString		m_strName;

	CObList		m_listHeader;

	HFONT		m_hFontHeader;
	HFONT		m_hFontBody;
};

typedef	CTypedPtrList<CPtrList, CSpreadCell*>	CSpreadCellList;

class CSpreadSheet
{
public:
	CSpreadSheet();
	CSpreadSheet(CWnd* pWnd);
	virtual ~CSpreadSheet();

public:
	TSpread m_Spread;

	// [LGLS] 헤더 클릭 정렬(토글) FarPoint SortEx
	long m_nHdrSortCol = -1;
	BOOL m_bHdrSortAsc = TRUE;
	void SortByHeaderColumn(long nCol){
		long mc = m_Spread.GetMaxCols();
		long mr = m_Spread.GetMaxRows();
		if(mr < 2 || nCol < 1 || nCol > mc) return;
		if(m_nHdrSortCol == nCol) m_bHdrSortAsc = !m_bHdrSortAsc;
		else { m_nHdrSortCol = nCol; m_bHdrSortAsc = TRUE; }
		SS_SORTKEY sk; sk.Reference = (SS_COORD)nCol;
		sk.wOrder = (WORD)(m_bHdrSortAsc ? SS_SORT_ASCENDING : SS_SORT_DESCENDING);
		m_Spread.SortEx(1, 1, mc, mr, SS_SORT_ROW, &sk, 1);
	}
	CWnd*   m_pParentWnd;
	CRect   m_rectWnd;
	enum	{IDC_SPREAD = 1000};

public:
	SS_COORD	m_nCurrentRow;
	short		m_nCurrentSheet;

	WORD		m_wRowHeaderDisplay;
	WORD		m_wColHeaderDisplay;

	WORD		m_wMode;
	WORD		m_wGridType;

	int			m_nSorting;
	int			m_nUserResizeColRow;
	BOOL		m_bAllowColMove;
	BOOL		m_bAutoResizeCol;
	int			m_nResizeIndent;

	COLORREF	m_colorBaseBack;
	COLORREF	m_colorBaseFore;

	CRect		m_rectMargin;

	CObList		m_listSheet;
	CSpreadCellList	m_listSelCell;


	typedef BOOL (*SearchAuxProperty)(PAUXPROPERTY);

public:
	void RemoveActiveCells();
	void RemoveAllColHead();
	void RemoveTailColHead();

public:
	BOOL Create(UINT nID = IDC_SPREAD, int nIndex=0);
//	BOOL CreateAuxIO(CString strFile, UINT nID, SearchAuxProperty pSearchAuxProperty);
	BOOL CreateZone(BYTE cHomePos, int nMaxBay, int nMaxLevel,  int nLevelL, int nLevelR, int nSheet, LONG lfHeight);
	BOOL CreateZone(BYTE cHomePos, int nMaxBay, int nMaxLevel,  int nLevelL, int nLevelR, LONG lfHeight, CWnd* pWnd, UINT nID = IDC_SPREAD, BOOL bCreate=true);

	void AddSheet(CString str, HFONT hFontHeader=NULL, HFONT hFontBody=NULL);
	void AddSheet(CString str, CWnd* pParent, HFONT hFontHeader=NULL, HFONT hFontBody=NULL);

	void AddColHead(int nNumber);
	void AddColHead(CString strText);
	void AddColHead(CString strText, int nColWidth, CString strColID=_T(""));

	void AddColHead(int nIndex, int nNumber);
	void AddColHead(int nIndex, CString strText);
	void AddColHead(int nIndex, CString strText, int nColWidth, CString strColID=_T(""));
	
	CSheet* FindSheet(CString strSheet);
	CSheet* FindSheet(int nIndex);

	void AutoResizeColumn();
	void ResizeAllColumn();
	void SetFontEntireSheet();

	void ResizeWindow();
	void ResizeWindow(CRect rect);
	void PrepareLoadSpread();
	void FinishLoadSpread();

	void SetMargin(CRect rect);
	void SetMargin(CWnd* pWndChild);

	void SetColWidth(SS_COORD Col, DOUBLE nWidth);
	void SetRowHeight();
	void SetRowHeight(SS_COORD Row, DOUBLE nHeigth);
	void SetAdjustRowHeight();

	void ShowHeaderRow(BOOL bShow);
	void ShowHeaderCol(BOOL bShow);

	void SetUserResizeRow(int nResize);  // SS_RESIZE_ON, SS_RESIZE_OFF
	void SetUserResizeCol(int nResize);

	void SetScrollBarHorz(BOOL bBool, BOOL bExtMode=0);
	void SetScrollBarVert(BOOL bBool, BOOL bExtMode=0);

	void SetScrollBarHeight(int nHeight);
	void SetScrollBarWidth(int nWidth);

	BOOL	SetUserCellTag(SS_COORD Col, int nCellTag);
	BOOL	SetUserCellTag(SS_COORD Col, LPCTSTR CellTag);
	BOOL	SetUserCellTagHEX(SS_COORD Col, UINT nCellTag);
	BOOL	SetUserCellTag(SS_COORD Col, SS_COORD Row, LPCTSTR CellTag);

	CString GetUserCellTag(SS_COORD Col);
	CString	GetUserCellTag(SS_COORD Col, SS_COORD Row);
	INT		GetUserCellTagINT(SS_COORD Col);
	UINT	GetUserCellTagHEX(SS_COORD Col);
	INT		GetUserCellTagINT(SS_COORD Col, SS_COORD Row);
	UINT	GetUserCellTagHEX(SS_COORD Col, SS_COORD Row);

	CString GetSheetName();


	void	SetSheetUserData(long nUserData);
	LONG	GetSheetUserData();
	BOOL	SetRowUserData(long nUserData);
	LONG	GetRowUserData(SS_COORD nRow);

public:
	int		GetCurrentRow() {return m_nCurrentRow;};
	int		GetColWidth(SS_COORD Col);
	int		GetRowHeight(SS_COORD Row);
	int		GetCellType(SS_COORD nCol, SS_COORD nRow);
	BOOL	GetCellLock(SS_COORD nCol);
	CPoint	GetActiveCell();
	CSpreadCellList* GetActiveCells();
	int		GetActiveSheet();
	CSheet* GetCurrentSheet();

public:
	BOOL SetCurrentRow();
	void DeleteRow(SS_COORD nRow);

	void SetData(SS_COORD nCol, CString strData);
	void SetData(SS_COORD nCol, int nData);
	void SetData(SS_COORD nCol, CString strData, COLORREF colorFore);
	void SetData(SS_COORD nCol, int nData, COLORREF colorFore);
	void SetData(SS_COORD nCol, CString strData, COLORREF colorBack, COLORREF colorFore);
	void SetData(SS_COORD nCol, int nData, COLORREF colorBack, COLORREF colorFore);
	void SetData(SS_COORD nCol, SS_COORD nRow, CString strData);
	void SetData(SS_COORD nCol, SS_COORD nRow, CString strData, COLORREF colorFore, COLORREF colorBack);

	void SetDataHex(SS_COORD nCol, int nData, COLORREF colorFore);

	void SetFormatData(SS_COORD nCol, CString strFormat);
	void SetColorBack(SS_COORD nCol, COLORREF colorBack);
	void SetColorFore(SS_COORD nColck, COLORREF colorFore);
	void SetColor(SS_COORD nCol, COLORREF colorBack, COLORREF colorFore);
	void SetColorBack(SS_COORD nCol, SS_COORD nRow, COLORREF colorBack);
	void SetColorFore(SS_COORD nCol, SS_COORD nRow, COLORREF colorFore);
	void SetColor(SS_COORD nCol, SS_COORD nRow, COLORREF colorBack, COLORREF colorFore);

	COLORREF GetColorBack(SS_COORD nCol, SS_COORD nRow);
	COLORREF GetColorFore(SS_COORD nCol, SS_COORD nRow);

	

	void SetAllRowColor(COLORREF colorBack, COLORREF colorFore);
	void SetAllRowColor(SS_COORD nRow, COLORREF colorBack, COLORREF colorFore);
	void SetAllColColor(SS_COORD nCol, COLORREF colorBack, COLORREF colorFore);
	void SetSheetColor(COLORREF colorBack, COLORREF colorFore);
	void SetGrayAreaColor(COLORREF colorBaseBackColor);

	void SetCellLock(SS_COORD nCol, BOOL bLock, COLORREF colorBack=SPREAD_COLOR_NONE, COLORREF colorFore=SPREAD_COLOR_NONE);
	void SetCellLock(SS_COORD nCol, SS_COORD nRow, BOOL bLock, COLORREF colorBack, COLORREF colorFore);

	BOOL SetCellSpan(SS_COORD nCol);
	BOOL SetCellSpan(SS_COORD nCol, SS_COORD lNumCols);
	BOOL SetCellSpan(SS_COORD nCol, SS_COORD nRow, SS_COORD lNumCols, SS_COORD lNumRows);
	void RemoveCellSpan(SS_COORD nCol, SS_COORD nRow);

	void SetColMerge(SS_COORD nCol, short nColMerge=SS_MERGE_ALWAYS);
	void SetRowMerge(SS_COORD nRow, short nRowMerge=SS_MERGE_ALWAYS);
	void SetHeaderColCheck(SS_COORD nCol);
	void SetHeaderColUnCheck(SS_COORD nCol);
	void SetHeaderColReverse(SS_COORD nCol);
	void SetHeaderCellCheck(SS_COORD nCol, BOOL bCheck);




public:
	void SetEdit(SS_COORD nCol, LPCTSTR pText);
	void SetEdit(SS_COORD nCol, int nText);
	void SetEdit(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText);
	void SetEdit(SS_COORD nCol, LPCTSTR pText, long nStyle, short nLen, short nChrSet=SS_CHRSET_CHR, short nChrCase=SS_CASE_NOCASE, BOOL nRowHeight=true);
	void SetEdit(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, long nStyle, short nLen, short nChrSet=SS_CHRSET_CHR, short nChrCase=SS_CASE_NOCASE, BOOL nRowHeight=true);

	void SetStatic(SS_COORD nCol, LPCTSTR pText, long nStyle = SS_TEXT_WORDWRAP | SS_TEXT_RIGHT | SS_TEXT_VCENTER | SS_TEXT_SHADOW);
	void SetStatic(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText=_T(""), long nStyle = SS_TEXT_RIGHT | SS_TEXT_VCENTER | SS_TEXT_SHADOW);
	void SetCheckBox(SS_COORD nCol, LPCTSTR pText, BOOL bCheck=0);
	void SetCheckBox(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText =_T(""), long lStyle=BS_CENTER, BOOL bCheck=0);

	void SetButton(SS_COORD nCol, LPCTSTR pText);
	void SetButton(SS_COORD nCol, LPCTSTR pText, COLORREF color, COLORREF colorText=SPREAD_COLOR_NONE);
	void SetButton(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, LPSUPERBTNCOLOR pColor=NULL);

	void SetComboBox(SS_COORD nCol, LPCTSTR pText, int nIndex = -1);
	void SetComboBox(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, int nIndex = -1);
	void SetPicture(SS_COORD nCol, long nStyle, UINT nIconId);
	void SetPicture(SS_COORD nCol, SS_COORD nRow, long nStyle, UINT nIconId);
	void SetPicture(SS_COORD nCol, long nStyle, LPCTSTR pID);
	void SetPicture(SS_COORD nCol, SS_COORD nRow, long nStyle, LPCTSTR pID);

	void SetButtonData(SS_COORD nCol, LPCTSTR lpText, COLORREF color, COLORREF colorText);
	void SetButtonColor(SS_COORD nCol, COLORREF color, COLORREF colorText);
	void SetButtonColor(SS_COORD nCol, SS_COORD nRow, COLORREF color, COLORREF colorText);
	void GetButtonColor(SS_COORD nCol, COLORREF* pcolorBack, COLORREF* pcolorText);
	void GetButtonColor(SS_COORD nCol, SS_COORD nRow, COLORREF* pcolorBack, COLORREF* pcolorText);

	CString GetComboBoxText(SS_COORD nCol, SS_COORD nRow);
	int		GetComboBoxCount(SS_COORD nCol, SS_COORD nRow);
	int		GetComboBoxSel(SS_COORD nCol, SS_COORD nRow);
	int		SetComboCurSel(SS_COORD nCol, WPARAM wParam);
	int		SetComboCurSel(SS_COORD nCol, SS_COORD nRow, WPARAM wParam);

	void	SetTextTip(WORD wTipStatus, LONG nDelay=10, LONG lfHeight=0);
	void	SetTextTip(WORD wTipStatus, LONG nDelay, LPLOGFONT lpLogFont, COLORREF clrBack, COLORREF clrFore);

public:
	CString GetButtonTXT(SS_COORD nCol, SS_COORD nRow);
	CString GetValueTXT(SS_COORD nCol, SS_COORD nRow);
	LONG	GetValueINT(SS_COORD nCol, SS_COORD nRow);
	void	SetValueINT(SS_COORD nCol, SS_COORD nRow, LONG nValue);

	LONG	GetValueCheckINT(SS_COORD nCol, SS_COORD nRow);
	BOOL	GetPioWordCell(CSpreadCell* pCell);


public:
	BOOL IsCellType(SS_COORD nCol, SS_COORD nRow, BYTE nType);
	BOOL IsCellChecked(SS_COORD nCol, SS_COORD nRow);

	BOOL IsCellActived();
	BOOL IsColsSelected(SS_COORD nCol);


protected:
	void InsertData(SS_COORD nCol, SS_COORD nRow, CString strData);
	void InsertData(SS_COORD nCol, SS_COORD nRow, int nData);
	void InsertData(SS_COORD nCol, SS_COORD nRow, CString strData, COLORREF colorBack, COLORREF colorFore);
};

class CSpreadHeaderCol : public CObject
{
public:
	CSpreadHeaderCol(CString strText);
	CSpreadHeaderCol(CString strText, int nWidth);
	CSpreadHeaderCol(CString strText, int nWidth, CString strColID);

	~CSpreadHeaderCol();

public:
	CString		m_strHeadColText;

	BOOL		m_bExclusiveWidth;
	int			m_nHeadColWidth;

	CString		m_strHeadColID;
};

