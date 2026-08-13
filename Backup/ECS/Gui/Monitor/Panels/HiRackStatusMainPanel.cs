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
using HECS.Gui.Monitor.Panels.HiRackStatus;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Util;
using System.Threading;

namespace HECS.Gui.Monitor.Panels
{
    public partial class HiRackStatusMainPanel : UserControl
    {
        HiRackStatusInformPanel hirackStatusInformPanel = null;
        HiRackStatusModifyPanel hirackStatusModifyPanel = null;
        List<HirackData> batchMaterialList = null;
        List<HirackData> cellList = null;

        List<CodeMasterData> hirackTransferStatusList = null;
        List<CodeMasterData> hirackStatusList = null;
        List<CodeMasterData> fromPositionList = null;
        List<CodeMasterData> companyTypeList = null;
        
        Dictionary<string, string> bayList = null;
        Dictionary<string, string> levelList = null;
        Dictionary<string, HirackData> hirackDataList = null;

        public HiRackStatusMainPanel()
        {
            InitializeComponent();
            InitComponent();
            InitializeDataGrid();
        }

        private enum EnableType
        {
            INIT = 1,
            CANCEL = 2,
            RETRIEVE = 3
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
            batchMaterialList = null;
            cellList = null;

            HirackData data = new HirackData();
            batchMaterialList = data.getHirackDataMeterialBatchList(GlobalConstant.ALL_CHARACTER, GlobalConstant.ALL_CHARACTER);
            cellList = data.getHirackDataCellList(GlobalConstant.ALL_CHARACTER, GlobalConstant.ALL_CHARACTER, GlobalConstant.ALL_CHARACTER);

            //Material No setting 
            comboBoxMaterialno.Items.Clear(); 
            comboBoxMaterialno.Items.Add(GlobalConstant.ALL_TEXT);
            string materialTemp = "";
            foreach (HirackData dataTemp in batchMaterialList)
            {
                if (!string.IsNullOrEmpty(dataTemp.Materialno))
                {
                    if (materialTemp != dataTemp.Materialno.TrimStart('0'))
                    {
                        comboBoxMaterialno.Items.Add(dataTemp.Materialno.TrimStart('0'));
                        materialTemp = dataTemp.Materialno.TrimStart('0');
                    }
                }
            }

            if (comboBoxMaterialno.Items.Count > 0)
            {
                comboBoxMaterialno.SelectedIndex = 0;
            }

            //Bank No No setting 
            comboBoxBank.Items.Clear(); 
            comboBoxBank.Items.Add(GlobalConstant.ALL_TEXT);
            string bankTemp = "";
            foreach (HirackData dataTemp in cellList)
            {
                if (bankTemp != dataTemp.Cellid.Substring(0, 2))
                {
                    comboBoxBank.Items.Add(dataTemp.Cellid.Substring(0, 2));
                    bankTemp = dataTemp.Cellid.Substring(0, 2);
                }
            }
            if (comboBoxBank.Items.Count > 0)
            {
                comboBoxBank.SelectedIndex = 0;
            }

            //Bay No No setting 
            comboBoxBay.Items.Clear(); 
            comboBoxBay.Items.Add(GlobalConstant.ALL_TEXT);
            bayList = new Dictionary<string, string>();
            foreach (HirackData dataTemp in cellList)
            {
                if (!bayList.ContainsKey(dataTemp.Cellid.Substring(2, 2)))
                {
                    comboBoxBay.Items.Add(dataTemp.Cellid.Substring(2, 2));
                    bayList.Add(dataTemp.Cellid.Substring(2, 2), dataTemp.Cellid.Substring(2, 2));
                }
            }
            if (comboBoxBay.Items.Count > 0)
            {
                comboBoxBay.SelectedIndex = 0;
            }

            //Level No No setting 
            comboBoxLevel.Items.Clear(); 
            comboBoxLevel.Items.Add(GlobalConstant.ALL_TEXT);
            levelList = new Dictionary<string, string>();
            foreach (HirackData dataTemp in cellList)
            {
                if (!levelList.ContainsKey(dataTemp.Cellid.Substring(4, 2)))
                {
                    comboBoxLevel.Items.Add(dataTemp.Cellid.Substring(4, 2));
                    levelList.Add(dataTemp.Cellid.Substring(4, 2), dataTemp.Cellid.Substring(4, 2));
                }
            }
            if (comboBoxLevel.Items.Count > 0)
            {
                comboBoxLevel.SelectedIndex = 0;
            }

            //Hirack 상태 정보 조회
            CodeMasterData codeMaster = new CodeMasterData();
            hirackTransferStatusList = codeMaster.getHirackTransferStatusValues(GlobalConstant.ALL_CHARACTER);
            hirackStatusList = codeMaster.getHirackStatusValues(GlobalConstant.ALL_CHARACTER);
            fromPositionList = codeMaster.getPositionValues(GlobalConstant.FROM_TEXT);
            companyTypeList = codeMaster.getCompanyTypeValues();

            //회사 유형 setting
            comboBoxCompanytype.Items.Clear();
            comboBoxCompanytype.Items.Add(GlobalConstant.ALL_TEXT);
            foreach (CodeMasterData dataTemp in companyTypeList)
            {
                comboBoxCompanytype.Items.Add(dataTemp.Value);
            }

            if (comboBoxCompanytype.Items.Count > 0)
            {
                comboBoxCompanytype.SelectedIndex = 0;
            }
        }

        delegate void setButtonEnabledDelegate(EnableType enableTypeTemp);
        private void setButtonEnabled(EnableType enableType)
        {
            if (this.InvokeRequired)
            {
                setButtonEnabledDelegate tempDelegate = new setButtonEnabledDelegate(setButtonEnabled);
                this.Invoke(tempDelegate, new object[] { enableType });
            }
            else
            {
                if (enableType == EnableType.INIT)
                {
                    buttonModify.Enabled = false;
                    buttonSave.Enabled = false;
                }
                else if (enableType == EnableType.CANCEL)
                {
                    buttonModify.Enabled = false;
                    buttonSave.Enabled = true;
                }
                else if (enableType == EnableType.RETRIEVE)
                {
                    if (dataGridView.RowCount > 0)
                    {
                        buttonModify.Enabled = true;
                        buttonSave.Enabled = false;
                    }
                    else
                    {
                        buttonModify.Enabled = false;
                        buttonSave.Enabled = false;
                    }

                }
                else
                {
                    buttonModify.Enabled = false;
                    buttonSave.Enabled = false;
                }
            }
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //공장코드 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Cell id
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Pallet id
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //반송 상태 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //상태
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //자재번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //자재명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //배치번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //수량
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //단위
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //유효일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //입고일자

                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Hirack 반송 상태(DB 값)
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Hirack 상태(DB 값) 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //유효 일자(DB 값)
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //입고 일자(DB 값)
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Cellid(DB 값)


                dataGridView.Columns[0].HeaderText = "공장 코드";
                dataGridView.Columns[1].HeaderText = "Cell 번호";
                dataGridView.Columns[2].HeaderText = "팔렛 번호";
                dataGridView.Columns[3].HeaderText = "반송 상태";
                dataGridView.Columns[4].HeaderText = "Cell 상태";
                dataGridView.Columns[5].HeaderText = "자재 번호";
                dataGridView.Columns[6].HeaderText = "자재명";
                dataGridView.Columns[7].HeaderText = "배치 번호";
                dataGridView.Columns[8].HeaderText = "수량";
                dataGridView.Columns[9].HeaderText = "단위";
                dataGridView.Columns[10].HeaderText = "유효 일자";
                dataGridView.Columns[11].HeaderText = "입고 일자";

                dataGridView.Columns[0].Name = "Plantcode";
                dataGridView.Columns[1].Name = "CellidDisplay";
                dataGridView.Columns[2].Name = "Palletid";
                dataGridView.Columns[3].Name = "HiracktransferstatusDisplay";
                dataGridView.Columns[4].Name = "CellstatusDisplay";
                dataGridView.Columns[5].Name = "Materialno";
                dataGridView.Columns[6].Name = "Materialdesc";
                dataGridView.Columns[7].Name = "Batchno";
                dataGridView.Columns[8].Name = "Qty";
                dataGridView.Columns[9].Name = "Unit";
                dataGridView.Columns[10].Name = "ValidexpireddateDisplay";
                dataGridView.Columns[11].Name = "CreatetimeDisplay";

                dataGridView.Columns[12].Name = "Hiracktransferstatus";
                dataGridView.Columns[13].Name = "Cellstatus";
                dataGridView.Columns[14].Name = "Validexpireddate";
                dataGridView.Columns[15].Name = "Createtime";
                dataGridView.Columns[16].Name = "Cellid";

                dataGridView.Columns[12].Visible = false;
                dataGridView.Columns[13].Visible = false;
                dataGridView.Columns[14].Visible = false;
                dataGridView.Columns[15].Visible = false;
                dataGridView.Columns[16].Visible = false;


                dataGridView.Columns[0].Width = 80;
                dataGridView.Columns[1].Width = 100;
                dataGridView.Columns[2].Width = 80;
                dataGridView.Columns[3].Width = 100;
                dataGridView.Columns[4].Width = 100;
                dataGridView.Columns[5].Width = 100;
                dataGridView.Columns[6].Width = 400;
                dataGridView.Columns[7].Width = 100;
                dataGridView.Columns[8].Width = 100;
                dataGridView.Columns[9].Width = 50;
                dataGridView.Columns[10].Width = 100;
                dataGridView.Columns[11].Width = 100;

                dataGridView.Columns[12].Width = 200;
                dataGridView.Columns[13].Width = 200;
                dataGridView.Columns[14].Width = 200;
                dataGridView.Columns[15].Width = 200;
                dataGridView.Columns[16].Width = 100;

                dataGridView.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[6].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[7].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[8].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridView.Columns[9].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[10].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[11].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

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

        delegate void DataGrivdViewDelegate(HirackData dataTemp);
        private void setDataGridView(HirackData data)
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

                string hirackTransferStatusDisplay = "";
                string cellStatusDisplay = "";
                string validExpiredDateDisplay = "";
                string createTimeDisplay = "";

                string cellid = data.Cellid;
                if (!string.IsNullOrEmpty(cellid))
                {
                    cellid = cellid.Substring(0, 2) + "-" +
                             cellid.Substring(2, 2) + "-" +
                             cellid.Substring(4, 2);
                }

                foreach (CodeMasterData dataTemp in hirackTransferStatusList)
                {
                    if (data.Hiracktransferstatus == dataTemp.Code)
                    {
                        hirackTransferStatusDisplay = dataTemp.Value;
                        break;
                    }
                }

                foreach (CodeMasterData dataTemp in hirackStatusList)
                {
                    if (data.Cellstatus == dataTemp.Code)
                    {
                        cellStatusDisplay = dataTemp.Value;
                        break;
                    }
                }

                if (!string.IsNullOrEmpty(data.Validexpireddate))
                {
                    validExpiredDateDisplay = data.Validexpireddate;
                    validExpiredDateDisplay = validExpiredDateDisplay.Substring(0, 4) + "." +
                                              validExpiredDateDisplay.Substring(4, 2) + "." +
                                              validExpiredDateDisplay.Substring(6, 2);
                }

                if (!string.IsNullOrEmpty(data.Createtime))
                {
                    createTimeDisplay = data.Createtime.Substring(0, 14);
                    createTimeDisplay = createTimeDisplay.Substring(0, 4) + "." +
                                        createTimeDisplay.Substring(4, 2) + "." +
                                        createTimeDisplay.Substring(6, 2) + " " +
                                        createTimeDisplay.Substring(8, 2) + ":" +
                                        createTimeDisplay.Substring(10, 2) + ":" +
                                        createTimeDisplay.Substring(12, 2);
                }

                string materialno = data.Materialno;
                string materialdesc = "";
                if (!(string.IsNullOrEmpty(materialno)))
                {
                    MaterialData mdata = new MaterialData();
                    mdata.getMaterialData(materialno);

                    materialdesc = mdata.Materialname;
                    materialno = materialno.TrimStart('0');
                }

                dataGridView.Rows[idx].Cells[0].Value = data.Plantcode;
                dataGridView.Rows[idx].Cells[1].Value = cellid;
                dataGridView.Rows[idx].Cells[2].Value = data.Palletid;
                dataGridView.Rows[idx].Cells[3].Value = hirackTransferStatusDisplay;
                dataGridView.Rows[idx].Cells[4].Value = cellStatusDisplay;
                dataGridView.Rows[idx].Cells[5].Value = materialno;
                dataGridView.Rows[idx].Cells[6].Value = materialdesc;
                dataGridView.Rows[idx].Cells[7].Value = data.Batchno;
                dataGridView.Rows[idx].Cells[8].Value = data.Qty;
                dataGridView.Rows[idx].Cells[9].Value = data.Unit;
                dataGridView.Rows[idx].Cells[10].Value = validExpiredDateDisplay;
                dataGridView.Rows[idx].Cells[11].Value = createTimeDisplay;

                dataGridView.Rows[idx].Cells[12].Value = data.Hiracktransferstatus;
                dataGridView.Rows[idx].Cells[13].Value = data.Cellstatus;
                dataGridView.Rows[idx].Cells[14].Value = data.Validexpireddate;
                dataGridView.Rows[idx].Cells[15].Value = data.Createtime;
                dataGridView.Rows[idx].Cells[16].Value = data.Cellid;
            }
        }

        private void buttonXPRetrieve_Click(object sender, EventArgs e)
        {
            ShowProgressBar(true);
            dataGridView.Rows.Clear();
            Thread viewThread = new Thread(new ThreadStart(LoadHirackData));
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
                    buttonXPRetrieve.Visible = false;
                    buttonXPExcel.Visible = false;
                }
                else
                {
                    panelProgress.Visible = false;
                    buttonXPRetrieve.Visible = true;
                    buttonXPExcel.Visible = true;
                }
            }

        }

        private void LoadHirackData()
        {
            HirackData data = new HirackData();
            string materialno = GetText(comboBoxMaterialno);
            string batchno = GetText(comboBoxBatchno);
            string bankno = GetText(comboBoxBank);
            string bayno = GetText(comboBoxBay);
            string levelno = GetText(comboBoxLevel);
            string palletid = textBoxPalletid.Text;
            string companyType = GetText(comboBoxCompanytype);

            foreach (CodeMasterData codeData in companyTypeList)
            {
                if (codeData.Value == companyType)
                {
                    companyType = codeData.Code;
                    break;
                }
            }

            hirackDataList = null;
            hirackDataList = data.getHirackDataList();

            dataGridView.Rows.Clear();
 //           dataGridView.AllowUserToAddRows = true;

            foreach (HirackData dataTemp in hirackDataList.Values)
            {
                Boolean setFlag = true;


                if (materialno != GlobalConstant.ALL_TEXT)
                {
                    if (materialno != dataTemp.Materialno.TrimStart('0'))
                    {
                        setFlag = false;
                    }
                }

                if (batchno != GlobalConstant.ALL_TEXT)
                {
                    if (batchno != dataTemp.Batchno)
                    {
                        setFlag = false;
                    }
                }

                if (bankno != GlobalConstant.ALL_TEXT)
                {
                    if (bankno != dataTemp.Cellid.Substring(0,2))
                    {
                        setFlag = false;
                    }
                }

                if (bayno != GlobalConstant.ALL_TEXT)
                {
                    if (bayno != dataTemp.Cellid.Substring(2, 2))
                    {
                        setFlag = false;
                    }
                }

                if (levelno != GlobalConstant.ALL_TEXT)
                {
                    if (levelno != dataTemp.Cellid.Substring(4, 2))
                    {
                        setFlag = false;
                    }
                }

                if (checkBoxBlockingFlag.Checked)
                {
                    if (dataTemp.Cellstatus != EntityEnumData.HIRACKSTATUS.Hold.ToString())
                    {
                        setFlag = false;
                    }
                }

                if (checkBoxEmpty.Checked == false)
                {
                    if (dataTemp.Cellstatus == EntityEnumData.HIRACKSTATUS.Empty.ToString())
                    {
                        setFlag = false;
                    }
                }

                if (companyType != GlobalConstant.ALL_TEXT)
                {
                    if (companyType != dataTemp.Fromplant)
                    {
                        setFlag = false;
                    }
                }

                if (!string.IsNullOrEmpty(palletid))
                {
                    if (palletid != dataTemp.Palletid)
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

            if (dataGridView.Rows.Count > 0)
            {
                displayDetailHirackData(0);
            }
            else
            {
                hirackStatusInformPanel.setComponentValues(new HirackData());
                hirackStatusModifyPanel.setComponentValues(new HirackData());
            }
            setButtonEnabled(EnableType.RETRIEVE);
            ShowProgressBar(false);
        }

        private void buttonXPExcel_Click(object sender, EventArgs e)
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

        private void buttonXPClose_Click(object sender, EventArgs e)
        {
            this.ParentForm.WindowState = FormWindowState.Minimized;
        }

        private void dataGridView_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex < 0)
            {
                return;
            }

            displayDetailHirackData(e.RowIndex);
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            hirackStatusModifyPanel.BringToFront();
            setButtonEnabled(EnableType.CANCEL);
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            DBConnection theDB = null;
            
            HirackData data = new HirackData();
            data = hirackStatusModifyPanel.getComponentValues();

            try
            {
                theDB = DBControl.GetConnection();
                theDB.Start();

                data.updateHirackData(theDB, true, true);

                theDB.Commit();
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            setButtonEnabled(EnableType.INIT);
            buttonXPRetrieve_Click(sender, e);
            setConditionData();
        }

        private void comboBoxMaterialno_SelectedIndexChanged(object sender, EventArgs e)
        {
            string materialno = comboBoxMaterialno.Text;

            comboBoxBatchno.Items.Clear();
            comboBoxBatchno.Text = "";

            //Batch No setting 
            comboBoxBatchno.Items.Add(GlobalConstant.ALL_TEXT);
            string batchTemp = "";
            foreach (HirackData dataTemp in batchMaterialList)
            {
                if (!string.IsNullOrEmpty(dataTemp.Batchno))
                {
                    if (materialno == dataTemp.Materialno.TrimStart('0'))
                    {
                        if (batchTemp != dataTemp.Batchno)
                        {
                            comboBoxBatchno.Items.Add(dataTemp.Batchno);
                            batchTemp = dataTemp.Batchno;
                        }
                    }
                    else if (materialno == GlobalConstant.ALL_TEXT)
                    {
                        if (batchTemp != dataTemp.Batchno)
                        {
                            comboBoxBatchno.Items.Add(dataTemp.Batchno);
                            batchTemp = dataTemp.Batchno;
                        }
                    }
                }
            }
            if (comboBoxBatchno.Items.Count > 0)
            {
                comboBoxBatchno.SelectedIndex = 0;
            }
        }

        delegate void DisplayDetailHirackDataDelegate(int iRowTemp);
        private void displayDetailHirackData(int iRow)
        {
            if (this.InvokeRequired)
            {
                DisplayDetailHirackDataDelegate tempDelegate = new DisplayDetailHirackDataDelegate(displayDetailHirackData);
                this.Invoke(tempDelegate, new object[] { iRow });
            }
            else
            {
                string cellid = (string)dataGridView.Rows[iRow].Cells["Cellid"].Value;

                HirackData data = new HirackData();

                data = hirackDataList[cellid];

                hirackStatusInformPanel.setComponentValues(new HirackData());
                hirackStatusModifyPanel.setComponentValues(new HirackData());

                hirackStatusInformPanel.setComponentValues(data);
                hirackStatusModifyPanel.setComponentValues(data);

                hirackStatusInformPanel.BringToFront();
                setButtonEnabled(EnableType.RETRIEVE);
            }
        }

        private void addDetailPanel()
        {
            hirackStatusInformPanel = new HiRackStatusInformPanel();

            this.splitContainer1.Panel2.Controls.Add(hirackStatusInformPanel);

            hirackStatusInformPanel.AutoScroll = true;
            hirackStatusInformPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            hirackStatusInformPanel.Location = new System.Drawing.Point(0, 0);
            hirackStatusInformPanel.Name = "hirackStatusInformPanel";
            hirackStatusInformPanel.Size = new System.Drawing.Size(994, 183);
            hirackStatusInformPanel.TabIndex = 0;

            hirackStatusModifyPanel = new HiRackStatusModifyPanel();

            this.splitContainer1.Panel2.Controls.Add(hirackStatusModifyPanel);

            hirackStatusModifyPanel.AutoScroll = true;
            hirackStatusModifyPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            hirackStatusModifyPanel.Location = new System.Drawing.Point(0, 0);
            hirackStatusModifyPanel.Name = "hirackStatusModifyPanel";
            hirackStatusModifyPanel.Size = new System.Drawing.Size(994, 183);
            hirackStatusModifyPanel.TabIndex = 0;
        }
    }
}
