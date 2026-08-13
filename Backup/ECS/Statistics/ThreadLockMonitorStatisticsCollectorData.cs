using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Statistics;

namespace HECS.Statistics
{
    class ThreadLockMonitorStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "LOCK_MONITOR";
        public const string TRIGGER_LOCK = "LOCK_MONITOR";

        private string message = "";

        public ThreadLockMonitorStatisticsCollectorData(string msg)
        {
            this.message = msg;
        }

        public override string GetReportedData()
        {
            return this.message;
        }

    }
}
