using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global
{
    public class ECPObject : MarshalByRefObject
    {
        public virtual bool IsAlive()
        {
            return true;
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
        
    }
}
