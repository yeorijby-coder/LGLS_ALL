using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;



using ECP.Db;
using ECP.Util;

using ECP.Global.Exception;
using ECP.Global;
using ECP.Service.Logger;


namespace ECP.Util.UDProperty
{
    /// <summary>
    /// User Defined Property Class. 어떤 Entity에 속해 있는 User Defined Property의 
    /// Add, delete, update, load을 위하여 사용되어짐. 
    /// 
    /// </summary>
    public class UserDefinedProperty : Persistence
    {
        /// <summary>
        /// Class Name
        /// </summary>
        public const string CLASS = "UserDefinedProperty";
   
        /// <summary>
        /// User Defined Property 의 Name의 최대 허용 길이
        /// </summary>
        public static int USER_DEFINED_PROPERTY_NAME_MAX_LENGTH = 128;


        /// <summary>
        /// 이 Class을 사용함으로서, User Defined Property 기능을 제공할 수 있는 Entity Class
        /// EntityClass 명은 User Defined Property가 저장될 Table 명의 Prefix가 됨
        /// </summary>
        protected string entityClass;

        public string EntityClass
        {
            get { return entityClass; }
            set { entityClass = value; }
        }

        /// <summary>
        /// Entity의 Id
        /// </summary>
        protected string entityId;

        public string EntityId
        {
            get { return entityId; }
            set { entityId = value; }
        }

        /// <summary>
        /// 저장될 User defined Property의 Name
        /// </summary>
        protected string name;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        /// <summary>
        /// 저장될 User Defined Property의 값
        /// </summary>
        protected string value;

        public string Value
        {
            get { return this.value; }
            set { this.value = value; dirtyFlag = true; Save(); }
        }

        /// <summary>
        /// 수정되어진 Data가 Data base에 저장되었는지 여부의 값
        /// 만약 저장되지 않은 수정된 Data가 있다면, Data가 dirty함으로 true
        /// </summary>
        protected bool dirtyFlag = false;

        /// <summary>
        /// User Defined Property가 저장될 Table의 명
        /// </summary>
        /// <param name="entity">User Defined Property를 가지고 있는 Entity</param>
        /// <returns>data base table 명</returns>
        public static string GetDBTableName(string entity)
        {
//            return entity + "UDProperty";
            return "TB_UDProperty";
        }

        /// <summary>
        /// User Defined Property를 새롭게 생성
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명/param>
        /// <param name="entityId">Entity의 ID</param>
        /// <param name="name">User Defined Properyt의 명</param>
        /// <param name="value">User Defined Property의 값</param>
        /// <returns>생성되어진 User Defined Property의 Reference</returns>
        public static UserDefinedProperty NewUserDefinedProperty(string entityClass,
            string entityId, string name, string value)
        {
            return new UserDefinedProperty(entityClass, entityId, name, value, false);

        }

        /// <summary>
        /// User Defined Property Private 생성자
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity의 ID</param>
        /// <param name="name">User Defined Properyt의 명</param>
        /// <param name="value">User Defined Property의 값</param>
        private UserDefinedProperty(string entityClass, string entityId, string name, string value)
            : this(entityClass, entityId,name, value,true)
        {
        }

        /// <summary>
        /// User Defined Property Private 생성자
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity의 ID</param>
        /// <param name="name">User Defined Properyt의 명</param>
        /// <param name="value">User Defined Property의 값</param>
        /// <param name="loading">Data를 Loading 중일 경우 true</param>
        private UserDefinedProperty(string entityClass, string entityId, string name, string value, bool loading)
        {
            this.entityClass = entityClass;
            this.entityId = entityId;

            if (Helper.IsNullOrEmpty(name))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE, entityId,
                " Null or empty property name are not allowed!");
            }

            if (!loading)
            {
                bool invalidCharFound = false;

                for(int i =0; i < name.Length; i ++)
                {
                    //if (Char.IsLetterOrDigit(name[i]) || Char.IsWhiteSpace(name[i]))
                    //{
                    //}
                    //else
                    //{
                    //    invalidCharFound = true; break;
                    //}
                    if (Char.IsControl (name[i]) || Char.IsSymbol(name[i]) )
                    {
                        invalidCharFound = true;
                        break;
                    }
                }
                
                if ((name.Length > USER_DEFINED_PROPERTY_NAME_MAX_LENGTH) || invalidCharFound)
                {
                    throw new ECPException(ECPExceptionType.INVALID_VALUE, entityId,
                       "Invalid attribute name. User defined property name must be " +
                       "less than 29 characters in length, and can only contain " +
                       "alphanumeric characters, or the characters _ or space");
                }
            }

            this.name = name;
            this.value = Helper.PreventNull(value);
            
            if(!loading)
            {
                Add();
            }           
       
        }

        /// <summary>
        /// Property 객체를 생성함 없이 User Defined Property Data를 Data base에 저장함
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="propertyList">Add되어질 Property들</param>
        public static void AddUserDefinedPropertyDataList(
            string entityClass, UserDefinedPropertyData[] propertyList)
        {
            const string METHOD = CLASS + "." + "AddUserDefinedPropertyDataList";
        
            DBConnection dbConnection = null;
            
            try
            {
                dbConnection = DBControl.GetConnection();
                
                AddUserDefinedPropertyDataList(entityClass, dbConnection,
                                          propertyList);
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                                       METHOD, ex.ToString());
            }
            finally
            {
                if (dbConnection != null)
                {

                    DBControl.RemoveDBConection(dbConnection);
                }
            }
        }

        /// <summary>
        /// Property 객체를 생성함 없이 User Defined Property Data를 Data base에 저장함
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="dbConnection">사용되어질 DBConnection 명</param>
        /// <param name="propertyList">Add되어질 Property들</param>
        public static void AddUserDefinedPropertyDataList(String entityClass, 
            DBConnection dbConnection, UserDefinedPropertyData[] propertyList)
        {
            const string METHOD = CLASS + "." + "AddUserDefinedPropertyDataList";

            try
            {
                string sqlstring = "insert into " + GetDBTableName(entityClass) +
                    " (ENTITYID, UDPROPERTYCD, VALUE) values (@ENTITYID,@NAME,@VALUE)";

                for (int i = 0; i < propertyList.Length; i++)
                {
                   
                    String entityId = propertyList[i].entityId;
                    UserDefinedPropertyInfo[] infos = propertyList[i].propertyList;

                    if (infos == null) break;

                    for (int j = 0; j < infos.Length; j++)
                    {
                        PreparedSqlParameter pp = dbConnection.GetPreparedSqlParameter();

                        pp.SetVarChar("ENTITYID", entityId.ToUpper());
                        pp.SetVarChar("NAME", infos[j].name);
                        pp.SetVarChar("VALUE", infos[j].value);

                        dbConnection.ExecuteUpdate(sqlstring, pp);
                    }
                }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                                       METHOD, ex.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }
        }

        /// <summary>
        /// Entity에 속하는 User Defined Property를 Data base로 부터 Load
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity Id</param>
        /// <returns>Entity에 속하는 Property의 Dictionary</returns>
        public static Dictionary<string,UserDefinedProperty> GetUserDefinedPropertiesForEntity
            (string entityClass, string entityId)
        {
            const string METHOD = CLASS + "." + "GetUserDefinedPropertiesForEntity";

            DBConnection dbConnection = null;
     
            Dictionary<string, UserDefinedProperty> entityAttrList = new Dictionary<string, UserDefinedProperty>();

            
            UserDefinedProperty entityAttr = null;
            PreparedSqlParameter pp = null;

            int index = 0;

            try
            {
                dbConnection = DBControl.GetConnection();
                pp = dbConnection.GetPreparedSqlParameter();
                pp.SetVarChar("ENTITYID", entityId);

                string sqlstring = "select * from " + GetDBTableName(entityClass) +
                            " where ENTITYID = @ENTITYID";

                SqlDataReader reader = dbConnection.ExecuteQuery(sqlstring, pp);

                while (reader.Read())
                {
                    string name = Helper.PreventNull((string)reader["UDPROPERTYCD"]);
                    string value = Helper.PreventNull((string)reader["VALUE"]);
                    entityAttr = new UserDefinedProperty(entityClass, entityId, name, value);

                    entityAttrList.Add(name, entityAttr);
                    index++;
                }           
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                    METHOD, ex.ToString());
            }
            finally
            {
                if(dbConnection != null)
                {
                    DBControl.RemoveDBConection(dbConnection);
                }
            }         
            return entityAttrList;
        }


        /// <summary>
        ///  Entity Class와 Entity에 속하는 User Defined Property를 Data base로 부터 Load
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity Id</param>
        /// <returns>Entity에 속하는 Property의 List</returns>
        public static List<string> GetUserDefinedPropertyNameList(string entityClass, string entityId)
        {
            DBConnection conn = null;
            SqlDataReader reader = null;
            List<string> result = new List<string>();

       
            try
            {
                conn = DBControl.GetConnection();

                PreparedSqlParameter ps = conn.GetPreparedSqlParameter();

                ps.SetVarChar("ENTITYID", entityId);

                string sqlstring = "select * from " + GetDBTableName(entityClass) +
                    " where ENTITYID = @ENTITYID";

                reader = conn.ExecuteQuery(sqlstring, ps);

                while (reader.Read())
                {
                    result.Add((string)reader["UDPROPERTYCD"]);
                }
            }
            finally
            {
                if (reader != null) reader.Close();
                DBControl.RemoveDBConection(conn);
            }

            return result;
        }

        /// <summary>
        /// Entity Class에 속한 모든 User Defined Property Data를 Return
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <returns>Entity Class에 속한 모든 User Defined Property Data</returns>
        public static UserDefinedPropertyData[] GetUserDefinedPropertyDataList(string entityClass)
        {

            const string METHOD = CLASS + ".GetUserDefinedPropertyDataList";

            Dictionary<string, List<UserDefinedPropertyInfo>> udpPropertyDataList = 
                new Dictionary<string, List<UserDefinedPropertyInfo>>();

            DBConnection dbConnection = null;
            SqlDataReader reader = null;
    
      
            List<string> entities = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select distinct entityId from " + GetDBTableName(entityClass);

                reader = dbConnection.ExecuteQuery(sqlstring);

                while (reader.Read())
                {
                    entities.Add((string)reader["ENTITYID"]);
                }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
            catch (Exception ex)
            {
                Log.log(5, "UserDefinedPropertyData", "debug", ECP.Service.Logger.Category.DEBUG, "", "UserDefinedPropertyData", ex.Message.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }

            UserDefinedPropertyData[] rtnval = new UserDefinedPropertyData[entities.Count];

            Dictionary<string,UserDefinedProperty> entityAttrList=null;

            int i =0;

            foreach (string entityId in entities)
            {
                entityAttrList = GetUserDefinedPropertiesForEntity(entityClass, entityId);
                
                UserDefinedPropertyInfo[] udpInfoes = new UserDefinedPropertyInfo[entityAttrList.Count];
                
                int j =0;
                foreach(UserDefinedProperty p in entityAttrList.Values)
                {
                    udpInfoes[j]= new UserDefinedPropertyInfo(p.name, p.value);
                    j++;
                }

                rtnval[i] = new UserDefinedPropertyData(entityId, udpInfoes);
                i++;
            }
            return rtnval;
        }

        /// <summary>
        /// Entity의 명을 주어진 새로운 Id로 변경, newId는 유일한 값이어야 함
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="id">변경하고자 하는 Entity명</param>
        /// <param name="newId">새로운 Entity 명</param>
        public static void RenameEntity(string entityClass,String id, String newId)
        {
            const string METHOD = CLASS + "." + "RenameEntity";

            DBConnection dbConnection = null;
            PreparedSqlParameter ps = null;

            try
            {
                dbConnection = DBControl.GetConnection();


                dbConnection.ExecuteUpdate("update " + GetDBTableName(entityClass) + 
                    " set entityId = '" + newId + "' where entityId = '" +
                    id + "'");
            
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                    METHOD, ex.ToString());
            }
            finally
            {
                if(dbConnection != null)
                {
                    DBControl.RemoveDBConection(dbConnection);
                }
            }
        }

        /// <summary>
        /// Entity에 속하는 모든 Property를 삭제
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity Id</param>
        public static void DeleteAllForEntity(string entityClass, string entityId)
        {
            const string METHOD = CLASS + ".DeleteAllForEntity";

            DBConnection dbConnection = null;

            try
            {
                dbConnection = DBControl.GetConnection();

                DeleteAllForEntity(entityClass, entityId, dbConnection);
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }
        }

        /// <summary>
        /// Entity에 속하는 모든 Property를 Data Base의 Table에서 삭제
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="entityId">Entity Id</param>
        /// <param name="conn">DBConnection 명</param>
        public static void DeleteAllForEntity(string entityClass, string entityId, DBConnection conn)
        {
            const string METHOD = CLASS + ".DeleteAllForEntity";

            try
            {
                PreparedSqlParameter pp = conn.GetPreparedSqlParameter();
                pp.SetVarChar("ENTITYID", entityId);

                conn.ExecuteUpdate("delete from " + GetDBTableName(entityClass) +
                " where ENTITYID =@ENTITYID", pp);
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
        }

        /// <summary>
        /// EntityClass의 모든 Property를 Database로 부터 삭제
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        public static void DeleteAllForEntity(string entityClass)
        {
            const string METHOD = CLASS + ".DeleteAllForEntity";

            DBConnection dbConnection = null;

            try
            {
                dbConnection = DBControl.GetConnection();
                dbConnection.ExecuteUpdate("truncate table " + GetDBTableName(entityClass));

            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }
        }

        /// <summary>
        /// EntityClass의 모든 Property를 Database로 부터 삭제
        /// </summary>
        /// <param name="entityClass">이 Class을 사용하여 UDF 기능을 제공하는 Entity Class명</param>
        /// <param name="dbConnection">사용되어진 DBConnection</param>
        public static void DeleteAllForEntityClass(string entityClass, DBConnection dbConnection)
        {
            try
            {
                dbConnection.ExecuteUpdate("delete from " + GetDBTableName(entityClass));
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, "", ex.ToString());
            }
        }


        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".add";

            try
            {
                base.Add(theDB);

                PreparedSqlParameter pp = theDB.GetPreparedSqlParameter();

                pp.SetVarChar("ENTITYID", this.entityId);
                pp.SetVarChar("NAME", this.name);
                pp.SetVarChar("VALUE", this.value);


                theDB.ExecuteUpdate("insert into " + GetDBTableName(entityClass) + "(ENTITYID,UDPROPERTYCD,VALUE) values (@ENTITYID,@NAME,@VALUE)", pp);
                
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,METHOD, ex.ToString());
            }
            finally
            {
                //if(reader != null) reader.Close();
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
            const string METHOD = CLASS + ".save";

            try
            {
                base.Save(theDB);
                if (dirtyFlag)
                {
                    this.dirtyFlag = false;
           

                    theDB.ExecuteUpdate("update " + GetDBTableName(entityClass) + " set\n" +
                        "VALUE='" + this.value + "'\n" +
                        "where ENTITYID='" + this.entityId + "' and NAME='" + this.name + "'");
 }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Delete";

            try
            {
                base.Delete(theDB);

                PreparedSqlParameter pp = theDB.GetPreparedSqlParameter();

                pp.SetVarChar("ENTITYID", this.entityId);
                pp.SetVarChar("NAME", this.name);

                theDB.ExecuteUpdate("delete from " + GetDBTableName(entityClass) + " where " +
                    "ENTITYID =@ENTITYID AND NAME=@NAME", pp);
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                                        METHOD, ex.ToString());
            }
        } 
    }
}
