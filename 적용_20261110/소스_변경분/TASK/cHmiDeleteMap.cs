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
    //   정의 : 영역마다 (영역 D/R/M, 주소) 두 값. 7개 = SC1~SC5 + RTV1 + CV.  비워 두면 미사용.
    //   [LGLS 2026-11-04] PLC 담당자 확정 : SC·RTV 6개 + C/V 용 1개, 모두 M 영역 한 비트씩.
    //     · SC1~SC5 / RTV1 : 운전 화면 상태창 [데이터 삭제] → 설비 통신(VehThread, CMD_RQ_ID HMIDEL)이 그 비트에 1 을 쓴다.
    //     · CV              : 리본 [수동조작]-[CV 지우기] → CvThread(CMD_RQ_ID CVCLR)가 그 비트에 1 을 쓴다.
    //                         R 트래킹은 ASCII 라 ECS 가 0 을 써도 0x30 이 남아 PLC 는 "값 있음" 으로 본다 - 그래서 PLC 가
    //                         이 비트를 보고 모든 C/V 의 R 영역을 0 으로 지운다.
    //     ECS 는 1 만 쓴다. 끄는 것은 PLC 다. D/R 로 정의되면 그 워드에 1 을 쓴다.
    //     [HMI_DELETE]
    //     SC1_AREA=D          ; D | R | M
    //     SC1_ADDR=1234       ; 문서 표기 주소 (D/R = 워드 10진, M = PLC 표기 그대로)
    //     ...  SC5_*, RTV1_*, CV_*
    //
    //   [LGLS 2026-11-05] PLC 담당자 주소 확정 : S/C#1~#5 = M6800~M6804, RGV = M6805, CONVEYOR = M6806.
    //     작업순서(S/C#1 예) : 1. ECS 가 M6800 ON → 2. PLC 가 DATA 삭제 → 3. PLC 가 M6800 OFF.
    //     M 의 _ADDR 은 담당자 표기를 그대로 적는다(XGT M 비트 표기 = 워드 10진 + 끝 한 자리 비트 0~F).
    //       M6800 → 워드 680 비트 0 → %MX10880 (= 680×16+0),  M6806 → 워드 680 비트 6 → %MX10886.
    //     종전(11/4)에는 _ADDR 을 %MX 비트 주소 10진으로 읽었다 - 담당자 표와 숫자가 달라 옮겨 적다 틀리기 쉬워 바꿨다.
    //     앞에 M 을 붙여 적어도 된다(SC1_ADDR=M6800).
    //
    //   이 클래스는 정의를 읽고 검증하고 XGT 실주소 문자열로 풀어 주기까지만 한다. 쓰기는 각 스레드가 한다.
    //   ※ 구 ECS / EQP_TASK 의 PLC 통신부는 건드리지 않는다.
    // ─────────────────────────────────────────────────────────────────────────
    public static class cHmiDeleteMap
    {
        public class Area
        {
            public string Name;     // SC1..SC5, RTV1, CV
            public string Device;   // D / R / M  (비어 있으면 미사용)
            public int    Addr;     // 통신에 쓰는 주소 : D/R = 워드, M = %MX 비트 주소(워드×16+비트)
            public string Text;     // ini 에 적힌 표기 (M 이면 PLC 표기, 예 6800)
            public bool   Used;     // 영역·주소가 모두 유효
            public string Error;    // 유효하지 않을 때 이유

            /// <summary>이력용 주소 표기. D/R = %DW워드, M = PLC 표기(%MX비트) 예) M6800(%MX10880).</summary>
            public string Xgt
            {
                get
                {
                    if (!Used) return "";
                    if (Device == "M") return "M" + Text + "(%MX" + Addr + ")";
                    return "%" + Device + "W" + Addr;
                }
            }
        }

        public static readonly string[] Names = new string[] { "SC1", "SC2", "SC3", "SC4", "SC5", "RTV1", "CV" };

        private static readonly List<Area> m_list = new List<Area>();
        public static IList<Area> Areas { get { return m_list; } }

        /// <summary>ini 를 읽어 7개 영역을 만든다. 읽기 실패해도 예외를 던지지 않는다.</summary>
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
                else if (a.Device == "M")
                {
                    // [LGLS 2026-11-05] M = PLC 표기(워드 10진 + 끝 한 자리 비트 0~F). 앞의 M 은 있어도 없어도 된다.
                    string t = strAddr.ToUpper();
                    if (t.StartsWith("M")) t = t.Substring(1);
                    int nWord = 0, nBit = -1;
                    if (t.Length >= 2) nBit = "0123456789ABCDEF".IndexOf(t[t.Length - 1]);
                    if (nBit < 0 || !int.TryParse(t.Substring(0, t.Length - 1), out nWord) || nWord < 0) { a.Error = "M 주소는 PLC 표기(워드 10진 + 비트 0~F), 예 6800"; }
                    else { a.Text = t; a.Addr = nWord * 16 + nBit; a.Used = true; }
                }
                else if (!int.TryParse(strAddr, out a.Addr) || a.Addr < 0) { a.Error = "주소는 0 이상 정수"; }
                else { a.Text = strAddr; a.Used = true; }
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
                sb.Append(' ').Append(a.Name).Append('=').Append(a.Used ? (a.Device == "M" ? a.Xgt : (a.Device + a.Addr + "(" + a.Xgt + ")")) : "-");
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
