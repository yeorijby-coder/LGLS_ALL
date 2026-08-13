using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    interface SmartCacheManger : CacheManger, UsageMonitor
    {

        void Reset(int theCapacity, int theDecacheSize);

        void Reset();

        int GetCapacity();

        int GetDecacheSize();
    }
}
