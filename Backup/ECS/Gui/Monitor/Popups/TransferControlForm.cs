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
using HECS.Gui.Widget;
using System.Threading;
using ECP.Db;
using System.Data.SqlClient;
using ECP.Global;

namespace HECS.Gui.Monitor.Popups
{
    public partial class TransferControlForm : DialogBaseForm
    {

        delegate void TransferViewDelegate();
        private string prevValue = "";
        public TransferControlForm()
        {
            InitializeComponent();
            try
            {
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());        //아이콘. 
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewTextBoxColumn());      //
                dataGridViewTransferList.Columns.Add(new DataGridViewCheckBoxColumn (false));      //

                dataGridViewTransferList.Columns[0].HeaderText = "구분";
                dataGridViewTransferList.Columns[1].HeaderText = "자재코드";
                dataGridViewTransferList.Columns[2].HeaderText = "작업번호";
                dataGridViewTransferList.Columns[3].HeaderText = "요청번호";
                dataGridViewTransferList.Columns[4].HeaderText = "출발지";
                dataGridViewTransferList.Columns[5].HeaderText = "도착지";
                dataGridViewTransferList.Columns[6].HeaderText = "우선순위";
                dataGridViewTransferList.Columns[7].HeaderText = "BOOST";



                dataGridViewTransferList.Columns[0].Width = 60;
                dataGridViewTransferList.Columns[0].Resizable = DataGridViewTriState.False;
                dataGridViewTransferList.Columns[1].Width = 80;
                dataGridViewTransferList.Columns[2].Width = 50;
                dataGridViewTransferList.Columns[3].Width = 100;
                dataGridViewTransferList.Columns[4].Width = 50;
                dataGridViewTransferList.Columns[5].Width = 50;
                dataGridViewTransferList.Columns[6].Width = 80;
                dataGridViewTransferList.Columns[7].Width = 70;
                dataGridViewTransferList.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridViewTransferList.MultiSelect = false;
                dataGridViewTransferList.AllowUserToResizeRows = false;

                dataGridViewTransferList.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

                dataGridViewTransferList.Columns[7].DefaultCellStyle.ForeColor = Color.Crimson;
                dataGridViewTransferList.Columns[7].DefaultCellStyle.BackColor = Color.Crimson;
                dataGridViewTransferList.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[6].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewTransferList.Columns[7].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }

        private void TransferControlForm_Activated(object sender, EventArgs e)
        {

        }

        private void buttonNo_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void buttonYes_Click(object sender, EventArgs e)
        {

        }

        private void buttonRefresh_Click(object sender, EventArgs e)
        {
            ViewTransferData();
        }

        private void ViewTransferData()
        {
            try
            {
                this.dataGridViewTransferList.Rows.Clear();
                this.dataGridViewTransferList.AllowUserToAddRows = true;
            }
            catch (Exception)
            {
                System.Console.WriteLine("XXXXXXX");
            }
            Dictionary<string, TransferData> transfers = MovementManager.getTransferDataList();
            foreach (TransferData transfer in transfers.Values)
            {
                int addedIndex = 0;
                DataGridViewRow addedRow = new DataGridViewRow();
                addedIndex = dataGridViewTransferList.Rows.Add();
                addedRow = dataGridViewTransferList.Rows[addedIndex];

                if (transfer.Commandtype == EntityEnumData.COMMANDTYPE.MOVE.ToString())
                {
                    addedRow.Cells[0].Value = "이동";
                }
                else if (transfer.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                {
                    addedRow.Cells[0].Value = "입고";
                }
                else if (transfer.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                {
                    addedRow.Cells[0].Value = "출고";
                }
                else
                {
                    addedRow.Cells[0].Value = transfer.Commandtype;
                }
                addedRow.Cells[1].Value = transfer.Materialno;
                addedRow.Cells[2].Value = transfer.Palletid;
                addedRow.Cells[3].Value = transfer.Commandid;
                addedRow.Cells[4].Value = transfer.Fromposition;
                addedRow.Cells[5].Value = transfer.Toposition;
                addedRow.Cells[6].Value = transfer.Priority;
                if (transfer.IsBoost)
                {
                    addedRow.Cells[7].Value = true;
                }
                else
                {
                    addedRow.Cells[7].Value = false;
                }
            }
            this.dataGridViewTransferList.AllowUserToAddRows = false;
            this.dataGridViewTransferList.EditMode = DataGridViewEditMode.EditProgrammatically;
        }


        private void ControlAddFunction(Control control, TransferData transfer)
        {

        }


        private void TransferControlForm_Shown(object sender, EventArgs e)
        {
            buttonRefresh_Click(this, e);
        }

        private void panelContent_VisibleChanged(object sender, EventArgs e)
        {

            if (this.Visible == true)
            {
                buttonRefresh_Click(this, e);
            }
        }

        private void dataGridViewTransferList_MouseClick(object sender, MouseEventArgs e)
        {

        }

        private void dataGridViewTransferList_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0 || e.ColumnIndex < 0)
            {
                return;
            }
            if (this.dataGridViewTransferList != null)
            {
                if (e.ColumnIndex == 6)
                {
                    this.dataGridViewTransferList.ReadOnly = false;
                    DataGridViewCell cell = dataGridViewTransferList[e.ColumnIndex, e.RowIndex];
                    this.prevValue = cell.Value.ToString();
                    this.dataGridViewTransferList.BeginEdit(true);
                }
                else
                {
                    this.dataGridViewTransferList.ReadOnly = true;
                    //this.dataGridViewTransferList.EndEdit();
                }
            }

        }

        private void dataGridViewTransferList_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0 || e.ColumnIndex < 0)
            {
                return;
            }
            DataGridViewCell cell = dataGridViewTransferList[e.ColumnIndex, e.RowIndex];

            if (prevValue == cell.Value.ToString())
            {
                return;
            }
            int newPriority = 0;
            if (int.TryParse(cell.Value.ToString(), out newPriority) == false)
            {
                cell.Value = this.prevValue;
                return;
            }

            TransferData transferData = MovementManager.getTransferData(dataGridViewTransferList[3, e.RowIndex].Value.ToString());
            if (transferData == null)
            {
                return;
            }

            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                transferData.Priority = newPriority.ToString();
                transferData.updateTransferData(theDB, true);
            }
            catch (SqlException se)
            {
                //Error Log
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }
            this.dataGridViewTransferList.ReadOnly = true;
        }

        private void dataGridViewTransferList_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex < 0)
            {
                return;
            }
            if (e.ColumnIndex != 7)
            {
                return;
            }

            DataGridViewCell cell = dataGridViewTransferList[3, e.RowIndex];
            DataGridViewCell checkValue = dataGridViewTransferList[e.ColumnIndex, e.RowIndex];
            if (checkValue.Value.ToString() == "True")
            {
                TransferData boostCommand = MovementManager.getTransferData(cell.Value.ToString());
                boostCommand.IsBoost = false;
                Dictionary<string, TransferData> transfers = MovementManager.getTransferDataPriorityList();
                foreach (TransferData transfer in transfers.Values)
                {
                    if (transfer.Fromlocationid.Substring(0, 5) == GlobalConstant.SUBSYSTEM_RACK)
                    {
                        if (transfer.Fromdeviceid == boostCommand.Fromdeviceid)
                        {
                            transfer.IsBoost = false;
                        }
                    }
                    else
                    {
                        if (transfer.Fromlocationid == boostCommand.Fromlocationid)
                        {
                            transfer.IsBoost = false;
                        }
                    }
                }
                checkValue.Value = false;
            }
            else
            {
                TransferData boostCommand = MovementManager.getTransferData(cell.Value.ToString());
                boostCommand.IsBoost = true;
                Dictionary<string, TransferData> transfers = MovementManager.getTransferDataPriorityList();
                foreach (TransferData transfer in transfers.Values)
                {
                    if (transfer.Fromlocationid.Substring(0, 5) == GlobalConstant.SUBSYSTEM_RACK)
                    {
                        if (transfer.Fromdeviceid == boostCommand.Fromdeviceid)
                        {
                            //반송 상태가 Wait인 것들은 부스터 기능에서 제외 시킴
                            if (transfer.Commandid != boostCommand.Commandid)
                            {
                                if (transfer.Transferstatus != EntityEnumData.TRANSFERSTATUS.Wait.ToString())
                                {
                                    transfer.IsBoost = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (transfer.Fromlocationid == boostCommand.Fromlocationid)
                        {
                            //반송 상태가 Wait인 것들은 부스터 기능에서 제외 시킴
                            if (transfer.Commandid != boostCommand.Commandid)
                            {
                                if (transfer.Transferstatus != EntityEnumData.TRANSFERSTATUS.Wait.ToString())
                                {
                                    transfer.IsBoost = true;
                                }
                            }
                        }
                    }
                }
                checkValue.Value = true;
            }
            //ViewTransferData();
        }
    }
}