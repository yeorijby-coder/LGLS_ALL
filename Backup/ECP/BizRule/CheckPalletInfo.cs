using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using System.Data.SqlClient;
using ECP.Global;

namespace ECP.BizRule
{
    //Return Value
    //00 : 성공
    //01 : Pallet이 존재하지 않을 경우
    //02 : Pallet의 상태가 맞지 않을 경우
    //03 : Pallet의 Batch 번호가 맞지 않을 경우
    //04 : Pallet의 Material 번호가 맞지 않을 경우
    //99 : 기타 오류
    [Serializable]
    public class CheckPalletInfo : ECPObject
    {
        public string checkIngoPallet(string palletid, string batchno, string materialno)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Empty.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        checkFlag = "00";
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }

        public string checkOutgoPallet(string palletid, string batchno, string materialno)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Assigned.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        if (palletData.Batchno != batchno)
                        {
                            checkFlag = "03";
                        }
                        else if (palletData.Materialno != materialno)
                        {
                            checkFlag = "04";
                        }
                        else
                        {
                            checkFlag = "00";
                        }
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }

        public string checkOutgoPallet(string palletid)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Assigned.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        checkFlag = "00";
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }

        public string checkMovePallet(string palletid)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Empty.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        checkFlag = "00";
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }

        public string checkMovePalletRackToRack(string palletid)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Assigned.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        checkFlag = "00";
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }

        public string checkSapMovementPallet(string palletid, string batchno, string materialno)
        {
            string checkFlag = "00";
            PalletData palletData = new PalletData();

            try
            {
                palletData.getPalletData(palletid);

                if (string.IsNullOrEmpty(palletData.Palletid))
                {
                    checkFlag = "01";
                }
                else
                {
                    if (palletData.Status != EntityEnumData.PALLETSTATUS.Empty.ToString())
                    {
                        checkFlag = "02";
                    }
                    else
                    {
                        checkFlag = "00";
                    }
                }
            }
            catch (SqlException se)
            {
                checkFlag = "99";
            }

            return checkFlag;
        }
    }
}
