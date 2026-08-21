using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace WcsCommon
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-08-21] 로그 ListView 공용 헬퍼 (WCS_TASK_CV / IO_TASK / HOST_TASK 링크 공유)
    //
    //   1) Attach(lv)   : 헤더를 마우스 오른쪽 클릭하면 열 표시/숨김 체크 메뉴를 띄운다.
    //                     숨김 = 폭 0 (원래 폭은 복원용으로 보관).
    //   2) ShortFile()  : CallerFilePath 전체 경로 → 파일명만.
    //
    //   헤더는 ListView 의 자식 컨트롤이라 ContextMenuStrip 을 달아도 우클릭을 받지
    //   못한다 → LVM_GETHEADER 로 헤더 핸들을 얻어 NativeWindow 로 서브클래싱한다.
    // ─────────────────────────────────────────────────────────────────────────
    public static class cLogCols
    {
        private const int LVM_GETHEADER   = 0x101F;
        private const int WM_RBUTTONUP    = 0x0205;
        private const int WM_CONTEXTMENU  = 0x007B;

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, IntPtr lParam);

        /// <summary>CallerFilePath 전체 경로 → 파일명만 (표시용)</summary>
        public static string ShortFile(string pPath)
        {
            if (string.IsNullOrEmpty(pPath)) return "";
            try { return System.IO.Path.GetFileName(pPath); }
            catch { return pPath; }
        }

        /// <summary>헤더 우클릭 → 열 표시/숨김 메뉴 부착</summary>
        public static void Attach(ListView pLv)
        {
            if (pLv == null) return;
            if (pLv.IsHandleCreated) DoAttach(pLv);
            else pLv.HandleCreated += delegate { DoAttach(pLv); };
            // 핸들 재생성(RecreateHandle) 대비
            pLv.HandleCreated += delegate { DoAttach(pLv); };
        }

        private static readonly Dictionary<ListView, HeaderHook> m_dicHook =
            new Dictionary<ListView, HeaderHook>();

        private static void DoAttach(ListView pLv)
        {
            try
            {
                IntPtr hHeader = SendMessage(pLv.Handle, LVM_GETHEADER, IntPtr.Zero, IntPtr.Zero);
                if (hHeader == IntPtr.Zero) return;

                HeaderHook hook;
                if (m_dicHook.TryGetValue(pLv, out hook))
                {
                    if (hook.Handle == hHeader) return;      // 이미 부착됨
                    try { hook.ReleaseHandle(); } catch { }
                    m_dicHook.Remove(pLv);
                }
                hook = new HeaderHook(pLv);
                hook.AssignHandle(hHeader);
                m_dicHook[pLv] = hook;
            }
            catch { }
        }

        private class HeaderHook : NativeWindow
        {
            private readonly ListView m_lv;
            public HeaderHook(ListView lv) { m_lv = lv; }

            protected override void WndProc(ref Message m)
            {
                if (m.Msg == WM_RBUTTONUP || m.Msg == WM_CONTEXTMENU)
                {
                    ShowColumnMenu(m_lv);
                    if (m.Msg == WM_CONTEXTMENU) return;     // 기본 메뉴 억제
                }
                base.WndProc(ref m);
            }
        }

        private static void ShowColumnMenu(ListView pLv)
        {
            try
            {
                ContextMenuStrip mnu = new ContextMenuStrip();
                foreach (ColumnHeader col in pLv.Columns)
                {
                    ColumnHeader colLocal = col;
                    bool bVisible = colLocal.Width > 0;
                    ToolStripMenuItem mi = new ToolStripMenuItem(
                        string.IsNullOrEmpty(colLocal.Text) ? ("(열 " + (colLocal.Index + 1) + ")") : colLocal.Text);
                    mi.Checked = bVisible;
                    mi.CheckOnClick = false;
                    mi.Click += delegate
                    {
                        if (colLocal.Width > 0)
                        {
                            // 숨김 : 현재 폭 보관 후 0. 마지막 남은 열 하나는 숨기지 않는다.
                            int nShown = 0;
                            foreach (ColumnHeader c in pLv.Columns) if (c.Width > 0) nShown++;
                            if (nShown <= 1) return;
                            colLocal.Tag = colLocal.Width;
                            colLocal.Width = 0;
                        }
                        else
                        {
                            int nRestore = 80;
                            if (colLocal.Tag is int && (int)colLocal.Tag > 0) nRestore = (int)colLocal.Tag;
                            colLocal.Width = nRestore;
                        }
                    };
                    mnu.Items.Add(mi);
                }
                mnu.Show(Cursor.Position);
            }
            catch { }
        }
    }
}
