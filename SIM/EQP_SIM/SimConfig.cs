using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

namespace EQP_SIM
{
    /// <summary>HOST_SIM.ini 로더 (섹션/키=값, # 또는 ; 주석)</summary>
    public class SimConfig
    {
        private readonly Dictionary<string, Dictionary<string, string>> data =
            new Dictionary<string, Dictionary<string, string>>(StringComparer.OrdinalIgnoreCase);

        public static string BaseDir { get { return AppDomain.CurrentDomain.BaseDirectory; } }

        public SimConfig(string path)
        {
            if (!File.Exists(path)) return;
            string section = "";
            foreach (string raw in File.ReadAllLines(path))
            {
                string line = raw.Trim();
                if (line.Length == 0 || line.StartsWith("#") || line.StartsWith(";")) continue;
                if (line.StartsWith("[") && line.EndsWith("]"))
                {
                    section = line.Substring(1, line.Length - 2).Trim();
                    if (!data.ContainsKey(section)) data[section] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                    continue;
                }
                int eq = line.IndexOf('=');
                if (eq <= 0) continue;
                if (!data.ContainsKey(section)) data[section] = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                data[section][line.Substring(0, eq).Trim()] = line.Substring(eq + 1).Trim();
            }
        }

        public string Get(string section, string key, string def)
        {
            Dictionary<string, string> s;
            string v;
            if (data.TryGetValue(section, out s) && s.TryGetValue(key, out v)) return v;
            return def;
        }

        public int GetInt(string section, string key, int def)
        {
            int v;
            return int.TryParse(Get(section, key, null), NumberStyles.Integer, CultureInfo.InvariantCulture, out v) ? v : def;
        }

        public bool GetBool(string section, string key, bool def)
        {
            string v = Get(section, key, null);
            if (v == null) return def;
            return v == "1" || v.Equals("true", StringComparison.OrdinalIgnoreCase) || v.Equals("Y", StringComparison.OrdinalIgnoreCase);
        }

        public int[] GetIntList(string section, string key, int[] def)
        {
            string v = Get(section, key, null);
            if (string.IsNullOrEmpty(v)) return def;
            var list = new List<int>();
            foreach (string p in v.Split(','))
            {
                int n;
                if (int.TryParse(p.Trim(), out n)) list.Add(n);
            }
            return list.Count > 0 ? list.ToArray() : def;
        }
    }
}
