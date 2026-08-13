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


namespace ECP.ComX.Secs.Hsms
{
    public class HsmsConnection : TimerListener, SocketListener
    {
        public const int REJECT_STYPE = 1;
        public const int REJECT_PTYPE = 2;
        public const int REJECT_TRANSACTION = 3;
        public const int REJECT_NOT_IN_SELECT = 4;

        
        private int				t3Timeout	= 45000;	// 45.0 seconds

        
        private int				t5Timeout	= 10000;	// 10.0 seconds


        private int				t6Timeout	= 5000;		// 5.0 seconds


        private int				t7Timeout	= 10000;	// 10.0 seconds


        private int				t8Timeout	= 5000;		// 5.0 seconds


        private int				sessionId	= -1;


        private int				pType		= 0;		// SecsII msgs


        private bool			active		= true;


        private HsmsState hsmsState = null;


        private String			id			= null;


	    private bool			enabled		= true;


        private byte[]			lastRecvHdr	= null;


        private SecsTransfer	st			= null;

        private PersistentSocket	socket	= null;

	
        private SocketInputStream 	_is		= null;

	  
        private SocketOutputStream 	_os		= null;

	  
        private Dictionary<SecsHeader, SecsMessage> replyWaitList	= new Dictionary<SecsHeader,SecsMessage>();

	    
        private  SecsLogger		logger		= null;

	    
        private static int      checkForShutdownInterval = 5000;

	    
	    private long selectedTimeout = 0;

        private  object lock_object = new object();

        Thread sendThread = null;

        Thread recvThread = null;



         
        public HsmsConnection( String id, String hostname, int port, bool active,
                                    SecsTransfer st, SecsLogger logger )
        {
            this.st = st;
            this.logger = logger;
            this.active = active;
		    PersistentSocket socket;

            this.hsmsState = new HsmsState(this);

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
			    }
			    else
			    {
				    socket = new PersistentServerSocket( host, port );
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

			    this.StartSendThread("HsmsSend/" + id);
                this.StartRecvThread("HsmsRecv/" + id);
		    }


            
        }

        
        public void SocketChanged(SocketEvent e)
        {
            switch (e.Type)
            {
			    case SocketEvent.CONNECTION_ESTABLISHED:
				    hsmsState.SetState( HsmsState.CONNECTED );
				    break;

			    case SocketEvent.CONNECTION_LOST:
				    hsmsState.SetState( HsmsState.NOT_CONNECTED );
				    break;

			    case SocketEvent.CONNECTION_ERROR:
				    logger.Log( -1, id,
						    "Error received from the persistent socket. " +
						    e.Text );
				    SecsEvent se = new SecsEvent(
						    SecsEvent.CONNECTION_ERROR,
						    "Failed to establish a connection, if the tool is " +
						    "operational contact the system administrator" );
				    st.SendPrimaryEvent(se);
				    break;
            }
        }

        
        public String GetId()
        {
            return id;
        }

        
        public int GetSessionId()
        {
            return sessionId;
        }

        
        public int GetPType()
        {
            return pType;
        }

        
        public int GetT3Timeout()
        {
            return t3Timeout;
        }

        
        public int GetT5Timeout()
        {
            return t5Timeout;
        }

        
        public int GetT6Timeout()
        {
            return t6Timeout;
        }

        
        public int GetT7Timeout()
        {
            return t7Timeout;
        }

        
        public int GetT8Timeout()
        {
            return t8Timeout;
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

        
        public  void Connect()
        {
            lock(lock_object)
            {
                if( socket != null && enabled )
                {
                    try
                    {
                        logger.Debug( -1, id, "Trying to establish connection." );
                        socket.Connect();
                    }
                    catch (IOException e)
                    {
                        logger.Log( -1, id, "Connection attempt failed." );
                    }
                }
		        else
		        {
			        String message = "Connect not possible.  See logs for reason. " +
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

        
        public  void Disconnect()
        {
            lock (lock_object)
            {
		        if( socket != null )
		        {
			        if( hsmsState.GetState() == HsmsState.SELECTED )
			        {
				        SendControlMessage( HsmsMessage.SEPARATE_REQ );
			        }
			        socket.Disconnect();
			        logger.Debug( -1, id, "Connection dropped." );
		        }
            }
        }

        
        private void BounceConnection()
        {
            logger.Debug( -1, id, "Bouncing the connection" );
            Disconnect();
            try
            {
                Thread.Sleep( t5Timeout );
            }
            catch( Exception ex )
            {
                System.Console.WriteLine("StopMonitorThread:" + ex.StackTrace);
            }
            Connect();

        }

        
        public  void Dispose()
        {
            lock (lock_object)
            {
		        if( socket != null && enabled )
		        {
			        SetEnable( false );
			        Disconnect();
			        logger.Debug( -1, id, "Connection has been disposed." );

			        st.Close();

			        
			        hsmsState.SetState( HsmsState.SHUTDOWN );
		        }
            }
        }

        
        public void SetSessionId(int sessionId)
        {
            this.sessionId = sessionId;
        }

        
        public void SetPType(int pType)
        {
            this.pType = pType;
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

        
        public void SetT5Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 240000  )
            {
                t5Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T5 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public void SetT6Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 240000  )
            {
                t6Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T6 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public void SetT7Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 240000  )
            {
                t7Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T7 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

        
        public void SetT8Timeout(int timeout)
        {
            if( timeout >= 1000 && timeout <= 120000  )
            {
                t8Timeout = timeout;
            }
		    else
		    {
			    logger.Log( -1, id, "T8 timeout(" + timeout +
					    ") not accepted, out of range." );
		    }
        }

	    
	    private void SetEnable( bool value )
	    {
		    enabled = value;
	    }

        
        public void ProcessTimeout(Object data)
        {
            if( data != null && data is SecsMessage )
            {
                SecsMessage msg = (SecsMessage) data;
                msg = RemoveReplyWaitList( msg );
                if( msg != null )
                {
                    // Is this a hsms control message
                    if( msg is HsmsMessage &&
                        ((HsmsMessage)msg).GetSType() != HsmsMessage.DATA_MESSAGE )
                    {
                        // T6 timeout: reply not received.
                        logger.Log( -1, id, "T6 timeout occurred. " +
							    msg.GetSecsHeader().ToString() );
                        hsmsState.SetState( HsmsState.NOT_CONNECTED );
                        BounceConnection();
                    }
                    else // This is a secs message.
                    {
                        // T3 timeout: reply not received.
                        logger.Log( msg.GetSessionId(), id,
                                "T3 timeout occurred. " +
							    msg.GetSecsHeader().ToString() );
                        SecsEvent evt = new SecsEvent(
                                            SecsEvent.REPLY_NOT_RECEIVED,
                                            msg );
                        st.SendReplyEvent( msg, evt );
                        return;
                    }
                }
            }
        }

        
        private void SendControlMessage( int messageType )
        {
            HsmsMessage control = new HsmsMessage();

            control.SetSessionId( 0xFFFF );
            control.SetByte2(0);
            control.SetByte3(0);
            control.SetPType(0);
            control.SetSType(messageType);

            object lockObject = new object();

            if( messageType != HsmsMessage.SEPARATE_REQ )
            {
                AddReplyWaitList( t6Timeout, control );
                lockObject = control;
            }

            lock( lockObject )
            {
                if( SendBlock(control.ToByteArray( )) )
                {
                    logger.LogProtocol( -1, id, true, control );
                }
                else
                {
                    logger.Log( -1, id, "Failed to send control message. " +
                    control.GetSecsHeader().ToString() );

                    // If the message was added to the reply wait list we need to
                    // remove it now.
                    if( lockObject == control )
                    {
                        RemoveReplyWaitList( control );
                    }
                }
            }
        }

        
        private void SendRejectRequest( int byte2, int reason, HsmsMessage msg )
        {
            HsmsMessage reject;

            reject = new HsmsMessage( (HsmsHeader)msg.GetSecsHeader() );
            reject.SetByte2( byte2 );
            reject.SetByte3( reason );
            reject.SetSType( HsmsMessage.REJECT_REQ );
            reject.SetPType( 0 );

            logger.LogProtocol( msg.GetSessionId(), id, true, reject );
            SendBlock( reject.ToByteArray() );
        }


        
        private void SendMessage( HsmsMessage message, SecsMessage original )
        {
            object lockObject = new object();

            if( (original != null) && (message.GetWbit() == true) )
            {
                AddReplyWaitList( t3Timeout, original );
                lockObject = original;
            }

            lock( lockObject )
            {
                if( SendBlock(message.ToByteArray()) == false )
                {
                    logger.Log( message.GetSessionId(), id, "Failed to send message " +
                            message.GetSecsHeader().ToString() );
          
                    if( (original != null) && (message.GetWbit() == true) )
                    {
                        SecsEvent se = new SecsEvent( SecsEvent.SEND_FAILED, original );
                        SendEvent( original, se );
                        RemoveReplyWaitList( original );
                    }
                    return;
                }

                if( original != null )
                {
                    // A message from the device driver. (see above comment)
                    logger.LogMessage( id, true, message );
                }
                else
                {
                    // A control message.
                    logger.LogProtocol( -1, id, true, message );
                }
            }
        }

        
        private bool SendBlock(byte[] block)
        {
            byte[] tmp = new byte[4+block.Length];

            lock( _os )
            {
                // Calculate the length bytes
                tmp[0] = (byte)( (block.Length >> 24) & 0xFF);
                tmp[1] = (byte)( (block.Length >> 16) & 0xFF);
                tmp[2] = (byte)( (block.Length >> 8) & 0xFF);
                tmp[3] = (byte)( block.Length & 0xFF);

                Array.Copy(block, 0, tmp, 4, block.Length );                

                // Send message
                try
                {
                    _os.Write(tmp, 0, tmp.Length);
                }
                catch (IOException e)
                {
                    logger.Log(-1, id, "Failed to write message to socket.");
                    return false;
                }
                catch (SocketException e2)
                {
                    logger.Log(-1, id, "Failed to write message to socket.");
                    return false;
                }

                
            }
            return true;
        }

        
        private void ReceiveMessages()
        {
            try
            {
                if( hsmsState.WaitForState(
                    HsmsState.CONNECTED, checkForShutdownInterval) )
                {
                    ReceiveMessage();
                }
            }
            catch (IOException e)
            {
                logger.Log( -1, id, "IO Exception while receving messages: " +
					    e.ToString() );
                return;
            }

		    if( hsmsState.IsState(HsmsState.NOT_SELECTED) )
		    {
			    if( new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds > selectedTimeout )
			    {
                    logger.Log( -1, id, "T7 Timeout- Failed to go to SELECTED state" );
                    BounceConnection();
			    }
		    }
        }

        
        private void ReceiveMessage() 
        {
            HsmsMessage reply;

            // Read the length bytes
            //socket.SetSoTimeout( t8Timeout );
            byte[] b = new byte[4];

            int chRead = 0; // The number of characters read so far
            int chs = 0; // The number of characters read this time
            int chToRead = b.Length; // The number of characters to be read

            socket.SetSoTimeout(0);

            try
            {
			    while( chRead != chToRead )
			    {
				    chs = _is.Read( b, chRead, chToRead - chRead );

                    socket.SetSoTimeout(5000);

				    if( chs == 0 )
				    {
					    logger.Log( -1, id, "Got EOF when reading length bytes" );
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
                    logger.Log(-1, id, "Got EOF when reading length bytes");
                    BounceConnection();
                    return;
                }
                logger.Log(-1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
                        "length bytes. Read " + chRead + " of " + 4 + " bytes.");
                BounceConnection();
                return;
            }
            catch( IOException e )
            {
			    if( chRead == 0 )
			    {
                    logger.Log(-1, id, "Got EOF when reading length bytes");
                    BounceConnection();
				    return;
			    }
			    logger.Log( -1, id, "T8 Timeout- Intercharacter timeout.  Reading " +
					    "length bytes. Read " + chRead + " of " + 4 + " bytes." );
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
                    if (chs == 0)
                    {
                        logger.Log(-1, id, "Got EOF when reading message");
                        BounceConnection();
                        return;
                    }
                    chRead += chs;
                }
            }
            catch (SocketException e)
            {
                StringBuilder sb = new StringBuilder();

                int ch;

                for (int i = 0; i < chRead && i < 10; i++)
                {
                    ch = m[i] & 0xFF;

                    if (ch < 16)
                    {
                        sb.Append("0");
                    }

                    sb.Append(String.Format("{0:X}", ch) + " ");
                }

                logger.Log(-1, id, "T8 Timeout- Intercharacter timeout. " +
                        "Read " + chRead + " of " + messageLength + " characters." +
                        " First characters read: " + sb.ToString());
                BounceConnection();
                return;
            }
            catch (IOException e)
            {
                StringBuilder sb = new StringBuilder();

                int ch;

                for (int i = 0; i < chRead && i < 10; i++)
                {
                    ch = m[i] & 0xFF;

                    if (ch < 16)
                    {
                        sb.Append("0");
                    }

                    sb.Append(String.Format("{0:X}", ch) + " ");
                }

                logger.Log(-1, id, "T8 Timeout- Intercharacter timeout. " +
                        "Read " + chRead + " of " + messageLength + " characters." +
                        " First characters read: " + sb.ToString());
                BounceConnection();
                return;
            }

            // Create the message
            HsmsMessage msg = null;
            try
            {
                msg = new HsmsMessage( m );
            }
            catch( Exception ex )
            {
                // Failed to parse the message.
                logger.Log( -1, id, "Failed to parse message.", false, m );
                BounceConnection();
                return;
            }

            if( IsDuplicateMessage( m ) )
            {
                logger.Log( msg.GetSessionId(), id, "Duplicate message ignored. " +
					    msg.GetSecsHeader().ToString() );
                return;
            }

            switch( msg.GetSType() )
            {
                case HsmsMessage.DATA_MESSAGE:
                    if( hsmsState.GetState() != HsmsState.SELECTED )
                    {
                        logger.LogMessage( id, false, msg );
                        logger.Log( msg.GetSessionId(), id,
                                "Rejected message not in select state. " +
							    msg.GetSecsHeader().ToString() );
                        SendRejectRequest(
                                    msg.GetSType(), REJECT_NOT_IN_SELECT, msg );
                    }

                        // Message type does not match supported message type.
                    else if( msg.GetPType() != GetPType() )
                    {
                        logger.LogMessage( id, false, msg );
                        logger.Log( msg.GetSessionId(), id,
                                "Rejected message incorrect P-Type. " +
							    msg.GetSecsHeader().ToString() );
                        SendRejectRequest( msg.GetPType(), REJECT_PTYPE, msg );
                    }
                    else
                    {
                        SecsEvent evt = new SecsEvent(
                                                SecsEvent.MESSAGE_RECEIVED, msg );

                        if( msg.IsPrimaryMessage() )
                        {
                            if( msg.GetStream() == 9 &&
                                    (msg.GetFunction() == 1 ||
                                    msg.GetFunction() == 3 ||
                                    msg.GetFunction() == 5 ||
                                    msg.GetFunction() == 7 ||
                                    msg.GetFunction() == 11) )
                            {
                                S9FX s9fx = new S9FX( msg );
                                Secs2Message s2m = new Secs2Message(
                                        s9fx.GetMHEAD(), null );
                                SecsMessage primary = null;
                                if( s2m.GetWbit() == true )
                                {
                                    primary = RemoveReplyWaitList(s2m);
                                }
                                if( primary != null )
                                {
                                    lock( primary )
                                    {
                                        logger.LogMessage( id, false, msg );
                                    }
								    st.SendPrimaryEvent(evt);
                                    evt = new SecsEvent(
                                        SecsEvent.REPLY_NOT_RECEIVED,
                                        primary );
                                    st.SendReplyEvent(  primary, evt );
                                }
                                else // missed a timeout, sys bytes bad, wbit false
                                {
                                    logger.LogMessage( id, false, msg );
                        
                                    if( s2m.GetWbit() == true )
                                    {
                                        logger.Log(msg.GetSessionId(), id,
                                            "Dropped reply error message no open " +
                                            "transaction. " +
                                            s2m.GetSecsHeader().ToString());
                                    }
								    st.SendPrimaryEvent(evt);
                                }
                            }
                            else
                            {
                                // Regular primary message
                                logger.LogMessage(id, false, msg);
							    st.SendPrimaryEvent(evt);
                            }
                        }
                        else // This is Reply
                        {
                            SecsMessage primary = RemoveReplyWaitList( msg );
                            if( primary != null )
                            {
                                lock( primary )
                                {
                                    msg.SetPrimaryMessage(primary);
                                    logger.LogMessage( id, false, msg );
                                }
                                st.SendReplyEvent(  primary, evt );
                            }
                            else /* just missed a timeout or sys bytes were bad */
                            {
                                logger.LogMessage( id, false, msg );
                                /* No open transaction drop message */
                                logger.Log( msg.GetSessionId(), id,
                                    "Dropped reply data message no open " +
								    "transaction. " +
								    msg.GetSecsHeader().ToString() );
                            }
                        }
                    }
                    break;
                case HsmsMessage.SELECT_REQ:
                    logger.LogProtocol( -1, id, false, msg );
                    reply = new HsmsMessage(msg.GetHsmsHeader());
                    reply.SetSType( HsmsMessage.SELECT_RSP );
                    SendMessage( reply, null );
                    hsmsState.SetState( HsmsState.SELECTED );
                    break;
                case HsmsMessage.SELECT_RSP:
                    logger.LogProtocol( -1, id, false, msg );
                    if( msg.GetByte3() == 0 )
                    {
                        hsmsState.SetState( HsmsState.SELECTED );
                    }
                    else
                    {
                        logger.Log(-1, id, "Rejected Select Request: " +
                                   msg.GetSecsHeader().ToString());
                    }
                    if( null == RemoveReplyWaitList(msg) )
				    {
		
                        logger.Log( -1, id,
							    "Rejected select response no open transaction. " +
							    msg.GetSecsHeader().ToString() );
					    SendRejectRequest(
							    msg.GetSType(), REJECT_TRANSACTION, msg );
				    }
                    break;
                case HsmsMessage.DESELECT_REQ:
                    logger.LogProtocol( -1, id, false, msg );
                    lock( hsmsState )
                    {
                        if( hsmsState.IsState( HsmsState.SELECTED ) )
                        {
                            hsmsState.SetState( HsmsState.NOT_SELECTED );
                        }
                    }
                    reply = new HsmsMessage(msg.GetHsmsHeader());
                    reply.SetSType( HsmsMessage.DESELECT_RSP );
                    SendMessage( reply, null );
                    break;
                case HsmsMessage.DESELECT_RSP:
                    logger.LogProtocol( -1, id, false, msg );
                    if( msg.GetByte3() == 0 )
                    {
                        lock( hsmsState )
                        {
                            if( hsmsState.IsState( HsmsState.SELECTED ) )
                            {
                                hsmsState.SetState( HsmsState.NOT_SELECTED );
                            }
                        }
                    }
                    if( null == RemoveReplyWaitList(msg) )
				    {
					    // We timed out waiting for the reply, reject message.
					    logger.Log( -1, id,
							    "Rejected deselect response no open transaction. " +
							    msg.GetSecsHeader().ToString() );
					    SendRejectRequest(
							    msg.GetSType(), REJECT_TRANSACTION, msg );
				    }
                    break;
                case HsmsMessage.LINKTEST_REQ:
                    logger.LogProtocol( -1, id, false, msg );
                    reply = new HsmsMessage(msg.GetHsmsHeader());
                    reply.SetSType( HsmsMessage.LINKTEST_RSP );
                    SendMessage( reply, null );
                    break;
                case HsmsMessage.LINKTEST_RSP:
                    logger.LogProtocol( -1, id, false, msg );
                    if( null == RemoveReplyWaitList(msg) )
				    {
					    // We timed out wait ing for the reply, reject message.
					    logger.Log( -1, id,
							    "Rejected link test response no open " +
							    "transaction. " +
							    msg.GetSecsHeader().ToString() );
					    SendRejectRequest(
							    msg.GetSType(), REJECT_TRANSACTION, msg );
				    }
                    break;
                case HsmsMessage.REJECT_REQ:
                    logger.LogProtocol( -1, id, false, msg );
                    // If they rejected by transaction it was a secondary message
                    // so we should not look it up in our wait list.
                    if( msg.GetByte3() != REJECT_TRANSACTION )
                    {
                        RemoveReplyWaitList( msg );
                    }
                    break;
                case HsmsMessage.SEPARATE_REQ:
                    logger.LogProtocol( -1, id, false, msg );
                    // Set it to NOT_CONNECTED so we wont also send a separate req
                    hsmsState.SetState( HsmsState.NOT_CONNECTED );
                    BounceConnection();
                    break;
                default:
                    logger.LogProtocol( -1, id, false, msg );
                    logger.Log( -1, id, "Rejected message invalid S-Type. " +
						    msg.GetSecsHeader().ToString() );
                    SendRejectRequest( msg.GetSType(), REJECT_STYPE, msg );
                    break;
            }
        }

        
        private void AddReplyWaitList( int timeout, SecsMessage message )
        {
            replyWaitList.Add( message.GetSecsHeader(), message );
            
            Timer.Timer.AddTimerListener( this, timeout, message );
        }

       
        private SecsMessage RemoveReplyWaitList( SecsMessage message )
        {
            if (!replyWaitList.ContainsKey(message.GetSecsHeader())) return null;
            else
            {
                SecsMessage msg = (SecsMessage)replyWaitList[message.GetSecsHeader()];

                replyWaitList.Remove(message.GetSecsHeader());
                if (msg != null)
                {
                    Timer.Timer.RemoveTimerListener(this, msg);
                }
                return msg;
            }
        }

       
        public void EmptyReplyWaitList()
        {
            if (replyWaitList.Count == 0) return;

            foreach(SecsMessage s in replyWaitList.Values)
            {
                SecsMessage msg = RemoveReplyWaitList(s );
                if( msg != null )
                {
                    // Is this a hsms control message
                    if( msg is HsmsMessage &&
                        ((HsmsMessage)msg).GetSType() != HsmsMessage.DATA_MESSAGE )
                    {
                        // Do nothing
                    }
                    else // This is a secs message.
                    {
                        SecsEvent evt = new SecsEvent(
                                            SecsEvent.REPLY_NOT_RECEIVED,
                                            msg );
                        st.SendReplyEvent( msg, evt );
                        return;
                    }
                }
            }


            
        }

       
        private bool IsDuplicateMessage(byte[] block)
        {
            if (lastRecvHdr == null)
            {
                
                lastRecvHdr = block;
                return false;
            }
            else
            {
                for (int i=0; i<10; i++)
                {
                    if (block[i] != lastRecvHdr[i])
                    {
                        lastRecvHdr = block;
                        return false;
                    }
                }
            }

            return true;
        }


       
        private void SendEvent( SecsMessage sm, SecsEvent se )
        {
            if( (sm != null) && sm.GetWbit() )
            {
                st.SendReplyEvent( sm, se );
            }
            else
            {
                st.SendPrimaryEvent( se );
            }
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
            SecsMessage sm;

            while (!hsmsState.IsState(HsmsState.SHUTDOWN))
            {
                

                sm = st.GetSecsMessage(checkForShutdownInterval);
                if (sm == null)
                {
                    continue;
                }

                
                if (hsmsState.IsState(HsmsState.SELECTED))
                {
                    if (sessionId < 0)
                    {
                        SendMessage(new HsmsMessage(pType, sm), sm);
                    }
                    else
                    {
                        SendMessage(
                                new HsmsMessage(sessionId, pType, sm), sm);
                    }
                }
                else
                {
                    do
                    {
                        if (sm.GetWbit() == true)
                        {
                            SecsEvent se = new SecsEvent(
                                                    SecsEvent.SEND_FAILED, sm);
                            SendEvent(sm, se);
                        }
                    } while (null != (sm = st.GetSecsMessageNoWait()));
                }
                
            }
            logger.Debug(-1, id, "Send Thread stopping");
        }


        public void StartRecvThread(string name)
        {
            recvThread = new Thread(new ThreadStart(RunRecv));
            recvThread.Name = name;
            recvThread.IsBackground = true;
            recvThread.Start();
        }

        public void RunRecv()
        {
            while (!hsmsState.IsState(HsmsState.SHUTDOWN))
            {
                ReceiveMessages();
            }
            logger.Debug(-1, id, "Recv Thread stopping");
        }
        

        public class HsmsState
        {
            public const int SHUTDOWN = 0;
            public const int NOT_CONNECTED = 1;
            public const int CONNECTED = 2;
            public const int NOT_SELECTED = 3;
            public const int SELECTED = 4;

            private int currentState = NOT_CONNECTED;

           
            HsmsConnection host = null;

            private object wait_monitor = new object();

            public HsmsState(HsmsConnection host)
            {
                this.host = host;
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

                    if( currentState == SHUTDOWN )
                    {
                        return;
                    }

                    switch( newState )
                    {
                        case SHUTDOWN:
                            if( currentState == SELECTED )
                            {
                                se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                            }
                            host.logger.Debug( -1, host.id, "State changing to SHUTDOWN" );
                            currentState = newState;
                            break;
                        case NOT_CONNECTED:
                            if( currentState == SELECTED )
                            {
                                se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                            }
                            host.EmptyReplyWaitList();
                            host.logger.Debug( -1, host.id, "State changing to NOT_CONNECTED" );
                            currentState = newState;
                            host.lastRecvHdr = null; // See isDuplicateMessage
                            break;
                        case CONNECTED:
                            newState = NOT_SELECTED; 
                            // No break here on purpose;

                            if (currentState == SELECTED)
                            {
                                se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                            }
                            host.logger.Debug(-1, host.id, "State changing to NOT_SELECTED");
                            host.selectedTimeout = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds + host.t7Timeout;
                            currentState = newState;
                            if (host.active)
                            {
                                host.SendControlMessage(HsmsMessage.SELECT_REQ);
                            }
                            break;

                        case NOT_SELECTED:
                            if( currentState == SELECTED )
                            {
                                se = new SecsEvent(SecsEvent.CONNECTION_LOST);
                            }
                            host.logger.Debug( -1, host.id, "State changing to NOT_SELECTED" );
					        host.selectedTimeout = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds + host.t7Timeout;
                            currentState = newState;
                            if( host.active )
                            {
                               host.SendControlMessage( HsmsMessage.SELECT_REQ );
                            }
                            break;
                        case SELECTED:
                            if( currentState != NOT_SELECTED )
                            {
                                return;
                            }
                            host.logger.Debug( -1, host.id, "State changing to SELECTED" );
                            se = new SecsEvent(SecsEvent.CONNECTION_ESTABLISHED);
                            currentState = newState;
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

            
            public  void WaitForState( int neededState )
            {
                lock (wait_monitor)
                {
                    while( true )
                    {
                        if( IsState( neededState ) )
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

                    long startTime = (long)new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds;
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
                                        (new TimeSpan(DateTime.Now.Ticks).TotalMilliseconds - startTime));

                            
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

            
            public  bool IsState( int neededState )
            {
                lock (wait_monitor)
                { 
                    if( (neededState == CONNECTED) &&
                        (currentState != NOT_CONNECTED &&
                            currentState != SHUTDOWN) )
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
    }
}
