
using Microsoft.VisualBasic;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
//using System.Data.OleDb;
using System.Diagnostics;
using System.Windows.Forms;
using Npgsql;


#if oracle
using System.Data.OleDb;
#endif

#if postgresql
using Npgsql;
#endif

#if mssql
using System.Data.SqlClient;
#endif

//*** DB 기초 클래스 **************************************************************************
//* DB 객체들과 실행 함수들을 정의
namespace WmsUp
{
	public class CBaseDb
	{
#if oracle
		// 외부에서 할당해주는 Connection 개체 참조(Connection이 하나 일 경우)

		public OleDbConnection conMain;
		// 자체 생성되는 DB 객체들
		public OleDbTransaction trnMain;
		public OleDbCommand comMain = new OleDbCommand();
		//Private daMain As New OleDbDataAdapter
		public OleDbDataAdapter daMain = new OleDbDataAdapter();

#endif
#if mssql
	// 외부에서 할당해주는 Connection 개체 참조(Connection이 하나 일 경우)

	public SqlConnection conMain;
	// 자체 생성되는 DB 객체들
	public SqlTransaction trnMain;
	public SqlCommand comMain = new SqlCommand();
   	private SqlDataAdapter daMain = new SqlDataAdapter();
#endif
#if postgresql
        public NpgsqlConnection conMain;
        // 자체 생성되는 DB 객체들
        public NpgsqlTransaction trnMain;
        public NpgsqlCommand comMain = new NpgsqlCommand();
        //Private daMain As New OleDbDataAdapter
        public NpgsqlDataAdapter daMain = new NpgsqlDataAdapter();
#endif

        public DataTable dtMain = new DataTable("Default");
		public DataTable dtSub = new DataTable("Default");

#if SERVER_PROGRAM
    Public DataTable dtLugDtl = new DataTable("Default");
	public DataTable dtMove = new DataTable("Default");
	public DataTable dtPkInf = new DataTable("Default");
    public DataTable dtSub = new DataTable("Default");
#endif

		// 바인딩 객체에 사용할지 여부
		// 바인딩 객체일 경우 Reset을 하면 안됨.(계속 연결된 상태, DataSource에 따라 작동)

		public bool blBindingType;
		// DB 에러
		public const int DB_ERR = -1;
		// DB 에러중 DB Lock
		public const int DB_LOCK = -2;
		// DB 에러중 중복 데이타

		public const int DB_DUP = -3;
		// DB Error Message
		public string strErrMsg = "";

		public bool blTran;
		public string ErrMsg
		{
			get
			{
				return strErrMsg;
			}

			set
			{
				strErrMsg = value;
			}
		}


		// DB Error 종류
		private int nErrKind = 0;
		public string ErrKind
		{
			get
			{
				return nErrKind.ToString();
			}
		}

		// 자체 Connection 객체 사용
		// 외부에서 New 생성 후 Connection 객체를 Open하고 Init을 호출한다.
		// 종료시 comMain.Close를 반드시 호출
		public CBaseDb(bool p_blBind = false)
		{
			blBindingType = p_blBind;
		}

		// 외부에서 Connection 객체 정의 (pc cliient 처럼 , 하나를 쓸 경우)
#if oracle
		public CBaseDb(ref OleDbConnection p_conObj, bool p_blBind = false)
#endif

#if postgresql
        public CBaseDb(ref NpgsqlConnection p_conObj, bool p_blBind = false)
#endif


#if mssql 
    public CBaseDb(ref SqlConnection p_conObj, bool p_blBind = false)
#endif
        {
			blBindingType = p_blBind;
			conMain = p_conObj;
			// DB init
			Init();
		}
		// DB init

		public void Init()
		{
			comMain.Connection = conMain;
			daMain.SelectCommand = comMain;

		}

		//*** DB Error Message ***
		//* 프로젝트 별로 메세지를 표시하는 방법을 패생 클래스에서 오버라이드 해서 사용한다.
		public virtual void ShowErrMsg(bool p_blMsgBox, string p_strERR = "DB")
		{
			if (p_blMsgBox)
			{
				MessageBox.Show(ErrMsg, "DB Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		public virtual void ShowErrMsg_Call(string p_strCALL, bool p_blMsgBox, string p_strERR = "DB")
		{
			if (p_blMsgBox)
			{
				MessageBox.Show(ErrMsg, "DB Error[" + p_strCALL + "]", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		public void Reset_dtMain()
		{
			dynamic icnt = null;
			if (blBindingType)
			{
				dtSub.Clear();
			}
			else
			{
				dtSub.Reset();
			}
			icnt = daMain.SelectCommand.Parameters.Count;

		}

		//*********************************************************************************
		// 쿼리 실행 For Select..  
		// Parameter: bMsgBox (메세지 박스 표시 여부)
		//            bRerutnErr (에러 발생시, 에러를 Return할 지 여부)
		// Return: 성공 - 쿼리한 레코드 수 (양의 정수)
		//         실패 - DB_ERR(-1):  일반 DB Err
		//         실패 - DB_LOCK(-2): DB Lock
		//         실패 - DB_DUP(-3):  데이타 중복
		public int ExcuteQry(string strQry, bool bMsgBox = true, bool bReturnErr = false)
		{
		
			try {
				// 바인딩 객체일 경우 연결유지, DATA만 클리어
				if (blBindingType)
				{
					dtMain.Clear();
				} else {
					dtMain.Reset();
				}
		
				// Sql_Display(strQry)
				string msgStr = modCom.ValidateInput(strQry);
				if (!string.IsNullOrEmpty(msgStr))
					throw new Exception(msgStr);
		
				comMain.CommandType = CommandType.Text;
				comMain.CommandText = strQry;
				return daMain.Fill(dtMain);
		
#if oracle
				} catch (OleDbException DbErr) { 
#elif mssql
				} catch (SqlException DbErr) {
#elif postgresql
				} catch (NpgsqlException DbErr) {
#endif

                                                   if ((bReturnErr)) {
					throw DbErr;
				} else {
					strErrMsg = DbErr.Message;
					// & CRLF & DbErr.StackTrace
					if (strErrMsg.IndexOf("ORA-00054") != -1 || strErrMsg.IndexOf("ORA-30006") != -1) {
						// No wait 를 사용할 경우
						nErrKind = DB_LOCK;
					} else {
						nErrKind = 0;
					}
					ShowErrMsg(bMsgBox);
				}
			} catch (Exception AppErr) {
				if ((bReturnErr)) {
					throw AppErr;
				} else {
					strErrMsg = AppErr.Message;
					nErrKind = 0;
					ShowErrMsg(bMsgBox);
				}
			}
		
			return DB_ERR;
		
		}

		//*********************************************************************************
		// 쿼리 실행 For Select.., 2개 이상 쿼리를 할 경우 datatable을 별도로 바인딩 한다.  
		// Parameter: bRerutnErr (에러 발생시, 에러를 Return할 지 여부)
		// Return: 성공 - 쿼리한 레코드 수 (양의 정수)
		//         실패 - DB_ERR(-1):  일반 DB Err
		//         실패 - DB_LOCK(-2): DB Lock
		//         실패 - DB_DUP(-3):  데이타 중복
		public int ExcuteQry(ref DataTable dtOther, string strQry, bool bMsgBox = true, bool bReturnErr = false)
		{
			try {
				// 바인딩 객체일 경우 연결유지, DATA만 클리어
				if (blBindingType) {
					dtOther.Clear();
				} else {
					dtOther.Reset();
				}
		
				// Sql_Display(strQry)
		
				string msgStr = modCom.ValidateInput(strQry);
				if (!string.IsNullOrEmpty(msgStr))
					throw new Exception(msgStr);
		
				comMain.CommandType = CommandType.Text;
				comMain.CommandText = strQry;
				return daMain.Fill(dtOther);
		
#if oracle
				} catch (OleDbException DbErr) {
#elif mssql
				} catch (SqlException DbErr) {	
#elif postgresql
				} catch (NpgsqlException DbErr) {
#endif

                                                   if ((bReturnErr)) {
					throw DbErr;
				} else {
					strErrMsg = DbErr.Message;
					// & CRLF & DbErr.StackTrace
					if (strErrMsg.IndexOf("ORA-00054") != -1 || strErrMsg.IndexOf("ORA-30006") != -1) {
						// No wait 를 사용할 경우
						nErrKind = DB_LOCK;
					} else {
						nErrKind = 0;
					}
					ShowErrMsg(bMsgBox);
				}
			} catch (Exception ex) {
				strErrMsg = ex.Message;
				ShowErrMsg(bMsgBox);
			}
		
			return DB_ERR;
		
		}
		
		//*********************************************************************************
		// None Query For insert, update, ...
		// Parameter: bRerutnErr (에러 발생시, 에러를 Return할 지 여부)
		// Return: 성공 - 반영된 레코드 수 (양의 정수)
		//         실패 - DB_ERR(-1):  일반 DB Err
		//         실패 - DB_LOCK(-2): DB Lock
		//         실패 - DB_DUP(-3):  데이타 중복
		public int ExcuteNonQry(string strQry, bool bMsgBox = true, bool bReturnErr = false)
		{
			try {
				//Sql_Display(strQry)
		
				string msgStr = modCom.ValidateInput(strQry);
				if (!string.IsNullOrEmpty(msgStr))
					throw new Exception(msgStr);
		
				comMain.CommandType = CommandType.Text;
				comMain.CommandText = strQry;
				return comMain.ExecuteNonQuery();
		
#if oracle
			}catch (OleDbException DbErr){
#elif mssql
			}catch (SqlException DbErr) {
#elif postgresql
				} catch (NpgsqlException DbErr) {
#endif

                                             if ((bReturnErr)) {
					throw DbErr;
				} else {
					strErrMsg = DbErr.Message;
					// & CRLF & DbErr.StackTrace
					if (strErrMsg.IndexOf("ORA-00054") != -1 || strErrMsg.IndexOf("ORA-30006") != -1) {
						// No wait 를 사용할 경우
						nErrKind = DB_LOCK;
					} else {
						nErrKind = 0;
					}
					ShowErrMsg(bMsgBox);
				}
			} catch (Exception ex) {
				strErrMsg = ex.Message;
				ShowErrMsg(bMsgBox);
			}
		
			return DB_ERR;
		}
		
		public void BeginTrans(bool p_blDistributedTrans = false)
		{
			//*********************************************************************************
			// Transction 객체 할당
			try {
				if (conMain.State != ConnectionState.Open) {
					conMain.Open();
				}
				trnMain = conMain.BeginTransaction() ;
				comMain.Transaction = trnMain;
				blTran = true;
		
		
			} catch (Exception AppErr) {
				throw AppErr;
			}
		}

		
		public void RollbackTrans(bool p_blDistributedTrans = false)
		{
			 try{
				//If g_conDb.State <> ConnectionState.Closed Then
				//    g_conDb.Close()
				//    blTran = False
				//End If
				if (blTran == true)
					trnMain.Rollback();
				blTran = false;
		
				//trnMain.Rollback()
			} catch (Exception AppErr) {
				// Throw AppErr
				modCmWork.ShowError(AppErr.Message, "Rollback");
			}
		}

		public void CommitTrans(bool p_blDistributedTrans = false)
		{	try{
				if (blTran == true)
					trnMain.Commit();
				blTran = false;
		
				//trnMain.Commit()
			} catch (Exception AppErr) {
				throw AppErr;
			}
		}

		public int ExecuteQuery(ref CUserDb Bdb, string sql)
		{
			return Bdb.ExcuteQry(sql, false, true);
		}
	}
}
