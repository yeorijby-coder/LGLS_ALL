using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.IO;
using System.Globalization;
using System.Runtime.Remoting;
using System.Threading;

using ECP.Util;
using ECP.Util.Client;
using ECP.Global.Exception;


namespace ECP.Service.Logger
{
    /// <summary>
    /// Logggind을 위한 Client 측 Helper Class
    /// Logging client는 이 Class의 Mehtod인 Log을 주로사용하여
    /// Log Message을 Persist 매체에 남기게 된다.
    /// </summary>
    public class Log
    {
        /// <summary>
        /// Class Name
        /// </summary>
        const string CLASS = "Log";

        /// <summary>
        /// Log의 가능여부를 지지하는 Bool변수
        /// </summary>
        private static bool logEnabled = true;

        public static bool LogEnabled
        {
            get { return Log.logEnabled; }
            set { Log.logEnabled = value; }
        }

        /// <summary>
        /// Error에 대한 Log 가능 여부를 나타내는 Flag
        /// </summary>
        private static bool errorLogging = true;

        public static bool ErrorLogging
        {
            get { return Log.errorLogging; }
            set { Log.errorLogging = value; }
        }

        /// <summary>
        /// Error의 Logging Level
        /// </summary>
        private static int errorLevel = 9;

        public static int ErrorLevel
        {
            get { return Log.errorLevel; }
            set { Log.errorLevel = value; }
        }

        /// <summary>
        /// Info의 Log 가능 여부를 나타내는 Flag
        /// </summary>
        private static bool infoLogging = true;

        public static bool InfoLogging
        {
            get { return Log.infoLogging; }
            set { Log.infoLogging = value; }
        }

        /// <summary>
        /// Info의 Logging Level
        /// </summary>
        private static int infoLevel = 9;

        public static int InfoLevel
        {
            get { return Log.infoLevel; }
            set { Log.infoLevel = value; }
        }
        
        /// <summary>
        /// Debug의 Logging 여부를 나타내는 Flag
        /// </summary>
        private static bool debugLogging = true;

        public static bool DebugLogging
        {
            get { return Log.debugLogging; }
            set { Log.debugLogging = value; }
        }
        /// <summary>
        /// Debug의 Logging Level
        /// </summary>
        private static int debugLevel = 9;

        public static int DebugLevel
        {
            get { return Log.debugLevel; }
            set { Log.debugLevel = value; }
        }

        /// <summary>
        /// Communicatiog Log의 Logging여부를 나타내는 Flag
        /// </summary>
        private static bool comLogging = true;

        public static bool ComLogging
        {
            get { return Log.comLogging; }
            set { Log.comLogging = value; }
        }

        /// <summary>
        /// Communicatiog Log의 Logging Level
        /// </summary>
        private static int comLevel = 9;

        public static int ComLevel
        {
            get { return Log.comLevel; }
            set { Log.comLevel = value; }
        }

        /// <summary>
        /// Logging을 하는 Program의 Id
        /// </summary>
        private static string programId = "";

        /// <summary>
        /// Logger를 사용하기 보다는 Standard Out에 Logging을 남길지를 나타내는 Flag
        /// </summary>
        private static bool localOutput = false;

        public static bool LocalOutput
        {
            get { return Log.localOutput; }
            set { Log.localOutput = value; }
        }

        /// <summary>
        /// Logging Message의 Sequence Number
        /// </summary>
        private static int sequenceNumber = 0;

        public static int SequenceNumber
        {
            get { return Log.sequenceNumber; }
            set { Log.sequenceNumber = value; }
        }       

        /// <summary>
        /// Log Message는 아래의 4가지로 분류 될수 있다.
        /// COM은 장비와 또는 다른 External System과의 통신 관련 Logging 분류
        /// DEBUG는 Programmer가 개발시 Debug목적으로 남기는 Log
        /// ERROR는 장비에서 보고하는 Error Log또는 System에서 남기는 각종 Error Log
        /// INFO는 Operator 또는 엔지니어에 보고 한는 Information Log을 의미한다. 
        /// </summary>
        public static  Category INFO = Category.INFO;
        public static  Category ERROR = Category.ERROR;
        public static  Category COM = Category.COM;
        public static  Category DEBUG = Category.DEBUG;


        /// <summary>
        /// Log Thread가 사용하는 logRequestQueue의 Max Size를 나타내는 상수
        /// </summary>
        private static readonly int MAX_LOG_REQUEST_SIZE = 100000;
        /// <summary>
        /// Log Thread가 logRequestQueue를 기다리는 Max Wait Time을 나타내는 상수
        /// </summary>
        private static readonly int MAX_LOG_REQUEST_WAIT_TIME = 10000;


        /// <summary>
        /// Log Manager로 갈 모든 Message가 Queueing되는 FIFO Message Queue
        /// Loggin Thread는 LogManager로 갈 Logging Request을 이 Queue로 부터 제거하여
        /// LogManager로 보내게 된다. 
        /// 
        /// </summary>
        protected static FIFOQueue logRequestQueue =  new FIFOQueue(MAX_LOG_REQUEST_SIZE);

        /// <summary>
        /// logRequestQueue을 서비스 하는 Thread임
        /// 이 Thread는 LogManger로 향하는 Message을 Queue로부터 제거하여
        /// LogManger로 보내는 역활을 한다.
        /// </summary>
        protected static LoggingThread loggingThread = new LoggingThread();

        /// <summary>
        /// Static Initializer
        /// </summary>
        static Log()
        {
            localOutput = true;
            logEnabled = true;
        }


        
   
        /// <summary>
        /// Log Message를 Persist 매체에 남기는 Method
        /// 
        /// Log.log(1, METHOD, "Error", Log.ERROR, getId(), "","Log Error Message");
        /// </summary>
        /// <param name="level">Log의 Level</param>
        /// <param name="function">Message를 남기는 Method</param>
        /// <param name="msgName">Message의 Name</param>
        /// <param name="category">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="device">Message를 남기는 장비의 이름</param>
        /// <param name="trackingId">Message의 Tracking ID</param>
        /// <param name="data">Logging 되어지는 Data를 나타내는 String</param>
        public static void log(int level, string function, string msgName, Category category,
            string device, string trackingId, string data)
        {
            log(level, function, msgName, category, device, trackingId, data, new byte[0], "", "");
        }

        /// <summary>
        ///  Ascii/binary message을 Persist 매체에 남기는 Method
        /// </summary>
        /// <param name="level">Log의 Level</param>
        /// <param name="function">Message를 남기는 Method</param>
        /// <param name="msgName">Message의 Name</param>
        /// <param name="category">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="device">Message를 남기는 장비의 이름</param>
        /// <param name="trackingId">Message의 Tracking ID</param>
        /// <param name="data">Logging 되어지는 Data를 나타내는 String</param>
        /// <param name="binaryData">Loggine 되어지는 binary message의 btye array</param>
        /// <param name="parserAssemblyName">binary message를 parser하는데 사용되어지는 class를 담고 있는 assembly의 명</param>
        /// <param name="parserClassName">binary message를 parser하는데 사용되어지는 class명</param>
        protected static void log(
            int level,
            string function,
            string msgName,
            Category category,
            string device,
            string trackingId,
            string data,
            byte[] binaryData,
            string parserAssemblyName,
            string parserClassName)
        {
            if (!logEnabled)
            {
                 return;
            }

            if (!IsLoggingEnabled(category, level))
            {
                return;
            }


            long timeInMilles = 0;

            timeInMilles = DateTime.Now.Ticks;

            function = Helper.PreventNull(function);
            msgName = Helper.PreventNull(msgName);
            device = Helper.PreventNull(device);
            trackingId = Helper.PreventNull(trackingId);
            data = Helper.PreventNull(data);
            binaryData = (binaryData != null ? binaryData : new byte[0]);
            parserAssemblyName = Helper.PreventNull(parserAssemblyName);
            parserClassName = Helper.PreventNull(parserClassName);

            if (!IsInitialized() ||
                GetLocalOutput())
            {
                LogToSystemOut(new DateTime(timeInMilles), level, function, msgName, category,
                    device, trackingId,
                    data, binaryData, parserAssemblyName, parserClassName);
            }
            else
            {
                LogData logData =
                    new LogData(category, function,device,GetProgramName()+
                    ":"+System.Threading.Thread.CurrentThread.Name,
                    trackingId,
                    msgName,
                    data,
                    timeInMilles,
                    (binaryData.Length >=0) ? "1":"",
                    Convert.ToString(level),
                    parserAssemblyName,
                    parserClassName,
                    binaryData);

                //logToLogManager(logData);

                if (Log.logRequestQueue.PutNoWait(logData))
                {
                    return;
                }
                LogToSystemOut(new DateTime(timeInMilles), level, function, msgName, category,
                    device, trackingId,
                    data, binaryData, parserAssemblyName, parserClassName);      

            }

        }

        ///  Ascii/binary message을 Persist 매체에 남기는 Method
        /// </summary>
        /// <param name="level">Log의 Level</param>
        /// <param name="function">Message를 남기는 Method</param>
        /// <param name="msgName">Message의 Name</param>
        /// <param name="category">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="device">Message를 남기는 장비의 이름</param>
        /// <param name="trackingId">Message의 Tracking ID</param>
        /// <param name="data">Logging 되어지는 Data를 나타내는 String</param>
        /// <param name="binaryData">Loggine 되어지는 binary message의 byte array</param>
        /// <param name="parserAssemblyName">binary message를 parser하는데 사용되어지는 class를 담고 있는 assembly의 명</param>
        /// <param name="parserClassName">binary message를 parser하는데 사용되어지는 class명</param>



        /// <summary>
        /// Ascii 나 binary message를 System Standard Out에 남기는 Method
        /// </summary>
        /// <param name="date">Logging하는 날짜</param>
        /// <param name="level">Log의 Level</param>
        /// <param name="function">Message를 남기는 Method</param>
        /// <param name="msgName">Message의 Name</param>
        /// <param name="category">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="device">Message를 남기는 장비의 이름</param>
        /// <param name="trackingId">Message의 Tracking ID</param>
        /// <param name="data">Logging 되어지는 Data를 나타내는 String</param>
        /// <param name="binaryData">Loggine 되어지는 binary message의 byte array</param>
        /// <param name="parserAssemblyName">binary message를 parser하는데 사용되어지는 class를 담고 있는 assembly의 명</param>
        /// <param name="parserClassName">binary message를 parser하는데 사용되어지는 class명</param>
        public static void LogToSystemOut(
            DateTime date,
            int level,
            string function,
            string msgName,
            Category category,
            string device,
            string trackingId,
            string data,
            byte[] binaryData,
            string parserAssemblyName,
            string parserClassName)
        {
            if (!logEnabled)
            {
                return;
            }

            string binaryDataTransration = "";
            if (binaryData.Length > 0)
            {
                GenericParser parser = GetBinaryDataParser(parserAssemblyName, parserClassName);
                if (parser != null)
                {
                    StringBuilder binaryDataTranslationBuffer = new StringBuilder();

                    parser.SetData(binaryData);
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


            Console.WriteLine(DateTime.Now.ToString("yyyy-MMM-dd HH:mm:ss:fff",
                DateTimeFormatInfo.InvariantInfo));
            Console.WriteLine("Program: " + GetProgramName() + ":" + System.Threading.Thread.CurrentThread.Name);
            Console.WriteLine("Function: " + function);
            Console.WriteLine("Category: " + category.ToString());
            if (!Helper.IsNullOrEmpty(device))
            {
                Console.WriteLine("Device: " + device);
            }
            if (!Helper.IsNullOrEmpty(trackingId))
            {
                Console.WriteLine("TrackingId: " + trackingId);
            }
            Console.WriteLine("Data: " + data);
            if (binaryData.Length > 0)
            {
                Console.WriteLine(binaryDataTransration);
            }
            Console.WriteLine("");

        }

        /// <summary>
        /// 모든 Loggging을 Enable또는 Disable하는 Method
        /// </summary>
        /// <param name="value">Enable일 경우 true</param>
        public static void EnableLog(bool value)
        {
            logEnabled = value;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static bool GetLocalOutput()
        {
            return localOutput;
        }

        /// <summary>
        /// 모든 Message을 LogManger로 보내기 보다는 Standard System out file로 보내는 Method
        /// 이 Method의 목적은 Debug 용임, logOutput property는 
        /// Application Level의 환경설정 파일에 의하여 초기화 되어질 수 있음.
        /// </summary>
        /// <param name="value"></param>
        public static void SetLocalOutput(bool value)
        {
            localOutput = value;
        }

        /// <summary>
        /// Log가 초기화되었는지의 여부
        /// </summary>
        /// <returns>초기화 되었다면 True</returns>
        public static bool IsInitialized()
        {
            return (GetProgramName() != null && GetProgramName().Length != 0);
        }

        /// <summary>
        /// LogManager와 Connection을 위한 Static Initializer
        /// </summary>
        /// <param name="prog"></param>
        public static void Init(string prog)
        {
            SetProgramName(prog);
        }

        /// <summary>
        /// 현재 Thread에서 Logging을 하는데 사용되어지는   Program의 이름을 Return
        /// </summary>
        /// <returns></returns>
        public static string GetProgramName()
        {
            return programId;
        }

        /// <summary>
        /// 현재 Thread에 Loggging을 하는데 사용되어지는 Program의이름을 설정
        /// </summary>
        /// <param name="prog">Program 명</param>
        public static void SetProgramName(string prog)
        {
            programId = prog;
        }

        /// <summary>
        /// Configration되어진  LogLevel설정에 대하여  Message의 Category와 이에 해당하는 Log Level 
        /// 주어 졌을때, Log가능여부를 나타내는 Method 
        /// </summary>
        /// <param name="category">Loggin되는 Message의 Type를 나타내는 정수 
        ///   Category.DEBUG, Category.INFO, Category.COM, Category.Error가 있음</param>
        /// <param name="level">Log의 Level</param>
        /// <returns></returns>
        private static bool IsLoggingEnabled(Category category, int level)
        {
            switch (category.Value())
            {
                case Category._INFO:
                    if (level > infoLevel || !infoLogging)
                        return false;
                    break;
                case Category._DEBUG:
                    if (level > debugLevel || !debugLogging)
                        return false;
                    break;
                case Category._COM:
                    if (level > comLevel || !comLogging)
                        return false;
                    break;
                case Category._ERROR:
                    if (level > errorLevel || !errorLogging)
                        return false;
                    break;
                default:
                    break;
            }
            return true;
        }

        /// <summary>
        /// Binary Message을 Parser하기 위한 Binary Message Parser의 Assemlby명과 Class명이 
        /// 주었졌을 때, Parser의 Instance명을 Return
        /// </summary>
        /// <param name="assemblyName">Parser의 Assembly 명</param>
        /// <param name="parserClassName">Parser의 Class명</param>
        /// <returns></returns>
        public static GenericParser GetBinaryDataParser(string assemblyName, string parserClassName)
        {
            GenericParser parser = null;
            ObjectHandle objHandle = null;
            Object obj = null;

            try
            {
                objHandle = Activator.CreateInstance(assemblyName, parserClassName);
                obj = objHandle.Unwrap();
            }
            catch (FileNotFoundException ex)
            {
                Console.WriteLine(ex.Message.ToString());
                //throw new ECPException(ECPExceptionType.NOT_FOUND, "", "Invalid paser class name");
            }
            parser = obj as GenericParser;

            return parser;
        }

        /// <summary>
        /// Binary또는 ascii Message을 LogManger로 보내는 Method
        /// </summary>
        /// <param name="logData">Logging되어질 Data를 담고 있는 Log Data</param>
        protected static void LogToLogManager(LogData logData)
        {
            try
            {
                long sequenceId = GetSequenceNumber();

                if (logData.binaryData.Length > 0)
                {
                        //Naming.getLogManager().logBinary(
                        //Convert.ToInt32(logData.level),
                        //logData.category,
                        //logData.time,
                        //sequenceId,
                        //logData.sourceId,
                        //logData.method,
                        //logData.device,
                        //logData.trackingId,
                        //logData.messageName,
                        //logData.data,
                        //logData.binaryData,
                        //logData.assemblyName,
                        //logData.className);
                    LogManager.LogBinary(
                    Convert.ToInt32(logData.level),
                    logData.category,
                    logData.time,
                    sequenceId,
                    logData.sourceId,
                    logData.method,
                    logData.device,
                    logData.trackingId,
                    logData.messageName,
                    logData.data,
                    logData.binaryData,
                    logData.assemblyName,
                    logData.className);
                }
                else
                {
                        //Naming.getLogManager().logString(
                        //Convert.ToInt32(logData.level),
                        //logData.category,
                        //logData.time,
                        //sequenceId,
                        //logData.sourceId,
                        //logData.method,
                        //logData.device,
                        //logData.trackingId,
                        //logData.messageName,
                        //logData.data);
                    LogManager.LogString(
                    Convert.ToInt32(logData.level),
                    logData.category,
                    logData.time,
                    sequenceId,
                    logData.sourceId,
                    logData.method,
                    logData.device,
                    logData.trackingId,
                    logData.messageName,
                    logData.data);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
                LogToSystemOut(logData);
            }
        }

        /// <summary>
        /// Binary또는 ascii Message을 System Standard Out 으로 보내는 Method
        /// </summary>
        /// <param name="logData"></param>
        public static void LogToSystemOut(LogData logData)
        {
            LogToSystemOut(
                new DateTime(logData.time),
                Convert.ToInt32(logData.level),
                logData.method,
                logData.messageName,
                logData.category,
                logData.device,
                logData.trackingId,
                logData.data,
                logData.binaryData,
                logData.assemblyName,
                logData.className);
        }

        private static long GetSequenceNumber()
        {
            return sequenceNumber++;
        }


        /// <summary>
        /// 이 Thread Class는 logRequestQueue에 담겨진 LogData Message을 FIFO순으로
        /// LogManager로 보내는 역활을 하는 Thread을 위한 Class임
        /// 
        /// </summary>
        public class LoggingThread
        {
         
            const string CLASS = "LoggingThread";
   
            private Thread  loggingThread = null;

            public LoggingThread()
            {
                this.loggingThread = new Thread(new ThreadStart(Run));
                this.loggingThread.Name = "LoggingThread";
                this.loggingThread.IsBackground = true;
                this.loggingThread.Start();
            }

            public void Run()
            {
                if (logRequestQueue == null) return;
                while (true)
                {
                    try
                    {
                        LogData logData = (LogData)logRequestQueue.Get(MAX_LOG_REQUEST_WAIT_TIME);
                       // Console.WriteLine("Logging Thread is running");

                        if (logData != null)
                        {
                           // Console.WriteLine("Logging Thread has data");

                            // call remote method, let log service do work!
                            Log.LogToLogManager(logData);
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.Write(ex.ToString());
                    }
                }
            }          

        }        
    }
}
