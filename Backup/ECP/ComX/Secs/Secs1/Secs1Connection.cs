using System;
using System.Collections.Generic;
using System.Text;

using System.Net;
using System.Net.Sockets;

using System.Threading;
using System.IO;

using ECP.Util;

using ECP.ComX.Secs.Timer;
using ECP.ComX.Secs;
using ECP.ComX.Secs.Secs2;
using ECP.ComX.Secs.Secs2.Gem;
using ECP.ComX.Sockets;

namespace ECP.ComX.Secs.Secs1
{
    public class Secs1Connection : TimerListener, SocketListener
    {
        private const byte EOT = 4;
        private const byte ENQ = 5;
        private const byte ACK = 6;
        private const byte NAK = 21;

        protected const int FORCE_DOWN_TIMEOUT = 15000;

    

        /// <summary>
        /// Inter character timeout
        /// </summary>
        private int			t1Timeout	= 500;		//  0.5 seconds

        /// <summary>
        /// Protocol timeout
        /// </summary>
        private int			t2Timeout	= 10000;	// 10.0 seconds

        /// <summary>
        /// Secondary message timeout
        /// </summary>
        private int			t3Timeout	= 45000;	// 45.0 seconds

        /// <summary>
        /// Inter block timeout
        /// </summary>
        private int			t4Timeout	= 45000;	// 45.0 seconds

        
        private int			numRetries	= 3;

       
        private bool		rBit		= false;	// Host->Equip

        
        private bool		slave		= true;		// Slave

       
        private int			deviceId	= -1;

        
        private String		id		= null;

       
	    private bool			enabled = true;

       
        private SecsLogger		logger	= null;

        
        private static int      checkForShutdownInterval = 5000;

        
        private byte[]		lastRecvHdr	= null;

        private SecsTransfer	st		= null;


        private Secs1State secs1State = null;


        private  object secs1State_monitor = new object();

       
        private PersistentSocket	socket	= null;

        
        private SocketInputStream		_is		= null;

        
        private SocketOutputStream	_os		= null;


        private Dictionary<SecsHeader, SecsMessage> replyWaitList	= new Dictionary<SecsHeader,SecsMessage>();


        private List<SecsMessage> multiBlkWaitList = new List<SecsMessage>();

        Thread sendThread = null;

        Thread recvThread = null;

        
        public Secs1Connection( String id, String hostname, int port,
			    bool active, SecsTransfer st, SecsLogger logger )
        {
            this.st = st;
            this.logger = logger;
		    PersistentSocket socket;

            this.secs1State = new Secs1State(this, this.secs1State_monitor);

		    try
		    {
                IPAddress host = null;
                if( hostname.Equals("") )
                {
                    if( active )
                    {
                        host = IPAddress.Parse("127.0.0.1");
                    }
                }
                else
                {
                    host = IPAddress.Parse(hostname);
                }

			    if( active )
			    {
				    socket = new PersistentClientSocket( host, port );
				    SetRbit( false );
				    SetSlave( true );
			    }
			    else
			    {
				    socket = new PersistentServerSocket( host, port );
				    SetRbit( true );
				    SetSlave( false );
			    }
		    }
		    catch( Exception e )
		    {
                logger.Log( -1, id, "Unknown host " + hostname + ". " );
			    socket = null;
		    }
            Init( id, socket );
        }


        
        private void Init(String id, PersistentSocket socket)
        {
            this.id = id;
            this.socket = socket;
		    if( socket != null )
		    {
                this._is = socket.GetInputStream();
                this._os = socket.GetOutputStream();

                socket.AddSocketListener(this);

                this.StartSendThread("Secs1Send/" + id);
                this.StartRecvThread("Secs1Recv/" + id);
		    }
        }

        
        public void SocketChanged(SocketEvent e)
        {
            // Ignore socket changed event if the Connection is disabled.
            if( ! enabled )
            {
                return;
            }
            switch (e.Type)
            {
			    case SocketEvent.CONNECTION_ESTABLISHED:
				    secs1State.SetState( Secs1State.CONNECTED );
				    break;

			    case SocketEvent.CONNECTION_LOST:
				    secs1State.SetState( Secs1State.NOT_CONNECTED );
				    break;

			    case SocketEvent.CONNECTION_ERROR:
				    logger.Log( -1, id,
						    "Error received from the persistent socket. " +
						    e.Text);
				    SecsEvent se = new SecsEvent(
						    SecsEvent.CONNECTION_ERROR,
						    "Failed to establish a Connection, if the tool is " +
						    "operational contact the system administrator" );
				    st.SendPrimaryEvent(se);
				    break;
            }
        }

        
        public String GetId()
        {
            return id;
        }

        
        public int GetDeviceId()
        {
            return deviceId;
        }

       
        public void SetDeviceId(int deviceId)
        {
            this.deviceId = deviceId;
        }

       
        public int GetT1Timeout()
        {
            return t1Timeout;
        }

        
        public void SetT1Timeout(int timeout)
        {
            if( timeout >= 100 && timeout <= 10000  )
            {
                t1Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T1 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public int GetT2Timeout()
        {
            return t2Timeout;
        }

        
        public void SetT2Timeout(int timeout)
        {
            if( timeout >= 200 && timeout <= 25000  )
            {
                t2Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T2 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

       
        public int GetT3Timeout()
        {
            return t3Timeout;
        }

       
        public void SetT3Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 120000  )
            {
                t3Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T3 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public int GetT4Timeout()
        {
            return t4Timeout;
        }

        
        public void SetT4Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 120000  )
            {
                t4Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T4 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public int GetNumRetries()
        {
            return numRetries;
        }

        
        public void SetNumRetries(int numRetries)
        {
            if( numRetries >= 0 && numRetries <= 31 )
            {
                this.numRetries = numRetries;
            }
		    else
		    {
			    logger.Log( -1, id, "Num retries(" + numRetries +
					    ") not accepted, out of range." );
		    }
        }

        
        public bool IsConnected()
        {
            return socket.IsConnected();
        }

        
        public void Connect()
        {           
		    // Synchronize on the state so that we cant be Connecting and
		    // DisConnecting at the same time.
		    lock( secs1State_monitor )
		    {
			    if( socket != null && enabled )
			    {
				    try
				    {
					    logger.Debug( -1, id, "Trying to establish Connection." );
					    socket.Connect();
				    }
				    catch (IOException e)
				    {
					    logger.Log( -1, id, "Connection attempt failed." );
				    }
			    }
			    else
			    {
				    String message =
					    "Connect not possible.  See logs for reason. " +
					    "Probably due to an unknown host name.";
				    if( st != null )
				    {
					    SecsEvent se = new SecsEvent(
							    SecsEvent.CONNECTION_ERROR,  message );
					    st.SendPrimaryEvent(se);
				    }
			    }
		    }
        }

        
        public void DisConnect()
        {
            /* The call to DisConnect can cause us to change states.  Inorder to
		    ** eliminate deadlocks we must lock the states in the same order.
		    ** Lock the secs1State, and then the socket State.
            */
            lock (secs1State_monitor)
            {
                socket.Disconnect();
            }
            logger.Debug( -1, id, "Connection dropped." );
        }

       
        private void BounceConnection()
        {
            logger.Debug( -1, id, "Bouncing the Connection" );
            DisConnect();
            try
            {
                Thread.Sleep( 10000 );
            }
            catch( Exception e )
            {
            }
            Connect();
        }
       

        public void RunDispose()
        {
            DisConnect();
            st.Close();
            secs1State.SetState( Secs1State.SHUTDOWN );
        }

        
        public void Dispose()
        {
            // Prevent Dispose from being entered twice.  Synchronized for the
            // test and set of the enabled flag.
            lock( this )
            {
                if( enabled && socket != null )
                {
                    // We are going away, so set us disabled...
                    SetEnable(false);
                }
                else
                {
                    return;
                }
            }

            Thread thread = new Thread(new ThreadStart(RunDispose));
            thread.Name = "dispose:" + id;
            thread.IsBackground = true;
            thread.Start();
            logger.Debug(-1, id, "Running " + "Dispose:" + id);

            try
            {
                thread.Join(FORCE_DOWN_TIMEOUT);
            }
            catch (ThreadInterruptedException ex)
            {
            }

            // Check and make sure the Connection went down, if not, attempt to
            // force it down.
            if (!secs1State.IsState(Secs1State.SHUTDOWN))
            {
                // Risk the deadlock and just DisConnect the socket.
                // It can't be any worse than how stuck we are now.
                logger.Log(-1, id, "Attempting to force a Secs1Connection Dispose");

                if(socket != null)
                {
                    // Call DisConnect directly on the socket
                    socket.Disconnect();
                    st.Close();
                    // Send replies back to the device for all outstanding
                    // messages.
                    EmptyWaitLists();
                    EmptySecsTransfer();
                }
            }
        }

	    
	    private void SetEnable( bool value )
	    {
		    enabled = value;
	    }

       
        public void SetRbit(bool rBit)
        {
            this.rBit = rBit;
        }

        
        public void SetSlave(bool slave)
        {
            this.slave = slave;
        }

        
        private void SendMessage( Secs1Message message, SecsMessage original )
        {
            byte[] tmp = new byte[2];

            if (message != null)
            {
                int j = 0;
                for( byte[] block = message.GetBlock(j);
                    block!=null;
                    block = message.GetBlock(++j))
                {
                    logger.Debug( message.GetDeviceId(), id,
                            "Sending block " + (j+1) + " of " +
                            message.GetNumBlocks() + "." );



                    lock (secs1State_monitor)
                    {
                        logger.Debug( message.GetDeviceId(), id,
                                "Sending " + message.GetStringId() );

    
                        for (int i=0; i<=numRetries; i++)
                        {
                                // While not set to idle
                            while(
                                !secs1State.WaitForState( Secs1State.IDLE, 5000 ) )
                            {
                                // Every 5 seconds see if the Connection was lost
                                if( secs1State.IsState(Secs1State.NOT_CONNECTED) )
                                {
                                    // Communications failure, no more retries.
                                    logger.Log( message.GetDeviceId(), id,
                                        "Connection lost. Failed to send message " +
                                         message.GetSecsHeader().ToString() );
                                    goto RETRY_LOOP;
                                }
                            }

                            secs1State.SetState( Secs1State.SENT_ENQ );

                            
                            try
                            {
                                logger.LogProtocol(
                                        message.GetDeviceId(), id, true, ENQ);
                                _os.Write(ENQ);
                            }
                            catch (SocketException e)
                            {
                                // The Connection was lost
                                logger.Log(-1, id, "Failed to write ENQ to socket.");
                                continue;
                            }
                            catch (IOException e)
                            {
                                // The Connection was lost
                                logger.Log(-1, id, "Failed to write ENQ to socket.");
                                continue;
                            }
                            secs1State.WaitForStateChange(t2Timeout);
                            
                            int state = secs1State.GetState();


                            if( state == Secs1State.IDLE ||
                                state == Secs1State.RECEIVING_MESSAGE )
                            {
                                /*
                                ** We must be a slave and got delayed for our
                                ** send.
                                */
                                i--;   // Dont count this against our retry limit.
                                continue;
                            }
                    	    // If we are still SENT_ENQ a t2 timeout occured
						    if( state == Secs1State.SENT_ENQ )
						    {
                                logger.Log( -1, id,
                                    "T2 timeout occurred while waiting for EOT." );
                                secs1State.SetState( Secs1State.IDLE );
						    }
                    
                            if( state == Secs1State.RECEIVED_EOT )
                            {
                                /*
                                 * Now we can send the message.
                                **/
                                secs1State.SetState( Secs1State.SENT_BLOCK );

                                // Calculate and send length byte
                                tmp[0] = (byte)block.Length;
                                try
                                {
                                    _os.Write(tmp[0]);
                                    // Send message
                                    try
                                    {
                                        _os.Write(block, 0, block.Length);
                                        // Calculate and send checksum
                                        int checksum = ComputeChecksum(block);
                                        tmp[0] = (byte)((checksum >> 8) & 0xFF);
                                        tmp[1] = (byte)(checksum & 0xFF);
                                        try
                                        {
                                            _os.Write(tmp);
                                        }
                                        catch (SocketException e)
                                        {
                                            // The Connection was lost
                                            logger.Log(message.GetDeviceId(),
                                                    id, "Failed to write " +
                                                    "checksum to socket.");
                                            continue;
                                        }
                                        catch (IOException e)
                                        {
                                            // The Connection was lost
                                            logger.Log(message.GetDeviceId(),
                                                    id, "Failed to write " +
                                                    "checksum to socket.");
                                            continue;
                                        }
                                    }
                                    catch (SocketException e)
                                    {
                                        // The Connection was lost
                                        logger.Log(message.GetDeviceId(), id,
                                            "Failed to write message to socket.");
                                        continue;
                                    }
                                    catch (IOException e)
                                    {
                                        // The Connection was lost
                                        logger.Log(message.GetDeviceId(), id,
                                            "Failed to write message to socket.");
                                        continue;
                                    }
                                }
                                catch (SocketException e)
                                {
                                    // The Connection was lost
                                    logger.Log(message.GetDeviceId(), id,
                                        "Failed to write length bytes to socket.");
                                    continue;
                                }
                                catch (IOException e)
                                {
                                    // The Connection was lost
                                    logger.Log(message.GetDeviceId(), id,
                                        "Failed to write length bytes to socket.");
                                    continue;
                                }


                                logger.LogProtocol(
                                        message.GetDeviceId(), id, true, block );

                                if( secs1State.WaitForState(
                                    Secs1State.RECEIVED_ACK, t2Timeout) )
                                {
                                    // Horray, the block was sent.

								    // If this is the last block of a primary
								    // message(that requires a reply) put it in
								    // the reply wait list.
								    if( ((j+1) == message.GetNumBlocks()) &&
										    (original != null) &&
										    (message.GetWbit() == true) )
								    {
									    AddReplyWaitList( t3Timeout, original );
								    }

								    // Set the state to idle
                                    secs1State.SetState( Secs1State.IDLE );

                                    block = null;
                                    break;
                                }
							    else // Did not change to received ACK in t2 time
							    {
								    logger.Log( message.GetDeviceId(), id,
										    "T2 timeout occurred while waiting" +
										    " for ACK." );
								    secs1State.SetState( Secs1State.IDLE );
							    }
                            }
                            // Failed to send block - Do retry.
                        }
            RETRY_LOOP:

                            // Check if the block was not sent.
                        if( block != null )
                        {
                            logger.Log( message.GetDeviceId(), id,
								    "Retries exceeded  - Failed to send block" );
                            if( true == original.GetWbit() )
                            {
                                SecsEvent evt = new SecsEvent(
                                            SecsEvent.SEND_FAILED, original );
                                st.SendReplyEvent( message, evt );
                            }
                            BounceConnection();
                            return;
                        }
                    }
                    // Yeild to receive thread since it may have received an
                    // ENQ.

                    Thread.Sleep(0);
                }
                logger.LogMessage( id, true, message );
            }
        }

       
        private void AddReplyWaitList( int timeout, SecsMessage message )
        {
            replyWaitList.Add( message.GetSecsHeader(), message );

            Timer.Timer.AddTimerListener(this, timeout, message);
        }

        
        private void EmptyWaitLists()
        {

            if (replyWaitList.Count == 0) return;
         
            lock( replyWaitList )
            {

                foreach(SecsHeader header in replyWaitList.Keys)
                {
                    SecsMessage msg = replyWaitList[header];
                    replyWaitList.Remove(header);
                        
                    Timer.Timer.RemoveTimerListener( this, msg );
                    if( msg != null )
                    {
                        SecsEvent evt = new SecsEvent(
                                            SecsEvent.REPLY_NOT_RECEIVED,
                                            msg );
                        st.SendReplyEvent( msg, evt );
                        return;
                    }
                }

               
            }
            // Cleanout any partially received messages since we wont be
            // receiving the rest of them.
            multiBlkWaitList.Clear();
        }

       
        private void EmptySecsTransfer()
        {
            SecsMessage sm;
            while( null != (sm = st.GetSecsMessageNoWait( )) )
            {
                if( sm.GetWbit() == true )
                {
                    SecsEvent se = new SecsEvent(
                                            SecsEvent.SEND_FAILED, sm );
                    st.SendReplyEvent( sm, se );
                }
            }
        }

        
        private void Cleanup()
        {
            EmptySecsTransfer();
            EmptyWaitLists();
        }

        
        public void ProcessTimeout(Object data)
        {
            if (data != null)
            {
                SecsMessage msg = (SecsMessage) data;
                SecsMessage original = null;

                
                if( msg is Secs1Message )
                {
                    lock( multiBlkWaitList )
                    {
                        int i = multiBlkWaitList.IndexOf( msg );
                        if( i > -1 )
                        {
                            // T4 timeout: next block not received.
                            logger.Log( msg.GetSessionId(), id,
                                "T4 timeout occurred: discarding message. "  +
                                msg.GetSecsHeader().ToString() );
                            multiBlkWaitList.RemoveAt( i );
                            if( msg.IsSecondaryMessage() )
                            {
                                original = (SecsMessage)replyWaitList[msg.GetSecsHeader()];
                                replyWaitList.Remove(
                                                msg.GetSecsHeader() );

                                if( original != null )
                                {
                                    SecsEvent evt = new SecsEvent(
                                        SecsEvent.REPLY_NOT_RECEIVED,
                                        original );
                                    st.SendReplyEvent( original, evt );
                                }
                            }
                        }
                    }
                }
                else
                {
                    original = (SecsMessage)replyWaitList[msg.GetSecsHeader()];
                    replyWaitList.Remove( msg.GetSecsHeader() );
                    if( original != null )
                    {
                        // T3 timeout: reply not received.
                        logger.Log( msg.GetSessionId(), id,
                                "T3 timeout occurred. " +
                                msg.GetSecsHeader().ToString() );
                        SecsEvent evt = new SecsEvent(
                            SecsEvent.REPLY_NOT_RECEIVED, msg );
                        st.SendReplyEvent( msg, evt );
                        return;
                    }
                }
            }
        }

        
        private int WaitForInput(int timeout)
        {
            long now = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;

            long end = now + timeout;

            int interval = 100;

            for (; now < end; now = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds)
            {
                long t = end - now;

                socket.SetSoTimeout((int) t);

                try
                {
                    for (;;)
                    {
                        int b = _is.Read();

                        if (b >= 0)
                        {
                            return b;
                        }
                    }
                }
                catch(SocketException e)
                {                    
                }
                catch (IOException e)
                {
                    logger.Log(
                            -1, id, "Character read failed due to IO Exception." );
                }

                // Couldn't read the socket for some reason.  Wait half the
                // remaining time, then retry the read.

                now = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;
                t = end - now;

                if (t > 0)
                {
                    interval *= 2;

                    if (t > interval)
                    {
                        t = interval;
                    }

                    try
                    {
                        Thread.Sleep((int) t );
                    }
                    catch (ThreadInterruptedException e)
                    {
                        return -1;
                    }
                }
            }

            return -1;
        }

        
        private void ReceiveMessage()
        {
            int ch;

            try
            {
                if( !secs1State.WaitForState( Secs1State.CONNECTED, checkForShutdownInterval ) )
                {
                    return;
                }
               
                socket.SetSoTimeout(0);
                ch = _is.Read();
                socket.SetSoTimeout(checkForShutdownInterval);
            }
            catch (SocketException e)
            {
                return;
            }
            catch (IOException e)
            {
                logger.Log( -1, id,
                        "IO Excpetion read while waiting for any message." );
                return;
            }
            switch (ch)
            {
            case -1:
                return;
            case ENQ:
                logger.LogProtocol( -1, id, false, ENQ );

             
                lock (secs1State_monitor)
                {
       			    
				    if( secs1State.IsState(Secs1State.RECEIVED_ACK) )
                    {
       				    secs1State.WaitForState(
                            Secs1State.IDLE, checkForShutdownInterval );
       			    }
       			    // If we are in the IDLE state or
				    // we are the slave and in the SENT_ENQ state.
                    if( secs1State.IsState(Secs1State.IDLE) ||
                        (slave && secs1State.IsState( Secs1State.SENT_ENQ )) )

                    {
                        secs1State.SetState( Secs1State.RECEIVING_MESSAGE );
                    }
                }
                // This does not need to be synchronized since this is only a
                // test.
                if( secs1State.IsState(Secs1State.RECEIVING_MESSAGE) )
                {
                    ReceiveBlock();
       

                    // synchronized for the test and set
                    lock (secs1State_monitor)
                    {
                        // It is very likely that the state is RECEIVING MESSAGE,
                        // if it isn't the most likely case is that it is
                        // NOT_CONNECTED or SHUTDOWN.  There is the slimist of
                        // chances that it is anything else.  The failure to
                        // receive the message will be logged elsewhere.
                        if( secs1State.IsState(Secs1State.RECEIVING_MESSAGE) )
                        {
                            secs1State.SetState( Secs1State.IDLE );
                        }
                    }
                }
                else
                {
                    logger.Debug( -1, id, "Discarding ENQ." );
                }
                break;

            case EOT:
                lock (secs1State_monitor)
                {
                    logger.LogProtocol(-1, id, false, EOT);
                    secs1State.SetState(Secs1State.RECEIVED_EOT);
                }
                break;

            case ACK:              
                logger.LogProtocol(-1, id, false, ACK);
                secs1State.SetState(Secs1State.RECEIVED_ACK);
                break;

            case NAK:
                logger.LogProtocol( -1, id, false, NAK );
                secs1State.SetState( Secs1State.IDLE );
                break;

            default:
                logger.Log(-1, id, "Discarding non-ENQ EOT ACK byte 0x" + String.Format("{0:X}", ch));
                break;
            }
        }

        
        private void ReceiveBlock()
        {
            try
            {
                _os.Write(EOT);
                logger.LogProtocol(-1, id, true, EOT);
            }
            catch (SocketException e)
            {
                logger.Log(-1, id, "Failed to write EOT to socket");
                return;
            }
            catch (IOException e)
            {
                logger.Log(-1, id, "Failed to write EOT to socket");
                return;
            }
            
            int len = WaitForInput( t2Timeout );


            if (len == -1)
            {
                logger.Log( -1, id,
                    "T2 timeout occurred while waiting for length byte." );
            }
            else if (len<10 || len>254)
            {
                logger.Log( -1, id, "Received bad length byte (" + len + ")." );
                SkipBadBlock();
            }
            else
            {
                logger.Debug( -1, id, "Received length byte (" + len + ").");

                byte []recvBuf = new byte[len];
                // Receive the message
                for (int i=0; i<len; i++)
                {
                    int ch = WaitForInput( t1Timeout );
                    if (ch == -1)
                    {
                        logger.Log( -1, id,
                            "T1 timeout occurred while reading block data." );
                        SkipBadBlock();
                        return;
                    }
                    else
                    {
                        recvBuf[i] = (byte) ch;
                    }
                }
                // Receive the checksum
                int checksum = 0;
                for( int i=0; i<2; i++ )
                {
                    int ch = WaitForInput( t1Timeout );
        
                    if( ch == -1 )
                    {
                        logger.Log( -1, id,
                                "T1 timeout occurred while reading checksum. " +
							    "Expected checksum val " +
							    ComputeChecksum(recvBuf) +
							    " Number of bytes read " + i +
							    " Current checksum " + checksum );
                        SkipBadBlock();
                        return;
                    }
                    else
                    {
                        checksum <<= 8; // Shift checksum
                        checksum |= (ch & 0xFF); // Add unsigned ch
                    }
                }
                int computedChecksum = ComputeChecksum( recvBuf );
                if( computedChecksum == checksum )
                {
                    logger.LogProtocol( -1, id, false, recvBuf );

                    try
                    {
                        _os.Write(ACK);
                        logger.LogProtocol(-1, id, true, ACK);
                    }
                    catch (SocketException e)
                    {
                        logger.Log(-1, id, "Failed to write ACK to socket.");
                        return;
                    }
                    catch (IOException e)
                    {
                        logger.Log(-1, id, "Failed to write ACK to socket.");
                        return;
                    }

                    if( IsDuplicateBlock(recvBuf) )
                    {
                        logger.Log( -1, id, "Discarding duplicate block." );
                    }
                    else
                    {
                        AddBlockToMessage(recvBuf);
                    }
                }
                else
                {
                    logger.Log( -1, id, "Checksum " + checksum +
                          " doesn't match computed checksum " +
                          computedChecksum + ".");

                    SkipBadBlock();
                }
            }
        }

        
        private bool IsDuplicateBlock( byte[] block )
        {
            if (lastRecvHdr == null)
            {
                // lastRecvHdr = new byte[10];
                // System.arraycopy(block,0,lastRecvHdr,0,10);

                // Maintain a reference to the last block received.
                // This should be cleared cleared when the Connection is
                // blocken.  For example- Eq starts, sends S1F1, Crashes, Starts
                // and sends S1F1 again.  We would throw away the second S1F1.
                lastRecvHdr = block;
                return false;
            }
            else
            {
                for (int i=0; i<10; i++)
                {
                    if (block[i] != lastRecvHdr[i])
                    {
                        //Array.Copy(block, 0, lastRecvHdr, 0, 10);
                        lastRecvHdr = block;
                        return false;
                    }
                }
            }

            return true;
        }

        
        private void SkipBadBlock()
        {
            logger.Debug( -1, id, "Skipping bad block." );

            while( WaitForInput(t1Timeout) != -1 )
            {
                // Empty
            }

            try
            {
                _os.Write(NAK);
            }
            catch (SocketException e)
            {
            }
            catch (IOException e)
            {
            }

            logger.LogProtocol( -1, id, true, NAK );
        }

        
        private void AddBlockToMessage(byte[] block)
        {
            Secs1Message msg = null;
            Secs1Header msgHdr;
            SecsMessage original;

                // Get the first 10 bytes and put that into the header.
            msgHdr = new Secs1Header(block);

            bool expectedReply = false;
            int blockId = msgHdr.GetBlockId();
         
            // Figure out if this block is a reply we're waiting for.

            // Is this the first block of a reply message?
            if (! msgHdr.IsPrimaryMessage() && (blockId==0 || blockId==1))
            {   // YES
                if (replyWaitList.ContainsKey(msgHdr))
                {
                    original = (SecsMessage)replyWaitList[msgHdr];
                }
                else
                {
                    original = null;
                }
                if( original != null )
                {
                    Timer.Timer.RemoveTimerListener( this, original );
                    expectedReply = true;
                    logger.Debug( -1, id, "Received reply, cancelled T3 timer." );

                    /* Note: We didn't remove the message from the
                    ** replyWaitList at this time.  We will wait until the
                    ** complete message has been received.
                    */
                }
            }

            // Add this block to the appropriate multiblock message.

            lock( multiBlkWaitList )
            {
                msg = null;
                for (int i=0; i<multiBlkWaitList.Count; i++)
                {
                    msg = (Secs1Message)multiBlkWaitList[i];
                    
                    if (msg.GetDeviceId()		== msgHdr.GetDeviceId() &&
                        msg.GetSystemBytes()	== msgHdr.GetSystemBytes() &&
                        msg.GetRbit()		== msgHdr.GetRbit() &&
                        msg.GetWbit()		== msgHdr.GetWbit() &&
                        msg.GetMessageId()		== msgHdr.GetMessageId() &&
                        msg.GetBlockId()+1		== msgHdr.GetBlockId() &&
                        msg.GetEbit()		== false)
                    {
                        Timer.Timer.RemoveTimerListener(this,msg);
                        multiBlkWaitList.RemoveAt(i);
                        break;
                    }
				    else
				    {
					    msg = null;
				    }
                }
                /* Now msg either points at the multi block message or is null */
            }


            if (msg == null)
            {
                if (blockId==0 || blockId==1)
                {
                    if( ! (msgHdr.IsPrimaryMessage() || expectedReply) )
                    {
                        logger.Log( msgHdr.GetDeviceId(), id,
                                "Received unexpected reply. " +
                                msgHdr.ToString() );
                    }

                    msg = new Secs1Message(block);
                }
                else
                {
                    logger.Log( msgHdr.GetDeviceId(), id,
                            "Ignored unexpected block. " + msgHdr.ToString() );
                }
            }
            else
            {
			    msg.SetHeader(block);
                msg.AppendBlock(block);
                logger.Debug( msg.GetDeviceId(), id, "Appended block to " + msg + " message." );
            }

            if( msg != null )
            {
                if( msgHdr.GetEbit() )
                {
                    logger.Debug( msg.GetDeviceId(), id, "Completed receiving " + msg );
                    logger.LogMessage( id, false, msg );
                    SecsEvent evt = new SecsEvent(SecsEvent.MESSAGE_RECEIVED, msg);

                    if( msg.IsPrimaryMessage() )
                    {
                        st.SendPrimaryEvent(evt);
                        if( msg.GetStream() == 9 &&
                                (msg.GetFunction() == 1 ||
                                msg.GetFunction() == 3 ||
                                msg.GetFunction() == 5 ||
                                msg.GetFunction() == 7 ||
                                msg.GetFunction() == 11) )
                        {
                            // This is a system error message in response
                            // to a message we sent.  The message should
                            // contain the header of the failed message.
                            S9FX s9fx = new S9FX( msg );
                            Secs2Message s2m = new Secs2Message(
                                    s9fx.GetMHEAD(), null );
                            if( s2m.GetWbit() == true )
                            {
                                SecsMessage primary = (SecsMessage)replyWaitList[s2m.GetSecsHeader()];
                                    
                                   replyWaitList.Remove( s2m.GetSecsHeader() );
                                if( primary != null )
                                {
                                    evt = new SecsEvent(
                                        SecsEvent.REPLY_NOT_RECEIVED,
                                        primary );
                                    st.SendReplyEvent(  primary, evt );
                                }
                                else // just missed timeout or sys bytes bad
                                {
                                    /* No open transaction drop message */
                                    logger.Log( msg.GetSessionId(), id,
                                        "Dropped reply error message no open " +
                                        "transaction. " +
                                        s2m.GetSecsHeader().ToString() );
                                }
                            }
                        }
                    }
                    else
                    {
                        if (replyWaitList.ContainsKey(msg.GetSecsHeader()))
                        {
                            original = (SecsMessage)replyWaitList[msg.GetSecsHeader()];
                            replyWaitList.Remove(msg.GetSecsHeader());
                        }
                        else
                        {
                            original = null;
                        }
                        if( original != null )
                        {
                            //Eventually Send Message to User Application through MessageTransfer queue
                            st.SendReplyEvent( original, evt );
                        }
                        else
                        {
                            logger.Log( msg.GetDeviceId(), id,
                                    "Failed to match secondary to primary msg. "  +
                                     msg.GetSecsHeader().ToString() );
                        }
                    }
                }
                else
                {
                    multiBlkWaitList.Add(msg);
                    Timer.Timer.AddTimerListener( this, t4Timeout, msg );
                }
            }
        }

        
        private static int ComputeChecksum(byte[] buf)
        {
            int result = 0;

            for (int i=0; i<buf.Length; i++)
            {
                result += buf[i] & 0xFF;
            }

            return result;
        }

       
        public class Secs1State
        {
            public const int SHUTDOWN = 0;
            public const int NOT_CONNECTED = 1;
            public const int CONNECTED = 2;
            public const int IDLE = 3;
            public const int RECEIVING_MESSAGE = 4;
            public const int SENT_ENQ = 5;
            public const int RECEIVED_EOT = 6;
            public const int SENT_BLOCK = 7;
            public const int RECEIVED_ACK = 8;

            private int currentState = NOT_CONNECTED;

            Secs1Connection host = null;

            private  object wait_monitor = null;

            public Secs1State(Secs1Connection host, object monitor)
            {
                this.host = host;
                this.wait_monitor = monitor;
            }
            
            public  void SetState( int newState )
            {

                lock (wait_monitor)
                {
                               
                    SecsEvent se = null;

                    if( IsState(newState) )
                    {
                        return;
                    }

                    // Don't allow anyone to change a state from shutdown.
                    if( currentState == SHUTDOWN )
                    {
                        return;
                    }

                    switch( newState )
                    {
                        case SHUTDOWN:
                            host.logger.Debug( -1, host.id, "State changing to SHUTDOWN" );
                            if( IsState(CONNECTED) )
                            {
                                se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                                host.Cleanup();
                            }
                            currentState = newState;
                            break;
                        case NOT_CONNECTED:
                            host.logger.Debug( -1, host.id, "State changing to NOT_CONNECTED" );
                            se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                            if( IsState(CONNECTED) )
                            {
                                host.Cleanup();
                            }
                            currentState = newState;
                            host.lastRecvHdr = null; // See isDuplicateMessage
                            break;
                        case CONNECTED:
                            host.logger.Debug( -1, host.id, "State changing to CONNECTED" );
                            se = new SecsEvent(SecsEvent.CONNECTION_ESTABLISHED);
                            currentState = CONNECTED;
                            newState = IDLE;
                            // Fall through

                            if (IsState(CONNECTED))
                            {
                                host.logger.Debug(-1, host.id, "State changing to IDLE");
                                currentState = newState;
                            }
                            break;
                        case IDLE:
                            if( IsState(CONNECTED) )
                            {
                                host.logger.Debug(-1, host.id, "State changing to IDLE");
                                currentState = newState;
                            }
                            break;
                        case RECEIVING_MESSAGE:
                            if( currentState == IDLE || currentState == SENT_ENQ )
                            {
                                host.logger.Debug(-1, host.id, "State changing to RECEIVING_MESSAGE");
                                currentState = newState;
                            }
                            break;
                        case SENT_ENQ:
                            if( currentState == IDLE )
                            {
                                host.logger.Debug(-1, host.id, "State changing to SENT_ENQ");
                                currentState = newState;
                            }
                            break;
                        case RECEIVED_EOT:
                            if( currentState == SENT_ENQ )
                            {
                                host.logger.Debug(-1, host.id, "State changing to RECEIVED_EOT");
                                currentState = newState;
                            }
                            break;
                        case SENT_BLOCK:
                            if( currentState == RECEIVED_EOT )
                            {
                                host.logger.Debug(-1, host.id, "State changing to SENT_BLOCK");
                                currentState = newState;
                            }
                            break;
                        case RECEIVED_ACK:
                            if( currentState == SENT_BLOCK )
                            {
                                host.logger.Debug(-1, host.id, "State changing to RECEIVED_ACK");
                                currentState = newState;
                            }
                            break;
                        default:
                            //invalid state
                            break;
                    }

                    if (se != null)
                    {
                        host.st.SendPrimaryEvent(se);
                    }
                    Monitor.PulseAll(wait_monitor);
                }
            }

            
            public void WaitForState( int neededState )
            {
                lock (wait_monitor)
                {
                    while( true )
                    {
                        // Although the state is never actually Connected.  Both
                        // selected and not_selected are substates of Connected.
                        if( IsState(neededState) )
                        {
                            break;
                        }
                        try
                        {
                            Monitor.Wait(wait_monitor);
                            
                        }
                        catch( ThreadInterruptedException e )
                        {
                        }
                    }
                }
            }

            
            public  bool WaitForState( int neededState, int timeout )
            {
                lock (wait_monitor)
                {
                    long startTime =(long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;
                    long timeLeft = timeout;

                    while( true )
                    {
                        if( IsState( neededState ) )
                        {
                            break;
                        }
                        try
                        {
                            Monitor.Wait(wait_monitor, (int)timeLeft);

                            timeLeft = (long)(timeout -
                                        (((long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds) - startTime));
                            if( timeLeft <= 0 )
                            {
                                break;
                            }
                        }
                        catch( ThreadInterruptedException e )
                        {
                        }
                    }
                    return IsState( neededState );
                }
            }

            
            public  void WaitForStateChange( int timeout )
            {

                lock (wait_monitor)
                {

                    try
                    {
                         Monitor.Wait(wait_monitor,timeout);
                    }
                    catch( ThreadInterruptedException e )
                    {
                    }
                }
            }


            
            public  bool IsState( int neededState )
            {
                lock (wait_monitor)
                {
			        if( neededState == NOT_CONNECTED &&
					        currentState == SHUTDOWN )
			        {
				        return true;
			        }
			        else if( neededState == CONNECTED &&
					        currentState != NOT_CONNECTED &&
                            currentState != SHUTDOWN )
                    {
                        return true;
                    }
                    else if( neededState == currentState )
                    {
                        return true;
                    }
                    return false;
                }
            }

            
            public int GetState()
            {
                return currentState;
            }
        }


        public void StartSendThread(string name)
        {
            sendThread = new Thread(new ThreadStart(RunSend));
            sendThread.Name = name;
            sendThread.IsBackground = true;
            sendThread.Start();
            logger.Debug( -1, id, "Running " + name );
        }





        public void RunSend()
        {
            while( !secs1State.IsState( Secs1State.SHUTDOWN ) )
            {
                /*
                 * There are three levels at which we could wait for a
                 * Connection before sending a messaga.
                 * 1- Fail immediatly if the state is not selected.
                 * 2- Wait for a length of time and then fail if a Connection
                 * was not established.
                 * 3- Wait forever for a Connection to be established.
                **/

                SecsMessage sm = st.GetSecsMessage( checkForShutdownInterval );
                if( sm == null )
                {
                    continue;
                }

                // Fail after a delay
                // if( secs1State.WaitForState( secs1State.CONNECTED, 5000 ) )
                // Fail immediatly
                if( secs1State.IsState(Secs1State.CONNECTED) )
                {
                    if( deviceId < 0 )
                    {
                        SendMessage( new Secs1Message(rBit, sm), sm );
                    }
                    else
                    {
                        SendMessage( new Secs1Message(deviceId, rBit, sm), sm );
                    }
                }

			    // Could have gone NOT_CONNECTED or SHUTDOWN when sending the
			    // last message.
			    if( secs1State.IsState(Secs1State.NOT_CONNECTED) )
                {
                    // Empty the send queue
                    do
                    {
                        if( sm.GetWbit() == true )
                        {
                            SecsEvent se = new SecsEvent(
                                                    SecsEvent.SEND_FAILED, sm );
                            st.SendReplyEvent( sm, se );
                        }
                    } while( null != (sm = st.GetSecsMessageNoWait() ) );
                }

                /*
                // Wait for ever
			    // This is bad to do because we don't periodically check to
			    // see if we need to shutdown.
                secs1State.WaitForState( secs1State.CONNECTED );
                sendMessage( new Secs1Message(deviceId, rBit, sm), sm );
                */
            }
            logger.Debug( -1, id, "Send Thread stopping" );
        }
     


        
        public void StartRecvThread(string name)
        {
            recvThread = new Thread(new ThreadStart(RunRecv));
            recvThread.Name = name;
            recvThread.IsBackground = true;
            recvThread.Start();
            logger.Debug( -1, id, "Running " + name );
        }

        public void RunRecv()
        {
            while( !secs1State.IsState( Secs1State.SHUTDOWN ) )
            {
                ReceiveMessage();
            }
            logger.Debug( -1, id, "Recv Thread stopping" );
        }
    }
}
