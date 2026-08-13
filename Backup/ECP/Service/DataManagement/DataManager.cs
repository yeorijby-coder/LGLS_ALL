using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Timers;

using ECP.Db;
using ECP.Util;
using ECP.Db.Entity;
using ECP.Global;
using ECP.BizRule;
using System.Runtime.Remoting;
using ECP.Service.Logger;

namespace ECP.Service.DataManagement
{
    /// <summary>
    /// Movement를 관리 함.
    /// </summary>
    
    [Serializable]
    public class DataManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "DataManager";

        //SAP I/F check : 기준 정보로 빼내야 함.
        Timer timerDataPurge = new Timer();

        protected static Boolean timerDataPurgeFlag = false;

        /// <summary>
        /// MovementManager 생성자
        /// 주어진 Owner Id와 Manager Id로 MovementManager를 생성한다.
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public DataManager(string ownerId, string id)
            : base(ownerId, id)
        {
            this.init();
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

        #region method

        /// <summary>
        ///0. 초기 Initialize 메소드
        ///  :처음 manager가 생성될 경우에 반송 정도 등록
        ///   -> TB_TRANSFER 테이블에 미반송 정보를 읽어서 Que에 저장함.
        ///   -> TB_TRANSFERDETAIL을 확인하여 반송 쪼개기 여부 변경
        ///  :Batch Job 구동
        ///   -> SAP I/F 반송 지시 Check timer 구동
        ///   -> 세부 반송 지시 Check timer 구동
        /// </summary>

        protected void init()
        {
            this.Load();

           if(GlobalConstant.START_MODE != EntityEnumData.STARTMODE.Client.ToString() )
           {
                this.startBatchJob();
           }
        }

        /// <summary>
        /// Batch Job 구동 메소드
        ///   -> SAP I/F 반송 지시 Check timer 구동
        ///   -> 세부 반송 지시 Check timer 구동
        /// </summary>
        protected void startBatchJob()
        {
            timerDataPurge.Interval = 3600000;
            timerDataPurge.Start();

            //checkDataPurge
            timerDataPurge.Elapsed += new ElapsedEventHandler(checkDataPurge);
        }

        void checkDataPurge(object sender, ElapsedEventArgs e)
        {
            DBConnection theDB = null;

            //timer Flag가 false 일 경우에만 진행함.
            if (!timerDataPurgeFlag)
            {
                //timer 시작 Flag True
                timerDataPurge.Stop();
                timerDataPurgeFlag = true;

                try
                {
                    purgeData();
                }
                catch (Exception ex)
                {
                    //Error Check
                }
                finally
                {
                    //timer 종료 Flag false
                    timerDataPurgeFlag = false;
                    timerDataPurge.Start();
                }
            }
        }

        //DB에서 반송을 조회해서 Que에 Setting함.
        public static void purgeData()
        {
            List<CodeMasterData> dataList = new List<CodeMasterData>();

            try
            {
                CodeMasterData codeData = new CodeMasterData();
                dataList = codeData.getPurgeTableList();
                
                foreach (CodeMasterData codeTemp in dataList)
                {
                    DBConnection theDB = null;
                    string sql = "";
                    string strTable = codeTemp.Code;
                    string strCol = codeTemp.Attribute1;
                    string strInterval = codeTemp.Attribute2;

                    TimeSpan delTime = TimeSpan.FromDays(double.Parse(strInterval));
                    string strTime = DateTime.Now.Subtract(delTime).ToString("yyyyMMddHHmmssfff");

                    sql += "" +
                           " DELETE " + strTable + "    " +
                           "  WHERE " + strCol   + " < '" + strTime + "' ";
                    
                    try
                    {
                        theDB = DBControl.GetConnection();
                        theDB.ExecuteUpdate(sql);

                    } catch(Exception se)
                    {
                        if (theDB != null) theDB.Rollback();
                    }
                    finally
                    {
                        if (theDB != null)
                        {
                            DBControl.RemoveDBConection(theDB);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        #endregion
    }
}
