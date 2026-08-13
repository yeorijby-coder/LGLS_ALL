using System;
using System.IO;
using DciLib;
using DciLib.Controls;

namespace Ecs
{
    /// <summary>
    /// C++의 CEcsLayout에 대응하는 WPF 레이아웃 관리자.
    /// DciMaster를 통해 EcsLayout.xml을 로딩하고,
    /// WPF의 DrawingVisual/Canvas에 렌더링을 위임한다.
    /// </summary>
    public class EcsLayout
    {
        public DciMaster DciMaster { get; } = new DciMaster();
        public string    XmlPath   { get; }
        public bool      Loaded    { get; private set; }
        public EcsDoc    Doc       { get; }

        // 컨트롤 클릭 이벤트 (장비 다이얼로그 열기 등에 사용)
        public event EventHandler<DciControl>? ControlClicked;

        public EcsLayout(EcsDoc doc, string xmlPath)
        {
            Doc     = doc;
            XmlPath = xmlPath;
        }

        public bool Load()
        {
            string fullPath = Path.GetFullPath(XmlPath);
            if (!File.Exists(fullPath))
            {
                System.Diagnostics.Debug.WriteLine($"EcsLayout: 파일 없음 - {fullPath}");
                return false;
            }

            Loaded = DciMaster.LoadLayout(fullPath);
            if (Loaded)
                System.Diagnostics.Debug.WriteLine($"EcsLayout 로딩 완료: {XmlPath} ({DciMaster.AllControls.Count}개 컨트롤)");
            return Loaded;
        }

        public void Resize(double w, double h)
        {
            DciMaster.Resize(w, h);
        }

        private DciControl? _pressedCtrl;

        /// <summary>마우스 버튼 누름 → Pressed=true (누른 모습 표시)</summary>
        public void HandleMouseDown(System.Windows.Point pt)
        {
            _pressedCtrl = DciMaster.HitTest(pt);
            if (_pressedCtrl == null) return;
            _pressedCtrl.OnClick(pt);
            ControlClicked?.Invoke(this, _pressedCtrl);
            InvalidateRequested?.Invoke();
        }

        /// <summary>마우스 버튼 뗌 → Pressed=false (올라오는 모습 표시)</summary>
        public void HandleMouseUp(System.Windows.Point pt)
        {
            if (_pressedCtrl == null) return;
            _pressedCtrl.Pressed = false;
            _pressedCtrl = null;
            InvalidateRequested?.Invoke();
        }

        // ─── 장비 상태 → DCI 컨트롤 업데이트 ─────────────────────────

        /// <summary>
        /// CV 트랙 상태를 DCI 컨트롤에 반영한다.
        /// C++의 CEcsLayout::UpdateCvTrack()에 대응.
        /// </summary>
        public void UpdateCvTrack(int cvNum, int trackNum, bool present, int luggNum, int error)
        {
            long ctrlId = MakeTrackCtrlId(cvNum, trackNum);
            var ctrl = DciMaster.FindById<DciTrackCtrl>(ctrlId);
            if (ctrl == null) return;

            ctrl.HasTray   = present;
            ctrl.LuggNum   = luggNum;
            ctrl.Status    = error > 0 ? 2 : (present ? 1 : 0);
            InvalidateControl(ctrl);
        }

        /// <summary>SC 상태를 DCI 컨트롤에 반영.</summary>
        public void UpdateScStatus(int scNum, ScState state, int row, int col, bool loaded)
        {
            long ctrlId = MakeScCtrlId(scNum);
            var ctrl = DciMaster.FindById<DciRackCtrl>(ctrlId);
            if (ctrl == null) return;

            ctrl.SetCellState(row - 1, col - 1, loaded ? 1 : 0);
            InvalidateControl(ctrl);
        }

        /// <summary>RTV 상태를 DCI 컨트롤에 반영.</summary>
        public void UpdateRtvStatus(int rtvNum, int pos, bool loaded, bool busy)
        {
            long ctrlId = MakeRtvCtrlId(rtvNum);
            var ctrl = DciMaster.FindById<DciRvCtrl>(ctrlId);
            if (ctrl == null) return;

            ctrl.Loaded  = loaded;
            ctrl.Status  = busy ? 1 : 0;
            InvalidateControl(ctrl);
        }

        // ─── 컨트롤 ID 생성 규칙 (EcsDefine.xml의 cid 값과 매핑) ─────

        private static long MakeTrackCtrlId(int cvNum, int trackNum)
            => 111000000L + cvNum * 100000L + trackNum;

        private static long MakeScCtrlId(int scNum)
            => 112000000L + scNum * 1000L;

        private static long MakeRtvCtrlId(int rtvNum)
            => 113000000L + rtvNum * 1000L;

        // ─── 렌더링 요청 ─────────────────────────────────────────────

        public event Action? InvalidateRequested;

        private void InvalidateControl(DciControl ctrl)
        {
            InvalidateRequested?.Invoke();
        }

        public void InvalidateAll() => InvalidateRequested?.Invoke();
    }
}
