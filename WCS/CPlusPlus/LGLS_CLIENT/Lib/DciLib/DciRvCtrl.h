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
	enum { enRvCtrlPropSize = CDciControl::enControlPropSize + 8 };
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
//	int m_nCol;
//	int m_nRow;
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
};

//#undef AFX_DATA
//#define AFX_DATA

