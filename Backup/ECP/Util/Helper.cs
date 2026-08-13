using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.Remoting;
using ECP.Global;

namespace ECP.Util
{
    public class Helper
    {

        /// <summary>
        /// 주어진 파라미터가 null일경우, Empty string을 Return
        /// </summary>
        /// <param name="inString">체크하고자하는 String</param>
        /// <returns>원본 String 또는 Null일 경우 Empty String</returns>
        public static string PreventNull(string inString)
        {
            if (inString == null)
            {
                return string.Empty;
            }
            else
            {
                return inString;
            }
        }


        public static byte[] PreventNull(byte[] inByte)
        {
            if (inByte == null)
            {
                return new byte[0];
            }
            else
            {
                return inByte;
            }
        }

        /// <summary>
        /// 주어진 String이 Null 또는 Empty인지를 체크함
        /// </summary>
        /// <param name="inString">체크하고자 하는 String</param>
        /// <returns>String이 Null또는 Empty이면 True, 아닐경우 False</returns>
        public static bool IsNullOrEmpty(string inString)
        {
            return (inString == null || inString.Equals(""));
        }

        /// <summary>
        /// 주어진 String을 주어진 Max길이의 끝에 CRLF를 추가함
        /// </summary>
        /// <param name="str">CRLF를 추가 하는 String</param>
        /// <param name="max">CRLF를 추가 하는 String의 Indexed + Offset</param>
        /// <returns>포함된 CRLF를 가진 새로운 String</returns>
        public static String BreakDownString(string str, int max)
        {
            int index = max;
            int offset = 0;
            string temp = "";

            if ((str.Length > 0) && (max > 0) && (max < str.Length))
            {
                while ((index + offset < str.Length) && (index + offset > 0))
                {
                    offset = offset - 1;
                    if (str[index + offset] == ' ')
                    {
                        temp = temp + str.Substring(0, index + offset) + "\n";
                        str = str.Substring(index + offset).TrimEnd();
                        index = max;
                        offset = 0;

                    }
                    else if (index + offset <= 0)
                    {
                        index = str.IndexOf(' ') + 1;
                        offset = 0;
                    }
                }
            }
            return temp + str;
        }


        /// <summary>
        /// NameSpace String을 받아서, String의 List로 변환
        /// </summary>
        /// <param name="str">Parsing하고자 하는 String</param>
        /// <returns>NameSpace String List</returns>
        public static string[] ParseNameSpace(string str)
        {
            int i = 0;

            string[] stringList;
            StringTokenizer tokens = new StringTokenizer(str, ".");

            stringList = new String[tokens.CountTokens];

            while (tokens.HasMoreTokens)
            {
                String token = tokens.NextToken;
                stringList[i++] = token;
            }

            return stringList;
        }

        /// <summary>
        /// 주어진 String를 콤마로 분리한 String을 Return
        /// </summary>
        /// <param name="str">Parse하고자 하는 String</param>
        /// <returns>주어진 String를 콤마로 분리한 String</returns>
        public static string[] ParseCommaSeperatedString(string str)
        {
            int i = 0;
            string[] stringList;
            StringTokenizer tokens = new StringTokenizer(str, ",");

            stringList = new String[tokens.CountTokens];
            while (tokens.HasMoreTokens)
            {
                string token = tokens.NextToken;
                stringList[i++] = token;
            }

            return stringList;
        }


        /// <summary>
        /// string s내 string replace가 발생하고는 곳 마다, string with로 대체한다.
        /// </summary>
        /// <param name="s">수정하고자 하는 string</param>
        /// <param name="replace">replace되어질 string</param>
        /// <param name="with">replace할 string</param>
        /// <returns></returns>
        public static string replaceString(string s, string replace, string with)
        {
            int i = s.IndexOf(replace);

            if (i < 0)
            {
                // don't bother doing anything else
                return s;
            }

            StringBuilder sb = new StringBuilder(s);

            sb.Replace(replace, with);

            return sb.ToString();
        }



        public static bool IsDigit(string str)
        {
            foreach (char c in str)
            {
                if (!char.IsDigit(c))
                {
                    return false;
                }
            }
            return true;
        }

        public static bool IsLetterOrDigit(string str)
        {
            foreach (char c in str)
            {
                if (!char.IsLetterOrDigit(c))
                {
                    return false;
                }
            }
            return true;
        }

        /// <summary>
        /// 주어진 String을 Byte Array List로 Format변환후 Return
        /// </summary>
        /// <param name="s">변환하고자 하는 string</param>
        /// <returns>주어진 String을 변환한 Byte Array List</returns>
        public static byte[] String2Bytes(string s)
        {
            return new System.Text.ASCIIEncoding().GetBytes(s);
        }

        /// <summary>
        /// 주어진  Byte Array List로 Format변환후 String Return
        /// </summary>
        /// <param name="bytes">변환하고자 하는 byte list</param>
        /// <returns>변환된 string</returns>
        public static string Bytest2String(byte[] bytes)
        {
            return new System.Text.ASCIIEncoding().GetString(bytes);
        }


        public static string GetCallStack()
        {
            string className = "";
            string fileName = "";
            string lineNumber = "";
            string methodName = "";
            string fullInfo = "";
            StringBuilder stackBuffer = new StringBuilder(2048);
            try
            {
                StackTrace st = new StackTrace(true);
                int frameIndex = 0;
                while (frameIndex < st.FrameCount)
                {
                    StackFrame frame = st.GetFrame(frameIndex);
                    //if(frame != null && frame.GetMethod().DeclaringType == callerStackBoundaryDeclaringType)
                    if (frame != null && !(frame.GetMethod().Module.Name.Contains("Stocker") || frame.GetMethod().Module.Name.Contains("ECP")))
                    {
                        break;
                    }
                    frameIndex++;
                }


                if (frameIndex < st.FrameCount)
                {
                    for (int i = 1; i < frameIndex; i++)
                    {
                        StackFrame locationFrame = st.GetFrame(i);
                        if (locationFrame != null)
                        {
                            System.Reflection.MethodBase method = locationFrame.GetMethod();
                            methodName = method.Name;
                            if (method.DeclaringType != null)
                            {
                                className = method.DeclaringType.FullName;
                            }
                            fileName = locationFrame.GetFileName();
                            lineNumber = locationFrame.GetFileLineNumber().ToString(System.Globalization.NumberFormatInfo.InvariantInfo);

                            // Combine all location info
                            fullInfo = className + '.' + methodName + '(' + fileName + ':' + lineNumber + ')';

                        }
                        stackBuffer.Append(fullInfo + "\r\n");
                    }

                }
            }
            catch (System.Security.SecurityException)
            {
                return stackBuffer.ToString();
            }
            return stackBuffer.ToString();

        }
        //private TransferData CopyObject(object sourceObject)
        //{
        //    ////Type objectType = transferDataValue.GetType(); 
        //    //ObjectHandle objHandle = Activator.CreateInstance(sourceObject.GetType().Assembly.FullName, sourceObject.GetType().Name);
        //    //object targetObject = objHandle.Unwrap();

        //    //System.Reflection.PropertyInfo [] properties = sourceObject.GetType().GetProperties();
        //    //foreach (System.Reflection.PropertyInfo prop in properties)
        //    //{
        //    //    object value 
        //    //    prop.SetValue(targetObject,s
        //    //    //propertyInfo
        //    //    //targetObject.GetType().GetProperty(prop.Name).SetValue (
                
        //    //}
        //}
    }
}
