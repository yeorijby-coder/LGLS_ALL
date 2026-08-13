using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;

using ECP.Db;
using ECP.Global.Exception;
using ECP.Service.Logger;
using ECP.Util;
using ECP.Global.Service;

namespace ECP.Service.User
{
    public class Permission : Persistence
    {
        const string CLASS = "Permisson";

        string thePermissionId = "";

        public string ThePermissionId
        {
            get { return thePermissionId; }
            set 
            {
                thePermissionId = value;

                SaveAsDirty();
            }
        }

        string thePermissionDescription = "";

        public string ThePermissionDescription
        {
            get { return thePermissionDescription; }
            set 
            { 
                thePermissionDescription = value;
                SaveAsDirty();
            }
        }


        private bool dirtyFlag = false;


        static Permission()
        {

        }

        

        public static Permission NewPermission(string permissionId, string description)
        {
            Permission thePermission = new Permission(permissionId, description);
            return (thePermission);
        }

        public static Permission GetPermission(string permissionId)
        {
            Permission thePermission = new Permission(permissionId);
            return thePermission;
        }

        

        public static void DeletePermission(string permissionId)
        {
            Permission thePermission = GetPermission(permissionId);
            thePermission.Delete();
        }

        public static string[] GetPermissionIdList()
        {
            const string METHOD = ".GetPermissionIdList";

            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> permissionIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select distinct PRIVILEGEID from PRIVILEGEDATA";

                reader = dbConnection.ExecuteQuery(sqlstring);

                while (reader.Read())
                {
                    permissionIds.Add((string)reader["PRIVILEGEID"]);
                }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
            catch (Exception ex)
            {
                //throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }          
          
            
            return permissionIds.ToArray();
        }

        public static PermissionData[] GetPermissionList()
        {
            const string METHOD = ".GetPermissionList";

            DBConnection dbConnection = null;
            SqlDataReader reader = null;



            List<PermissionData> permissions = new List<PermissionData>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select distinct PRIVILEGEID, DESCRIPTION  from PRIVILEGEDATA";

                reader = dbConnection.ExecuteQuery(sqlstring);

                while (reader.Read())
                {
                    string permissionId = (string)reader["PRIVILEGEID"];
                    string description = (string)reader["DESCRIPTION"];


                    PermissionData data = new PermissionData(permissionId, description);
                    permissions.Add(data);
                }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, METHOD, ex.ToString());
            }
            catch (Exception ex)
            {
                //throw ex;
            }
            finally
            {
                DBControl.RemoveDBConection(dbConnection);
            }


            return permissions.ToArray();
        }



        public Permission(string permissionId)
        {
            this.thePermissionId = permissionId;
            Load();
        }


        public Permission(string thePermissionId,
                      string thePermissionDescription)
        {
            const string METHOD = CLASS + "." + "Permission";

            if (Helper.IsNullOrEmpty(thePermissionId))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                                         thePermissionId, METHOD + ":" +
                                         "PermissionId is Null.");
            }

            this.thePermissionId = Helper.PreventNull(thePermissionId);            
            this.thePermissionDescription = Helper.PreventNull(thePermissionDescription);

            Add();
        }


        public void SetPermissionData(string desc)
        {
             this.thePermissionDescription = desc;

             this.dirtyFlag = true;
             SaveAsDirty();
        }

        public static void SetPermissionData(string thePermissionId,
            string thePermissionDescription)
        {
            Permission permission = GetPermission(thePermissionId);
            permission.SetPermissionData(thePermissionDescription);
        }

        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;

            try
            {
                base.Add(theDB);


                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();


                psparam.SetVarChar("PRIVILEGEID", this.thePermissionId);

                reader = theDB.ExecuteQuery("select * from PRIVILEGEDATA where PRIVILEGEID =@PRIVILEGEID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.DEBUG, "", "permissionId", this.thePermissionId + "already Exist");
                    
                    throw new ECPException(ECPExceptionType.DUPLICATE,"", this.thePermissionId + "already Exist");
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("PRIVILEGEID", this.thePermissionId);
                    pss.SetVarChar("DESCRIPTION", this.thePermissionDescription);


                    theDB.ExecuteUpdate("insert into PRIVILEGEDATA (PRIVILEGEID,DESCRIPTION) values (@PRIVILEGEID,@DESCRIPTION)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "permission", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
            }
        }

        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Delete";

            try
            {
                base.Delete(theDB);
                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("PRIVILEGEID", this.thePermissionId);

                theDB.ExecuteUpdate("delete from PRIVILEGEDATA where PRIVILEGEID =@PRIVILEGEID", ps);

                
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "permission", "sql error \n " + 
                    ex.Message.ToString());
                throw ex;
            }
        }

        protected override void Load(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Load";

            SqlDataReader reader = null;
            try
            {
                base.Load(theDB);
                
                PreparedSqlParameter psqlparam = theDB.GetPreparedSqlParameter();

                psqlparam.SetVarChar("PRIVILEGEID", this.thePermissionId);

                reader = theDB.ExecuteQuery("select * from PRIVILEGEDATA where PRIVILEGEID =@PRIVILEGEID", psqlparam);


                if (reader.Read())
                {
                    this.thePermissionId = (string)reader["PRIVILEGEID"];
                    this.thePermissionDescription = (string)reader["DESCRIPTION"];

                }
                else
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.ERROR, "", "permission", this.thePermissionId + "doesnot exist");

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "permission", "sql error \n " + ex.Message.ToString());
            }
            finally
            {
                //if (reader != null) reader.Close();
            }
        }

        protected override void Save(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Save";

            try
            {
                base.Save(theDB);
                if (dirtyFlag)
                {
                    this.dirtyFlag = false;

                    theDB.ExecuteUpdate("update PRIVILEGEDATA set\n" +
                        "DESCRIPTION='" + this.thePermissionDescription+ "'\n" +
                        "where PRIVILEGEID = '" + this.thePermissionId + "'");
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "permission", 
                    "sql error \n " + ex.Message.ToString());
            }
        }

        protected override void SetDirtyFlag()
        {
            this.dirtyFlag = true;
        }

    }
}
