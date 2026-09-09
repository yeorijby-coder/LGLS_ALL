using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Text;
using System.Threading;

namespace WcsCommon
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-09-09] cTaskLog : ALL_TASK 공용 로그 싱크
    //
    //   WCS_TASK_CV / IO_TASK / WCS_TASK_HOST 를 한 프로세스(ALL_TASK)로 합치면서
    //   "로그를 스레드별 파일로 날짜별 1년 보관 + DB 에도 적재" 를 한 곳에서 처리한다.
    //   (cLogCols.cs 와 같이 네 프로젝트에 링크로 공유되는 파일이다)
    //
    //   파일 : <RootDir>\<스레드명>\<스레드명>_yyyyMMdd.log   (CP949, CRLF)
    //   DB   : all_task_log  (없으면 최초 1회 자동 생성)
    //
    //   호출 측(기존 CLogging.Write / CLog.WriteLog)은 Write() 한 줄만 덧붙이면 되고,
    //   실제 파일/DB 쓰기는 이 클래스의 백그라운드 스레드가 맡는다. 로그 때문에
    //   통신 스레드가 멈추지 않도록 큐잉만 하고 즉시 반환한다.
    //
    //   ※ 큐가 폭주하면(DB 장애 등) 오래된 항목부터 버린다. 로그가 설비를 멈추게
    //     하는 일은 없어야 한다.
    // ─────────────────────────────────────────────────────────────────────────
    public static class cTaskLog
    {
        public class Item
        {
            public DateTime Dt;
            public string   Task;
            public string   Thread;
            public string   Level;
            public string   Message;
        }

        private const int MAX_QUEUE = 20000;

        private static readonly Queue<Item> m_que = new Queue<Item>();
        private static readonly object      m_lock = new object();

        private static Thread   m_thWriter;
        private static volatile bool m_bRun;

        private static string m_strRootDir  = "";
        private static int    m_nKeepDays   = 365;
        private static string m_strDbConn   = "";
        private static bool   m_bDbEnabled  = false;
        private static bool   m_bDbChecked  = false;

        private static DateTime m_dtLastPurge = DateTime.MinValue;

        /// <summary>버린 로그 건수(큐 폭주). 화면에 표시해 이상을 알 수 있게 한다.</summary>
        public static long DroppedCount { get; private set; }
        /// <summary>DB 적재 실패 건수</summary>
        public static long DbFailCount  { get; private set; }
        /// <summary>마지막 DB 오류 메시지</summary>
        public static string LastDbError = "";

        public static string RootDir  { get { return m_strRootDir; } }
        public static int    KeepDays { get { return m_nKeepDays; } }
        public static bool   DbEnabled { get { return m_bDbEnabled; } }
        public static int    QueueCount { get { lock (m_lock) { return m_que.Count; } } }

        // ─────────────────────────────────────────────────────────────────
        // 기동 / 종료
        // ─────────────────────────────────────────────────────────────────

        /// <summary>
        /// 로그 싱크를 켠다. ALL_TASK 기동 시 1회 호출.
        /// pRootDir 이 비면 &lt;실행파일 폴더&gt;\LOG 를 쓴다.
        /// </summary>
        public static void Start(string pRootDir, int pKeepDays, string pDbConn, bool pDbEnabled)
        {
            lock (m_lock)
            {
                if (string.IsNullOrEmpty(pRootDir))
                    pRootDir = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "LOG");

                m_strRootDir = pRootDir;
                m_nKeepDays  = (pKeepDays > 0) ? pKeepDays : 365;
                m_strDbConn  = (pDbConn == null) ? "" : pDbConn;
                m_bDbEnabled = pDbEnabled && m_strDbConn.Length > 0;
                m_bDbChecked = false;

                try { Directory.CreateDirectory(m_strRootDir); }
                catch { }

                if (m_thWriter != null && m_thWriter.IsAlive)
                    return;

                m_bRun = true;
                m_thWriter = new Thread(WriterLoop);
                m_thWriter.IsBackground = true;
                m_thWriter.Name = "cTaskLogWriter";
                m_thWriter.Start();
            }
        }

        /// <summary>남은 큐를 비우고 쓰기 스레드를 멈춘다.</summary>
        public static void Shutdown()
        {
            m_bRun = false;
            Thread th = m_thWriter;
            m_thWriter = null;
            try
            {
                if (th != null && th.IsAlive)
                    th.Join(3000);
            }
            catch { }
            try { DrainOnce(); } catch { }
        }

        // ─────────────────────────────────────────────────────────────────
        // 적재
        // ─────────────────────────────────────────────────────────────────

        public static void Write(string pTask, string pThread, string pLevel, string pMessage)
        {
            if (string.IsNullOrEmpty(m_strRootDir))
                return;                                  // Start() 전이면 조용히 무시

            Item it = new Item();
            it.Dt      = DateTime.Now;
            it.Task    = Sanitize(pTask,   "TASK");
            it.Thread  = Sanitize(pThread, "MAIN");
            it.Level   = string.IsNullOrEmpty(pLevel) ? "INFO" : pLevel;
            it.Message = (pMessage == null) ? "" : pMessage.Replace("\r", " ").Replace("\n", " ");

            lock (m_lock)
            {
                if (m_que.Count >= MAX_QUEUE)
                {
                    m_que.Dequeue();                     // 오래된 것부터 버린다
                    DroppedCount++;
                }
                m_que.Enqueue(it);
            }
        }

        /// <summary>파일/DB 이름에 쓸 수 없는 문자를 정리한다.</summary>
        private static string Sanitize(string pStr, string pDefault)
        {
            if (string.IsNullOrEmpty(pStr)) return pDefault;
            StringBuilder sb = new StringBuilder(pStr.Length);
            foreach (char c in pStr)
            {
                if (char.IsLetterOrDigit(c) || c == '_' || c == '-')
                    sb.Append(c);
                else
                    sb.Append('_');
            }
            string s = sb.ToString();
            if (s.Length > 30) s = s.Substring(0, 30);
            return (s.Length == 0) ? pDefault : s;
        }

        // ─────────────────────────────────────────────────────────────────
        // 쓰기 스레드
        // ─────────────────────────────────────────────────────────────────

        private static void WriterLoop()
        {
            while (m_bRun)
            {
                try
                {
                    DrainOnce();
                    PurgeIfDue();
                }
                catch
                {
                    // 로그 때문에 프로세스가 죽지 않게 한다
                }
                Thread.Sleep(200);
            }
        }

        private static void DrainOnce()
        {
            List<Item> lst = null;
            lock (m_lock)
            {
                if (m_que.Count == 0) return;
                lst = new List<Item>(m_que.Count);
                while (m_que.Count > 0) lst.Add(m_que.Dequeue());
            }

            WriteFiles(lst);
            if (m_bDbEnabled) WriteDb(lst);
        }

        private static void WriteFiles(List<Item> pLst)
        {
            // 같은 파일에 몰아 쓰도록 스레드+날짜로 묶는다
            Dictionary<string, StringBuilder> dic = new Dictionary<string, StringBuilder>();

            foreach (Item it in pLst)
            {
                string strKey = it.Thread + "|" + it.Dt.ToString("yyyyMMdd");
                StringBuilder sb;
                if (!dic.TryGetValue(strKey, out sb))
                {
                    sb = new StringBuilder();
                    dic[strKey] = sb;
                }
                sb.Append(it.Dt.ToString("HH:mm:ss.fff"));
                sb.Append(" [");
                sb.Append(it.Level);
                sb.Append("] ");
                sb.Append(it.Message);
                sb.Append("\r\n");
            }

            Encoding enc;
            try { enc = Encoding.GetEncoding(949); }
            catch { enc = Encoding.Default; }

            foreach (KeyValuePair<string, StringBuilder> kv in dic)
            {
                string[] arr = kv.Key.Split('|');
                string strThread = arr[0];
                string strDay    = arr[1];

                try
                {
                    string strDir = Path.Combine(m_strRootDir, strThread);
                    Directory.CreateDirectory(strDir);
                    string strFile = Path.Combine(strDir, strThread + "_" + strDay + ".log");
                    File.AppendAllText(strFile, kv.Value.ToString(), enc);
                }
                catch
                {
                    // 파일 잠김 등 - 다음 주기에 다시 시도할 수 없으므로 이 묶음은 버린다
                }
            }
        }

        // ─────────────────────────────────────────────────────────────────
        // DB 적재
        // ─────────────────────────────────────────────────────────────────

        private static void WriteDb(List<Item> pLst)
        {
            try
            {
                using (SqlConnection con = new SqlConnection(m_strDbConn))
                {
                    con.Open();

                    if (!m_bDbChecked)
                    {
                        EnsureTable(con);
                        m_bDbChecked = true;
                    }

                    using (SqlTransaction trn = con.BeginTransaction())
                    using (SqlCommand cmd = con.CreateCommand())
                    {
                        cmd.Transaction = trn;
                        cmd.CommandText =
                            "INSERT INTO all_task_log (ins_dt, task_nm, thread_nm, log_level, message) " +
                            "VALUES (@dt, @task, @thread, @lvl, @msg)";

                        SqlParameter pDt  = cmd.Parameters.Add("@dt",     SqlDbType.DateTime2, 3);
                        SqlParameter pTk  = cmd.Parameters.Add("@task",   SqlDbType.VarChar,  20);
                        SqlParameter pTh  = cmd.Parameters.Add("@thread", SqlDbType.VarChar,  30);
                        SqlParameter pLv  = cmd.Parameters.Add("@lvl",    SqlDbType.VarChar,  10);
                        SqlParameter pMsg = cmd.Parameters.Add("@msg",    SqlDbType.NVarChar, 2000);

                        foreach (Item it in pLst)
                        {
                            pDt.Value  = it.Dt;
                            pTk.Value  = it.Task;
                            pTh.Value  = it.Thread;
                            pLv.Value  = (it.Level.Length > 10) ? it.Level.Substring(0, 10) : it.Level;
                            pMsg.Value = (it.Message.Length > 2000) ? it.Message.Substring(0, 2000) : it.Message;
                            cmd.ExecuteNonQuery();
                        }
                        trn.Commit();
                    }
                }
            }
            catch (Exception ex)
            {
                DbFailCount += pLst.Count;
                LastDbError = ex.Message;
                m_bDbChecked = false;        // 다음 번에 테이블 존재 확인부터 다시
            }
        }

        private static void EnsureTable(SqlConnection pCon)
        {
            string strSql =
                "IF OBJECT_ID('all_task_log', 'U') IS NULL " +
                "BEGIN " +
                "  CREATE TABLE all_task_log ( " +
                "    log_seq   bigint IDENTITY(1,1) NOT NULL PRIMARY KEY, " +
                "    ins_dt    datetime2(3)   NOT NULL, " +
                "    task_nm   varchar(20)    NOT NULL, " +
                "    thread_nm varchar(30)    NOT NULL, " +
                "    log_level varchar(10)    NOT NULL, " +
                "    message   nvarchar(2000) NULL " +
                "  ); " +
                "  CREATE INDEX ix_all_task_log_dt ON all_task_log (ins_dt); " +
                "  CREATE INDEX ix_all_task_log_th ON all_task_log (thread_nm, ins_dt); " +
                "END";

            using (SqlCommand cmd = pCon.CreateCommand())
            {
                cmd.CommandText = strSql;
                cmd.CommandTimeout = 30;
                cmd.ExecuteNonQuery();
            }
        }

        // ─────────────────────────────────────────────────────────────────
        // 보관기간 경과 파일 삭제 (기본 365일). 하루 한 번만 돈다.
        // ─────────────────────────────────────────────────────────────────

        private static void PurgeIfDue()
        {
            if (m_dtLastPurge.Date == DateTime.Today) return;
            m_dtLastPurge = DateTime.Now;

            try
            {
                if (!Directory.Exists(m_strRootDir)) return;
                DateTime dtLimit = DateTime.Today.AddDays(-m_nKeepDays);

                foreach (string strDir in Directory.GetDirectories(m_strRootDir))
                {
                    foreach (string strFile in Directory.GetFiles(strDir, "*.log"))
                    {
                        try
                        {
                            // 파일명 끝의 _yyyyMMdd 로 판단한다 (수정시각보다 확실하다)
                            string strName = Path.GetFileNameWithoutExtension(strFile);
                            int nPos = strName.LastIndexOf('_');
                            if (nPos < 0 || strName.Length - nPos - 1 != 8) continue;

                            DateTime dtFile;
                            if (!DateTime.TryParseExact(strName.Substring(nPos + 1), "yyyyMMdd",
                                    System.Globalization.CultureInfo.InvariantCulture,
                                    System.Globalization.DateTimeStyles.None, out dtFile))
                                continue;

                            if (dtFile < dtLimit)
                                File.Delete(strFile);
                        }
                        catch { }
                    }
                }
            }
            catch { }
        }

        /// <summary>보관기간이 지난 DB 로그도 지운다. 호출 측(ALL_TASK)이 하루 한 번 부른다.</summary>
        public static int PurgeDb()
        {
            if (!m_bDbEnabled) return 0;
            try
            {
                using (SqlConnection con = new SqlConnection(m_strDbConn))
                {
                    con.Open();
                    using (SqlCommand cmd = con.CreateCommand())
                    {
                        cmd.CommandText = "DELETE FROM all_task_log WHERE ins_dt < @lim";
                        cmd.Parameters.Add("@lim", SqlDbType.DateTime2, 3).Value =
                            DateTime.Today.AddDays(-m_nKeepDays);
                        cmd.CommandTimeout = 120;
                        return cmd.ExecuteNonQuery();
                    }
                }
            }
            catch (Exception ex)
            {
                LastDbError = ex.Message;
                return -1;
            }
        }
    }
}
