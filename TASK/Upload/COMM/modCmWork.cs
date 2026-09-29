using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace WmsUp
{
	//*** 일반적인 작업(DB와 관련없는) 정의 *********************************************************
	static class modCmWork
	{
		public static void ShowMsg(string strMsg, string strTitle, MessageBoxButtons ButtonType = MessageBoxButtons.OK, MessageBoxIcon IconType = MessageBoxIcon.Information, bool MessageBoxView = true)
		{
			if (MessageBoxView == true)
			{
				MessageBox.Show(strMsg, strTitle, ButtonType, IconType);
			}
		}


		public static void ShowError(string msg, string title, bool MessageBoxView = true)
		{
			// 단순히 msg로 메시지출력
			ShowMsg(msg, title, MessageBoxButtons.OK, MessageBoxIcon.Error, MessageBoxView);
		}

		public static void ShowError(Exception e, string msg, string title, bool MessageBoxView = true)
		{
			// msg 메시지는 ()을 처리하지 못했습니다. 와 같은 형식
			// e의 message를 위에 먼저 뿌리고 다음 라인에 msg뿌림
			ShowError(e.Message + modCom.CRLF + msg, title, MessageBoxView);
		}

		public static void ShowWarning(string msg, string title, bool MessageBoxView = true)
		{
			// 단순히 msg로 메시지출력
			ShowMsg(msg, title, MessageBoxButtons.OK, MessageBoxIcon.Warning, MessageBoxView);
		}

		public static void ShowWarning(Exception e, string msg, string title, bool MessageBoxView = true)
		{
			// msg 메시지는 ()을 처리하지 못했습니다. 와 같은 형식
			// e의 message를 위에 먼저 뿌리고 다음 라인에 msg뿌림
			ShowWarning(e.Message + modCom.CRLF + msg, title, MessageBoxView);
		}

		public static void ShowInfo(string msg)
		{
			ShowMsg(msg, "WMS", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		public static void ShowInfo(string msg, string title)
		{
			ShowMsg(msg, title, MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		public static void ShowInfo(string msg, string title, params object[] Args)
		{
			// !표시된 메시지
			string s = string.Format(msg, Args);
			ShowInfo(s, title);
		}

		// [LGLS 2026-09-30] 제목 띠에 조회 건수를 붙인다(종전에는 Spread 의 머리글 첫 줄에 썼다).
		public static void ShowMsgGrid(ref SliGrid ActiveGrid, string strMsg)
		{
			if (ActiveGrid == null) return;
			ActiveGrid.ShowMsg(strMsg);
		}


		public static void ThrowException(string msg, params object[] Args)
		{
			// msg로 에러발생
			throw new Exception(string.Format(msg, Args));
		}

		public static void ThrowException(string msg)
		{
			// msg로 에러발생
			throw new Exception(msg);

		}

		public static void ThrowException(Exception e, string msg, params object[] Args)
		{
			// 기존의 오류메시지 + 다음줄에 msg더해서 에러발생
			ThrowException(e.Message + modCom.CRLF + msg, Args);
		}
	}
}
