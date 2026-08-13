using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;

namespace HECS.Util
{
    public class ThreadMonitor
    {
        Thread monitorThread = null;
        Process process = null;
        bool isProcessConnect = false;
        public void StartMonitor()
        {



            monitorThread = new Thread(new ThreadStart(Run));
            monitorThread.Start();
        }
        public void Run()
        {
            while (true)
            {
                try
                {
                    if (isProcessConnect == false)
                    {
                        CheckProcess();
                    }
                    if (process != null)
                    {
                        ProcessThreadCollection threads = process.Threads;
                        for (int i = 0; i < threads.Count; i++)
                        {
                            System.Console.WriteLine(threads[i].ThreadState.ToString ()  + "/" + threads[i].Id );
                           
                        }
                        System.Console.WriteLine("[Total = " + threads.Count + "]");
                        System.Console.WriteLine("----------------------------------------------------------------------------");
                    }
                    Thread.Sleep(5000);

                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("ThreadMonitor:" + ex.StackTrace);
                }
            }
        }

        public void CheckProcess()
        {
            Process[] procs = Process.GetProcesses();
            for (int i = 0; i < procs.Length; i++)
            {
                if (procs[i].ProcessName.Equals("Stocker.vshost") || procs[i].ProcessName.Equals("Stocker"))
                {
                    process = procs[i];
                    System.Console.WriteLine("================Found Process===============");
                    System.Console.WriteLine("" + process.ProcessName); 
                    System.Console.WriteLine("" + process.StartInfo );
                    System.Console.WriteLine("" + process.Modules );
                    System.Console.WriteLine("" + process.MachineName );
                    System.Console.WriteLine("" + process.MainModule );
                    System.Console.WriteLine("" + process.Id );
                    System.Console.WriteLine("" + process.Handle );
                    System.Console.WriteLine("" + process.HandleCount );
                    System.Console.WriteLine("=============================================");
                    isProcessConnect = true;
                    return;
                }
            }
            isProcessConnect = false;
        }
    }
}
