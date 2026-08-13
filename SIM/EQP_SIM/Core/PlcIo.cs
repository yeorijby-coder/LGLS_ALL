using EQP_SIM.Core;

namespace EQP_SIM.Core
{
    /// <summary>
    /// 관측값 이름 기반 타입 접근 (ECS FenetDriver 의 인코딩 규칙과 동일):
    ///  - Boolean(B) : 비트주소 = ADDRESS_NO (절대비트)
    ///  - Short(W)   : 워드주소 = ADDRESS_NO, 워드 전체 값
    ///  - String     : 워드주소 = ADDRESS_NO, LENGTH 워드, 워드당 2문자(하위→상위)
    /// </summary>
    public class PlcIo
    {
        private readonly PlcMemory mem;
        private readonly ObservableMap map;

        public PlcIo(PlcMemory mem, ObservableMap map)
        {
            this.mem = mem;
            this.map = map;
        }

        public PlcMemory Memory { get { return mem; } }
        public ObservableMap Map { get { return map; } }

        public bool GetBool(string owner, string name)
        {
            var d = map.Get(owner, name);
            return mem.GetBit(d.Device, d.Address);
        }

        public void SetBool(string owner, string name, bool value)
        {
            var d = map.Get(owner, name);
            mem.SetBit(d.Device, d.Address, value);
        }

        public ushort GetShort(string owner, string name)
        {
            var d = map.Get(owner, name);
            return mem.GetWord(d.Device, d.Address);
        }

        public void SetShort(string owner, string name, ushort value)
        {
            var d = map.Get(owner, name);
            mem.SetWord(d.Device, d.Address, value);
        }

        public string GetString(string owner, string name)
        {
            var d = map.Get(owner, name);
            return mem.GetString(d.Device, d.Address, d.Length);
        }

        public void SetString(string owner, string name, string value)
        {
            var d = map.Get(owner, name);
            mem.SetString(d.Device, d.Address, d.Length, value);
        }
    }
}
