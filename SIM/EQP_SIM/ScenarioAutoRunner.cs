// [LGLS 2026-09-01] 시나리오 헤드리스 검증 러너 (사용자 지시 : 전 시나리오 자동 검증)
//   UI 폼(ScenarioStepForm)의 자동 진행 규칙을 그대로 쓰되, 파일 트리거로 무인 구동한다.
//     bin\Debug\scenario_cmd.txt    : "run <id> [crane]" / "resume" 를 쓰면 1초 내 처리
//     bin\Debug\scenario_result.txt : "OK <id>" 또는 "STUCK <id> step <n> : <desc>"
//     bin\Debug\scenario_run.log    : 스텝별 진행 로그(추적용)
//   실행 중에는 engine.Pause()(자동 운전 정지) - 폼의 [시나리오 테스트]와 동일 조건.
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using EQP_SIM.Core;
using EQP_SIM.Sim;

namespace EQP_SIM
{
    internal class ScenarioAutoRunner : IDisposable
    {
        private readonly ScenarioEngine engine;
        private readonly PlcMemory mem;
        private readonly string dir;
        private readonly Thread th;
        private volatile bool stop;

        private const int STEP_TIMEOUT_SEC = 90;    // 이 시간 안에 조건이 안 서면 STUCK
        private const int EQP_GAP_MS = 1500;        // EQP 비트 간 간격(WCS 폴링이 앞 상태를 볼 시간)
        private const int WORD_SHOW_MS = 1200;      // D 워드 표시 대기

        public ScenarioAutoRunner(ScenarioEngine engine)
        {
            this.engine = engine;
            this.mem = engine.Memory;
            this.dir = AppDomain.CurrentDomain.BaseDirectory;
            th = new Thread(Loop) { IsBackground = true, Name = "ScenarioAutoRunner" };
            th.Start();
        }

        public void Dispose() { stop = true; }

        private string CmdPath { get { return Path.Combine(dir, "scenario_cmd.txt"); } }
        private string ResPath { get { return Path.Combine(dir, "scenario_result.txt"); } }
        private string LogPath { get { return Path.Combine(dir, "scenario_run.log"); } }

        private void Log(string msg)
        {
            try { File.AppendAllText(LogPath, DateTime.Now.ToString("HH:mm:ss.fff ") + msg + "\r\n", Encoding.UTF8); }
            catch { }
        }
        private void Result(string msg)
        {
            try { File.WriteAllText(ResPath, msg, Encoding.UTF8); } catch { }
            Log("RESULT: " + msg);
        }

        private void Loop()
        {
            while (!stop)
            {
                try
                {
                    if (File.Exists(CmdPath))
                    {
                        string cmd = File.ReadAllText(CmdPath, Encoding.UTF8).Trim();
                        File.Delete(CmdPath);
                        if (cmd.StartsWith("run ", StringComparison.OrdinalIgnoreCase))
                        {
                            string[] a = cmd.Substring(4).Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                            string id = a.Length > 0 ? a[0] : "1";
                            int crane = 1; if (a.Length > 1) int.TryParse(a[1], out crane);
                            RunOne(id, crane);
                        }
                        else if (cmd.StartsWith("verify ", StringComparison.OrdinalIgnoreCase))
                        {
                            string[] a = cmd.Substring(7).Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                            string id = a.Length > 0 ? a[0] : "1";
                            int crane = 1; if (a.Length > 1) int.TryParse(a[1], out crane);
                            VerifyOne(id, crane);
                        }
                        else if (string.Equals(cmd, "resume", StringComparison.OrdinalIgnoreCase))
                        {
                            engine.Resume();
                            Result("RESUMED");
                        }
                    }
                }
                catch (Exception ex) { Result("ERROR " + ex.Message); }
                Thread.Sleep(1000);
            }
        }

        private void RunOne(string id, int crane)
        {
            var list = ScenarioTestForm.BuildScenariosFromXml(crane);
            Scenario sc = null;
            if (list != null) foreach (var s in list) if (s.Id == id) { sc = s; break; }
            if (sc == null) { Result("NOSCENARIO " + id); return; }

            engine.Pause();
            Log("=== RUN " + id + " (crane " + crane + ") : " + sc.Title + " / steps " + sc.Steps.Count);

            // 시나리오 관련 비트/R 초기화 (폼 ResetAll 과 동일)
            foreach (var st in sc.Steps)
            {
                if (st.Kind != 2) mem.SetBit(st.Dev, st.Bit, false);
                else if (st.Dev == 'R') mem.SetString('R', st.WordAddr, 2, "0000");
            }
            // [LGLS 2026-09-01] 차량 상태를 IDLE(1)로 복원.
            //   시나리오 모드에선 설비 시뮬이 정지라 이전 운전의 상태값(RUN=2 등)이 D 영역에
            //   눌러앉고, DriveSC/DriveRGV 는 유휴(1)에만 지시하므로 시작부터 막힌다(실측).
            //   이 시나리오가 "IDLE" 을 관측하는 상태 주소들에 1 을 써 둔다.
            foreach (var st in sc.Steps)
                if (st.Kind == 2 && st.Dev == 'D' && !st.IsString &&
                    st.Desc != null && st.Desc.Contains("IDLE"))
                    mem.SetWord('D', st.WordAddr, 1);

            DateTime waitStart = DateTime.Now;
            for (int i = 0; i < sc.Steps.Count && !stop; )
            {
                var st = sc.Steps[i];
                if ((DateTime.Now - waitStart).TotalSeconds > STEP_TIMEOUT_SEC)
                {
                    Result("STUCK " + id + " step " + (i + 1) + " : " + st.Desc);
                    return;
                }
                if (st.Kind == 0)
                {
                    if ((DateTime.Now - waitStart).TotalMilliseconds < EQP_GAP_MS) { Thread.Sleep(100); continue; }
                    mem.SetBit(st.Dev, st.Bit, st.Val);
                    Log("step " + (i + 1) + " [EQP] " + st.Desc);
                    i++; waitStart = DateTime.Now;
                }
                else if (st.Kind == 1)
                {
                    if (mem.GetBit('M', st.Bit))
                    { Log("step " + (i + 1) + " [WCS] " + st.Desc); i++; waitStart = DateTime.Now; }
                    else Thread.Sleep(200);
                }
                else
                {
                    if (st.Dev == 'R' && st.IsString)
                    {
                        string v = mem.GetString('R', st.WordAddr, 2);
                        bool hot = !string.IsNullOrEmpty(v) && v != "0" && v != "0000";
                        if (!hot && IsPlcWritten(st))
                        {
                            int src = PrevRAddr(sc, st);
                            string pv = (src >= 0) ? mem.GetString('R', src, 2) : null;
                            if (!string.IsNullOrEmpty(pv) && pv != "0" && pv != "0000")
                            {
                                mem.SetString('R', st.WordAddr, 2, pv);
                                mem.SetString('R', src, 2, "0000");
                                hot = true;
                                Log("step " + (i + 1) + " [PLC 재현] " + st.Desc + " (" + pv + " 이동)");
                            }
                            else
                            {
                                // [LGLS 2026-09-01] 출고 첫 탑재 : 앞선 R 트래킹이 없다(화물이 랙에서 나옴).
                                //   실물 규약대로 WCS 지시의 Job No(D 워드 2워드 ASCII)에서 채운다.
                                int dj = PrevDJobNo(sc, st);
                                string jv = (dj >= 0) ? mem.GetString('D', dj, 2) : null;
                                Log("  (진단) JobNo 폴백 dj=" + dj + " raw='" + (jv ?? "(null)") + "' word=" +
                                    (dj >= 0 ? mem.GetWord('D', dj).ToString("X4") + " " + mem.GetWord('D', dj + 1).ToString("X4") : "-"));
                                if (!string.IsNullOrEmpty(jv) && jv != "0" && jv != "0000")
                                {
                                    mem.SetString('R', st.WordAddr, 2, jv);
                                    hot = true;
                                    Log("step " + (i + 1) + " [PLC 재현] " + st.Desc + " (지시 Job No " + jv + " 적재)");
                                }
                            }
                        }
                        if (hot)
                        { if (!IsPlcWritten(st)) Log("step " + (i + 1) + " [R 관측] " + st.Desc + " = " + mem.GetString('R', st.WordAddr, 2)); i++; waitStart = DateTime.Now; }
                        else Thread.Sleep(200);
                    }
                    else
                    {
                        if ((DateTime.Now - waitStart).TotalMilliseconds < WORD_SHOW_MS) { Thread.Sleep(100); continue; }
                        Log("step " + (i + 1) + " [D 관측] " + st.Desc);
                        i++; waitStart = DateTime.Now;
                    }
                }
            }
            Result("OK " + id);
        }

        // [LGLS 2026-09-01] EQP_TASK(CV_TASK) 관점 검증 : ★관측자 모드★
        //   아무 신호도 쓰지 않는다. EQP_SIM 은 자동 운전, WCS(CV_TASK/IO_TASK)도 정상 가동인 채로
        //   실작업이 흐르는 동안 시나리오의 신호 체인이 순서대로 성립하는지 PLC 메모리에서 확인한다.
        //   펄스(ON→OFF 수 초)는 폴링이 놓칠 수 있으므로 : OFF 스텝은 이미 OFF 면 통과,
        //   ON 스텝은 대기 중 뒤 스텝(최대 4개) 조건이 먼저 성립하면 '펄스 관측 놓침' 으로 스킵한다.
        private void VerifyOne(string id, int crane)
        {
            var list = ScenarioTestForm.BuildScenariosFromXml(crane);
            Scenario sc = null;
            if (list != null) foreach (var s in list) if (s.Id == id) { sc = s; break; }
            if (sc == null) { Result("NOSCENARIO " + id); return; }
            engine.Resume();   // 자동 운전 보장
            Log("=== VERIFY " + id + " (crane " + crane + ") : " + sc.Title + " / steps " + sc.Steps.Count);

            DateTime waitStart = DateTime.Now;
            for (int i = 0; i < sc.Steps.Count && !stop; )
            {
                var st = sc.Steps[i];
                bool ok = StepSatisfied(st);
                if (ok)
                {
                    Log("step " + (i + 1) + " 관측 " + st.Desc);
                    i++; waitStart = DateTime.Now; continue;
                }
                // 뒤 스텝 선성립 → 펄스 놓침 스킵.
                // [LGLS 2026-09-01] 근거는 ★적극적 관측★(비트 ON / R 에 JOB 실림)만 인정한다 -
                //   D 관측이나 OFF 기대는 항상 참이라, 이를 근거로 삼으면 아직 오지도 않은
                //   구간을 폭주 스킵한다(실측 : 시작 직후 스텝 9~12 를 무근거 스킵).
                for (int k = i + 1; k <= i + 4 && k < sc.Steps.Count; k++)
                {
                    var nx = sc.Steps[k];
                    bool positive =
                        (nx.Kind == 1 && mem.GetBit('M', nx.Bit)) ||
                        (nx.Kind == 0 && nx.Val && mem.GetBit('M', nx.Bit)) ||
                        (nx.Kind == 2 && nx.Dev == 'R' && nx.IsString && StepSatisfied(nx));
                    if (positive)
                    {
                        for (int m = i; m < k; m++) Log("step " + (m + 1) + " (펄스 관측 놓침 - 뒤 스텝 " + (k + 1) + " 성립) " + sc.Steps[m].Desc);
                        i = k; waitStart = DateTime.Now; ok = true; break;
                    }
                }
                if (ok) continue;
                if ((DateTime.Now - waitStart).TotalSeconds > STEP_TIMEOUT_SEC * 2)   // 실물 사이클(이동 수십 초) 여유
                { Result("STUCK-VERIFY " + id + " step " + (i + 1) + " : " + st.Desc); return; }
                Thread.Sleep(150);
            }
            Result("VERIFIED " + id);
        }

        private bool StepSatisfied(ScStep st)
        {
            if (st.Kind == 0 || st.Kind == 1)
            {
                bool v = mem.GetBit('M', st.Bit);
                bool want = (st.Kind == 1) ? true : st.Val;
                if (!want) return !v;      // OFF 기대 : 이미 꺼져 있으면 성립
                return v;
            }
            if (st.Dev == 'R' && st.IsString)
            {
                string v = mem.GetString('R', st.WordAddr, 2);
                return !string.IsNullOrEmpty(v) && v != "0" && v != "0000";
            }
            return true;   // D 워드는 값 표시용 - 관측 검증에서는 통과
        }

        private static bool IsPlcWritten(ScStep st)
        {
            if (st.Kind != 2 || st.Dev != 'R') return false;
            if (string.IsNullOrEmpty(st.Src)) return true;
            return st.Src.Contains("PollObservations") || st.Src.Contains("CvStatusScenario");
        }

        // 이 스텝보다 앞의 D 지시 스텝 중 Job No 워드주소 (없으면 -1)
        private static int PrevDJobNo(Scenario sc, ScStep st)
        {
            int idx = sc.Steps.IndexOf(st);
            for (int k = idx - 1; k >= 0; k--)
                if (sc.Steps[k].Kind == 2 && sc.Steps[k].Dev == 'D' &&
                    sc.Steps[k].Desc != null && sc.Steps[k].Desc.Contains("Job No"))
                    return sc.Steps[k].WordAddr;
            return -1;
        }

        private static int PrevRAddr(Scenario sc, ScStep st)
        {
            int idx = sc.Steps.IndexOf(st);
            for (int k = idx - 1; k >= 0; k--)
                if (sc.Steps[k].Kind == 2 && sc.Steps[k].Dev == 'R' && sc.Steps[k].WordAddr != st.WordAddr)
                    return sc.Steps[k].WordAddr;
            return -1;
        }
    }
}
