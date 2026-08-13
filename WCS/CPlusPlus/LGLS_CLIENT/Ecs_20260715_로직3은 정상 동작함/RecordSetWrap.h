
#include "StdAfx.h"
#include "Ecs.h"
#include "EcsDoc.h"

class CRecordSetWrap
{
public:
	CRecordSetWrap(_RecordsetPtr pRecordSet);
	~CRecordSetWrap();

public:
	_RecordsetPtr m_pRecordSet;

public:
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveFirst();
	CString GetItem(CString strFiledName);
	CString GetColumn(CString strFiledName);
	
};

