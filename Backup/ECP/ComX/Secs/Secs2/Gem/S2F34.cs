using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F34 : Secs2Message
    {
        public const String DRACK = "0";

        
        public S2F34( SecsHeader secsHeader )
            :this( secsHeader, (byte)0 )
        {
           
        }

        
        public S2F34( SecsHeader secsHeader, byte drack )
            :base( secsHeader, new ItemB(drack) )
        {
           
        }

       
        public S2F34( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

       
        public void SetDRACK( byte  drack )
        {
            SetMessageBody( new ItemB(drack) );
        }

       
        public byte GetDRACK()
        {
            return ((ItemB)Find(DRACK)).GetData()[0];
        }
    }
}
