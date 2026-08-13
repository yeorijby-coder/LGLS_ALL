using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Configuration;
using System.Collections.Specialized;

using ECP.Global.Com;
using ECP.Com.FieldBus;
using ECP.Global;
using ECP.Device.Observe;
using ECP.Service.Logger;
using ECP.Util.Client;
using ECP.Event;

namespace ECP.ComX.Sockets
{
    public class PersistentServerSocket : PersistentSocket
    {
        const string CLASS = "PersistentServerSocket";

        private IPAddress host;

        private int port;

        private int retryInterval = 100;

        private int reconnectInterval = 0;

        private bool reconnecting = false;

        TcpListener listener = null;


        public PersistentServerSocket()
        {
        }

        public PersistentServerSocket(int port)
        {
            if (port <= 0)
            {
                throw new ArgumentException("Port must be > 0.");
            }

            this.port = port;
            host = null;
        }

        public PersistentServerSocket(IPAddress host, int port)
        {
            if (port <= 0)
            {
                throw new ArgumentException("Port must be > 0.");
            }

            this.port = port;
            this.host = host;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("Server:");
            if (host != null)
            {
                result.Append(host.ToString());
            }
            else
            {
                result.Append("AllHost");
            }
            result.Append("/");
            result.Append(port);

            return result.ToString();
        }

       
        protected override void Reconnect()
        {
            connectThread = new Thread(new ThreadStart(Run));
            connectThread.Name = ToString();
            connectThread.IsBackground = true;
            connectThread.Start();
        }

        public void StopListener()
        {
            if (listener != null)
            {
                listener.Stop();
                listener = null;
            }
        }


        public override void Disconnect()
        {
            lock (socketState)
            {
                if (socketState.IsState(PersistentSocketState.CONNECTING))
                {
                    if(this.listener != null) this.StopListener();
                }
                base.Disconnect();
            }
        }
        
        

        public void Run()
        {
           // TcpListener listener = null;


            Socket s = null ;
		    
		    if( reconnecting )
		    {
			     Thread.Sleep( reconnectInterval * 1000 );			   
            }
		    else
		    {
			    reconnecting = true;
		    }


            while (true)
            {

                try
                {

                    listener = new TcpListener(host, port);
                    listener.Start();

                    s = listener.AcceptSocket();

                    Naming.PublishEvent("*",
                               new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Connected"));

                    socketState.SetState(PersistentSocketState.CONNECTING);

                    while (true)
                    {
                        try
                        {
                            //s = listener.AcceptSocket();

                            
                            lock (socketState)
                            {
                                if (socketState.IsState(PersistentSocketState.DISCONNECTING))
                                {
                                    if (s != null)
                                    {
                                        d_is.Close();
                                        d_os.Close();
                                        s.Close();
                                    }
                                    socketState.SetState(PersistentSocketState.DISCONNECTED);
                                    return;
                                }
                                else if (s != null &&
                                    socketState.IsState(PersistentSocketState.CONNECTING))
                                {
                                    
                                    SetSocket(s);
                                    socketState.SetState(PersistentSocketState.CONNECTED);
                                    this.ChangeConnectionState(FBConnectionState.CONNECTED);


                                    return;
                                }
                            }
                        }
                        catch (InvalidOperationException e)
                        {
                            lock (socketState)
                            {
                                if (socketState.IsState(PersistentSocketState.DISCONNECTING))
                                {
                                    socketState.SetState(PersistentSocketState.DISCONNECTED);
                                    return;
                                }
                            }
                        }
                        finally
                        {
                            if (listener != null) listener.Stop();
                            listener = null;
                        }
                    }
                }
                catch (SocketException e)
                {
                    lock (socketState)
                    {
                        if (socketState.IsState(PersistentSocketState.DISCONNECTING))
                        {
                            socketState.SetState(PersistentSocketState.DISCONNECTED);
                            return;
                        }
                    }
                    string hostName = "AllHost";

                    if (host != null)
                    {
                        hostName = host.ToString();
                    }
                    SocketEvent evt = new SocketEvent(
                            SocketEvent.CONNECTION_ERROR, this, e.Message +
                            " on host " + hostName + ":" + port.ToString());
                    NotifySocketListeners(evt);
                }

                try
                {
                    Thread.Sleep(retryInterval);
                }
                catch (Exception se)
                {
                }
            }
        
        }

        public override void SwitchConnection(IPAddress host, int port)
        {
            this.host = host;
            this.port = port;

        }

        override protected void InitConnectionString(string cnnString)
        {
            this.connectionInfo = new DriverConnectionString();
            this.connectionInfo.Parse(cnnString);

            string sHost;
            string sPort;

            if (cnnString == string.Empty || cnnString == null ||
                !cnnString.Contains("HOST") || !cnnString.Contains("PORT"))
            {
                NameValueCollection appSettings = ConfigurationManager.AppSettings;
                sHost = appSettings[ECPProperties.EmulationHost].ToString();
                sPort = appSettings[ECPProperties.EmulationPort].ToString();
            }
            else
            {
                sHost = this.connectionInfo["HOST", "127.0.0.1"];
                sPort = this.connectionInfo["PORT", "10000"];
            }

            SwitchConnection(IPAddress.Parse(sHost), int.Parse(sPort));
        }

        override public int Open()
        {
            Reconnect();
            return 0;
        }

        override protected void DoScan()
        {
            Read();
            StopScaning();
        }

        public virtual int Read()
        {
            const string METHOD = CLASS + ".Read";

            int iRtn = 0;

            try
            {
                while (this.Connected)
                {
                    int headersize = int.Parse(this.connectionInfo[DriverConnectionString.HEADER_SIZE].ToString());

                    PersistentPacket pkt = new PersistentPacket(headersize);
                    pkt.RegisterObservables(this.registeredObservableList);
                    pkt.Read(d_is);

                    this.EnQueue(pkt);
                }
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                Close();
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to read observable value \n" + ex.Message.ToString());

                Naming.PublishEvent("*",
                           new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));
            }
            catch (Exception ex)
            {
                iRtn = 1003;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to read observable value \n" + ex.Message.ToString());
                Close();

                Naming.PublishEvent("*",
                           new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));
            }
            return iRtn;
        }

        public void Write(PersistentPacket pkt)
        {
            pkt.RegisterObservables(this.registeredObservableList);
            EnQueue(pkt);
        }


        override public void QueueThread()
        {
            while (mIsRunningQueueThread)
            {
                if (mQueue != null && mQueue.Count > 0)
                {
                    try
                    {
                        PersistentPacket pkt = (PersistentPacket)mQueue.Dequeue();
                    }
                    catch (Exception ex)
                    {
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
