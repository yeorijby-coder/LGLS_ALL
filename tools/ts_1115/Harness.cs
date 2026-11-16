using System;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;
namespace TSK_HostCom
{
	static class modCmWork { public static void ShowMsgServer(string m, string k = "N") { Console.WriteLine("LOG " + k + " " + m); } }
	static class modDefApp { public const string MSG_IMP = "IMP", MSG_ERR = "ERR"; }
	static class Harness
	{
		// 사용: h.exe <포트> [self]   self 를 주면 이 프로세스가 그 포트에서 LISTEN 한다(= 구동 중인 HOST_TASK 흉내)
		[STAThread] static void Main(string[] a)
		{
			Application.EnableVisualStyles();
			TcpListener l = null;
			if (a.Length > 1 && a[1] == "self") { l = new TcpListener(IPAddress.Any, int.Parse(a[0])); l.Start(); }
			PortCleaner.RunUi(null, int.Parse(a[0]), delegate { Console.WriteLine("EXIT_CALLED"); });
			Console.WriteLine("RunUi 끝");
		}
	}
}
