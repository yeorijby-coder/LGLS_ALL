using System.Text;
using XgtPlcSimulator.Memory;

namespace XgtPlcSimulator.Protocol;

// SPL  : ECS/SPL LgXgtFEnetIfSk.cpp  — DATACOUNT = byte count (LE uint16)
// LGLS : LGLS_1WH FenetDriver.cs      — DATACOUNT = byte count (BCD digit-pair), port 2004
public enum ProtocolMode { SPL, LGLS }

public class XgtProtocolHandler
{
    private readonly PlcMemory _mem;

    // SPL  : LE uint16 byte-count / 2  (ECS/SPL LgXgtFEnetIfSk.cpp)
    // LGLS : BCD digit-pair byte-count / 2  (LGLS_1WH FenetDriver.cs, port 2004)
    public ProtocolMode Mode { get; set; } = ProtocolMode.SPL;

    // 프로토콜 이벤트 로그 (주소·데이터 단위 상세)
    public event Action<string>? ProtocolLog;

    public XgtProtocolHandler(PlcMemory mem) => _mem = mem;

    // Returns response bytes, or null for invalid/unrecognised packets.
    public byte[]? HandlePacket(byte[] packet)
    {
        if (packet.Length < XgtHeader.SIZE) return null;

        var header = XgtHeader.Parse(packet);
        if (!header.IsValid()) return null;

        if (packet.Length < XgtHeader.SIZE + header.DataLength) return null;

        var payload = packet[XgtHeader.SIZE..(XgtHeader.SIZE + header.DataLength)];
        if (payload.Length < 4) return null;

        ushort cmd      = BitConverter.ToUInt16(payload, 0);
        ushort dataType = BitConverter.ToUInt16(payload, 2);

        // dataType 0x0014 = 연속(Continuous), 그 외(0x00~0x04) = 개별(Individual)
        const ushort DT_CONTINUOUS = 0x0014;

        byte[] respPayload = cmd switch
        {
            0x0054 => dataType == DT_CONTINUOUS
                        ? HandleContinuousRead(payload, dataType, 0x0055)
                        : HandleIndividualRead(payload, dataType),
            0x0058 => dataType == DT_CONTINUOUS
                        ? HandleContinuousWrite(payload, dataType, 0x0059)
                        : HandleIndividualWrite(payload, dataType),
            0x0060 => HandleContinuousRead(payload, dataType, 0x0061),
            0x0064 => HandleContinuousWrite(payload, dataType, 0x0065),
            _      => Array.Empty<byte>()
        };

        if (respPayload.Length == 0) return null;

        // 응답 헤더 : 요청 헤더의 정보를 그대로 echo, 방향(byte13)만 PLC→PC(0x11)
        var respHeader = new XgtHeader
        {
            Reserved1  = header.Reserved1,   // PLC Info echo
            CpuInfo    = header.CpuInfo,     // CPU Info echo
            SrcDest    = 0x11,               // Source of Frame : PLC→PC
            InvokeId   = header.InvokeId,    // Invoke ID echo
            DataLength = (ushort)respPayload.Length,
            Reserved2  = header.Reserved2    // FEnet 위치 echo
            // ErrorFlag(byte19=BCC) 는 Serialize() 에서 자동 계산
        };

        var response = new byte[XgtHeader.SIZE + respPayload.Length];
        respHeader.Serialize().CopyTo(response, 0);
        respPayload.CopyTo(response, XgtHeader.SIZE);
        return response;
    }

    // ── Individual Read (0x54) ──────────────────────────────────────────────
    // Payload layout (after header):
    //   [2] cmd=0x0054  [2] dataType  [2] rsv  [2] blockCount
    //   For each block: [2] nameLen  [nameLen] varName

    private byte[] HandleIndividualRead(byte[] p, ushort dt)
    {
        if (p.Length < 8) return ErrResp(0x0055, dt, 0x0001);
        ushort blockCount = BitConverter.ToUInt16(p, 6);

        var out_ = new List<byte>();
        out_.AddRange(BitConverter.GetBytes((ushort)0x0055));
        out_.AddRange(BitConverter.GetBytes(dt));
        out_.AddRange(new byte[2]); // reserved
        out_.AddRange(new byte[2]); // error = 0
        out_.AddRange(BitConverter.GetBytes(blockCount));

        int pos = 8;
        for (int i = 0; i < blockCount && pos + 2 <= p.Length; i++)
        {
            ushort nameLen = BitConverter.ToUInt16(p, pos); pos += 2;
            if (pos + nameLen > p.Length) break;
            string name = Encoding.ASCII.GetString(p, pos, nameLen); pos += nameLen;

            byte[] data = ReadVar(name, dt);
            out_.AddRange(BitConverter.GetBytes((ushort)data.Length));
            out_.AddRange(data);
        }
        return out_.ToArray();
    }

    // ── Individual Write (0x58) ─────────────────────────────────────────────
    // Payload:
    //   [2] cmd  [2] dt  [2] rsv  [2] blockCount
    //   For each block: [2] nameLen  [nameLen] name  [2] dataLen  [dataLen] value

    private byte[] HandleIndividualWrite(byte[] p, ushort dt)
    {
        if (p.Length < 8) return ErrResp(0x0059, dt, 0x0001);
        ushort blockCount = BitConverter.ToUInt16(p, 6);

        int pos = 8;
        for (int i = 0; i < blockCount && pos + 2 <= p.Length; i++)
        {
            ushort nameLen = BitConverter.ToUInt16(p, pos); pos += 2;
            if (pos + nameLen > p.Length) break;
            string name = Encoding.ASCII.GetString(p, pos, nameLen); pos += nameLen;

            if (pos + 2 > p.Length) break;
            ushort dataLen = BitConverter.ToUInt16(p, pos); pos += 2;
            if (pos + dataLen > p.Length) break;
            byte[] data = p[pos..(pos + dataLen)]; pos += dataLen;

            ProtocolLog?.Invoke(
                $"[WRITE] {name} | dt=0x{dt:X4} | data({dataLen}B)=" +
                string.Join(" ", data.Select(b => $"{b:X2}")));
            WriteVar(name, dt, data);
        }
        _mem.Save();

        var out_ = new List<byte>();
        out_.AddRange(BitConverter.GetBytes((ushort)0x0059));
        out_.AddRange(BitConverter.GetBytes(dt));
        out_.AddRange(new byte[2]);
        out_.AddRange(new byte[2]);
        out_.AddRange(BitConverter.GetBytes(blockCount));
        return out_.ToArray();
    }

    // ── Continuous Read (0x54 + dt 0x14, 또는 0x60) ──────────────────────────
    // 요청 Payload:
    //   [0-1] cmd  [2-3] dt=0x14  [4-5] rsv  [6-7] 블록수(=1)
    //   [8-9] 변수길이  [10..] 변수명  [+0] dcLow  [+1] dcHigh
    //     wordCount = dcHigh*100 + dcLow  (FenetDriver.cs BCD 10진 분할 인코딩)
    //
    // 응답 총 32 + wordCount*2 바이트 (클라이언트 FENET_RECV_HEADER=32 기준)
    //   헤더 20  + [0-1]cmd+1 [2-3]dt [4-5]rsv [6-7]에러=0 [8-9]블록수=1
    //             [10-11]바이트수  [12..] 데이터

    private byte[] HandleContinuousRead(byte[] p, ushort dt, ushort respCmd)
    {
        if (p.Length < 10) return ErrResp(respCmd, dt, 0x0001);
        ushort nameLen = BitConverter.ToUInt16(p, 8);
        if (p.Length < 10 + nameLen + 2) return ErrResp(respCmd, dt, 0x0001);
        string name = Encoding.ASCII.GetString(p, 10, nameLen);

        int count = DecodeDataCount(p, 10 + nameLen);
        if (count <= 0) count = 1;

        if (!_mem.ParseVarName(name, out var area, out int start, out _, out _))
            return ErrResp(respCmd, dt, 0x0002);

        var    words   = _mem.ReadWords(area, start, count);
        ushort byteCnt = (ushort)(count * 2);

        ProtocolLog?.Invoke(
            $"[READ ] {name} | {area} word#{start} × {count}w | " +
            string.Join(" ", words.Take(8).Select(w => $"0x{w:X4}")) +
            (words.Length > 8 ? $" ...(+{words.Length-8})" : ""));

        // 페이로드 12바이트 고정 헤더 → 총 20+12+N*2 = 32+N*2 바이트 ✓
        var out_ = new List<byte>();
        out_.AddRange(BitConverter.GetBytes(respCmd));            // [0-1]  응답 cmd
        out_.AddRange(BitConverter.GetBytes(dt));                 // [2-3]  dt
        out_.AddRange(new byte[2]);                               // [4-5]  rsv  ← 에러코드 위치(=0)
        out_.AddRange(new byte[2]);                               // [6-7]  에러상태 = 0
        out_.AddRange(BitConverter.GetBytes((ushort)0x0001));     // [8-9]  블록수 = 1
        out_.AddRange(BitConverter.GetBytes(byteCnt));            // [10-11] 데이터 바이트 수
        foreach (var w in words) out_.AddRange(BitConverter.GetBytes(w));
        return out_.ToArray();
    }

    // ── Continuous Write (0x58 + dt 0x14, 또는 0x64) ─────────────────────────
    // 요청 Payload:
    //   [0-1] cmd  [2-3] dt=0x14  [4-5] rsv  [6-7] 블록수(=1)
    //   [8-9] 변수길이  [10..] 변수명  [+0] dcLow  [+1] dcHigh  [+2*n] 데이터
    //     wordCount = dcHigh*100 + dcLow  (FenetDriver.cs BCD 10진 분할 인코딩)
    //
    // 응답 총 32 바이트 (클라이언트 FENET_RECV_HEADER=32 기준, 데이터 없음)
    //   헤더 20  + 페이로드 12바이트

    private byte[] HandleContinuousWrite(byte[] p, ushort dt, ushort respCmd)
    {
        if (p.Length < 10) return ErrResp(respCmd, dt, 0x0001);
        ushort nameLen = BitConverter.ToUInt16(p, 8);
        if (p.Length < 10 + nameLen + 2) return ErrResp(respCmd, dt, 0x0001);
        string name = Encoding.ASCII.GetString(p, 10, nameLen);

        int rawCount = (p[10 + nameLen] | (p[10 + nameLen + 1] << 8)); // 원본 count 필드 (로그용)
        int count    = DecodeDataCount(p, 10 + nameLen);
        if (count <= 0) count = 1;
        int dataOff  = 10 + nameLen + 2;

        if (!_mem.ParseVarName(name, out var area, out int start, out _, out _))
        {
            ProtocolLog?.Invoke($"[WRITE] {name} | 주소 파싱 실패 → ErrResp 0x0002");
            return ErrResp(respCmd, dt, 0x0002);
        }

        var values = new ushort[count];
        for (int i = 0; i < count; i++)
        {
            int off = dataOff + i * 2;
            if (off + 2 <= p.Length) values[i] = BitConverter.ToUInt16(p, off);
        }
        _mem.WriteWords(area, start, values);
        _mem.Save();

        ProtocolLog?.Invoke(
            $"[WRITE] {name} | {area} word#{start} × {count}w " +
            $"(raw count={rawCount}, payload={p.Length}b, dataOff={dataOff}) | " +
            string.Join(" ", values.Select(w => $"0x{w:X4}")));

        // 페이로드 10바이트 → 총 20+10 = 30바이트
        // SPL(ECS) Response() 확인: pRxBuff[20]==0x59, pRxBuff[26-27]==0x00
        // 10바이트로 맞추면 ECS 내부 'nRecvByteCnt > 10' 체크도 안전
        var out_ = new List<byte>();
        out_.AddRange(BitConverter.GetBytes(respCmd));            // [0-1]  응답 cmd (0x59/0x65)
        out_.AddRange(BitConverter.GetBytes(dt));                 // [2-3]  dt
        out_.AddRange(new byte[2]);                               // [4-5]  예약 (에러코드=0)
        out_.AddRange(new byte[2]);                               // [6-7]  에러상태=0  ← ECS pRxBuff[26-27]
        out_.AddRange(BitConverter.GetBytes((ushort)0x0001));     // [8-9]  블록수=1
        return out_.ToArray();
    }

    // ── helpers ────────────────────────────────────────────────────────────

    private byte[] ReadVar(string name, ushort dt)
    {
        if (!_mem.ParseVarName(name, out var area, out int wordAddr, out int bitIdx, out bool isBit))
            return new byte[2];

        if (isBit || dt == 0x0000)
        {
            bool b = isBit
                ? _mem.ReadBit(area, wordAddr, bitIdx)
                : _mem.ReadWord(area, wordAddr) != 0;
            return new byte[] { (byte)(b ? 1 : 0) };
        }
        if (dt == 0x0003) // dword
        {
            uint dw = (uint)_mem.ReadWord(area, wordAddr + 1) << 16 | _mem.ReadWord(area, wordAddr);
            return BitConverter.GetBytes(dw);
        }
        return BitConverter.GetBytes(_mem.ReadWord(area, wordAddr));
    }

    private void WriteVar(string name, ushort dt, byte[] data)
    {
        if (!_mem.ParseVarName(name, out var area, out int wordAddr, out int bitIdx, out bool isBit))
            return;

        if (isBit || dt == 0x0000)
        {
            bool val = data.Length > 0 && data[0] != 0;
            if (isBit) _mem.WriteBit(area, wordAddr, bitIdx, val);
            else       _mem.WriteWord(area, wordAddr, (ushort)(val ? 1 : 0));
        }
        else if (dt == 0x0003 && data.Length >= 4)
        {
            uint dw = BitConverter.ToUInt32(data, 0);
            _mem.WriteWord(area, wordAddr,     (ushort)(dw & 0xFFFF));
            _mem.WriteWord(area, wordAddr + 1, (ushort)(dw >> 16));
        }
        else if (data.Length >= 2)
        {
            _mem.WriteWord(area, wordAddr, BitConverter.ToUInt16(data, 0));
        }
    }

    // SPL  : {lo, hi} LE uint16 byte-count → wordCount = byteCount / 2
    // LGLS : {dcLow, dcHigh} BCD digit-pair byte-count → wordCount = (dcHigh*100+dcLow) / 2
    private int DecodeDataCount(byte[] p, int offset)
    {
        if (Mode == ProtocolMode.LGLS)
            return (p[offset + 1] * 100 + p[offset]) / 2;
        // SPL
        return (p[offset] | (p[offset + 1] << 8)) / 2;
    }

    private static byte[] ErrResp(ushort respCmd, ushort dt, ushort errCode)
    {
        var out_ = new List<byte>();
        out_.AddRange(BitConverter.GetBytes(respCmd));
        out_.AddRange(BitConverter.GetBytes(dt));
        out_.AddRange(new byte[2]);
        out_.AddRange(BitConverter.GetBytes(errCode));
        out_.AddRange(new byte[2]); // block count = 0
        return out_.ToArray();
    }
}
