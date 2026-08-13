using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util.Cache
{
    abstract public class AbstractObjectManager : CacheObjectManager
    {
        const string CLASS = "AbstractObjectManger";

        private string managerId = "";

        int cacheSize = 3000;

        int deCacheSize = 200;

        private LRUCache theCache;

        public AbstractObjectManager()
        {
        }

        public string GetId()
        {
            return this.managerId;
        }
        public object LoadObject(string theId)
        {
            return null;
        }

        public void UnloadObject(string theId, object theObject)
        {
            theId = null;
            theObject = null;
        }

        public CacheInfo GetCacheInfo()
        {
            return GetCache().GetCacheInfo();
        }

        public void SetObjectCacheSize(CacheSize cacheSize)
        {

            if (this.cacheSize == cacheSize.cache &&
                this.deCacheSize == cacheSize.decache) 
            {
                return;
            }

            if (cacheSize.cache > 0 && cacheSize.decache > 0)
            {
                this.cacheSize = cacheSize.cache;
                this.deCacheSize = cacheSize.decache;
            }
            else
            {
                throw new Exception(
                                   "ObjectCache value(" + cacheSize.cache + ") & " +
                                   "ObjectDecache value(" + cacheSize.decache + ") " +
                                   "must be greater than 0");
            }

            GetCache().DecacheSize = cacheSize.decache;
            GetCache().Capacity = cacheSize.cache;
       }

        public void ResetCacheInfo()
        {
            GetCache().ResetCacheInfo();
        }

        protected object GetSynchronizationObject()
        {
            return (GetCache().SynchronizationObject);
        }

        public bool HasCaching()
        {
            return true;
        }

        private LRUCache GetCache()
        {
            if (this.theCache == null)
            {
                lock (typeof(LRUCache))
                {
                    // If the cache has not been created.
                    if (this.theCache == null)
                    {
                        // Create the cache.
                        CacheSize cacheSize = new CacheSize(this.cacheSize,
                                                            this.deCacheSize);
                        this.theCache =
                            CreateCache(
                                GetId(), cacheSize, this, null);
                    }
                }
            }

            return (this.theCache);
        }

        protected LRUCache CreateCache(string cacheId,
                               CacheSize cacheSize,
                               CacheObjectManager objectManager,
                               Object synchronizationObject)
        {
            return new LRUCache(cacheId, cacheSize.cache, cacheSize.decache,
                                objectManager, synchronizationObject);
        }

        protected void CacheObject(string id, object obj)
        {
            GetCache().Put(id, obj);
        }

        protected object GetObject(string id)
        {
            return GetCache().Get(id);
        }

        protected object RemoveObject(string id)
        {
            return GetCache().Remove(id);
        }

        protected void ClearCache()
        {
            GetCache().Reset();
        }
    }
}
