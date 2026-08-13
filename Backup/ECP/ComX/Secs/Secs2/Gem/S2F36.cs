using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F36 : Secs2Message
    {
        public const String LRACK = "0";

        
        public S2F36( SecsHeader secsHeader )
            :this( secsHeader, (byte)0 )
        {
           
        }

       
        public S2F36( SecsHeader secsHeader, byte lrack )
            :base( secsHeader, new ItemB(lrack) )
        {
        
        }

        
        public S2F36( SecsMessage secsMessage )
            :base( secsMessage )
        {
        
        }

        public void SetLRACK( byte  lrack )
        {
            SetMessageBody( new ItemB(lrack) );
        }

        
        public byte GetLRACK()
        {
            return ((ItemB)Find(LRACK)).GetData()[0];
        }
    }
}
