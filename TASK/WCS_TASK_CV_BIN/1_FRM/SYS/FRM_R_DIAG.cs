using System;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace WCS_TASK_CV
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-08-21] R(트래킹) 진법 진단 창
    //
    //   목적 : 실 PLC 에서 R 영역 문서표기(예 R0100)가 10진 워드주소인지
    //          16진 워드주소인지 실측으로 판별한다.
    //
    //   원리 : 같은 문서표기를 두 방식으로 해석한 두 주소에 서로 다른 표식을 쓴다.
    //            16진 해석 주소(0x100=256워드)  ← "HEX!"
    //            10진 해석 주소(100워드)        ← "DEC!"
    //          ① 쓰기 직후 양쪽을 읽어 에코가 맞으면 통신 왕복은 정상.
    //          ② PLC 래더/XG5000 모니터에서 벤더 표기 R0100 위치를 보면
    //             "DEC!" 가 보이면 10진, "HEX!" 가 보이면 16진이 정답이다.
    //          ③ 자동운전 중이면 WCS 가 JOB NO 를 쓰는 위치도 같은 원리로 확인된다.
    //
    //   ⚠ 운영 주의 : 시험 후 [지우기]로 두 주소를 0 으로 되돌릴 것.
    //     기본값 R0100(C/V#11 트래킹)은 시험 파렛트가 없는 상태에서 쓸 것.
    // ─────────────────────────────────────────────────────────────────────────
    public class FRM_R_DIAG : Form
    {
        private FenetProtocol _plc = null;
        private TextBox txtDoc;
        private TextBox txtLog;
        private Label   lblAddr;
        private Button  btnWrite, btnRead, btnClear;
        private string  _ip = ""; private int _port = 0;

        public FRM_R_DIAG()
        {
            Text = "R 진법 진단 — 실 PLC 판별 (16진 vs 10진)";
            Width = 640; Height = 480; StartPosition = FormStartPosition.CenterParent;
            Font = new Font("맑은 고딕", 9F);

            var top = new FlowLayoutPanel { Dock = DockStyle.Top, Height = 34, Padding = new Padding(8, 6, 0, 0), WrapContents = false };
            top.Controls.Add(new Label { Text = "문서표기 R", AutoSize = true, Margin = new Padding(0, 5, 0, 0) });
            txtDoc = new TextBox { Width = 60, Text = "0100" };
            txtDoc.TextChanged += delegate { UpdateAddrLabel(); };
            top.Controls.Add(txtDoc);
            btnWrite = new Button { Text = "① 표식 기록", Width = 100, Height = 24 };
            btnWrite.Click += delegate { DoWrite(); };
            top.Controls.Add(btnWrite);
            btnRead = new Button { Text = "② 읽어 확인", Width = 100, Height = 24 };
            btnRead.Click += delegate { DoRead(); };
            top.Controls.Add(btnRead);
            btnClear = new Button { Text = "③ 지우기(0)", Width = 100, Height = 24 };
            btnClear.Click += delegate { DoClear(); };
            top.Controls.Add(btnClear);

            lblAddr = new Label { Dock = DockStyle.Top, Height = 40, Padding = new Padding(10, 2, 0, 0), ForeColor = Color.DarkBlue };

            var guide = new Label
            {
                Dock = DockStyle.Top, Height = 76, Padding = new Padding(10, 4, 4, 0), ForeColor = Color.DarkRed,
                Text = "판별법 : ① 기록 후 PLC측 XG5000 모니터에서 벤더 표기 주소(R0100)를 볼 것.\n" +
                       "  \"DEC!\" 가 보이면 → 문서표기는 10진 (R_ADDR_MODE=DEC 가 정답)\n" +
                       "  \"HEX!\" 가 보이면 → 문서표기는 16진 (R_ADDR_MODE=HEX 유지, 구 ECS 와 동일)\n" +
                       "⚠ 시험 후 반드시 ③ 지우기. 자동운전 정지 상태(시험 파렛트 없음)에서 수행할 것."
            };

            txtLog = new TextBox { Dock = DockStyle.Fill, Multiline = true, ReadOnly = true, ScrollBars = ScrollBars.Vertical, Font = new Font("Consolas", 9F) };

            Controls.Add(txtLog);
            Controls.Add(guide);
            Controls.Add(lblAddr);
            Controls.Add(top);

            UpdateAddrLabel();
            Load += delegate { Connect(); };
            FormClosed += delegate { if (_plc != null) { try { _plc.CloseSocket(); } catch { } _plc = null; } };
        }

        private void Log(string s) { txtLog.AppendText(DateTime.Now.ToString("HH:mm:ss") + "  " + s + Environment.NewLine); }

        private bool ParseDoc(out int hexAddr, out int decAddr)
        {
            hexAddr = -1; decAddr = -1;
            string doc = (txtDoc.Text ?? "").Trim();
            try
            {
                hexAddr = Convert.ToInt32(doc, 16);
                decAddr = int.Parse(doc);
                return true;
            }
            catch { return false; }
        }

        private void UpdateAddrLabel()
        {
            int h, d;
            if (!ParseDoc(out h, out d)) { lblAddr.Text = "문서표기가 올바르지 않습니다 (0~9 숫자, 16진 해석 가능해야 함)"; return; }
            lblAddr.Text = string.Format("16진 해석 : 워드 {0}  (%RB{1})  ← \"HEX!\" 기록\r\n10진 해석 : 워드 {2}  (%RB{3})  ← \"DEC!\" 기록",
                                         h, h * 2, d, d * 2);
        }

        // ── COMM0(TASK 와 동일 설정)으로 접속 ─────────────────────────────
        private void Connect()
        {
            try
            {
                string strPlcNo = "", strIp = "", strLogPath = "", strFileNm = "", strEqmt = "", strMsg = "";
                int nCur = 0, nFrom = 0, nTo = 0, nPortCnt = 0, nCnt = 0, nIdx = 0, nFrTrack = 0, nToTrack = 0;
                cDefApi.GsReadInitProfileCom("COMM0",
                                             ref strPlcNo, ref strIp, ref nCur, ref nFrom, ref nTo,
                                             ref nPortCnt, ref nCnt, ref nIdx, ref nFrTrack, ref nToTrack,
                                             ref strLogPath, ref strFileNm, ref strEqmt, ref strMsg);
                if (string.IsNullOrEmpty(strIp) || nCur <= 0) { Log("[접속] COMM0 설정 없음 — WCS_DB.INI 확인"); return; }

                _plc = new FenetProtocol();
                string rtn = "";
                if (_plc.OpenSocketOnly(strIp, nCur, ref rtn))
                {
                    _ip = strIp; _port = nCur;
                    Log(string.Format("[접속] {0}:{1} 성공 (TASK 와 동일한 COMM0 설정)", strIp, nCur));
                    Log("       현재 R_ADDR_MODE=" + (cDefApp.GM_R_ADDR_HEX ? "HEX(16진)" : "DEC(10진)") + " — 이 창의 판별 결과와 대조하세요.");
                }
                else { _plc = null; Log("[접속 실패] " + rtn); }
            }
            catch (Exception ex) { Log("[접속 예외] " + ex.Message); }
        }

        private bool WriteWords(int wordAddr, string ascii4)
        {
            byte[] tx = new byte[4];
            for (int i = 0; i < 4; i++) tx[i] = (byte)ascii4[i];
            return _plc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                              (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R,
                              wordAddr, 2, tx);
        }

        private string ReadWords(int wordAddr)
        {
            byte[] rx = new byte[16];
            if (!_plc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT,
                           (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R,
                           wordAddr, 2, ref rx))
                return null;
            var sb = new StringBuilder();
            for (int i = 0; i < 4; i++)
            {
                char c = (char)rx[i];
                sb.Append(c >= 0x20 && c < 0x7F ? c : '.');
            }
            return string.Format("\"{0}\"  ({1:X2} {2:X2} {3:X2} {4:X2})", sb, rx[0], rx[1], rx[2], rx[3]);
        }

        private void DoWrite()
        {
            int h, d;
            if (_plc == null) { Log("[기록] 미접속"); return; }
            if (!ParseDoc(out h, out d)) { Log("[기록] 문서표기 오류"); return; }
            if (h == d) { Log("[기록] 이 표기는 16진/10진 해석이 같아 판별에 쓸 수 없습니다. 0100 처럼 자릿수에 A~F 없이 두 해석이 갈리는 값을 쓰세요."); return; }
            Log(string.Format("[기록] 워드 {0} ← \"HEX!\" : {1}", h, WriteWords(h, "HEX!") ? "OK" : "실패"));
            Log(string.Format("[기록] 워드 {0} ← \"DEC!\" : {1}", d, WriteWords(d, "DEC!") ? "OK" : "실패"));
            Log("→ 이제 PLC측 모니터에서 벤더 표기 R" + txtDoc.Text.Trim() + " 위치의 문자를 확인하세요.");
        }

        private void DoRead()
        {
            int h, d;
            if (_plc == null) { Log("[읽기] 미접속"); return; }
            if (!ParseDoc(out h, out d)) return;
            string vh = ReadWords(h), vd = ReadWords(d);
            Log(string.Format("[읽기] 16진 해석 워드 {0} = {1}", h, vh ?? "실패"));
            Log(string.Format("[읽기] 10진 해석 워드 {0} = {1}", d, vd ?? "실패"));
            if (vh != null && vh.Contains("HEX!") && vd != null && vd.Contains("DEC!"))
                Log("→ 에코 정상. 통신 왕복은 양쪽 다 유효합니다. 판별은 PLC측 모니터 확인으로 하세요.");
        }

        private void DoClear()
        {
            int h, d;
            if (_plc == null) { Log("[지우기] 미접속"); return; }
            if (!ParseDoc(out h, out d)) return;
            Log(string.Format("[지우기] 워드 {0} ← 0000 : {1}", h, WriteWords(h, "\0\0\0\0") ? "OK" : "실패"));
            Log(string.Format("[지우기] 워드 {0} ← 0000 : {1}", d, WriteWords(d, "\0\0\0\0") ? "OK" : "실패"));
        }
    }
}
