using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    [Serializable]
    public class CacheInfo
    {
        public int hits = 0;

        public int requests = 0;

        public int size = 0;


        public CacheInfo()
        {
        }

        public CacheInfo(int hits, int requests, int size)
        {
            this.hits = hits;
            this.requests = requests;
            this.size = size;

        }
    }
}
