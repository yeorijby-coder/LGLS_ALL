using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Service
{
    [Serializable]
    public class UserPermissionData
    {
        public UserData userData = null;
        public RoleData[] roleDataList = null;
        public PermissionData[] permissionDataList = null;

        public UserPermissionData ()
        {
        } // ctor

        public UserPermissionData (UserData _userData, RoleData[] _roleDataList, 
            PermissionData[] _permissionDataList)
        {
            userData = _userData;
            roleDataList = _roleDataList;
            permissionDataList = _permissionDataList;
        } 
    }
}
