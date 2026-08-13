using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    public interface UsageMonitor
    {

        void RecordUsage(string theId);
    }
}
