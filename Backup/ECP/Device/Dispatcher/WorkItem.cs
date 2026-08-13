using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Device.Dispatch
{
    public class WorkItem
    {
        private string jobId = null;
        private string jobMessage =null;
        private int priority;
        private WorkState workState = null;

        #region Constructor
        
        public WorkItem(string jobId, string jobMessage, int priority,  WorkState workState)
        {
            this.jobId = jobId;
            this.jobMessage = jobMessage;
            this.priority = priority;
            this.workState = workState;
        }
        #endregion Constructor

        #region Attribute
        public string GetJobId()
        {
            return jobId;
        }

        public string GetJobMessage()
        {
            return jobMessage;

        }
        public int GetPriority()
        {
            return this.priority;
        }

        public string GetMessage()
        {
            return this.jobMessage;
        }

        public WorkState GetWorkState()
        {
            return workState;
        }
        public string GetWorkStateString()
        {
            return workState.ToString();
        }

        public void SetPriority(int priority)
        {
            this.priority = priority;
        }

        public void SetWorkState(WorkState workState)
        {
            this.workState = workState;
        }
        #endregion Attribute


    }

    public class WorkItemComparer : IComparer<WorkItem>
    {
        public int Compare(WorkItem x, WorkItem y)
        {
            if (x == null)
            {
                if (y == null)
                {
                    // If x is null and y is null, they're
                    // equal. 
                    return 0;
                }
                else
                {
                    // If x is null and y is not null, y
                    // is greater. 
                    return 1;
                }
            }
            else
            {
                // If x is not null...
                //
                if (y == null)
                // ...and y is null, x is greater.
                {
                    return -1;
                }
                else
                {
                    // ...and y is not null, compare the 
                    // priority of the two workItem.
                    //

                    if (x.GetPriority() > y.GetPriority())
                        return -1;
                    else if (x.GetPriority() < y.GetPriority())
                        return 1;
                    else
                        return 0;


                }
            }
        }
    }
}
