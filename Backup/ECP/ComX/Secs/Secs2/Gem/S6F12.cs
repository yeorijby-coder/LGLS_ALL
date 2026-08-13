using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S6F12 : Secs2Message
    {
        public const String ACKC6 = "0";

        
        public S6F12( SecsHeader secsHeader )
            :this( secsHeader, (byte)0 )
        {
            
        }

        
        public S6F12( SecsHeader secsHeader, byte ackc6 )
            : base(secsHeader, new ItemB(ackc6))
        {
           
        }

        
        public S6F12( SecsMessage secsMessage )
           : base(secsMessage)
        {
           
        }

        
        public void SetACKC6( byte  ackc6 )
        {
            SetMessageBody( new ItemB(ackc6) );
        }

        
        public byte GetACKC6()
        {
            return ((ItemB)Find(ACKC6)).GetData()[0];
        }
    }
}
