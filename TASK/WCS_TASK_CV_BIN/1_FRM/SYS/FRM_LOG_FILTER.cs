using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace WCS_TASK_CV
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-09-02] 로그 필터 창 (IO_TASK 의 헤더별 필터 폼과 같은 역할)
    //
    //   · 소스 = 메인 화면의 로그 ListView(lsvCOMM1) 그 자체. 사본을 만들지 않고 1초마다
    //     메인의 Items 를 다시 훑어 조건에 맞는 행만 표시한다.
    //     → 메인에서 로그 중지/Clear 하면 여기에도 다음 갱신(1초 이내)에 반영된다.
    //   · 헤더 체크박스 = 메인 로그의 "Thread No"(설비/스레드 구분) 값을 동적으로 수집.
    //   · 텍스트 검색 = Message / Telegram / 파일 / 함수 / Cmd 열, 주소 검색 = "주소" 열.
    //
    //   메인 열 순서 : 0 시각 / 1 Thread No / 2 Cmd / 3 주소 / 4 파일 / 5 함수 / 6 Message / 7 Telegram
    // ─────────────────────────────────────────────────────────────────────────
    public class FRM_LOG_FILTER : Form
    {
        private const int COL_TIME = 0, COL_ID = 1, COL_CMD = 2, COL_ADDR = 3, COL_FILE = 4, COL_FUNC = 5, COL_MSG = 6, COL_TGM = 7;

        private readonly SYS_MAIN m_frmMain;
        private readonly ListView m_lvSrc;

        private CheckedListBox m_clbHeader;
        private TextBox m_txtText;
        private TextBox m_txtAddr;
        private CheckBox m_chkAllHeader;
        private CheckBox m_chkPause;
        private CheckBox m_chkTail;
        private Label m_lblCount;
        private ListView m_lv;
        private Timer m_timer;

        private readonly Dictionary<string, bool> m_dicHeader = new Dictionary<string, bool>();
        private bool m_bHeaderLoading = false;
        private string m_strLastSig = null;   // 마지막 표시 상태 서명 (불필요한 재그리기 방지)

        public FRM_LOG_FILTER(SYS_MAIN pMain)
        {
            m_frmMain = pMain;
            m_lvSrc = pMain.lsvCOMM1;
            BuildUi();
            m_timer = new Timer();
            m_timer.Interval = 1000;
            m_timer.Tick += delegate { Refresh_(); };
            m_timer.Start();
            Refresh_();
        }

        private void BuildUi()
        {
            this.Text = "로그 필터 (메인 로그 참조 - 1초 갱신)";
            this.Size = new Size(1300, 640);
            this.StartPosition = FormStartPosition.CenterParent;
            this.Font = new Font("맑은 고딕", 9F);

            // 왼쪽 : 헤더 체크 목록
            var pnlLeft = new Panel { Dock = DockStyle.Left, Width = 200, Padding = new Padding(4) };
            m_chkAllHeader = new CheckBox { Text = "전체 선택/해제", Dock = DockStyle.Top, Checked = true, Height = 22 };
            m_chkAllHeader.CheckedChanged += delegate
            {
                if (m_bHeaderLoading) return;
                m_bHeaderLoading = true;
                for (int i = 0; i < m_clbHeader.Items.Count; i++) m_clbHeader.SetItemChecked(i, m_chkAllHeader.Checked);
                var keys = new List<string>(m_dicHeader.Keys);
                foreach (string k in keys) m_dicHeader[k] = m_chkAllHeader.Checked;
                m_bHeaderLoading = false;
                m_strLastSig = null; Refresh_();
            };
            var lblH = new Label { Text = "헤더(Thread No / 설비)", Dock = DockStyle.Top, Height = 20 };
            m_clbHeader = new CheckedListBox { Dock = DockStyle.Fill, CheckOnClick = true };
            m_clbHeader.ItemCheck += delegate(object s, ItemCheckEventArgs e)
            {
                if (m_bHeaderLoading) return;
                string key = m_clbHeader.Items[e.Index].ToString();
                m_dicHeader[key] = (e.NewValue == CheckState.Checked);
                m_strLastSig = null;
                BeginInvoke(new MethodInvoker(Refresh_));
            };
            pnlLeft.Controls.Add(m_clbHeader);
            pnlLeft.Controls.Add(lblH);
            pnlLeft.Controls.Add(m_chkAllHeader);

            // 위 : 검색 조건
            var pnlTop = new Panel { Dock = DockStyle.Top, Height = 34 };
            var lblT = new Label { Text = "텍스트", Location = new Point(6, 9), AutoSize = true };
            m_txtText = new TextBox { Location = new Point(56, 6), Width = 320 };
            var lblA = new Label { Text = "주소값", Location = new Point(390, 9), AutoSize = true };
            m_txtAddr = new TextBox { Location = new Point(440, 6), Width = 160 };
            m_txtText.TextChanged += delegate { m_strLastSig = null; Refresh_(); };
            m_txtAddr.TextChanged += delegate { m_strLastSig = null; Refresh_(); };
            m_chkPause = new CheckBox { Text = "갱신 정지", Location = new Point(615, 8), AutoSize = true };
            m_chkTail = new CheckBox { Text = "끝으로 스크롤", Location = new Point(700, 8), AutoSize = true, Checked = true };
            var btnClear = new Button { Text = "조건 지우기", Location = new Point(810, 5), Size = new Size(90, 24) };
            btnClear.Click += delegate { m_txtText.Text = ""; m_txtAddr.Text = ""; };
            m_lblCount = new Label { Location = new Point(910, 9), AutoSize = true, Text = "0 / 0" };
            pnlTop.Controls.AddRange(new Control[] { lblT, m_txtText, lblA, m_txtAddr, m_chkPause, m_chkTail, btnClear, m_lblCount });

            // 가운데 : 결과
            m_lv = new ListView { Dock = DockStyle.Fill, View = View.Details, FullRowSelect = true, GridLines = true, MultiSelect = false };
            foreach (ColumnHeader c in m_lvSrc.Columns)
                m_lv.Columns.Add(c.Text, c.Width > 0 ? c.Width : 80);
            m_lv.HeaderStyle = ColumnHeaderStyle.Nonclickable;
            try { WcsCommon.cLogCols.Attach(m_lv); } catch { }

            this.Controls.Add(m_lv);
            this.Controls.Add(pnlTop);
            this.Controls.Add(pnlLeft);
            this.FormClosed += delegate { try { m_timer.Stop(); m_timer.Dispose(); } catch { } };
        }

        private static string Sub(ListViewItem it, int idx)
        {
            return (it.SubItems.Count > idx) ? (it.SubItems[idx].Text ?? "") : "";
        }

        /// <summary>메인 로그를 다시 훑어 헤더 목록/표시 행을 갱신</summary>
        private void Refresh_()
        {
            if (this.IsDisposed || m_lvSrc == null || m_lvSrc.IsDisposed) return;
            if (m_chkPause != null && m_chkPause.Checked) return;
            try
            {
                // 1) 헤더 수집 (새 값은 [전체] 체크 상태를 따라 추가)
                bool bNewHeader = false;
                var lstSrc = new List<ListViewItem>(m_lvSrc.Items.Count);
                foreach (ListViewItem it in m_lvSrc.Items)
                {
                    lstSrc.Add(it);
                    string id = Sub(it, COL_ID);
                    if (id.Length == 0) id = "(없음)";
                    if (!m_dicHeader.ContainsKey(id)) { m_dicHeader[id] = m_chkAllHeader.Checked; bNewHeader = true; }
                }
                if (bNewHeader)
                {
                    m_bHeaderLoading = true;
                    var keys = new List<string>(m_dicHeader.Keys); keys.Sort(StringComparer.Ordinal);
                    m_clbHeader.BeginUpdate();
                    m_clbHeader.Items.Clear();
                    foreach (string k in keys) m_clbHeader.Items.Add(k, m_dicHeader[k]);
                    m_clbHeader.EndUpdate();
                    m_bHeaderLoading = false;
                }

                // 2) 필터
                string sText = m_txtText.Text.Trim();
                string sAddr = m_txtAddr.Text.Trim();
                var lstHit = new List<ListViewItem>();
                foreach (ListViewItem it in lstSrc)
                {
                    string id = Sub(it, COL_ID); if (id.Length == 0) id = "(없음)";
                    bool bOn; if (m_dicHeader.TryGetValue(id, out bOn) && !bOn) continue;
                    if (sAddr.Length > 0 && Sub(it, COL_ADDR).IndexOf(sAddr, StringComparison.OrdinalIgnoreCase) < 0) continue;
                    if (sText.Length > 0)
                    {
                        bool hit = Sub(it, COL_MSG).IndexOf(sText, StringComparison.OrdinalIgnoreCase) >= 0
                                || Sub(it, COL_TGM).IndexOf(sText, StringComparison.OrdinalIgnoreCase) >= 0
                                || Sub(it, COL_FILE).IndexOf(sText, StringComparison.OrdinalIgnoreCase) >= 0
                                || Sub(it, COL_FUNC).IndexOf(sText, StringComparison.OrdinalIgnoreCase) >= 0
                                || Sub(it, COL_CMD).IndexOf(sText, StringComparison.OrdinalIgnoreCase) >= 0;
                        if (!hit) continue;
                    }
                    lstHit.Add(it);
                }

                // 3) 변화 없으면 재그리기 생략 (첫/마지막 행 시각 + 건수 + 조건으로 판정)
                string sig = lstSrc.Count + "|" + lstHit.Count + "|" + sText + "|" + sAddr
                           + "|" + (lstHit.Count > 0 ? Sub(lstHit[0], COL_TIME) + Sub(lstHit[lstHit.Count - 1], COL_TIME) : "");
                if (sig == m_strLastSig) return;
                m_strLastSig = sig;

                m_lv.BeginUpdate();
                m_lv.Items.Clear();
                foreach (ListViewItem it in lstHit)
                {
                    var v = new ListViewItem(Sub(it, COL_TIME));
                    for (int i = 1; i < m_lvSrc.Columns.Count; i++) v.SubItems.Add(Sub(it, i));
                    v.BackColor = it.BackColor; v.ForeColor = it.ForeColor;
                    m_lv.Items.Add(v);
                }
                m_lv.EndUpdate();
                if (m_chkTail.Checked && m_lv.Items.Count > 0) m_lv.EnsureVisible(m_lv.Items.Count - 1);
                m_lblCount.Text = lstHit.Count + " / " + lstSrc.Count + " 건";
            }
            catch (Exception ex)
            {
                m_lblCount.Text = "갱신 오류: " + ex.Message;
            }
        }
    }
}
