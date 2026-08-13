using System.Text.Json;

namespace XgtPlcSimulator.Memory;

public enum MemArea { M, D, R, B, W }

public class PlcMemory
{
    private readonly ushort[] _m;
    private readonly ushort[] _d;
    private readonly ushort[] _r;
    private readonly ushort[] _b;
    private readonly ushort[] _w;
    private readonly string   _dataDir;

    public int MSize => _m.Length;
    public int DSize => _d.Length;
    public int RSize => _r.Length;
    public int BSize => _b.Length;
    public int WSize => _w.Length;

    public PlcMemory(string dataDir = "data")
    {
        _dataDir = dataDir;
        Directory.CreateDirectory(dataDir);
        _m = LoadOrCreate("M.json", 1000);
        _d = LoadOrCreate("D.json", 10000);
        _r = LoadOrCreate("R.json", 10000);
        _b = LoadOrCreate("B.json", 8192);
        _w = LoadOrCreate("W.json", 8192);
    }

    private ushort[] LoadOrCreate(string fileName, int size)
    {
        var path = Path.Combine(_dataDir, fileName);
        if (File.Exists(path))
        {
            try
            {
                var arr = JsonSerializer.Deserialize<ushort[]>(File.ReadAllText(path));
                if (arr != null && arr.Length == size) return arr;
            }
            catch { }
        }
        return new ushort[size];
    }

    public void Save()
    {
        File.WriteAllText(Path.Combine(_dataDir, "M.json"), JsonSerializer.Serialize(_m));
        File.WriteAllText(Path.Combine(_dataDir, "D.json"), JsonSerializer.Serialize(_d));
        File.WriteAllText(Path.Combine(_dataDir, "R.json"), JsonSerializer.Serialize(_r));
        File.WriteAllText(Path.Combine(_dataDir, "B.json"), JsonSerializer.Serialize(_b));
        File.WriteAllText(Path.Combine(_dataDir, "W.json"), JsonSerializer.Serialize(_w));
    }

    // ── word access ────────────────────────────────────────────────────────

    public ushort ReadWord(MemArea area, int addr)
    {
        var arr = GetArray(area);
        return (addr >= 0 && addr < arr.Length) ? arr[addr] : (ushort)0;
    }

    public void WriteWord(MemArea area, int addr, ushort value)
    {
        var arr = GetArray(area);
        if (addr >= 0 && addr < arr.Length) arr[addr] = value;
    }

    public ushort[] ReadWords(MemArea area, int startAddr, int count)
    {
        var arr = GetArray(area);
        var result = new ushort[count];
        for (int i = 0; i < count; i++)
        {
            int a = startAddr + i;
            result[i] = (a >= 0 && a < arr.Length) ? arr[a] : (ushort)0;
        }
        return result;
    }

    public void WriteWords(MemArea area, int startAddr, ushort[] values)
    {
        var arr = GetArray(area);
        for (int i = 0; i < values.Length; i++)
        {
            int a = startAddr + i;
            if (a >= 0 && a < arr.Length) arr[a] = values[i];
        }
    }

    // ── bit access ─────────────────────────────────────────────────────────

    public bool ReadBit(MemArea area, int wordAddr, int bitIdx)
        => (ReadWord(area, wordAddr) & (1 << bitIdx)) != 0;

    public void WriteBit(MemArea area, int wordAddr, int bitIdx, bool value)
    {
        var arr = GetArray(area);
        if (wordAddr < 0 || wordAddr >= arr.Length) return;
        if (value) arr[wordAddr] |= (ushort)(1 << bitIdx);
        else       arr[wordAddr] &= (ushort)~(1 << bitIdx);
    }

    // ── address parsing ────────────────────────────────────────────────────
    // Supported: %MX{w}.{b}  %MW{n}  %MB{n}  %DW{n}  %DB{n}  %RW{n}  %RB{n}
    //            %BW{n}  %BB{n}  %BX{w}.{b}  %WW{n}  %WB{n}
    //            %QW{n}  %IW{n}  (Q/I mapped to D)

    public bool ParseVarName(string name,
        out MemArea area, out int wordAddr, out int bitIdx, out bool isBit)
    {
        area     = MemArea.D;
        wordAddr = 0;
        bitIdx   = 0;
        isBit    = false;

        if (!name.StartsWith('%')) return false;
        var s = name[1..]; // strip leading %

        if (s.StartsWith("MX", StringComparison.OrdinalIgnoreCase))
        {
            area  = MemArea.M;
            isBit = true;
            var parts = s[2..].Split('.');
            return parts.Length == 2
                && int.TryParse(parts[0], out wordAddr)
                && int.TryParse(parts[1], out bitIdx);
        }
        if (s.StartsWith("MW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.M;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("MB", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.M;
            if (!int.TryParse(s[2..], out int ba)) return false;
            wordAddr = ba / 2;
            return true;
        }
        if (s.StartsWith("DW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.D;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("DB", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.D;
            if (!int.TryParse(s[2..], out int ba)) return false;
            wordAddr = ba / 2;
            return true;
        }
        if (s.StartsWith("RW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.R;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("RB", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.R;
            if (!int.TryParse(s[2..], out int ba)) return false;
            wordAddr = ba / 2;
            return true;
        }
        if (s.StartsWith("BX", StringComparison.OrdinalIgnoreCase))
        {
            area  = MemArea.B;
            isBit = true;
            var parts = s[2..].Split('.');
            return parts.Length == 2
                && int.TryParse(parts[0], out wordAddr)
                && int.TryParse(parts[1], out bitIdx);
        }
        if (s.StartsWith("BW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.B;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("BB", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.B;
            if (!int.TryParse(s[2..], out int ba)) return false;
            wordAddr = ba / 2;
            return true;
        }
        if (s.StartsWith("WW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.W;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("WB", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.W;
            if (!int.TryParse(s[2..], out int ba)) return false;
            wordAddr = ba / 2;
            return true;
        }
        if (s.StartsWith("QW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.D;
            return int.TryParse(s[2..], out wordAddr);
        }
        if (s.StartsWith("IW", StringComparison.OrdinalIgnoreCase))
        {
            area = MemArea.D;
            return int.TryParse(s[2..], out wordAddr);
        }
        return false;
    }

    private ushort[] GetArray(MemArea area) => area switch
    {
        MemArea.M => _m,
        MemArea.D => _d,
        MemArea.R => _r,
        MemArea.B => _b,
        MemArea.W => _w,
        _         => throw new ArgumentOutOfRangeException()
    };
}
