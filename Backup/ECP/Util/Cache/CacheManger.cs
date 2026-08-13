using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    public interface CacheManger
    {

        void Put(string theId, object theObject);


        object Remove(String theId);


        object Get(String theId);
    }
}
