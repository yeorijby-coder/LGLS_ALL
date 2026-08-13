using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class SXF0 : Secs2Message
    {

        public SXF0(SecsHeader secsHeader) : base (secsHeader, null)
        {
         
            SetFunction(0);
            GetSecsHeader().SetWbit(false);
        }

        public SXF0(SecsMessage secsMessage) :base(secsMessage)
        {

        }
    }
}
