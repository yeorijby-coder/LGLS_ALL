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
using ECP.BizRule;
using ECP.Util;

namespace HECS.Gui.Monitor.Popups
{
    public partial class TransferDetailForm : DialogBaseForm
    {
        private string datestring = "";

        public string Datestring
        {
            get { return datestring; }
            set { datestring = value; }
        }
        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }
        private string transferstatus = "";

        public string Transferstatus
        {
            get { return transferstatus; }
            set { transferstatus = value; }
        }

        List<TransferDetail> transferDetailData = null;

        private delegate void refreshButtonRetrieve();

        public TransferDetailForm()
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
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //요청번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //요청시간
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //ECS 작업번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //자재번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //요청수량
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //ECS수량
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //단위
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //팔렛번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //시작위치
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //도착위치
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //시작 Cell
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //도착 Cell
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //작업상태

                dataGridView.Columns[0].HeaderText = "요청번호";
                dataGridView.Columns[1].HeaderText = "요청시간";
                dataGridView.Columns[2].HeaderText = "ECS 작업번호";
                dataGridView.Columns[3].HeaderText = "자재번호";
                dataGridView.Columns[4].HeaderText = "요청수량";
                dataGridView.Columns[5].HeaderText = "ECS수량";
                dataGridView.Columns[6].HeaderText = "단위";
                dataGridView.Columns[7].HeaderText = "작업번호";
                dataGridView.Columns[8].HeaderText = "시작위치";
                dataGridView.Columns[9].HeaderText = "도착위치";
                dataGridView.Columns[10].HeaderText = "시작 Cell";
                dataGridView.Columns[11].HeaderText = "도착 Cell";
                dataGridView.Columns[12].HeaderText = "작업상태";


                dataGridView.Columns[0].Width = 100;
                dataGridView.Columns[1].Width = 80;
                dataGridView.Columns[2].Width = 120;
                dataGridView.Columns[3].Width = 80;
                dataGridView.Columns[4].Width = 100;
                dataGridView.Columns[5].Width = 100;
                dataGridView.Columns[6].Width = 60;
                dataGridView.Columns[7].Width = 80;
                dataGridView.Columns[8].Width = 80;
                dataGridView.Columns[9].Width = 80;
                dataGridView.Columns[10].Width = 80;
                dataGridView.Columns[11].Width = 80;
                dataGridView.Columns[12].Width = 100;

                dataGridView.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridView.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridView.Columns[6].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[7].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[8].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[9].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[10].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[11].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[12].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

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

        private void setDataGridView(TransferDetail data)
        {
            DataGridViewRow addedRow = new DataGridViewRow();
            int idx = dataGridView.Rows.Add(addedRow);

            string createTimeDisplay = "";

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

            UDPropertyData udpropertyData = new UDPropertyData();
            udpropertyData.getUdpropertyValue(GlobalConstant.PLANT_CODE,
                                              data.Movementtype,
                                              GlobalConstant.SUMMER_TIME_CODE);


            DateTime createtime = DateTime.Parse(createTimeDisplay);
            TimeSpan summerTime = TimeSpan.FromHours(int.Parse(udpropertyData.Value));
            DateTime koreacTime = createtime.Add(summerTime);

            createTimeDisplay = string.Format("{0:HH:mm:ss}", koreacTime);

            string fromsublocationidDisplay = "";
            string tosublocationidDisplay = "";

            if (data.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
            {
                fromsublocationidDisplay = "";
                tosublocationidDisplay = data.Tosublocationid;

                tosublocationidDisplay = tosublocationidDisplay.Substring(0, 2) + "-" +
                                         tosublocationidDisplay.Substring(2, 2) + "-" +
                                         tosublocationidDisplay.Substring(4, 2);
            }
            else if (data.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
            {
                fromsublocationidDisplay = data.Fromsublocationid;

                fromsublocationidDisplay = fromsublocationidDisplay.Substring(0, 2) + "-" +
                                         fromsublocationidDisplay.Substring(2, 2) + "-" +
                                         fromsublocationidDisplay.Substring(4, 2);
                tosublocationidDisplay = "";
            }
            else
            {
                fromsublocationidDisplay = "";
                tosublocationidDisplay = "";
            }

            dataGridView.Rows[idx].Cells[0].Value = data.Reqno;
            dataGridView.Rows[idx].Cells[1].Value = createTimeDisplay;
            dataGridView.Rows[idx].Cells[2].Value = data.Commandid;
            dataGridView.Rows[idx].Cells[3].Value = data.Materialno;
            dataGridView.Rows[idx].Cells[4].Value = data.Sapqty;
            dataGridView.Rows[idx].Cells[5].Value = data.Qty;
            dataGridView.Rows[idx].Cells[6].Value = data.Unit;
            dataGridView.Rows[idx].Cells[7].Value = data.Palletid;
            dataGridView.Rows[idx].Cells[8].Value = data.Fromposition;
            dataGridView.Rows[idx].Cells[9].Value = data.Toposition;
            dataGridView.Rows[idx].Cells[10].Value = fromsublocationidDisplay;
            dataGridView.Rows[idx].Cells[11].Value = tosublocationidDisplay;
            dataGridView.Rows[idx].Cells[12].Value = data.Transferstatus;
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
            TransferDetail data = new TransferDetail();

            transferDetailData = null;
            transferDetailData = data.getTransferDetailData(this.Datestring, this.Fromposition, this.Toposition, this.Transferstatus);

            dataGridView.Rows.Clear();
            dataGridView.AllowUserToAddRows = true;

            foreach (TransferDetail dataTemp in transferDetailData)
            {
                Boolean setFlag = true;

                if (setFlag)
                {
                    setDataGridView(dataTemp);
                }
            }

            dataGridView.AllowUserToAddRows = false;
        }

        private void buttonXPExcel_Click(object sender, EventArgs e)
        {
            ExcelExport exportFile = new ExcelExport();

            if (dataGridView.RowCount > 0)
            {
                //Boolean bFlag = exportFile.saveCSVFile(dataGridView, this.GetType().Name);
                Boolean bFlag = exportFile.saveExcelFile(dataGridView, this.GetType().Name, true);


                if (bFlag)
                {
                    Util.Util.ShowMesagePopup("파일이 저장되었습니다.");
                }
                else
                {
                    Util.Util.ShowMesagePopup("파일 저장에 실패했습니다.");
                }
            }
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }
    }
}