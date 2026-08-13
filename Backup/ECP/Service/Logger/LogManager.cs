using System;
using System.Collections.Generic;
using System.Text;

using ECP.Db;

namespace ECP.Service.Logger
{
    /// <summary>
    /// 이 Class는 Manager를 상속한다.
    /// 이 Class는 시스템에서 Log을 남기는 서비스를 담당하는 Manager Class이다. 
    /// 
    /// </summary>
    public class LogManager: Manager
    {

        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = ".LogManager";


        /// <summary>
        /// 기본 생성자
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="managerId"></param>
        public LogManager(string ownerId, string managerId)
            : base(ownerId, managerId)
        {

        }

        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        /// <summary>
        /// Data base로 부터 Data를 Load함. 항상 Sub Class에 의하여 Override되어
        /// Implement되어져야 함 항상 base.Load()가 먼저 Call되어져야 함
        /// 
        /// SubClass가 Instance화 된 직후, 이 Method을 Call함으로 Configration되어있는
        /// 기준 정보에 의하여 Sub Object가 초기화 되어짐
        /// 
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection</param>
        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

        /// <summary>
        /// Data base로 Data를 Save함
        /// Sub Class는 이 Method를 Override하여 Implement하여야 하며,
        /// 항상 base.Save() 먼저 Call되어 상속 계층에 있는 Parent Class가 먼저 
        /// 저장되도록 하여야 함
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }



        /// <summary>
        /// Binary Log Message을 Database또는 Persist매체에 남김
        /// </summary>
        /// <param name="theSeverityLevel">Binary Message 의 theSeverityLevel</param>
        /// <param name="theCategory">Binary Message의 카테고리</param>
        /// <param name="theTimeInMillis">Log Data에  대한 Time Stamp</param>
        /// <param name="theSequenceNumber">Message에 대한 Sequence Number</param>
        /// <param name="theSourceID">Binary Message를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">Binary Message를 Logging하는 Method명</param>
        /// <param name="theDeviceID">Binary Message를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">Binary Message에 대한 Tracking Id</param>
        /// <param name="theMessageName">Binary Message에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        /// <param name="theBinaryData">Binary Message에 대한 Binary Data</param>
        /// <param name="theLogParserAssemblyName">Binary Message에 대한 Log 해석기에 대한 Assembly Name</param>
        /// <param name="theLogParserClassName">Binary Message에 대한 Log 해석기에 대한 Class Name</param>
        public static void LogBinary(
            int theSeverityLevel,
            Category theCategory,
            long theTimeInMillis,
            long theSequenceNumber,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData,
            byte[] theBinaryData,
            string theLogParserAssemblyName,            
            string theLogParserClassName
            )
        {
            LogBinaryMessage(
                theSeverityLevel,
                theCategory,
                theTimeInMillis,
                theSequenceNumber,
                theSourceID,
                theMethodID,
                theDeviceID,
                theTrackingID,
                theMessageName,
                theAsciiData,
                theBinaryData,
                theLogParserAssemblyName,
                theLogParserClassName
            );
        }

        /// <summary>
        /// Binary Log Message을 Logging Thread Pool를 사용하여,  Database또는 Persist매체에 남김
        /// 이 Method는 message을 LoggingRequestQueue에 놓게 되고, Logging Thread Pool중의 가용한
        /// Thread가 LoggingRequestQueue로부터 Log Data를 하나 취출하여, LogDataHandle로 하여금
        /// 실제 Data base또는 Persist매체로 Log을 남기도록한다.
        /// 
        /// 현재 프로젝트에서는 Thread Pool없이, LogDataHandle이 File에 Logging하고 있음
        /// </summary>
        /// <param name="theSeverityLevel">Binary Message 의 theSeverityLevel</param>
        /// <param name="theCategory">Binary Message의 카테고리</param>
        /// <param name="theTimeInMillis">Log Data에  대한 Time Stamp</param>
        /// <param name="theSequenceNumber">Message에 대한 Sequence Number</param>
        /// <param name="theSourceID">Binary Message를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">Binary Message를 Logging하는 Method명</param>
        /// <param name="theDeviceID">Binary Message를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">Binary Message에 대한 Tracking Id</param>
        /// <param name="theMessageName">Binary Message에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        /// <param name="theBinaryData">Binary Message에 대한 Binary Data</param>
        /// <param name="theLogParserAssemblyName">Binary Message에 대한 Log 해석기에 대한 Assembly Name</param>
        /// <param name="theLogParserClassName">Binary Message에 대한 Log 해석기에 대한 Class Name</param>
        public static void LogBinaryMessage(
            int theSeverityLevel,
            Category theCategory,
            long theTimeInMillis,
            long theSequenceNumber,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData,
            byte[] theBinaryData,
            string theLogParserAssemblyName,   
            string theLogParserClassName
            )
        {

            // Later we will use thread pool for logging , now we just log.

            LogDataHandle.GetInstance(theSourceID).LogBinaryMessage(
                new DateTime(theTimeInMillis),
                theCategory.ToString(),
                theSeverityLevel,
                theSourceID,
                theMethodID,
                theDeviceID,
                theTrackingID,
                theMessageName,
                theAsciiData,
                theLogParserAssemblyName,
                theLogParserClassName,
                theBinaryData);         
            
        }


        
        /// <summary>
        /// AsciiMessage을 Database또는 Persist매체에 남김
        /// </summary>
        /// <param name="theSeverityLevel">AsciiMessage 의 theSeverityLevel</param>
        /// <param name="theCategory">AsciiMessage의 카테고리</param>
        /// <param name="theTimeInMillis">Log Data에  대한 Time Stamp</param>
        /// <param name="theSequenceNumber">Message에 대한 Sequence Number</param>
        /// <param name="theSourceID">AsciiMessage를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">AsciiMessage를 Logging하는 Method명</param>
        /// <param name="theDeviceID">AsciiMessage를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">AsciiMessage에 대한 Tracking Id</param>
        /// <param name="theMessageName">AsciiMessage에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        public static  void LogString(
            int theSeverityLevel,
            Category theCategory,
            long theTimeInMillis,
            long theSequenceNumber,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData
            )
        {
            LogAsciiMessage(
                theSeverityLevel,
                theCategory,
                theTimeInMillis,
                theSequenceNumber,
                theSourceID,
                theMethodID,
                theDeviceID,
                theTrackingID,
                theMessageName,
                theAsciiData
            );
        }


       

        /// <summary>
        /// AsciiMessage Log Message을 Logging Thread Pool를 사용하여,  Database또는 Persist매체에 남김
        /// 이 Method는 message을 LoggingRequestQueue에 놓게 되고, Logging Thread Pool중의 가용한
        /// Thread가 LoggingRequestQueue로부터 Log Data를 하나 취출하여, LogDataHandle로 하여금
        /// 실제 Data base또는 Persist매체로 Log을 남기도록한다.
        /// 
        /// 현재 프로젝트에서는 Thread Pool없이, LogDataHandle이 File에 Logging하고 있음
        /// </summary>
        /// <param name="theSeverityLevel">AsciiMessage 의 theSeverityLevel</param>
        /// <param name="theCategory">AsciiMessage의 카테고리</param>
        /// <param name="theTimeInMillis">Log Data에  대한 Time Stamp</param>
        /// <param name="theSequenceNumber">Message에 대한 Sequence Number</param>
        /// <param name="theSourceID">AsciiMessage를 Logging하는 Source의 Id</param>
        /// <param name="theMethodID">AsciiMessage를 Logging하는 Method명</param>
        /// <param name="theDeviceID">AsciiMessage를 Logging하는 장비의  Id</param>
        /// <param name="theTrackingID">AsciiMessage에 대한 Tracking Id</param>
        /// <param name="theMessageName">AsciiMessage에 대한 Message Name</param>
        /// <param name="theAsciiData">Binary Message에 대한 Ascii Data</param>
        public static void LogAsciiMessage(
            int theSeverityLevel,
            Category theCategory,
            long theTimeInMillis,
            long theSequenceNumber,
            string theSourceID,
            string theMethodID,
            string theDeviceID,
            string theTrackingID,
            string theMessageName,
            string theAsciiData
        )
        {
            LogDataHandle.GetInstance(theSourceID).LogAsciiMessage(
                new DateTime(theTimeInMillis),
                theCategory.ToString(),
                theSeverityLevel,
                theSourceID,
                theMethodID,
                theDeviceID,
                theTrackingID,
                theMessageName,
                theAsciiData);
        }
    }
}
