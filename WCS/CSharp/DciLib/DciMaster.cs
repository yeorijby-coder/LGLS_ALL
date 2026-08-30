using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Xml;
using DciLib.Controls;

namespace DciLib
{
    /// <summary>
    /// C++의 CDciMaster에 대응하는 WPF 레이아웃 매니저.
    /// EcsLayout.xml을 파싱해 DCI 컨트롤 목록을 구성하고
    /// 논리 좌표 ↔ 화면 좌표 변환을 담당한다.
    /// </summary>
    public class DciMaster
    {
        // ─── 논리 그리드 범위 (EcsLayout.xml 루트 속성) ──────────────
        public int GridLeft   { get; private set; }
        public int GridBottom { get; private set; }
        public int GridRight  { get; private set; }
        public int GridTop    { get; private set; }

        // ─── 화면 크기 ────────────────────────────────────────────────
        public double ScreenWidth  { get; private set; } = 800;
        public double ScreenHeight { get; private set; } = 600;

        // 트랙 텍스트 표시 모드 (C++ m_nTrackTextMode: 0=작업번호,1=트랙번호,2=제품정보)
        public int TrackTextMode { get; set; } = 1;

        // ─── 스케일 (줌) ─────────────────────────────────────────────
        public double ScaleX { get; private set; } = 1.0;
        public double ScaleY { get; private set; } = 1.0;

        // ─── 그룹별 컨트롤 목록 ─────────────────────────────────────
        public List<ControlGroup> Groups { get; } = new();
        public DciControlList     AllControls { get; } = new();

        // ─── 컨트롤 팩토리 등록 ─────────────────────────────────────
        private static readonly Dictionary<string, Func<DciControl>> _factory = new()
        {
            ["CDciStaticCtrl"] = () => new DciStaticCtrl(),
            ["CDciButtonCtrl"] = () => new DciButtonCtrl(),
            ["CDciRackCtrl"]   = () => new DciRackCtrl(),
            ["CDciTrackCtrl"]  = () => new DciTrackCtrl(),
            ["CDciRvCtrl"]     = () => new DciRvCtrl(),
            ["CDciLifterCtrl"] = () => new DciLifterCtrl(),
            ["CDciOpboxCtrl"]  = () => new DciOpboxCtrl(),
        };

        // ─── XML 로딩 ─────────────────────────────────────────────────

        /// <summary>
        /// EcsLayout.xml 파일을 읽어 DCI 컨트롤 트리를 구성한다.
        /// C++의 CDciMaster 생성 + 각 CDci* 생성에 대응.
        /// </summary>
        public bool LoadLayout(string xmlPath)
        {
            try
            {
                var doc = new XmlDocument();
                // euc-kr 인코딩 선언된 파일도 처리하기 위해 StreamReader로 명시 로딩
                Encoding enc = Encoding.UTF8;
                try
                {
                    // XML 선언의 encoding 속성 미리 읽기
                    using var sr = new StreamReader(xmlPath, detectEncodingFromByteOrderMarks: true);
                    string firstLine = sr.ReadLine() ?? string.Empty;
                    if (firstLine.IndexOf("euc-kr", StringComparison.OrdinalIgnoreCase) >= 0)
                        enc = Encoding.GetEncoding("euc-kr");
                }
                catch { }
                using (var fs = new FileStream(xmlPath, FileMode.Open, FileAccess.Read))
                using (var reader = new StreamReader(fs, enc))
                    doc.Load(reader);
                var root = doc.DocumentElement;
                if (root == null) return false;

                // 루트 <Builder> 속성으로 논리 그리드 범위 설정
                GridLeft   = int.TryParse(root.GetAttribute("l"),  out int l) ? l : 0;
                GridBottom = int.TryParse(root.GetAttribute("b"),  out int b) ? b : 0;
                GridRight  = int.TryParse(root.GetAttribute("r"),  out int r) ? r : 83;
                GridTop    = int.TryParse(root.GetAttribute("t"),  out int t) ? t : 49;

                Groups.Clear();
                AllControls.Clear();

                // <Group> 노드 파싱
                foreach (XmlNode groupNode in root.SelectNodes("Group") ?? new XmlNodeListEmpty())
                {
                    var group = new ControlGroup();
                    group.LoadFromXml(groupNode);
                    Groups.Add(group);

                    // 그룹 내 각 컨트롤 타입 파싱
                    foreach (XmlNode typeNode in groupNode.ChildNodes)
                    {
                        if (typeNode.NodeType != XmlNodeType.Element) continue;
                        if (!_factory.TryGetValue(typeNode.Name, out var factory)) continue;

                        foreach (XmlNode ctrlNode in typeNode.SelectNodes("Control") ?? new XmlNodeListEmpty())
                        {
                            var ctrl = factory();
                            ctrl.LoadFromXml(ctrlNode);
                            group.Controls.Add(ctrl);
                            AllControls.AddLast(ctrl);
                        }
                    }
                }

                RecalcScreenCoords();
                return true;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"DciMaster.LoadLayout 오류: {ex.Message}");
                return false;
            }
        }

        // ─── 좌표 변환 ────────────────────────────────────────────────

        /// <summary>
        /// 화면 크기 변경 시 모든 컨트롤의 화면 좌표를 재계산.
        /// C++의 CDciMaster::RecalcLayout() 에 대응.
        /// </summary>
        public void Resize(double screenW, double screenH)
        {
            ScreenWidth  = screenW;
            ScreenHeight = screenH;
            RecalcScreenCoords();
        }

        private void RecalcScreenCoords()
        {
            int logW = GridRight  - GridLeft;
            int logH = GridTop    - GridBottom;
            if (logW <= 0 || logH <= 0) return;

            ScaleX = ScreenWidth  / logW;
            ScaleY = ScreenHeight / logH;

            foreach (var ctrl in AllControls)
            {
                ctrl.ScreenRect = LogicalToScreen(
                    ctrl.LogLeft, ctrl.LogBottom, ctrl.LogRight, ctrl.LogTop);
            }
        }

        /// <summary>논리 좌표(l,b,r,t) → WPF Rect 변환.</summary>
        public Rect LogicalToScreen(int l, int b, int r, int t)
        {
            double x1 = (l - GridLeft)   * ScaleX;
            double y1 = (GridTop - t)     * ScaleY;
            double x2 = (r - GridLeft)   * ScaleX;
            double y2 = (GridTop - b)     * ScaleY;
            return new Rect(x1, y1, Math.Abs(x2 - x1), Math.Abs(y2 - y1));
        }

        /// <summary>화면 좌표 → 논리 좌표 변환 (클릭 히트 테스트용).</summary>
        public Point ScreenToLogical(double sx, double sy)
        {
            return new Point(
                sx / ScaleX + GridLeft,
                GridTop - sy / ScaleY);
        }

        // ─── 그리기 ──────────────────────────────────────────────────

        /// <summary>모든 컨트롤을 DrawingContext에 그린다.</summary>
        public void Draw(DrawingContext dc)
        {
            foreach (var ctrl in AllControls)
            {
                if (ctrl.Visible)
                    ctrl.Draw(dc, this);
            }
        }

        // ─── 히트 테스트 ─────────────────────────────────────────────

        /// <summary>클릭된 화면 좌표에서 최상위 컨트롤을 찾는다.</summary>
        public DciControl? HitTest(Point screenPt)
        {
            // 역순으로 탐색해 맨 위에 그려진 컨트롤부터 히트
            var node = AllControls.Last;
            while (node != null)
            {
                var ctrl = node.Value;
                if (ctrl.Visible && ctrl.Enable && ctrl.ScreenRect.Contains(screenPt))
                    return ctrl;
                node = node.Previous;
            }
            return null;
        }

        // ─── 컨트롤 조회 ─────────────────────────────────────────────

        public DciControl? FindById(long id) => AllControls.FindById(id);

        public T? FindById<T>(long id) where T : DciControl
        {
            return AllControls.FindById(id) as T;
        }
    }

    // XmlNodeList 빈 구현 (SelectNodes가 null 반환 시 foreach 안전하게)
    internal class XmlNodeListEmpty : XmlNodeList
    {
        public override int Count => 0;
        public override XmlNode Item(int index) => throw new IndexOutOfRangeException();
        public override System.Collections.IEnumerator GetEnumerator()
            => System.Array.Empty<XmlNode>().GetEnumerator();
    }
}
