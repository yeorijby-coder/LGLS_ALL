using System;
using System.Collections.Generic;
using System.Text;

using ECP.Util;
using ECP.ComX.Secs;

namespace ECP.ComX.Prj
{
    public class SecsMessageTransfer : MessageTransfer, SecsTransfer
    {
        private int deviceId = -1;

        

        
        public SecsMessageTransfer( int deviceId ) : base()
        {
           
            this.deviceId = deviceId;
        }

        
        public SecsMessageTransfer( String deviceName )
            : base(deviceName)
        {
            
        }


       
        public SecsMessage SendMessage( SecsMessage message )
        {
            if( deviceId != -1 )
            {
                message.SetSessionId( deviceId );
            }
            if( message.GetWbit() )
            {
                SecsEvent se;

                // Only one thread can be waiting for a reply at a time.
                lock( this )
                {
                    SendMessageToListenersWithReply( message );
                    se = (SecsEvent)GetMessageWait();
                }
                if( se.GetType() != SecsEvent.MESSAGE_RECEIVED )
                {
                    throw new SecsException(
                                    se.GetType(), se.ToString(), se.GetMessage() );
                }
                else if( se.GetMessage().GetFunction() == 0 )
                {
                    throw new SecsException(
                        SecsException.FUNCTION_ZERO, "Tool replied with function 0", se.GetMessage() );
                }
                return se.GetMessage();
            }
            else
            {
                SendMessageToListeners( message );
                return null;
            }
        }

        
        public SecsMessage GetMessage() 
        {
            SecsEvent se = (SecsEvent)GetMessageWait();
            if( se.GetType() != SecsEvent.MESSAGE_RECEIVED )
            {
                throw new SecsException(
                                se.GetType(), se.ToString(), se.GetMessage() );
            }

            return se.GetMessage();
        }

        
        public SecsMessage GetMessage(int timeout)
        {
            SecsEvent se = (SecsEvent)GetMessageWithTimeout( timeout );
		    if( se == null )
		    {
			    return null;
		    }
            if( se.GetType() != SecsEvent.MESSAGE_RECEIVED )
            {
                throw new SecsException(
                                se.GetType(), se.ToString(), se.GetMessage() );
            }

            return se.GetMessage();
        }


        
        public void SendPrimaryEvent( SecsEvent message )
        {
            MessageTransfer mt;

            // Do our own looping through the listeners.

            foreach(MessageTransfer m in GetListeners().Values)
            {
                mt = m;

                // See if e is an instance of SMT (not just MessageTransfer)
                if( mt is SecsMessageTransfer )
                {
                    // Are was sending a secs Message back.
                    if( message.GetType() == SecsEvent.MESSAGE_RECEIVED )
                    {
                        // Make sure the listening SMT has the same devId as the msg
                        if( (message.GetMessage().GetSessionId() ==
                                            ((SecsMessageTransfer)mt).deviceId) ||
                             // if devId is -1 the SMT is listening to all msgs.
                             (((SecsMessageTransfer)mt).deviceId == -1) )
                        {
                            // Send the msg.  The deviceId matches.
                            SendMessage( mt,  message );
                        }
                    }
                    else
                    {
                        // Send the msg.  It is an event which should be broadcast.
                        SendMessage( mt,  message );
                    }
                }
                else
                {
                    // Send the msg.  It is a MessageTransfer object.
                    SendMessage( mt,  message );
                }
            }
        }

        
        public void SendReplyEvent(
                        SecsMessage primaryMessage, SecsEvent secondaryEvent )
        {
            SendReplyMessage( primaryMessage, secondaryEvent );
        }

        
        public SecsMessage GetSecsMessage( int timeout )
        {
            return (SecsMessage)GetMessageWithTimeout( timeout );
        }

        
        public SecsMessage GetSecsMessageNoWait()
        {
            return (SecsMessage)GetMessageNoWait();
        }
    }
}
