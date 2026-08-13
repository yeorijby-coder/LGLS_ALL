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
    public class Person : Persistence
    {
        bool dirtyFlag = false;

        const string CLASS = "Persistence";

        string theUserId = "";

        public string TheUserId
        {
            get { return theUserId; }
            set { theUserId = value; }
        }

        string thePassword = "";

        public string ThePassword
        {
            get { return thePassword; }
            set
            {
                thePassword = value;
                this.thePasswordCreateTime = DateTime.Now;

                SaveAsDirty();
            }
        }

        string theUserName = "";

        public string TheUserName
        {
            get { return theUserName; }
            set
            {
                theUserName = value;
                SaveAsDirty();
            }
        }

        private string theUserDescription = "";

        public string TheUserDrescription
        {
            get { return theUserDescription; }
            set
            {
                theUserDescription = value;
                SaveAsDirty();
            }
        }

        DateTime thePasswordCreateTime;

        public DateTime ThePasswordCreateTime
        {
            get { return thePasswordCreateTime; }
            set { thePasswordCreateTime = value; }
        }

        int thePasswordMaxAge = 0;

        public int ThePasswordMaxAge
        {
            get { return thePasswordMaxAge; }
            set
            {
                thePasswordMaxAge = value;
                SaveAsDirty();
            }
        }

        string[] theRoleIdList = null;

        public string[] TheRoleIdList
        {
            get { return theRoleIdList; }
            set
            {
                theRoleIdList = value;
                SaveAsDirty();
            }
        }

        private bool disabled = false;

        public bool Disabled
        {
            get { return disabled; }
            set 
            { 
                disabled = value;
                SaveAsDirty();
            }
        }


        static Person()
        {
        }

        public Person(string theUserId)
        {
            this.theUserId = theUserId;
            Load();
        }
        public Person(string theUserId, string thePassword, string theUserName,
                string theUserDescription, int thePasswordMaxAge,
                bool disabled)
        {
            const string METHOD = CLASS + "." + "User";
            if (Helper.IsNullOrEmpty(theUserId))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                                         theUserId, METHOD + ":" +
                                         "UserId is Null.");
            }
            this.theUserId = Helper.PreventNull(theUserId);
            this.thePassword = Helper.PreventNull(thePassword);
            this.theUserName = Helper.PreventNull(theUserName);
            this.theUserDescription = Helper.PreventNull(theUserDescription);
            this.thePasswordCreateTime = DateTime.Now;
            this.thePasswordMaxAge = thePasswordMaxAge;
            this.disabled = disabled;

            Add();
        }

        public static Person NewUser(string theUserId, string thePassword,
                               string theUserName, string theUserDescription,
                               int thePasswordMaxAge, bool disabled)
        {
            return (new Person(theUserId, thePassword, theUserName,
                    theUserDescription, thePasswordMaxAge, disabled));
        }

        public void SaveUser()
        {
            Save();
            this.dirtyFlag = false;
        }

        protected override void Add(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Add";

            SqlDataReader reader = null;
            try
            {
                base.Add(theDB);

                PreparedSqlParameter psparam = theDB.GetPreparedSqlParameter();

                psparam.SetVarChar("USERID", this.theUserId);

                reader = theDB.ExecuteQuery("select * from ECPUSER where USERID =@USERID", psparam);
                if (reader.Read())
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.DEBUG, "", "user", this.theUserId + "already Exist");
                }
                else
                {
                    PreparedSqlParameter pss = theDB.GetPreparedSqlParameter();
                    pss.SetVarChar("USERID", this.TheUserId);
                    pss.SetVarChar("PASSWORD", this.thePassword);
                    pss.SetVarChar("USERNAME", this.theUserName);
                    pss.SetVarChar("USERDESCRIPTION", this.theUserDescription);
                    pss.SetDateTime("PASSWORDCREATETIME", this.thePasswordCreateTime);
                    pss.SetInt("PASSWORDMAXAGE", this.thePasswordMaxAge);
                    pss.SetChar("DISABLED", (this.disabled == true) ? "T" : "F");


                    theDB.ExecuteUpdate("insert into ECPUSER (USERID, PASSWORD,USERNAME,USERDESCRIPTION,PASSWORDCREATETIME,PASSWORDMAXAGE,DISABLED) values (@USERID,@PASSWORD,@USERNAME,@USERDESCRIPTION,@PASSWORDCREATETIME,@PASSWORDMAXAGE,@DISABLED)", pss);

                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "user", "sql error \n " + ex.Message.ToString());
                throw ex;
            }
            finally
            {
                //if(reader != null) reader.Close();
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

                psqlparam.SetVarChar("USERID", this.theUserId);

                reader = theDB.ExecuteQuery("select * from ECPUSER where USERID =@USERID", psqlparam);


                if (reader.Read())
                {
                    this.thePassword = (string)reader["PASSWORD"];
                    this.theUserName = (string)reader["USERNAME"];
                    this.theUserDescription = (string)reader["USERDESCRIPTION"];
                    this.thePasswordCreateTime = (DateTime)reader["PASSWORDCREATETIME"];
                    this.thePasswordMaxAge = (int)reader["PASSWORDMAXAGE"];
                    this.disabled = (string)reader["DISABLED"] == "T" ? true : false;
                }
                else
                {
                    Log.log(5, METHOD, "debug", ECP.Service.Logger.Category.ERROR, "", "User",
                        this.theUserId + "doesnot exist");

                }

                this.theRoleIdList = GetRoleIdList(this.theUserId);

            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "User",
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



                    theDB.ExecuteUpdate("update ECPUSER set\n" +
                        "PASSWORD='" + this.thePassword + "',\n" +
                        "USERNAME='" + this.theUserName + "',\n" +
                        "USERDESCRIPTION='" + this.theUserDescription + "',\n" +
                      //  "PASSWORDCREATETIME='" + (DateTime)this.thePasswordCreateTime+ "',\n" +
                        "PASSWORDMAXAGE='" + this.thePasswordMaxAge + "',\n" +
                        "DISABLED='" + ((this.disabled == true) ? "T" : "F") + "'\n" +
                        "where USERID = '" + this.theUserId + "'");


                    theDB.ExecuteUpdate("delete from ECPUSERROLE " +
                        "where USERID = '" + this.theUserId + "'");

                    if (this.TheRoleIdList != null)
                    {
                        for (int i = 0; i < this.theRoleIdList.Length; i++)
                        {
                            string theRoleId = this.theRoleIdList[i];

                            string theInsertString = "insert into ECPUSERROLE\n"
                                    + "(" + "USERID, " + "ROLEID" + ")\n "
                                    + "values\n" + "(" + "'" + this.theUserId + "',"
                                    + "'" + theRoleId + "'" + ")\n";
                            theDB.ExecuteUpdate(theInsertString);
                        }
                    }
                }
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "user",
                    "sql error \n " + ex.Message.ToString());
            }
        }
        protected override void Delete(DBConnection theDB)
        {
            const string METHOD = CLASS + ".Delete";

            try
            {
                base.Delete(theDB);

                PreparedSqlParameter ps = theDB.GetPreparedSqlParameter();
                ps.SetVarChar("USERID", this.theUserId);

                theDB.ExecuteUpdate("delete from ECPUSER where USERID =@USERID", ps);

                PreparedSqlParameter ps2 = theDB.GetPreparedSqlParameter();
                ps2.SetVarChar("USERID", this.theUserId);

                theDB.ExecuteUpdate("delete from ECPUSERROLE where USERID =@USERID", ps2);
            }
            catch (SqlException ex)
            {
                Log.log(5, METHOD, "error", ECP.Service.Logger.Category.ERROR, "", "Role", "sql error \n " +
                    ex.Message.ToString());
                throw ex;
            }
        }



        protected override void SetDirtyFlag()
        {
            this.dirtyFlag = true;

        }

        public static string[] GetUserIdList()
        {
            const string METHOD = CLASS + "." + "GetUserIdList";


            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> userIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string theUserIdQuery = "select USERID from ECPUSER " +
                                            "order by USERID";


                reader = dbConnection.ExecuteQuery(theUserIdQuery);

                while (reader.Read())
                {
                    userIds.Add((string)reader["USERID"]);
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


            return userIds.ToArray();
        }

        public static string[] GetUserIdsWithRole(String role)
        {
            const string METHOD = CLASS + "." + "GetUserIdsWithRole";


            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> userIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string theUserIdQuery = "select USERID from ECPUSERROLE\n" +
                "  where ROLEID = '" + role + "' order by USERID";

                reader = dbConnection.ExecuteQuery(theUserIdQuery);

                while (reader.Read())
                {
                    userIds.Add((string)reader["USERID"]);
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


            return userIds.ToArray();
        }

        static string[] GetRoleIdList(string theUserId)
        {
            const string METHOD = CLASS + "." + "GetRoleIdList";


            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<string> roleIds = new List<string>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string theRoleIdQuery = "select ROLEID from ECPUSERROLE " +
                                            "where USERID = '" + theUserId + "' " +
                                            "order by USERID";


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

        public static UserData[] GetUserList()
        {
            const string METHOD = ".GetUserList";

            DBConnection dbConnection = null;
            SqlDataReader reader = null;

            List<UserData> useres = new List<UserData>();

            try
            {
                dbConnection = DBControl.GetConnection();

                string sqlstring = "select * from ECPUSER order by USERID";

                reader = dbConnection.ExecuteQuery(sqlstring);

                while (reader.Read())
                {
                    string userId = (string)reader["USERID"];
                    string passworld = (string)reader["PASSWORD"];
                    string username = (string)reader["USERNAME"];
                    string description = (string)reader["USERDESCRIPTION"];
                    DateTime createTime = (DateTime)reader["PASSWORDCREATETIME"];
                    int maxage = (int)reader["PASSWORDMAXAGE"];


                    UserData data = new UserData(userId, passworld, username, description, createTime, maxage);


                    useres.Add(data);
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


            return useres.ToArray();
        }

        public bool IsPasswordExpired()
        {
            if (this.thePasswordMaxAge == -1)
            {
                return false;
            }
            else
            {

                long hoursInDay = 24;
                long minutesInHour = 60;
                long secondsInMinute = 60;
                long millisecondsInSecond = 1000;

                // convert the passwordMaxAge, which is in days, to milliseconds
                long passwordMaxAgeInMilliSeconds = this.thePasswordMaxAge *
                        hoursInDay * minutesInHour * secondsInMinute *
                        millisecondsInSecond;

                // calculate the password expiration date
                DateTime passwordExpires = new DateTime(this.thePasswordCreateTime.Ticks +
                                       passwordMaxAgeInMilliSeconds);

                DateTime today = DateTime.Now;

                // compare today to the password expiration date
                return (today.CompareTo(passwordExpires) > 0);

            }
        }

        public string[] GetAvailableRoleIdList()
        {
            string[] masterRoleIdList = Role.GetRoleIdList();

            List<string> theRoleIdList = new List<string>();

            for (int i = 0; i < this.theRoleIdList.Length; i++)
            {
                theRoleIdList.Add(this.theRoleIdList[i]);
            }

            List<string> theAvailableRoleIdList = new List<string>();

            for (int i = 0; i < masterRoleIdList.Length; i++)
            {
                if (!theRoleIdList.Contains(masterRoleIdList[i]))
                {
                    theAvailableRoleIdList.Add(masterRoleIdList[i]);
                }
            }

            return theAvailableRoleIdList.ToArray();
        }

        public RoleData[] GetAvailableRoleList()
        {
            const string METHOD = CLASS + "." + "GetAvailableRoleList";

            string[] masterRoleIdList = Role.GetRoleIdList();

            List<Role> theAvailableRoleList = new List<Role>();

            List<string> theRoleIdList = new List<string>();
            for (int i = 0; i < this.theRoleIdList.Length; i++)
            {
                theRoleIdList.Add(this.theRoleIdList[i]);
            }

            for (int i = 0; i < theRoleIdList.Count; i++)
            {
                if (!theRoleIdList.Contains(masterRoleIdList[i]))
                {
                    try
                    {
                        Role theRole = Role.GetRole(masterRoleIdList[i]);
                        theAvailableRoleList.Add(theRole);
                    }
                    catch (ECPException ex)
                    {
                        Log.log(0, METHOD, "error", Log.ERROR, "", "",
                                ex.ToString());
                    }
                }
            }

            RoleData[] returnList = new RoleData[theAvailableRoleList.Count];
            for (int i = 0; i < theAvailableRoleList.Count; i++)
            {
                Role theRole = (Role)theAvailableRoleList[i];
                returnList[i] = new RoleData(theRole.RoleId, theRole.RoleDescription);
            }
            return (returnList);
        }

        public void SetInformation(UserData userData, string[] roleList)
        {
            DelayPersistance(true);

            this.ThePassword = userData.password;
            this.ThePasswordMaxAge = userData.passwordMaxAge;
            this.TheUserName = userData.userName;
            this.TheUserDrescription = userData.userDescription;

            this.TheRoleIdList = roleList;

            DelayPersistance(false);
            SaveAsDirty();
        }

        public override bool IsAlive()
        {
            return true;
        }

        public UserData GetData()
        {
            UserData userData = new UserData();

            userData.userId = this.theUserId;
            userData.password = this.thePassword;
            userData.userName = this.theUserName;
            userData.userDescription = this.theUserDescription;

            long currentTime = this.thePasswordCreateTime.Ticks;

            userData.passwordCreateTime = new DateTime(currentTime);
            userData.passwordMaxAge = this.thePasswordMaxAge;

            return (userData);

        }

        public UserPermissionData GetUserPermissionData()
        {
            const string METHOD = CLASS + "." + "GetUserPermissionData";
            
            UserPermissionData userPermissionData = new UserPermissionData();

            // User data list
            userPermissionData.userData = this.GetData();

            PermissionData[] permissionData = null;


            List<PermissionData[]> permissionVector = new List<PermissionData[]>();

            int size=0;

            RoleData[] theRoleList = new RoleData[this.theRoleIdList.Length];

            for (int i = 0; i < this.theRoleIdList.Length; i++)
            {
                try
                {
                    Role theRole = Role.GetRole(this.theRoleIdList[i]);
                    permissionData = theRole.GetPermissionList();
                    size += permissionData.Length;
                    permissionVector.Add(permissionData);
                    theRoleList[i] = new RoleData(theRole.RoleId,
                            theRole.RoleDescription);
                }
                catch (ECPException ex)
                {
                    Log.log(0, METHOD, "error", Log.ERROR, "", "",
                            ex.ToString());
                }
            }
            userPermissionData.roleDataList = theRoleList;

            PermissionData[] allPermissionData = new PermissionData[size];
            int count = 0;
            for (int i=0; i < permissionVector.Count; i++)
            {
                permissionData = (PermissionData[])permissionVector[i];
                for (int j =0;j<permissionData.Length;j++)
                {
                    allPermissionData[count++] = permissionData[j];
                }
            }
            userPermissionData.permissionDataList = allPermissionData;

            return (userPermissionData);
        }

        public static void RemoveRoleFromUsers(string theRoleId)
        {
            const string METHOD = CLASS + "." + "RemoveRoleFromUsers";
            
            DBConnection theDB = null;

            try
            {
                string theDeleteString = "delete from ECPUSERROLE " +
                                            "where ROLEID = '" + theRoleId + "'";

                theDB = DBControl.GetConnection();
                theDB.ExecuteUpdate(theDeleteString);
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                                         theRoleId, METHOD + ":" + ex.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
        }


        public static bool[] GetDisableDataList()
        {
            

            DBConnection theDB = null;
            SqlDataReader reader = null;

            List<bool> disableDataVector = new List<bool>();

            try
            {
                string theUserQuery = "select " + "DISABLE" + " from " +
                "ECPUSER" + " order by USERID";

                theDB = DBControl.GetConnection();
                reader = theDB.ExecuteQuery(theUserQuery);



                while (reader.Read())
                {
                    disableDataVector.Add((((char)reader["DISABLE"]).Equals("T") )? true : false);
                }
            }
            catch (SqlException ex)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED,
                    "ID", ex.ToString());
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
            return disableDataVector.ToArray();
        }


    }
}
