using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace TSK_HostCom
{
	static class Program
	{
		/// <summary>
		/// 해당 응용 프로그램의 주 진입점입니다.
		/// </summary>
		[STAThread]
		static void Main()
		{
			// [LGLS 2026-11-11] 예외 보고서/덤프 핸들러는 폼을 만들기 전에 건다(화면 스레드 예외 모드가 그래야 적용된다)
			CrashReporter.InstallHandlers();
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			// [LGLS 2026-11-11] 보고서 시험용 (현장에서 쓰지 않는다) : /crashtest-thread = 8초 뒤 작업 스레드 치명 예외,
			//   /crashtest-ui = 시작 직후 화면 스레드 예외, /crashtest-handled = SafeRun 이 삼키는 스레드 예외
			CrashReporter.TestHook(Environment.GetCommandLineArgs());
			Application.Run(new frmMain());
			CrashReporter.MarkCleanExit();   // 정상 종료면 running.flag 를 지운다
		}
	}
}
