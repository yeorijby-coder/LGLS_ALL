using System.Collections.Generic;
using System.Windows;

namespace Ecs.Views
{
    /// <summary>
    /// C++ CRtvSkinDlg(IDD_SKIN_RTV_CTRL, 353x438 DLU) 절대좌표 재현 — RTV/RGV 상태창.
    /// </summary>
    public class RtvSkinDialog : SkinDialogBase
    {
        private const string TABLE = "RTV_DATA_LGLS";

        public RtvSkinDialog(EcsDoc doc, int rtvNo) : base(doc, rtvNo, $"RGV {rtvNo % 100}호기")
        {
            SetClient(353, 438);

            Fld("title", 7, 7, 339, 14, center: true);

            // ── RTV STATUS ──
            Grp("RTV STATUS", 7, 26, 339, 230);
            Grp("ITEM", 7, 37, 64, 84); Grp("FORK1", 71, 37, 96, 84); Grp("FORK2", 167, 37, 91, 84);
            Lbl("작업번호", 14, 53, 53, 10, 'c'); Fld("job_no1", 77, 51, 85, 12); Fld("job_no2", 174, 51, 84, 12);
            Lbl("출발위치", 14, 68, 54, 10, 'c'); Fld("spos1", 77, 67, 85, 12);  Fld("spos2", 174, 67, 84, 14);
            Lbl("도착위치", 14, 88, 53, 10, 'c'); Fld("dpos1", 77, 85, 85, 12);  Fld("dpos2", 174, 85, 84, 12);
            Lbl("작업구분", 14, 106, 53, 10, 'c'); Fld("jtyp1", 77, 103, 85, 12); Fld("jtyp2", 174, 103, 84, 12);

            Grp("ITEM", 7, 125, 64, 68); Grp("RTV STATUS", 71, 125, 187, 68);
            Lbl("운영모드", 8, 140, 54, 12, 'c'); Fld("mode", 66, 140, 82, 12);
            Lbl("RTV상태", 150, 140, 54, 12, 'c'); Fld("status", 206, 140, 50, 12);
            Lbl("완료상태", 8, 158, 54, 12, 'c'); Fld("complete", 66, 158, 82, 12);
            Lbl("ACTIVE", 150, 158, 54, 12, 'c'); Fld("active", 206, 158, 50, 12);
            Lbl("화물유무", 8, 176, 54, 12, 'c'); Fld("prodload", 66, 176, 82, 12);
            Lbl("주행위치", 150, 176, 54, 12, 'c'); Fld("pos_h", 206, 176, 50, 12);

            // ── COMMAND (우측 세로) ──
            Grp("COMMAND", 258, 36, 88, 219);
            Btn("강제완료", 270, 69, 65, 14, () => Cmd("FCMP", "강제완료 명령을 전송할까요?"));
            Btn("비상정지", 270, 84, 65, 15, () => Cmd("EMERGENCY", "비상정지 명령을 전송할까요?"));
            Btn("ACTIVE", 270, 100, 65, 15, () => Cmd("ACTIVE", "ACTIVE 명령을 전송할까요?"));
            Btn("정지", 270, 116, 65, 15, () => Cmd("PAUSE", "정지 명령을 전송할까요?"));
            Btn("에러해제", 270, 132, 65, 15, () => Cmd("RESET", "에러해제 명령을 전송할까요?"));
            Btn("복귀명령", 270, 148, 65, 15, () => Cmd("CTH", "복귀명령을 전송할까요?"));
            Chk("1 FORK", 274, 183, 59, 9); Chk("2 FORK", 274, 195, 59, 9); Chk("1,2 FORK", 274, 207, 62, 9);
            Btn("삭제", 270, 222, 65, 15, () => Cmd("DELFK1", "작업을 삭제할까요?"));

            // ── JOB_STATUS ──
            Grp("JOB_STATUS", 7, 232, 339, 124);
            Grp("ITEM", 7, 244, 63, 108); Grp("FORK1", 71, 244, 180, 108); Grp("COMMAND", 259, 244, 87, 108);
            Lbl("작업번호", 14, 248, 52, 10, 'c'); Fld("j_lugg", 76, 248, 175, 12);
            Lbl("작업구분", 14, 264, 52, 10, 'c'); Fld("j_typ", 76, 264, 175, 12);
            Lbl("작업상태", 14, 280, 52, 10, 'c'); Fld("j_status", 76, 280, 175, 12);
            Lbl("출발위치", 14, 296, 52, 10, 'c'); Fld("j_spos", 76, 296, 175, 12);
            Lbl("출발지", 14, 312, 52, 10, 'c');   Fld("j_sloc", 76, 312, 175, 12);
            Lbl("도착위치", 14, 328, 52, 10, 'c'); Fld("j_dpos", 76, 328, 175, 12);
            Lbl("도착지", 14, 344, 52, 10, 'c');   Fld("j_dloc", 76, 344, 175, 12);
            Btn("RTV SUSPEND", 270, 321, 65, 15, () => Cmd("SUSPEND", "보류로 설정할까요?"));

            // ── ERROR / SUSPEND ──
            Grp("ERROR INFORMATION", 7, 361, 339, 34); Fld("diag", 7, 373, 339, 12);
            Grp("RTV SUSPEND", 8, 394, 338, 34);       Fld("suspend", 8, 407, 338, 12);

            StartPolling();
        }

        private void Cmd(string cmdId, string confirm)
        {
            if (!Confirm(confirm)) return;
            bool ok = _doc.Db != null && _doc.Db.SendEquipCommand(TABLE, "rtv_no", _no.ToString(), cmdId, hasParm: false);
            MessageBox.Show(this, ok ? $"명령({cmdId}) 전송 성공" : $"명령({cmdId}) 전송 실패", Title,
                            MessageBoxButton.OK, ok ? MessageBoxImage.Information : MessageBoxImage.Warning);
            Refresh();
        }

        protected override void Refresh()
        {
            var r = _doc.Db?.SelectRtvStatus(_no);
            if (r == null) { Set("title", "조회안됨"); return; }

            Set("title", $"RGV {_no % 100}호기");
            string lugg = G(r, "pallet_on_vehicle_rd");
            if (string.IsNullOrEmpty(lugg) || lugg == "0") lugg = G(r, "lugg_od");
            Set("job_no1", Dash(lugg)); Set("job_no2", "-");
            Set("spos1", Trio(G(r, "from_01_od"), G(r, "from_02_od"), G(r, "from_03_od"))); Set("spos2", "-");
            Set("dpos1", Trio(G(r, "to_01_od"), G(r, "to_02_od"), G(r, "to_03_od"))); Set("dpos2", "-");
            Set("jtyp1", _doc.JobTypName(G(r, "job_typ_od"))); Set("jtyp2", "-");

            Set("mode", OnOff(G(r, "auto_mode_rd"), "자동", "수동"));
            Set("status", Pick(G(r, "subsystem_status_rd"), "DOWN", ("2", "RUN"), ("1", "IDLE")));
            Set("complete", OnOff(G(r, "COMPLETE_RD"), "완료", "-"));
            Set("active", Pick(G(r, "subsystem_status_rd"), "대기", ("2", "동작")));
            Set("prodload", OnOff(G(r, "sensor_rtv_rd"), "있음", "없음"));
            Set("pos_h", G(r, "pos_h_rd"));

            string err = G(r, "err_code_rd");
            Set("diag", (string.IsNullOrEmpty(err) || err == "0" || err == "0000") ? "정상" : err);
            Set("suspend", Pick(G(r, "suspend"), "-", ("1", "입고보류"), ("2", "출고보류"), ("3", "전체보류")));

            var j = _doc.Db?.SelectJobByLugg(_whTyp, lugg);
            if (j == null)
                foreach (var k in new[] { "j_lugg", "j_typ", "j_status", "j_spos", "j_sloc", "j_dpos", "j_dloc" }) Set(k, "-");
            else
            {
                Set("j_lugg", G(j, "LUGG_NO")); Set("j_typ", _doc.JobTypName(G(j, "JOB_TYP")));
                Set("j_status", _doc.JobStatusName(G(j, "JOB_STATUS")));
                Set("j_spos", G(j, "START_POS")); Set("j_sloc", G(j, "START_LOCATION"));
                Set("j_dpos", G(j, "DEST_POS")); Set("j_dloc", G(j, "DEST_LOCATION"));
            }
        }
    }
}
