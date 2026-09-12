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

        // [LGLS 2026-08-30] 설비 에러이력(EQP_ERR_HIS) 에 남길 EQP_TYP.
        //   Client 설비에러이력 창은 EQP_ERR_HIS.EQP_TYP 로 EQP_ECD_MST 를 조인해 메시지를 표시하므로
        //   현장 크레인(SFA) 코드표 'SC_SFA' 를 써야 이중입고/공출고 문구가 뜬다.
        //   ('SC' 는 무라타 기계코드표라 같은 번호가 다른 뜻이고 0058 은 아예 없다)
        //   WCS_DB.INI [CNF] SC_ERR_TYP / RTV_ERR_TYP 로 전환.
        private static string s_strScErrTyp = "SC_SFA";
        private static string s_strRtvErrTyp = "RTV";
        public static void SetErrCodeTypes(string scTyp, string rtvTyp)
        {
            if (!string.IsNullOrEmpty(scTyp)) s_strScErrTyp = scTyp.Trim();
            if (!string.IsNullOrEmpty(rtvTyp)) s_strRtvErrTyp = rtvTyp.Trim();
        }

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

        /// <summary>[LGLS 2026-09-09] ALL_TASK 스레드 상태표시용</summary>
        public bool IsAlive { get { return (m_thThread != null && m_thThread.IsAlive); } }

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

        /// <summary>
        /// [LGLS 2026-08-30] 설비 에러이력 적재 (Client [설비에러이력] 창 = EQP_ERR_HIS).
        ///   CvThread.UpdateEQMT_ERR_LOG 의 크레인/RGV 판. EQP_NO 는 CV 와 같이 3자리로 맞춘다.
        /// </summary>
        private void InsertEqpErrHis(VehDef v, string strErrCd, string strLuggNo)
        {
            try
            {
                if (string.IsNullOrEmpty(strErrCd) || strErrCd == "0000" || strErrCd == "0") return;
                string strEqpTyp = (m_strKind == "SC") ? s_strScErrTyp : s_strRtvErrTyp;
                int nNo; string strEqpNo = int.TryParse((v.KeyVal ?? "").Trim(), out nNo)
                                           ? nNo.ToString("000") : (v.KeyVal ?? "");

                string sql = "";
                sql += CRLF + " INSERT INTO EQP_ERR_HIS (WH_TYP, EQP_TYP, EQP_NO, ERROR_DT, EQP_ERR_CD, BCR_BOTTOM, BCR_TOP, LUGG_NO) ";
                sql += CRLF + " VALUES ('" + Esc(m_strWhTyp) + "'   ";
                sql += CRLF + "       , '" + Esc(strEqpTyp) + "'    ";
                sql += CRLF + "       , '" + Esc(strEqpNo) + "'     ";
                sql += CRLF + "       , GETDATE()                   ";
                sql += CRLF + "       , '" + Esc(strErrCd) + "'     ";
                sql += CRLF + "       , null, null                  ";
                sql += CRLF + "       , '" + Esc(strLuggNo) + "' )  ";
                DbExec(sql);
                LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 설비에러 이력 적재 ("
                      + strEqpTyp + " #" + strEqpNo + " 코드 " + strErrCd + ")");
            }
            catch (Exception ex) { LogDb("[VEH_" + m_strKind + "] EQP_ERR_HIS 적재 오류: " + ex.Message); }
        }
        private const string CRLF = "\r\n";   // [LGLS 2026-07-22] 쿼리 다중행 표기(DBMS 복사 실행 가능하게)

        private void LogDb(string msg)
        {
            // [LGLS 2026-09-09] ALL_TASK 공용 로그 싱크(스레드별 파일 + DB)
            try { WcsCommon.cTaskLog.Write("EQP", "EQP_VEH_" + m_strKind, "INFO", msg); } catch { }

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

        // ---------- 주소표 재구성 요청 ----------
        // [LGLS 2026-08-21] R 주소모드(XML rAddrMode / 라디오) 전환 시 SYS_MAIN 이 호출.
        //   주소표는 생성 시 1회 확정되므로, 다음 사이클 진입 시점에 스레드가 직접 재구성한다.
        private volatile bool m_bReloadReq = false;
        public void RequestReloadObservables() { m_bReloadReq = true; }

        // ---------- 메인 루프 ----------
        // [LGLS 2026-08-22] 통신 실패/무응답 감시 — 설비 재기동 후 관측이 되살아나지 못하던 문제.
        private int m_nComFailCycle = 0;
        private const int COM_FAIL_LIMIT = 3;
        private DateTime m_dtLastPollOk = DateTime.Now;
        private const int POLL_STALL_MS = 30000;

        public void Thread_Doing()
        {
            while (m_bRun)
            {
                try
                {
                    if (m_bReloadReq)
                    {
                        m_bReloadReq = false;
                        LoadObservables();
                        LogDb("[VEH_" + m_strKind + "] 주소표 재구성 (R 주소모드 전환, 차량 " + m_lstVeh.Count + "대)");
                    }
                    // [LGLS 2026-08-22] 종전에는 두 플래그가 **모두** 내려갔을 때만 재접속했다.
                    //   설비 재기동으로 상대가 세션만 리셋하면 한쪽만 내려가 이 블록을 건너뛰고,
                    //   관측이 조용히 멈춘 채 되살아나지 못했다(RTV_DATA_LGLS READ_UPD_DT 가 멈춤).
                    if (m_msQPlc.m_bSocCon == false || m_msQPlc.m_bDBOpen == false)
                    {
                        try { m_msQPlc.Close(ref m_strRtnMsg); } catch { }
                        m_msQPlc.SetConfig(m_strIp, m_nPort, 2);
                        if (!m_msQPlc.Open(ref m_strRtnMsg))
                        {
                            m_msQPlc.Close(ref m_strRtnMsg);
                            UpdateEqpMstConn("N");
                            Thread.Sleep(2000);
                            continue;
                        }
                        m_nComFailCycle = 0;
                        m_dtLastPollOk  = DateTime.Now;
                        // [LGLS 2026-09-11] (재)접속했으면 Ack 캐시를 버린다.
                        //   끊겨 있는 동안 PLC 쪽 Ack 비트가 어떻게 됐는지 알 수 없으므로
                        //   다음 주기에 AckFollow 가 다시 맞추게 한다.
                        foreach (VehDef vc in m_lstVeh)
                        {
                            vc.Cache.Remove("__lcAck");
                            vc.Cache.Remove("__ucAck");
                        }
                        LogDb("[VEH_" + m_strKind + "] PLC 접속 성공 (" + m_strIp + ":" + m_nPort + ", 차량 " + m_lstVeh.Count + "대)");
                        UpdateEqpMstConn("Y");
                    }

                    // [LGLS 2026-08-22] 차량 한 대의 관측 실패가 스레드 전체를 멈추지 않게 개별로 감싼다.
                    //   (종전에는 PollObservations 의 예외가 바깥 catch 까지 올라가 나머지 차량의
                    //    지시 소비(ConsumeCommands)까지 통째로 건너뛰었다)
                    bool bAnyOk = false;
                    foreach (VehDef v in m_lstVeh)
                    {
                        try { PollObservations(v); bAnyOk = true; }
                        catch (Exception exv) { LogDb("[VEH_" + m_strKind + "] 관측 실패: " + exv.Message); }
                        try { ConsumeOperatorCommands(v); } catch (Exception exCmd) { LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 운전 명령 처리 실패: " + exCmd.Message); }
                        try { ConsumeCommands(v); }
                        catch (Exception exc) { LogDb("[VEH_" + m_strKind + "] 지시 소비 실패: " + exc.Message + " @ " + (exc.StackTrace ?? "").Replace("\r\n", " | ")); }
                    }

                    // [LGLS 2026-08-22] 통신 연속 실패 감시 (CvThread 와 같은 규칙).
                    //   세션이 죽어도 소켓 플래그가 살아 있는 경우가 있어, 실패 횟수로 판단해 강제 재접속한다.
                    if (bAnyOk) { m_nComFailCycle = 0; m_dtLastPollOk = DateTime.Now; }
                    else if (++m_nComFailCycle >= COM_FAIL_LIMIT)
                    {
                        LogDb("[VEH_" + m_strKind + "] 통신 연속 실패 " + m_nComFailCycle + "회 - 소켓 재접속");
                        try { m_msQPlc.Close(ref m_strRtnMsg); } catch { }
                        m_nComFailCycle = 0;
                    }
                    // 관측이 한동안 한 번도 갱신되지 않으면(무응답인데 실패도 아닌 상태) 강제로 끊고 다시 붙는다.
                    else if ((DateTime.Now - m_dtLastPollOk).TotalMilliseconds > POLL_STALL_MS)
                    {
                        LogDb("[VEH_" + m_strKind + "] 관측 무갱신 " + (POLL_STALL_MS / 1000) + "초 - 소켓 재접속");
                        try { m_msQPlc.Close(ref m_strRtnMsg); } catch { }
                        m_dtLastPollOk = DateTime.Now;
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

        /// <summary>
        /// [LGLS 2026-09-05] 이벤트 비트를 따라 Ack 비트를 올리고 내린다(사양 슬라이드 : ON → Ack ON → OFF → Ack OFF).
        ///   상태가 바뀔 때만 쓴다(매 주기 PLC 쓰기 방지).
        /// </summary>
        private void AckFollow(VehDef v, string strAckTag, string strCacheKey, bool bEventOn)
        {
            AckFollow(v, strAckTag, strCacheKey, bEventOn, -1, -1);
        }

        // [LGLS 2026-09-11 밤] ★캐시가 아니라 PLC 의 실제 Ack 비트를 기준으로 맞춘다★
        //   종전 : 우리가 마지막에 쓴 값(캐시)과 같으면 아무것도 안 했다. 그런데 이 비트가 담긴 워드
        //   (S/C#1 이면 M1536~1551)는 우리 말고도 쓰는 쪽이 있다 - CvThread.CvAlarmCheck 가
        //   "공통 알람 Ack"(M1539/1540) 를 쓰는데 그 자리가 S/C#1 Ack 블록 안이다. 비트 쓰기는
        //   양쪽 다 [워드 읽기 → 비트 수정 → 워드 쓰기] 라, 한쪽이 낡은 워드를 되쓰면 상대 비트가
        //   지워진다. 지워진 Ack 를 캐시는 "이미 올렸다" 고 믿어 영영 다시 쓰지 않았다.
        //   (현장 2026-09-11 : S/C#1 만 "Unload Complete Ack 가 안 왔다" - S/C#2~5 의 Ack 워드는
        //    CvThread 가 건드리지 않는다)
        //   nWordAddr/nWord : 호출부가 이번 주기에 읽어 둔 Ack 워드(같은 워드의 두 Ack 를 한 번에).
        //   읽기에 실패했으면(-1) 종전 캐시 동작으로 물러난다 - 통신 장애 중 쓰기 폭주를 막는다.
        private void AckFollow(VehDef v, string strAckTag, string strCacheKey, bool bEventOn, int nWordAddr, int nWord)
        {
            ObsDef d = O(v, strAckTag);
            if (d == null) return;
            string strNow = bEventOn ? "1" : "0";
            string strCached = Cached(v, strCacheKey);

            bool bKnown = (nWord >= 0 && nWordAddr == d.Address / 16);
            if (!bKnown)
            {
                // 종전 동작 (기동 직후/재접속 후 첫 주기는 반드시 한 번 쓴다)
                if (strCached != null && strCached == strNow) return;
                if (WriteBit(d, bEventOn)) v.Cache[strCacheKey] = strNow;
                return;
            }

            bool bAckNow = ((nWord >> (d.Address % 16)) & 1) != 0;
            if (bAckNow == bEventOn)
            {
                v.Cache[strCacheKey] = strNow;          // PLC 값이 이미 원하는 값이다
                v.Cache.Remove(strCacheKey + "_x");
                return;
            }

            string strTag = "[VEH_" + m_strKind + "] " + v.OwnerId + " " + strAckTag + " (M" + d.Address + ")";
            if (bEventOn)
            {
                // 이벤트 ON 인데 Ack 가 0 : 처음 올리는 것이거나, 올려 둔 것이 되돌아간 것이다.
                bool bReassert = (strCached == "1");
                if (bReassert)
                {
                    // 재기록은 1초에 한 번만 - PLC 가 스스로 내리는 규약이어도 맞서 폭주하지 않는다
                    string strLast = Cached(v, strCacheKey + "_t");
                    int nLast;
                    if (strLast != null && int.TryParse(strLast, out nLast)
                        && unchecked(Environment.TickCount - nLast) < 1000) return;
                }
                bool bOk = WriteBit(d, true);
                if (bOk) v.Cache[strCacheKey] = "1";
                v.Cache[strCacheKey + "_t"] = Environment.TickCount.ToString();
                LogDb(strTag + (bReassert
                    ? " ON 재기록 - 올려 둔 비트가 PLC 에서 되돌아가 있었음(외부 덮어쓰기 의심)"
                    : " ON 기록") + (bOk ? "" : " - 쓰기 실패"));
            }
            else
            {
                // 이벤트 OFF 인데 Ack 가 1
                if (strCached == "1" || strCached == null)
                {
                    // 우리가 올린 것이거나(캐시 "1"), 기동/재접속 직후 처음 보는 것(캐시 없음)이다.
                    //   후자는 이전 실행이 남긴 잔재다 - 그대로 두면 다음 완료 이벤트를 설비가
                    //   "이미 Ack 됨" 으로 보고 즉시 내려 WCS 가 관측하지 못한다(09-05 유실 형태).
                    //   규약(설비 Report OFF → WCS Ack OFF)대로 내린다.
                    bool bOk = WriteBit(d, false);
                    if (bOk) v.Cache[strCacheKey] = "0";
                    LogDb(strTag + (strCached == null ? " OFF 기록 - 기동 직후 정합(이전 실행의 Ack 잔재)" : " OFF 기록")
                          + (bOk ? "" : " - 쓰기 실패"));
                }
                else if (Cached(v, strCacheKey + "_x") != "1")
                {
                    // 운영 중(캐시 "0")에 우리가 올린 적 없는 Ack 가 켜졌다(수동 조작 등). 되돌리지 않고 한 번만 알린다.
                    v.Cache[strCacheKey + "_x"] = "1";
                    LogDb(strTag + " 이벤트 OFF 인데 Ack ON - 우리가 올린 것이 아니라 손대지 않음");
                }
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

        // [LGLS 2026-09-08] 운전 화면의 설비 접속정보(EQP_MST)에 실제 접속 상태/주소를 남긴다.
        //   종전에는 SC/RTV 가 EQP_MST 를 한 번도 건드리지 않아 화면에는 늘 "미접속(N)" 이었고
        //   IP 도 옛 값 그대로였다. 접속 규약은 손대지 않고 상태만 적는다.
        private void UpdateEqpMstConn(string strYn)
        {
            try
            {
                string strTyp = (m_strKind == "SC") ? "SC" : "RTV";
                string sql = "";
                sql += CRLF + " UPDATE EQP_MST                                  ";
                sql += CRLF + "    SET CONNECTED_YN  = :YN                      ";
                sql += CRLF + "      , PLC_IP        = :IP                      ";
                sql += CRLF + "      , PLC_PORT      = :PORT                    ";
                sql += CRLF + "      , PLC_PORT_FROM = :PORT                    ";
                sql += CRLF + "      , UPD_DT        = " + DbLang.SYSDATE + "   ";
                sql += CRLF + "  WHERE WH_TYP        = :WH                      ";
                sql += CRLF + "    AND EQP_TYP       = :TYP                     ";
                m_msQPlc._pBdb.mComMain.CommandType = CommandType.Text;
                m_msQPlc._pBdb.mComMain.Parameters.Clear();
                m_msQPlc._pBdb.mComMain.Parameters.Add("YN",   DbLang.VARCHAR, 255).Value = strYn;
                m_msQPlc._pBdb.mComMain.Parameters.Add("IP",   DbLang.VARCHAR, 255).Value = m_strIp;
                m_msQPlc._pBdb.mComMain.Parameters.Add("PORT", DbLang.VARCHAR, 255).Value = Convert.ToString(m_nPort);
                m_msQPlc._pBdb.mComMain.Parameters.Add("WH",   DbLang.VARCHAR, 255).Value = m_strWhTyp;
                m_msQPlc._pBdb.mComMain.Parameters.Add("TYP",  DbLang.VARCHAR, 255).Value = strTyp;
                m_msQPlc._pBdb.ExcuteNonQry(sql);
            }
            catch (Exception ex) { LogDb("[VEH_" + m_strKind + "] EQP_MST 접속정보 기록 오류: " + ex.Message); }
        }

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
            // [LGLS 2026-09-05] ★핸드셰이크는 양방향이다★ : 이벤트 ON → Ack ON, 이벤트 OFF → Ack OFF.
            //   종전에는 Ack 를 올리기만 하고 내리지 않아 그 비트가 계속 ON 으로 남았다.
            //   그러면 설비는 다음 완료 이벤트를 올리자마자 "이미 Ack 됨" 으로 보고 즉시 내려버려,
            //   WCS 가 그 이벤트를 한 번도 관측하지 못한다 → 완료신호 유실.
            //   (실측 2026-09-05 : 작업 4722 가 크레인 유휴·포크 빔 상태로 25 에 10분 이상 정체.
            //    S/C·RGV 전 호기의 LOAD/UNLOAD_COMPLETE_ACK 가 ON 으로 굳어 있었다.)
            //   CV(CvEventCheck)는 이미 해제까지 하고 있었고, 차량(S/C·RGV)만 빠져 있었다.
            // [LGLS 2026-09-11 밤] Ack 워드(두 Ack 가 같은 워드)를 한 번 읽어 실제 값 기준으로 맞춘다.
            int nAckWordAddr = -1, nAckWord = -1;
            {
                ObsDef dAck = O(v, "LOAD_COMPLETE_ACK");
                if (dAck != null)
                {
                    byte[] bufAck = new byte[8];
                    if (PlcReadWords(dAck.Device, dAck.Address / 16, 1, bufAck))
                    {
                        nAckWordAddr = dAck.Address / 16;
                        nAckWord     = bufAck[0] | (bufAck[1] << 8);
                    }
                }
            }
            AckFollow(v, "LOAD_COMPLETE_ACK",   "__lcAck", loadCmp,   nAckWordAddr, nAckWord);
            AckFollow(v, "UNLOAD_COMPLETE_ACK", "__ucAck", unloadCmp, nAckWordAddr, nAckWord);
            // [LGLS 2026-07-25] Transfer Complete Ack. 근거: PPT V1.1 슬라이드22 메모리맵(Bit: Ack & Command)
            //   RGV #1 Ack base M101.0(%MX1616) + 5 = M101.5 %MX1621(=0x655). (슬라이드17 시나리오 다이어그램엔 생략됨)
            //   설비 Transfer Complete(TRANSFER_ACK 0x3B4=M059.4) 관측 시 WCS Ack 발행. observables.tsv VEHICLE:1 0655 와 일치.
            //   TRANSFER_COMPLETE_ACK 관측이 정의된 차량(RGV)만 대상 — O()가 null 이면 SC 라 스킵.
            if (transferAck)
            {
                ObsDef tca = O(v, "TRANSFER_COMPLETE_ACK");
                if (tca != null) WriteBit(tca, true);
            }

            // ── [LGLS 2026-08-21] 설비 알람 보고 관측 + Ack 핸드셰이크 ─────────────────
            //   구 ECS 는 S/C·RGV 각 호기의 ALARM_SET/RESET_REPORT 를 개별 관측했다(TB_OBSERVABLE).
            //   종전 신 WCS 는 CvAlarmCheck 가 Global 단일 비트(786/787 = 실은 S/C#1 전용)만 봐서
            //   S/C#2~#5·RGV 알람이 유실됐다 → 호기별 관측으로 복원.
            //   규약(슬라이드5): 설비 Report ON → WCS Ack ON → 설비 Report OFF → WCS Ack OFF.
            ObsDef oSetRep   = O(v, "ALARM_SET_REPORT");
            ObsDef oResetRep = O(v, "ALARM_RESET_REPORT");
            ObsDef oSetAck   = O(v, "ALARM_SET_REPORT_ACK");
            ObsDef oResetAck = O(v, "ALARM_RESET_REPORT_ACK");
            if (oSetRep != null && oSetAck != null)
            {
                bool almSet = false;
                ReadBit(oSetRep, ref almSet);
                bool prevSet = (Cached(v, "ALM_SET_ACKED") == "1");
                if (almSet && !prevSet)
                {
                    int code = 0;
                    ObsDef oc = O(v, "ALARM_SET_CODE");
                    if (oc != null) ReadShort(oc, ref code);
                    WriteBit(oSetAck, true);
                    v.Cache["ALM_SET_ACKED"] = "1";
                    // [LGLS 2026-09-01] RTV 는 SC 와 달리 별도 ERR_CODE_RD 워드가 없다(원문서 미정의).
                    //   알람 발생 보고의 코드(D0211)를 설비 에러코드로 삼아 아래 DB 반영부에서
                    //   ERR_CODE_RD + HOST_ERR_SEND_YN='N' 으로 기록 → HOST_TASK 가 E 전문으로 올린다.
                    if (m_strKind != "SC" && code != 0) v.Cache["__almErrCode"] = code.ToString("0000");
                    System.Diagnostics.Debug.WriteLine(v.OwnerId + " 알람 발생 보고 감지 (code=" + code.ToString("0000") + ") → Ack ON");
                }
                else if (!almSet && prevSet)
                {
                    WriteBit(oSetAck, false);
                    v.Cache["ALM_SET_ACKED"] = "0";
                }
            }
            if (oResetRep != null && oResetAck != null)
            {
                bool almReset = false;
                ReadBit(oResetRep, ref almReset);
                bool prevReset = (Cached(v, "ALM_RESET_ACKED") == "1");
                if (almReset && !prevReset)
                {
                    int code = 0;
                    ObsDef oc = O(v, "ALARM_RESET_CODE");
                    if (oc != null) ReadShort(oc, ref code);
                    WriteBit(oResetAck, true);
                    v.Cache["ALM_RESET_ACKED"] = "1";
                    // [LGLS 2026-09-01] 알람 해제 → RTV 에러코드 정상(0000) 복귀 (E 재보고는 하지 않는다)
                    if (m_strKind != "SC") v.Cache["__almErrClr"] = "1";
                    System.Diagnostics.Debug.WriteLine(v.OwnerId + " 알람 해제 보고 감지 (code=" + code.ToString("0000") + ") → Ack ON");
                }
                else if (!almReset && prevReset)
                {
                    WriteBit(oResetAck, false);
                    v.Cache["ALM_RESET_ACKED"] = "0";
                }
            }

            string sen = palletExist ? "1" : "0";
            string lugg = (pallet ?? "").Trim();
            if (lugg.Length == 0) lugg = "0";
            // [LGLS 2026-08-22] "화물감지는 섰는데 작업번호가 없는" 표시 방지.
            //   ID(문자열)와 감지(비트)는 서로 다른 READ 로 올라오므로 상차 직후 한 주기 어긋날 수 있다.
            //   그 순간 ID 를 0 으로 덮어쓰면 화면의 크레인에서 작업번호가 사라진다 → 직전 값을 유지한다.
            if (sen == "1" && lugg == "0")
            {
                string prevLugg = Cached(v, "ITN_LUGG_FK1");
                if (!string.IsNullOrEmpty(prevLugg) && prevLugg != "0") lugg = prevLugg;
            }

            var set = new StringBuilder();
            bool bNewErr = false;              // [LGLS 2026-08-30] 이번 폴링에서 새 설비에러가 관측됨(에러보고 E 대상)
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
                // 표시용 레일 위치 : Bay(LOCATION_02) 를 그대로 쓴다.
                // [LGLS 2026-09-08] ★뱅크(LOCATION_01) 게이트를 뺐다★ (구 ECS 기준, 사용자 실측)
                //   종전에는 뱅크가 "00"/빈값이면 위치를 0 으로 눌렀다. 그런데 크레인이 통로를
                //   달리는 동안 설비는 뱅크를 00 으로 두고 베이만 올려 준다. 그래서 화면의
                //   크레인이 제자리에 멈춰 있다가 셀에 닿아 포킹할 때(뱅크가 채워질 때) 한 번에
                //   뛰었다. 구 ECS 는 게이트 없이 베이만 썼다 :
                //     MonitorAllEquipmentPanel01.RefreshStockerVehicle
                //       if (int.TryParse(vehicle.VehicleLocation02, out column) == false) column = 0;
                //       stackerWidget.Top = stackerTop - (int)(column * 9.5);
                //   같은 규약으로 되돌린다. (표시 전용 파생값 - 통신 규약은 건드리지 않는다)
                string posH = "0";
                int bay;
                if (int.TryParse((loc2 ?? "").Trim(), out bay)) posH = bay.ToString();
                chg("POS_H_RD", posH);

                // ── [LGLS 2026-08-21] 설비 실상태를 상위 상태보고(S)에 반영 ────────────────
                //   구 ECS 상태 체계(Vehicle.cs VEHICLE_STATE) : DOWN=0, IDLE=1, RUN=2
                //   종전에는 AUTO/ONLINE/ACTIVE 를 "1" 상수로, UCSTATUS 를 1/2 로만 써서
                //   크레인을 사용정지해도 HOST_TASK 가 항상 '가능(0)' 으로 보고했다.
                //   HOST_TASK 의 불가 판정 : ONLINE/AUTO/ACTIVE != "1" 또는 UCSTATUS=="4"(에러)
                bool bDown = (status == 0);                       // DOWN = 사용정지/이상
                //   ※ OPERATION_MODE(운전모드) 비트는 실 PLC 에서 채워지는지 확인되지 않았고
                //     시뮬레이터는 채우지 않아 0 으로 읽힌다 → 오판 방지를 위해 지금은 쓰지 않는다.
                //     DOWN 판정만으로 사용정지를 반영한다(실 PLC 확인 후 확장).
                chg("AUTO_MODE_RD", bDown ? "0" : "1");
                chg("UCSTATUS_RD", bDown ? "4" : (status == 1 ? "1" : "2"));   // 4 = 에러/사용정지
                chg("ONLINE_MODE_RD", bDown ? "0" : "1");
                chg("ACTIVE_MODE_RD", bDown ? "0" : "1");
                // [LGLS] 이중입고(54)/공출고(58): 설비 ERR_CODE_RD 관측(EQP_SIM/실PLC)에서 읽어 SC_DATA 반영. 관측 없으면 정상(0000).
                string errCode = "0000";
                ObsDef eco = O(v, "ERR_CODE_RD");
                if (eco != null) { int ec = 0; if (ReadShort(eco, ref ec) && ec != 0) errCode = ec.ToString("0000"); }
                // [LGLS 2026-08-30] 에러가 '새로' 올라온 순간에만 에러보고(E) 플래그를 내린다.
                //   HOST_TASK CCliWork.IsEquip_ERROR_Modified 는 HOST_ERR_SEND_YN='N' 인 건만 E 전문으로
                //   올린다. 종전에는 ERR_CODE_RD 가 바뀌어도 상태보고(S) 플래그(HOST_SEND_YN)만 내려서,
                //   이중입고(54)/공출고(58)가 DB 까지 와도 상위로 보고되지 않아 재지정 절차가 시작되지 않았다.
                bNewErr = (errCode != "0000") && ((Cached(v, "ERR_CODE_RD") ?? "") != errCode);
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

                // [LGLS 2026-09-01] RTV 에러도 에러보고(E) 대상 — 알람 핸드셰이크에서 캐시한 코드를 반영.
                //   (알람 블록은 chg 정의보다 앞서 돌므로 캐시로 넘겨받는다)
                // [LGLS 2026-09-11] ★RTV 에러코드는 보고 펄스가 아니라 워드 상태로 본다★
                //   현장 보고 : RTV 에러를 현장에서 해제했는데 WCS 에는 에러가 그대로 남아 있었다.
                //   원인 : 종전에는 ERR_CODE_RD 를 알람 보고 비트의 모서리로만 움직였다.
                //     발생 = (almSet && !prevSet), 해제 = (almReset && !prevReset)
                //   두 보고는 펄스인데 15설비 순회가 약 16초다. 해제 보고가 그 사이에 스쳐
                //   지나가면 관측 자체를 못 해 에러코드가 영영 안 내려간다.
                //   (SC 는 ERR_CODE_RD 워드를 매 주기 상태로 읽어 스스로 풀린다 - RTV 만 이 구멍)
                //   구 ECS 는 상태로 본다(판단 기준) :
                //     Vehicle.OnAlarmSetCode(short value) { this.AlarmSetCode = value; }
                //       → ALARM_SET_CODE 워드가 0 으로 내려가면 그대로 정상이 된다
                //     ECSDispatcher.CheckEquipmentStatus : if (vehicle.AlarmSetCode != 0) Available = "5"
                //   같은 규약으로 되돌린다. 모서리 분기는 Ack 핸드셰이크와 E 전문 판정용으로 남긴다.
                int nAlmCodeNow = 0;
                ObsDef oAlmCode = O(v, "ALARM_SET_CODE");
                bool bAlmCodeObs = (oAlmCode != null && ReadShort(oAlmCode, ref nAlmCodeNow));

                string strAlmErr;
                if (bAlmCodeObs)
                {
                    // 워드가 관측되면 그것이 진실이다. 모서리 캐시는 소비만 하고 버린다.
                    v.Cache.Remove("__almErrCode");
                    v.Cache.Remove("__almErrClr");
                    string strErrNow = (nAlmCodeNow != 0) ? nAlmCodeNow.ToString("0000") : "0000";
                    string strErrPrev = (Cached(v, "ERR_CODE_RD") ?? "");
                    bNewErr = (strErrNow != "0000") && (strErrPrev != strErrNow);
                    if (strErrPrev != strErrNow && strErrNow == "0000" && strErrPrev.Length > 0
                        && strErrPrev != "0000")
                        LogDb("[VEH_" + m_strKind + "] " + v.OwnerId
                            + " 알람코드 워드가 0 → 에러 해제 반영 (종전 [" + strErrPrev + "])");
                    chg("ERR_CODE_RD", strErrNow);
                }
                else if (v.Cache.TryGetValue("__almErrCode", out strAlmErr))
                {
                    // 워드가 관측에 없는 구성 - 종전 모서리 동작 (퇴행 방지)
                    v.Cache.Remove("__almErrCode");
                    bNewErr = ((Cached(v, "ERR_CODE_RD") ?? "") != strAlmErr);
                    chg("ERR_CODE_RD", strAlmErr);
                }
                else if (v.Cache.ContainsKey("__almErrClr"))
                {
                    v.Cache.Remove("__almErrClr");
                    chg("ERR_CODE_RD", "0000");
                }
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

            // [LGLS 2026-08-21] 상위 상태보고(S) 대상 항목이 바뀌면 HOST_SEND_YN='N' 으로 내려
            //   HOST_TASK 가 30초 주기를 기다리지 않고 즉시 보고하게 한다.
            //   (종전에는 VehThread 가 이 플래그를 전혀 건드리지 않아 크레인 사용정지 같은
            //    상태 변화가 최대 30초 늦게 보고됐다)
            //   HOST 가 보는 항목 : 가용판정(UCSTATUS/AUTO_MODE/ONLINE_MODE/ACTIVE_MODE) + 적재화물(ITN_LUGG_FK1)
            string strChanged = set.ToString();
            if (m_strKind == "SC" &&
                (strChanged.Contains("UCSTATUS_RD")   || strChanged.Contains("AUTO_MODE_RD") ||
                 strChanged.Contains("ONLINE_MODE_RD")|| strChanged.Contains("ACTIVE_MODE_RD") ||
                 strChanged.Contains("ITN_LUGG_FK1")  || strChanged.Contains("ERR_CODE_RD")))
            {
                set.Append(", HOST_SEND_YN = 'N'");
            }
            // [LGLS 2026-08-30] 새 설비에러는 에러보고(E) 로도 올린다 — 이중입고(54)/공출고(58) 재지정 절차의 출발점.
            // [LGLS 2026-09-01] RTV 도 대상 (RTV_DATA_LGLS 에 HOST_ERR_SEND_YN 신설, DeviceClass=3)
            if (bNewErr)
            {
                set.Append(", HOST_ERR_SEND_YN = 'N'");
            }
            // [LGLS 2026-08-30] 설비 에러이력 적재 — 종전에는 CvThread(컨베이어)만 남기고
            //   크레인/RGV 는 남기지 않아, 이중입고/공출고가 운전화면 [설비에러이력] 에 뜨지 않았다.
            if (bNewErr) InsertEqpErrHis(v, Cached(v, "ERR_CODE_RD"), lugg);

            string strSqlObs = "";
            strSqlObs += CRLF + " UPDATE " + m_strTable + "                          ";
            strSqlObs += CRLF + "    SET READ_UPD_DT = GETDATE()" + set.ToString();
            strSqlObs += CRLF + "  WHERE WH_TYP        = '" + Esc(m_strWhTyp) + "'   ";
            strSqlObs += CRLF + "    AND " + m_strKeyCol + " = '" + Esc(v.KeyVal) + "' ";
            DbExec(strSqlObs);
        }

        // ---------- 운전 화면 명령(CMD_RQ_ID / CMD_RQ_YN) 소비 ----------
        // [LGLS 2026-09-04] 크레인/RGV 상태창의 [지시 삭제](DELFK1/DELFK2/DELFK12)는 CMD_RQ_YN='Y' 로만 남고
        //   설비 통신이 소비하지 않아 영영 'Y' 였다(운전자 눈에는 "삭제해도 안 지워짐").
        //   구 EQP_TASK 역할대로 여기서 소비한다 :
        //   · DELFK* : 설비에 낸 반송지시 취소 - TRANSFER_REQUEST 스트로브 OFF + _OD 지시값 비움 + OD_RQ_YN/TRANSFER_REQUEST_OD='N'
        //   · FCMP   : 스케줄러(IO_TASK cThread_SC)가 직접 소비하므로 건드리지 않는다
        //   · 그 외(CTH/PAUSE/RESET …) : 이 설비 규약(XGT 메모리)에 대응 신호가 없어 소비만 하고 이력에 남긴다
        private void ConsumeOperatorCommands(VehDef v)
        {
            string strSql = "";
            strSql += CRLF + " SELECT CMD_RQ_ID FROM " + m_strTable + "                       ";
            strSql += CRLF + "  WHERE WH_TYP = '" + Esc(m_strWhTyp) + "' AND " + m_strKeyCol + " = '" + Esc(v.KeyVal) + "' ";
            strSql += CRLF + "    AND CMD_RQ_YN = 'Y'                                           ";
            DataTable dt = DbQuery(strSql);
            if (dt.Rows.Count == 0) return;
            string cmd = ("" + dt.Rows[0]["CMD_RQ_ID"]).Trim().ToUpper();
            if (cmd.Length == 0 || cmd == "FCMP") return;

            // [LGLS 2026-09-12] 운전 화면 [Ack 쓰기] : CMD_RQ_ID = "ACKW-LC=1" / "ACKW-UC=0"
            //   ※구분자는 '-' 다. ':' 를 쓰면 DB 계층이 파라미터 표시 '@' 로 바꿔 처리 완료 UPDATE 가 안 맞는다(실측 72회 반복).
            //   LC = LOAD_COMPLETE_ACK, UC = UNLOAD_COMPLETE_ACK, =1 켬 / =0 끔.
            //   상황 A(설비가 완료 보고를 들고 Ack 를 기다리는데 Ack 가 지워진 경우)에서 운전원이 되살린다.
            //   반드시 이 스레드가 쓴다 - 같은 Ack 워드를 만지는 다른 손이 없게 하고, 캐시를 함께 맞춰
            //   AckFollow 가 설비 보고 OFF 때 규약대로 Ack 를 내리게 한다(다른 경로로 켠 Ack 는 내려 주는 이가 없다).
            //   PLC→WCS 보고 비트(LOAD/UNLOAD_COMPLETE)는 대상이 아니다 - 그것은 PLC 의 말이다.
            if (cmd.StartsWith("ACKW"))
            {
                string strSpec = cmd.Length > 5 ? cmd.Substring(5).Trim() : "";     // "LC=1"
                int eq = strSpec.IndexOf('=');
                string strSig = eq > 0 ? strSpec.Substring(0, eq).Trim() : "";
                string strVal = eq > 0 ? strSpec.Substring(eq + 1).Trim() : "";
                string strAckTag = (strSig == "LC") ? "LOAD_COMPLETE_ACK" : (strSig == "UC") ? "UNLOAD_COMPLETE_ACK" : null;
                string strRepTag = (strSig == "LC") ? "LOAD_COMPLETE"     : (strSig == "UC") ? "UNLOAD_COMPLETE"     : null;
                string strKey    = (strSig == "LC") ? "__lcAck" : "__ucAck";
                bool bOn = (strVal == "1");
                string strRes;
                if (strAckTag == null || (strVal != "1" && strVal != "0"))
                    strRes = "형식 오류(허용: ACKW-LC=1|0, ACKW-UC=1|0) - 무시";
                else
                {
                    ObsDef dAck = O(v, strAckTag);
                    ObsDef dRep = O(v, strRepTag);
                    bool bRep = false;
                    if (dRep != null) ReadBit(dRep, ref bRep);
                    bool bOk = (dAck != null) && WriteBit(dAck, bOn);
                    if (bOk) { v.Cache[strKey] = bOn ? "1" : "0"; v.Cache.Remove(strKey + "_x"); }
                    strRes = strAckTag + (bOn ? " ON" : " OFF") + (dAck != null ? " (M" + dAck.Address + ")" : "")
                           + " 수동 " + (bOk ? "기록" : "기록 실패") + " / 설비 보고 " + strRepTag + "=" + (bRep ? "1" : "0")
                           + ((bOn && !bRep) ? " - 설비가 질문을 들고 있지 않다(상황 B) : 다음 주기에 규약대로 내려간다" : "");
                }
                string strUpdA = "";
                strUpdA += CRLF + " UPDATE " + m_strTable + " SET CMD_RQ_YN = 'N', WRITE_UPD_DT = GETDATE()";
                strUpdA += CRLF + "  WHERE WH_TYP = '" + Esc(m_strWhTyp) + "' AND " + m_strKeyCol + " = '" + Esc(v.KeyVal) + "' ";
                strUpdA += CRLF + "    AND CMD_RQ_YN = 'Y' AND CMD_RQ_ID = '" + Esc(cmd) + "' ";
                DbExec(strUpdA);
                LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 운전 명령 " + cmd + " → " + strRes);
                return;
            }

            string act;
            string extra = "";
            if (cmd.StartsWith("DELFK"))
            {
                WriteBit(O(v, "TRANSFER_REQUEST"), false);
                extra = ", OD_RQ_YN = 'N', TRANSFER_REQUEST_OD = 'N', PALLET_ID_OD = '', "
                      + "FROM_01_OD = '', FROM_02_OD = '', FROM_03_OD = '', TO_01_OD = '', TO_02_OD = '', TO_03_OD = ''";
                if (m_strKind == "RTV") extra += ", DEPART_TRACK = '', ARRIVE_TRACK = ''";
                act = "지시 삭제(반송지시 취소, 스트로브 OFF)";
            }
            else act = "명령 접수(설비 규약에 대응 신호 없음 - 이력만 기록)";

            string strUpd = "";
            strUpd += CRLF + " UPDATE " + m_strTable + " SET CMD_RQ_YN = 'N', WRITE_UPD_DT = GETDATE()" + extra;
            strUpd += CRLF + "  WHERE WH_TYP = '" + Esc(m_strWhTyp) + "' AND " + m_strKeyCol + " = '" + Esc(v.KeyVal) + "' ";
            strUpd += CRLF + "    AND CMD_RQ_YN = 'Y' AND CMD_RQ_ID = '" + Esc(cmd) + "'                   ";
            DbExec(strUpd);
            LogDb("[VEH_" + m_strKind + "] " + v.OwnerId + " 운전 명령 " + cmd + " → " + act);
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

            // [LGLS 2026-09-04] 관측 정의 누락 진단 - 어떤 태그가 없는지 이력에 남긴다
            foreach (string tg in new[] { "PALLET_ID", "FROM_01", "FROM_02", "FROM_03", "TO_01", "TO_02", "TO_03", "TRANSFER_REQUEST" })
                if (O(v, tg) == null) throw new Exception(v.OwnerId + " 관측 정의 없음: " + tg + " (보유: " + string.Join(",", new List<string>(v.Obs.Keys).ToArray()) + ")");
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
