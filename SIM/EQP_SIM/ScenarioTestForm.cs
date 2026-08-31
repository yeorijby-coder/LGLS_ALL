using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Windows.Forms;
using EQP_SIM.Core;
using EQP_SIM.Sim;

namespace EQP_SIM
{
    // [LGLS 2026-07-27] [시나리오 테스트] — 사용자 제공 PPTX(ECS-분석-PLC사양및시나리오 V1.1) 슬라이드 8~17.
    //   버튼 1=슬8~9, 2=슬10~11, 3=슬12~13, 4=슬14~15, 5=슬16~17 (각 반송의 전체 핸드셰이크).
    //   ★스텝은 실제 PPTX 도형의 y좌표(위→아래) 순서로 나열한다. M비트 + D/R 워드관측을 하나의 순서로 통합.
    //   ★열(x좌표) 규약: 오른쪽열=WCS 반응(ACK ON), 왼쪽열=설비/PLC(설비 송신·ACK OFF 리셋), 중앙=트래킹(WCS D/R 쓰기).
    //     → ACK OFF(예 M090.4 OFF)는 설비/PLC 리셋이라 WCS 반응이 아님(핸들러 표시 없음).

    // 통합 스텝: Kind 0=설비/PLC 비트(버튼), 1=WCS ACK 비트(램프 관측), 2=D/R 워드 관측(값 표시)
    internal class ScStep
    {
        public int Kind;         // 0=EQP/PLC 비트, 1=WCS ACK 비트, 2=D/R 워드 관측
        public char Dev;         // 'M' / 'D' / 'R'
        public int Bit;          // M 비트 절대주소 (Kind 0/1)
        public int WordAddr;     // D/R 워드주소 (Kind 2)
        public bool Val;         // M 비트 ON(true)/OFF(false) (Kind 0)
        public bool IsString;    // D/R 문자열(JOB NO 등) (Kind 2)
        public string Addr;      // 표기 (예: "M090.4 %MX1444 ON", "%DB980 (D0310)")
        public string Desc;
        public string Src;       // WCS 처리위치 [시스템]파일.cs::함수 (Kind 1/2). 설비/PLC(Kind 0)는 null

        private ScStep() { }

        // 설비/PLC 가 쓰는 M 비트 (버튼으로 재현). ACK OFF(리셋)도 설비/PLC 몫이므로 이걸로.
        public static ScStep E(int bit, bool val, string addr, string desc)
        { return new ScStep { Kind = 0, Dev = 'M', Bit = bit, Val = val, Addr = addr, Desc = desc }; }

        // WCS 가 쓰는 ACK M 비트 ON (램프 관측 + 처리위치)
        public static ScStep W(int bit, string addr, string desc, string src)
        { return new ScStep { Kind = 1, Dev = 'M', Bit = bit, Val = true, Addr = addr, Desc = desc, Src = src }; }

        // D/R 워드/문자열 관측 (WCS 가 쓰는 Direction/JOB/Transfer Data 등, 값 표시 + 처리위치)
        public static ScStep D(char dev, int wordAddr, bool isStr, string addr, string desc, string src)
        { return new ScStep { Kind = 2, Dev = dev, WordAddr = wordAddr, IsString = isStr, Addr = addr, Desc = desc, Src = src }; }
    }

    internal class Scenario
    {
        public string Title;
        // [LGLS 2026-08-19] XML(PlcAddressMap.xml) 유래 정보
        public string Id;                  // "1" / "2" / "3-1" ...
        public bool   CraneSelectable;     // crane="select" 이면 스텝 창에서 호기(1~5) 선택
        public int    CraneNo = 1;
        public List<ScStep> Steps = new List<ScStep>();
        public Scenario(string title) { Title = title; }
    }

    internal class ScenarioTestForm : Form
    {
        private readonly ScenarioEngine engine;
        private readonly PlcMemory mem;
        private readonly List<Scenario> scenarios;
        private DataGridView gridObs;
        private Timer timer;

        public ScenarioTestForm(ScenarioEngine engine)
        {
            this.engine = engine;
            this.mem = engine.Memory;
            this.scenarios = BuildScenarios();

            Text = "[시나리오 테스트] — 자동 운전 정지됨 (PlcAddressMap.xml 기준)";
            Width = 1120; Height = 820; StartPosition = FormStartPosition.CenterScreen;

            var lbl = new Label
            {
                Text = "각 시나리오 버튼 = 반송 전체. 스텝 창에서 호기(1~5)를 고르면 주소가 그 호기로 재계산됩니다. 창을 닫으면 자동 운전이 재개됩니다.",
                Dock = DockStyle.Top, Height = 28, TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(10, 0, 0, 0), ForeColor = Color.DarkRed
            };

            var pnl = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 245, Padding = new Padding(10), FlowDirection = FlowDirection.TopDown, WrapContents = false, AutoScroll = true };
            for (int i = 0; i < scenarios.Count; i++)
            {
                var sc = scenarios[i];
                var b = new Button
                {
                    // [LGLS 2026-08-19] XML Title 이 이미 "1. …" / "3-1. …" 로 시작하므로 번호를 덧붙이지 않는다
                    Text = string.IsNullOrEmpty(sc.Id) ? ((i + 1) + ". " + sc.Title) : sc.Title,
                    Width = 1080, Height = 32, Margin = new Padding(3),
                    Font = new Font("맑은 고딕", 10F, FontStyle.Bold),
                    TextAlign = ContentAlignment.MiddleLeft
                };
                var scLocal = sc;
                b.Click += (s, e) => new ScenarioStepForm(mem, scLocal).Show(this);
                pnl.Controls.Add(b);
            }

            // 하단: M/D/R 직접 읽기/쓰기 3영역 (가로 분할, 자동 리프레시)
            var tlp = new TableLayoutPanel { Dock = DockStyle.Fill, ColumnCount = 3, RowCount = 1 };
            tlp.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.4f));
            tlp.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.3f));
            tlp.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33.3f));
            tlp.Controls.Add(new MemAreaPanel(mem, 'M', "MX", true), 0, 0);
            tlp.Controls.Add(new MemAreaPanel(mem, 'D', "DW", false), 1, 0);
            tlp.Controls.Add(new MemAreaPanel(mem, 'R', "RW", false), 2, 0);

            var lblMDR = new Label { Text = "  ▼ M / D / R 영역 직접 읽기·쓰기 (자동 리프레시 · 시작주소 + Count 읽기 / 값 쓰기)", Dock = DockStyle.Top, Height = 22, Font = new Font("맑은 고딕", 9F, FontStyle.Bold) };

            // 상단: 시나리오 관측
            gridObs = new DataGridView
            {
                Dock = DockStyle.Top, Height = 170, ReadOnly = true, AllowUserToAddRows = false,
                RowHeadersVisible = false, AllowUserToResizeRows = false,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect
            };
            gridObs.Columns.Add("addr", "주소");
            gridObs.Columns.Add("desc", "의미");
            gridObs.Columns.Add("state", "상태/값");
            gridObs.Columns.Add("src", "WCS 반응 처리위치 [시스템]파일.cs::함수");
            gridObs.Columns[0].Width = 110; gridObs.Columns[1].Width = 360; gridObs.Columns[2].Width = 70; gridObs.Columns[3].Width = 350;
            BuildObsRows();
            var lblObs = new Label { Text = "  ▼ 비트/워드 상태 관측판 (중복 제거 · 실제 진행 순서는 각 시나리오 창 참조)", Dock = DockStyle.Top, Height = 22, Font = new Font("맑은 고딕", 9F, FontStyle.Bold) };

            Controls.Add(tlp);
            Controls.Add(lblMDR);
            Controls.Add(gridObs);
            Controls.Add(lblObs);
            Controls.Add(pnl);
            Controls.Add(lbl);

            timer = new Timer { Interval = 300 };
            timer.Tick += (s, e) => RefreshObs();
            timer.Start();
            FormClosed += (s, e) => { timer.Stop(); engine.Resume(); };
        }

        // 관측 행: Kind 0/1=비트, 2=워드(문자열이면 str).
        private class ObsRow { public int Kind; public char Dev; public int Addr; public bool IsString; }
        private readonly List<ObsRow> obsRows = new List<ObsRow>();

        private void BuildObsRows()
        {
            var seenBit = new HashSet<int>();
            var seenWord = new HashSet<string>();
            foreach (var sc in scenarios)
            {
                foreach (var st in sc.Steps)
                {
                    if (st.Kind == 2)
                    {
                        string key = st.Dev + ":" + st.WordAddr;
                        if (seenWord.Contains(key)) continue;
                        seenWord.Add(key);
                        obsRows.Add(new ObsRow { Kind = 2, Dev = st.Dev, Addr = st.WordAddr, IsString = st.IsString });
                        gridObs.Rows.Add(st.Addr, "[관측] " + st.Desc, "", st.Src ?? "");
                    }
                    else
                    {
                        if (seenBit.Contains(st.Bit)) continue;
                        seenBit.Add(st.Bit);
                        obsRows.Add(new ObsRow { Kind = st.Kind, Dev = 'M', Addr = st.Bit });
                        string a = st.Addr.Contains("%MX") ? st.Addr.Substring(st.Addr.IndexOf("%MX")) : ("%MX" + st.Bit);
                        // %MX593 ON / OFF 표기에서 주소만
                        int sp = a.IndexOf(' '); if (sp > 0) a = a.Substring(0, sp);
                        string tag = st.Kind == 1 ? "[WCS] " : "[EQP] ";
                        gridObs.Rows.Add(a, tag + st.Desc, "", st.Src ?? "");
                    }
                }
            }
        }

        private void RefreshObs()
        {
            for (int i = 0; i < obsRows.Count; i++)
            {
                var r = obsRows[i];
                var cell = gridObs.Rows[i].Cells[2];
                string txt; bool hot;
                if (r.Kind != 2)
                {
                    hot = mem.GetBit(r.Dev, r.Addr);
                    txt = hot ? "ON" : "off";
                }
                else if (r.IsString)
                {
                    string s = mem.GetString(r.Dev, r.Addr, 2);
                    txt = string.IsNullOrEmpty(s) ? "-" : s;
                    hot = !string.IsNullOrEmpty(s) && s != "0" && s != "0000";
                }
                else
                {
                    ushort w = mem.GetWord(r.Dev, r.Addr);
                    txt = w.ToString("X4");
                    hot = w != 0;
                }
                if ((cell.Value as string) != txt) cell.Value = txt;
                cell.Style.BackColor = hot ? Color.SeaGreen : Color.White;
                cell.Style.ForeColor = hot ? Color.White : Color.Black;
            }
        }

        // ------------------------------------------------------------------
        // PPT 시나리오 정의 (슬라이드 쌍, 실제 PPTX 도형 y순 통합)
        // ------------------------------------------------------------------
        // ── [LGLS 2026-08-19] XML(PlcAddressMap.xml) 에서 시나리오 생성 ────────────
        //   EQP_TASK 와 같은 XML 1개를 공유한다(파서 cPlcAddrMap.cs 는 링크 참조).
        //   ★역할이 반대다 : EQP_SIM 은 "설비"이므로
        //       XML kind=observe(설비가 쓰는 비트) → 여기서는 [EQP 송신] 버튼
        //       XML kind=force  (WCS 가 쓰는 ACK)  → 여기서는 [WCS 반응] 관측 램프
        //       XML kind=word   (D/R)              → 값 관측
        internal static string EqTag(string equip, int no)
        {
            if (string.Equals(equip, "CV", StringComparison.OrdinalIgnoreCase))  return "C/V #" + no + "  ";
            if (string.Equals(equip, "SC", StringComparison.OrdinalIgnoreCase))  return "S/C #" + no + "  ";
            if (string.Equals(equip, "RGV", StringComparison.OrdinalIgnoreCase)) return "RGV #" + no + "  ";
            return "";
        }

        private static string BitLabel(int bit)
        {
            return string.Format("M{0:000}.{1} %MX{2}", bit / 16, bit % 16, bit);
        }

        internal static List<Scenario> BuildScenariosFromXml(int craneNo)
        {
            try
            {
                if (!WCS_TASK_CV.cPlcAddrMap.IsLoaded) return null;
                var defs = WCS_TASK_CV.cPlcAddrMap.Scenarios();
                if (defs == null || defs.Count == 0) return null;

                var list = new List<Scenario>();
                foreach (var sd in defs)
                {
                    var sc = new Scenario(sd.Title);
                    sc.Id = sd.Id;
                    sc.CraneSelectable = sd.CraneSelectable;
                    sc.CraneNo = craneNo;
                    foreach (var st in sd.Steps)
                    {
                        int no = WCS_TASK_CV.cPlcAddrMap.ResolveNo(st.NoExpr, craneNo, st.No);
                        int addr = st.RawAddr;
                        if (addr < 0)
                        {
                            if (string.Equals(st.Equip, "Global", StringComparison.OrdinalIgnoreCase))
                                addr = WCS_TASK_CV.cPlcAddrMap.GlobalBit(st.Signal);
                            else
                                addr = WCS_TASK_CV.cPlcAddrMap.Addr(st.Equip, no, st.Block, st.Signal, st.Slot);
                        }
                        if (addr < 0) continue;   // 정의 누락 스텝은 건너뛴다

                        string tag = EqTag(st.Equip, no);
                        if (st.Kind == "observe")
                            sc.Steps.Add(ScStep.E(addr, st.Value, tag + BitLabel(addr) + (st.Value ? " ON" : " OFF"), st.Desc));
                        else if (st.Kind == "force")
                            sc.Steps.Add(ScStep.W(addr, tag + BitLabel(addr) + " ON", st.Desc, st.Src));
                        else
                        {
                            string lbl = tag + ((st.Device == 'R')
                                       ? string.Format("%RB{0} (워드 {1})", addr * 2, addr)
                                       : string.Format("%DW{0} (%DB{1})", addr, addr * 2));
                            sc.Steps.Add(ScStep.D(st.Device, addr, st.IsString, lbl, st.Desc, st.Src));
                        }
                    }
                    if (sc.Steps.Count > 0) list.Add(sc);
                }
                return (list.Count > 0) ? list : null;
            }
            catch { return null; }
        }

        private static List<Scenario> BuildScenarios() { return BuildScenarios(1); }

        private static List<Scenario> BuildScenarios(int craneNo)
        {
            var fromXml = BuildScenariosFromXml(craneNo);
            if (fromXml != null) return fromXml;

            const string CV_ACK = "[WCS_TASK_CV]CvThread.cs::CvEventCheck";
            const string CV_TRK = "[WCS_TASK_CV]CvThread.cs::CvTrackingWrite";
            const string CV_DIR = "[WCS_TASK_CV]CvThread.cs::CvStatusScenario (읽기→STOCK_MODE)";
            const string VEH_CMD = "[WCS_TASK_CV]VehThread.cs::ConsumeCommands";
            const string VEH_POLL = "[WCS_TASK_CV]VehThread.cs::PollObservations";
            const string CV_ALARM = "[WCS_TASK_CV]CvThread.cs::CvAlarmCheck";

            var list = new List<Scenario>();

            // 1. 입고 : C/V #11 (슬라이드 8~9)
            var s1 = new Scenario("반송 시나리오 (입고 : C/V #11) — 슬라이드 8~9");
            s1.Steps.Add(ScStep.E(593, true,  "M037.1 %MX593 ON",  "Pallet Exist #22 ON (입고대 적재)"));
            s1.Steps.Add(ScStep.E(580, true,  "M036.4 %MX580 ON",  "Load Complete #2 ON"));
            s1.Steps.Add(ScStep.W(1444,        "M090.4 %MX1444 ON", "Load Complete Ack #2 ON (WCS)", CV_ACK));
            s1.Steps.Add(ScStep.E(580, false, "M036.4 %MX580 OFF", "Load Complete #2 OFF"));
            s1.Steps.Add(ScStep.E(1444, false, "M090.4 %MX1444 OFF","Load Complete Ack #2 OFF (설비/PLC 리셋)"));
            s1.Steps.Add(ScStep.D('D', 490, false, "%DB980 (D0310→W01EA)", "Direction Mode Check '1' (입고0/출고1)", CV_DIR));
            s1.Steps.Add(ScStep.D('R', 102, true,  "%RB204 (R0102)", "JOB NO #22 최초 기록 (입고 진입 시 #22 위치에 JOB Write)", CV_TRK));
            s1.Steps.Add(ScStep.E(593, false, "M037.1 %MX593 OFF", "Pallet Exist #22 OFF (ID Shift)"));
            s1.Steps.Add(ScStep.E(592, true,  "M037.0 %MX592 ON",  "Pallet Exist #21 ON (#21 도착)"));
            s1.Steps.Add(ScStep.D('R', 100, true,  "%RB200 (R0100)", "PLC ID Shift 결과: #22(R0102) JOB → #21(R0100)로 이동(원위치 #22 클리어)", null));
            // 슬9: RGV Call → Unload Complete #1
            s1.Steps.Add(ScStep.E(582, true,  "M036.6 %MX582 ON",  "W.O 보고 (RGV Call)"));
            s1.Steps.Add(ScStep.D('R', 100, true,  "%RB200 (R0100)", "(슬9) RGV Call 시 #21(R0100) 반출 대상 JOB 재표시 — 10번과 동일 값(새 트래킹 아님). RGV지시는 D0530~", null));
            s1.Steps.Add(ScStep.E(592, false, "M037.0 %MX592 OFF", "Pallet Exist #21 OFF (RGV Load 완료)"));
            s1.Steps.Add(ScStep.E(577, true,  "M036.1 %MX577 ON",  "Unload Complete #1 ON"));
            s1.Steps.Add(ScStep.W(1441,        "M090.1 %MX1441 ON", "Unload Complete Ack #1 ON (WCS)", CV_ACK));
            s1.Steps.Add(ScStep.E(577, false, "M036.1 %MX577 OFF", "Unload Complete #1 OFF"));
            s1.Steps.Add(ScStep.E(1441, false, "M090.1 %MX1441 OFF","Unload Complete Ack #1 OFF (설비/PLC 리셋)"));
            s1.Steps.Add(ScStep.E(582, false, "M036.6 %MX582 OFF", "W.O OFF"));
            list.Add(s1);

            // 2. 출고 : C/V #11 (슬라이드 10~11)
            var s2 = new Scenario("반송 시나리오 (출고 : C/V #11) — 슬라이드 10~11");
            s2.Steps.Add(ScStep.E(592, true,  "M037.0 %MX592 ON",  "Pallet Exist #21 ON (RGV 하역)"));
            s2.Steps.Add(ScStep.E(578, true,  "M036.2 %MX578 ON",  "Load Complete #1 ON"));
            s2.Steps.Add(ScStep.W(1442,        "M090.2 %MX1442 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s2.Steps.Add(ScStep.E(578, false, "M036.2 %MX578 OFF", "Load Complete #1 OFF"));
            s2.Steps.Add(ScStep.E(1442, false, "M090.2 %MX1442 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s2.Steps.Add(ScStep.D('D', 490, false, "%DB980 (D0310→W01EA)", "Direction Mode Check '1' (출고1)", CV_DIR));
            s2.Steps.Add(ScStep.D('R', 100, true,  "%RB200 (R0100)", "JOB NO #21 Tracking Write", CV_TRK));
            s2.Steps.Add(ScStep.E(592, false, "M037.0 %MX592 OFF", "Pallet Exist #21 OFF (ID Shift)"));
            s2.Steps.Add(ScStep.E(593, true,  "M037.1 %MX593 ON",  "Pallet Exist #22 ON (#22 도착)"));
            s2.Steps.Add(ScStep.D('R', 102, true,  "%RB204 (R0102)", "PLC ID Shift 결과: #21(R0100) JOB → #22(R0102)로 이동(원위치 클리어)", null));
            // 슬11: 작업자 Unload
            s2.Steps.Add(ScStep.E(579, true,  "M036.3 %MX579 ON",  "Unload Complete #2 ON (작업자 Unload)"));
            s2.Steps.Add(ScStep.W(1443,        "M090.3 %MX1443 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s2.Steps.Add(ScStep.E(579, false, "M036.3 %MX579 OFF", "Unload Complete #2 OFF"));
            s2.Steps.Add(ScStep.E(1443, false, "M090.3 %MX1443 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            list.Add(s2);

            // 3. 출고 : C/V #14 피킹존 (슬라이드 12~13)
            var s3 = new Scenario("반송 시나리오 (출고 : C/V #14 피킹존) — 슬라이드 12~13");
            s3.Steps.Add(ScStep.E(688, true,  "M043.0 %MX688 ON",  "Pallet Exist #27 ON (RGV 하역)"));
            s3.Steps.Add(ScStep.E(674, true,  "M042.2 %MX674 ON",  "Load Complete #1 ON"));
            s3.Steps.Add(ScStep.W(1490,        "M093.2 %MX1490 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s3.Steps.Add(ScStep.E(674, false, "M042.2 %MX674 OFF", "Load Complete #1 OFF"));
            s3.Steps.Add(ScStep.E(1490, false, "M093.2 %MX1490 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStep.D('R', 130, true,  "%RB260 (R0130)", "JOB NO #27 Tracking Write", CV_TRK));
            s3.Steps.Add(ScStep.E(688, false, "M043.0 %MX688 OFF", "Pallet Exist #27 OFF (ID Shift)"));
            s3.Steps.Add(ScStep.E(689, true,  "M043.1 %MX689 ON",  "Pallet Exist #28 ON (#28 도착)"));
            s3.Steps.Add(ScStep.D('R', 132, true,  "%RB264 (R0132)", "PLC ID Shift 결과: #27(R0130) JOB → #28(R0132)로 이동(원위치 클리어)", null));
            // 슬13: 피킹존 Unload Request #2
            s3.Steps.Add(ScStep.E(690, true,  "M043.2 %MX690 ON",  "Pallet Exist #29 ON"));
            s3.Steps.Add(ScStep.E(722, true,  "M045.2 %MX722 ON",  "Pallet Exist #30 ON"));
            s3.Steps.Add(ScStep.E(678, true,  "M042.6 %MX678 ON",  "W.O ON"));
            s3.Steps.Add(ScStep.D('R', 134, true,  "%RB268 (R0134)", "JOB NO #29 Tracking Write", CV_TRK));
            s3.Steps.Add(ScStep.W(1493,        "M093.5 %MX1493 ON", "Unload Request #2 ON (WCS)", CV_ACK + " (ACK base+5)"));
            s3.Steps.Add(ScStep.E(681, true,  "M042.9 %MX681 ON",  "Unload Request #2 Ack ON"));
            s3.Steps.Add(ScStep.E(690, false, "M043.2 %MX690 OFF", "Pallet Exist #29 OFF"));
            s3.Steps.Add(ScStep.E(675, true,  "M042.3 %MX675 ON",  "Unload Complete #2 ON"));
            s3.Steps.Add(ScStep.W(1491,        "M093.3 %MX1491 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s3.Steps.Add(ScStep.E(1493, false, "M093.5 %MX1493 OFF","Unload Request #2 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStep.E(681, false, "M042.9 %MX681 OFF", "Unload Request #2 Ack OFF"));
            s3.Steps.Add(ScStep.E(675, false, "M042.3 %MX675 OFF", "Unload Complete #2 OFF"));
            s3.Steps.Add(ScStep.E(1491, false, "M093.3 %MX1491 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s3.Steps.Add(ScStep.E(678, false, "M042.6 %MX678 OFF", "W.O OFF"));
            list.Add(s3);

            // 4. 입/출고 : 중간 C/V #2 (슬라이드 14~15)
            var s4 = new Scenario("반송 시나리오 (입/출고 : 중간 C/V #2) — 슬라이드 14~15");
            s4.Steps.Add(ScStep.E(304, true,  "M019.0 %MX304 ON",  "Pallet Exist #3 ON (RGV 하역)"));
            s4.Steps.Add(ScStep.E(290, true,  "M018.2 %MX290 ON",  "Load Complete #1 ON"));
            s4.Steps.Add(ScStep.W(1298,        "M081.2 %MX1298 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s4.Steps.Add(ScStep.E(290, false, "M018.2 %MX290 OFF", "Load Complete #1 OFF"));
            s4.Steps.Add(ScStep.E(1298, false, "M081.2 %MX1298 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s4.Steps.Add(ScStep.D('D', 481, false, "%DB962 (D word481, PPT: M024.9확인·M018.9설정)", "Direction Mode Check '0' (입고0/출고1) — 실구현은 %DB962 word", CV_DIR));
            s4.Steps.Add(ScStep.D('R', 10, true,  "%RB20 (R0010)", "JOB NO #3 Tracking Write", CV_TRK));
            s4.Steps.Add(ScStep.E(304, false, "M019.0 %MX304 OFF", "Pallet Exist #3 OFF (ID Shift)"));
            s4.Steps.Add(ScStep.E(305, true,  "M019.1 %MX305 ON",  "Pallet Exist #4 ON (#4 도착)"));
            s4.Steps.Add(ScStep.D('R', 12, true,  "%RB24 (R0012)", "PLC ID Shift 결과: #3(R0010) JOB → #4(R0012)로 이동(원위치 클리어)", null));
            // 슬15: S/C Call → Unload Complete #2
            s4.Steps.Add(ScStep.E(294, true,  "M018.6 %MX294 ON",  "W.O 보고 (S/C Call)"));
            s4.Steps.Add(ScStep.D('R', 12, true,  "%RB24 (R0012)", "(슬15) S/C Call 시 #4 반출 대상 JOB 재표시 — 이동 스텝과 동일 값(새 트래킹 아님)", null));
            s4.Steps.Add(ScStep.E(305, false, "M019.1 %MX305 OFF", "Pallet Exist #4 OFF (S/C Load 완료)"));
            s4.Steps.Add(ScStep.E(291, true,  "M018.3 %MX291 ON",  "Unload Complete #2 ON"));
            s4.Steps.Add(ScStep.W(1299,        "M081.3 %MX1299 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s4.Steps.Add(ScStep.E(291, false, "M018.3 %MX291 OFF", "Unload Complete #2 OFF"));
            s4.Steps.Add(ScStep.E(1299, false, "M081.3 %MX1299 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s4.Steps.Add(ScStep.E(294, false, "M018.6 %MX294 OFF", "W.O OFF"));
            list.Add(s4);

            // 5. S/C·RGV 반송 : RGV #1 (슬라이드 16~17)
            var s5 = new Scenario("반송 시나리오 (S/C·RGV 반송 : RGV #1) — 슬라이드 16~17");
            s5.Steps.Add(ScStep.D('D', 336, false, "%DB672 (D0210→W0150)", "RGV 상태 IDLE(1) — 초기 대기", VEH_POLL));
            s5.Steps.Add(ScStep.W(1616,        "M101.0 %MX1616 ON", "Transfer Command Req' ON (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStep.D('D', 848, false, "%DB1696 (D0530→W0350)", "Transfer From (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStep.D('D', 851, false, "%DB1702 (D0533→W0353)", "Transfer To (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStep.D('D', 854, true,  "%DB1708 (D0536→W0356)", "Transfer Job No (WCS 지시)", VEH_CMD));
            s5.Steps.Add(ScStep.E(1616, false, "M101.0 %MX1616 OFF","Transfer Command Req' OFF (설비 소비 후)"));
            s5.Steps.Add(ScStep.D('D', 336, false, "%DB672 (D0210→W0150)", "RGV 상태 RUN(2) 전이 (지시 수신·상차 시작)", VEH_POLL));
            s5.Steps.Add(ScStep.E(928, true,  "M058.0 %MX928 ON",  "Pallet Presence ON (상차)"));
            s5.Steps.Add(ScStep.D('R', 310, true,  "%RB620 (R0310)", "On RGV Job No", VEH_POLL));
            s5.Steps.Add(ScStep.E(944, true,  "M059.0 %MX944 ON",  "Load Complete ON"));
            s5.Steps.Add(ScStep.D('D', 339, false, "%DB678 (D0213→W0153)", "Transfer Complete Location", VEH_POLL));
            s5.Steps.Add(ScStep.W(1617,        "M101.1 %MX1617 ON", "Load Complete Ack ON (WCS)", VEH_POLL));
            s5.Steps.Add(ScStep.E(944, false, "M059.0 %MX944 OFF", "Load Complete OFF"));
            s5.Steps.Add(ScStep.E(1617, false, "M101.1 %MX1617 OFF","Load Complete Ack OFF (설비/PLC 리셋)"));
            // 슬17: 하차 → Unload Complete → Transfer Complete (Ack는 slide22 메모리맵 RGV#1 +5=M101.5)
            s5.Steps.Add(ScStep.E(928, false, "M058.0 %MX928 OFF", "Pallet Presence OFF (하차)"));
            s5.Steps.Add(ScStep.E(945, true,  "M059.1 %MX945 ON",  "Unload Complete ON"));
            s5.Steps.Add(ScStep.D('D', 339, false, "%DB678 (D0213→W0153)", "Transfer Complete Location (하역 완료 위치)", VEH_POLL));
            s5.Steps.Add(ScStep.W(1618,        "M101.2 %MX1618 ON", "Unload Complete Ack ON (WCS)", VEH_POLL));
            s5.Steps.Add(ScStep.E(948, true,  "M059.4 %MX948 ON",  "Transfer Complete ON"));
            s5.Steps.Add(ScStep.W(1621,        "M101.5 %MX1621 ON", "Transfer Complete Ack ON (WCS, slide22 메모리맵)", VEH_POLL + " (%MX1621)"));
            s5.Steps.Add(ScStep.E(948, false, "M059.4 %MX948 OFF", "Transfer Complete OFF"));
            s5.Steps.Add(ScStep.E(1621, false, "M101.5 %MX1621 OFF","Transfer Complete Ack OFF (설비/PLC 리셋)"));
            s5.Steps.Add(ScStep.E(945, false, "M059.1 %MX945 OFF", "Unload Complete OFF"));
            s5.Steps.Add(ScStep.E(1618, false, "M101.2 %MX1618 OFF","Unload Complete Ack OFF (설비/PLC 리셋)"));
            s5.Steps.Add(ScStep.D('D', 336, false, "%DB672 (D0210→W0150)", "RGV 상태 IDLE(1) 복귀 (반송 완료)", VEH_POLL));
            list.Add(s5);

            // 6. 반송 : C/V #15 피킹존 입고 (V1.2 신규, 물리 30→32→31)
            //    근거: Backup ECSDeviceManager.cs:660 IsInputConveyor(#15=input),
            //          ConveyorScenario.cs:304 GetNextPort(input=PortOrder 내림차순 N→1),
            //          GetWaitOutPort(input=PortOrder 1).  PortOrder↔위치: PO3=#30, PO2=#32, PO1=#31
            //    → 물리 파렛트 이동 #30(진입)→#32→#31(반출). 신호셋은 C/V#14와 공통, 방향/반출위치만 반대.
            //    주소: Event=M044, PalletExist=M045(#31=720/#32=721/#30=722), Ack=M094, JOB R0140/0142/0144
            var s6 = new Scenario("반송 시나리오 (입고 : C/V #15 피킹존) — V1.2 신규 (물리 30→32→31)");
            // 전반: #30 피킹존 투입 → Load Complete #1 → Direction(입고0) → ID Shift #30→#32
            s6.Steps.Add(ScStep.E(722, true,  "M045.2 %MX722 ON",  "Pallet Exist #30 ON (피킹존 투입)"));
            s6.Steps.Add(ScStep.E(706, true,  "M044.2 %MX706 ON",  "Load Complete #1 ON"));
            s6.Steps.Add(ScStep.W(1506,        "M094.2 %MX1506 ON", "Load Complete Ack #1 ON (WCS)", CV_ACK));
            s6.Steps.Add(ScStep.E(706, false, "M044.2 %MX706 OFF", "Load Complete #1 OFF"));
            s6.Steps.Add(ScStep.E(1506, false, "M094.2 %MX1506 OFF","Load Complete Ack #1 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStep.D('D', 494, false, "%DB988 (D0316→W01EE)", "Direction Mode Check '0' (입고 방향)", CV_DIR));
            s6.Steps.Add(ScStep.D('R', 144, true,  "%RB288 (R0144)", "JOB NO #30 최초 기록 (피킹존 진입 시 JOB Write)", CV_TRK));
            s6.Steps.Add(ScStep.E(722, false, "M045.2 %MX722 OFF", "Pallet Exist #30 OFF (ID Shift)"));
            s6.Steps.Add(ScStep.E(721, true,  "M045.1 %MX721 ON",  "Pallet Exist #32 ON (#32 도착)"));
            s6.Steps.Add(ScStep.D('R', 142, true,  "%RB284 (R0142)", "PLC ID Shift 결과: #30(R0144) JOB → #32(R0142)로 이동", null));
            // 후반: #31 반출 위치 도착 → W.O(S/C Call) → Unload Request #2 → S/C 반출 → 완료
            s6.Steps.Add(ScStep.E(720, true,  "M045.0 %MX720 ON",  "Pallet Exist #31 ON (반출 위치 도착)"));
            s6.Steps.Add(ScStep.D('R', 140, true,  "%RB280 (R0140)", "PLC ID Shift 결과: #32(R0142) JOB → #31(R0140)로 이동(반출 대상)", null));
            s6.Steps.Add(ScStep.E(710, true,  "M044.6 %MX710 ON",  "W.O 보고 (S/C Call)"));
            s6.Steps.Add(ScStep.W(1509,        "M094.5 %MX1509 ON", "Unload Request #2 ON (WCS, ACK base+5)", CV_ACK + " (%MX1509)"));
            s6.Steps.Add(ScStep.E(713, true,  "M044.9 %MX713 ON",  "Unload Request #2 Ack ON"));
            s6.Steps.Add(ScStep.E(721, false, "M045.1 %MX721 OFF", "Pallet Exist #32 OFF"));
            s6.Steps.Add(ScStep.E(707, true,  "M044.3 %MX707 ON",  "Unload Complete #2 ON (S/C 반출)"));
            s6.Steps.Add(ScStep.W(1507,        "M094.3 %MX1507 ON", "Unload Complete Ack #2 ON (WCS)", CV_ACK));
            s6.Steps.Add(ScStep.E(1509, false, "M094.5 %MX1509 OFF","Unload Request #2 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStep.E(713, false, "M044.9 %MX713 OFF", "Unload Request #2 Ack OFF"));
            s6.Steps.Add(ScStep.E(707, false, "M044.3 %MX707 OFF", "Unload Complete #2 OFF"));
            s6.Steps.Add(ScStep.E(1507, false, "M094.3 %MX1507 OFF","Unload Complete Ack #2 OFF (설비/PLC 리셋)"));
            s6.Steps.Add(ScStep.E(710, false, "M044.6 %MX710 OFF", "W.O OFF"));
            s6.Steps.Add(ScStep.E(720, false, "M045.0 %MX720 OFF", "Pallet Exist #31 OFF (반출 완료)"));
            list.Add(s6);

            // 7. 알람 리포트 (Alarm Set/Reset) — 슬라이드 7 (CvThread.CvAlarmCheck 대응)
            var s7 = new Scenario("알람 시나리오 (Alarm Set / Reset) — 슬라이드 7");
            s7.Steps.Add(ScStep.E(786, true,  "M049.2 %MX786 ON",  "Alarm Set Report ON (설비 알람 발생)"));
            s7.Steps.Add(ScStep.D('D', 257, false, "D0101 (※취소·근거없음)", "Set Alarm Code — ※취소: Backup에 알람코드 주소 근거 없음(0101=STOCKER FAULT_CODE), WCS CvAlarmCheck는 비트(M786)만 관측", null));
            s7.Steps.Add(ScStep.W(1539,        "M096.3 %MX1539 ON", "Alarm Set Report Ack ON (WCS)", CV_ALARM));
            s7.Steps.Add(ScStep.E(786, false, "M049.2 %MX786 OFF", "Alarm Set Report OFF (설비 리셋)"));
            s7.Steps.Add(ScStep.E(1539, false, "M096.3 %MX1539 OFF","Alarm Set Report Ack OFF (설비/PLC 리셋)"));
            s7.Steps.Add(ScStep.E(787, true,  "M049.3 %MX787 ON",  "Alarm Reset Report ON (설비 알람 해제)"));
            s7.Steps.Add(ScStep.D('D', 258, false, "D0162 (※취소·근거없음)", "Reset Alarm Code — ※취소: Backup에 알람코드 주소 근거 없음(0162=STOCKER CST_X_SERVO_MAX), WCS CvAlarmCheck는 비트(M787)만 관측", null));
            s7.Steps.Add(ScStep.W(1540,        "M096.4 %MX1540 ON", "Alarm Reset Report Ack ON (WCS)", CV_ALARM));
            s7.Steps.Add(ScStep.E(787, false, "M049.3 %MX787 OFF", "Alarm Reset Report OFF (설비 리셋)"));
            s7.Steps.Add(ScStep.E(1540, false, "M096.4 %MX1540 OFF","Alarm Reset Report Ack OFF (설비/PLC 리셋)"));
            list.Add(s7);

            return list;
        }
    }

    // ------------------------------------------------------------------
    // 개별 시나리오 창 : 스텝(EQP=버튼 / WCS ACK=램프 / D·R=값 관측) + [자동 진행] + [초기화]
    // ------------------------------------------------------------------
    internal class ScenarioStepForm : Form
    {
        private readonly PlcMemory mem;
        private Scenario sc;
        private readonly List<Control> stepCtrls = new List<Control>();
        private Timer lampTimer;
        private Timer autoTimer;
        private int autoIdx = -1;

        // [LGLS 2026-08-19] 호기 선택(1~5). 선택이 바뀌면 XML 을 다시 해석해 전 스텝 주소를 재생성한다.
        private readonly RadioButton[] rdoCrane = new RadioButton[5];
        private Label lblCraneInfo;
        private FlowLayoutPanel flowSteps;

        public ScenarioStepForm(PlcMemory mem, Scenario sc)
        {
            this.mem = mem;
            this.sc = sc;

            Text = sc.Title;
            Width = 700; Height = 110 + sc.Steps.Count * 42 + 40;
            if (Height > 980) Height = 980;
            StartPosition = FormStartPosition.CenterParent;

            var head = new Label
            {
                Text = "위→아래 순서(PPT 도형순). [EQP]=버튼(누르면 세팅) · [WCS]=ACK 관측 램프 · [D/R]=워드 값 관측",
                Dock = DockStyle.Top, Height = 30, TextAlign = ContentAlignment.MiddleLeft,
                Padding = new Padding(10, 0, 0, 0), ForeColor = Color.DarkBlue
            };

            // ── 호기 선택 줄 (초기화 버튼 줄 위) ─────────────────────────────
            var craneBar = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 32, Padding = new Padding(10, 4, 0, 0), WrapContents = false };
            craneBar.Controls.Add(new Label { Text = "호기 선택 :", Width = 70, TextAlign = ContentAlignment.MiddleLeft, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), Margin = new Padding(0, 3, 0, 0) });
            for (int c = 0; c < 5; c++)
            {
                rdoCrane[c] = new RadioButton
                {
                    Text = "S/C #" + (c + 1), Width = 76, AutoSize = false, Height = 22,
                    Checked = (c + 1 == sc.CraneNo), Enabled = sc.CraneSelectable,
                    Margin = new Padding(2, 3, 2, 0)
                };
                rdoCrane[c].CheckedChanged += OnCraneChanged;
                craneBar.Controls.Add(rdoCrane[c]);
            }
            lblCraneInfo = new Label { Text = "", AutoSize = false, Width = 340, Height = 22, TextAlign = ContentAlignment.MiddleLeft, ForeColor = Color.DarkGreen, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), Margin = new Padding(8, 3, 0, 0) };
            craneBar.Controls.Add(lblCraneInfo);

            var bar = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 40, Padding = new Padding(8, 4, 0, 0) };
            var btnAuto = new Button { Text = "▶ 자동 진행", Width = 120, Height = 30, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), BackColor = Color.FromArgb(210, 245, 210) };
            btnAuto.Click += (s, e) => StartAuto();
            var btnReset = new Button { Text = "■ 초기화 (전체 OFF)", Width = 160, Height = 30, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), BackColor = Color.FromArgb(255, 235, 235) };
            btnReset.Click += (s, e) => ResetAll();
            bar.Controls.Add(btnAuto); bar.Controls.Add(btnReset);

            flowSteps = new FlowLayoutPanel { Dock = DockStyle.Fill, FlowDirection = FlowDirection.TopDown, Padding = new Padding(10), WrapContents = false, AutoScroll = true };
            BuildStepRows(flowSteps);

            Controls.Add(flowSteps);
            Controls.Add(bar);
            Controls.Add(craneBar);
            Controls.Add(head);
            UpdateCraneInfo();

            lampTimer = new Timer { Interval = 250 };
            lampTimer.Tick += (s, e) => RefreshLamps();
            lampTimer.Start();
            FormClosed += (s, e) => { lampTimer.Stop(); if (autoTimer != null) autoTimer.Stop(); };
        }

        private int SelectedCrane()
        {
            for (int i = 0; i < 5; i++) if (rdoCrane[i] != null && rdoCrane[i].Checked) return i + 1;
            return 1;
        }

        private void UpdateCraneInfo()
        {
            if (lblCraneInfo == null) return;
            if (!sc.CraneSelectable) { lblCraneInfo.Text = "(호기 고정 시나리오)"; return; }
            var c = WCS_TASK_CV.cPlcAddrMap.Crane(SelectedCrane());
            lblCraneInfo.Text = (c == null) ? ""
                : string.Format("입고통로 C/V #{0} (#{1}→#{2})   출고통로 C/V #{3} (#{4}→#{5})",
                                c.InCv, c.InRgvPort, c.InScPort, c.OutCv, c.OutScPort, c.OutRgvPort);
        }

        private bool rebuilding;
        private void OnCraneChanged(object sender, EventArgs e)
        {
            if (rebuilding) return;
            var rb = sender as RadioButton;
            if (rb == null || !rb.Checked) return;
            if (!sc.CraneSelectable) return;

            rebuilding = true;
            try
            {
                if (autoTimer != null) autoTimer.Stop();
                int crane = SelectedCrane();
                var list = ScenarioTestForm.BuildScenariosFromXml(crane);
                if (list != null)
                {
                    foreach (var s2 in list)
                        if (s2.Id == sc.Id) { sc = s2; break; }
                }
                sc.CraneNo = crane;
                flowSteps.SuspendLayout();
                flowSteps.Controls.Clear();
                stepCtrls.Clear();
                BuildStepRows(flowSteps);
                flowSteps.ResumeLayout();
                UpdateCraneInfo();
            }
            finally { rebuilding = false; }
        }

        // [LGLS 2026-09-01] 이 R 값을 쓰는 주체가 설비/PLC 인가 (시나리오 모드에선 정지 → 재현 필요).
        //   src 의 함수명으로 가른다 : CvTrackingWrite/ConsumeCommands = WCS 가 쓰는 값(관측 대기),
        //   PollObservations/CvStatusScenario 또는 src 없음 = WCS 는 읽기만(설비가 씀 → 재현 대상).
        //   ★스텝 22(RGV 탑재번호, src=PollObservations)에서 무한 대기하던 원인★ - src 유무로만 갈랐었다.
        private static bool IsPlcWritten(ScStep st)
        {
            if (st.Kind != 2 || st.Dev != 'R') return false;
            if (string.IsNullOrEmpty(st.Src)) return true;
            return st.Src.Contains("PollObservations") || st.Src.Contains("CvStatusScenario");
        }

        // 이 스텝보다 앞에 있는, 주소가 다른 R 트래킹 스텝의 워드주소 (없으면 -1)
        private int PrevRAddr(ScStep st)
        {
            int idx = sc.Steps.IndexOf(st);
            for (int k = idx - 1; k >= 0; k--)
                if (sc.Steps[k].Kind == 2 && sc.Steps[k].Dev == 'R' && sc.Steps[k].WordAddr != st.WordAddr)
                    return sc.Steps[k].WordAddr;
            return -1;
        }

        private void BuildStepRows(FlowLayoutPanel flow)
        {
            int n = 1;
            foreach (var st in sc.Steps)
            {
                Control ctrl;
                if (st.Kind == 0)
                {
                    var b = new Button
                    {
                        Text = n + ") [EQP 송신]  " + st.Addr + "  →  " + st.Desc,
                        Width = 620, Height = 32, Margin = new Padding(2),
                        TextAlign = ContentAlignment.MiddleLeft,
                        BackColor = st.Val ? Color.FromArgb(220, 240, 255) : Color.FromArgb(255, 235, 235)
                    };
                    var stLocal = st;
                    b.Click += (s, e) => { mem.SetBit(stLocal.Dev, stLocal.Bit, stLocal.Val); };
                    ctrl = b;
                }
                else if (st.Kind == 1)
                {
                    ctrl = new Label
                    {
                        Text = n + ") [WCS 반응]  " + st.Addr + "  :  " + st.Desc
                               + (string.IsNullOrEmpty(st.Src) ? "" : ("\n        ⇒ " + st.Src)),
                        Width = 620, Height = 40, Margin = new Padding(2),
                        TextAlign = ContentAlignment.MiddleLeft, BorderStyle = BorderStyle.FixedSingle,
                        BackColor = Color.WhiteSmoke, Padding = new Padding(6, 0, 0, 0)
                    };
                }
                else if (st.Kind == 2 && IsPlcWritten(st) && PrevRAddr(st) >= 0)
                {
                    // [LGLS 2026-09-01] ★ID Shift(트래킹 이동)는 PLC 래더의 일★ (사용자 실측 : 스텝 10 정지)
                    //   시나리오 모드는 자동 운전을 멈추므로 이 이동을 재현할 주체가 없다 -
                    //   1~9번은 진행되는데 10번(ID Shift 결과 관측)에서 영영 멈췄다.
                    //   src 없는 R 트래킹 스텝 = WCS 가 쓰는 값이 아니라 PLC 가 옮기는 값이므로,
                    //   버튼으로 만들어 사람이 PLC 역할을 대신한다 : 직전 R 트래킹 스텝의 값을
                    //   이 주소로 옮기고 원위치를 '0000' 으로 클리어(실 PLC ID Shift 와 동일).
                    var stL = st; int srcL = PrevRAddr(st);
                    var b2 = new Button
                    {
                        Text = n + ") [PLC 재현]  " + st.Addr + "  →  " + st.Desc,
                        Width = 620, Height = 32, Margin = new Padding(2),
                        TextAlign = ContentAlignment.MiddleLeft,
                        BackColor = Color.FromArgb(235, 255, 235)
                    };
                    b2.Click += (s2, e2) =>
                    {
                        string v = mem.GetString('R', srcL, 2);
                        if (string.IsNullOrEmpty(v) || v == "0" || v == "0000") return;   // 옮길 JOB 없음
                        mem.SetString('R', stL.WordAddr, 2, v);
                        mem.SetString('R', srcL, 2, "0000");
                    };
                    ctrl = b2;
                }
                else // Kind 2 : D/R 워드 관측
                {
                    ctrl = new Label
                    {
                        Text = n + ") [" + st.Dev + " 관측]  " + st.Addr + "  :  " + st.Desc + "   = ?"
                               + (string.IsNullOrEmpty(st.Src) ? "" : ("\n        ⇒ " + st.Src)),
                        Width = 620, Height = 40, Margin = new Padding(2),
                        TextAlign = ContentAlignment.MiddleLeft, BorderStyle = BorderStyle.FixedSingle,
                        BackColor = Color.FromArgb(255, 252, 230), Padding = new Padding(6, 0, 0, 0)
                    };
                }
                flow.Controls.Add(ctrl);
                stepCtrls.Add(ctrl);
                n++;
            }
        }

        private void RefreshLamps()
        {
            for (int i = 0; i < sc.Steps.Count; i++)
            {
                var st = sc.Steps[i];
                if (st.Kind == 1)
                {
                    bool on = mem.GetBit('M', st.Bit);
                    stepCtrls[i].BackColor = on ? Color.SeaGreen : Color.WhiteSmoke;
                    stepCtrls[i].ForeColor = on ? Color.White : Color.Black;
                }
                else if (st.Kind == 2)
                {
                    string v = st.IsString ? mem.GetString(st.Dev, st.WordAddr, 2) : mem.GetWord(st.Dev, st.WordAddr).ToString("X4");
                    if (string.IsNullOrEmpty(v)) v = "-";
                    bool hot = v != "-" && v != "0" && v != "0000";
                    if (stepCtrls[i] is Button)   // [LGLS 2026-09-01] PLC 재현 버튼은 값에 따라 배경만
                    {
                        stepCtrls[i].BackColor = hot ? Color.FromArgb(120, 170, 90) : Color.FromArgb(235, 255, 235);
                        stepCtrls[i].ForeColor = hot ? Color.White : Color.Black;
                        continue;
                    }
                    string baseText = (i + 1) + ") [" + st.Dev + " 관측]  " + st.Addr + "  :  " + st.Desc + "   = " + v
                                      + (string.IsNullOrEmpty(st.Src) ? "" : ("\n        ⇒ " + st.Src));
                    if (stepCtrls[i].Text != baseText) stepCtrls[i].Text = baseText;
                    stepCtrls[i].BackColor = hot ? Color.FromArgb(120, 170, 90) : Color.FromArgb(255, 252, 230);
                    stepCtrls[i].ForeColor = hot ? Color.White : Color.Black;
                }
            }
        }

        // [자동 진행] EQP 비트는 자동 세팅. WCS ACK 비트는 ON 될 때까지 대기(최대 8초). D/R 관측은 1.2초 표시 후 다음.
        private DateTime waitStart;
        private void StartAuto()
        {
            if (autoTimer != null) { autoTimer.Stop(); autoTimer.Dispose(); }
            autoIdx = 0;
            waitStart = DateTime.Now;
            autoTimer = new Timer { Interval = 500 };
            autoTimer.Tick += AutoTick;
            autoTimer.Start();
        }

        // [LGLS 2026-09-01] ★자동 진행을 시간이 아니라 상태로★ (사용자 지적 : 설비가 못 따라감)
        //   종전에는 ACK 8초 타임아웃/관측 1.2초 후 무조건 통과라, WCS 폴링(수 초)이
        //   따라오기 전에 다음 비트를 눌러 시나리오가 실제 교신과 어긋났다.
        //   이제 각 스텝의 완료 조건이 실제로 관측될 때까지 기다린다(타임아웃 없음 -
        //   어디서 기다리는지는 금색 하이라이트로 보인다. 정지하려면 다시 [자동 진행]).
        private void AutoTick(object sender, EventArgs e)
        {
            if (autoIdx >= sc.Steps.Count) { autoTimer.Stop(); HighlightNone(); return; }
            var st = sc.Steps[autoIdx];
            HighlightCurrent(autoIdx);
            if (st.Kind == 0)
            {
                // EQP 비트 : 직전 스텝 처리 후 1.5초 여유(WCS 폴링이 앞 상태를 볼 시간)를 두고 누른다
                if ((DateTime.Now - waitStart).TotalSeconds < 1.5) return;
                mem.SetBit(st.Dev, st.Bit, st.Val);
                autoIdx++; waitStart = DateTime.Now;
            }
            else if (st.Kind == 1)
            {
                // WCS ACK : 실제로 ON 이 관측될 때까지 기다린다
                if (mem.GetBit('M', st.Bit))
                { autoIdx++; waitStart = DateTime.Now; }
            }
            else // D/R 관측
            {
                if (st.Dev == 'R' && st.IsString)
                {
                    // R 트래킹 : JOB 값이 실제로 실릴 때까지 기다린다.
                    //   PLC 재현 스텝(src 없음)은 사람이 하던 이동을 자동이 대신 수행한다.
                    string v = mem.GetString('R', st.WordAddr, 2);
                    bool hot = !string.IsNullOrEmpty(v) && v != "0" && v != "0000";
                    if (!hot && IsPlcWritten(st))
                    {
                        int src = PrevRAddr(st);
                        if (src >= 0)
                        {
                            string pv = mem.GetString('R', src, 2);
                            if (!string.IsNullOrEmpty(pv) && pv != "0" && pv != "0000")
                            {
                                mem.SetString('R', st.WordAddr, 2, pv);
                                mem.SetString('R', src, 2, "0000");
                                hot = true;
                            }
                        }
                    }
                    if (hot) { autoIdx++; waitStart = DateTime.Now; }
                }
                else if ((DateTime.Now - waitStart).TotalSeconds > 1.2)
                {
                    // D 워드(방향/지시값 등)는 표시 확인용 - 잠깐 보여주고 진행
                    autoIdx++; waitStart = DateTime.Now;
                }
            }
        }

        private void HighlightCurrent(int idx)
        {
            for (int i = 0; i < stepCtrls.Count; i++)
                if (sc.Steps[i].Kind == 0 && i != idx)
                    stepCtrls[i].BackColor = sc.Steps[i].Val ? Color.FromArgb(220, 240, 255) : Color.FromArgb(255, 235, 235);
            if (idx < stepCtrls.Count && sc.Steps[idx].Kind == 0)
                stepCtrls[idx].BackColor = Color.Gold;
            // [LGLS 2026-09-01] 어디서 기다리는지 모든 스텝 종류에 표시(▶) - 종전에는 EQP 버튼만
            //   금색이라, ACK/관측 대기 중이면 멈춘 위치가 보이지 않았다.
            for (int i = 0; i < stepCtrls.Count; i++)
            {
                string t = stepCtrls[i].Text;
                bool cur = (i == idx);
                bool marked = t.StartsWith("▶ ");
                if (cur && !marked) stepCtrls[i].Text = "▶ " + t;
                else if (!cur && marked) stepCtrls[i].Text = t.Substring(2);
            }
        }
        private void HighlightNone()
        {
            for (int i = 0; i < stepCtrls.Count; i++)
                if (sc.Steps[i].Kind == 0)
                    stepCtrls[i].BackColor = sc.Steps[i].Val ? Color.FromArgb(220, 240, 255) : Color.FromArgb(255, 235, 235);
        }

        private void ResetAll()
        {
            if (autoTimer != null) autoTimer.Stop();
            foreach (var st in sc.Steps)
            {
                if (st.Kind != 2) { mem.SetBit(st.Dev, st.Bit, false); continue; }
                // [LGLS 2026-09-01] R 트래킹도 초기화 (사용자 지적 - 비트만 지워 JOB 잔재가 남았다).
                //   D 워드(방향/지시값)는 WCS 가 관리하는 값이라 건드리지 않는다.
                if (st.Dev == 'R')
                    mem.SetString('R', st.WordAddr, 2, "0000");
            }
            HighlightNone();
        }
    }

    // ------------------------------------------------------------------
    // M/D/R 영역 직접 읽기/쓰기 패널 (자동 리프레시)
    //   M: 비트(%MX, 값 0/1) / D,R: 워드(%DW,%RW, 16진 4자리) 또는 바이트(%DB,%RB, 16진 2자리)
    // ------------------------------------------------------------------
    internal class MemAreaPanel : Panel
    {
        private readonly PlcMemory mem;
        private readonly char dev;
        private readonly bool bitMode;
        private TextBox txtStart, txtCount, txtVal;
        private DataGridView grid;
        private ComboBox cboUnit;
        private Timer autoTimer;

        private bool wordUnit = true;
        private string Prefix
        {
            get
            {
                if (bitMode) return "MX";
                return (dev == 'D') ? (wordUnit ? "DW" : "DB") : (wordUnit ? "RW" : "RB");
            }
        }

        public MemAreaPanel(PlcMemory mem, char dev, string prefix, bool bitMode)
        {
            this.mem = mem; this.dev = dev; this.bitMode = bitMode;
            Dock = DockStyle.Fill; BorderStyle = BorderStyle.FixedSingle; Padding = new Padding(4);

            var title = new Label { Text = dev + " 영역 (자동 리프레시)", Dock = DockStyle.Top, Height = 22, Font = new Font("맑은 고딕", 9F, FontStyle.Bold), ForeColor = Color.DarkSlateBlue };

            var row1 = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 30, WrapContents = false };
            if (!bitMode)
            {
                cboUnit = new ComboBox { Width = 96, DropDownStyle = ComboBoxStyle.DropDownList, Margin = new Padding(0, 3, 4, 0) };
                cboUnit.Items.Add("%" + dev + "W 워드");
                cboUnit.Items.Add("%" + dev + "B 바이트");
                cboUnit.SelectedIndex = 0;
                cboUnit.SelectedIndexChanged += (s, e) => { wordUnit = (cboUnit.SelectedIndex == 0); Rebuild(); };
                row1.Controls.Add(cboUnit);
            }
            row1.Controls.Add(new Label { Text = "시작", Width = 30, TextAlign = ContentAlignment.MiddleLeft, Margin = new Padding(2, 5, 0, 0) });
            txtStart = new TextBox { Width = 56, Text = bitMode ? "576" : "0" };
            txtStart.TextChanged += (s, e) => Rebuild();
            row1.Controls.Add(txtStart);
            row1.Controls.Add(new Label { Text = "Count", Width = 42, TextAlign = ContentAlignment.MiddleLeft, Margin = new Padding(4, 5, 0, 0) });
            txtCount = new TextBox { Width = 40, Text = "16" };
            txtCount.TextChanged += (s, e) => Rebuild();
            row1.Controls.Add(txtCount);
            var btnRead = new Button { Text = "읽기", Width = 52, Height = 24 };
            btnRead.Click += (s, e) => Rebuild();
            row1.Controls.Add(btnRead);

            var row2 = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 30, WrapContents = false };
            row2.Controls.Add(new Label { Text = "값", Width = 22, TextAlign = ContentAlignment.MiddleLeft, Margin = new Padding(2, 5, 0, 0) });
            txtVal = new TextBox { Width = 100, Text = bitMode ? "1" : "0000" };
            row2.Controls.Add(txtVal);
            var btnWrite = new Button { Text = "쓰기(→시작주소)", Width = 130, Height = 24 };
            btnWrite.Click += (s, e) => DoWrite();
            row2.Controls.Add(btnWrite);

            grid = new DataGridView
            {
                Dock = DockStyle.Fill, ReadOnly = true, AllowUserToAddRows = false,
                RowHeadersVisible = false, AllowUserToResizeRows = false,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect, ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize
            };
            grid.Columns.Add("addr", "주소");
            grid.Columns.Add("val", "값");
            grid.Columns[0].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            grid.Columns[1].Width = 80;

            Controls.Add(grid);
            Controls.Add(row2);
            Controls.Add(row1);
            Controls.Add(title);

            Rebuild();

            // 자동 리프레시: 값만 갱신(구조가 같으면). 400ms.
            autoTimer = new Timer { Interval = 400 };
            autoTimer.Tick += (s, e) => RefreshValues();
            autoTimer.Start();
            HandleDestroyed += (s, e) => { if (autoTimer != null) autoTimer.Stop(); };
        }

        private int ParseInt(string s, int def)
        {
            s = (s ?? "").Trim();
            int v;
            if (s.StartsWith("0x") || s.StartsWith("0X"))
                return int.TryParse(s.Substring(2), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out v) ? v : def;
            return int.TryParse(s, out v) ? v : def;
        }

        private string CellVal(int addr, out bool hot)
        {
            if (bitMode) { bool b = mem.GetBit(dev, addr); hot = b; return b ? "1" : "0"; }
            if (wordUnit) { ushort w = mem.GetWord(dev, addr); hot = w != 0; return w.ToString("X4"); }
            byte by = mem.ReadBytes(dev, addr, 1)[0]; hot = by != 0; return by.ToString("X2");
        }

        // 구조(행 수/주소) 재생성
        private void Rebuild()
        {
            int start = ParseInt(txtStart.Text, 0);
            int count = Math.Max(1, Math.Min(256, ParseInt(txtCount.Text, 16)));
            grid.SuspendLayout();
            grid.Rows.Clear();
            for (int i = 0; i < count; i++)
            {
                int addr = start + i;
                bool hot; string val = CellVal(addr, out hot);
                grid.Rows.Add("%" + Prefix + addr, val);
                if (hot) grid.Rows[i].Cells[1].Style.BackColor = Color.SeaGreen;
            }
            grid.ResumeLayout();
        }

        // 값만 갱신(깜빡임 없음). 시작/Count 가 바뀌어 행 수가 달라지면 재생성.
        private void RefreshValues()
        {
            int start = ParseInt(txtStart.Text, 0);
            int count = Math.Max(1, Math.Min(256, ParseInt(txtCount.Text, 16)));
            if (grid.Rows.Count != count) { Rebuild(); return; }
            for (int i = 0; i < count; i++)
            {
                int addr = start + i;
                bool hot; string val = CellVal(addr, out hot);
                var cell = grid.Rows[i].Cells[1];
                if ((cell.Value as string) != val) cell.Value = val;
                var want = hot ? Color.SeaGreen : Color.White;
                if (cell.Style.BackColor != want) cell.Style.BackColor = want;
            }
        }

        private void DoWrite()
        {
            int start = ParseInt(txtStart.Text, 0);
            string raw = (txtVal.Text ?? "").Trim();
            try
            {
                if (bitMode)
                    mem.SetBit(dev, start, raw == "1" || raw.ToLower() == "on" || raw.ToLower() == "true");
                else if (wordUnit)
                    mem.SetWord(dev, start, (ushort)(int.Parse(raw, NumberStyles.HexNumber, CultureInfo.InvariantCulture) & 0xFFFF));
                else
                    mem.WriteBytes(dev, start, new byte[] { (byte)(int.Parse(raw, NumberStyles.HexNumber, CultureInfo.InvariantCulture) & 0xFF) }, 1, false);
                Rebuild();
            }
            catch (Exception ex)
            {
                MessageBox.Show("쓰기 실패: " + ex.Message + "\n(값: " + (bitMode ? "0/1" : "16진") + ")",
                    "MemAreaPanel", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }
    }
}
