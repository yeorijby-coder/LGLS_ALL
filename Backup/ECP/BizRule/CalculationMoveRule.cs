using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db.Entity;
using ECP.Global;

namespace ECP.BizRule
{
    [Serializable]
    public class CalculationMoveRule : ECPObject 
    {

        public CalculationMoveRule()
        {
        }

        public Dictionary<string, TransferData> getTransferDataPriorityList(Dictionary<string, TransferData> dictionary)
        {
            List<string[]> list = new List<string[]>();
            Dictionary<string, TransferData> newDictionary = new Dictionary<string,TransferData>();

            foreach(TransferData tempTransfer in dictionary.Values) 
            {
                newDictionary.Add(tempTransfer.Commandid, tempTransfer);
            }

            MoveRuleData moveData = new MoveRuleData();
            Dictionary<string, MoveRuleData> dicMoveRuleList = moveData.getMoveRuleDataList();

            double dTransferOrder = newDictionary.Count + 1;

            foreach (TransferData transferTemp in newDictionary.Values)
            {
                double dTotalPriority = 0;
                double dTransferPriority = 0;
                double dLocationPriority = 0;
                double dDelayPriority = 0;

                //0. Que에 존재하는 Order 순서대로 순번 매기기
                dTransferOrder = dTransferOrder - 1;

                // 1. 반송 우선순위 값 가져오기
                dTransferPriority = double.Parse(transferTemp.Priority);

                // 2. Location 가중치 가져오기
                CodeMasterData codeMasterData = new CodeMasterData();

                if (transferTemp.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                {
                    codeMasterData.getPositionPriorityValue(transferTemp.Fromposition);
                    dLocationPriority = double.Parse(codeMasterData.Attribute3);
                }
                else if (transferTemp.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    codeMasterData.getPositionPriorityValue(transferTemp.Toposition);
                    dLocationPriority = double.Parse(codeMasterData.Attribute4);
                }
                else
                {
                    codeMasterData.getPositionPriorityValue(transferTemp.Fromposition);
                    dLocationPriority = double.Parse(codeMasterData.Attribute5);
                }
                // 3. 지연 반송 가져오기
                string createTimeDisplay = transferTemp.Createtime.Substring(0, 14);

                createTimeDisplay = createTimeDisplay.Substring(0, 4) + "." +
                                    createTimeDisplay.Substring(4, 2) + "." +
                                    createTimeDisplay.Substring(6, 2) + " " +
                                    createTimeDisplay.Substring(8, 2) + ":" +
                                    createTimeDisplay.Substring(10, 2) + ":" +
                                    createTimeDisplay.Substring(12, 2);

                DateTime startTime = DateTime.Parse(createTimeDisplay);
                DateTime nowTime = DateTime.Now;

                //UDPropertyData udpropertyData = new UDPropertyData();
                //udpropertyData.getUdpropertyValue(GlobalConstant.PLANT_CODE,
                //                                  transferTemp.Movementtype,
                //                                  GlobalConstant.SUMMER_TIME_CODE);

                //TimeSpan summerTime = TimeSpan.FromHours(int.Parse(udpropertyData.Value));
                //DateTime koreacTime = startTime.Add(summerTime);
                //TimeSpan timeSpan = nowTime.Subtract(koreacTime);
                TimeSpan timeSpan = nowTime.Subtract(startTime);

                double delaytime = timeSpan.Days * 24 * 60 +
                                   timeSpan.Hours * 60 +
                                   timeSpan.Minutes;

                if (double.Parse(delaytime.ToString()) >= double.Parse(dicMoveRuleList[GlobalConstant.DELAY_RULE].Rulevalue))
                {
                    if (string.IsNullOrEmpty(transferTemp.Executetime))
                    {
                        dDelayPriority = double.Parse(delaytime.ToString());
                    }
                    else
                    {
                        dDelayPriority = double.Parse(dicMoveRuleList[GlobalConstant.DELAY_RULE].Rulevalue) - 1;
                    }
                }
                else
                {
                    dDelayPriority = double.Parse(dicMoveRuleList[GlobalConstant.DELAY_RULE].Rulevalue) - 1;
                }

                // 4. 반송 Priority 총합 구하기
                dTransferPriority = dTransferPriority * double.Parse(dicMoveRuleList[GlobalConstant.PRIORITY_RULE].Priority);
                dLocationPriority = dLocationPriority * double.Parse(dicMoveRuleList[GlobalConstant.LOCATION_RULE].Priority);
                dDelayPriority = (dDelayPriority - double.Parse(dicMoveRuleList[GlobalConstant.DELAY_RULE].Rulevalue) - 1) *
                                 double.Parse(dicMoveRuleList[GlobalConstant.DELAY_RULE].Priority);
                dTotalPriority = dTransferOrder + dTransferPriority + dLocationPriority + dDelayPriority;

                
                string[] strArray = new string[2];
                strArray[0] = transferTemp.Commandid;
                strArray[1] = dTotalPriority.ToString();

                list.Add(strArray);
            }

            Dictionary<string, TransferData> sortedList = new Dictionary<string, TransferData>();
            while (list.Count != 0)
            {
                double maxValue = 0;
                double TempValue = 0;

                int iRow = 0;
                int iDeleteRow = 0;

                foreach (string[] strTemp in list)
                {
                    double priorityValue = double.Parse(strTemp[1]);
                    if (TempValue <= priorityValue)
                    {
                        maxValue = priorityValue;
                        TempValue = priorityValue;
                        iDeleteRow = iRow;
                    }
                    iRow += 1;
                }

                newDictionary[list[iDeleteRow][0]].Newpriority = maxValue.ToString();
                sortedList.Add(list[iDeleteRow][0], (TransferData)newDictionary[list[iDeleteRow][0]]);
                list.Remove(list[iDeleteRow]);
            }

            //반송 순위 Sorting 하기
            return sortedList;
        }
    }
}
