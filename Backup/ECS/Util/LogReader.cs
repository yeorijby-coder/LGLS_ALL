using System;
using System.Collections.Generic;
using System.Text;
using System.Collections.Specialized;
using System.Configuration;
using System.IO;
using System.Windows.Forms;
using System.Threading;
using System.Drawing;

using ECP.Global;
using ECP.Service.Statistics;

using HECS.Statistics;

namespace HECS.Util
{
    /// <summary>
    /// Log Reading Status
    /// </summary>
    public enum LogReadStatus
    { 
        BeginLoading = 1,
        EndLoading = 2,
        DisplayRequest = 3
    }

    /// <summary>
    /// 페이지 정보를 담는 구조체
    /// 총 로그가 담긴 배열에서 해당 패이지가 가지는 정보
    /// </summary>
    public struct PageInfo
    {
        /// <summary>
        /// 페이지의 Start Index
        /// </summary>
        public int startIndex;
        /// <summary>
        /// 페이지의 End Index
        /// </summary>
        public int endIndex;
        /// <summary>
        /// 현제 페이지 번호
        /// </summary>
        public int currentPage;
    }
    public delegate void delegateLogLoadingStatusChanged(LogReadStatus status, List<string> logList);

    public class LogReader
    {
        private string logDir;

        /// <summary>
        /// Display Log Type
        /// </summary>
        private string collectorId;
      
        private const int MEGA_BYTE = 1024*1024;
        /// <summary>
        /// 화면 표기 Line 수
        /// </summary>
        private const int MAX_DISPLAY_LINE = 500;
        /// <summary>
        /// File Read를 통해 읽어들인 All Log Data를 담고 있음.
        /// </summary>
        private string[] logData;
        /// <summary>
        /// 로그 Display List View
        /// </summary>
        private ListView view;

        /// <summary>
        /// 파일 Loading Status Event
        /// </summary>
        public event delegateLogLoadingStatusChanged LogLoadingStatusChanged = null;

        /// <summary>
        /// 검색하고자 하는 From Day
        /// </summary>
        private DateTime fromDate = DateTime.Now;
        /// <summary>
        /// 검색하고자 하는 End Day
        /// </summary>
        private DateTime toDate = DateTime.Now;
        /// <summary>
        /// 현재 보고 있는 화면의 패이지 0부터 시작 
        /// </summary>
        private int currentPage = 0;

        /// <summary>
        /// 검색 Keyword
        /// </summary>
        private string keyWord = "";

        private enum LogColorType
        { 
            OBSERVABLE_LOG_COLOR,
            TRANSFER_LOG_COLOR,
            ERROR_LOG_COLOR,
            SERVO_LOG_COLOR
        }

        #region 생성자
        public LogReader(string collectorId, ListView view)
        {
            this.collectorId = collectorId;
            this.view = view;
            GetLogDirectory();
        }
        #endregion 생성자

        private string GetLogDirectory()
        {
            NameValueCollection appSettings = ConfigurationManager.AppSettings;

            if (collectorId.Equals("HOST"))
            {
                DateTime workDate = DateTime.Now;
                string strDay = workDate.ToString("yyyy-MM-dd");
                return appSettings[ECPProperties.HOST_LOG].ToString() + @"\" + strDay;
            }
            else
            {
                return appSettings[ECPProperties.ECP_LOG].ToString() + @"\" + "Log" + @"\" + "Statistics" + @"\" + collectorId;
            }
        }

        /// <summary>
        /// 지정된 날짜 범위 않에 생성된 File Name을 가져 옴.
        /// </summary>
        /// <param name="fromDate">검색 대상 시작일자</param>
        /// <param name="toDate">검색 대상 마직막일 </param>
        /// <returns>범위내 파일명들</returns>
        private List<string> GetLogFileNames(DateTime fromDate, DateTime toDate)
        {
            
            List<string> fileNames = new List<string>();
            try
            {
                this.logDir = GetLogDirectory();
                DirectoryInfo dirInfo = new DirectoryInfo(this.logDir);

                if (dirInfo == null)
                    return null;

                FileInfo[] files = dirInfo.GetFiles();

                if (files.Length == 0)
                    return null;

                foreach (FileInfo file in files)
                {
                    if (Util.DayCompare(fromDate, file.LastWriteTime) >= 0 && Util.DayCompare(toDate, file.LastWriteTime) <= 0)
                    {
                        fileNames.Add(file.Name);
                    }
                }
                return fileNames;
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "GetLogFileNames", "", ex.ToString(), ex.StackTrace.ToString()));

                return fileNames;  
            }
        }

        /// <summary>
        /// 1개 이상의 File을 읽어 들여 LogData에 넣어 둠.
        /// </summary>
        /// <param name="fileNames">읽을 파일명</param>
        private void ReadMultiFile(List<string> fileNames)
        {
            if (fileNames.Count == 0)
                return;
            
            List<string> stringBuffer = new List<string>(4096);
            try
            {
                for (int i = 0; i < fileNames.Count; i++)
                {
                    string filepath = this.logDir + @"\" + fileNames[i].ToString();
                    FileStream fileStream = new FileStream(filepath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
                    using (BufferedStream stream = new BufferedStream(fileStream))
                    {
                        StreamReader reader = new StreamReader(stream, Encoding.UTF8);
                        string readLine;
                        while (true)
                        {
                            readLine = reader.ReadLine();
                            if (readLine == null)
                            {
                                break;
                            }
                            if (String.IsNullOrEmpty(this.keyWord))
                            {
                                stringBuffer.Add(readLine);
                            }
                            else
                            {
                                if (IsHasKeyInLine(readLine))
                                    stringBuffer.Add(readLine);
                            }
                        }
                    }
                    stringBuffer.TrimExcess();
                    
                    fileStream.Close();
                }
                this.logData = stringBuffer.ToArray();
                stringBuffer.Clear();
            }
            catch (ArgumentOutOfRangeException ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "ReadMultiFile", "", ex.ToString(), ex.StackTrace.ToString()));

            }
            catch (OutOfMemoryException ex)
            {

                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "ReadMultiFile", "", ex.ToString(), ex.StackTrace.ToString()));
 
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "ReadMultiFile", "", ex.ToString(), ex.StackTrace.ToString()));
            }

        }

        private bool IsHasKeyInLine(string line)
        {
            string lineUpper = line.ToUpper();
            string keyUpper = null;

            if (String.IsNullOrEmpty(this.keyWord))
                return false;

            keyUpper = this.keyWord.ToUpper();

            string[] keys = keyUpper.Split(',');

            foreach (string key in keys)
            {
                if (lineUpper.Contains(key))
                    return true;
            }

            return false;
        }

        public void WirtLogForCSV(string log)
        {
            try
            {
                DateTime workDate = DateTime.Now;
                // 바탕화면 Get Path
                string yourAppPath = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);

                FileStream fileStream = new FileStream(yourAppPath +@"\" + collectorId + workDate.ToString("yyyy-MM-dd") + ".CSV", FileMode.OpenOrCreate, FileAccess.Write, FileShare.ReadWrite);
                using (StreamWriter stream = new StreamWriter(fileStream))
                {
                    stream.Write(log);
                    stream.Close();
                }
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "WirtLogForCSV", "", ex.ToString(), ex.StackTrace.ToString()));

            }
        }

        /// <summary>
        /// 마지막으로 화면에 표시한 화면 다음 Page를 보여 줌
        /// </summary>
        public void DisplayNextPage()
        {
            if (this.logData == null)
                return;

            List<string> tmpLogForDisplay = new List<string>();

            try
            {

                int totalPage = GetTotalPage();

                this.currentPage = totalPage <= this.currentPage ? this.currentPage : this.currentPage + 1;

                PageInfo page = GetPageInfo(this.currentPage);

                for (int i = page.startIndex; i <= page.endIndex; i++)
                {
                    tmpLogForDisplay.Add(this.logData[i].ToString());
                }

            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayNextPage","", ex.ToString(), ex.StackTrace.ToString()));
            }

            DisplayLogList(tmpLogForDisplay);
        }

        /// <summary>
        /// 현재 Page의 이전 Page Display
        /// </summary>
        public void DisplayPreviousPage()
        {
  
            if (this.logData == null)
                return;

            List<string> tmpLogForDisplay = new List<string>();

            try
            {
                int totalPage = GetTotalPage();

                this.currentPage = 0 >= this.currentPage ? 0 : this.currentPage -1;

                PageInfo page = GetPageInfo(this.currentPage);

                for (int i = page.startIndex; i <= page.endIndex; i++)
                {
                    tmpLogForDisplay.Add(this.logData[i].ToString());
                }
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayPreviousPage", "", ex.ToString(), ex.StackTrace.ToString()));
            }

            DisplayLogList(tmpLogForDisplay);

        }

        private void DisplayLogPeriodUsingThread()
        {
            this.logData = null;

            List<string> files = GetLogFileNames(this.fromDate, this.toDate);

            if (files != null)
            {
                if (this.LogLoadingStatusChanged != null)
                    LogLoadingStatusChanged(LogReadStatus.BeginLoading,null);

                ReadMultiFile(files);
            }

            if (this.logData == null)
            {
                if (this.LogLoadingStatusChanged != null)
                    LogLoadingStatusChanged(LogReadStatus.EndLoading,null);

                return;
            }

            List<string> latestLog = new List<string>();

            try
            {

                PageInfo page =  GetPageInfo(0);

                for (int i = page.startIndex; i <= page.endIndex; i++)
                {
                    latestLog.Add(this.logData[i].ToString());

                }
                this.currentPage = 0;

                if (this.LogLoadingStatusChanged != null)
                {
                    LogLoadingStatusChanged(LogReadStatus.EndLoading, null);
                    LogLoadingStatusChanged(LogReadStatus.DisplayRequest,latestLog);
                    
                }

            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayLatestLog", "", ex.ToString(), ex.StackTrace.ToString()));
            }

        }
        /// <summary>
        /// 가장 최근의 LOG를 읽어 드림.
        /// </summary>
        public void DisplayLogPeriod(DateTime fromDate, DateTime toDate)
        {
            try
            {
                this.fromDate = fromDate;
                this.toDate = toDate;
                this.keyWord = "";
                Thread readThread = new Thread(new ThreadStart(DisplayLogPeriodUsingThread));
                readThread.Name = "Log Read Thread";
                readThread.IsBackground = true;
                readThread.Start();
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayLogPeriod", "", ex.ToString(), ex.StackTrace.ToString()));

            }
           
        }
        /// <summary>
        /// 가장 최근의 LOG를 읽어 드림.
        /// </summary>
        public void DisplayLogPeriod(DateTime fromDate, DateTime toDate, string keyword)
        {
            try
            {
                this.fromDate = fromDate;
                this.toDate = toDate;
                this.keyWord = keyword;

                Thread readThread = new Thread(new ThreadStart(DisplayLogPeriodUsingThread));
                readThread.Name = "Log Read Thread";
                readThread.IsBackground = true;
                readThread.Start();
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayLogPeriod", "", ex.ToString(), ex.StackTrace.ToString()));

            }

        }


        /// <summary>
        /// LOG를 LIST VIEW로 Loading
        /// </summary>
        public void DisplayLogList(List<string> logData)
        {
            if (this.view == null || logData == null)
                return;

            view.BeginUpdate();
            view.Items.Clear();

            if (this.collectorId.Equals("HOST"))
            {
                logData.Reverse();
            }

            try
            {
                for (int i = 0; i < logData.Count; i++)
                {
                    ListViewItem item = new ListViewItem();

                    string[] tokens = logData[i].Split('/');

                    switch (this.collectorId)
                    {
                        case "DEB":
                            try
                            {
                                item.Text = tokens[0];
                                item.SubItems.Add(tokens[1].Trim());
                                item.SubItems.Add(tokens[2].Trim());
                                item.SubItems.Add(tokens[3].Trim());
                                item.SubItems.Add(tokens[4].Trim());
                                item.SubItems.Add(tokens[5].Trim());
                                view.Columns[3].TextAlign = HorizontalAlignment.Left;
                                view.Columns[4].TextAlign = HorizontalAlignment.Left;
                                view.Columns[5].TextAlign = HorizontalAlignment.Left;
                            }
                            catch (Exception ex)
                            { 
                                System.Console.WriteLine("LogReader:" + ex.StackTrace);
                            }
                            break;
                        case "OPR":
                            try
                            {
                                item.Text = tokens[0];
                                view.Columns[1].TextAlign = HorizontalAlignment.Left;
                                item.SubItems.Add(tokens[2].Trim());
                                view.Columns[2].TextAlign = HorizontalAlignment.Left;
                                item.SubItems.Add(tokens[3].Trim());
                                view.Columns[3].TextAlign = HorizontalAlignment.Left;
                                item.SubItems.Add(tokens[4].Trim());
                            }
                            catch(Exception ex)
                            {
                                // Nothing 포멧을 마추지 않으면 발생
                            }
                            break;

                        case "OBSERVABLE":
                            item.Text = tokens[0];
                            item.SubItems.Add(tokens[2].Trim());
                            item.SubItems.Add(tokens[3].Trim());
                            item.SubItems.Add(tokens[5].Trim());
                            item.SubItems.Add(tokens[6].Trim());
                            item.SubItems.Add(tokens[8].Trim());
                            item.SubItems.Add(tokens[4].Trim());
                            view.Columns[1].TextAlign = HorizontalAlignment.Left;
                            view.Columns[3].TextAlign = HorizontalAlignment.Left;
                            view.Columns[4].TextAlign = HorizontalAlignment.Left;

                            // Color
                            item.BackColor = GetLogColorPatern(tokens[2],LogColorType.OBSERVABLE_LOG_COLOR);

                            break;
                        case "ALARM":
                            item.Text = tokens[0];
                            item.SubItems.Add(tokens[2].Trim());
                            item.SubItems.Add(tokens[3].Trim().Equals("False") ? "RESET" : "SET");
                            item.SubItems.Add(tokens[4].Trim());
                            // Color
                            item.BackColor = GetLogColorPatern(tokens[3], LogColorType.ERROR_LOG_COLOR); // TrsnsferState

                            break;
                        case "HOST":
                            {
                                string[] seperater = new string[] { "SystemBytes"};
                                string[] HostTokens = logData[i].Split(seperater,StringSplitOptions.RemoveEmptyEntries);

                                if (HostTokens.Length > 1)
                                {
                                    item.Text = logData[i];
                                    item.SubItems.Add("");
                                }
                                else
                                {
                                    item.Text = "";
                                    item.SubItems.Add(logData[i]);
                                }
                                view.Columns[0].TextAlign = HorizontalAlignment.Left;
                                view.Columns[1].TextAlign = HorizontalAlignment.Left;

                            }
                            break;
                        case "TRS":
                            {
                                item.Text = tokens[0];
                                item.SubItems.Add(tokens[2].Trim());
                                item.SubItems.Add(tokens[3].Trim());
                                item.SubItems.Add(tokens[4].Trim());
                                item.SubItems.Add(tokens[5].Trim());
                                item.SubItems.Add(tokens[6].Trim());
                                item.SubItems.Add(tokens[7].Trim());
                                item.SubItems.Add(tokens[8].Trim());
                                item.SubItems.Add(tokens[9].Trim());
                                item.SubItems.Add(tokens[10].Trim());
                                item.SubItems.Add(tokens[11].Trim());
                                item.SubItems.Add(tokens[12].Trim());
                                item.SubItems.Add(tokens[13].Trim());
                                item.SubItems.Add(tokens[14].Trim());
                                item.SubItems.Add(tokens[15].Trim());
                                item.SubItems.Add(tokens[16].Trim());

                                // Color
                                item.BackColor = GetLogColorPatern(tokens[4], LogColorType.TRANSFER_LOG_COLOR); // TrsnsferState

                            }
                            break;
                        case "Servo":
                            {
                                item.Text = tokens[0];
                                item.SubItems.Add(tokens[1].Trim());
                                item.SubItems.Add(tokens[2].Trim());
                                // Color
                                item.BackColor = GetLogColorPatern(tokens[1], LogColorType.SERVO_LOG_COLOR); // TrsnsferState
                            }
                            break;
                    }
                    view.Items.Insert(0, item);

                    if (i == MAX_DISPLAY_LINE - 1)
                    {
                   //     break;
                    }
                }
            }
            catch (Exception ex)
            {
                StatisticsCollector.TriggerOccured(
                RunTimeExceptionStatisticsCollectorData.COLLECTOR_ID,
                RunTimeExceptionStatisticsCollectorData.TRIGGER_UI_EXCEPTION,
                new RunTimeExceptionStatisticsCollectorData("LogReader", "DisplayLogList", "", ex.ToString(), ex.StackTrace.ToString()));
            }

            view.EndUpdate();
        }

        private Color GetLogColorPatern(string key, LogColorType type)
        {
            Color color = Color.Beige;

            if( String.IsNullOrEmpty(key))
                color =  Color.Beige;

            switch (type)
            {
                case LogColorType.OBSERVABLE_LOG_COLOR:
                    {
                        if (key.Contains("WRITE"))
                            color = Color.Khaki;
                    }
                    break;
                case LogColorType.TRANSFER_LOG_COLOR:
                    {
                        if (key.Contains("QUEUED"))
                            color = Color.Khaki;
                        else if (key.Contains("TRANSFERRING"))
                            color = Color.Gold;
                        else if (key.Contains("COMPLETED"))
                            color = Color.GreenYellow;
                        else if (key.Contains("ALT_HOFF"))
                            color = Color.Plum;
                    }
                    break;
                case LogColorType.ERROR_LOG_COLOR:
                    {
                        if (key.Contains("True"))
                            color = Color.Plum;
                        else if (key.Contains("False"))
                            color = Color.GreenYellow;
                    }
                    break;
                case LogColorType.SERVO_LOG_COLOR:
                    {
                        if (key.Contains("ERROR"))
                            color = Color.Khaki;
                    }
                    break;
            }


            return  color;
        }

        /// <summary>
        /// 총 Log 파일을 몇 페이지로 구성 되는지 알 수 있음.
        /// </summary>
        /// <returns></returns>
        private int GetTotalPage()
        {
            return this.logData.Length / MAX_DISPLAY_LINE;
        }

        /// <summary>
        /// 로그 파일을 화면으로 구성하였을때 패이지 수를 넘기면 총 Log의 Start Index와 End Index를 알려 줌
        /// </summary>
        /// <param name="newPage">보고자하는 페이지</param>
        /// <returns>패이지 인포메이션</returns>
        private PageInfo GetPageInfo(int newPage)
        {
            PageInfo page = new PageInfo();
            int totalPage =  GetTotalPage();
            
            // New Page가 - 또는 0 또는 Max Page 보다 클 경우 현재 Page를 돌려 줌
            if (0 >= newPage)
            {
                page.currentPage = 0;

                // 1패이지 이상이라면 Total page*DisplayLine 0 패이지라면 Array 0가 Start Index
                page.startIndex = totalPage > 0 ? logData.Length - MAX_DISPLAY_LINE : 0;
                page.endIndex = logData.Length-1;

            }
            else if (1 <= newPage && newPage < totalPage)
            {
                page.endIndex = this.logData.Length - MAX_DISPLAY_LINE * newPage-1;
                page.startIndex = page.endIndex - MAX_DISPLAY_LINE+1;
                page.currentPage = newPage;
            }
            else if (totalPage <= newPage)
            {
                page.endIndex = this.logData.Length - MAX_DISPLAY_LINE * newPage - 1;
                page.startIndex = 0;
                page.currentPage = newPage;

            }

            Console.WriteLine("Start : " + page.startIndex + " End : " + page.endIndex +" NewPage : "+ newPage +" Page : " + page.currentPage);

            return page;
        }
    }
}
