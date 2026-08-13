using System;
using System.Collections.Generic;
using System.Text;



namespace ECP.Util.Cache
{
    public class LRUCache : SmartCacheManger
    {
        const string CLASS = "LRUCache";

        public class CacheUnit
        {
            public string theId = null;
            public object theObject = null;
            public DateTime theLastUsageTime;

    
            public CacheUnit(string theId, object theObject)
            {
                this.theId = theId;
                this.theObject = theObject;
                this.theLastUsageTime = new DateTime(DateTime.Now.Ticks);
            }
            public string toString()
            {
                return ("ID:" + this.theId + ", " +
                        "LastUsageTime:" + this.theLastUsageTime.ToString());
            }
        }


        int theCapacity = 0;

        public int Capacity
        {
            get { return theCapacity; }
            set 
            {
                lock(SynchronizationObject)
                {
                    theCapacity = value; 

                    if(theCache.Count > this.theCapacity)
                    {
                        Decache();
                    }
                } 
            }
        }

        int theDecacheSize = 0;

        public int DecacheSize
        {
            get { return theDecacheSize; }
            set { theDecacheSize = value; }
        }

        int totalRequests = 0;

        public int TotalRequests
        {
            get { return totalRequests; }
            set { totalRequests = value; }
        }

        int totalHits = 0;

        public int TotalHits
        {
            get { return totalHits; }
            set { totalHits = value; }
        }

        CacheObjectManager theObjectManager = null;

        Dictionary<string, CacheUnit> theCache = null;


        private  object synchronizationObject = new object();

        public  object SynchronizationObject
        {
            get { return this.synchronizationObject; }
            set { this.synchronizationObject = value; }
        }


        private string  name = "";


        public LRUCache(string name) 
            : this(name, 100, 10, null, null)
        {
        }

        public LRUCache(string name, int theCapacity, int theDecacheSize)
            : this(name, theCapacity, theDecacheSize, null, null)
        {
        }

        public LRUCache(string name, int theCapacity, int theDecacheSize,
                    CacheObjectManager theObjectManager)
            : this(name, theCapacity, theDecacheSize, theObjectManager, null)
        {
        }

        public LRUCache(
            string name,
            int theCapacity,
            int theDecacheSize,
            CacheObjectManager theObjectManager,
            object synchronizationObject)
        {
            const string METHOD = CLASS + ".LRUCache";

            Console.WriteLine(METHOD+":"+"LRUCache(" + theCapacity + ", " + 
                theDecacheSize + ") " + "created.");

            this.theObjectManager = theObjectManager;

            if (synchronizationObject != null)
            {
                this.synchronizationObject = synchronizationObject;
            }
            this.name = name;
    
            Reset(theCapacity, theDecacheSize);
        }

        public void Reset()
        {
            Reset(GetCapacity(), GetDecacheSize());
        }


        public void Reset(int theCapacity, int theDecacheSize)
        {
            const string METHOD = CLASS + ".reset";

            lock (SynchronizationObject)
            {
                if (theCache != null)
                {
                    this.theDecacheSize = theCache.Count;
                    Decache();
                }
                this.theCapacity = Math.Max(1, theCapacity);
                this.theDecacheSize = Math.Max(1, Math.Min(theCapacity, theDecacheSize));
                theCache = new Dictionary<string, CacheUnit>();
            }

            Console.WriteLine(METHOD+":"+"LRUCache(" + theCapacity + ", " + theDecacheSize + ").");

        }

        new public string ToString()
        {
            return ("Cache Name: " + name + ", " +
                    "Cache Capacity:" + theCapacity + ", " +
                    "Cache DecacheSize:" + theDecacheSize + ", " +
                    "Cache:" + theCache.ToString());
        }

        public void Put(string theId, object theObject)
        {
            const string METHOD = CLASS + ".Put";

            lock (SynchronizationObject)
            {
                if (theCache.Count >= this.theCapacity)
                {
                    Decache();
                }
                theCache.Add(theId, new CacheUnit(theId, theObject));
            }

            Console.WriteLine(METHOD+":"+"Object(" + theId + ") added to LRUCache.");
        }
        

        public object Remove(String theId) 
        {
            const string METHOD = CLASS + ".remove";

            object removedObject = null;

            lock (SynchronizationObject)
            {
                if(theCache.ContainsKey(theId))
                {
                    CacheUnit theCacheUnit = theCache[theId];
                    theCache.Remove(theId);

                    removedObject = theCacheUnit.theObject;

                    if (theObjectManager != null)
                    {
                        theObjectManager.UnloadObject(theCacheUnit.theId,
                                                      theCacheUnit.theObject);
                    }
                }                
            }
            if (removedObject != null)
            {
                Console.WriteLine(METHOD+":"+"Object(" + theId + ") removed from LRUCache.");
            }
    	    return(removedObject);
        }

        public object Get(String theId)
        {
            lock (SynchronizationObject)
            {
                CacheUnit theCacheUnit = GetCacheUnit(theId);
                return(theCacheUnit == null ? null : theCacheUnit.theObject);
            }
        }

        public CacheUnit GetCacheUnit(String theId)
        {
            lock (SynchronizationObject)
            {
                CacheUnit theCacheUnit = null; ;

                totalRequests =
                    (totalRequests >= Int32.MaxValue
                        ?   (totalHits=0)
                        :   totalRequests+1);

                if (theCache.ContainsKey(theId))
                {
                    theCacheUnit = (CacheUnit)theCache[theId];

                    totalHits++;
                    theCacheUnit.theLastUsageTime = new DateTime(DateTime.Now.Ticks);
                    
                }
                else
                {
                }
                return theCacheUnit;
            }
        }


        public CacheInfo GetCacheInfo()
        {
            return new CacheInfo(this.totalHits, this.totalRequests, theCache.Count);
        }

        public void ResetCacheInfo()
        {
            totalHits = 0;
            totalRequests = 0;
        }

        public int Decache()
        {
            const string METHOD = CLASS + ".Decache";

            Console.WriteLine(METHOD+":"+name+"LRUCache decache Started");

            int decachedObjectCount = 0;

            lock ( SynchronizationObject)
            {

                List<CacheUnit> theDecacheList = GetDecacheList();

                decachedObjectCount = theDecacheList.Count;

                for (int i = 0; i < theDecacheList.Count; i++)
                {
                     Remove(((CacheUnit)theDecacheList[i]).theId);
                }
            }
            Console.WriteLine(METHOD+":"+name+"LRUCache decached." +
                    "Decached Objects(" + decachedObjectCount + ").");
            return (decachedObjectCount);            
        }


        List<CacheUnit> GetDecacheList()
        {
            const string METHOD = CLASS + ".GetDecacheList";

               	    
            // decache list will maintain order, old one head side, new one is on tail side
            List<CacheUnit> theDecacheList = new List<CacheUnit>();

            lock (SynchronizationObject)
            {
                foreach(CacheUnit theCacheUnit in theCache.Values)
                {
                    // if decache size is grater than decash size, put all the thing to decache list
                    // this will not happen normal situation
                    if (theDecacheList.Count ==0 || theCache.Count <= theDecacheSize)
                    {
                        theDecacheList.Add(theCacheUnit);
                        continue;
                    }

                    int first = 0;
                    int last = Math.Min(theDecacheList.Count-1, theDecacheSize-1);
                    
                    
                    if (theCacheUnit.theLastUsageTime.CompareTo(
                        ((CacheUnit)theDecacheList[last]).theLastUsageTime) > 0  // after
                        || theCacheUnit.theLastUsageTime.CompareTo(
                        ((CacheUnit)theDecacheList[last]).theLastUsageTime) ==0 )
                    {
                        if (last < (theDecacheSize-1))
                        {                          
                            // last one means new, so that last one on the decahelist is older than 
                            // new Unit, new unit will add as last one
                            theDecacheList.Insert(last + 1, theCacheUnit);
                        }
                        else
                        {
                            continue;  
                        }
                    }
                    else if (theCacheUnit.theLastUsageTime.CompareTo(
                            ((CacheUnit)theDecacheList[first]).theLastUsageTime) < 0  // before
                            || theCacheUnit.theLastUsageTime.CompareTo(
                            ((CacheUnit)theDecacheList[first]).theLastUsageTime) ==0 )
                    {
                        // new unit will be added head side on decache list
                        theDecacheList.Insert(first, theCacheUnit);
                    }
                    else
                    {
                        // let find approprite index on decache list
                        // we will used devide middle and conquer

                        while ((last - first) > 1)
                        {
                            int middle = (first + last)/2;
                            if (theCacheUnit.theLastUsageTime.CompareTo(
                                ((CacheUnit)theDecacheList[middle]).theLastUsageTime) > 0  // after
                                || theCacheUnit.theLastUsageTime.CompareTo(
                                ((CacheUnit)theDecacheList[middle]).theLastUsageTime) ==0)
                            {
                                // new unit is newer than middle one, abandom from firt to middle
                                // lets look into from middle to end
                                // so that middle should be first
                                first = middle;
                            }
                            else if (theCacheUnit.theLastUsageTime.CompareTo(
                                     ((CacheUnit)theDecacheList[middle]).theLastUsageTime) <0  // before
                                     || theCacheUnit.theLastUsageTime.CompareTo(
                                     ((CacheUnit)theDecacheList[middle]).theLastUsageTime) ==0)
                            {
                                last = middle;
                            }
                            else
                            {
                                Console.WriteLine(METHOD+":"+"SHOULD NEVER EXECUTE THIS STATEMENT. " +
                                      "First(" + first + ") " +
                                      "Middle(" + middle + ") " +
                                      "Last(" + last + ").");
                                break;
                            }
                        }
                        theDecacheList.Insert(first + 1, theCacheUnit);
                    }
                    while (theDecacheList.Count > theDecacheSize)
                    {
                        theDecacheList.RemoveAt(theDecacheList.Count - 1);
                    }
                }
            }
            return theDecacheList;
        }


        public int GetCapacity()
        {
            return this.theCapacity;
        }

        public int GetDecacheSize()
        {
            return this.DecacheSize;
        }


        public void RecordUsage(string theId)
        {
            GetCacheUnit(theId);
        }       

    }
}
