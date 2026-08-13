using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;



namespace ECP.ComX.Secs.Timer
{
    public class Timer
    {

        private  object wait_monitor = new object();
        private  object workList_monitor = new object();

        private static Timer timer = new Timer();

        private List<TimerData> timerList = new List<TimerData>();
        
        private Thread timerThread;

        
        public static void AddTimerListener( 
                TimerListener listener, long milliseconds, Object returnObject)
        {
            timer.Add( listener, milliseconds, returnObject );
        }

        
        public static void AddTimerListener(
                TimerListener listener, long milliseconds)
        {
            timer.Add( listener, milliseconds, null );
        }

        
        public static bool RemoveTimerListener(TimerListener listener)
        {
            return timer.Remove(listener);
        }

        
        public static bool RemoveTimerListener(
                TimerListener listener, Object returnObject)
        {
            return timer.Remove(listener,returnObject);
        }

        
        public Timer()
        {
            timerThread = new Thread(new ThreadStart(Run));
            timerThread.IsBackground = true;
            timerThread.Start();
        }

        
        public static String ClassToString()
        {
            return timer.ToString();
        }

        
        public  override String ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("Timer[");


            long currentTime = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;
            
            String delim = "";

            lock (workList_monitor)
            {

                int size = timerList.Count;

                for (int i = 0; i < size; i++)
                {
                    TimerData timerData = (TimerData)timerList[i];

                    result.Append(delim);
                    result.Append(timerData.GetExpirationTime() - currentTime);
                    delim = ",";
                }
            }

            result.Append("]");

            return result.ToString();
        }

        
        private void Add( 
                TimerListener listener, long milliseconds, Object returnObject )
        {
            
            if( listener == null )
            {
                return;
            }
            TimerData timerData = new TimerData( listener, milliseconds, returnObject);
         
            int size;
            int i;

            lock(workList_monitor)
            {
                size = timerList.Count;
            
                for (i=0; i<size; i++)
                {
                    TimerData p2 = (TimerData) timerList[i];

                    if (timerData.GetExpirationTime() < p2.GetExpirationTime())
                    {
                        break;
                    }
                }            
            

                timerList.Insert(i, timerData);

            }
            
            if (i == 0)
            {
                Restart();
            }
            
        }

        
        private bool Remove( TimerListener listener )
        {

            int size;
            int i;


            lock (workList_monitor)
            {
                size = timerList.Count;

                for (i = 0; i < size; i++)
                {
                    TimerData timerData = (TimerData)timerList[i];


                    if (timerData.GetListener() == listener)
                    {
                        timerList.RemoveAt(i);
                    
                        if (i == 0)
                        {
                            Restart();
                        }
                        return true;
                    }
                }
            }
            
            return false;
            
        }

        
        private bool Remove(
                TimerListener listener, Object returnObject )
        {
            int size;
            int i;

            lock(workList_monitor)
            {
                size = timerList.Count;

                for (i=0; i<size; i++)
                {
                    TimerData timerData = (TimerData) timerList[i];

                    if ((timerData.GetListener() == listener) &&
                        (((timerData.GetReturnObject() != null) &&
                          timerData.GetReturnObject().Equals(returnObject)) ||
                         ((timerData.GetReturnObject() == null) && 
                          (returnObject == null))))
                    {

                        timerList.RemoveAt(i);

                        
                        if (i == 0)
                        {
                            Restart();
                        }

                        return true;
                    }
                }
            }
            return false;
        }

        
        private  long GetNextTimeout()
        {
            long result = -1;

            lock (workList_monitor)
            {
                if (timerList.Count > 0)
                {
                    TimerData timerData = (TimerData)timerList[0];
                    long currentTime = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;

                    result = timerData.GetExpirationTime() - currentTime;
                }
            }

            return result;
        }

       
        private  TimerData GetNextExpiredTimerData()
        {

            TimerData timerData = null;
            lock(workList_monitor)
            {
                if( timerList.Count>0 && GetNextTimeout()<=0)
                {
                    timerData = (TimerData)timerList[0];
                    timerList.RemoveAt(0);
                }
            }
            return timerData;
        }

        
        private void NotifyListener( TimerData timerData )
        {
            try 
            {
                
                long currentTime = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;

                //Console.WriteLine( "Accuracy in milliseconds = " +
                  //      (currentTime - timerData.GetExpirationTime()) );
                

                timerData.GetListener().ProcessTimeout(timerData.GetReturnObject());
            }
            catch (Exception ex) 
            {
                // process timeout threw an exception
                Console.WriteLine(ex.StackTrace.ToString());
            }
        }

        
        public void Run()
        {
            TimerData expiredTimerData = null;
            while( true ) 
            {     
                if (timerList.Count > 0)
                {
                    long timeToNextTimeout = GetNextTimeout();
    
                    if (timeToNextTimeout > 0)
                    {
                        lock (wait_monitor)
                        {
                            try
                            {
                                Monitor.Wait(wait_monitor, (int)timeToNextTimeout);
                            }
                            catch (Exception e)
                            {
                            }
                        }
                    }
                    
                    expiredTimerData = GetNextExpiredTimerData();
                }
                else
                {
                    lock (wait_monitor)
                    {
                        try
                        {
                            Monitor.Wait(wait_monitor);
                        }
                        catch (Exception e)
                        {
                        }
                    }
                }
               
                
                if( expiredTimerData != null )
                {
                    NotifyListener( expiredTimerData );
                    expiredTimerData = null;
                }

                
            }
        }

        
        public void Restart()
        {
            lock (wait_monitor)
            {
                Monitor.PulseAll(wait_monitor);
            }
        }
    }

    
    class TimerData
    {
        
        private long expirationTime;

        
        private TimerListener timerListener;

        
        private Object timerReturnObject;


        
        public TimerData(
              TimerListener listener, long milliseconds, Object returnObject)
        {
            timerListener = listener;       
            expirationTime =  (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds + milliseconds;
            timerReturnObject = returnObject;
        }

        
        public long GetExpirationTime()
        {
            return expirationTime; 
        }

        
        public TimerListener GetListener()
        {
            return timerListener;
        }

        
        public Object GetReturnObject()
        {
            return timerReturnObject;
        }
    }
}
