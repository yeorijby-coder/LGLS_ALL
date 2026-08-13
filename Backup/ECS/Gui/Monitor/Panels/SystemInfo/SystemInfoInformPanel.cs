using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;
using ECP.Global;

namespace HECS.Gui.Monitor.Panels.SystemInfo
{
    public partial class SystemInfoInformPanel : UserControl
    {
        public SystemInfoInformPanel()
        {
            InitializeComponent();
            InitComponent();
        }

        private void InitComponent()
        {
            setConditionData();
        }

        private void setConditionData()
        {
        }

        public void setComponentValues(CodeMasterData data)
        {
            setConditionData();

            this.labelSystemcd.Text = data.Systemcd;
            this.labelCodeclass.Text = data.Codeclass;
            this.labelCode.Text = data.Code;
            this.labelValue.Text = data.Value;
            this.labelDisabled.Text = data.Disabled;
            this.labelRemark.Text = data.Remark;
            this.labelAttribute1.Text = data.Attribute1;
            this.labelAttribute2.Text = data.Attribute2;
            this.labelAttribute3.Text = data.Attribute3;
            this.labelAttribute4.Text = data.Attribute4;
            this.labelAttribute5.Text = data.Attribute5;
        }

        public CodeMasterData getComponentValues()
        {
            CodeMasterData data = new CodeMasterData();

            data.Systemcd = this.labelSystemcd.Text;
            data.Codeclass = this.labelCodeclass.Text;
            data.Code = this.labelCode.Text;
            data.Value = this.labelValue.Text;
            data.Disabled = this.labelDisabled.Text;
            data.Remark = this.labelRemark.Text;
            data.Attribute1 = this.labelAttribute1.Text;
            data.Attribute2 = this.labelAttribute2.Text;
            data.Attribute3 = this.labelAttribute3.Text;
            data.Attribute4 = this.labelAttribute4.Text;
            data.Attribute5 = this.labelAttribute5.Text; 

            return data;
        }
    }
}
