using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Data;
using System.Data.SqlClient;

using ECP.Global;
using ECP.Service.Logger;


namespace ECP.Db
{
    /// <summary>
    /// DB connection class, data base에 query 및 Update을 위한 class
    /// 
    /// .NET에서 제공하는 SqlConnection, SqlTransaction,PreparedSqlParameter,SqlDataReader을
    /// Project 전반에 걸쳐서 일관성 있게 사용하기위한 것이 이 Class의 주 목적임
    /// 
    /// </summary>
    public partial class DBConnection : ECPObject
    {
        /// <summary>
        ///  Class Name을 담고 이는 property
        /// </summary>
        private const string CLASS = "DBConnection";

        /// <summary>
        /// General Query에대한 Default Time은 60임
        /// </summary>
        public const int GENERAL_QUERY_TIMEOUT = 60;

        /// <summary>
        /// data base에 대한 sql connection 객체
        /// </summary>
        private SqlConnection sqlConnection = null;

        /// <summary>
        /// 새롭게 수행되어진 data base  transaction에 객체
        /// </summary>
        private SqlTransaction sqlTranaction = null;

        /// <summary>
        /// SQL Query의 수행 결과 Return 되어진 reader 객체, 이 객체를 통하여
        /// Application은 data base의 cloumn을 읽을 수 있다.
        /// SQL Server 데이터베이스에서 행의 앞으로만 이동 가능한 스트림을 읽을 수 있음
        /// </summary>
        private SqlDataReader reader = null;

        #region Constructor

        public DBConnection()
        {
        }
        #endregion Constructor

        #region DB_Handling_Method

        /// <summary>
        ///  주어진 sqlconnection으로 connection을 생성함
        /// </summary>
        /// <param name="sqlConnection"> sql connection </param>
        public DBConnection(SqlConnection sqlConnection)
        {
            this.sqlConnection = sqlConnection;
            Open();
        }

        /// <summary>
        /// 이 Method는 DBControl 객체로 부터 생성되어진 SqlConnection 객체를 Return함
        /// </summary>
        /// <returns></returns>
        public SqlConnection GetSqlConnection()
        {
            return sqlConnection;
        }

        /// <summary>
        ///  System.Data.SqlClient.SqlConnection.ConnectionString이 지정하는 속성 설정을 사용하여
        ///  데이터베이스 연결함
        /// </summary>
        public void Open()
        {
            const string METHOD = CLASS + ".open";

            try
            {
                if (sqlConnection != null)
                {
                    sqlConnection.Open();
                }
            }
            catch (SqlException ex)
            {

                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base open error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
            }
        }

        /// <summary>
        ///  Data base와의 sql connection을 close함
        /// </summary>
        public void Close()
        {
            const string METHOD = CLASS + ".close";
            try
            {
                if (sqlConnection != null && sqlConnection.State != ConnectionState.Closed)
                {
                    sqlConnection.Close();
                    sqlConnection.Dispose();
                    sqlConnection = null;
                }
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base close error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
            }
        }

        // Transaction Control Functionality

        /// <summary>
        ///  data base transaction을 start함 
        /// </summary>
        public void Start()
        {
            const string METHOD = CLASS + ".start";
            try
            {
                if (sqlConnection != null)
                    sqlTranaction = sqlConnection.BeginTransaction();
            }
            catch (InvalidOperationException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base Invalid exception error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
            }
        }

        /// <summary>
        /// Transaction을 Commit
        /// </summary>
        public void Commit()
        {
            const string METHOD = CLASS + ".commit";

            CloseReader();
            try
            {
                if (sqlConnection != null)
                    sqlTranaction.Commit();
            }
            catch (InvalidOperationException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base Invalid exception error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
            }
        }

        /// <summary>
        /// Transaction을 Roll back
        /// </summary>
        public void Rollback()
        {
            const string METHOD = CLASS + ".rollback";

            CloseReader();
            try
            {
                if (sqlConnection != null)
                    sqlTranaction.Rollback();
            }
            catch (InvalidOperationException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base Invalid exception error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
            }
        }

        /// <summary>
        /// SqlCommand에 대한 매개 변수을 담고 있는 List 객체로, 
        /// System.Data.DataSet 열에 대한 매개 변수의 매핑을 위하여 사용되어지는 객체를 생성하여 Return함
        /// Return되어지는 객체를 이용하여 Query나, Update시 반복적으로 이용되어지는 Attribute을 
        /// 지정하여 사용함.
        /// 
        /// PrearedSqlParameter는 Java의 PreparedStatement의 binding과 같은 역활로.
        /// Oracle 또는 SqlServer에 Bind 를 사용하지 않을 경우, 동일한 Query 나 Update문의 계속적인
        /// Parcind으로 DBMS의 Mememory Full 이 발생할 수 있음.
        /// </summary>
        /// <returns></returns>
        public PreparedSqlParameter GetPreparedSqlParameter()
        {
            return new PreparedSqlParameter();
        }

        /// <summary>
        ///  data base에 sqlstring을 보냄. INSERT, UPDATE, CREATE 와 DELETE 문에 사용
        /// </summary>
        /// <param name="sqlstring">실행되어진 sql 문장</param>
        /// <returns></returns>
        public int ExecuteUpdate(string sqlstring)
        {
            const string METHOD = CLASS + ".executeUpdate";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);

            try
            {
                return ExecuteUpdateWithTimeOut(cmd, DBConnection.GENERAL_QUERY_TIMEOUT);

            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base excuteUpdate error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString().ToString());
                throw ex;
            }
        }

        /// <summary>
        /// data base에 sqlstring을 보냄. INSERT, UPDATE, CREATE 와 DELETE 문에 사용
        /// </summary>
        /// <param name="sqlstring">실행되어진 sql 문장</param>
        /// <param name="timeout">실행되어질 문장의 Timeout 값</param>
        /// <returns> int 이 Update문에 의하여 영향을 받은 row의 갯수</returns>
        public int ExecuteUpdate(string sqlstring, int timeout)
        {
            const string METHOD = CLASS + ".executeUpdate";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);

            try
            {
                return ExecuteUpdateWithTimeOut(cmd, timeout);

            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base excuteUpdate error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }
        }


        /// <summary>
        /// data base에 SqlCommand을 보냄. INSERT, UPDATE, CREATE 와 DELETE 문에 사용
        /// </summary>
        /// <param name="cmd">실행 되어질 SqlCommand 객체</param>
        /// <param name="queryTimeout">Timeout 값 , 초 단위</param>
        /// <returns>이 실행에 의하여 영향 받는 row의 숫자</returns>
        private int ExecuteUpdateWithTimeOut(SqlCommand cmd, int queryTimeout)
        {
            const string METHOD = CLASS + ".executeUpdate";

            int i = 0;

            CloseReader();

            cmd.CommandTimeout = queryTimeout;
            if (sqlConnection != null) cmd.Transaction = sqlTranaction;

            try
            {
                i = cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base excuteUpdate error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }

            return i;
        }

        /// <summary>
        /// data base에 sqlstring 및 binding을 위한 paramter list을 보냄. INSERT, UPDATE, CREATE 와 DELETE 문에 사용
        /// 이 실행은 Default Time out으로 실행됨
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장, parameter을 담기 위한 @로 시작되는 문장이 포함되어야 함</param>
        /// <param name="paramList">Binding을 위한 Parameter list</param>
        /// <returns></returns>
        public int ExecuteUpdate(string sqlstring, PreparedSqlParameter paramList)
        {
            return ExecuteUpdate(sqlstring, paramList, GENERAL_QUERY_TIMEOUT);
        }

        /// <summary>
        /// data base에서 Procedure를 호출함
        /// </summary>
        /// <param name="sqlstring">프로시저 명</param>
        /// <param name="paramList">Binding을 위한 Parameter list</param>
        /// <param name="cmdType">Binding을 위한 Parameter list</param>
        /// <returns></returns>
        public int ExecuteUpdate(string sqlstring, PreparedSqlParameter paramList, CommandType commandType)
        {
            return ExecuteUpdate(sqlstring, paramList, commandType, GENERAL_QUERY_TIMEOUT);
        }

        /// <summary>
        /// data base에 sqlstring 및 binding을 위한 paramter list을 보냄. INSERT, UPDATE, CREATE 와 DELETE 문에 사용
        /// 이 실행은 지정된 Timeout으로 실행됨
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장, parameter을 담기 위한 @로 시작되는 문장이 포함되어야 함</param>
        /// <param name="paramList">Binding을 위한 Parameter list</param>
        /// <param name="timeout">Time out, 초단위</param>
        /// <returns></returns>
        public int ExecuteUpdate(string sqlstring, PreparedSqlParameter paramList, int timeout)
        {
            const string METHOD = CLASS + ".executeUpdate";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);
            paramList.AddParameterToCommand(cmd);

            try
            {
                return ExecuteUpdateWithTimeOut(cmd, timeout);

            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base excuteUpdate error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }
        }

        /// <summary>
        /// data base에서 Procedure를 호출함
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장, parameter을 담기 위한 @로 시작되는 문장이 포함되어야 함</param>
        /// <param name="paramList">Binding을 위한 Parameter list</param>
        /// <param name="cmdType">Binding을 위한 Parameter list</param>
        /// <param name="timeout">Time out, 초단위</param>
        /// <returns></returns>
        public int ExecuteUpdate(string sqlstring, PreparedSqlParameter paramList, CommandType commandType, int timeout)
        {
            const string METHOD = CLASS + ".executeUpdate";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);
            cmd.CommandType = commandType;

            paramList.AddParameterToCommand(cmd);

            try
            {
                return ExecuteUpdateWithTimeOut(cmd, timeout);

            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base excuteUpdate error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }
        }

        /// <summary>
        /// Data Base에 실행 되어질  sqlstring 제출, SELECT문에 사용
        /// Time out은 System Default Time out임
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장</param>
        /// <returns>Query에 의한 결과를 담고 있는 SqlDataReader</returns>
        public SqlDataReader ExecuteQuery(string sqlstring)
        {
            return ExecuteQuery(sqlstring, DBConnection.GENERAL_QUERY_TIMEOUT);

        }

        /// <summary>
        /// Data Base에 실행 되어질  sqlstring 제출, SELECT문에 사용
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장</param>
        /// <param name="timeout">Timeout, 초단위</param>
        /// <returns>Query에 의한 결과를 담고 있는 SqlDataReader</returns>
        public SqlDataReader ExecuteQuery(string sqlstring, int timeout)
        {
            const string METHOD = CLASS + ".executeQuery";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);
            try
            {
                return ExecuteQeuryWithTimeOut(cmd, timeout);
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base execute query error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }
        }

        /// <summary>
        /// Data Base에 실행 되어질  sqlstring 및 Bind을 위한 Parameter List제출, SELECT문에 사용
        /// </summary>
        /// <param name="sqlstring">실행 되어질 sql 문장</param>
        /// <param name="paramList">Binding 되어질 Parameter list</param>
        /// <returns>Query에 의한 결과를 담고 있는 SqlDataReader</returns>
        public SqlDataReader ExecuteQuery(string sqlstring, PreparedSqlParameter paramList)
        {
            return ExecuteQuery(sqlstring, paramList, DBConnection.GENERAL_QUERY_TIMEOUT);
        }

        public SqlDataReader ExecuteQuery(string sqlstring, PreparedSqlParameter paramList, int timeout)
        {
            const string METHOD = CLASS + ".executeQuery";

            SqlCommand cmd = new SqlCommand(sqlstring, sqlConnection);
            paramList.AddParameterToCommand(cmd);

            //foreach (SqlParameter para in cmd.Parameters)
            //{
            //    Console.WriteLine("Sql String:" + sqlstring); 
            //    Console.WriteLine("Sql SourceColumn:" + para.SourceColumn); 
            //    Console.WriteLine("Sql Parameter Name:" + para.ParameterName);
            //    Console.WriteLine("Sql Parameter Value:" + para.Value.ToString());
            //}

            try
            {
                return ExecuteQeuryWithTimeOut(cmd, timeout);

            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base execute query error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }
        }

        /// <summary>
        /// Data Base에 실행 되어질  SqlCommand제출, SELECT문에 사용
        /// </summary>
        /// <param name="cmd">실행 되어질 SqlCommand</param>
        /// <param name="queryTimeout">Timeout, 초단위</param>
        /// <returns>Query에 의한 결과를 담고 있는 SqlDataReader</returns>
        private SqlDataReader ExecuteQeuryWithTimeOut(SqlCommand cmd, int queryTimeout)
        {
            const string METHOD = CLASS + ".executeQeuryWithTimeOut";

            //SqlDataReader reader= null;
            CloseReader();

            cmd.CommandTimeout = queryTimeout;

            if (sqlTranaction != null) cmd.Transaction = sqlTranaction;

            try
            {
                //reader = cmd.ExecuteReader(CommandBehavior.CloseConnection);
                reader = cmd.ExecuteReader(CommandBehavior.Default);
            }
            catch (SqlException ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", "data base execute query error occuer ");
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
                // if (ex.ErrorCode == "Query Timeout") ;
                //TO-DO : add more code later
            }
            catch (Exception ex)
            {
                Log.log(1, METHOD, "error", Category.ERROR, "", "sql error", ex.Message.ToString());
                throw ex;
            }

            return reader;
        }

        /// <summary>
        /// Open되어진 SqlReader을 Close
        /// </summary>
        public void CloseReader()
        {
            if (reader != null) reader.Close();
            reader = null;
        }

        /// <summary>
        /// Data Base에 실행 되어질  sql string 및 query 대상 table, query에 대한 복사본을 저장위한 dataset제출, SELECT문에 사용
        /// 
        /// </summary>
        /// <param name="sqlstring">실행되어질 sql string ans</param>
        /// <param name="sourceTable">Query 대상 Table 명</param>
        /// <param name="dataset">Query의 결과를 복사해 담을 Dataset</param>
        /// <returns></returns>
        public int ExecuteQuery(string sqlstring, string sourceTable, DataSet dataset)
        {
            return ExecuteQuery(sqlstring, sourceTable, dataset, DBConnection.GENERAL_QUERY_TIMEOUT);
        }

        public int ExecuteQuery(string sqlstring, string sourceTable, DataSet dataset, int timeout)
        {
            SqlDataAdapter adapter = new SqlDataAdapter(sqlstring, sqlConnection);
            adapter.SelectCommand.CommandType = CommandType.Text;

            return ExecuteQeuryWithTimeOut(adapter, sourceTable, dataset, timeout);
        }

        public int ExecuteQuery(string sqlstring, string sourceTable, DataSet dataset, PreparedSqlParameter paramList)
        {
            return ExecuteQuery(sqlstring, sourceTable, dataset, paramList, DBConnection.GENERAL_QUERY_TIMEOUT);
        }

        public int ExecuteQuery(string sqlstring, string sourceTable, DataSet dataset, PreparedSqlParameter paramList, int timeout)
        {
            SqlDataAdapter adapter = new SqlDataAdapter(sqlstring, sqlConnection);
            adapter.SelectCommand.CommandType = CommandType.Text;

            paramList.AddParameterToCommand(adapter.SelectCommand);

            return ExecuteQeuryWithTimeOut(adapter, sourceTable, dataset, timeout);
        }

        /// <summary>
        /// Data Base에 실행 되어질  sql adpater 및 query 대상 table, query에 대한 복사본을 저장위한 dataset,
        /// Query Timeout제출, SELECT문에 사용
        /// </summary>
        /// <param name="adapter">sqlstring과 sqlconnection에 의하여 생성되어진 SqlDataAdapter</param>
        /// <param name="sourceTable">Query 대상 Table</param>
        /// <param name="dataset">Query의 결과의 복사 객체</param>
        /// <param name="queryTimeout">Timeout, 초단위</param>
        /// <returns>System.Data.DataTable에 성공적으로 추가했거나 새로 고친 행의 수를 반환</returns>
        private int ExecuteQeuryWithTimeOut(SqlDataAdapter adapter, string sourceTable, DataSet dataset, int queryTimeout)
        {
            CloseReader();

            adapter.SelectCommand.CommandTimeout = queryTimeout;

            if (dataset == null) return 0;
            //    dataset = new DataSet();
            if (sourceTable != null)
                return adapter.Fill(dataset, sourceTable);
            else
                return 0;
        }

        /// <summary>
        /// 지정한 데이터베이스에 있는 데이터 및 로그 파일의 크기를 축소합니다.
        /// </summary>
        public void DbCompact()
        {
            string strSQL = "";
            strSQL += string.Format("BACKUP LOG {0} WITH NO_LOG\r\n", sqlConnection.Database);
            strSQL += string.Format("DBCC SHRINKFILE (N'{0}_log' , 0, TRUNCATEONLY)\r\n", sqlConnection.Database);
            strSQL += string.Format("DBCC SHRINKDATABASE(N'{0}')\r\n", sqlConnection.Database);

            ExecuteUpdate(strSQL);
        }

        #endregion DB_Handling_Method
    }
}
