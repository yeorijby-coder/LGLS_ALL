#pragma once

using namespace Gdiplus;
class CNtnChart :
	public CWnd
{
	DECLARE_DYNAMIC(CNtnChart)
	void Create(const CWnd * const parent,CRect * rect);
public:
	CNtnChart();
	virtual ~CNtnChart();
	CWnd * m_parent;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void SetData(REAL * pData);
	void SetData(REAL * pData, int pnCnt);
	void SetSubject(LPCWSTR str);
	void SetYUnit(LPCWSTR str);

	int m_nXCnt;

protected:
	//템플릿에 대한 변수
	int m_tmplYAxisFntSize;
	int m_tmplXAxisFntSize;
	REAL m_tmplWidth; //외곽선 굵기
	Rect m_tmplRect;
	int m_tmplnTickX;
	int m_tmplnTickY;
	REAL m_tmplMaxY;
	CStringW m_strTickX[10];//x 값이 증가 할때마다 밑에 쓰는 문자열 현재는 10개지만 나중에 변화시킬수 있도록
	int m_tmplUnitFntSize;
	CStringW m_tmplxUnit;
	CStringW m_tmplyUnit;
	int m_tmplSubjectFntSize;
	CStringW m_tmplSubject;

	//막대그래프관련
	int m_xMidPoint[10];//막대그래프 중심의 x좌표값 픽셀 코디네이트에서임.
	int m_barHalfWidth;//막대 반폭

	REAL m_data[10];//픽셀 코디네이트엣 순수한 막대의 길이임 끝점이 좌표가 아님.

};

