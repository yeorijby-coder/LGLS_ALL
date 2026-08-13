using System;
using System.Collections.Generic;
using System.Text;

using ECP.Device.Observe;
using ECP.Service.Statistics;
using ECP.Util.Client;
using ECP.Global.Device;


using HECS.Device.Unit;
using HECS.Statistics;
 

namespace HECS.Util
{
    public class LogFormat
    {
        

        public static void AddDebug(string messageType, string deviceId, string description, string remark)
        {
            StatisticsCollector.TriggerOccured(DebugStatisticsCollectorData.COLLECTOR_ID, messageType,
                  new DebugStatisticsCollectorData(messageType, deviceId, description, remark));
        }

        public static void AddAlarm(string messageType, string deviceId, string alarmCode, string alarmId, string alarmText)
        {
        }


        public static void AddOperation(string messageType, string deviceId, string trigger, string value, string description, string remark)
        {
            StatisticsCollector.TriggerOccured(DebugStatisticsCollectorData.COLLECTOR_ID, messageType,
                  new DebugStatisticsCollectorData(messageType, deviceId, description, remark));
        }
        public static void AddServo(string messageType, string deviceId, string errorCode, string errorText, string description)
        {
        }
/*
        /// <summary>
        /// Log Wirte 및 PLC,운영로그,Host로그 UI 반영
        /// </summary>
        /// <param name="xMsgType"></param>
        /// <param name="xTriggerId"></param>
        /// <param name="xMsg"></param>
        public static void AddLog(MessageType xMsgType, string xTriggerId, string xMsg)
        {
            StockerSEM StkSEM = (StockerSEM)Naming.GetDeviceManager().GetDevice();

            if (xMsgType != null)
            {

                switch (xMsgType.Value())
                {

                    case MessageType._CARRIER:
                        StatisticsCollector.TiggerOccured(
                                                          OperationStatisticsCollectorData.COLLECTOR_ID,
                                                          OperationStatisticsCollectorData.TRIGGER_CDB_ID,
                                                          new OperationStatisticsCollectorData(xMsg));
                        break;

                    case MessageType._ERROR:
                        StatisticsCollector.TiggerOccured(
                                                          ErrorStatisticsCollectorData.COLLECTOR_ID,
                                                          ErrorStatisticsCollectorData.TRIGGER_TRK_ID,
                                                          new ErrorStatisticsCollectorData(xMsg));
                        break;

                    case MessageType._HOST:
                        StatisticsCollector.TiggerOccured(
                                                          HostStatisticsCollectorData.COLLECTOR_ID,
                                                          HostStatisticsCollectorData.TRIGGER_TRK_ID,
                                                          new HostStatisticsCollectorData(xMsg));
                        break;

                    case MessageType._OPERATION:
                        StatisticsCollector.TiggerOccured(
                                                          OperationStatisticsCollectorData.COLLECTOR_ID,
                                                          xTriggerId, new OperationStatisticsCollectorData(xMsg));
                        break;

                    case MessageType._TRANSFER:
                        StatisticsCollector.TiggerOccured(
                                                          TransferStatisticsCollectorData.COLLECTOR_ID,
                                                          TransferStatisticsCollectorData.TRIGGER_IHB_ID,
                                                          new TransferStatisticsCollectorData(xMsg));
                        break;

                    case MessageType._SERVO_WARNING:
                        StatisticsCollector.TiggerOccured(
                                                          ServoStatisticsCollectorData.COLLECTOR_ID,
                                                          ServoStatisticsCollectorData.TRIGGER_SERVOWARNING_ID,
                                                          new ServoStatisticsCollectorData(xMsg));
                        break;
                    case MessageType._SERVO_ERROR:
                        StatisticsCollector.TiggerOccured(
                                                          ServoStatisticsCollectorData.COLLECTOR_ID,
                                                          ServoStatisticsCollectorData.TRIGGER_SERVOERROR_ID,
                                                          new ServoStatisticsCollectorData(xMsg));
                        break;

                    default:
                        break;

                }

                if (xMsgType != MessageType.TRANSFER && xMsgType != MessageType.HOST &&  xMsgType != MessageType.PLC)
                {
                    Naming.PublishEvent(MessageSendEvent.FormSubject("*"), new MessageSendEvent(StkSEM.ElementId,
                      EquipmentType.ValueToString(EquipmentType.STOCKER), xMsgType, xMsg));
                }

                xMsg = "[ " + Util.GetNowDateTimeString(Util.dateTimeFormat.YYYYMMDDHHMMSS_COLRON) + " ] " + xMsg;

                if (xMsgType == MessageType.HOST)
                {
                    
                    Naming.GetEventBrokerManager().Publish(LogMsgEvent.FormSubject("*"),
                                                   new LogMsgEvent(StkSEM.ElementId, EquipmentType.ValueToString(EquipmentType.STOCKER), new LogMsgContents(LogMsgContents.LogMsgEventType.LOG_MSG_HOST, xMsg)));
                }
                if (xMsgType == MessageType.PLC)
                {
                    Naming.GetEventBrokerManager().Publish(LogMsgEvent.FormSubject("*"),
                                                   new LogMsgEvent(StkSEM.ElementId, EquipmentType.ValueToString(EquipmentType.STOCKER), new LogMsgContents(LogMsgContents.LogMsgEventType.LOG_MSG_PLC, xMsg)));
                }

            }
        }
 */
    }


}
