using System;
using System.Collections.Generic;
using System.Xml;
using XmlLib;

namespace Ecs.Config
{
    /// <summary>
    /// C++의 CConfig에 대응하는 ECS 설정 관리자.
    /// EcsConfig.xml을 읽어 장비 IP/Port 및 시스템 설정을 관리한다.
    /// </summary>
    public class EcsConfig : XmlDom
    {
        // 장비별 Host/Port 설정
        private readonly List<string> _cvHosts  = new();
        private readonly List<int>    _cvPorts  = new();
        private readonly List<string> _scHosts  = new();
        private readonly List<int>    _scPorts  = new();
        private readonly List<string> _rtvHosts = new();
        private readonly List<int>    _rtvPorts = new();

        // 호스트(MES) 연결 설정
        public string HostAddress   { get; private set; } = "127.0.0.1";
        public int    HostPort      { get; private set; } = 7001;
        public int    HostListenPort{ get; private set; } = 7002;

        // 모니터 멀티캐스트 설정
        public string MulticastGroup{ get; private set; } = "239.0.0.1";
        public int    MulticastPort { get; private set; } = 9001;

        // 시스템 설정
        public int    AutoRunInterval   { get; private set; } = 100;
        public string BackupFolder      { get; private set; } = EcsPath.BackupFolder;
        public bool   AutoReconnect     { get; private set; } = true;

        public bool Load(string filePath)
        {
            if (!LoadXml(filePath)) return false;

            var root = Document.DocumentElement;
            if (root == null) return false;

            ParseHost(root);
            ParseMonitor(root);
            ParseEquipments(root);
            ParseSystem(root);

            return true;
        }

        public bool Save(string filePath)
        {
            if (!CreateXml("EcsConfig")) return false;

            var root = Document.DocumentElement!;

            // Host 노드
            var hostEl = Document.CreateElement("Host");
            hostEl.SetAttribute("address",    HostAddress);
            hostEl.SetAttribute("port",       HostPort.ToString());
            hostEl.SetAttribute("listenport", HostListenPort.ToString());
            root.AppendChild(hostEl);

            return SaveXml(filePath);
        }

        private void ParseHost(XmlNode root)
        {
            var n = root.SelectSingleNode("Host");
            if (n == null) return;
            HostAddress    = Attr(n, "address",    "127.0.0.1");
            HostPort       = AttrInt(n, "port",    7001);
            HostListenPort = AttrInt(n, "listenport", 7002);
        }

        private void ParseMonitor(XmlNode root)
        {
            var n = root.SelectSingleNode("Monitor");
            if (n == null) return;
            MulticastGroup = Attr(n, "group", "239.0.0.1");
            MulticastPort  = AttrInt(n, "port", 9001);
        }

        private void ParseEquipments(XmlNode root)
        {
            var cvNodes = root.SelectNodes("CV");
            if (cvNodes != null) foreach (XmlNode n in cvNodes)
            {
                _cvHosts.Add(Attr(n, "host", "192.168.0.10"));
                _cvPorts.Add(AttrInt(n, "port", 5000));
            }
            var scNodes = root.SelectNodes("SC");
            if (scNodes != null) foreach (XmlNode n in scNodes)
            {
                _scHosts.Add(Attr(n, "host", "192.168.0.20"));
                _scPorts.Add(AttrInt(n, "port", 5001));
            }
            var rtvNodes = root.SelectNodes("RTV");
            if (rtvNodes != null) foreach (XmlNode n in rtvNodes)
            {
                _rtvHosts.Add(Attr(n, "host", "192.168.0.30"));
                _rtvPorts.Add(AttrInt(n, "port", 5002));
            }
        }

        private void ParseSystem(XmlNode root)
        {
            var n = root.SelectSingleNode("System");
            if (n == null) return;
            AutoRunInterval = AttrInt(n, "autorun_interval", 100);
            BackupFolder    = Attr(n, "backup_folder", EcsPath.BackupFolder);
            AutoReconnect   = Attr(n, "auto_reconnect", "1") == "1";
        }

        // ─── 장비별 접속 정보 조회 ────────────────────────────────────
        public string GetCvHost(int idx) => idx < _cvHosts.Count ? _cvHosts[idx] : "127.0.0.1";
        public int    GetCvPort(int idx) => idx < _cvPorts.Count ? _cvPorts[idx] : 5000;
        public string GetScHost(int idx) => idx < _scHosts.Count ? _scHosts[idx] : "127.0.0.1";
        public int    GetScPort(int idx) => idx < _scPorts.Count ? _scPorts[idx] : 5001;
        public string GetRtvHost(int idx)=> idx < _rtvHosts.Count? _rtvHosts[idx]: "127.0.0.1";
        public int    GetRtvPort(int idx)=> idx < _rtvPorts.Count? _rtvPorts[idx]: 5002;

        private static string Attr(XmlNode n, string key, string def = "")
            => (n as XmlElement)?.GetAttribute(key) ?? def;
        private static int AttrInt(XmlNode n, string key, int def = 0)
            => int.TryParse(Attr(n, key), out int v) ? v : def;
    }
}
