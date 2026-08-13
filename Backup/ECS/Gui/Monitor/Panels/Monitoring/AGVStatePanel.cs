using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor.Panels.Monitoring
{
    public partial class AGVStatePanel : UserControl
    {
        public AGVStatePanel()
        {
            InitializeComponent();
            InitializeDataGrid();
        }

        private void InitializeDataGrid()
        {
            try
            {
                dataGridView.AllowUserToAddRows = true;
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //작업번호 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //AGV 작업번호  
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Pallet 번호
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //Vehicle상태 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //출발위치
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //도착위치 
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //FROM
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //TO
                dataGridView.Columns.Add(new DataGridViewTextBoxColumn());      //POS
                


                dataGridView.Columns[0].HeaderText = "작업번호";
                dataGridView.Columns[1].HeaderText = "AGV작업번호";
                dataGridView.Columns[2].HeaderText = "Pallet번호";
                dataGridView.Columns[3].HeaderText = "Vehicle상태";
                dataGridView.Columns[4].HeaderText = "출발위치";
                dataGridView.Columns[5].HeaderText = "도착위치";
                dataGridView.Columns[6].HeaderText = "FROM";
                dataGridView.Columns[7].HeaderText = "TO";
                dataGridView.Columns[8].HeaderText = "LOCATION";

                dataGridView.Columns[0].Width = 120;
                dataGridView.Columns[1].Width = 120;
                dataGridView.Columns[2].Width = 120;
                dataGridView.Columns[3].Width = 120;
                dataGridView.Columns[4].Width = 120;
                dataGridView.Columns[5].Width = 120;
                dataGridView.Columns[6].Width = 120;
                dataGridView.Columns[7].Width = 120;
                dataGridView.Columns[8].Width = 120;


                dataGridView.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;
                dataGridView.AllowUserToAddRows = false;

                dataGridView.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

                for (int i = 0; i < 10; i++)
                {
                    int rowIndex = dataGridView.Rows.Add();
                    if (rowIndex < 0)
                    {
                        return;
                    }
                    DataGridViewRow addedRow = dataGridView.Rows[rowIndex];
                    addedRow.Resizable = DataGridViewTriState.False;
                    addedRow.Cells[0].Value = i.ToString();
                    addedRow.Cells[1].Value = (i % 3).ToString();
                    addedRow.Cells[2].Value = (i * 1000).ToString();
                    addedRow.Cells[3].Value = i % 2 == 1 ? "FROM 이동 중" : "TO 도착완료";
                    addedRow.Cells[4].Value = i % 2 == 1 ? "HIRACK#1" : "포장";
                    addedRow.Cells[5].Value = i % 2 == 1 ? "입고대" : "HIRACK#1";
                    addedRow.Cells[6].Value = i * 3;
                    addedRow.Cells[7].Value = i*4 +3; 
                    addedRow.Cells[8].Value = i * 3 + 2;

                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(this.Name + ":" + ex.StackTrace);
            }
        }
    }
}
