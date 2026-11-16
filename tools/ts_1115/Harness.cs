using System;
using System.Windows.Forms;
namespace TSK_HostCom
{
	static class modCmWork { public static void ShowMsgServer(string m, string k = "N") { Console.WriteLine("LOG " + k + " " + m); } }
	static class modDefApp { public const string MSG_IMP = "IMP", MSG_ERR = "ERR"; }
	static class Harness
	{
		[STAThread] static void Main(string[] a)
		{
			Application.EnableVisualStyles();
			PortCleaner.RunUi(null, int.Parse(a[0]));
			Console.WriteLine("RunUi 끝");
		}
	}
}
