// DciButtonCtrl.h: interface for the CDciButtonCtrl class.
//

//#undef AFX_DATA
//#define AFX_DATA AFX_EXT_DATA

#pragma once

#include "DciControl.h"


class AFX_EXT_CLASS CDciButtonCtrl : public CDciControl
{
	DECLARE_SERIAL(CDciButtonCtrl)

public:
	CDciButtonCtrl(void);
	virtual ~CDciButtonCtrl(void);

public:
	enum { enButtonCtrlPropSize = CDciControl::enControlPropSize + 2 };
	//==> 삼각형 4방향 추가 - 최원빈 수정
	enum EN_BTNCTRL_SHAPE {	
		enBcsRectangle, enBcsEllipse, enBcsRoundRect,																													//  0,  1,  2
		enBcsRectangleToLeft, enBcsRectangleToRight, enBcsRectangleBoLeft, enBcsRectangleBoRight,																		//  3,  4,  5,  6 
		enBcsIsoscelesTriangleToLeft, enBcsIsoscelesTriangleToRight, enBcsIsoscelesTriangleToTop, enBcsIsoscelesTriangleToBottom,										//  7,  8,  9, 10
		enBcsDoubleIsoscelesTriangleToLeft, enBcsDoubleIsoscelesTriangleToRight, enBcsDoubleIsoscelesTriangleToTop, enBcsDoubleIsoscelesTriangleToBottom,				// 11, 12, 13, 14
		enBcsTripleIsoscelesTriangleToLeft, enBcsTripleIsoscelesTriangleToRight, enBcsTripleIsoscelesTriangleToTop, enBcsTripleIsoscelesTriangleToBottom,				// 15, 16, 17, 18
		enBcsQuadrupleIsoscelesTriangleToLeft, enBcsQuadrupleIsoscelesTriangleToRight, enBcsQuadrupleIsoscelesTriangleToTop, enBcsQuadrupleIsoscelesTriangleToBottom,	// 19, 20, 21, 22
		enBcsSize};
	//==<

public:
	int m_nFontSize;
	int m_nShape;

public:
	virtual void InitControl(CDciMaster* pDCI);
	virtual void UpdateControl(CDC* pDC);

public:
	virtual int UpdatePropNames(CDciPropertyArray& properties);
	virtual int UpdatePropValues(CDciPropertyArray& properties, BOOL bSaveObject = TRUE);
	virtual int GetPropSize() { return enButtonCtrlPropSize; }
};

//#undef AFX_DATA
//#define AFX_DATA

