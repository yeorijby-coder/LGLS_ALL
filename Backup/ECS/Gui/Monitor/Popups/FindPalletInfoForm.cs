using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ECP.Service.Movement;
using ECP.Db.Entity;
using HECS.Gui.Monitor.Controls;
using ECP.Global;
using HECS.Util;

namespace HECS.Gui.Monitor.Popups
{
    public partial class FindPalletInfoForm : DialogBaseForm
    {
        List<HirackData> batchMaterialList = null;
        Dictionary<string, HirackData> hirackDataList = null;

        List<CodeMasterData> hirackTransferStatusList = null;
        List<CodeMasterData> hirackStatusList = null;
      
        public delegate void DelegatePalletSelected(HirackData hirackData);
        public event DelegatePalletSelected OnPalletSelected;

        private delegate void refreshButtonRetrieve();

        public FindPalletInfoForm()
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
            batchMaterialList = null;

            HirackData data = new HirackData();
            batchMaterialList = data.getHirackDataMeterialBatchList(GlobalConstant.ALL_CHARACTER, GlobalConstant.ALL_CHARACTER);

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

            //Hirack 상태 정보 조회
            CodeMasterData codeMaster = new CodeMasterData();
            hirackTransferStatusList = codeMaster.getHirackTransferStatusValues(GlobalConstant.ALL_CHARACTER);
            hirackStatusList = codeMaster.getHirackStatusValues(GlobalConstant.ALL_CHARACTER);
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Cell id
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Pallet id
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Material No
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //자재명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Batch No
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //수량
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //측정 단위
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //유효 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //입고 일자
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Hirack 반송 상태 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Hirack 상태

                dataGridView.Columns[0].HeaderText = "Cell 번호";
                dataGridView.Columns[1].HeaderText = "팔렛 번호";
                dataGridView.Columns[2].HeaderText = "자재 번호";
                dataGridView.Columns[3].HeaderText = "자재명";
                dataGridView.Columns[4].HeaderText = "배치 번호";
                dataGridView.Columns[5].HeaderText = "수량";
                dataGridView.Columns[6].HeaderText = "단위";
                dataGridView.Columns[7].HeaderText = "유효 일자";
                dataGridView.Columns[8].HeaderText = "입고 일자";
                dataGridView.Columns[9].HeaderText = "Cell 상태";
                dataGridView.Columns[10].HeaderText = "반송 상태";

                dataGridView.Columns[0].Width = 80;
                dataGridView.Columns[1].Width = 80;
                dataGridView.Columns[2].Width = 100;
                dataGridView.Columns[3].Width = 400;
                dataGridView.Columns[4].Width = 100;
                dataGridView.Columns[5].Width = 100;
                dataGridView.Columns[6].Width = 80;
                dataGridView.Columns[7].Width = 140;
                dataGridView.Columns[8].Width = 140;
                dataGridView.Columns[9].Width = 100;
                dataGridView.Columns[10].Width = 100;

                dataGridView.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridView.Columns[6].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[7].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[8].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[9].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[10].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

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

        private void setDataGridView(HirackData data)
        {
            DataGridViewRow addedRow = new DataGridViewRow();
            int idx = dataGridView.Rows.Add(addedRow);

            string hirackTransferStatusDisplay = "";
            string cellStatusDisplay = "";
            string validExpiredDateDisplay = "";
            string createTimeDisplay = "";

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

            dataGridView.Rows[idx].Cells[0].Value = data.Cellid;
            dataGridView.Rows[idx].Cells[1].Value = data.Palletid;
            dataGridView.Rows[idx].Cells[2].Value = materialno;
            dataGridView.Rows[idx].Cells[3].Value = materialdesc;
            dataGridView.Rows[idx].Cells[4].Value = data.Batchno;
            dataGridView.Rows[idx].Cells[5].Value = data.Qty;
            dataGridView.Rows[idx].Cells[6].Value = data.Unit;
            dataGridView.Rows[idx].Cells[7].Value = validExpiredDateDisplay;
            dataGridView.Rows[idx].Cells[8].Value = createTimeDisplay;
            dataGridView.Rows[idx].Cells[9].Value = hirackTransferStatusDisplay;
            dataGridView.Rows[idx].Cells[10].Value = cellStatusDisplay;
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

        public void RefreshButtonEventCall()
        {
            refreshButtonRetrieve update = new refreshButtonRetrieve(RefreshButtonEvent);
            update.Invoke();
        }

        private void RefreshButtonEvent()
        {
            buttonRetrieve_Click(null, null);
        }

        private void buttonRetrieve_Click(object sender, EventArgs e)
        {
            HirackData data = new HirackData();
            string materialno = comboBoxMaterialno.Text;
            string batchno = comboBoxBatchno.Text;

            if (string.IsNullOrEmpty(materialno))
            {
                materialno = "";
            }
            else
            {
                if (materialno == GlobalConstant.ALL_TEXT)
                {
                    materialno = "%";
                }
                else
                {
                    materialno = materialno.PadLeft(18, '0');
                }
            }

            if (string.IsNullOrEmpty(batchno))
            {
                batchno = "";
            }
            else
            {
                if (batchno == GlobalConstant.ALL_TEXT)
                {
                    batchno = "%";
                }
            }

            hirackDataList = null;
            hirackDataList = data.getHirackDataListWithBatchMaterial(materialno, batchno);

            dataGridView.Rows.Clear();
            dataGridView.AllowUserToAddRows = true;

            foreach (HirackData dataTemp in hirackDataList.Values)
            {
                Boolean setFlag = true;


                //if (materialno != GlobalConstant.ALL_TEXT)
                //{
                //    if (materialno != dataTemp.Materialno.TrimStart('0'))
                //    {
                //        setFlag = false;
                //    }
                //}

                //if (batchno != GlobalConstant.ALL_TEXT)
                //{
                //    if (batchno != dataTemp.Batchno)
                //    {
                //        setFlag = false;
                //    }
                //}

                if (dataTemp.Cellstatus != EntityEnumData.HIRACKSTATUS.Full.ToString())
                {
                    setFlag = false;
                }

                if (setFlag)
                {
                    setDataGridView(dataTemp);
                }
            }

            dataGridView.AllowUserToAddRows = false;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void buttonCheck_Click(object sender, EventArgs e)
        {
            HirackData hirackData = new HirackData();
            string cellid = "";

            if (dataGridView.RowCount < 1)
            {
                return;
            }
            else
            {
                if (dataGridView.RowCount == 1)
                {
                    cellid = (string)dataGridView.Rows[0].Cells[0].Value;
                }
                else
                {
                    DataGridViewSelectedRowCollection collection = dataGridView.SelectedRows;
                    cellid = (string)collection[0].Cells[0].Value;
                }

                hirackData = hirackDataList[cellid];

                if (hirackData == null || (string.IsNullOrEmpty(hirackData.Palletid)))
                {
                    Util.Util.ShowMesagePopup("선택한 Pallet 번호가 존재하지 않습니다.");
                    return;
                }
                else
                {
                    if (hirackData.Cellstatus != EntityEnumData.HIRACKSTATUS.Full.ToString())
                    {
                        Util.Util.ShowMesagePopup("Pallet Blocking을 할수 있는 Cell 상태가 아닙니다.");
                        return;
                    }
                }
            }

            if (OnPalletSelected != null)
            {
                OnPalletSelected(hirackData);
            }

            buttonClose_Click(sender, e);
        }
    }
}