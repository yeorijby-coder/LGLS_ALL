using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TSK_HostCom
{
	// [LGLS 2026-11-19] 종전에는 ECSCOMALog\Cli|Srv\yyyyMMdd.Log 에 파일로 남겼다.
	//   파일 로그는 이제 실행 폴더\LOG\MMdd.log 한 곳에만 남는다(WcsCommon.cTaskLog) -
	//   ShowMsgClient/ShowMsgServer 가 이미 그리로 보내므로 이 클래스는 아무것도 쓰지 않는다.
	class CLog
	{
		public CLog(string p_strSubDir)
		{
			// 폴더를 만들지 않는다
		}

		public bool WriteLog(ref LogMsgInfo p_MsgLog)
		{
			return true;
		}

		public bool DelLog()
		{
			return true;
		}
	}
}
