using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Service.User.PermissionTarget.TargetType
{
    class TargetType
    {
        protected string permissionKey = "";
        public string GetPermissionKey()
        {
            return this.permissionKey;
        }
    }
}
