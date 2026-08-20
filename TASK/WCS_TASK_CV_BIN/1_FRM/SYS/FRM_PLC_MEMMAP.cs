using System;
using System.Collections.Generic;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WCS_TASK_CV
{
    // PPT [ECS-분석-PLC사양및시나리오_LG생명과학_V0.9] 메모리 맵 기반 읽기/쓰기 폼
    // LS XGT PLC Fenet 프로토콜 사용 (DB 연결 불필요)
    public partial class FRM_PLC_MEMMAP : Form
    {
        private FenetProtocol _plc     = null;
        private string        _ip      = "127.0.0.1";
        private int           _port    = 3001;
        private volatile bool _closing = false;         // 폼 닫기 중 플래그
        private int           _runningTasks = 0;        // 실행 중인 백그라운드 Task 수
        private readonly object _plcLock = new object(); // PLC 요청 직렬화 (하트비트/사용자 조작 충돌 방지)
        private System.Windows.Forms.Timer _tmrHeartbeat; // 유휴 세션 유지용

        // 장치 코드 (FenetProtocol.GetDeviceChar 참조)
        private const byte DEV_M = 0x90;  // M 비트 영역
        private const byte DEV_D = 0xA8;  // D 워드 영역
        private const byte DEV_R = 0xAF;  // R 트래킹 영역

        // PPT V1.4 슬라이드 18~22: 비트 영역 (실전송 %MX, Address = M 워드주소 = 비트/16)
        private static readonly MemMapEntry[] BitMapEntries = new MemMapEntry[]
        {
            new MemMapEntry("%MX256",  256/16,  "C/V #1 Event (구 M0160)"),
            new MemMapEntry("%MX272",  272/16,  "C/V #1 PalletExist #1,#2 (구 M0170)"),
            new MemMapEntry("%MX288",  288/16,  "C/V #2 Event (구 M0180)"),
            new MemMapEntry("%MX304",  304/16,  "C/V #2 PalletExist #3,#4 (구 M0190)"),
            new MemMapEntry("%MX320",  320/16,  "C/V #3 Event (구 M0200)"),
            new MemMapEntry("%MX336",  336/16,  "C/V #3 PalletExist #5,#6 (구 M0210)"),
            new MemMapEntry("%MX352",  352/16,  "C/V #4 Event (구 M0220)"),
            new MemMapEntry("%MX368",  368/16,  "C/V #4 PalletExist #7,#8 (구 M0230)"),
            new MemMapEntry("%MX384",  384/16,  "C/V #5 Event (구 M0240)"),
            new MemMapEntry("%MX400",  400/16,  "C/V #5 PalletExist #9,#10 (구 M0250)"),
            new MemMapEntry("%MX416",  416/16,  "C/V #6 Event (구 M0260)"),
            new MemMapEntry("%MX432",  432/16,  "C/V #6 PalletExist #11,#12 (구 M0270)"),
            new MemMapEntry("%MX448",  448/16,  "C/V #7 Event (구 M0280)"),
            new MemMapEntry("%MX464",  464/16,  "C/V #7 PalletExist #13,#14 (구 M0290)"),
            new MemMapEntry("%MX480",  480/16,  "C/V #8 Event (구 M0300)"),
            new MemMapEntry("%MX496",  496/16,  "C/V #8 PalletExist #15,#16 (구 M0310)"),
            new MemMapEntry("%MX512",  512/16,  "C/V #9 Event (구 M0320)"),
            new MemMapEntry("%MX528",  528/16,  "C/V #9 PalletExist #17,#18 (구 M0330)"),
            new MemMapEntry("%MX544",  544/16,  "C/V #10 Event (구 M0340)"),
            new MemMapEntry("%MX560",  560/16,  "C/V #10 PalletExist #19,#20 (구 M0350)"),
            new MemMapEntry("%MX576",  576/16,  "C/V #11 Event (구 M0360)"),
            new MemMapEntry("%MX592",  592/16,  "C/V #11 PalletExist #21,#22 (구 M0370)"),
            new MemMapEntry("%MX608",  608/16,  "C/V #12 Event (구 M0380)"),
            new MemMapEntry("%MX624",  624/16,  "C/V #12 PalletExist #23,#24 (구 M0390)"),
            new MemMapEntry("%MX640",  640/16,  "C/V #13 Event (구 M0400)"),
            new MemMapEntry("%MX656",  656/16,  "C/V #13 PalletExist #25,#26 (구 M0410)"),
            new MemMapEntry("%MX672",  672/16,  "C/V #14 Event (구 M0420)"),
            new MemMapEntry("%MX688",  688/16,  "C/V #14 PalletExist #27,#28 (구 M0430)"),
            new MemMapEntry("%MX704",  704/16,  "C/V #15 Event (구 M0440)"),
            new MemMapEntry("%MX720",  720/16,  "C/V #15 PalletExist #31,#32 (구 M0450)"),
            new MemMapEntry("%MX768",  768/16,  "S/C #1 Status(PalletPresence) (구 M0480)"),
            new MemMapEntry("%MX784",  784/16,  "S/C #1 Event(Load/Unload/Alarm/Transfer) (구 M0490)"),
            new MemMapEntry("%MX800",  800/16,  "S/C #2 PalletPresence (구 M0500)"),
            new MemMapEntry("%MX816",  816/16,  "S/C #2 Event (구 M0510)"),
            new MemMapEntry("%MX832",  832/16,  "S/C #3 PalletPresence (구 M0520)"),
            new MemMapEntry("%MX848",  848/16,  "S/C #3 Event (구 M0530)"),
            new MemMapEntry("%MX864",  864/16,  "S/C #4 PalletPresence (구 M0540)"),
            new MemMapEntry("%MX880",  880/16,  "S/C #4 Event (구 M0550)"),
            new MemMapEntry("%MX896",  896/16,  "S/C #5 PalletPresence (구 M0560)"),
            new MemMapEntry("%MX912",  912/16,  "S/C #5 Event (구 M0570)"),
            new MemMapEntry("%MX928",  928/16,  "RGV #1 PalletPresence (구 M0580)"),
            new MemMapEntry("%MX944",  944/16,  "RGV #1 Event(Load/Unload/Alarm/Transfer) (구 M0590)"),
            new MemMapEntry("%MX1280", 1280/16, "C/V #1  Ack(+1:UnloadAck#1 +2:LoadAck#1 +3:UnloadAck#2 +4:LoadAck#2) (구 M0800)"),
            new MemMapEntry("%MX1296", 1296/16, "C/V #2  Ack (구 M0810)"),
            new MemMapEntry("%MX1312", 1312/16, "C/V #3  Ack (구 M0820)"),
            new MemMapEntry("%MX1328", 1328/16, "C/V #4  Ack (구 M0830)"),
            new MemMapEntry("%MX1344", 1344/16, "C/V #5  Ack (구 M0840)"),
            new MemMapEntry("%MX1360", 1360/16, "C/V #6  Ack (구 M0850)"),
            new MemMapEntry("%MX1376", 1376/16, "C/V #7  Ack (구 M0860)"),
            new MemMapEntry("%MX1392", 1392/16, "C/V #8  Ack (구 M0870)"),
            new MemMapEntry("%MX1408", 1408/16, "C/V #9  Ack (구 M0880)"),
            new MemMapEntry("%MX1424", 1424/16, "C/V #10 Ack (구 M0890)"),
            new MemMapEntry("%MX1440", 1440/16, "C/V #11 Ack (구 M0900)"),
            new MemMapEntry("%MX1456", 1456/16, "C/V #12 Ack (구 M0910)"),
            new MemMapEntry("%MX1472", 1472/16, "C/V #13 Ack (구 M0920)"),
            new MemMapEntry("%MX1488", 1488/16, "C/V #14 Ack +5:UnloadReq#2 (구 M0930)"),
            new MemMapEntry("%MX1504", 1504/16, "C/V #15 Ack (구 M0940)"),
            new MemMapEntry("%MX1536", 1536/16, "S/C #1 Cmd(+0:Transfer +1:LoadAck +2:UnloadAck +3:AlarmSetAck +4:AlarmRstAck +5:TransferAck) (구 M0960)"),
            new MemMapEntry("%MX1552", 1552/16, "S/C #2 Cmd (구 M0970)"),
            new MemMapEntry("%MX1568", 1568/16, "S/C #3 Cmd (구 M0980)"),
            new MemMapEntry("%MX1584", 1584/16, "S/C #4 Cmd (구 M0990)"),
            new MemMapEntry("%MX1600", 1600/16, "S/C #5 Cmd (구 M1000)"),
            new MemMapEntry("%MX1616", 1616/16, "RGV #1 Cmd (구 M1010)"),
        };

        // PPT V1.4 슬라이드 23~25: 워드 영역 (실전송 %DB = 워드주소×2, Address = 워드주소)
        private static readonly MemMapEntry[] WordMapEntries = new MemMapEntry[]
        {
            new MemMapEntry("%DB512",  512/2,  "S/C #1 +0:RunIdleDown +1:SetAlarm +2:ResetAlarm +3~5:TransferCompLoc +6~8:Location +9:OpMode (구 D0160)"),
            new MemMapEntry("%DB544",  544/2,  "S/C #2 상태 (구 D0170)"),
            new MemMapEntry("%DB576",  576/2,  "S/C #3 상태 (구 D0180)"),
            new MemMapEntry("%DB608",  608/2,  "S/C #4 상태 (구 D0190)"),
            new MemMapEntry("%DB640",  640/2,  "S/C #5 상태 (구 D0200)"),
            new MemMapEntry("%DB672",  672/2,  "RGV #1 상태(RunIdleDown/AlarmCode/...) (구 D0210, %DW336)"),
            new MemMapEntry("%DB960",  960/2,  "C/V #1  방향(0=입고, 1=출고) (구 D0300)"),
            new MemMapEntry("%DB962",  962/2,  "C/V #2  방향 (구 D0301)"),
            new MemMapEntry("%DB964",  964/2,  "C/V #3  방향 (구 D0302)"),
            new MemMapEntry("%DB966",  966/2,  "C/V #4  방향 (구 D0303)"),
            new MemMapEntry("%DB968",  968/2,  "C/V #5  방향 (구 D0304)"),
            new MemMapEntry("%DB970",  970/2,  "C/V #6  방향 (구 D0305)"),
            new MemMapEntry("%DB972",  972/2,  "C/V #7  방향 (구 D0306)"),
            new MemMapEntry("%DB974",  974/2,  "C/V #8  방향 (구 D0307)"),
            new MemMapEntry("%DB976",  976/2,  "C/V #9  방향 (구 D0308)"),
            new MemMapEntry("%DB978",  978/2,  "C/V #10 방향 (구 D0309)"),
            new MemMapEntry("%DB980",  980/2,  "C/V #11 방향(입고=0, 출고=1) (구 D0310, %DW490)"),
            new MemMapEntry("%DB982",  982/2,  "C/V #12 방향 (구 D0311)"),
            new MemMapEntry("%DB984",  984/2,  "C/V #13 방향 (구 D0312)"),
            new MemMapEntry("%DB986",  986/2,  "C/V #14 방향 (구 D0313)"),
            new MemMapEntry("%DB988",  988/2,  "C/V #15 방향 (구 D0314)"),
            new MemMapEntry("%DB1536", 1536/2, "S/C #1 ECS Cmd: +0~2:From +3~5:To +6~7:JobNo (구 D0320)"),
            new MemMapEntry("%DB1568", 1568/2, "S/C #2 ECS Cmd (구 D0330)"),
            new MemMapEntry("%DB1600", 1600/2, "S/C #3 ECS Cmd (구 D0340)"),
            new MemMapEntry("%DB1632", 1632/2, "S/C #4 ECS Cmd (구 D0350)"),
            new MemMapEntry("%DB1664", 1664/2, "S/C #5 ECS Cmd (구 D0360)"),
            new MemMapEntry("%DB1696", 1696/2, "RGV #1 ECS Cmd(From/To/JobNo) (구 D0370)"),
        };

        // PPT V1.4 슬라이드 26: R 트래킹 영역 (10진 워드주소, 실전송 %RB = 주소×2)
        private static readonly MemMapEntry[] TrackingMapEntries = new MemMapEntry[]
        {
            new MemMapEntry("R0000 (%RB0)", 0,   "C/V #1  Position #1  JobNo Word1"),
            new MemMapEntry("R0001 (%RB2)", 1,   "C/V #1  Position #1  JobNo Word2"),
            new MemMapEntry("R0002 (%RB4)", 2,   "C/V #1  Position #2  JobNo Word1"),
            new MemMapEntry("R0003 (%RB6)", 3,   "C/V #1  Position #2  JobNo Word2"),
            new MemMapEntry("R0040 (%RB80)", 40,  "C/V #5  Position #9  JobNo Word1"),
            new MemMapEntry("R0041 (%RB82)", 41,  "C/V #5  Position #9  JobNo Word2"),
            new MemMapEntry("R0100 (%RB200)", 100, "C/V #11 Position #21 JobNo Word1"),
            new MemMapEntry("R0101 (%RB202)", 101, "C/V #11 Position #21 JobNo Word2"),
            new MemMapEntry("R0102 (%RB204)", 102, "C/V #11 Position #22 JobNo Word1"),
            new MemMapEntry("R0103 (%RB206)", 103, "C/V #11 Position #22 JobNo Word2"),
            new MemMapEntry("R0130 (%RB260)", 130, "C/V #14 Position #27 JobNo Word1"),
            new MemMapEntry("R0131 (%RB262)", 131, "C/V #14 Position #27 JobNo Word2"),
            new MemMapEntry("R0132 (%RB264)", 132, "C/V #14 Position #28 JobNo Word1"),
            new MemMapEntry("R0133 (%RB266)", 133, "C/V #14 Position #28 JobNo Word2"),
            new MemMapEntry("R0134 (%RB268)", 134, "C/V #14 Position #29 JobNo Word1"),
            new MemMapEntry("R0135 (%RB270)", 135, "C/V #14 Position #29 JobNo Word2"),
            new MemMapEntry("R0300 (%RB600)", 300, "S/C #1  Tracking Word1"),
            new MemMapEntry("R0301 (%RB602)", 301, "S/C #1  Tracking Word2"),
            new MemMapEntry("R0302 (%RB604)", 302, "S/C #2  Tracking Word1"),
            new MemMapEntry("R0303 (%RB606)", 303, "S/C #2  Tracking Word2"),
            new MemMapEntry("R0304 (%RB608)", 304, "S/C #3  Tracking Word1"),
            new MemMapEntry("R0305 (%RB610)", 305, "S/C #3  Tracking Word2"),
            new MemMapEntry("R0306 (%RB612)", 306, "S/C #4  Tracking Word1"),
            new MemMapEntry("R0307 (%RB614)", 307, "S/C #4  Tracking Word2"),
            new MemMapEntry("R0308 (%RB616)", 308, "S/C #5  Tracking Word1"),
            new MemMapEntry("R0309 (%RB618)", 309, "S/C #5  Tracking Word2"),
            new MemMapEntry("R0310 (%RB620)", 310, "RGV #1  Tracking Word1"),
            new MemMapEntry("R0311 (%RB622)", 311, "RGV #1  Tracking Word2"),
        };

        // 트래킹: 논리 R워드번호 + 실전송 %RB(바이트=×2) 표기
        private static MemMapEntry[] TrackingLabels(MemMapEntry[] src)
        {
            var list = new List<MemMapEntry>();
            foreach (var e in src)
            {
                // [LGLS 2026-08-19] 표의 Address 는 '문서 표기'. 실 워드주소는 R 주소모드를 따른다.
                //   HEX(구 ECS 호환) : R0100 → 0x100 = 256,  DEC(현행) : R0100 → 100
                int nWord = cDefApp.GsRTrackWord(e.Address);
                string strBase = e.Name.Split(' ')[0];   // "R0000"
                string strName = strBase + " (%RB" + (nWord * 2) + ")";
                list.Add(new MemMapEntry(strName, nWord, e.Description));
            }
            return list.ToArray();
        }

        public FRM_PLC_MEMMAP()
        {
            InitializeComponent();
            txtIp.Text   = _ip;
            txtPort.Text = _port.ToString();
            LoadGrids();
            LoadAddrXmlText();   // [LGLS 2026-08-19] 주소맵 XML 탭 내용 채우기
            // [LGLS 2026-08-19] 메인화면 R주소 라디오 변경 → 이 창으로 돌아올 때 자동 갱신
            this.Activated += new EventHandler(FRM_PLC_MEMMAP_Activated);
            TryAutoConnect();

            // ─── 하트비트: 3초마다 1워드 읽기로 세션 유지 ───────────────────
            // 실 PLC가 유휴 세션을 정리하므로 실제 FEnet 프레임을 주기 전송한다.
            _tmrHeartbeat = new System.Windows.Forms.Timer();
            _tmrHeartbeat.Interval = 3000;
            _tmrHeartbeat.Tick += Heartbeat_Tick;
            _tmrHeartbeat.Start();
        }

        // ─── TASK 접속정보(ini COMM0)로 자동 접속 ────────────────────────────
        // TASK가 사용하는 것과 동일한 접속정보를 쓰므로, 자동 접속에 성공하면
        // 접속 관련 입력/버튼을 비활성화한다.
        private void TryAutoConnect()
        {
            try
            {
                string strPlcNo = "", strIp = "", strLogPath = "", strFileNm = "", strEqmt = "", strMsg = "";
                int nCur = 0, nFrom = 0, nTo = 0, nPortCnt = 0, nCnt = 0, nIdx = 0, nFrTrack = 0, nToTrack = 0;

                cDefApi.GsReadInitProfileCom("COMM0",
                                             ref strPlcNo, ref strIp,
                                             ref nCur, ref nFrom, ref nTo,
                                             ref nPortCnt, ref nCnt, ref nIdx,
                                             ref nFrTrack, ref nToTrack,
                                             ref strLogPath, ref strFileNm, ref strEqmt,
                                             ref strMsg);

                if (string.IsNullOrEmpty(strIp) || nCur <= 0)
                    return;   // ini 정보 없음 - 수동 접속 사용

                txtIp.Text   = strIp;
                txtPort.Text = nCur.ToString();

                _plc = new FenetProtocol();
                string rtn = "";
                if (_plc.OpenSocketOnly(strIp, nCur, ref rtn))
                {
                    _ip = strIp; _port = nCur;
                    SetConnStatus(true);
                    SetConnPanelEnabled(false);   // TASK와 동일 접속 - 접속 정보 비활성화
                    AppendLog(string.Format("[자동 연결] TASK 설정(COMM0) {0}:{1} 접속 성공 - 접속 정보 잠금", strIp, nCur));
                }
                else
                {
                    _plc = null;
                    SetConnStatus(false);
                    AppendLog("[자동 연결 실패] " + rtn + " → 접속 정보를 확인 후 수동으로 [연결] 하세요.");
                }
            }
            catch (Exception ex)
            {
                AppendLog("[자동 연결] 예외: " + ex.Message);
            }
        }

        // 접속 패널 일괄 활성/비활성 (자동 접속 성공 시 잠금)
        private void SetConnPanelEnabled(bool bEnabled)
        {
            txtIp.Enabled         = bEnabled;
            txtPort.Enabled       = bEnabled;
            btnConnect.Enabled    = bEnabled;
            btnDisconnect.Enabled = bEnabled;
        }

        private void Heartbeat_Tick(object sender, EventArgs e)
        {
            if (_closing) return;
            var plc = _plc;
            if (plc == null || !plc.m_bSocCon) return;
            if (_runningTasks > 0) return;   // 사용자 Read/Write 진행 중이면 생략

            RunPlcTask(() =>
            {
                bool ok = false;
                string err = "";
                try
                {
                    byte[] buf = new byte[2];
                    var p = _plc;
                    if (p != null)
                    {
                        ok = p.READ(0, DEV_D, 0, 1, ref buf);   // 1워드 - 무해한 세션 유지 읽기
                        if (!ok) err = p.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                if (!ok)
                    SafeInvoke(() =>
                    {
                        AppendLog("[하트비트] 세션 끊김 감지");
                        TryRecover(err.Length > 0 ? err : "Send 실패");
                    });
            });
        }

        // ─── 끊김 감지 시 자동 재연결 (UI 스레드에서 호출) ──────────────────
        private void TryRecover(string strErr)
        {
            if (string.IsNullOrEmpty(strErr)) return;
            if (strErr.IndexOf("끊") < 0 && strErr.IndexOf("Send 실패") < 0 && strErr.IndexOf("Recv") < 0)
                return;

            AppendLog("[재연결] 원격 종료 감지 → 재접속 시도...");
            ForceCloseSocket();

            var plc = new FenetProtocol();
            string msg = "";
            if (plc.OpenSocketOnly(_ip, _port, ref msg))
            {
                _plc = plc;
                SetConnStatus(true);
                AppendLog(string.Format("[재연결] {0}:{1} 성공 - 다시 Read/Write 하세요.", _ip, _port));
            }
            else
            {
                SetConnStatus(false);
                SetConnPanelEnabled(true);
                AppendLog("[재연결] 실패: " + msg + " → 접속 정보 잠금 해제 (PLC 접속수/호스트테이블 확인 필요)");
            }
        }

        // 그리드/탭 구성 (V1.1 매핑 고정 — V0.9는 2026-08-10 실 현장 판정으로 폐기)
        // [LGLS 2026-08-19] 그리드를 채울 때 적용된 R 주소모드. 창이 활성화될 때 이 값과 현재 설정을
        //   비교해 달라졌으면 자동으로 다시 채운다(메인화면 라디오로 바꾼 뒤 창을 다시 열 필요 없음).
        private bool _loadedRHex = true;

        // ─────────────────────────────────────────────────────────────────────
        // [LGLS 2026-08-19] 주소맵 XML(7_DeviceMap\PlcAddressMap.xml)에서 표를 만든다.
        //   XML 이 없거나 해당 디바이스 블록이 없으면 아래 내장 배열로 폴백한다.
        // ─────────────────────────────────────────────────────────────────────
        private static MemMapEntry[] EntriesFromXml(char device, MemMapEntry[] fallback)
        {
            try
            {
                if (!cPlcAddrMap.IsLoaded) return fallback;
                var list = new List<MemMapEntry>();
                foreach (string strTyp in cPlcAddrMap.GroupTypes())
                {
                    int nFrom, nCnt;
                    var blocks = cPlcAddrMap.BlocksOf(strTyp, out nFrom, out nCnt);
                    foreach (var b in blocks)
                    {
                        if (b.Device != device) continue;
                        for (int n = nFrom; n < nFrom + nCnt; n++)
                        {
                            int nBase = cPlcAddrMap.BlockBase(strTyp, n, b.Name);
                            if (nBase < 0) continue;
                            string strLegacy = string.IsNullOrEmpty(b.Legacy) ? "" : "  (구 " + b.Legacy + ")";
                            string strDesc = strTyp + " #" + n + "  " + b.Name
                                           + (string.IsNullOrEmpty(b.Desc) ? "" : " - " + b.Desc) + strLegacy;
                            if (device == 'M')
                                list.Add(new MemMapEntry("%MX" + nBase, nBase / 16, strDesc));
                            else if (device == 'D')
                                list.Add(new MemMapEntry("%DB" + (nBase * 2), nBase, strDesc));
                            else   // R : 슬롯별로 펼친다 (문서표기 + 실전송 %RB)
                            {
                                int nSlots = (b.MaxSlots > 0) ? b.MaxSlots : 1;
                                int nWords = (b.PerSlotWords > 0) ? b.PerSlotWords : 1;
                                for (int s = 0; s < nSlots; s++)
                                {
                                    for (int w = 0; w < nWords; w++)
                                    {
                                        int nDoc  = b.Origin + (n - nFrom) * b.Stride + s * nWords + w;
                                        int nWord = cDefApp.GsRTrackWord(nDoc);
                                        list.Add(new MemMapEntry(
                                            string.Format("R{0:0000} (%RB{1})", nDoc, nWord * 2), nWord,
                                            string.Format("{0} #{1}  Position #{2} Word{3}{4}", strTyp, n, s + 1, w + 1, strLegacy)));
                                    }
                                }
                            }
                        }
                    }
                }
                if (list.Count == 0) return fallback;
                return list.ToArray();
            }
            catch { return fallback; }
        }

        private void LoadGrids()
        {
            tabBit.Text      = "Bit 영역 (%MX)";
            tabWord.Text     = "Word 영역 (%DB)";
            tabTracking.Text = "Tracking 영역 (R→%RB)";
            // 주소맵 XML 우선, 없으면 내장 배열
            PopulateGrid(dgvBit,      EntriesFromXml('M', BitMapEntries));
            PopulateGrid(dgvWord,     EntriesFromXml('D', WordMapEntries));
            MemMapEntry[] trk = EntriesFromXml('R', null);
            PopulateGrid(dgvTracking, (trk != null) ? trk : TrackingLabels(TrackingMapEntries));
            _loadedRHex = cDefApp.GM_R_ADDR_HEX;
            // 현재 어느 R 주소모드/주소맵으로 보고 있는지 제목에 표시
            this.Text = "LS XGT PLC 메모리 맵 읽기/쓰기   [R주소: " + cDefApp.GsRAddrModeText() + "]"
                      + "   [" + cPlcAddrMap.StatusText() + "]";
        }

        /// <summary>
        /// [LGLS 2026-08-19] 창이 활성화될 때 R 주소모드 변경을 감지해 트래킹 주소를 자동 갱신한다.
        /// (메인화면 라디오를 바꾸고 이 창으로 돌아오면 즉시 반영)
        /// </summary>
        private void FRM_PLC_MEMMAP_Activated(object sender, EventArgs e)
        {
            RefreshRMode();
        }

        // ═════════════════════════════════════════════════════════════════════
        // [LGLS 2026-08-19] 주소맵 XML 보기 / 편집
        //   · 저장 + 적용 : XML 문법 검사 → .bak 백업 → 저장 → 재로드 → 모든 표 즉시 갱신
        //   · 다시 읽기   : 파일 내용으로 되돌림(편집 취소)
        //   · 메모장 열기 / 폴더 열기
        // ═════════════════════════════════════════════════════════════════════
        private void LoadAddrXmlText()
        {
            try
            {
                string strPath = cPlcAddrMap.FilePath;
                lblXmlPath.Text = strPath + "    —    " + cPlcAddrMap.StatusText();
                if (System.IO.File.Exists(strPath))
                    txtAddrXml.Text = System.IO.File.ReadAllText(strPath, System.Text.Encoding.UTF8);
                else
                    txtAddrXml.Text = "// 파일이 없습니다: " + strPath + Environment.NewLine
                                    + "// (현재는 소스 내장 기본값으로 동작 중입니다)";
                txtAddrXml.SelectionStart = 0;
                txtAddrXml.SelectionLength = 0;
            }
            catch (Exception ex) { AppendLog("[주소맵 XML] 읽기 실패: " + ex.Message); }
        }

        private void btnXmlReload_Click(object sender, EventArgs e)
        {
            LoadAddrXmlText();
            AppendLog("[주소맵 XML] 파일 내용을 다시 읽었습니다.");
        }

        private void btnXmlApply_Click(object sender, EventArgs e)
        {
            string strPath = cPlcAddrMap.FilePath;
            try
            {
                // 1) XML 문법 검사 — 깨진 내용을 저장해 통신이 멎는 것을 막는다
                System.Xml.XmlDocument chk = new System.Xml.XmlDocument();
                chk.LoadXml(txtAddrXml.Text);
                if (chk.SelectSingleNode("/PlcAddressMap") == null)
                    throw new Exception("루트 요소 <PlcAddressMap> 이 없습니다.");
            }
            catch (Exception ex)
            {
                MessageBox.Show("XML 형식 오류로 저장하지 않았습니다.\r\n\r\n" + ex.Message,
                                "주소맵 XML", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                AppendLog("[주소맵 XML] 형식 오류 — 저장 취소: " + ex.Message);
                return;
            }

            try
            {
                // 2) 백업 후 저장
                if (System.IO.File.Exists(strPath))
                    System.IO.File.Copy(strPath, strPath + ".bak", true);
                System.IO.File.WriteAllText(strPath, txtAddrXml.Text, new System.Text.UTF8Encoding(false));

                // 3) 재로드 + 화면 즉시 갱신 (통신부는 다음 사이클부터 새 주소 사용)
                cPlcAddrMap.Reload();
                LoadGrids();
                LoadAddrXmlText();

                if (!cPlcAddrMap.IsLoaded)
                {
                    MessageBox.Show("저장은 됐지만 주소맵을 읽지 못했습니다.\r\n" + cPlcAddrMap.LoadError
                                  + "\r\n\r\n종전 내장 기본값으로 동작합니다.",
                                    "주소맵 XML", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                else
                {
                    AppendLog("[주소맵 XML] 저장 + 적용 완료 — " + cPlcAddrMap.StatusText()
                            + "  (백업: " + System.IO.Path.GetFileName(strPath) + ".bak)");
                    MessageBox.Show("저장하고 적용했습니다.\r\n" + cPlcAddrMap.StatusText()
                                  + "\r\n\r\n· 이 창의 표는 즉시 갱신되었습니다."
                                  + "\r\n· 자동운전 통신은 다음 스캔 주기부터 새 주소를 사용합니다."
                                  + "\r\n· 시나리오 테스트 창은 다시 열면 반영됩니다.",
                                    "주소맵 XML", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("저장 실패: " + ex.Message, "주소맵 XML",
                                MessageBoxButtons.OK, MessageBoxIcon.Error);
                AppendLog("[주소맵 XML] 저장 실패: " + ex.Message);
            }
        }

        private void btnXmlNotepad_Click(object sender, EventArgs e)
        {
            try
            {
                string strPath = cPlcAddrMap.FilePath;
                if (!System.IO.File.Exists(strPath))
                { MessageBox.Show("파일이 없습니다.\r\n" + strPath, "주소맵 XML"); return; }
                System.Diagnostics.Process.Start("notepad.exe", "\"" + strPath + "\"");
                AppendLog("[주소맵 XML] 메모장으로 열었습니다. 편집 후 [다시 읽기]를 누르세요.");
            }
            catch (Exception ex) { MessageBox.Show("메모장 실행 실패: " + ex.Message, "주소맵 XML"); }
        }

        private void btnXmlFolder_Click(object sender, EventArgs e)
        {
            try
            {
                string strPath = cPlcAddrMap.FilePath;
                if (System.IO.File.Exists(strPath))
                    System.Diagnostics.Process.Start("explorer.exe", "/select,\"" + strPath + "\"");
                else
                {
                    string strDir = System.IO.Path.GetDirectoryName(strPath);
                    if (System.IO.Directory.Exists(strDir))
                        System.Diagnostics.Process.Start("explorer.exe", "\"" + strDir + "\"");
                    else
                        MessageBox.Show("경로를 찾을 수 없습니다.\r\n" + strPath, "주소맵 XML");
                }
            }
            catch (Exception ex) { MessageBox.Show("탐색기 실행 실패: " + ex.Message, "주소맵 XML"); }
        }

        /// <summary>
        /// [LGLS 2026-08-19] R 주소모드가 바뀌었으면 트래킹 그리드를 다시 채운다.
        /// 메인화면 라디오 변경 시 SYS_MAIN 이 직접 호출하고, 창 활성화 시에도 한 번 더 확인한다.
        /// (다른 스레드에서 불릴 수 있으므로 Invoke 로 UI 스레드에 위임)
        /// </summary>
        public void RefreshRMode()
        {
            try
            {
                if (this.IsDisposed || !this.IsHandleCreated) return;
                if (this.InvokeRequired) { this.BeginInvoke(new MethodInvoker(RefreshRMode)); return; }
                if (_loadedRHex == cDefApp.GM_R_ADDR_HEX) return;
                LoadGrids();
                AppendLog("[R주소모드] " + cDefApp.GsRAddrModeText()
                        + " 로 변경되어 트래킹 주소를 갱신했습니다. (예: R0100 → %RB"
                        + (cDefApp.GsRTrackWord(100) * 2).ToString() + ")");
            }
            catch { }
        }

        // ─── 연결 ──────────────────────────────────────────────────────────────
        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (_plc != null && _plc.m_bSocCon)
            {
                AppendLog("이미 연결되어 있습니다.");
                return;
            }
            _ip   = txtIp.Text.Trim();
            if (!int.TryParse(txtPort.Text.Trim(), out _port))
            {
                AppendLog("[연결 오류] 포트 번호가 잘못되었습니다.");
                return;
            }
            _plc = new FenetProtocol();
            string msg = "";
            if (_plc.OpenSocketOnly(_ip, _port, ref msg))
            {
                SetConnStatus(true);
                AppendLog(string.Format("[연결 성공] {0}:{1}", _ip, _port));
            }
            else
            {
                _plc = null;
                SetConnStatus(false);
                AppendLog("[연결 실패] " + msg);
            }
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            ForceCloseSocket();
            SetConnStatus(false);
            AppendLog("연결 해제");
        }

        // 소켓을 강제로 닫고 _plc 를 null 로 만든다 (중복 호출 안전)
        private void ForceCloseSocket()
        {
            var plc = Interlocked.Exchange(ref _plc, null);
            if (plc != null)
            {
                try { plc.CloseSocket(); } catch { }
            }
        }

        // ─── Bit 영역 (M) 읽기 ────────────────────────────────────────────────
        private void btnReadBit_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;
            SetReadButtons(false);
            var rows = GetDgvRows(dgvBit);
            RunPlcTask(() =>
            {
                var results = new List<Tuple<int, bool, int>>();
                string firstErr = "";
                foreach (var kv in rows)
                {
                    if (_closing) break;
                    byte[] buf = new byte[2];
                    var plc = _plc;
                    bool ok = (plc != null) && plc.READ(0, DEV_M, kv.Key, 1, ref buf);
                    LogProtocol(plc, "Bit");
                    if (!ok)
                    {
                        if (firstErr.Length == 0 && plc != null)
                            firstErr = plc.GetErrorMsg();
                        results.Add(Tuple.Create(kv.Value, false, -1));
                        break;   // 첫 실패 시 중단
                    }
                    int raw = (buf[1] << 8) | buf[0];
                    results.Add(Tuple.Create(kv.Value, true, raw));
                }
                string capturedErr = firstErr;
                SafeInvoke(() =>
                {
                    foreach (var r in results)
                    {
                        if (r.Item2)
                        {
                            dgvBit.Rows[r.Item1].Cells["colBitRaw"].Value    = string.Format("0x{0:X4}", r.Item3);
                            dgvBit.Rows[r.Item1].Cells["colBitDec"].Value    = r.Item3.ToString();
                            dgvBit.Rows[r.Item1].Cells["colBitBinary"].Value = Convert.ToString(r.Item3, 2).PadLeft(16, '0');
                        }
                        else
                        {
                            dgvBit.Rows[r.Item1].Cells["colBitRaw"].Value    = "ERR";
                            dgvBit.Rows[r.Item1].Cells["colBitDec"].Value    = "";
                            dgvBit.Rows[r.Item1].Cells["colBitBinary"].Value = "";
                        }
                    }
                    bool anyErr = results.Exists(r => !r.Item2);
                    if (anyErr)
                    {
                        AppendLog(string.Format("[Bit Read] 실패: {0}", capturedErr.Length > 0 ? capturedErr : "알 수 없는 오류"));
                        TryRecover(capturedErr);
                    }
                    else
                        AppendLog(string.Format("[Bit Read] {0}개 항목 완료", results.Count));
                    SetReadButtons(true);
                });
            });
        }

        // ─── Word 영역 (D) 읽기 ───────────────────────────────────────────────
        private void btnReadWord_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;
            SetReadButtons(false);
            var rows = GetDgvRows(dgvWord);
            RunPlcTask(() =>
            {
                var results = new List<Tuple<int, bool, int>>();
                string firstErr = "";
                foreach (var kv in rows)
                {
                    if (_closing) break;
                    byte[] buf = new byte[2];
                    var plc = _plc;
                    bool ok = (plc != null) && plc.READ(0, DEV_D, kv.Key, 1, ref buf);
                    LogProtocol(plc, "Word");
                    if (!ok)
                    {
                        if (firstErr.Length == 0 && plc != null)
                            firstErr = plc.GetErrorMsg();
                        results.Add(Tuple.Create(kv.Value, false, -1));
                        break;
                    }
                    int raw = (buf[1] << 8) | buf[0];
                    results.Add(Tuple.Create(kv.Value, true, raw));
                }
                string capturedErr = firstErr;
                SafeInvoke(() =>
                {
                    foreach (var r in results)
                    {
                        if (r.Item2)
                        {
                            dgvWord.Rows[r.Item1].Cells["colWordRaw"].Value = string.Format("0x{0:X4}", r.Item3);
                            dgvWord.Rows[r.Item1].Cells["colWordDec"].Value = r.Item3.ToString();
                        }
                        else
                        {
                            dgvWord.Rows[r.Item1].Cells["colWordRaw"].Value = "ERR";
                            dgvWord.Rows[r.Item1].Cells["colWordDec"].Value = "";
                        }
                    }
                    bool anyErr = results.Exists(r => !r.Item2);
                    if (anyErr)
                    {
                        AppendLog(string.Format("[Word Read] 실패: {0}", capturedErr.Length > 0 ? capturedErr : "알 수 없는 오류"));
                        TryRecover(capturedErr);
                    }
                    else
                        AppendLog(string.Format("[Word Read] {0}개 항목 완료", results.Count));
                    SetReadButtons(true);
                });
            });
        }

        // ─── Tracking 영역 (R) 읽기 ──────────────────────────────────────────
        private void btnReadTracking_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;
            SetReadButtons(false);
            var rows = GetDgvRows(dgvTracking);
            RunPlcTask(() =>
            {
                var results = new List<Tuple<int, bool, int>>();
                string firstErr = "";
                foreach (var kv in rows)
                {
                    if (_closing) break;
                    byte[] buf = new byte[2];
                    var plc = _plc;
                    bool ok = (plc != null) && plc.READ(0, DEV_R, kv.Key, 1, ref buf);
                    LogProtocol(plc, "Track");
                    if (!ok)
                    {
                        if (firstErr.Length == 0 && plc != null)
                            firstErr = plc.GetErrorMsg();
                        results.Add(Tuple.Create(kv.Value, false, -1));
                        break;
                    }
                    int raw = (buf[1] << 8) | buf[0];
                    results.Add(Tuple.Create(kv.Value, true, raw));
                }
                string capturedErr = firstErr;
                SafeInvoke(() =>
                {
                    foreach (var r in results)
                    {
                        if (r.Item2)
                        {
                            dgvTracking.Rows[r.Item1].Cells["colTrackRaw"].Value = string.Format("0x{0:X4}", r.Item3);
                            dgvTracking.Rows[r.Item1].Cells["colTrackDec"].Value = r.Item3.ToString();
                        }
                        else
                        {
                            dgvTracking.Rows[r.Item1].Cells["colTrackRaw"].Value = "ERR";
                            dgvTracking.Rows[r.Item1].Cells["colTrackDec"].Value = "";
                        }
                    }
                    bool anyErr = results.Exists(r => !r.Item2);
                    if (anyErr)
                    {
                        AppendLog(string.Format("[Tracking Read] 실패: {0}", capturedErr.Length > 0 ? capturedErr : "알 수 없는 오류"));
                        TryRecover(capturedErr);
                    }
                    else
                        AppendLog(string.Format("[Tracking Read] {0}개 항목 완료", results.Count));
                    SetReadButtons(true);
                });
            });
        }

        // ─── 탭 Read 결과 Write (선택 행) ─────────────────────────────────────
        private void btnWriteBit_Click(object sender, EventArgs e)
        {
            WriteSelected(dgvBit, DEV_M, txtWriteBitVal);
        }
        private void btnWriteWord_Click(object sender, EventArgs e)
        {
            WriteSelected(dgvWord, DEV_D, txtWriteWordVal);
        }
        private void btnWriteTracking_Click(object sender, EventArgs e)
        {
            WriteSelected(dgvTracking, DEV_R, txtWriteTrackVal);
        }

        // ─── CV 데이터 쓰기 (V1.1 시나리오: ECS→PLC 쓰기 세트) ──────────────
        // 방향워드: %DB(960 + (N-1)*2) = 워드주소 480+(N-1), 값 0=입고/1=출고
        // 트래킹  : R(N-1)*10 부터 JOB NO 2워드 (JOB 1234 → BCD 쌍교환 2143)
        private void btnCvWrite11_Click(object sender, EventArgs e) { WriteCvDataSet(11); }
        private void btnCvWrite12_Click(object sender, EventArgs e) { WriteCvDataSet(12); }
        private void btnCvWrite15_Click(object sender, EventArgs e) { WriteCvDataSet(15); }

        private void WriteCvDataSet(int cvNo)
        {
            if (!CheckConn()) return;

            // [LGLS 2026-08-21] 주소맵 XML 우선 (통신부 CvThread 와 같은 출처 - XML 하나로 함께 바뀐다)
            int dirWordAddr = cPlcAddrMap.BlockBase("CV", cvNo, "Direction");
            if (dirWordAddr < 0) dirWordAddr = 480 + (cvNo - 1);      // 폴백 %DB(960+(N-1)*2)
            int trkWordAddr = cPlcAddrMap.Addr("CV", cvNo, "Tracking", "JobNo", 0);
            if (trkWordAddr < 0) trkWordAddr = cDefApp.GsRTrackWord((cvNo - 1) * 10);
            const int DIR_VAL = 0;                   // 0=입고
            const int JOB_BCD = 2143;                // JOB NO 1234 → 2143

            RunPlcTask(() =>
            {
                bool   ok  = true;
                string err = "";
                string step = "";
                try
                {
                    var plc = _plc;
                    if (plc == null) { ok = false; }
                    else
                    {
                        // 1) 방향 워드
                        step = "%DB" + (dirWordAddr * 2);
                        byte[] buf = new byte[2];
                        buf[0] = DIR_VAL & 0xFF; buf[1] = 0;
                        ok = plc.WRITE(0, DEV_D, dirWordAddr, 1, buf);
                        LogProtocol(plc, "CvWrite");

                        // 2) 트래킹 JOB NO 2워드
                        if (ok)
                        {
                            step = "R" + trkWordAddr.ToString("0000") + "(%RB" + (trkWordAddr * 2) + ")";
                            byte[] job = new byte[4];
                            job[0] = (byte)(JOB_BCD & 0xFF);
                            job[1] = (byte)((JOB_BCD >> 8) & 0xFF);
                            job[2] = 0; job[3] = 0;
                            ok = plc.WRITE(0, DEV_R, trkWordAddr, 2, job);
                            LogProtocol(plc, "CvWrite");
                        }
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { ok = false; err = ex.Message; }

                string capturedStep = step;
                SafeInvoke(() =>
                {
                    if (ok)
                        AppendLog(string.Format(
                            "[CV#{0} 데이터 쓰기] %DB{1}←{2}(입고) / R{3}(%RB{4})←{5}, R{6}←0 완료",
                            cvNo, dirWordAddr * 2, DIR_VAL,
                            trkWordAddr.ToString("0000"), trkWordAddr * 2, JOB_BCD,
                            (trkWordAddr + 1).ToString("0000")));
                    else
                    {
                        AppendLog(string.Format("[CV#{0} 데이터 쓰기] 실패 ({1}){2}",
                                  cvNo, capturedStep, err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                    }
                });
            });
        }

        // ─── 수동 입력 해석 ──────────────────────────────────────────────────
        // 콤보 표기: [% + 디바이스문자 + 크기문자] (%DB/%RB=바이트, %DW/%RW=워드, %MX=비트)
        // 입력 주소 단위: %DB/%RB=바이트주소, %DW/%RW=워드주소, %MX=비트주소
        // 통신은 항상 워드 단위 → 내부적으로 워드주소로 환산해서 READ/WRITE
        private bool ParseManualTarget(string strCaller, out string devSel,
                                       out byte devCode, out int wordAddr, out int dispBase)
        {
            devSel   = cmbManualDev.Text;
            // 콤보 병기 표기("%DB (D영역 바이트)")에서 앞 3자리 표기만 추출
            if (devSel.Length > 3) devSel = devSel.Substring(0, 3);
            devCode  = DEV_D;
            wordAddr = 0;
            dispBase = 0;

            int inAddr = ParseHexOrDec(txtManualAddr.Text);

            switch (devSel)
            {
                case "%MX":   // 비트주소 입력 → 포함 워드 읽기
                    devCode  = DEV_M;
                    wordAddr = inAddr / 16;
                    dispBase = wordAddr * 16;             // 워드 정렬된 비트주소
                    return true;

                case "%RB":   // R 트래킹 바이트주소 입력 (짝수만, 예: %RB260 → 260)
                    if (inAddr % 2 != 0)
                    {
                        AppendLog(strCaller + " %RB 주소는 짝수(워드 정렬)만 가능합니다.");
                        return false;
                    }
                    devCode  = DEV_R;
                    wordAddr = inAddr / 2;
                    dispBase = inAddr;                    // 바이트주소 그대로 표시
                    return true;

                case "%RW":   // R 트래킹 워드주소 입력 (10진, 예: R0130 → 130)
                    devCode  = DEV_R;
                    wordAddr = inAddr;
                    dispBase = inAddr;
                    return true;

                case "%DW":   // D영역 워드주소 입력
                    devCode  = DEV_D;
                    wordAddr = inAddr;
                    dispBase = inAddr;                    // 워드주소 그대로 표시
                    return true;

                default:      // "%DB" D영역 바이트주소 입력 (짝수만)
                    if (inAddr % 2 != 0)
                    {
                        AppendLog(strCaller + " %DB 주소는 짝수(워드 정렬)만 가능합니다.");
                        return false;
                    }
                    devCode  = DEV_D;
                    wordAddr = inAddr / 2;
                    dispBase = inAddr;                    // 바이트주소 그대로 표시
                    return true;
            }
        }

        // 그리드/로그용 주소 라벨 생성: 콤보와 동일한 [%+디바이스+크기+주소] 표기
        private static string ManualAddrLabel(string devSel, int dispBase, int wordIdx)
        {
            switch (devSel)
            {
                case "%MX": return "%MX" + (dispBase + wordIdx * 16);
                case "%DW": return "%DW" + (dispBase + wordIdx);
                case "%RW": return "R" + (dispBase + wordIdx).ToString("0000");
                case "%RB": return "%RB" + (dispBase + wordIdx * 2);
                default:    return "%DB" + (dispBase + wordIdx * 2);
            }
        }

        // 실제 전송 변수명 라벨: %MX->M영역, %DW/%DB->D영역, %RW/%RB->R영역 (ParseManualTarget 매핑과 동일)
        private static string ManualWireLabel(string devSel, int wordAddr, int wordIdx)
        {
            int  addr = wordAddr + wordIdx;
            char dev  = (devSel == "%MX") ? 'M'
                      : (devSel == "%RW" || devSel == "%RB") ? 'R' : 'D';
            return string.Format("%{0}B{1}", dev, addr * 2);
        }

        // ─── PPT(구 표기) 주소 계산기 ────────────────────────────────────────
        //   M492  : 마지막 자리 = 비트(0~9, A~F), 앞자리 = M워드 → %MX = 워드*16+비트
        //           예) M492 = 워드49 비트2 → %MX786, 워드 읽기 주소는 %MW49(비트 784~799)
        //   Dnnnn : PPT 세그먼트별 변환(다른 탭의 V0.9→V1.1 표와 동일 규칙)
        //           D0160~ : 256 + (n-160)/10*16 + n%10   (S/C·RGV 상태, 유닛당 10워드)
        //           D0300~ : 480 + (n-300)                (C/V 방향, 유닛당 1워드)
        //           D0320~ : 768 + (n-320)/10*16 + n%10   (ECS Cmd)
        //   Rnnnn : 10진 워드 그대로 (%RB = ×2)
        private char _pptDev  = 'M';
        private int  _pptWord = 0;      // 읽기/쓰기 워드 주소
        private int  _pptBit  = 0;      // M 전용: 워드 내 비트
        private bool _pptValid = false;

        private bool PptCalc()
        {
            _pptValid = false;
            string dev = cmbPptDev.Text;
            string s   = txtPptAddr.Text.Trim().ToUpper();
            if (s.StartsWith(dev)) s = s.Substring(dev.Length);   // "M492" 전체 입력도 허용
            if (s.Length == 0) { AppendLog("[PPT 계산] 주소를 입력하세요."); return false; }

            try
            {
                if (dev == "M")
                {
                    int bit;
                    char last = s[s.Length - 1];
                    if (last >= 'A' && last <= 'F') { bit = 10 + (last - 'A'); s = s.Substring(0, s.Length - 1); }
                    else                            { int all = int.Parse(s); bit = all % 10; s = (all / 10).ToString(); }
                    int word = int.Parse(s);
                    _pptDev = 'M'; _pptWord = word; _pptBit = bit;
                    lblPptResult.Text = string.Format("%MX{0} +{1} (%MW{2})", word * 16, bit, word);
                    AppendLog(string.Format("[PPT 계산] M{0}{1} → 비트 {2}*16+{3}={4} → %MX{5} +{6}, 읽기워드 %MW{7}",
                              s, bit < 10 ? bit.ToString() : ((char)('A' + bit - 10)).ToString(),
                              word, bit, word * 16 + bit, word * 16, bit, word));
                }
                else if (dev == "D")
                {
                    int old = int.Parse(s);
                    int w;
                    if      (old >= 320) w = 768 + (old - 320) / 10 * 16 + old % 10;
                    else if (old >= 300) w = 480 + (old - 300);
                    else if (old >= 160) w = 256 + (old - 160) / 10 * 16 + old % 10;
                    else                 w = old;   // 표 밖 영역은 직접 워드로 취급
                    _pptDev = 'D'; _pptWord = w; _pptBit = 0;
                    lblPptResult.Text = string.Format("%DW{0} (%DB{1})", w, w * 2);
                    AppendLog(string.Format("[PPT 계산] D{0:0000} → %DW{1} (%DB{2})", old, w, w * 2));
                }
                else   // R
                {
                    // [LGLS 2026-08-19] R 주소모드 반영 : HEX(구 ECS)=표기를 16진 파싱 / DEC(현행)=10진 그대로
                    int n = int.Parse(s);
                    int w = cDefApp.GsRTrackWord(n);
                    _pptDev = 'R'; _pptWord = w; _pptBit = 0;
                    lblPptResult.Text = string.Format("R{0:0000} → 워드 {1} (%RB{2})  [{3}]", n, w, w * 2, cDefApp.GsRAddrModeText());
                    AppendLog(string.Format("[PPT 계산] R{0:0000} → 워드 {1} (%RB{2})  R주소모드={3}", n, w, w * 2, cDefApp.GsRAddrModeText()));
                }
            }
            catch (Exception ex) { AppendLog("[PPT 계산] 입력 오류: " + ex.Message); return false; }
            _pptValid = true;
            return true;
        }

        private byte PptDevCode()
        {
            return (_pptDev == 'M') ? DEV_M : (_pptDev == 'R') ? DEV_R : DEV_D;
        }

        private string PptRowLabel(int i)
        {
            int w = _pptWord + i;
            if (_pptDev == 'M') return string.Format("M{0:0000} (%MX{1})", w * 10, w * 16);
            // [LGLS 2026-08-19] R 은 모드에 따라 문서표기≠워드주소 이므로 워드주소로 표기한다
            if (_pptDev == 'R') return string.Format("%RW{0} (%RB{1})", w, w * 2);
            return string.Format("%DW{0} (%DB{1})", w, w * 2);
        }

        private void btnPptCalc_Click(object sender, EventArgs e)
        {
            PptCalc();
        }

        // 계산된 주소부터 '길이' 워드 읽기 → 그리드 + 값 상자(첫 워드)
        private void PptReadInternal(bool bAfterWrite)
        {
            if (!CheckConn()) return;
            if (!_pptValid && !PptCalc()) return;

            int cnt;
            if (!int.TryParse(txtPptLen.Text.Trim(), out cnt) || cnt <= 0 || cnt > 500)
            {
                AppendLog("[PPT 읽기] 길이는 1~500 워드 범위입니다.");
                return;
            }
            int  word = _pptWord;
            int  bit  = _pptBit;
            char devc = _pptDev;
            byte code = PptDevCode();

            SetReadButtons(false);
            RunPlcTask(() =>
            {
                byte[] buf = new byte[cnt * 2];
                bool   ok  = false;
                string err = "";
                try
                {
                    var plc = _plc;
                    if (plc != null)
                    {
                        ok = plc.READ(0, code, word, cnt, ref buf);
                        LogProtocol(plc, "PPT");
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                SafeInvoke(() =>
                {
                    if (!ok)
                    {
                        AppendLog(string.Format("[PPT 읽기] 실패{0}", err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                        SetReadButtons(true);
                        return;
                    }
                    dgvManual.Rows.Clear();
                    for (int i = 0; i < cnt; i++)
                    {
                        int raw = (buf[i * 2 + 1] << 8) | buf[i * 2];
                        dgvManual.Rows.Add(
                            PptRowLabel(i),
                            string.Format("0x{0:X4}", raw),
                            raw.ToString(),
                            Convert.ToString(raw, 2).PadLeft(16, '0'));
                    }
                    int first = (buf[1] << 8) | buf[0];
                    txtPptVal.Text = string.Format("0x{0:X4}", first);
                    if (devc == 'M')
                        AppendLog(string.Format("[PPT 읽기] %MW{0} = 0x{1:X4} (비트{2} = {3}) x{4}워드{5}",
                                  word, first, bit, (first >> bit) & 1, cnt, bAfterWrite ? " [쓰기 후 재조회]" : ""));
                    else
                        AppendLog(string.Format("[PPT 읽기] {0} = 0x{1:X4} x{2}워드{3}",
                                  PptRowLabel(0), first, cnt, bAfterWrite ? " [쓰기 후 재조회]" : ""));
                    SetReadButtons(true);
                });
            });
        }

        private void btnPptRead_Click(object sender, EventArgs e)
        {
            PptReadInternal(false);
        }

        // 값 상자의 워드를 계산된 주소에 기록 → 아래 그리드 자동 재조회
        private void btnPptWrite_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;
            if (!_pptValid && !PptCalc()) return;

            int val;
            try { val = ParseHexOrDec(txtPptVal.Text); }
            catch (Exception ex) { AppendLog("[PPT 쓰기] 값 오류: " + ex.Message); return; }

            int  word = _pptWord;
            byte code = PptDevCode();

            RunPlcTask(() =>
            {
                bool   ok  = false;
                string err = "";
                try
                {
                    byte[] buf = new byte[2];
                    buf[0] = (byte)(val & 0xFF);
                    buf[1] = (byte)((val >> 8) & 0xFF);
                    var plc = _plc;
                    if (plc != null)
                    {
                        ok = plc.WRITE(0, code, word, 1, buf);
                        LogProtocol(plc, "PPT");
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                SafeInvoke(() =>
                {
                    if (!ok)
                    {
                        AppendLog(string.Format("[PPT 쓰기] 실패{0}", err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                        return;
                    }
                    AppendLog(string.Format("[PPT 쓰기] {0} ← 0x{1:X4} 완료 → 재조회", PptRowLabel(0), val));
                    PptReadInternal(true);   // 그리드 자동 갱신
                });
            });
        }

        // ─── 수동 Read ────────────────────────────────────────────────────────
        private void btnManualRead_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;

            string devSel;
            byte   devCode;
            int    wordAddr, dispBase, cnt;
            try
            {
                if (!ParseManualTarget("[Manual Read]", out devSel, out devCode, out wordAddr, out dispBase))
                    return;
                cnt = int.Parse(txtManualCnt.Text.Trim());
                if (cnt <= 0 || cnt > 500)
                {
                    AppendLog("[Manual Read] 워드 수는 1~500 범위입니다.");
                    return;
                }
            }
            catch (Exception ex) { AppendLog("[Manual Read] 입력 오류: " + ex.Message); return; }

            SetReadButtons(false);
            RunPlcTask(() =>
            {
                byte[] buf = new byte[cnt * 2];
                bool   ok  = false;
                string err = "";
                try
                {
                    var plc = _plc;
                    if (plc != null)
                    {
                        ok = plc.READ(0, devCode, wordAddr, cnt, ref buf);
                        LogProtocol(plc, "Manual");
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                SafeInvoke(() =>
                {
                    if (!ok)
                    {
                        AppendLog(string.Format("[Manual Read] 실패{0}", err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                        SetReadButtons(true);
                        return;
                    }
                    dgvManual.Rows.Clear();
                    for (int i = 0; i < cnt; i++)
                    {
                        int raw = (buf[i * 2 + 1] << 8) | buf[i * 2];
                        dgvManual.Rows.Add(
                            string.Format("{0} ({1})",
                                ManualAddrLabel(devSel, dispBase, i),
                                ManualWireLabel(devSel, wordAddr, i)),
                            string.Format("0x{0:X4}", raw),
                            raw.ToString(),
                            Convert.ToString(raw, 2).PadLeft(16, '0'));
                    }
                    AppendLog(string.Format("[Manual Read] {0} ({1}) x{2}워드 완료",
                              ManualAddrLabel(devSel, dispBase, 0),
                              ManualWireLabel(devSel, wordAddr, 0), cnt));
                    SetReadButtons(true);
                });
            });
        }

        // ─── 수동 Write ───────────────────────────────────────────────────────
        private void btnManualWrite_Click(object sender, EventArgs e)
        {
            if (!CheckConn()) return;
            string devSel;
            byte   devCode;
            int    wordAddr, dispBase, val;
            try
            {
                if (!ParseManualTarget("[Manual Write]", out devSel, out devCode, out wordAddr, out dispBase))
                    return;
                val = ParseHexOrDec(txtManualWriteVal.Text);
            }
            catch (Exception ex) { AppendLog("[Manual Write] 입력 오류: " + ex.Message); return; }

            RunPlcTask(() =>
            {
                bool   ok  = false;
                string err = "";
                try
                {
                    byte[] buf = new byte[2];
                    buf[0] = (byte)(val & 0xFF);
                    buf[1] = (byte)((val >> 8) & 0xFF);
                    var plc = _plc;
                    if (plc != null)
                    {
                        ok = plc.WRITE(0, devCode, wordAddr, 1, buf);
                        LogProtocol(plc, "Manual");
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                SafeInvoke(() =>
                {
                    if (ok)
                        AppendLog(string.Format("[Manual Write] {0} ({1}) <- 0x{2:X4} 완료",
                                  ManualAddrLabel(devSel, dispBase, 0),
                                  ManualWireLabel(devSel, wordAddr, 0), val));
                    else
                    {
                        AppendLog(string.Format("[Manual Write] 실패{0}", err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                    }
                });
            });
        }

        // ─── 선택 행 Write (Bit/Word/Tracking 탭) ────────────────────────────
        private void WriteSelected(DataGridView dgv, byte devCode, TextBox txtVal)
        {
            if (!CheckConn()) return;
            if (dgv.SelectedRows.Count == 0)
            {
                AppendLog("쓰기할 행을 먼저 선택하세요.");
                return;
            }
            int    addr;
            int    val;
            string addrLabel;
            try
            {
                addr      = (int)dgv.SelectedRows[0].Tag;
                val       = ParseHexOrDec(txtVal.Text);
                object o  = dgv.SelectedRows[0].Cells[0].Value;
                addrLabel = o != null ? o.ToString() : addr.ToString("0000");
            }
            catch (Exception ex) { AppendLog("[Write] 입력 오류: " + ex.Message); return; }

            RunPlcTask(() =>
            {
                bool   ok  = false;
                string err = "";
                try
                {
                    byte[] buf = new byte[2];
                    buf[0] = (byte)(val & 0xFF);
                    buf[1] = (byte)((val >> 8) & 0xFF);
                    var plc = _plc;
                    if (plc != null)
                    {
                        ok = plc.WRITE(0, devCode, addr, 1, buf);
                        LogProtocol(plc, "Write");
                        if (!ok) err = plc.GetErrorMsg();
                    }
                }
                catch (Exception ex) { err = ex.Message; }

                SafeInvoke(() =>
                {
                    if (ok)
                        AppendLog(string.Format("[Write] {0} <- 0x{1:X4} ({2}) 완료", addrLabel, val, val));
                    else
                    {
                        AppendLog(string.Format("[Write] {0} 실패{1}", addrLabel,
                                  err.Length > 0 ? ": " + err : ""));
                        TryRecover(err);
                    }
                });
            });
        }

        // ─── 공통 유틸리티 ────────────────────────────────────────────────────

        // PLC 작업을 백그라운드 스레드에서 실행. 폼 닫기 중에는 즉시 반환.
        private void RunPlcTask(Action work)
        {
            Interlocked.Increment(ref _runningTasks);
            Task.Run(() =>
            {
                // 모든 PLC 작업 직렬화 (하트비트와 사용자 조작이 겹치면 응답이 어긋나 PLC가 끊음)
                try   { lock (_plcLock) { work(); } }
                catch (Exception ex) { AppendLog("[오류] " + ex.Message); }
                finally { Interlocked.Decrement(ref _runningTasks); }
            });
        }

        // UI 스레드 안전 Invoke — 폼이 닫히는 중이면 아무것도 하지 않는다
        private void SafeInvoke(Action action)
        {
            if (_closing || IsDisposed) return;
            try
            {
                if (InvokeRequired)
                    Invoke(action);   // Invoke(동기): 결과를 화면에 즉시 반영
                else
                    action();
            }
            catch (ObjectDisposedException) { }
            catch (InvalidOperationException) { }
        }

        private bool CheckConn()
        {
            if (_plc == null || !_plc.m_bSocCon)
            {
                MessageBox.Show("PLC에 연결되어 있지 않습니다.\n먼저 [연결] 버튼을 눌러 주세요.", "연결 필요",
                                MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            return true;
        }

        private void SetConnStatus(bool connected)
        {
            if (connected)
            {
                lblConnStatus.Text      = "● 연결됨";
                lblConnStatus.ForeColor = Color.LimeGreen;
                btnConnect.Enabled      = false;
                btnDisconnect.Enabled   = true;
            }
            else
            {
                lblConnStatus.Text      = "● 미연결";
                lblConnStatus.ForeColor = Color.Red;
                btnConnect.Enabled      = true;
                btnDisconnect.Enabled   = false;
            }
        }

        // Read 버튼 일괄 활성/비활성
        private void SetReadButtons(bool enabled)
        {
            btnReadBit.Enabled      = enabled;
            btnReadWord.Enabled     = enabled;
            btnReadTracking.Enabled = enabled;
            btnManualRead.Enabled   = enabled;
            btnPptRead.Enabled      = enabled;
        }

        private void AppendLog(string msg)
        {
            string line = string.Format("[{0}] {1}", DateTime.Now.ToString("HH:mm:ss"), msg);
            SafeInvoke(() => txtLog.AppendText(line + "\r\n"));
        }

        // chkProtoLog 가 체크된 경우 송수신 Hex 바이트를 로그에 출력
        private void LogProtocol(FenetProtocol plc, string tag)
        {
            if (plc == null) return;
            bool protoOn = false;
            SafeInvoke(() => protoOn = chkProtoLog.Checked);
            if (!protoOn) return;

            string snd = plc.SndHexString;
            string rcv = plc.RcvHexString;
            if (!string.IsNullOrEmpty(snd))
                AppendLog(string.Format("  [{0}] TX: {1}", tag, snd));
            if (!string.IsNullOrEmpty(rcv))
                AppendLog(string.Format("  [{0}] RX: {1}", tag, rcv));
        }

        private static void PopulateGrid(DataGridView dgv, MemMapEntry[] entries)
        {
            dgv.Rows.Clear();
            foreach (var e in entries)
            {
                int idx = dgv.Rows.Add(e.Name, e.Description, "", "", "");
                dgv.Rows[idx].Tag = e.Address;
            }
        }

        // UI 스레드에서 (PLC 주소, 행 인덱스) 목록 수집
        private List<KeyValuePair<int, int>> GetDgvRows(DataGridView dgv)
        {
            var list = new List<KeyValuePair<int, int>>();
            for (int i = 0; i < dgv.Rows.Count; i++)
            {
                if (dgv.Rows[i].Tag != null)
                    list.Add(new KeyValuePair<int, int>((int)dgv.Rows[i].Tag, i));
            }
            return list;
        }

        private static int ParseHexOrDec(string s)
        {
            s = s.Trim();
            if (s.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
                return Convert.ToInt32(s.Substring(2), 16);
            return int.Parse(s);
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            // 1. 더 이상 SafeInvoke 가 UI 를 건드리지 못하도록 플래그 설정
            _closing = true;

            // 하트비트 중지
            if (_tmrHeartbeat != null) _tmrHeartbeat.Stop();

            // 2. 소켓 즉시 닫기 → 블로킹 중인 RecvRst 가 예외를 받아 탈출
            ForceCloseSocket();

            // 3. 백그라운드 Task 가 완전히 끝날 때까지 최대 5초 대기
            //    (SetReadButtons/AppendLog 등 SafeInvoke 는 _closing 덕에 skip 됨)
            int waited = 0;
            while (_runningTasks > 0 && waited < 50)
            {
                Thread.Sleep(100);
                waited++;
            }

            SetConnStatus(false);
            base.OnFormClosing(e);
        }
    }

    // 메모리 맵 항목 정의
    public class MemMapEntry
    {
        public string Name;
        public int    Address;
        public string Description;
        public MemMapEntry(string name, int address, string description)
        {
            Name = name; Address = address; Description = description;
        }
    }
}
