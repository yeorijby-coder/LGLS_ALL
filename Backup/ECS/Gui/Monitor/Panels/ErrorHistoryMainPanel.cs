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
    public partial class ErrorHistoryMainPanel : UserControl
    {
        List<CodeMasterData> errorTypeList = null;
        List<ErrorHistData> errorHistDataList = null;

        public ErrorHistoryMainPanel()
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
            errorTypeList = null;

            CodeMasterData data = new CodeMasterData();
            errorTypeList = data.getErrorTypeValues(GlobalConstant.ALL_CHARACTER);

            //external If No setting 

            comboBoxErrortype.Items.Clear(); 
            comboBoxErrortype.Items.Add(GlobalConstant.ALL_TEXT);
            foreach (CodeMasterData dataTemp in errorTypeList)
            {
                comboBoxErrortype.Items.Add(dataTemp.Value);
            }
            if (comboBoxErrortype.Items.Count > 0)
            {
                comboBoxErrortype.SelectedIndex = 0;
            }
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //오류 발생 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //오류 유형
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //오류 코드
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //오류 내용
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //오류 유형(DB값)

                dataGridView.Columns[0].HeaderText = "오류 발생 일자";
                dataGridView.Columns[1].HeaderText = "오류 유형";
                dataGridView.Columns[2].HeaderText = "오류 코드";
                dataGridView.Columns[3].HeaderText = "오류 내용";
                dataGridView.Columns[4].HeaderText = "오류 내용(DB값)";
                
                dataGridView.Columns[0].Width = 150;
                dataGridView.Columns[1].Width = 100;
                dataGridView.Columns[2].Width = 100;
                dataGridView.Columns[3].Width = 400;
                dataGridView.Columns[4].Width = 100;

                dataGridView.Columns[4].Visible = false;

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

        delegate void DataGrivdViewDelegate(ErrorHistData dataTemp);
        private void setDataGridView(ErrorHistData data)
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

                string errortypeDisplay = "";

                foreach (CodeMasterData dataTemp in errorTypeList)
                {
                    if (dataTemp.Code == data.Errortype)
                    {
                        errortypeDisplay = dataTemp.Value;
                        break;
                    }
                }

                dataGridView.Rows[idx].Cells[0].Value = data.Createtime;
                dataGridView.Rows[idx].Cells[1].Value = errortypeDisplay;
                dataGridView.Rows[idx].Cells[2].Value = data.Errorcode;
                dataGridView.Rows[idx].Cells[3].Value = data.Errortext;
                dataGridView.Rows[idx].Cells[4].Value = data.Errortext;
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
            Thread viewThread = new Thread(new ThreadStart(LoadErrorHistoryData));
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

        private void LoadErrorHistoryData()
        {
            ErrorHistData data = new ErrorHistData();

            string errortype = GetText(comboBoxErrortype);

            foreach (CodeMasterData codeData in errorTypeList)
            {
                if (codeData.Value == errortype)
                {
                    errortype = codeData.Code;
                    break;
                }
            }

            string fromtransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerFrom.Value) + "000000001";
            string totransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerTo.Value) + "235959999";

            errorHistDataList = null;
            errorHistDataList = data.getErrorHistDataListCreatetime(fromtransferdate, totransferdate);
            
            dataGridView.Rows.Clear();
//            dataGridView.AllowUserToAddRows = true;

            foreach (ErrorHistData dataTemp in errorHistDataList)
            {
                Boolean setFlag = true;


                if (errortype != GlobalConstant.ALL_TEXT)
                {
                    if (errortype != dataTemp.Errortype)
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
