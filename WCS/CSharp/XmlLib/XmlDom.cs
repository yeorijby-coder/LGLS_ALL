using System;
using System.IO;
using System.Xml;
using System.Xml.XPath;

namespace XmlLib
{
    /// <summary>
    /// C++의 CXmlDom (MSXML3 래퍼)에 대응하는 XML DOM 조작 클래스.
    /// XmlDocument 기반으로 동일한 인터페이스를 제공한다.
    /// </summary>
    public class XmlDom
    {
        protected XmlDocument _doc = new XmlDocument();
        protected XmlNode? _curNode;
        protected XmlNodeList? _curNodeList;
        protected int _curNodeListIndex;
        private string _errorMsg = string.Empty;

        public string ErrorMessage => _errorMsg;

        // ─── 파일 I/O ───────────────────────────────────────────────

        public bool LoadXml(string filePath)
        {
            try
            {
                _doc.Load(filePath);
                _curNode = _doc.DocumentElement;
                _errorMsg = string.Empty;
                return true;
            }
            catch (Exception ex)
            {
                _errorMsg = ex.Message;
                return false;
            }
        }

        public bool SaveXml(string filePath)
        {
            try
            {
                var settings = new XmlWriterSettings
                {
                    Indent = true,
                    IndentChars = "\t",
                    Encoding = System.Text.Encoding.UTF8
                };
                using var writer = XmlWriter.Create(filePath, settings);
                _doc.Save(writer);
                return true;
            }
            catch (Exception ex)
            {
                _errorMsg = ex.Message;
                return false;
            }
        }

        public bool CreateXml(string rootElement)
        {
            _doc = new XmlDocument();
            _doc.AppendChild(_doc.CreateXmlDeclaration("1.0", "utf-8", null));
            var root = _doc.CreateElement(rootElement);
            _doc.AppendChild(root);
            _curNode = root;
            return true;
        }

        // ─── 노드 탐색 (C++ Move* 메서드에 대응) ─────────────────────

        public bool MoveDocument()
        {
            _curNode = _doc;
            return _curNode != null;
        }

        public bool MoveRoot()
        {
            _curNode = _doc.DocumentElement;
            return _curNode != null;
        }

        public bool MoveParent()
        {
            if (_curNode?.ParentNode == null) return false;
            _curNode = _curNode.ParentNode;
            return true;
        }

        public bool MoveChild(string tagName)
        {
            var child = _curNode?.SelectSingleNode(tagName);
            if (child == null) return false;
            _curNode = child;
            return true;
        }

        public bool MoveSibling(string tagName)
        {
            if (_curNode?.ParentNode == null) return false;
            var sibling = _curNode.ParentNode.SelectSingleNode(tagName);
            if (sibling == null) return false;
            _curNode = sibling;
            return true;
        }

        // ─── 속성 접근 ─────────────────────────────────────────────

        public string GetAttribute(string name)
        {
            if (_curNode is XmlElement el)
                return el.GetAttribute(name);
            return string.Empty;
        }

        public string GetAttribute(XmlNode node, string name)
        {
            if (node is XmlElement el)
                return el.GetAttribute(name);
            return string.Empty;
        }

        public int GetAttributeInt(string name, int def = 0)
        {
            var val = GetAttribute(name);
            return int.TryParse(val, out int n) ? n : def;
        }

        public int GetAttributeInt(XmlNode node, string name, int def = 0)
        {
            var val = GetAttribute(node, name);
            return int.TryParse(val, out int n) ? n : def;
        }

        public bool SetAttribute(string name, string value)
        {
            if (_curNode is XmlElement el)
            {
                el.SetAttribute(name, value);
                return true;
            }
            return false;
        }

        public string GetText()
        {
            return _curNode?.InnerText ?? string.Empty;
        }

        public string GetText(XmlNode node)
        {
            return node?.InnerText ?? string.Empty;
        }

        // ─── 자식 노드 열거 ─────────────────────────────────────────

        public XmlNodeList? SelectNodes(string xpath)
        {
            return _curNode?.SelectNodes(xpath);
        }

        public XmlNode? SelectSingleNode(string xpath)
        {
            return _curNode?.SelectSingleNode(xpath);
        }

        public XmlNodeList? GetChildren(string tagName)
        {
            return _curNode?.SelectNodes(tagName);
        }

        public XmlNodeList? GetChildren()
        {
            return _curNode?.ChildNodes;
        }

        // ─── 노드 생성/추가 ─────────────────────────────────────────

        public XmlElement CreateElement(string tagName)
        {
            return _doc.CreateElement(tagName);
        }

        public XmlNode? AppendChild(string tagName)
        {
            var el = _doc.CreateElement(tagName);
            _curNode?.AppendChild(el);
            _curNode = el;
            return el;
        }

        public void AppendChildNode(XmlNode parent, XmlNode child)
        {
            parent.AppendChild(child);
        }

        public XmlNode? CurrentNode => _curNode;

        public XmlDocument Document => _doc;
    }

    /// <summary>
    /// C++의 Convert 유틸리티에 대응하는 형변환 헬퍼.
    /// </summary>
    public static class XmlConvert
    {
        public static int ToInt(string s, int def = 0) =>
            int.TryParse(s, out int n) ? n : def;

        public static bool ToBool(string s, bool def = false) =>
            bool.TryParse(s, out bool b) ? b : (s == "1" ? true : (s == "0" ? false : def));

        public static string FromInt(int n) => n.ToString();
        public static string FromBool(bool b) => b ? "1" : "0";
    }

    /// <summary>
    /// C++의 Propertys.h에 대응하는 XML 기반 속성 직렬화 유틸리티.
    /// </summary>
    public class Propertys : XmlDom
    {
        public bool Load(string filePath) => LoadXml(filePath);
        public bool Save(string filePath) => SaveXml(filePath);

        public string Get(XmlNode node, string key, string def = "")
        {
            var child = node.SelectSingleNode(key);
            if (child != null) return child.InnerText;
            if (node is XmlElement el)
            {
                string attr = el.GetAttribute(key);
                if (!string.IsNullOrEmpty(attr)) return attr;
            }
            return def;
        }

        public int GetInt(XmlNode node, string key, int def = 0)
        {
            string v = Get(node, key, def.ToString());
            return int.TryParse(v, out int n) ? n : def;
        }

        public void Set(XmlNode node, string key, string value)
        {
            var child = node.SelectSingleNode(key);
            if (child == null)
            {
                child = _doc.CreateElement(key);
                node.AppendChild(child);
            }
            child.InnerText = value;
        }
    }
}
