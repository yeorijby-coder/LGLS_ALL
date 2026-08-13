using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Service.User.PermissionTarget
{
    public abstract class PermissionTarget
    {
        public IPermission CreateType(string key, object targetType)
        {
            IPermission target;
            target = this.TargetTypeDefine(key, targetType);
            return target;
        }

        protected abstract IPermission TargetTypeDefine(string key, object type);
    }
}
