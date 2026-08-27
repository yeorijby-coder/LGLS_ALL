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

                timerUi.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(this, "초기화 실패: " + ex.Message, "EQP_SIM",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
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

        // [LGLS 2026-08-24] 가운데 투입 버튼은 "26 입고대" 를 가리킨다.
        //   WCS 모드(MODE/TARGET != ECS, 기본) : EcsDefine 정렬로 126=C/V#13 이 입고대, 124=C/V#12 는 출고대다.
        //     → 종전처럼 CONVEYOR:12 로 투입하면 IngoPath 가 없어 항상 실패했다(시나리오 3-1 시험 불가).
        //   구 ECS 모드                        : C/V#12 가 입고대이므로 그대로 12 로 보낸다.
        //   (WorldModel(wcsOrientation) 의 정의와 1:1 로 맞춘다)
        private void btnFeed12_Click(object sender, EventArgs e)
        {
            ManualFeed((engine != null && engine.WcsSupport) ? "CONVEYOR:13" : "CONVEYOR:12");
        }

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
            try { if (engine != null) engine.Dispose(); } catch { }
        }
    }
}
