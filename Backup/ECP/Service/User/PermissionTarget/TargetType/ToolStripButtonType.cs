using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace ECP.Service.User.PermissionTarget.TargetType
{
    class ToolStripButtonType : TargetType, IPermission
    {
        private ToolStripButton target = null;
        public ToolStripButtonType(string key, ToolStripButton target)
        {
            this.permissionKey = key;
            this.target = target;
        }

        #region IPermission 멤버
        public void Apply()
        {
            this.target.Enabled = true;
        }

        public void Cancel()
        {
            this.target.Enabled = false;
        }
        public string GetId()
        {
            return this.permissionKey;
        }
        #endregion
    }
}
