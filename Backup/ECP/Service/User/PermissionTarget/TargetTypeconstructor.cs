using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.User.PermissionTarget.TargetType;
using System.Windows.Forms;
namespace ECP.Service.User.PermissionTarget
{
    public class TargetTypeConstructor : PermissionTarget
    {
        protected override IPermission TargetTypeDefine(string key, object type)
        {
            const string CLASS = "PermissionTarget";
            try
            {
                if (type.GetType().IsSubclassOf(typeof(ToolStripItem)))
                {
                    return new ToolStripItemType(key, (ToolStripItem)type);
                }
                else if (type.GetType().IsSubclassOf(typeof(ToolStripDropDown)))
                {
                    return new ToolStripDropDownType(key, (ToolStripDropDown)type);
                }
                else if (type.GetType().IsSubclassOf(typeof(Control)))
                {
                    return new ControlType(key, (Control)type);
                }
                else if (type.GetType().IsSubclassOf(typeof(Panel)))
                {
                    return new PanelType(key, (Panel)type);
                }
                else
                {
                    Console.WriteLine(key + ",selected object type (" + type.ToString() + ")");
                    return new DefaultType(key, type);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(CLASS + ", " + ex.Source + ", " + ex.Message);
                return new DefaultType(key, type);
            }
        }
    }
}
