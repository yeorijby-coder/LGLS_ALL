using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F17 : Secs2Message
    {        
        public S1F17()
            : base(1, 17, null)
        {
            
        }
        
        public S1F17(SecsMessage secsMessage)
         : base (secsMessage)
        {

        }
        
        public override Secs2Message GetSecondary()
        {
            return new S1F18(GetSecsHeader().NewSecondaryHeader());
        }
    }
}
