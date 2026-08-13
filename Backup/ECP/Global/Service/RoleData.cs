using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Service
{
    [Serializable]
    public class RoleData
    {
        public String roleId = null;
        public String roleDescription = null;

        public RoleData()
        {
        } // ctor

        public RoleData(string roleId, string desc)
        {
            this.roleId = roleId;
            this.roleDescription = desc;
        } // ct
    }
}
