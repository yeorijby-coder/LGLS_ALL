using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F38 : Secs2Message
    {
        public const String ERACK = "0";

        
        public S2F38( SecsHeader secsHeader )
            :this( secsHeader, (byte)0 )
        {
          
        }

        
        public S2F38( SecsHeader secsHeader, byte erack )
            :base( secsHeader, new ItemB(erack) )
        {
           
        }

       
        public S2F38( SecsMessage secsMessage )
            :base( secsMessage )
        {
          
        }

       
        public void SetERACK( byte  erack )
        {
            SetMessageBody( new ItemB(erack) );
        }

       
        public byte GetERACK()
        {
            return ((ItemB)Find(ERACK)).GetData()[0];
        }
    }
}
