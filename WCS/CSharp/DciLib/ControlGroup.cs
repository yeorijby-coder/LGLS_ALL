using System.Collections.Generic;
using System.Xml;
using DciLib.Controls;

namespace DciLib
{
    /// <summary>
    /// C++의 ControlGroup에 대응. EcsLayout.xml의 &lt;Group&gt; 노드.
    /// LAYOUT/LABEL 등 그룹별로 컨트롤을 분류하고 show 속성으로 표시/숨김 제어.
    /// </summary>
    public class ControlGroup
    {
        public string Name    { get; set; } = string.Empty;
        public string GroupId { get; set; } = string.Empty;
        public bool   Show    { get; set; } = true;

        public List<DciControl> Controls { get; } = new();

        public void LoadFromXml(XmlNode node)
        {
            if (node is XmlElement el)
            {
                Name    = el.GetAttribute("gname");
                GroupId = el.GetAttribute("gid");
                Show    = el.GetAttribute("show") != "0";
            }
        }

        public void SetVisible(bool visible)
        {
            Show = visible;
            foreach (var ctrl in Controls)
                ctrl.Visible = visible;
        }
    }
}
