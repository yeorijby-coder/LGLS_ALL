using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Db.Entity;

namespace ECP.BizRule
{
    [Serializable]
    public class FindPalletData : ECPObject 
    {
        private string plantcode = "";

        public string Plantcode
        {
            get { return plantcode; }
            set { plantcode = value; }
        }
        private string commandid = "";

        public string Commandid
        {
            get { return commandid; }
            set { commandid = value; }
        }
        private string palletid = "";

        public string Palletid
        {
            get { return palletid; }
            set { palletid = value; }
        }
        private string batchno = "";

        public string Batchno
        {
            get { return batchno; }
            set { batchno = value; }
        }
        private string materialno = "";

        public string Materialno
        {
            get { return materialno; }
            set { materialno = value; }
        }
        private string qty = "";

        public string Qty
        {
            get { return qty; }
            set { qty = value; }
        }
        private string sapqty = "";

        public string Sapqty
        {
            get { return sapqty; }
            set { sapqty = value; }
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
        private string createtime = "";

        public string Createtime
        {
            get { return createtime; }
            set { createtime = value; }
        }
        private string createman = "";

        public string Createman
        {
            get { return createman; }
            set { createman = value; }
        }

        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }

        public FindPalletData()
        {
        }

        public List<FindPalletData> getFindPalletDataList(MovementSapIfData moveData)
        {
            List<FindPalletData> list = new List<FindPalletData>();
            List<string> selectedPalletList = new List<string>();
            List<HirackData> hirackDataList = new List<HirackData>();
            List<HirackData> selectedDataList = new List<HirackData>();

            //SAP 전체 반송 경로를 가져옴.
            List<PositionData> fromList = new List<PositionData>();
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();

            //Total 수량 확인
            List<HirackData> sameQtyList = new List<HirackData>();
            decimal decimalTotalQty = 0;
            decimal remainQty = 0;

            try
            {
                //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
                fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, moveData.Frlgort, "");
                toList = data.getPositionDataList(GlobalConstant.TO_TEXT, moveData.Tolgort, "");

                if (!(string.IsNullOrEmpty(moveData.Pallet)))
                {
                    string[] palletList = null;

                    palletList = moveData.Pallet.Split(GlobalConstant.PALLET_DELIMITER);

                    foreach (string palletValue in palletList)
                    {
                        if (!(string.IsNullOrEmpty(palletValue)) && (palletValue != "0000"))
                        {
                            //Pallet Data Check
                            CheckPalletInfo checkPallet = new CheckPalletInfo();
                            string strResult = checkPallet.checkOutgoPallet(palletValue, moveData.Charg, moveData.Matnr);

                            if (strResult == "00")
                            {
                                HirackData hdOutgoData = new HirackData();
                                hdOutgoData.getHirackOutgoPalletData(palletValue, moveData.Frwerks);

                                if (string.IsNullOrEmpty(hdOutgoData.Palletid))
                                {
                                    continue;
                                }
                                else
                                {
                                    if ((hdOutgoData.Batchno == moveData.Charg) &&
                                        (hdOutgoData.Materialno == moveData.Matnr)
                                        )
                                    {
                                         FindPalletData findPalletData = new FindPalletData();
                                        findPalletData.makeFindPalletData(moveData, fromList, toList, hdOutgoData);
                                        list.Add(findPalletData);

                                        selectedPalletList.Add(palletValue);

                                        decimalTotalQty += decimal.Parse(hdOutgoData.Qty);
                                    }
                                }
                            }
                        }
                    }
                }

                if (decimalTotalQty < decimal.Parse(moveData.Menge))
                {
                    //나머지 잔량으로 Pallet 찾아야 함.
                    remainQty = decimal.Parse(moveData.Menge) - decimalTotalQty;

                    HirackData hdOutgoData = new HirackData();

                    hirackDataList = hdOutgoData.getHirackOutgoDataList(moveData);
                    
                    //선택하지 않은 Pallet List찾기
                    foreach (HirackData hirackDataTemp in hirackDataList)
                    {
                        Boolean checkPalletFlag = true;
                        foreach (string tempPalletid in selectedPalletList)
                        {
                            if (tempPalletid == hirackDataTemp.Palletid)
                            {
                                checkPalletFlag = false;
                                break;
                            }
                        }
                        if (checkPalletFlag)
                        {
                            selectedDataList.Add(hirackDataTemp);
                        }
                    }

                    //수량이 일치하는 Pallet 찾기
                    foreach (HirackData hirackDataTemp in selectedDataList)
                    {
                        if (decimal.Parse(hirackDataTemp.Qty) == remainQty)
                        {
                            sameQtyList.Add(hirackDataTemp);
                        }
                    }

                    //수량 일치 List에서 입고 일자 빠른 것 찾기
                    if (sameQtyList.Count != 0)
                    {
                        string startCreatetime = "";
                        HirackData hdData = new HirackData();

                        int iCount = 0;
                        foreach (HirackData hirackDataTemp in sameQtyList)
                        {
                            iCount += 1;
                            if (iCount == 1)
                            {
                                startCreatetime = hirackDataTemp.Createtime;

                                if (string.IsNullOrEmpty(startCreatetime))
                                {
                                    startCreatetime = "0";
                                }
                                hdData = hirackDataTemp;
                            }
                            else
                            {
                                string tempCreatetime = hirackDataTemp.Createtime;
                                if (string.IsNullOrEmpty(tempCreatetime))
                                {
                                    tempCreatetime = "0";
                                }

                                if (decimal.Parse(startCreatetime) >= decimal.Parse(tempCreatetime))
                                {
                                    startCreatetime = hirackDataTemp.Createtime;
                                    hdData = hirackDataTemp;
                                }
                            }
                        }

                        FindPalletData findPalletData = new FindPalletData();

                        findPalletData.makeFindPalletData(moveData, fromList, toList, hdData);
                        list.Add(findPalletData);
                    }
                    else  //수량이 일치하지 않을 경우는 입고일자 빠르고, 수량이 작은 순으로 (+) 시킴.
                    {
                        //수량은 소수점 3자리까지 가능함.
                        decimal addQty = 0;
                        sameQtyList = new List<HirackData>();

                        foreach (HirackData hirackDataTemp in selectedDataList)
                        {
                            addQty += decimal.Parse(hirackDataTemp.Qty);
                            sameQtyList.Add(hirackDataTemp);

                            if (addQty >= remainQty)
                            {
                                break;
                            }
                        }

                        foreach (HirackData hirackDataTemp in sameQtyList)
                        {
                            FindPalletData findPalletData = new FindPalletData();

                            findPalletData.makeFindPalletData(moveData, fromList, toList, hirackDataTemp);
                            list.Add(findPalletData);
                        }
                    }
                }
            }
            catch (SqlException se)
            {
                throw se;
            }

            return list;
        }

        public void makeFindPalletData(MovementSapIfData moveData,
                                       List<PositionData> fromList,
                                       List<PositionData> toList,
                                       HirackData hdData)
        {
            this.Plantcode = hdData.Plantcode;
            this.Commandid = moveData.Reqno;

            this.Fromposition = moveData.Frlgort;
            this.Toposition = moveData.Tolgort;

            this.Palletid = hdData.Palletid;
            this.Batchno = hdData.Batchno;
            this.Materialno = hdData.Materialno;
            this.Sapqty = moveData.Menge;
            this.Qty = hdData.Qty;

            //출고의 경우 To 위치는 하나로 정의됨.                                     
            foreach (PositionData listTemp in toList)
            {
                this.Todeviceid = listTemp.Deviceid;
                this.Tolocationid = listTemp.Locationid;
                this.Tosublocationid = listTemp.Sublocationid;
            }

            string strLocationCheck = GlobalConstant.SUBSYSTEM_RACK + hdData.Cellid.Substring(1, 1);

            foreach (PositionData listTemp in fromList)
            {
                if (strLocationCheck == listTemp.Locationid)
                {
                    this.Fromdeviceid = listTemp.Deviceid;
                    this.Fromlocationid = listTemp.Locationid;
                    this.Fromsublocationid = hdData.Cellid;

                    break;
                }
            }

            this.Createtime = moveData.Crdat + moveData.Crtim + "001";
            this.Createman = moveData.Rqnam;
        }
    }
}
