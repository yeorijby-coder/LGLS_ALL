using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Globalization;
using System.Configuration;
using System.Collections.Specialized;
using ECP.Global;

namespace ECP.Util
{
    
    public class LocalLog
    {
        //private static readonly string CLASS = "LocalLog";
        private static string DATE_FORMAT_LOGDATA = "yyyy-MMM-dd HH:mm:ss:fff";
        private static string DATE_FORMAT_FILE = "yyyy-MMM-dd-HHmmssfff";

        /// <summary>
        /// Indicates if messages should be logged to stdout rather than to the logger 
        /// </summary>
        private bool localOutput = false;

        private static LocalLogInfo localLogInfo = new LocalLogInfo(true, 5, 10000);

        private FileStream fileStream = null;

        private StreamWriter streamWriter = null;

        public static LocalLog instance = new LocalLog();

        private string program = string.Empty;

        private string lastCreatedFileName = string.Empty;
        
        private long currentFileSize = 0;

        private long maxFileSize = 0;

        private string logDirName = string.Empty;

        private string suffix = ".log";

        private int suffixCount = 0;

        private LocalLog()
        { 
        }

       
        public void SetLocalLogInfo(LocalLogInfo localLogInfo)
        {   
            this.maxFileSize = Math.Max(1, localLogInfo.entries);
        }

        public LocalLogInfo GetLocalLogInfo()
        {
            return localLogInfo;
        }

        public static LocalLog GetInstance()
        {
            return instance;
        }

        public void Init(string prog)
        {
            if (streamWriter != null) return;
            this.program = prog;
            CreateNewFile(program);
        }

        public void SetLocalOutput(bool value)
        {
            localOutput = value;
        }
        public void EnableLog(bool value)
        {
            localLogInfo.enabled = value;
        }

        public void Log(string data)
        {
            Log(DateTime.Now, data);
        }
        
        public void Log(DateTime date, string data)
        {
            if (!localLogInfo.enabled) return;

            string logString = "(" +
                date.ToString(DATE_FORMAT_LOGDATA, DateTimeFormatInfo.InvariantInfo) + ") " + data;

            if (localOutput)
                Console.WriteLine(logString);
            else if (streamWriter != null)
            {
                if (this.currentFileSize > maxFileSize-1)
                {
                    CreateNewFile(this.program);
                }
                streamWriter.WriteLine(logString);
                streamWriter.Flush();
                this.currentFileSize++;
            }
        }

        

        private void CreateNewFile(string prog)
        {
            if (fileStream != null)
            {
                fileStream.Close();
            }

            if (logDirName.Equals(string.Empty))
            {
                NameValueCollection appSettings =
                     ConfigurationManager.AppSettings;
                this.logDirName = appSettings[ECPProperties.ECP_BASE].ToString() + @"\" + "logs";
            }            

            //check the directory exist if not create

            if (!Directory.Exists(logDirName))
            {
                Directory.CreateDirectory(logDirName);
            }

            
            try
            {
                string newlyCreatedFileName = logDirName + @"\" + prog + "-" +
                    DateTime.Now.ToString(DATE_FORMAT_FILE, DateTimeFormatInfo.InvariantInfo);

                suffixCount = (lastCreatedFileName.Equals(newlyCreatedFileName)) ? ++suffixCount : 0;

                string suffixCountString = "-"+ Convert.ToString(suffixCount)+suffix;

                fileStream = new FileStream(newlyCreatedFileName+suffixCountString, FileMode.Append, FileAccess.Write);
                streamWriter = new StreamWriter(fileStream);
                currentFileSize = 0;
                lastCreatedFileName = newlyCreatedFileName;
            }
            catch (IOException ex)
            {
                throw ex;
            }

        }
    }

    public class LocalLogInfo 
    {
        public bool enabled = false;
        public int files = 0;
        public int entries = 0;

        public LocalLogInfo(bool enabled, int files, int entries)
        {
            this.enabled = enabled;
            this.files = files;
            this.entries = entries;

        }
        public LocalLogInfo(bool enabled, int entries)
        {
            this.enabled = enabled;
            this.entries = entries;
        }
    }
}
