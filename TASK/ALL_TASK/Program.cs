using System;
using System.Windows.Forms;

namespace ALL_TASK
{
    static class Program
    {
        [STAThread]
        static void Main()
        {
            // [LGLS 2026-09-09] 세 태스크가 한 프로세스에 있으므로, 한 곳의 예외가
            //   전체를 끌고 내려가지 않도록 잡아서 로그로 남긴다.
            //   (스택 오버플로 같은 치명 오류는 CLR 이 잡을 수 없으니 원인 쪽에서 막아야 한다)
            Application.ThreadException += delegate(object s, System.Threading.ThreadExceptionEventArgs e)
            {
                try { WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "ERROR", "[UI 예외] " + e.Exception.ToString()); }
                catch { }
                MessageBox.Show(e.Exception.ToString(), "ALL_TASK - 처리되지 않은 오류",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
            };
            AppDomain.CurrentDomain.UnhandledException += delegate(object s, UnhandledExceptionEventArgs e)
            {
                try { WcsCommon.cTaskLog.Write("ALL", "ALL_TASK", "ERROR", "[치명 예외] " + e.ExceptionObject); }
                catch { }
                try { WcsCommon.cTaskLog.Shutdown(); } catch { }
            };
            Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new AllTaskMain());
        }
    }
}
