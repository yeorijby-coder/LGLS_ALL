using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Material;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Db.Entity;

namespace ECP.BizRule
{
    [Serializable]
    public class FindLocationData : ECPObject 
    {
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string fromdeviceid = "";

        public string Fromdeviceid
        {
            get { return fromdeviceid; }
            set { fromdeviceid = value; }
        }
        private string fromlocationid = "";

        public string Fromlocationid
        {
            get { return fromlocationid; }
            set { fromlocationid = value; }
        }
        private string fromsublocationid = "";

        public string Fromsublocationid
        {
            get { return fromsublocationid; }
            set { fromsublocationid = value; }
        }
        private string todeviceid = "";

        public string Todeviceid
        {
            get { return todeviceid; }
            set { todeviceid = value; }
        }
        private string tolocationid = "";

        public string Tolocationid
        {
            get { return tolocationid; }
            set { tolocationid = value; }
        }
        private string tosublocationid = "";

        public string Tosublocationid
        {
            get { return tosublocationid; }
            set { tosublocationid = value; }
        }

        private string checkqty = "";

        public string Checkqty
        {
            get { return checkqty; }
            set { checkqty = value; }
        }

        public FindLocationData()
        {
        }

        public void getFindLocationData(TransferData td)
        {
            //SAP 전체 반송 경로를 가져옴.
            List<PositionData> fromList = new List<PositionData>();
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();
            CodeMasterData codeMasterData = null;

            try
            {
                //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
                fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, td.Fromposition, td.Sampleflag);
                toList = data.getPositionDataList(GlobalConstant.TO_TEXT, td.Toposition, "");

                Dictionary<string, HirackData> dicHirack = new Dictionary<string, HirackData>();

                //입고의 경우 FROM 위치는 하나로 정의됨.
                foreach (PositionData listTemp in fromList)
                {
                    this.Fromdeviceid = listTemp.Deviceid;
                    this.Fromlocationid = listTemp.Locationid;
                    this.Fromsublocationid = listTemp.Sublocationid;
                }

                //특정 자재에 대한 적재 위치 결정
                MaterialData mData = new MaterialData();

                UDPropertyData udpropertyMaterialData = new UDPropertyData();
                string strPlantcode = GlobalConstant.PLANT_CODE;
                string strEntityid = td.Materialno;
                string strUdpropertycode = GlobalConstant.BIG_SIZE_LOCATION;

                int iRow = udpropertyMaterialData.getUdpropertyValue(strPlantcode, strEntityid, strUdpropertycode);

                if (iRow != 0)
                {
                    HirackData hdLargeData = new HirackData();

                    //적치 그룹 가져 오기
                    udpropertyMaterialData = new UDPropertyData();
                    strPlantcode = GlobalConstant.PLANT_CODE;
                    strEntityid = td.Materialno;
                    strUdpropertycode = GlobalConstant.PRIORITY_LOCATION;

                    iRow = udpropertyMaterialData.getUdpropertyValue(strPlantcode, strEntityid, strUdpropertycode);

                    CodeMasterData cmData = new CodeMasterData();
                    if (iRow != 0)
                    {
                        cmData.getLocationConditionGroupCode(udpropertyMaterialData.Value);
                    }
                    else
                    {
                        cmData.getLocationConditionMaxGroupCode();
                    }

                    hdLargeData.getHirackIngoLargeData(cmData);

                    foreach (PositionData listTemp in toList)
                    {
                        if (listTemp.Locationid == GlobalConstant.BIG_SIZE_RACK)
                        {
                            this.Todeviceid = listTemp.Deviceid;
                            this.Tolocationid = listTemp.Locationid;
                            this.Tosublocationid = hdLargeData.Cellid;

                            break;
                        }
                    }
                }
                else
                {
                    foreach (PositionData listTemp in toList)
                    {
                        HirackData hdIngoData = new HirackData();
                        mData = new MaterialData();
                        mData.getMaterialData(td.Materialno);

                        //적치 그룹 가져 오기
                        udpropertyMaterialData = new UDPropertyData();
                        strPlantcode = GlobalConstant.PLANT_CODE;
                        strEntityid = td.Materialno;
                        strUdpropertycode = GlobalConstant.PRIORITY_LOCATION;

                        iRow = udpropertyMaterialData.getUdpropertyValue(strPlantcode, strEntityid, strUdpropertycode);

                        CodeMasterData cmData = new CodeMasterData();
                        if (iRow != 0)
                        {
                            cmData.getLocationConditionGroupCode(udpropertyMaterialData.Value);
                        }
                        else
                        {
                            cmData.getLocationConditionMaxGroupCode();
                        }

                        //온도 조건, 포장재, 원료 조건 위치 정보 조회
                        int iResult = 0;
                        codeMasterData = new CodeMasterData();

                        if (mData.Tempconditionflg == "Y")
                        {
                            iResult = codeMasterData.getLocationConditionCode(mData.Tempconditionflg);
                        }
                        else
                        {
                            iResult = codeMasterData.getLocationConditionCode(mData.Materialtype);
                        }

                        if (iResult == 0)
                        {
                            hdIngoData.getHirackIngoData(listTemp, cmData);
                        }
                        else
                        {
                            hdIngoData.getHirackIngoLocationConditionData(listTemp, codeMasterData, cmData);
                        }

                        if ((hdIngoData != null) && !(string.IsNullOrEmpty(hdIngoData.Cellid)))
                        {
                            dicHirack.Add(listTemp.Locationid, hdIngoData);
                        }
                    }

                    //Rack을 Random하게 선택함. (순차진행으로 변경 08.15)
 //                   Random rd = new Random();
                    HirackData hdata = null;
                    string randomRack = "";

                    codeMasterData = new CodeMasterData();
                    codeMasterData.getRandomValueCode(this.GetType().Name);

                    CodeMasterData craneCheck = new CodeMasterData();
                    List<CodeMasterData> CranList = craneCheck.getNormalCraneInfo();

                    Boolean loofFlag = true;
                    int iRandomCnt = 0;

                    while (loofFlag)
                    {
                        //int iValue = rd.Next(int.Parse(codeMasterData.Attribute1), int.Parse(codeMasterData.Attribute2));
                        //randomRack = GlobalConstant.SUBSYSTEM_RACK + iValue.ToString();

                        //////////////////////////////////전체 Rack 사용 시, 소스 변경 필요///////////
                        int iNext  = 0;
                        int iValue = int.Parse(GlobalConstant.START_RACK);
                        int iRackValue = iValue % 8;

                        if (iRackValue == 0) {
                            iRackValue = int.Parse(codeMasterData.Attribute2) - 1;
                            iNext = int.Parse(codeMasterData.Attribute1);
                            GlobalConstant.START_RACK = iNext.ToString();
                        }
                        else 
                        {
                            iNext = iRackValue + 1;
                            GlobalConstant.START_RACK = iNext.ToString();
                        }
                        randomRack = GlobalConstant.SUBSYSTEM_RACK + iRackValue.ToString();
                        //////////////////////////////////////////////////////////////////////
                        
                        //2009.12.10 rack 상태 check
                        Boolean craneFlag = false;
                        foreach (CodeMasterData codeTemp in CranList)
                        {
                            if ((codeTemp.Attribute2 == randomRack) || (codeTemp.Attribute3 == randomRack))
                            {
                                craneFlag = true;
                                break;
                            }
                        }

                        if (craneFlag)
                        {
                            if (dicHirack.ContainsKey(randomRack))
                            {
                                hdata = (HirackData)dicHirack[randomRack];
                                loofFlag = false;
                                break;
                            }
                            else
                            {
                                loofFlag = true;
                            }
                        }

                        iRandomCnt += 1;

                        if (iRandomCnt > 10)
                        {
                            loofFlag = false;
                            break;
                        }
                    }

                    foreach (PositionData listTemp in toList)
                    {
                        if (listTemp.Locationid == randomRack)
                        {
                            this.Todeviceid = listTemp.Deviceid;
                            this.Tolocationid = listTemp.Locationid;

                            if ((hdata != null) && !(string.IsNullOrEmpty(hdata.Cellid)))
                            {
                                this.Tosublocationid = hdata.Cellid;
                            }
                            else
                            {
                                this.Tosublocationid = "";
                            }

                            break;
                        }
                    }
                }

                //Hirack이 꽉 찼을 경우 Error 메시지
                if (string.IsNullOrEmpty(this.Tosublocationid))
                {
                    throw new Exception("(요청번호:" + td.Commandid + ")의 적치 공간이 없습니다.");
                }
            }
            catch (Exception se)
            {
                throw se;
            }
        }

        public void getSapMoveFindLocationData(TransferData td)
        {
            //SAP 전체 반송 경로를 가져옴.
            List<PositionData> fromList = new List<PositionData>();
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();
            CodeMasterData codeMasterData = null;

            try
            {
                //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
                fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, td.Fromposition, td.Sampleflag);
                toList = data.getPositionDataList(GlobalConstant.TO_TEXT, td.Toposition, "");

                Dictionary<string, HirackData> dicHirack = new Dictionary<string, HirackData>();

                //SAP 이동의 경우 FROM 위치는 하나로 정의됨.
                foreach (PositionData listTemp in fromList)
                {
                    this.Fromdeviceid = listTemp.Deviceid;
                    this.Fromlocationid = listTemp.Locationid;
                    this.Fromsublocationid = listTemp.Sublocationid;
                }

                //SAP 이동의 경우 FROM 위치는 하나로 정의됨.
                foreach (PositionData listTemp in toList)
                {
                    this.Todeviceid = listTemp.Deviceid;
                    this.Tolocationid = listTemp.Locationid;
                    this.Tosublocationid = listTemp.Sublocationid;
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
        }
    }
}
