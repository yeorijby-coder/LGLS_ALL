using System;
using System.Collections.Generic;
using System.Globalization;
using System.Xml;

namespace WCS_TASK_CV
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-08-19] PLC 주소맵 / 시나리오 외부 정의 (7_DeviceMap\PlcAddressMap.xml)
    //
    //   PPT(V1.3)의 메모리맵과 반송 시나리오를 XML 한 파일로 두고 런타임에 읽는다.
    //   → XML 만 고치면 아래가 모두 따라간다.
    //       · 자동운전 통신 : CvThread (이벤트/ACK 비트, 방향 워드, R 트래킹, 알람)
    //       · 화면          : FRM_PLC_MEMMAP (Bit/Word/Tracking 탭)
    //       · 시나리오 테스트: FRM_SCENARIO_TEST
    //
    //   ※ 안전장치 : XML 이 없거나 항목이 빠져 있으면 조회가 -1 을 돌려주고,
    //     호출부는 종전 하드코딩 계산식으로 폴백한다. 즉 파일 문제로 통신이 멎지 않는다.
    // ─────────────────────────────────────────────────────────────────────────

    /// <summary>[LGLS 2026-08-19] 설비 관측/지시 태그 1건 (observables.tsv 를 대체)</summary>
    public class VehTagInfo
    {
        public char Device;    // 'M'(비트) / 'D'(워드) / 'R'(트래킹)
        public int  Address;   // M=비트주소, D/R=워드주소
        public int  Length = 1;
    }

    /// <summary>블록 내 개별 신호</summary>
    public class AddrSignal
    {
        public string Name;
        public int    Offset;
        public int    Words = 1;
        public bool   IsArray;      // SignalArray (포지션 반복)
        public int    PerSlot = 1;
        public int    MaxSlots = 1;
        public string AppliesTo;    // "14,15" 처럼 특정 설비에만 존재
        public string Tag;          // observables.tsv 태그명 (SC/RGV 통신부 조회용)
    }

    /// <summary>설비군의 주소 블록 (base = Origin + (설비번호-NumberFrom) * Stride)</summary>
    public class AddrBlock
    {
        public string Name;
        public char   Device;        // 'M' / 'D' / 'R'
        public int    Origin;
        public int    Stride;
        public int    ReadWords = 1;
        public int    PerSlotWords = 1;
        public int    MaxSlots = 1;
        public int    Length = 1;
        public string Legacy;
        public string Desc;
        public Dictionary<string, AddrSignal> Signals = new Dictionary<string, AddrSignal>(StringComparer.OrdinalIgnoreCase);
    }

    /// <summary>설비군 (CV 15대 / SC 5대 / RGV 1대)</summary>
    public class AddrGroup
    {
        public string Type;
        public int    NumberFrom = 1;
        public int    Count;
        public Dictionary<string, AddrBlock> Blocks = new Dictionary<string, AddrBlock>(StringComparer.OrdinalIgnoreCase);
    }

    /// <summary>설비 레이아웃 (PLC 번호 ↔ 트랙 범위)</summary>
    public class EquipDef
    {
        public string Plc;
        public string Typ;
        public int    No;
        public int    FrTrack;
        public int    ToTrack;
    }

    /// <summary>시나리오 스텝</summary>
    public class ScStepDef
    {
        public string Kind;         // observe / force / word
        public char   Device;       // M / D / R
        public string Equip;        // CV / SC / RGV / Global
        public int    No;
        public string Block;
        public string Signal;
        public string NoExpr;       // 설비번호 식 : "$"(선택호기) / "$inCv" / "$outCv" / 숫자
        public int    Slot = -1;
        public bool   Value;        // observe 기대값
        public bool   IsString;
        public string Desc;
        public string Src;
        public int    RawAddr = -1; // 심볼 대신 직접 주소를 적은 경우
    }

    public class ScenarioDef
    {
        public string Id;
        public string Title;
        public bool   CraneSelectable;          // crane="select" — 실행 시 호기(1~5) 선택
        public List<ScStepDef> Steps = new List<ScStepDef>();
    }

    /// <summary>[LGLS 2026-08-19] 호기별 통로 C/V 매핑 (XML &lt;CraneMap&gt;)</summary>
    public class CraneMapDef
    {
        public int No;
        public int ScNo;
        public int InCv,  InRgvPort,  InScPort;
        public int OutCv, OutScPort,  OutRgvPort;
        public string Note;
    }

    public static class cPlcAddrMap
    {
        private static readonly object m_lock = new object();
        private static bool m_bTried = false;
        private static bool m_bLoaded = false;
        private static string m_strErr = "";
        private static string m_strPath = "";
        private static string m_strVersion = "";

        private static Dictionary<string, AddrGroup> m_dicGroup =
            new Dictionary<string, AddrGroup>(StringComparer.OrdinalIgnoreCase);
        private static Dictionary<string, int> m_dicGlobalBit =
            new Dictionary<string, int>(StringComparer.OrdinalIgnoreCase);
        private static List<EquipDef>    m_lstEquip    = new List<EquipDef>();
        private static List<ScenarioDef> m_lstScenario = new List<ScenarioDef>();
        private static List<CraneMapDef> m_lstCrane    = new List<CraneMapDef>();

        public static bool   IsLoaded  { get { EnsureLoaded(); return m_bLoaded; } }
        public static string LoadError { get { EnsureLoaded(); return m_strErr; } }
        public static string FilePath  { get { EnsureLoaded(); return m_strPath; } }
        public static string Version   { get { EnsureLoaded(); return m_strVersion; } }

        /// <summary>
        /// [LGLS 2026-08-19] R(트래킹) 문서표기 → 실주소 변환 훅.
        ///   EQP_TASK 는 기동 시 cDefApp.GsRTrackWord 를 꽂아 R_ADDR_MODE 라디오와 연동한다.
        ///   훅이 없으면(EQP_SIM 등) 구 ECS 호환 기본값인 16진 해석을 쓴다.
        /// </summary>
        public static Func<int, int> RTrackWordFn;

        private static int RWord(int nDoc)
        {
            Func<int, int> f = RTrackWordFn;
            if (f != null) return f(nDoc);
            try { return Convert.ToInt32(nDoc.ToString(CultureInfo.InvariantCulture), 16); }
            catch { return nDoc; }
        }

        /// <summary>강제 재로드 (파일을 수정한 뒤 프로그램 재시작 없이 반영)</summary>
        public static void Reload()
        {
            lock (m_lock) { m_bTried = false; }
            EnsureLoaded();
        }

        private static void EnsureLoaded()
        {
            lock (m_lock)
            {
                if (m_bTried) return;
                m_bTried = true;
                try { LoadXml(); m_bLoaded = true; m_strErr = ""; }
                catch (Exception ex) { m_bLoaded = false; m_strErr = ex.Message; }
            }
        }

        private static string FindFile()
        {
            string[] cand = new string[]
            {
                System.IO.Path.Combine(System.Windows.Forms.Application.StartupPath, @"7_DeviceMap\PlcAddressMap.xml"),
                System.IO.Path.Combine(System.Windows.Forms.Application.StartupPath, "PlcAddressMap.xml"),
                @".\7_DeviceMap\PlcAddressMap.xml",
            };
            foreach (string p in cand) { if (System.IO.File.Exists(p)) return p; }

            // [LGLS 2026-08-19] EQP_SIM 등 다른 실행폴더에서도 같은 XML 1개를 쓰도록
            //   상위 폴더를 거슬러 올라가며 TASK\WCS_TASK_CV_BIN\7_DeviceMap\PlcAddressMap.xml 을 찾는다.
            //   (주소맵/시나리오는 단일 소스여야 하므로 복사본을 만들지 않는다)
            try
            {
                System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(System.Windows.Forms.Application.StartupPath);
                for (int i = 0; i < 8 && di != null; i++, di = di.Parent)
                {
                    string p = System.IO.Path.Combine(di.FullName, @"TASK\WCS_TASK_CV_BIN\7_DeviceMap\PlcAddressMap.xml");
                    if (System.IO.File.Exists(p)) return p;
                }
            }
            catch { }
            return cand[0];
        }

        private static int Attr(XmlNode n, string name, int def)
        {
            if (n == null || n.Attributes == null) return def;
            XmlAttribute a = n.Attributes[name];
            if (a == null) return def;
            string s = a.Value.Trim();
            if (s.Length == 0) return def;
            try
            {
                if (s.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
                    return Convert.ToInt32(s.Substring(2), 16);
                return int.Parse(s, CultureInfo.InvariantCulture);
            }
            catch { return def; }
        }

        private static string AttrS(XmlNode n, string name, string def)
        {
            if (n == null || n.Attributes == null) return def;
            XmlAttribute a = n.Attributes[name];
            return (a == null) ? def : a.Value;
        }

        private static void LoadXml()
        {
            m_dicGroup.Clear(); m_dicGlobalBit.Clear();
            m_lstEquip.Clear(); m_lstScenario.Clear(); m_lstCrane.Clear();

            m_strPath = FindFile();
            if (!System.IO.File.Exists(m_strPath))
                throw new Exception("PlcAddressMap.xml 없음: " + m_strPath);

            XmlDocument doc = new XmlDocument();
            doc.Load(m_strPath);
            XmlNode root = doc.SelectSingleNode("/PlcAddressMap");
            if (root == null) throw new Exception("루트 <PlcAddressMap> 없음");
            m_strVersion = AttrS(root, "version", "");

            // ── 설비군 / 블록 / 신호
            foreach (XmlNode gn in doc.SelectNodes("/PlcAddressMap/EquipGroup"))
            {
                AddrGroup g = new AddrGroup();
                g.Type       = AttrS(gn, "type", "");
                g.NumberFrom = Attr(gn, "numberFrom", 1);
                g.Count      = Attr(gn, "count", 1);

                foreach (XmlNode bn in gn.SelectNodes("Block"))
                {
                    AddrBlock b = new AddrBlock();
                    b.Name         = AttrS(bn, "name", "");
                    string dev     = AttrS(bn, "device", "M");
                    b.Device       = (dev.Length > 0) ? dev[0] : 'M';
                    b.Origin       = Attr(bn, "origin", 0);
                    b.Stride       = Attr(bn, "stride", 0);
                    b.ReadWords    = Attr(bn, "readWords", 1);
                    b.PerSlotWords = Attr(bn, "perSlotWords", 1);
                    b.MaxSlots     = Attr(bn, "maxSlots", 1);
                    b.Length       = Attr(bn, "length", 1);
                    b.Legacy       = AttrS(bn, "legacy", "");
                    b.Desc         = AttrS(bn, "desc", "");

                    foreach (XmlNode sn in bn.SelectNodes("Signal"))
                    {
                        AddrSignal s = new AddrSignal();
                        s.Name      = AttrS(sn, "name", "");
                        s.Offset    = Attr(sn, "offset", 0);
                        s.Words     = Attr(sn, "words", 1);
                        s.AppliesTo = AttrS(sn, "appliesTo", "");
                        s.Tag       = AttrS(sn, "tag", "");
                        if (s.Name.Length > 0) b.Signals[s.Name] = s;
                    }
                    foreach (XmlNode sn in bn.SelectNodes("SignalArray"))
                    {
                        AddrSignal s = new AddrSignal();
                        s.Name     = AttrS(sn, "name", "");
                        s.Offset   = Attr(sn, "offset", 0);
                        s.IsArray  = true;
                        s.PerSlot  = Attr(sn, "perSlot", 1);
                        s.MaxSlots = Attr(sn, "maxSlots", 1);
                        if (s.Name.Length > 0) b.Signals[s.Name] = s;
                    }
                    if (b.Name.Length > 0) g.Blocks[b.Name] = b;
                }
                if (g.Type.Length > 0) m_dicGroup[g.Type] = g;
            }

            // ── 전역 단일주소 (알람 등)
            foreach (XmlNode sn in doc.SelectNodes("/PlcAddressMap/Global/Signal"))
            {
                string nm = AttrS(sn, "name", "");
                int addr  = Attr(sn, "addr", -1);
                if (nm.Length > 0 && addr >= 0) m_dicGlobalBit[nm] = addr;
            }

            // ── 설비 레이아웃
            foreach (XmlNode en in doc.SelectNodes("/PlcAddressMap/Equipments/Equip"))
            {
                EquipDef e = new EquipDef();
                e.Plc     = AttrS(en, "plc", "");
                e.Typ     = AttrS(en, "type", "CV");
                e.No      = Attr(en, "no", 0);
                e.FrTrack = Attr(en, "frTrack", 0);
                e.ToTrack = Attr(en, "toTrack", 0);
                if (e.Plc.Length > 0) m_lstEquip.Add(e);
            }

            // ── 호기별 통로 C/V 매핑
            foreach (XmlNode cn in doc.SelectNodes("/PlcAddressMap/CraneMap/Crane"))
            {
                CraneMapDef c = new CraneMapDef();
                c.No         = Attr(cn, "no", 0);
                c.ScNo       = Attr(cn, "scNo", 900 + c.No);
                c.InCv       = Attr(cn, "inCv", 0);
                c.InRgvPort  = Attr(cn, "inRgvPort", 0);
                c.InScPort   = Attr(cn, "inScPort", 0);
                c.OutCv      = Attr(cn, "outCv", 0);
                c.OutScPort  = Attr(cn, "outScPort", 0);
                c.OutRgvPort = Attr(cn, "outRgvPort", 0);
                c.Note       = AttrS(cn, "note", "");
                if (c.No > 0) m_lstCrane.Add(c);
            }

            // ── 시나리오
            foreach (XmlNode cn in doc.SelectNodes("/PlcAddressMap/Scenarios/Scenario"))
            {
                ScenarioDef sc = new ScenarioDef();
                sc.Id    = AttrS(cn, "id", "");
                sc.Title = AttrS(cn, "title", "");
                sc.CraneSelectable = (AttrS(cn, "crane", "").Trim().ToLower() == "select");
                foreach (XmlNode stn in cn.SelectNodes("Step"))
                {
                    ScStepDef st = new ScStepDef();
                    st.Kind     = AttrS(stn, "kind", "observe");
                    string dev  = AttrS(stn, "device", "M");
                    st.Device   = (dev.Length > 0) ? dev[0] : 'M';
                    st.Equip    = AttrS(stn, "equip", "CV");
                    st.NoExpr   = AttrS(stn, "no", "0");        // "$", "$inCv", "11" 등
                    st.No       = Attr(stn, "no", 0);           // 숫자면 그대로
                    st.Block    = AttrS(stn, "block", "");
                    st.Signal   = AttrS(stn, "signal", "");
                    st.Slot     = Attr(stn, "slot", -1);
                    st.Value    = (AttrS(stn, "value", "on").Trim().ToLower() == "on");
                    st.IsString = (AttrS(stn, "isString", "false").Trim().ToLower() == "true");
                    st.Desc     = AttrS(stn, "desc", "");
                    st.Src      = AttrS(stn, "src", "");
                    int raw = Attr(stn, "bit", -1);
                    if (raw < 0) raw = Attr(stn, "word", -1);
                    st.RawAddr = raw;
                    sc.Steps.Add(st);
                }
                if (sc.Steps.Count > 0) m_lstScenario.Add(sc);
            }
        }

        // ── 조회 API (실패 시 -1 → 호출부가 종전 계산식으로 폴백) ─────────────

        private static AddrBlock GetBlock(string equipType, string block)
        {
            EnsureLoaded();
            if (!m_bLoaded) return null;
            AddrGroup g;
            if (!m_dicGroup.TryGetValue(equipType, out g)) return null;
            AddrBlock b;
            if (!g.Blocks.TryGetValue(block, out b)) return null;
            return b;
        }

        /// <summary>블록 시작주소 : origin + (설비번호 - numberFrom) * stride. 실패 시 -1</summary>
        public static int BlockBase(string equipType, int no, string block)
        {
            AddrBlock b = GetBlock(equipType, block);
            if (b == null) return -1;
            AddrGroup g = m_dicGroup[equipType];
            int nBase = b.Origin + (no - g.NumberFrom) * b.Stride;
            // R(트래킹)은 문서표기를 R_ADDR_MODE 로 해석한다
            if (b.Device == 'R') nBase = RWord(nBase);
            return nBase;
        }

        /// <summary>블록 1회 읽기 워드수 (기본 1)</summary>
        public static int BlockReadWords(string equipType, string block, int def)
        {
            AddrBlock b = GetBlock(equipType, block);
            return (b == null) ? def : b.ReadWords;
        }

        /// <summary>블록 최대 슬롯수</summary>
        public static int BlockMaxSlots(string equipType, string block, int def)
        {
            AddrBlock b = GetBlock(equipType, block);
            return (b == null) ? def : b.MaxSlots;
        }

        /// <summary>신호 오프셋. 실패 시 -1</summary>
        public static int SignalOffset(string equipType, string block, string signal)
        {
            AddrBlock b = GetBlock(equipType, block);
            if (b == null) return -1;
            AddrSignal s;
            if (!b.Signals.TryGetValue(signal, out s)) return -1;
            return s.Offset;
        }

        /// <summary>
        /// 신호 절대주소. M 이면 비트주소, D/R 이면 워드주소. 실패 시 -1
        ///   slot &gt;= 0 이면 SignalArray(포지션) 또는 트래킹 슬롯을 더한다.
        /// </summary>
        public static int Addr(string equipType, int no, string block, string signal, int slot)
        {
            AddrBlock b = GetBlock(equipType, block);
            if (b == null) return -1;
            AddrSignal s;
            if (!b.Signals.TryGetValue(signal, out s)) return -1;

            AddrGroup g = m_dicGroup[equipType];
            int nBase;
            if (b.Device == 'R')
            {
                // 문서표기 기준으로 슬롯까지 더한 뒤 한 번에 변환 (구 ECS 16진 해석 대응)
                int nDoc = b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset;
                if (slot > 0) nDoc += slot * b.PerSlotWords;
                return RWord(nDoc);
            }
            nBase = b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset;
            if (slot > 0) nBase += slot * (s.IsArray ? s.PerSlot : 1);
            return nBase;
        }

        public static int Addr(string equipType, int no, string block, string signal)
        {
            return Addr(equipType, no, block, signal, -1);
        }

        /// <summary>
        /// [LGLS 2026-08-19] observables.tsv 태그명으로 주소를 조회한다 (SC/RGV 통신부용).
        ///   · XML 의 Signal 에 tag="LOAD_COMPLETE" 처럼 적어둔 값으로 찾는다.
        ///   · "FROM_02" 처럼 _NN 접미사가 붙은 태그는 base(_01) 를 찾아 (NN-1) 워드를 더한다.
        ///   · 반환 : 주소(M=비트, D/R=워드). 없으면 -1
        /// </summary>
        public static int TagAddr(string equipType, int no, string tsvTag, out char device, out int words)
        {
            device = '?'; words = 1;
            EnsureLoaded();
            if (!m_bLoaded || string.IsNullOrEmpty(tsvTag)) return -1;

            AddrGroup g;
            if (!m_dicGroup.TryGetValue(equipType, out g)) return -1;

            string strFind = tsvTag.Trim().ToUpperInvariant();
            int nExtra = 0;
            for (int pass = 0; pass < 2; pass++)
            {
                foreach (AddrBlock b in g.Blocks.Values)
                {
                    foreach (AddrSignal s in b.Signals.Values)
                    {
                        if (string.IsNullOrEmpty(s.Tag)) continue;
                        if (!string.Equals(s.Tag.Trim(), strFind, StringComparison.OrdinalIgnoreCase)) continue;
                        device = b.Device;
                        words  = s.Words;
                        if (b.Device == 'R')
                        {
                            int nDoc = b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset + nExtra;
                            return RWord(nDoc);
                        }
                        return b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset + nExtra;
                    }
                }
                // 1차 실패 : "XXX_02" → base "XXX_01" 로 재시도
                if (pass == 0)
                {
                    int u = strFind.LastIndexOf('_');
                    if (u <= 0 || u == strFind.Length - 1) break;
                    int nSeq;
                    if (!int.TryParse(strFind.Substring(u + 1), out nSeq) || nSeq < 2) break;
                    nExtra   = nSeq - 1;
                    strFind  = strFind.Substring(0, u + 1) + "01";
                }
            }
            device = '?'; words = 1;
            return -1;
        }

        /// <summary>
        /// [LGLS 2026-08-19] 설비 1대의 '태그명 → (디바이스, 주소, 길이)' 전체 표를 만든다.
        ///   observables.tsv 를 대체하기 위한 것으로, VehThread 가 이 표만으로 동작한다.
        ///   · tag 가 "FROM_01" 처럼 _NN 으로 끝나고 words&gt;1 이면 _01.._NN 으로 전개한다.
        ///     (예: tag="FROM_01" words="3" → FROM_01, FROM_02, FROM_03 각 1워드)
        ///   · 그 외 words&gt;1 은 단일 태그의 길이로 본다 (예: PALLET_ID 2워드).
        /// 반환 : key=태그명, value=VehTagInfo. XML 미로드 시 빈 표.
        /// </summary>
        public static Dictionary<string, VehTagInfo> TagTable(string equipType, int no)
        {
            var r = new Dictionary<string, VehTagInfo>(StringComparer.OrdinalIgnoreCase);
            EnsureLoaded();
            if (!m_bLoaded) return r;

            AddrGroup g;
            if (!m_dicGroup.TryGetValue(equipType, out g)) return r;

            foreach (AddrBlock b in g.Blocks.Values)
            {
                foreach (AddrSignal s in b.Signals.Values)
                {
                    if (string.IsNullOrEmpty(s.Tag)) continue;
                    if (!SignalAppliesTo(s, no)) continue;

                    int nBase;
                    if (b.Device == 'R')
                        nBase = RWord(b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset);
                    else
                        nBase = b.Origin + (no - g.NumberFrom) * b.Stride + s.Offset;

                    string strTag = s.Tag.Trim();
                    int nWords = (s.Words > 0) ? s.Words : 1;

                    // "_01" 로 끝나고 words>1 → 시리즈 전개
                    bool bSeries = false;
                    string strPrefix = "";
                    if (nWords > 1 && strTag.Length > 3)
                    {
                        int u = strTag.LastIndexOf('_');
                        int nSeq;
                        if (u > 0 && int.TryParse(strTag.Substring(u + 1), out nSeq) && nSeq == 1)
                        { bSeries = true; strPrefix = strTag.Substring(0, u + 1); }
                    }

                    if (bSeries)
                    {
                        for (int i = 0; i < nWords; i++)
                            r[strPrefix + (i + 1).ToString("00")] =
                                new VehTagInfo { Device = b.Device, Address = nBase + i, Length = 1 };
                    }
                    else
                    {
                        r[strTag] = new VehTagInfo { Device = b.Device, Address = nBase, Length = nWords };
                    }
                }
            }
            return r;
        }

        private static bool SignalAppliesTo(AddrSignal s, int no)
        {
            if (s == null || string.IsNullOrEmpty(s.AppliesTo)) return true;
            foreach (string t in s.AppliesTo.Split(','))
            {
                int v;
                if (int.TryParse(t.Trim(), out v) && v == no) return true;
            }
            return false;
        }

        /// <summary>전역 비트(알람 등). 실패 시 -1</summary>
        public static int GlobalBit(string name)
        {
            EnsureLoaded();
            if (!m_bLoaded) return -1;
            int v;
            return m_dicGlobalBit.TryGetValue(name, out v) ? v : -1;
        }

        public static List<EquipDef> Equips()
        {
            EnsureLoaded();
            return m_lstEquip;
        }

        public static List<CraneMapDef> Cranes()
        {
            EnsureLoaded();
            return m_lstCrane;
        }

        public static CraneMapDef Crane(int no)
        {
            EnsureLoaded();
            foreach (CraneMapDef c in m_lstCrane) if (c.No == no) return c;
            return null;
        }

        /// <summary>
        /// [LGLS 2026-08-19] 스텝의 설비번호 식을 선택 호기로 치환한다.
        ///   "$"      → 호기번호        "$inCv"  → 그 호기의 입고 통로 C/V
        ///   "$outCv" → 출고 통로 C/V   숫자     → 그대로
        /// </summary>
        public static int ResolveNo(string expr, int craneNo, int defVal)
        {
            if (string.IsNullOrEmpty(expr)) return defVal;
            string s = expr.Trim();
            if (s.Length > 0 && s[0] != '$')
            {
                int v;
                return int.TryParse(s, out v) ? v : defVal;
            }
            CraneMapDef c = Crane(craneNo);
            if (s == "$") return craneNo;
            if (c == null) return defVal;
            if (string.Equals(s, "$inCv",  StringComparison.OrdinalIgnoreCase)) return c.InCv;
            if (string.Equals(s, "$outCv", StringComparison.OrdinalIgnoreCase)) return c.OutCv;
            return defVal;
        }

        public static List<ScenarioDef> Scenarios()
        {
            EnsureLoaded();
            return m_lstScenario;
        }

        /// <summary>설비군의 블록 목록 (메모리맵 화면 생성용)</summary>
        public static List<AddrBlock> BlocksOf(string equipType, out int numberFrom, out int count)
        {
            EnsureLoaded();
            numberFrom = 1; count = 0;
            List<AddrBlock> r = new List<AddrBlock>();
            AddrGroup g;
            if (!m_bLoaded || !m_dicGroup.TryGetValue(equipType, out g)) return r;
            numberFrom = g.NumberFrom; count = g.Count;
            foreach (AddrBlock b in g.Blocks.Values) r.Add(b);
            return r;
        }

        /// <summary>설비군 이름 목록</summary>
        public static List<string> GroupTypes()
        {
            EnsureLoaded();
            List<string> r = new List<string>();
            if (m_bLoaded) foreach (string k in m_dicGroup.Keys) r.Add(k);
            return r;
        }

        /// <summary>신호가 해당 설비번호에 존재하는가 (appliesTo 필터)</summary>
        public static bool SignalApplies(string equipType, string block, string signal, int no)
        {
            AddrBlock b = GetBlock(equipType, block);
            if (b == null) return true;
            AddrSignal s;
            if (!b.Signals.TryGetValue(signal, out s)) return true;
            if (string.IsNullOrEmpty(s.AppliesTo)) return true;
            foreach (string t in s.AppliesTo.Split(','))
            {
                int v;
                if (int.TryParse(t.Trim(), out v) && v == no) return true;
            }
            return false;
        }

        /// <summary>상태 요약 (로그/타이틀 표시용)</summary>
        public static string StatusText()
        {
            EnsureLoaded();
            if (!m_bLoaded) return "주소맵 XML 미적용(내장 기본값 사용) - " + m_strErr;
            return "주소맵 XML v" + m_strVersion + " 적용 (" + m_dicGroup.Count + "군 / 시나리오 "
                 + m_lstScenario.Count + "개)";
        }
    }
}
