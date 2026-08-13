using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Globalization;
using ECP.Db.Entity;
using ECP.Global;
using HECS.Gui.Monitor.Panels.MovementRule;
using ECP.Util;
using HECS.Gui.Monitor.Popups;
using HECS.Util;

namespace HECS.Gui.Monitor.Panels
{
    public partial class MovementRuleInfoMainPanel : UserControl
    {
        MovementRuleInfoModifyPanel movementRuleInfoModifyPanel = null;
        MovementRuleInfoInformPanel movementRuleInfoInformPanel = null;

        List<CodeMasterData> valueConfigList = null;
        
        Dictionary<string, MoveRuleData> moveRuleDataList = null;
        private static Boolean AddModifyFlag = false;

        public MovementRuleInfoMainPanel()
        {
            InitializeComponent();
            InitComponent();
            InitializeDataGrid();
        }

        private enum EnableType
        {
            INIT = 1,
            REG = 2,
            RETRIEVE = 3,
            MODIFY = 4,
            DELETE = 5
        }

        private void InitComponent()
        {
            setConditionData();

            //Panel2에 상세 화면 추가
            addDetailPanel();

            //Buttom Enabled Setting
            setButtonEnabled(EnableType.INIT);
        }

        private void setConditionData()
        {
            valueConfigList = null;

            CodeMasterData codeData = new CodeMasterData();
            valueConfigList = codeData.getValueConfigValues(GlobalConstant.ALL_CHARACTER);
        }

        private void setButtonEnabled(EnableType enableType)
        {
            if (enableType == EnableType.INIT)
            {
                buttonAdd.Enabled = false;
                buttonModify.Enabled = false;
                buttonDelete.Enabled = false;
                buttonSave.Enabled = false;
            }
            else if (enableType == EnableType.REG)
            {
                buttonAdd.Enabled = false;
                buttonModify.Enabled = false;
                buttonDelete.Enabled = false;
                buttonSave.Enabled = true;
            }
            else if (enableType == EnableType.RETRIEVE)
            {
                if (dataGridView.RowCount > 0)
                {
                    buttonAdd.Enabled = true;
                    buttonModify.Enabled = true;
                    buttonDelete.Enabled = true;
                    buttonSave.Enabled = false;
                }
                else
                {
                    buttonAdd.Enabled = true;
                    buttonModify.Enabled = false;
                    buttonDelete.Enabled = false;
                    buttonSave.Enabled = false;
                }
            }
            else if (enableType == EnableType.MODIFY)
            {
                buttonAdd.Enabled = false;
                buttonModify.Enabled = false;
                buttonDelete.Enabled = false;
                buttonSave.Enabled = true;
            }
            else if (enableType == EnableType.DELETE)
            {
                buttonAdd.Enabled = false;
                buttonModify.Enabled = false;
                buttonDelete.Enabled = false;
                buttonSave.Enabled = false;
            }
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //반송 룰 번호 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //반송 룰 명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //룰 유형
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //값 조건
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //하한 값
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //상한 값
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //우선 순위
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //사용 여부
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //생성 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //생성자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //수정 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //수정자

                dataGridView.Columns[0].HeaderText = "반송 룰 번호";
                dataGridView.Columns[1].HeaderText = "반송 룰 명";
                dataGridView.Columns[2].HeaderText = "관련 Table";
                dataGridView.Columns[3].HeaderText = "관련 속성";
                dataGridView.Columns[4].HeaderText = "관련 값";
                dataGridView.Columns[5].HeaderText = "상세 설명";
                dataGridView.Columns[6].HeaderText = "우선 순위";
                dataGridView.Columns[7].HeaderText = "사용 여부";
                dataGridView.Columns[8].HeaderText = "생성 일자";
                dataGridView.Columns[9].HeaderText = "생성자";
                dataGridView.Columns[10].HeaderText = "수정 일자";
                dataGridView.Columns[11].HeaderText = "수정자";

                dataGridView.Columns[0].Width = 100;
                dataGridView.Columns[1].Width = 100;
                dataGridView.Columns[2].Width = 110;
                dataGridView.Columns[3].Width = 110;
                dataGridView.Columns[4].Width = 130;
                dataGridView.Columns[5].Width = 400;
                dataGridView.Columns[6].Width = 80;
                dataGridView.Columns[7].Width = 80;
                dataGridView.Columns[8].Width = 130;
                dataGridView.Columns[9].Width = 130;
                dataGridView.Columns[10].Width = 130;
                dataGridView.Columns[11].Width = 130;

 
                dataGridView.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridView.AllowUserToAddRows = false;

                dataGridView.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridView.AllowUserToAddRows = false;
                dataGridView.MultiSelect = false;

                dataGridView.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private void setDataGridView(MoveRuleData data)
        {
            DataGridViewRow addedRow = new DataGridViewRow();
            int idx = dataGridView.Rows.Add(addedRow);

            dataGridView.Rows[idx].Cells[0].Value = data.Ruleid;
            dataGridView.Rows[idx].Cells[1].Value = data.Rulename;
            dataGridView.Rows[idx].Cells[2].Value = data.Reftable;
            dataGridView.Rows[idx].Cells[3].Value = data.Ruleproperty;
            dataGridView.Rows[idx].Cells[4].Value = data.Rulevalue;
            dataGridView.Rows[idx].Cells[5].Value = data.Ruledescription;
            dataGridView.Rows[idx].Cells[6].Value = data.Priority;
            dataGridView.Rows[idx].Cells[7].Value = data.Disabled;
            dataGridView.Rows[idx].Cells[8].Value = data.Createtime;
            dataGridView.Rows[idx].Cells[9].Value = data.Createman;
            dataGridView.Rows[idx].Cells[10].Value = data.Updatetime;
            dataGridView.Rows[idx].Cells[11].Value = data.Updateman;    
        }

        private MoveRuleData getDataGridView(int idx)
        {
            MoveRuleData data = new MoveRuleData();
            data.Ruleid = (string) dataGridView.Rows[idx].Cells[0].Value;
            data.Rulename = (string)dataGridView.Rows[idx].Cells[1].Value;
            data.Reftable = (string)dataGridView.Rows[idx].Cells[2].Value;
            data.Ruleproperty = (string)dataGridView.Rows[idx].Cells[3].Value;
            data.Rulevalue = (string)dataGridView.Rows[idx].Cells[4].Value;
            data.Ruledescription = (string)dataGridView.Rows[idx].Cells[5].Value;
            data.Priority = (string)dataGridView.Rows[idx].Cells[6].Value;
            data.Disabled = (string)dataGridView.Rows[idx].Cells[7].Value;
            data.Createtime = (string)dataGridView.Rows[idx].Cells[8].Value;
            data.Createman = (string)dataGridView.Rows[idx].Cells[9].Value;
            data.Updatetime = (string)dataGridView.Rows[idx].Cells[10].Value;
            data.Updateman = (string)dataGridView.Rows[idx].Cells[11].Value;

            return data;
        }

        private void buttonRetrieve_Click(object sender, EventArgs e)
        {
            MoveRuleData data = new MoveRuleData();

            moveRuleDataList = null;
            moveRuleDataList = data.getMoveRuleDataListOrderPriority();

            dataGridView.Rows.Clear();
            dataGridView.AllowUserToAddRows = true;

            foreach (MoveRuleData dataTemp in moveRuleDataList.Values)
            {
                Boolean setFlag = true;

                if (setFlag)
                {
                    setDataGridView(dataTemp);
                }
            }

            dataGridView.AllowUserToAddRows = false;

            AddModifyFlag = false;

            if (dataGridView.Rows.Count > 0)
            {
                displayDetailData(0);
            }
            else
            {
                movementRuleInfoInformPanel.setComponentValues(new MoveRuleData());
                movementRuleInfoModifyPanel.setComponentValues(new MoveRuleData());
            }
            setButtonEnabled(EnableType.RETRIEVE);

        }


        private void buttonExcel_Click(object sender, EventArgs e)
        {
            ExcelExport exportFile = new ExcelExport();

            if (dataGridView.RowCount > 0)
            {
                Boolean bFlag = exportFile.saveCSVFile(dataGridView, this.GetType().Name);

                if (bFlag)
                {
                    Util.Util.ShowMesagePopup("C:\\ Temp 폴더에 파일이 저장되었습니다.");
                }
                else
                {
                    Util.Util.ShowMesagePopup("파일 저장에 실패했습니다.");
                }
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.ParentForm.WindowState = FormWindowState.Minimized;
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            AddModifyFlag = true;
            movementRuleInfoModifyPanel.setComponentValues(new MoveRuleData()); 
            movementRuleInfoModifyPanel.BringToFront();
            movementRuleInfoModifyPanel.BringToFrontComponent(AddModifyFlag);
            
            setButtonEnabled(EnableType.REG);
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            AddModifyFlag = false;
            movementRuleInfoModifyPanel.BringToFront();
            movementRuleInfoModifyPanel.BringToFrontComponent(AddModifyFlag);

            setButtonEnabled(EnableType.REG);
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;
            popUpOpen(rows[0]);
        }

        private void popUpOpen(DataGridViewRow row)
        {
            string ruleid = (string)row.Cells[0].Value;
            String message = "반송 룰 ID: (" + ruleid + ") 를 삭제하시겠습니까?";

            DeleteCheckDialog form = (DeleteCheckDialog)SimpleCache.GetObject("Popup", this.GetType().Name);

            if (form == null)
            {
                form = new DeleteCheckDialog();
                SimpleCache.AddObject("Popup", this.GetType().Name, form);
                form.OnButtonSelected += new DeleteCheckDialog.DelegateButtonSelected(form_OnButtonSelected);
            }
            form.MessageData = message;
            form.FormName = this.GetType().Name;

            form.Show();
        }

        void form_OnButtonSelected(DialogResult result, string formClass)
        {
            if (result == DialogResult.Yes && (this.GetType().Name == formClass))
            {
                DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;

                string ruleid = (string)rows[0].Cells[0].Value;

                MoveRuleData data = new MoveRuleData();
                data = moveRuleDataList[ruleid];

                data.deleteMoveRuleData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(null, null);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (AddModifyFlag)
            {
                MoveRuleData moveRuleData = null;
                moveRuleData = movementRuleInfoModifyPanel.getComponentValues();

                moveRuleData.insertMoveRuleData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(sender, e);
            }
            else
            {
                MoveRuleData moveRuleData = null;

                moveRuleData = movementRuleInfoModifyPanel.getComponentValues();

                moveRuleData.updateMoveRuleData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(sender, e);
            }

            setConditionData();
        }

        private void buttonUp_Click(object sender, EventArgs e)
        {
            if (dataGridView.SelectedRows[0].Index <= 0)
            {
                return;
            }

            int index = dataGridView.SelectedRows[0].Index;

            DataGridViewRow row = dataGridView.Rows[index];

            dataGridView.Rows.Remove(row);
            dataGridView.Rows.Insert(index - 1, row);

            //SortedPriority();

            if (dataGridView.SelectedRows.Count > 0)
            {
                dataGridView.SelectedRows[0].Selected = false;
            }
            row.Selected = true;
            ScrollGrid();
        }

        private void buttonDown_Click(object sender, EventArgs e)
        {
            if (dataGridView.SelectedRows[0].Index >= dataGridView.Rows.Count - 1)
            {
                return;
            }

            int index = dataGridView.SelectedRows[0].Index;
            DataGridViewRow row = dataGridView.Rows[index];

            dataGridView.Rows.Remove(row);
            dataGridView.Rows.Insert(index + 1, row);

            //SortedPriority();

            if (dataGridView.SelectedRows.Count > 0)
            {
                dataGridView.SelectedRows[0].Selected = false;
            }

            row.Selected = true;
            ScrollGrid();

        }

        private void SortedPriority()
        {
            int iTotal = dataGridView.Rows.Count;
            for (int inx = 0; inx < iTotal; inx++)
            {
                int iRow = inx + 1;
                dataGridView.Rows[inx].Cells[6].Value = iRow.ToString();
            }
        }

        private void ScrollGrid()
        {
            int halfWay = (dataGridView.DisplayedRowCount(false) / 2);
            if (dataGridView.FirstDisplayedScrollingRowIndex + halfWay > dataGridView.SelectedRows[0].Index ||
                (dataGridView.FirstDisplayedScrollingRowIndex + dataGridView.DisplayedRowCount(false) - halfWay) <= dataGridView.SelectedRows[0].Index)
            {
                int targetRow = dataGridView.SelectedRows[0].Index;

                targetRow = Math.Max(targetRow - halfWay, 0);
                dataGridView.FirstDisplayedScrollingRowIndex = targetRow;

            }
        }

        private void buttonSave1_Click(object sender, EventArgs e)
        {
            int iTotalRow = dataGridView.Rows.Count;

            for (int inx = 0; inx < iTotalRow; inx++)
            {
                MoveRuleData data = new MoveRuleData();
                data = getDataGridView(inx);

                data.updateMoveRuleData();
            }
            setButtonEnabled(EnableType.INIT);
            buttonRetrieve_Click(sender, e);
        }

        private void dataGridView_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex < 0)
            {
                return;
            }

            displayDetailData(e.RowIndex);
        }

        private void displayDetailData(int iRow)
        {
            string ruleid = (string)dataGridView.Rows[iRow].Cells[0].Value;

            MoveRuleData data = new MoveRuleData();
            data = moveRuleDataList[ruleid];

            movementRuleInfoInformPanel.setComponentValues(new MoveRuleData());
            movementRuleInfoModifyPanel.setComponentValues(new MoveRuleData());

            movementRuleInfoInformPanel.setComponentValues(data);
            movementRuleInfoModifyPanel.setComponentValues(data);

            movementRuleInfoInformPanel.BringToFront();
            setButtonEnabled(EnableType.RETRIEVE);
        }

        private void addDetailPanel()
        {
            movementRuleInfoInformPanel = new MovementRuleInfoInformPanel();

            this.splitContainer1.Panel2.Controls.Add(movementRuleInfoInformPanel);

            movementRuleInfoInformPanel.AutoScroll = true;
            movementRuleInfoInformPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            movementRuleInfoInformPanel.Location = new System.Drawing.Point(0, 0);
            movementRuleInfoInformPanel.Name = "movementRuleInfoInformPanel";
            movementRuleInfoInformPanel.Size = new System.Drawing.Size(994, 183);
            movementRuleInfoInformPanel.TabIndex = 0;

            movementRuleInfoModifyPanel = new MovementRuleInfoModifyPanel();

            this.splitContainer1.Panel2.Controls.Add(movementRuleInfoModifyPanel);

            movementRuleInfoModifyPanel.AutoScroll = true;
            movementRuleInfoModifyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            movementRuleInfoModifyPanel.Location = new System.Drawing.Point(0, 0);
            movementRuleInfoModifyPanel.Name = "movementRuleInfoModifyPanel";
            movementRuleInfoModifyPanel.Size = new System.Drawing.Size(994, 183);
            movementRuleInfoModifyPanel.TabIndex = 0;
        }

    }
}
