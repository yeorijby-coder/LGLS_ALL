using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    public interface CacheObjectManager
    {

        object LoadObject(string theId);

        void UnloadObject(string theId, object theObject);

    }
}
