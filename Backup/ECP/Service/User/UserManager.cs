using System;
using System.Collections.Generic;
using System.Text;

using ECP.Db;
using ECP.Service.Logger;
using ECP.Global.Exception;
using ECP.Global.Service;
using ECP.Util;

namespace ECP.Service.User
{
    public class UserManager : AbstractObjectManager
    {

        const string CLASS = "UserManager";

        public UserManager(string ownerId, string managerId)
            : base(ownerId, managerId)
        {
            this.Initialize();
        }

        private void Initialize()
        {
        }

        private Person NewUser(String userId,String password,
                String userName,String userDescription, int passwordMaxAge,
                bool disabled)
        {
            const string METHOD = CLASS + "." + "NewUser";

            if (this.ManagerId.Equals(userId))
            {
                Log.log(1, METHOD, "DuplicateIdError", Log.ERROR, "", userId,
                        "Attempt to create " +
                        "User(" + userId + ") " +
                        "with the same id as of the " +
                        "Server(" + this.ManagerId + "). ");

                throw new ECPException(
                    ECPExceptionType.DUPLICATE,
                    userId, "Id cannot be the same as the server's name." );
            }

            try
            {
                Person userImpl =
                    Person.NewUser(
                        userId, password, userName,
                        userDescription, passwordMaxAge, disabled);

                   
                this.CacheObject(userId,userImpl);

                // Create and return the interface object.
                return userImpl;
            }
            catch (ECPException ex)
            {
                throw ex;
            }
            catch (Exception ex)
            {
                String message =
                    "Failed to create User(" + userId + "). ";
                Log.log(1, METHOD, "UserCreateError", Log.ERROR, "", userId,
                        message + "\n" + ex.ToString());

                throw new ECPException(
                    ECPExceptionType.OPERATION_ABORTED, userId,
                    message + "See logs for more details. ");
            }
        }

        public Person CreateUser(String id,String password,
            String userName,String userDescription, int passwordMaxAge,
            bool disabled)
        {
             return NewUser(id, password, userName, userDescription, passwordMaxAge,  disabled);
        }

        public void DeleteUser(String id)
        {

            GetUserImpl(id).Delete();

            RemoveObject(id);
        }

        public Person GetUser( String id)
        {
            return ((Person)GetUserImpl(id));
        }

        private Person GetUserImpl(String id)
        {
            const string METHOD = CLASS + "." + "GetUserImpl";
            Person userImpl = null;
            try
            {
                userImpl = (Person)GetObject(id);

                if (userImpl == null)
                {
                    lock (GetSynchronizationObject())
                    {
                        userImpl = (Person)GetObject(id);
                        if (userImpl == null)
                        {
                            userImpl = new Person(id);
                            

                            CacheObject(id, userImpl);
                        }
                    }
                }
                return userImpl;
            }
            catch (ECPException ex)
            {
                throw (ex);
            }
            catch (Exception ex)
            {
                String message =
                    "Failed to access User(" + id + "). ";
                Log.log(1, METHOD, "UserAccessError", Log.ERROR,
                        "", id,
                        message + "\n" +ex.ToString());
                throw new ECPException(
                    ECPExceptionType.OPERATION_ABORTED,
                    id,
                    message + "See logs for more details. ");
            }
        }

        public string[] GetUserIds()
        {
            return (Person.GetUserIdList());
        }

        public string[] GetUserIdsWithRole(string role)
        {
            return (Person.GetUserIdsWithRole(role));
        }

        public UserData[] GetUserList()
        {
            return (Person.GetUserList());
        }

        public string[] GetPermissionIdList()
        {
            return (Permission.GetPermissionIdList());
        }

        public PermissionData[] GetPermissionList()
        {
            return (Permission.GetPermissionList());
        }

        public string[] GetPermissionIdListByRole(string theRoleId)
        {
            return (Role.GetPermissionIdList(theRoleId));
        }

        public bool IsPasswordExpired(string userId)
        {
            Person user = this.GetUser(userId);

            return user.IsPasswordExpired();
        }

        public void CreatePermission(string thePermissionId, string thePermissionDescription)
        {
            Permission.NewPermission(thePermissionId, thePermissionDescription);
        }

        public void DeletePermission(string thePermissionId)
        {
            Permission.DeletePermission(thePermissionId);
            Role.DeleteRolePermission(thePermissionId);
        }
        public void ModifyPermission(string thePermissionId,
            string thePermissionDescription)
        {
            Permission.SetPermissionData(thePermissionId,  thePermissionDescription);
        }

        public void ReplacePermissionList(string[] oldPermissionIdList,
            PermissionData[] newPermissionList)
        {
            const string METHOD = CLASS + ".replacePermissionList";

            for (int index = 0; index < oldPermissionIdList.Length; index++)
            {
                try
                {
                    DeletePermission(oldPermissionIdList[index]);
                }
                catch (ECPException ex)
                {
                    if (!ex.type.Equals(ECPExceptionType.NOT_FOUND))
                    {
                        Log.log(2, METHOD, "Permission: deleting old", Log.ERROR,
                            "", newPermissionList[index].permissionId,
                            ex.ToString());

                        throw ex;
                    }
                }
            }

            for (int index = 0; index < newPermissionList.Length; index++)
            {
                try
                {
                    CreatePermission(
                            newPermissionList[index].permissionId,
                            newPermissionList[index].permissionDescription);
                }
                catch (ECPException ex)
                {
                    if (!ex.type.Equals(ECPExceptionType.DUPLICATE))
                    {
                        Log.log(2, METHOD, "Permission: adding new", Log.ERROR,
                            "", newPermissionList[index].permissionId,
                            ex.ToString());

                        throw ex;
                    }
                }
            }
        }

        public RoleData[] GetRoleList()
        {
            return (Role.GetRoleList());
        }

        public string[] GetRoleIdList()
        {
           return(Role.GetRoleIdList());
        }

        public string[] GetAvailablePermissionIdByRole(string theRoleId)
        {
            return (Role.GetAvailablePermissionIdList(theRoleId));
        }



        public void SetRolePermissionIdList(string theRoleId, string[] theRolePermissionList)
        {
            Role.SetPermissionIdList(theRoleId, theRolePermissionList);
        }

        public void CreateRole(string theRoleId,string theRoleDescription)
        {
            Role.NewRole(theRoleId, theRoleDescription);
        }

        public void DeleteRole(string theRoleId)
        {
            lock (GetSynchronizationObject())
            {
                ClearCache();
                Role.DeleteRole(theRoleId);
                Person.RemoveRoleFromUsers(theRoleId);
            }
        }

        public void ModifyRole(string theRoleId, string theRoleDescription)
        {
            Role.SetRoleDescription(theRoleId, theRoleDescription);
        }

        public bool IsAdministrator(string userId)
        {
            string[] userAdminList =
                GetUserIdsWithRole("Administrator");
            for (int i=0; i<userAdminList.Length; i++)
            {
                if (userAdminList[i].Equals(userId))
                {
                   return (true);
                }
            }
            return (false);
        }

        public bool[] GetDisableDataList()
        {
            return (Person.GetDisableDataList());
        }

        public bool ChangePassword(string userId, string newPassword)
        {
            Person user = GetUser(userId);

            if (user != null)
            {
                string encodedPassworld = CryptorEngine.Encrypt(newPassword, true);

                try
                {
                    UserData userData = user.GetData();

                    userData.password = encodedPassworld;
                    user.SetInformation(userData, user.TheRoleIdList);

                    return true;
                }
                catch (ECPException ex)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }



        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }
    }
}
