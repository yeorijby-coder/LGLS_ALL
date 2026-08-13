using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;

namespace ECP.Service.Statistics
{
    public class Trigger
    {
        /// <summary>
        /// Trigger를 위한 Container Class
        /// </summary>
        public static Dictionary<string, Trigger> triggers = 
            new Dictionary<string, Trigger>();

        /// <summary>
        /// 통계성 Data를 실제 Persist매체에 남길 Logger의 Instance
        /// </summary>
        private StatisticalLog logger = null;

        public StatisticalLog Logger
        {
            get { return logger; }
            set { logger = value; }
        }
        
        /// <summary>
        /// Trigger의 Id
        /// </summary>
        public string id = "";


        /// <summary>
        /// 주어진 Trigger의 Id에 해당하는 Reference을 Return
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public static Trigger GetInstance(string id)
        {
            Trigger trigger = null;

            if(triggers.ContainsKey(id))
            {
                trigger = triggers[id];

            }
            else
            {
                trigger = new Trigger();
                trigger.Init(id, new StatisticalLogInfo(true, 30, 100,10000));
                triggers.Add(id, trigger);
            }
            return trigger;
        }

        /// <summary>
        /// 기본 생성자
        /// </summary>
        public Trigger()
        {
        }

        /// <summary>
        /// Trigger을 주어진 Log Info로 초기화
        /// </summary>
        /// <param name="id">Trigger Id</param>
        /// <param name="info">Trigger의 정보(Enable여부, 최대 남겨질 data설정, 최대 남겨질 화일의 수, 
        /// 1개의 화일에 남겨질 Entry의 수)</param>
        public void Init(string id, StatisticalLogInfo info)
        {
            logger = new StatisticalLog();
            logger.Init(id);
            logger.SetStatisticalLogInfo(info);
            logger.SetLocalOutput(false);
        }

        /// <summary>
        /// StatisticsManager에 의하여 호출되어지는 Method
        /// </summary>
        /// <param name="theDate">Time Stamp</param>
        /// <param name="collectorId">Collector의 Id</param>
        /// <param name="triggerId">Trigger의 Id</param>
        /// <param name="reportedData">실제 남겨질 통계 Message</param>
        public void AddData(
            DateTime theDate,
            string collectorId,
            string triggerId,
            string reportedData)
        {
            SaveData(theDate, collectorId, triggerId, reportedData);
        }

        /// <summary>
        /// 통계 Message을 실제 Persist 매체에 남기는 Method
        /// </summary>
        /// <param name="theDate">Time Stamp</param>
        /// <param name="collectorId">Collector의 Id</param>
        /// <param name="triggerId">Trigger의 Id</param>
        /// <param name="reportedData">실제 남겨질 통계 Message</param>
        protected  void SaveData(
            DateTime theDate,
            string collectorId,
            string triggerId,
            string reportedData)
        {

            StringBuilder outputString = new StringBuilder();

            outputString.Append(triggerId);
            outputString.Append(reportedData);

            logger.Log(theDate, collectorId, outputString.ToString());
        }
    }
}
