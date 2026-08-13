using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S5F2 : Secs2Message
    {
        public const String ACKC5 = "0";

        
        public S5F2( SecsHeader secsHeader )
            :this( secsHeader, (byte)0 )
        {
           
        }

       
        public S5F2( SecsHeader secsHeader, byte ackc5 )
            : base( secsHeader, new ItemB(ackc5))
        {
            
        }

        
        public S5F2( SecsMessage secsMessage )
            : base( secsMessage )
        {
            
        }

        
        public void SetACKC5( byte  ackc5 )
        {
            SetMessageBody( new ItemB(ackc5) );
        }

        
        public byte GetACKC5()
        {
            return ((ItemB)Find(ACKC5)).GetData()[0];
        }
    }
}
