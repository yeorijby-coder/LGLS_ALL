using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EQP_SIM.Core;
using EQP_SIM.Sim;
using EQP_SIM.Xgt;

namespace EQP_SIM
{
    public partial class MainForm : Form
    {
        private SimConfig config;
        private PlcMemory memory;
        private ObservableMap obsMap;
        private PlcIo io;
        private XgtServer server;
        private ScenarioEngine engine;
        private ScenarioAutoRunner autoRunner;   // [LGLS 2026-09-01] 시나리오 헤드리스 검증
        private readonly Queue<string> pendingLogs = new Queue<string>();
        private readonly object logSync = new object();

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            try
            {
                string baseDir = AppDomain.CurrentDomain.BaseDirectory;
                config = new SimConfig(Path.Combine(baseDir, "EQP_SIM.ini"));

                // 관측값 주소 맵
                // [LGLS 2026-08-21] 1순위 = 주소맵 XML(PlcAddressMap.xml, EQP_TASK 와 공유) - XML 하나로 양쪽이 함께 바뀐다
                //   2순위(폴백) = observables.tsv (구 TB_OBSERVABLE 덤프)
                obsMap = new ObservableMap();
                if (!obsMap.LoadFromAddrMap())
                {
                    string tsv = Path.Combine(baseDir, "observables.tsv");
                    if (!File.Exists(tsv))
                    {
                        MessageBox.Show(this, "주소맵 XML 도 observables.tsv 도 없습니다.\n(PlcAddressMap.xml 또는 tsv 가 필요)",
                            "EQP_SIM", MessageBoxButtons.OK, MessageBoxIcon.Error);
                        Close();
                        return;
                    }
                    obsMap.Load(tsv);
                    obsMap.Source = "observables.tsv (폴백)";
                }
                Text += "   [주소: " + obsMap.Source + "]";

                memory = new PlcMemory();
                io = new PlcIo(memory, obsMap);
                engine = new ScenarioEngine(io, config);
                autoRunner = new ScenarioAutoRunner(engine);
                engine.LogAdded += OnEngineLog;

                string dataDir = Path.Combine(baseDir, "Data");
                memory.Load(Path.Combine(dataDir, "plc_memory.bin"));
                engine.Start(dataDir, config);

                chkAutoFeed.Checked = config.GetBool("FEED", "AUTO", true);

                // XGT 서버
                server = new XgtServer(memory);
                server.Log += OnEngineLog;
                server.ClientCountChanged += n => SafeUI(() =>
                {
                    lblServer.ForeColor = n > 0 ? System.Drawing.Color.Green : System.Drawing.Color.Black;
                });
                int[] ports = config.Get("XGT", "PORTS", "2004,3001")
                    .Split(',').Select(s => int.Parse(s.Trim())).ToArray();
                server.Start(ports);
                lblServer.Text = "XGT 서버: 포트 " + string.Join(", ", ports.Select(p => p.ToString()).ToArray()) +
                                 " (관측값 " + obsMap.Count + "점)";
                lblStatus.Text = "가동 중 — ECS 접속 대기";

                BuildMemToolPanel();   // [LGLS 2026-09-02] 수동 메모리 쓰기/읽기 + 구ECS 주소 변환
                timerUi.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "초기화 실패: " + ex.Message, "EQP_SIM",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // ── [LGLS 2026-09-02] 수동 메모리 도구 (사용자 요청) ──────────────────────
        //   ① 특정 영역 강제 쓰기/읽기 : D(워드,10진) / M(비트) / R(워드,10진)
        //   ② 구ECS 표기(B/W/R, 16진) → 실주소 변환 (CV_TASK 메모리맵 계산기와 동일 공식)
        private ComboBox mtDev; private TextBox mtAddr; private TextBox mtVal; private Label mtOut;
        private ComboBox cvDev; private TextBox cvAddr; private Label cvOut;
        private void BuildMemToolPanel()
        {
            var pnl = new Panel { Dock = DockStyle.Bottom, Height = 58, BackColor = System.Drawing.Color.WhiteSmoke };
            Controls.Add(pnl);
            pnl.BringToFront();

            Func<string,int,int,Label> L = delegate(string t, int x, int y)
            { var l = new Label { Text = t, Location = new System.Drawing.Point(x, y + 4), AutoSize = true }; pnl.Controls.Add(l); return l; };

            // ① 강제 쓰기/읽기
            L("메모리", 6, 4);
            mtDev = new ComboBox { Location = new System.Drawing.Point(52, 4), Width = 46, DropDownStyle = ComboBoxStyle.DropDownList };
            mtDev.Items.AddRange(new object[] { "D", "M", "R" }); mtDev.SelectedIndex = 0;
            mtAddr = new TextBox { Location = new System.Drawing.Point(102, 4), Width = 60, Text = "210" };
            L("값", 166, 4);
            mtVal = new TextBox { Location = new System.Drawing.Point(186, 4), Width = 60, Text = "1" };
            var btnW = new Button { Text = "쓰기", Location = new System.Drawing.Point(252, 3), Width = 48 };
            var btnR = new Button { Text = "읽기", Location = new System.Drawing.Point(304, 3), Width = 48 };
            mtOut = L("(D=워드10진, M=비트, R=워드10진)", 358, 4);
            btnW.Click += delegate { MemToolRun(true); };
            btnR.Click += delegate { MemToolRun(false); };
            pnl.Controls.Add(mtDev); pnl.Controls.Add(mtAddr); pnl.Controls.Add(mtVal);
            pnl.Controls.Add(btnW); pnl.Controls.Add(btnR);

            // ② 구ECS 주소 변환
            L("구ECS", 6, 30);
            cvDev = new ComboBox { Location = new System.Drawing.Point(52, 30), Width = 46, DropDownStyle = ComboBoxStyle.DropDownList };
            cvDev.Items.AddRange(new object[] { "W", "B", "R" }); cvDev.SelectedIndex = 0;
            cvAddr = new TextBox { Location = new System.Drawing.Point(102, 30), Width = 60, Text = "0150" };
            var btnC = new Button { Text = "변환", Location = new System.Drawing.Point(166, 29), Width = 48 };
            cvOut = L("(16진 입력 → 실주소)", 220, 30);
            btnC.Click += delegate { ConvertOldAddr(); };
            pnl.Controls.Add(cvDev); pnl.Controls.Add(cvAddr); pnl.Controls.Add(btnC);
        }

        private void MemToolRun(bool bWrite)
        {
            try
            {
                char dev = mtDev.Text[0];
                int addr = Convert.ToInt32(mtAddr.Text.Trim(), 10);
                if (bWrite)
                {
                    int val = Convert.ToInt32(mtVal.Text.Trim(), 10);
                    if (dev == 'M') memory.SetBit('M', addr, val != 0);
                    else memory.SetWord(dev, addr, (ushort)val);
                    mtOut.Text = string.Format("{0}{1} <- {2} 기록됨", dev, addr, val);
                }
                else
                {
                    int val = (dev == 'M') ? (memory.GetBit('M', addr) ? 1 : 0) : memory.GetWord(dev, addr);
                    mtOut.Text = string.Format("{0}{1} = {2}", dev, addr, val);
                }
            }
            catch (Exception ex) { mtOut.Text = "오류: " + ex.Message; }
        }

        private void ConvertOldAddr()
        {
            try
            {
                int n = Convert.ToInt32(cvAddr.Text.Trim(), 16);
                char dev = cvDev.Text[0];
                if (dev == 'B')       // 비트 16진 → %MX / M워드.비트
                    cvOut.Text = string.Format("B{0} -> %MX{1} = M{2:000}.{3:X} (비트 {1})",
                        cvAddr.Text.Trim().ToUpper(), n, n / 16, n % 16);
                else if (dev == 'W')  // 워드 16진 → 구환산 %DW = 같은 수의 10진
                    cvOut.Text = string.Format("W{0} -> D{1} (구환산 %DW{1} / %DB{2})",
                        cvAddr.Text.Trim().ToUpper(), n, n * 2);
                else                  // R : 10진 확정(구 문서도 10진) - 참고로 16진 해석값 병기
                {
                    int nDec = Convert.ToInt32(cvAddr.Text.Trim(), 10);
                    cvOut.Text = string.Format("R{0} -> R{1} (10진 확정) / 16진 해석 시 R{2}",
                        cvAddr.Text.Trim(), nDec, n);
                }
            }
            catch (Exception ex) { cvOut.Text = "오류: " + ex.Message; }
        }

        private void OnEngineLog(string msg)
        {
            lock (logSync)
            {
                pendingLogs.Enqueue(msg);
                while (pendingLogs.Count > 500) pendingLogs.Dequeue();
            }
        }

        private void timerUi_Tick(object sender, EventArgs e)
        {
            // 로그 반영
            List<string> logs = null;
            lock (logSync)
            {
                if (pendingLogs.Count > 0)
                {
                    logs = new List<string>(pendingLogs);
                    pendingLogs.Clear();
                }
            }
            if (logs != null)
            {
                lvLog.BeginUpdate();
                foreach (string msg in logs)
                {
                    var item = new ListViewItem(DateTime.Now.ToString("HH:mm:ss.fff"));
                    item.SubItems.Add(msg);
                    lvLog.Items.Add(item);
                }
                while (lvLog.Items.Count > 2000) lvLog.Items.RemoveAt(0);
                if (lvLog.Items.Count > 0) lvLog.Items[lvLog.Items.Count - 1].EnsureVisible();
                lvLog.EndUpdate();
            }

            // 설비 상태 표
            if (engine == null) return;
            var rows = new List<string[]>();
            foreach (var cv in engine.AllConveyors.OrderBy(c => c.Def.No))
            {
                var sb = new StringBuilder();
                for (int i = 1; i <= cv.Def.Ports.Length; i++)
                {
                    SimPallet p;
                    string cell = cv.Pallets.TryGetValue(i, out p)
                        ? (p.Id.Length > 0 ? p.Id : "(대기)")
                        : "-";
                    sb.Append("P").Append(cv.Def.PortOfOrder(i)).Append(":").Append(cell).Append("  ");
                }
                if (cv.Def.HasDirection)
                {
                    sb.Append(" [방향 ").Append(cv.Direction == "0" ? "입고" : "출고").Append("]");
                    // [LGLS 2026-08-22] 방향 파생 신호 — 출고 모드에서만 ON
                    if (cv.Def.RtvArrivePort > 0)
                        sb.Append(" P").Append(cv.Def.RtvArrivePort).Append(" RTV도착지:").Append(cv.RtvArriveHs ? "ON" : "OFF");
                    if (cv.Def.RetHsPort > 0)
                        sb.Append(" P").Append(cv.Def.RetHsPort).Append(" 출고HS:").Append(cv.RetHs ? "ON" : "OFF");
                }
                rows.Add(new[] { "C/V #" + cv.Def.No, sb.ToString() });
            }
            foreach (var v in engine.AllVehicles.OrderBy(x => x.Def.Id))
            {
                string name = v.Def.IsRgv ? "RGV #1" : "S/C #" + v.Def.ScNo;
                rows.Add(new[] { name, v.StatusText });
            }
            rows.Add(new[] { "랙 재고", engine.Rack.Count + "셀 (출고완료 " + engine.CompletedOutgoCount + "건)" });

            lvDevices.BeginUpdate();
            while (lvDevices.Items.Count < rows.Count)
                lvDevices.Items.Add(new ListViewItem(new[] { "", "" }));
            while (lvDevices.Items.Count > rows.Count)
                lvDevices.Items.RemoveAt(lvDevices.Items.Count - 1);
            for (int i = 0; i < rows.Count; i++)
            {
                if (lvDevices.Items[i].Text != rows[i][0]) lvDevices.Items[i].Text = rows[i][0];
                if (lvDevices.Items[i].SubItems[1].Text != rows[i][1]) lvDevices.Items[i].SubItems[1].Text = rows[i][1];
            }
            lvDevices.EndUpdate();
        }

        private void chkAutoFeed_CheckedChanged(object sender, EventArgs e)
        {
            if (engine == null) return;
            foreach (var cv in engine.AllConveyors)
                if (cv.Def.IngoPath != null && cv.Def.No >= (engine.WcsSupport ? 12 : 11))   // [LGLS] WCS 모드에서 C/V#11 입고대는 IO_TASK(FeedInGate) 소유 → AutoFeed 체크박스 제어 제외(항상 off)
                    cv.AutoFeed = chkAutoFeed.Checked && cv.AutoFeedConfigured;               // INI [FEED] CVnn=0 은 체크박스로도 켜지지 않음
        }

        // [LGLS] 이중입고/공출고 에러 주입: 체크 시 다음 최초 입고/출고 S/C 작업에서 1회 발생 (VehicleSim 소비)
        private void chkInjectDouble_CheckedChanged(object sender, EventArgs e)
        {
            if (engine == null) return;
            engine.InjectDoubleStorage = chkInjectDouble.Checked;
            if (chkInjectDouble.Checked) lblStatus.Text = "이중입고 에러 예약됨 — 다음 입고 크레인에서 발생(ERR 54)";
        }

        private void chkInjectEmpty_CheckedChanged(object sender, EventArgs e)
        {
            if (engine == null) return;
            engine.InjectEmptyRetrieval = chkInjectEmpty.Checked;
            if (chkInjectEmpty.Checked) lblStatus.Text = "공출고 에러 예약됨 — 다음 출고 크레인에서 발생(ERR 58)";
        }

        private void btnFeed11_Click(object sender, EventArgs e) { ManualFeed("CONVEYOR:11"); }

        // [LGLS 2026-08-24] 가운데 투입 버튼 = "24 입고대"(C/V#12).
        //   현장 확인 기준 : C/V#12 = 입고대(TR#24) / C/V#13 = 출고대(TR#26).
        //   두 모드 모두 C/V#12 가 입고대이므로 분기 없이 12 로 보낸다.
        private void btnFeed12_Click(object sender, EventArgs e) { ManualFeed("CONVEYOR:12"); }

        private void btnFeed15_Click(object sender, EventArgs e) { ManualFeed("CONVEYOR:15"); }

        private void ManualFeed(string id)
        {
            if (engine != null && !engine.ManualInject(id))
                lblStatus.Text = id + " 투입 불가 (포트 점유/방향)";
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            if (engine == null) return;
            if (MessageBox.Show(this, "모든 파렛트/랙/PLC 메모리를 초기화합니다.", "EQP_SIM",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == DialogResult.OK)
            {
                engine.ResetAll();
            }
        }

        // [LGLS 2026-08-22] 잔재 화물 강제 제거 - 작업은 지워졌는데 실물만 남아
        //   RGV/S/C 의 HS 를 막고 있을 때 그 슬롯만 비운다([상태 초기화]와 달리 전체를 건드리지 않는다).
        private void btnClearPallet_Click(object sender, EventArgs e)
        {
            if (engine == null) return;
            string spec = txtClearTrack.Text;
            if (MessageBox.Show(this, "[" + spec + "] 의 화물을 강제로 제거합니다.\r\n" +
                                      "정상 운전 경로가 아닙니다 - 잔재 정리에만 쓰세요.", "EQP_SIM",
                MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) != DialogResult.OK) return;
            string msg;
            engine.RemovePallet(spec, out msg);
            lblStatus.Text = msg;
        }

        // [LGLS 2026-07-24] [시나리오 테스트] — 자동 운전 정지 + PPT 시나리오 수동 재현 창
        private ScenarioTestForm scenarioForm;
        private void btnScenarioTest_Click(object sender, EventArgs e)
        {
            if (engine == null) return;
            if (scenarioForm != null && !scenarioForm.IsDisposed) { scenarioForm.Activate(); return; }
            engine.Pause();
            lblStatus.Text = "[시나리오 테스트] 자동 운전 정지 — 시나리오 창에서 수동 재현";
            scenarioForm = new ScenarioTestForm(engine);
            scenarioForm.FormClosed += (s, ev) => { lblStatus.Text = "가동 중 — 자동 운전 재개됨"; };
            scenarioForm.Show(this);
        }

        private void SafeUI(Action action)
        {
            if (IsDisposed) return;
            if (InvokeRequired) { try { BeginInvoke(action); } catch { } }
            else action();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try { if (server != null) server.Dispose(); } catch { }
            try { if (autoRunner != null) autoRunner.Dispose(); } catch { }
            try { if (engine != null) engine.Dispose(); } catch { }
        }
    }
}
