using System;
using System.Collections.Generic;
using System.Text;
using Samoh_Lib;
using System.Data;
using System.Data.OleDb;
using System.Net.Sockets;
using Samoh_Socket;
#if POSTGRESQL
// [LGLS] DB 타입 별칭 (POSTGRESQL/SQL 겸용, IO_TASK 방식)
using DbConn = Npgsql.NpgsqlConnection;
#elif SQL
// [LGLS] MS SQL Server (System.Data.SqlClient)
using DbConn = System.Data.SqlClient.SqlConnection;
#endif

/*
 * FenetProtocol.cs  [BIN판 - WCS_TASK_CV_BIN]
 *
 * 원본(WCS_TASK_CV)과의 차이 3가지 (XGT 공식 규격 / ECS CLgXgtFEnetIfSk 와 동일하게 수정):
 *   1) DATACOUNT 인코딩 : 10진 분할 -> 2진수 리틀엔디언 (규격)
 *   2) 체크섬          : LENGTH 확정 후 0~18바이트 합산 (규격)
 *   3) 응답 에러 판정  : [24-25](예약영역) -> [26-27](에러상태), NAK 코드는 [28-29]
 *   + 1회 요청 최대 워드 수 500 -> 50 (양쪽 인코딩이 일치하는 안전 범위로 분할 전송)
 *
 * MelsecQ3EProtocol.cs 와 동일한 함수 구조를 그대로 대체하면서
 * 신 프로토콜은 Mitsubishi 3E Frame 을 LS Electric LSIS-XGT FEnet 으로 교체.
 *
 * [프로토콜 참조]
 *   FenetDriver.cs (ECP\Com\FieldBus\Fenet\FenetDriver.cs) 의 DirectReadFromPLC /
 *   DirectWriteToPLC 구현을 기반으로 SocketClient 구조에 맞게 재구현.
 *
 * [디바이스 코드 매핑]  MelsecQ3E 와 XGT 디바이스 문자
 *   0xA8 (MELSECQ_DEVICE_CODE_D)  -> 'D'  (%DW : 워드 접근)
 *   0x90 (MELSECQ_DEVICE_CODE_M)  -> 'M'  (%MW : 워드 접근)
 *   0xAF (MELSECQ_DEVICE_CODE_R)  -> 'R'  (%RW : 워드 접근)
 *
 * [교체 방법]
 *   CvThread.cs 에서 MelsecQ3EProtocol 을 FenetProtocol 로 클래스명 변경.
 *   READ / WRITE / Open / Close / SetConfig / _pBdb 멤버 모두 동일.
 *
 * [LSIS-XGT FEnet 요청 프레임 구조] (최대 20바이트 고정 헤더 + 커맨드 데이터)
 *   [0-9]   COMPANY_ID   : 'L','S','I','S','-','X','G','T',0x00,0x00
 *   [10-11] PLC_INFO     : 0x00, 0x00
 *   [12]    CPU_INFO     : 0xA0
 *   [13]    SOURCE_FRAME : 0x33
 *   [14-15] INVOKE_ID    : 커맨드별 고정값 + 시퀀스 카운트
 *   [16-17] LENGTH       : 커맨드 데이터 길이 (little-endian)
 *   [18]    POSITION     : 0x00
 *   [19]    CHECKSUM     : 바이트 0~18 합산 (확정된 LENGTH 포함 - XGT 규격)
 *   [20-]   커맨드 데이터 (READ: 0x54 / WRITE: 0x58, DATATYPE 0x14, 디바이스문자, DATACOUNT)
 *
 * [응답 프레임]
 *   [0-31]  응답 헤더 (FENET_RECV_HEADER = 32 바이트)
 *   [32-]   데이터 (wordCount * 2 바이트, little-endian)
 */

namespace WCS_TASK_CV
{
    public class FenetProtocol : SocketClient
    {
#if ORACLE
        public OleDbConnection _pConObj;
        public cDbUse _pBdb;
#endif
#if POSTGRESQL || SQL
        public DbConn _pConObj; // [LGLS]
        public cDbPostUse _pBdb;
#endif

        #region 상수
        // 응답 헤더 크기 (FenetDriver.cs 의 RECV_HEADER = 32 와 동일)
        private const int FENET_RECV_HEADER = 32;
        // [BIN판] 요청 1회당 최대 워드 수 : 500 -> 50 으로 축소 (ECS 방식 호환)
        //   50워드(100바이트)씩 끊어 읽으면 상대가 10진 분할/2진수 어느 방식으로
        //   DATACOUNT 를 해석하더라도 같은 값이 되어 안전하다.
        private const int FENET_MAX_WORDS = 50;
        // LSIS-XGT 회사 ID (10 바이트)
        private static readonly byte[] FENET_COMPANY_ID = new byte[10]
        {
            (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-',
            (byte)'X', (byte)'G', (byte)'T', 0x00, 0x00
        };
        #endregion

        #region 멤버 변수
        private string _strConnectionString;
        private string _strErrorMsg = "";
        private string _strSndHexString;
        private string _strRcvHexString;
        private string _strSndAsciiString;
        private string _strRcvAsciiString;
        private bool _Hex  = true;
        private bool _Ascii = false;
        private byte _invokeSeq = 0x01;   // 요청마다 증가하는 시퀀스 번호

        public bool m_bDBOpen;

        public bool   IsHex         { get { return _Hex;            } set { _Hex            = value; } }
        public bool   IsAscii       { get { return _Ascii;          } set { _Ascii          = value; } }
        public string SndHexString  { get { return _strSndHexString; } set { _strSndHexString = value; } }
        public string RcvHexString  { get { return _strRcvHexString; } set { _strRcvHexString = value; } }
        public string SndAsciiString{ get { return _strSndAsciiString;} set { _strSndAsciiString = value; } }
        public string RcvAsciiString{ get { return _strRcvAsciiString;} set { _strRcvAsciiString = value; } }
        #endregion

        #region 생성자 (MelsecQ3EProtocol 동일 시그니처)
        public FenetProtocol(string connectionString)
        {
            _strConnectionString = connectionString;
        }

        public FenetProtocol(string strId, string strName, string connectionString)
        {
            _strConnectionString = connectionString;
        }

        // DB 없이 소켓만 연결 (메모리맵 직접 R/W 폼 전용)
        public FenetProtocol() { }
        #endregion

        #region OpenSocketOnly() - TCP 소켓만 오픈 (DB 불필요)
        public bool OpenSocketOnly(string ip, int port, ref string strRtnMsg)
        {
            try
            {
                // RecvRst 의 GetDate() 는 초(seconds) 단위 반환 → timeout 을 초로 지정
                SetConfig(ip, port, 3);   // 3초 타임아웃
                if (!Connect(ref strRtnMsg))
                {
                    strRtnMsg = "[OpenSocketOnly] 소켓 연결 실패: " + strRtnMsg;
                    return false;
                }
                m_bSocCon = true;
                m_bDBOpen = false;
                return true;
            }
            catch (Exception ex)
            {
                strRtnMsg = "[OpenSocketOnly] Exception: " + ex.Message;
                return false;
            }
        }

        public void CloseSocket()
        {
            try
            {
                if (m_bSocCon)
                {
                    string msg = "";
                    ThreadStop(ref msg);
                    m_bSocCon = false;
                }
            }
            catch { }
        }
        #endregion

        #region Open() - DB + TCP 소켓 오픈
#if ORACLE
        public bool Open(ref string strRtnMsg)
        {
            string strTitle = "[Open]";
            m_bDBOpen = false;
            _pConObj = new OleDbConnection();
            try
            {
                _pConObj.ConnectionString = _strConnectionString;
                _pConObj.Open();
                if (_pConObj.State != ConnectionState.Open)
                {
                    _pConObj.Dispose();
                    strRtnMsg = strTitle + "DataBase Open에 실패하였습니다.";
                    return false;
                }
                Samoh_Lib.Cls_Shared.DB_INIT(_pConObj);
                _pBdb = new cDbUse(ref _pConObj, false);
                m_bDBOpen = true;

                if (!Connect(ref strRtnMsg))
                {
                    strRtnMsg = strTitle + strRtnMsg;
                    return false;
                }
                m_bSocCon = true;
                return true;
            }
            catch (Exception e)
            {
                strRtnMsg = strTitle + e.ToString();
                _pConObj.Dispose();
                return false;
            }
        }
#endif
#if POSTGRESQL || SQL
        public bool Open(ref string strRtnMsg) // [LGLS] SQL 모드 겸용 (DbConn 별칭 사용)
        {
            string strTitle = "[Open]";
            m_bDBOpen = false;
            _pConObj = new DbConn();
            try
            {
                _pConObj.ConnectionString = _strConnectionString;
                _pConObj.Open();
                if (_pConObj.State != ConnectionState.Open)
                {
                    _pConObj.Dispose();
                    strRtnMsg = strTitle + "DataBase Open에 실패하였습니다.";
                    return false;
                }
                _pBdb = new cDbPostUse(_pConObj, false);
                m_bDBOpen = true;

                if (!Connect(ref strRtnMsg))
                {
                    strRtnMsg = strTitle + strRtnMsg;
                    return false;
                }
                m_bSocCon = true;
                return true;
            }
            catch (Exception e)
            {
                strRtnMsg = strTitle + e.ToString();
                _pConObj.Dispose();
                return false;
            }
        }
#endif
        #endregion

        #region Close() - DB + 소켓 닫기
        public void Close(ref string strRtnMsg)
        {
            try
            {
                if (m_bSocCon)
                {
                    string msg = "";
                    ThreadStop(ref msg);
                }
                if (m_bDBOpen)
                {
                    _pConObj.Close();
                    _pConObj.Dispose();
                    m_bDBOpen = false;
                }
                strRtnMsg = "[Close] DB, Socket Close. Success";
            }
            catch (SocketException sex)
            {
                strRtnMsg = "[Close] SocketException: " + sex.ToString();
            }
            catch (Exception ex)
            {
                strRtnMsg = "[Close] Exception: " + ex.ToString();
            }
        }
        #endregion

        #region READ - MelsecQ3EProtocol.READ 와 동일 시그니처
        /// <summary>
        /// PLC 워드 영역 읽기.
        /// nStartAddr : 워드 주소 (예: D100 이면 100)
        /// nReadLen   : 읽을 워드 수
        /// RXBUFF     : 결과 버퍼 (nReadLen * 2 바이트, little-endian)
        /// </summary>
        public bool READ(int nUnitType, byte DeviceCode, int nStartAddr, int nReadLen, ref byte[] RXBUFF)
        {
            int outOffset  = 0;
            int remaining  = nReadLen;
            int curAddr    = nStartAddr;

            while (remaining > 0)
            {
                int chunkWords = Math.Min(remaining, FENET_MAX_WORDS);
                byte[] txFrame = BuildReadFrame(DeviceCode, curAddr, chunkWords);

                if (IsHex)
                    SndHexString = BytesToHexs(txFrame, txFrame.Length);
                if (IsAscii)
                    SndAsciiString = Encoding.Default.GetString(txFrame, 0, txFrame.Length);

                Clearbuffer();
                string msg = "";
                if (!SendRst(txFrame, txFrame.Length, ref msg))
                {
                    SetErrorMsg("[READ] Send 실패: " + msg + " TX[" + SndHexString + "]");
                    return false;
                }

                // 응답: 고정 헤더 + 데이터(chunkWords * 2)
                int recvHeader = GetRecvHeaderSize();
                int recvTotal = recvHeader + chunkWords * 2;
                byte[] rxBuf  = new byte[recvTotal + 64];
                Array.Clear(rxBuf, 0, rxBuf.Length);

                if (!RecvRst(ref rxBuf, recvTotal, ref msg))
                {
                    if (IsHex && RcvLen > 0)
                        RcvHexString = BytesToHexs(rxBuf, RcvLen);
                    SetErrorMsg("[READ] Recv 실패: " + msg);
                    return false;
                }

                if (IsHex && RcvLen > 0)
                    RcvHexString = BytesToHexs(rxBuf, RcvLen);
                if (IsAscii && RcvLen > 0)
                    RcvAsciiString = Encoding.Default.GetString(rxBuf, 0, RcvLen);

                if (!ValidateResponse(rxBuf, "[READ]"))
                    return false;

                // 데이터 복사: 응답[헤더] 이후 -> RXBUFF[outOffset]
                Buffer.BlockCopy(rxBuf, recvHeader, RXBUFF, outOffset, chunkWords * 2);

                outOffset += chunkWords * 2;
                curAddr   += chunkWords;
                remaining -= chunkWords;
            }
            return true;
        }
        #endregion

        #region WRITE - MelsecQ3EProtocol.WRITE 와 동일 시그니처
        /// <summary>
        /// PLC 워드 영역 쓰기.
        /// nStartAddr : 워드 주소 (예: D200 이면 200)
        /// nWriteLen  : 쓸 워드 수
        /// TXBUFF     : 데이터 버퍼 (nWriteLen * 2 바이트, little-endian)
        /// </summary>
        public bool WRITE(int nUnitType, byte DeviceCode, int nStartAddr, int nWriteLen, byte[] TXBUFF)
        {
            int inOffset  = 0;
            int remaining = nWriteLen;
            int curAddr   = nStartAddr;

            while (remaining > 0)
            {
                int chunkWords = Math.Min(remaining, FENET_MAX_WORDS);
                byte[] chunkData = new byte[chunkWords * 2];
                Buffer.BlockCopy(TXBUFF, inOffset, chunkData, 0, chunkWords * 2);

                byte[] txFrame = BuildWriteFrame(DeviceCode, curAddr, chunkWords, chunkData);

                if (IsHex)
                    SndHexString = BytesToHexs(txFrame, txFrame.Length);
                if (IsAscii)
                    SndAsciiString = Encoding.Default.GetString(txFrame, 0, txFrame.Length);

                Clearbuffer();
                string msg = "";
                if (!SendRst(txFrame, txFrame.Length, ref msg))
                {
                    SetErrorMsg("[WRITE] Send 실패: " + msg + " TX[" + SndHexString + "]");
                    return false;
                }

                // 쓰기 응답: 쓰기 ACK 크기만큼만 수신
                // [BIN판] XGT 쓰기 ACK = 헤더(20)+응답데이터(10) = 30바이트.
                //   읽기 응답 헤더(32)를 기다리면 2바이트가 모자라 항상 타임아웃이 나면서
                //   실제로는 성공한 쓰기가 "실패"로 보고되는 버그가 있었음.
                byte[] rxBuf = new byte[GetRecvHeaderSize() + 64];
                Array.Clear(rxBuf, 0, rxBuf.Length);

                if (!RecvRst(ref rxBuf, GetWriteAckSize(), ref msg))
                {
                    if (IsHex && RcvLen > 0)
                        RcvHexString = BytesToHexs(rxBuf, RcvLen);
                    SetErrorMsg("[WRITE] Recv ACK 실패: " + msg);
                    return false;
                }

                if (IsHex && RcvLen > 0)
                    RcvHexString = BytesToHexs(rxBuf, RcvLen);
                if (IsAscii && RcvLen > 0)
                    RcvAsciiString = Encoding.Default.GetString(rxBuf, 0, RcvLen);

                if (!ValidateResponse(rxBuf, "[WRITE]"))
                    return false;

                inOffset  += chunkWords * 2;
                curAddr   += chunkWords;
                remaining -= chunkWords;
            }
            return true;
        }
        #endregion

        #region 프레임 빌더 (LSIS-XGT FEnet)
        /*
         * LSIS-XGT FEnet 요청 프레임을 생성한다.
         * FenetDriver.cs 의 DirectReadFromPLC / DirectWriteToPLC 구조를 따름.
         *
         * 디바이스 문자열: %{devChar}W{wordAddr}  (예: %DW100, %MW0, %RW200)
         *   'W' = Word 접근 -> wordAddr 은 워드 단위 주소를 그대로 사용
         *
         * DATACOUNT 인코딩: [BIN판] XGT 공식 규격의 2진수 리틀엔디언
         *   예) 500바이트 -> [dcLow=0xF4, dcHigh=0x01]
         *   (구판의 10진 분할 방식은 100바이트 이상에서 규격과 어긋나 제거)
         *
         * CHECKSUM: [BIN판] LENGTH 를 실제 값으로 확정한 뒤 바이트 0~18 합산 (규격)
         */

        // 응답 고정 헤더 크기 (파생 프로토콜에서 재정의)
        protected virtual int GetRecvHeaderSize()
        {
            return FENET_RECV_HEADER;
        }

        // [BIN판] 쓰기 ACK 프레임 크기: 헤더(20) + 응답데이터(10) = 30바이트
        //   (읽기 응답의 32바이트와 다름. Melsec 은 읽기/쓰기 헤더가 같으므로 재정의)
        protected virtual int GetWriteAckSize()
        {
            return FENET_RECV_HEADER - 2;   // 30
        }

        protected virtual byte[] BuildReadFrame(byte deviceCode, int wordAddr, int wordCount)
        {
            // INVOKE_ID 고정값 0x40, 시퀀스 (FenetDriver.cs DirectReadFromPLC 참조)
            return BuildFenetFrame(0x54, (byte)0x40, GetDeviceChar(deviceCode), wordAddr, wordCount, null);
        }

        protected virtual byte[] BuildWriteFrame(byte deviceCode, int wordAddr, int wordCount, byte[] data)
        {
            // INVOKE_ID 고정값 0x30, 시퀀스 (FenetDriver.cs DirectWriteToPLC 참조)
            return BuildFenetFrame(0x58, (byte)0x30, GetDeviceChar(deviceCode), wordAddr, wordCount, data);
        }

        private byte[] BuildFenetFrame(byte command, byte invokeBase, char devChar,
                                        int wordAddr, int wordCount, byte[] writeData)
        {
            bool isWrite = (writeData != null);

            // [V1.5 매핑] ECS FenetDriver(mdSend/mdReceive)와 동일하게 논리 영역별
            //   디바이스 문자를 그대로 전송한다: M→%MB, D→%DB, R→%RB.
            //   (V1.4의 "전부 %MB" 방식은 ECS 코드 오독 — ECS는 mdDevSet(%MX)만 M 고정이고
            //    연속 읽기/쓰기는 영역 문자를 구분함. EQP_SIM/실PLC의 R·D 영역과 정합 필요)
            //   워드주소 × 2 = 바이트주소  (FenetDriver.cs DirectWriteToPLC 와 동일)
            //   예) D방향워드 word490 → %DB980, R트래킹 word100 → %RB200
            //   (R 논리주소는 10진 워드주소: R0100 = word100, ×2 = %RB200)
            //
            // [V0.9 모드] (SYS_MAIN 라디오 / ini [PLC] PROTO_VER=9)
            //   XGK 래더 직접 표기: %{디바이스}W + 10진 워드주소 그대로 (×2 안함)
            string strAddr;
            if (cDefApp.GM_ADDR_V09)
                strAddr = wordAddr.ToString();          // V0.9 : 워드주소 그대로
            else
                strAddr = (wordAddr * 2).ToString();    // V1.1 : 바이트주소(×2)
            byte[] addrBytes = Encoding.ASCII.GetBytes(strAddr);

            // DATACOUNT: 연속 접근(0x14)은 바이트 수 단위 (워드 수 * 2)
            // [BIN판] XGT 공식 규격대로 2진수 리틀엔디언으로 인코딩 (ECS CLgXgtFEnetIfSk 와 동일)
            //   구판(10진 분할: 1000 -> [0x00,0x0A])은 100바이트 이상에서 규격과 어긋남.
            int byteCount = wordCount * 2;
            byte dcLow  = (byte)(byteCount & 0xFF);          // 하위 바이트
            byte dcHigh = (byte)((byteCount >> 8) & 0xFF);   // 상위 바이트

            // 커맨드 데이터 길이 계산
            int varStrLen   = 3 + addrBytes.Length;   // '%' + devChar + 'W' + 주소 문자열
            int cmdDataLen  = 2                        // COMMAND
                            + 2                        // DATATYPE
                            + 2                        // RESERVED
                            + 2                        // BLOCKCOUNT
                            + 2                        // VARIABLE_LENGTH
                            + varStrLen                // DEVICE(%DW) + ADDRESS
                            + 2;                       // DATACOUNT
            if (isWrite)
                cmdDataLen += writeData.Length;        // DATABLOCK

            // --- 헤더 구성 (0~18 까지 19바이트, 체크섬용) ---
            byte[] companyId  = (byte[])FENET_COMPANY_ID.Clone();
            byte[] plcInfo    = new byte[2] { 0x00, 0x00 };
            byte[] cpuInfo    = new byte[1] { 0xA0 };
            byte[] srcFrame   = new byte[1] { 0x33 };
            byte[] invokeId   = new byte[2] { invokeBase, _invokeSeq++ };
            // [BIN판] LENGTH 를 먼저 확정하고 체크섬에 포함 (XGT 규격 / ECS CLgXgtFEnetIfSk 와 동일)
            //   구판은 플레이스홀더(0x10,0x00) 상태로 합산해 체크섬이 규격과 어긋났음.
            byte[] length     = new byte[2] { (byte)(cmdDataLen & 0xFF), (byte)((cmdDataLen >> 8) & 0xFF) };
            byte[] position   = new byte[1] { 0x00 };

            // 체크섬: 바이트 0~18 합산 (확정된 LENGTH 포함)
            int checksum = 0;
            foreach (byte b in companyId) checksum += b;
            foreach (byte b in plcInfo)   checksum += b;
            foreach (byte b in cpuInfo)   checksum += b;
            foreach (byte b in srcFrame)  checksum += b;
            foreach (byte b in invokeId)  checksum += b;
            foreach (byte b in length)    checksum += b;
            foreach (byte b in position)  checksum += b;
            byte[] checksumBytes = new byte[1] { (byte)(checksum & 0xFF) };

            // 커맨드 데이터
            byte[] cmdBytes   = new byte[2] { command, 0x00 };
            byte[] dataType   = new byte[2] { 0x14, 0x00 };       // 워드 개별 접근
            byte[] reserved   = new byte[2] { 0x00, 0x00 };
            byte[] blkCount   = new byte[2] { 0x01, 0x00 };
            byte[] varLen     = new byte[2] { (byte)(varStrLen & 0xFF), 0x00 };
            // V1.1 : 논리 디바이스 문자 + 바이트 접근 (%MB/%DB/%RB)
            // V0.9 : 논리 디바이스 문자 + 워드 접근  (%MW/%DW/%RW)
            byte[] device;
            if (cDefApp.GM_ADDR_V09)
                device = new byte[3] { (byte)'%', (byte)devChar, (byte)'W' };
            else
                device = new byte[3] { (byte)'%', (byte)devChar, (byte)'B' };
            byte[] dataCount  = new byte[2] { dcLow, dcHigh };

            // 전체 프레임 조립
            List<byte[]> parts = new List<byte[]>
            {
                companyId,
                plcInfo,
                cpuInfo,
                srcFrame,
                invokeId,
                length,         // 실제 길이값으로 갱신된 LENGTH
                position,
                checksumBytes,
                cmdBytes,
                dataType,
                reserved,
                blkCount,
                varLen,
                device,
                addrBytes,
                dataCount
            };
            if (isWrite)
                parts.Add(writeData);

            int total = 0;
            foreach (byte[] p in parts) total += p.Length;
            byte[] frame = new byte[total];
            int offset = 0;
            foreach (byte[] p in parts)
            {
                Buffer.BlockCopy(p, 0, frame, offset, p.Length);
                offset += p.Length;
            }
            return frame;
        }
        #endregion

        #region 응답 검증
        protected virtual bool ValidateResponse(byte[] rxBuf, string caller)
        {
            // [BIN판] 응답 최소 길이 = 쓰기 ACK(30바이트) 기준으로 확인
            //   (에러상태[26-27]/에러코드[28-29]까지 접근하므로 30바이트면 충분.
            //    32바이트 기준이면 정상적인 쓰기 ACK 가 "길이 부족"으로 거부됨)
            int nMin = GetWriteAckSize();
            if (RcvLen < nMin)
            {
                SetErrorMsg(caller + " 응답 길이 부족(" + RcvLen + " < " + nMin + ")");
                return false;
            }

            // Company ID 확인: 응답[0-7] = 'LSIS-XGT'
            string compId = Encoding.ASCII.GetString(rxBuf, 0, 8);
            if (compId != "LSIS-XGT")
            {
                SetErrorMsg(caller + " 응답 Company ID 이상: [" + compId + "]");
                return false;
            }

            // [BIN판] 에러 상태 확인: 응답[26-27] = Error State (0x0000 = 정상)
            //   XGT 규격상 [24-25]는 예약영역이므로 구판 위치로는 NAK 를 감지 못함.
            //   NAK 시 에러 코드는 응답[28-29]에 실린다. (ECS CLgXgtFEnetIfSk 와 동일)
            int errState = rxBuf[26] | (rxBuf[27] << 8);
            if (errState != 0)
            {
                int errCode = rxBuf[28] | (rxBuf[29] << 8);
                SetErrorMsg(caller + " PLC NAK: STATE=0x" + errState.ToString("X4")
                                   + " CODE=0x" + errCode.ToString("X4"));
                return false;
            }

            return true;
        }
        #endregion

        #region 유틸리티 (MelsecQ3EProtocol 과 동일한 공개 인터페이스)
        public void ResetError()
        {
            _strErrorMsg = "";
        }

        public void SetErrorMsg(string strMsg)
        {
            _strErrorMsg = strMsg;
        }

        public string GetErrorMsg()
        {
            return _strErrorMsg;
        }

        /// <summary>
        /// MelsecQ3EProtocol.GetDataLength 와 동일 -> CvThread.cs 호환용
        /// </summary>
        public int GetDataLength(int nUnitType, int wLen)
        {
            if (nUnitType == (int)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT)
                return wLen * 2;
            else
                return (wLen / 2) + (wLen % 2);
        }

        /// <summary>
        /// MelsecQ3E 디바이스 코드 -> XGT 디바이스 문자 변환
        /// </summary>
        private char GetDeviceChar(byte deviceCode)
        {
            switch (deviceCode)
            {
                case 0xA8: return 'D';   // Melsec D -> XGT D (데이터 레지스터)
                case 0x90: return 'M';   // Melsec M -> XGT M (내부 릴레이)
                case 0xAF: return 'R';   // Melsec R -> XGT R (파일 레지스터)
                case 0xA0: return 'B';   // Melsec B (링크 릴레이)
                case 0xB4: return 'W';   // Melsec W (링크 레지스터)
                default:   return 'D';
            }
        }
        #endregion
    }
}
