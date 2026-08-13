using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;

using ECP.Util;
using ECP.Global.Exception;

namespace ECP.Service.Logger
{

    /// <summary>
    /// LogManager의 모든 Functionality에 대하여 실제로 Persist 매체에
    /// 기록하는 책임을 가지는 Class
    /// </summary>
    public class LogDataHandle
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "LogDataHandle";

        /// <summary>
        /// Singleton Object를 위한 attribute
        /// </summary>
        public static LogDataHandle theLogDataHandle = null;

        /// <summary>
        /// 
        /// </summary>
        public string id = "";


        /// <summary>
        /// 주어진 programId에 대한 LogDataHandle의 Singleton Instance을 Return
        /// </summary>
        /// <param name="prog">program Id</param>
        /// <returns><주어진 programId에 대한 LogDataHandle의 Singleton Instance/returns>
        public static LogDataHandle GetInstance(string prog)
        {
            if (theLogDataHandle == null)
            {
                theLogDataHandle = new LogDataHandle();
                
            }
            theLogDataHandle.Init(prog);
            return theLogDataHandle;
        }

        /// <summary>
        /// Static Initializer
        /// </summary>
        static LogDataHandle()
        {
        }


        /// <summary>
        /// LogDataHandle의 초기화를 실시
        /// </summary>
        /// <param name="prog">Program Id</param>
        public void Init(string prog)
        {
            LocalLog.GetInstance().SetLocalLogInfo(new LocalLogInfo(true, 10000)); 
            LocalLog.GetInstance().Init(prog.Substring(0, prog.IndexOf(":", 0))); // Program Id의 초기화
            LocalLog.GetInstance().SetLocalOutput(false);  // Local Standard Output로의 Out를 Disable
        }

        /// <summary>
        /// Binary Log Message을 LoggerManager로부터 받아서 직접 Persist 매체에 기록하는 Method
        /// </summary>
        /// <param name="theDate">Logging DateTime Stamp</param>
        /// <param name="theCategory">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="theLevel">Log의 Level</param>
        /// <param name="theSourceID">Binary Message를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">Binary Message를 Logging하는 Method명</param>
        /// <param name="theDeviceID">Binary Message를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">Binary Message에 대한 Tracking Id</param>
        /// <param name="theMessageName">Binary Message에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        /// <param name="theLogParserAssemblyName">Binary Message에 대한 Log 해석기에 대한 Assembly Name</param>
        /// <param name="theLogParserClassName">Binary Message에 대한 Log 해석기에 대한 Class Name</param>
        /// <param name="theBinaryData">Binary Message에 대한 Binary Data의 byte array</param>
        public  void LogBinaryMessage(
            DateTime theDate,
            string theCategory,
            int theLevel,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData,
            string theLogParserAssemblyName,
            string theLogParserClassName,
            byte[] theBinaryData)
        {
            SaveLogData(
               theDate,
               theCategory,
               theLevel,
               theSourceID,
               theMethodID,
               theDeviceID,
               theTrackingID,
               theMessageName,
               theAsciiData,
               theLogParserAssemblyName,
               theLogParserClassName,
               theBinaryData
           );
        }


        /// <summary>
        /// Binary Log Message을 LoggerManager로부터 받아서 직접 Persist 매체에 기록하는 Method
        /// </summary>
        /// <param name="theDate">Logging DateTime Stamp</param>
        /// <param name="theCategory">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="theLevel">Log의 Level</param>
        /// <param name="theSourceID">Binary Message를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">Binary Message를 Logging하는 Method명</param>
        /// <param name="theDeviceID">Binary Message를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">Binary Message에 대한 Tracking Id</param>
        /// <param name="theMessageName">Binary Message에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        /// <param name="theLogParserAssemblyName">Binary Message에 대한 Log 해석기에 대한 Assembly Name</param>
        /// <param name="theLogParserClassName">Binary Message에 대한 Log 해석기에 대한 Class Name</param>
        /// <param name="theBinaryData">Binary Message에 대한 Binary Data의 byte array</param>
        protected  void SaveLogData(
            DateTime theDate,
            string theCategory,
            int theLevel,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData,
            string theLogParserAssemblyName,
            string theLogParserClassName,
            byte[] theBinaryData)
        {

            StringBuilder logOutputString = new StringBuilder();

            string binaryDataTransration = "";
            if (theBinaryData.Length > 0)
            {
                GenericParser parser = Log.GetBinaryDataParser(theLogParserAssemblyName, theLogParserClassName);
                if (parser != null)
                {
                    StringBuilder binaryDataTranslationBuffer = new StringBuilder();

                    parser.SetData(theBinaryData);
                    string[] parserTypes = parser.GetParserTypes();
                    for (int i = 0; i < parserTypes.Length; i++)
                    {
                        try
                        {
                            binaryDataTranslationBuffer.Append(
                                (i == 0 ? "" : "\n") +
                                "BinaryDataIn" + parserTypes[i] + ": " +
                                 parser.GetParsedData(parserTypes[i]));


                        }
                        catch (ECPException ex)
                        {
                            Console.WriteLine(ex.Message.ToString());
                        }
                    }
                    binaryDataTransration = binaryDataTranslationBuffer.ToString();
                }
            }

           // logOutputString.Append("\n"+DateTime.Now.ToString("yyyy-MMM-dd HH:mm:ss:fff",
           //     DateTimeFormatInfo.InvariantInfo));

            logOutputString.Append("\n" + "Program: " + theSourceID);
            logOutputString.Append("\n" + "Function: " + theMethodID);
            logOutputString.Append("\n" + "Category: " + theCategory.ToString());
            if (!Helper.IsNullOrEmpty(theDeviceID))
            {
                logOutputString.Append("\n" + "Device: " + theDeviceID);
            }
            if (!Helper.IsNullOrEmpty(theTrackingID))
            {
                logOutputString.Append("\n" + "TrackingId: " + theTrackingID);
            }
            logOutputString.Append("\n" + "Data: " + theAsciiData);
            if (theBinaryData.Length > 0)
            {
                logOutputString.Append(binaryDataTransration);
            }
            logOutputString.Append("\n");

            LocalLog.GetInstance().Log(logOutputString.ToString());
        }


        /// <summary>
        /// Ascii Message를 Persist 매체에 직접 기록하는 Method
        /// </summary>
        /// <param name="theDate">Logging DateTime Stamp</param>
        /// <param name="theCategory">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="theLevel">Log의 Level</param>
        /// <param name="theSourceID">Ascii Message를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">Ascii Message를 Logging하는 Method명</param>
        /// <param name="theDeviceID">Ascii Message를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">Ascii Message에 대한 Tracking Id</param>
        /// <param name="theMessageName">Ascii Message에 대한 Message Name</param>
        /// <param name="theAsciiData">Ascii Message에 대한 String</param>
        public void LogAsciiMessage(
            DateTime theDate,
            string theCategory,
            int theLevel,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData
        )
        {
       
            SaveLogData(
                theDate,
                theCategory,
                theLevel,
                theSourceID,
                theMethodID,
                theDeviceID,
                theTrackingID,
                theMessageName,
                theAsciiData,
                "",
                "",
                new byte[0]);
        }
    }
}
