using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Statistics;

namespace HECS.Statistics
{
    public class ErrorStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "ALARM";

        public const string TRIGGER_TRK_ID = "ALM";

        private string alarmCode = "";
        private string alarmStatus = "";
        private string alarmDescription = "";
        private string xLogMsg = "";

        public ErrorStatisticsCollectorData(string alarmCode,string alarmStatus, string alarmDescription)
        {
            this.alarmCode = alarmCode;
            this.alarmStatus = alarmStatus;
            this.alarmDescription = alarmDescription;
        }

        public ErrorStatisticsCollectorData(string sMsg)
        {
            this.xLogMsg = sMsg;
        }

        public override string GetReportedData()
        {
            if (this.xLogMsg == "")
            {
                StringBuilder ret = new StringBuilder();
                ret.Append(DELIM);
                ret.Append(this.alarmCode);
                ret.Append(DELIM);
                ret.Append(this.alarmStatus);
                ret.Append(DELIM);
                ret.Append(this.alarmDescription);
                return ret.ToString();
            }

            return this.xLogMsg;
        }        
    }
}
