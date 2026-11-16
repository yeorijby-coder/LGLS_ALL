using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Threading;

// 포트를 쥔 소켓 핸들을 모든 프로세스에서 찾는다. --close 를 주면 그 핸들을 닫는다(프로세스는 죽이지 않는다).
// 사용: HandleHunter.exe <포트> [--close]     (x64 로 빌드, 다른 사용자의 프로세스까지 보려면 관리자)
static class HandleHunter
{
	[DllImport("ntdll.dll")] static extern int NtQuerySystemInformation(int cls, IntPtr buf, int len, out int ret);
	[DllImport("kernel32.dll", SetLastError = true)] static extern IntPtr OpenProcess(uint access, bool inherit, int pid);
	[DllImport("kernel32.dll", SetLastError = true)] static extern bool DuplicateHandle(IntPtr srcProc, IntPtr srcHandle, IntPtr dstProc, out IntPtr dst, uint access, bool inherit, uint options);
	[DllImport("kernel32.dll", SetLastError = true)] static extern bool DuplicateHandle(IntPtr srcProc, IntPtr srcHandle, IntPtr dstProc, IntPtr dst, uint access, bool inherit, uint options);
	[DllImport("kernel32.dll")] static extern bool CloseHandle(IntPtr h);
	[DllImport("kernel32.dll")] static extern IntPtr GetCurrentProcess();
	[DllImport("ws2_32.dll")] static extern int WSAStartup(ushort ver, IntPtr data);
	[DllImport("ws2_32.dll")] static extern int getsockname(IntPtr s, byte[] name, ref int len);
	[DllImport("ws2_32.dll")] static extern int getpeername(IntPtr s, byte[] name, ref int len);
	[DllImport("ws2_32.dll")] static extern int getsockopt(IntPtr s, int level, int opt, out int val, ref int len);

	[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
	struct PROCESSENTRY32
	{
		public uint dwSize, cntUsage, th32ProcessID;
		public IntPtr th32DefaultHeapID;
		public uint th32ModuleID, cntThreads, th32ParentProcessID;
		public int pcPriClassBase;
		public uint dwFlags;
		[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)] public string szExeFile;
	}
	[DllImport("kernel32.dll", SetLastError = true)] static extern IntPtr CreateToolhelp32Snapshot(uint flags, uint pid);
	[DllImport("kernel32.dll", CharSet = CharSet.Unicode)] static extern bool Process32First(IntPtr h, ref PROCESSENTRY32 e);
	[DllImport("kernel32.dll", CharSet = CharSet.Unicode)] static extern bool Process32Next(IntPtr h, ref PROCESSENTRY32 e);

	// ownerPid 의 자손(자식, 손자 ...) PID 집합. 부모가 이미 죽어도 자식에는 부모 PID 가 남아 있다.
	static HashSet<int> Descendants(int ownerPid)
	{
		Dictionary<int, int> parent = new Dictionary<int, int>();
		IntPtr snap = CreateToolhelp32Snapshot(2, 0);
		PROCESSENTRY32 e = new PROCESSENTRY32();
		e.dwSize = (uint)Marshal.SizeOf(typeof(PROCESSENTRY32));
		if (Process32First(snap, ref e))
			do { parent[(int)e.th32ProcessID] = (int)e.th32ParentProcessID; } while (Process32Next(snap, ref e));
		CloseHandle(snap);

		HashSet<int> set = new HashSet<int>();
		bool grew = true;
		while (grew)
		{
			grew = false;
			foreach (KeyValuePair<int, int> kv in parent)
				if (!set.Contains(kv.Key) && (kv.Value == ownerPid || set.Contains(kv.Value))) { set.Add(kv.Key); grew = true; }
		}
		set.Remove(ownerPid);
		return set;
	}

	const uint PROCESS_DUP_HANDLE = 0x0040;
	const uint DUPLICATE_CLOSE_SOURCE = 1, DUPLICATE_SAME_ACCESS = 2;

	[StructLayout(LayoutKind.Sequential)]
	struct HandleEntryEx
	{
		public IntPtr Object;
		public IntPtr UniqueProcessId;
		public IntPtr HandleValue;
		public uint GrantedAccess;
		public ushort CreatorBackTraceIndex;
		public ushort ObjectTypeIndex;
		public uint HandleAttributes;
		public uint Reserved;
	}

	class ProbeResult { public bool Match, Listening; public string Local = "", Peer = "-"; }

	static void Probe(IntPtr dup, int port, ProbeResult r)
	{
		byte[] sa = new byte[128]; int len = sa.Length;
		if (getsockname(dup, sa, ref len) != 0) return;
		if (sa[0] != 2 || sa[1] != 0) return;						// AF_INET 만
		int lport = (sa[2] << 8) | sa[3];
		if (lport != port) return;
		r.Local = new IPAddress(new byte[] { sa[4], sa[5], sa[6], sa[7] }).ToString() + ":" + lport;
		int listen = 0, ol = 4;
		getsockopt(dup, 0xFFFF, 0x0002, out listen, ref ol);			// SOL_SOCKET, SO_ACCEPTCONN
		r.Listening = listen != 0;
		byte[] pa = new byte[128]; int pl = pa.Length;
		if (getpeername(dup, pa, ref pl) == 0 && pa[0] == 2)
			r.Peer = new IPAddress(new byte[] { pa[4], pa[5], pa[6], pa[7] }).ToString() + ":" + ((pa[2] << 8) | pa[3]);
		r.Match = true;
	}

	static int Main(string[] a)
	{
		if (a.Length < 1) { Console.WriteLine("사용: HandleHunter.exe <포트> [--owner <죽은PID>] [--close]"); return 2; }
		int port = int.Parse(a[0]);
		bool doClose = false;
		int ownerPid = 0;
		for (int ai = 1; ai < a.Length; ai++)
		{
			if (a[ai] == "--close") doClose = true;
			else if (a[ai] == "--owner" && ai + 1 < a.Length) ownerPid = int.Parse(a[++ai]);
		}
		HashSet<int> only = null;
		if (ownerPid > 0)
		{
			only = Descendants(ownerPid);
			Console.WriteLine("PID {0} 의 자손 프로세스 {1}개만 검사합니다: {2}", ownerPid, only.Count, string.Join(",", new List<int>(only).ConvertAll(x => x.ToString()).ToArray()));
		}
		if (IntPtr.Size != 8) { Console.WriteLine("x64 로 빌드해야 합니다."); return 2; }

		IntPtr wsa = Marshal.AllocHGlobal(1024);
		WSAStartup(0x0202, wsa);

		// 1) 시스템의 모든 핸들 표
		int size = 1 << 22, ret;
		IntPtr buf = Marshal.AllocHGlobal(size);
		int st;
		while ((st = NtQuerySystemInformation(0x40, buf, size, out ret)) == unchecked((int)0xC0000004))
		{
			Marshal.FreeHGlobal(buf); size = Math.Max(size * 2, ret + (1 << 20)); buf = Marshal.AllocHGlobal(size);
		}
		if (st != 0) { Console.WriteLine("NtQuerySystemInformation 실패 0x" + st.ToString("X")); return 1; }
		long n = Marshal.ReadInt64(buf);
		int esz = Marshal.SizeOf(typeof(HandleEntryEx));
		long baseAddr = buf.ToInt64() + 16;

		// 2) 소켓(AFD) 핸들의 개체 형식 번호를 알아낸다: 내 소켓 하나를 만들어 표에서 찾는다
		Socket probe = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
		int myPid = Process.GetCurrentProcess().Id;
		long probeVal = probe.Handle.ToInt64();
		// 표를 한 번 더 읽어 방금 만든 핸들이 들어 있게 한다
		Marshal.FreeHGlobal(buf); size = Math.Max(size, 1 << 22);
		buf = Marshal.AllocHGlobal(size);
		while ((st = NtQuerySystemInformation(0x40, buf, size, out ret)) == unchecked((int)0xC0000004))
		{
			Marshal.FreeHGlobal(buf); size = Math.Max(size * 2, ret + (1 << 20)); buf = Marshal.AllocHGlobal(size);
		}
		n = Marshal.ReadInt64(buf); baseAddr = buf.ToInt64() + 16;
		int typeIdx = -1;
		for (long i = 0; i < n; i++)
		{
			HandleEntryEx e = (HandleEntryEx)Marshal.PtrToStructure(new IntPtr(baseAddr + i * esz), typeof(HandleEntryEx));
			if (e.UniqueProcessId.ToInt64() == myPid && e.HandleValue.ToInt64() == probeVal) { typeIdx = e.ObjectTypeIndex; break; }
		}
		if (typeIdx < 0) { Console.WriteLine("소켓 핸들 형식 번호를 찾지 못했습니다."); return 1; }
		Console.WriteLine("전체 핸들 {0}개, 소켓 형식 번호 {1}, 포트 {2} 를 찾습니다{3}", n, typeIdx, port, doClose ? " (찾으면 닫음)" : "");

		// 3) 소켓 형식 핸들만 복제해서 포트를 본다
		Dictionary<int, IntPtr> procs = new Dictionary<int, IntPtr>();
		int found = 0, closed = 0, scanned = 0, hung = 0;
		for (long i = 0; i < n; i++)
		{
			HandleEntryEx e = (HandleEntryEx)Marshal.PtrToStructure(new IntPtr(baseAddr + i * esz), typeof(HandleEntryEx));
			if (e.ObjectTypeIndex != typeIdx) continue;
			int pid = (int)e.UniqueProcessId.ToInt64();
			if (pid == myPid || pid <= 4) continue;
			if (only != null && !only.Contains(pid)) continue;

			IntPtr hp;
			if (!procs.TryGetValue(pid, out hp))
			{
				hp = OpenProcess(PROCESS_DUP_HANDLE, false, pid);
				procs[pid] = hp;
			}
			if (hp == IntPtr.Zero) continue;

			if (Environment.GetEnvironmentVariable("HH_TRACE") != null) { Console.Error.WriteLine("dup pid=" + pid + " h=0x" + e.HandleValue.ToInt64().ToString("X") + " #" + i); }
			IntPtr dup;
			if (!DuplicateHandle(hp, e.HandleValue, GetCurrentProcess(), out dup, 0, false, DUPLICATE_SAME_ACCESS)) continue;
			scanned++;

			// 소켓이 아닌 File 핸들(동기 파이프 등)은 getsockname 이 영원히 멈출 수 있어, 별도 스레드에서 시간 제한을 두고 부른다
			ProbeResult pr = new ProbeResult();
			IntPtr dupCopy = dup;
			Thread th = new Thread(delegate() { try { Probe(dupCopy, port, pr); } catch { } });
			th.IsBackground = true;
			th.Start();
			if (!th.Join(250)) { hung++; continue; }				// 멈춘 핸들은 닫지 않고 버린다(도구가 끝나면 사라진다)

			CloseHandle(dup);
			if (!pr.Match) continue;

			string pname = "?";
			try { pname = Process.GetProcessById(pid).ProcessName; } catch { }
			found++;
			Console.WriteLine("  PID {0,-6} {1,-18} 핸들 0x{2:X}  {3,-22} 상대 {4,-22} {5}",
				pid, pname, e.HandleValue.ToInt64(), pr.Local, pr.Peer, pr.Listening ? "[LISTENING]" : "[접속/바인드]");

			if (doClose)
			{
				if (DuplicateHandle(hp, e.HandleValue, IntPtr.Zero, IntPtr.Zero, 0, false, DUPLICATE_CLOSE_SOURCE)) { closed++; Console.WriteLine("    -> 이 핸들을 닫았습니다"); }
				else Console.WriteLine("    -> 닫기 실패 (오류 " + Marshal.GetLastWin32Error() + ")");
			}
		}
		Console.WriteLine("검사한 File 형식 핸들 {0}개(멈춰서 건너뜀 {1}개), 포트 {2} 를 쥔 핸들 {3}개{4}", scanned, hung, port, found, doClose ? ", 닫음 " + closed + "개" : "");
		return 0;
	}
}
