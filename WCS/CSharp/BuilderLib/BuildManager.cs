using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using DciLib;
using DciLib.Controls;
using XmlLib;

namespace BuilderLib
{
    /// <summary>
    /// C++의 CBuildManager에 대응하는 EcsLayout.xml 빌더 관리자.
    /// 컨트롤 추가/삭제/이동/속성 편집 후 XML 저장.
    /// </summary>
    public class BuildManager
    {
        public DciMaster     Master   { get; } = new DciMaster();
        public List<ControlGroup> Groups => Master.Groups;

        private string _filePath = string.Empty;
        private bool   _modified;

        public bool Modified => _modified;

        // ─── 파일 I/O ─────────────────────────────────────────────────

        public bool LoadLayout(string path)
        {
            _filePath = path;
            bool ok = Master.LoadLayout(path);
            if (ok) _modified = false;
            return ok;
        }

        public bool SaveLayout(string? path = null)
        {
            string savePath = path ?? _filePath;
            if (string.IsNullOrEmpty(savePath)) return false;

            try
            {
                var doc = new XmlDocument();
                doc.AppendChild(doc.CreateXmlDeclaration("1.0", "utf-8", null));

                var root = doc.CreateElement("Builder");
                root.SetAttribute("id",  "0");
                root.SetAttribute("l",   Master.GridLeft.ToString());
                root.SetAttribute("b",   Master.GridBottom.ToString());
                root.SetAttribute("r",   Master.GridRight.ToString());
                root.SetAttribute("t",   Master.GridTop.ToString());
                doc.AppendChild(root);

                foreach (var group in Groups)
                {
                    var gEl = doc.CreateElement("Group");
                    gEl.SetAttribute("gname", group.Name);
                    gEl.SetAttribute("gid",   group.GroupId);
                    gEl.SetAttribute("show",  group.Show ? "1" : "0");

                    // 컨트롤 타입별로 그룹화
                    var typeMap = new Dictionary<string, List<DciControl>>();
                    foreach (var ctrl in group.Controls)
                    {
                        string typeName = ctrl.GetType().Name;
                        if (!typeMap.ContainsKey(typeName))
                            typeMap[typeName] = new List<DciControl>();
                        typeMap[typeName].Add(ctrl);
                    }

                    foreach (var kv in typeMap)
                    {
                        var typeEl = doc.CreateElement(kv.Key);
                        foreach (var ctrl in kv.Value)
                        {
                            var ctrlEl = doc.CreateElement("Control");
                            WriteControlAttributes(ctrlEl, ctrl);
                            typeEl.AppendChild(ctrlEl);
                        }
                        gEl.AppendChild(typeEl);
                    }
                    root.AppendChild(gEl);
                }

                var settings = new XmlWriterSettings { Indent = true, IndentChars = "\t", Encoding = System.Text.Encoding.UTF8 };
                using var writer = XmlWriter.Create(savePath, settings);
                doc.Save(writer);

                _modified = false;
                return true;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"SaveLayout 오류: {ex.Message}");
                return false;
            }
        }

        private static void WriteControlAttributes(XmlElement el, DciControl ctrl)
        {
            el.SetAttribute("id",      ctrl.Id.ToString());
            el.SetAttribute("text",    ctrl.Text);
            el.SetAttribute("l",       ctrl.LogLeft.ToString());
            el.SetAttribute("b",       ctrl.LogBottom.ToString());
            el.SetAttribute("r",       ctrl.LogRight.ToString());
            el.SetAttribute("t",       ctrl.LogTop.ToString());
            el.SetAttribute("bgcolor", ctrl.BgColorHex);
            el.SetAttribute("fgcolor", ctrl.FgColorHex);
            if (ctrl.FontSize != 12) el.SetAttribute("fontsize", ctrl.FontSize.ToString());
            if (!ctrl.Visible) el.SetAttribute("visible", "0");
        }

        // ─── 컨트롤 편집 ─────────────────────────────────────────────

        public DciControl AddControl(string groupName, string controlType, int l, int b, int r, int t)
        {
            DciControl ctrl = controlType switch
            {
                "CDciStaticCtrl"  => new DciStaticCtrl(),
                "CDciButtonCtrl"  => new DciButtonCtrl(),
                "CDciRackCtrl"    => new DciRackCtrl(),
                "CDciTrackCtrl"   => new DciTrackCtrl(),
                "CDciRvCtrl"      => new DciRvCtrl(),
                "CDciLifterCtrl"  => new DciLifterCtrl(),
                "CDciOpboxCtrl"   => new DciOpboxCtrl(),
                _                 => new DciStaticCtrl(),
            };

            ctrl.Id        = GenerateId();
            ctrl.LogLeft   = l; ctrl.LogBottom = b;
            ctrl.LogRight  = r; ctrl.LogTop    = t;

            var group = FindOrCreateGroup(groupName);
            group.Controls.Add(ctrl);
            Master.AllControls.AddLast(ctrl);
            _modified = true;
            return ctrl;
        }

        public bool RemoveControl(long id)
        {
            foreach (var group in Groups)
            {
                var ctrl = group.Controls.Find(c => c.Id == id);
                if (ctrl != null)
                {
                    group.Controls.Remove(ctrl);
                    Master.AllControls.Remove(ctrl);
                    _modified = true;
                    return true;
                }
            }
            return false;
        }

        public bool MoveControl(long id, int dl, int db)
        {
            var ctrl = Master.FindById(id);
            if (ctrl == null) return false;
            ctrl.LogLeft   += dl; ctrl.LogRight  += dl;
            ctrl.LogBottom += db; ctrl.LogTop     += db;
            _modified = true;
            return true;
        }

        // ─── 헬퍼 ─────────────────────────────────────────────────────

        private ControlGroup FindOrCreateGroup(string name)
        {
            foreach (var g in Groups)
                if (g.Name == name) return g;
            var ng = new ControlGroup { Name = name };
            Groups.Add(ng);
            return ng;
        }

        private long _nextId = 900000000L;
        private long GenerateId() => ++_nextId;
    }
}
