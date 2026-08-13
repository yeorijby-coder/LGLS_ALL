using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace ECP.Service.User.PermissionTarget.TargetType
{
    class DefaultType : TargetType, IPermission
    {
        private Object target = null;

        public DefaultType(string key, Object target)
        {
            this.permissionKey = key;
            this.target = target;
        }

        #region IPermission 멤버
        public void Apply()
        {
        }

        public void Cancel()
        {
        }
        public string GetId()
        {
            return this.permissionKey;
        }
        #endregion
    }
}
