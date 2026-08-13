using System;
using System.Xml;
using Ecs.Equipment;
using Ecs.Info;
using XmlLib;

namespace Ecs
{
    /// <summary>
    /// C++의 CEcsDefine에 대응하는 EcsDefine.xml 파서.
    /// 장비 구성(CV, SC, RTV, BCR 등)과 트랙/스테이션 레이아웃을 로딩한다.
    /// </summary>
    public class EcsDefine : XmlDom
    {
        private readonly EcsDoc _doc;

        public EcsDefine(EcsDoc doc)
        {
            _doc = doc;
        }

        public bool ParseXml(string xmlPath)
        {
            if (!LoadXml(xmlPath))
            {
                System.Diagnostics.Debug.WriteLine($"EcsDefine.ParseXml 로딩 실패: {ErrorMessage}");
                return false;
            }

            var root = Document.DocumentElement;
            if (root == null) return false;

            foreach (XmlNode eqNode in root.ChildNodes)
            {
                if (eqNode.NodeType != XmlNodeType.Element) continue;
                string cls = GetAttribute(eqNode, "class");

                switch (cls)
                {
                    case "CCv":     ParseCv(eqNode);     break;
                    case "CSc":     ParseSc(eqNode);     break;
                    case "CRtv":    ParseRtv(eqNode);    break;
                    case "CBcr":    ParseBcr(eqNode);    break;
                    case "CLifter": ParseLifter(eqNode); break;
                    case "CLgv":    ParseLgv(eqNode);    break;
                    case "CRfid":   ParseRfid(eqNode);   break;
                    case "CDisplay":ParseDisplay(eqNode);break;
                    case "CThs":    ParseThs(eqNode);    break;
                }
            }

            return true;
        }

        private void ParseCv(XmlNode node)
        {
            int    index  = GetAttributeInt(node, "number") - 1;
            int    number = GetAttributeInt(node, "number");
            string device = GetAttribute(node, "device");
            string name   = GetAttribute(node, "name");
            long   cid    = long.TryParse(GetAttribute(node, "cid"), out long c) ? c : 0;

            var cv = new Cv(_doc, index, number, device, name);
            cv.Host = _doc.Config?.GetCvHost(index) ?? string.Empty;
            cv.Port = _doc.Config?.GetCvPort(index) ?? 5000;

            // 트랙 파싱
            var tracksNode = node.SelectSingleNode("Tracks");
            if (tracksNode != null)
                ParseTracks(cv, tracksNode);

            _doc.CvArray.Add(cv);
        }

        private void ParseSc(XmlNode node)
        {
            int    index  = GetAttributeInt(node, "number") - 1;
            int    number = GetAttributeInt(node, "number");
            string device = GetAttribute(node, "device");
            string name   = GetAttribute(node, "name");

            var sc = new Sc(_doc, index, number, device, name);
            sc.Host = _doc.Config?.GetScHost(index) ?? string.Empty;
            sc.Port = _doc.Config?.GetScPort(index) ?? 5001;

            // 쌍(pair) SC 설정
            var pairNode = node.SelectSingleNode("Pair");
            if (pairNode != null)
            {
                sc.PairScIndex = GetAttributeInt(pairNode, "index", -1);
                sc.IsPairMaster= GetAttribute(pairNode, "master") == "1";
            }

            // 랙 크기
            var rackNode = node.SelectSingleNode("Rack");
            if (rackNode != null)
            {
                sc.MaxRow = GetAttributeInt(rackNode, "rows", 10);
                sc.MaxCol = GetAttributeInt(rackNode, "cols", 50);
            }

            _doc.ScArray.Add(sc);
        }

        private void ParseRtv(XmlNode node)
        {
            int    index  = GetAttributeInt(node, "number") - 1;
            int    number = GetAttributeInt(node, "number");
            string device = GetAttribute(node, "device");
            string name   = GetAttribute(node, "name");

            var rtv = new Rtv(_doc, index, number, device, name);
            rtv.Host = _doc.Config?.GetRtvHost(index) ?? string.Empty;
            rtv.Port = _doc.Config?.GetRtvPort(index) ?? 5002;

            _doc.RtvArray.Add(rtv);
        }

        private void ParseBcr(XmlNode node)
        {
            // BCR 파싱 - 향후 구현
        }

        private void ParseLifter(XmlNode node)
        {
            // Lifter 파싱 - 향후 구현
        }

        private void ParseLgv(XmlNode node)
        {
            // LGV 파싱 - 향후 구현
        }

        private void ParseRfid(XmlNode node)
        {
            // RFID 파싱 - 향후 구현
        }

        private void ParseDisplay(XmlNode node)
        {
            // Display 파싱 - 향후 구현
        }

        private void ParseThs(XmlNode node)
        {
            // THS 파싱 - 향후 구현
        }

        private void ParseTracks(Cv cv, XmlNode tracksNode)
        {
            var trackNodes = tracksNode.SelectNodes("Track");
            if (trackNodes == null) return;

            cv.Info.InitTracks(trackNodes.Count);
            int idx = 0;

            foreach (XmlNode tn in trackNodes)
            {
                var track = cv.Info.Tracks[idx];
                track.Number    = GetAttributeInt(tn, "number");
                track.TabNumber = GetAttributeInt(tn, "tabnumber");
                track.Loop      = GetAttribute(tn, "loop") == "1";
                track.NextTrack = GetAttributeInt(tn, "NextTrack");

                // <Status> 하위 노드 파싱
                var statusNode = tn.SelectSingleNode("Status");
                if (statusNode != null)
                {
                    ParseTrackStatus(track, statusNode);
                }

                idx++;
            }
        }

        private void ParseTrackStatus(TrackInfo track, XmlNode statusNode)
        {
            foreach (XmlNode sn in statusNode.ChildNodes)
            {
                if (sn.NodeType != XmlNodeType.Element) continue;
                switch (sn.Name)
                {
                    case "ScStoHS":
                        track.IsScStoHS = true;
                        break;
                    case "ScRetHS":
                        track.IsScRetHS = true;
                        break;
                    case "CvCheckPos":
                        track.IsCvCheckPos = true;
                        break;
                    case "WaitForMainLoop":
                        track.IsWaitForMain = true;
                        break;
                    case "ArvStation":
                        track.ArvStationName = GetAttribute(sn, "name");
                        track.ArvStationId   = GetAttributeInt(sn, "id");
                        track.ArvStationUse  = GetAttribute(sn, "use") == "1";
                        break;
                    case "DelegateHS":
                        track.DelegateHsId = GetAttributeInt(sn, "id");
                        break;
                }
            }
        }

        private string GetAttribute(XmlNode node, string name)
            => (node as XmlElement)?.GetAttribute(name) ?? string.Empty;

        private int GetAttributeInt(XmlNode node, string name, int def = 0)
            => int.TryParse(GetAttribute(node, name), out int v) ? v : def;
    }
}
