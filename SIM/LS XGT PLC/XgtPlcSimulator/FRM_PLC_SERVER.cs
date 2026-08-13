using XgtPlcSimulator.Memory;
using XgtPlcSimulator.Server;

namespace XgtPlcSimulator;

public partial class FRM_PLC_SERVER : Form
{
    private readonly PlcMemory     _mem;
    private readonly XgtTcpServer  _server;
    private readonly System.Windows.Forms.Timer _refreshTimer;

    // Current manual-view range
    private MemArea _manArea  = MemArea.D;
    private int     _manStart = 0;
    private int     _manCount = 100;

    // Display row limits per main tab
    private const int SHOW_M = 500;
    private const int SHOW_D = 1000;
    private const int SHOW_R = 1000;
    private const int SHOW_B = 1000;
    private const int SHOW_W = 1000;

    public FRM_PLC_SERVER()
    {
        InitializeComponent();

        _mem    = new PlcMemory(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "data"));
        _server = new XgtTcpServer(_mem);

        _server.LogMessage        += OnLog;
        _server.ClientConnected   += OnClientConnected;
        _server.ClientDisconnected += OnClientDisconnected;

        _refreshTimer = new System.Windows.Forms.Timer { Interval = 500 };
        _refreshTimer.Tick += (_, _) => RefreshCurrentTab();
        _refreshTimer.Start();

        // Pre-populate main tabs once
        PopulateBit();
        PopulateWord();
        PopulateTracking();
        PopulateB();
        PopulateW();
    }

    // ── server control ──────────────────────────────────────────────────────

    private void BtnStart_Click(object? sender, EventArgs e)
    {
        if (!int.TryParse(txtPort.Text.Trim(), out int port) || port < 1 || port > 65535)
        {
            Log("포트 번호가 올바르지 않습니다.");
            return;
        }
        try
        {
            string bindIp = txtBindIp.Text.Trim();
            _server.Start(port, bindIp);
            btnStart.Enabled     = false;
            btnStop.Enabled      = true;
            txtPort.Enabled      = false;
            txtBindIp.Enabled    = false;
            lblSrvStatus.Text    = $"● 실행 중 ({bindIp}:{port})";
            lblSrvStatus.ForeColor = Color.LimeGreen;
        }
        catch (Exception ex)
        {
            Log($"[오류] 서버 시작 실패: {ex.Message}");
        }
    }

    private void BtnStop_Click(object? sender, EventArgs e)
    {
        _server.Stop();
        btnStart.Enabled      = true;
        btnStop.Enabled       = false;
        txtPort.Enabled       = true;
        txtBindIp.Enabled     = true;
        lblSrvStatus.Text     = "● 중지";
        lblSrvStatus.ForeColor = Color.OrangeRed;
        lstClients.Items.Clear();
    }

    // ── client list ─────────────────────────────────────────────────────────

    private void OnClientConnected(ClientInfo ci)
        => SafeInvoke(() =>
        {
            lstClients.Items.Add($"[{DateTime.Now:HH:mm:ss}] 연결: {ci.Ip}:{ci.Port}");
            lstClients.TopIndex = lstClients.Items.Count - 1;
        });

    private void OnClientDisconnected(ClientInfo ci)
        => SafeInvoke(() =>
        {
            // Mark disconnected but keep log entry
            for (int i = 0; i < lstClients.Items.Count; i++)
            {
                var s = lstClients.Items[i]?.ToString() ?? "";
                if (s.Contains($"{ci.Ip}:{ci.Port}") && !s.Contains("해제"))
                {
                    lstClients.Items[i] = s + "  ← 해제";
                    break;
                }
            }
        });

    // ── write handlers ──────────────────────────────────────────────────────

    private void BtnBitWrite_Click(object? sender, EventArgs e)
    {
        if (!ParseAddr(txtBitAddr.Text, out int addr))  { Log("M 주소 오류"); return; }
        if (!ParseValue(txtBitVal.Text, out ushort val)) { Log("M 값 오류");   return; }
        _mem.WriteWord(MemArea.M, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] MW{addr} = 0x{val:X4} ({val})");
        PopulateBit();
    }

    private void BtnBWrite_Click(object? sender, EventArgs e)
    {
        if (!ParseAddr(txtBAddr.Text, out int addr))   { Log("B 주소 오류"); return; }
        if (!ParseValue(txtBVal.Text, out ushort val)) { Log("B 값 오류");   return; }
        _mem.WriteWord(MemArea.B, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] BW{addr} = 0x{val:X4} ({val})");
        PopulateB();
    }

    private void BtnWWrite_Click(object? sender, EventArgs e)
    {
        if (!ParseAddr(txtWAddr.Text, out int addr))   { Log("W 주소 오류"); return; }
        if (!ParseValue(txtWVal.Text, out ushort val)) { Log("W 값 오류");   return; }
        _mem.WriteWord(MemArea.W, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] WW{addr} = 0x{val:X4} ({val})");
        PopulateW();
    }

    private void BtnWordWrite_Click(object? sender, EventArgs e)
    {
        if (!ParseAddr(txtWordAddr.Text, out int addr))  { Log("D 주소 오류"); return; }
        if (!ParseValue(txtWordVal.Text, out ushort val)) { Log("D 값 오류");   return; }
        _mem.WriteWord(MemArea.D, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] DW{addr} = 0x{val:X4} ({val})");
        PopulateWord();
    }

    private void BtnTrackWrite_Click(object? sender, EventArgs e)
    {
        if (!ParseAddr(txtTrackAddr.Text, out int addr))  { Log("R 주소 오류"); return; }
        if (!ParseValue(txtTrackVal.Text, out ushort val)) { Log("R 값 오류");   return; }
        _mem.WriteWord(MemArea.R, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] RW{addr} = 0x{val:X4} ({val})");
        PopulateTracking();
    }

    private MemArea ParseManualArea() => cmbManualDev.Text switch
    {
        "%MB" or "%MW" => MemArea.M,
        "%DB" or "%DW" => MemArea.D,
        "%RW" or "%RB" => MemArea.R,
        "%BW"          => MemArea.B,
        "%WW"          => MemArea.W,
        _              => MemArea.D
    };

    private void BtnManualView_Click(object? sender, EventArgs e)
    {
        _manArea  = ParseManualArea();
        if (!ParseAddr(txtManualAddr.Text, out _manStart)) { Log("Manual 주소 오류"); return; }
        if (!int.TryParse(txtManualCnt.Text.Trim(), out _manCount) || _manCount < 1)
        { Log("개수 오류"); return; }
        _manCount = Math.Min(_manCount, 5000);
        PopulateManual();
    }

    private void BtnManualWrite_Click(object? sender, EventArgs e)
    {
        _manArea = ParseManualArea();
        if (!ParseAddr(txtManualAddr.Text, out int addr))   { Log("Manual 주소 오류"); return; }
        if (!ParseValue(txtManualVal.Text, out ushort val)) { Log("Manual 값 오류");   return; }

        // If a row is selected in dgvManual, write to that address
        if (dgvManual.SelectedRows.Count > 0)
        {
            var cell = dgvManual.SelectedRows[0].Cells["colManAddr"].Value?.ToString() ?? "";
            if (int.TryParse(cell, out int selAddr)) addr = selAddr;
        }

        _mem.WriteWord(_manArea, addr, val);
        _mem.Save();
        Log($"[FORCE WRITE] {_manArea}W{addr} = 0x{val:X4} ({val})");
        PopulateManual();
    }

    // ── grid population ─────────────────────────────────────────────────────

    private void PopulateBit()
    {
        dgvBit.SuspendLayout();
        dgvBit.Rows.Clear();
        for (int i = 0; i < Math.Min(SHOW_M, _mem.MSize); i++)
        {
            var w = _mem.ReadWord(MemArea.M, i);
            dgvBit.Rows.Add(i, $"0x{w:X4}", w, ToBin16(w));
        }
        dgvBit.ResumeLayout();
    }

    private void PopulateWord()
    {
        dgvWord.SuspendLayout();
        dgvWord.Rows.Clear();
        for (int i = 0; i < Math.Min(SHOW_D, _mem.DSize); i++)
        {
            var w = _mem.ReadWord(MemArea.D, i);
            dgvWord.Rows.Add(i, $"0x{w:X4}", w);
        }
        dgvWord.ResumeLayout();
    }

    private void PopulateTracking()
    {
        dgvTracking.SuspendLayout();
        dgvTracking.Rows.Clear();
        for (int i = 0; i < Math.Min(SHOW_R, _mem.RSize); i++)
        {
            var w = _mem.ReadWord(MemArea.R, i);
            dgvTracking.Rows.Add(i, $"0x{w:X4}", w);
        }
        dgvTracking.ResumeLayout();
    }

    private void PopulateB()
    {
        dgvB.SuspendLayout();
        dgvB.Rows.Clear();
        for (int i = 0; i < Math.Min(SHOW_B, _mem.BSize); i++)
        {
            var w = _mem.ReadWord(MemArea.B, i);
            dgvB.Rows.Add(i, $"0x{w:X4}", w, ToBin16(w));
        }
        dgvB.ResumeLayout();
    }

    private void PopulateW()
    {
        dgvW.SuspendLayout();
        dgvW.Rows.Clear();
        for (int i = 0; i < Math.Min(SHOW_W, _mem.WSize); i++)
        {
            var w = _mem.ReadWord(MemArea.W, i);
            dgvW.Rows.Add(i, $"0x{w:X4}", w);
        }
        dgvW.ResumeLayout();
    }

    private void PopulateManual()
    {
        dgvManual.SuspendLayout();
        dgvManual.Rows.Clear();
        // 표시 접두사: 선택된 Device 항목 그대로 사용 (예: %MB, %DW)
        string prefix = cmbManualDev.Text.Length > 0 ? cmbManualDev.Text : "%DW";
        for (int i = 0; i < _manCount; i++)
        {
            int addr = _manStart + i;
            var w = _mem.ReadWord(_manArea, addr);
            // byte 접두사(%MB/%DB 등)는 주소를 byte 환산, word 접두사는 그대로
            string dispAddr = prefix.EndsWith("B")
                ? $"{prefix}{addr * 2}"
                : $"{prefix}{addr}";
            dgvManual.Rows.Add(dispAddr, $"0x{w:X4}", w, ToBin16(w));
        }
        dgvManual.ResumeLayout();
    }

    // ── auto-refresh (500 ms) ────────────────────────────────────────────────

    private void RefreshCurrentTab()
    {
        var sel = tabMain.SelectedTab;
        if      (sel == tabBit)      UpdateBitGrid();
        else if (sel == tabWord)     UpdateWordGrid();
        else if (sel == tabTracking) UpdateTrackingGrid();
        else if (sel == tabB)        UpdateBGrid();
        else if (sel == tabW)        UpdateWGrid();
        else if (sel == tabManual)   UpdateManualGrid();
    }

    private void UpdateBitGrid()
    {
        int limit = Math.Min(SHOW_M, _mem.MSize);
        if (dgvBit.Rows.Count != limit) { PopulateBit(); return; }
        for (int i = 0; i < limit; i++)
        {
            var w = _mem.ReadWord(MemArea.M, i);
            var row = dgvBit.Rows[i];
            SetCell(row, "colBitHex", $"0x{w:X4}");
            SetCell(row, "colBitDec", w.ToString());
            SetCell(row, "colBitBin", ToBin16(w));
        }
    }

    private void UpdateWordGrid()
    {
        int limit = Math.Min(SHOW_D, _mem.DSize);
        if (dgvWord.Rows.Count != limit) { PopulateWord(); return; }
        for (int i = 0; i < limit; i++)
        {
            var w = _mem.ReadWord(MemArea.D, i);
            var row = dgvWord.Rows[i];
            SetCell(row, "colWordHex", $"0x{w:X4}");
            SetCell(row, "colWordDec", w.ToString());
        }
    }

    private void UpdateTrackingGrid()
    {
        int limit = Math.Min(SHOW_R, _mem.RSize);
        if (dgvTracking.Rows.Count != limit) { PopulateTracking(); return; }
        for (int i = 0; i < limit; i++)
        {
            var w = _mem.ReadWord(MemArea.R, i);
            var row = dgvTracking.Rows[i];
            SetCell(row, "colTrackHex", $"0x{w:X4}");
            SetCell(row, "colTrackDec", w.ToString());
        }
    }

    private void UpdateBGrid()
    {
        int limit = Math.Min(SHOW_B, _mem.BSize);
        if (dgvB.Rows.Count != limit) { PopulateB(); return; }
        for (int i = 0; i < limit; i++)
        {
            var w = _mem.ReadWord(MemArea.B, i);
            var row = dgvB.Rows[i];
            SetCell(row, "colBHex", $"0x{w:X4}");
            SetCell(row, "colBDec", w.ToString());
            SetCell(row, "colBBin", ToBin16(w));
        }
    }

    private void UpdateWGrid()
    {
        int limit = Math.Min(SHOW_W, _mem.WSize);
        if (dgvW.Rows.Count != limit) { PopulateW(); return; }
        for (int i = 0; i < limit; i++)
        {
            var w = _mem.ReadWord(MemArea.W, i);
            var row = dgvW.Rows[i];
            SetCell(row, "colWHex", $"0x{w:X4}");
            SetCell(row, "colWDec", w.ToString());
        }
    }

    private void UpdateManualGrid()
    {
        if (dgvManual.Rows.Count != _manCount) { PopulateManual(); return; }
        for (int i = 0; i < _manCount; i++)
        {
            int addr = _manStart + i;
            var w = _mem.ReadWord(_manArea, addr);
            var row = dgvManual.Rows[i];
            SetCell(row, "colManHex", $"0x{w:X4}");
            SetCell(row, "colManDec", w.ToString());
            SetCell(row, "colManBin", ToBin16(w));
        }
    }

    private static void SetCell(DataGridViewRow row, string col, string val)
    {
        if (row.Cells[col].Value?.ToString() != val)
            row.Cells[col].Value = val;
    }

    // ── utilities ────────────────────────────────────────────────────────────

    private static string ToBin16(ushort w)
    {
        var bits = new char[19]; // 16 bits + 3 spaces between groups of 4
        int pos = 0;
        for (int b = 15; b >= 0; b--)
        {
            bits[pos++] = (w & (1 << b)) != 0 ? '1' : '0';
            if (b > 0 && b % 4 == 0) bits[pos++] = ' ';
        }
        return new string(bits, 0, pos);
    }

    private static bool ParseAddr(string s, out int addr)
        => int.TryParse(s.Trim(), out addr) && addr >= 0;

    private static bool ParseValue(string s, out ushort val)
    {
        s = s.Trim();
        if (s.StartsWith("0x", StringComparison.OrdinalIgnoreCase) ||
            s.StartsWith("0X", StringComparison.OrdinalIgnoreCase))
        {
            if (ushort.TryParse(s[2..], System.Globalization.NumberStyles.HexNumber, null, out val))
                return true;
        }
        return ushort.TryParse(s, out val);
    }

    private void Log(string msg)
    {
        SafeInvoke(() =>
        {
            string line = $"[{DateTime.Now:HH:mm:ss}] {msg}";
            txtLog.AppendText(line + Environment.NewLine);
        });
    }

    private void OnLog(string msg) => Log(msg);

    private void ChkHexDump_CheckedChanged(object? sender, EventArgs e)
        => _server.HexDumpEnabled = chkHexDump.Checked;

    private void RdoProtocol_CheckedChanged(object? sender, EventArgs e)
    {
        var mode = rdoLGLS.Checked ? XgtPlcSimulator.Protocol.ProtocolMode.LGLS
                                   : XgtPlcSimulator.Protocol.ProtocolMode.SPL;
        _server.ProtocolMode = mode;
        Log($"[프로토콜] {mode} 모드로 전환");
    }

    private void BtnClearLog_Click(object? sender, EventArgs e)
        => txtLog.Clear();

    private void SafeInvoke(Action action)
    {
        if (InvokeRequired) Invoke(action);
        else action();
    }

    protected override void OnFormClosing(FormClosingEventArgs e)
    {
        _refreshTimer.Stop();
        _server.Stop();
        base.OnFormClosing(e);
    }
}
