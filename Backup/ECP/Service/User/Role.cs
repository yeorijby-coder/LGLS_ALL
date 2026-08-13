using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;


using ECP.Db;
using ECP.Service.Logger;
using ECP.Global.Exception;
using ECP.Util;
using ECP.Global.Service;

namespace ECP.Service.User
{
    public class Role : Persistence
    {
        const string CLASS = "Role";

        string theRoleId = string.Empty;

        public string RoleId
        {
            get { return theRoleId; }
        }

        string theRoleDescription = string.Empty;

        public string RoleDescription
        {
            get { return theRoleDescription; }
            set 
            {
                this.theRoleDescription = value;
                SaveAsDirty();
            }
        }

        

        List<string> thePermissionIdList = new List<string>();

        private bool dirtyFlag = false;

        static Role()
        {
        }

        public Role(string theRoleId)
        {
            this.theRoleId = theRoleId;
            Load();
        }

        public Role(string theRoleId, string theRoleDescription)
        {
		    const string METHOD = CLASS + "." + "Role";

		    if (Helper.IsNullOrEmpty(theRoleId)) {
			    throw new ECPException(ECPExceptionType.INVALID_VALUE, theRoleId,
					    METHOD + ":" + "RoleId is Null.");
		    }
		    this.theRoleId = Helper.PreventNull(theRoleId);
            this.theRoleDescription = Helper.PreventNull(theRoleDescription);
		    Add();
	    }

        public static Role NewRole(string theRoleId, string theRoleDescription)
        {
		    Role theRole = new Role(theRoleId, theRoleDescription);
		    return (theRole);
	    }

        public static Role GetRole(string theRoleId)
        {
		    Role theRole = new Role(theRoleId);
		    return (theRole);
	    }

        public void SaveRole()
        {
            this.Save();
            this.dirtyFlag = false;
	    }

        public static void DeleteRole(string theRoleId)
        {
		    Role theRole = GetRole(theRoleId);
		    theRole.Delete();
	    }

        public static void DeleteRolePermission(string thePermissionId) 
        {
            const string METHOD = CLASS + "." + "DeleteRolePermission";

            DBConnection theDB = DBControl.GetConnection();

		    try 
            {

                theDB = DBControl.GetConnection();

                string theDeleteString = "delete from ROLEPRIVILEGE "
					    + "where PRIVILEGEID = '" + thePermissionId + "'";
			    theDB.ExecuteUpdate(theDeleteString);
		    } 
            catch (SqlException ex) {
			    throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
					    thePermissionId, METHOD + ":" + ex.ToString());
		    }
	    }

        public static string[] GetRoleIdList()
        {
            const string METHOD = CLASS + "." + "GetRoleIdList";


            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> roleIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string theRoleIdQuery = "select distinct ROLEID from ROLE "
                    + "order by ROLEID";

                reader = dbConnection.ExecuteQuery(theRoleIdQuery);

                while (reader.Read())
                {
                    roleIds.Add((string)reader["ROLEID"]);
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


            return roleIds.ToArray();
        }

        public static RoleData[] GetRoleList()
        {
            const string METHOD = ".GetRoleList";

            DBConnection dbConnection = null;
            SqlDataReader reader = null;



            List<RoleData> roles = new List<RoleData>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select distinct ROLEID, DESCRIPTION  from ROLE";

                reader = dbConnection.ExecuteQuery(sqlstring);

                while (reader.Read())
                {
                    string roleId = (string)reader["ROLEID"];
                    string description = (string)reader["DESCRIPTION"];


                    RoleData data = new RoleData(roleId, description);
                    roles.Add(data);
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


            return roles.ToArray();
        }



        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;

            try
            {
                base.Add(theDB);


                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();


                psparam.SetVarChar("ROLEID", this.theRoleId);

                reader = theDB.ExecuteQuery("select * from ROLE where ROLEID =@ROLEID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.DEBUG, "", "Role", 
                        this.theRoleId + "already Exist");

                    throw new ECPException(ECPExceptionType.DUPLICATE, "", this.theRoleId + "already Exist");
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("ROLEID", this.theRoleId);
                    pss.SetVarChar("DESCRIPTION", this.theRoleDescription);


                    theDB.ExecuteUpdate("insert into ROLE (ROLEID,DESCRIPTION) values (@ROLEID,@DESCRIPTION)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "Role", "sql error \n " +
                    ex.Message.ToString());
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
                ps.SetVarChar("ROLEID", this.theRoleId);

                theDB.ExecuteUpdate("delete from ROLE where ROLEID =@ROLEID", ps);

                PreparedSqlParameter ps2 = theDB.GetPreparedSqlParameter();
                ps2.SetVarChar("ROLEID", this.theRoleId);                
                
                theDB.ExecuteUpdate("delete from ROLEPRIVILEGE where ROLEID =@ROLEID", ps2);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "Role", "sql error \n " +
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

                psqlparam.SetVarChar("ROLEID", this.theRoleId);

                reader = theDB.ExecuteQuery("select * from ROLE where ROLEID =@ROLEID", psqlparam);


                if (reader.Read())
                {
                    this.theRoleDescription = (string)reader["DESCRIPTION"];

                }
                else
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.ERROR, "", "Role",
                        this.theRoleId + "doesnot exist");

                }


                String[] thePermissionIdList = GetPermissionIdList(this.theRoleId);

                
                for (int i = 0; i < thePermissionIdList.Length; i++)
                {
                    try
                    {
                        Permission.GetPermission(thePermissionIdList[i]);
                        this.thePermissionIdList.Add(thePermissionIdList[i]);
                    }
                    catch (ECPException ex)
                    {
                        Log.log(0, METHOD, "error", ECP.Service.Logger.Log.ERROR, "", "",
                                ex.ToString());
                    }
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "Role",
                    "sql error \n " + ex.Message.ToString());
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

                    theDB.ExecuteUpdate("update ROLE set\n" +
                        "DESCRIPTION='" + this.theRoleDescription + "'\n" +
                        "where ROLEID = '" + this.theRoleId + "'");

                    theDB.ExecuteUpdate("delete from ROLEPRIVILEGE "+
                        "where ROLEID = '"+this.theRoleId+"'");

                    for( int i =0; i < this.thePermissionIdList.Count; i++)
                    {
                        string thePermissionId = this.thePermissionIdList[i];

					    string theInsertString = "insert into ROLEPRIVILEGE\n"
							    + "(" + "ROLEID, " + "PRIVILEGEID" + ")\n "
							    + "values\n" + "(" + "'" + this.theRoleId + "',"
							    + "'" + thePermissionId + "'" + ")\n";
					    theDB.ExecuteUpdate(theInsertString);
                    }
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "role",
                    "sql error \n " + ex.Message.ToString());
            }
        }

        public static string[] GetPermissionIdList(string roleId)
        {
            const string METHOD = ".GetPermissionIdList";

            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> permissionIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select distinct PRIVILEGEID from ROLEPRIVILEGE where ROLEID ='"+
                    roleId + "' order by PRIVILEGEID";

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


        public static void SetRoleDescription(string theRoleId,
			string theRoleDescription)
        {
		    Role roleId = GetRole(theRoleId);
		    roleId.RoleDescription = theRoleDescription;
	    }

        public void SetPermissionIdList(string[] thePermissionIdList)
        {

            this.thePermissionIdList.Clear();

		    for (int i = 0; i < thePermissionIdList.Length; i++) {
			    this.thePermissionIdList.Add(thePermissionIdList[i]);
		    }

            SaveAsDirty();
	    }

        public static void SetPermissionIdList(string theRoleId,
			    string[] thePermissionIdList)
        {
		    Role roleId = GetRole(theRoleId);
		    roleId.SetPermissionIdList(thePermissionIdList);
	    }


        public PermissionData[] GetPermissionList() 
        {
            const string METHOD = CLASS + "." + "GetPermissionList";

		    PermissionData[] permissionData = new PermissionData[thePermissionIdList.Count];


		    for (int i = 0; i < this.thePermissionIdList.Count; i++)
            {			    
                try 
                {
				    Permission thePermission = Permission.GetPermission(this.thePermissionIdList[i]);

				    permissionData[i] = new PermissionData(thePermission
						    .ThePermissionId, thePermission.ThePermissionDescription);
			    } catch (ECPException ex) {
				    Log.log(0, METHOD, "error", Log.ERROR, "", "",
						    ex.ToString());
			    }
		    }
		    return (permissionData);
	    }

        public string[] GetAvailablePermissionIdList()
        {
            string[] thePermissionIdList = Permission.GetPermissionIdList();
            
            List<string>  theAvailablePermissionIdList = new List<string>();		    
            
            for (int i = 0; i < thePermissionIdList.Length; i++)
            {
			    if (!this.thePermissionIdList.Contains(thePermissionIdList[i])) {
				    theAvailablePermissionIdList.Add(thePermissionIdList[i]);
			    }
		    }

            return theAvailablePermissionIdList.ToArray();
	    }

	    public static string[] GetAvailablePermissionIdList(string roleId)
        {
		    Role theRole = GetRole(roleId);
		    return (theRole.GetAvailablePermissionIdList());
            
	    }


        public List<Permission> GetAvailablePermissionList()
        {
		    const string  METHOD = CLASS + "." + "GetAvailablePermissionList";

		    string[] thePermissionIdList = Permission.GetPermissionIdList();
            
		    List<Permission> theAvailablePermissionList = new List<Permission>();

		    for (int i = 0; i < thePermissionIdList.Length; i++)
            {
			    if (!this.thePermissionIdList.Contains(thePermissionIdList[i])) {
				    try 
                    {
					    Permission thePermission = Permission
							    .GetPermission(thePermissionIdList[i]);
					    theAvailablePermissionList.Add(thePermission);
				    } catch (ECPException ex) {
					    Log.log(0, METHOD, "error", Log.ERROR, "", "",
							    ex.ToString());
				    }
			    }
		    }
		    return (theAvailablePermissionList);
	    }



        protected override void SetDirtyFlag()
        {
            this.dirtyFlag = true;
        }


    }
}
