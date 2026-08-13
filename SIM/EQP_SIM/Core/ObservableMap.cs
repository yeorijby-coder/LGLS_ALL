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
