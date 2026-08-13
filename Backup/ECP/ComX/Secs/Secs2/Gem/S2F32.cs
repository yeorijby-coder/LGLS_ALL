using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F32 : Secs2Message
    {
         public const String TIACK = "0";

       
        public S2F32( SecsHeader secsHeader )
            : base( secsHeader, new ItemB((byte)0) )
        {
           
        }

        
        public S2F32( SecsHeader secsHeader, byte ackcode )
            : base( secsHeader, new ItemB(ackcode) )
        {
           
        }

        
        public S2F32( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

        
        public void SetTIACK( byte  tiack )
        {
            SetMessageBody( new ItemB(tiack) );
        }

        
        public byte GetTIACK()
        {
            return ((ItemB)Find(TIACK)).GetData()[0];
        }
    }
}
