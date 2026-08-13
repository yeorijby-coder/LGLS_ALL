using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections;


namespace HECS.Util
{
    public static class SimpleCache
    {

        private static  Dictionary<string, Dictionary<string, Object>> objectList =            new Dictionary<string, Dictionary<string, object>>();


        private static void CreateNode(string nodeID)
        {
            Dictionary<string, Object> subObject = null;
            if (nodeID == null)
            {
                throw new NullReferenceException("DataStorage.CreateNode(string nodeID) : nodeID is NULL.");
            }
            try
            {
                subObject = new Dictionary<string, Object>();
                objectList.Add(nodeID, subObject);
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("CreateNode:" + ex.StackTrace);
                throw new Exception("DataStorage.CreateNode(string nodeID) : Duplicate NodeID, NodeID = " + nodeID);
            }
        }

        public static bool IsNodeExist(string nodeID)
        {
            return objectList.ContainsKey(nodeID);
        }

        public static void RemoveNode(string nodeID)
        {
            objectList.Remove(nodeID);
        }

        public static int GetNodeCount()
        {
            return objectList.Count;
        }

        public static int GetObjectCount(string nodeID)
        {
            Dictionary<string, Object> subList = null;

            subList = objectList[nodeID];

            if (subList == null)
            {
                return 0;
            }
            return subList.Count;
        }

        public static Object GetObject(string nodeID, string objectID)
        {
            if(objectList.ContainsKey(nodeID))
            {
                if(objectList[nodeID].ContainsKey(objectID))
                {
                    return objectList[nodeID][objectID];
                }
            }
            Dictionary<string, Object> subList = null;
            try
            {
                subList = objectList[nodeID];
                if (subList == null)
                {
                    return null;
                }
                if (subList.Count <= 0)
                {
                    return null;
                }
                foreach (KeyValuePair<string, Object> outObject in subList)
                {
                    if (outObject.Key.Equals(objectID))
                    {
                        return outObject.Value;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("GetObject:" + ex.StackTrace);
                return null;
            }
            return null;
        }

        public static String[] GetObjectNodeKey(string nodeID)
        {
            Dictionary<string, Object> subList = null;
            subList = objectList[nodeID];

            if (subList == null)
            {
                return null;
            }

            string[] keyValue = new string[subList.Count];
            subList.Keys.CopyTo(keyValue, 0);
            return keyValue;
        }

        public static Object[] GetObjectAll(string nodeID)
        {
            Dictionary<string, Object> subList = null;
            Object[] outObject = null;
            try
            {
                subList = objectList[nodeID];
                if (subList == null)
                {
                    return null;
                }
                if (subList.Count <= 0)
                {
                    return null;
                }

                int i = 0;
                outObject = new Object[subList.Count];
                foreach (KeyValuePair<string, Object> outValue in subList)
                {
                    outObject[i] = outValue.Value;
                    i++;
                }
                return outObject;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("GetObjectAll:" + ex.StackTrace);
                CreateNode(nodeID);
                return outObject;
            }
        }

        public static Object[] GetObjectAllByValue(string nodeID)
        {
            Dictionary<string, Object> subList = null;
            Object[] valueList = null;
            try
            {
                subList = objectList[nodeID];
                if (subList == null)
                {
                    return null;
                }
                if (subList.Count <= 0)
                {
                    return null;
                }
                valueList = new object[subList.Count];
                subList.Values.CopyTo(valueList, 0);
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("GetObjectAllByValue:" + ex.StackTrace);
                return null;
            }
            return valueList;
        }


        public static Object[] GetObject(string nodeID, string objectID, string query)
        {
            // CarrierID='AAA' && CarrierType=0  <-구문분석기를 사용해야 하나.. 일단은 ...
            return null;
        }

        public static void RemoveObject(string nodeID, string objectID)
        {
            Dictionary<string, Object> subList = null;
            try
            {
                subList = objectList[nodeID];
                if (subList == null)
                {
                    return;
                }
                if (subList.Count <= 0)
                {
                    return;
                }
                foreach (KeyValuePair<string, Object> outObject in subList)
                {
                    if (outObject.Key.Equals(objectID))
                    {
                        subList.Remove(objectID);
                        return;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("RemoveObject:" + ex.StackTrace);
                return;
            }
        }

        public static bool IsObjectExist(string nodeID, string objectID)
        {
            Dictionary<string, Object> subList = null;
            try
            {
                subList = objectList[nodeID];
                if (subList == null)
                {
                    return false;
                }
                if (subList.Count <= 0)
                {
                    return false;
                }
                foreach (KeyValuePair<string, Object> outObject in subList)
                {
                    if (outObject.Key.Equals(objectID))
                    {
                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("IsObjectExist:" + ex.StackTrace);
                return false;
            }
            return false;
        }



        public static void RecoverStorage()
        {

        }

        public static void UpdateObject(string nodeID, string objectID, Object objects)
        {

        }

        public static void AddObject(string nodeID, string objectID, Object objectValue)
        {
            Dictionary<string, Object> subObject = null;
            if (objectValue as System.Windows.Forms.Form != null)
            {
                ((System.Windows.Forms.Form)objectValue).Visible = false;
                ((System.Windows.Forms.Form)objectValue).WindowState = System.Windows.Forms.FormWindowState.Minimized;

                ((System.Windows.Forms.Form)objectValue).Show();
                ((System.Windows.Forms.Form)objectValue).Hide();
            }
            if (nodeID == null)
            {
                throw new NullReferenceException("DataStorage.AddObject(string nodeID, string objectID, Object object) : nodeID is NULL.");
            }
            if (!objectList.ContainsKey(nodeID))
            {
                subObject = new Dictionary<string, Object>();
                objectList.Add(nodeID, subObject);
                subObject.Add(objectID, objectValue);
            }
            else
            {
                subObject = objectList[nodeID];
                try
                {
                    subObject.Add(objectID, objectValue);
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("AddObject:" + ex.StackTrace);
                    //System.Console.WriteLine("Object 중복 되었습니다." + nodeID + "/" + objectID);
                    //throw new Exception("DataStorage.AddObject(string nodeID, string objectID, Object object) : Duplicate ObjectID, ObjectID = " + objectID);
                }
            }
        }
        public static void AddObject(string nodeID, Object objectValue)
        {
            Dictionary<string, Object> subObject = null;
            string subObjectKey = "";
            if (nodeID == null)
            {
                throw new NullReferenceException("DataStorage.AddObject(string nodeID, Object object) : nodeID is NULL.");
            }

            if (!objectList.ContainsKey(nodeID))
            {
                subObject = new Dictionary<string, Object>();
                objectList.Add(nodeID, subObject);

                Type type = objectValue.GetType();
                PropertyInfo prop = type.GetProperty("ObjectID");

                if (prop == null)
                {
                    throw new Exception("DataStorage.AddObject(string nodeID, Object object) : The 'object' has not a ObjectID ");
                }

                subObjectKey = (string)prop.GetValue(objectValue, null);
                subObject.Add(subObjectKey, objectValue);
            }
            else
            {
                subObject = objectList[nodeID];

                Type type = objectValue.GetType();
                PropertyInfo prop = type.GetProperty("ObjectID");
                if (prop == null)
                {
                    throw new Exception("DataStorage.AddObject(string nodeID, Object object) : The 'object' has not a ObjectID ");
                }

                subObjectKey = (string)prop.GetValue(objectValue, null);
                try
                {
                    subObject.Add(subObjectKey, objectValue);   // Key 중복나면 Exception 처리 해 버린다.
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("AddObject:" + ex.StackTrace);
                    throw new Exception("DataStorage.AddObject(string nodeID, Object object) : Duplicate ObjectID, ObjectID = " + subObjectKey);
                }
            }
        }
    }
}
