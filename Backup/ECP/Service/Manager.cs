using System;
using System.Collections.Generic;
using System.Text;

using ECP.Db;

namespace ECP.Service
{

    /// <summary>
    /// Manager는 여러 Service을 담당하고 있는 하위 Manger의 상위 Class임
    /// Manager는 Owner를 가질 수 있으며, 이것은 Operating System의 
    /// Process단위가 될 수 있다, 즉 다시말하면, 하나의 Process가 
    /// 여러개의 Manager를 가질 수 있는 구조임
    /// 
    /// 궁극적으로는 하나의 Owner가 모든 Manger를 가지는 구조가
    /// 가장 Ideal하나, 시스템의 부하, 및 Load Balancing을 고려한
    /// 최적의 Deployment 정책에 따라서 유연하게 구성되어질 수 있다.
    /// </summary>
    public class Manager : AbstractManager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "Manager";

        /// <summary>
        /// 주어진 Manager Id로 Manager 생성 
        /// </summary>
        /// <param name="ownerId">Owner Id</param>
        /// <param name="managerId">Manager Id</param>
        public Manager(string ownerId, string managerId) 
            : base (ownerId,managerId)
        {
            this.id = managerId;
        }

        /// <summary>
        /// Manger Id
        /// </summary>
        public string id = "";


        public static string BASE_DIR_NAME = string.Empty;

        /// <summary>
        /// Manager 가 Database에 연결하기 위한 DB URL
        /// </summary>
        private string dbConnectionString = string.Empty;

        //private int logLevel;
        //private bool dirtyFlag = false;

        //private bool errorLogging;
        //private int errorLevel;

        //private bool infoLogging;
        //private int infoLevel;

        //private bool debugLogging;
        //private int debugLevel;

        //private bool comLogging;
        //private int comLevel;

        //private bool localLogEnabled;
        //private int localLogFiles;

        //private int localLogEntries;

        //public long startTime;

        /// <summary>
        /// Manager Static Initializer
        /// </summary>
        static Manager() 
        {
            //createDBTabel();
            

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
        /// Manager의 Id을 Return
        /// </summary>
        /// <returns></returns>
        public string GetId()
        {
            return this.ManagerId;
        }


        /// <summary>
        /// Manager가 살아 있는지에 대한 Ping
        /// </summary>
        /// <returns></returns>
        public override bool IsAlive()
        {
            return base.IsAlive();
        }

        

    }
}
