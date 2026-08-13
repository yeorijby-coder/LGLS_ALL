using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Timer
{
    public interface TimerListener
    {
        void ProcessTimeout(Object returnData);
    }
}
