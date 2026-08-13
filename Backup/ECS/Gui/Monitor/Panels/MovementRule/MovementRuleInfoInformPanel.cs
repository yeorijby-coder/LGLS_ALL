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
    public partial class MovementRuleInfoInformPanel : UserControl
    {
        List<CodeMasterData> valueConfigList = null;

        public MovementRuleInfoInformPanel()
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
            valueConfigList = null;

            CodeMasterData codeData = new CodeMasterData();
            valueConfigList = codeData.getValueConfigValues(GlobalConstant.ALL_CHARACTER);
        }

        public void setComponentValues(MoveRuleData data)
        {
            setConditionData();

            this.labelRuleid.Text = data.Ruleid;
            this.labelRulename.Text = data.Rulename;
            this.labelReftable.Text = data.Reftable;
            this.labelRuleproperty.Text = data.Ruleproperty;

            this.labelRulevalue.Text = data.Rulevalue;
            this.labelRuledescription.Text = data.Ruledescription;
            this.labelPriority.Text = data.Priority;
            this.labelDisabled.Text = data.Disabled;
            this.labelCreatetime.Text = data.Createtime;
            this.labelCreateman.Text = data.Createman;
            this.labelUpdatetime.Text = data.Updatetime;
            this.labelUpdateman.Text = data.Updateman;
            
        }

        public MoveRuleData getComponentValues()
        {
            MoveRuleData data = new MoveRuleData();

            string valueconfig = "";

            data.Ruleid = this.labelRuleid.Text;
            data.Rulename = this.labelRulename.Text;
            data.Reftable = this.labelReftable.Text;
            data.Ruleproperty = this.labelRuleproperty.Text;
            data.Rulevalue = this.labelRulevalue.Text;
            data.Ruledescription = this.labelRuledescription.Text;
            data.Priority = this.labelPriority.Text;
            data.Disabled = this.labelDisabled.Text;
            data.Createtime = this.labelCreatetime.Text;
            data.Createman = this.labelCreateman.Text;
            data.Updatetime = this.labelUpdatetime.Text;
            data.Updateman = this.labelUpdateman.Text;

            return data;
        }
    }
}
