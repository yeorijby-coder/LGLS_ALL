using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Threading;

using ECP.Util;
using ECP.Global.Exception;
using ECP.Service.Logger;

namespace ECP.Device.Dispatch
{

    public class Dispatcher
    {
        public const string CLASS = "Dispatcher";


        #region Attribute
        private static readonly int WAIT_DELAY = 1000 * 3;

        private bool running = false;

        private Thread theThread = null;

        private object owner = null;

        private bool keepWorking = true;

        private bool reorderWorkList = true;

        private List<WorkItem> workList = null;

        WorkItemComparer workItemComparer = null;

        private static object wait_monitor = new object();

        private static object workList_monitor = new object();
        #endregion Attribute

        #region Constructor

        public Dispatcher(object owner)
        {
            this.owner = owner;
            this.running = false;
            this.workList = new List<WorkItem>();
            this.workItemComparer = new WorkItemComparer();
            this.running = false;

        }

        #endregion Constructor

        #region ThreadControl_Method
        protected bool IsRunning()
        {
            return this.running;
        }
        public void Start()
        {
            lock (this)
            {
                if (!running)
                {
                    this.running = true;
                    this.theThread = new Thread(new ThreadStart(Run));
                    this.theThread.Name = CLASS + "-" + this.owner.ToString();
                    this.theThread.IsBackground = true;
                    this.theThread.Start();
                }
            }

        }
        public void StopNow()
        {
            lock (wait_monitor)
            {
                this.running = false;
                if (theThread != null)
                {
                    Monitor.PulseAll(wait_monitor);
                    theThread = null;
                }
            }
        }
        public void Stop()
        {
            lock (wait_monitor)
            {
                this.running = false;
                Monitor.PulseAll(wait_monitor);
            }
        }
        public void Wakeup()
        {
            lock (wait_monitor)
            {
                keepWorking = true;
                Monitor.PulseAll(wait_monitor);
            }

        }
        public void Run()
        {

            const string METHOD = CLASS + "run";

            while (IsRunning())
            {
                try
                {
                    do
                    {
                        keepWorking = false;
                        //sort work list if necessary


                        // expensive later add the bool value see if worklist should be sorted,
                        // just sort now

                        if (reorderWorkList == true)
                        {
                            lock (workList_monitor)
                            {
                                workList.Sort(workItemComparer);
                            }
                        }

                        if (CheckWork())
                        {
                            keepWorking = true;
                        }

                    } while (keepWorking);
                }
                catch (Exception ex)
                {
                    Log.log(3, METHOD, "debug", Category.DEBUG, "", "dispatcher", " dispatcher error");
                    Log.log(3, METHOD, "debug", Category.DEBUG, "", "dispatcher", ex.Message.ToString());
                }
                WaitForWork();
            }
            Log.log(4, METHOD, "debug", Category.DEBUG, "", "dispatcher", " dispatcher thread is exiting");
        }
        #endregion ThreadControl_Method

        #region Method

        private void Display(string where, List<WorkItem> workList)
        {
            const string METHOD = CLASS + ".Display";


            Log.log(5, METHOD, "debug", Category.DEBUG, "", "dispatcher", where);
            lock (workList_monitor)
            {
                foreach (WorkItem w in workList)
                {
                    Log.log(5, METHOD, "debug", Category.DEBUG, "", "dispatcher", w.GetJobId() + " ( " + Convert.ToString(w.GetPriority()) + " , " +
                        w.GetMessage() + " , " + w.GetWorkStateString() + " )");

                }
            }


        }


        public void RequestJob(string jobId, string jobMessage, int priority)
        {

            WorkItem workItem = new WorkItem(jobId, jobMessage, priority, WorkState.REQUESTED);

            lock (workList_monitor)
            {
                workList.Add(workItem);
            }
            Wakeup();
        }
        public void ClearAllJobs()
        {
            lock (workList_monitor)
            {
                workList.Clear();
            }
        }

        bool CancelJob(string jobId)
        {
            WorkItem w = GetJob(jobId);

            if (w != null)
            {
                lock (w)
                {
                    if (w.GetWorkState().Equals(WorkState.REQUESTED)) return true;
                    else return false;
                }
            }
            else
            {
                return true;
            }
        }

        public WorkItem GetJob(string jobId)
        {
            lock (workList_monitor)
            {
                foreach (WorkItem w in workList)
                {
                    if (w.GetJobId().Equals(jobId)) return w;
                }
            }

            return null;
        }

        public void DeleteJob(string jobId)
        {
            WorkItem w = GetJob(jobId);

            if (w != null)
            {
                w.SetWorkState(WorkState.PROCESSED);
                Wakeup();
            }
        }


        public bool HasPendingWork()
        {
            bool found = false;

            if (this.running)
            {
                lock (workList_monitor)
                {
                    foreach (WorkItem w in workList)
                    {
                        if (w.GetWorkState().Equals(WorkState.PROCESSING))
                        {
                            found = true;
                            break;
                        }
                    }
                }
            }

            return found;
        }


        public int GetSizeOfWork()
        {
            lock (workList_monitor)
            {
                return workList.Count;
            }
        }


        public bool CheckWork()
        {
            DispatchWork();

            GetNewWork();
            if (PrepareNewWork()) return true;
            else return false;
        }


        private void DispatchWork()
        {
            //if owner is ok
            DoAnyWork();
        }
        protected void GetNewWork()
        {
        }

        private void DoAnyWork()
        {
            //disPlay( workList);
           /* lock (workList_monitor)
            {
                foreach (WorkItem w in workList)
                {
                    if (w.GetWorkState().Equals(WorkState.READY))
                    {
                        // let owner do our real job and chang job state to REMOTE_COMMAND_SENT
                        w.SetWorkState(WorkState.REMOTE_COMMAND_SENT);
                    }
                    else if (w.GetWorkState().Equals(WorkState.REMOTE_COMMAND_SENT))
                    {
                        // let owner do our real job and chang job state to PROCESSING
                        w.SetWorkState(WorkState.PROCESSING);
                    }
                    else if (w.GetWorkState().Equals(WorkState.PROCESSING))
                    {
                        // when owner finished job and change job state to PROCESSED
                        w.SetWorkState(WorkState.PROCESSED);
                    }
                }
            }*/
            //Display("after do any work ", workList);
        }

        protected bool PrepareNewWork()
        {

            bool found = false;

            lock (workList_monitor)
            {
                foreach (WorkItem w in workList)
                {
                    if (w.GetWorkState().Equals(WorkState.REQUESTED) ||
                        w.GetWorkState().Equals(WorkState.READY))
                    {
                        if (w.GetWorkState().Equals(WorkState.REQUESTED))
                        {
                            w.SetWorkState(WorkState.READY);
                        }
                        found = true;
                    }
                }
            }
            //Display("afeter prepare new work ", workList);
            lock (workList_monitor)
            {

            AGAIN:
                foreach (WorkItem w in workList)
                {
                    if (w.GetWorkState().Equals(WorkState.PROCESSED))
                    {
                        RemoveWork(w, true);
                        goto AGAIN;
                    }
                }
            }
            //Display("after remove finished work ", workList);


            return found;
        }

        private void RemoveWork(WorkItem w, bool force)
        {
            if (w.GetWorkState().Equals(WorkState.PROCESSING) && force == false)
            {
                throw new ECPException(ECPExceptionType.OPERATION_ABORTED, w.GetJobId(), "Unable to remove work for job : " +
                    w.GetJobId() + " because is is under processing");
            }

            lock (workList_monitor)
            {
                workList.Remove(w);
            }
        }

        public void WaitForWork()
        {           
            const string METHOD = CLASS + ".waitForWork";

            lock (wait_monitor)
            {
                if (IsRunning())
                {
                    if (!keepWorking)
                    {
                        try
                        {
                            Monitor.Wait(wait_monitor, WAIT_DELAY);
                        }
                        catch (ThreadInterruptedException ex)
                        {
                            Log.log(4, METHOD, "error", Category.ERROR, "", "dispatcher",
                                "dispatcher wait monitor error \n" + ex.Message.ToString());
                        }
                    }
                }
            }
        }

        public void SetPriority(string jobId, int priority)
        {

            WorkItem w = GetJob(jobId);
            if (w != null)
            {
                w.SetPriority(priority);
                this.reorderWorkList = true;
                Wakeup();
            }
        }



        private void InsertByPriorityOrder(List<WorkItem> workList, WorkItem newWorkItem, WorkItemComparer wc)
        {
            int index = workList.BinarySearch(newWorkItem, wc);

            if (index > 0)
            {
                workList.Insert(index, newWorkItem);
            }
        }


        #endregion Method
    }
}    
