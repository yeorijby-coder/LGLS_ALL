using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Runtime.Remoting;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class Name
    {
        public static String GetPackagePrefix(String class_name)
        {
            String package_prefix = "";
            int lastdot = class_name.LastIndexOf('.');
            if (lastdot > 0)
            {
                package_prefix = class_name.Substring(0, lastdot + 1);
            }
            return package_prefix;
        }

        public static String GetBaseName(String class_name)
        {
            int lastdot = class_name.LastIndexOf('.');
            return class_name.Substring(lastdot + 1);
        }

        
        public static Object NewInstance(String class_name)
        {   
            Object _object = null;
            try
            {
                ObjectHandle objHandle = Activator.CreateInstance("ECP", class_name);
                _object = objHandle.Unwrap();
            }
            catch (Exception  e)
            {
                _object = null;
            }
            
            return _object;
        }

        
        public static String[] GetArray(String string_with_spaces)
        {
            if (string_with_spaces == null)
            {
                return new String[0];
            }

            List<string> v = new List<string>();
            
            StringTokenizer st = new StringTokenizer(string_with_spaces);


            while (st.HasMoreTokens)
            {
                v.Add(st.NextToken);
            }

            String[] string_array = new String[v.Count];
            
            for (int i = 0; i < v.Count; i++)
            {
                string_array[i] = (String)v[i];
            }
            return string_array;
        }
    }
}
