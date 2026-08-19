using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;
using Samoh_Lib;
using System.Data;

namespace WCS_TASK_CV
{
    // ================================================================================
    // [LGLS 2026-07-21] 물리 이관: SC/RTV 통신 스레드 (사용자 요구 구조)
    //
    //  - EQP_SIM(마스터 PLC)의 Vehicle 메모리(구 ECS observables 주소, observables.tsv)를
    //    검증된 FenetProtocol(무수정)로 읽고 써서 SC_DATA_LGLS/RTV_DATA_LGLS 를 중계한다.
    //      · 관측(READ)  : SUBSYSTEM_STATUS/LOCATION, PALLET_ON_VEHICLE, LOAD/UNLOAD_COMPLETE,
    //                      TRANSFER_ACK 등 → *_rd 컬럼 갱신 (+파생: sensor/작업번호/완료)
    //      · 지시(WRITE) : IO_TASK 가 기록한 transfer_request_od='Y' + from/to/pallet_id_od 를
    //                      Vehicle 명령 메모리(FROM/TO/PALLET_ID + TRANSFER_REQUEST 스트로브)로 반영
    //      · 핸드셰이크  : LOAD/UNLOAD_COMPLETE 이벤트는 본 스레드가 자동 Ack (통신 계층 규약,
    //                      구 ECS의 ECP FenetDriver 가 하던 역할) — 관측은 *_rd 로 남긴다.
    //      · 완료 보고   : TRANSFER_ACK 관측 시 complete_rd='1' → IO_TASK CompleteSC/CompleteRGV 소비.
    //  - kind="SC"  : VEHICLE:11~15 ↔ SC_DATA_LGLS (SC_NO 901~905)
    //    kind="RTV" : VEHICLE:1     ↔ RTV_DATA_LGLS (RTV_NO 801)
    // ================================================================================
    public class VehThread
    {
        // ---------- 관측값 주소 정의 (observables.tsv) ----------
        private class ObsDef
        {
            public char Device;     // 'M'(B) / 'D'(W) / 'R'(R)
            public int Address;     // 비트(B) 또는 워드(W/R) 주소
            public int Length = 1;  // String 워드 길이
        }

        private class VehDef
        {
            public string OwnerId;      // VEHICLE:11 ...
            public string KeyVal;       // 901~905 / 801
            public Dictionary<string, ObsDef> Obs = new Dictionary<string, ObsDef>();
            // 변화 감지 캐시
            public Dictionary<string, string> Cache = new Dictionary<string, string>();
        }

        private readonly string m_strKind;          // "SC" / "RTV"
        private readonly string m_strTable;         // SC_DATA_LGLS / RTV_DATA_LGLS
        private readonly string m_strKeyCol;        // SC_NO / RTV_NO
        private readonly string m_strWhTyp;
        private readonly string m_strIp;
        private readonly int m_nPort;
        private readonly List<VehDef> m_lstVeh = new List<VehDef>();
        private FenetProtocol m_msQPlc;
        private Thread m_thThread;
        private volatile bool m_bRun;
        private string m_strRtnMsg = "";

        public VehThread(string kind, string connectString, string whTyp, string ip, int port)
        {
            m_strKind = kind;
            m_strWhTyp = whTyp;
            m_strIp = ip;
            m_nPort = port;
            m_strTable = (kind == "SC") ? "SC_DATA_LGLS" : "RTV_DATA_LGLS";
            m_strKeyCol = (kind == "SC") ? "SC_NO" : "RTV_NO";

            m_msQPlc = new FenetProtocol(connectString);
            m_msQPlc.IsHex = true;

            LoadObservables();
        }

        public void Start()
        {
            m_bRun = true;
            m_thThread = new Thread(Thread_Doing);
            m_thThread.IsBackground = true;
            m_thThread.Name = "VehThread:" + m_strKind;
            m_thThread.Start();
        }

        public void Stop() { m_bRun = false; }

        // ---------- 관측/지시 주소 로드 ----------
        //  [LGLS 2026-08-19] 주소맵 XML(7_DeviceMap\PlcAddressMap.xml) 단일화.
        //    · 1순위 : XML 의 SC/RGV 정의(tag 속성)로 주소표를 만든다 → observables.tsv 불필요
        //    · 2순위 : XML 을 못 읽으면 종전처럼 observables.tsv 를 읽는다(폴백)
        private void LoadObservables()
        {
            if (LoadFromAddrMap()) return;      // XML 로 확정되면 tsv 는 읽지 않는다
            LoadFromTsv();
        }

        /// <summary>주소맵 XML 로 설비 주소표 구성. 성공 시 true</summary>
        private bool LoadFromAddrMap()
        {
            try
            {
                if (!cPlcAddrMap.IsLoaded) return false;
                string strTyp = (m_strKind == "SC") ? "SC" : "RGV";
                int nCnt = (m_strKind == "SC") ? 5 : 1;
                var lst = new List<VehDef>();

                for (int k = 1; k <= nCnt; k++)
                {
                    var tbl = cPlcAddrMap.TagTable(strTyp, k);
                    if (tbl == null || tbl.Count == 0) return false;   // 정의 부족 → tsv 폴백

                    var obs = new Dictionary<string, ObsDef>(StringComparer.OrdinalIgnoreCase);
                    foreach (var kv in tbl)
                        obs[kv.Key] = new ObsDef { Device = kv.Value.Device, Address = kv.Value.Address, Length = kv.Value.Length };

                    string strOwner = (m_strKind == "SC") ? ("VEHICLE:1" + k) : "VEHICLE:1";
                    string strKey   = (m_strKind == "SC") ? (900 + k).ToString() : "801";
                    lst.Add(new VehDef { OwnerId = strOwner, KeyVal = strKey, Obs = obs });
                }

                m_lstVeh.Clear();
                m_lstVeh.AddRange(lst);
                System.Diagnostics.Debug.WriteLine(string.Format(
                    "[{0}] 주소맵 XML 로 관측주소 구성 - 설비 {1}대 (observables.tsv 미사용)", m_strKind, m_lstVeh.Count));
                return true;
            }
            catch { return false; }
        }

        // ---------- observables.tsv 로드 (폴백 : XML 부재/오류 시에만) ----------
        private void LoadFromTsv()
        {
            string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "observables.tsv");
            if (!File.Exists(path)) return;
            var byOwner = new Dictionary<string, Dictionary<string, ObsDef>>(StringComparer.OrdinalIgnoreCase);
            foreach (string raw in File.ReadAllLines(path))
            {
                string line = raw.Trim('﻿', ' ', '\r');
                if (line.Length == 0) continue;
                string[] cols = line.Split('\t');
                if (cols.Length < 6) continue;
                string owner = cols[0].Trim();
                if (!owner.StartsWith("VEHICLE:", StringComparison.OrdinalIgnoreCase)) continue;

                var def = new ObsDef();
                string strAddrNo = "";      // [LGLS 2026-08-19] 원문 보관 후 디바이스 확정 뒤 변환
                foreach (string part in cols[5].Split(','))
                {
                    int eq = part.IndexOf('=');
                    if (eq <= 0) continue;
                    string key = part.Substring(0, eq).Trim().ToUpperInvariant();
                    string val = part.Substring(eq + 1).Trim();
                    if (key == "DEVICE_TYPE") def.Device = val == "B" ? 'M' : val == "W" ? 'D' : val == "R" ? 'R' : '?';
                    else if (key == "ADDRESS_NO") strAddrNo = val;
                    else if (key == "LENGTH") int.TryParse(val, out def.Length);
                }
                if (def.Device == '?') continue;

                // [LGLS 2026-08-19] ADDRESS_NO 해석
                //   B(→M)/W(→D) : 항상 16진 (구 ECS ECP.dll 과 동일, 변경 없음)
                //   R(트래킹)    : ini [PLC] R_ADDR_MODE 를 따른다 (HEX=구 ECS 호환, DEC=현행 10진)
                if (def.Device == 'R' && !cDefApp.GM_R_ADDR_HEX)
                    int.TryParse(strAddrNo, NumberStyles.Integer, CultureInfo.InvariantCulture, out def.Address);
                else
                    int.TryParse(strAddrNo, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out def.Address);
                if (!byOwner.ContainsKey(owner)) byOwner[owner] = new Dictionary<string, ObsDef>(StringComparer.OrdinalIgnoreCase);
                byOwner[owner][cols[1].Trim()] = def;
            }

            if (m_strKind == "SC")
            {
                for (int k = 1; k <= 5; k++)
                {
                    string owner = "VEHICLE:1" + k;
                    if (!byOwner.ContainsKey(owner)) continue;
                    OverrideFromAddrMap("SC", k, byOwner[owner]);   // [LGLS 2026-08-19] 주소맵 XML 우선
                    m_lstVeh.Add(new VehDef { OwnerId = owner, KeyVal = (900 + k).ToString(), Obs = byOwner[owner] });
                }
            }
            else
            {
                if (byOwner.ContainsKey("VEHICLE:1"))
                {
                    OverrideFromAddrMap("RGV", 1, byOwner["VEHICLE:1"]);   // [LGLS 2026-08-19]
                    m_lstVeh.Add(new VehDef { OwnerId = "VEHICLE:1", KeyVal = "801", Obs = byOwner["VEHICLE:1"] });
                }
            }
        }

        /// <summary>
        /// [LGLS 2026-08-19] 주소맵 XML(7_DeviceMap\PlcAddressMap.xml)에 정의된 주소로 덮어쓴다.
        ///   · XML 의 SC/RGV Signal 에 tag="LOAD_COMPLETE" 처럼 적힌 태그명으로 매칭한다.
        ///   · XML 에 없는 태그는 observables.tsv 값을 그대로 둔다(폴백).
        ///   → 이로써 SC/RGV 통신 주소도 XML 한 파일로 조정할 수 있다.
        /// </summary>
        private void OverrideFromAddrMap(string strEquipType, int nNo, Dictionary<string, ObsDef> dicObs)
        {
            try
            {
                if (!cPlcAddrMap.IsLoaded || dicObs == null) return;
                int nHit = 0;
                // 키를 복사해 순회(순회 중 값 수정 대비)
                var keys = new List<string>(dicObs.Keys);
                foreach (string strTag in keys)
                {
                    char chDev; int nWords;
                    int nAddr = cPlcAddrMap.TagAddr(strEquipType, nNo, strTag, out chDev, out nWords);
                    if (nAddr < 0) continue;
                    ObsDef def = dicObs[strTag];
                    if (def.Device != chDev) continue;         // 디바이스가 다르면 건너뜀(안전)
                    if (def.Address == nAddr) { nHit++; continue; }
                    def.Address = nAddr;
                    dicObs[strTag] = def;
                    nHit++;
                }
                if (nHit > 0)
                    System.Diagnostics.Debug.WriteLine(string.Format(
                        "[{0}#{1}] 주소맵 XML 적용 - 관측주소 {2}건 확정", strEquipType, nNo, nHit));
            }
            catch { }
        }

        // ---------- PLC 액세스 헬퍼 (CvThread 의 검증된 READ/WRITE 사용 패턴 그대로) ----------
        private bool PlcReadWords(char device, int wordAddr, int nWords, byte[] buf)
        {
            byte dev = device == 'M' ? (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M
                     : device == 'D' ? (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D
                                     : (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R;
            Array.Clear(buf, 0, buf.Length);
            return m_msQPlc.READ((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT, dev, wordAddr, nWords, ref buf);
        }

        private bool PlcWriteWords(char device, int wordAddr, int nWords, byte[] buf)
        {
            byte dev = device == 'M' ? (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_M
                     : device == 'D' ? (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_D
                                     : (byte)MelsecQ3E_UnitType_DEVICE.MELSECQ_DEVICE_CODE_R;
            return m_msQPlc.WRITE((byte)MelsecQ3E_UnitType.MELSECQ_CMD_WORD_UNIT, dev, wordAddr, nWords, buf);
        }

        private bool ReadBit(ObsDef d, ref bool value)
        {
            byte[] buf = new byte[8];
            if (!PlcReadWords(d.Device, d.Address / 16, 1, buf)) return false;
            int word = buf[0] | (buf[1] << 8);
            value = (word & (1 << (d.Address % 16))) != 0;
            return true;
        }

        private bool WriteBit(ObsDef d, bool value)
        {
            byte[] buf = new byte[8];
            if (!PlcReadWords(d.Device, d.Address / 16, 1, buf)) return false;
            int word = buf[0] | (buf[1] << 8);
            if (value) word |= (1 << (d.Address % 16));
            else word &= ~(1 << (d.Address % 16));
            byte[] tx = new byte[2];
            tx[0] = (byte)(word & 0xFF);
            tx[1] = (byte)((word >> 8) & 0xFF);
            return PlcWriteWords(d.Device, d.Address / 16, 1, tx);
        }

        private bool ReadShort(ObsDef d, ref int value)
        {
            byte[] buf = new byte[8];
            if (!PlcReadWords(d.Device, d.Address, 1, buf)) return false;
            value = buf[0] | (buf[1] << 8);
            return true;
        }

        /// <summary>워드당 2문자(하위→상위) 패킹 문자열 (EQP_SIM PlcMemory.GetString 과 동일 규격)</summary>
        // [LGLS 2026-08-05] 위치/화물 문자열 정제 : 숫자만 허용, 아니면 기본값
        private static string SanDigits(string s, string def)
        {
            if (string.IsNullOrEmpty(s)) return def;
            foreach (char c in s) if (c < '0' || c > '9') return def;
            return s;
        }

        private bool ReadString(ObsDef d, ref string value)
        {
            byte[] buf = new byte[d.Length * 2 + 8];
            if (!PlcReadWords(d.Device, d.Address, d.Length, buf)) return false;
            var sb = new StringBuilder();
            for (int i = 0; i < d.Length * 2; i++)
            {
                char c = (char)buf[i];
                if (c == '\0') break;
                sb.Append(c);
            }
            value = sb.ToString().Trim();
            return true;
        }

        private bool WriteString(ObsDef d, string value)
        {
            value = (value ?? "").PadRight(d.Length * 2, '\0');
            byte[] tx = new byte[d.Length * 2];
            for (int i = 0; i < tx.Length; i++) tx[i] = (byte)value[i];
            return PlcWriteWords(d.Device, d.Address, d.Length, tx);
        }

        // ---------- DB 헬퍼 ----------
        private void DbExec(string sql)
        {
            m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
            m_msQPlc._pBdb.mComMain.Parameters.Clear();
            if (m_msQPlc._pBdb.ExcuteNonQry(sql) < 0)
                throw new Exception("DB 오류: " + m_msQPlc._pBdb.ErrMsg);
        }

        private DataTable DbQuery(string sql)
        {
            m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
            m_msQPlc._pBdb.mComMain.Parameters.Clear();
            if (m_msQPlc._pBdb.ExcuteQry(sql) < 0)
                throw new Exception("DB 오류: " + m_msQPlc._pBdb.ErrMsg);
            return m_msQPlc._pBdb.mDtMain.Copy();
        }

        private static string Esc(string s) { return (s ?? "").Replace("'", "''"); }
        private const string CRLF = "\r\n";   // [LGLS 2026-07-22] 쿼리 다중행 표기(DBMS 복사 실행 가능하게)

        private void LogDb(string msg)
        {
            try
            {
                string strSqlLog = "";
                strSqlLog += CRLF + " INSERT INTO WCS_LOG_PGR                                    ";
                strSqlLog += CRLF + "        (WH_TYP, INS_DT, LOG_SEQ, LUGG_NO, PGR_NM, LOG_KOR) ";
                strSqlLog += CRLF + " VALUES ('" + Esc(m_strWhTyp) + "', GETDATE(), '0', '',     ";
                strSqlLog += CRLF + "         'VEH_" + m_strKind + "', '" + Esc(msg) + "')       ";
                DbExec(strSqlLog);
            }
            catch { }
        }

        // ---------- 메인 루프 ----------
        public void Thread_Doing()
        {
            while (m_bRun)
            {
                try
                {
                    if (m_msQPlc.m_bSocCon == false && m_msQPlc.m_bDBOpen == false)
                    {
                        m_msQPlc.SetConfig(m_strIp, m_nPort, 2);
                        if (!m_msQPlc.Open(ref m_strRtnMsg))
                        {
                            m_msQPlc.Close(ref m_strRtnMsg);
                            Thread.Sleep(2000);
                            continue;
                        }
                        LogDb("[VEH_" + m_strKind + "] PLC 접속 성공 (" + m_strIp + ":" + m_nPort + ", 차량 " + m_lstVeh.Count + "대)");
                    }

                    foreach (VehDef v in m_lstVeh)
                    {
                        PollObservations(v);
                        ConsumeCommands(v);
                    }
                }
                catch (Exception ex)
                {
                    LogDb("[VEH_" + m_strKind + "] 오류: " + ex.Message);
                    try { m_msQPlc.Close(ref m_strRtnMsg); } catch { }
                    Thread.Sleep(2000);
                }
                Thread.Sleep(300);
            }
        }

        private ObsDef O(VehDef v, string name)
        {
            ObsDef d;
            return v.Obs.TryGetValue(name, out d) ? d : null;
        }

        private string Cached(VehDef v, string key)
        {
            string s;
            return v.Cache.TryGetValue(key, out s) ? s : null;
        }

        // ---------- 관측 → *_rd ----------
        // [LGLS 2026-07-22] RTV 포트 → Client <Position> plc 값 (IO_TASK m_dicRgvCell 트랙→plc 매핑과 동일 근거)
        private static readonly Dictionary<int, string> m_dicRtvPosH = new Dictionary<int, string>()
        { { 1, "1" }, { 3, "3" }, { 5, "5" }, { 7, "6" }, { 9, "8" }, { 11, "9" }, { 13, "11" }, { 15, "12" },
          { 17, "13" }, { 19, "14" }, { 21, "15" }, { 23, "2" }, { 24, "2" }, { 25, "4" }, { 26, "4" },
          { 27, "7" }, { 29, "7" }, { 30, "10" }, { 31, "10" } };

        private void PollObservations(VehDef v)
        {
            int status = 0; string loc1 = "", loc2 = "", loc3 = "", pallet = "";
            string tcl1 = "", tcl2 = "", tcl3 = "";
            bool palletExist = false, loadCmp = false, unloadCmp = false, transferAck = false;

            if (!ReadShort(O(v, "SUBSYSTEM_STATUS"), ref status)) throw new Exception(v.OwnerId + " STATUS 읽기 실패");
            ReadString(O(v, "SUBSYSTEM_LOCATION_01"), ref loc1);
            ReadString(O(v, "SUBSYSTEM_LOCATION_02"), ref loc2);
            ReadString(O(v, "SUBSYSTEM_LOCATION_03"), ref loc3);
            ReadString(O(v, "PALLET_ON_VEHICLE"), ref pallet);
            ReadString(O(v, "TRANSFER_COMPLETE_LOCATION_01"), ref tcl1);
            ReadString(O(v, "TRANSFER_COMPLETE_LOCATION_02"), ref tcl2);
            ReadString(O(v, "TRANSFER_COMPLETE_LOCATION_03"), ref tcl3);
            // [LGLS 2026-08-05] PLC 미초기화 영역이 바이너리(0x3F, 0x02 등)로 읽혀
            //   Client 에 'r'/'M'/'?' 같은 깨진 글자로 표시되는 버그. 숫자 아닌 값은 버린다.
            loc1 = SanDigits(loc1, "00"); loc2 = SanDigits(loc2, "00"); loc3 = SanDigits(loc3, "00");
            tcl1 = SanDigits(tcl1, "00"); tcl2 = SanDigits(tcl2, "00"); tcl3 = SanDigits(tcl3, "00");
            pallet = SanDigits(pallet, "");
            ReadBit(O(v, "PALLET_EXIST_FLAG"), ref palletExist);
            ReadBit(O(v, "LOAD_COMPLETE"), ref loadCmp);
            ReadBit(O(v, "UNLOAD_COMPLETE"), ref unloadCmp);
            ReadBit(O(v, "TRANSFER_ACK"), ref transferAck);

            // 이벤트 자동 Ack (PLC 가 이벤트+Ack 모두 리셋 — 슬라이드5 규약)
            if (loadCmp) WriteBit(O(v, "LOAD_COMPLETE_ACK"), true);
            if (unloadCmp) WriteBit(O(v, "UNLOAD_COMPLETE_ACK"), true);
            // [LGLS 2026-07-25] Transfer Complete Ack. 근거: PPT V1.1 슬라이드22 메모리맵(Bit: Ack & Command)
            //   RGV #1 Ack base M101.0(%MX1616) + 5 = M101.5 %MX1621(=0x655). (슬라이드17 시나리오 다이어그램엔 생략됨)
            //   설비 Transfer Complete(TRANSFER_ACK 0x3B4=M059.4) 관측 시 WCS Ack 발행. observables.tsv VEHICLE:1 0655 와 일치.
            //   TRANSFER_COMPLETE_ACK 관측이 정의된 차량(RGV)만 대상 — O()가 null 이면 SC 라 스킵.
            if (transferAck)
            {
                ObsDef tca = O(v, "TRANSFER_COMPLETE_ACK");
                if (tca != null) WriteBit(tca, true);
            }

            string sen = palletExist ? "1" : "0";
            string lugg = (pallet ?? "").Trim();
            if (lugg.Length == 0) lugg = "0";

            var set = new StringBuilder();
            Action<string, string> chg = delegate(string col, string val)
            {
                string key = col;
                if ((Cached(v, key) ?? "") != val)
                {
                    set.Append(", ").Append(col).Append(" = '").Append(Esc(val)).Append("'");
                    v.Cache[key] = val;
                }
            };

            chg("SUBSYSTEM_STATUS_RD", status.ToString());
            chg("LOCATION_01_RD", loc1);
            chg("LOCATION_02_RD", loc2);
            chg("LOCATION_03_RD", loc3);
            chg("PALLET_ON_VEHICLE_RD", lugg == "0" ? "" : lugg);
            chg("TRANSFER_COMPLETE_LOCATION_01_RD", tcl1);
            chg("TRANSFER_COMPLETE_LOCATION_02_RD", tcl2);
            chg("TRANSFER_COMPLETE_LOCATION_03_RD", tcl3);
            chg("LOAD_COMPLETE_RD", loadCmp ? "1" : "0");
            chg("UNLOAD_COMPLETE_RD", unloadCmp ? "1" : "0");
            chg("TRANSFER_ACK_RD", transferAck ? "1" : "0");

            if (m_strKind == "SC")
            {
                chg("SENSOR_FK_RD", sen);
                chg("ITN_LUGG_FK1", lugg);
                // 표시용 레일 위치: 셀 이동 중이면 Bay(LOCATION_02), 포트/홈이면 0
                string posH = "0";
                int bay;
                if (int.TryParse((loc2 ?? "").Trim(), out bay) && !((loc1 ?? "").Trim() == "00" || (loc1 ?? "").Trim() == "")) posH = bay.ToString();
                chg("POS_H_RD", posH);
                chg("AUTO_MODE_RD", "1");
                chg("UCSTATUS_RD", status == 1 ? "1" : "2");
                // [LGLS] DriveSC 게이트 필드 유지 (실설비면 각자의 관측이 채우는 값들)
                chg("ONLINE_MODE_RD", "1");
                chg("ACTIVE_MODE_RD", "1");
                // [LGLS] 이중입고(54)/공출고(58): 설비 ERR_CODE_RD 관측(EQP_SIM/실PLC)에서 읽어 SC_DATA 반영. 관측 없으면 정상(0000).
                string errCode = "0000";
                ObsDef eco = O(v, "ERR_CODE_RD");
                if (eco != null) { int ec = 0; if (ReadShort(eco, ref ec) && ec != 0) errCode = ec.ToString("0000"); }
                chg("ERR_CODE_RD", errCode);
            }
            else
            {
                chg("SENSOR_RTV_RD", sen);
                chg("SENSOR_FK1_RD", sen);
                chg("WAITING_ORDER_RD", status == 1 ? "1" : "0");
                chg("AUTO_MODE_RD", "1");
                // [LGLS 2026-07-22] 표시용 레일 위치: LOCATION 포트(00/00/pp) → Client EcsDefine <Position> plc 값.
                //   (Client RtvInfo 는 RTV_DATA_LGLS.POS_H_RD 를 m_MapRtvPosition[plc]→view 로 재매핑해 그린다.
                //    구 경로에선 IO_TASK WriteCranePos 가 채웠으나 실경로는 관측 파생으로 채운다)
                int rtvPort;
                if ((loc1 ?? "").Trim() == "00" && int.TryParse((loc3 ?? "").Trim(), out rtvPort)
                    && m_dicRtvPosH.ContainsKey(rtvPort))
                    chg("POS_H_RD", m_dicRtvPosH[rtvPort]);
            }

            // 반송 완료: 하역 완료(UNLOAD_COMPLETE — 본 스레드가 Ack 하므로 반드시 1회 관측됨) 또는
            //   TRANSFER_ACK(1.5초 자동해제라 보조 신호) 관측 시 complete_rd='1'
            //   → IO_TASK CompleteSC 가 소비. (RTV 는 complete_rd 컬럼이 없고 OD_RQ_YN='N'+포크 빈 것으로 완료 판정)
            bool cmpNow = unloadCmp || transferAck;
            if (cmpNow && (Cached(v, "__cmpSeen") ?? "0") != "1")
            {
                v.Cache["__cmpSeen"] = "1";
                set.Append(", COMPLETE_RD = '1'");   // SC/RTV 공통 (RTV_DATA_LGLS 에도 컬럼 신설)
                LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 반송 완료 관측 (하역완료, 위치 "
                      + tcl1 + "/" + tcl2 + "/" + tcl3 + ")");
            }

            if (set.Length == 0) return;

            string strSqlObs = "";
            strSqlObs += CRLF + " UPDATE " + m_strTable + "                          ";
            strSqlObs += CRLF + "    SET READ_UPD_DT = GETDATE()" + set.ToString();
            strSqlObs += CRLF + "  WHERE WH_TYP        = '" + Esc(m_strWhTyp) + "'   ";
            strSqlObs += CRLF + "    AND " + m_strKeyCol + " = '" + Esc(v.KeyVal) + "' ";
            DbExec(strSqlObs);
        }

        // ---------- 지시(_od) → Vehicle 명령 메모리 ----------
        private void ConsumeCommands(VehDef v)
        {
            string strSqlCmd = "";
            strSqlCmd += CRLF + " SELECT PALLET_ID_OD, FROM_01_OD, FROM_02_OD, FROM_03_OD, ";
            strSqlCmd += CRLF + "        TO_01_OD, TO_02_OD, TO_03_OD                      ";
            strSqlCmd += CRLF + "   FROM " + m_strTable + "                                ";
            strSqlCmd += CRLF + "  WHERE WH_TYP              = '" + Esc(m_strWhTyp) + "'   ";
            strSqlCmd += CRLF + "    AND " + m_strKeyCol + "       = '" + Esc(v.KeyVal) + "' ";
            strSqlCmd += CRLF + "    AND TRANSFER_REQUEST_OD = 'Y'                         ";
            DataTable dt = DbQuery(strSqlCmd);
            if (dt.Rows.Count == 0) return;

            // [LGLS 2026-07-21] 핸드셰이크 가드: 설비가 직전 스트로브(TRANSFER_REQUEST)를 아직 내리지 않았으면
            //   이번 폴링은 보류한다. 스트로브 잔존 중에 FROM/TO 를 덮어쓰면 설비가 갱신 도중의 값을
            //   소비해 엉뚱한 목적지로 반송한다(기록 To=23 vs 설비 수신 P2 유실 사례).
            bool prevStrobe = false;
            if (!ReadBit(O(v, "TRANSFER_REQUEST"), ref prevStrobe)) return;
            if (prevStrobe) return;

            string pid = ("" + dt.Rows[0]["PALLET_ID_OD"]).Trim();
            string f1 = ("" + dt.Rows[0]["FROM_01_OD"]).Trim();
            string f2 = ("" + dt.Rows[0]["FROM_02_OD"]).Trim();
            string f3 = ("" + dt.Rows[0]["FROM_03_OD"]).Trim();
            string t1 = ("" + dt.Rows[0]["TO_01_OD"]).Trim();
            string t2 = ("" + dt.Rows[0]["TO_02_OD"]).Trim();
            string t3 = ("" + dt.Rows[0]["TO_03_OD"]).Trim();

            bool ok = WriteString(O(v, "PALLET_ID"), pid)
                   && WriteString(O(v, "FROM_01"), f1)
                   && WriteString(O(v, "FROM_02"), f2)
                   && WriteString(O(v, "FROM_03"), f3)
                   && WriteString(O(v, "TO_01"), t1)
                   && WriteString(O(v, "TO_02"), t2)
                   && WriteString(O(v, "TO_03"), t3)
                   && WriteBit(O(v, "TRANSFER_REQUEST"), true);

            if (!ok) throw new Exception(v.OwnerId + " 반송지시 기록 실패");

            // 지시 소비: OD_RQ_YN='N' → IO_TASK RunSC/RunRGV 가 '수락됨' 으로 전이. 완료 관측 대비 complete 리셋.
            string strSqlCsm = "";
            strSqlCsm += CRLF + " UPDATE " + m_strTable + "                          ";
            strSqlCsm += CRLF + "    SET TRANSFER_REQUEST_OD = 'N',                  ";
            strSqlCsm += CRLF + "        OD_RQ_YN            = 'N',                  ";
            strSqlCsm += CRLF + "        COMPLETE_RD         = '0',                  ";
            strSqlCsm += CRLF + "        WRITE_UPD_DT        = GETDATE()             ";
            strSqlCsm += CRLF + "  WHERE WH_TYP              = '" + Esc(m_strWhTyp) + "'   ";
            strSqlCsm += CRLF + "    AND " + m_strKeyCol + "       = '" + Esc(v.KeyVal) + "' ";
            DbExec(strSqlCsm);
            v.Cache["__cmpSeen"] = "0";

            LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 반송지시 기록 - JOB " + pid +
                  " From " + f1 + "/" + f2 + "/" + f3 + " To " + t1 + "/" + t2 + "/" + t3);
        }
    }
}
