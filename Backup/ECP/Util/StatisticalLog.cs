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

    public class StatisticalLog
    {
        //private static readonly string CLASS = "StatisticalLog";
        private static string DATE_FORMAT_LOGDATA = "yyyy-MM-dd HH:mm:ss:fff";
        private static string DATE_FORMAT_FILE = "yyyy-MM-dd";

        /// <summary>
        /// Indicates if messages should be logged to stdout rather than to the logger 
        /// </summary>
        private bool localOutput = false;

        private bool isRolloverBydays = false;

        private int rollOverdays = 30;

        private StatisticalLogInfo statisticalLogInfo = new StatisticalLogInfo(true, 30, 100, 10000);

        private FileStream fileStream = null;

        private StreamWriter streamWriter = null;

        public StatisticalLog instance = null;

        private string program = string.Empty;

        private string workingDay;

        private string lastCreatedFileName = string.Empty;

        private long currentFileSize = 0;

        private long maxFileSize = 0;

        private int maxFileCount = 0;

        private string logDirName = string.Empty;

        private string suffix = ".log";

        private int suffixCount = 0;

        private bool initialized = false;

        private StatisticalFileModificationComparer fileModificationComparer =
            new StatisticalFileModificationComparer();


        public StatisticalLog()
        {
        }


        public void SetStatisticalLogInfo(StatisticalLogInfo localLogInfo)
        {
            this.maxFileSize = Math.Max(1, localLogInfo.entries);
            this.maxFileCount = Math.Max(1, localLogInfo.files);
            this.isRolloverBydays = localLogInfo.isRolloverByDays;
            this.rollOverdays = localLogInfo.days;
        }

        public StatisticalLogInfo GetStatisticalLogInfo()
        {
            return statisticalLogInfo;
        }

        public void Init(string prog)
        {
            if (streamWriter != null) return;
            this.program = prog;
        }

        public void SetLocalOutput(bool value)
        {
            localOutput = value;
        }
        public void EnableLog(bool value)
        {
            statisticalLogInfo.enabled = value;
        }

        public void Log(string collectorId, string data)
        {
            Log(DateTime.Now, collectorId, data);
        }

        private bool isWorkingDayChanged()
        {
            string now = DateTime.Now.ToString("yyyy-MM-dd");
            if( !now.Equals( workingDay ) )
            {
                return true;
            }
            return false;
        }

        public void Log(DateTime date, string collectorId, string data)
        {
            try
            {
                if (!statisticalLogInfo.enabled) return;

                string logString =
                    date.ToString(DATE_FORMAT_LOGDATA, DateTimeFormatInfo.InvariantInfo) + "/" + data;

                if (workingDay == null) { workingDay = DateTime.Now.ToString("yyyy-MM-dd"); }

                if (localOutput)
                {
                    Console.WriteLine(logString);
                }
                else if (streamWriter != null)
                {
                    if (this.isWorkingDayChanged())
                    {
                        CreateNewFile(collectorId);
                    }

                    if (this.currentFileSize > maxFileSize - 1)
                    {
                        CreateNewFile(collectorId);
                    }

                    //if (logString.Length > 0)
                    //{
                    //Encoding unicode = Encoding.UTF7;
                    //byte[] unicodeBytes = unicode.GetBytes(logString);
                    //streamWriter.WriteLine(unicodeBytes);
                    //}

                    streamWriter.WriteLine(logString);
                    streamWriter.Flush();
                    this.currentFileSize++;
                }
                else
                {
                    CreateNewFile(collectorId);
                    Log(date, collectorId, data);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
             }
        }


        private void CreateNewFile(string collectorId)
        {

            if (fileStream != null)
            {
                fileStream.Close();
            }

            if (logDirName.Equals(string.Empty))
            {
                NameValueCollection appSettings =
                     ConfigurationManager.AppSettings;
                this.logDirName = appSettings[ECPProperties.ECP_LOG].ToString() +
                @"\" + "Log" + @"\" + "Statistics";
            }

            //check the directory exist if not create
            if (!Directory.Exists(logDirName))
            {
                Directory.CreateDirectory(logDirName);
            }

            string collectorDir = logDirName + @"\" + collectorId;

            if (!Directory.Exists(collectorDir))
            {
                Directory.CreateDirectory(collectorDir);
            }

            //try
            //{
            string newlyCreatedFileNamePreFix = collectorDir + @"\" + collectorId + "-" +
                DateTime.Now.ToString(DATE_FORMAT_FILE, DateTimeFormatInfo.InvariantInfo);

            DirectoryInfo dirInfo = new DirectoryInfo(collectorDir);

            FileInfo[] fileInfo = dirInfo.GetFiles(collectorId + "*");

            List<FileInfo> fileInfoList = new List<FileInfo>();

            for (int i = 0; i < fileInfo.Length; i++)
            {
                fileInfoList.Add(fileInfo[i]);
            }

            fileInfoList.Sort(fileModificationComparer);

            if (this.initialized == false && fileInfoList.Count != 0)
            {
                FileInfo lastUsedFileInfo = fileInfoList[fileInfoList.Count - 1];
                string lastUsedFileName = lastUsedFileInfo.FullName;

                if (lastUsedFileName.StartsWith(newlyCreatedFileNamePreFix))
                {
                    int startIndex = lastUsedFileName.LastIndexOf("-") + 1;
                    int lastIndex = lastUsedFileName.LastIndexOf(".log");

                    this.suffixCount = Convert.ToInt32(lastUsedFileName.
                     Substring(startIndex, lastIndex - startIndex));
                    this.lastCreatedFileName = lastUsedFileName;
                }
                this.initialized = true;
            }


            int oldCnt = 0;

            if (!(this.lastCreatedFileName == null || this.lastCreatedFileName.Equals("")))
            {
                using (StreamReader sr = new StreamReader(lastCreatedFileName))
                {
                    String line;
                    // Read and display lines from the file until the end of 
                    // the file is reached.
                    while ((line = sr.ReadLine()) != null)
                    {
                        oldCnt++;
                    }
                    sr.Close();
                }
            }
            else
            {
                suffixCount = 0;
            }

            if (this.isRolloverBydays)
            {
                foreach (FileInfo info in fileInfoList)
                {
                    TimeSpan span = DateTime.Now.Subtract(info.LastWriteTime);

                    if ((int)span.TotalDays > this.rollOverdays)
                    {
                        info.Delete();
                    }
                }
            }
            else
            {
                // remove oldest file.
                if (fileInfoList.Count > this.maxFileCount - 1)
                {
                    for (int i = 0; i <= fileInfoList.Count - this.maxFileCount; i++)
                    {
                        fileInfoList[i].Delete();
                    }
                }
            }

            suffixCount = (lastCreatedFileName.StartsWith(newlyCreatedFileNamePreFix)) ? ++suffixCount : 0;

            string suffixCountString = "-" + Convert.ToString(suffixCount) + suffix;

            if ((this.lastCreatedFileName == null || this.lastCreatedFileName.Equals("")))
            {
                lastCreatedFileName = newlyCreatedFileNamePreFix + "-0.log";
            }
            else if (isWorkingDayChanged())
            {
                currentFileSize = 0;
                lastCreatedFileName = newlyCreatedFileNamePreFix + suffixCountString;
                workingDay = DateTime.Now.ToString("yyyy-MM-dd");
            }
            else if (maxFileSize <= oldCnt)
            {
                currentFileSize = 0;
                lastCreatedFileName = newlyCreatedFileNamePreFix + suffixCountString;
            }
            else
            {
                currentFileSize = oldCnt;
            }

            fileStream = new FileStream(lastCreatedFileName, FileMode.Append, FileAccess.Write, FileShare.Read);
            streamWriter = new StreamWriter(fileStream);
            //}
            //catch (IOException ex)
            //{
            //    throw ex;
            //}

        }
    }

    public class StatisticalLogInfo
    {
        public bool enabled = false;
        public bool isRolloverByDays = false;
        public int files = 0;
        public int entries = 0;
        public int days = 0;

        public StatisticalLogInfo(bool enabled, int days, int files, int entries)
        {
            this.enabled = enabled;
            this.files = files;
            this.entries = entries;
            this.isRolloverByDays = true;
            this.days = days;
        }

        public StatisticalLogInfo(bool enabled, int files, int entries)
        {
            this.enabled = enabled;
            this.files = files;
            this.entries = entries;
            this.isRolloverByDays = false;
            this.days = 30;
        }
        public StatisticalLogInfo(bool enabled, int entries)
        {
            this.enabled = enabled;
            this.entries = entries;
            this.files = 5;
            this.isRolloverByDays = false;
            this.days = 30;
        }

    }
    public class StatisticalFileModificationComparer : IComparer<FileInfo>
    {
        public int Compare(FileInfo x, FileInfo y)
        {
            if (x == null)
            {
                if (y == null)
                {
                    // If x is null and y is null, they're
                    // equal. 
                    return 0;
                }
                else
                {
                    // If x is null and y is not null, y
                    // is greater. 
                    return 1;
                }
            }
            else
            {
                // If x is not null...
                //
                if (y == null)
                // ...and y is null, x is greater.
                {
                    return -1;
                }
                else
                {
                    // ...and y is not null, compare the 
                    // priority of the two workItem.
                    //

                    if (x.LastWriteTime.Ticks > y.LastWriteTime.Ticks)
                        return 1;
                    else if (x.LastWriteTime.Ticks < y.LastWriteTime.Ticks)
                        return -1;
                    else
                        return 0;

                }
            }
        }
    }
}
