// BcrInfo.h: interface for the CBcrInfo class.
//

#pragma once

#include "info.h"
#include "DciButtonCtrl.h"
#include "TrackInfo.h"
#include "BCR_MST.h"
#include "CV_DATA.h"



class CBcrItem;
class CBcrInfo : public CInfo  
{
	DECLARE_SERIAL(CBcrInfo)

public:
	CBcrInfo() {}
	CBcrInfo(CEquipment* pEquipment);
	virtual ~CBcrInfo();

public:
	enum EN_ERROR_CODE { 
		enErrorNone, 
		enErrorLength, 
		enErrorNoSTX, 
		enErrorNoETX, 
		enErrorData, 
		enErrorNoRead, 
		enErrorInvalidJob,
		enErrorMissmatchIDPID,
		enErrorCommInterface,
		enErrorCommInterface1,
		enErrorCommInterface2,
		enErrorCommInterface3,
		enErrorCommInterface4,
		enErrorCommInterface5,
		enErrorCommunication 
	};
	enum EN_STATION_KIND { enStationKindNo, enStationKindSto, enStationKindRet, enStationKindArv, enStationKindSize };
	enum EN_BCR_KIND { enBcrKindNo, enBcrKindSingle, enBcrKindTwin, enBcrKindTriple, enBcrKindSize };
	enum EN_BCR_STATION { enBcrMagazine, enBcrStoStation, enBcrArvStation, enBcrStationSize };

public:
	CDciButtonCtrl* m_pControl;

public:
	CMap<CString, LPCTSTR, CBCR_MST*, CBCR_MST*> m_MapBCR_MST;
	CBCR_MST* m_pBCR_MST;
	CCV_DATA* m_pCV_DATA;

public:
	virtual void Serialize(CArchive& ar);

public:
	void InvokeControl(CBCR_MST* pBCR_MST, CCV_DATA* pCV_DATA);
	virtual void InvokeControl();
	COLORREF GetColor(CBCR_MST* pBCR_MST, CCV_DATA* pCV_DATA);
	COLORREF GetColor();

public:
	CBCR_MST* CreateBCR_MST( CString pstrPLC_NO, CString pstrBCR_MC_NO );
	CString GetStringPLC_NO();
	CString GetStringPLC_NO(int pnPLC_NO);
	CString GetStringBCR_MC_NO(CString pstrBCR_MC_NO);
};

