using System.Collections.Generic;
using System.Windows;

namespace Ecs.Views
{
    /// <summary>
    /// C++ CCvSkinDlg(IDD_SKIN_CV_CTRL1, 366x453 DLU) 절대좌표 재현 — 컨베이어/트랙 상태창.
    /// </summary>
    public class CvSkinDialog : SkinDialogBase
    {
        private const string TABLE = "cv_data";

        public CvSkinDialog(EcsDoc doc, int mcNo) : base(doc, mcNo, "TRACK")
        {
            SetClient(366, 453);

            // 상단: 트랙번호(좌) + 트랙명칭(중앙)
            Fld("track", 6, 4, 64, 13, center: true);
            Fld("track_nm", 69, 4, 290, 13, center: true);

            // ── CV STATUS ──
            Grp("CV STATUS", 7, 33, 263, 80);
            Grp("ITEM", 7, 45, 64, 69); Grp("VALUE", 71, 45, 200, 69);
            Lbl("JOB NO", 12, 57, 56, 12);   Fld("job_no", 76, 57, 189, 12);
            Lbl("DEST POS", 12, 74, 56, 12); Fld("dest_pos", 76, 74, 189, 13);
            Lbl("JOB TYP", 12, 91, 56, 12);  Fld("job_typ", 76, 91, 189, 12);

            // ── COMMAND (우측 세로) ──
            Grp("COMMAND", 270, 33, 83, 148);
            Btn("WRITE",  277, 45, 73, 14, () => Info("WRITE"));
            Btn("DELETE", 277, 61, 73, 15, () => Cmd("DELETE", "트랙 작업을 삭제할까요?"));
            Btn("COPY",   277, 77, 73, 15, () => Info("COPY"));
            Btn("CUT",    277, 92, 73, 15, () => Info("CUT"));
            Btn("PASTE",  277, 108, 73, 15, () => Info("PASTE"));
            Btn("MG 이동", 277, 125, 73, 14, () => Info("MG 이동"));
            var zoom = Btn("확대", 277, 146, 73, 14, () => { });
            SetZoomButton(zoom);
            Chk("자동조회", 279, 165, 67, 10);

            // 일시정지
            Grp("일시정지", 270, 260, 83, 45);
            Fld("pause", 277, 272, 73, 12, center: true);
            Btn("TRACK PAUSE", 277, 286, 73, 14, () => Cmd("PAUSE", "트랙 일시정지 명령을 전송할까요?"));

            // ── JOB_STATUS ──
            Grp("JOB_STATUS", 7, 117, 263, 189);
            Grp("ITEM", 7, 130, 64, 176); Grp("VALUE", 71, 130, 199, 176);
            Lbl("JOB NO", 12, 143, 56, 12);     Fld("j_lugg", 76, 143, 189, 12);
            Lbl("JOB TYP", 12, 159, 56, 12);    Fld("j_typ", 76, 158, 189, 12);
            Lbl("JOB STATUS", 12, 175, 56, 12); Fld("j_status", 76, 174, 189, 12);
            Lbl("적재 용기", 12, 191, 56, 12);   Fld("j_lot", 76, 191, 189, 12);
            Lbl("제품 정보", 12, 207, 56, 12);   Fld("j_prd", 76, 207, 189, 12);
            Lbl("START POS", 11, 237, 54, 12);  Fld("j_spos", 76, 238, 189, 12);
            Lbl("START LOC", 11, 253, 53, 12);  Fld("j_sloc", 76, 254, 189, 12);
            Lbl("DEST POS", 11, 270, 53, 12);   Fld("j_dpos", 76, 270, 189, 12);
            Lbl("DEST LOC", 11, 287, 53, 12);   Fld("j_dloc", 76, 286, 189, 12);

            // ── 에러 상태 ──
            Grp("에러 상태", 8, 312, 346, 35);
            Lbl("ERROR CODE", 12, 326, 56, 12); Fld("err", 76, 326, 271, 14);

            // ── STATUS (상태 컬러 버튼) ──
            Grp("STATUS", 7, 375, 347, 70);
            StateBtn("팔레트 감지", 12, 385, 110, 16, "sensor0_data_rd");
            StateBtn("출고/입고 모드", 125, 385, 110, 16, "stock_mode");
            StateBtn("수동/자동 모드", 12, 404, 110, 16, "auto_mode_rd");
            StateBtn("입고대 신호", 125, 404, 110, 16, "sto_ready_rd");
            StateBtn("출고대 신호", 239, 404, 110, 16, "ret_ready_rd");
            StateBtn("SC 인터락", 12, 423, 110, 16, "rtv_lock_sensor");
            StateBtn("입고 HS 신호", 125, 423, 110, 16, "stohs_ready_rd");
            StateBtn("출고 HS 신호", 239, 423, 110, 16, "reths_ready_rd");

            // 확대 패널(차량 트래킹 LED)
            var g = ExtraGroup("차량 상태(트래킹)");
            FillLeds(g, new (string, string)[]
            {
                ("적재완료", "load_complete_rd"), ("적재ACK", "load_complete_ack_od"),
                ("하역완료", "unload_complete_rd"), ("하역ACK", "unload_complete_ack_od"),
                ("출고요청", "unload_request_od"), ("출고ACK", "unload_request_ack_rd"),
                ("입고준비", "in_ready_rd"), ("대기IN", "wait_in_rd"),
                ("대기OUT", "wait_out_rd"), ("자동모드", "auto_mode_rd"), ("화물존재", "sensor0_data_rd"),
            }, cols: 1);

            StartPolling();
        }

        private void Info(string what)
            => MessageBox.Show(this, $"[{what}] 기능은 편집/클립보드 동작으로, C++ 원본과 동일 UI 위치에 배치되어 있습니다.",
                               Title, MessageBoxButton.OK, MessageBoxImage.Information);

        private void Cmd(string cmdId, string confirm)
        {
            if (!Confirm(confirm)) return;
            bool ok = _doc.Db != null && _doc.Db.SendEquipCommand(TABLE, "mc_no", _no.ToString(), cmdId);
            MessageBox.Show(this, ok ? $"명령({cmdId}) 전송 성공" : $"명령({cmdId}) 전송 실패", Title,
                            MessageBoxButton.OK, ok ? MessageBoxImage.Information : MessageBoxImage.Warning);
            Refresh();
        }

        protected override void Refresh()
        {
            var r = _doc.Db?.SelectCvStatus(_no);
            if (r == null) { Set("track_nm", "조회안됨"); return; }

            Set("track", G(r, "mc_no"));
            string nm = G(r, "mc_no_nm");
            Set("track_nm", string.IsNullOrEmpty(nm) ? G(r, "mc_no") : nm);
            if (!string.IsNullOrEmpty(nm)) Title = $"TRACK - {nm}";
            Set("job_no", Dash(G(r, "lugg_no_rd")));
            Set("dest_pos", Dash(G(r, "dest_pos_rd")));
            Set("job_typ", _doc.JobTypName(G(r, "job_typ_rd")));
            Set("pause", OnOff(G(r, "tr_pause_rd"), "정지", "해제"));
            Set("err", Dash(G(r, "error_code")));

            // 상태 컬러 버튼
            foreach (var k in new[] { "sensor0_data_rd", "stock_mode", "auto_mode_rd", "sto_ready_rd",
                "ret_ready_rd", "rtv_lock_sensor", "stohs_ready_rd", "reths_ready_rd" })
                SetState(k, On(r, k));

            // 작업정보(JOB_MST)
            var j = _doc.Db?.SelectJobByLugg(_whTyp, G(r, "lugg_no_rd"));
            if (j == null)
                foreach (var k in new[] { "j_lugg", "j_typ", "j_status", "j_lot", "j_prd", "j_spos", "j_sloc", "j_dpos", "j_dloc" }) Set(k, "-");
            else
            {
                Set("j_lugg", G(j, "LUGG_NO")); Set("j_typ", _doc.JobTypName(G(j, "JOB_TYP")));
                Set("j_status", _doc.JobStatusName(G(j, "JOB_STATUS")));
                Set("j_lot", G(j, "LOT_NO")); Set("j_prd", G(j, "PRODUCT_ID"));
                Set("j_spos", G(j, "START_POS")); Set("j_sloc", G(j, "START_LOCATION"));
                Set("j_dpos", G(j, "DEST_POS")); Set("j_dloc", G(j, "DEST_LOCATION"));
            }

            foreach (var k in new[] { "load_complete_rd", "load_complete_ack_od", "unload_complete_rd",
                "unload_complete_ack_od", "unload_request_od", "unload_request_ack_rd", "in_ready_rd",
                "wait_in_rd", "wait_out_rd", "auto_mode_rd", "sensor0_data_rd" })
                SetLed(k, On(r, k));
        }
    }
}
