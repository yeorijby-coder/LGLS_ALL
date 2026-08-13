using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Statistics;



namespace HECS.Statistics
{
    public class OperationStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "OPR";

        public const string TRIGGER_APP_ID = "APP";
        public const string TRIGGER_EXP_ID = "EXP";
        public const string TRIGGER_TO_ID = "T.O";
        public const string TRIGGER_TRQ_ID = "TR.Q";
        public const string TRIGGER_CDB_ID = "CDB";
        public const string TRIGGER_BCR_ID = "BCR";
        public const string TRIGGER_HOST_ID = "HOST";

        private string equipment = "";
        private string msgName = "";
        private string data = "";
        private string xLogMsg = "";

        public OperationStatisticsCollectorData(string xMsg)
        {
            this.xLogMsg = xMsg;
        }

      

        public OperationStatisticsCollectorData(string equipment, string msgName,
            string data)
        {
            this.equipment = equipment;
            this.msgName = msgName;
            this.data = data;

            this.xLogMsg = "";
        }

    


        public override string GetReportedData()
        {
            if (this.xLogMsg == "")
            {
                StringBuilder ret = new StringBuilder();
                ret.Append(DELIM);
                ret.Append(this.equipment);
                ret.Append(DELIM);
                ret.Append(this.msgName);
                ret.Append(DELIM);
                ret.Append(this.data);

                return ret.ToString();
            }

            return this.xLogMsg;
        }
    }
}
