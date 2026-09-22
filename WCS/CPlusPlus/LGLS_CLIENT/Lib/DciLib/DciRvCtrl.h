// DciRvCtrl.h: interface for the CDciRvCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciRvCtrl : public CDciControl  
{
	DECLARE_SERIAL(CDciRvCtrl)
		
public:
	CDciRvCtrl(void);
	virtual ~CDciRvCtrl(void);

public:
	enum { enRvCtrlPropSize = CDciControl::enControlPropSize + 10 };
	enum EN_TYPE { enL2R, enR2L, enT2B, enB2T,  enTypeSize };
	enum EN_FORK_TYPE { enSingle, enTwin, enDouble, /*enGap2, */enenForkTypeSize };	// m_nIncrease 사용함으로 enGap2은 사용하지 않음!

protected:
	int m_nType;
	int m_nIncrease;

public:
	int m_nProd;			// 화물유무
	int m_nForkType;
	int m_nForkPos;
	COLORREF m_clrFork;		// 포크 색깔
	COLORREF m_clrFork1;		// 포크 색깔
	COLORREF m_clrFork2;		// 포크 색깔
	COLORREF m_clrWing;		// 날개 색깔
	COLORREF m_clrRail;		// 레일 색깔

	// [LGLS 2026-08-22] 크레인 위 표시 문자(작업번호/호기/제품정보).
	//   CDciTrackCtrl 과 같은 규격 - 갱신 스레드와 그리기 스레드가 CString 을 함께 만지므로 락으로 감싼다.
	CString  m_strExtraText;
	COLORREF m_clrExtraText;
	void    SetExtraTextSafe(LPCTSTR s, COLORREF c);
	CString GetExtraTextSafe(COLORREF* pColor);
//	int m_nFontSize;

private:
	CRect m_rcForkS;	// Single용 포크 그림
	CRect m_rcForkT;	// Twin용   포크 그림	
	CRect m_rcForkD;	// Double용 포크 그림	

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual void UpdateControlVertical(		int nType,
											CPoint & ptRailS1, CPoint & ptRailS2, 
											CRect & rcRailS1, CRect & rcRailS2, 
											CRect & rcWingS1, CRect & rcWingS2, 
											CRect & rcForkL1, CRect & rcForkL2,
											CRect & rcForkS, CRect & rcForkT, CRect & rcForkD,
											int nForkType = 0);
											
	virtual void UpdateControlHorizental(	int nType, 
											CPoint & ptRailS1, CPoint & ptRailS2, 
											CRect & rcRailS1, CRect & rcRailS2, 
											CRect & rcWingS1, CRect & rcWingS2, 
											CRect & rcForkL1, CRect & rcForkL2,
											CRect & rcForkS, CRect & rcForkT, CRect & rcForkD,
											int nForkType = 0);

	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enRvCtrlPropSize; }

	virtual void IndicateProdSensor(CDC * pDC, CRect rect, int ng, int ns, COLORREF nColor=0x000000);

public:
	virtual BOOL SetClick(CWnd* pWnd, const CPoint& ptClickS);

	// [LGLS 2026-09-22] 포크 크기를 격자 칸 수로 지정한다 (사용자 지시).
	//   레이아웃을 3배 격자로 늘려 트랙을 3x2 로 키웠는데, 포크는 짧은 변 기준 정사각형으로만
	//   그려져 크기를 맞출 수 없었다. row=세로 칸, col=가로 칸.
	//   0(기본) 이면 종전 그대로. 포크의 중심은 그대로 두고 크기만 바꾸므로 forkpos 의미는 그대로다.
	//   ※ 멤버를 클래스 맨 뒤에 둔 이유 : 앞에 끼우면 기존 멤버 오프셋이 밀려
	//     예전에 빌드한 Ecs.exe 와 어긋난다(DciLib.dll 만 바꿔 끼울 수 없게 된다).
	int m_nRow;
	int m_nCol;

protected:
	// 포크 ★논리★ 사각형에 row/col 을 적용한다 (윙 계산이 끝난 뒤에 부른다 - 윙은 건드리지 않는다).
	void ApplyForkGridL(CRect& rcForkL);
	// 포크 논리사각형을 보정하고(참조) 화면좌표로 바꿔 돌려준다.
	//   포크를 만든 직후에 부른다 - 뒤따르는 윙 계산이 보정된 포크를 기준으로 삼아 비례한다.
	CRect ForkRectS(CRect& rcForkL);
};

//#undef AFX_DATA
//#define AFX_DATA

