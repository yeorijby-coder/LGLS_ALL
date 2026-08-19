using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace WCS_TASK_CV
{
    // [LGLS 2026-07-27] CV_TASK 관점 [시나리오 테스트] — EQP_SIM ScenarioTestForm 의 역할 반대판.
    //   EQP_SIM : EQP=버튼(쓰기) / WCS·D·R=관측(읽기)
    //   CV_TASK : WCS·D·R=버튼(강제 write) / EQP=관측(읽기)   ← CV_TASK 가 WCS 이므로 대칭
    //   FRM_PLC_MEMMAP 과 동일하게 자체 FenetProtocol 소켓(ini COMM0)으로 PLC(EQP_SIM)에 직접 READ/WRITE.
    //   창이 열려 있는 동안 CvThread 자동통신 정지(m_bPaused=true) → 강제 write 와 충돌 방지.

    // 통합 스텝: Kind 0=EQP/PLC 비트(관측 램프), 1=WCS ACK 비트(강제 ON/OFF 버튼), 2=D/R 워드(값입력+강제 송신)
    internal class ScStepCv
    {
        public int Kind;
        public char Dev;         // 'M' / 'D' / 'R'
        public int Bit;          // M 비트 절대주소 (Kind 0/1)
        public int WordAddr;     // D/R 워드주소 (Kind 2)
        public bool Val;         // 시나리오상 ON/OFF (Kind0 관측 기대값)
        public bool IsString;    // D/R 문자열(JOB NO 등)
        public string Addr;
        public string Desc;
        public string Src;

        private ScStepCv() { }
        public static ScStepCv E(int bit, bool val, string addr, string desc)
        { return new ScStepCv { Kind = 0, Dev = 'M', Bit = bit, Val = val, Addr = addr, Desc = desc }; }
        public static ScStepCv W(int bit, string addr, string desc, string src)
        { return new ScStepCv { Kind = 1, Dev = 'M', Bit = bit, Val = true, Addr = addr, Desc = desc, Src = src }; }
        public static ScStepCv D(char dev, int wordAddr, bool isStr, string addr, string desc, string src)
        {
            // [LGLS 2026-08-19] R(트래킹) 스텝의 wordAddr 은 'PPT 문서 표기'로 적혀 있다.
            //   실제 전송 워드주소는 R 주소모드(ini [PLC] R_ADDR_MODE)를 따라 변환한다.
            //   HEX(구 ECS 호환) : R0102 → 0x102 = 258 / DEC(현행) : R0102 → 102
            //   D 는 이미 실주소로 적혀 있으므로 변환하지 않는다.
            int nWord = (dev == 'R') ? cDefApp.GsRTrackWord(wordAddr) : wordAddr;
            string strAddr = addr;
            if (dev == 'R')
                strAddr = string.Format("%RB{0} (R{1:0000})", nWord * 2, wordAddr);
            return new ScStepCv { Kind = 2, Dev = dev, WordAddr = nWord, IsString = isStr, Addr = strAddr, Desc = desc, Src = src };
        }
    }

    internal class ScenarioCv
    {
        public string Title;
        public List<ScStepCv> Steps = new List<ScStepCv>();
        public ScenarioCv(string title) { Title = title; }
    }

    internal class FRM_SCENARIO_TEST : Form
    {
        private const byte DEV_M = 0x90, DEV_D = 0xA8, DEV_R = 0xAF;

        private readonly SYS_MAIN owner;
        private FenetProtocol _plc;
        private readonly List<ScenarioCv> scenarios;
        private Label lblConn;

        public FRM_SCENARIO_TEST(SYS_MAIN main)
        {
            owner = main;
            scenarios = BuildScenarios();

            Text = "[CV_TASK 시나리오 테스트] — WCS·D·R 강제 송신 / EQP 관측 (EQP_SIM 대칭)";
            Width = 1000; Height = 620; StartPosition = FormStartPosition.CenterScreen;

            PauseCvThread(true);   // 자동통신 정지
            ConnectPlc();          // 자체 소켓 접속(ini COMM0)

            var lbl = new Label
            {
                Text = "시나리오 버튼 = 반송 전체. [WCS 강제]=ON/OFF 버튼, [D/R 강제]=값입력+송신, [EQP 관측]=설비 비트 램프. 창 닫으면 CvThread 자동통신 재개.",
                Dock = DockStyle.Top, Height = 28, TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(10, 0, 0, 0), ForeColor = Color.DarkRed
            };
            lblConn = new Label
            {
                Dock = DockStyle.Top, Height = 24, TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(10, 0, 0, 0), Font = new Font("맑은 고딕", 9F, FontStyle.Bold)
            };
            UpdateConnLabel();

            var pnl = new FlowLayoutPanel { Dock = DockStyle.Fill, FlowDirection = FlowDirection.TopDown, WrapContents = false, AutoScroll = true, Padding = new Padding(10) };
            for (int i = 0; i < scenarios.Count; i++)
            {
                var sc = scenarios[i];
                var b = new Button
                {
                    Text = (i + 1) + ". " + sc.Title,
                    Width = 940, Height = 32, Margin = new Padding(2),
                    Font = new Font("맑은 고딕", 10F, FontStyle.Bold), TextAlign = ContentAlignment.MiddleLeft
                };
                var scLocal = sc;
                b.Click += (s, e) => { if (EnsurePlc()) new StepFormCv(this, scLocal).Show(this); };
                pnl.Controls.Add(b);
            }

            Controls.Add(pnl);
            Controls.Add(lblConn);
            Controls.Add(lbl);

            FormClosed += (s, e) => { PauseCvThread(false); try { _plc?.CloseSocket(); } catch { } };
        }

        private void PauseCvThread(bool p)
        {
            try { if (owner != null && owner.m_thCvThread != null && owner.m_thCvThread[0] != null) owner.m_thCvThread[0].m_bPaused = p; }
            catch { }
        }

        // ─── 자체 소켓 접속 (FRM_PLC_MEMMAP.TryAutoConnect 와 동일 패턴) ───
        private void ConnectPlc()
        {
            try
            {
                string strPlcNo = "", strIp = "", strLogPath = "", strFileNm = "", strEqmt = "", strMsg = "";
                int nCur = 0, nFrom = 0, nTo = 0, nPortCnt = 0, nCnt = 0, nIdx = 0, nFrTrack = 0, nToTrack = 0;
                cDefApi.GsReadInitProfileCom("COMM0",
                    ref strPlcNo, ref strIp, ref nCur, ref nFrom, ref nTo,
                    ref nPortCnt, ref nCnt, ref nIdx, ref nFrTrack, ref nToTrack,
                    ref strLogPath, ref strFileNm, ref strEqmt, ref strMsg);
                if (string.IsNullOrEmpty(strIp) || nCur <= 0) { _plc = null; return; }
                _plc = new FenetProtocol();
                string rtn = "";
                if (!_plc.OpenSocketOnly(strIp, nCur, ref rtn)) _plc = null;
            }
            catch { _plc = null; }
        }

        private bool EnsurePlc()
        {
            if (_plc != null) return true;
            ConnectPlc();
            UpdateConnLabel();
            if (_plc == null)
            {
                MessageBox.Show("PLC(COMM0) 접속 실패 — ini 접속정보/EQP_SIM 기동을 확인하세요.", "시나리오 테스트",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }
            return true;
        }

        private void UpdateConnLabel()
        {
            if (lblConn == null) return;
            lblConn.Text = (_plc != null)
                ? "  ● PLC(COMM0) 접속됨 — CvThread 자동통신 정지 중(강제 송신 가능)"
                : "  ● PLC(COMM0) 미접속 — EQP_SIM 기동 후 시나리오 버튼 재시도";
            lblConn.ForeColor = (_plc != null) ? Color.SeaGreen : Color.Firebrick;
        }

        // ─── PLC READ/WRITE 헬퍼 (모두 워드 접근; M 비트는 워드 read-modify-write) ───
        public bool ReadBit(int bitAddr, out bool on)
        {
            on = false;
            if (_plc == null) return false;
            int w = bitAddr / 16, bp = bitAddr % 16;
            byte[] rx = new byte[16];
            if (!_plc.READ(0, DEV_M, w, 1, ref rx)) return false;
            int raw = (rx[1] << 8) | rx[0];
            on = ((raw >> bp) & 1) != 0;
            return true;
        }

        public bool SendBit(int bitAddr, bool onVal)
        {
            if (_plc == null) return false;
            int w = bitAddr / 16, bp = bitAddr % 16;
            byte[] rx = new byte[16];
            if (!_plc.READ(0, DEV_M, w, 1, ref rx)) return false;
            int raw = (rx[1] << 8) | rx[0];
            if (onVal) raw |= (1 << bp); else raw &= ~(1 << bp);
            byte[] tx = { (byte)(raw & 0xFF), (byte)((raw >> 8) & 0xFF) };
            return _plc.WRITE(0, DEV_M, w, 1, tx);
        }

        // [LGLS 2026-07-27] 한 M워드에서 mask 비트들을 한 번의 read-modify-write로 clear (초기화용).
        //   비트별 SendBit 를 연속 호출하면 앞 WRITE 반영 전 다음 READ 가 stale 값을 읽어 이미 clear 한
        //   비트가 복원되는 문제가 있어(같은 워드 공유 비트: %MX1506/1507/1509=M094), 워드당 1회만 처리한다.
        public bool ClearBits(int wordAddr, int mask)
        {
            if (_plc == null) return false;
            byte[] rx = new byte[16];
            if (!_plc.READ(0, DEV_M, wordAddr, 1, ref rx)) return false;
            int raw = (rx[1] << 8) | rx[0];
            raw &= ~mask;
            byte[] tx = { (byte)(raw & 0xFF), (byte)((raw >> 8) & 0xFF) };
            return _plc.WRITE(0, DEV_M, wordAddr, 1, tx);
        }

        // D/R 워드 강제 송신. 입력 문자열을 ASCII 팩(하위바이트=앞문자, 워드 little-endian)으로.
        //   Direction "0" → 1워드 0x0030 ,  JOB "0001" → 2워드 0x3030/0x3130 (EncodeJobNoR 동일)
        public bool SendWordAscii(char dev, int wordAddr, string s)
        {
            if (_plc == null) return false;
            byte devCode = (dev == 'R') ? DEV_R : DEV_D;
            if (s == null) s = "";
            byte[] a = Encoding.ASCII.GetBytes(s);
            int wl = (a.Length + 1) / 2; if (wl < 1) wl = 1;
            byte[] buf = new byte[wl * 2];
            for (int i = 0; i < a.Length; i++) buf[i] = a[i];
            return _plc.WRITE(0, devCode, wordAddr, wl, buf);
        }

        // [LGLS 2026-07-27] 비활성 창 첫 클릭이 창 활성화로만 소비되지 않고 시나리오 버튼까지 전달되도록
        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == 0x0021 && m.Result == (IntPtr)2) m.Result = (IntPtr)1; // MA_ACTIVATEANDEAT(2)→MA_ACTIVATE(1) // WM_MOUSEACTIVATE: MA_ACTIVATEANDEAT→MA_ACTIVATE
        }

        private static List<ScenarioCv> BuildScenarios()
        {
            const string CV_ACK = "[WCS_TASK_CV]CvThread.cs::CvEventCheck";
            const string CV_TRK = "[WCS_TASK_CV]CvThread.cs::CvTrackingWrite";
            const string CV_DIR = "[WCS_TASK_CV]CvThread.cs::CvStatusScenario (읽기→STOCK_MODE)";
            const string VEH_CMD = "[WCS_TASK_CV]VehThread.cs::ConsumeCommands";
            const string VEH_POLL = "[WCS_TASK_CV]VehThread.cs::PollObservations";
            const string CV_ALARM = "[WCS_TASK_CV]CvThread.cs::CvAlarmCheck";

            var list = new List<ScenarioCv>();

            // 1. 입고 : C/V #11 (슬라이드 8~9)
            var s1 = new ScenarioCv("반송 시나리오 (입고 : C/V #11) — 슬라이드 8~9");
            s1.Steps.Add(ScStepCv.E(593, true,  "M037.1 %MX593 ON",  "Pallet Exist #22 ON (입고대 적재)"));
            s1.Steps.Add(ScStepCv.E(580, true,  "M036.4 %MX580 ON",  "Load Complete #2 ON"));
            s1.Steps.Add(ScStepCv.W(1444,        "M090.4 %MX1444 ON", "Load Complete Ack #2 ON (WCS)", CV_ACK));
            s1.Steps.Add(ScStepCv.E(580, false, "M036.4 %MX580 OFF", "Load Complete #2 OFF"));
            s1.Steps.Add(ScStepCv.E(1444, false, "M090.4 %MX1444 OFF","Load Complete Ack #2 OFF (설비/PLC 리셋)"));
            s1.Steps.Add(ScStepCv.D('D', 490, false, "D0310 → %DW490(%DB980)", "Direction Mode Check '1' (입고0/출고1)", CV_DIR));
            s1.Steps.Add(ScStepCv.D('R', 102, true,  "%RB204 (R0102)", "JOB NO #22 최초 기록 (입고 진입 시 #22 위치에 JOB Write)", CV_TRK));
            s1.Steps.Add(ScStepCv.E(593, false, "M037.1 %MX593 OFF", "Pallet Exist #22 OFF (ID Shift)"));
            s1.Steps.Add(ScStepCv.E(592, true,  "M037.0 %MX592 ON",  "Pallet Exist #21 ON (#21 도착)"));
            s1.Steps.Add(ScStepCv.D('R', 100, true,  "%RB200 (R0100)", "PLC ID Shift 결과: #22(R0102) JOB → #21(R0100)로 이동(원위치 #22 클리어)", null));
            s1.Steps.Add(ScStepCv.E(582, true,  "M036.6 %MX582 ON",  "W.O 보고 (RGV Call)"));
            s1.Steps.Add(ScStepCv.D('R', 100, true,  "%RB200 (R0100)", "(슬9) RGV Call 시 #21(R0100) 반출 대상 JOB 재표시 — RGV지시는 D0530~", null));
            s1.Steps.Add(ScStepCv.E(592, false, "M037.0 %MX592 OFF", "Pallet Exist #21 OFF (RGV Load 완료)"));
            s1.Steps.Add(ScStepCv.E(577, true,  "M036.1 %MX577 ON",  "Unload Complete #1 ON"));
            s1.Steps.Add(ScStepCv.W(1441,        "M090.1 %MX1441 ON", "Unload Complete Ack #1 ON (WCS)", CV_ACK));
            s1.Steps.Add(ScStepCv.E(577, false, "M036.1 %MX577 OFF", "Unload Complete #1 OFF"));
            s1.Steps.Add(ScStepCv.E(1441, false, "M090.1 %MX1441 OFF","Unload Complete Ack #1 OFF (설비/PLC 리셋)"));
            s1.Steps.Add(ScStepCv.E(582, false, "M036.6 %MX582 OFF", "W.O OFF"));
            list.Add(s1);

            // 2. 출고 : C/V #11 (슬라이드 10~11)
            var s2 = new ScenarioCv("반송 시나리오 (출고 : C/V #11) — 슬라이드 10~11");
            s2.Steps.Add(ScStepCv.E(592, true,  "M037.0 %MX592 ON",  "Pallet Exist #21 ON (RGV 하역)"));
            s2.Steps.Add(ScStepCv.E(578, true,  "M036.2 %MX578 ON",  "Load Complete #1 ON"));
            s2.Steps.Add(ScStepCv.W(1442,        "M090.2 %MX1442 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s2.Steps.Add(ScStepCv.E(578, false, "M036.2 %MX578 OFF", "Load Complete #1 OFF"));
            s2.Steps.Add(ScStepCv.E(1442, false, "M090.2 %MX1442 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s2.Steps.Add(ScStepCv.D('D', 490, false, "D0310 → %DW490(%DB980)", "Direction Mode Check '1' (출고1)", CV_DIR));
            s2.Steps.Add(ScStepCv.D('R', 100, true,  "%RB200 (R0100)", "JOB NO #21 Tracking Write", CV_TRK));
            s2.Steps.Add(ScStepCv.E(592, false, "M037.0 %MX592 OFF", "Pallet Exist #21 OFF (ID Shift)"));
            s2.Steps.Add(ScStepCv.E(593, true,  "M037.1 %MX593 ON",  "Pallet Exist #22 ON (#22 도착)"));
            s2.Steps.Add(ScStepCv.D('R', 102, true,  "%RB204 (R0102)", "PLC ID Shift 결과: #21(R0100) JOB → #22(R0102)로 이동(원위치 클리어)", null));
            s2.Steps.Add(ScStepCv.E(579, true,  "M036.3 %MX579 ON",  "Unload Complete #2 ON (작업자 Unload)"));
            s2.Steps.Add(ScStepCv.W(1443,        "M090.3 %MX1443 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s2.Steps.Add(ScStepCv.E(579, false, "M036.3 %MX579 OFF", "Unload Complete #2 OFF"));
            s2.Steps.Add(ScStepCv.E(1443, false, "M090.3 %MX1443 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            list.Add(s2);

            // 3. 출고 : C/V #14 피킹존 (슬라이드 12~13)
            var s3 = new ScenarioCv("반송 시나리오 (출고 : C/V #14 피킹존) — 슬라이드 12~13");
            s3.Steps.Add(ScStepCv.E(688, true,  "M043.0 %MX688 ON",  "Pallet Exist #27 ON (RGV 하역)"));
            s3.Steps.Add(ScStepCv.E(674, true,  "M042.2 %MX674 ON",  "Load Complete #1 ON"));
            s3.Steps.Add(ScStepCv.W(1490,        "M093.2 %MX1490 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s3.Steps.Add(ScStepCv.E(674, false, "M042.2 %MX674 OFF", "Load Complete #1 OFF"));
            s3.Steps.Add(ScStepCv.E(1490, false, "M093.2 %MX1490 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStepCv.D('R', 130, true,  "%RB260 (R0130)", "JOB NO #27 Tracking Write", CV_TRK));
            s3.Steps.Add(ScStepCv.E(688, false, "M043.0 %MX688 OFF", "Pallet Exist #27 OFF (ID Shift)"));
            s3.Steps.Add(ScStepCv.E(689, true,  "M043.1 %MX689 ON",  "Pallet Exist #28 ON (#28 도착)"));
            s3.Steps.Add(ScStepCv.D('R', 132, true,  "%RB264 (R0132)", "PLC ID Shift 결과: #27(R0130) JOB → #28(R0132)로 이동(원위치 클리어)", null));
            s3.Steps.Add(ScStepCv.E(690, true,  "M043.2 %MX690 ON",  "Pallet Exist #29 ON"));
            s3.Steps.Add(ScStepCv.E(722, true,  "M045.2 %MX722 ON",  "Pallet Exist #30 ON"));
            s3.Steps.Add(ScStepCv.E(678, true,  "M042.6 %MX678 ON",  "W.O ON"));
            s3.Steps.Add(ScStepCv.D('R', 134, true,  "%RB268 (R0134)", "JOB NO #29 Tracking Write", CV_TRK));
            s3.Steps.Add(ScStepCv.W(1493,        "M093.5 %MX1493 ON", "Unload Request #2 ON (WCS)", CV_ACK + " (ACK base+5)"));
            s3.Steps.Add(ScStepCv.E(681, true,  "M042.9 %MX681 ON",  "Unload Request #2 Ack ON"));
            s3.Steps.Add(ScStepCv.E(690, false, "M043.2 %MX690 OFF", "Pallet Exist #29 OFF"));
            s3.Steps.Add(ScStepCv.E(675, true,  "M042.3 %MX675 ON",  "Unload Complete #2 ON"));
            s3.Steps.Add(ScStepCv.W(1491,        "M093.3 %MX1491 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s3.Steps.Add(ScStepCv.E(1493, false, "M093.5 %MX1493 OFF","Unload Request #2 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStepCv.E(681, false, "M042.9 %MX681 OFF", "Unload Request #2 Ack OFF"));
            s3.Steps.Add(ScStepCv.E(675, false, "M042.3 %MX675 OFF", "Unload Complete #2 OFF"));
            s3.Steps.Add(ScStepCv.E(1491, false, "M093.3 %MX1491 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStepCv.E(678, false, "M042.6 %MX678 OFF", "W.O OFF"));
            list.Add(s3);

            // 4. 입/출고 : 중간 C/V #2 (슬라이드 14~15)
            var s4 = new ScenarioCv("반송 시나리오 (입/출고 : 중간 C/V #2) — 슬라이드 14~15");
            s4.Steps.Add(ScStepCv.E(304, true,  "M019.0 %MX304 ON",  "Pallet Exist #3 ON (RGV 하역)"));
            s4.Steps.Add(ScStepCv.E(290, true,  "M018.2 %MX290 ON",  "Load Complete #1 ON"));
            s4.Steps.Add(ScStepCv.W(1298,        "M081.2 %MX1298 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s4.Steps.Add(ScStepCv.E(290, false, "M018.2 %MX290 OFF", "Load Complete #1 OFF"));
            s4.Steps.Add(ScStepCv.E(1298, false, "M081.2 %MX1298 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s4.Steps.Add(ScStepCv.D('D', 481, false, "%DB962 (D word481)", "Direction Mode Check '0' (입고0/출고1) — 실구현 %DB962 word", CV_DIR));
            s4.Steps.Add(ScStepCv.D('R', 10, true,  "%RB20 (R0010)", "JOB NO #3 Tracking Write", CV_TRK));
            s4.Steps.Add(ScStepCv.E(304, false, "M019.0 %MX304 OFF", "Pallet Exist #3 OFF (ID Shift)"));
            s4.Steps.Add(ScStepCv.E(305, true,  "M019.1 %MX305 ON",  "Pallet Exist #4 ON (#4 도착)"));
            s4.Steps.Add(ScStepCv.D('R', 12, true,  "%RB24 (R0012)", "PLC ID Shift 결과: #3(R0010) JOB → #4(R0012)로 이동(원위치 클리어)", null));
            s4.Steps.Add(ScStepCv.E(294, true,  "M018.6 %MX294 ON",  "W.O 보고 (S/C Call)"));
            s4.Steps.Add(ScStepCv.D('R', 12, true,  "%RB24 (R0012)", "(슬15) S/C Call 시 #4 반출 대상 JOB 재표시 — 이동 스텝과 동일 값", null));
            s4.Steps.Add(ScStepCv.E(305, false, "M019.1 %MX305 OFF", "Pallet Exist #4 OFF (S/C Load 완료)"));
            s4.Steps.Add(ScStepCv.E(291, true,  "M018.3 %MX291 ON",  "Unload Complete #2 ON"));
            s4.Steps.Add(ScStepCv.W(1299,        "M081.3 %MX1299 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s4.Steps.Add(ScStepCv.E(291, false, "M018.3 %MX291 OFF", "Unload Complete #2 OFF"));
            s4.Steps.Add(ScStepCv.E(1299, false, "M081.3 %MX1299 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s4.Steps.Add(ScStepCv.E(294, false, "M018.6 %MX294 OFF", "W.O OFF"));
            list.Add(s4);

            // 5. S/C·RGV 반송 : RGV #1 (슬라이드 16~17)
            var s5 = new ScenarioCv("반송 시나리오 (S/C·RGV 반송 : RGV #1) — 슬라이드 16~17");
            s5.Steps.Add(ScStepCv.D('D', 336, false, "D0210 → %DW336(%DB672)", "RGV 상태 IDLE(1) — 초기 대기", VEH_POLL));
            s5.Steps.Add(ScStepCv.W(1616,        "M101.0 %MX1616 ON", "Transfer Command Req' ON (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStepCv.D('D', 848, false, "D0530 → %DW848(%DB1696)", "Transfer From (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStepCv.D('D', 851, false, "D0533 → %DW851(%DB1702)", "Transfer To (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStepCv.D('D', 854, true,  "D0536 → %DW854(%DB1708)", "Transfer Job No (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStepCv.E(1616, false, "M101.0 %MX1616 OFF","Transfer Command Req' OFF (설비 소비 후)"));
            s5.Steps.Add(ScStepCv.D('D', 336, false, "D0210 → %DW336(%DB672)", "RGV 상태 RUN(2) 전이 (지시 수신·상차 시작)", VEH_POLL));
            s5.Steps.Add(ScStepCv.E(928, true,  "M058.0 %MX928 ON",  "Pallet Presence ON (상차)"));
            s5.Steps.Add(ScStepCv.D('R', 310, true,  "%RB620 (R0310)", "On RGV Job No", VEH_POLL));
            s5.Steps.Add(ScStepCv.E(944, true,  "M059.0 %MX944 ON",  "Load Complete ON"));
            s5.Steps.Add(ScStepCv.D('D', 339, false, "D0213 → %DW339(%DB678)", "Transfer Complete Location", VEH_POLL));
            s5.Steps.Add(ScStepCv.W(1617,        "M101.1 %MX1617 ON", "Load Complete Ack ON (WCS)", VEH_POLL));
            s5.Steps.Add(ScStepCv.E(944, false, "M059.0 %MX944 OFF", "Load Complete OFF"));
            s5.Steps.Add(ScStepCv.E(1617, false, "M101.1 %MX1617 OFF","Load Complete Ack OFF (설비/PLC 리셋)"));
            s5.Steps.Add(ScStepCv.E(928, false, "M058.0 %MX928 OFF", "Pallet Presence OFF (하차)"));
            s5.Steps.Add(ScStepCv.E(945, true,  "M059.1 %MX945 ON",  "Unload Complete ON"));
            s5.Steps.Add(ScStepCv.D('D', 339, false, "D0213 → %DW339(%DB678)", "Transfer Complete Location (하역 완료 위치)", VEH_POLL));
            s5.Steps.Add(ScStepCv.W(1618,        "M101.2 %MX1618 ON", "Unload Complete Ack ON (WCS)", VEH_POLL));
            s5.Steps.Add(ScStepCv.E(948, true,  "M059.4 %MX948 ON",  "Transfer Complete ON"));
            s5.Steps.Add(ScStepCv.W(1621,        "M101.5 %MX1621 ON", "Transfer Complete Ack ON (WCS, slide22 메모리맵)", VEH_POLL + " (%MX1621)"));
            s5.Steps.Add(ScStepCv.E(948, false, "M059.4 %MX948 OFF", "Transfer Complete OFF"));
            s5.Steps.Add(ScStepCv.E(1621, false, "M101.5 %MX1621 OFF","Transfer Complete Ack OFF (설비/PLC 리셋)"));
            s5.Steps.Add(ScStepCv.E(945, false, "M059.1 %MX945 OFF", "Unload Complete OFF"));
            s5.Steps.Add(ScStepCv.E(1618, false, "M101.2 %MX1618 OFF","Unload Complete Ack OFF (설비/PLC 리셋)"));
            s5.Steps.Add(ScStepCv.D('D', 336, false, "D0210 → %DW336(%DB672)", "RGV 상태 IDLE(1) 복귀 (반송 완료)", VEH_POLL));
            list.Add(s5);

            // 6. 반송 : C/V #15 피킹존 입고 (V1.2 신규, 물리 30→32→31)
            var s6 = new ScenarioCv("반송 시나리오 (입고 : C/V #15 피킹존) — V1.2 신규 (물리 30→32→31)");
            s6.Steps.Add(ScStepCv.E(722, true,  "M045.2 %MX722 ON",  "Pallet Exist #30 ON (피킹존 투입)"));
            s6.Steps.Add(ScStepCv.E(706, true,  "M044.2 %MX706 ON",  "Load Complete #1 ON"));
            s6.Steps.Add(ScStepCv.W(1506,        "M094.2 %MX1506 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s6.Steps.Add(ScStepCv.E(706, false, "M044.2 %MX706 OFF", "Load Complete #1 OFF"));
            s6.Steps.Add(ScStepCv.E(1506, false, "M094.2 %MX1506 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStepCv.D('D', 494, false, "D0316 → %DW494(%DB988)", "Direction Mode Check '0' (입고 방향)", CV_DIR));
            s6.Steps.Add(ScStepCv.D('R', 144, true,  "%RB288 (R0144)", "JOB NO #30 최초 기록 (피킹존 진입 시 JOB Write)", CV_TRK));
            s6.Steps.Add(ScStepCv.E(722, false, "M045.2 %MX722 OFF", "Pallet Exist #30 OFF (ID Shift)"));
            s6.Steps.Add(ScStepCv.E(721, true,  "M045.1 %MX721 ON",  "Pallet Exist #32 ON (#32 도착)"));
            s6.Steps.Add(ScStepCv.D('R', 142, true,  "%RB284 (R0142)", "PLC ID Shift 결과: #30(R0144) JOB → #32(R0142)로 이동", null));
            s6.Steps.Add(ScStepCv.E(720, true,  "M045.0 %MX720 ON",  "Pallet Exist #31 ON (반출 위치 도착)"));
            s6.Steps.Add(ScStepCv.D('R', 140, true,  "%RB280 (R0140)", "PLC ID Shift 결과: #32(R0142) JOB → #31(R0140)로 이동(반출 대상)", null));
            s6.Steps.Add(ScStepCv.E(710, true,  "M044.6 %MX710 ON",  "W.O 보고 (S/C Call)"));
            s6.Steps.Add(ScStepCv.W(1509,        "M094.5 %MX1509 ON", "Unload Request #2 ON (WCS, ACK base+5)", CV_ACK + " (%MX1509)"));
            s6.Steps.Add(ScStepCv.E(713, true,  "M044.9 %MX713 ON",  "Unload Request #2 Ack ON"));
            s6.Steps.Add(ScStepCv.E(721, false, "M045.1 %MX721 OFF", "Pallet Exist #32 OFF"));
            s6.Steps.Add(ScStepCv.E(707, true,  "M044.3 %MX707 ON",  "Unload Complete #2 ON (S/C 반출)"));
            s6.Steps.Add(ScStepCv.W(1507,        "M094.3 %MX1507 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s6.Steps.Add(ScStepCv.E(1509, false, "M094.5 %MX1509 OFF","Unload Request #2 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStepCv.E(713, false, "M044.9 %MX713 OFF", "Unload Request #2 Ack OFF"));
            s6.Steps.Add(ScStepCv.E(707, false, "M044.3 %MX707 OFF", "Unload Complete #2 OFF"));
            s6.Steps.Add(ScStepCv.E(1507, false, "M094.3 %MX1507 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStepCv.E(710, false, "M044.6 %MX710 OFF", "W.O OFF"));
            s6.Steps.Add(ScStepCv.E(720, false, "M045.0 %MX720 OFF", "Pallet Exist #31 OFF (반출 완료)"));
            list.Add(s6);

            // 7. 알람 리포트 (Alarm Set/Reset) — 슬라이드 7 (CvThread.CvAlarmCheck 대응)
            //   설비 Alarm Set(%MX786)/Reset(%MX787) → WCS ACK(%MX1539/%MX1540). Alarm Code(%DB514/516)는 설비 기록(WCS는 코드 read 안 함).
            var s7 = new ScenarioCv("알람 시나리오 (Alarm Set / Reset) — 슬라이드 7");
            s7.Steps.Add(ScStepCv.E(786, true,  "M049.2 %MX786 ON",  "Alarm Set Report ON (설비 알람 발생)"));
            s7.Steps.Add(ScStepCv.D('D', 257, false, "D0101 (※취소·근거없음)", "Set Alarm Code — ※취소: Backup에 알람코드 주소 근거 없음(0101=STOCKER FAULT_CODE), WCS CvAlarmCheck는 비트(M786)만 관측", null));
            s7.Steps.Add(ScStepCv.W(1539,        "M096.3 %MX1539 ON", "Alarm Set Report Ack ON (WCS)", CV_ALARM));
            s7.Steps.Add(ScStepCv.E(786, false, "M049.2 %MX786 OFF", "Alarm Set Report OFF (설비 리셋)"));
            s7.Steps.Add(ScStepCv.E(1539, false, "M096.3 %MX1539 OFF","Alarm Set Report Ack OFF (설비/PLC 리셋)"));
            s7.Steps.Add(ScStepCv.E(787, true,  "M049.3 %MX787 ON",  "Alarm Reset Report ON (설비 알람 해제)"));
            s7.Steps.Add(ScStepCv.D('D', 258, false, "D0162 (※취소·근거없음)", "Reset Alarm Code — ※취소: Backup에 알람코드 주소 근거 없음(0162=STOCKER CST_X_SERVO_MAX), WCS CvAlarmCheck는 비트(M787)만 관측", null));
            s7.Steps.Add(ScStepCv.W(1540,        "M096.4 %MX1540 ON", "Alarm Reset Report Ack ON (WCS)", CV_ALARM));
            s7.Steps.Add(ScStepCv.E(787, false, "M049.3 %MX787 OFF", "Alarm Reset Report OFF (설비 리셋)"));
            s7.Steps.Add(ScStepCv.E(1540, false, "M096.4 %MX1540 OFF","Alarm Reset Report Ack OFF (설비/PLC 리셋)"));
            list.Add(s7);

            return list;
        }
    }

    // ─── 스텝 상세 창: WCS/D/R = 강제 송신 버튼, EQP = 관측 램프 ───
    internal class StepFormCv : Form
    {
        private readonly FRM_SCENARIO_TEST host;
        private readonly ScenarioCv sc;
        private readonly List<Label> obsLamps = new List<Label>();
        private readonly List<int> obsBits = new List<int>();
        private readonly List<Color> obsOffColor = new List<Color>();  // 비트 OFF 시 되돌릴 색 (EQP 관측=WhiteSmoke, WCS 강제=연파랑)
        private Timer timer;

        public StepFormCv(FRM_SCENARIO_TEST h, ScenarioCv s)
        {
            host = h; sc = s;
            Text = s.Title;
            Width = 780;
            Height = Math.Min(980, 90 + s.Steps.Count * 40 + 40);
            StartPosition = FormStartPosition.CenterParent;

            var head = new Label
            {
                Text = "[EQP 관측]=설비 비트 램프(read) · [WCS 강제]=ON/OFF 송신 버튼 · [D/R 강제]=값 입력 후 송신 (CV_TASK가 WCS로서 강제 write)",
                Dock = DockStyle.Top, Height = 30, TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(10, 0, 0, 0), ForeColor = Color.DarkBlue
            };

            // 초기화 바: 이 시나리오에 등장하는 모든 M비트(EQP 관측·WCS 강제)를 강제 OFF → 깨끗한 상태에서 시작
            var bar = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 38, Padding = new Padding(8, 4, 0, 0) };
            var btnReset = new Button { Text = "■ 초기화 (이 시나리오 M비트 전체 OFF)", Width = 280, Height = 30, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), BackColor = Color.FromArgb(255, 235, 235) };
            btnReset.Click += (s2, e2) => ResetAll();
            bar.Controls.Add(btnReset);

            var flow = new FlowLayoutPanel { Dock = DockStyle.Fill, FlowDirection = FlowDirection.TopDown, WrapContents = false, AutoScroll = true, Padding = new Padding(8) };
            int n = 1;
            foreach (var st in sc.Steps)
            {
                var row = new FlowLayoutPanel { FlowDirection = FlowDirection.LeftToRight, Width = 730, Height = 36, Margin = new Padding(1), WrapContents = false };
                if (st.Kind == 0)
                {
                    var lamp = new Label
                    {
                        Text = n + ") [EQP 관측]  " + st.Addr + "  :  " + st.Desc,
                        Width = 710, Height = 32, BorderStyle = BorderStyle.FixedSingle,
                        TextAlign = ContentAlignment.MiddleLeft, BackColor = Color.WhiteSmoke, Padding = new Padding(6, 0, 0, 0)
                    };
                    row.Controls.Add(lamp);
                    obsLamps.Add(lamp); obsBits.Add(st.Bit); obsOffColor.Add(Color.WhiteSmoke);
                }
                else if (st.Kind == 1)
                {
                    var lab = new Label
                    {
                        Text = n + ") [WCS 강제]  " + st.Addr + "  :  " + st.Desc,
                        Width = 540, Height = 32, TextAlign = ContentAlignment.MiddleLeft,
                        BackColor = Color.FromArgb(225, 240, 255), Padding = new Padding(6, 0, 0, 0), BorderStyle = BorderStyle.FixedSingle
                    };
                    var bon = new Button { Text = "ON 송신", Width = 78, Height = 30, Margin = new Padding(2, 1, 1, 1) };
                    var boff = new Button { Text = "OFF 송신", Width = 82, Height = 30, Margin = new Padding(1) };
                    var bit = st.Bit;
                    // [LGLS 2026-07-27] 색은 RefreshLamps 가 실제 PLC 비트를 실시간 read 해서 반영
                    //   → 강제 ON 후 설비/PLC 가 그 비트를 OFF 하면 색도 자동으로 원래대로 돌아옴(관측 스텝과 동일 동작)
                    bon.Click += (s2, e2) => host.SendBit(bit, true);
                    boff.Click += (s2, e2) => host.SendBit(bit, false);
                    row.Controls.Add(lab); row.Controls.Add(bon); row.Controls.Add(boff);
                    obsLamps.Add(lab); obsBits.Add(bit); obsOffColor.Add(Color.FromArgb(225, 240, 255));
                }
                else // Kind 2 : D/R 워드 강제 송신
                {
                    var lab = new Label
                    {
                        Text = n + ") [" + st.Dev + " 강제]  " + st.Addr + "  :  " + st.Desc,
                        Width = 470, Height = 32, TextAlign = ContentAlignment.MiddleLeft,
                        BackColor = Color.FromArgb(255, 250, 225), Padding = new Padding(6, 0, 0, 0), BorderStyle = BorderStyle.FixedSingle
                    };
                    var txt = new TextBox { Width = 100, Height = 28, Margin = new Padding(2, 2, 1, 1), Text = st.IsString ? "0001" : "0" };
                    var btn = new Button { Text = "송신", Width = 70, Height = 30, Margin = new Padding(1) };
                    var dev = st.Dev; var wa = st.WordAddr; var tb = txt;
                    btn.Click += (s2, e2) =>
                    {
                        bool ok = host.SendWordAscii(dev, wa, tb.Text);
                        lab.BackColor = ok ? Color.FromArgb(120, 170, 90) : Color.Firebrick;
                        lab.ForeColor = Color.White;
                    };
                    row.Controls.Add(lab); row.Controls.Add(txt); row.Controls.Add(btn);
                }
                flow.Controls.Add(row);
                n++;
            }

            Controls.Add(flow);
            Controls.Add(bar);
            Controls.Add(head);

            timer = new Timer { Interval = 400 };
            timer.Tick += (s2, e2) => RefreshLamps();
            timer.Start();
            FormClosed += (s2, e2) => timer.Stop();
        }

        // 이 시나리오에 등장하는 모든 M비트(Kind0 관측·Kind1 강제)를 OFF 로 강제 write → 잔재 제거
        private void ResetAll()
        {
            // 같은 워드를 공유하는 비트들을 스텝별 read-modify-write 하면 stale read 로 복원되므로,
            // 워드 단위로 clear 마스크를 모아 워드당 1회만 처리한다.
            var words = new Dictionary<int, int>();
            foreach (var st in sc.Steps)
                if (st.Kind == 0 || st.Kind == 1)
                {
                    int w = st.Bit / 16, bp = st.Bit % 16;
                    if (!words.ContainsKey(w)) words[w] = 0;
                    words[w] |= (1 << bp);
                }
            foreach (var kv in words) host.ClearBits(kv.Key, kv.Value);
        }

        private void RefreshLamps()
        {
            for (int i = 0; i < obsLamps.Count; i++)
            {
                bool on;
                if (!host.ReadBit(obsBits[i], out on)) continue;
                obsLamps[i].BackColor = on ? Color.SeaGreen : obsOffColor[i];
                obsLamps[i].ForeColor = on ? Color.White : Color.Black;
            }
        }

        // [LGLS 2026-07-27] 비활성 창 첫 클릭이 창 활성화(MA_ACTIVATEANDEAT)로 소비되지 않고
        //   버튼까지 바로 전달되도록(MA_ACTIVATE) — 초기화/송신 버튼 첫 클릭 무시 문제 해결
        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == 0x0021 && m.Result == (IntPtr)2) m.Result = (IntPtr)1; // MA_ACTIVATEANDEAT(2)→MA_ACTIVATE(1) // WM_MOUSEACTIVATE
        }
    }
}
