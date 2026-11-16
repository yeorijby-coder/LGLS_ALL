using System;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Threading;

// 재현: 리스너를 열고 자식 프로세스를 띄운 뒤(핸들 상속) 부모가 죽는다. 자식이 리스너를 쥔 채 남는다.
static class Repro
{
	static void Main(string[] a)
	{
		int port = int.Parse(a[0]);
		TcpListener l = new TcpListener(IPAddress.Any, port);
		l.Start();
		Process.Start(new ProcessStartInfo("cmd.exe", "/c ping -n 1500 127.0.0.1 > nul") { UseShellExecute = false, CreateNoWindow = true });
		Thread.Sleep(1500);
		Environment.Exit(0);		// 부모(소유 PID)가 죽는다
	}
}
