using System;
using System.Collections.Generic;
using System.Text;
#if POSTGRESQL
using NpgsqlTypes; // [LGLS] POSTGRESQL 전용
#endif
using System.Data.OleDb;

namespace WCS_TASK_CV
{
    class DbLang
    {

#if ORACLE
        public const string NVL = "NVL";
        public const string SYSDATE = "SYSDATE";
        public const string II = "||";
        public const OleDbType VARCHAR = OleDbType.VarChar;
        public const OleDbType INT = OleDbType.Integer;
        public static string TO_NUMBER(string strVALUE)
        {
            string strRtnValue;
            strRtnValue = "TO_NUMBER(" + strVALUE + ")";
            return strRtnValue;
        }
#elif POSTGRESQL
        public const string NVL = "COALESCE";
        public const string SYSDATE = "NOW()";
        public const string II = "||";
        public const NpgsqlDbType VARCHAR = NpgsqlDbType.Varchar;
        public const NpgsqlDbType INT = NpgsqlDbType.Integer;
        public static string TO_NUMBER(string strVALUE) 
        { 
            string strRtnValue;
            strRtnValue = strVALUE + "::integer";
            return strRtnValue;
        }
#elif SQL
        // [LGLS] MS SQL Server 방언 (IO_TASK DbLang 기준으로 보완)
        public const string NVL = "ISNULL";
        public const string SYSDATE = "GETDATE()";
        public const string II = "+";
        public const System.Data.SqlDbType VARCHAR = System.Data.SqlDbType.VarChar;
        public const System.Data.SqlDbType INT = System.Data.SqlDbType.Int;
        public static string TO_NUMBER(string strVALUE)
        {
            string strRtnValue;
            strRtnValue = "CAST(" + strVALUE + " AS INT)";
            return strRtnValue;
        }
#endif
    }
}
