using System;
using System.Collections.Generic;
using System.Text;

using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Collections.Specialized;
using System.Configuration;

using ECP.Global.Com;
using ECP.Global;
using ECP.Com.FieldBus;
using ECP.Device.Observe;
using ECP.Service.Logger;
using ECP.Event;
using ECP.Util.Client;

namespace ECP.ComX.Sockets
{
    public class PersistentClientSocket : PersistentSocket
    {
        const string CLASS = "PersistentClientSocket";

        private IPAddress host = null;

        private int port = -1;

        private int retryInterval = 1000;
      
        private int reconnectInterval = 30;

        private bool reconnecting = false;


        public PersistentClientSocket()
        {
        }

        public PersistentClientSocket(IPAddress host, int port)
        {
            if (port <= 0)
            {
                throw new ArgumentException("Port must be > 0.");
            }

            if (host == null)
            {
                throw new ArgumentException("Host must be non null.");
            }

            this.host = host;
            this.port = port;
        }

        public void SetRetryInterval(int value)
        {
            retryInterval = value;
        }

        public override string  ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append("Client:");
            result.Append(host.ToString());
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

       
        public void Run()
        {
            Socket s = null;

		    if( reconnecting )
		    {
			    try
			    {
				    Thread.Sleep( reconnectInterval * 1000 );
			    }
			    catch (Exception e)
			    {
			    }
		    }
		    else
		    {
			    reconnecting = true;
		    }

		    for (;;)
		    {
           
	    	    try
	    	    {
                    
                    s = new TcpClient().Client;
                    s.Connect(new IPEndPoint(host,port));

                    if (s.Connected)
                    {
                        this.ConnectionState = FBConnectionState.CONNECTED;

                        //Naming.PublishEvent("*",
                        //           new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Connected"));
                    }
                    else
                    {
                        this.ConnectionState = FBConnectionState.DISCONNECTED;
                        socketState.SetState(PersistentSocketState.DISCONNECTING);
                    }
				
                    lock( socketState )
                    {
                        if( socketState.IsState(PersistentSocketState.DISCONNECTING) )
                        {
                            
                            s.Close();
                            
                            socketState.SetState( PersistentSocketState.DISCONNECTED );
                            return;
                        }
                        else
                        {
                            socketState.SetState( PersistentSocketState.CONNECTED );
                            SetSocket( s );
                            return;
                        }
                    }
	    	    }
	    	    catch (SocketException ioe)
                {
               
                    lock( socketState )
                    {
                        if( socketState.IsState(PersistentSocketState.DISCONNECTING) )
                        {
                            socketState.SetState( PersistentSocketState.DISCONNECTED );
                            return;
                        }
                    }
                    SocketEvent evt = new SocketEvent(
						    SocketEvent.CONNECTION_ERROR, this, ioe.Message +
						    " on host " + host.ToString() + ":" + Convert.ToString(port) );
				    NotifySocketListeners(evt);
				    try
				    {
		    		    Thread.Sleep(retryInterval);
				    }
				    catch (Exception e)
				    {
        		    }
                    
                }
            }
        }

        public override void SwitchConnection(IPAddress host, int port)
        {
            this.host = host;
            this.port = port;
        }

        override  protected void InitConnectionString(string cnnString)
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
                sPort = this.connectionInfo["PORT", "9800"];
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
            while (true)
            {
                Read();
                Thread.Sleep(this.ScanInterval);
            }
        }

        virtual public int  Read()
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
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus", "Failed to read observable value \n" + ex.Message.ToString());

                //Naming.PublishEvent("*", new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));

                Reconnect();
            }
            catch (Exception ex)
            {
                Close();

                iRtn = 1003;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to read observable value \n" + ex.Message.ToString());

                //Naming.PublishEvent("*",                           new ECPEvent("Driver." + this.DriverNo.ToString() + "." + "Disconnected"));
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
