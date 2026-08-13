using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Threading;
using ECP.Service.Logger;


namespace ECP.Util
{
    /// <summary>
    /// FIFO(First In, First Out) Queue Class
    /// </summary>
    public class FIFOQueue
    {
        /// <summary>
        /// Class Name
        /// </summary>
        static readonly string CLASS = "FIFOQueue";

        /// <summary>
        /// Max Size Flag
        /// </summary>
        protected bool theMaxSizeFlag = false;


        /// <summary>
        /// Max Size
        /// </summary>
        protected int theMaxSize = 0;

        /// <summary>
        /// Queue의 Sync 관리를 위한 모니터
        /// </summary>
        private object monitor = new object();

        /// <summary>
        /// Queue의 Sync 관리를 위한 Wait 모니터
        /// </summary>
        private object wait_monitor = new object();

        /// <summary>
        /// FIFO Queue
        /// </summary>
        private Queue<object> theFIFOQueue = null;

        
        /// <summary>
        /// FIFO Queue기본 생성자
        /// </summary>
        public FIFOQueue()
        {
            this.theFIFOQueue = new Queue<object>();
        }

        /// <summary>
        /// 주어진 Max Size로 Queue를 생성
        /// </summary>
        /// <param name="theMaxSize">Queue의 Max Size</param>
        public FIFOQueue(int theMaxSize)
        {
            if (theMaxSize > 0)
            {
                this.theMaxSizeFlag = true;
                this.theMaxSize = theMaxSize;

            }
            this.theFIFOQueue = new Queue<object>();
        }


        public override string ToString()
        {
            lock (monitor)
            {
                return (CLASS + " :: " +
                    "MaxSize=" + GetMaxSize() + " : " +
                     "Size=" + GetSize() + " : " +
                    this.theFIFOQueue.ToString());
            }
        }

        /// <summary>
        /// Queue의 Max Size를 Return
        /// </summary>
        /// <returns>Queue의 Max Size</returns>
        public int GetMaxSize()
        {
            return this.theMaxSize;
        }

        /// <summary>
        /// Queue의 Current Size를 Return
        /// </summary>
        /// <returns>Queue의 Current Size</returns>
        public int GetSize()
        {
            lock (monitor)
            {
                return this.theFIFOQueue.Count;
            }
        }

        /// <summary>
        /// FIFO Queue의 상태가 Empty인지를 Return
        /// </summary>
        /// <returns>Empty일 경우 true, 아니면 false</returns>
        public bool IsEmpty()
        {
            lock (monitor)
            {
                return (this.theFIFOQueue.Count > 0) ? false : true;
            }
        }

        /// <summary>
        /// FIFO Queue의 Full상태를 Return
        /// </summary>
        /// <returns>Full일경우 true,아니면 false</returns>
        public bool IsFull()
        {
            lock (monitor)
            {

                return (this.theMaxSizeFlag ? (GetSize() >= GetMaxSize()) : false);
            }
        }

        /// <summary>
        /// FIFO Queue의 End Point에 객체를 Add
        /// </summary>
        /// <param name="theObj">Add할 Object</param>
        public void Put(object theObj)
        {
            lock (wait_monitor)
            {
                while (IsFull())
                {
                    try
                    {
                        Monitor.Wait(wait_monitor);
                    }
                    catch (ThreadInterruptedException ex)
                    {
                        Console.Write  (ex.ToString());
                    }
                }
                this.theFIFOQueue.Enqueue(theObj);
                Monitor.PulseAll(wait_monitor);
            }
        }

        /// <summary>
        /// FIFO Queue의 End Point에 객체를 Add
        /// 만약 Full일 경우, Wait하지 않고 바로 false 값을 Return
        /// 
        /// </summary>
        /// <param name="theObj">Queue 되어질 Object</param>
        /// <returns>Queue되어질 경우 true, 아니면 즉시 false</returns>
        public bool PutNoWait(object theObj)
        {

            if (!IsFull())
            {
                Put(theObj);
                return true;
            }
            else
            {
                return false;
            }

        }

        /// <summary>
        /// FIFO Queue의 Front에서 Object를 Return, 만약 Empty일 경우는 Wait
        /// </summary>
        /// <returns>FIFO Queue의 First Element</returns>
        public object Get()
        {
            lock (wait_monitor)
            {
                while (IsEmpty())
                {
                    try
                    {
                        Monitor.Wait(wait_monitor);
                    }
                    catch (ThreadInterruptedException ex)
                    {
                        Console.Write(ex.ToString());
                    }                  
                }
                if (theFIFOQueue.Count <= 0) return null;
                object firstElement = this.theFIFOQueue.Dequeue();

                Monitor.PulseAll(wait_monitor);
                return firstElement;
            }

        }
        
        /// <summary>
        /// FIFO Queue의 Front에서 Object를 Return, 만약 Empty일 경우는, timeout시간 까지 Wait
        /// </summary>
        /// <param name="theTimeOut">밀리 초내의 Time out시간</param>
        /// <returns>FIFO Queue의 First Object</returns>
        public object Get(int theTimeOut)
        {
            DateTime startTime = DateTime.Now;
            int timeLeft = theTimeOut;

            lock (wait_monitor)
            {
                while (IsEmpty())
                {
                    try
                    {                       
                        Monitor.Wait(wait_monitor, timeLeft);
                           
                        timeLeft = theTimeOut - (int)DateTime.Now.Subtract(startTime).TotalMilliseconds;

               
                        if (timeLeft <= 0)
                        {
                            return null;
                        }
                    }
                    catch (ThreadInterruptedException ex)
                    {
                        Console.Write(ex.ToString());
                    }
                }
                if (theFIFOQueue.Count <= 0) return null;

                object firstElement = this.theFIFOQueue.Dequeue();
                Monitor.PulseAll(wait_monitor);
                return firstElement;
            }
        }
        /// <summary>
        /// FIFO Queue의 Front에서 Object를 Return, 만약 Empty일 경우는, 기다리지 않고 Return null
        /// </summary>
        /// <returns>FIFO Queue의 First Object, Queue가 Empty일 경우 Return null</returns>
        public object GetNoWait()
        {
            if (!IsEmpty())
            {
                return Get();
            }
            else
            {
                return null;
            }
        }
    }
}
