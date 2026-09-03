

#include "stdafx.h"

#include "SpreadSheet.h"
//#include "..\\Parser\\Xml\\XmlDom.h"
//#include "XmlDom.h"

CSpreadSheet::CSpreadSheet()
{ 
	m_nSorting = SS_USERCOLACTION_SORT;
	m_bAllowColMove = 0;
	
	m_nUserResizeColRow = SS_USERRESIZE_COL|SS_USERRESIZE_ROW;
	m_bAutoResizeCol = 0;
	m_nResizeIndent = 0;
	m_wMode = SS_OPMODE_NORMAL;
	m_wGridType = 0;

	m_wRowHeaderDisplay = SS_HEADERDISPLAY_NUMBERS;
	m_wColHeaderDisplay = SS_HEADERDISPLAY_BLANK;
	m_colorBaseBack = SPREAD_COLOR_NONE;
	m_colorBaseFore = SPREAD_COLOR_NONE;

	m_pParentWnd = NULL;
	m_rectWnd = CRect(0, 0, 100, 100);

	m_nCurrentRow = 0;
	m_nCurrentSheet = 0;

	m_rectMargin = CRect(0, 0, 0, 0);
}

CSpreadSheet::CSpreadSheet(CWnd* pWnd)
{ 
	m_nSorting = SS_USERCOLACTION_SORT;
	m_bAllowColMove = 0;

	m_nUserResizeColRow = SS_USERRESIZE_COL|SS_USERRESIZE_ROW;
	m_bAutoResizeCol = 0;
	m_nResizeIndent = 0;
	m_wMode = SS_OPMODE_NORMAL;
	m_wGridType = 0;

	m_wRowHeaderDisplay = SS_HEADERDISPLAY_NUMBERS;
	m_wColHeaderDisplay = SS_HEADERDISPLAY_BLANK;
	m_colorBaseBack = SPREAD_COLOR_NONE;
	m_colorBaseFore = SPREAD_COLOR_NONE;

	m_pParentWnd = pWnd;
	m_rectWnd = CRect(0, 0, 100, 100);

	m_nCurrentRow = 0;
	m_nCurrentSheet = 0;

	m_rectMargin = CRect(0, 0, 0, 0);
}

CSpreadSheet::~CSpreadSheet()
{
	for (POSITION pos = m_listSheet.GetHeadPosition(); pos != NULL;)
	{
		delete m_listSheet.GetNext(pos);
	}
	m_listSheet.RemoveAll();

	RemoveActiveCells();
}

void CSpreadSheet::RemoveActiveCells()
{
	for (POSITION pos = m_listSelCell.GetHeadPosition(); pos != NULL;)
	{
		CSpreadCell* pCell = m_listSelCell.GetNext(pos);
		if ( pCell )
			delete pCell;
	}
	m_listSelCell.RemoveAll();
}

/////////////////////////////////////////////
//
void CSpreadSheet::AddSheet(CString str, HFONT hFontHeader, HFONT hFontBody)
{
	CSheet* pSheet = new CSheet(str, hFontHeader, hFontBody);
	m_listSheet.AddTail(pSheet);
}

void CSpreadSheet::AddSheet(CString str, CWnd* pParent, HFONT hFontHeader, HFONT hFontBody)
{
	m_pParentWnd = pParent;

	CSheet* pSheet = new CSheet(str, hFontHeader, hFontBody);
	m_listSheet.AddTail(pSheet);
}

void CSpreadSheet::AddColHead(int nIndex, int nNumber)
{
	CSheet* pSheet = FindSheet(nIndex);
	if ( pSheet )
	{
		CString str;
		str.Format(_T("%d"), nNumber);

		pSheet->AddColHead(str);
	}
}

void CSpreadSheet::AddColHead(int nIndex, CString strText)
{
	CSheet* pSheet = FindSheet(nIndex);
	if ( pSheet )
	{
		pSheet->AddColHead(strText);
	}
}

void CSpreadSheet::AddColHead(int nIndex, CString strText, int nColWidth, CString strColID)
{
	CSheet* pSheet = FindSheet(nIndex);
	if ( pSheet )
	{
		pSheet->AddColHead(strText, nColWidth, strColID);
	}
}




void CSpreadSheet::AddColHead(int nNumber)
{
	CSheet* pSheet = FindSheet(0);
	if ( pSheet )
	{
		CString str;
		str.Format(_T("%d"), nNumber);

		pSheet->AddColHead(str);
	}
}

void CSpreadSheet::AddColHead(CString strText)
{
	CSheet* pSheet = FindSheet(0);
	if ( pSheet )
	{
		pSheet->AddColHead(strText);
	}
}

void CSpreadSheet::AddColHead(CString strText, int nColWidth, CString strColID)
{
	CSheet* pSheet = FindSheet(0);
	if ( pSheet )
	{
		pSheet->AddColHead(strText, nColWidth, strColID);
	}
}


void CSpreadSheet::RemoveAllColHead()
{
	CSheet* pSheet = FindSheet(0);
	if ( pSheet )
	{
		pSheet->RemoveAllColHead();
	}
}

void CSpreadSheet::RemoveTailColHead()
{
	CSheet* pSheet = FindSheet(0);
	if ( pSheet )
	{
		POSITION pos = pSheet->m_listHeader.GetTailPosition();
		if (pos != NULL)
			delete pSheet->m_listHeader.GetPrev(pos);

		pSheet->m_listHeader.RemoveTail();
	}
}

CSheet* CSpreadSheet::FindSheet(CString strSheet)
{
	for (POSITION pos = m_listSheet.GetHeadPosition(); pos != NULL;)
	{
		CSheet* pSheet = (CSheet*)m_listSheet.GetNext(pos);
		if ( pSheet == NULL )	continue;
		if ( pSheet->m_strName == strSheet )
			return pSheet;
	}

	return NULL;
}

CSheet* CSpreadSheet::FindSheet(int nIndex)
{
	POSITION pos = m_listSheet.FindIndex(nIndex);
	if ( pos )
		return (CSheet*)m_listSheet.GetNext(pos);

	return NULL;
}

////////////////////////////////////////////
//
void CSpreadSheet::ResizeWindow()
{
	if ( !m_rectMargin.IsRectNull() )
	{
		m_pParentWnd->GetClientRect(&m_rectWnd);
		m_rectWnd.DeflateRect(m_rectMargin);
		m_Spread.MoveWindow(m_rectWnd);
	}
}

void CSpreadSheet::ResizeWindow(CRect rect)
{
	m_rectWnd = rect;
	m_Spread.MoveWindow(m_rectWnd);
}

BOOL CSpreadSheet::Create(UINT nID, int nIndex)
{
	if ( !::IsWindow(m_Spread.m_hWnd) )
	{
		if ( !m_Spread.Create(WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
							  m_rectWnd, m_pParentWnd, nID) )
			return false;
	}

	m_Spread.BringWindowToTop();
	
	CSheet* pSheet = FindSheet(nIndex);
	if ( pSheet == NULL )
		return false;

	m_Spread.SetSheetCount(m_listSheet.GetCount());

	int i = 0;
	for (POSITION pos = m_listSheet.GetHeadPosition(); pos != NULL; i++)
	{
		CSheet* pSheet = (CSheet*)m_listSheet.GetNext(pos);
		if ( pSheet == NULL )	continue;

		m_Spread.SetSheet(i+1);
		m_Spread.SetSheetName(i+1, pSheet->m_strName);

		m_Spread.SetUserColAction(m_nSorting);
		m_Spread.SetUserResize(m_nUserResizeColRow);

		m_Spread.SetMaxCols(pSheet->m_listHeader.GetCount());
		m_Spread.SetMaxRows(0);

		//m_Spread.SetBool(SSB_AUTOSIZE, true);
		m_Spread.SetGridType(m_wGridType);

		m_Spread.SetColHeaderRows(1);
		m_Spread.SetRowHeaderCols(1);
		//m_Spread.SetRowHeadersAutoTextIndex(0);//0, 1, 2... 순번이 뿌려지는 열의 번호를 지정
		m_Spread.SetRowHeaderDisplay(m_wRowHeaderDisplay);
		m_Spread.SetColHeaderDisplay(m_wColHeaderDisplay);
		
		m_Spread.SetOperationMode(m_wMode);
		m_Spread.SetBool(SSB_ALLOWCOLMOVE, m_bAllowColMove);
		m_Spread.SetGrayAreaColor(m_colorBaseBack, m_colorBaseFore);
		m_Spread.SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

		SS_CELLTYPE sCellType;
		m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER);
		
		int nCol = 1;
		for (POSITION pos = pSheet->m_listHeader.GetHeadPosition(); pos != NULL; nCol++)
		{
			CSpreadHeaderCol* pHeader = (CSpreadHeaderCol*)pSheet->m_listHeader.GetNext(pos);

			m_Spread.SetValue(nCol, SS_HEADER, pHeader->m_strHeadColText);
			if ( pHeader->m_nHeadColWidth )
				m_Spread.SetColWidth(nCol, pHeader->m_nHeadColWidth);

			m_Spread.SetCellType(nCol, SS_ALLROWS, &sCellType);
			if ( !pHeader->m_strHeadColID.IsEmpty() )
				m_Spread.SetColID(nCol, pHeader->m_strHeadColID);
			//SSGetColFromID
		}

		if ( pSheet->m_hFontHeader )
		{
			SS_COORD nCols = m_Spread.GetMaxCols( );
			SS_COORD nRows = m_Spread.GetColHeaderRows( );
			m_Spread.SetFontRange(SS_HEADER, SS_HEADER, nCols, nRows, pSheet->m_hFontHeader, false);
			// 마지막 값이 true이면 입력 폰트 Delete
		}
	}

	m_Spread.SetSheet(nIndex+1);

	if ( !m_rectMargin.IsRectNull() )
	{
		CRect rect;
		::GetClientRect(m_pParentWnd->m_hWnd, &rect);
		rect.DeflateRect(m_rectMargin);
		ResizeWindow(rect);
	}

	return true;
}

BOOL CSpreadSheet::CreateZone(BYTE cHomePos, int nMaxBay, int nMaxLevel,  int nLevelL, int nLevelR, int nSheet, LONG lfHeight)
{
	m_Spread.SetSheet(nSheet);
	return CreateZone(cHomePos, nMaxBay, nMaxLevel, nLevelL, nLevelR, lfHeight, NULL, 0, false);
}

BOOL CSpreadSheet::CreateZone(BYTE cHomePos, int nMaxBay, int nMaxLevel, int nLevelL, int nLevelR, LONG lfHeight, CWnd* pWnd, UINT nID, BOOL bCreate)
{
	m_bAutoResizeCol = false;

	if ( bCreate )
	{
		m_pParentWnd = pWnd;
		if ( !m_Spread.Create(WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
							  m_rectWnd, m_pParentWnd, nID) )
			return false;
	}

	m_Spread.BringWindowToTop();

	//m_Spread.SetBool(SSB_AUTOSIZE, true);
	m_Spread.SetGridType(SS_GRID_HORIZONTAL|SS_GRID_VERTICAL|SS_GRID_SOLID);
	m_Spread.SetUserColAction(SS_USERCOLACTION_DEFAULT);
	m_Spread.SetUserResize(SS_USERRESIZE_COL|SS_USERRESIZE_ROW);
	
	m_Spread.SetOperationMode(SS_OPMODE_NORMAL);
	m_Spread.SetBool(SSB_ALLOWMULTIBLOCKS, true); // These functions have an effect only when the sheet is in normal operation mode. 
	m_Spread.SetBool(SSB_RETAINSELBLOCK, true);

	m_Spread.SetBool(SSB_ALLOWCOLMOVE, false);
	m_Spread.SetGrayAreaColor(WHITE, SPREAD_COLOR_NONE);
	m_Spread.SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);

	int nMaxCol = nMaxBay;
	int nMaxRow = nMaxLevel*2+1;
	m_Spread.SetMaxCols(nMaxCol);
	m_Spread.SetMaxRows(nMaxRow);
	m_Spread.SetColHeaderRows(2);
	m_Spread.SetRowHeaderCols(3);
	m_Spread.SetRowHeaderDisplay(SS_HEADERDISPLAY_BLANK);
	m_Spread.SetColHeaderDisplay(SS_HEADERDISPLAY_BLANK);

	HFONT hFont = CreateFont(30,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));
	HFONT hFont2 = CreateFont(15,0,0,0,700,0,0,0,0,0,0,0,0,_T("System"));

	SS_CELLTYPE sCellType = {0};
	SS_CELLTYPE sCellType2 = {0};
	m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER|SSS_TEXTORIENT_UP);
	m_Spread.SetTypeStaticText(&sCellType2, SS_TEXT_CENTER | SS_TEXT_VCENTER);

	SetTextTip(SS_TT_STATUS_FLOATING, 10 , lfHeight);
	m_Spread.SetCellNoteIndicator(SS_CELLNOTEINDICATOR_SHOWANDFIREEVENT);


	///////////////// Header Row
	m_Spread.SetValue(1, SS_HEADER+0, _T("Bay"));
	m_Spread.SetFont(1, SS_HEADER+0, hFont, FALSE);
	m_Spread.AddCellSpan(1, SS_HEADER+0, nMaxCol, 1);

	for ( int nCol=1; nCol<=nMaxCol; nCol++ )
	{
		CString strTemp;
		strTemp.Format(_T("%d"), cHomePos == 'L' ? nCol : nMaxCol-nCol+1);
		
		m_Spread.SetValue(nCol, SS_HEADER+1, strTemp);
		m_Spread.SetFont(nCol, SS_HEADER+1, hFont2, FALSE);
		m_Spread.SetColor(nCol, SS_HEADER+1, SPREAD_COLOR_NONE, RED);
		m_Spread.SetColWidth(nCol, 5);
	}

	///////////////// Header Column

	m_Spread.SetCellType(SS_HEADER+0, 1, &sCellType);
	m_Spread.SetColor(SS_HEADER+0, 1, ::GetSysColor(COLOR_BTNFACE), SPREAD_COLOR_NONE); 
	m_Spread.SetValue(SS_HEADER+0, 1, _T("Level"));
	m_Spread.SetFont(SS_HEADER+0, 1, hFont, FALSE);
	m_Spread.AddCellSpan(SS_HEADER+0, 1, 1, nMaxRow);

	
	m_Spread.SetCellType(SS_HEADER+1, 1, &sCellType);
	m_Spread.SetColor(SS_HEADER+1, 1, ::GetSysColor(COLOR_BTNFACE), SPREAD_COLOR_NONE); 
	m_Spread.SetValue(SS_HEADER+1, 1, cHomePos == 'L' ? _T("L-Bank") : _T("R-Bank"));
	m_Spread.SetFont(SS_HEADER+1, 1, hFont, FALSE);
	m_Spread.AddCellSpan(SS_HEADER+1, 1, 1, nMaxLevel);

	int nOffRow = nMaxLevel+2;
	m_Spread.SetCellType(SS_HEADER+1, nOffRow, &sCellType);
	m_Spread.SetColor(SS_HEADER+1, nOffRow, ::GetSysColor(COLOR_BTNFACE), SPREAD_COLOR_NONE); 
	m_Spread.SetValue(SS_HEADER+1, nOffRow, cHomePos == 'L' ? _T("R-Bank") : _T("L-Bank"));
	m_Spread.SetFont(SS_HEADER+1, nOffRow, hFont, FALSE);
	m_Spread.AddCellSpan(SS_HEADER+1, nOffRow, 1, nMaxLevel);

	int nTemp = nMaxLevel;
	int nRow = 1;
	for (nRow = 1; nRow<=nMaxLevel; nRow++ )
	{
		CString strTemp;
		strTemp.Format(_T("%d"), nTemp--);
		
		m_Spread.SetValue(SS_HEADER+2, nRow, strTemp);
		m_Spread.SetFont(SS_HEADER+2, nRow, hFont2, FALSE);
		m_Spread.SetColor(SS_HEADER+2, nRow, SPREAD_COLOR_NONE, RED);
		m_Spread.SetRowHeight(nRow, 22);
	}
	
	nTemp = 1;
	for ( nRow = nMaxLevel+2; nRow<=nMaxRow; nRow++ )
	{
		CString strTemp;
		strTemp.Format(_T("%d"), nTemp++);
		
		m_Spread.SetValue(SS_HEADER+2, nRow, strTemp);
		m_Spread.SetFont(SS_HEADER+2, nRow, hFont2, FALSE);
		m_Spread.SetColor(SS_HEADER+2, nRow, SPREAD_COLOR_NONE, RED);
		m_Spread.SetRowHeight(nRow, 22);
	}
	m_Spread.SetColWidth(SS_HEADER+2, 3);
	
	m_Spread.SetCellType(1, nMaxLevel+1, &sCellType2);
	m_Spread.SetValue(1, nMaxLevel+1, cHomePos == 'L' ? _T("HP") : _T("OP"));
	m_Spread.SetFont(1, nMaxLevel+1, hFont, FALSE);
	m_Spread.SetColor(1, nMaxLevel+1, SPREAD_COLOR_NONE, BLUE);
	
	m_Spread.SetCellType(nMaxCol, nMaxLevel+1, &sCellType2);
	m_Spread.SetValue(nMaxCol, nMaxLevel+1, cHomePos == 'L' ? _T("OP") : _T("HP"));
	m_Spread.SetFont(nMaxCol, nMaxLevel+1, hFont, FALSE);
	m_Spread.SetColor(nMaxCol, nMaxLevel+1, SPREAD_COLOR_NONE, RED);
	m_Spread.AddCellSpan(2, nMaxLevel+1, nMaxCol-2, 1);
	
	DeleteObject(hFont);
	DeleteObject(hFont2);

	if ( !m_rectMargin.IsRectNull() )
	{
		CRect rect;
		::GetClientRect(m_pParentWnd->m_hWnd, &rect);
		rect.DeflateRect(m_rectMargin);
		ResizeWindow(rect);
	}

	m_Spread.SetSheet(1);

	return true;
}
/*
BOOL CSpreadSheet::CreateAuxIO(CString strFile, UINT nID, SearchAuxProperty pSearchAuxProperty)
{
	if ( !m_Spread.Create(WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
						  m_rectWnd, m_pParentWnd, nID) )
		return false;

	m_Spread.BringWindowToTop();

	CXmlDom dom;
	try
	{
		dom.InitializeXmlDom();
		dom.LoadXmlFile(strFile);

		IXMLDOMNodePtr pPosAux = dom.moveRootXPath("/AuxIO");
		IXMLDOMNodeListPtr pSheetList = pPosAux->selectNodes("./Sheet");
		if ( pSheetList==NULL)
			return FALSE;
		
		m_Spread.SetSheetCount(pSheetList->length);
		
		int i = 0;
		// Sheet
		IXMLDOMNodePtr pSheetNode = pSheetList->nextNode();
		for (; pSheetNode; pSheetNode = pSheetList->nextNode() )
		{
			dom.SetCurrentNode(pSheetNode);
			
			CString strSheetName = dom.getAttributeSTR("name", true);
			int nMaxRow = dom.getNodeINT("MaxBodyRow", true, true);
			int nMaxCol = dom.getNodeINT("MaxBodyCol", true, true);
			int nOffSet = nMaxCol/8;
			
			dom.moveNodeXPath("./Header");
			int nHeadRows = dom.getNodeINT("MaxHeadRow", true, true);
			int nHeadCols = dom.getNodeINT("MaxHeadCol", true, true);
			
			AUXPROPERTY auxProperty;
			auxProperty.strUnitID = strSheetName;
			pSearchAuxProperty(&auxProperty);
			
			BYTE* pStartAddr = auxProperty.pStartAddr;
			int nMaxByteLen = auxProperty.nMaxByteLen;
			
			m_Spread.SetSheet(++i);
			m_Spread.SetSheetName(i, strSheetName);
			
			//m_Spread.SetBool(SSB_AUTOSIZE, true);
			m_Spread.SetGridType(m_wGridType);
			m_Spread.SetUserColAction(m_nSorting);
			m_Spread.SetUserResize(m_nUserResizeColRow);
			
			m_Spread.SetOperationMode(m_wMode);
			m_Spread.SetBool(SSB_ALLOWCOLMOVE, m_bAllowColMove);
			m_Spread.SetGrayAreaColor(WHITE, SPREAD_COLOR_NONE);
			m_Spread.SetBackColorStyle(SS_BACKCOLORSTYLE_UNDERGRID);
			
			m_Spread.SetMaxCols(nMaxCol);
			m_Spread.SetMaxRows(nMaxRow);
			m_Spread.SetColHeaderRows(nHeadRows);
			m_Spread.SetRowHeaderCols(nHeadCols);
			m_Spread.SetRowHeaderDisplay(m_wRowHeaderDisplay);
			m_Spread.SetColHeaderDisplay(m_wColHeaderDisplay);
			
			dom.SetCurrentNode(pSheetNode);
			IXMLDOMNodePtr pPos = dom.moveNodeXPath("./Header/HeaderRow/Row", false);
			for ( ; pPos; pPos = dom.moveSiblingNext() )
			{
				CString strName = dom.GetNodeName(pPos);
				if ( strName == "Row" )
				{
					int nRow = dom.getAttributeINT("row", true);
					
					if ( dom.moveNodeXPath("./Font", false) )
					{
						int nHeight = dom.getAttributeINT("Height", true);
						int nBold = dom.getAttributeINT("Bold", true);
						CString strName = dom.getAttributeSTR("Name", true);
						HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
						
						m_Spread.SetFontRange(1,SS_HEADER+nRow, nMaxCol, SS_HEADER+nRow, hFont, FALSE);
						DeleteObject(hFont);
						dom.moveParent();
					}
					
					if ( dom.SelectElements("./Col", FALSE) )
					{
						while ( dom.getSelectNextNode() )
						{
							int nCol = dom.getAttributeINT("col", true);
							int nWidth = dom.getAttributeINT("width");
							CString strText = dom.getNodeSTR("Text", true);
							m_Spread.SetValue(nCol, SS_HEADER+nRow, _T(strText));
							
							if ( dom.moveNodeXPath("./Font", false) )
							{
								int nHeight = dom.getAttributeINT("Height", true);
								int nBold = dom.getAttributeINT("Bold", true);
								CString strName = dom.getAttributeSTR("Name", true);
								HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
								
								m_Spread.SetFont(nCol,SS_HEADER+nRow, hFont, FALSE);
								DeleteObject(hFont);
								dom.moveParent();
							}
							
							//DOUBLE dRealColWidth = 0;
							//if ( m_Spread.GetMaxTextColWidth(nCol, &dRealColWidth) )
							{
								if ( nWidth )
									m_Spread.SetColWidth(nCol, nWidth);
							}
						}
					}
					
					dom.SetCurrentNode(pPos);
				}
				
				else if ( strName == "Span" )
				{
					int nCol = dom.getAttributeINT("col", true);
					int nRow = dom.getAttributeINT("row", true);
					int nNumCol = dom.getAttributeINT("numcol", true);
					int nNumRow = dom.getAttributeINT("numrow", true);
					
					m_Spread.AddCellSpan(nCol, SS_HEADER+nRow, nNumCol, nNumRow);
					dom.SetCurrentNode(pPos);
				}
				
				else if ( strName == "Merge" )
				{
					int nRow = dom.getAttributeINT("row", true);
					dom.SetCurrentNode(pPos);
				}
			}
			
			dom.SetCurrentNode(pSheetNode);
			pPos = dom.moveNodeXPath("./Header/HeaderCol/Col", false);
			for ( ; pPos; pPos = dom.moveSiblingNext() )
			{
				CString strName = dom.GetNodeName(pPos);
				if ( strName == "Col" )
				{
					int nCol = dom.getAttributeINT("col", true);
					int nWidth = dom.getAttributeINT("width");
					
					if ( dom.SelectElements("./Row", FALSE) )
					{
						while ( dom.getSelectNextNode() )
						{
							int nRow = dom.getAttributeINT("row", true);
							CString strText = dom.getNodeSTR("Text", true);
							int nTextOrient = dom.getNodeHEX("TextOrient", false);
							
							if ( nTextOrient )
							{
								SS_CELLTYPE sCellType = {0};
								m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER|nTextOrient);
								m_Spread.SetCellType(SS_HEADER+nCol, SS_HEADER+nRow, &sCellType);
								m_Spread.SetColor(SS_HEADER+nCol, SS_HEADER+nRow, ::GetSysColor(COLOR_BTNFACE), SPREAD_COLOR_NONE); 
							}
							m_Spread.SetValue(SS_HEADER+nCol, SS_HEADER+nRow, _T(strText));
							
							if ( dom.moveNodeXPath("./Font", false) )
							{
								int nHeight = dom.getAttributeINT("Height", true);
								int nBold = dom.getAttributeINT("Bold", true);
								CString strName = dom.getAttributeSTR("Name", true);
								HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
								
								m_Spread.SetFont(SS_HEADER+nCol,SS_HEADER+nRow, hFont, FALSE);
								DeleteObject(hFont);
								dom.moveParent();
							}
						}
					}
					dom.SetCurrentNode(pPos);
					
					if ( dom.moveNodeXPath("./Font", false) )
					{
						int nHeight = dom.getAttributeINT("Height", true);
						int nBold = dom.getAttributeINT("Bold", true);
						CString strName = dom.getAttributeSTR("Name", true);
						HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
						
						m_Spread.SetFontRange(SS_HEADER+nCol, SS_HEADER+nHeadRows, SS_HEADER+nCol, SS_HEADER+nHeadRows+nMaxRow, hFont, FALSE);
						DeleteObject(hFont);
						dom.moveParent();
					}
					
					//DOUBLE dRealColWidth = 0;
					//if ( m_Spread.GetMaxTextColWidth(SS_HEADER+nCol, &dRealColWidth) )
					{
						if ( nWidth )
							m_Spread.SetColWidth(SS_HEADER+nCol, nWidth);
					}
				}
				
				else if ( strName == "Span" )
				{
					int nCol = dom.getAttributeINT("col", true);
					int nRow = dom.getAttributeINT("row", true);
					int nNumCol = dom.getAttributeINT("numcol", true);
					int nNumRow = dom.getAttributeINT("numrow", true);
					
					m_Spread.AddCellSpan(SS_HEADER+nCol, SS_HEADER+nRow, nNumCol, nNumRow);
					dom.SetCurrentNode(pPos);
				}
				
				else if ( strName == "Merge" )
				{
					int nRow = dom.getAttributeINT("row", true);
					dom.SetCurrentNode(pPos);
				}
			}
			
			/////////////////////////////// Body /////////////////////////////////
			
			
			dom.SetCurrentNode(pSheetNode);
			IXMLDOMNodePtr pPosBody = dom.moveNodeXPath("./Body", false);
			if ( pPosBody )
			{
				int nStartIndex = 0x0FFFFFFF;
				
				if ( dom.moveNodeXPath("./Font", false) )
				{
					int nHeight = dom.getAttributeINT("Height", true);
					int nBold = dom.getAttributeINT("Bold", true);
					CString strName = dom.getAttributeSTR("Name", true);
					HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
					
					m_Spread.SetFontRange(1, 1, SS_LAST, SS_LAST, hFont, FALSE);
					DeleteObject(hFont);
					dom.moveParent();
				}
				
				IXMLDOMNodeListPtr pNodeList = pPosBody->selectNodes("./Row");
				if ( pNodeList )
				{
					for(IXMLDOMNodePtr pPos = pNodeList->nextNode(); pPos; pPos=pNodeList->nextNode() )
					{
						dom.SetCurrentNode(pPos);
						int nRow = dom.getAttributeINT("row", true);
						int nHeight = dom.getAttributeINT("height");
						if ( nHeight )
							m_Spread.SetRowHeight(nRow, nHeight);
						
						IXMLDOMNodeListPtr pList = pPos->selectNodes("./Col");
						if ( pList )
						{
							for(IXMLDOMNodePtr pPos = pList->nextNode(); pPos; pPos=pList->nextNode() )
							{
								dom.SetCurrentNode(pPos);
								int nCol = dom.getAttributeINT("col", true);
								CString strText = dom.getAttributeSTR("desc", true);
								m_Spread.SetValue(nCol, nRow, _T(strText));
								//m_Spread.SetColor(nCol, nRow, PIO_COLOR());
							}
						}
						
						dom.SetCurrentNode(pPos);
						if ( dom.moveNodeXPath("./Font", false) )
						{
							int nHeight = dom.getAttributeINT("Height", true);
							int nBold = dom.getAttributeINT("Bold", true);
							CString strName = dom.getAttributeSTR("Name", true);
							HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
							
							m_Spread.SetFontRange(1, nRow, SS_LAST, nRow, hFont, FALSE);
							DeleteObject(hFont);
							dom.moveParent();
						}
						
						dom.SetCurrentNode(pPos);
						if ( dom.moveNodeXPath("./Memory", false) )
						{
							if ( pStartAddr && (nOffSet==1 || nOffSet==2 || nOffSet==4) )
							{
								int nIndex = dom.getAttributeINT("index", true);
								if ( nIndex < nMaxByteLen && (nIndex+nOffSet)<=nMaxByteLen )
								{
									BYTE* pAddr = (BYTE*)pStartAddr+nIndex;
									m_Spread.SetRowUserData(nRow, (long)pAddr);
									nStartIndex = min(nIndex, nStartIndex);
								}
								else
									int a = 0;
							}
							else
								int a = 0;
						}
						
						dom.SetCurrentNode(pPos);
						if ( dom.moveNodeXPath("./Color", false) )
						{
							COLORREF colorBack = SPREAD_COLOR_NONE;
							COLORREF colorFore = SPREAD_COLOR_NONE;
							
							if ( dom.moveNodeXPath("./ColorBack", false) )
							{
								COLORREF R = dom.getAttributeINT("r", false);
								COLORREF G = dom.getAttributeINT("g", false);
								COLORREF B = dom.getAttributeINT("b", false);
								colorBack = RGB(R, G, B);
								dom.moveParent();
							}
							if ( dom.moveNodeXPath("./ColorFore", false) )
							{
								COLORREF R = dom.getAttributeINT("r", false);
								COLORREF G = dom.getAttributeINT("g", false);
								COLORREF B = dom.getAttributeINT("b", false);
								colorFore = RGB(R, G, B);
								dom.moveParent();
							}
							m_Spread.SetColorRange(1, nRow, SS_LAST, nRow, colorBack, colorFore);
						}
					}
				}
				
				dom.SetCurrentNode(pPosBody);
				SS_CELLTYPE sCellType;
				m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER | SS_TEXT_WORDWRAP);
				m_Spread.SetCellTypeRange(1, 1, SS_LAST, SS_LAST, &sCellType);
				
				if ( pStartAddr && nStartIndex != 0x0FFFFFFF && nStartIndex<nMaxByteLen && (nStartIndex+nOffSet)<=nMaxByteLen)
				{
					BYTE* pAddr = (BYTE*)pStartAddr+nStartIndex;
					if ( pAddr == auxProperty.pSheetAddr )
						m_Spread.SetUserData((long)pAddr);
				}
			}
		}// Sheet

	}
	catch ( CXmlException& e)
	{
		AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
	catch ( _com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}
	
	if ( !m_rectMargin.IsRectNull() )
	{
		CRect rect;
		::GetClientRect(m_pParentWnd->m_hWnd, &rect);
		rect.DeflateRect(m_rectMargin);
		ResizeWindow(rect);
	}

	return true;
}
//*/
/*
BOOL CSpreadSheet::CreateSheet(CString strFile, UINT nID)
{
	if ( !m_Spread.Create(WS_BORDER | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,
						  m_rectWnd, m_pParentWnd, nID) )
		return false;

	m_Spread.BringWindowToTop();

	CXmlDom dom;
	try
	{
		dom.InitizeXmlDom();
		dom.LoadXmlFile(strFile);

		IXMLDOMNodePtr pPosAux = dom.moveRootXPath("/AuxIO");
		IXMLDOMNodeListPtr pSheetList = pPosAux->selectNodes("./Sheet");
		if ( pSheetList==NULL)
			return FALSE;

		m_Spread.SetSheetCount(pSheetList->length);

		int i = 0;
		// Sheet
		IXMLDOMNodePtr pSheetNode = pSheetList->nextNode();
		for (; pSheetNode; pSheetNode = pSheetList->nextNode() )
		{
			dom.SetCurrentNode(pSheetNode);

			CString strSheetName = dom.getAttributeSTR("name", true);
			int nMaxRow = dom.getNodeINT("MaxBodyRow", true, true);
			int nMaxCol = dom.getNodeINT("MaxBodyCol", true, true);

			dom.moveNodeXPath("./Header");

			//m_Spread.SetBool(SSB_AUTOSIZE, true);
			m_Spread.SetGridType(dom.getNodeINT("GridType"));
			m_Spread.SetUserColAction(dom.getNodeINT("Sorting"));
			m_Spread.SetUserResize(dom.getNodeINT("ResizeColRow"));

			m_Spread.SetOperationMode(dom.getNodeINT("OperationMode"));
			m_Spread.SetBool(SSB_ALLOWCOLMOVE, dom.getNodeINT("AllowColMove"));
			m_Spread.SetGrayAreaColor(dom.getNodeHEX("GrayAreaColor"), SPREAD_COLOR_NONE);
			m_Spread.SetBackColorStyle(dom.getNodeINT("BackColorStyle"));

			int nHeadRows = dom.getNodeINT("MaxHeadRow", true, true);
			int nHeadCols = dom.getNodeINT("MaxHeadCol", true, true);

			m_Spread.SetSheet(++i);
			m_Spread.SetSheetName(i, strSheetName);
			m_Spread.SetMaxCols(nMaxCol);
			m_Spread.SetMaxRows(nMaxRow);
			m_Spread.SetColHeaderRows(nHeadRows);
			m_Spread.SetRowHeaderCols(nHeadCols);
			m_Spread.SetRowHeaderDisplay(dom.getNodeINT("RowHeaderDisplay"));
			m_Spread.SetColHeaderDisplay(dom.getNodeINT("ColHeaderDisplay"));

		dom.SetCurrentNode(pSheetNode);
		IXMLDOMNodePtr pPos = dom.moveNodeXPath("./Header/HeaderRow/Row", false);
		for ( ; pPos; pPos = dom.moveSiblingNext() )
		{
			CString strName = dom.GetNodeName(pPos);
			if ( strName == "Row" )
			{
				int nRow = dom.getAttributeINT("row", true);

				if ( dom.moveNodeXPath("./Font", false) )
				{
					int nHeight = dom.getAttributeINT("Height", true);
					int nBold = dom.getAttributeINT("Bold", true);
					CString strName = dom.getAttributeSTR("Name", true);
					HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);

					m_Spread.SetFontRange(1,SS_HEADER+nRow, nMaxCol, SS_HEADER+nRow, hFont, FALSE);
					DeleteObject(hFont);
					dom.moveParent();
				}

				if ( dom.SelectElements("./Col", FALSE) )
				{
					while ( dom.getSelectNextNode() )
					{
						int nCol = dom.getAttributeINT("col", true);
						int nWidth = dom.getAttributeINT("width");
						CString strText = dom.getNodeSTR("Text", true);
						m_Spread.SetValue(nCol, SS_HEADER+nRow, _T(strText));

						if ( dom.moveNodeXPath("./Font", false) )
						{
							int nHeight = dom.getAttributeINT("Height", true);
							int nBold = dom.getAttributeINT("Bold", true);
							CString strName = dom.getAttributeSTR("Name", true);
							HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
	
							m_Spread.SetFont(nCol,SS_HEADER+nRow, hFont, FALSE);
							DeleteObject(hFont);
							dom.moveParent();
						}

						//DOUBLE dRealColWidth = 0;
						//if ( m_Spread.GetMaxTextColWidth(nCol, &dRealColWidth) )
						{
							if ( nWidth )
								m_Spread.SetColWidth(nCol, nWidth);
						}
					}
				}

				dom.SetCurrentNode(pPos);
			}

			else if ( strName == "Span" )
			{
				int nCol = dom.getAttributeINT("col", true);
				int nRow = dom.getAttributeINT("row", true);
				int nNumCol = dom.getAttributeINT("numcol", true);
				int nNumRow = dom.getAttributeINT("numrow", true);
				
				m_Spread.AddCellSpan(nCol, SS_HEADER+nRow, nNumCol, nNumRow);
				dom.SetCurrentNode(pPos);
			}

			else if ( strName == "Merge" )
			{
				int nRow = dom.getAttributeINT("row", true);
				dom.SetCurrentNode(pPos);
			}
		}

		dom.SetCurrentNode(pSheetNode);
		pPos = dom.moveNodeXPath("./Header/HeaderCol/Col", false);
		for ( ; pPos; pPos = dom.moveSiblingNext() )
		{
			CString strName = dom.GetNodeName(pPos);
			if ( strName == "Col" )
			{
				int nCol = dom.getAttributeINT("col", true);
				int nWidth = dom.getAttributeINT("width");

				if ( dom.SelectElements("./Row", FALSE) )
				{
					while ( dom.getSelectNextNode() )
					{
						int nRow = dom.getAttributeINT("row", true);
						CString strText = dom.getNodeSTR("Text", true);
						int nTextOrient = dom.getNodeHEX("TextOrient", false);

						if ( nTextOrient )
						{
							SS_CELLTYPE sCellType = {0};
						    m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER|nTextOrient);
							m_Spread.SetCellType(SS_HEADER+nCol, SS_HEADER+nRow, &sCellType);
							m_Spread.SetColor(SS_HEADER+nCol, SS_HEADER+nRow, ::GetSysColor(COLOR_BTNFACE), SPREAD_COLOR_NONE); 
						}
						m_Spread.SetValue(SS_HEADER+nCol, SS_HEADER+nRow, _T(strText));

						if ( dom.moveNodeXPath("./Font", false) )
						{
							int nHeight = dom.getAttributeINT("Height", true);
							int nBold = dom.getAttributeINT("Bold", true);
							CString strName = dom.getAttributeSTR("Name", true);
							HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);
	
							m_Spread.SetFont(SS_HEADER+nCol,SS_HEADER+nRow, hFont, FALSE);
							DeleteObject(hFont);
							dom.moveParent();
						}
					}
				}
				dom.SetCurrentNode(pPos);

				if ( dom.moveNodeXPath("./Font", false) )
				{
					int nHeight = dom.getAttributeINT("Height", true);
					int nBold = dom.getAttributeINT("Bold", true);
					CString strName = dom.getAttributeSTR("Name", true);
					HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);

					m_Spread.SetFontRange(SS_HEADER+nCol, SS_HEADER+nHeadRows, SS_HEADER+nCol, SS_HEADER+nHeadRows+nMaxRow, hFont, FALSE);
					DeleteObject(hFont);
					dom.moveParent();
				}

				//DOUBLE dRealColWidth = 0;
				//if ( m_Spread.GetMaxTextColWidth(SS_HEADER+nCol, &dRealColWidth) )
				{
					if ( nWidth )
						m_Spread.SetColWidth(SS_HEADER+nCol, nWidth);
				}
			}

			else if ( strName == "Span" )
			{
				int nCol = dom.getAttributeINT("col", true);
				int nRow = dom.getAttributeINT("row", true);
				int nNumCol = dom.getAttributeINT("numcol", true);
				int nNumRow = dom.getAttributeINT("numrow", true);
				
				m_Spread.AddCellSpan(SS_HEADER+nCol, SS_HEADER+nRow, nNumCol, nNumRow);
				dom.SetCurrentNode(pPos);
			}

			else if ( strName == "Merge" )
			{
				int nRow = dom.getAttributeINT("row", true);
				dom.SetCurrentNode(pPos);
			}
		}

		/////////////////////////////// Body /////////////////////////////////


		dom.SetCurrentNode(pSheetNode);
		IXMLDOMNodePtr pPosBody = dom.moveNodeXPath("./Body", false);
		if ( pPosBody )
		{
			int nStartIndex = 0x0FFFFFFF;

			if ( dom.moveNodeXPath("./Font", false) )
			{
				int nHeight = dom.getAttributeINT("Height", true);
				int nBold = dom.getAttributeINT("Bold", true);
				CString strName = dom.getAttributeSTR("Name", true);
				HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);

				m_Spread.SetFontRange(1, 1, SS_LAST, SS_LAST, hFont, FALSE);
				DeleteObject(hFont);
				dom.moveParent();
			}

			IXMLDOMNodeListPtr pNodeList = pPosBody->selectNodes("./Row");
			if ( pNodeList )
			{
				for(IXMLDOMNodePtr pPos = pNodeList->nextNode(); pPos; pPos=pNodeList->nextNode() )
				{
					dom.SetCurrentNode(pPos);
					int nRow = dom.getAttributeINT("row", true);
					int nHeight = dom.getAttributeINT("height");
					if ( nHeight )
						m_Spread.SetRowHeight(nRow, nHeight);

					IXMLDOMNodeListPtr pList = pPos->selectNodes("./Col");
					if ( pList )
					{
						for(IXMLDOMNodePtr pPos = pList->nextNode(); pPos; pPos=pList->nextNode() )
						{
							dom.SetCurrentNode(pPos);
							int nCol = dom.getAttributeINT("col", true);
							CString strText = dom.getAttributeSTR("desc", true);
							m_Spread.SetValue(nCol, nRow, _T(strText));
							//m_Spread.SetColor(nCol, nRow, PIO_COLOR());
						}
					}

					dom.SetCurrentNode(pPos);
					if ( dom.moveNodeXPath("./Font", false) )
					{
						int nHeight = dom.getAttributeINT("Height", true);
						int nBold = dom.getAttributeINT("Bold", true);
						CString strName = dom.getAttributeSTR("Name", true);
						HFONT hFont = CreateFont(nHeight,0,0,0,nBold,0,0,0,0,0,0,0,0,strName);

						m_Spread.SetFontRange(1, nRow, SS_LAST, nRow, hFont, FALSE);
						DeleteObject(hFont);
						dom.moveParent();
					}

					dom.SetCurrentNode(pPos);
					if ( dom.moveNodeXPath("./Memory", false) )
					{
						if ( pStartAddr && (nOffSet==1 || nOffSet==2 || nOffSet==4) )
						{
							int nIndex = dom.getAttributeINT("index", true);
							if ( nIndex < nMaxByteLen && (nIndex+nOffSet)<=nMaxByteLen )
							{
								BYTE* pAddr = (BYTE*)pStartAddr+nIndex;
								m_Spread.SetRowUserData(nRow, (long)pAddr);
								nStartIndex = min(nIndex, nStartIndex);
							}
							else
								int a = 0;
						}
						else
							int a = 0;
					}

					dom.SetCurrentNode(pPos);
					if ( dom.moveNodeXPath("./Color", false) )
					{
						COLORREF colorBack = SPREAD_COLOR_NONE;
						COLORREF colorFore = SPREAD_COLOR_NONE;

						if ( dom.moveNodeXPath("./ColorBack", false) )
						{
							COLORREF R = dom.getAttributeINT("r", false);
							COLORREF G = dom.getAttributeINT("g", false);
							COLORREF B = dom.getAttributeINT("b", false);
							colorBack = RGB(R, G, B);
							dom.moveParent();
						}
						if ( dom.moveNodeXPath("./ColorFore", false) )
						{
							COLORREF R = dom.getAttributeINT("r", false);
							COLORREF G = dom.getAttributeINT("g", false);
							COLORREF B = dom.getAttributeINT("b", false);
							colorFore = RGB(R, G, B);
							dom.moveParent();
						}
						m_Spread.SetColorRange(1, nRow, SS_LAST, nRow, colorBack, colorFore);
					}
				}
			}

			dom.SetCurrentNode(pPosBody);
			SS_CELLTYPE sCellType;
			m_Spread.SetTypeStaticText(&sCellType, SS_TEXT_CENTER | SS_TEXT_VCENTER | SS_TEXT_WORDWRAP);
			m_Spread.SetCellTypeRange(1, 1, SS_LAST, SS_LAST, &sCellType);

			if ( pStartAddr && nStartIndex != 0x0FFFFFFF && nStartIndex<nMaxByteLen && (nStartIndex+nOffSet)<=nMaxByteLen)
			{
				BYTE* pAddr = (BYTE*)pStartAddr+nStartIndex;
				if ( pAddr == auxProperty.pSheetAddr )
					m_Spread.SetUserData((long)pAddr);
			}
		}
		}// Sheet
	}
	catch ( CXmlException& e)
	{
		AfxMessageBox(e.ErrorMessage());
		return FALSE;
	}
	catch ( _com_error& e)
	{
		AfxMessageBox(dom.GetErrorMessage(e));
		return FALSE;
	}

	if ( !m_rectMargin.IsRectNull() )
	{
		CRect rect;
		::GetClientRect(m_pParentWnd->m_hWnd, &rect);
		rect.DeflateRect(m_rectMargin);
		ResizeWindow(rect);
	}

	return true;
}
*/

void CSpreadSheet::AutoResizeColumn()
{
	if ( m_bAutoResizeCol == 0 )
		return;

	ResizeAllColumn();
}

void CSpreadSheet::ResizeAllColumn()
{
	SS_COORD nMaxCols = m_Spread.GetMaxCols();

	CSheet* pSheet = GetCurrentSheet();
	if ( pSheet == NULL )
		return;

	int nCol = 1;
	for (POSITION pos = pSheet->m_listHeader.GetHeadPosition(); pos != NULL; nCol++)
	{
		CSpreadHeaderCol* pHeader = (CSpreadHeaderCol*)pSheet->m_listHeader.GetNext(pos);

		DOUBLE dColWidth = 0;
		if ( !m_Spread.GetMaxTextColWidth(nCol, &dColWidth) )
			continue;

		if ( dColWidth > pHeader->m_nHeadColWidth )
			m_Spread.SetColWidth(nCol, dColWidth+m_nResizeIndent);
	}
}

void CSpreadSheet::SetFontEntireSheet()
{
	CSheet* pSheet = GetCurrentSheet();
	if ( pSheet == NULL )
		return;

	if ( pSheet->m_hFontBody )
		m_Spread.SetFontRange(1, 1, SS_LAST, SS_LAST, pSheet->m_hFontBody, false);
}

void CSpreadSheet::PrepareLoadSpread()
{
	m_Spread.SetBool(SSB_REDRAW, false);

	m_Spread.DelRowRange(SS_FIRST, SS_LAST);
	m_Spread.SetMaxRows(0);
}

void CSpreadSheet::FinishLoadSpread()
{
	// [LGLS 2026-09-03] 시트 정의(헤더 목록)에 없는 열은 기본 셀 타입(편집형=왼쪽 정렬)으로 남아
	//   첫 열(창고 타입)만 왼쪽에 붙어 보였다. 편집형으로 남은 열만 가운데 정렬 정적 셀로 바꾼다.
	//   (버튼/체크박스/콤보 셀은 건드리지 않는다)
	if (::IsWindow(m_Spread.m_hWnd))
	{
		SS_COORD nMaxCol = m_Spread.GetMaxCols();
		SS_COORD nMaxRow = m_Spread.GetMaxRows();
		if (nMaxRow >= 1)
		{
			SS_CELLTYPE sCenter = {0};
			m_Spread.SetTypeStaticText(&sCenter, SS_TEXT_CENTER | SS_TEXT_VCENTER);
			for (SS_COORD c = 1; c <= nMaxCol; c++)
			{
				int nType = GetCellType(c, 1);
				if (nType == SS_TYPE_BUTTON)
					continue;
				m_Spread.SetCellTypeRange(c, 1, c, nMaxRow, &sCenter);
			}
		}
	}
	SetFontEntireSheet();
	AutoResizeColumn();

	m_Spread.SetBool(SSB_REDRAW, true);
}

void CSpreadSheet::SetMargin(CRect rect)
{
	CRect rectClient;
	::GetClientRect(m_pParentWnd->m_hWnd, &rectClient);

	m_rectMargin.left = rect.left;
	m_rectMargin.top = rect.top;
	m_rectMargin.right = rectClient.right-rect.right;
	m_rectMargin.bottom = rectClient.bottom-rect.bottom;
}

void CSpreadSheet::SetMargin(CWnd* pWndChild)
{
	CRect rectChild;
	pWndChild->GetWindowRect(rectChild);
	m_pParentWnd->ScreenToClient(rectChild);

	SetMargin(rectChild);
}

void CSpreadSheet::SetColWidth(SS_COORD Col, DOUBLE nWidth)
{
	if ( !::IsWindow(m_Spread.m_hWnd) )
		return;

	// [LGLS 2026-09-03] 그리드 창들이 데이터 글자수로 폭을 주는데, 폭이 글자에 딱 맞으면
	//   가운데 정렬이어도 왼쪽에 붙어 보인다(창고 타입 열). 양옆 여백 2자를 더한다.
	m_Spread.SetColWidth(Col, nWidth + 2);
}

void CSpreadSheet::SetAdjustRowHeight()
{
	double nRowHeight;
	m_Spread.GetMaxTextRowHeight(m_nCurrentRow, &nRowHeight);
	SetRowHeight(m_nCurrentRow, nRowHeight);
}

void CSpreadSheet::SetRowHeight()
{
	double nRowHeight;
	m_Spread.GetMaxTextRowHeight(m_nCurrentRow, &nRowHeight);
	SetRowHeight(m_nCurrentRow, nRowHeight);
}

void CSpreadSheet::SetRowHeight(SS_COORD Row, DOUBLE nHeight)
{
	if ( !::IsWindow(m_Spread.m_hWnd) )
		return;

	m_Spread.SetRowHeight(Row, nHeight);
}

void CSpreadSheet::ShowHeaderRow(BOOL bShow)
{
	if ( !::IsWindow(m_Spread.m_hWnd) )
		return;
	m_Spread.SetBool(SSB_SHOWROWHEADERS, bShow);
}

void CSpreadSheet::ShowHeaderCol(BOOL bShow)
{
	if ( !::IsWindow(m_Spread.m_hWnd) )
		return;
	m_Spread.SetBool(SSB_SHOWCOLHEADERS, bShow);
}


void CSpreadSheet::SetUserResizeRow(int nResize)
{
    m_Spread.SetUserResizeRow(SS_ALLROWS, nResize);
}

void CSpreadSheet::SetUserResizeCol(int nResize)
{
    m_Spread.SetUserResizeCol(SS_ALLCOLS, nResize);
}

void CSpreadSheet::SetScrollBarHorz(BOOL bBool, BOOL bExtMode)
{
	m_Spread.SetBool(SSB_HORZSCROLLBAR, bBool);
	if ( bBool && bExtMode )
		m_Spread.SetBool(SSB_SCROLLBAREXTMODE, 1); // 스클롤바 필요시 생성
}

void CSpreadSheet::SetScrollBarVert(BOOL bBool, BOOL bExtMode)
{
	m_Spread.SetBool(SSB_VERTSCROLLBAR, bBool);
	if ( bBool && bExtMode )
		m_Spread.SetBool(SSB_SCROLLBAREXTMODE, 1); // 스클롤바 필요시 생성
}

void CSpreadSheet::SetScrollBarHeight(int nHeight)
{
	m_Spread.SetBool(SSB_HORZSCROLLBAR, TRUE);

	short nWidth = 0;
	short nHeightA = 0;
	m_Spread.GetScrollBarSize(&nWidth, &nHeightA); 
	m_Spread.SetScrollBarSize(nWidth, nHeight); 
}

void CSpreadSheet::SetScrollBarWidth(int nWidth)
{
	m_Spread.SetBool(SSB_VERTSCROLLBAR, TRUE);

	short nWidthA = 0;
	short nHeight = 0;
	m_Spread.GetScrollBarSize(&nWidthA, &nHeight); 
	m_Spread.SetScrollBarSize(nWidth, nHeight); 
}

void CSpreadSheet::SetSheetUserData(long nUserData)
{
	m_Spread.SetUserData(nUserData);
}

LONG CSpreadSheet::GetSheetUserData()
{
	return m_Spread.GetUserData();
}

BOOL CSpreadSheet::SetRowUserData(long nUserData)
{
	return m_Spread.SetRowUserData(m_nCurrentRow, nUserData);
}

LONG CSpreadSheet::GetRowUserData(SS_COORD nRow)
{
	LONG nUserData = 0;
	m_Spread.GetRowUserData(nRow, &nUserData);
	return nUserData;
}

BOOL CSpreadSheet::SetUserCellTag(SS_COORD Col, LPCTSTR CellTag)
{
	return SetUserCellTag(Col, m_nCurrentRow, CellTag);
}

BOOL CSpreadSheet::SetUserCellTag(SS_COORD Col, int nCellTag)
{
	CString strTag;
	strTag.Format(_T("%d"), nCellTag);
	return SetUserCellTag(Col, m_nCurrentRow, strTag);
}

BOOL CSpreadSheet::SetUserCellTagHEX(SS_COORD Col, UINT nCellTag)
{
	CString strValue;
	strValue.Format(_T("%x"), nCellTag);

	return SetUserCellTag(Col, m_nCurrentRow, strValue);
}

BOOL CSpreadSheet::SetUserCellTag(SS_COORD Col, SS_COORD Row, LPCTSTR CellTag)
{
	return m_Spread.SetCellTag(Col, Row, CellTag);
}

CString CSpreadSheet::GetUserCellTag(SS_COORD Col)
{
	return GetUserCellTag(Col, m_nCurrentRow);
}

int CSpreadSheet::GetUserCellTagINT(SS_COORD Col)
{
	CString strTag = GetUserCellTag(Col, m_nCurrentRow);
	return _ttoi(strTag);
}

UINT CSpreadSheet::GetUserCellTagHEX(SS_COORD Col)
{
	return GetUserCellTagHEX(Col, m_nCurrentRow);
}

int CSpreadSheet::GetUserCellTagINT(SS_COORD Col, SS_COORD Row)
{
	TCHAR szText[256] = {0};
	m_Spread.GetCellTag(Col, Row, (LPTSTR)szText);
	return _ttoi(szText);
}

UINT CSpreadSheet::GetUserCellTagHEX(SS_COORD Col, SS_COORD Row)
{
	TCHAR szText[256] = {0};
	m_Spread.GetCellTag(Col, Row, (LPTSTR)szText);

	UINT	nTag = 0;
	swscanf(szText, _T("%x"), &nTag);
	return nTag;
}

CString CSpreadSheet::GetUserCellTag(SS_COORD Col, SS_COORD Row)
{
	TCHAR szText[256] = {0};
	m_Spread.GetCellTag(Col, Row, (LPTSTR)szText);
	return szText;
}

CString CSpreadSheet::GetSheetName()
{
	TCHAR szName[1024];
	memset(szName, 0x00, sizeof(szName));
	int nSheet = m_Spread.GetSheet();
	m_Spread.GetSheetName(nSheet, szName, sizeof(szName));

	return szName;
}

///////////////////////////////////////////////////////////
//
BOOL CSpreadSheet::SetCurrentRow()
{
	m_nCurrentRow = m_Spread.GetMaxRows()+1;
	m_Spread.SetMaxRows(m_nCurrentRow);	

	return true;
}

void CSpreadSheet::DeleteRow(SS_COORD nRow)
{
	if ( m_Spread.DelRow(nRow) )
	{
		int nMaxRow = m_Spread.GetMaxRows();
		m_Spread.SetMaxRows(nMaxRow-1);	
	}
}

void CSpreadSheet::SetData(SS_COORD nCol, CString strData)
{
	InsertData(nCol, m_nCurrentRow, strData);
}

void CSpreadSheet::SetData(SS_COORD nCol, int nData)
{
	InsertData(nCol, m_nCurrentRow, nData);
}

void CSpreadSheet::SetData(SS_COORD nCol, CString strData, COLORREF colorFore)
{
	InsertData(nCol, m_nCurrentRow, strData, SPREAD_COLOR_NONE, colorFore);
}

void CSpreadSheet::SetData(SS_COORD nCol, int nData, COLORREF colorFore)
{
	CString strData;
	strData.Format(_T("%d"), nData);

	InsertData(nCol, m_nCurrentRow, strData, SPREAD_COLOR_NONE, colorFore);
}

void CSpreadSheet::SetDataHex(SS_COORD nCol, int nData, COLORREF colorFore)
{
	CString strData;
	strData.Format(_T("0x%.4X"), nData);

	InsertData(nCol, m_nCurrentRow, strData, SPREAD_COLOR_NONE, colorFore);
}

void CSpreadSheet::SetData(SS_COORD nCol, CString strData, COLORREF colorBack, COLORREF colorFore)
{
	if ( GetCellType(nCol, m_nCurrentRow) == SS_TYPE_BUTTON )
	{
		SetButtonData(nCol, strData, colorBack, colorFore);
		return;
	}

	InsertData(nCol, m_nCurrentRow, strData, colorBack, colorFore);
}

void CSpreadSheet::SetData(SS_COORD nCol, int nData, COLORREF colorBack, COLORREF colorFore)
{
	CString strData;
	strData.Format(_T("%d"), nData);

	if ( GetCellType(nCol, m_nCurrentRow) == SS_TYPE_BUTTON )
	{
		SetButtonData(nCol, strData, colorBack, colorFore);
		return;
	}

	InsertData(nCol, m_nCurrentRow, strData, colorBack, colorFore);
}

void CSpreadSheet::SetData(SS_COORD nCol, SS_COORD nRow, CString strData)
{
	InsertData(nCol, nRow, strData);
}

void CSpreadSheet::SetData(SS_COORD nCol, SS_COORD nRow, CString strData, COLORREF colorFore, COLORREF colorBack)
{
	InsertData(nCol, nRow, strData, colorBack, colorFore);
}

void CSpreadSheet::SetFormatData(SS_COORD nCol, CString strFormat)
{
	m_Spread.ClipIn(nCol, m_nCurrentRow, SS_LAST, m_nCurrentRow, strFormat, strFormat.GetLength());
}

COLORREF CSpreadSheet::GetColorBack(SS_COORD nCol, SS_COORD nRow)
{
	COLORREF colorBack = 0;
	COLORREF colorFore = 0;

	switch ( GetCellType(nCol, nRow) )
	{
	case	SS_TYPE_BUTTON:
		GetButtonColor(nCol, nRow, &colorBack, &colorFore);
		return colorBack;
	}


	m_Spread.GetColor(nCol, nRow, &colorBack, &colorFore);
	return colorBack;
}

COLORREF CSpreadSheet::GetColorFore(SS_COORD nCol, SS_COORD nRow)
{
	COLORREF colorBack = 0;
	COLORREF colorFore = 0;

	switch ( GetCellType(nCol, nRow) )
	{
	case	SS_TYPE_BUTTON:
		GetButtonColor(nCol, nRow, &colorBack, &colorFore);
		return colorFore;
	}

	m_Spread.GetColor(nCol, nRow, &colorBack, &colorFore);
	return colorFore;
}

void CSpreadSheet::SetColorBack(SS_COORD nCol, COLORREF colorBack)
{
	SetColorBack(nCol, m_nCurrentRow, colorBack);
}

void CSpreadSheet::SetColorFore(SS_COORD nCol, COLORREF colorFore)
{
	SetColorFore(nCol, m_nCurrentRow, colorFore);
}

void CSpreadSheet::SetColor(SS_COORD nCol, COLORREF colorBack, COLORREF colorFore)
{
	SetColor(nCol, m_nCurrentRow, colorBack, colorFore);
}

void CSpreadSheet::SetColorBack(SS_COORD nCol, SS_COORD nRow, COLORREF colorBack)
{
	switch ( GetCellType(nCol, nRow) )
	{
	case	SS_TYPE_BUTTON:
		SetButtonColor(nCol, nRow, colorBack, SPREAD_COLOR_NONE);		return;
	}

	m_Spread.SetColor(nCol, nRow, colorBack, SPREAD_COLOR_NONE);
}

void CSpreadSheet::SetColorFore(SS_COORD nCol, SS_COORD nRow, COLORREF colorFore)
{
	switch ( GetCellType(nCol, nRow) )
	{
	case	SS_TYPE_BUTTON:
		SetButtonColor(nCol, nRow, SPREAD_COLOR_NONE, colorFore);		return;
	}

	m_Spread.SetColor(nCol, nRow, SPREAD_COLOR_NONE, colorFore);
}

void CSpreadSheet::SetColor(SS_COORD nCol, SS_COORD nRow, COLORREF colorBack, COLORREF colorFore)
{
	switch ( GetCellType(nCol, nRow) )
	{
	case	SS_TYPE_BUTTON:
		SetButtonColor(nCol, nRow, colorBack, colorFore);		return;
	}

	m_Spread.SetColor(nCol, nRow, colorBack, colorFore);
}

void CSpreadSheet::SetAllRowColor(COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetColorRange(SS_FIRST, m_nCurrentRow, SS_LAST, m_nCurrentRow, colorBack, colorFore); 
}

void CSpreadSheet::SetAllRowColor(SS_COORD nRow, COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetColorRange(SS_FIRST, nRow, SS_LAST, nRow, colorBack, colorFore); 
}

void CSpreadSheet::SetAllColColor(SS_COORD nCol, COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetColorRange(nCol, SS_FIRST, nCol, SS_LAST, colorBack, colorFore); 
}

void CSpreadSheet::SetSheetColor(COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetColorRange(SS_FIRST, SS_FIRST, SS_LAST, SS_LAST, colorBack, colorFore); 
}

///////////////////////////////////////////////////////////
//
void CSpreadSheet::InsertData(SS_COORD nCol, SS_COORD nRow, CString strData)
{
	m_Spread.SetData(nCol, nRow, (LPCTSTR)strData);
}

void CSpreadSheet::InsertData(SS_COORD nCol, SS_COORD nRow, int nData)
{
	m_Spread.SetInteger(nCol, nRow, nData);
}

void CSpreadSheet::InsertData(SS_COORD nCol, SS_COORD nRow, CString strData, COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetData(nCol, nRow, (LPCTSTR)strData);
	m_Spread.SetColor(nCol, nRow, colorBack, colorFore);
}

void CSpreadSheet::SetCellLock(SS_COORD nCol, BOOL bLock, COLORREF colorBack, COLORREF colorFore)
{
	SetCellLock(nCol, m_nCurrentRow, bLock, colorBack, colorFore);
}

void CSpreadSheet::SetCellLock(SS_COORD nCol, SS_COORD nRow, BOOL bLock, COLORREF colorBack, COLORREF colorFore)
{
	m_Spread.SetLock(nCol, nRow, bLock);
	SetColor(nCol, nRow, colorBack, bLock ? DARK_GRAY : colorFore);
}

BOOL CSpreadSheet::SetCellSpan(SS_COORD nCol)
{
	return m_Spread.AddCellSpan(nCol, m_nCurrentRow, (m_Spread.GetMaxCols()-nCol)+1, m_nCurrentRow); 
}

BOOL CSpreadSheet::SetCellSpan(SS_COORD nCol, SS_COORD lNumCols)
{
	return m_Spread.AddCellSpan(nCol, m_nCurrentRow, lNumCols, m_nCurrentRow); 
}

BOOL CSpreadSheet::SetCellSpan(SS_COORD nCol, SS_COORD nRow, SS_COORD lNumCols, SS_COORD lNumRows)
{
	return m_Spread.AddCellSpan(nCol, nRow, lNumCols, lNumRows); 
}

void CSpreadSheet::RemoveCellSpan(SS_COORD nCol, SS_COORD nRow)
{
	m_Spread.RemoveCellSpan(nCol, nRow); 
}

void CSpreadSheet::SetColMerge(SS_COORD nCol, short nColMerge)
{
	m_Spread.SetColMerge(nCol, nColMerge);
}

void CSpreadSheet::SetRowMerge(SS_COORD nRow, short nRowMerge)
{
	m_Spread.SetRowMerge(nRow, nRowMerge);
}

void CSpreadSheet::SetHeaderColReverse(SS_COORD nCol)
{
	if ( !IsCellType(nCol, 0, SS_TYPE_CHECKBOX) )
		return;

	if ( IsCellType(nCol, 1, SS_TYPE_CHECKBOX) )
	{
		BOOL bCheck = GetValueINT(nCol, 0);
		m_Spread.SetInteger(nCol, 0, !bCheck);
		m_Spread.SetIntegerRange(nCol, 1, nCol, SS_ALLROWS, !bCheck); 
	}
}

void CSpreadSheet::SetHeaderColCheck(SS_COORD nCol)
{
	if ( !IsCellType(nCol, 0, SS_TYPE_CHECKBOX) )
		return;

	if ( IsCellType(nCol, 1, SS_TYPE_CHECKBOX) )
	{
		m_Spread.SetInteger(nCol, 0, true);
		m_Spread.SetIntegerRange(nCol, 1, nCol, SS_ALLROWS, true); 
	}
}

void CSpreadSheet::SetHeaderColUnCheck(SS_COORD nCol)
{
	if ( !IsCellType(nCol, 0, SS_TYPE_CHECKBOX) )
		return;

	if ( IsCellType(nCol, 1, SS_TYPE_CHECKBOX) )
	{
		m_Spread.SetInteger(nCol, 0, false);
		m_Spread.SetIntegerRange(nCol, 1, nCol, SS_ALLROWS, false); 
	}
}

void CSpreadSheet::SetHeaderCellCheck(SS_COORD nCol, BOOL bCheck)
{
	if ( !IsCellType(nCol, 0, SS_TYPE_CHECKBOX) )
		return;

	if ( IsCellType(nCol, 1, SS_TYPE_CHECKBOX) )
	{
		m_Spread.SetInteger(nCol, 0, bCheck);
	}
}

//////////////////////////////////////////////////////////
//
void CSpreadSheet::SetEdit(SS_COORD nCol, int nText)
{
	CString strText;
	strText.Format(_T("%d"), nText);
	SetEdit(nCol, m_nCurrentRow, strText);
}

void CSpreadSheet::SetEdit(SS_COORD nCol, LPCTSTR pText)
{
	SetEdit(nCol, m_nCurrentRow, pText);
}

void CSpreadSheet::SetEdit(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText)
{
	SetEdit(nCol, nRow, pText, ES_LEFT|SSS_ALIGN_VCENTER, 60, SS_CHRSET_CHR, SS_CASE_NOCASE, false);
}

void CSpreadSheet::SetEdit(SS_COORD nCol, LPCTSTR pText, long nStyle, short nLen, short nChrSet, short nChrCase, BOOL nRowHeight)
{
	SetEdit(nCol, m_nCurrentRow, pText, nStyle, nLen, nChrSet, nChrCase, nRowHeight);
}

void CSpreadSheet::SetEdit(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, long nStyle, short nLen, short nChrSet, short nChrCase, BOOL nRowHeight)
{
	SS_CELLTYPE sCellType = {0};

	LPSS_CELLTYPE aa = m_Spread.SetTypeEdit(&sCellType,
			nStyle,
			nLen,			//Maximum number of characters the user can type in the edit cell 
			nChrSet,    // Type of characters to use - SS_CHRSET_CHR:All characters 
			nChrCase);  // Case displays as typed
	m_Spread.SetCellType(nCol, nRow, &sCellType);
	m_Spread.SetData(nCol, nRow, pText);

	if ( (nStyle&ES_MULTILINE) && nRowHeight )
	{
		CSheet* pSheet = GetCurrentSheet();
		if ( pSheet == NULL )
			return;

		if ( pSheet->m_hFontBody )
			m_Spread.SetFontRange(nCol, nRow, SS_LAST, m_nCurrentRow, pSheet->m_hFontBody, false);

		SetRowHeight();
	}
}

void CSpreadSheet::SetStatic(SS_COORD nCol, LPCTSTR pText, long nStyle)
{
	SetStatic(nCol, m_nCurrentRow, pText, nStyle);
}

void CSpreadSheet::SetStatic(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, long nStyle)
{
	SS_CELLTYPE sCellType = {0};

    m_Spread.SetTypeStaticText(&sCellType, nStyle);
    m_Spread.SetCellType(nCol, nRow, &sCellType);
	m_Spread.SetData(nCol, nRow, pText);
}

void CSpreadSheet::SetCheckBox(SS_COORD nCol, LPCTSTR pText, BOOL bCheck)
{
	SetCheckBox(nCol, m_nCurrentRow, pText);
}

void CSpreadSheet::SetCheckBox(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, long nStyle, BOOL bCheck)
{
	SS_CELLTYPE sCellType = {0};
	if ( m_Spread.SetTypeCheckBox (&sCellType, nStyle, pText,
			NULL, BT_NONE,
			NULL, BT_NONE,
			NULL, BT_NONE,
			NULL, BT_NONE,
			NULL, BT_NONE,
			NULL, BT_NONE ) )
		m_Spread.SetCellType (nCol, nRow, &sCellType);

	m_Spread.SetInteger(nCol, nRow, bCheck);
}

void CSpreadSheet::SetButton(SS_COORD nCol, LPCTSTR pText)
{
	SetButton(nCol, m_nCurrentRow, pText, NULL);
}

void CSpreadSheet::SetButton(SS_COORD nCol, LPCTSTR pText, COLORREF color, COLORREF colorText)
{
	SetButton(nCol, m_nCurrentRow, pText, NULL);
	SetButtonColor(nCol, m_nCurrentRow, color, colorText);
}

void CSpreadSheet::SetButton(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, LPSUPERBTNCOLOR pColor)
{
	SS_CELLTYPE sCellType = {0};

	if ( !m_Spread.SetTypeButton (&sCellType,
				SBS_TEXTLEFT_PICTRIGHT,
				pText,
				NULL,								// Picture
				SUPERBTN_PICT_NONE,					// Type of picture to display
				NULL,								// PictureDown
				SUPERBTN_PICT_NONE,					// nPictureDownType
				SUPERBTN_TYPE_NORMAL,				// Button type
				2,									// Size in pixels of the shadow area (usually 1 or 2 pixels)
				pColor								// Button color
			)
		)
		return;

//	m_Spread.SetButtonDrawMode(SS_BDM_ALWAYS);
	m_Spread.SetCellType (nCol, nRow, &sCellType);
}

void CSpreadSheet::SetComboBox(SS_COORD nCol, LPCTSTR pText, int nIndex)
{
	SetComboBox(nCol, m_nCurrentRow, pText, nIndex);
}

void CSpreadSheet::SetComboBox(SS_COORD nCol, SS_COORD nRow, LPCTSTR pText, int nIndex)
{
	SS_CELLTYPE sCellType = {0};
    if ( !m_Spread.SetTypeComboBox(&sCellType, SSS_ALIGN_VCENTER|SSS_ALIGN_CENTER, (LPTSTR)pText) )
		return;

	if(m_Spread.SetCellType (nCol, nRow, &sCellType))
	{
		if ( nIndex >= 0 && nIndex < GetComboBoxCount(nCol, nRow) )
			SetComboCurSel(nCol, nRow, nIndex);
	}
}

CString CSpreadSheet::GetComboBoxText(SS_COORD nCol, SS_COORD nRow)
{
	int nIndex = GetComboBoxSel(nCol, nRow);
	if ( nIndex == -1 )
		return _T("");

	TCHAR szText[512] = {0};
	m_Spread.ComboBoxSendMessage(nCol, nRow, SS_CBM_GETLBTEXT, nIndex, (LPARAM)(LPTSTR)szText);
	return szText;
}

int CSpreadSheet::GetComboBoxCount(SS_COORD nCol, SS_COORD nRow)
{
	return m_Spread.ComboBoxSendMessage(nCol, nRow, SS_CBM_GETCOUNT, 0, 0);
}

int CSpreadSheet::GetComboBoxSel(SS_COORD nCol, SS_COORD nRow)
{
	return m_Spread.ComboBoxSendMessage(nCol, nRow, SS_CBM_GETCURSEL, 0, 0);
}

int CSpreadSheet::SetComboCurSel(SS_COORD nCol, WPARAM wParam)
{
	return SetComboCurSel(nCol, m_nCurrentRow, wParam);
}

int CSpreadSheet::SetComboCurSel(SS_COORD nCol, SS_COORD nRow, WPARAM wParam)
{
	if ( GetComboBoxCount(nCol, nRow) > (int)wParam )
		return m_Spread.ComboBoxSendMessage(nCol, nRow, SS_CBM_SETCURSEL, wParam, 0);
	return 0;
}

void CSpreadSheet::SetButtonData(SS_COORD nCol, LPCTSTR lpText, COLORREF color, COLORREF colorText)
{
	SetButtonColor(nCol, color, colorText);
	m_Spread.SetButtonText(nCol, m_nCurrentRow, lpText);
}

CString CSpreadSheet::GetButtonTXT(SS_COORD nCol, SS_COORD nRow)
{
	TCHAR szText[512];
	m_Spread.GetButtonText(nCol, nRow, szText);
	return szText;
}

void CSpreadSheet::SetButtonColor(SS_COORD nCol, COLORREF color, COLORREF colorText)
{
	SetButtonColor(nCol, m_nCurrentRow, color, colorText);
}

void CSpreadSheet::SetButtonColor(SS_COORD nCol, SS_COORD nRow, COLORREF color, COLORREF colorText)
{
	SS_CELLTYPE sCellType = {0};

	if ( m_Spread.GetCellType(nCol, nRow, &sCellType) )
	{
		if ( sCellType.Type == SS_TYPE_BUTTON )
		{
			TCHAR szText[256];
			m_Spread.GetButtonText(nCol, nRow, szText);

			sCellType.Spec.Button.Color.Color = color;
			sCellType.Spec.Button.Color.ColorText = colorText;

			SUPERBTNCOLOR colorBtn = sCellType.Spec.Button.Color;
			SetButton(nCol, nRow, szText, &colorBtn);
		}
	}
}

void CSpreadSheet::GetButtonColor(SS_COORD nCol, COLORREF* pcolorBack, COLORREF* pcolorText)
{
	GetButtonColor(nCol, m_nCurrentRow, pcolorBack, pcolorText);
}

void CSpreadSheet::GetButtonColor(SS_COORD nCol, SS_COORD nRow, COLORREF* pcolorBack, COLORREF* pcolorText)
{
	SS_CELLTYPE sCellType = {0};

	if ( m_Spread.GetCellType(nCol, nRow, &sCellType) )
	{
		if ( sCellType.Type == SS_TYPE_BUTTON )
		{
			*pcolorBack = sCellType.Spec.Button.Color.Color;
			*pcolorText = sCellType.Spec.Button.Color.ColorText;
		}
	}
}

void CSpreadSheet::SetPicture(SS_COORD nCol, long nStyle, UINT nIconID)
{
	SetPicture(nCol, m_nCurrentRow, nStyle, nIconID);
}

void CSpreadSheet::SetPicture(SS_COORD nCol, long nStyle, LPCTSTR pID)
{
	SetPicture(nCol, m_nCurrentRow, nStyle, pID);
}

void CSpreadSheet::SetPicture(SS_COORD nCol, SS_COORD nRow, long nStyle, LPCTSTR pID)
{
	SS_CELLTYPE sCellType = {0};

	if ( m_Spread.SetTypePicture(&sCellType,
			VPS_MAINTAINSCALE | VPS_STRETCH | VPS_CENTER | nStyle, pID) )
		m_Spread.SetCellType(nCol, nRow, &sCellType);
}

void CSpreadSheet::SetPicture(SS_COORD nCol, SS_COORD nRow, long nStyle, UINT nIconID)
{
	SS_CELLTYPE sCellType = {0};

	if ( nStyle & VPS_ICON )
	{
		CWinApp* pApp = (CWinApp *)AfxGetApp();
		if ( pApp == NULL )
			return;

		HICON hIcon = pApp->LoadIcon(nIconID);
		if ( hIcon == NULL )
			return;

		if ( m_Spread.SetTypePicture(&sCellType,
				VPS_MAINTAINSCALE | VPS_STRETCH | VPS_CENTER | VPS_HANDLE | nStyle, (LPCTSTR)(&hIcon)) )
			m_Spread.SetCellType(nCol, nRow, &sCellType);
	}
	else if ( nStyle & VPS_BMP )
	{
		HBITMAP hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIconID));
		if ( hBmp == NULL )
			return;

		if ( m_Spread.SetTypePictureHandle(&sCellType, 
				VPS_MAINTAINSCALE | VPS_STRETCH | VPS_CENTER | VPS_HANDLE | nStyle, hBmp, 0, TRUE) )
			m_Spread.SetCellType(nCol, nRow, &sCellType);

//		DeleteObject(hBmp);
	}
}

void CSpreadSheet::SetTextTip(WORD wTipStatus, LONG nDelay, LONG lfHeight)
{
	WORD wStatusOut;
	LONG nDelayOut;
	LOGFONT lfFont = {0};
	COLORREF crFore, crBack;
	m_Spread.GetTextTip(&wStatusOut, &nDelayOut, &lfFont, &crBack, &crFore);

	lfFont.lfQuality = PROOF_QUALITY;
	if ( lfHeight )
		lfFont.lfHeight = lfHeight;
	lfFont.lfWeight = FW_HEAVY;
//	lstrcpy(lfFont.lfFaceName, _T("System"));

	SetTextTip(wTipStatus, nDelay, &lfFont, crFore, crBack);
}

void CSpreadSheet::SetTextTip(WORD wTipStatus, LONG lDelay, LPLOGFONT lpLogFont, COLORREF clrBack, COLORREF clrFore)
{
	m_Spread.SetTextTip(SS_TT_STATUS_FLOATING, lDelay, lpLogFont, clrBack, clrFore);
}

////////////////////////////////////////////
//
BOOL CSpreadSheet::GetCellLock(SS_COORD nCol)
{
	return m_Spread.GetLock(nCol, m_nCurrentRow);
}

int CSpreadSheet::GetCellType(SS_COORD nCol, SS_COORD nRow)
{
	SS_CELLTYPE sCellType = {0};
	if ( m_Spread.GetCellType(nCol, nRow, &sCellType) )
		return sCellType.Type;
			
	return 0;
}

CPoint CSpreadSheet::GetActiveCell()
{
	SS_COORD nCol = 0;
	SS_COORD nRow = 0;
	if ( !m_Spread.GetActiveCell(&nCol, &nRow) )
		return CPoint(0, 0);

	return CPoint(nCol, nRow);
}

CSpreadCellList* CSpreadSheet::GetActiveCells()
{
	RemoveActiveCells();

	short nBlockCnt = 0;
	GLOBALHANDLE hHandle = m_Spread.GetMultiSelBlocks(&nBlockCnt);
	if ( hHandle == 0 )
	{
		CPoint point = GetActiveCell();
		if ( point.x == 0 || point.y == 0 )
			return NULL;

		CSpreadCell* pCell = new CSpreadCell(point.x, point.y, GetUserCellTagHEX(point.x, point.y));
		TRY
		{
			m_listSelCell.AddTail(pCell);
		}
		CATCH(CMemoryException, e)
		{
			return NULL;
		}
		END_CATCH

		return &m_listSelCell;
	}

	LPSS_SELBLOCK pSelBlock = (LPSS_SELBLOCK)::GlobalLock(hHandle);
	if ( pSelBlock == NULL )
		return NULL;

	for ( int i=0; i<nBlockCnt; i++ )
	{
		SS_SELBLOCK sBlock = pSelBlock[i];
		int nColUL = sBlock.UL.Col;
		int nRowUL = sBlock.UL.Row;
		int nColLR = sBlock.LR.Col;
		int nRowLR = sBlock.LR.Row;
		
		for ( int nRow = nRowUL; nRow<=nRowLR; nRow++ )
		{
			for ( int nCol = nColUL; nCol<=nColLR; nCol++ )
			{
				CSpreadCell* pCell = new CSpreadCell(nCol, nRow, GetUserCellTagHEX(nCol, nRow));
				
				TRY
				{
					m_listSelCell.AddTail(pCell);
				}
				CATCH(CMemoryException, e)
				{
					return NULL;
				}
				END_CATCH
			}
		}
	}

	::GlobalUnlock(hHandle);

	return &m_listSelCell;
}

int CSpreadSheet::GetActiveSheet()
{
	return m_Spread.GetSheet();
}

CSheet* CSpreadSheet::GetCurrentSheet()
{
	int nIndex = m_Spread.GetSheet();
	CSheet* pSheet = FindSheet(nIndex-1);
	return pSheet;
}

///////////////////////////////////////////
//
BOOL CSpreadSheet::IsCellType(SS_COORD nCol, SS_COORD nRow, BYTE nType)
{
	SS_CELLTYPE sCellType = {0};
	if ( m_Spread.GetCellType(nCol, nRow, &sCellType) )
		return sCellType.Type == nType ? true : false;
			
	return false;
}

BOOL CSpreadSheet::IsCellChecked(SS_COORD nCol, SS_COORD nRow)
{
	if ( IsCellType(nCol, nRow, SS_TYPE_CHECKBOX) )
		return GetValueINT(nCol, nRow);

	return false;
}

BOOL CSpreadSheet::IsCellActived()
{
	SS_COORD nCol = 0;
	SS_COORD nRow = 0;
	if ( !m_Spread.GetActiveCell(&nCol, &nRow) )
		return FALSE;

	if ( nCol != 0 && nRow != 0 )
		return TRUE;

	return FALSE;
}

BOOL CSpreadSheet::IsColsSelected(SS_COORD nCol) 
{
	SS_COORD nMaxRow = m_Spread.GetMaxRows();
	for (int nRow = 1; nRow <= nMaxRow; nRow++ )
	{
		if ( IsCellChecked(nCol, nRow) )
			return TRUE;
	}

	return FALSE;
}

LONG CSpreadSheet::GetValueINT(SS_COORD nCol, SS_COORD nRow)
{
	LONG nValue = 0;
	if ( !m_Spread.GetInteger(nCol, nRow, &nValue) )
		return 0;

	return nValue;
}

void CSpreadSheet::SetValueINT(SS_COORD nCol, SS_COORD nRow, LONG nValue)
{
	m_Spread.SetInteger(nCol, nRow, nValue);
}

CString CSpreadSheet::GetValueTXT(SS_COORD nCol, SS_COORD nRow)
{
	int nLen = m_Spread.GetDataLen(nCol, nRow);
	if ( nLen <= 0)
		return _T("");

	CString strText;
	if ( !m_Spread.GetData(nCol, nRow, strText.GetBuffer(nLen)) )
	{
		strText.ReleaseBuffer();
		return _T("");
	}
	strText.ReleaseBuffer();

	return strText;
}

BOOL CSpreadSheet::GetPioWordCell(CSpreadCell* pCell)
{
	SS_COORD nCol = 0;
	SS_COORD nRow = 0;
	if ( !m_Spread.GetActiveCell(&nCol, &nRow) )
		return FALSE;

	if ( nCol==0 || nRow==0 )
		return FALSE;

	int nMaxCol = m_Spread.GetMaxCols();
	if ( nMaxCol == 8 )
	{
		int nColA = abs(nCol - 8);
		int nRowA = (nRow-1) / 2;
		if ( ((nRow-1)%2)==1 )
			nColA += 8;

		pCell->m_nCol = nColA;
		pCell->m_nRow = nRowA;
		return TRUE;
	}
	if ( nMaxCol == 16 )
	{
		int nColA = abs(nCol - 16);
		int nRowA = nRow-1;

		pCell->m_nCol = nColA;
		pCell->m_nRow = nRowA;
		return TRUE;
	}

	return FALSE;
}

void CSpreadSheet::SetGrayAreaColor(COLORREF colorBaseBackColor)
{
	m_colorBaseBack = colorBaseBackColor;

	m_Spread.SetGrayAreaColor(m_colorBaseBack, m_colorBaseFore);
}

/*
SS_COORD CSpreadSheet::GetColFromID(int nID); 
{
	

}
*/

////////////////////////////////////////////////
//

CSheet::CSheet()
{
	m_strName = "";

	m_hFontHeader = NULL;
	m_hFontBody = NULL;
}

CSheet::CSheet(CString str, HFONT hFontHeader, HFONT hFontBody)
{
	m_strName = str;

	m_hFontHeader = hFontHeader;
	m_hFontBody = hFontBody;
}

CSheet::~CSheet()
{
	for (POSITION pos = m_listHeader.GetHeadPosition(); pos != NULL;)
	{
		delete m_listHeader.GetNext(pos);
	}
	m_listHeader.RemoveAll();

	if ( m_hFontHeader )
		DeleteObject(m_hFontHeader);
	if ( m_hFontBody )
		DeleteObject(m_hFontBody);
}

void CSheet::RemoveAllColHead()
{
	// Ver. JBY
//	for (POSITION pos = m_listHeader.GetHeadPosition(); pos != NULL;)
//	{
//		CObList::RemoveAt(pos);
//	}

	for (POSITION pos = m_listHeader.GetHeadPosition(); pos != NULL;)
	{
		delete m_listHeader.GetNext(pos);
	}
	m_listHeader.RemoveAll();
}

void CSheet::AddColHead(int nNumber)
{
	CString str;
	str.Format(_T("%d"), nNumber);
	
	AddColHead(str);
}

void CSheet::AddColHead(CString strText)
{
	CSpreadHeaderCol* pHeadCol = new CSpreadHeaderCol(strText);
	m_listHeader.AddTail(pHeadCol);
}

void CSheet::AddColHead(CString strText, int nColWidth, CString strColID)
{
	CSpreadHeaderCol* pHeadCol = new CSpreadHeaderCol(strText, nColWidth, strColID);
	m_listHeader.AddTail(pHeadCol);
}


////////////////////////////////////////////////
//

CSpreadHeaderCol::CSpreadHeaderCol(CString strText)
{
	m_strHeadColText = strText;
	m_strHeadColID = "";

	m_nHeadColWidth = 0;
}

CSpreadHeaderCol::CSpreadHeaderCol(CString strText, int nWidth)
{
	m_strHeadColText = strText;
	m_strHeadColID = "";

	m_nHeadColWidth = nWidth;
}

CSpreadHeaderCol::CSpreadHeaderCol(CString strText, int nWidth, CString strColID)
{
	m_strHeadColText = strText;
	m_strHeadColID = strColID;

	m_nHeadColWidth = nWidth;
}

CSpreadHeaderCol::~CSpreadHeaderCol()
{

}

////////////////////////////////////////////////
//
CSpreadCell::CSpreadCell()
{
	m_nCol = 0;
	m_nRow = 0;
}

CSpreadCell::CSpreadCell(int nCol, int nRow, UINT nLong)
{
	m_nCol = nCol;
	m_nRow = nRow;
	m_nLong = nLong;
}
