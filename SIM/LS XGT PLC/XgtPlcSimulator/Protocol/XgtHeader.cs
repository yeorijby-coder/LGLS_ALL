using System.Text;

namespace XgtPlcSimulator.Protocol;

public class XgtHeader
{
    public const int SIZE = 20;

    private static readonly byte[] _companyIdBytes =
        Encoding.ASCII.GetBytes("LSIS-XGT\0\0");

    public byte[] CompanyId { get; set; } = (byte[])_companyIdBytes.Clone();
    public ushort Reserved1  { get; set; } = 0;        // byte10~11 : PLC Info
    public byte   CpuInfo    { get; set; } = 0xA0;     // byte12    : CPU Info
    public byte   SrcDest    { get; set; } = 0x11;     // byte13    : Source of Frame (PC→PLC=0x33, PLC→PC=0x11)
    public ushort InvokeId   { get; set; }             // byte14~15
    public ushort DataLength { get; set; }             // byte16~17 : 데이터부 길이
    public byte   Reserved2  { get; set; } = 0;        // byte18    : FEnet 모듈 위치
    public byte   ErrorFlag  { get; set; } = 0;        // byte19    : BCC (Serialize 시 자동 계산)

    public static XgtHeader Parse(byte[] buf, int offset = 0)
    {
        return new XgtHeader
        {
            CompanyId  = buf[(offset)..(offset + 10)],
            Reserved1  = BitConverter.ToUInt16(buf, offset + 10),
            CpuInfo    = buf[offset + 12],
            SrcDest    = buf[offset + 13],
            InvokeId   = BitConverter.ToUInt16(buf, offset + 14),
            DataLength = BitConverter.ToUInt16(buf, offset + 16),
            Reserved2  = buf[offset + 18],
            ErrorFlag  = buf[offset + 19],
        };
    }

    public byte[] Serialize()
    {
        var buf = new byte[SIZE];
        Array.Copy(CompanyId, buf, 10);
        BitConverter.GetBytes(Reserved1).CopyTo(buf, 10);
        buf[12] = CpuInfo;
        buf[13] = SrcDest;
        BitConverter.GetBytes(InvokeId).CopyTo(buf, 14);
        BitConverter.GetBytes(DataLength).CopyTo(buf, 16);
        buf[18] = Reserved2;
        // byte19 = BCC : byte0~18 합산 하위 1바이트
        int sum = 0;
        for (int i = 0; i < 19; i++) sum += buf[i];
        buf[19] = (byte)(sum & 0xFF);
        return buf;
    }

    public bool IsValid()
    {
        if (CompanyId.Length < 8) return false;
        var expected = new byte[] { (byte)'L',(byte)'S',(byte)'I',(byte)'S',(byte)'-',
                                     (byte)'X',(byte)'G',(byte)'T' };
        for (int i = 0; i < 8; i++)
            if (CompanyId[i] != expected[i]) return false;
        return true;
    }
}
