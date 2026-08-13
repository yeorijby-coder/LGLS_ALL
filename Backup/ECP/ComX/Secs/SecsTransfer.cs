using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs
{
    public interface SecsTransfer
    {
        void SendPrimaryEvent( SecsEvent sevent );

        
        void SendReplyEvent(
                            SecsMessage primaryMessage, SecsEvent secondaryEvent );

       
        SecsMessage GetSecsMessage( int timeout );

        
        SecsMessage GetSecsMessageNoWait();

        
        void Close();
    }
}
