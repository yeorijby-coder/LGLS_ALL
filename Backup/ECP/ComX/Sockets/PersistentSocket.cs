using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using System.Net;
using System.Net.Sockets;
using System.Threading;

using ECP.Com.FieldBus;
using ECP.Device.Observe;
using ECP.Service.Logger;

namespace ECP.ComX.Sockets
{

    abstract public class PersistentSocket : AbstractFieldBusDriver
    {
        const string CLASS = "PersistentSocket";

        const int MAX_SEQUNCE_NO = 99999;

        private Socket d_socket = null;

        private List<SocketListener> d_listeners = new List<SocketListener>();

        protected SocketInputStream d_is = null;
        protected SocketOutputStream d_os = null;

        private int d_soTimeout = 0;

        public PersistentSocketState socketState = new PersistentSocketState();

        protected Thread connectThread = null;

        int sequence_no;
  
        public bool IsConnected()
        {
            //return d_socket != null;
            //return d_socket.Connected;
            return Connected(d_socket);
        }

        private static bool Connected(Socket s)
        {
            try
            {
                if (!s.Connected) return false;

                if (!s.Poll(-1, SelectMode.SelectWrite)) return false;

                bool BlockingState = s.Blocking;
                s.Blocking = false;

                SocketError errorCode = SocketError.Success;
                s.Send(new byte[1], 0, 0, SocketFlags.None, out errorCode);
                s.Blocking = BlockingState;
                return ((errorCode == SocketError.WouldBlock) || (errorCode == SocketError.Success));
            }
            catch (SocketException e)
            {
                //log.Log("Socket exception: (" + Enum.Format(e.SocketErrorCode.GetType(), e.SocketErrorCode, "G") + ") " + e.Message);
                return false;
            }
            catch (Exception e)
            {
                //log.Log("Unexpected exception: (" + e.GetType().ToString() + ") " + e.Message);
                return false;
            }
        }



        public void Connect()
        {
            while(true)
            {               
                lock(socketState )
                {
                    if( socketState.IsState(PersistentSocketState.CONNECTED) ||
                        socketState.IsState(PersistentSocketState.CONNECTING))
                    {
                        return;
                    }
                    if (socketState.IsState(PersistentSocketState.DISCONNECTED))
                    {
                        socketState.SetState( PersistentSocketState.CONNECTING );
                        Reconnect(); // start reconnect thread 
                        return;
                    }
                }
                socketState.WaitForStateChange();
            }
        }

        
        virtual public void Disconnect()
        {
            lock( socketState )
            {
                if( socketState.IsState(PersistentSocketState.CONNECTED) )
                {
                    socketState.SetState( PersistentSocketState.DISCONNECTED );
                    SocketEvent evt;
                    if (d_socket != null)
                    {
                        try
                        {
                            d_is.Close();
                            d_os.Close();
                            d_socket.Close();
                            evt = new SocketEvent(SocketEvent.CONNECTION_LOST, this);
                            NotifySocketListeners(evt);
                        }
                        catch (Exception e)
                        {
                        }
                        SetSocket(null);
                    }
                }
                else if( socketState.IsState(PersistentSocketState.CONNECTING) )
                {
                    socketState.SetState( PersistentSocketState.DISCONNECTING );
                }
            }
        }

        public int GetNextSequence()
        {
            sequence_no += 1;

            if (sequence_no == MAX_SEQUNCE_NO)
            {
                sequence_no = 0;
            }

            return sequence_no;
        }
        
        public SocketInputStream GetInputStream()
        {
            return d_is;
        }

        public SocketOutputStream GetOutputStream()
        {
            return d_os;
        }

        public void SetSoTimeout(int timeout)
        {
            if (timeout >= 0 && timeout != d_soTimeout)
            {
                d_soTimeout = timeout;

                if (d_socket != null)
                {
                    try
                    {
                        //d_socket.SetSocketOption(SocketOptionLevel.Tcp,
                        //    SocketOptionName.ReceiveTimeout, 
                        //    d_soTimeout);
                        if (d_socket != null) d_socket.ReceiveTimeout =d_soTimeout;
                    }
                    catch (SocketException e)
                    {
                    }
                }
            }
        }

        public int GetSoTimeout()
        {
            return d_soTimeout;
        }

        public void AddSocketListener(SocketListener l)
        {
            if (d_listeners.IndexOf(l) == -1)
            {
                d_listeners.Add(l);
            }
        }

        public void RemoveSocketListener(SocketListener l)
        {
            d_listeners.Remove(l);
        }
        

        protected PersistentSocket()
        {
            d_is = new SocketInputStream(this); 
            d_os = new SocketOutputStream(this);
        }

        protected abstract void Reconnect();

        public abstract void SwitchConnection(IPAddress netAddress, int port);

        private static readonly long ERROR_INTERVAL = 2 *  60 * 1000;
	
	    private long errorTimeout;
	
	    private SocketEvent lastError;


        protected void NotifySocketListeners(SocketEvent evt)
        {
            if (evt.Type == SocketEvent.CONNECTION_ERROR)
            {
                long currentTime = TimeSpan.FromTicks(DateTime.Now.Ticks).Milliseconds;

                if (lastError != null && lastError.Type.Equals(evt.Type)
                        && currentTime < errorTimeout)
                {
                    return;
                }

                lastError = evt;
                errorTimeout = currentTime + ERROR_INTERVAL;
            }

            // Send the event to all the listeners
            int size = d_listeners.Count;

            for (int i = 0; i < size; i++)
            {
                SocketListener l = (SocketListener)d_listeners[i];
                l.SocketChanged(evt);
            }
        }

        protected void SetSocket(Socket s)
        {
            SocketEvent evt;
            Socket oldvalue = d_socket;
          
            d_socket = s;

            if (s == null)
            {
                d_os.SetStream(null);
                d_is.SetStream(null);

                if (oldvalue != null)
                {
                    try
                    {
                        oldvalue.Close();  // close socket when connection goes down
                    }
                    catch (IOException e)
                    {
                    }
                    evt = new SocketEvent(SocketEvent.CONNECTION_LOST, this);
                    NotifySocketListeners(evt);
                }
            }
            else
            {
                try
                {
                    //s.SetSocketOption(SocketOptionLevel.Tcp,
                    //  SocketOptionName.ReceiveTimeout, d_soTimeout);    
                    
                   //s.ReceiveTimeout = 0;                   
                    
                    NetworkStream ns = new NetworkStream(s,true);                    

                    d_os.SetStream(new BinaryWriter(ns));
                    d_is.SetStream(new BinaryReader(ns));
             
                    evt = new SocketEvent(SocketEvent.CONNECTION_ESTABLISHED, this);
                    NotifySocketListeners(evt);
                }
                catch (IOException e)
                {
                    try
                    {
                        d_socket.Close();
                    }
                    catch (IOException ioe)
                    {
                    }

                    d_socket = null;

                    // Call connection lost to start the reconnect thread again.
                    ConnectionLost();
                }
            }
        }

        public void ConnectionLost()
        {
            SetSocket(null);
            lock( socketState )
            {
                if( socketState.IsState(PersistentSocketState.CONNECTED) )
                {
                    socketState.SetState(PersistentSocketState.CONNECTING);
                    Reconnect();
                }
            }
        }

// 2009. 04.28 윤선화 추가 queueing logic
        protected System.Threading.Thread mQueueThread;
        protected System.Collections.Queue mQueue;
        protected bool mIsRunningQueueThread;
        protected object mWaitQueueMonitor;

        public void startQueueThread()
        {
            mQueue = new System.Collections.Queue();
            mWaitQueueMonitor = new object();
            System.Threading.ThreadStart sTh = new System.Threading.ThreadStart(QueueThread);

            if (mQueueThread == null)
            {
                mQueueThread = new System.Threading.Thread(sTh);

                mIsRunningQueueThread = true;
                mQueueThread.Start();
            }
        }

        public void stopQueueThread()
        {
            mIsRunningQueueThread = false;
            if (mQueueThread != null)
            {
                lock (mWaitQueueMonitor)
                    System.Threading.Monitor.PulseAll(mWaitQueueMonitor);

                mQueueThread = null;
            }
        }

        public void WakeUpQueueThread()
        {
            if (mQueueThread != null)
            {
                lock (mWaitQueueMonitor)
                    System.Threading.Monitor.PulseAll(mWaitQueueMonitor);
            }
        }

        public void EnQueue(object value)
        {
            if (mQueueThread != null)
            {
                lock (mQueue)
                {
                    mQueue.Enqueue(value);
                    lock (mWaitQueueMonitor)
                        System.Threading.Monitor.PulseAll(mWaitQueueMonitor);
                }
            }
        }

        virtual public void QueueThread()
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

        public override int Read(Observable observable, out object value)
        {
            const string METHOD = CLASS + ".Read";

            int iRtn = 0;

            value = null;

            try
            {
                value = observable.Value;
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to read observable value \n" + ex.Message.ToString());
            }
            catch (Exception ex)
            {
                iRtn = 1003;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to read observable value \n" + ex.Message.ToString());
            }

            return iRtn;
        }

        public override int Write(Observable observalbe, object value)
        {
            const string METHOD = CLASS + ".Write";

            int iRtn = 0;

            try
            {
                ;
            }
            catch (System.Net.Sockets.SocketException ex)
            {
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to write value to observable\n" + ex.Message.ToString());
            }
            catch (Exception ex)
            {
                iRtn = 1004;
                Log.log(5, METHOD, "error", Category.ERROR, "", "fieldbus",
                                "Failed to write value to observable\n" + ex.Message.ToString());
            }

            return iRtn;
        }

    }

    public class SocketInputStream 
    { 

        private BinaryReader d_is = null;

        private PersistentSocket d_socket = null;

        public SocketInputStream(PersistentSocket socket)
        {
            d_socket = socket;
        }


        public void  Close()
        {
            d_socket.Disconnect();
        }      
        
        public int  Read()
        { 	         
            if (d_is == null)
            {
                throw new SocketException(-1); //Socket not connected
            }

            int result = -1;
            while( result == -1 )
            {
                try
                {
                    //result = d_is.Read();         
                    result = d_is.ReadByte();

                    if (result == -1)
                    {
                        throw new SocketException(-2); // Broken socket
                    }
                }                
                catch (SocketException e)
                {
                    d_socket.ConnectionLost();
                    throw e;
                }
                catch (IOException e)
                {
                    d_socket.ConnectionLost();
                    throw e;
                }
                
            }
            return result;
        }

        public int Read(byte[] b)
        {  
            return Read(b, 0, b.Length);
        }        

        public int  Read(byte[] b, int off, int len)
        {       
            if (d_is == null)
            {
                throw new SocketException(-1); // Socket not connected
               
            }

            try
            {
                int result = d_is.Read(b, off, len);

                if (result == 0)
                {
                    throw new SocketException(-2); // Broken socket                  
                }

                return result;
            }
            catch (IOException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
            catch (SocketException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
            catch (Exception e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
            
        }        

        public void SetStream(BinaryReader _is)
        {
            d_is = _is;
        }       
    }

    public class SocketOutputStream
    {

        private BinaryWriter d_os = null;
      
        private PersistentSocket d_socket = null;

        public SocketOutputStream(PersistentSocket socket)
        {
            d_socket = socket;
        }

        public void SetStream(BinaryWriter os)
        {            
            d_os = os;
        }

        public void  Close()
        {
            d_socket.Disconnect();
        }

        public void  Write(int i)
        {
 	         if (d_os == null)
            {
                throw new SocketException(-1); // Socket Not Connected
            }

            try
            {           
                d_os.Write(i);
                d_os.Flush();
               
            }
            catch (IOException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
        }

        public void Write(byte b)
        {
            if (d_os == null)
            {
                throw new SocketException(-1); // Socket Not Connected
            }

            try
            {
                d_os.Write(b);
                d_os.Flush();

            }
            catch (SocketException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
            catch (IOException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
        }

        public void Write(byte[] b)
        {
            Write(b, 0, b.Length);
        }


       public void  Write(byte[] b, int off, int len)
       {
 	        if (d_os == null)
            {
                throw new SocketException(-1);
            }

            try
            {
                d_os.Write(b,off,len);
            }
            catch (IOException e)
            {
                d_socket.ConnectionLost();
                throw e;
            }
       }
    }

    public class PersistentSocketState
    {
        private object wait_monitor = new object();

        public static readonly int CONNECTED = 1;
        public static readonly int DISCONNECTED = 2;
        public static readonly int CONNECTING = 3;
        public static readonly int DISCONNECTING = 4;
        private int currentState = DISCONNECTED;
        
        public void SetState( int newState )
        {
            lock (wait_monitor)
            {
                if (IsState(newState))
                {
                    return;
                }
                currentState = newState;

                Monitor.PulseAll(wait_monitor);
            }
        }
        
        public void WaitForStateChange()
        {
            try
            {
                lock (wait_monitor)
                {
                    Monitor.Wait(wait_monitor);
                }
            }
            catch( ThreadInterruptedException e )
            {
            }
        }
        
        public bool IsState( int neededState )
        {
            if( neededState == currentState )
            {
                return true;
            }
            return false;
        }
    }

}
