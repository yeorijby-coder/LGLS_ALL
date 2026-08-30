using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

namespace EQP_SIM.Core
{
    public class ObservableDef
    {
        public string OwnerId;        // CONVEYOR:11, VEHICLE:1 ...
        public string Name;           // WAIT_OUT, PALLET_EXIST01 ...
        public string DataType;       // Boolean / Short / String
        public char Device;           // 'M'(B) / 'D'(W) / 'R'(R)  — XGT 영역 문자
        public int Address;           // ADDRESS_NO (hex 파싱 결과)
        public int BitIndex;          // W영역 비트필드용
        public int Length;            // String 워드 길이
        public string ConnectString;
    }

    /// <summary>
    /// TB_OBSERVABLE 덤프(observables.tsv)를 로드해 (OwnerId, 관측값명) → 주소 정보를 제공.
    /// TSV 컬럼: OWNERID, OBSERVABLEID, ACCESSTYPE, DATATYPE, MONFLAG, CONNECTSTRING
    /// CONNECTSTRING 예: DEVICE_TYPE=B,ADDRESS_NO=0244  /  DEVICE_TYPE=R,ADDRESS_NO=0100,LENGTH=2
    /// DEVICE_TYPE: B→%M, W→%D, R→%R  (ECS FenetDriver mdSend/mdReceive 매핑과 동일)
    /// </summary>
    public class ObservableMap
    {
        private readonly Dictionary<string, ObservableDef> map =
            new Dictionary<string, ObservableDef>(StringComparer.OrdinalIgnoreCase);

        public int Count { get { return map.Count; } }

        /// <summary>이 주소표의 출처 표시 ("XML v1.4" / "observables.tsv")</summary>
        public string Source = "";

        // ─────────────────────────────────────────────────────────────────
        // [LGLS 2026-08-21] 주소맵 XML(PlcAddressMap.xml) 1순위 구성.
        //   EQP_TASK 와 같은 파서(cPlcAddrMap, 링크 참조)로 같은 XML 을 읽어
        //   구 ECS TB_OBSERVABLE 과 동일한 이름 규칙으로 관측표를 만든다.
        //   → XML 하나만 고치면 EQP_TASK 통신과 EQP_SIM 설비 거동이 함께 바뀐다.
        //   R(트래킹) 해석도 XML rAddrMode 를 따른다(파서 RWord 경유).
        //   실패 시 false → 호출부가 tsv 폴백.
        // ─────────────────────────────────────────────────────────────────
        public bool LoadFromAddrMap()
        {
            try
            {
                if (!WCS_TASK_CV.cPlcAddrMap.IsLoaded) return false;

                // ── CONVEYOR:2~15 : 블록 주소식 + tsv 태그명 매핑 ──
                //    (CONVEYOR:1 은 구 ECS TB_OBSERVABLE 에도 없음 - PPT 'C/V#1 미사용')
                string[,] evtMap = {
                    { "UNLOAD_COMPLETE_01", "UnloadComplete1" },
                    { "LOAD_COMPLETE_01",   "LoadComplete1"   },
                    { "UNLOAD_COMPLETE_02", "UnloadComplete2" },
                    { "LOAD_COMPLETE_02",   "LoadComplete2"   },
                    { "WAIT_IN",            "WorkInstruction" },
                    { "WAIT_OUT",           "WorkOrder"       },
                    { "IN_READY_02",        "InReady2"        },
                    { "OPERATION_MODE",     "OperationMode"   },
                    { "UNLOAD_REQUEST_ACK_02", "UnloadRequest2Ack" },
                };
                string[,] ackMap = {
                    { "UNLOAD_COMPLETE_ACK_01", "UnloadComplete1Ack" },
                    { "LOAD_COMPLETE_ACK_01",   "LoadComplete1Ack"   },
                    { "UNLOAD_COMPLETE_ACK_02", "UnloadComplete2Ack" },
                    { "LOAD_COMPLETE_ACK_02",   "LoadComplete2Ack"   },
                    { "UNLOAD_REQUEST_02",      "UnloadRequest2"     },
                };

                var tmp = new Dictionary<string, ObservableDef>(StringComparer.OrdinalIgnoreCase);
                for (int n = 2; n <= 15; n++)
                {
                    string owner = "CONVEYOR:" + n;
                    int evBase  = WCS_TASK_CV.cPlcAddrMap.BlockBase("CV", n, "Event");
                    int ackBase = WCS_TASK_CV.cPlcAddrMap.BlockBase("CV", n, "Ack");
                    int dirAddr = WCS_TASK_CV.cPlcAddrMap.BlockBase("CV", n, "Direction");
                    if (evBase < 0 || ackBase < 0 || dirAddr < 0) return false;

                    for (int k = 0; k < evtMap.GetLength(0); k++)
                    {
                        int ofs = WCS_TASK_CV.cPlcAddrMap.SignalOffset("CV", "Event", evtMap[k, 1]);
                        if (ofs < 0) continue;
                        tmp[Key(owner, evtMap[k, 0])] = new ObservableDef
                        { OwnerId = owner, Name = evtMap[k, 0], DataType = "Boolean", Device = 'M', Address = evBase + ofs, Length = 1 };
                    }
                    for (int k = 0; k < ackMap.GetLength(0); k++)
                    {
                        int ofs = WCS_TASK_CV.cPlcAddrMap.SignalOffset("CV", "Ack", ackMap[k, 1]);
                        if (ofs < 0) continue;
                        tmp[Key(owner, ackMap[k, 0])] = new ObservableDef
                        { OwnerId = owner, Name = ackMap[k, 0], DataType = "Boolean", Device = 'M', Address = ackBase + ofs, Length = 1 };
                    }
                    // 파렛트 감지 5슬롯 (구 ECS 와 동일하게 5개 정의, 실제 사용은 2~3개)
                    int peOfs = WCS_TASK_CV.cPlcAddrMap.SignalOffset("CV", "Event", "PalletExist");
                    if (peOfs < 0) peOfs = 16;
                    for (int s = 0; s < 5; s++)
                        tmp[Key(owner, "PALLET_EXIST_FLAG_0" + (s + 1))] = new ObservableDef
                        { OwnerId = owner, Name = "PALLET_EXIST_FLAG_0" + (s + 1), DataType = "Boolean", Device = 'M', Address = evBase + peOfs + s, Length = 1 };
                    // 방향 워드
                    tmp[Key(owner, "DIRECTION_MODE")] = new ObservableDef
                    { OwnerId = owner, Name = "DIRECTION_MODE", DataType = "String", Device = 'D', Address = dirAddr, Length = 1 };
                    // 트래킹 (JOB NO, 슬롯당 2워드 - rAddrMode 는 파서 Addr() 안에서 적용된다)
                    for (int s = 0; s < 5; s++)
                    {
                        int rAddr = WCS_TASK_CV.cPlcAddrMap.Addr("CV", n, "Tracking", "JobNo", s);
                        if (rAddr < 0) continue;
                        tmp[Key(owner, "PALLET_EXIST0" + (s + 1))] = new ObservableDef
                        { OwnerId = owner, Name = "PALLET_EXIST0" + (s + 1), DataType = "String", Device = 'R', Address = rAddr, Length = 2 };
                    }
                }

                // ── VEHICLE:1(RGV) / VEHICLE:11~15(S/C) : TagTable 그대로 ──
                for (int k = 0; k <= 5; k++)
                {
                    string typ   = (k == 0) ? "RGV" : "SC";
                    int    no    = (k == 0) ? 1 : k;
                    string owner = (k == 0) ? "VEHICLE:1" : ("VEHICLE:1" + k);
                    var tbl = WCS_TASK_CV.cPlcAddrMap.TagTable(typ, no);
                    if (tbl == null || tbl.Count == 0) return false;
                    foreach (var kv in tbl)
                        tmp[Key(owner, kv.Key)] = new ObservableDef
                        {
                            OwnerId = owner, Name = kv.Key,
                            DataType = (kv.Value.Device == 'M') ? "Boolean" : (kv.Value.Length > 1 ? "String" : "Short"),
                            Device = kv.Value.Device, Address = kv.Value.Address, Length = kv.Value.Length
                        };
                }

                map.Clear();
                foreach (var kv in tmp) map[kv.Key] = kv.Value;
                Source = "PlcAddressMap.xml v" + WCS_TASK_CV.cPlcAddrMap.Version
                       + " (R=" + (WCS_TASK_CV.cPlcAddrMap.RAddrModeHex ? "HEX" : "DEC") + ")";
                return true;
            }
            catch { return false; }
        }

        public void Load(string tsvPath)
        {
            foreach (string raw in File.ReadAllLines(tsvPath))
            {
                string line = raw.Trim('﻿', ' ', '\r');
                if (line.Length == 0) continue;
                string[] cols = line.Split('\t');
                if (cols.Length < 6) continue;

                var def = new ObservableDef
                {
                    OwnerId = cols[0].Trim(),
                    Name = cols[1].Trim(),
                    DataType = cols[3].Trim(),
                    ConnectString = cols[5].Trim(),
                    Length = 1
                };
                if (def.OwnerId.Length == 0 || def.ConnectString.Length == 0) continue;

                foreach (string part in def.ConnectString.Split(','))
                {
                    int eq = part.IndexOf('=');
                    if (eq <= 0) continue;
                    string key = part.Substring(0, eq).Trim();
                    string val = part.Substring(eq + 1).Trim();
                    switch (key.ToUpperInvariant())
                    {
                        case "DEVICE_TYPE":
                            def.Device = val == "B" ? 'M' : val == "W" ? 'D' : val == "R" ? 'R' : '?';
                            break;
                        case "ADDRESS_NO":
                            // [LGLS 2026-08-21] R(트래킹)은 XML rAddrMode 를 따른다 (B/W 는 항상 16진 - 구 ECS 동일)
                            if (def.Device == 'R' && WCS_TASK_CV.cPlcAddrMap.IsLoaded && !WCS_TASK_CV.cPlcAddrMap.RAddrModeHex)
                                int.TryParse(val, NumberStyles.Integer, CultureInfo.InvariantCulture, out def.Address);
                            else
                                int.TryParse(val, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out def.Address);
                            break;
                        case "BIT_INDEX":
                            int.TryParse(val, out def.BitIndex);
                            break;
                        case "LENGTH":
                            int.TryParse(val, out def.Length);
                            break;
                    }
                }
                if (def.Device == '?') continue;
                map[Key(def.OwnerId, def.Name)] = def;
            }
        }

        private static string Key(string owner, string name) { return owner + "|" + name; }

        public ObservableDef Get(string owner, string name)
        {
            ObservableDef def;
            if (!map.TryGetValue(Key(owner, name), out def))
                throw new KeyNotFoundException("관측값 없음: " + owner + "." + name);
            return def;
        }

        public bool TryGet(string owner, string name, out ObservableDef def)
        {
            return map.TryGetValue(Key(owner, name), out def);
        }
    }
}
