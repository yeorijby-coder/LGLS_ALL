using System;
using System.Collections.Generic;
using System.Text;

namespace WcsCommon
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-10-15] 지상반(HMI) [삭제] 버튼 영역 정의 - ini [HMI_DELETE]
    //
    //   현장 : SC 가 비정상 오류로 멈췄을 때 화물을 출고 H/S 에 내려놓고, SC 지상반(PLC HMI 터치)의
    //          [삭제] 버튼을 누르면 SC 에 있던 작업이 지워지고 진행 중이던 작업이 완료 처리된다.
    //          PLC 담당자가 이 버튼에 대응하는 영역을 따로 열어 주기로 해서, 그 자리를 미리 ini 로 정의해 둔다.
    //
    //   정의 : 영역마다 (영역 D/R/M, 주소) 두 값. 6개 = SC1~SC5 + RTV1.  비워 두면 미사용.
    //     [HMI_DELETE]
    //     SC1_AREA=D          ; D | R | M
    //     SC1_ADDR=1234       ; 문서 표기 주소 (D/R = 워드 10진, M = 비트 주소 10진)
    //     ...  SC5_*, RTV1_*
    //
    //   이 클래스는 정의를 읽고 검증하고 XGT 실주소 문자열로 풀어 주기까지만 한다.
    //   PLC 를 읽거나 쓰는 동작은 담당자가 영역을 확정해 준 뒤(비트인지 워드인지, 누름/해제 극성 등) 붙인다.
    //   ※ 구 ECS / EQP_TASK 의 PLC 통신부는 건드리지 않는다.
    // ─────────────────────────────────────────────────────────────────────────
    public static class cHmiDeleteMap
    {
        public class Area
        {
            public string Name;     // SC1..SC5, RTV1
            public string Device;   // D / R / M  (비어 있으면 미사용)
            public int    Addr;     // 문서 표기 주소
            public bool   Used;     // 영역·주소가 모두 유효
            public string Error;    // 유효하지 않을 때 이유

            /// <summary>XGT 실주소 표기. D/R = %DW워드, M = %MX비트.</summary>
            public string Xgt
            {
                get
                {
                    if (!Used) return "";
                    if (Device == "M") return "%MX" + Addr;
                    return "%" + Device + "W" + Addr;
                }
            }
        }

        public static readonly string[] Names = new string[] { "SC1", "SC2", "SC3", "SC4", "SC5", "RTV1" };

        private static readonly List<Area> m_list = new List<Area>();
        public static IList<Area> Areas { get { return m_list; } }

        /// <summary>ini 를 읽어 6개 영역을 만든다. 읽기 실패해도 예외를 던지지 않는다.</summary>
        public static void Load(string pIni)
        {
            m_list.Clear();
            foreach (string n in Names)
            {
                Area a = new Area();
                a.Name   = n;
                a.Device = Read(pIni, n + "_AREA").ToUpper();
                string strAddr = Read(pIni, n + "_ADDR");
                if (a.Device.Length == 0 && strAddr.Length == 0) { a.Error = "미사용"; }
                else if (a.Device != "D" && a.Device != "R" && a.Device != "M") { a.Error = "영역은 D/R/M 중 하나"; }
                else if (!int.TryParse(strAddr, out a.Addr) || a.Addr < 0) { a.Error = "주소는 0 이상 정수"; }
                else a.Used = true;
                m_list.Add(a);
            }
        }

        public static Area Get(string pName)
        {
            foreach (Area a in m_list) if (a.Name == pName) return a;
            return null;
        }

        /// <summary>기동 때 한 번 남기는 한 줄 요약 로그.</summary>
        public static string Summary()
        {
            StringBuilder sb = new StringBuilder("[HMI_DEL]");
            foreach (Area a in m_list)
                sb.Append(' ').Append(a.Name).Append('=').Append(a.Used ? (a.Device + a.Addr + "(" + a.Xgt + ")") : "-");
            return sb.ToString();
        }

        private static string Read(string pIni, string pKey)
        {
            StringBuilder sb = new StringBuilder(64);
            GetPrivateProfileString("HMI_DELETE", pKey, "", sb, sb.Capacity, pIni);
            string s = sb.ToString();
            int p = s.IndexOf(';');            // 줄 끝 주석 허용
            if (p >= 0) s = s.Substring(0, p);
            return s.Trim();
        }

        [System.Runtime.InteropServices.DllImport("kernel32", CharSet = System.Runtime.InteropServices.CharSet.Auto)]
        private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder ret, int size, string file);
    }
}
