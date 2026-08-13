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
using ECP.Util.Client;
using HECS.Gui.Monitor.Panels.MoveRequest;
using ECP.Util;

namespace HECS.Gui.Monitor.Panels
{
    public partial class MoveRequestMainPanel : UserControl
    {
        MoveRequestAddPanel moveRequestAddPanel = null;
        MoveRequestInformPanel moveRequestInformPanel = null;

        List<CodeMasterData> positionList = null;
        Dictionary<string, TransferData> transferDataList = null;
        private static Boolean AddModifyFlag = false;

        public MoveRequestMainPanel()
        {
            InitializeComponent();
            InitComponent();
            InitializeDataGrid();
            if (ECP.Global.GlobalConstant.START_MODE != "ECS")
            {
                buttonXPExcel.Visible = false;
                buttonXPModify.Visible = false;
            }
        }

        private enum EnableType
        {
            INIT = 1,
            REG = 2,
            RETRIEVE = 3,
            CANCEL = 4
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
            positionList = null;
            CodeMasterData data = new CodeMasterData();
            positionList = data.getPositionValueList();

        }

        private void setButtonEnabled(EnableType enableType)
        {
            if (enableType == EnableType.INIT)
            {
                buttonXPAdd.Enabled = false;
                buttonXPModify.Enabled = false;
                buttonSave.Enabled = false;
            }
            else if (enableType == EnableType.REG)
            {
                buttonXPAdd.Enabled = false;
                buttonXPModify.Enabled = false;
                buttonSave.Enabled = true;
            }
            else if (enableType == EnableType.RETRIEVE)
            {
                if (dataGridView.RowCount > 0)
                {
                    buttonXPAdd.Enabled = true;
                    buttonXPModify.Enabled = true;
                    buttonSave.Enabled = false;
                }
                else
                {
                    buttonXPAdd.Enabled = true;
                    buttonXPModify.Enabled = false;
                    buttonSave.Enabled = false;
                }
            }
            else if (enableType == EnableType.CANCEL)
            {
                buttonXPAdd.Enabled = false;
                buttonXPModify.Enabled = false;
                buttonSave.Enabled = true;
            }
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //이동일자 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //이동번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //이동상태
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Pallet 번호 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //시작위치
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //시작위치명
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //도착위치 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //도착위치명 

                dataGridView.Columns[0].HeaderText = "이동일자";
                dataGridView.Columns[1].HeaderText = "이동번호";
                dataGridView.Columns[2].HeaderText = "이동상태";
                dataGridView.Columns[3].HeaderText = "팔렛 번호";
                dataGridView.Columns[4].HeaderText = "시작위치";
                dataGridView.Columns[5].HeaderText = "시작위치명";
                dataGridView.Columns[6].HeaderText = "도착위치";
                dataGridView.Columns[7].HeaderText = "도착위치명";

                dataGridView.Columns[0].Width = 160;
                dataGridView.Columns[1].Width = 100;
                dataGridView.Columns[2].Width = 100;
                dataGridView.Columns[3].Width = 80;
                dataGridView.Columns[4].Width = 80;
                dataGridView.Columns[5].Width = 120;
                dataGridView.Columns[6].Width = 80;
                dataGridView.Columns[7].Width = 120;

                dataGridView.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[2].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[3].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[4].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[5].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dataGridView.Columns[6].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dataGridView.Columns[7].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;

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

        private void setDataGridView(TransferData data)
        {
            DataGridViewRow addedRow = new DataGridViewRow();
            int idx = dataGridView.Rows.Add(addedRow);

            string orderDate = data.Createtime.Substring(0, 14);
            orderDate = orderDate.Substring(0, 4) + "-" +
                        orderDate.Substring(4, 2) + "-" +
                        orderDate.Substring(6, 2) + " " +
                        orderDate.Substring(8, 2) + ":" +
                        orderDate.Substring(10, 2) + ":" +
                        orderDate.Substring(12, 2);

            string fromValue = "";
            foreach (CodeMasterData fromTemp in positionList)
            {
                if (fromTemp.Code == data.Fromposition)
                {
                    fromValue = fromTemp.Value;
                    break;
                }
            }

            string toValue = "";
            foreach (CodeMasterData toTemp in positionList)
            {
                if (toTemp.Code == data.Toposition)
                {
                    toValue = toTemp.Value;
                    break;
                }
            }

            dataGridView.Rows[idx].Cells[0].Value = orderDate;
            dataGridView.Rows[idx].Cells[1].Value = data.Commandid;
            dataGridView.Rows[idx].Cells[2].Value = data.Transferstatus;
            dataGridView.Rows[idx].Cells[3].Value = data.Palletid;
            dataGridView.Rows[idx].Cells[4].Value = data.Fromposition;
            dataGridView.Rows[idx].Cells[5].Value = fromValue;
            dataGridView.Rows[idx].Cells[6].Value = data.Toposition;
            dataGridView.Rows[idx].Cells[7].Value = toValue;
        }

        private void buttonXPRetrieve_Click(object sender, EventArgs e)
        {
            TransferData data = new TransferData();
            string fromtransferdate = "";
            string totransferdate = "";

            fromtransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerFrom.Value);
            totransferdate = string.Format("{0:yyyyMMdd}", dateTimePickerTo.Value);

            transferDataList = null;
            transferDataList = data.getTransferDataList();

            dataGridView.Rows.Clear();
            dataGridView.AllowUserToAddRows = true;

            foreach (TransferData dataTemp in transferDataList.Values)
            {
                if (int.Parse(fromtransferdate) <= int.Parse(dataTemp.Createtime.Substring(0, 8)) &&
                    int.Parse(totransferdate) >= int.Parse(dataTemp.Createtime.Substring(0, 8)))
                {
                    setDataGridView(dataTemp);
                }
            }

            dataGridView.AllowUserToAddRows = false;
            AddModifyFlag = false;

            if (dataGridView.Rows.Count > 0)
            {
                displayDetailTransferData(0);
            }
            else
            {
                moveRequestInformPanel.setComponentValues(new TransferData());
                moveRequestAddPanel.setComponentValues(new TransferData());
            }
            setButtonEnabled(EnableType.RETRIEVE);
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

            displayDetailTransferData(e.RowIndex);
        }

        private void displayDetailTransferData(int iRow)
        {
            try
            {
                string commandid = (string)dataGridView.Rows[iRow].Cells[1].Value;

                TransferData data = new TransferData();
                data = transferDataList[commandid];

                moveRequestInformPanel.setComponentValues(new TransferData());
                moveRequestAddPanel.setComponentValues(new TransferData());

                moveRequestInformPanel.setComponentValues(data);
                moveRequestInformPanel.BringToFront();
                setButtonEnabled(EnableType.RETRIEVE);
            }
            catch (Exception ex)
            {
            }

        }

        private void buttonXPModify_Click(object sender, EventArgs e)
        {
            AddModifyFlag = false;
            DataGridViewSelectedRowCollection collection = dataGridView.SelectedRows;

            string transferstatus = (string)collection[0].Cells[2].Value;

            if (transferstatus == EntityEnumData.TRANSFERSTATUS.Wait.ToString())
            {
                string commandid = (string)collection[0].Cells[1].Value;

                TransferData data = new TransferData();
                data = transferDataList[commandid];

                //data.Resultcode = Convert.ToInt32(EntityEnumData.TRANSFERRESULTCODE.CANCEL).ToString();
                data.Transferstatus = EntityEnumData.TRANSFERSTATUS.Complete.ToString();

                moveRequestInformPanel.setComponentValues(data);
                setButtonEnabled(EnableType.CANCEL);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            TransferData data = new TransferData();

            if (AddModifyFlag)
            {
                try
                {
                    data = moveRequestAddPanel.getComponentValues();

                    if (string.IsNullOrEmpty(data.Commandid))
                    {
                        Util.Util.ShowMesagePopup("이동 정보 저장 오류 : 이동 요청 번호가 없습니다.");
                        return;
                    }

                    if (string.IsNullOrEmpty(data.Palletid))
                    {
                        Util.Util.ShowMesagePopup("이동 정보 저장 오류 : Pallet 번호가 없습니다.");
                        return;
                    }

                }
                catch (Exception ex)
                {
                    Util.Util.ShowMesagePopup("이동 정보 저장 오류 : " + ex.Message);
                    return;
                }

                string movementtype = EntityEnumData.MOVEMENTTYPE.MOVE.ToString();

                data.Palletidname = data.Palletid;

                try
                {
                    if (data.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
                    {
                        Naming.GetMovementManager().createIngoTransfer(data);
                    }
                    else if (data.Commandtype == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
                    {
                        Naming.GetMovementManager().createOutgoTransfer(data);
                    }
                    else
                    {
                        Naming.GetMovementManager().createMovementTransfer(data);
                    }

                }
                catch (Exception ex)
                {
                    if (ex.Message == "01")
                    {
                        Util.Util.ShowMesagePopup("이동 정보 저장 오류 : Pallet이 존재하지 않습니다.");
                        return;
                    }
                    else if (ex.Message == "02")
                    {
                        Util.Util.ShowMesagePopup("이동 정보 저장 오류 : Pallet 상태가 맞지 않습니다.");
                        return;
                    }
                    else
                    {
                        Util.Util.ShowMesagePopup(ex.Message);
                        return;
                    }
                }
                setButtonEnabled(EnableType.INIT);
                buttonXPRetrieve_Click(sender, e);
            }
            else
            {
                data = moveRequestInformPanel.getComponentValues();

                try
                {
                    Naming.GetMovementManager().chageTransferCancel(data);
                }
                catch (Exception ex)
                {
                    Util.Util.ShowMesagePopup("이동 정보 저장 오류 : " + ex.Message);
                    return;
                }

                setButtonEnabled(EnableType.INIT);
                buttonXPRetrieve_Click(sender, e);
            }

            setConditionData();
        }

        private void buttonXPAdd_Click(object sender, EventArgs e)
        {
            moveRequestAddPanel.BringToFront();
            AddModifyFlag = true;

            setButtonEnabled(EnableType.REG);
        }

        private void addDetailPanel()
        {
            moveRequestInformPanel = new MoveRequestInformPanel();

            this.splitContainer1.Panel2.Controls.Add(moveRequestInformPanel);

            moveRequestInformPanel.AutoScroll = true;
            moveRequestInformPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            moveRequestInformPanel.Location = new System.Drawing.Point(0, 0);
            moveRequestInformPanel.Name = "moveRequestInformPanel";
            moveRequestInformPanel.Size = new System.Drawing.Size(994, 183);
            moveRequestInformPanel.TabIndex = 0;
            
            moveRequestAddPanel = new MoveRequestAddPanel();

            this.splitContainer1.Panel2.Controls.Add(moveRequestAddPanel);

            moveRequestAddPanel.AutoScroll = true;
            moveRequestAddPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            moveRequestAddPanel.Location = new System.Drawing.Point(0, 0);
            moveRequestAddPanel.Name = "moveRequestAddPanel";
            moveRequestAddPanel.Size = new System.Drawing.Size(994, 183);
            moveRequestAddPanel.TabIndex = 0;
        }
    }
}
