using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using Ecs.Job;

namespace Ecs.Db
{
    /// <summary>
    /// C++ CAdoDB + ManualJob/ViewJobListDlg의 JOB_MST SQL을 이식한 C# DB 접근 계층.
    /// MS SQL Server(현행 LGLS: DB=LGLS_MCS_IO)를 System.Data.SqlClient로 접속.
    /// C++는 %s 문자열 결합이었으나 여기서는 파라미터 바인딩으로 안전하게 처리.
    /// DB 미접속 시에도 앱이 죽지 않도록 각 메서드는 실패를 bool/빈목록으로 반환.
    /// </summary>
    public class EcsDb
    {
        private readonly string _connStr;
        public bool   IsConnected { get; private set; }
        public string LastError   { get; private set; } = string.Empty;

        public EcsDb(string server, string database, string user, string pwd, int timeoutSec = 5)
        {
            _connStr = $"Server={server};Database={database};User Id={user};Password={pwd};" +
                       $"Connect Timeout={timeoutSec};";
        }

        /// <summary>접속 테스트. 실패해도 예외를 던지지 않고 false 반환(앱 계속 동작).</summary>
        public bool TestConnect()
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                IsConnected = true;
                LastError = string.Empty;
                return true;
            }
            catch (Exception ex)
            {
                IsConnected = false;
                LastError = ex.Message;
                return false;
            }
        }

        // ─── JOB_TYP 코드 매핑 (C++ ManualJob 규칙) ──────────────────
        public static string JobTypCode(JobType t) => t switch
        {
            JobType.Store      => "1",
            JobType.Retrieve   => "2",
            JobType.HsToHs     => "3",
            JobType.RackToRack => "4",
            _                  => "1",
        };
        private static JobType JobTypFromCode(string code) => code switch
        {
            "2" => JobType.Retrieve,
            "3" => JobType.HsToHs,
            "4" or "5" => JobType.RackToRack,
            "6" => JobType.HsToHs,
            _   => JobType.Store,
        };
        private static string Loc(int sta, int row, int col) => $"{sta:00}-{row:000}-{col:00}";

        // ─── INSERT (수동 작업 생성; JOB_STATUS='99' 신규, INS_DT=GETDATE) ─
        public bool InsertJob(JobItem j, string whTyp, string insUser = "MANUAL")
        {
            const string sql =
                "INSERT INTO JOB_MST " +
                "(WH_TYP, LUGG_NO, START_POS, START_LOCATION, DEST_POS, DEST_LOCATION, " +
                " PRODUCT_SIZE, JOB_TYP, JOB_STATUS, JOB_PRIORITY, INS_DT, INS_USER_ID, REMARKS, WC_STEP) " +
                "VALUES (@wh, @lugg, @spos, @sloc, @dpos, @dloc, @psize, @jtyp, '99', @pri, GETDATE(), @user, @remark, '0')";
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(sql, c);
                cmd.Parameters.AddWithValue("@wh",     whTyp);
                cmd.Parameters.AddWithValue("@lugg",   j.LuggNum.ToString());
                cmd.Parameters.AddWithValue("@spos",   j.SrcStation.ToString());
                cmd.Parameters.AddWithValue("@sloc",   Loc(j.SrcStation, j.SrcRow, j.SrcCol));
                cmd.Parameters.AddWithValue("@dpos",   j.DstStation.ToString());
                cmd.Parameters.AddWithValue("@dloc",   Loc(j.DstStation, j.DstRow, j.DstCol));
                cmd.Parameters.AddWithValue("@psize",  string.Empty);
                cmd.Parameters.AddWithValue("@jtyp",   JobTypCode(j.JobType));
                cmd.Parameters.AddWithValue("@pri",    j.Priority.ToString());
                cmd.Parameters.AddWithValue("@user",   insUser);
                cmd.Parameters.AddWithValue("@remark", j.Tray?.Barcode ?? string.Empty);
                cmd.ExecuteNonQuery();
                IsConnected = true;
                return true;
            }
            catch (Exception ex) { LastError = ex.Message; IsConnected = false; return false; }
        }

        // ─── SELECT (창고별 작업 목록) ──────────────────────────────
        public List<JobItem> SelectJobs(string whTyp)
        {
            var list = new List<JobItem>();
            const string sql =
                "SELECT WH_TYP, LUGG_NO, START_LOCATION, DEST_LOCATION, JOB_TYP, JOB_STATUS, JOB_PRIORITY " +
                "FROM JOB_MST WHERE WH_TYP=@wh ORDER BY LUGG_NO DESC";
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(sql, c);
                cmd.Parameters.AddWithValue("@wh", whTyp);
                using var rd = cmd.ExecuteReader();
                while (rd.Read())
                {
                    int.TryParse(rd["LUGG_NO"]?.ToString(), out int lugg);
                    int.TryParse(rd["JOB_PRIORITY"]?.ToString(), out int pri);
                    string rawTyp = rd["JOB_TYP"]?.ToString() ?? "1";
                    string rawSta = rd["JOB_STATUS"]?.ToString() ?? "99";
                    var item = new JobItem
                    {
                        LuggNum      = lugg,
                        JobType      = JobTypFromCode(rawTyp),
                        Priority     = pri,
                        Status       = MapStatus(rawSta),
                        RawJobTyp    = rawTyp,
                        RawJobStatus = rawSta,
                        TrayId       = (rd["START_LOCATION"]?.ToString() ?? "") + " → " + (rd["DEST_LOCATION"]?.ToString() ?? ""),
                    };
                    list.Add(item);
                }
                IsConnected = true;
            }
            catch (Exception ex) { LastError = ex.Message; IsConnected = false; }
            return list;
        }

        private static JobStatus MapStatus(string code)
        {
            if (code == "99") return JobStatus.Wait;
            if (int.TryParse(code, out int n))
            {
                if (n == 29) return JobStatus.Done;
                if (n > 20 && n < 30) return JobStatus.Moving;
            }
            return JobStatus.Wait;
        }

        // ─── DELETE ─────────────────────────────────────────────────
        public bool DeleteJob(string whTyp, int luggNo)
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand("DELETE FROM JOB_MST WHERE WH_TYP=@wh AND LUGG_NO=@lugg", c);
                cmd.Parameters.AddWithValue("@wh", whTyp);
                cmd.Parameters.AddWithValue("@lugg", luggNo.ToString());
                cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex) { LastError = ex.Message; return false; }
        }

        // ─── COMMON_CODE 로드 (코드→한글) ──────────────────────────
        public bool LoadCommonCodes(CommonCode dst)
        {
            dst.Clear();
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(
                    "SELECT cdx_cd, ccd_cd, wh_typ, ccd_nm_kor FROM COMMON_CODE", c);
                using var rd = cmd.ExecuteReader();
                while (rd.Read())
                    dst.Set(rd[0]?.ToString() ?? "", rd[1]?.ToString() ?? "",
                            rd[2]?.ToString() ?? "", rd[3]?.ToString() ?? "");
                return true;
            }
            catch (Exception ex) { LastError = ex.Message; return false; }
        }

        // ─── 완료 처리(JOB_STATUS='29') ─────────────────────────────
        public bool CompleteJob(string whTyp, int luggNo)
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(
                    "UPDATE JOB_MST SET JOB_STATUS='29' WHERE WH_TYP=@wh AND LUGG_NO=@lugg", c);
                cmd.Parameters.AddWithValue("@wh", whTyp);
                cmd.Parameters.AddWithValue("@lugg", luggNo.ToString());
                cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex) { LastError = ex.Message; return false; }
        }

        // ─── 설비 상태창(SkinDlg)용: 한 행을 컬럼명→문자열 딕셔너리로 조회 ───
        //     C++ Cv/Sc/RtvSkinDlg의 InvalidateXxxData() SELECT에 대응.
        private Dictionary<string, string>? SelectRow(string sql, params (string name, object val)[] ps)
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(sql, c);
                foreach (var p in ps) cmd.Parameters.AddWithValue(p.name, p.val);
                using var rd = cmd.ExecuteReader();
                if (!rd.Read()) { IsConnected = true; return null; }
                var d = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
                for (int i = 0; i < rd.FieldCount; i++)
                {
                    object v = rd.GetValue(i);
                    d[rd.GetName(i)] = v == null || v is DBNull ? "" : v.ToString().Trim();
                }
                IsConnected = true;
                return d;
            }
            catch (Exception ex) { LastError = ex.Message; IsConnected = false; return null; }
        }

        /// <summary>SC 상태(SC_DATA_LGLS) 1행. 레이아웃 ID 1712NNNN → sc_no=NNNN.</summary>
        public Dictionary<string, string>? SelectScStatus(int scNo)
            => SelectRow("SELECT * FROM SC_DATA_LGLS WHERE sc_no=@no", ("@no", scNo.ToString()));

        /// <summary>RTV 상태(RTV_DATA_LGLS) 1행. 레이아웃 ID 1713NNNN → rtv_no=NNNN.</summary>
        public Dictionary<string, string>? SelectRtvStatus(int rtvNo)
            => SelectRow("SELECT * FROM RTV_DATA_LGLS WHERE rtv_no=@no", ("@no", rtvNo.ToString()));

        /// <summary>CV 상태(cv_data) 1행. 레이아웃 ID 1711NNNN → mc_no=NNNN.</summary>
        public Dictionary<string, string>? SelectCvStatus(int mcNo)
            => SelectRow("SELECT * FROM cv_data WHERE mc_no=@no", ("@no", mcNo.ToString()));

        /// <summary>작업정보 그룹용: LUGG_NO로 JOB_MST 1행.</summary>
        public Dictionary<string, string>? SelectJobByLugg(string whTyp, string luggNo)
        {
            if (string.IsNullOrEmpty(luggNo) || luggNo == "0" || luggNo == "0000") return null;
            return SelectRow(
                "SELECT TOP 1 * FROM JOB_MST WHERE WH_TYP=@wh AND LUGG_NO=@lugg",
                ("@wh", whTyp), ("@lugg", luggNo));
        }

        // ─── 메뉴 조회창(그리드)용 범용 헬퍼 ──────────────────────
        /// <summary>임의 SELECT → DataTable(그리드 바인딩용).</summary>
        public DataTable SelectTable(string sql, params (string name, object val)[] ps)
        {
            var dt = new DataTable();
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(sql, c);
                foreach (var p in ps) cmd.Parameters.AddWithValue(p.name, p.val);
                using var da = new SqlDataAdapter(cmd);
                da.Fill(dt);
                IsConnected = true;
            }
            catch (Exception ex) { LastError = ex.Message; IsConnected = false; }
            return dt;
        }

        /// <summary>임의 UPDATE/DELETE/INSERT 실행. 영향 행수>=0이면 true.</summary>
        public bool ExecNonQuery(string sql, params (string name, object val)[] ps)
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                using var cmd = new SqlCommand(sql, c);
                foreach (var p in ps) cmd.Parameters.AddWithValue(p.name, p.val);
                cmd.ExecuteNonQuery();
                IsConnected = true;
                return true;
            }
            catch (Exception ex) { LastError = ex.Message; IsConnected = false; return false; }
        }

        /// <summary>설비 명령 전송: cmd_rq_id 설정 + cmd_rq_yn='Y' (C++ UpdateScData 등).</summary>
        public bool SendEquipCommand(string table, string keyCol, string keyVal, string cmdId,
                                     string parm = "", bool hasParm = true)
        {
            try
            {
                using var c = new SqlConnection(_connStr);
                c.Open();
                string set = hasParm
                    ? "cmd_rq_id=@cid, cmd_rq_yn='Y', cmd_rq_parm=@parm, write_upd_dt=GETDATE()"
                    : "cmd_rq_id=@cid, cmd_rq_yn='Y', write_upd_dt=GETDATE()";
                using var cmd = new SqlCommand($"UPDATE {table} SET {set} WHERE {keyCol}=@key", c);
                cmd.Parameters.AddWithValue("@cid", cmdId);
                if (hasParm) cmd.Parameters.AddWithValue("@parm", parm);
                cmd.Parameters.AddWithValue("@key", keyVal);
                int n = cmd.ExecuteNonQuery();
                return n > 0;
            }
            catch (Exception ex) { LastError = ex.Message; return false; }
        }
    }
}
