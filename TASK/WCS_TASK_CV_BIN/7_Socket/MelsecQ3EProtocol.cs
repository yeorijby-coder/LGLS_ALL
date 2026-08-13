using System;
using System.Collections.Generic;
using System.Text;
using Samoh_Lib;
using System.Data;
using System.Data.OleDb;
using System.Collections;
using System.Net.Sockets;
using Samoh_Socket;
#if POSTGRESQL
using Npgsql; // [LGLS] POSTGRESQL 전용
#endif
namespace WCS_TASK_CV
{
    #region enum
    public enum MelsecQ3E_UnitType
    {
        MELSECQ_CMD_WORD_UNIT = 0,
        MELSECQ_CMD_BIT_UNIT = 1
    }
    public enum MelsecQ3E_UnitType_DEVICE
    {
        MELSECQ_DEVICE_CODE_SM = 0x91,
        MELSECQ_DEVICE_CODE_SD = 0xA9,
        MELSECQ_DEVICE_CODE_X = 0x9C,
        MELSECQ_DEVICE_CODE_Y = 0x9D,
        MELSECQ_DEVICE_CODE_M = 0x90,
        MELSECQ_DEVICE_CODE_L = 0x92,
        MELSECQ_DEVICE_CODE_F = 0x93,
        MELSECQ_DEVICE_CODE_V = 0x94,
        MELSECQ_DEVICE_CODE_B = 0xA0,
        MELSECQ_DEVICE_CODE_D = 0xA8,
        MELSECQ_DEVICE_CODE_W = 0xB4,
        MELSECQ_DEVICE_CODE_TS = 0xC1,
        MELSECQ_DEVICE_CODE_TC = 0xC0,
        MELSECQ_DEVICE_CODE_TN = 0xC2,
        MELSECQ_DEVICE_CODE_SS = 0xC7,
        MELSECQ_DEVICE_CODE_SC = 0xC6,
        MELSECQ_DEVICE_CODE_SN = 0xC8,
        MELSECQ_DEVICE_CODE_CS = 0xC4,
        MELSECQ_DEVICE_CODE_CC = 0xC3,
        MELSECQ_DEVICE_CODE_CN = 0xC5,
        MELSECQ_DEVICE_CODE_SB = 0xA1,
        MELSECQ_DEVICE_CODE_SW = 0xB5,
        MELSECQ_DEVICE_CODE_S = 0x98,
        MELSECQ_DEVICE_CODE_DX = 0xA2,
        MELSECQ_DEVICE_CODE_DY = 0xA3,
        MELSECQ_DEVICE_CODE_Z = 0xCC,
        MELSECQ_DEVICE_CODE_R = 0xAF,
        MELSECQ_DEVICE_CODE_ZR = 0xB0
    }
    #endregion

    /*
     * MelsecQ3EProtocol
     *
     * Mitsubishi MELSEC QnA-3E Frame (Binary) 프로토콜 구현.
     * FenetProtocol 을 상속하여 소켓/DB/READ/WRITE 흐름은 그대로 쓰고
     * 프레임 생성/응답 검증만 Q3E 형식으로 재정의한다.
     * (ini [PLC] MAKER=0 일 때 CvThread 가 이 클래스를 사용)
     *
     * [요청 프레임 - Binary]
     *   [0-1]   서브헤더        : 0x50 0x00
     *   [2]     네트워크 번호   : 0x00
     *   [3]     PC 번호         : 0xFF
     *   [4-5]   요구상대 모듈IO : 0xFF 0x03
     *   [6]     요구상대 국번   : 0x00
     *   [7-8]   요구데이터 길이 : 이후 바이트 수 (little-endian)
     *   [9-10]  CPU 감시 타이머 : 0x10 0x00
     *   [11-12] 커맨드          : READ 0x0401, WRITE 0x1401 (little-endian)
     *   [13-14] 서브커맨드      : 0x0000 (워드 단위)
     *   [15-17] 선두 디바이스   : 3바이트 little-endian
     *   [18]    디바이스 코드   : D=0xA8, M=0x90, R=0xAF
     *   [19-20] 디바이스 점수   : 워드 수 (little-endian)
     *   [21-]   (WRITE 시) 데이터
     *
     * [응답 프레임 - Binary]
     *   [0-1]   서브헤더        : 0xD0 0x00
     *   [2-6]   네트워크/PC/모듈IO/국번
     *   [7-8]   응답데이터 길이
     *   [9-10]  종료코드        : 0x0000 = 정상
     *   [11-]   (READ 시) 데이터 (워드수 * 2 바이트, little-endian)
     *
     * [주의] M 등 비트 디바이스를 워드 단위로 접근할 때 Melsec 은
     *        선두 디바이스 번호를 '비트 번호'로 지정해야 하므로
     *        호출측의 워드 주소를 비트 주소(*16)로 변환한다.
     *        (FenetProtocol 은 %MW 워드 주소를 그대로 쓰므로 호출측 주소 체계 동일 유지)
     */
    public class MelsecQ3EProtocol : FenetProtocol
    {
        private const int Q3E_RECV_HEADER = 11;   // 응답 고정 헤더(서브헤더~종료코드)

        #region 생성자 (FenetProtocol 동일 시그니처)
        public MelsecQ3EProtocol(string connectionString)
            : base(connectionString) { }

        public MelsecQ3EProtocol(string strId, string strName, string connectionString)
            : base(strId, strName, connectionString) { }

        public MelsecQ3EProtocol()
            : base() { }
        #endregion

        protected override int GetRecvHeaderSize()
        {
            return Q3E_RECV_HEADER;
        }

        // Q3E 는 읽기 응답 헤더와 쓰기 ACK 크기가 동일(11바이트)
        protected override int GetWriteAckSize()
        {
            return Q3E_RECV_HEADER;
        }

        #region 프레임 빌더 (QnA-3E Binary)
        protected override byte[] BuildReadFrame(byte deviceCode, int wordAddr, int wordCount)
        {
            return BuildQ3EFrame(0x0401, deviceCode, wordAddr, wordCount, null);
        }

        protected override byte[] BuildWriteFrame(byte deviceCode, int wordAddr, int wordCount, byte[] data)
        {
            return BuildQ3EFrame(0x1401, deviceCode, wordAddr, wordCount, data);
        }

        private byte[] BuildQ3EFrame(int command, byte deviceCode, int wordAddr, int wordCount, byte[] writeData)
        {
            // 비트 디바이스(M/L/F/V/B/S/X/Y 등)는 선두 디바이스가 비트 번호이므로
            // 호출측의 워드 주소를 비트 주소로 변환 (워드 디바이스 D/R/W/ZR 는 그대로)
            int devAddr = wordAddr;
            if (IsBitDevice(deviceCode))
                devAddr = wordAddr * 16;

            int dataLen = (writeData == null) ? 0 : writeData.Length;
            // 요구데이터길이 = 감시타이머(2)+커맨드(2)+서브커맨드(2)+디바이스(3)+코드(1)+점수(2)+데이터
            int reqLen = 12 + dataLen;

            byte[] frame = new byte[21 + dataLen];
            frame[0]  = 0x50; frame[1] = 0x00;                    // 서브헤더
            frame[2]  = 0x00;                                     // 네트워크 번호
            frame[3]  = 0xFF;                                     // PC 번호
            frame[4]  = 0xFF; frame[5] = 0x03;                    // 요구상대 모듈 IO
            frame[6]  = 0x00;                                     // 요구상대 국번
            frame[7]  = (byte)(reqLen & 0xFF);                    // 요구데이터 길이
            frame[8]  = (byte)((reqLen >> 8) & 0xFF);
            frame[9]  = 0x10; frame[10] = 0x00;                   // CPU 감시 타이머
            frame[11] = (byte)(command & 0xFF);                   // 커맨드
            frame[12] = (byte)((command >> 8) & 0xFF);
            frame[13] = 0x00; frame[14] = 0x00;                   // 서브커맨드 (워드 단위)
            frame[15] = (byte)(devAddr & 0xFF);                   // 선두 디바이스 (3바이트)
            frame[16] = (byte)((devAddr >> 8) & 0xFF);
            frame[17] = (byte)((devAddr >> 16) & 0xFF);
            frame[18] = deviceCode;                               // 디바이스 코드
            frame[19] = (byte)(wordCount & 0xFF);                 // 디바이스 점수
            frame[20] = (byte)((wordCount >> 8) & 0xFF);

            if (dataLen > 0)
                Buffer.BlockCopy(writeData, 0, frame, 21, dataLen);

            return frame;
        }

        private bool IsBitDevice(byte deviceCode)
        {
            switch (deviceCode)
            {
                case 0x90:  // M
                case 0x91:  // SM
                case 0x92:  // L
                case 0x93:  // F
                case 0x94:  // V
                case 0x98:  // S
                case 0x9C:  // X
                case 0x9D:  // Y
                case 0xA0:  // B
                case 0xA1:  // SB
                    return true;
                default:
                    return false;
            }
        }
        #endregion

        #region 응답 검증 (QnA-3E)
        protected override bool ValidateResponse(byte[] rxBuf, string caller)
        {
            if (RcvLen < Q3E_RECV_HEADER)
            {
                SetErrorMsg(caller + " Q3E 응답 길이 부족(" + RcvLen + " < " + Q3E_RECV_HEADER + ")");
                return false;
            }

            // 서브헤더 확인: 0xD0 0x00
            if (rxBuf[0] != 0xD0 || rxBuf[1] != 0x00)
            {
                SetErrorMsg(caller + " Q3E 응답 서브헤더 이상: "
                            + rxBuf[0].ToString("X2") + " " + rxBuf[1].ToString("X2"));
                return false;
            }

            // 종료코드 확인: [9-10] = 0x0000 정상
            int endCode = rxBuf[9] | (rxBuf[10] << 8);
            if (endCode != 0)
            {
                SetErrorMsg(caller + " Q3E 종료코드 에러: 0x" + endCode.ToString("X4"));
                return false;
            }

            return true;
        }
        #endregion
    }
}