
#pragma once

class CIOParam : public CObject
{
public:
	CIOParam();
	CIOParam(CString strName, CString strData);
	CIOParam(CString strName, int nData);

	CIOParam(CString strName, int nSize, enum DataTypeEnum nType);

	~CIOParam();

public:
	enum DataTypeEnum m_nType;

	CString m_strName;
	CString m_strData;
	long    m_nSize;

public:
	void SetData(CString strData);
	void JustShowData();

public:
	BOOL IsSameSame(CIOParam *pIOParam);
	BOOL IsResError();
};

#define STO		"01"
#define RET		"02"
#define RTR		"03"

class CAdoDbIO
{
public:
	CAdoDbIO();
	~CAdoDbIO();

public:
	CString m_strProcName;

	CObList m_listInput;
	CObList m_listOutput;

	CString m_strErrMsg;

public:
	BOOL GetOutputData(CIOParam *pIOParam);
	BOOL GetOutputData(CString strName, CString &strData);

public:
	BOOL SetInputData(CString strName, CString strData);

public:
	BOOL IsNormalJob();

};

