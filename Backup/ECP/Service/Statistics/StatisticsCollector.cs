using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.Remoting;
using System.Threading;

using ECP.Util;
using ECP.Util.Client;
using ECP.Service.Logger;

namespace ECP.Service.Statistics
{
    /// <summary>
    /// 통계 Data를 남기기 위한 Client 측 Helper Class
    /// 통계 Data 남기기 위한 client는 이 Class의 Mehtod인 TiggerOccured을 주로사용하여
    /// 통계 Message을 Persist 매체에 남기게 된다.
    /// </summary>
    public class StatisticsCollector
    {
        /// <summary>
        /// Class Name
        /// </summary>
        const string CLASS = "StatisticsCollector";

        /// <summary>
        /// StatisticsThread가 사용하는 statisticsRequestQueue의 Max Size를 나타내는 상수
        /// </summary>
        private static readonly int MAX_LOG_REQUEST_SIZE = 100000;

        /// <summary>
        /// StatisticsThread가 statisticsRequestQueue를 기다리는 Max Wait Time을 나타내는 상수
        /// </summary>
        private static readonly int MAX_LOG_REQUEST_WAIT_TIME = 10000;

 
        /// <summary>
        /// StatisticsCollector의 Single Instance
        /// </summary>
        public static StatisticsCollector instance = new StatisticsCollector();

        /// <summary>
        /// StatisticsManager로 갈 모든 Message가 Queueing되는 FIFO Message Queue
        /// StatisticsThread는 StatisticsManager로 갈 Statistics Request을 이 Queue로 부터 제거하여
        /// StatisticsManager로 보내게 된다. 
        /// </summary>
        protected static FIFOQueue statisticsRequestQueue = new FIFOQueue(MAX_LOG_REQUEST_SIZE);

        /// <summary>
        /// statisticsRequestQueue을 서비스 하는 Thread임
        /// 이 Thread는 StatisticsManager로 향하는 Message을 Queue로부터 제거하여
        /// StatisticsManager로 보내는 역활을 한다.
        /// </summary>
        protected static StatisticsThread statisticsThread = new StatisticsThread();

        /// <summary>
        /// StatisticsCollector의 Single Instance을 Return
        /// </summary>
        /// <returns></returns>
        public static StatisticsCollector GetInstance()
        {
            return instance;
        }
 
        /// <summary>
        /// 통계성 Data를 Persist 매체에 남기는 Method
        /// 
        /// StatisticsCollectorData는 Interface이며, 매개변수로 넘겨올 통계성 Data는 
        /// 이 Interface을 구현한 Concreate Class임
        /// 
        /// 통계성 Data는 Log Data처럼 Ascii나 Binary Message처럼 자유로운 Format으로 
        /// 기술 되는 대신에, 틀(Frame)을 가진 메세지 포멧으로, Data의 형태에 제약을 가함으로서,
        /// 통계성 Data의 일관성을 이루게 하도록 한다.
        /// 
        /// </summary>
        /// <param name="collectorId">Collector의 Id</param>
        /// <param name="triggerId">Collector의 Trigger Id</param>
        /// <param name="specificData">남기고자 하는 통계성 Data의 Content</param>
        public static void TriggerOccured(string collectorId, string triggerId, StatisticsCollectorData specificData)
        {
            long timeInMilles = 0;

            timeInMilles = DateTime.Now.Ticks;

            collectorId = Helper.PreventNull(collectorId);
            triggerId = Helper.PreventNull(triggerId);


            StatisticsData data =   new StatisticsData(collectorId, triggerId, timeInMilles, specificData.GetReportedData());

            if (StatisticsCollector.statisticsRequestQueue.PutNoWait(data))
            {
                return;
            }

        }

        /// <summary>
        /// 통계성 Data를 StatisticsManager에 전송하는 Method
        /// </summary>
        /// <param name="data"></param>
        public static void sendStatisticReports(StatisticsData data)
        {
            //Naming.GetStatisticManger

            StatisticsManager.reportTrigger(data.time, 
                data.collectorId, data.triggerName, data.reportedData);
        }

        /// <summary>
        /// 이 Thread Class는 statisticsRequestQueue에 담겨진 Statistical Message을 FIFO순으로
        /// StatisticsManager로 보내는 역활을 하는 Thread을 위한 Class임
        /// </summary>
        public class StatisticsThread
        {

            const string CLASS = "StatisticsThread";

            private Thread statisticsThread = null;

            public StatisticsThread()
            {
                this.statisticsThread = new Thread(new ThreadStart(Run));
                this.statisticsThread.Name = "StatisticsThread";
                this.statisticsThread.IsBackground = true;
                this.statisticsThread.Start();
            }

            public void Run()
            {
                if (statisticsRequestQueue == null) return;
                while (true)
                {
                    try
                    {
                        StatisticsData data = (StatisticsData)statisticsRequestQueue.Get(MAX_LOG_REQUEST_WAIT_TIME);
                        // Console.WriteLine("Static Logging Thread is running");

                        if (data != null)
                        {
                        // call remote method, let statics service do work!
                        //     Console.WriteLine("Static Logging Thread has data");
                            StatisticsCollector.sendStatisticReports(data);
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.Write(ex.ToString());
                        Log.log(5, "Run", "debug", ECP.Service.Logger.Category.DEBUG, "", "Run", ex.Message.ToString());
                    }
                }
            }

        }

        /// <summary>
        /// statisticsRequestQueue에 저장되어질 통계 Data을 위한 Class
        /// </summary>
        [Serializable]
        public class StatisticsData
        {
            public long time = 0;
            public string collectorId = "";
            public string triggerName = "";

            public string reportedData = null;

            public StatisticsData(string collectorId, string triggerName, long time, string reportedData)
            {
                this.collectorId = collectorId;
                this.triggerName = triggerName;
                this.time = time;
                this.reportedData = reportedData;
            }

        }
    }
}
