using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;

using ECP.Global;
using ECP.Service.Logger;
using ECP.Db.Entity;

namespace ECP.Db
{
    /// <summary>
    /// DBControl은 Static 객체이며, 제품에 의하여 사용되어지는 DBConnection에대한 Control를 가지고 있음
    /// Connection Pool은 당초 Applcation Level에서 Pool로 관리 하려 했으나,
    /// .NET Level에서 Pool로서 관리되는 것으로 확인되어. Framework의 Pool 메카니즘을 이용하기로 함
    /// </summary>
    public partial class DBControl : ECPObject
    {

        /// <summary>
        ///  Class 이름
        /// </summary>
        const string CLASS = "DBControl";

        /// <summary>
        /// Data Base Connetion의 연결 정보를 담고 있는 String
        /// </summary>
        private static string dbConnectionString = string.Empty;

        //시스템 Startmode
        private static string startModeString = "Client";

        /// <summary>
        /// Data Base Default Query Time out , 초단위
        /// </summary>
        public const int DEFAULT_QUERY_TIMEOUT = 60;


        #region Constructor

        /// <summary>
        /// Stiatic Initializer.
        /// App.Config 화일의 <connectionStrings> tag의 DataBaseConnectionString의 값을 사용하여
        /// Class 전역 Property인 dbConnectionString의 값을 초기화 함
        /// </summary>
        static DBControl()
        {
            const string METHOD = CLASS + ".DBControl";
            ConnectionStringSettings setting = null;

            if (GlobalConstant.START_MODE == EntityEnumData.STARTMODE.Client.ToString())
            {
                setting = ConfigurationManager.ConnectionStrings["GlobalDataBaseConnectionString"];
            }
            else
            {
                setting = ConfigurationManager.ConnectionStrings["DataBaseConnectionString"];
            }
            
            if (setting != null)
            {
                dbConnectionString = setting.ConnectionString;
            }
            else
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "ERROR: Attempt to connect to database," +
                    "but no database connectionString is configured on this computer.");

                /// I feel that we need to have some logging way to leave message to file system instead of throw except 
                /// That will stop the program
                throw new Exception("ERROR: Attempt to connect to database, but no database connectionString is " +
                "configured on this computer.");
            }
        }

        public DBControl()
        {
            //const string METHOD = CLASS + ".constructor";
        }
        #endregion Constructor

        #region DB_Handle_Method


        /// <summary>
        /// Data base와dml private Connection을 Establish하기 위하여 사용되어짐
        /// App.config에 의해 초기화된 Class Property인 dbConnectionString을 이용하여 
        /// Connection을 가져옴
        /// </summary>
        /// <returns>Reserved SqlConnection</returns>
        public static DBConnection GetConnection()
        {
            SqlConnection sqlDB = MakeDBConnection(dbConnectionString);
            return new DBConnection(sqlDB);
        }

        /// <summary>
        /// Data base와 private Connection을 Establish하기 위하여 사용되어짐
        /// Mehtod Parameter로 제공되어진 dbConnectionString에 의하여 Connection을 가져옴
        /// </summary>
        /// <param name="dbConnectionString">Data base Connection을 위한 String 정보</param>
        /// <returns>DBConnection</returns>
        public static DBConnection GetConnection(string dbConnectionString)
        {
            SqlConnection sqlDB = MakeDBConnection(dbConnectionString);
            return new DBConnection(sqlDB);

        }

        /// <summary>
        /// ADO의 SqlConnection 객체로 생성함, 새로이 생성되어진 SqlConnection이 Return됨
        /// </summary>
        /// <param name="dbConnectionString">Data base Connection을 위한 String 정보</param>
        /// <returns>SqlConnection</returns>
        private static SqlConnection MakeDBConnection(string dbConnectionString)
        {
            return new SqlConnection(dbConnectionString);
        }

        /// <summary>
        /// 주어진 DBconnection 자원을 해지함
        ///  
        /// </summary>
        /// <param name="dc">삭제하고자하는 DBConnection</param>
        public static void RemoveDBConection(DBConnection dc)
        {
            dc.Close();
            dc = null;
        }

        /// <summary>
        /// 단일 sql 문장을 실행함. 이 Method는 Application Default Timeout을 사용하며,
        /// SqlConnection을 1회 획득후 sql문장을 실행후, 바로 Connection을 Close함
        /// </summary>
        /// <param name="sqlstring">실행하고자 하는 sql문</param>
        /// <returns></returns>
        public static bool Execute(string sqlstring)
        {
            return ExecuteWithTimeout(sqlstring, DBControl.DEFAULT_QUERY_TIMEOUT);
        }

        /// <summary>
        /// 단일 sql 문장을 실행함. 
        /// SqlConnection을 1회 획득후 sql문장을 실행후, 바로 Connection을 Close함
        /// </summary>
        /// <param name="sqlstring">실행하고자 하는 sql문</param>
        /// <param name="timeout">Timeout 단위 초</param>
        /// <returns>SqlException이 발생하지 않으면 True</returns>
        public static bool ExecuteWithTimeout(string sqlstring, int timeout)
        {
            const string METHOD = CLASS + ".executeWithTimeout";

            DBConnection connection = null;
            bool rtn = true;
            try
            {
                connection = GetConnection();
                connection.ExecuteUpdate(sqlstring, timeout);
                rtn = true;
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", " sql exception. execute sql with timeout");
                rtn = false;
                throw ex;
            }
            finally
            {
                connection.Close();
            }
            return rtn;

        }

        #endregion DB_Handle_Method


    }
}

