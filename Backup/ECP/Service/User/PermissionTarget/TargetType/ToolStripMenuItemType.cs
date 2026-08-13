using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
namespace ECP.Service.User.PermissionTarget.TargetType
{
    class ToolStripMenuItemType : TargetType, IPermission
    {
        private ToolStripMenuItem target = null;

        public ToolStripMenuItemType(string key, ToolStripMenuItem target)
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

    class ContextMenuStripType : TargetType, IPermission
    {
        private ContextMenuStrip target = null;

        public ContextMenuStripType(string key, ContextMenuStrip target)
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

    class ToolStripItemType : TargetType, IPermission
    {
        private ToolStripItem target = null;

        public ToolStripItemType(string key, ToolStripItem target)
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

    class ToolStripDropDownType : TargetType, IPermission
    {
        private ToolStripDropDown target = null;

        public ToolStripDropDownType(string key, ToolStripDropDown target)
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

    class PanelType : TargetType, IPermission
    {
        private Panel target = null;

        public PanelType(string key, Panel target)
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
