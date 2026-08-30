using System;
using System.IO;

namespace EQP_SIM.Core
{
    /// <summary>
    /// XGT PLC 메모리 (M/D/R 3개 영역, 각 64KB 바이트 배열).
    /// ECS FenetDriver 와의 정합 규칙:
    ///  - 워드 주소 A(16진) 의 데이터는 바이트 오프셋 2*A 에 리틀엔디언으로 위치
    ///  - 비트 주소 H(16진) 는 워드 H>>4 의 비트 H&0xF = 절대비트 H (바이트 H/8, 비트 H%8)
    ///  - 문자열은 워드당 2문자, char[0]=하위바이트, char[1]=상위바이트
    /// 상태는 파일(Data\plc_memory.bin)로 저장/복원한다 (DB 미사용).
    /// </summary>
    public class PlcMemory
    {
        public const int AREA_SIZE = 65536;

        private readonly byte[] m = new byte[AREA_SIZE];   // %M (B 관측값)
        private readonly byte[] d = new byte[AREA_SIZE];   // %D (W 관측값)
        private readonly byte[] r = new byte[AREA_SIZE];   // %R (R 관측값)
        private readonly object sync = new object();
        private volatile bool dirty;

        /// <summary>ECS가 쓰기를 수행했을 때 (영역문자, 바이트오프셋, 길이)</summary>
        public event Action<char, int, int> EcsWrite;

        public byte[] Area(char device)
        {
            switch (device)
            {
                case 'M': return m;
                case 'D': return d;
                case 'R': return r;
                default: return null;
            }
        }

        // ------------------------------------------------------------------
        // 프로토콜 레벨 (바이트 단위)
        // ------------------------------------------------------------------
        public byte[] ReadBytes(char device, int byteOffset, int count)
        {
            byte[] area = Area(device);
            byte[] result = new byte[count];
            if (area == null) return result;
            lock (sync)
            {
                for (int i = 0; i < count; i++)
                {
                    int idx = byteOffset + i;
                    result[i] = (idx >= 0 && idx < AREA_SIZE) ? area[idx] : (byte)0;
                }
            }
            return result;
        }

        public void WriteBytes(char device, int byteOffset, byte[] data, int count, bool fromEcs)
        {
            byte[] area = Area(device);
            if (area == null) return;
            lock (sync)
            {
                for (int i = 0; i < count; i++)
                {
                    int idx = byteOffset + i;
                    if (idx >= 0 && idx < AREA_SIZE) area[idx] = data[i];
                }
                dirty = true;
            }
            if (fromEcs)
            {
                var h = EcsWrite;
                if (h != null) h(device, byteOffset, count);
            }
        }

        /// <summary>%MX 개별 비트 쓰기 (mdDevSet/Rst)</summary>
        public void WriteBitDirect(char device, int bitAddr, bool value, bool fromEcs)
        {
            byte[] area = Area(device);
            if (area == null) return;
            int byteIdx = bitAddr / 8;
            if (byteIdx < 0 || byteIdx >= AREA_SIZE) return;
            lock (sync)
            {
                if (value) area[byteIdx] |= (byte)(1 << (bitAddr % 8));
                else area[byteIdx] &= (byte)~(1 << (bitAddr % 8));
                dirty = true;
            }
            if (fromEcs)
            {
                var h = EcsWrite;
                if (h != null) h(device, byteIdx, 1);
            }
        }

        // ------------------------------------------------------------------
        // 시나리오 엔진용 (워드/비트/문자열 단위, 주소는 관측값 ADDRESS_NO 값)
        // ------------------------------------------------------------------
        public bool GetBit(char device, int bitAddr)
        {
            byte[] area = Area(device);
            if (area == null) return false;
            int byteIdx = bitAddr / 8;
            if (byteIdx < 0 || byteIdx >= AREA_SIZE) return false;
            lock (sync) return (area[byteIdx] & (1 << (bitAddr % 8))) != 0;
        }

        public void SetBit(char device, int bitAddr, bool value)
        {
            WriteBitDirect(device, bitAddr, value, false);
        }

        public ushort GetWord(char device, int wordAddr)
        {
            byte[] area = Area(device);
            if (area == null) return 0;
            int idx = wordAddr * 2;
            if (idx < 0 || idx + 1 >= AREA_SIZE) return 0;
            lock (sync) return (ushort)(area[idx] | (area[idx + 1] << 8));
        }

        public void SetWord(char device, int wordAddr, ushort value)
        {
            byte[] area = Area(device);
            if (area == null) return;
            int idx = wordAddr * 2;
            if (idx < 0 || idx + 1 >= AREA_SIZE) return;
            lock (sync)
            {
                area[idx] = (byte)(value & 0xFF);
                area[idx + 1] = (byte)(value >> 8);
                dirty = true;
            }
        }

        /// <summary>워드당 2문자(하위→상위) 패킹 문자열 읽기. NUL/공백 trim.</summary>
        public string GetString(char device, int wordAddr, int words)
        {
            byte[] raw = ReadBytes(device, wordAddr * 2, words * 2);
            char[] chars = new char[raw.Length];
            for (int i = 0; i < raw.Length; i++) chars[i] = (char)raw[i];
            return new string(chars).Trim().Trim('\0', ' ');
        }

        public void SetString(char device, int wordAddr, int words, string value)
        {
            value = (value ?? "").PadRight(words * 2, '\0');
            byte[] raw = new byte[words * 2];
            for (int i = 0; i < raw.Length; i++) raw[i] = (byte)value[i];
            WriteBytes(device, wordAddr * 2, raw, raw.Length, false);
        }

        // ------------------------------------------------------------------
        // 파일 저장/복원
        // ------------------------------------------------------------------
        public void Save(string path)
        {
            lock (sync)
            {
                if (!dirty && File.Exists(path)) return;
                using (var fs = new FileStream(path, FileMode.Create, FileAccess.Write))
                {
                    fs.Write(m, 0, AREA_SIZE);
                    fs.Write(d, 0, AREA_SIZE);
                    fs.Write(r, 0, AREA_SIZE);
                }
                dirty = false;
            }
        }

        public bool Load(string path)
        {
            if (!File.Exists(path)) return false;
            byte[] all = File.ReadAllBytes(path);
            if (all.Length != AREA_SIZE * 3) return false;
            lock (sync)
            {
                Array.Copy(all, 0, m, 0, AREA_SIZE);
                Array.Copy(all, AREA_SIZE, d, 0, AREA_SIZE);
                Array.Copy(all, AREA_SIZE * 2, r, 0, AREA_SIZE);
                dirty = false;
            }
            return true;
        }

        public void Clear()
        {
            lock (sync)
            {
                Array.Clear(m, 0, AREA_SIZE);
                Array.Clear(d, 0, AREA_SIZE);
                Array.Clear(r, 0, AREA_SIZE);
                dirty = true;
            }
        }
    }
}
