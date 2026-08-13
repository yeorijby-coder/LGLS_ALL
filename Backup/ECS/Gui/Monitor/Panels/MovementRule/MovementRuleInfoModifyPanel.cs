using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;
using ECP.Global;

namespace HECS.Gui.Monitor.Panels.MovementRule
{
    public partial class MovementRuleInfoModifyPanel : UserControl
    {
        List<CodeMasterData> valueConfigList = null;

        public MovementRuleInfoModifyPanel()
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

        public void setComponentValues(MoveRuleData data)
        {
            setConditionData();

            this.labelRuleid.Text = data.Ruleid;
            this.textBoxRulename.Text = data.Rulename;
            this.labelReftable.Text = data.Reftable;
            this.labelRuleproperty.Text = data.Ruleproperty;

            this.textBoxRulevalue.Text = data.Rulevalue;
            this.textBoxRuledescription.Text = data.Ruledescription;
            this.textBoxPriority.Text = data.Priority;
            
            if (data.Disabled == "Y")
            {
                this.radioButtonNo.Checked = true;
                this.radioButtonYes.Checked = false;
            }
            else if (data.Disabled == "N")
            {
                this.radioButtonNo.Checked = false;
                this.radioButtonYes.Checked = true;
            }
            else
            {
                this.radioButtonNo.Checked = false;
                this.radioButtonYes.Checked = true;
            }

            this.labelCreatetime.Text = data.Createtime;
            this.labelCreateman.Text = data.Createman;
            this.labelUpdatetime.Text = data.Updatetime;
            this.labelUpdateman.Text = data.Updateman;
        }

        public MoveRuleData getComponentValues()
        {
            MoveRuleData data = new MoveRuleData();

            data.Ruleid = this.labelRuleid.Text;
            data.Rulename = this.textBoxRulename.Text;
            data.Reftable = this.labelReftable.Text;
            data.Ruleproperty = this.labelRuleproperty.Text;
            data.Rulevalue = this.textBoxRulevalue.Text;
            data.Ruledescription = this.textBoxRuledescription.Text;
            data.Priority = this.textBoxPriority.Text;

            if (this.radioButtonYes.Checked)
            {
                data.Disabled = "N";
            }
            else
            {
                data.Disabled = "Y";
            }

            if (string.IsNullOrEmpty(this.labelCreatetime.Text))
            {
                data.Createtime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                data.Createman = "SYSTEM";
            }
            else
            {
                data.Createtime = this.labelCreatetime.Text;
                data.Createman = this.labelCreateman.Text;
                data.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
                data.Updateman = "SYSTEM";
            }

            return data;
        }

        public void BringToFrontComponent(Boolean bFlag)
        {
            //if (bFlag)
            //{
            //    this.textBoxRuleid.BringToFront();
            //}
            //else
            //{
            //    this.labelRuleid.BringToFront();
            //}
        }
    }
}
