using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F16 : Secs2Message
    {
         public const String EAC = "0";

        
        public S2F16( SecsHeader secsHeader )
          :this( secsHeader, (byte)0)
        {
            
        }

        
        public S2F16( SecsHeader secsHeader, byte eac )
            : base( secsHeader, new ItemB(eac) )
        {
            
        }

        
        public S2F16( SecsMessage secsMessage )
            : base(secsMessage)
        {
            
        }

        
        public void SetEAC(byte eac)
        {
            SetMessageBody( new ItemB(eac) );
        }

        
        public byte GetEAC()
        {
            return (byte)((ItemB)Find(EAC)).GetDatum(0);
        }
    }
}
