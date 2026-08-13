using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Statistics;

namespace HECS.Statistics
{
    public class DebugStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "DEB";

        public const string TRIGGER_TRK_ID = "TRK";
        public const string TRIGGER_CMD_ID = "CMD";
        public const string TRIGGER_TRC_ID = "TRC";
        public const string TRIGGER_REC_ID = "REC";
        public const string TRIGGER_EVT_ID = "EVT";
        public const string TRIGGER_EXCEPTION_ID = "EXP";
        public const string TRIGGER_CDB_ID = "CDB";

        private string messageType = "";
        private string deviceId = "";
        private string description = "";
        private string remark = "";
        private string LogMessage = "";
        //public DebugStatisticsCollectorData(string xMsg)
        //{
        //    this.LogMessage = xMsg;
        //}

        public DebugStatisticsCollectorData(string messageType , string deviceId, string description, string remark)
        {
            this.messageType = messageType;
            this.deviceId = deviceId;
            this.description = description;
            this.remark = remark;
            this.LogMessage = "";
        }

        public override string GetReportedData()
        {
            StringBuilder messageBuffer = new StringBuilder(1024);
            messageBuffer.Append(DELIM);
            messageBuffer.Append(messageType.PadRight(18));
            messageBuffer.Append(DELIM);
            messageBuffer.Append(deviceId.PadRight(14));
            messageBuffer.Append(DELIM);
            messageBuffer.Append(description.PadRight(80));
            messageBuffer.Append (DELIM);
            messageBuffer.Append(remark.PadRight(80));
            this.LogMessage = messageBuffer.ToString();
            return this.LogMessage;
        }        
    }
}
