using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F1 : Secs2Message
    {

        public S1F1() :base(1, 1, null)
        {
           
        }
       
        public S1F1(SecsMessage secsMessage): base(secsMessage)
        {
            
        }
        
        public override Secs2Message GetSecondary()
        {
            return new S1F2(GetSecsHeader().NewSecondaryHeader());
        }
    }
}
