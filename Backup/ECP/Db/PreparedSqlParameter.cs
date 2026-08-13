using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Data;
using System.Data.SqlClient;

namespace ECP.Db
{
    /// <summary>
    /// Data에 Data를 Binding으로 처리 하기 위하여 사용되어지는 SqlParmeter의 Container Class
    /// </summary>
    public class PreparedSqlParameter
    {
        /// <summary>
        /// binding을 위한 sqlparmeter를 담고 있는 Container, List로 구현됨
        /// </summary>
        List<SqlParameter> sqlParameters = null;

        #region Constructor

        /// <summary>
        ///  PreparedSqlParameter의 생성자
        /// </summary>
        public PreparedSqlParameter()
        {
            sqlParameters = new List<SqlParameter>();
        }
        #endregion Constructor

        #region Method

        /// <summary>
        /// SqlParameter를 담고 있는 Container를 Return함
        /// </summary>
        /// <returns>SqlParameter의 List</returns>
        public List<SqlParameter> GetParameters()
        {
            return sqlParameters;
        }

        /// <summary>
        /// 주어진 SqlCommand에 Container에 저장된 SqlParamter를 일괄 저장하는 Method
        /// </summary>
        /// <param name="cmd">SqlParameter를 저장하고자하는 SqlCommand</param>
        public void AddParameterToCommand(SqlCommand cmd)
        {
            //for (int i = 0; i < sqlParameters.Count; i++)
            //{
            //    cmd.Parameters.Add(sqlParameters[i]);
            //}
            foreach (SqlParameter sql in sqlParameters)
            {
                cmd.Parameters.Add(sql);
            }
        }

        /// <summary>
        /// Table의 특정 Column에 주어진 SqlDbType을 저장하기 위한 Method
        /// 이값들은 내부 SqlParameter의 Container에 저장됨
        /// </summary>
        /// <param name="columnId">SqlParameter에 대응하는 Column Id</param>
        /// <param name="dbType">Column의 Data Type</param>
        /// <param name="value">저장하고자 하는 paramter 값</param>
        private void AddParameter(string columnId, SqlDbType dbType, object value)
        {
            SqlParameter param = new SqlParameter("@" + columnId, dbType);
            param.SourceColumn = columnId;
            param.SourceVersion = DataRowVersion.Current;
            param.Direction = ParameterDirection.Input;
            param.Value = value;
            sqlParameters.Add(param);
        }

        public void SetBigInt(string columnId, System.Int64 value)
        {
            AddParameter(columnId, SqlDbType.BigInt, value);
        }

        public void SetBinary(string columnId, System.Array value)
        {
            AddParameter(columnId, SqlDbType.Binary, value);
        }

        public void SetBit(string columnId, System.Boolean value)
        {
            AddParameter(columnId, SqlDbType.Bit, value);
        }

        public void SetChar(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.Char, value);
        }

        public void SetDateTime(string columnId, System.DateTime value)
        {
            AddParameter(columnId, SqlDbType.DateTime, value);
        }
        public void SetDecimal(string columnId, System.Decimal value)
        {
            AddParameter(columnId, SqlDbType.Decimal, value);
        }
        public void SetFloat(string columnId, System.Double value)
        {
            AddParameter(columnId, SqlDbType.Float, value);
        }
        public void SetImage(string columnId, System.Array value)
        {
            AddParameter(columnId, SqlDbType.Image, value);
        }

        public void SetInt(string columnId, System.Int32 value)
        {
            AddParameter(columnId, SqlDbType.Int, value);
        }

        public void SetMoney(string columnId, System.Decimal value)
        {
            AddParameter(columnId, SqlDbType.Money, value);
        }
        public void SetNChar(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.NChar, value);
        }
        public void SetNText(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.NText, value);
        }
        public void SetNVarChar(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.NVarChar, value);
        }
        public void SetReal(string columnId, System.Single value)
        {
            AddParameter(columnId, SqlDbType.Real, value);
        }
        public void SetSmallDateTime(string columnId, System.DateTime value)
        {
            AddParameter(columnId, SqlDbType.SmallDateTime, value);
        }
        public void SetSmallInt(string columnId, System.Int16 value)
        {
            AddParameter(columnId, SqlDbType.SmallInt, value);
        }
        public void SetSmallMoney(string columnId, System.Decimal value)
        {
            AddParameter(columnId, SqlDbType.SmallMoney, value);
        }
        public void SetText(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.Text, value);
        }

        public void SetTimestamp(string columnId, System.Array value)
        {
            AddParameter(columnId, SqlDbType.Timestamp, value);
        }
        public void SetTinyInt(string columnId, System.Byte value)
        {
            AddParameter(columnId, SqlDbType.TinyInt, value);
        }
        public void SetUdt(string columnId, SqlDbType value)
        {
            AddParameter(columnId, SqlDbType.Udt, value);
        }
        public void SetUniqueIdentifier(string columnId, System.Guid value)
        {
            AddParameter(columnId, SqlDbType.UniqueIdentifier, value);
        }

        public void SetVarBinary(string columnId, System.Array value)
        {
            AddParameter(columnId, SqlDbType.VarBinary, value);
        }
        public void SetVarChar(string columnId, System.String value)
        {
            AddParameter(columnId, SqlDbType.VarChar, value);
        }
        public void SetVariant(string columnId, SqlDbType value)
        {
            AddParameter(columnId, SqlDbType.Variant, value);
        }
        public void SetXml(string columnId, string value) /// Should be fixed
        {
            AddParameter(columnId, SqlDbType.Xml, value);
        }

        /// <summary>
        /// Table의 특정 Column에 주어진 SqlDbType을 저장하기 위한 Method
        /// 이값들은 내부 SqlParameter의 Container에 저장됨
        /// </summary>
        /// <param name="columnId">SqlParameter에 대응하는 Column Id</param>
        /// <param name="dbType">Column의 Data Type</param>
        /// <param name="value">저장하고자 하는 paramter 값</param>
        private void AddProcedureParameter(string parameterName, SqlDbType dbType, 
                                           int size, ParameterDirection parameterDirection,
                                           string value)
        {
            SqlParameter param = new SqlParameter("@" + parameterName, dbType);

            if (size != 0)
            {
                param.Size = size;
            }

            param.Direction = parameterDirection;
            
            if (!(string.IsNullOrEmpty(value)))
            {
                param.Value = value;
            }
            sqlParameters.Add(param);
        }

        public void SetProcedureParameter(string parameterName, SqlDbType dbType,
                                          int size, ParameterDirection parameterDirection,
                                          string value)
        {
            AddProcedureParameter(parameterName, dbType, size, parameterDirection, value);
        }

        #endregion Method

    }
}
