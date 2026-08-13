using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Service.Movement;
using ECP.Util.Server;
using ECP.Db.Entity;
using HECS.Device.Dispatcher;
using HECS.Gui.Monitor.Popups;
using HECS.Util;

namespace HECS.Gui.Monitor.Panels.Monitoring
{
    public partial class MonitorMainTransferListPanel : UserControl
    {
        Timer refreshTimer = new Timer();
        private int tickCount = 0;
        public MonitorMainTransferListPanel()
        {
            InitializeComponent();
            InitializeDataGridTransferList();
            InitializeDataGridTransferDetail();
            refreshTimer.Tick += new EventHandler(RefreshWorkList);
            refreshTimer.Interval = 2000;
            refreshTimer.Enabled = true;

        }

        private void InitializeDataGridTransferDetail()
        {
            try
            {
                dataGridViewTransferDetail.Columns.Add(new DataGridViewImageColumn());        //아이콘. 
                dataGridViewTransferDetail.Columns.Add(new DataGridViewTextBoxColumn());      //CommandID
                dataGridViewTransferDetail.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferDetail.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferDetail.Columns.Add(new DataGridViewTextBoxColumn());      //
 
                dataGridViewTransferDetail.Columns[0].HeaderText = "";
                dataGridViewTransferDetail.Columns[1].HeaderText = "SEQ";
                dataGridViewTransferDetail.Columns[2].HeaderText = "디바이스";
                dataGridViewTransferDetail.Columns[3].HeaderText = "시작";
                dataGridViewTransferDetail.Columns[4].HeaderText = "도착";




                dataGridViewTransferDetail.Columns[0].Width = 24;
                dataGridViewTransferDetail.Columns[0].Resizable = DataGridViewTriState.False;
                dataGridViewTransferDetail.Columns[1].Width = 100;
                dataGridViewTransferDetail.Columns[2].Width = 150;
                dataGridViewTransferDetail.Columns[3].Width = 150;
                dataGridViewTransferDetail.Columns[4].Width = 150;

                dataGridViewTransferDetail.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridViewTransferDetail.MultiSelect = false;
                dataGridViewTransferDetail.AllowUserToResizeRows = false;

                dataGridViewTransferDetail.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }
        private void InitializeDataGridTransferList()
        {
            try
            {
                dataGridViewTransferList.Columns.Add(new DataGridViewImageColumn());        //아이콘. 
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //CommandID
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                

                dataGridViewTransferList.Columns[0].HeaderText = "";
                dataGridViewTransferList.Columns[1].HeaderText = "ECS번호";
                dataGridViewTransferList.Columns[2].HeaderText = "구분";
                dataGridViewTransferList.Columns[3].HeaderText = "순위";
                dataGridViewTransferList.Columns[4].HeaderText = "자재코드";
                dataGridViewTransferList.Columns[5].HeaderText = "수량";
                dataGridViewTransferList.Columns[6].HeaderText = "작업번호";
                dataGridViewTransferList.Columns[7].HeaderText = "출발위치";
                dataGridViewTransferList.Columns[8].HeaderText = "도착지";

                dataGridViewTransferList.Columns[0].Width = 24;
                dataGridViewTransferList.Columns[0].Resizable = DataGridViewTriState.False;
                dataGridViewTransferList.Columns[1].Width = 100;
                dataGridViewTransferList.Columns[2].Width = 45;
                dataGridViewTransferList.Columns[3].Width = 45;
                dataGridViewTransferList.Columns[4].Width = 80;
                dataGridViewTransferList.Columns[5].Width = 60;
                dataGridViewTransferList.Columns[6].Width = 45;
                dataGridViewTransferList.Columns[7].Width = 140;
                dataGridViewTransferList.Columns[8].Width = 140;

                dataGridViewTransferList.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridViewTransferList.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;

                dataGridViewTransferList.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridViewTransferList.MultiSelect = false;
                dataGridViewTransferList.AllowUserToResizeRows = false;

                dataGridViewTransferList.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private void RefreshWorkList(object sender, EventArgs e)
        {
            tickCount = tickCount + 1;
            ECSDispatcher.DoWork();
            if (tickCount < 5)
            {
                if (dataGridViewTransferList.Rows.Count == 0)
                {
                    this.dataGridViewTransferList.Rows.Clear();
                    this.dataGridViewTransferDetail.Rows.Clear();

                    return;
                }
                if (dataGridViewTransferList.SelectedRows.Count > 0)
                {
                    ViewTransferDetail((string)dataGridViewTransferList.SelectedRows[0].Cells[1].Value);
                }
                else
                {
                    this.dataGridViewTransferDetail.Rows.Clear();
                }
                return;
            }
            tickCount = 0;
            //MovementManager manager = Naming.GetMovementManager();
            Dictionary<string, TransferData> jobs = MovementManager.getTransferDataList();
            try
            {
                Dictionary<string, int> updatedList = new Dictionary<string, int>(100);
                foreach (TransferData job in jobs.Values)
                {
                    DataGridViewRow rowForJob = GetDataGridRow(job.Commandid);
                    if (rowForJob == null)  //Job 이 없는거.
                    {
                        int addedRowIndex = dataGridViewTransferList.Rows.Add();
                        rowForJob = dataGridViewTransferList.Rows[addedRowIndex];
                        updatedList.Add(addedRowIndex.ToString(), addedRowIndex);
                        UpdateJob(rowForJob, job);

                    }
                    else
                    {
                        updatedList.Add(rowForJob.Index.ToString(), rowForJob.Index);
                        UpdateJob(rowForJob, job);
                    }
                }

                if (dataGridViewTransferList.Rows.Count >= updatedList.Count)
                {
                    RemoveJob(updatedList);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(ex.StackTrace);
            }


        }

        private void RemoveJob(Dictionary<string, int> updatedList)
        {
            try
            {
                List<DataGridViewRow> removeList = new List<DataGridViewRow>(50);
                foreach (DataGridViewRow gridRow in dataGridViewTransferList.Rows)
                {
                    if (gridRow.Index >= 0 && !updatedList.ContainsKey(gridRow.Index.ToString()))
                    {
                        removeList.Add(gridRow);
                    }
                }
                for (int i = 0; i < removeList.Count; i++)
                {
                    dataGridViewTransferList.Rows.Remove(removeList[i]);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private void UpdateJob(DataGridViewRow displayJob, TransferData job)
        {
            try
            {
                if (job == null || displayJob == null)
                {
                    return;
                }

                if (EntityEnumData.TRANSFERSTATUS.Wait.ToString() == job.Transferstatus)
                {
                    if (job.Priority == "0")
                    {
                        displayJob.Cells[0].Value = this.imageListTransferState.Images[6];
                    }
                    else
                    {
                        displayJob.Cells[0].Value = this.imageListTransferState.Images[1];
                    }
                }
                else if (EntityEnumData.TRANSFERSTATUS.Transferring.ToString() == job.Transferstatus)
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[5];
                }
                else if (EntityEnumData.TRANSFERSTATUS.Complete.ToString() == job.Transferstatus)
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[11];
                }
                else
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[0];
                }

                string commandType = job.Commandtype;
                if (job.Commandtype == "INGO")
                {
                    commandType = "입고";
                }
                else if (job.Commandtype == "OUTGO")
                {
                    commandType = "출고";
                }
                else if (job.Commandtype == "MOVE")
                {
                    commandType = "이동";
                }

                displayJob.Cells[1].Value = job.Commandid;
                displayJob.Cells[2].Value = commandType;
                displayJob.Cells[3].Value = job.Priority;
                displayJob.Cells[4].Value = job.Materialno.TrimStart('0');
                displayJob.Cells[5].Value = job.Qty;
                displayJob.Cells[6].Value = job.Palletid;

                if (job.Fromlocationid.Contains("RACK"))
                {
                    string str = "";
                    str = job.Fromsublocationid;
                    if (str.Length == 6)
                    {
                        str = str.Substring(0, 2) + "-" + str.Substring(2, 2) + "-" + str.Substring(4, 2);
                    }

                    displayJob.Cells[7].Value = Util.Util.ConvertLocation(job.Fromlocationid) + "," + str;
                }
                else
                {
                    displayJob.Cells[7].Value = Util.Util.ConvertLocation(job.Fromlocationid);
                }

                if (job.Tolocationid.Contains("RACK"))
                {
                    string str = "";
                    str = job.Tosublocationid;
                    if (str.Length == 6)
                    {
                        str = str.Substring(0, 2) + "-" + str.Substring(2, 2) + "-" + str.Substring(4, 2);
                    }
                    displayJob.Cells[8].Value = Util.Util.ConvertLocation(job.Tolocationid) + "," + str;
                }
                else
                {
                    displayJob.Cells[8].Value = Util.Util.ConvertLocation(job.Tolocationid);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }


        private DataGridViewRow GetDataGridRow(string commandId)
        {
            try
            {
                foreach (DataGridViewRow gridRow in dataGridViewTransferList.Rows)
                {
                    string cmdId = (string)gridRow.Cells[2].Value;
                    if ((cmdId != null && cmdId.Equals(commandId)))
                    {
                        return gridRow;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
            return null;
        }

        private void dataGridView_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0)
            {
                return;
            }
            dataGridViewTransferDetail.Rows.Clear();
            ViewTransferDetail((string)dataGridViewTransferList.Rows[e.RowIndex].Cells[1].Value);
            try
            {
                this.textBoxPriority.Text = (string)dataGridViewTransferList.Rows[e.RowIndex].Cells[3].Value;
            }
            catch (Exception)
            {
            }
        }


        private void ViewTransferDetail(string commandId)
        {
            try
            {
                MovementManager manager = Naming.GetMovementManager();
                TransferData transferData = MovementManager.getTransferData(commandId);
                Dictionary<string, TransferDetailData> transferDetails = manager.getTransferDetailDataList(commandId);

                labelCommandId.Text = transferData.Commandid;
                labelPalletNo.Text = transferData.Palletid;

                Dictionary<string, int> updatedList = new Dictionary<string, int>(100);
                foreach (TransferDetailData transferDetail in transferDetails.Values)
                {
                    DataGridViewRow rowForJob = GetDataGridRowTransferDetail(transferDetail.Commandseq);
                    if (rowForJob == null)  //Job 이 없는거.
                    {
                        int addedRowIndex = dataGridViewTransferDetail.Rows.Add();
                        rowForJob = dataGridViewTransferDetail.Rows[addedRowIndex];
                        updatedList.Add(addedRowIndex.ToString(), addedRowIndex);
                        UpdateTransferDetail(rowForJob, transferDetail);

                    }
                    else
                    {
                        updatedList.Add(rowForJob.Index.ToString(), rowForJob.Index);
                        UpdateTransferDetail(rowForJob, transferDetail);
                    }
                }

                if (dataGridViewTransferDetail.Rows.Count - 1 > updatedList.Count)
                {
                    RemoveTransferDetail(updatedList);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(ex.StackTrace);
            }

        }


        private DataGridViewRow GetDataGridRowTransferDetail(string commandSeq)
        {
            try
            {
                foreach (DataGridViewRow gridRow in dataGridViewTransferDetail.Rows)
                {
                    string seq = (string)gridRow.Cells[1].Value;
                    if (seq != null && seq.Equals(commandSeq))
                    {
                        return gridRow;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
            return null;
        }


        private void RemoveTransferDetail(Dictionary<string, int> updatedList)
        {
            try
            {
                List<DataGridViewRow> removeList = new List<DataGridViewRow>(50);
                foreach (DataGridViewRow gridRow in dataGridViewTransferDetail.Rows)
                {
                    if (gridRow.Index >= 0 && !updatedList.ContainsKey(gridRow.Index.ToString()))
                    {
                        removeList.Add(gridRow);
                    }
                }
                for (int i = 0; i < removeList.Count; i++)
                {
                    dataGridViewTransferDetail.Rows.Remove(removeList[i]);
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private void UpdateTransferDetail(DataGridViewRow displayJob, TransferDetailData transferDetail)
        {
            try
            {
                if (transferDetail == null || displayJob == null)
                {
                    return;
                }

                if (EntityEnumData.TRANSFERDETAILSTATUS.Wait.ToString() == transferDetail.Transferdetailstatus)
                {

                    displayJob.Cells[0].Value = this.imageListTransferState.Images[1];
                }
                else if (EntityEnumData.TRANSFERDETAILSTATUS.Transferring.ToString() == transferDetail.Transferdetailstatus)
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[5];
                }
                else if (EntityEnumData.TRANSFERDETAILSTATUS.Complete.ToString() == transferDetail.Transferdetailstatus)
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[11];
                }
                else
                {
                    displayJob.Cells[0].Value = this.imageListTransferState.Images[0];
                }
                displayJob.Cells[1].Value = transferDetail.Commandseq;
                ItineraryData itinerary = new ItineraryData();
                itinerary.getItineraryData(transferDetail.Pathid);
                if (itinerary != null)
                {
                    displayJob.Cells[2].Value = DeviceNameConverter(itinerary.Fromlocationid );
                    displayJob.Cells[3].Value = string.IsNullOrEmpty(itinerary.Fromsublocationid) ? itinerary.Fromdeviceid : itinerary.Fromsublocationid;
                    displayJob.Cells[4].Value = string.IsNullOrEmpty(itinerary.Tosublocationid) ? itinerary.Todeviceid : itinerary.Tosublocationid;
                }
                else
                {
                    displayJob.Cells[2].Value = DeviceNameConverter(transferDetail.Pathid);
                    displayJob.Cells[3].Value = "알수없음";
                    displayJob.Cells[4].Value = "알수없음";

                }

            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private string DeviceNameConverter(string deviceId)
        {
            string strTemp = deviceId;

            switch (strTemp)
            {
                case "CONVEYOR:1R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:2R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:3R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:4R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:5R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:6R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:7R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:8R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:9R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:10R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:11R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:12R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:13R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:14R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:15R":
                    strTemp = "RGV";
                    break;
                case "CONVEYOR:1S":
                    strTemp = "S/C 1";
                    break;
                case "CONVEYOR:2S":
                    strTemp = "S/C 1";
                    break;
                case "CONVEYOR:3S":
                    strTemp = "S/C 2";
                    break;
                case "CONVEYOR:4S":
                    strTemp = "S/C 2";
                    break;
                case "CONVEYOR:5S":
                    strTemp = "S/C 3";
                    break;
                case "CONVEYOR:6S":
                    strTemp = "S/C 3";
                    break;
                case "CONVEYOR:7S":
                    strTemp = "S/C 4";
                    break;
                case "CONVEYOR:8S":
                    strTemp = "S/C 4";
                    break;
                case "CONVEYOR:9S":
                    strTemp = "S/C 5";
                    break;
                case "CONVEYOR:10S":
                    strTemp = "S/C 5";
                    break;
                case "RACK:1":
                    strTemp = "S/C 1";
                    break;
                case "RACK:2":
                    strTemp = "S/C 1";
                    break;
                case "RACK:3":
                    strTemp = "S/C 2";
                    break;
                case "RACK:4":
                    strTemp = "S/C 2";
                    break;
                case "RACK:5":
                    strTemp = "S/C 3";
                    break;
                case "RACK:6":
                    strTemp = "S/C 3";
                    break;
                case "RACK:7":
                    strTemp = "S/C 4";
                    break;
                case "RACK:8":
                    strTemp = "S/C 4";
                    break;
                case "RACK:9":
                    strTemp = "S/C 5";
                    break;
                case "RACK:10":
                    strTemp = "S/C 5";
                    break;
            }
            return strTemp;
        }

        private void buttonUp_Click(object sender, EventArgs e)
        {

        }

        private void dataGridViewTransferList_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void buttonTransferControl_Click(object sender, EventArgs e)
        {


            TransferControlForm form = (TransferControlForm)SimpleCache.GetObject("Popup", "TransferControlForm");
            if (form == null)
            {
                form = new TransferControlForm();
                SimpleCache.AddObject("Popup", "TransferControlForm", form);
            }
            form.Show();
        }

        private void buttonComplete_Click(object sender, EventArgs e)
        {
            string commandId = "";
            string commandSeq = "";

            //if (dataGridViewTransferDetail.SelectedRows.Count <= 0)
            //{
            //    return;
            //}

            //MovementManager manager = Naming.GetMovementManager();
            //TransferDetailData transferDetailNext = ECSDispatcher.GetNextTransferDetail(commandId, commandSeq);
            //if (transferDetailNext != null)
            //{
            //    transferDetailNext.Palletidtomove = this.labelPalletNo.Text ;
            //    transferDetailNext.updateTransferDetailData();
            //}

            if (dataGridViewTransferDetail.SelectedRows.Count <= 0)
            {
                return;
            }

            commandId = this.labelCommandId.Text;
            commandSeq = (string)dataGridViewTransferDetail.SelectedRows[0].Cells[1].Value;
            //manager.chageTransferDetailComplete(commandId, commandSeq);

            popUpOpen(commandId, commandSeq);
       }

        private void popUpOpen(string commandId, string commandSeq)
        {
            String message = "명령번호(" + commandId + "), 명령순번(" + commandSeq + ")  반송을 삭제 삭제하시겠습니까?";

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
                string commandId = "";
                string commandSeq = "";

                if (dataGridViewTransferDetail.SelectedRows.Count <= 0)
                {
                    return;
                }

                MovementManager manager = Naming.GetMovementManager();
                TransferDetailData transferDetailNext = ECSDispatcher.GetNextTransferDetail(commandId, commandSeq);
                if (transferDetailNext != null)
                {
                    transferDetailNext.Palletidtomove = this.labelPalletNo.Text;
                    transferDetailNext.updateTransferDetailData();

                    //HISTORY Backup LGLS를 위해 추가함.
                    try
                    {
                        transferDetailNext.insertTransferDetailDataBackup();
                    }
                    catch (Exception ex)
                    {
                        //throw ex;
                    }
                }


                commandId = this.labelCommandId.Text;
                commandSeq = (string)dataGridViewTransferDetail.SelectedRows[0].Cells[1].Value;
                manager.chageTransferDetailComplete(commandId, commandSeq);
            }
        }
    }
}
