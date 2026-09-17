using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using Microsoft.VisualBasic;
using Npgsql;

namespace TSK_HostCom
{
    class CSrvWork
	{
		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//화면개요   : ECS 서버 소켓용 작업 클래스
		//수정이력   : 01. 20161122 이길문 작업취소정보 수신 주석해제
		//             02. 20161122 이길문 WMS에서 먼저 작업취소처리 했을 경우 체크 추가
		//             03. 20161122 이길문 이중입고일 경우 로케이션 재지정
		//             04. 20161228 이길문 ECS상태정보수신작업대Get
		//             05. 20170105 이길문 설비지시일시추가
		//             06. 20170105 이길문 설비완료일시추가
		//             07. 20170105 이길문 응답메세지총길이변경:27->47
		//             08. 20170105 이길문 응답메세지 바코드정보추가
		//             09. 20170105 이길문 응답메세지 작업차수추가
		//             10. 20170105 이길문 작업완료보고수신 응답메세지 작업차수추가(strStep)
		//             11. 20170105 이길문 바코드보고수신 응답메세지 바코드추가(strReadingData)

		public Socket m_sktSock;//소켓
		public bool m_blSockConnected;//소켓접속여부
		public System.Threading.Thread m_thrThreadObj;//쓰레드 객체	
		
		public bool m_blDbConnted;//DB 연결 유무
		public CUserDb m_BDb = new CUserDb("Multi", false);//쓰레드별 Connection 별도
		
		private byte[] m_bytRxHead = new byte[modDefApp.MSG_HEAD_CNT + 1];	//Header
		private byte[] m_bytRxBuff = new byte[1025];//프로젝트 별로 최대 허용되는 m_bytRxBuff 설정	
		private byte[] m_bytTxBuff;//클라이언트로 보낼 메세지	
		
		private string m_strLog = "";	//로그
		private string m_strSql;	//SQL 문장
		private int m_iSelCnt;	//Return record cnt

		private string m_strMsgType;	//메세지 구분

        //Command
        private string m_strHostCmd;
        //Direction
        // [LGLS 2026-08-23] 이 클래스는 WMS 요청을 '받는' 쪽이고, 여기서 내보내는 것은 그 응답이다.
        //   즉 SendSock 이 기록하는 방향은 ECS→WMS(E2W) 다. 종전 W2E 는 수신 방향을 그대로 쓴 것이라
        //   HOST 로그에서 응답 전문(ECS_MBX ...)까지 W2E 로 남아 송수신자 조회가 뒤섞였다.
        //   (수신 전문 자체는 Parsing 에서 "W2E" 로 따로 기록한다)
        private string m_strDirection = "E2W";

		public System.DateTime m_tmSCMD_RecvTime;
				
		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 서버클래스 생성시 초기값 할당
		public CSrvWork()
		{
			m_blSockConnected = false;
			m_blDbConnted = false;
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: ECS수신정보Read
		public bool ReadRequest(ref int p_iBodyLen)
		{
			string strLog = null;
			int iRxCnt = 1;
			//return false:Close socket, true:Continue

			ClearBuff(ref m_bytRxHead);
			// Close시 Return 0 or Exception 발생, Receive전 Close되었느냐 Receive대기상태일 때 Close되었는냐 등에 따라 다름
			iRxCnt = m_sktSock.Receive(m_bytRxHead, modDefApp.MSG_HEAD_CNT, SocketFlags.None);

            if (iRxCnt <= 0)
                return false;

            // Header 체크

            if (!CheckHeader(iRxCnt, ref p_iBodyLen))
			{
				// 잘못된 헤더 로그
				strLog = System.Text.Encoding.UTF8.GetString(m_bytRxHead);
				modCmWork.ShowMsgServer(strLog, modDefApp.MSG_ERR);

				// Nak Response
				SendSock();
				return false;
			}

            // Body 체크

            iRxCnt = m_sktSock.Available;
			iRxCnt = m_sktSock.Receive(m_bytRxBuff,modDefApp.MSG_HEAD_CNT, p_iBodyLen, SocketFlags.None);
			if (!CheckBody(p_iBodyLen, iRxCnt))
			{
				// 잘못된 바디 로그
				strLog = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, 0, modDefApp.MSG_HEAD_CNT + p_iBodyLen);
				modCmWork.ShowMsgServer(strLog, modDefApp.MSG_ERR);

				// Nak Response
				SendSock();
				return false;
			}

			return true;
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 해더메세지 길이체크
		private bool CheckHeader(int p_iRxCnt, ref int p_iBodyLen)
		{
			string strTemp = null;

			m_strMsgType = " ";
			try
			{
				if (p_iRxCnt != modDefApp.MSG_HEAD_CNT)
				{
					ClearSock();
					MakeResponse("X", "", modDefApp.MSG_INVALID_HEADER_LENG);

					m_strLog = string.Format("정해진 헤더의 길이가 아닙니다.[Leng={0}]", p_iRxCnt);
					modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
					return false;
				}

				// MailBox명 체크 (HOST -> WCS 는 'WMS_MBX')
				strTemp = System.Text.Encoding.UTF8.GetString(m_bytRxHead, 0, 10).TrimEnd();
				if (strTemp != modDefApp.MSG_MAILBOX_RECV)
				{
					ClearSock();
					MakeResponse("X", "", modDefApp.MSG_INVALID_MAILBOX_NAME);

					m_strLog = string.Format("MailBox명이 올바르지 않습니다.[{0}]", strTemp);
					modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
					return false;
				}

				strTemp = System.Text.Encoding.UTF8.GetString(m_bytRxHead, 10, 4);

				p_iBodyLen = Convert.ToInt32(strTemp);
				if (p_iBodyLen < 3)
				{
					ClearSock();
					MakeResponse("X", "", modDefApp.MSG_INVALID_HEADER_LENG);

					m_strLog = string.Format("Body의 길이가 '3' 이하 입니다.[Leng={0}]", p_iBodyLen);
					modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
					return false;
				}
			}
			catch (Exception ex)
			{
				ClearSock();
				MakeResponse("X", "", modDefApp.MSG_INVALID_HEADER_LENG);

				m_strLog = "헤더의 정보가 틀립니다.";
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
				return false;
			}

			ClearBuff(ref m_bytRxBuff);
			m_bytRxHead.CopyTo(m_bytRxBuff, 0);

			return true;

		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 소켓 클리어
		private void ClearSock()
		{
			byte[] bytTempByte = null;
			int i = 0;

			try
			{
				// data가 있으면 클리어
				System.Threading.Thread.Sleep(1000);
				i = m_sktSock.Available;
				if (i > 0)
				{
					bytTempByte = new byte[i + 1];
					m_sktSock.Receive(bytTempByte, i, SocketFlags.None);
				}
			}
			catch (Exception ex)
			{
				m_strLog = "소켓 클리어 실패.";
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
				throw ex;
			}

		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 소켓 클리어


		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 메세지 Body체크
		private bool CheckBody(int p_iBodyLen, int p_iRxCnt)
		{
			// 3회 반복
			int i = 0;
			int iRemain = 0;

			iRemain = p_iBodyLen - p_iRxCnt;
			for (i = 0; i <= 1; i++)
			{
				if (iRemain == 0)
				{
					break; // TODO: might not be correct. Was : Exit For
				}
				//Debug
				//Console.WriteLine("Read fail. 회수[{0}]:ReadCnt[{1}]", i, nRxCnt)
				System.Threading.Thread.Sleep(500);
				p_iRxCnt = m_sktSock.Receive(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + p_iBodyLen - iRemain, iRemain, SocketFlags.None);
				iRemain -= p_iRxCnt;
			}

			if (iRemain != 0)
			{
				ClearSock();
				MakeResponse("X", "", modDefApp.MSG_INVALID_MSG_LENG);

				m_strLog = string.Format("정해진 메세지 길이만큼 읽지 못했습니다..[Leng={0}]", p_iBodyLen - iRemain);
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
				return false;
			}

			if (m_bytRxBuff[modDefApp.MSG_HEAD_CNT] != modDefApp.STX)
			{
				ClearSock();
				MakeResponse("X", "", modDefApp.MSG_NO_STX);

				m_strLog = "메세지의 시작이 'STX'가 아닙니다.";
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
				return false;
			}

			if (m_bytRxBuff[modDefApp.MSG_HEAD_CNT + p_iBodyLen - 1] != modDefApp.ETX)
			{
				ClearSock();
				MakeResponse("X", "", modDefApp.MSG_NO_ETX);

				m_strLog = "메세지의 끝이 ETX가 아닙니다.";
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
				return false;
			}

			return true;
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 응답메세지(두 작업번호일 경우)
		private void MakeResponse(string p_strMsgType, string p_LuggNo, byte p_bytReasonCode1)
		{
			string strTemp = null;
			byte[] bytTempByte = null;
			int iTxCnt = 0;
			string strAckNak1 = null;
			int iRspsLen = 0;

			// [LGLS 2026-07-30] 인터페이스명세서(20100311) 응답 규격 :
			//   STX + Type소문자(1) + ACK/NAK(1) + Reason(2) + LuggNo(4) + StepCount(1) + ETX = 11
			//   (구 13바이트 DeviceNo(3) 규격은 원본 ECP/명세와 불일치라 폐기. StepCount는 F 응답 외 '0')
			iRspsLen = modDefApp.MSG_RSPS_BODY_CNT;
			iTxCnt = modDefApp.MSG_HEAD_CNT + iRspsLen;

			m_bytTxBuff = new byte[iTxCnt];

			//### Header ###
			strTemp = string.Format("{0,-10}{1:0000} ", modDefApp.MSG_MAILBOX_SEND, iRspsLen);
			bytTempByte = System.Text.Encoding.UTF8.GetBytes(strTemp);

			Array.Copy(bytTempByte, 0, m_bytTxBuff, 0, modDefApp.MSG_HEAD_CNT);

			//### Body ###
			if (p_bytReasonCode1 == modDefApp.MSG_NO_ERROR)
			{
				strAckNak1 = modDefApp.MSG_ACK;
			}
			else
			{
				strAckNak1 = modDefApp.MSG_NAK;
			}

			m_bytTxBuff[modDefApp.MSG_HEAD_CNT] = modDefApp.STX;

			strTemp = string.Format("{0,1}{1,1}{2:00}", p_strMsgType.ToLower(), strAckNak1, p_bytReasonCode1); // ACK/NAK #1, Reason Code #1
			strTemp += p_LuggNo.PadLeft(4, char.Parse("0")); //Luggage No #1.
			strTemp += "0"; //Step Count(1) - 작업완료 차수(F 전용), 그 외 항상 0

			bytTempByte = System.Text.Encoding.UTF8.GetBytes(strTemp);
			Array.Copy(bytTempByte, 0, m_bytTxBuff, modDefApp.MSG_HEAD_CNT + 1, iTxCnt - modDefApp.MSG_HEAD_CNT - 2);

			m_bytTxBuff[iTxCnt - 1] = modDefApp.ETX;

		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 소켓전송
		public void SendSock()
		{

            m_sktSock.Send(m_bytTxBuff, SocketFlags.None);
            m_strLog = System.Text.Encoding.UTF8.GetString(m_bytTxBuff);
            modCmWork.ShowMsgServer(m_strLog);

            bool bResult = modDefApp.g_frmForm.InsertHostIfLog(m_BDb, m_strLog, m_strHostCmd, m_strDirection);     // 함수안에서 Transaction 처리함!

        }

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: Buff Clear
		private void ClearBuff(ref byte[] p_bytBuff)
		{
			Array.Clear(p_bytBuff, 0, p_bytBuff.Length);
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 메세지 분석 후 DB 작업
		public bool Parsing(int p_iBodyCnt)
		{

			m_strLog = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, 0, modDefApp.MSG_HEAD_CNT + p_iBodyCnt);
			modCmWork.ShowMsgServer(m_strLog);
			m_strHostCmd = m_strMsgType = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 1, 1);

            // IMS 메시지 LOG 추가
            modDefApp.g_frmForm.InsertHostIfLog(m_BDb, m_strLog, m_strMsgType, "W2E");

            switch (m_strMsgType)
            {
                case "O":
                case "R":
                    // 작업지시 메세지 
                    ParseOorR(p_iBodyCnt);
                    break;
                case "D":
                    // 작업삭제 메세지
                    ParseCancel(p_iBodyCnt);
                    break;
                case "M":
                    // [LGLS] 모드변경 (C/V#11 RGV 방향, ECS 원본 checkMode 대응)
                    ParseModeChange(p_iBodyCnt);
                    break;
                default:
					m_strLog = string.Format("알수 없는 MsgType입니다.[{0}]", m_strMsgType);
					modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);
                    m_strHostCmd = "X";
					MakeResponse("X", "", modDefApp.MSG_UNKNOWN_MSG_TYPE);
					break;
			}
            DataTable_Dispose();

			return true;

		}
        
        //최초작성자	: BASE(이길문)
        //작성일		: 20160829
        //설명		: 데이타테이블 초기화
        private void DataTable_Dispose()
		{
            m_BDb.dtMain.Dispose();
            m_BDb.dtMain.Reset();
        }

        #region [LGLS] 스테이션 존재 검증 (901~905=크레인 SC_DATA_LGLS, 그 외=작업대 CV_DATA)
        /// <summary>
        /// LGLS 스테이션 검증: ECS 원본의 TB_CODEMASTER '022' 개념을 대체 DB로 매핑.
        ///   9xx  → SC_DATA_LGLS.SC_NO 존재 확인 (하이랙 코드 201~205 는 ParseOorR 에서 90x 로 변환됨)
        ///   그 외 → CV_DATA.MC_NO 존재 확인 (101~105 = C/V#11~#15 작업대)
        /// (기존 IsValidStation/IsValidLocation 은 LFC 사이트의 STN_KIND enum/CELL_MST 체계
        ///  기준이라 LGLS 데이터(비트마스크 STN_KIND, CELL_MST 부재)와 맞지 않음)
        /// </summary>
        private bool IsValidStationLgls(string strPos)
        {
            int nPos;
            if (!int.TryParse(strPos, out nPos)) return false;

            m_BDb.ParamsClear();
            if (nPos >= 900)
            {
                m_strSql = "SELECT * FROM SC_DATA_LGLS";
                m_strSql += modDefApp.CRLF + "WHERE WH_TYP = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                m_strSql += modDefApp.CRLF + "  AND SC_NO  = " + m_BDb.ParamsAdd("SC_NO", strPos);
            }
            else
            {
                m_strSql = "SELECT * FROM CV_DATA";
                m_strSql += modDefApp.CRLF + "WHERE WH_TYP = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                m_strSql += modDefApp.CRLF + "  AND MC_NO  = " + m_BDb.ParamsAdd("MC_NO", strPos);
            }
            return m_BDb.ExcuteQry_Par(ref m_strSql) > 0;
        }
        #endregion

        #region 작업대에 대한 Validation
        private bool IsValidStation(int nJobPattern, string strStation, bool bStart, ref string strCV_PLT_JOB_TYP)
        {
            m_BDb.ParamsClear();

            m_strSql = "SELECT * FROM CV_DATA";
            m_strSql += modDefApp.CRLF + "WHERE MC_NO = " + m_BDb.ParamsAdd("MC_NO", strStation);
            m_strSql += modDefApp.CRLF + "  AND WH_TYP = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);

            m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);

            if (m_iSelCnt <= 0)
            {
                return false;
            }

            for (int ii = 0; ii < m_iSelCnt; ii++)
            {
                string strKIND = "" + m_BDb.dtMain.Rows[ii]["STN_KIND"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[ii]["STN_KIND"].ToString();
                string strID = "" + m_BDb.dtMain.Rows[ii]["MC_NO"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[ii]["MC_NO"].ToString();

                int nKind = Convert.ToInt32(strKIND);

                 bool bValid = false;
                if (bStart == true)
                {
                    if (IsValidStartStation(nKind, strID, nJobPattern, strStation, ref bValid) == false)
                    {
                        continue;
                    }
                    else 
                    {
                        strCV_PLT_JOB_TYP = "" + m_BDb.dtMain.Rows[ii]["SC_PLT_JOB_TYP"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[ii]["SC_PLT_JOB_TYP"].ToString();
                        return bValid;
                    }
                }
                else
                {
                    //return IsValidDestStation(nKind, strID, nJobPattern);                   
                    if (IsValidDestStation(nKind, strID, nJobPattern, strStation, ref bValid) == false)
                    {
                        continue;
                    }
                    else
                    {
                        strCV_PLT_JOB_TYP = "" + m_BDb.dtMain.Rows[ii]["SC_PLT_JOB_TYP"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[ii]["SC_PLT_JOB_TYP"].ToString();
                        return bValid;
                    }
                }
            }

            return false;
        }
        #endregion
        #region 출발 작업대에 대한 Validation
        private bool IsValidStartStation(int nKIND, string strID, int nJobPattern, string strStationID, ref bool bValid)
        {
            bValid = false;
		    switch (nJobPattern)
		    {
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternSto:
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternMove:
			    if ((nKIND != (int)modDefApp.EN_STN_KIND.enStoStation) &&
				    (nKIND != (int)modDefApp.EN_STN_KIND.enArvStation))
                {
				    return false;
                }
			    break;

		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternRet:
    //		case (int)modDefApp.EN_JOB_PATTERN.enJobPatternPR:
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternW2W:
			    if (nKIND != (int)modDefApp.EN_STN_KIND.enScStation)
                {
				    return false;
                }
			    break;
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternR2R:
                bValid = true;
			    return true;
		    default:
			    //DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
				return false;
		    }

		    if (strID == strStationID)
            {
                bValid = true;
			    return true;
            }

            return true;
        }
        #endregion
        #region 도착 작업대에 대한 Validation
        private bool IsValidDestStation(int nKIND, string strID, int nJobPattern, string strStationID, ref bool bValid)
        {
            bValid = false;

		    switch (nJobPattern)
		    {
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternSto:
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternW2W:
		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternR2R:
                if (nKIND != (int)modDefApp.EN_STN_KIND.enScStation)
                {
                    //continue;
                    return false;
                }
			    break;

		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternRet:
    //		case (int)modDefApp.EN_JOB_PATTERN.enJobPatternPR:
                if ((nKIND != (int)modDefApp.EN_STN_KIND.enArvStation) &&
                    (nKIND != (int)modDefApp.EN_STN_KIND.enRetStation))
                {
                    //continue;
                    return false;
                }
			    break;

		    case (int)modDefApp.EN_JOB_PATTERN.enJobPatternMove:
                if ((nKIND != (int)modDefApp.EN_STN_KIND.enArvStation) &&
                    (nKIND != (int)modDefApp.EN_STN_KIND.enRetStation))
                {
                    //continue;
                    return false;
                }

			    break;
	
		    default:
                //DEBUGER_ASSERT_VALID_LOG(FALSE, CConvert::ToString(nJobPattern));
                //continue;
                return false;
            }

		    if (strID == strStationID)
            {
                bValid = true;
			    return true;
            }

            return true;
        }
        #endregion

        // [LGLS 2026-08-01] 설비 입출고 방향 지시. CV_DATA 커맨드 채널에 남기면
        //   WCS_TASK_CV(CvChg_CMD_RQ_YN, CMD_RQ_ID='DIR')가 설비 방향 워드에 기록한다.
        //   pDir : "0"=입고 모드, "1"=출고 모드
        private bool SetCvDirection(string pMcNo, string pDir)
        {
            return SetCvDirection(pMcNo, pDir, false);
        }

        // [LGLS 2026-08-23] 겸용 입출고대(C/V#11=22번)는 출고 화물이 아직 그 자리에 있는 동안
        //   입고 모드로 넘어가면 설비가 같은 자리를 입고대로 쓰기 시작한다.
        //   그래서 '입고로 되돌리라' 는 지시는 즉시 반영하지 않고 대기 커맨드(DIRW)로 남기고,
        //   IO_TASK 가 그 작업대(21/22)에 화물도 데이터도 없는 것을 확인한 뒤 DIR 로 승격한다.
        //   출고(1) 전환은 종전대로 즉시 반영한다.
        private bool SetCvDirection(string pMcNo, string pDir, bool pbDeferIfBusy)
        {
            string strCmdId = (pbDeferIfBusy && pDir == "0") ? "DIRW" : "DIR";
            try
            {
                m_BDb.BeginTrans();
                m_BDb.ParamsClear();

                m_strSql = "";
                m_strSql += modDefApp.CRLF + " UPDATE CV_DATA                                        ";
                m_strSql += modDefApp.CRLF + "    SET CMD_RQ_ID   = " + m_BDb.ParamsAdd("CMD_RQ_ID", strCmdId);
                m_strSql += modDefApp.CRLF + "      , CMD_RQ_PARM = " + m_BDb.ParamsAdd("CMD_RQ_PARM", pDir);
                m_strSql += modDefApp.CRLF + "      , CMD_RQ_YN   = 'Y'                              ";
                m_strSql += modDefApp.CRLF + "      , WRITE_UPD_DT = " + modDateTime.SYSDATE;
                m_strSql += modDefApp.CRLF + "  WHERE WH_TYP      = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                m_strSql += modDefApp.CRLF + "    AND MC_NO       = " + m_BDb.ParamsAdd("MC_NO", pMcNo);

                int nRtn = m_BDb.ExcuteNonQry_Par(ref m_strSql);
                if (nRtn <= 0)
                {
                    m_BDb.RollbackTrans();
                    return false;
                }
                m_BDb.CommitTrans();
                return true;
            }
            catch (Exception ex)
            {
                try { m_BDb.RollbackTrans(); } catch { }
                modCmWork.ShowMsgServer("[SetCvDirection] " + ex.Message, modDefApp.MSG_ERR);
                return false;
            }
        }

        // [LGLS 2026-07-30] WMS-ECS 인터페이스명세서(20100311) 작업대 스테이션 코드 → 현행 CV_DATA.MC_NO 변환.
        //   명세/원본 ECP(MakeErrorString.cs) 체계: 101=외부 입고 전용 입출고대(C/V#11), 102=Picking 작업대(C/V#14/#15),
        //   103=제품 입고대, 104=원부자재 불출대. 102 는 방향에 따라 설비가 갈린다(피킹존 입고=C/V#15, 출고=C/V#14).
        //   103/104 는 기능(입고/출고) 기준 매핑 — [LGLS 2026-08-31 정정] 현장기준 124=입고대(C/V#12)/126=출고대(C/V#13)
        //   (구 ECS 물리 CV#12=입고와 반대. 설비 단위가 아니라 입고대/불출대 기능 단위로 대응시킴).
        // [LGLS 2026-08-24] 매핑표를 modStationMap 으로 이관.
        //   INI [Host]StationMapMode = ECS | WMS 로 해석 기준을 전환할 수 있고,
        //   [StationMap_ECS] / [StationMap_WMS] 에서 개별 코드 재정의도 가능하다.
        //   기본값(WMS 기준)은 종전 하드코딩과 동일하며, 103/104 만 2026-08-24 협의로
        //   설비 용도 기준(103=124 입고대 / 104=126 불출대)으로 바뀌었다.
        //   호출부 시그니처는 그대로 유지한다.
        public static string WmsStationToMcNo(int nCode, bool bSource)
        {
            string strInner = nCode.ToString();
            modStationMap.ToInner(nCode.ToString(), bSource, ref strInner);
            return strInner;
        }
        // 역변환: MC_NO → 명세 작업대 코드 (상태/에러 보고용. 미대응 트랙은 원값 유지)
        public static string McNoToWmsStation(string strMcNo)
        {
            return modStationMap.ToIms(strMcNo);
        }

        //설명 : [LGLS] 모드변경(M) 수신 — C/V#11 RGV 방향 (0=입고, 1=출고)
        //       ECS 원본 ExternalIFManager.checkMode 대응: ConveyorNo=body[2,3], Mode=body[5,1]
        //       [2026-07-30] BODY 7바이트 (STX + 'M' + No(3) + Mode(1) + ETX) — 명세서/원본 실전문("0007") 정합, 구 11(예비4) 폐기
        private void ParseModeChange(int p_iBodyCnt)
        {
            string strTitle = "[ParseModeChange] .. ";

            if (p_iBodyCnt != modDefApp.MSG_MODE_BODY_CNT)
            {
                m_strLog = string.Format("정해진 메세지 길이가 아닙니다.[MsgType:M][수신:{0}][규격:{1}]", p_iBodyCnt, modDefApp.MSG_MODE_BODY_CNT);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse("M", "", modDefApp.MSG_INVALID_MSG_LENG);
                return;
            }

            string strCvNo = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 2, 3);
            string strMode = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 5, 1);

            m_strLog = string.Format("모드변경 수신 [C/V:{0}][Mode:{1}(0=입고,1=출고)]", strCvNo, strMode);
            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_IMP);

            // [LGLS 2026-08-01] 방향 반영 연결: 명세 작업대 코드(101~104)를 MC_NO 로 바꾼 뒤
            //   CV_DATA 커맨드 채널(CMD_RQ_ID='DIR')로 남기면 WCS_TASK_CV 가 설비 방향 워드에 기록한다.
            //   (그 전까지는 수신 로그만 남겨 실제 설비 방향이 바뀌지 않았고, IO_TASK 겸용대 게이트도 무발화였다)
            int nCvCode = 0;
            int.TryParse(strCvNo.Trim(), out nCvCode);
            string strMcNo = (nCvCode >= 101 && nCvCode <= 105)
                             ? WmsStationToMcNo(nCvCode, (strMode != "1"))   // 입고 모드면 입고측 설비로 해석
                             : strCvNo.Trim();
            string strDir = (strMode == "1") ? "1" : "0";

            //   겸용 입출고대로 가는 '입고 복귀' 는 작업대가 빌 때까지 미룬다(IO_TASK 가 승격).
            if (!SetCvDirection(strMcNo, strDir, true))
            {
                m_strLog = string.Format("모드변경 반영 실패 [MC_NO:{0}][Mode:{1}]", strMcNo, strDir);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse("M", "", modDefApp.MSG_INTERNAL_ERROR);
                return;
            }

            m_strLog = string.Format("모드변경 {0} [MC_NO:{1}][Mode:{2}({3})]",
                                     (strDir == "0") ? "접수(작업대가 비면 반영)" : "반영",
                                     strMcNo, strDir, (strDir == "1") ? "출고" : "입고");
            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_IMP);
            MakeResponse("M", "", modDefApp.MSG_NO_ERROR);
        }

        //최초작성자	: BASE(정복열)
        //작성일		: 20200508
        //설명		    : 상위로부터 작업지시 받기
        private void ParseOorR(int p_iBodyCnt)
        {
            //string strFunctionName = "ParseOorR .. ";
            string strTitle = "[ParseOorR] .. ";
            string strMsg = "";
            m_strMsgType = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 1, 1);
            strMsg = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, 0, modDefApp.MSG_HEAD_CNT + p_iBodyCnt);//상위에서 내린 작업 메시지

            #region Received Default Data Parsing
            // Interface목록서[ECS-자동창고] 고정 자리수 파싱 (XML 변환 없이 수신 스트링 직접 파싱)
            //  'O' : STX(15) Type(16) JobDefine(17) WhDefine(18) LuggNo(19,4) SeqNo(23,4) PalletNo(27,20)
            //        SrcStn(47,3) SrcBank(50,2) SrcBay(52,3) SrcLevel(55,2) RouteStn(57,3)
            //        DestStn(60,3) DestBank(63,2) DestBay(65,3) DestLevel(68,2) PairLuggNo(70,4)
            //        Priority(74,3) JobRouting(77,1) ProductID(78,20) UserData(98,20) ETX(118) : Body=104
            //  'R' : (구형 참고용 — 현행은 명세서 31바이트, 아래 R 분기 주석 참조)
            string strO_SeqNo = "";
            string strO_PalletNo = "";
            string strO_RouteStn = "";
            string strO_PairLuggNo = "";
            string strO_Priority = "";
            string strO_JobRouting = "";
            string strO_ProductID = "";
            string strO_UserData = "";
            string strO_LotNo = "";
            string strO_Size = "0";
            string strR_Kind = "";
            string strR_ScNum = "";

            #region Body 길이 체크
            int nBodyCheckCnt = (m_strMsgType == "O") ? modDefApp.MSG_ORDER_BODY_CNT : modDefApp.MSG_REDIRECT_BODY_CNT;
            if (p_iBodyCnt != nBodyCheckCnt)
            {
                m_strLog = string.Format("정해진 메세지 길이가 아닙니다.[MsgType:{0}][수신:{1}][규격:{2}]", m_strMsgType, p_iBodyCnt, nBodyCheckCnt);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse(m_strMsgType, "", modDefApp.MSG_INVALID_MSG_LENG);
                return;
            }
            #endregion

            // [LGLS 66B 규격] O: STX(0) Type(1) JobDefine(2) LuggNo(3,4) SeqNo(7,4) PalletNo(11,7)
            //   SrcStn(18,3) SrcBank(21,2) SrcBay(23,3) SrcLevel(26,2) RouteStn(28,3)
            //   DestStn(31,3) DestBank(34,2) DestBay(36,3) DestLevel(39,2)
            //   Priority(41,3) JobRouting(44,1) ProductID(45,20) ETX(65)  — WhDefine 필드 없음
            string strJob_Define = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 2, 1);
            string strWhDefine = modDefApp.WH_DEFINE;   // 규격에 WhDefine 없음 — 설정값으로 간주
            // [LGLS 2026-07-30] 명세서 규격: O/R 모두 LuggNo = body[3,4] (JobDefine 바로 다음)
            string strLuggNo = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 3, 4);

            string strStartPos = "";
            string strStartBank = "";
            string strStartBay = "";
            string strStartLevel = "";
            string strDestPos = "";
            string strDestBank = "";
            string strDestBay = "";
            string strDestLevel = "";

            int nOffSet = 0; int nSize = 0;
            if (m_strMsgType == "O")
            {
                // [LGLS 66B 규격] SeqNo(7,4) PalletNo(11,7) ... ProductID(45,20)
                nOffSet = 7;
                nSize = 4;   strO_SeqNo = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 7;  strO_PalletNo = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strStartPos = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strStartBank = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strStartBay = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strStartLevel = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strO_RouteStn = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strDestPos = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strDestBank = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strDestBay = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strDestLevel = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strO_Priority = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 1;  strO_JobRouting = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 20; strO_ProductID = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                strO_PairLuggNo = "";
                strO_UserData = "";

                // 파레트번호는 기존 LOT_NO 컬럼에 저장 (공백 제거)
                strO_LotNo = strO_PalletNo.Trim();

                m_strLog = string.Format("[SeqNo:{0}][PalletNo:{1}][RouteStn:{2}][PairLuggNo:{3}][JobRouting:{4}][ProductID:{5}][UserData:{6}]",
                    strO_SeqNo, strO_PalletNo.Trim(), strO_RouteStn, strO_PairLuggNo, strO_JobRouting, strO_ProductID.Trim(), strO_UserData.Trim());
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_NOR);
            }
            else // "R"
            {
                // [LGLS 2026-07-30] 명세서(20100311)/원본 ECP 규격 31바이트:
                //   STX(0) 'R'(1) JobDefine(2) LuggNo(3,4) SrcStn(7,3) SrcBank(10,2) SrcBay(12,3) SrcLevel(15,2)
                //   DestStn(17,3) DestBank(20,2) DestBay(22,3) DestLevel(25,2) Kind(27,1) StackerNo(28,2) ETX(30)
                //   (구 32바이트 규격의 body[3] 예비(WhDefine 자리)는 명세에 없음 — 폐기)
                nOffSet = 7;
                nSize = 3;   strStartPos = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strStartBank = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strStartBay = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strStartLevel = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strDestPos = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strDestBank = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 3;  strDestBay = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strDestLevel = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 1;  strR_Kind = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
                nOffSet += nSize; nSize = 2;  strR_ScNum = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + nOffSet, nSize);
            }

            string strStartLoc = strStartBank + "-" + strStartBay + "-" + strStartLevel;
            string strDestLoc = strDestBank + "-" + strDestBay + "-" + strDestLevel;

            #region WareHouse Define 체크 (A:PalletRack, B:P-BoxRack)
            if (strWhDefine != modDefApp.WH_DEFINE)
            {
                m_strLog = string.Format("WareHouse Define이 올바르지 않습니다.[수신:{0}][설정:{1}]", strWhDefine, modDefApp.WH_DEFINE);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_CONTENT);
                return;
            }
            #endregion

            #region Convert S/C No
            // 크레인 일때 크레인 번호로 저장
            int nStartPos = Convert.ToInt32(strStartPos);
            if (nStartPos < 100 && nStartPos != 0)
                strStartPos = string.Format("9{0:00}", nStartPos);
            // [LGLS] 하이랙 스테이션 코드 201~205 (= S/C#1~#5, TB_CODEMASTER '022' 규격)
            //        → 크레인 번호 901~905 로 변환 (랙 셀은 Bank/Bay/Level 필드로 별도 수신)
            else if (nStartPos >= 201 && nStartPos <= 205)
                strStartPos = string.Format("9{0:00}", nStartPos - 200);
            // [LGLS 2026-07-30] 명세서(20100311) 작업대 스테이션 코드 101~104 → CV_DATA.MC_NO 변환 (출발지=입고측)
            else if (nStartPos >= 101 && nStartPos <= 104)
                strStartPos = WmsStationToMcNo(nStartPos, true);

            // 크레인 일때 크레인 번호로 저장
            int nDestPos = Convert.ToInt32(strDestPos);
            if (nDestPos < 100 && nDestPos != 0)
                strDestPos = string.Format("9{0:00}", nDestPos);
            else if (nDestPos >= 201 && nDestPos <= 205)
                strDestPos = string.Format("9{0:00}", nDestPos - 200);
            // [LGLS 2026-07-30] 명세서 작업대 스테이션 코드 101~104 → CV_DATA.MC_NO 변환 (도착지=출고측)
            else if (nDestPos >= 101 && nDestPos <= 104)
                strDestPos = WmsStationToMcNo(nDestPos, false);
            #endregion

            #endregion

            #region Received Data Validation Check
            // 작업번호가 올바른지 체크!(온라인 작업번호)
            int nLuggNum = Convert.ToInt32(strLuggNo);
            //if ((nLuggNum > 0) && (nLuggNum < 9000))
            if (modDefApp.IsOnlineLuggNum(nLuggNum) == false)
            {
                m_strLog = string.Format("작업번호가 올바르지 않습니다.[{0}]", nLuggNum);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LUGG_NO);
                return;
            }

            // 작업구분이 올바른지 체크!(온라인 작업구분)
            int nJobType = Convert.ToInt32(strJob_Define);
            if (modDefApp.IsOnlineJobType(nJobType) == false)
            {
                m_strLog = string.Format("작업구분이 올바르지 않습니다.[{0}]", strJob_Define);
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_UNKNOWN_JOB_TYPE);
                return;
            }

            // 출발지 올바른지 체크! - 작업구분에 따라 나중에
            // 도착지 올바른지 체크! - 작업구분에 따라 나중에

            // 출발LOC 올바른지 체크! - 작업구분에 따라 나중에 
            // 도착LOC 올바른지 체크! - 작업구분에 따라 나중에
            #endregion

            bool bExistJob = false;
            //string strWhTyp = "10";
            string strJOB_TYPE = "";
            string strJOB_STATUS = "";
            string strSTART_LOC = "";
            string strDEST_LOC = "";
            string strLUGG_NO = "";

            #region 작업번호 중복 체크
            m_strSql = "SELECT * FROM JOB_MST";
            m_strSql += modDefApp.CRLF + "WHERE WH_TYP = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
            m_strSql += modDefApp.CRLF + "  AND LUGG_NO = " + m_BDb.ParamsAdd("LUGG_NO", strLuggNo);

            m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);

            if (m_iSelCnt < 0)
            {
                m_strLog = m_BDb.ErrMsg + m_strSql;
                modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INTERNAL_ERROR);
                return;
            }
            else if (m_iSelCnt == 0)
            {
                bExistJob = false;
            }
            else //if (m_iSelCnt > 0)
            {
                bExistJob = true;
                strJOB_TYPE = "" + m_BDb.dtMain.Rows[0]["JOB_TYP"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[0]["JOB_TYP"].ToString();
                strJOB_STATUS = "" + m_BDb.dtMain.Rows[0]["JOB_STATUS"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[0]["JOB_STATUS"].ToString();
                strSTART_LOC = "" + m_BDb.dtMain.Rows[0]["START_LOCATION"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[0]["START_LOCATION"].ToString();
                strDEST_LOC = "" + m_BDb.dtMain.Rows[0]["DEST_LOCATION"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[0]["DEST_LOCATION"].ToString();
                strLUGG_NO = "" + m_BDb.dtMain.Rows[0]["LUGG_NO"].ToString() == "" ? "0" : m_BDb.dtMain.Rows[0]["LUGG_NO"].ToString();
            }
            #endregion
            
            switch (m_strMsgType)
            { 
            case "O":
                // Priority/PalletNo(LOT_NO) 등은 상단 고정 자리수 파싱에서 이미 추출됨

                #region 존재하는 작업인지 체크
                #region 작업번호가 존재하는 경우 에러 로깅
                if (bExistJob == true)
                {
                    m_strLog = string.Format("이미 지시된 작업입니다. [창고 타입:{0}][작업번호:{1}]", modDefApp.WH_TYP, strLuggNo);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_ALREADY_INVOKED_JOB);
                    return;
                }
                #endregion
                #region 작업정보가 완전히 같은 작업존재하는지 체크
                m_strSql = "SELECT * FROM JOB_MST";
                m_strSql += modDefApp.CRLF + "WHERE WH_TYP = '" + modDefApp.WH_TYP + "'";// m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                m_strSql += modDefApp.CRLF + "  AND START_POS = '" + strStartPos + "'";// mm_BDb.ParamsAdd("START_POS", strStartPos);
                m_strSql += modDefApp.CRLF + "  AND START_LOCATION = '" + strStartLoc + "'";// mm_BDb.ParamsAdd("START_LOCATION", strStartLoc);
                m_strSql += modDefApp.CRLF + "  AND DEST_POS = '" + strDestPos + "'";// mm_BDb.ParamsAdd("DEST_POS", strDestPos);
                m_strSql += modDefApp.CRLF + "  AND DEST_LOCATION = '" + strDestLoc + "'";// mm_BDb.ParamsAdd("DEST_LOCATION", strDestLoc);
                m_strSql += modDefApp.CRLF + "  AND LOT_NO = '" + strO_LotNo + "'";// mm_BDb.ParamsAdd("LOT_NO", strO_LotNo);
                m_strSql += modDefApp.CRLF + "  AND PRODUCT_SIZE = '" + strO_Size + "'";// mm_BDb.ParamsAdd("LOT_NO", strO_LotNo);

                m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);

                if (m_iSelCnt > 0)
                {
                    m_strLog = string.Format("이미 존재하는 작업입니다.[출발지:{0}][출발LOC:{1}][도착지:{2}][도착LOC:{3}][LOT_NO:{4}]",
                        strStartPos, strStartLoc, strDestPos, strDestLoc, strO_LotNo);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_LUGG_NO_DUPLICATED);
                    return;
                }
                #endregion
                #endregion

                #region 작업 구분 별로 확인해야 하는 부분 처리
                string strCELL_USE_YN = "";
                string strCV_PLT_JOB_TYP = "";
                string strSC_PLT_JOB_TYP1 = "";
                string strSC_PLT_JOB_TYP2 = "";
                switch (strJob_Define)
                {
                    case "1":       // 입고
                        #region 입고시에 출발지와 도착지가 정상적인지 체크
                        // [LGLS] 출발지(입고대 작업대) 존재 체크
                        if (IsValidStationLgls(strStartPos) == false)
                        {
                            m_strLog = string.Format("출발지가 올바르지 않습니다.[작업번호:{0}][출발지:{1}]", nLuggNum, strStartPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }

                        // [LGLS] 도착지(크레인 901~905) 존재 체크
                        //  ※ 랙 셀(Bank/Bay/Level)의 CELL_MST 검증은 LGLS 대체 DB 에 CELL_MST 가
                        //     없어 생략 — 셀 관리 도입 시 이 지점에 복원할 것 (기존 IsValidLocation 참조)
                        if (IsValidStationLgls(strDestPos) == false)
                        {
                            m_strLog = string.Format("도착지가 올바르지 않습니다.[작업번호:{0}][도착지:{1}]", nLuggNum, strDestPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }

                        #endregion
                        #region 해당 출발지에 지시하지 않은 작업이 존재할때 작업 생성하지 않기
                        m_strSql = "";
                        m_strSql += modDefApp.CRLF + " SELECT CD.*, JM.*                            ";
                        m_strSql += modDefApp.CRLF + "   FROM CV_DATA CD                            ";
                        m_strSql += modDefApp.CRLF + "  INNER JOIN JOB_MST JM                       ";
                        m_strSql += modDefApp.CRLF + "     ON CD.TRACK_NO = JM.START_POS            ";
                        m_strSql += modDefApp.CRLF + "    AND JM.JOB_STATUS = '99'                  ";
                        m_strSql += modDefApp.CRLF + "  WHERE CD.PLC_NO	= :PLC_NO                   ";
                        m_strSql += modDefApp.CRLF + "    AND CD.LUGG_NO_RD 	= '0'               ";
                        m_strSql += modDefApp.CRLF + "    AND JM.START_POS 	    = :START_POS        ";
                        m_strSql += modDefApp.CRLF + "    AND CD.OD_RQ_YN		= 'N'               ";
                        m_strSql += modDefApp.CRLF + "    AND CD.OD_RQ_FLAG		= 'N'               ";
                        m_strSql += modDefApp.CRLF + "    AND CD.WH_TYP		    = :WH_TYP           ";
                        m_strSql += modDefApp.CRLF + "    AND 0 = (SELECT COUNT(*)                  ";
                        m_strSql += modDefApp.CRLF + "               FROM JOB_MST                   ";
                        m_strSql += modDefApp.CRLF + "              WHERE LUGG_NO = CD.LUGG_NO_RD)  ";

                        m_BDb.comMain.CommandType = System.Data.CommandType.Text;
                        m_BDb.comMain.Parameters.Clear();       
                        m_BDb.comMain.Parameters.Add("PLC_NO", modSpDb.DbTypeChar).Value = "01";
                        m_BDb.comMain.Parameters.Add("WH_TYP", modSpDb.DbTypeChar).Value = modDefApp.WH_TYP;
                        m_BDb.comMain.Parameters.Add("START_POS", modSpDb.DbTypeChar).Value = strStartPos;

                        m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);

                        if (m_iSelCnt > 0)
                        {
                            m_strLog = string.Format("해당 출발지에 지시하지 않은 작업이 존재합니다. [신규 출발지:{0}][신규 작업번호:{1}][기존 작업번호:{2}]",
                                strStartPos, strLuggNo, m_BDb.dtMain.Rows[0]["LUGG_NO"]);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_LUGG_NO_DUPLICATED);
                            return;
                        }

                        #endregion
                        break;
                    case "2":       // 출고
                        #region 출고시에 출발지와 도착지가 정상적인지 체크
                        // [LGLS] 출발지(크레인 901~905, 랙 셀은 Bank/Bay/Level 필드) 존재 체크
                        //  ※ CELL_MST 기반 출발 LOC/금지랙 검증은 LGLS 대체 DB 에 CELL_MST 가
                        //     없어 생략 — 셀 관리 도입 시 복원할 것 (기존 IsValidLocation 참조)
                        if (IsValidStationLgls(strStartPos) == false)
                        {
                            m_strLog = string.Format("출발지가 올바르지 않습니다.[작업번호:{0}][출발지:{1}]", nLuggNum, strStartPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }

                        // [LGLS] 도착지(출고대 작업대) 존재 체크
                        if (IsValidStationLgls(strDestPos) == false)
                        {
                            m_strLog = string.Format("도착지가 올바르지 않습니다.[작업번호:{0}][도착지:{1}]", nLuggNum, strDestPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }
                        #endregion
                        break;
                    //case "3":       // 피킹 출고
                    //    break;
                    case "4":       // 랙투랙
                        #region 랙투랙시에 출발지와 도착지가 정상적인지 체크
                        // 출발LOC 올바른지 체크!
                        if (modDefApp.g_frmForm.IsValidLocation(m_BDb, strStartPos, strStartLoc, ref strCELL_USE_YN, ref strSC_PLT_JOB_TYP1) == false)
                        {
                            m_strLog = string.Format("출발 Location이 올바르지 않습니다.[작업번호:{0}][출발지:{1}][출발LOC:{2}]", nLuggNum, strStartPos, strStartLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strCELL_USE_YN != "Y")
                        {
                            m_strLog = string.Format("출발 Location이 금지랙 입니다.[작업번호:{0}][출발지:{1}][출발LOC:{2}]", nLuggNum, strStartPos, strStartLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }

                        // 도착LOC 올바른지 체크!
                        if (modDefApp.g_frmForm.IsValidLocation(m_BDb, strDestPos, strDestLoc, ref strCELL_USE_YN, ref strSC_PLT_JOB_TYP2) == false)
                        {
                            m_strLog = string.Format("도착 Location이 올바르지 않습니다.[작업번호:{0}][도착지:{1}][도착LOC:{2}]", nLuggNum, strDestPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strCELL_USE_YN != "Y")
                        {
                            m_strLog = string.Format("도착 Location이 금지랙 입니다.[작업번호:{0}][도착지:{1}][도착LOC:{2}]", nLuggNum, strDestPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }

                        // 출발 랙과 도착 랙의 SC 번호가 다른지 체크 
                        if (strStartPos != strDestPos)
                        {
                            m_strLog = string.Format("출발 S/C와 도착 S/C가 다릅니다.[작업번호:{0}][출발SC:{1}][도착SC:{2}]", nLuggNum, strStartPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }
                        // 출발 Location과 도착 Location의 SC 번호가 다른지 체크 
                        if (modDefApp.GetStackerNum(10, strStartLoc) != modDefApp.GetStackerNum(10, strDestLoc))
                        {
                            m_strLog = string.Format("출발 S/C와 도착 S/C가 다릅니다.[작업번호:{0}][출발Location:{1}][도착Location:{2}]", nLuggNum, strStartLoc, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strSC_PLT_JOB_TYP1 != strSC_PLT_JOB_TYP2)
                        {
                            m_strLog = string.Format("해당 Location에서는 도착Location으로 이동할수 없습니다. ROLL랙 <-> PLT 랙[작업번호:{0}][출발LOC:{1}][도착LOC:{2}]", nLuggNum, strStartLoc, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }
                        
                        #endregion
                        break;
                    case "5":       // 호기간 이동
                        #region 호기간 이동시에 출발지와 도착지가 정상적인지 체크
                        // 출발LOC 올바른지 체크!
                        if (modDefApp.g_frmForm.IsValidLocation(m_BDb, strStartPos, strStartLoc, ref strCELL_USE_YN, ref strSC_PLT_JOB_TYP1) == false)
                        {
                            m_strLog = string.Format("출발 Location이 올바르지 않습니다.[작업번호:{0}][출발지:{1}][출발LOC:{2}]", nLuggNum, strStartPos, strStartLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strCELL_USE_YN != "Y")
                        {
                            m_strLog = string.Format("출발 Location이 금지랙 입니다.[작업번호:{0}][출발지:{1}][출발LOC:{2}]", nLuggNum, strStartPos, strStartLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }

                        // 도착LOC 올바른지 체크!
                        if (modDefApp.g_frmForm.IsValidLocation(m_BDb, strDestPos, strDestLoc, ref strCELL_USE_YN, ref strSC_PLT_JOB_TYP2) == false)
                        {
                            m_strLog = string.Format("도착 Location이 올바르지 않습니다.[작업번호:{0}][도착지:{1}][도착LOC:{2}]", nLuggNum, strDestPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strCELL_USE_YN != "Y")
                        {
                            m_strLog = string.Format("도착 Location이 금지랙 입니다.[작업번호:{0}][도착지:{1}][도착LOC:{2}]", nLuggNum, strDestPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }


                        // 출발 랙과 도착 랙의 SC 번호가 다른지 체크 
                        if (strStartPos == strDestPos)
                        {
                            m_strLog = string.Format("출발 S/C와 도착 S/C가 같습니다.[작업번호:{0}][출발SC:{1}][도착SC:{2}]", nLuggNum, strStartPos, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }
                        // 출발 Location과 도착 Location의 SC 번호가 다른지 체크 
                        if (modDefApp.GetStackerNum(10, strStartLoc) != modDefApp.GetStackerNum(10, strDestLoc))
                        {
                            m_strLog = string.Format("출발 S/C와 도착 S/C가 같습니다.[작업번호:{0}][출발Location:{1}][도착Location:{2}]", nLuggNum, strStartLoc, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                            return;
                        }

                        if (strSC_PLT_JOB_TYP1 != strSC_PLT_JOB_TYP2)
                        {
                            m_strLog = string.Format("해당 Location에서는 도착Location으로 이동할수 없습니다. ROLL랙 <-> PLT 랙[작업번호:{0}][출발LOC:{1}][도착LOC:{2}]", nLuggNum, strStartLoc, strDestLoc);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INHIBITED_LOC);
                            return;
                        }

                        #endregion
                        break;
                    case "6":       // 작업대 이동
                        #region 작업대 이동시에 출발지와 도착지가 정상적인지 체크
                        // 출발지 올바른지 체크! 
                        if (IsValidStation(modDefApp.ConvertJobPattern(nJobType), strStartPos, true, ref strCV_PLT_JOB_TYP) == false)
                        {
                            m_strLog = string.Format("출발지가 올바르지 않습니다.[작업번호:{0}][출발지:{1}]", nLuggNum, strStartPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }

                        // 도착지 올바른지 체크! 
                        if (IsValidStation(modDefApp.ConvertJobPattern(nJobType), strDestPos, false, ref strCV_PLT_JOB_TYP) == false)
                        {
                            m_strLog = string.Format("도착지가 올바르지 않습니다.[작업번호:{0}][도착지:{1}]", nLuggNum, strDestPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }

                        if (strSC_PLT_JOB_TYP1 != strSC_PLT_JOB_TYP2)
                        {
                            m_strLog = string.Format("해당 출발지에서는 도착지로 이동할수 없습니다. ROLL CV <-> PLT CV[작업번호:{0}][출발지:{1}][도착지:{2}]", nLuggNum, strStartPos, strDestPos);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_STN_NO);
                            return;
                        }
                        #endregion
                        #region 해당 출발지에 지시하지 않은 작업이 존재할때 작업 생성하지 않기
                        m_strSql = "";
                        m_strSql += modDefApp.CRLF + " SELECT CD.*, JM.*                            ";
                        m_strSql += modDefApp.CRLF + "   FROM CV_DATA CD                            ";
                        m_strSql += modDefApp.CRLF + "  INNER JOIN JOB_MST JM                       ";
                        m_strSql += modDefApp.CRLF + "     ON CD.TRACK_NO = JM.START_POS            ";
                        m_strSql += modDefApp.CRLF + "    AND JM.JOB_STATUS = '99'                  ";
                        m_strSql += modDefApp.CRLF + "  WHERE CD.PLC_NO	= :PLC_NO                   ";
                        m_strSql += modDefApp.CRLF + "    AND CD.LUGG_NO_RD 	= '0'               ";
                        m_strSql += modDefApp.CRLF + "    AND JM.START_POS 	    = :START_POS        ";
                        m_strSql += modDefApp.CRLF + "    AND CD.OD_RQ_YN		= 'N'               ";
                        m_strSql += modDefApp.CRLF + "    AND CD.OD_RQ_FLAG		= 'N'               ";
                        m_strSql += modDefApp.CRLF + "    AND CD.WH_TYP		    = :WH_TYP           ";
                        m_strSql += modDefApp.CRLF + "    AND 0 = (SELECT COUNT(*)                  ";
                        m_strSql += modDefApp.CRLF + "               FROM JOB_MST                   ";
                        m_strSql += modDefApp.CRLF + "              WHERE LUGG_NO = CD.LUGG_NO_RD)  ";

                        m_BDb.comMain.CommandType = System.Data.CommandType.Text;
                        m_BDb.comMain.Parameters.Clear();       
                        m_BDb.comMain.Parameters.Add("PLC_NO", modSpDb.DbTypeChar).Value = "01";
                        m_BDb.comMain.Parameters.Add("WH_TYP", modSpDb.DbTypeChar).Value = modDefApp.WH_TYP;
                        m_BDb.comMain.Parameters.Add("START_POS", modSpDb.DbTypeChar).Value = strStartPos;

                        m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);

                        if (m_iSelCnt > 0)
                        {
                            m_strLog = string.Format("해당 출발지에 지시하지 않은 작업이 존재합니다. [신규 출발지:{0}][신규 작업번호:{1}][기존 작업번호:{1}]",
                                strStartPos, strLuggNo, m_BDb.dtMain.Rows[0]["LUGG_NO"]);
                            modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_LUGG_NO_DUPLICATED);
                            return;
                        }

                        #endregion
                        break;
                    default:
                        m_strLog = string.Format("작업구분이 올바르지 않습니다.[{0}]", strJob_Define);
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, "", modDefApp.MSG_UNKNOWN_JOB_TYPE);
                        return;
                }
                #endregion

                #region 작업 생성
                m_BDb.BeginTrans();

                m_BDb.ParamsClear();
                if (modDefApp.g_frmForm.InsertJobMst(m_BDb, strLuggNo, strStartPos, strStartLoc, strDestPos, strDestLoc, strJob_Define, strO_LotNo, strO_Priority, strO_Size, true, strO_ProductID.Trim()) == false)
                {
                    // 함수안에서 실패시 화면에 메세지 출력함!
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_ECS_BUFFER_FULL);
                    m_BDb.RollbackTrans();
                    break;
                }

                    /*
                m_strSql = "INSERT INTO JOB_MST     ";
                m_strSql += modDefApp.CRLF + "(  WH_TYP			    ";
                m_strSql += modDefApp.CRLF + " , LUGG_NO			";
                m_strSql += modDefApp.CRLF + " , START_POS			";
                m_strSql += modDefApp.CRLF + " , START_LOCATION    	";
                m_strSql += modDefApp.CRLF + " , DEST_POS          	";
                m_strSql += modDefApp.CRLF + " , DEST_LOCATION     	";
                m_strSql += modDefApp.CRLF + " , JOB_TYP		    ";
                m_strSql += modDefApp.CRLF + " , JOB_STATUS        	";
                m_strSql += modDefApp.CRLF + " , LOT_NO		    	";
                m_strSql += modDefApp.CRLF + " , JOB_PRIORITY     	";
                m_strSql += modDefApp.CRLF + " , INS_DT		    	";
                m_strSql += modDefApp.CRLF + " , INS_USER_ID       	";
                m_strSql += modDefApp.CRLF + " , REMARKS )         	";
                m_strSql += modDefApp.CRLF + "VALUES ('" + modDefApp.WH_TYP + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strLuggNo + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strStartPos + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strStartLoc + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strDestPos + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strDestLoc + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strJob_Define + "'";
                m_strSql += modDefApp.CRLF + "     ,  '99'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strO_LotNo + "'";
                m_strSql += modDefApp.CRLF + "     ,  '" + strO_Priority + "'";
                m_strSql += modDefApp.CRLF + "     ,   " + modDateTime.SYSDATE;
                m_strSql += modDefApp.CRLF + "     ,  'HOST_TASK'"; 
                m_strSql += modDefApp.CRLF + "     ,  '');";

                m_iSelCnt = m_BDb.ExcuteNonQry_Par(ref m_strSql);

                if (m_iSelCnt <= 0)
                {
                    m_strLog = string.Format("작업 생성에 실패하였습니다. [작업번호:{0}][출발지:{1}][출발LOC:{2}][도착지:{3}][도착LOC:{4}][LOT_NO:{5}][DB Err:{6}]",
                        strLuggNo, strStartPos, strStartLoc, strDestPos, strDestLoc, strO_LotNo, m_BDb.strErrMsg);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_ECS_BUFFER_FULL);
                    m_BDb.RollbackTrans();
                    return;
                }
                    //*/
                m_BDb.CommitTrans();

                modCmWork.ShowMsgServer(strTitle + "작업 추가 되었습니다.[작업번호:" + strLuggNo + "]", modDefApp.MSG_IMP);

                #endregion

                #region 공파레트 입고, 출고 요청이 있을때 해당 작업인지 체크할것!
                // [LGLS] 본 대체 시스템은 재고(공파레트)를 관리하지 않음 — 공파레트 매칭 처리 제거
                //if (modDefApp.g_strEmtpyPltKind != "" && modDefApp.g_strEmtpyPltStation != "") { ... }
                #endregion
                break;
            case "R":
                // Kind/StackerNo는 상단 고정 자리수 파싱에서 이미 추출됨(strR_Kind, strR_ScNum)

                #region 존재하는 작업인지 체크
                #region 이중입고 작업번호가 존재하지 않을경우 에러로깅
                if (bExistJob == false)
                {
                    m_strLog = string.Format("이중입고 에러 작업정보가 존재하지 않음. [작업번호:{0}]", strLuggNo);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_NO_EXIST_JOB);
                    return;
                }
                #endregion
                #region 작업 구분이 같은지 체크 
                if (strJOB_TYPE != "" && strJOB_TYPE != strJob_Define)
                {
                    m_strLog = string.Format("작업구분 불일치 [작업번호:{0}][OLD:{1}][NEW:{2}]", strLuggNo, strJOB_TYPE, strJob_Define);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_UNKNOWN_JOB_TYPE);
                    return;
                }
                #endregion
                #endregion

                #region 재지정 종류에 따라 동일 호기 작업인지 체크 
                switch (strR_Kind)
                {
                case "1":       // 입고 재지정
                    // 기존 도착 Location과 신규 도착 Location의 SC 번호가 다른지 체크 
                    if (modDefApp.GetStackerNum(10, strDEST_LOC) != modDefApp.GetStackerNum(10, strDestLoc))
                    {
                        m_strLog = string.Format("S/C 동일호기 이중입고 재지정 작업이 아닙니다.[작업번호:{0}][OLD LOC:{1}][NEW LOC:{2}]", nLuggNum, strDEST_LOC, strDestLoc);
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                        return;
                    }
                    if (strDEST_LOC == strDestLoc)
                    {
                        m_strLog = string.Format("재지정 받은 LOCATION이 기존과 같습니다.[LOCATION:{0}][OLD LUGG:{1}][NEW LUGG:{2}]", strDEST_LOC, strLUGG_NO, nLuggNum);
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                        return;
                    }

                    m_BDb.BeginTrans();

                    m_BDb.ParamsClear();

                    m_strSql = modDefApp.CRLF + "  UPDATE JOB_MST ";
                    m_strSql += modDefApp.CRLF + "    SET DEST_LOCATION = " + m_BDb.ParamsAdd("DEST_LOCATION", strDestLoc);
                    // [LGLS 2026-08-30] 재지정 상태는 common_code JOB_STATUS 코드값(2자리)으로 쓴다.
                    //   enum 값을 그대로 쓰면 이중입고=9, 공출고=10 인데 현행 체계에서 10 은 "CV 구동대기"라
                    //   재지정 작업이 CV 구동대기로 오인돼 DriveCV 에 잡힌다. 07=이중입고 재지정, 06=공출고 재지정.
                    m_strSql += modDefApp.CRLF + "      , JOB_STATUS    = " + m_BDb.ParamsAdd("JOB_STATUS", modDefApp.JOB_ST_DUAL_RETRY);
                    m_strSql += modDefApp.CRLF + "      , UPD_DT        = " + modDateTime.SYSDATE;
                    m_strSql += modDefApp.CRLF + "      , UPD_USER_ID   = 'HOST_TASK'";
                    m_strSql += modDefApp.CRLF + "  WHERE WH_TYP        = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                    m_strSql += modDefApp.CRLF + "    AND LUGG_NO        = " + m_BDb.ParamsAdd("LUGG_NO", strLuggNo);
                    m_iSelCnt = m_BDb.ExcuteNonQry_Par(ref m_strSql);
                    if (m_iSelCnt < 0)
                    {
                        m_strLog = m_BDb.ErrMsg + m_strSql;
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INTERNAL_ERROR);
                        m_BDb.RollbackTrans();
                        return;
                    }

                    m_strLog = string.Format("입고 재지정 수신 및 작업정보 변경했습니다.[작업번호:{0}]", nLuggNum);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_NOR);
                    m_BDb.CommitTrans();

                    break;
                case "2":       // 출고 재지정
                    // 기존 출발 Location과 신규 출발 Location의 SC 번호가 다른지 체크 
                    if (modDefApp.GetStackerNum(10, strSTART_LOC) != modDefApp.GetStackerNum(10, strStartLoc))
                    {
                        m_strLog = string.Format("S/C 동일호기 이중입고 재지정 작업이 아닙니다.[작업번호:{0}][OLD LOC:{1}][NEW LOC:{2}]", nLuggNum, strSTART_LOC, strStartLoc);
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                        return;
                    }
                    if (strSTART_LOC == strStartLoc)
                    {
                        m_strLog = string.Format("재지정 받은 LOCATION이 기존과 같습니다.[LOCATION:{0}][OLD LUGG:{1}][NEW LUGG:{2}]", strSTART_LOC, strLUGG_NO, nLuggNum);
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                        return;
                    }

                    m_BDb.BeginTrans();

                    m_BDb.ParamsClear();

                    // [LGLS 2026-09-05] 종전에는 존재하지 않는 테이블 LUGG_MST(컬럼 LUGGNO)를 갱신해
                    //   출고 재지정이 항상 실패했다. 입고 재지정과 같이 JOB_MST 를 갱신한다.
                    m_strSql = modDefApp.CRLF + "  UPDATE JOB_MST ";
                    m_strSql += modDefApp.CRLF + "    SET START_LOCATION = " + m_BDb.ParamsAdd("START_LOCATION", strStartLoc);
                    // [LGLS 2026-08-30] 위와 동일 - 06 = 공출고 재지정
                    m_strSql += modDefApp.CRLF + "      , JOB_STATUS    = '" + modDefApp.JOB_ST_EMPTY_RETRY + "'";
                    m_strSql += modDefApp.CRLF + "      , UPD_DT        = " + modDateTime.SYSDATE;
                    m_strSql += modDefApp.CRLF + "      , UPD_USER_ID   = 'HOST_TASK'";
                    m_strSql += modDefApp.CRLF + "  WHERE WH_TYP        = " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
                    m_strSql += modDefApp.CRLF + "    AND LUGG_NO       = " + m_BDb.ParamsAdd("LUGG_NO", strLuggNo);
                    m_iSelCnt = m_BDb.ExcuteNonQry_Par(ref m_strSql);
                    if (m_iSelCnt < 0)
                    {
                        m_strLog = m_BDb.ErrMsg + m_strSql;
                        modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                        MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INTERNAL_ERROR);
                        m_BDb.RollbackTrans();
                        return;
                    }

                    m_strLog = string.Format("출고 재지정 수신 및 작업정보 변경했습니다.[작업번호:{0}]", nLuggNum);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_NOR);
                    m_BDb.CommitTrans();
                    break;
                default:
                    m_strLog = string.Format("유효하지 않는 재지정 종류입니다.[작업번호:{0}][재지정 종류:{1}]", nLuggNum, strR_Kind);
                    modCmWork.ShowMsgServer(strTitle + m_strLog, modDefApp.MSG_ERR);
                    MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_INVALID_LOC);
                    return;
                }
                #endregion
                break;
            }

            // 죄종적으로 트랜잭션 커밋하기!
            //m_BDb.CommitTrans();
            MakeResponse(m_strMsgType, strLuggNo, modDefApp.MSG_NO_ERROR);
        }

        //최초작성자	: BASE(이길문)
        //작성일		: 20160829
        //설명		: 작업취소정보 수신처리
        private void ParseCancel(int p_iBodyCnt)
		{
			string strJob_Define = null;
			string strWorkNo = null;

			//If Not CheckBodyLen(nBodyCnt, 37) Then Exit Sub

			strJob_Define = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 2, 1);
			strWorkNo = System.Text.Encoding.UTF8.GetString(m_bytRxBuff, modDefApp.MSG_HEAD_CNT + 3, 4);

			m_BDb.ParamsClear();

            m_strSql = modDefApp.CRLF + "  SELECT * ";
            m_strSql += modDefApp.CRLF + "   FROM JOB_MST ";
            m_strSql += modDefApp.CRLF + "  WHERE WH_TYP     =  " + m_BDb.ParamsAdd("WH_TYP", modDefApp.WH_TYP);
            m_strSql += modDefApp.CRLF + "    AND LUGG_NO    =  " + m_BDb.ParamsAdd("LUGG_NO", strWorkNo);
            m_strSql += modDefApp.CRLF + "    AND JOB_STATUS IN ('99', '10', '20')";
            m_iSelCnt = m_BDb.ExcuteQry_Par(ref m_strSql);
			if (m_iSelCnt < 0)
			{
				m_strLog = m_BDb.ErrMsg + m_strSql;
				modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);

                MakeResponse(m_strMsgType, strWorkNo, modDefApp.MSG_INTERNAL_ERROR);
				return;
			}
            if (m_iSelCnt == 1)
            {
                bool bResult = modDefApp.g_frmForm.DeleteJobMst(m_BDb, true, strWorkNo, false, true);     // 함수안에서 Transaction 처리함!
                if (bResult == false)
                {
                    m_strLog = string.Format("작업 삭제 실패하였습니다. [작업번호:{0}][실패내용:{1}]", strWorkNo, modDefApp.GM_RTN_MSG);
                    modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);

                    MakeResponse(m_strMsgType, strWorkNo, modDefApp.MSG_INTERNAL_ERROR);
                    return;
                }
            }
            else if (m_iSelCnt == 0)
            {
                m_strLog = "삭제할 작업정보가 존재하지 않습니다.- 삭제 불가능한 경우(이미 진행중인 작업) 포함[작업번호:" + strWorkNo + "]";
                modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);

                MakeResponse(m_strMsgType, strWorkNo, modDefApp.MSG_NO_EXIST_JOB);
                return;            
            }
            else 
            {
                m_strLog = "작업정보가 2개 이상입니다.[작업번호:" + strWorkNo + "]";
                modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_ERR);

                MakeResponse(m_strMsgType, strWorkNo, modDefApp.MSG_ALREADY_INVOKED_JOB);
                return;
            }
            m_strLog = string.Format("작업 삭제 성공하였습니다. [작업번호:{0}]", strWorkNo);
            modCmWork.ShowMsgServer(m_strLog, modDefApp.MSG_NOR);

            MakeResponse(m_strMsgType, strWorkNo, modDefApp.MSG_NO_ERROR);
		}

		//최초작성자	: BASE(이길문)
		//작성일		: 20160829
		//설명		: 통계일자, Shift 구하기

	}
}
