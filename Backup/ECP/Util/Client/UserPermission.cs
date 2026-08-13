using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.User;
using ECP.Global.Service;
using ECP.Util;
using ECP.Gui.LogIn;
using ECP.Global.Exception;
using ECP.Util.Client;

namespace ECP.Util.Client
{
    public class UserPermission
    {
        private static bool online = false;

        private static UserPermission instance = null;

        private Person offlineUser;

        private static bool isSuperUser = false;
        string userId = null;

        static Dictionary<string, List<IPermission>> targetList = new Dictionary<string, List<IPermission>>();
        static Dictionary<string, PermissionData> permissionList = new Dictionary<string, PermissionData>();

        private UserPermission()
        {

        }
        /// <summary>
        /// Permission Key를 사용하여 제한하고자 하는 Target을 Dictionay에 등록합니다.
        /// </summary>
        /// <param name="className">Target이 속한 Class 이름</param>
        /// <param name="target">IPermission을 구현한 target</param>
        public static void AddTarget(string className, IPermission target)
        {
            List<IPermission> tempTragetList= null;
            //permissionTargetList.Add(target);

            if (!targetList.TryGetValue(className, out tempTragetList))
            {
                tempTragetList = new List<IPermission>();
                tempTragetList.Add(target);
                targetList.Add(className, tempTragetList);
            }
            else
            {
                foreach (IPermission item in tempTragetList)
                {
                    if (item.GetId().Equals(target.GetId()))
                    {
                        return;
                    }
                }
                tempTragetList.Add(target);
            }
            
        }

        /// <summary>
        ///
        /// </summary>
        public static void SetSuperUser()
        {
            UserPermission.isSuperUser = true;            
        }

        /// <summary>
        /// Permission Key를 사용하여 제한하고자 하는 여러개의 Target을 Dictionay에 등록합니다.
        /// </summary>
        /// <param name="className">Targets이 속한 Class 이름</param>
        /// <param name="target">IPermission을 구현한 target의 List</param>
        public static void AddTarget(string className, List<IPermission> targets)
        {
            if (targets == null) return;
            if (targetList.ContainsKey(className))
            {
                targetList.Remove(className);
            }
            targetList.Add(className, targets);
        }

        public static UserPermission GetInstance()
        {
            if (instance == null)
            {
                instance = new UserPermission();


                try
                {
                    instance.SetUser("", "");

                }
                catch (Exception ex)
                {

                    //new LoginForm(instance, "", "").Show();
                }
            }
            return instance;
        }

        public static UserPermission GetInstance(
            string userId, string password, bool verifyOnly)
        {
            if (instance == null)
            {
                instance = new UserPermission();
            }

            if (verifyOnly)
            {
                instance.CheckUserPassword(userId, password);
            }
            else
            {
                isSuperUser = false;
                instance.SetUser(userId, password);
            }

            return instance;
        }

        public static void SetOnlineLogin(bool onlineLogin)
        {
            online = onlineLogin;
        }

        public static bool GetOnlineLogin()
        {
            return online;
        }

        public void CheckUserPassword(string userId, string password)
        {
            string encodedpassword = CryptorEngine.Encrypt(password, true);

            UserPermissionData userPermissionData = GetUserPermissionData(userId);

            if (!userPermissionData.userData.password.Trim().Equals(encodedpassword))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                                         userId, "Invalid Password");
            }
        }



        public void SetUser(string userId, string password)
        {
            this.userId = userId;

            if (Helper.IsNullOrEmpty(userId) || Helper.IsNullOrEmpty(password))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                                         userId, "Invalid UserId or Password");
            }

            string encodedpassword = CryptorEngine.Encrypt(password, true);

            permissionList.Clear();

            UserPermissionData userPermissionData = GetUserPermissionData(userId);

            if (!userPermissionData.userData.password.Trim().Equals(encodedpassword))
            {
                throw new ECPException(ECPExceptionType.INVALID_VALUE,
                                         userId, "Invalid Password");
            }


            for (int i = 0; i < userPermissionData.permissionDataList.Length; i++)
            {
                try
                {
                    permissionList.Add(userPermissionData.permissionDataList[i].permissionId,
                            userPermissionData.permissionDataList[i]);
                }
                catch (Exception ex)
                {
                    // If we have more than one role, this will occuer, Let it be
                }
            }
            this.ClearPermission();
        }

        private UserPermissionData GetUserPermissionData(string userId)
        {
            UserPermissionData userPermissionData;

            if (online)
            {
                userPermissionData = Naming.GetUserManager().GetUser(userId).GetUserPermissionData();


            }
            else
            {
                if (offlineUser == null)
                {
                    offlineUser = new Person(userId);
                }

                userPermissionData = offlineUser.GetUserPermissionData();
            }

            return userPermissionData;
        }

        public string GetUser()
        {
            if (userId == null)
            {
                return "";
            }
            else
            {
                return userId;
            }
        }

        public void RefreshPermissionData()
        {
            UserPermissionData userPermissionData = GetUserPermissionData(userId);

            permissionList.Clear();

            for (int i = 0; i < userPermissionData.permissionDataList.Length; i++)
            {
                permissionList.Add(userPermissionData.permissionDataList[i].permissionId,
                        userPermissionData.permissionDataList[i]);
            }
        }

        public bool HasPermission(String userId, String permissionId)
        {
            if (Helper.IsNullOrEmpty(permissionId))
            {
                return true;
            }

            UserPermissionData userPermissionData = GetUserPermissionData(userId);

            for (int i = 0; i < userPermissionData.permissionDataList.Length; i++)
            {
                if (userPermissionData.permissionDataList[i].permissionId.Equals(
                    permissionId))
                {
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// 현재 등록된 모든 Target에 대한 Permission을 해제 후,
        /// 현재 사용자에 대한 Permission에 맞게 재설정 합니다.
        /// </summary>
        /// <returns></returns>
        public bool DisableFuncationByUserPermission()
        {
            if (permissionList == null) return false;
            if (targetList == null) return false;
            if (isSuperUser == true) return true;

            this.ClearPermission();
            foreach (PermissionData pdata in permissionList.Values)
            {
                foreach (List<IPermission> itemList in targetList.Values)
                {
                    foreach (IPermission item in itemList)
                    {
                        if (item.GetId().Equals(pdata.permissionId))
                        {
                            item.Apply();
                        }
                        else
                        {
                            if (item == null) { }
                        }
                    }
                }
            }
            return true;
        }
        /// <summary>
        /// 현재 등록된 모든 Target에 대한 Permission을 해제합니다.
        /// </summary>
        /// <returns></returns>
        public bool ClearPermission()
        {
            if (targetList != null && targetList.Count > 0)
            {
                foreach (List<IPermission> itemList in targetList.Values)
                {
                    foreach (IPermission item in itemList)
                    {
                        item.Cancel();
                    }
                }
                return true;
            }
            return false;
        }

        public bool HasPermission(string permissionId)
        {
            if (isSuperUser == true) return true;

            if (Helper.IsNullOrEmpty(permissionId))
            {
                return false;
            }

            if (permissionList.ContainsKey(permissionId))
            {
                return true;
            }
            else
            {
                return false;
            }

        }
        public void SetLogOut()
        {
            this.userId = null;
            permissionList.Clear();
        }
    }
}
