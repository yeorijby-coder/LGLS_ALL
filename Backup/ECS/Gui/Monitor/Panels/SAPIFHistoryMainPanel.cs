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
using ECP.Util;
using System.Threading;

namespace HECS.Gui.Monitor.Panels
{
    public partial class SAPIFHistoryMainPanel : UserControl
    {
        List<CodeMasterData> externalIfTypeList = null;
        List<ExternalIfData> externalIfDataList = null;

        public SAPIFHistoryMainPanel()
        {
            InitializeComponent();
            InitComponent();
            InitializeDataGrid();
        }

        private void InitComponent()
        {
            setConditionData();
        }

        private void setConditionData()
        {
            externalIfTypeList = null;

            CodeMasterData data = new CodeMasterData();
            externalIfTypeList = data.getSAPIFTypeValues(GlobalConstant.ALL_CHARACTER);

            //external If No setting 
            comboBoxExternaliftype.Items.Clear(); 
            comboBoxExternaliftype.Items.Add(GlobalConstant.ALL_TEXT);
            foreach (CodeMasterData dataTemp in externalIfTypeList)
            {
                comboBoxExternaliftype.Items.Add(dataTemp.Value);
            }
            if (comboBoxExternaliftype.Items.Count > 0)
            {
                comboBoxExternaliftype.SelectedIndex = 0;
            }
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //SAP I/F 명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //반송 지시 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //I/F 항목
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //I/F 처리 시작 시간
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //I/F 처리 완료 시간
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //I/F 횟수
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //SAP I/F 명(DB값)

                dataGridView.Columns[0].HeaderText = "I/F 생성 일자";
                dataGridView.Columns[1].HeaderText = "SAP I/F 명";
                dataGridView.Columns[2].HeaderText = "I/F 항목";
                dataGridView.Columns[3].HeaderText = "I/F 처리 시작 시간";
                dataGridView.Columns[4].HeaderText = "I/F 처리 완료 시간";
                dataGridView.Columns[5].HeaderText = "I/F 횟수";
                dataGridView.Columns[6].HeaderText = "SAP I/F 명(DB값)";

                dataGridView.Columns[6].Visible = false;

                dataGridView.Columns[0].Width = 150;
                dataGridView.Columns[1].Width = 300;
                dataGridView.Columns[2].Width = 500;
                dataGridView.Columns[3].Width = 150;
                dataGridView.Columns[4].Width = 150;
                dataGridView.Columns[5].Width = 100;
                dataGridView.Columns[6].Width = 100;

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

        delegate void DataGrivdViewDelegate(ExternalIfData dataTemp);
        private void setDataGridView(ExternalIfData data)
        {
            if (dataGridView.InvokeRequired)
            {
                DataGrivdViewDelegate tempDelegate = new DataGrivdViewDelegate(setDataGridView);
                this.Invoke(tempDelegate, new object[] { data });
            }
            else
            {
                DataGridViewRow addedRow = new DataGridViewRow();
                int idx = dataGridView.Rows.Add(addedRow);

                string externalIfTypeDisplay = "";

                foreach (CodeMasterData dataTemp in externalIfTypeList)
                {
                    if (dataTemp.Code == data.Externaliftype)
                    {
                        externalIfTypeDisplay = dataTemp.Value;
                        break;
                    }
                }

                dataGridView.Rows[idx].Cells[0].Value = data.Createtime;
                dataGridView.Rows[idx].Cells[1].Value = externalIfTypeDisplay;
                dataGridView.Rows[idx].Cells[2].Value = data.Ifvalue;
                dataGridView.Rows[idx].Cells[3].Value = data.Transstarttime;
                dataGridView.Rows[idx].Cells[4].Value = data.Transcompletetime;
                dataGridView.Rows[idx].Cells[5].Value = data.Ifcnt;
                dataGridView.Rows[idx].Cells[6].Value = data.Externaliftype;
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.ParentForm.WindowState = FormWindowState.Minimized;
        }

        private void buttonRetrieve_Click(object sender, EventArgs e)
        {
            ShowProgressBar(true);
            dataGridView.Rows.Clear();
            Thread viewThread = new Thread(new ThreadStart(LoadSAPIFHistoryData));
            viewThread.Start();
        }

        delegate string GetTextCallback(ComboBox objectCombo);
        private string GetText(ComboBox objectCombo)
        {
            string textValue = "";
            if (objectCombo.InvokeRequired)
            {
                GetTextCallback getText = new GetTextCallback(GetText);
                return (string)this.Invoke(getText, new object[] { objectCombo });
            }
            else
            {
                textValue = objectCombo.Text;
            }

            return textValue;
        }

        delegate void ProgressViewDelegate(bool value);
        private void ShowProgressBar(bool value)
        {
            if (panelProgress.InvokeRequired)
            {
                ProgressViewDelegate tempDelegate = new ProgressViewDelegate(ShowProgressBar);
                this.Invoke(tempDelegate, new object[] { value });
            }
            else
            {
                if (value == true)
                {
                    panelProgress.Visible = true;
                    buttonRetrieve.Visible = false;
                    buttonExcel.Visible = false;
                }
                else
                {
                    panelProgress.Visible = false;
                    buttonRetrieve.Visible = true;
                    buttonExcel.Visible = true;
                }
            }

        }

        private void LoadSAPIFHistoryData()
        {
            ExternalIfData data = new ExternalIfData();

            string externalittype = GetText(comboBoxExternaliftype);

            foreach (CodeMasterData codeData in externalIfTypeList)
            {
                if (codeData.Value == externalittype)
                {
                    externalittype = codeData.Code;
                    break;
                }
            }

            string fromtransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerFrom.Value) + "000000001";
            string totransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerTo.Value) + "235959999";

            externalIfDataList = null;
            externalIfDataList = data.getExternalIfDataTransferList(fromtransferdate, totransferdate);
            
            dataGridView.Rows.Clear();
//            dataGridView.AllowUserToAddRows = true;

            foreach (ExternalIfData dataTemp in externalIfDataList)
            {
                Boolean setFlag = true;


                if (externalittype != GlobalConstant.ALL_TEXT)
                {
                    if (externalittype != dataTemp.Externaliftype)
                    {
                        setFlag = false;
                    }
                }

                if (setFlag)
                {
                    setDataGridView(dataTemp);
                }
            }

//            dataGridView.AllowUserToAddRows = false;
            ShowProgressBar(false);
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


    }
}
