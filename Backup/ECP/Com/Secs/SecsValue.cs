using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.Secs
{
    public class SecsValue
    {
        private Dictionary<string, string> itemValue = new Dictionary<string, string>();
        private Dictionary<string, string> requestValue = new Dictionary<string, string>();

        private object secsData = null;

        public object SecsData
        {
            get { return secsData; }
            set { secsData = value; }
        }


        public SecsValue(object secsData)
        {
            this.secsData = secsData;
        }
   
        public void AddValue(string key, string value)
        {
            //
            itemValue.Add (key, value);
        }
        public void AddRequestValue(string key)
        {
            //
            requestValue.Add(key, "");
        }
        public void SetRequestValue(string key, string value)
        {
            //
            if (requestValue.ContainsKey(key))
            {
                requestValue[key] = value;
            }
            else
            {
                requestValue.Add(key, value);
            }
        }
        public string  GetValue(string key)
        {
            string returnValue;
            itemValue.TryGetValue(key, out returnValue);
            return returnValue;
        }

        public string GetRequestValue(string key)
        {
            string returnValue;
            requestValue.TryGetValue(key, out returnValue);
            return returnValue;
        }
    
        public void Clear()
        {
            itemValue.Clear();
        
        }

        public void Delete(string key)
        {
            itemValue.Remove(key);
        }

        public bool IsExistRequestValue(string key)
        {
            return requestValue.ContainsKey(key);
        }
        public bool IsExistItem(string key)
        {
            return itemValue.ContainsKey(key);
        }

        public string[] GetKeys()
        {
            return null;
        }

    }
}
