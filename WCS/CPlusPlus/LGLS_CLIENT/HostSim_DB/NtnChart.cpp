// NtnChart.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "NtnChart.h"

using namespace Gdiplus;
// CNtnChart

IMPLEMENT_DYNAMIC(CNtnChart, CWnd)

void CNtnChart::Create(const CWnd * const parent,CRect * rect)
{
	CString strWndClass = AfxRegisterWndClass (
		CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,                                     // Class style
		AfxGetApp ()->LoadStandardCursor (IDC_ARROW),   // Class cursor
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),                    // Background brush
		AfxGetApp ()->LoadStandardIcon (IDI_WINLOGO)    // Class icon
		);

	CreateEx (WS_EX_STATICEDGE, strWndClass, _T ("chart"),WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		parent->m_hWnd, NULL);

	SetWindowPos (NULL, rect->left, rect->top, rect->Width (), rect->Height (),SWP_DRAWFRAME|SWP_SHOWWINDOW);	
	m_parent = (CWnd *)parent;

	//변수 디폴트 초기화
	int marginTxtSide = 50;
	int totalMargin = 80;
	int orgLeft = marginTxtSide;
	int orgTop =totalMargin - marginTxtSide;
	m_tmplRect = Rect(orgLeft,orgTop,(int)(rect->Width()-totalMargin),(int)(rect->Height()-totalMargin));

	int dm_xMidPoint =(int)m_tmplRect.Width/m_nXCnt;
	m_barHalfWidth = (int)(dm_xMidPoint*0.8)/2;
	m_xMidPoint[0] =  m_tmplRect.GetLeft() + (int)dm_xMidPoint/2;
	for (int i=1;i<10;i++) {
		m_xMidPoint[i] = m_xMidPoint[i-1] + dm_xMidPoint;
	}
}

CNtnChart::CNtnChart()
{
	m_tmplWidth = 2.0;
	m_tmplMaxY = 140;

	m_tmplYAxisFntSize= 11;
	m_tmplXAxisFntSize =11;
	m_tmplUnitFntSize = 12.5;
	m_tmplSubjectFntSize = 15;
	m_strTickX[0]=L"63";
	m_strTickX[1]=L"125";
	m_strTickX[2]=L"250";
	m_strTickX[3]=L"500";
	m_strTickX[4]=L"1k";
	m_strTickX[5]=L"2k";
	m_strTickX[6]=L"4k";
	m_strTickX[7]=L"8k";
	m_strTickX[8]=L"Over\r\nAll(L)";
	m_strTickX[9]=L"Over\r\nAll(A)";

	m_tmplyUnit = L"dB";
	m_tmplxUnit = L"Octave band center frequency (Hz)";
	m_tmplSubject = L"test test test";
	ZeroMemory(m_data,sizeof(REAL)*10);
}

CNtnChart::~CNtnChart()
{
}


BEGIN_MESSAGE_MAP(CNtnChart, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CNtnChart 메시지 처리기입니다.



void CNtnChart::OnPaint()
{
	using namespace Gdiplus;
	CPaintDC dc(this); // device context for painting
	Graphics graphics(dc.m_hDC);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentCenter);
	stringFormat.SetLineAlignment(StringAlignmentCenter);

	//템플릿 그리기
	Pen pen(Color(255, 0, 0, 0),m_tmplWidth);
	REAL dashValues[2] = {2.0f,2.5f};
	Pen pen2(Color(255, 0, 0, 0),1.0);
	pen2.SetDashPattern(dashValues,2);
	graphics.DrawRectangle(&pen,m_tmplRect);

	FontFamily fontFamily(_T("Arial"));
	Gdiplus::Font font(&fontFamily,	(REAL)m_tmplYAxisFntSize, FontStyleRegular, UnitPixel);
	SolidBrush  BlackBrush(Color(255, 0, 0, 0));
	int deltaY=20;
	int deltaYinPixel = (int)(m_tmplRect.Height*deltaY/m_tmplMaxY);
	int yinPixel = m_tmplRect.GetBottom()-deltaYinPixel;
	CStringW yTxt=L"";
	float y=deltaY;
	while (1) {
		graphics.DrawLine(&pen2,m_tmplRect.GetLeft(),yinPixel,m_tmplRect.GetRight(),yinPixel);
		PointF pointF(30.0f, (REAL)yinPixel-m_tmplYAxisFntSize/2.0f);
		yTxt.Format(L"%.0f",y);
		y=y+deltaY;
		if (yTxt.GetLength() > 2) {
			pointF.X-=5;
		}
		graphics.DrawString(yTxt, yTxt.GetLength(), &font, pointF, &BlackBrush);
		yinPixel = yinPixel-deltaYinPixel;
		if(yinPixel < m_tmplRect.GetTop()+deltaYinPixel)
			break;
	}

	Gdiplus::Font font2(&fontFamily,	(REAL)m_tmplUnitFntSize, FontStyleRegular, UnitPixel);

	PointF point(17.0f, (REAL)m_tmplRect.GetTop()-5);
	graphics.DrawString(m_tmplyUnit, m_tmplyUnit.GetLength(), &font2, point, &BlackBrush);

	point =  PointF(180.0f, (REAL)m_tmplRect.GetBottom()+30);
	graphics.DrawString(m_tmplxUnit, m_tmplxUnit.GetLength(), &font2, point, &BlackBrush);

	for (int i=0;i<m_nXCnt;i++) {
		RectF rect(m_xMidPoint[i]-m_barHalfWidth,m_tmplRect.GetBottom()+3
			,m_barHalfWidth*2,23);
		graphics.DrawString(m_strTickX[i], m_strTickX[i].GetLength(), &font, rect, &stringFormat,&BlackBrush);
	}

	Gdiplus::Font font3(&fontFamily,	(REAL)m_tmplSubjectFntSize, FontStyleBold, UnitPixel);
	CRect wndRect;
	this->GetWindowRect(wndRect);
	RectF rect(0,0,wndRect.Width(),30);
	graphics.DrawString(m_tmplSubject, m_tmplSubject.GetLength(), &font3, rect, &stringFormat,&BlackBrush);

	//막대 그리기
	Rect rect1,rect2;
	for (int i=0;i<8;i++) {
		Rect rect1(m_xMidPoint[i]-m_barHalfWidth,m_tmplRect.GetBottom()-(INT)m_data[i]
		,m_barHalfWidth*2,m_data[i]);
		Rect rect2(m_xMidPoint[i]-m_barHalfWidth,m_tmplRect.GetTop()
			,m_barHalfWidth*2,m_tmplRect.Height);
		LinearGradientBrush linGrBrush(
			rect2,
			Color(255, 255, 0, 0),
			Color(255, 0, 0, 255),
			LinearGradientModeVertical);
		graphics.FillRectangle(&linGrBrush, rect1);
	}
	for (int i=8;i<10;i++) {
		Rect rect1(m_xMidPoint[i]-m_barHalfWidth,m_tmplRect.GetBottom()-(INT)m_data[i]
		,m_barHalfWidth*2,m_data[i]);
		Rect rect2(m_xMidPoint[i]-m_barHalfWidth,m_tmplRect.GetTop()
			,m_barHalfWidth*2,m_tmplRect.Height);
		LinearGradientBrush linGrBrush(
			rect2,
			Color(255, 0, 0, 0),
			Color(255, 100, 100, 100),
			LinearGradientModeVertical);
		graphics.FillRectangle(&linGrBrush, rect1);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
}

void CNtnChart::SetSubject(LPCTSTR str)
{
	m_tmplSubject = str;
	Invalidate();
}

void CNtnChart::SetYUnit(LPCTSTR str)
{
	m_tmplyUnit = str;
	Invalidate();
}

void CNtnChart::SetData(REAL * pData)
{

	for (int i=0;i<10;i++) {
		if (pData[i] < 0) { 
			continue;
		}
		if (pData[i] > m_tmplMaxY) {
			pData[i] = m_tmplMaxY;
		}
		m_data[i] = pData[i]*m_tmplRect.Height/m_tmplMaxY;
	}

	Invalidate();
}

void CNtnChart::SetData(REAL * pData, int pnCnt)
{

	for (int i=0;i<pnCnt;i++) {
		if (pData[i] < 0) { 
			continue;
		}
		if (pData[i] > m_tmplMaxY) {
			pData[i] = m_tmplMaxY;
		}
		m_data[i] = pData[i] * m_tmplRect.Height/m_tmplMaxY;
	}

	Invalidate();
}


