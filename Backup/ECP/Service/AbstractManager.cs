using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using ECP.Interfaces.Service;
using ECP.Global.Exception;
using ECP.Db;

namespace ECP.Service
{
    /// <summary>
    /// 하나의 Owner(Process 또는 Server)는 여러대의 Manager를 Hosting할 수 있다.
    /// AbstractManager는 Manager에 의하여 상속되어지며, 해당 Owner와 
    /// 해당 Manager에 Assigne되어진 Manager를 다룬다.
    /// </summary>
    public abstract class AbstractManager : Persistence, IManager
    {
  
        /// <summary>
        /// Owner의 Id에 따라, manger의 map을 가지고 있는 Container
        /// key : owner Id, value : Dictionary (key : manager Id, value : manager )
        /// </summary>
        private static Dictionary<string, Dictionary<string,AbstractManager>> managerMap = 
            new Dictionary<string,Dictionary<string,AbstractManager>>();

        /// <summary>
        /// Owner의 Id, Process 단위의 Server Id
        /// </summary>
        protected string ownerId = string.Empty;

        /// <summary>
        /// Owner Id
        /// </summary>
        public string OwnerId
        {
            get { return this.ownerId; }
        }
 
        /// <summary>
        /// Manager Id 
        /// </summary>
        private string managerId = string.Empty;
        /// <summary>
        /// Manager Id
        /// </summary>
        public string ManagerId
        {
            get { return this.managerId; }
        }

        //private bool dirtyFlag = false;

        /// <summary>
        /// 생성자
        /// </summary>
        static AbstractManager()
        {
            //createDBTable();
        }

  
        /// <summary>
        /// 주어진 OwnerId 및 ManagerId로 Manager생성
        /// </summary>
        /// <param name="ownerId">Owner Id, ServerID을 의미할수 있음</param>
        /// <param name="managerId">Manger Id</param>
        public AbstractManager(string ownerId, string managerId)
        {
            Dictionary<string, AbstractManager> managers = null;

            this.ownerId = ownerId;
            this.managerId = managerId;

            if (!managerMap.ContainsKey(ownerId))
            {
                managers = new Dictionary<string, AbstractManager>();
                managerMap.Add(ownerId, managers);
            }
            else
            {
                managers = managerMap[ownerId];
            }


            if (managers.ContainsKey(managerId))
            {
                managers[managerId] = this;
            }
            else
            {
                managers.Add(managerId, this);
            }

        }

        /// <summary>
        /// AbstractManger을 위한 Table생성
        /// </summary>
        private static void CreateDBTable()
        {

        }

        /// <summary>
        /// 주어진 OwnerId 와 ManagerId에 해당하는 Manager Return
        /// </summary>
        /// <param name="ownerId">Owner Id</param>
        /// <param name="managerId">Manager Id</param>
        /// <returns>Manager</returns>
        public static AbstractManager GetManager(string ownerId, string managerId)
        {
            AbstractManager manger = null;

            if (managerMap.ContainsKey(ownerId))
            {
                Dictionary<string, AbstractManager> managers = managerMap[ownerId];

                if (managers.ContainsKey(managerId))
                {
                    manger = managers[managerId];
                }
                else
                {
                    throw new ECPException(ECPExceptionType.NOT_FOUND, "", "Invalid ManagerId " + managerId);
                }
            }
            else
            {           
                throw new ECPException(ECPExceptionType.NOT_FOUND, "", "Invalid OwnerId " + ownerId);
            }
            return manger;

        }

        /// <summary>
        /// Owner(OS Process or Server)가지 현재 Hosting하고 있는 Manager의 List 
        /// </summary>
        /// <param name="ownerId">Owner Id</param>
        /// <returns>Owner가 Hosting하는 Manager의 List</returns>
        public static string[] GetHostedManager(string ownerId)
        {
            string[] ids = new string[0];

            if(managerMap.ContainsKey(ownerId))
            {
                Dictionary<string, AbstractManager> managers = managerMap[ownerId];
                
                ids = new string[managers.Count];
                if (managers != null)
                {
                    int i =0;
                    foreach (AbstractManager manager in managers.Values)
                    {
                        ids[i++] = manager.managerId;
                    }
                    return ids;
                }
            }
            return null;
        }
    }
}