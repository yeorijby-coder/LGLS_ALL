using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S9F3 : S9FX
    {

        public S9F3()
            : this(new SecsHeader(emptyHeader))
        {
        }

        
        public S9F3(SecsHeader mhead)
            :base(3, mhead)
        {
            
        }

       
        public S9F3(SecsMessage secsMessage)
            : base(secsMessage)
        {
            
        }

    }
}
