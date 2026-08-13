using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Test
{
    public interface Logger
    {
         void Debug(int machineId, string id, string message);
    }
}
