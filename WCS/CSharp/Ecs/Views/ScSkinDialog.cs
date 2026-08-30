using System.Collections.Generic;
using System.Windows;

namespace Ecs.Views
{
    /// <summary>
    /// C++ CScSkinDlg(IDD_SKIN_SC_CTRL, 327x446 DLU) 절대좌표 재현 — 스태커크레인 상태창.
    /// </summary>
    public class ScSkinDialog : SkinDialogBase
    {
        private const string TABLE = "SC_DATA_LGLS";

        public ScSkinDialog(EcsDoc doc, int scNo) : base(doc, scNo, $"스태커 크레인 {scNo % 100:00}")
        {
            SetClient(327, 446);

            Fld("title", 7, 7, 315, 12, center: true);

            // ── SC STATUS ──
            Grp("SC STATUS", 7, 30, 314, 212);
            Grp("ITEM", 7, 41, 57, 88);   Grp("VALUE", 64, 41, 176, 88);
            Lbl("JOB NO", 13, 49, 44, 12);    Fld("job_no", 73, 49, 159, 12);
            Lbl("JOB TYP", 13, 62, 44, 12);   Fld("job_typ", 73, 62, 159, 12);
            Lbl("START POS", 13, 75, 44, 12); Fld("start_pos", 73, 75, 159, 13);
            Lbl("DEST POS", 13, 88, 44, 12);  Fld("dest_pos", 73, 88, 159, 12);
            Lbl("현재위치", 13, 101, 44, 12);  Fld("cur", 73, 101, 159, 12);
            Lbl("완료위치", 13, 114, 44, 12);  Fld("cmp", 73, 114, 159, 12);

            // 상태(2열): 좌 label 14/val 72(44), 우 label 129/val 187(44)
            Grp("ITEM", 7, 142, 57, 100);  Grp("VALUE", 64, 142, 60, 100);
            Grp("ITEM", 123, 142, 57, 100); Grp("VALUE", 180, 142, 60, 100);
            Lbl("지상반", 14, 156, 44, 12);   Fld("rc_mode", 72, 156, 44, 12);
            Lbl("화물유무", 128, 155, 44, 12); Fld("prod_load", 187, 155, 44, 12);
            Lbl("기상반", 13, 171, 44, 12);   Fld("sc_mode", 72, 172, 44, 12);
            Lbl("포크위치", 129, 171, 44, 12); Fld("fork_pos", 187, 171, 44, 12);
            Lbl("SRC 상태", 13, 187, 44, 12); Fld("rc_status", 72, 188, 44, 12);
            Lbl("수직주행", 129, 203, 44, 12); Fld("pos_v", 186, 203, 44, 12);
            Lbl("동작상태", 13, 204, 44, 12);  Fld("sc_status", 72, 204, 44, 12);
            Lbl("완료상태", 13, 220, 44, 12);  Fld("complete", 72, 220, 44, 12);
            Lbl("수평주행", 129, 219, 44, 12); Fld("pos_h", 187, 219, 44, 12);

            // ── COMMAND (우측 세로) ──
            Grp("COMMAND", 239, 41, 82, 201);
            Btn("CONFIRM",     244, 54, 72, 12, () => Cmd("FCMP", "강제완료 명령을 전송할까요?"));
            Btn("EMEGENCY",    244, 72, 72, 12, () => Cmd("EMERGENCY", "비상정지 명령을 전송할까요?"));
            Btn("A C T I V E", 244, 90, 72, 12, () => Cmd("ACTIVE", "ACTIVE 명령을 전송할까요?"));
            Btn("STOP",        244, 107, 72, 12, () => Cmd("PAUSE", "정지(STOP) 명령을 전송할까요?"));
            Btn("ERROR RESET", 244, 128, 72, 12, () => Cmd("RESET", "에러리셋 명령을 전송할까요?"));
            Btn("DELETE",      244, 145, 72, 12, () => Cmd("DELFK1", "작업(Fork1)을 삭제할까요?"));
            Btn("MANUAL RET",  243, 163, 72, 12, () => Info("MANUAL RET"));
            Btn("지시 재전송", 243, 180, 72, 12, () => Cmd("RESEND", "지시를 재전송할까요?"));
            var zoom = Btn("확대", 243, 197, 72, 12, () => { });
            SetZoomButton(zoom);

            // ── ERROR / SUSPEND ──
            Grp("ERROR_INFORMATION", 7, 245, 233, 29); Fld("err", 12, 256, 222, 12, center: true);
            Grp("SC_SUSPEND", 239, 245, 81, 29);       Fld("suspend", 244, 256, 71, 12, center: true);

            // ── JOB_STATUS ──
            Grp("JOB_STATUS", 7, 278, 313, 148);
            Grp("ITEM", 7, 290, 59, 136); Grp("VALUE", 66, 290, 176, 136);
            Lbl("JOB NO", 14, 306, 44, 12);     Fld("j_lugg", 71, 306, 159, 12);
            Lbl("JOB TYP", 14, 323, 44, 12);    Fld("j_typ", 71, 323, 159, 13);
            Lbl("JOB STATUS", 15, 340, 44, 12); Fld("j_status", 71, 340, 159, 12);
            Lbl("START POS", 15, 356, 44, 12);  Fld("j_spos", 71, 357, 159, 13);
            Lbl("START LOC", 19, 373, 40, 12);  Fld("j_sloc", 71, 374, 159, 12);
            Lbl("DEST POS", 15, 390, 44, 12);   Fld("j_dpos", 71, 390, 159, 12);
            Lbl("DEST LOC", 19, 406, 40, 12);   Fld("j_dloc", 71, 407, 159, 12);
            Grp("COMMAND", 241, 290, 79, 136);
            Btn("STO SUSPEND", 245, 306, 72, 13, () => Cmd("STOSUSP", "입고보류로 설정할까요?"));
            Btn("RET SUSPEND", 245, 324, 72, 13, () => Cmd("RETSUSP", "출고보류로 설정할까요?"));
            Btn("ALL SUSPEND", 245, 341, 72, 13, () => Cmd("ALLSUSP", "전체보류로 설정할까요?"));

            // 확대 패널(차량 핸드셰이크 + 위치)
            var g = ExtraGroup("차량 상태");
            FillLeds(g, new (string, string)[]
            {
                ("적재완료", "load_complete_rd"), ("적재ACK", "load_complete_ack_od"),
                ("하역완료", "unload_complete_rd"), ("하역ACK", "unload_complete_ack_od"),
                ("반송요청", "transfer_request_od"), ("반송ACK", "transfer_ack_rd"),
                ("화물존재", "sensor_fk_rd"), ("알람SET", "alarm_set_report_rd"),
                ("알람SET ACK", "alarm_set_report_ack_od"), ("알람RST", "alarm_reset_report_rd"),
                ("알람RST ACK", "alarm_reset_report_ack_od"),
            }, cols: 1);
            AddExtraKV();

            StartPolling();
        }

        // 확대 패널 위치/팔레트 값 필드
        private void AddExtraKV()
        {
            var g = ExtraGroup("위치 / 팔레트");
            g.ColumnDefinitions.Add(new System.Windows.Controls.ColumnDefinition { Width = new GridLength(70) });
            g.ColumnDefinitions.Add(new System.Windows.Controls.ColumnDefinition { Width = new GridLength(130) });
            string[,] rows = { { "현재위치", "loc_cur" }, { "지시출발", "loc_from" }, { "지시도착", "loc_to" },
                               { "완료위치", "loc_comp" }, { "팔레트ID", "pallet_id" }, { "알람코드", "alarm_code" } };
            for (int i = 0; i < rows.GetLength(0); i++)
            {
                g.RowDefinitions.Add(new System.Windows.Controls.RowDefinition());
                var lbl = new System.Windows.Controls.TextBlock { Text = rows[i, 0], Margin = new Thickness(4, 2, 6, 2) };
                System.Windows.Controls.Grid.SetRow(lbl, i); System.Windows.Controls.Grid.SetColumn(lbl, 0); g.Children.Add(lbl);
                var val = new System.Windows.Controls.TextBlock { Text = "-", Margin = new Thickness(2, 2, 4, 2) };
                System.Windows.Controls.Grid.SetRow(val, i); System.Windows.Controls.Grid.SetColumn(val, 1); g.Children.Add(val);
                _kv[rows[i, 1]] = val;
            }
        }
        private readonly Dictionary<string, System.Windows.Controls.TextBlock> _kv = new();
        private void SetKV(string k, string v) { if (_kv.TryGetValue(k, out var t)) t.Text = string.IsNullOrEmpty(v) ? "-" : v; }

        private void Info(string what)
            => MessageBox.Show(this, $"[{what}]는 C++ 원본과 동일 위치에 배치되어 있습니다.", Title,
                               MessageBoxButton.OK, MessageBoxImage.Information);

        private void Cmd(string cmdId, string confirm)
        {
            if (!Confirm(confirm)) return;
            bool ok = _doc.Db != null && _doc.Db.SendEquipCommand(TABLE, "sc_no", _no.ToString(), cmdId);
            MessageBox.Show(this, ok ? $"명령({cmdId}) 전송 성공" : $"명령({cmdId}) 전송 실패", Title,
                            MessageBoxButton.OK, ok ? MessageBoxImage.Information : MessageBoxImage.Warning);
            Refresh();
        }

        protected override void Refresh()
        {
            var r = _doc.Db?.SelectScStatus(_no);
            if (r == null) { Set("title", "조회안됨"); return; }

            string nm = G(r, "mc_no_nm");
            Set("title", $"스태커 크레인 {_no % 100:00}" + (string.IsNullOrEmpty(nm) ? "" : $"  ({nm})"));
            Set("job_no", Dash(G(r, "itn_lugg_fk1")));
            Set("job_typ", _doc.JobTypName(G(r, "job_typ_rd")));
            Set("start_pos", Trio(G(r, "start_bank_fk1_rd"), G(r, "start_bay_fk1_rd"), G(r, "start_level_fk1_rd")));
            Set("dest_pos", Trio(G(r, "dest_bank_fk1_rd"), G(r, "dest_bay_fk1_rd"), G(r, "dest_level_fk1_rd")));
            Set("cur", Trio(G(r, "location_01_rd"), G(r, "location_02_rd"), G(r, "location_03_rd")));
            Set("cmp", Trio(G(r, "transfer_complete_location_01_rd"), G(r, "transfer_complete_location_02_rd"), G(r, "transfer_complete_location_03_rd")));
            Set("rc_mode", OnOff(G(r, "online_mode_rd"), "온라인", "오프라인"));
            Set("sc_mode", OnOff(G(r, "auto_mode_rd"), "자동", "수동"));
            Set("rc_status", OnOff(G(r, "active_mode_rd"), "동작", "정지"));
            Set("sc_status", Pick(G(r, "subsystem_status_rd"), "정지", ("2", "동작"), ("1", "대기")));
            Set("complete", OnOff(G(r, "complete_rd"), "완료", "-"));
            Set("prod_load", OnOff(G(r, "sensor_fk_rd"), "있음", "없음"));
            Set("fork_pos", Pick(G(r, "forkpos_fk1_rd"), "CENTER", ("1", "좌측"), ("2", "우측")));
            Set("pos_h", G(r, "pos_h_rd"));
            Set("pos_v", G(r, "pos_v_rd"));
            Set("err", Dash(G(r, "err_code_rd")));
            Set("suspend", Pick(G(r, "suspend"), "-", ("1", "입고보류"), ("2", "출고보류"), ("3", "전체보류")));

            var j = _doc.Db?.SelectJobByLugg(_whTyp, G(r, "itn_lugg_fk1"));
            if (j == null)
                foreach (var k in new[] { "j_lugg", "j_typ", "j_status", "j_spos", "j_sloc", "j_dpos", "j_dloc" }) Set(k, "-");
            else
            {
                Set("j_lugg", G(j, "LUGG_NO")); Set("j_typ", _doc.JobTypName(G(j, "JOB_TYP")));
                Set("j_status", _doc.JobStatusName(G(j, "JOB_STATUS")));
                Set("j_spos", G(j, "START_POS")); Set("j_sloc", G(j, "START_LOCATION"));
                Set("j_dpos", G(j, "DEST_POS")); Set("j_dloc", G(j, "DEST_LOCATION"));
            }

            foreach (var k in new[] { "load_complete_rd", "load_complete_ack_od", "unload_complete_rd",
                "unload_complete_ack_od", "transfer_request_od", "transfer_ack_rd", "sensor_fk_rd",
                "alarm_set_report_rd", "alarm_set_report_ack_od", "alarm_reset_report_rd", "alarm_reset_report_ack_od" })
                SetLed(k, On(r, k));
            SetKV("loc_cur", Trio(G(r, "location_01_rd"), G(r, "location_02_rd"), G(r, "location_03_rd")));
            SetKV("loc_from", Trio(G(r, "from_01_od"), G(r, "from_02_od"), G(r, "from_03_od")));
            SetKV("loc_to", Trio(G(r, "to_01_od"), G(r, "to_02_od"), G(r, "to_03_od")));
            SetKV("loc_comp", Trio(G(r, "transfer_complete_location_01_rd"), G(r, "transfer_complete_location_02_rd"), G(r, "transfer_complete_location_03_rd")));
            SetKV("pallet_id", Dash(G(r, "pallet_id_od")));
            SetKV("alarm_code", Dash(G(r, "alarm_set_code_rd")));
        }
    }
}
