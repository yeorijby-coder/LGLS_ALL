using System;
using System.Collections.Generic;
using System.Text;


using ECP.Db;
using ECP.Util;

namespace ECP.Service.Statistics
{
    public class StatisticsManager : Manager
    {

        /// <summary>
        /// 주어진 Owner Id와 ManagerId로 StatisticsManager생성한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="managerId"></param>
        public StatisticsManager(string ownerId, string managerId)
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
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        /// <summary>
        /// StatisticsManager에게 통계성 Data를 보내는 Method
        /// </summary>
        /// <param name="timeStamp">Time Stamp</param>
        /// <param name="collectorId">통계 Message의 Collector Id</param>
        /// <param name="triggerId">통계 Message의 Trigger Id</param>
        /// <param name="data">통계 Data Message</param>
        public static void reportTrigger(long timeStamp, string collectorId, 
            string triggerId, string data)
        {

            Trigger.GetInstance(collectorId).
                AddData(new DateTime(timeStamp), collectorId, triggerId, data);

        }

        /// <summary>
        /// 주어진 Id로 Trigger Reference을 Return
        /// </summary>
        /// <param name="id">Trigger Id</param>
        /// <returns>Trigger의 Reference</returns>
        public Trigger GetTrigger(string id)
        {
            return Trigger.GetInstance(id);
        }

        /// <summary>
        /// 모든 Trigger를 Return
        /// </summary>
        /// <returns>All Triggers</returns>
        public Dictionary<string, Trigger> GetAllTrigger()
        {
            return Trigger.triggers;
        }
  

        /// <summary>
        /// Trigger의 정보(Enable여부, 최대 남겨질 data설정, 최대 남겨질 화일의 수, 
        /// 1개의 화일에 남겨질 Entry의 수)를 초기화 하는 Method
        /// 
        /// </summary>
        /// <param name="id">Trigger Id</param>
        /// <param name="info">Trigger의 정보(Enable여부, 최대 남겨질 data설정, 최대 남겨질 화일의 수, 
        /// 1개의 화일에 남겨질 Entry의 수)</param>
        public static void SetTriggerInfo(string id, StatisticalLogInfo info)
        {
            Trigger trigger = Trigger.GetInstance(id);
            trigger.Init(id, info);
        }
    }
}
