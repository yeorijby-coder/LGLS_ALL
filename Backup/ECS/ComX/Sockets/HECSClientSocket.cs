using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

using ECP.Service.Logger;
using ECP.Com.FieldBus;
using ECP.Util.Client;
using ECP.Event;

namespace HECS.ComX.Sockets
{
    public class HECSClientSocket : ECP.ComX.Sockets.PersistentClientSocket
    {
        const string CLASS = "HECSClientSocket";

        Hashtable mWaitItemList;
        int mRetryLimit;
        int mReplyTimeout;

        public HECSClientSocket(/* IPAddress host, int port */)
        {
            mWaitItemList = new Hashtable();

            mRetryLimit = 3;
            mReplyTimeout = 1; // 1초
        }

        override public int Read()
        {
            const string METHOD = CLASS + ".Read";

            int iRtn = 0;

            try
            {
                while (this.Connected)
                {
                    if (mQueueThread == null)
                    {
                        startQueueThread();
                    }

                    int headersize = int.Parse(this.connectionInfo[DriverConnectionString.HEADER_SIZE].ToString());

                    HECSPacket pkt = new HECSPacket(headersize);
                    pkt.RegisterObservables(this.registeredObservableList);
                    pkt.Read(d_is);

                    //Log.log(5, METHOD, "debug", Category.DEBUG, "", "fieldbus",
                    //                string.Format("Receive Time {0}\r\n\r\n", DateTime.Now.ToString("yyyy/MM/dd hh:mm:ss.fff")));

                    if (pkt.Direction == "1")
                    {
                        // 응답메세지가 도착했다면..
                        if (mWaitItemList.ContainsKey(pkt.SeqNo))
                        {
                            HECSPacket request = (HECSPacket)mWaitItemList[pkt.SeqNo];
                            System.Timers.Timer tmr = request.WaitTimer;
                            tmr.Stop();

                            mWaitItemList.Remove(pkt.SeqNo);
                        }
                    }

                    this.EnQueue(pkt);
                }
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                //Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                //                "Failed to read observable value \n" + ex.Message.ToString());
                System.Console.WriteLine("Read1:" + ex.StackTrace);
                stopQueueThread();
                Close();
              
                //Naming.PublishEvent("*",
                //          new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));
            }
            catch (Exception ex)
            {
                iRtn = 1003;
                //Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                //                "Failed to read observable value \n" + ex.Message.ToString());
                System.Console.WriteLine("Read2:" + ex.StackTrace);
                stopQueueThread();
                Close();

                //Naming.PublishEvent("*",
                //           new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));
            }
            return iRtn;
        }

        public void Write(HECSPacket pkt)
        {
            pkt.SeqNo = GetNextSequence().ToString("00000000000000000");
            pkt.RegisterObservables(this.registeredObservableList);
            EnQueue(pkt);

            if (pkt.Direction == "0" && pkt.Reply == "1")
            {
                // 응답을 기다림.

                pkt.RetryLimit = mRetryLimit;
                pkt.WaitTimer = new HECSTimer();
                pkt.WaitTimer.Interval = mReplyTimeout * 1000;
                pkt.WaitTimer.Elapsed += new System.Timers.ElapsedEventHandler(timer_Elapsed);
                pkt.WaitTimer.Start();

                if (pkt.RetryCount == 0)
                    mWaitItemList.Add(pkt.WaitTimer, pkt);
            }
        }

        void timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            System.Timers.Timer tmr = (System.Timers.Timer)sender;
            tmr.Stop();

            if (mWaitItemList.ContainsKey(sender))
            {
                HECSPacket pkt = (HECSPacket)mWaitItemList[sender];

                if (pkt.RetryLimit > pkt.RetryCount)
                {
                    pkt.RetryCount++;
                    this.Write(pkt);

                    tmr.Start();
                }
                else
                {
                    mWaitItemList.Remove(sender);
                    Close();
                }
            }
        }


        override public void QueueThread()
        {
            const string METHOD = CLASS + ".Read";
            
            while (mIsRunningQueueThread)
            {
                if (mQueue != null && mQueue.Count > 0)
                {
                    try
                    {
                        HECSPacket pkt = (HECSPacket)mQueue.Dequeue();
                        if (pkt.Direction == "1")
                        {
                            // 받은 메세지
                            pkt.OnRead();
                        }
                        else if (pkt.Direction == "0")
                        {
                            // 보내는 메세지
                            pkt.Write(d_os);

                        }
                        else
                            ;
                    }
                    catch (Exception ex)
                    {
                        Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                        "Fail to transfer message\r\n" + ex.Message.ToString() + "\r\n" + ex.StackTrace);
                        //
                    }
                }
                else
                {
                    lock (mWaitQueueMonitor)
                    {
                        System.Threading.Monitor.Wait(mWaitQueueMonitor, 3000);
                    }
                }
            }
        }
    }
}
