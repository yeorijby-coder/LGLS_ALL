using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    [Serializable]
    public class CacheSize
    {
        public int cache = 0;

        public int decache = 0;

        public CacheSize()
        {
        } 

        public CacheSize(int cache, int decache)
        {
            this.cache = cache;
            this.decache = decache;
        } 
    }
}
