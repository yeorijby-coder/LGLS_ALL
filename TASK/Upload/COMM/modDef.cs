using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WmsUp
{
	static class modDef
	{
		
		// 조회조건 중 '전체'
		public const string ALL_QRY = "전체";
		//**************************************************************************************
		// 처리 메세지
		public const string MSG_FORM_DISABEL = "Error로 인하여 현재 화면을 사용할 수 없습니다.";
		public const string MSG_SEL_ING = "조회중입니다. 잠시만 기다려주십시요.";
		public const string MSG_SEL_CPT = "조회가 완료 되었습니다.";
		
		public const string MSG_PROC_ERR = "Error가 발생하였습니다.";
		//콤보박스 바인딩 에러
		
		public const string MSG_PROC_ERR_CBO_WRHS = "창고구분을 알 수 없습니다.";
		//조회조건 기준 알림
		public const string MSG_INFO_ERR_TERM = "조회 기간은 30일을 초과할 수 없습니다.";
		
		public const string MSG_INFO_ERR_DATESEQ = "조회기간 시작일이 종료일 보다 늦을 수 없습니다.";
#if oracle
		public const string WMS_SF_GET_LUGG_JOB_STA_INIT = "dbo.WMS_SF_GET_LUGG_JOB_STA_INIT";
		public const string NVL = "NVL";
		public const string WMS_SF_DATE_FORMAT = "WMS_SF_DATE_FORMAT";
		public const string WMS_SF_TIME_FORMAT = "WMS_SF_TIME_FORMAT";
		public const string WMS_SF_DATETIME_FORMAT = "WMS_SF_DATETIME_FORMAT";
		public const string II = "||";
#elif mssql
		public const string WMS_SF_GET_LUGG_JOB_STA_INIT = "WMS_SF_GET_LUGG_JOB_STA_INIT";
		public const string NVL = "ISNULL";
		public const string WMS_SF_DATE_FORMAT = "dbo.wms_sf_Date_Format";
		public const string WMS_SF_TIME_FORMAT = "dbo.wms_sf_Time_Format";
		public const string WMS_SF_DATETIME_FORMAT = "dbo.wms_sf_dateTime_Format";
		public const string II = "+";
 #elif postgresql
		public const string WMS_SF_GET_LUGG_JOB_STA_INIT = "dbo.WMS_SF_GET_LUGG_JOB_STA_INIT";
		public const string NVL = "COALESCE";
		public const string WMS_SF_DATE_FORMAT = "WMS_SF_DATE_FORMAT";
		public const string WMS_SF_TIME_FORMAT = "WMS_SF_TIME_FORMAT";
		public const string WMS_SF_DATETIME_FORMAT = "WMS_SF_DATETIME_FORMAT";
		public const string II = "||";
#endif

        public static string f_MESSAGE00(int no)
		{
			string lx_st_message = "";

			switch (no)
			{
				case 1:
					lx_st_message = "조회중입니다. 잠시만 기다려주십시요.";
					break;
				case 2:
					lx_st_message = "조회가 정상적으로 완료되었습니다.";
					break;
				case 3:
					lx_st_message = "현재 판정GRADE가 없습니다. 다시 확인하십시요.";
					break;
				case 4:
					lx_st_message = "'포장대기'상태가 아닙니다. 다시 확인하십시요.";
					break;
				case 5:
					lx_st_message = "'포장중' 상태입니다. 실행할 수 없습니다.";
					break;
				case 6:
					lx_st_message = "정상적으로 처리가 완료되었습니다. ";
					break;
				case 7:
					lx_st_message = "현재 SAP의 상태가 정상이므로 등록하실 수 없습니다.";
					break;
				case 8:
					lx_st_message = "등록할 생산SILO를 선택하여 주십시요.";
					break;
				case 9:
					lx_st_message = "생산GRADE를 입력하여 주십시요.";
					break;
				case 10:
					lx_st_message = "판정GRADE를 입력하여 주십시요.";
					break;
				case 11:
					lx_st_message = "LOT를 입력하여 주십시요.";
					break;
				case 12:
					lx_st_message = "생산량을 입력하여 주십시요.";
					break;
				case 13:
					lx_st_message = "SILO등록이 정상적으로 완료되었습니다.";
					break;
				case 14:
					lx_st_message = "처리하실 포장SILO를 먼저 선택하여 주십시요.";
					break;
				case 15:
					lx_st_message = "현재 '포장중'상태가 아닙니다. 실행할 수 없습니다.";
					break;
				case 16:
					lx_st_message = "처리중 입니다. 잠시만 기다려주십시요.";
					break;
				case 17:
					lx_st_message = "초기화면 설정중입니다. 잠시만 기다려주십시요.";
					break;
				case 18:
					lx_st_message = "전송하실 입고LINE을 먼저 선택하여 주십시요. ";
					break;
				case 19:
					lx_st_message = "등록된 GRADE가 아닙니다. 실행하시겠습니까? ";
					break;
				case 20:
					lx_st_message = "SAP의 포장계획정보를 조회중입니다.";
					break;
				case 21:
					lx_st_message = "등록된 GRADE가 아닙니다. 다시 확인하십시요.";
					break;
				case 22:
					lx_st_message = "등록된 LOT가 아닙니다. 다시 확인하십시요.";
					break;
				case 23:
					lx_st_message = "현재 '입고중'상태입니다. 실행할 수 없습니다.";
					break;
				case 24:
					lx_st_message = "GRADE를 입력하여 주십시요.";
					break;
				case 25:
					lx_st_message = "현재 '인쇄전송'상태입니다. 실행할 수 없습니다. ";
					break;
				case 30:
					lx_st_message = "현재 '포장완료'상태입니다. 실행할 수 없습니다. ";
					break;
			}

			return lx_st_message;
		}

		public static string f_MESSAGE01(int no)
		{
			string lx_st_message = "";

			switch (no)
			{
				case 1:
					lx_st_message = "처리도중에 에러가 발생했습니다.";
					break;
				case 2:
					lx_st_message = "입고대 초기화가 정상적으로 처리되었습니다.";
					break;
				case 3:
					lx_st_message = "화면을 재설정합니다. 잠시만 기다려주십시요.";
					break;
				case 4:
					lx_st_message = "현재 '입고중'상태가 아닙니다. 다시 확인하십시요.";
					break;
				case 5:
					lx_st_message = "현재 NEW-CIE의 상태가 비정상이라서 포장계획정보를 조회할 수 없습니다.";
					break;
				case 6:
					lx_st_message = "현재 인쇄전송할 수 없는 상태입니다. 다시 확인하십시요.";
					break;
				case 7:
					lx_st_message = "입고지시처리가 되었거나 입고대에 물류가 없습니다.";
					break;
				case 8:
					lx_st_message = "연결할 수 없는 입고대입니다. 다시 확인하십시요.";
					break;
				case 9:
					lx_st_message = "인쇄전송이 완료된 상태입니다. 포장중지를 먼저 하십시요.";
					break;
				case 10:
					lx_st_message = "'포장완료' 실행은 '포장중' 혹은 '포장중지'상태에서만 가능합니다.";
					break;
				case 11:
					lx_st_message = "입고대 초기화 실행이 실패하였습니다.";
					break;
				case 12:
					lx_st_message = "자동입고실행정보가 없습니다.";
					break;
				case 13:
					lx_st_message = "인쇄전송를 먼저 실행 하십시요.";
					break;
				case 14:
					lx_st_message = "재포장설정은 '입고완료'상태에서만 가능합니다" + modCom.CRLF + modCom.CRLF + "'입고완료'처리후 실행하십시요.";
					break;
				case 15:
					lx_st_message = "자동입고실행을 할 수 없는 상태입니다" + modCom.CRLF + modCom.CRLF + " 입고LINE 설정을 먼저 실행하십시요.";
					break;
				case 16:
					lx_st_message = "지정입고모드 상태라서 실행할 수 없습니다.";
					break;
				case 17:
					lx_st_message = "자동입고모드 상태라서 실행할 수 없습니다.";
					break;
				case 18:
					lx_st_message = "'입고중' 상태라서 실행할 수 없습니다.";
					break;
				case 19:
					lx_st_message = "BY-PASS입고 처리중이라 포장중지할 수 없습니다.";
					break;
				case 20:
					lx_st_message = "자동창고서버가 비정상입니다." + modCom.CRLF + modCom.CRLF + "정보시스템실로 문의바랍니다.";
					break;
			}

			return lx_st_message;
		}

		public static string f_MESSAGE02(int no)
		{
			string lx_st_message = "";

			switch (no)
			{
				case 1:
					lx_st_message = "입고 금지/해제 대상을 먼저 조회하세요";
					break;
				case 2:
					lx_st_message = "금지/해제 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 3:
					lx_st_message = "창고 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 4:
					lx_st_message = "BANK 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 5:
					lx_st_message = "FROM BAY 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 6:
					lx_st_message = "FROM LEVEL 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 7:
					lx_st_message = "TO BAY 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 8:
					lx_st_message = "TO LEVEL 조회 조건이 변경되었습니다. 다시 확인 하세요!";
					break;
				case 9:
					lx_st_message = "입고금지 사유를 입력하세요!";
					break;
				case 10:
					lx_st_message = "CELL을 입고금지로 지정하시겠습니까? ";
					break;
				case 11:
					lx_st_message = "입고금지 CELL을 해제 하시겠습니까? ";
					break;
				case 12:
					lx_st_message = "처리중 입니다. 잠시만 기다려주십시요.";
					break;
				case 13:
					lx_st_message = "정상적으로 저장이 완료되었습니다.";
					break;
				case 14:
					lx_st_message = "정상적으로 저장이 완료되지않았습니다.";
					break;
			}

			return lx_st_message;
		}
	}
}
