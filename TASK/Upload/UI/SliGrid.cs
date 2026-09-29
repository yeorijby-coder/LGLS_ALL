//최초작성자	: LGLS
//작성일		: 20260929
//개요		    : FarPoint Spread 를 대신하는 목록 컨트롤.
//
//  왜 바꾸었나
//    FarPoint Spread 는 유료 부품이고, 이 PC 에도 현장 PC 에도 라이선스가 없다.
//    라이선스가 없으면 빌드할 때 "Spread.NET License Notification" 창이 떠서
//    빌드가 멈추고, 어떻게 넘겨도 프로그램을 띄울 때 같은 창이 다시 뜬다.
//    목록을 보여 주는 일에 유료 부품이 필요하지 않으므로, .NET 이 기본으로 주는
//    DataGridView 로 바꾸었다.
//
//  종전 코드가 쓰던 이름을 그대로 남겨 두어, 부르는 쪽은 거의 고치지 않았다.
//    InitSpread / GetGridText / ActiveRowIndex / RowCount / Rows / DataSource
//
//수정이력		:

using System;
using System.Data;
using System.Drawing;
using System.Windows.Forms;

namespace WmsUp
{
	/// <summary>목록 하나 = 제목 띠 + 표</summary>
	public class SliGrid : UserControl
	{
		private Label m_lblTitle;
		private DataGridView m_grid;

		// [LGLS 2026-09-30] 자료를 붙이는 동안임을 표시한다.
		//   붙이는 순간 DataGridView 가 스스로 첫 줄로 들어가면서 RowEnter 를 낸다.
		//   그 처리기가 다시 같은 표를 조회하면 제자리를 맴돌다
		//   "이 이벤트 처리기에서 작업을 수행할 수 없습니다" 가 난다.
		private bool m_bBinding = false;

		private string m_strTitle = "";
		private string m_strMsg = "";

		public SliGrid()
		{
			m_lblTitle = new Label();
			m_lblTitle.Dock = DockStyle.Top;
			m_lblTitle.Height = 22;
			m_lblTitle.TextAlign = ContentAlignment.MiddleLeft;
			m_lblTitle.BackColor = Color.Lavender;
			m_lblTitle.ForeColor = Color.Black;
			m_lblTitle.Font = new Font("굴림", 9F, FontStyle.Bold, GraphicsUnit.Point, ((byte)(129)));
			m_lblTitle.Padding = new Padding(6, 0, 0, 0);

			m_grid = new DataGridView();
			m_grid.Dock = DockStyle.Fill;
			m_grid.AllowUserToAddRows = false;
			m_grid.AllowUserToDeleteRows = false;
			m_grid.AllowUserToResizeRows = false;
			m_grid.ReadOnly = true;
			m_grid.RowHeadersVisible = false;
			m_grid.MultiSelect = false;
			m_grid.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
			m_grid.AutoGenerateColumns = true;
			m_grid.BackgroundColor = SystemColors.Window;
			m_grid.BorderStyle = BorderStyle.FixedSingle;
			m_grid.Font = new Font("굴림", 9F, FontStyle.Regular, GraphicsUnit.Point, ((byte)(129)));
			m_grid.ColumnHeadersDefaultCellStyle.BackColor = Color.Lavender;
			m_grid.ColumnHeadersDefaultCellStyle.ForeColor = Color.Black;
			m_grid.ColumnHeadersDefaultCellStyle.Font
				= new Font("굴림", 9F, FontStyle.Bold, GraphicsUnit.Point, ((byte)(129)));
			m_grid.EnableHeadersVisualStyles = false;
			m_grid.DefaultCellStyle.SelectionBackColor = Color.SlateGray;
			m_grid.DefaultCellStyle.SelectionForeColor = Color.White;

			m_grid.CellClick += new DataGridViewCellEventHandler(Grid_CellClick);

			this.Controls.Add(m_grid);
			this.Controls.Add(m_lblTitle);
		}

		// ── 종전 이름 그대로 ────────────────────────────────────────────
		public DataGridView Grid { get { return m_grid; } }

		/// <summary>제목을 달고, 자기 자신을 sh 에 돌려준다(종전 Spread 의 시트 자리).</summary>
		public void InitSpread(ref SliGrid sh, string strTitle, bool blEdit)
		{
			m_strTitle = strTitle;
			m_grid.ReadOnly = !blEdit;
			RefreshTitle();
			sh = this;
		}

		/// <summary>고른 줄의 어느 칸 값. 없으면 빈 문자열 - 지어내지 않는다.</summary>
		public string GetGridText(int nRow, string strCol)
		{
			if (nRow < 0 || nRow >= m_grid.Rows.Count) return "";
			if (!m_grid.Columns.Contains(strCol)) return "";
			object o = m_grid.Rows[nRow].Cells[strCol].Value;
			return (o == null || o == DBNull.Value) ? "" : o.ToString().Trim();
		}

		/// <summary>고른 줄 번호. 고른 줄이 없으면 -1.</summary>
		public int ActiveRowIndex
		{
			get
			{
				if (m_grid.CurrentRow != null) return m_grid.CurrentRow.Index;
				// [LGLS 2026-09-30] 자료를 막 붙인 직후에는 아직 고른 줄이 없다.
				//   그때 -1 을 주면 상세가 늘 비어 보였다. 줄이 있으면 첫 줄을 알려 준다.
				return (m_grid.Rows.Count > 0) ? 0 : -1;
			}
		}

		/// <summary>0 을 넣으면 비운다(종전 Spread 와 같다).</summary>
		public int RowCount
		{
			get { return m_grid.Rows.Count; }
			set
			{
				if (value == 0)
				{
					m_grid.DataSource = null;
					m_grid.Rows.Clear();
					RefreshTitle();
				}
			}
		}

		public DataGridViewRowCollection Rows { get { return m_grid.Rows; } }

		public object DataSource
		{
			get { return m_grid.DataSource; }
			set
			{
				// [LGLS 2026-09-30] 여기서 첫 줄을 골라 두면 안 된다.
				//   자료를 붙이는 도중에 줄을 고르면 RowEnter 가 곧바로 나고,
				//   그 안에서 다른 표에 자료를 붙이다가
				//   "이 이벤트 처리기에서 작업을 수행할 수 없습니다" 가 난다.
				//   대신 ActiveRowIndex 가 고른 줄이 없을 때 첫 줄을 알려 준다.
				m_bBinding = true;
				try
				{
					m_grid.DataSource = value;
					ApplyColumns();		// 자료를 붙이면 칸이 다시 만들어진다
					RefreshTitle();
				}
				finally { m_bBinding = false; }
			}
		}

		/// <summary>제목 띠에 "조회 건수" 같은 말을 붙인다(종전 ShowMsgGrid).</summary>
		public void ShowMsg(string strMsg)
		{
			m_strMsg = strMsg;
			RefreshTitle();
		}

		private void RefreshTitle()
		{
			m_lblTitle.Text = (m_strMsg.Length == 0)
							? m_strTitle
							: m_strTitle + "     [ " + m_strMsg + " ]";
		}

		/// <summary>칸 이름을 한글로 바꾸고 너비를 준다. 이름이 없는 칸은 감춘다.</summary>
		public void SetColumns(string[] arrCol, string[] arrHead, int[] arrWidth, string[] arrAlign)
		{
			m_arrCol = arrCol;
			m_arrHead = arrHead;
			m_arrWidth = arrWidth;
			m_arrAlign = arrAlign;
			ApplyColumns();
		}

		private string[] m_arrCol, m_arrHead, m_arrAlign;
		private int[] m_arrWidth;

		private void ApplyColumns()
		{
			if (m_arrCol == null) return;

			// [LGLS 2026-09-30] 먼저 전부 숨기면 안 된다.
			//   고른 칸까지 사라지면서 DataGridView 가 스스로 칸을 옮기려 들고,
			//   그 자리에서 "이 이벤트 처리기에서 작업을 수행할 수 없습니다" 가 난다.
			//   쓸 칸을 먼저 세워 두고, 남은 칸만 나중에 숨긴다.
			for (int i = 0; i < m_arrCol.Length; i++)
			{
				if (!m_grid.Columns.Contains(m_arrCol[i])) continue;
				DataGridViewColumn c = m_grid.Columns[m_arrCol[i]];
				c.HeaderText = m_arrHead[i];
				c.Width = m_arrWidth[i];
				c.DisplayIndex = i;
				c.Visible = true;
				c.SortMode = DataGridViewColumnSortMode.Automatic;

				if (m_arrAlign[i] == "R")
					c.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
				else if (m_arrAlign[i] == "C")
					c.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
				else
					c.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
			}

			// 목록에 없는 칸만 숨긴다
			foreach (DataGridViewColumn c in m_grid.Columns)
			{
				bool bUse = false;
				for (int i = 0; i < m_arrCol.Length; i++)
					if (c.Name == m_arrCol[i]) { bUse = true; break; }
				if (!bUse) c.Visible = false;
			}
		}

		// ── 종전 Spread 이벤트를 그대로 흉내 낸다 ──────────────────────
		public event EventHandler CellClickEx;
		public event EventHandler EnterCellEx;

		// [LGLS 2026-09-30] 사람이 누를 때만 알린다.
		//   종전 Spread 의 EnterCell 은 사람이 칸을 옮길 때만 났는데,
		//   DataGridView 의 RowEnter 는 자료를 붙이는 순간에도 난다.
		//   그대로 이으면 상세 조회가 스스로를 다시 불러 맴돈다.
		private void Grid_CellClick(object sender, DataGridViewCellEventArgs e)
		{
			if (m_bBinding) return;
			if (CellClickEx != null) CellClickEx(this, EventArgs.Empty);
			if (EnterCellEx != null) EnterCellEx(this, EventArgs.Empty);
		}
	}
}
