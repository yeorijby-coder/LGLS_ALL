using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.Statistics;

namespace ECP.Device.Observe
{
    public class ObservableStatisticsCollectorData : StatisticsCollectorData
    {
        public const string COLLECTOR_ID = "OBSERVABLE";
        public const string TRIGGER_ID = "OBSERVABLE";

        private const int OBSERVABLE_LENGTH = 25;
        private const int HOSTID_LENGTH = 15;
        private const int TYPE_LENGTH = 15;
        private const int VALUE_LENGTH = 20;

        private string preValue = "";
        private string curValue = "";
        private string observableId ="";
        public string ownerId ="";
        private string dataType ="";
        private string driverNo ="";
        private string connectionString ="";
        private bool isWriteLog;
        private string valueInfoamtion = "";


        public ObservableStatisticsCollectorData(string observableId, string ownerId,
            string dataType, Object preValue, Object curValue, string driverNo, string conString, bool isWriteLog, string valueInfomation)
        {
            this.observableId = observableId;
            this.ownerId = ownerId;
            this.dataType = dataType;
            this.preValue = GetValueInfo(preValue, valueInfomation, dataType);
            this.curValue = GetValueInfo(curValue, valueInfomation, dataType);
            this.driverNo = driverNo;
            this.connectionString = conString;
            this.isWriteLog = isWriteLog;
            this.valueInfoamtion = valueInfomation;
        }
/*
        public ObservableStatisticsCollectorData( string observableId, string ownerId,
            string dataType, string preValue, string curValue, string driverNo, string conString, bool isWriteLog,string valueInfomation)
        {       
            this.observableId = observableId;
            this.ownerId = ownerId;
            this.dataType =dataType;
            this.preValue = GetValueInfo(preValue, valueInfomation);
            this.curValue = GetValueInfo(curValue, valueInfomation);
            this.driverNo = driverNo;
            this.connectionString = conString;
            this.isWriteLog = isWriteLog;
            this.valueInfoamtion = valueInfomation;
        }
*/    
        public override string GetReportedData()
        {
            string observableId = (this.observableId.Length > OBSERVABLE_LENGTH)? this.observableId :  this.observableId.PadRight(OBSERVABLE_LENGTH);
            string ownerId = (this.ownerId.Length > HOSTID_LENGTH) ? this.ownerId : this.ownerId.PadRight(HOSTID_LENGTH);
            string dataType = (this.dataType.Length > TYPE_LENGTH) ? this.dataType : this.dataType.PadRight(TYPE_LENGTH);
            string preValue = (this.preValue.Length > VALUE_LENGTH) ? this.preValue : this.preValue.PadRight(VALUE_LENGTH);
            string curValue = (this.curValue.Length > VALUE_LENGTH) ? this.curValue : this.curValue.PadRight(VALUE_LENGTH);


            return DELIM + ((this.isWriteLog) ? "CIM WRITE>>> " : "CIM READ <<< ") + observableId +
                DELIM + ownerId +
                DELIM + dataType +
                DELIM + preValue +
                DELIM + curValue +
                DELIM +this.driverNo +
                DELIM +this.connectionString; 
        }


    }
}
