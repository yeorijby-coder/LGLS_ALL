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
    public partial class SystemInfoModifyPanel : UserControl
    {
        public SystemInfoModifyPanel()
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
            
            this.textBoxSystemcd.Text = data.Systemcd;
            this.labelSystemcd.Text = data.Systemcd;

            this.textBoxCodeclass.Text = data.Codeclass;
            this.labelCodeclass.Text = data.Codeclass;

            this.textBoxCode.Text = data.Code;
            this.labelCode.Text = data.Code;

            this.textBoxValue.Text = data.Value;

            if (data.Disabled == "Y")
            {
                this.radioButtonYes.Checked = true;
                this.radioButtonNo.Checked = false;
            }
            else
            {
                this.radioButtonYes.Checked = false;
                this.radioButtonNo.Checked = true;
            }
            this.textBoxRemark.Text = data.Remark;
            this.textBoxAttribute1.Text = data.Attribute1;
            this.textBoxAttribute2.Text = data.Attribute2;
            this.textBoxAttribute3.Text = data.Attribute3;
            this.textBoxAttribute4.Text = data.Attribute4;
            this.textBoxAttribute5.Text = data.Attribute5;
        }

        public CodeMasterData getComponentValues()
        {
            CodeMasterData data = new CodeMasterData();

            if (string.IsNullOrEmpty(labelSystemcd.Text))
            {
                data.Systemcd = this.textBoxSystemcd.Text;
            }
            else
            {
                data.Systemcd = this.labelSystemcd.Text;
            }

            if (string.IsNullOrEmpty(labelCodeclass.Text))
            {
                data.Codeclass = this.textBoxCodeclass.Text;
            }
            else
            {
                data.Codeclass = this.labelCodeclass.Text;
            }

            if (string.IsNullOrEmpty(labelCode.Text))
            {
                data.Code = this.textBoxCode.Text;
            }
            else
            {
                data.Code = this.labelCode.Text;
            }

            data.Value = this.textBoxValue.Text;

            if (this.radioButtonYes.Checked)
            {
                data.Disabled = "Y";
            }
            else
            {
                data.Disabled = "N";
            }

            data.Remark = this.textBoxRemark.Text;
            data.Attribute1 = this.textBoxAttribute1.Text;
            data.Attribute2 = this.textBoxAttribute2.Text;
            data.Attribute3 = this.textBoxAttribute3.Text;
            data.Attribute4 = this.textBoxAttribute4.Text;
            data.Attribute5 = this.textBoxAttribute5.Text;

            return data;
        }

        public void BringToFrontComponent(Boolean bFlag)
        {
            if (bFlag)
            {
                this.textBoxSystemcd.BringToFront();
                this.textBoxCodeclass.BringToFront();
                this.textBoxCode.BringToFront();
            }
            else
            {
                this.labelSystemcd.BringToFront();
                this.labelCodeclass.BringToFront();
                this.labelCode.BringToFront();
            }
        }
    }
}
