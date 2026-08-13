using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Statistics;

namespace HECS.Statistics
{
    class RunTimeExceptionStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "RUN_TIME_EXCEPTION";
        public const string TRIGGER_UI_EXCEPTION = "UI_EXCEPTION";     // UI Exception Log 처리
        public const string TRIGGER_CORE_EXCEPTION = "CORE_EXCETPION"; // CORE Exception Log 처리

        private string className = "";
        private string functionName = "";
        private string keyValue = "";
        private string excetion = "";
        private string stactraceException = "";

        public RunTimeExceptionStatisticsCollectorData(string className, string functionName,
            string keyValue, string excetion, string stactraceException)
        {
            this.className = className;
            this.functionName = functionName;
            this.keyValue = keyValue;
            this.excetion = excetion;
            this.stactraceException = stactraceException;
        }

        public override string GetReportedData()
        {
            return DELIM + this.className +
                DELIM + this.functionName +
                DELIM + this.keyValue +
                DELIM + this.excetion +
                DELIM + this.stactraceException;
        }        

    }
}
