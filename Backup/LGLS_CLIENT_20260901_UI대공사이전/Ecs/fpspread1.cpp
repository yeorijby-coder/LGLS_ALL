// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오. Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.


#include "stdafx.h"
#include "fpspread1.h"

/////////////////////////////////////////////////////////////////////////////
// CFpspread1

IMPLEMENT_DYNCREATE(CFpspread1, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CFpspread1 속성입니다.

/////////////////////////////////////////////////////////////////////////////
// CFpspread1 작업입니다.

// [LGLS] 헤더(컬럼) 클릭 시 해당 열 기준 전체 행 정렬 (재클릭 시 오름/내림 토글)
void CFpspread1::SortByHeaderColumn(long nCol)
{
	long maxCol = GetMaxCols();
	long maxRow = GetMaxRows();
	if(maxRow < 2 || nCol < 1 || nCol > maxCol) return;
	if(m_nLastSortCol == nCol) m_bLastSortAsc = !m_bLastSortAsc;
	else { m_nLastSortCol = nCol; m_bLastSortAsc = TRUE; }
	COleVariant vKeys((long)nCol);
	COleVariant vOrders((long)(m_bLastSortAsc ? 1 : 2));
	Sort(1, 1, maxCol, maxRow, 0 /*SortByRow*/, (VARIANT*)&vKeys, (VARIANT*)&vOrders);
}
