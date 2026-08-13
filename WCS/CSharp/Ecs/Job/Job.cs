using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Ecs.Job
{
    /// <summary>
    /// C++의 CJob (CObList 기반)에 대응하는 Job 큐 관리자.
    /// Job 전체 생명주기(생성→실행→완료→제거)를 담당한다.
    /// </summary>
    public class JobManager
    {
        private readonly List<JobItem> _jobs = new();
        private readonly object _lock = new object();
        private int _nextSequence = 1;

        public event EventHandler<JobItem>? JobAdded;
        public event EventHandler<JobItem>? JobStatusChanged;
        public event EventHandler<JobItem>? JobCompleted;
        public event EventHandler<JobItem>? JobRemoved;

        // ─── CRUD ─────────────────────────────────────────────────────

        public bool Add(JobItem job)
        {
            lock (_lock)
            {
                if (job.LuggNum != 0 && FindByLuggNum(job.LuggNum) != null)
                    return false; // 중복 방지
                job.Sequence = _nextSequence++;
                job.CreateTime = DateTime.Now;
                _jobs.Add(job);
            }
            JobAdded?.Invoke(this, job);
            return true;
        }

        public bool Remove(int luggNum)
        {
            JobItem? job;
            lock (_lock)
            {
                job = FindByLuggNum(luggNum);
                if (job == null) return false;
                _jobs.Remove(job);
            }
            JobRemoved?.Invoke(this, job);
            return true;
        }

        public bool Cancel(int luggNum)
        {
            var job = FindByLuggNum(luggNum);
            if (job == null) return false;
            SetStatus(job, JobStatus.Cancelled);
            return true;
        }

        // ─── 상태 변경 ────────────────────────────────────────────────

        public void SetStatus(JobItem job, JobStatus status)
        {
            job.Status = status;
            if (status == JobStatus.Done) job.CompleteTime = DateTime.Now;
            if (status == JobStatus.Moving) job.StartTime = DateTime.Now;
            JobStatusChanged?.Invoke(this, job);
            if (status == JobStatus.Done) JobCompleted?.Invoke(this, job);
        }

        public bool Arrive(int luggNum, int trackNum, int cvIndex)
        {
            var job = FindByLuggNum(luggNum);
            if (job == null) return false;
            job.ArriveTrackNum = trackNum;
            job.SrcCvIndex    = cvIndex;
            SetStatus(job, JobStatus.Arrived);
            return true;
        }

        public bool Complete(int luggNum)
        {
            var job = FindByLuggNum(luggNum);
            if (job == null) return false;
            SetStatus(job, JobStatus.Done);
            return true;
        }

        public bool InvokeJob(int luggNum, int scIndex)
        {
            var job = FindByLuggNum(luggNum);
            if (job == null) return false;
            job.ScIndex = scIndex;
            SetStatus(job, JobStatus.Invoking);
            return true;
        }

        // ─── 검색 ─────────────────────────────────────────────────────

        public JobItem? FindByLuggNum(int luggNum)
        {
            lock (_lock)
                return _jobs.FirstOrDefault(j => j.LuggNum == luggNum);
        }

        public JobItem? FindByTrayId(string trayId)
        {
            lock (_lock)
                return _jobs.FirstOrDefault(j => j.TrayId == trayId);
        }

        public List<JobItem> GetByStatus(JobStatus status)
        {
            lock (_lock)
                return _jobs.Where(j => j.Status == status).ToList();
        }

        public List<JobItem> GetByScIndex(int scIndex)
        {
            lock (_lock)
                return _jobs.Where(j => j.ScIndex == scIndex).ToList();
        }

        public List<JobItem> GetWaitingStoreJobs()
        {
            lock (_lock)
                return _jobs
                    .Where(j => j.JobType == JobType.Store && j.Status == JobStatus.Wait)
                    .OrderBy(j => j.Priority)
                    .ThenBy(j => j.Sequence)
                    .ToList();
        }

        public List<JobItem> GetWaitingRetrieveJobs()
        {
            lock (_lock)
                return _jobs
                    .Where(j => j.JobType == JobType.Retrieve && j.Status == JobStatus.Wait)
                    .OrderBy(j => j.Priority)
                    .ThenBy(j => j.Sequence)
                    .ToList();
        }

        public bool IsIdleJob(int luggNum)
        {
            var job = FindByLuggNum(luggNum);
            return job != null && job.Status == JobStatus.Wait;
        }

        public System.Collections.IEnumerable Items { get { lock (_lock) return _jobs.ToArray(); } }

        public void Clear()
        {
            List<JobItem> removed;
            lock (_lock) { removed = new List<JobItem>(_jobs); _jobs.Clear(); }
            foreach (var j in removed) JobRemoved?.Invoke(this, j);
        }

        public int Count
        {
            get { lock (_lock) return _jobs.Count; }
        }

        public List<JobItem> GetAll()
        {
            lock (_lock) return new List<JobItem>(_jobs);
        }

        // ─── 백업/복원 ────────────────────────────────────────────────

        public void Backup(string filePath)
        {
            try
            {
                using var bw = new BinaryWriter(File.Open(filePath, FileMode.Create));
                lock (_lock)
                {
                    bw.Write(_jobs.Count);
                    foreach (var job in _jobs)
                        WriteJob(bw, job);
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Job Backup 오류: {ex.Message}");
            }
        }

        public void Restore(string filePath)
        {
            if (!File.Exists(filePath)) return;
            try
            {
                using var br = new BinaryReader(File.Open(filePath, FileMode.Open));
                int count = br.ReadInt32();
                lock (_lock)
                {
                    _jobs.Clear();
                    for (int i = 0; i < count; i++)
                        _jobs.Add(ReadJob(br));
                }
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Job Restore 오류: {ex.Message}");
            }
        }

        private static void WriteJob(BinaryWriter bw, JobItem j)
        {
            bw.Write(j.LuggNum);
            bw.Write(j.TrayId);
            bw.Write((int)j.JobType);
            bw.Write((int)j.Status);
            bw.Write(j.Priority);
            bw.Write(j.SrcCvIndex);  bw.Write(j.SrcTrackNum);
            bw.Write(j.DstCvIndex);  bw.Write(j.DstTrackNum);
            bw.Write(j.DstRow);      bw.Write(j.DstCol);
            bw.Write(j.ScIndex);
            bw.Write(j.HsNum);
            bw.Write(j.CreateTime.ToBinary());
        }

        private static JobItem ReadJob(BinaryReader br)
        {
            return new JobItem
            {
                LuggNum     = br.ReadInt32(),
                TrayId      = br.ReadString(),
                JobType     = (JobType)br.ReadInt32(),
                Status      = (JobStatus)br.ReadInt32(),
                Priority    = br.ReadInt32(),
                SrcCvIndex  = br.ReadInt32(),  SrcTrackNum = br.ReadInt32(),
                DstCvIndex  = br.ReadInt32(),  DstTrackNum = br.ReadInt32(),
                DstRow      = br.ReadInt32(),  DstCol      = br.ReadInt32(),
                ScIndex     = br.ReadInt32(),
                HsNum       = br.ReadInt32(),
                CreateTime  = DateTime.FromBinary(br.ReadInt64()),
            };
        }
    }
}
