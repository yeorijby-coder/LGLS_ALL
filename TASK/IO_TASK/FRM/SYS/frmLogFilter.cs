// [LGLS 2026-09-02] 로그 필터 폼
//   메인 화면(SYS_MAIN.lsvR)에 실제 표시된 로그를 그대로 읽어 헤더(스레드/구분)별로 걸러 보여준다.
//   별도 사본을 쌓지 않으므로 메인에서 Stop Log / Clear Log 를 하면 여기에도 그대로 반영된다.
//   열 구성 : 0시각 1스레드 2구분 3작업 4상태 5파일 6함수 7Message 8Tgm (SYS_MAIN.PsMsgView 와 동일)

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace TSK_COMM_IOSCH
{
    public partial class frmLogFilter : Form
    {
        private const int COL_THREAD = 1;
        private const int COL_KIND   = 2;
        private const int COL_JOBNO  = 3;
        private const int COL_STATUS = 4;
        private const string KEY_NONE = "(없음)";
        private const string STAT_ALL = "(전체)";

        private ListView m_src;                          // 메인 화면의 로그 ListView (소스)
        private Dictionary<string, bool> m_head = new Dictionary<string, bool>();   // 헤더 → 체크 여부
        private Dictionary<string, bool> m_stat = new Dictionary<string, bool>();   // 등장한 작업상태 집합
        private bool   m_statUpd  = false;               // 작업상태 콤보 갱신 중
        private object m_lastFirst = null;               // 마지막 갱신 시 소스 첫/끝 항목 (변경 감지)
        private object m_lastLast  = null;
        private int    m_lastCnt   = -1;
        private bool   m_dirty     = true;               // 필터 조건 변경 → 재구성 필요
        private bool   m_updating  = false;              // 헤더 목록 갱신 중 (ItemCheck 이벤트 무시)

        public frmLogFilter(ListView pSrc)
        {
            InitializeComponent();
            m_src = pSrc;
            BuildColumns();
            m_statUpd = true;
            try { cboStat.Items.Add(STAT_ALL); cboStat.SelectedIndex = 0; }
            finally { m_statUpd = false; }
        }

        #region 열 구성 (소스 ListView 와 동일)
        private void BuildColumns()
        {
            lsvF.Columns.Clear();
            foreach (ColumnHeader ch in m_src.Columns)
            {
                ColumnHeader c = new ColumnHeader();
                c.Text = ch.Text;
                c.Width = ch.Width;
                lsvF.Columns.Add(c);
            }
            // 소스는 Tgm SubItem 이 열 없이 붙어 있다 - 여기서는 열을 하나 더 둔다.
            ColumnHeader cT = new ColumnHeader();
            cT.Text = "TGM";
            cT.Width = 200;
            lsvF.Columns.Add(cT);
        }
        #endregion

        #region 헤더 키
        private static string SubText(ListViewItem it, int idx)
        {
            return (it.SubItems.Count > idx) ? (it.SubItems[idx].Text ?? "") : "";
        }

        private static string HeadKey(ListViewItem it)
        {
            string th = SubText(it, COL_THREAD).Trim();
            string kd = SubText(it, COL_KIND).Trim();
            if (th.Length == 0) th = KEY_NONE;
            if (kd.Length == 0) kd = KEY_NONE;
            return th + " / " + kd;
        }
        #endregion

        #region 갱신 (타이머 1초)
        private void tmrRefresh_Tick(object sender, EventArgs e)
        {
            try
            {
                if (m_src == null || m_src.IsDisposed) { Close(); return; }
                RefreshHeaders();
                RefreshStatus();
                RefreshList();
            }
            catch (Exception) { }
        }

        // 소스에 등장하는 헤더를 모아 체크 목록을 갱신한다 (새 헤더는 체크 상태로 추가).
        private void RefreshHeaders()
        {
            bool added = false;
            foreach (ListViewItem it in m_src.Items)
            {
                string k = HeadKey(it);
                if (!m_head.ContainsKey(k)) { m_head[k] = true; added = true; }
            }
            if (!added) return;

            List<string> keys = new List<string>(m_head.Keys);
            keys.Sort(StringComparer.Ordinal);

            m_updating = true;
            try
            {
                clbHead.BeginUpdate();
                clbHead.Items.Clear();
                foreach (string k in keys) clbHead.Items.Add(k, m_head[k]);
                clbHead.EndUpdate();
            }
            finally { m_updating = false; }
            m_dirty = true;
        }

        // 현재 표시된 로그의 "작업상태" 열([NN]텍스트)에 등장한 값을 모아 콤보를 갱신한다.
        //   - 첫 항목은 항상 (전체), 사용자가 고른 선택은 유지한다.
        private void RefreshStatus()
        {
            bool added = false;
            foreach (ListViewItem it in m_src.Items)
            {
                string s = SubText(it, COL_STATUS).Trim();
                if (s.Length == 0) continue;
                if (!m_stat.ContainsKey(s)) { m_stat[s] = true; added = true; }
            }
            if (!added) return;

            List<string> keys = new List<string>(m_stat.Keys);
            keys.Sort(StringComparer.Ordinal);

            string sel = (cboStat.SelectedItem as string) ?? STAT_ALL;
            m_statUpd = true;
            try
            {
                cboStat.BeginUpdate();
                cboStat.Items.Clear();
                cboStat.Items.Add(STAT_ALL);
                foreach (string k in keys) cboStat.Items.Add(k);
                int idx = cboStat.Items.IndexOf(sel);
                cboStat.SelectedIndex = (idx >= 0) ? idx : 0;
                cboStat.EndUpdate();
            }
            finally { m_statUpd = false; }
        }

        // 메시지 검색 대상 : 구분 / 파일 / 함수 / Message / Tgm
        private static bool MsgHit(ListViewItem it, string pFind)
        {
            int[] cols = new int[] { COL_KIND, 5, 6, 7, 8 };
            for (int i = 0; i < cols.Length; i++)
            {
                string s = SubText(it, cols[i]);
                if (s.Length > 0 && s.IndexOf(pFind, StringComparison.OrdinalIgnoreCase) >= 0) return true;
            }
            return false;
        }

        // 헤더 체크 AND 작업상태 AND 메시지검색 AND 작업번호검색 (모두 만족해야 표시)
        private bool Pass(ListViewItem it, string pFind, string pJob, string pStat)
        {
            bool on;
            if (!m_head.TryGetValue(HeadKey(it), out on) || !on) return false;

            if (pStat.Length > 0 && pStat != STAT_ALL)
            {
                if (!string.Equals(SubText(it, COL_STATUS).Trim(), pStat, StringComparison.OrdinalIgnoreCase))
                    return false;
            }
            if (pFind.Length > 0 && !MsgHit(it, pFind)) return false;
            if (pJob.Length > 0)
            {
                string j = SubText(it, COL_JOBNO);
                if (j.IndexOf(pJob, StringComparison.OrdinalIgnoreCase) < 0) return false;
            }
            return true;
        }

        // 소스가 바뀌었거나(추가/삭제/Clear) 필터 조건이 바뀌면 목록을 다시 만든다.
        private void RefreshList()
        {
            int cnt = m_src.Items.Count;
            object first = (cnt > 0) ? m_src.Items[0] : null;
            object last  = (cnt > 0) ? m_src.Items[cnt - 1] : null;

            if (!m_dirty && cnt == m_lastCnt && ReferenceEquals(first, m_lastFirst) && ReferenceEquals(last, m_lastLast))
                return;

            m_lastCnt = cnt; m_lastFirst = first; m_lastLast = last; m_dirty = false;

            string find = txtFind.Text.Trim();
            string job  = txtJob.Text.Trim();
            string stat = (cboStat.SelectedItem as string) ?? STAT_ALL;
            int shown = 0;

            lsvF.BeginUpdate();
            try
            {
                lsvF.Items.Clear();
                foreach (ListViewItem it in m_src.Items)
                {
                    if (!Pass(it, find, job, stat)) continue;
                    ListViewItem n = new ListViewItem(it.Text);
                    for (int i = 1; i < it.SubItems.Count; i++) n.SubItems.Add(it.SubItems[i].Text);
                    n.BackColor = it.BackColor;
                    n.ForeColor = it.ForeColor;
                    lsvF.Items.Add(n);
                    shown++;
                }
            }
            finally { lsvF.EndUpdate(); }

            if (chkAuto.Checked && lsvF.Items.Count > 0)
                lsvF.EnsureVisible(lsvF.Items.Count - 1);

            lblCnt.Text = shown + " / " + cnt;
        }
        #endregion

        #region 이벤트
        private void clbHead_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (m_updating) return;
            string k = clbHead.Items[e.Index] as string;
            if (k == null) return;
            m_head[k] = (e.NewValue == CheckState.Checked);
            m_dirty = true;
        }

        private void txtFind_TextChanged(object sender, EventArgs e)
        {
            m_dirty = true;
        }

        private void cboStat_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (m_statUpd) return;
            m_dirty = true;
        }

        // 검색어 / 작업상태 선택을 한 번에 초기화한다 (헤더 체크는 건드리지 않는다).
        private void btnClear_Click(object sender, EventArgs e)
        {
            txtFind.Text = "";
            txtJob.Text = "";
            if (cboStat.Items.Count > 0) cboStat.SelectedIndex = 0;
            m_dirty = true;
        }

        private void SetAll(bool pOn)
        {
            List<string> keys = new List<string>(m_head.Keys);
            foreach (string k in keys) m_head[k] = pOn;
            m_updating = true;
            try
            {
                for (int i = 0; i < clbHead.Items.Count; i++) clbHead.SetItemChecked(i, pOn);
            }
            finally { m_updating = false; }
            m_dirty = true;
        }

        private void btnAll_Click(object sender, EventArgs e)  { SetAll(true); }
        private void btnNone_Click(object sender, EventArgs e) { SetAll(false); }

        private void lsvF_Click(object sender, EventArgs e)
        {
            try
            {
                if (lsvF.SelectedItems.Count == 0) return;
                ListViewItem it = lsvF.SelectedItems[0];
                string strFile = SubText(it, 5);
                string strFunc = SubText(it, 6);
                string strBody = SubText(it, 7);
                string strTgm  = SubText(it, 8);
                string strHead = (strFile.Length > 0 || strFunc.Length > 0) ? ("[" + strFile + " :: " + strFunc + "]  ") : "";
                txtMsg.Text = strHead + strBody + (strTgm.Trim().Length > 0 ? ("\r\nTGM : " + strTgm) : "");
            }
            catch (Exception) { }
        }

        private void frmLogFilter_Load(object sender, EventArgs e)
        {
            tmrRefresh.Enabled = true;
            tmrRefresh_Tick(null, null);
        }

        private void frmLogFilter_FormClosed(object sender, FormClosedEventArgs e)
        {
            tmrRefresh.Enabled = false;
        }
        #endregion
    }
}
