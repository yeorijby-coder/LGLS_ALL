#include "stdafx.h"
#include "DrawGradationRect.h"

CDrawGradationRect::CDrawGradationRect()
{
	m_pDC=NULL;

	m_crBkgnd1 = 0;
	m_crBkgnd2 = 0;
}

CDrawGradationRect::CDrawGradationRect(CDC *pDC, CRect rect, COLORREF crBkgnd, COLORREF crBkgndHigh , UINT mode, UINT tpercent)
{
	m_pDC = pDC;
	CPoint pt = CPoint(0,0);

	SetDrawArea(pt, rect);
	SetBkColor( crBkgnd, crBkgndHigh , mode, tpercent);
}

CDrawGradationRect::~CDrawGradationRect()
{
}

void CDrawGradationRect::GetDC(CDC* pDC)
{
	m_pDC = pDC;
}

void CDrawGradationRect::SetDrawArea(CPoint nStartPt, CRect rect)
{
	m_StartPoint	= nStartPt;
	m_rect			= rect;
}

void CDrawGradationRect::SetBkColor(COLORREF crBkgnd, COLORREF crBkgndHigh , UINT mode, UINT tpercent)
{
	m_crBkgnd1 = crBkgnd;
	if ( crBkgndHigh )
		m_crBkgnd2 = crBkgndHigh;
	m_fillmode = mode;
	m_tpercent = tpercent;
}

void CDrawGradationRect::SetGradationType(UINT tGType)
{
	m_Gradation_Type = tGType;
}

void CDrawGradationRect::Draw()
{
	if(m_pDC == NULL)
		return;

	CDC* pDCMem;
	CBitmap bmp;

	pDCMem = new CDC;
	pDCMem->CreateCompatibleDC(m_pDC);
	bmp.CreateCompatibleBitmap(m_pDC,m_rect.Width(),m_rect.Height());
	pDCMem->SelectObject(&bmp);
//	pDCMem = m_pDC;

	UINT nMode = pDCMem->SetBkMode(TRANSPARENT);

	CBrush brush;
	brush.CreateSolidBrush(m_crBkgnd1);
	CBrush *oldb = pDCMem->SelectObject(&brush);

	if (m_fillmode == Normal)
		pDCMem->FillRect(m_rect,&brush);
	else // Gradient Fill
		DrawGradientFill(pDCMem);

	m_pDC->BitBlt(m_StartPoint.x,m_StartPoint.y,m_rect.Width(),m_rect.Height(),pDCMem,0,0,SRCCOPY);

	// Restore DC's State
	pDCMem->SetBkMode(nMode);
	pDCMem->SelectObject(oldb);

	delete pDCMem;
}

void CDrawGradationRect::DrawGradientFill(CDC* pDC)
{
	COLORREF cr;
	COLORREF crStart = m_crBkgnd1;
	COLORREF crEnd = m_crBkgnd2;
	int nSegments = m_rect.Width();

	int nR = GetRValue(crStart);
	int nG = GetGValue(crStart);
	int nB = GetBValue(crStart);

	int neB = GetBValue(crEnd);
	int neG = GetGValue(crEnd);
	int neR = GetRValue(crEnd);

	int fillspace2;

	//------------------------
	// 그릴 데이터 영역 설정
	//------------------------
	if( m_Gradation_Type == RIGHTTOLEFT || m_Gradation_Type == LEFTTORIGHT)
	{
		// 그릴 영역 설정
		nSegments = m_rect.Width() * m_tpercent / 100;
		//  그려진 영역을 제외한 빈 공간
		fillspace2 = m_rect.Width() - nSegments;
	}
	else if(m_Gradation_Type == UPTODOWN ||  m_Gradation_Type == DOWNTOUP)
	{
		nSegments = m_rect.Height() * m_tpercent / 100;
		fillspace2 = m_rect.Height() - nSegments;
	}
	else if(m_Gradation_Type == SpreadToLeftAndRightFromCenter)
	{
		// 중간에서 왼쪽, 오른쪽으로 색상을 Gradation 
		// (하지만, 실제 코드는 왼쪽에서 중간으로, 오른쪽에서 중간으로 칠함)
		nSegments = (m_rect.Width() * m_tpercent / 100) / 2;
		fillspace2 = m_rect.Width() - (nSegments*2);
	}
	else if(m_Gradation_Type == SpreadToUpAndDownFromCenter)
	{
		// 중간에서 위, 아래로 색상을 Gradation 
		// (하지만, 실제 코드는 위에서 중간으로, 아래에서 중간으로 칠함)
		nSegments = (m_rect.Height() * m_tpercent / 100) / 2;
		fillspace2 = (m_rect.Height() - (nSegments*2)) / 2;
	}


	int nDiffR = (neR - nR);
	int nDiffG = (neG - nG);
	int nDiffB = (neB - nB);

	int ndR = 256 * (nDiffR) / (max(nSegments,1));
	int ndG = 256 * (nDiffG) / (max(nSegments,1));
	int ndB = 256 * (nDiffB) / (max(nSegments,1));

	nR *= 256;
	nG *= 256;
	nB *= 256;

	neR *= 256;
	neG *= 256;
	neB *= 256;

	int nCX = 1;
	int nLeft = m_rect.left, nRight = m_rect.right;
	int nUp = m_rect.top, nDown = m_rect.bottom;

	pDC->SelectStockObject(NULL_PEN);

	
	// Default Gradiation
	for (int i = 0; i < nSegments; i++, nR += ndR, nG += ndG, nB += ndB)
	{
		switch(m_Gradation_Type)
		{
		case LEFTTORIGHT :
			if (i == (nSegments - 1))
				nRight = m_rect.right;
			else
				nRight = nLeft + nCX;
			break;
		case RIGHTTOLEFT :
			if (i == (nSegments - 1))
				nLeft = m_rect.left;
			else
				nLeft = nRight - nCX;
			break;
		case UPTODOWN :
			if (i == (nSegments - 1))
				nDown = m_rect.bottom;
			else
				nDown = nUp + nCX;
			break;
		case DOWNTOUP :
			if (i == (nSegments - 1))
				nUp = m_rect.top;
			else
				nUp = nDown - nCX;
			break;
		case SpreadToLeftAndRightFromCenter :
			// 왼쪽 끝에서 오른쪽으로 색상 칠하기
			if (i == (nSegments - 1))
				nRight = m_rect.right / 2; //right;
			else
				nRight = nLeft + nCX;

		case SpreadToUpAndDownFromCenter :
			// 위에서 아래로 색상 칠하기
			if (i == (nSegments - 1))
				nDown = m_rect.bottom / 2;
			else
				nDown = nUp + nCX;
			break;
		}
		

		cr = RGB(nR / 256, nG / 256, nB / 256);
		
		if(m_Gradation_Type == LEFTTORIGHT 
		|| m_Gradation_Type == RIGHTTOLEFT//) 
		|| m_Gradation_Type == SpreadToLeftAndRightFromCenter)
		{
			CBrush br(cr);
			CBrush* pbrOld = pDC->SelectObject(&br);
			pDC->Rectangle(nLeft, m_rect.top, nRight + 1, m_rect.bottom+1);
			pDC->SelectObject(pbrOld);
		}
		else
		{
			CBrush br(cr);
			CBrush* pbrOld = pDC->SelectObject(&br);
			pDC->Rectangle(m_rect.left, nUp, m_rect.right+1, nDown+1);
			pDC->SelectObject(pbrOld);
		}

		switch(m_Gradation_Type)
		{
		case LEFTTORIGHT:
		// Reset the left side of the drawing rectangle.
			nLeft = nRight;
			break;
		case RIGHTTOLEFT:
			nRight = nLeft;
			break;
		case UPTODOWN:
			nUp = nDown;
			break;
		case DOWNTOUP:
			nDown = nUp;
			break;
		case SpreadToLeftAndRightFromCenter:
			nLeft = nRight;
			break;
		case SpreadToUpAndDownFromCenter:
			nUp = nDown;
			break;
		}
	}

	// 양방향으로 확산되는 그라데이션의 경우
	// 반대편도 그려줌
	if( (m_Gradation_Type == SpreadToLeftAndRightFromCenter)
	 || (m_Gradation_Type == SpreadToUpAndDownFromCenter) )
	{
		nR = GetRValue(crStart);
		nG = GetGValue(crStart);
		nB = GetBValue(crStart);

		nR *= 256;
		nG *= 256;
		nB *= 256;

		nCX = 1;

		nLeft = m_rect.left, nRight = m_rect.right;
		nUp = m_rect.top, nDown = m_rect.bottom;

		switch(m_Gradation_Type)
		{
		case SpreadToLeftAndRightFromCenter:
			nLeft = (m_rect.right/2);
			break;
		case SpreadToUpAndDownFromCenter:
			nUp = (m_rect.bottom/2);
			break;
		}

		for (int i = 0; i < nSegments; i++, nR += ndR, nG += ndG, nB += ndB)
		{
			switch(m_Gradation_Type)
			{	
			case SpreadToLeftAndRightFromCenter :
				// 오른끝에서 왼쪽으로 색상 칠하기
				if (i == (nSegments - 1))
					nLeft = (m_rect.right/2)+1;
				else
					nLeft = nRight - nCX;
			break;
			case SpreadToUpAndDownFromCenter :
				// 아래에서 위로 색상 칠하기
				if (i == (nSegments - 1))
					nUp = (m_rect.bottom/2)+1;
				else
					nUp = nDown - nCX;
				break;
			}
		
			cr = RGB(nR / 256, nG / 256, nB / 256);
		
			if(m_Gradation_Type == SpreadToLeftAndRightFromCenter)
			{
				CBrush br(cr);
				CBrush* pbrOld = pDC->SelectObject(&br);
				pDC->Rectangle(nLeft, m_rect.top, nRight + 1, m_rect.bottom+1);
				pDC->SelectObject(pbrOld);
			}
			else
			{
				CBrush br(cr);
				CBrush* pbrOld = pDC->SelectObject(&br);
				pDC->Rectangle(m_rect.left, nUp, m_rect.right+1, nDown+1);
				pDC->SelectObject(pbrOld);
			}
	
			switch(m_Gradation_Type)
			{
			case SpreadToLeftAndRightFromCenter:
				nRight = nLeft;
				break;
			case SpreadToUpAndDownFromCenter:
				nDown = nUp;
				break;
			}
		}
	
	}

	// 빈공간 채워줌
	{
		CRect rect;

		if(m_Gradation_Type == LEFTTORIGHT)
		{
			rect.top = 0;
			rect.bottom = m_rect.Height();
			rect.left = nSegments;
			rect.right = m_rect.Width();
		}
		else if(m_Gradation_Type == RIGHTTOLEFT)
		{
			rect.top = 0;
			rect.bottom = m_rect.Height();
			rect.left = 0;
			rect.right = fillspace2-1;//nSegments-1;
		}
		else if(m_Gradation_Type == UPTODOWN)
		{
			rect.top = nSegments+1;
			rect.bottom = m_rect.Height();
			rect.left = 0;
			rect.right = m_rect.Width();
		}
		else if(m_Gradation_Type == DOWNTOUP)
		{
			rect.top = 0;
			rect.bottom = fillspace2-1;
			rect.left = 0;
			rect.right = m_rect.Width();
		}
		else if(m_Gradation_Type == SpreadToLeftAndRightFromCenter)
		{
			rect.top = 0;
			rect.bottom = m_rect.Height();
			rect.left = nSegments;
			rect.right = rect.left + m_rect.Width()-(nSegments*2);
		}
		else if(m_Gradation_Type == SpreadToUpAndDownFromCenter)
		{
			rect.top = nSegments;
			rect.bottom = nSegments + m_rect.Height()-(nSegments*2);
			if(rect.top == rect.bottom)
				rect.bottom++;
			rect.left = 0;
			rect.right = m_rect.Width();
		}
		
		CBrush br;
		br.CreateSolidBrush(crEnd);
		CBrush *lbr = pDC->SelectObject(&br);
		CPen pen;
		pen.CreatePen(PS_SOLID , 0, crEnd);
		CPen *lodp = pDC->SelectObject(&pen);

		pDC->Rectangle(rect);
		pDC->SelectObject(lbr);
		pDC->SelectObject(lodp);
		
		pen.DeleteObject();
		br.DeleteObject();
	}
}