using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Service
{
    [Serializable]
    public class PermissionData
    {
        public String permissionId = null;
        public String permissionDescription = null;

        public PermissionData()
        {
        } // ctor

        public PermissionData(string permissionId, string permissionDescription)
        {
            this.permissionId = permissionId;
            this.permissionDescription = permissionDescription;
        } // ctor
    }
}
