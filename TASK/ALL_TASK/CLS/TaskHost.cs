using System;
using System.Windows.Forms;

namespace ALL_TASK
{
    public enum TaskKind
    {
        EQP  = 0,       // 설비 통신 (구 WCS_TASK_CV)
        IO   = 1,       // 스케줄러 (구 IO_TASK)
        HOST = 2        // 상위 통신 (구 WCS_TASK_HOST)
    }

    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-09-09] TaskHost : 태스크 1개(EQP/IO/HOST)의 기동·정지 래퍼
    //
    //   기동 : 원래 태스크의 메인 폼을 만들어 TopLevel=false 로 탭 안에 끼운다.
    //          폼의 Load 가 그대로 돌면서 통신/작업 스레드가 종전과 똑같이 뜬다.
    //   정지 : 폼의 TaskStop() 으로 스레드를 세우고 소켓/DB 를 반납한 뒤 폼을 버린다.
    //          다시 켜면 폼을 새로 만들어 처음부터 태운다(=완전 재기동).
    //
    //   ※ 세 폼 모두 CheckForIllegalCrossThreadCalls=false 로 돌던 코드라 UI 스레드를
    //     공유해도 종전과 동작이 같다. 다만 한 태스크가 UI 를 오래 붙잡으면 다른 탭도
    //     같이 굳으므로, 화면 로그 폭주 시에는 각 폼의 [로그중지] 를 쓴다.
    // ─────────────────────────────────────────────────────────────────────────
    public class TaskHost
    {
        public readonly TaskKind Kind;
        public readonly string   Name;
        public readonly TabPage  Page;

        private Form   m_frm;
        private string m_strLastError = "";

        public TaskHost(TaskKind pKind, string pName, TabPage pPage)
        {
            Kind = pKind;
            Name = pName;
            Page = pPage;
        }

        public bool   IsRunning { get { return (m_frm != null && !m_frm.IsDisposed); } }
        public string LastError { get { return m_strLastError; } }
        public Form   HostedForm { get { return m_frm; } }

        // ─────────────────────────────────────────────────────────────────
        public bool Start()
        {
            if (IsRunning) return true;
            m_strLastError = "";

            try
            {
                Form frm;
                switch (Kind)
                {
                    case TaskKind.EQP:  frm = new WCS_TASK_CV.SYS_MAIN();  break;
                    case TaskKind.IO:   frm = new TSK_COMM_IOSCH.SYS_MAIN(); break;
                    case TaskKind.HOST: frm = new TSK_HostCom.frmMain();   break;
                    default: return false;
                }

                // 탭 안에 끼우기 위한 최소 변형 (폼 내부 로직은 건드리지 않는다)
                frm.TopLevel        = false;
                frm.FormBorderStyle = FormBorderStyle.None;
                frm.Dock            = DockStyle.Fill;
                frm.ControlBox      = false;

                // TabControl 은 선택되지 않은 TabPage 의 핸들을 만들지 않는다.
                //   -> 그대로 Show() 하면 폼 핸들이 안 생겨 Load 가 돌지 않고(=스레드가 안 뜨고)
                //      겉보기에만 "기동됨" 이 된다. 탭을 잠깐 선택해 핸들을 만든 뒤 되돌린다.
                TabControl tc = Page.Parent as TabControl;
                TabPage prevSel = (tc != null) ? tc.SelectedTab : null;
                if (tc != null) tc.SelectedTab = Page;

                Page.Controls.Add(frm);
                m_frm = frm;
                frm.Show();

                if (!frm.IsHandleCreated) { IntPtr hDummy = frm.Handle; }
                frm.CreateControl();        // 여기서 각 태스크의 Load 가 돌며 스레드가 뜬다
                Application.DoEvents();     // Load 안의 대화상자/화면갱신을 끝까지 태운다

                if (tc != null && prevSel != null && !prevSel.IsDisposed)
                    tc.SelectedTab = prevSel;

                return true;
            }
            catch (Exception ex)
            {
                m_strLastError = ex.Message;
                try { WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "ERROR",
                        "[" + Name + "] 기동 실패 : " + ex.ToString()); } catch { }
                CleanUp();
                return false;
            }
        }

        // ─────────────────────────────────────────────────────────────────
        // ─────────────────────────────────────────────────────────────────
        // 정지
        //   TaskStop() 안에서 통신 스레드가 끝나기를 Join 으로 기다리는데, 그 스레드는
        //   마지막 로그를 화면(ListView)에 직접 쓴다(세 태스크 모두 
        //   CheckForIllegalCrossThreadCalls=false 로 만들어져 있다).
        //   UI 스레드가 Join 으로 막혀 있으면 그 화면 쓰기가 반환되지 않아 서로 물린다
        //   -> 정지가 Join 타임아웃(수십 초)까지 걸리고 그동안 창 전체가 굳는다.
        //   그래서 정지 작업은 별도 스레드에 맡기고, UI 스레드는 메시지를 계속 돌린다.
        // ─────────────────────────────────────────────────────────────────
        public bool Stop()
        {
            if (!IsRunning) { CleanUp(); return true; }
            m_strLastError = "";

            Exception exWork = null;
            bool bDone = false;

            System.Threading.Thread thStop = new System.Threading.Thread(delegate()
            {
                try { StopWorker(); }
                catch (Exception ex) { exWork = ex; }
                finally { bDone = true; }
            });
            thStop.IsBackground = true;
            thStop.Name = "TaskStop:" + Name;
            thStop.Start();

            DateTime dtEnd = DateTime.Now.AddSeconds(60);
            while (!bDone && DateTime.Now < dtEnd)
            {
                Application.DoEvents();          // 워커의 화면 로그가 반환되도록 UI 를 계속 돌린다
                System.Threading.Thread.Sleep(20);
            }

            if (!bDone)
            {
                m_strLastError = "정지 60초 초과 - 스레드가 남아 있을 수 있습니다";
                try { WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "ERROR",
                        "[" + Name + "] " + m_strLastError); } catch { }
            }
            else if (exWork != null)
            {
                m_strLastError = exWork.Message;
                try { WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "ERROR",
                        "[" + Name + "] 정지 중 오류 : " + exWork.ToString()); } catch { }
            }

            CleanUp();
            return (bDone && exWork == null);
        }

        private void StopWorker()
        {
            switch (Kind)
            {
                case TaskKind.EQP:  ((WCS_TASK_CV.SYS_MAIN)m_frm).TaskStop();    break;
                case TaskKind.IO:   ((TSK_COMM_IOSCH.SYS_MAIN)m_frm).TaskStop(); break;
                case TaskKind.HOST: ((TSK_HostCom.frmMain)m_frm).TaskStop();     break;
            }
        }

        private void CleanUp()
        {
            try
            {
                if (m_frm != null)
                {
                    if (Page.Controls.Contains(m_frm)) Page.Controls.Remove(m_frm);
                    if (!m_frm.IsDisposed) m_frm.Dispose();
                }
            }
            catch { }
            m_frm = null;
        }

        // ─────────────────────────────────────────────────────────────────
        /// <summary>"스레드명|RUN|STOP" 목록. 정지 상태면 빈 배열.</summary>
        public string[] ThreadStates()
        {
            if (!IsRunning) return new string[0];
            try
            {
                switch (Kind)
                {
                    case TaskKind.EQP:  return ((WCS_TASK_CV.SYS_MAIN)m_frm).GetThreadStates();
                    case TaskKind.IO:   return ((TSK_COMM_IOSCH.SYS_MAIN)m_frm).GetThreadStates();
                    case TaskKind.HOST: return ((TSK_HostCom.frmMain)m_frm).GetThreadStates();
                }
            }
            catch { }
            return new string[0];
        }
    }
}
