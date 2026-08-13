using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;

using ECP.Service.Logger;
using ECP.Device;
using ECP.Db;
using ECP.Global.Device;
using ECP.Service.Material;
using ECP.Util.Client;
using ECP.Service.Statistics;

using HECS.Device;
 
 
using HECS.Global;
using HECS.Statistics;


namespace HECS.Device.Dispatcher
{
    /// <summary>
    /// 
    /// </summary>
    public class TransferWorkItem : Persistence
    {
        /// <summary>
        /// CLASS명
        /// </summary>
        private const string CLASS = "TransferWorkItem";  

        #region Attribute

        private bool dirtyFlag = true;

        /// <summary>
        /// 반송명령 ID
        /// </summary>
        private string commandID;
        public string CommandID
        {
            get { return commandID; }
            set { commandID = value; }
        }

        /// <summary>
        /// MCS 우선순위.
        /// </summary>
        private int priority;
        public int Priority
        {
            get { return priority; }
            set { priority = value; }
        }

        /// <summary>
        /// CST ID - PLC에 WRITE되는 12자리 CSTID
        /// </summary>
        private string carrierID;
        public string CarrierID
        {
            get { return carrierID; }
            set { carrierID = value; }
        }

        /// <summary>
        /// CST ID 확장 - MCS에서 전송받은 CST ID가 12자리보다 클 경우 
        /// CST ID는 HSTXXX.. 로 사용하고 전송받은 아이디는 carrierID64에 저장.
        /// </summary>
        private string carrierID64;
        public string CarrierID64
        {
            get { return carrierID64; }
            set { carrierID64 = value; }
        }

        /// <summary>
        /// MCS에서 전송받은 반송 시작 위치.
        /// </summary>
        private string source;
        public string Source
        {
            get { return source; }
            set { source = value; }
        }

        /// <summary>
        /// MCS에서 전송받은 반송 목적 위치.
        /// </summary>
        private string dest;
        public string Dest
        {
            get { return dest; }
            set { dest = value; }
        }


        /// <summary>
        /// 반송되는 CARRIER의 위치 정보.
        /// 해당 대상 장비의 Element ID
        /// </summary>
        private string carrierLoc;
        public string CarrierLoc
        {
            get { return carrierLoc; }
            set { carrierLoc = value; }
        }

        /// <summary>
        /// 반송되는 CARRIER의 위치 정보를 저장.
        /// 해당 대상 장비의 Element ID의 버퍼(STOCKER PORT인 경우) 위치.
        /// </summary>
        private string carrierSubLoc;
        public string CarrierSubLoc
        {
            get { return carrierSubLoc; }
            set { carrierSubLoc = value; }
        }

        /// <summary>
        /// MCS에서 전송(또는 사용자가 입력)한 LOT ID
        /// </summary>
        private string lotID;
        public string LotID
        {
            get { return lotID; }
            set { lotID = value; }
        }

        /// <summary>
        /// CARRIER TYPE
        /// </summary>
        private CarrierType carrierType;
        public CarrierType CarrierType
        {
            get { return carrierType; }
            set { carrierType = value; }
        }

        /// <summary>
        /// 반송상태
        /// </summary>
        private TransferState transferState;
        public TransferState TransferState
        {
            get { return transferState; }
            set { transferHostState = value; }
        }

        /// <summary>
        /// HOST로 보고하는 반송상태.
        /// P8버전에 들어와서 CIM은 반송상태를 좀더 세부적으로 구분하였으나
        /// MCS는 이전 상태를 그대로 유지하여 
        /// CIM에서 MCS로 보고할때 이전 상황에 맞춰서 보고해야 할 필요성에 의해서 작성.
        /// </summary>
        private TransferState transferHostState;
        public TransferState TransferHostState
        {
            get { return transferHostState; }
            set
            {
                transferHostState = value;
            }
        }

        /// <summary>
        /// 이전 반송상태
        /// previousTransferState값과 현재 바뀐 transferState의 비교를 통해서
        /// transferHostState값을 결정하고 보고한다.
        /// </summary>
        private TransferState previousTransferState;
        public TransferState PreviousTransferState
        {
            get { return previousTransferState; }
            set { previousTransferState = value; }
        }

        /// <summary>
        /// 반송 결과.
        /// </summary>
        private TransferResult resultCode;
        public TransferResult ResultCode
        {
            get { return resultCode; }
            set { resultCode = value; }
        }

        /// <summary>
        /// 반송명령이 수신(CREATE)된 시간
        /// </summary>
        private string queuedTime;
        public string QueuedTime
        {
            get { return queuedTime; }
            set { 
                queuedTime = value;
                priorityUpdateTime = value;
            }
        }

        /// <summary>
        /// 반송명령 우선순위가 변경될시 변경한 시간.
        /// </summary>
        private string priorityUpdateTime;
        public string PriorityUpdateTime
        {
            get { return priorityUpdateTime; }
            set { priorityUpdateTime = value; }
        }

        /// <summary>
        /// 반송명령 실행시간 - 반송명령을 받은 RM이 ACTIVE로 상태가 변경된 시간.
        /// </summary>
        private string executeTime;
        public string ExecuteTime
        {
            get { return executeTime; }
            set { executeTime = value; }
        }

        /// <summary>
        /// 반송명령이 완료된 시간 - 반송명령 상태가 COMPLETE로 변경된 시간.
        /// </summary>
        private string completeTime;
        public string CompleteTime
        {
            get { return completeTime; }
            set { completeTime = value; }
        }

        /// <summary>
        /// 반송명령이 RM으로 전송된 횟수.
        /// executeCount 가 10번을 넘기는 경우, 이 반송은 이상이 있는것으로 간주
        /// RM을 DOWN시킨다.
        /// </summary>
        private int executeCount;
        public int ExecuteCount
        {
            get { return executeCount; }
            set { executeCount = value; }
        }

        /// <summary>
        /// 반송명령이 전송되어진 RACKMASTER 의 ELEMENT ID
        /// </summary>
        private string assignedRM;
        public string AssignedRM
        {
            get { return assignedRM; }
            set { assignedRM = value; }
        }

        /// <summary>
        /// 현재 반송명령이 HAND OFF 반송을 수행할것인지 여부 표시.
        /// true로 SET되어 진다면 plcTo값이 HAND OFF ZONE인 SHELF임.
        /// 주의! : DEST가 아님. plcTo값임.
        /// </summary>
        private bool isHandoffTransfer;
        public bool IsHandoffTransfer
        {
            get { return isHandoffTransfer; }
            set { isHandoffTransfer = value; }
        }

        /// <summary>
        /// 현재 반송이 ALT상태인지 여부.
        /// </summary>
        private bool isAlternativeTransfer;
        public bool IsAlternativeTransfer
        {
            get { return isAlternativeTransfer; }
            set { isAlternativeTransfer = value; }
        }
                
        /// <summary>
        /// NEXT LOCATION에 대한 정보.
        /// T형 CNV(분기형 CNV)의 PORT내의 분기 목적지.
        /// </summary>
        private string nextLocation;
        public string NextLocation
        {
            get { return nextLocation; }
            set { nextLocation = value; }
        }

        /// <summary>
        /// 사용안함.
        /// </summary>
        private int newPriority=0;
        public int NewPriority
        {
            get { return newPriority; }
            set { newPriority = value; }
        }

        /// <summary>
        /// RM으로 전송된 FROM PLC 주소.
        /// </summary>
        private int plcFROM = 0;
        public int PlcFROM
        {
            get { return plcFROM; }
            set { plcFROM = value; }
        }

        /// <summary>
        /// RM으로 전송된 TO PLC 주소.
        /// </summary>
        private int plcTO = 0;
        public int PlcTO
        {
            get { return plcTO; }
            set { plcTO = value; }
        }

        /// <summary>
        ///overlap - Entry Request시도한 마지막 시간.
        /// </summary>
        private DateTime entryTryTime = DateTime.Now;
        public DateTime EntryTryTime
        {
            get { return entryTryTime; }
            set { entryTryTime = value; }
        }

        /// <summary>
        /// 필터 조건에 의해 계산된 계산가중치.
        /// </summary>
        private int calculatedPriority = 0;
        public int CalculatedPriority
        {
            get { return calculatedPriority; }
            set { calculatedPriority = value; }
        }

        /// <summary>
        /// 반송 불가능 사유
        /// </summary>
        private string description = "";
        public string Description
        {
            get { return description; }
            set { description = value; }
        }

        #endregion Attribute

        

        /// <summary>
        /// 객체생성자.
        /// </summary>
        public TransferWorkItem()
        { 
        }

        
        /// <summary>
        /// Queued된 시점과 호출되어진 시점과의 시간차이(분)을 반환.
        /// </summary>
        /// <returns>Queued된 시점과 호출되어진 시점과의 시간차이(분)</returns>
        public int GetWaitTime()
        {
            int ret;
            try{
                ret = Util.Util.GetGapTime1toNow(Util.Util.DateFormat.MIN, this.queuedTime);
            }
            catch {
                ret = 0;
            }
            return ret;
        }

    }

    public class WorkItemComparer : IComparer<TransferWorkItem>
    {
        /// <summary>
        /// 반송명령 LIST의 정렬을 위해 구현된 IComparer 인터페이스.
        /// 이 비교 인터페이스를 통해서 SORTER를 통해 계산되어진
        /// 계산가중치값을 기준으로 WORKLIST를 정렬한다.
        /// </summary>
        /// <param name="x">반송명령의 계산가중치값.</param>
        /// <param name="y">반송명령의 계산가중치값.</param>
        /// <returns></returns>
        public int Compare(TransferWorkItem x, TransferWorkItem y)
        {
            if (x == null)
            {
                if (y == null)
                {
                    // If x is null and y is null, they're
                    // equal. 
                    return 0;
                }
                else
                {
                    // If x is null and y is not null, y
                    // is greater. 
                    return 1;
                }
            }
            else
            {
                // If x is not null...
                //
                if (y == null)
                // ...and y is null, x is greater.
                {
                    return -1;
                }
                else
                {
                    // ...and y is not null, compare the 
                    // priority of the two workItem.
                    //

                    if (x.CalculatedPriority < y.CalculatedPriority)
                    {
                        return 1;
                    }
                    else if (x.CalculatedPriority > y.CalculatedPriority)
                    {
                        return -1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            }
            return -1;
        }  
    }
}
