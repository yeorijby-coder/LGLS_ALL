using System;
using System.Collections.Generic;
using System.Text;

using System.Net;
using System.Net.Sockets;

using System.Threading;
using System.IO;
using ECP.Util;
using ECP.ComX.Sockets;

using System.Windows.Forms;


namespace ECP.ComX.Test
{   

    public class TestPersistentSocket
    {     

        MessageTransfer mt = null;
        SendMessageThread sendMessageThread = null;
      
        string hostName;
        int portNumber;
        bool activeConnect;
        int continousMessageInterval;

        public int ContinousMessageInterval
        {
            get { return continousMessageInterval; }
            set { continousMessageInterval = value; }
        }

        Connection connection = null;

        int count = 0;


        Logger logger = null;

        public TestPersistentSocket(string hostname,
            int portNumber, bool activeConnect, int interval
            
            )
        {
            this.hostName = hostname;
            this.portNumber = portNumber;
            this.activeConnect = activeConnect;
            this.continousMessageInterval = interval;
        }

        public TestPersistentSocket(string hostname,
            int portNumber, bool activeConnect, int sleepTime, Logger logger

            )
        {
            this.hostName = hostname;
            this.portNumber = portNumber;
            this.activeConnect = activeConnect;
            this.continousMessageInterval = sleepTime;
            this.logger = logger;
        }

        public void ChangeConnectState( bool connect )
        {
            if( connect == true)
            {
                try
                {              
			        connection = new Connection("DEV", hostName,
						        portNumber, activeConnect, -1 ,this.logger);
                }
                catch( Exception e )
                {                  
                    return;
                }

                this.mt = new MessageTransfer("Main");

                // 2 way Connection을 수립한다.
                mt.EstablishConnection("Connection");               
                
            }
            else
            {
                connection.Dispose();
                if(mt != null) mt.Close();
                mt = null;
            }
        }


        public void StartSendContinousMessage()
        {
            sendMessageThread = new SendMessageThread(this.continousMessageInterval, this.logger);
        }

        public void StopSendContinousMessage()
        {
            sendMessageThread.Stop();
        }

        public void SendOnsShotTestMessage()
        {
            //if (this.activeConnect == true)
            {
                string message = "TestData" + Convert.ToString(count++);

                MessageData m = new MessageData(Helper.String2Bytes(message));

                mt.SendMessageToListeners(m);

            }
        }

        public void SendMessage(string message)
        {
            MessageData m = new MessageData(Helper.String2Bytes(message));
            mt.SendMessageToListeners(m);
        }
    }



    public class MessageData
    {
        private byte[] mData = null;

        public byte[] MData
        {
            get { return mData; }
            set { mData = value; }
        }

        public MessageData(byte [] msg)
        {
            this.mData = msg;
        }
    }

    public class Connection
    {
        ExampleConnection sc = null;
        MessageProcesser mp = null;
        MessageTransfer mt = null;
        Logger logger = null;

      
        public Connection(string deviceName, string host, int port,
                bool active, int deviceId, Logger logger)
        {
            this.logger = logger;

            mt = new MessageTransfer("Connection");

            sc = new ExampleConnection(deviceName, host, port, active,
                    mt,this.logger);

            mp = new MessageProcesser(this.logger);

            sc.Connect();          
        }

        public void Dispose()
        {
            if (sc != null)
            {
                sc.Dispose();
                sc = null;

                mp.Die();

                mt.Close();
            }

            if (mp != null) mp.Die();


        }
    }

    public class SendMessageThread
    {
        public static int threadCount = 0;

        MessageTransfer mt = new MessageTransfer("SendMessageThread"+threadCount++);
		public bool running = true;
        Thread currentThread = null;
        int sleepTime = 0;
        Logger logger = null;

        public SendMessageThread(int sleepTime, Logger logger)
        {
            this.logger = logger;
            mt.EstablishConnection("Connection");

            this.sleepTime = sleepTime;

            currentThread = new Thread(new ThreadStart(Run));
            this.currentThread.Name = "SendMessageThread" + threadCount++;
            this.currentThread.IsBackground = true;
            
            this.currentThread.Start();
        }

        public void Run()
        {
            int count =0;

            while(running )
            {
                //Send Message 

                if (sleepTime > 0)
                    System.Threading.Thread.Sleep(sleepTime);
          
                string message = "( "+ this.currentThread.Name+" ) Message "+count++;
             
                MessageData m = new MessageData(Helper.String2Bytes(message));

                mt.SendMessageToListeners(m);

             }

            logger.Debug(-1, "", "SendMessageThread stopping");

            mt.Close();
        }

        public void Stop()
        {
            this.running = false;
            this.mt.Close();
        }

    }

    public class MessageProcesser
    {
        Logger logger = null;
        MessageTransfer mt = new MessageTransfer("MessageProcesser");
        bool threadKilled = false;

        Thread currentThread  = null;

        public MessageProcesser(Logger logger)
        {
            this.logger = logger;

            mt.EstablishConnection("Connection");

            currentThread = new Thread(new ThreadStart(Run));
            this.currentThread.Name = "MessageProcesser";
            this.currentThread.IsBackground = true;
            this.currentThread.Start();
        }

        public void Run()
        {
      
            while(!threadKilled)
            {
                try
                {
                    MessageData m = (MessageData)mt.GetMessageWait();
                    this.logger.Debug(-1,"","Message Data : "+ m.ToString());
                    Thread.Sleep(1000);


                }
                catch( Exception e )
                {
                   
                }
             }
             this.mt.Close();

            logger.Debug(-1, "", "SendMessageThread stopping");
        }

        /**
         * Replaces the deprecated stop.  Trys to stop this thread.
        **/
        public void Die()
        {
            this.threadKilled = true;
            this.mt.Close();
        }       
    }

    public class ExampleConnection : SocketListener
    {

        private static object lock_object = new object();

        string id = null;

        MessageTransfer mt = null;

        public PersistentSocket socket = null;

        public SocketInputStream _is = null;

        public SocketOutputStream _os = null;

        public bool active = true;

        public ConnectionState connState = null;


        Thread sendThread = null;

        Thread recvThread = null;

        Logger logger = null;



        public ExampleConnection(string id, string hostname, int port, bool active,
                                MessageTransfer mt, Logger logger)
        {
            this.logger = logger;

            connState = new ConnectionState(this.logger);


            this.mt = mt;
            this.active = active;

            PersistentSocket socket = null;

            try
            {
                IPAddress host = null;

                if (hostname.Equals(string.Empty))
                {
                    if (active)
                    {
                        host = IPAddress.Parse("127.0.0.1");
                    }
                }
                else
                {
                    host = IPAddress.Parse(hostname);
                }

                if (active)
                {
                    socket = new PersistentClientSocket(host, port);
                }
                else
                {
                    socket = new PersistentServerSocket(host, port);
                }
            }
            catch (Exception e)
            {
                socket = null;
            }
            Init(id, socket);
        }

        private void Init(string id, PersistentSocket socket)
        {
            this.id = id;
            this.socket = socket;
            
           
            if (socket != null)
            {
                this._is = socket.GetInputStream();
                this._os = socket.GetOutputStream();


                socket.AddSocketListener(this);

                this.StartSendThread("Send/" + id);
                this.StartRecvThread("Recv/" + id);
            }
        }

        public void SocketChanged(SocketEvent e)
        {
            switch (e.Type)
            {
                case SocketEvent.CONNECTION_ESTABLISHED:
                    connState.SetState(ConnectionState.CONNECTED);
                    break;

                case SocketEvent.CONNECTION_LOST:
                    connState.SetState(ConnectionState.NOT_CONNECTED);
                    break;

                case SocketEvent.CONNECTION_ERROR:
                    logger.Debug(-1, id,
                            "Error received from the persistent socket. " +
                            e.Text);
                    break;
            }
        }

        public bool IsConnected()
        {
		    if( socket != null )
		    {
			    return false;
		    }
		    else
		    {
			    return socket.IsConnected();
		    }
        }

        public void Connect()
        {
            lock(lock_object)
            {
                if (socket != null && !connState.IsState(ConnectionState.SHUTDOWN))
                {
                    try
                    {
                        logger.Debug(-1, id, "Trying to establish connection.");
                        socket.Connect();
                    }
                    catch (IOException e)
                    {
                        logger.Debug(-1, id, "Connection attempt failed.");
                    }
                }
                else
                {

                }
		    }
        }

        public void Dispose()
        {
            lock (lock_object)
            {
                if (socket != null)
                {
                    Disconnect();

                    logger.Debug(-1, id, "Connection has been disposed.");

                    mt.Close();


                    connState.SetState(ConnectionState.SHUTDOWN);
                }
            }
        }
       
        public void Disconnect()
        {
            lock (lock_object)
            {
                if (socket != null)
                {
                    if (connState.GetState() == ConnectionState.CONNECTED)
                    {
                    }
                    socket.Disconnect();
                    logger.Debug(-1, id, "Connection dropped.");
                }
            }
        }

        
        private void BounceConnection()
        {
            logger.Debug( -1, id, "Bouncing the connection" );
            Disconnect();
            try
            {
                Thread.Sleep( 10000 );
            }
            catch( Exception e )
            {
            }
            Connect();

        }


        public class ConnectionState 
        {
            public Logger logger = null;

            private static object wait_monitor = new object();

            public const int SHUTDOWN = 0;

            public const int CONNECTED = 1;
            public const int NOT_CONNECTED = 2;

            private int currentState = NOT_CONNECTED;

            public ConnectionState(Logger logger)
            {
                this.logger = logger;
            }

            public void SetState( int newState )
            {
                lock (wait_monitor)
                {
                    if (IsState(newState))
                    {
                        return;
                    }

                    if (currentState == SHUTDOWN)
                    {
                        return;
                    }


                    switch (newState)
                    {
                        case SHUTDOWN:                            
                            logger.Debug(-1, "", "State changing to SHUTDOWN");
                            currentState = newState;
                            break;
                        case CONNECTED:
                            currentState = newState;
                            logger.Debug(-1, "", "State changing to CONNECTED");
                            break;
                        case NOT_CONNECTED:
                            currentState = newState;
                            logger.Debug(-1, "", "State changing to NOT_CONNECTED");
                            break;
                        default:
                            //invalid state
                            break;
                    }
                    Monitor.PulseAll(wait_monitor);
                }

            }

            
            public  bool IsState( int neededState )
            {
                lock (wait_monitor)
                {                    
                    if (neededState == currentState)
                    {
                        return true;
                    }
                    return false;
                }
            }

            public  bool WaitForState( int neededState, int timeout )
            {
                lock (wait_monitor)
                {
                    int startTime = (int)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;
                    int timeLeft = timeout;

                    while (true)
                    {
                        if (IsState(neededState))
                        {
                            break;
                        }
                        try
                        {
                            Monitor.Wait(wait_monitor, timeLeft);
                            timeLeft = (int)( timeout -
                                (new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds - startTime));
                            if (timeLeft <= 0)
                            {
                                break;
                            }
                        }
                        catch (ThreadInterruptedException e)
                        {
                        }
                    }
                    return IsState(neededState);
                }
            }

            public void WaitForState( int neededState )
            {
                lock (wait_monitor)
                {
                    while (true)
                    {
                        if (IsState(neededState))
                        {
                            break;
                        }
                        try
                        {
                            Monitor.Wait(wait_monitor);
                        }
                        catch (ThreadInterruptedException e)
                        {
                        }
                    }
                }
            }

         
            public  int GetState()
            {
                return this.currentState;
            }
        }       


        public void StartRecvThread( string name )
        {
            recvThread = new Thread(new ThreadStart(RunRecv));
            recvThread.Name = name;
            recvThread.IsBackground = true;
            recvThread.Start();
        }

        public void RunRecv()
        {
            while( !connState.IsState(ConnectionState.SHUTDOWN) || 
                !socket.socketState.IsState(PersistentSocketState.DISCONNECTED))
            {   
                ReceiveMessages();
            }
            logger.Debug(-1, id, "Recv Thread stopping");
        }

        private void ReceiveMessages()
        {
            try
            {
                if (connState.WaitForState(ConnectionState.CONNECTED, 1000))
                {
                    ReceiveMessage();
                }
            }
            catch (Exception e)
            {
                logger.Debug(-1, "", "Exception while receving messages: " +
                        e.Message);
                return;
            }            
        }


        private void ReceiveMessage()
        {
            //socket.SetSoTimeout(5000);
            byte[] b = new byte[4];

            int chRead = 0; 
            int chs = 0;
            int chToRead = b.Length;

            socket.SetSoTimeout(0);

            try
            {

                while (chRead != chToRead)
                {

                    chs = _is.Read(b, chRead, chToRead - chRead);
                    socket.SetSoTimeout(5000);

                    if (chs == 0)
                    {
                        logger.Debug(-1, id, "Got EOF when reading length bytes");
                        BounceConnection();
                        return;
                    }
                    chRead += chs;
                }
            }
            catch (SocketException e)
            {
                if (chRead == 0)
                {
                    logger.Debug(-1, id, "Got EOF when reading length bytes");
                    BounceConnection();
                    return;
                }

                logger.Debug(-1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
                        "length bytes. Read " + chRead + " of " + 4 + " bytes.");
                BounceConnection();
                return;
            }
            catch (IOException e)
            {
                if (chRead == 0)
                {
                    logger.Debug(-1, id, "Got EOF when reading length bytes");
                    BounceConnection();
                    return;
                }

                logger.Debug(-1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
                        "length bytes. Read " + chRead + " of " + 4 + " bytes.");
                BounceConnection();
                return;
            }

            int messageLength =
			    b[0]<<24 | (b[1]&0xff)<<16 | (b[2]&0xff)<<8 | (b[3]&0xff);

            byte[] m = new byte[messageLength];
            chRead = 0;
            try
            {
                socket.SetSoTimeout(0);

                while (chRead != messageLength)
                {
                    chs = _is.Read(m, chRead, messageLength - chRead);

                    socket.SetSoTimeout(5000);

                    if (chs == 0)
                    {
                        logger.Debug(-1, id, "Got EOF when reading message");
                        BounceConnection();
                        return;
                    }
                    chRead += chs;
                }
            }
            catch (SocketException e)
            {
                logger.Debug(-1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
                        "length bytes. Read " + chRead + " of " + 4 + " bytes.");
                BounceConnection();
                return;
            }
            catch (IOException e)
            {
                logger.Debug(-1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
                        "length bytes. Read " + chRead + " of " + 4 + " bytes.");
                BounceConnection();
                return;
            }

            // Create the message
            MessageData msg = null;
            try
            {
                msg = new MessageData( m );
            }
            catch( Exception ex )
            {
                // Failed to parse the message.
                logger.Debug( -1, id, "Failed to parse message.");
                return;
            }

            this.ProcessMessage(msg);
        }

        public void ProcessMessage(MessageData m)
        {
            logger.Debug(-1, id, Helper.Bytest2String(m.MData));
        }



        public void StartSendThread(string name)
        {
            sendThread = new Thread(new ThreadStart(RunSend));
            sendThread.Name = name;
            sendThread.IsBackground = true;
            sendThread.Start();
        }
           
        public void RunSend()
        {
            MessageData mdata;

            while (!connState.IsState(ConnectionState.SHUTDOWN))
            {
                 mdata = (MessageData)mt.GetMessageWithTimeout(1000);
                if (mdata == null)
                {
                    continue;
                }

                if (connState.IsState(ConnectionState.CONNECTED))
                {
                    this.SendMessage(mdata);
                }
                else
                {
                    // throw away...
                }
            }
            logger.Debug( -1, id, "Send Thread stopping" );
        }

        private void SendMessage(MessageData mdata)
        {
            SendBlock(mdata.MData);
        }

        private bool SendBlock(byte[] block)
        {
            byte[] tmp = new byte[4 + block.Length];

            lock( _os )
            {
                tmp[0] = (byte)((block.Length >> 24) & 0xFF);
                tmp[1] = (byte)((block.Length >> 16) & 0xFF);
                tmp[2] = (byte)((block.Length >> 8) & 0xFF);
                tmp[3] = (byte)(block.Length & 0xFF);

                for (int i = 0; i < block.Length; i++)
                {
                    tmp[i + 4] = block[i];
                }
                // Send message
                try
                {
                    _os.Write(tmp, 0, tmp.Length);
                }
                catch (SocketException e)
                {
                    logger.Debug(-1, id, "Failed to write message to socket.");
                    return false;
                }
                catch (IOException e)
                {
                    logger.Debug(-1, id, "Failed to write message to socket.");
                    return false;
                }             
            }
            return true;
        }
    }    

    

}
