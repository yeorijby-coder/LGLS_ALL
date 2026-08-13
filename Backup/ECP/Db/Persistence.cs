/*
 *  LG CNS Copy Right Reserved since 2008 
 */
using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using ECP.Global;
using ECP.Service.Logger;

namespace ECP.Db
{

    /// <summary>
    /// Persist Class는 Abstarct Class, SubClass에 의하여 상속되어져 사용되어짐
    /// 이 Class의 목적은 하위 Class로 하여금, SubClass의 Persist되어질  
    /// Property를 SqlConnection을 생성 및 삭제, Transaction처리 등과 같은 
    /// Sql 기본 조작 없이 일관성 있게 해당 Table에 Query 및 Update작업을 
    /// 수행 가능하도록하는데 있음
    /// 
    /// SubClass는 Virtual Method인 Add(), Delete(), Load(), Save()을
    /// Override해야 하며, Override한 Method내에서, Persistence가 필요한
    /// Property에 한하여, Bind를 위하여 PreparedSqlParameter 값을 채운후,
    /// Sql 문장을 작성해야 함
    /// 
    /// 이 Class는 계층적으로 아래로 많은 Level로 내려가며 상속 될수 있으며,
    /// 이것은, 객체가 더욱 구체화 되어로 계층적으로 아래 Level로 내려 가는 것을
    /// 의미하며, 각 Level별로 1 Sub Class당 1개 Table로 객체 지향적 Data모델링 될 수 있으며,
    /// 
    /// 다르게, 1 Table에 Colum을 늘려 가며 Data가 Persistence될 수 있다.
    /// 이것은 프로그램의 복잡도 및 성능과 연관될 수 있으므로
    /// 신중하게 판단하여, 좋은 선택이 될 수 있어야 한다.
    /// 
    /// 또한, Sql 문장이 Application Code와 분리하지 않은 의도는 
    /// 실시간 제어를 필요로 자동 제어 관련 Applcation은 Data 처리 위주의
    /// 비지니스 처리 Program과는 달리 Sql 문장의 변경이 자주 발생하지 않으며,
    /// 장비의 모델링 정보 위주로 Data를 Persist함으로, 
    /// 문맥내에 Sql문장이 있는 것이 적절하다고 판단됨을 근거로 한다.
    /// 
    /// 즉각적인 Property의 Persist가 필요하지 않은 경우
    /// SetDirtyFlag, SaveAsDirty을 사용하여, 적절한 시점에
    /// Property의 변화를 Data base에 반영 할 수 있다.
    /// 
    /// </summary>
    [Serializable]
    public abstract class Persistence : ECPObject
    {

        /// <summary>
        /// Class Name
        /// </summary>
        const string CLASS = "Persistence";

        /// <summary>
        /// 이 delayPersistenctCount가 0이 될경우, persistenct data는 database에 쓰여 져야 한다.
        /// </summary>
        private int delayPersistenctCount =0;


        #region Manage_Persistent

        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected virtual void Add(DBConnection theDB)
        {
        }

        /// <summary>
        /// Data를 data base에 Add함
        /// <see cref="Add(DBConnection theDB)"/>
        /// </summary>
        public void Add()
        {
            const string METHOD = CLASS + ".Add";

            DBConnection theDB = null;

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();
                Add(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db insert error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

                if (theDB != null) theDB.Rollback();
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }

        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected virtual void Delete(DBConnection theDB)
        {
        }


        /// <summary>
        /// Database로 부터 data를 Delete
        /// 
        /// <seealso cref="Delete(DBConnection theDB)"/>
        /// 
        /// </summary>
        public void Delete()
        {
            const string METHOD = CLASS + ".Delete";

            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();
                Delete(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db delete error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

                if (theDB != null) theDB.Rollback();
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }

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
        protected virtual void Load(DBConnection theDB)
        {
        }

        /// <summary>
        /// Data base로 부터 Data를 Load함. 
        /// <seealso cref="Load(DBConnection theDB)"/>
        /// </summary>
        public void Load()
        {
            const string METHOD = CLASS + ".Load";

            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                //theDB.start();
                Load(theDB);
                //theDB.commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db select error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
                //theDB.rollback();
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }

        /// <summary>
        /// Data base로 Data를 Save함
        /// Sub Class는 이 Method를 Override하여 Implement하여야 하며,
        /// 항상 base.Save() 먼저 Call되어 상속 계층에 있는 Parent Class가 먼저 
        /// 저장되도록 하여야 함
        /// </summary>
        /// <param name="theDB"></param>
        protected virtual void Save(DBConnection theDB)
        {

        }

        /// <summary>
        /// Data를 Data base에 저장함
        /// <seealso cref="Save(DBConnection theDB)"/>
        /// </summary>
        public void Save()
        {
            const string METHOD = CLASS + ".save";


            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();
                Save(theDB);
                theDB.Commit();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db save error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

                if (theDB != null) theDB.Rollback();
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }
         
        /// <summary>
        /// 지정한 데이터베이스에 있는 데이터 및 로그 파일의 크기를 축소합니다.
        /// </summary>
        public void DbCompact()
        {
            const string METHOD = CLASS + ".DbCompact";


            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                theDB.DbCompact();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "db Compact error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());

            }

        }

        #endregion Manage_Persistent

        #region Method


        /// <summary>
        /// Persistence가 잠시 지연되어져야 함을 나타 내기 위하여 사용되어짐
        /// 항상 true에 Matching되는 false가 있어야 함
        /// </summary>
        /// <param name="flag">ture 또는 false Persistence가 지연되는가 아닌가를 나타내는 flag </param>
        public void DelayPersistance(bool flag)
        {
            delayPersistenctCount += flag ? 1 : -1;

            if (delayPersistenctCount < 0)
            {
                // leave error log 
            }
        }

        /// <summary>
        /// Persitence가 지연되고 있는 가를 검사하기 위한 Method
        /// delayPersistenctCount가 0보다 클 경우 지연된 상태임
        /// </summary>
        /// <returns></returns>
        public bool PersistenceIsDelayed()
        {
            return (delayPersistenctCount > 0);
        }


        /// <summary>
        /// Sub Class의 Persistence 되어져야 한 Property가 변경되어졌으며,
        /// 이것이 Data Base에 반영되어 져야 함을 알려주는 Method임
        /// 
        /// Sub Class의 상화에 따라 맞게 Override되어져 사용되어야 함
        /// </summary>
        protected virtual void SetDirtyFlag()
        {
        }

        /// <summary>
        /// Dirty Flag을 Set한 후 Save을 Call하는 Method
        /// </summary>
        public void SaveAsDirty()
        {
            SetDirtyFlag();
            Save();
        }

        #endregion Method

    }

}
