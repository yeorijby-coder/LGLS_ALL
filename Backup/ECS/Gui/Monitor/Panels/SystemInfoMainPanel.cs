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
using HECS.Gui.Monitor.Panels.SystemInfo;
using ECP.Util;
using HECS.Gui.Monitor.Popups;
using HECS.Util;

namespace HECS.Gui.Monitor.Panels
{
    public partial class SystemInfoMainPanel : UserControl
    {
        SystemInfoModifyPanel systemInfoModifyPanel = null;
        SystemInfoInformPanel systemInfoInformPanel = null;

        List<CodeMasterData> systemValueList = null; 
        List<CodeMasterData> codeMasterDataList = null;

        private static Boolean AddModifyFlag = false;

        public SystemInfoMainPanel()
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
            systemValueList = null;

            CodeMasterData codeData = new CodeMasterData();
            systemValueList = codeData.getSystemInfoValues();

            comboBoxSystemInfo.Items.Clear();
            comboBoxSystemInfo.Items.Add(GlobalConstant.ALL_TEXT);
            foreach (CodeMasterData dataTemp in systemValueList)
            {
                comboBoxSystemInfo.Items.Add(dataTemp.Value);
            }

            if (comboBoxSystemInfo.Items.Count > 0)
            {
                comboBoxSystemInfo.SelectedIndex = 0;
            }
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
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //시스템 코드 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //코드 번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //코드 명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //코드 값
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //미사용 여부
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //비고
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //속성 1
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //속성 2
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //속성 3
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //속성 4
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //속성 5

                dataGridView.Columns[0].HeaderText = "시스템 코드";
                dataGridView.Columns[1].HeaderText = "코드 번호";
                dataGridView.Columns[2].HeaderText = "코드 명";
                dataGridView.Columns[3].HeaderText = "코드 값";
                dataGridView.Columns[4].HeaderText = "미사용 여부";
                dataGridView.Columns[5].HeaderText = "비고";
                dataGridView.Columns[6].HeaderText = "속성 1";
                dataGridView.Columns[7].HeaderText = "속성 2";
                dataGridView.Columns[8].HeaderText = "속성 3";
                dataGridView.Columns[9].HeaderText = "속성 4";
                dataGridView.Columns[10].HeaderText = "속성 5";

                dataGridView.Columns[0].Width = 130;
                dataGridView.Columns[1].Width = 130;
                dataGridView.Columns[2].Width = 130;
                dataGridView.Columns[3].Width = 130;
                dataGridView.Columns[4].Width = 130;
                dataGridView.Columns[5].Width = 130;
                dataGridView.Columns[6].Width = 130;
                dataGridView.Columns[7].Width = 130;
                dataGridView.Columns[8].Width = 130;
                dataGridView.Columns[8].Width = 130;
                dataGridView.Columns[9].Width = 130;

 
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

        private void setDataGridView(CodeMasterData data)
        {
            DataGridViewRow addedRow = new DataGridViewRow();
            int idx = dataGridView.Rows.Add(addedRow);

            string systeminfo = "";

            foreach (CodeMasterData dataTemp in systemValueList)
            {
                if (data.Codeclass == dataTemp.Codeclass)
                {
                    systeminfo = dataTemp.Value;
                    break;
                }
            }

            dataGridView.Rows[idx].Cells[0].Value = data.Systemcd;
            dataGridView.Rows[idx].Cells[1].Value = systeminfo;
            dataGridView.Rows[idx].Cells[2].Value = data.Code;
            dataGridView.Rows[idx].Cells[3].Value = data.Value;
            dataGridView.Rows[idx].Cells[4].Value = data.Disabled;
            dataGridView.Rows[idx].Cells[5].Value = data.Remark;
            dataGridView.Rows[idx].Cells[6].Value = data.Attribute1;
            dataGridView.Rows[idx].Cells[7].Value = data.Attribute2;
            dataGridView.Rows[idx].Cells[8].Value = data.Attribute3;
            dataGridView.Rows[idx].Cells[9].Value = data.Attribute4;
            dataGridView.Rows[idx].Cells[10].Value = data.Attribute5;
        }

        private void buttonRetrieve_Click(object sender, EventArgs e)
        {
            CodeMasterData data = new CodeMasterData();
            string systeminfo = comboBoxSystemInfo.Text;

            codeMasterDataList = null;
            codeMasterDataList = data.getCodeMasterDataList();

            foreach (CodeMasterData dataTemp in systemValueList)
            {
                if (systeminfo == dataTemp.Value)
                {
                    systeminfo = dataTemp.Codeclass;
                    break;
                }
            }

            dataGridView.Rows.Clear();
            dataGridView.AllowUserToAddRows = true;

            foreach (CodeMasterData dataTemp in codeMasterDataList)
            {
                Boolean setFlag = true;

                if (systeminfo != GlobalConstant.ALL_TEXT)
                {
                    if (systeminfo != dataTemp.Codeclass)
                    {
                        setFlag = false;
                    }
                }

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
                systemInfoInformPanel.setComponentValues(new CodeMasterData());
                systemInfoModifyPanel.setComponentValues(new CodeMasterData());
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
            systemInfoModifyPanel.setComponentValues(new CodeMasterData()); 
            systemInfoModifyPanel.BringToFront();
            systemInfoModifyPanel.BringToFrontComponent(AddModifyFlag);
            
            setButtonEnabled(EnableType.REG);
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            AddModifyFlag = false;
            systemInfoModifyPanel.BringToFront();
            systemInfoModifyPanel.BringToFrontComponent(AddModifyFlag);

            setButtonEnabled(EnableType.REG);
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            DataGridViewSelectedRowCollection rows = dataGridView.SelectedRows;
            popUpOpen(rows[0]);
        }


        private void popUpOpen(DataGridViewRow row)
        {
            string systemcd = (string)row.Cells[0].Value;
            string codeclass = (string)row.Cells[1].Value;
            string code = (string)row.Cells[2].Value;
            String message = "시스템 코드: (" + systemcd + "), " +
                             "코드 구분: (" + codeclass + "), " +
                             "코드: (" + code + ") 를 삭제하시겠습니까?";

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
                string systemcd = (string)rows[0].Cells[0].Value;
                string codeclass = (string)rows[0].Cells[1].Value;
                string code = (string)rows[0].Cells[2].Value;

                foreach (CodeMasterData temp in systemValueList)
                {
                    if (codeclass == temp.Value)
                    {
                        codeclass = temp.Codeclass;
                    }
                }

                CodeMasterData data = new CodeMasterData();

                foreach (CodeMasterData dataTemp in codeMasterDataList)
                {
                    if (systemcd == dataTemp.Systemcd &&
                        codeclass == dataTemp.Codeclass &&
                        code == dataTemp.Code)
                    {
                        data = dataTemp;
                        break;
                    }
                }

                data.deleteCodeMasterData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(null, null);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            if (AddModifyFlag)
            {
                CodeMasterData codeMasterData = null;
                codeMasterData = systemInfoModifyPanel.getComponentValues();

                codeMasterData.insertCodeMasterData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(sender, e);
            }
            else
            {
                CodeMasterData codeMasterData = null;

                codeMasterData = systemInfoModifyPanel.getComponentValues();

                codeMasterData.updateCodeMasterData();

                setButtonEnabled(EnableType.INIT);
                buttonRetrieve_Click(sender, e);
            }

            setConditionData();
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
            CodeMasterData data = new CodeMasterData();
            
            string systemcd = (string)dataGridView.Rows[iRow].Cells[0].Value;
            string codeclass = (string)dataGridView.Rows[iRow].Cells[1].Value;
            string code = (string)dataGridView.Rows[iRow].Cells[2].Value;

            foreach (CodeMasterData temp in systemValueList)
            {
                if (codeclass == temp.Value)
                {
                    codeclass = temp.Codeclass;
                }
            }

            foreach (CodeMasterData dataTemp in codeMasterDataList)
            {
                if (systemcd  == dataTemp.Systemcd  && 
                    codeclass == dataTemp.Codeclass &&
                    code      == dataTemp.Code)
                {
                    data = dataTemp;
                }
            }

            systemInfoInformPanel.setComponentValues(new CodeMasterData());
            systemInfoModifyPanel.setComponentValues(new CodeMasterData());

            systemInfoInformPanel.setComponentValues(data);
            systemInfoModifyPanel.setComponentValues(data);

            systemInfoInformPanel.BringToFront();
            setButtonEnabled(EnableType.RETRIEVE);
        }

        private void addDetailPanel()
        {
            systemInfoInformPanel = new SystemInfoInformPanel();
            this.splitContainer1.Panel2.Controls.Add(systemInfoInformPanel);

            systemInfoInformPanel.AutoScroll = true;
            systemInfoInformPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            systemInfoInformPanel.Location = new System.Drawing.Point(0, 0);
            systemInfoInformPanel.Name = "systemInfoInformPanel";
            systemInfoInformPanel.Size = new System.Drawing.Size(994, 183);
            systemInfoInformPanel.TabIndex = 0;

            systemInfoModifyPanel = new SystemInfoModifyPanel();

            this.splitContainer1.Panel2.Controls.Add(systemInfoModifyPanel);

            systemInfoModifyPanel.AutoScroll = true;
            systemInfoModifyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            systemInfoModifyPanel.Location = new System.Drawing.Point(0, 0);
            systemInfoModifyPanel.Name = "systemInfoModifyPanel";
            systemInfoModifyPanel.Size = new System.Drawing.Size(994, 183);
            systemInfoModifyPanel.TabIndex = 0;
        }

    }
}
