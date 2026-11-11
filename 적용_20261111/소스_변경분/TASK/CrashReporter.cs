using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace TSK_HostCom
{
	// ─────────────────────────────────────────────────────────────────────────
	// [LGLS 2026-11-11] 비정상 종료 기록 (MSJ 의 msjexhnd.cpp 와 같은 쓰임 - 죽기 직전의 상태를 남긴다)
	//
	//   현장 원격이 안 돼 HOST_TASK 가 왜 죽었는지 알 수 없었다(2026-11-11 아침). 그래서
	//     1) 처리되지 않은 예외(작업 스레드)   : 보고서(.txt) + 덤프(.dmp) 를 남기고 프로세스는 .NET 규칙대로 종료
	//     2) 화면 스레드 예외                   : 보고서만 남기고 ★계속 돈다★ (종전엔 "처리되지 않은 예외" 창이 떴다)
	//     3) SafeRun 이 삼킨 스레드 예외        : 보고서만 남긴다(이 스레드만 끝난 것 - 수신 접속 스레드가 이렇게 끝나면 통신이 멈춘다)
	//     4) 1분마다 자원(스레드·핸들·메모리·접속 수) 한 줄 + running.flag 갱신
	//        → 다음 기동 때 flag 가 남아 있으면 "직전 실행이 정상 종료가 아니었다" 와 마지막 한 줄을 보고서로 남긴다.
	//        (StackOverflow · 네이티브 크래시 · 강제 종료 · 정전은 .NET 이 알려 주지 않아 이 방법으로만 흔적이 남는다)
	//
	//   파일 위치(실행 폴더 기준)
	//     .\CrashReport\HOST_<시각>_<종류>.txt   보고서 (예외 종류/호출 경로, 자원, 접속 상태, 최근 로그 300줄, 모듈)
	//     .\CrashReport\HOST_<시각>_FATAL.dmp    덤프 (WinDbg / Visual Studio 로 연다)
	//     .\CrashReport\running.flag             정상 종료하면 지워진다
	//     .\ECSCOMALog\Health_yyyyMMdd.log       1분 자원 기록
	//
	//   EcsComA.ini [Crash]  (모두 선택, 기본값으로 동작)
	//     ENABLE=1    0 이면 이 기능 전체를 끈다(핸들러 미설치)
	//     DUMP=1      1 이면 치명 예외에 덤프를 남긴다
	//     DUMP_FULL=0 1 이면 전체 메모리 덤프(시험에서 약 210MB - 힙까지 모두 필요할 때만) / 0 이면 호출 경로가 보이는 작은 덤프(기본)
	//     KEEP=5      덤프 보관 개수 (보고서는 100개)
	//     HEALTH_SEC=60  자원 기록 간격(초), 0 이면 기록 안 함
	//
	//   ※ 이 기능은 통신 규약과 무관하다. 실패해도(디스크 가득 등) 조용히 넘기고 통신을 방해하지 않는다.
	//   ※ Windows 오류 보고(LocalDumps) 설정 파일(WER_LocalDumps_HOST_TASK.reg)을 함께 두었다 - 스택 오버플로 같은
	//      .NET 이 가로채지 못하는 치명 오류의 덤프는 그쪽이 남긴다(관리자 권한으로 한 번 등록).
	// ─────────────────────────────────────────────────────────────────────────
	static class CrashReporter
	{
		private static bool s_bInstalled;
		private static bool s_bMonitoring;
		private static bool s_bOwnsFlag;
		private static int  s_nInFatal;
		private static string s_strDir;
		private static string s_strIni;
		private static DateTime s_dtStart = DateTime.Now;
		private static DateTime s_dtLastUiDump = DateTime.MinValue;
		private static readonly Dictionary<string, DateTime> s_dicLastHandled = new Dictionary<string, DateTime>();
		private static int s_nHandledToday;
		private static DateTime s_dtHandledDay = DateTime.MinValue;
		private static string s_strLastHealth = "";

		// 최근 로그 줄 (화면/파일 로그에 남기는 것과 같은 내용 - ShowMsg 가 넣는다)
		private static readonly string[] s_ring = new string[300];
		private static int s_nRingPos;
		private static readonly object s_lockRing = new object();

		/// <summary>앱 상태를 한두 줄 문자열로 돌려주는 함수 (접속 수 · 상대 · 스레드 상태). frmMain 이 건다.</summary>
		public static Func<string> StateProvider;

		// ───────── 설정 ─────────
		private static int IniInt(string pKey, int pDef)
		{
			try { return modDefAPI.GetPrivateProfileInt("Crash", pKey, pDef, s_strIni); } catch { return pDef; }
		}

		// ───────── 설치 (Program.Main 에서 폼을 만들기 전에) ─────────
		public static void InstallHandlers()
		{
			try
			{
				s_strIni = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "EcsComA.ini");
				if (IniInt("ENABLE", 1) == 0) return;
				s_strDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "CrashReport");
				try { Directory.CreateDirectory(s_strDir); } catch { }

				// 화면 스레드 예외는 창을 띄우지 않고 보고서만 남긴 뒤 계속 돈다
				Application.SetUnhandledExceptionMode(UnhandledExceptionMode.CatchException);
				Application.ThreadException += delegate(object s, ThreadExceptionEventArgs e) { OnUiThreadException(e.Exception); };
				AppDomain.CurrentDomain.UnhandledException += delegate(object s, UnhandledExceptionEventArgs e) { OnFatal(e.ExceptionObject as Exception, e.IsTerminating); };
				s_bInstalled = true;
			}
			catch { }
		}

		// ───────── 시험용 (명령줄 /crashtest-thread · /crashtest-ui · /crashtest-handled) ─────────
		public static void TestHook(string[] pArgs)
		{
			try
			{
				foreach (string a in pArgs)
				{
					if (a == "/crashtest-thread")
					{
						Thread t = new Thread(delegate() { Thread.Sleep(8000); throw new InvalidOperationException("보고서 시험 - 작업 스레드 치명 예외"); });
						t.Name = "CrashTest"; t.Start();
					}
					else if (a == "/crashtest-ui")
					{
						// 화면 스레드의 실제 이벤트 경로(WM_TIMER)에서 예외를 낸다 - Application.Idle 에서 던진 예외는 ThreadException 으로 가지 않는다
						System.Windows.Forms.Timer tm = new System.Windows.Forms.Timer();
						tm.Interval = 6000;
						tm.Tick += delegate(object s, EventArgs e) { tm.Stop(); throw new InvalidOperationException("보고서 시험 - 화면 스레드 예외"); };
						tm.Start();
					}
					else if (a == "/crashtest-handled")
					{
						Thread t = new Thread(delegate() { modWorkThread.SafeRun("TEST_HANDLED", delegate() { Thread.Sleep(5000); throw new ArgumentException("보고서 시험 - SafeRun 이 삼키는 예외"); }); });
						t.Name = "CrashTestHandled"; t.Start();
					}
				}
			}
			catch { }
		}

		// ───────── 감시 시작 (PrevInstance 확인을 통과한 뒤) ─────────
		public static void StartMonitoring()
		{
			if (!s_bInstalled || s_bMonitoring) return;
			s_bMonitoring = true;
			try
			{
				string strFlag = Path.Combine(s_strDir, "running.flag");
				if (File.Exists(strFlag))
				{
					// 직전 실행이 정상 종료하지 않았다
					string strOld = "";
					try { strOld = File.ReadAllText(strFlag, Encoding.UTF8); } catch { }
					StringBuilder sb = new StringBuilder();
					sb.AppendLine("[직전 실행이 정상 종료가 아니었습니다]");
					sb.AppendLine("이번 기동 시각 : " + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
					sb.AppendLine("직전 실행의 마지막 자원 기록(1분 간격) :");
					sb.AppendLine(strOld);
					sb.AppendLine();
					sb.AppendLine("원인 단서 : 이 파일과 같은 시각 앞뒤의");
					sb.AppendLine("  - CrashReport\\ 의 다른 보고서/덤프 (처리되지 않은 예외였다면 거기에 호출 경로가 있다)");
					sb.AppendLine("  - Windows 이벤트 뷰어 > Windows 로그 > 응용 프로그램 (.NET Runtime / Application Error)");
					sb.AppendLine("  - ECSCOMALog\\Cli, Srv 의 마지막 줄 시각 / ECSCOMALog\\Health_*.log 의 스레드·핸들 추세");
					sb.AppendLine("  - 정전 · 재부팅 · 작업 관리자에서 끝냄 이었다면 위 단서가 없다");
					WriteFile("PrevAbnormalExit", sb.ToString());
				}
				File.WriteAllText(strFlag, "started " + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + " pid " + Process.GetCurrentProcess().Id, Encoding.UTF8);
				s_bOwnsFlag = true;

				int nSec = IniInt("HEALTH_SEC", 60);
				if (nSec > 0)
				{
					Thread th = new Thread(delegate() { HealthLoop(nSec); });
					th.IsBackground = true;
					th.Name = "CrashReporter Health";
					th.Start();
				}
			}
			catch { }
		}

		/// <summary>정상 종료 - running.flag 를 지운다 (Program.Main 이 Application.Run 뒤에 부른다)</summary>
		public static void MarkCleanExit()
		{
			try
			{
				if (!s_bOwnsFlag) return;
				string strFlag = Path.Combine(s_strDir, "running.flag");
				if (File.Exists(strFlag)) File.Delete(strFlag);
			}
			catch { }
		}

		// ───────── 로그 한 줄 (최근 300줄을 보고서에 싣는다) ─────────
		public static void AddLine(string pKind, string pMsg)
		{
			try
			{
				string s = DateTime.Now.ToString("HH:mm:ss.fff") + " " + pKind + " " + (pMsg ?? "");
				if (s.Length > 600) s = s.Substring(0, 600) + "...";
				lock (s_lockRing) { s_ring[s_nRingPos] = s; s_nRingPos = (s_nRingPos + 1) % s_ring.Length; }
			}
			catch { }
		}

		// ───────── 1분 자원 기록 ─────────
		private static void HealthLoop(int pSec)
		{
			while (true)
			{
				try
				{
					string line = HealthLine();
					s_strLastHealth = line;
					string strLog = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "ECSCOMALog");
					try { Directory.CreateDirectory(strLog); } catch { }
					File.AppendAllText(Path.Combine(strLog, "Health_" + DateTime.Now.ToString("yyyyMMdd") + ".log"), line + Environment.NewLine, Encoding.GetEncoding(949));
					File.WriteAllText(Path.Combine(s_strDir, "running.flag"), line, Encoding.UTF8);
				}
				catch { }
				Thread.Sleep(pSec * 1000);
			}
		}

		private static string HealthLine()
		{
			Process p = Process.GetCurrentProcess();
			p.Refresh();
			string st = "";
			try { if (StateProvider != null) st = StateProvider(); } catch { }
			return string.Format("{0:yyyy-MM-dd HH:mm:ss} threads={1} handles={2} private={3}MB working={4}MB gc={5}MB {6}",
				DateTime.Now, p.Threads.Count, p.HandleCount, p.PrivateMemorySize64 / 1048576, p.WorkingSet64 / 1048576,
				GC.GetTotalMemory(false) / 1048576, st);
		}

		// ───────── 예외 처리 ─────────
		private static void OnUiThreadException(Exception ex)
		{
			try
			{
				string kind = "UI";
				WriteReport(kind, ex, false, false);
				// 화면 스레드 예외가 반복되면 덤프는 1시간에 한 번만
				if (IniInt("DUMP", 1) != 0 && (DateTime.Now - s_dtLastUiDump).TotalMinutes >= 60)
				{
					s_dtLastUiDump = DateTime.Now;
					WriteDump("UI", IntPtr.Zero);
				}
			}
			catch { }
		}

		private static void OnFatal(Exception ex, bool pTerminating)
		{
			if (Interlocked.Exchange(ref s_nInFatal, 1) != 0) return;   // 보고 중에 또 나면 건너뛴다
			try
			{
				IntPtr pExp = IntPtr.Zero;
				try { pExp = Marshal.GetExceptionPointers(); } catch { }
				WriteReport("FATAL", ex, pTerminating, true);
				if (IniInt("DUMP", 1) != 0) WriteDump("FATAL", pExp);
			}
			catch { }
		}

		/// <summary>SafeRun 이 삼킨 스레드 예외를 남긴다(덤프 없음). 같은 곳에서 10초 안에 또 나면 건너뛰고, 하루 50건까지.</summary>
		public static void ReportHandled(string pWhere, Exception ex)
		{
			if (!s_bInstalled) return;
			try
			{
				lock (s_dicLastHandled)
				{
					if (s_dtHandledDay.Date != DateTime.Today) { s_dtHandledDay = DateTime.Today; s_nHandledToday = 0; }
					if (s_nHandledToday >= 50) return;
					DateTime last;
					if (s_dicLastHandled.TryGetValue(pWhere, out last) && (DateTime.Now - last).TotalSeconds < 10) return;
					s_dicLastHandled[pWhere] = DateTime.Now;
					s_nHandledToday++;
				}
				WriteReport("HANDLED_" + pWhere, ex, false, false);
			}
			catch { }
		}

		// ───────── 보고서 ─────────
		private static void WriteReport(string pKind, Exception ex, bool pTerminating, bool pFatal)
		{
			StringBuilder sb = new StringBuilder();
			sb.AppendLine("==== HOST_TASK 예외 보고서 [" + pKind + "] ====");
			sb.AppendLine("시각          : " + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
			sb.AppendLine("종류          : " + (pFatal ? "처리되지 않은 예외 - 프로세스가 종료됩니다" + (pTerminating ? "" : " (종료 안 함으로 표시됨)")
									   : pKind == "UI" ? "화면 스레드 예외 - 보고만 하고 계속 실행합니다"
									   : "작업 스레드 예외 - 그 스레드만 끝났습니다(프로세스는 계속)"));
			sb.AppendLine("스레드        : #" + Thread.CurrentThread.ManagedThreadId + " '" + Thread.CurrentThread.Name + "'");
			sb.AppendLine();
			sb.AppendLine("---- 예외 ----");
			int depth = 0;
			for (Exception e = ex; e != null && depth < 8; e = e.InnerException, depth++)
			{
				sb.AppendLine((depth == 0 ? "" : "  <- 내부 예외 " + depth + " : ") + e.GetType().FullName + " : " + e.Message);
				try
				{
					System.Net.Sockets.SocketException se = e as System.Net.Sockets.SocketException;
					if (se != null) sb.AppendLine("  SocketErrorCode=" + se.ErrorCode + " (" + se.SocketErrorCode + ")");
				}
				catch { }
				sb.AppendLine("  HResult=0x" + Marshal.GetHRForException(e).ToString("X8") + "  Source=" + e.Source + "  TargetSite=" + e.TargetSite);
				sb.AppendLine(e.StackTrace);
			}
			if (ex == null) sb.AppendLine("(예외 개체 없음 - 네이티브 예외일 수 있음)");
			sb.AppendLine();
			sb.AppendLine("---- 프로세스 ----");
			try
			{
				sb.AppendLine(HealthLine());
				Process p = Process.GetCurrentProcess();
				sb.AppendLine("PID=" + p.Id + "  기동 후 " + (DateTime.Now - s_dtStart).ToString() + "  Peak working=" + p.PeakWorkingSet64 / 1048576 + "MB  Peak virtual=" + p.PeakVirtualMemorySize64 / 1048576 + "MB  Virtual=" + p.VirtualMemorySize64 / 1048576 + "MB");
				sb.AppendLine("GC 횟수 gen0/1/2 = " + GC.CollectionCount(0) + "/" + GC.CollectionCount(1) + "/" + GC.CollectionCount(2));
				int wt, cpt; ThreadPool.GetAvailableThreads(out wt, out cpt);
				sb.AppendLine("ThreadPool 여유 worker=" + wt + " io=" + cpt);
				sb.AppendLine("마지막 1분 기록 : " + s_strLastHealth);
			}
			catch { }
			sb.AppendLine();
			sb.AppendLine("---- 시스템 ----");
			try
			{
				sb.AppendLine("OS=" + Environment.OSVersion + "  64bitOS=" + Environment.Is64BitOperatingSystem + "  64bit프로세스=" + Environment.Is64BitProcess + "  CLR=" + Environment.Version + "  CPU=" + Environment.ProcessorCount);
				sb.AppendLine("PC=" + Environment.MachineName + "  사용자=" + Environment.UserName + "  실행=" + System.Reflection.Assembly.GetExecutingAssembly().Location);
				sb.AppendLine("빌드 시각=" + File.GetLastWriteTime(System.Reflection.Assembly.GetExecutingAssembly().Location).ToString("yyyy-MM-dd HH:mm:ss"));
				DriveInfo di = new DriveInfo(Path.GetPathRoot(AppDomain.CurrentDomain.BaseDirectory));
				sb.AppendLine("디스크 " + di.Name + " 여유 " + di.AvailableFreeSpace / 1048576 + "MB");
			}
			catch { }
			sb.AppendLine();
			sb.AppendLine("---- 앱 상태 ----");
			try { sb.AppendLine(StateProvider != null ? StateProvider() : "(상태 함수 없음)"); } catch (Exception e2) { sb.AppendLine("(상태 조회 실패 " + e2.Message + ")"); }
			sb.AppendLine();
			sb.AppendLine("---- 최근 로그 (오래된 것 → 최신) ----");
			lock (s_lockRing)
			{
				for (int i = 0; i < s_ring.Length; i++)
				{
					string s = s_ring[(s_nRingPos + i) % s_ring.Length];
					if (s != null) sb.AppendLine(s);
				}
			}
			sb.AppendLine();
			sb.AppendLine("---- 이 스레드의 호출 경로 ----");
			try { sb.AppendLine(Environment.StackTrace); } catch { }
			sb.AppendLine();
			sb.AppendLine("---- 로드된 모듈 ----");
			try
			{
				foreach (ProcessModule m in Process.GetCurrentProcess().Modules)
				{
					string ver = ""; try { ver = m.FileVersionInfo.FileVersion; } catch { }
					sb.AppendLine(m.ModuleName + "  " + ver + "  0x" + m.BaseAddress.ToInt64().ToString("X") + "  " + m.FileName);
				}
			}
			catch { }
			WriteFile(pKind, sb.ToString());
		}

		private static string StampName(string pKind, string pExt)
		{
			string k = pKind;
			foreach (char c in Path.GetInvalidFileNameChars()) k = k.Replace(c, '_');
			return "HOST_" + DateTime.Now.ToString("yyyyMMdd_HHmmss_fff") + "_" + k + pExt;
		}

		private static void WriteFile(string pKind, string pText)
		{
			try
			{
				if (string.IsNullOrEmpty(s_strDir)) return;
				Directory.CreateDirectory(s_strDir);
				File.WriteAllText(Path.Combine(s_strDir, StampName(pKind, ".txt")), pText, Encoding.UTF8);
				Prune("*.txt", 100);
			}
			catch { }
		}

		// ───────── 덤프 (dbghelp MiniDumpWriteDump) ─────────
		[DllImport("dbghelp.dll", EntryPoint = "MiniDumpWriteDump", CallingConvention = CallingConvention.StdCall, SetLastError = true)]
		private static extern bool MiniDumpWriteDump(IntPtr hProcess, uint processId, IntPtr hFile, uint dumpType, IntPtr exceptionParam, IntPtr userStreamParam, IntPtr callbackParam);

		[StructLayout(LayoutKind.Sequential, Pack = 4)]
		private struct MINIDUMP_EXCEPTION_INFORMATION { public uint ThreadId; public IntPtr ExceptionPointers; public int ClientPointers; }

		[DllImport("kernel32.dll")] private static extern uint GetCurrentThreadId();

		private static void WriteDump(string pKind, IntPtr pExceptionPointers)
		{
			IntPtr pInfo = IntPtr.Zero;
			try
			{
				if (string.IsNullOrEmpty(s_strDir)) return;
				Directory.CreateDirectory(s_strDir);
				string path = Path.Combine(s_strDir, StampName(pKind, ".dmp"));
				// MiniDumpWithDataSegs 0x1 | WithHandleData 0x4 | WithUnloadedModules 0x20 | WithIndirectlyReferencedMemory 0x40 |
				//   WithProcessThreadData 0x100 | WithThreadInfo 0x1000, 전체 메모리 0x2
				uint type = 0x1 | 0x4 | 0x20 | 0x40 | 0x100 | 0x1000;
				if (IniInt("DUMP_FULL", 0) != 0) type |= 0x2;
				if (pExceptionPointers != IntPtr.Zero)
				{
					MINIDUMP_EXCEPTION_INFORMATION mei = new MINIDUMP_EXCEPTION_INFORMATION();
					mei.ThreadId = GetCurrentThreadId(); mei.ExceptionPointers = pExceptionPointers; mei.ClientPointers = 0;
					pInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(MINIDUMP_EXCEPTION_INFORMATION)));
					Marshal.StructureToPtr(mei, pInfo, false);
				}
				using (FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.None))
				{
					Process p = Process.GetCurrentProcess();
					MiniDumpWriteDump(p.Handle, (uint)p.Id, fs.SafeFileHandle.DangerousGetHandle(), type, pInfo, IntPtr.Zero, IntPtr.Zero);
				}
				Prune("*.dmp", Math.Max(1, IniInt("KEEP", 5)));
			}
			catch { }
			finally { if (pInfo != IntPtr.Zero) { try { Marshal.FreeHGlobal(pInfo); } catch { } } }
		}

		private static void Prune(string pPattern, int pKeep)
		{
			try
			{
				string[] files = Directory.GetFiles(s_strDir, pPattern);
				if (files.Length <= pKeep) return;
				Array.Sort(files, delegate(string a, string b) { return File.GetLastWriteTime(b).CompareTo(File.GetLastWriteTime(a)); });
				for (int i = pKeep; i < files.Length; i++) { try { File.Delete(files[i]); } catch { } }
			}
			catch { }
		}
	}
}
