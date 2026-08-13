using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;
using System.Text;
using System.Data;

using ECP.Global.Device;
using ECP.Device.Observe;
using ECP.Global;
using ECP.Com.FieldBus;
using ECP.Global.Com;
using ECP.Service.Logger;
using System.Net;
using System.Net.Sockets;

namespace EzControl.Driver.Fenet
{
    /// <summary>
    /// FenetDriver에 대한 요약 설명입니다.
    /// </summary>
    public partial class FenetDriver : AbstractFieldBusDriver
    {
        private static object socketlockObject = new object();
        //private static IPHostEntry hostEntry = Dns.GetHostEntry(GlobalConstant.PLC_IP);
        //private static IPEndPoint ipe = new IPEndPoint(hostEntry.AddressList[0].Address, 2004);
        private static Socket plcSocket = null;

        #region Properties
        private FenetMemoryCollection m_MonitorMemory = new FenetMemoryCollection();

        private ThreadPriority m_ThreadPriority = ThreadPriority.Highest;
        private Thread m_MonitorThread = null;
        private System.Timers.Timer m_tmrMonitor = new System.Timers.Timer();
        private bool m_bMonitoring = false;
        private bool m_bEnableMonitoring = false;
        private short m_sMaxReadSize = 8192;

        private bool m_bScanflag = false;


        public ThreadPriority ThreadPriority
        {
            get
            {
                return m_ThreadPriority;
            }
            set
            {
                m_ThreadPriority = value;
            }
        }

        private int m_nMonitorInterval = 2000;
        public int MonitorInterval
        {
            get
            {
                return m_nMonitorInterval;
            }
            set
            {
                m_nMonitorInterval = value;
            }
        }

        /// <summary>
        /// Fenet Driver 사용을 위한 Minimum WorkingSet Size를 1MB로 설정
        /// </summary>
        private const int m_iMinWorkingSetSize = 0x100000;
        /// <summary>
        /// Fenet Driver 사용을 위한 Maximum WorkingSet Size를 3MB로 설정
        /// </summary>
        private const int m_iMaxWorkingSetSize = 0x300000;
        #endregion

        #region Constructor & Disposer
        public FenetDriver()
        {
            //plcSocket = new Socket(ipe.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            //plcSocket.Connect(GlobalConstant.PLC_IP, GlobalConstant.PLC_PORT);

            plcSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress[] IPs = Dns.GetHostAddresses(GlobalConstant.PLC_IP);
            //plcSocket.Blocking = false;
            plcSocket.Connect(IPs[0], GlobalConstant.PLC_PORT);


            m_sChannelNo = 51;
            m_sStationNo = 0xFF;
            m_sMode = -1;
            m_iPath = 0;
            // Working Set Size를 설정 한다.
            SetProcessWorkingSetSize(Process.GetCurrentProcess().Handle, m_iMinWorkingSetSize, m_iMaxWorkingSetSize);

            m_tmrMonitor.Elapsed += new System.Timers.ElapsedEventHandler(m_tmrMonitor_Elapsed);
            //InitMonitorThread();

            return;
        }

        protected void Dispose(bool disposing)
        {
            StopMonitorThread();
        }
        #endregion

        #region Monitoring
        private int RefreshMonitorMemory()
        {
            int iRet = 0;
            lock (socketlockObject)
            {
                int exceptionCount = 0;
            jmp:
                try
                {
                    foreach (FenetMemory mem in m_MonitorMemory.Values)
                    {
                        ////////sSize = (short)(mem.RawMemorySize * 2);
                        ////////int iRet = 0;
                        ////////iRet = mdReceive(m_iPath, m_sStationNo, (short)mem.DeviceType, mem.StartAdderess, ref sSize, mem.RawMemory);
                        ////////if (!IsSuccess(iRet))
                        ////////{
                        ////////    this.Notify(1003, this.Name, string.Format("Melsec Net Driver Monitoring Failed!(mdReceive, ErrorCode = {0})", iRet));
                        ////////    this.m_bMonitoring = false;
                        ////////}

                        switch (mem.DeviceType)
                        {
                            case enumDeviceType.X:
                            case enumDeviceType.Y:
                            case enumDeviceType.B:
                            case enumDeviceType.SB:
                                //short sSize = (short)(mem.RawMemorySize * 2);
                                int retryCount = 0;
                            jmp1:
                                short sSize = (short)(mem.RawMemorySize);
                            iRet = mdReceive(m_iPath, m_sStationNo, (short)mem.DeviceType, mem.StartAdderess, ref sSize, mem.RawMemory);
                            //iRet = mdReceiveEx(m_iPath, 1, 1, (int)mem.DeviceType, mem.StartAdderess, ref sSize, mem.RawMemory);

                            if (!IsSuccess(iRet))
                            {
                                if (retryCount < 3)
                                {
                                    retryCount++;
                                    goto jmp1;
                                }
                                Log.log(5, "RefreshMonitorMemory", "error", Category.ERROR, "", "Melsec Driver Read Error", "Driver ErrorCode = " + iRet);
                                ////this.FBConnectionStateChange(FBConnectionState.DISCONNECTED);
                                ////this.m_bMonitoring = false;
                                return -1;
                            }
                            break;
                            case enumDeviceType.W:
                            case enumDeviceType.SW:
                            case enumDeviceType.WW:
                            case enumDeviceType.WR:
                            case enumDeviceType.R:

                            int i, j;
                            short[] sMemory = new short[m_sMaxReadSize];
                            short sMemorySize = 0;
                            i = 0;
                            j = 0;
                            while (true)
                            {
                                if (mem.RawMemorySize <= m_sMaxReadSize * i)
                                    break;

                                if (mem.RawMemorySize >= (m_sMaxReadSize * (i + 1)))
                                {
                                    sMemorySize = (short)(m_sMaxReadSize * 2);
                                }
                                else
                                {
                                    sMemorySize = (short)((mem.RawMemorySize - (m_sMaxReadSize * i)) * 2);
                                }

                                //                            iRet = mdReceive(m_iPath, m_sStationNo, (short)mem.DeviceType, (short)(m_sMaxReadSize * i), ref sMemorySize, sMemory);
                                //신승원 소스 수정함.
                                //(short)(mem.StartAdderess  * i)
                                int retryCount2 = 0;
                            jmp2:
                                iRet = mdReceive(m_iPath, m_sStationNo, (short)mem.DeviceType, (short)(mem.StartAdderess * i), ref sMemorySize, sMemory);
                                //iRet = mdReceiveEx(m_iPath, 1, 1, (int)mem.DeviceType, mem.StartAdderess * i, ref sMemorySize, sMemory);

                                if (!IsSuccess(iRet))
                                {
                                    if (retryCount2 < 3)
                                    {
                                        retryCount2++;
                                        goto jmp2;
                                    }
                                    Log.log(5, "RefreshMonitorMemory", "error", Category.ERROR, "", "Melsec Driver Read Error", "Driver ErrorCode = " + iRet);
                                    //this.m_bMonitoring = false;
                                    return -1;
                                }
                                for (j = 0; j < sMemorySize / 2; j++)
                                {
                                    mem.RawMemory[i * m_sMaxReadSize + j] = sMemory[j];
                                }
                                i++;
                            }
                            break;
                        }

                    }
                }
                catch (InvalidOperationException ex)
                {
                    exceptionCount++;
                    if (exceptionCount < 3)
                    {
                        goto jmp;
                    }
                }
            }
            return 0;
        }

        /// <summary>
        /// MNet Function 실행 결과 값의 성공 여부 확인
        /// 
        /// - 성공
        ///    0      : Normal End
        ///    66     : Already Opend
        ///    -28150 : Data Link Error, Access the Own station's link device when data link not in progress.
        ///             Writing the data or reading are done. However, the data is not guaranteed.
        /// </summary>
        /// <param name="iRet"></param>
        /// <returns></returns>
        private bool IsSuccess(int iRet)
        {
            return true;
            bool bIsSuccess = (iRet == 0) || (iRet == 0) || (iRet == 66) || (iRet == -28150);
            if (!bIsSuccess)
            {
                RaiseErrorOccurred(iRet);
                Close();
                ActiveOpen();
            }
            return bIsSuccess;
        }

        private void InitMonitorThread()
        {
            //m_MonitorThread = new Thread(new ThreadStart(DoMonitor));
            //m_MonitorThread.IsBackground = true;
            //m_MonitorThread.Priority = m_ThreadPriority;

            //m_bMonitoring = true;
            //m_MonitorThread.Start();


            m_tmrMonitor.Interval = m_nMonitorInterval;

            // 위치 이동 Init 할때마다 Deleagation 생성 될 수 있음.
            //m_tmrMonitor.Elapsed += new System.Timers.ElapsedEventHandler(m_tmrMonitor_Elapsed);

            //Test m_tmrMonitor.Enabled = true;
            m_bScanflag = true;
        }

        public override void RefreshMonitorMemoryByObservable()
        {
            //Test m_tmrMonitor.Enabled = false;
            m_bScanflag = false;

            lock (m_Monitor)
            {
                if (RefreshMonitorMemory() == 0)
                {
                    //Test m_tmrMonitor.Enabled = true;
                    m_bScanflag = true;
                }
                else
                {
                    //Test m_tmrMonitor.Enabled = false;
                    m_bScanflag = false;
                }
            }

        }

        /*        
                public void RefreshMonitorMemoryByObservable()
                {
                    //Test m_tmrMonitor.Enabled = false;
                    m_bScanflag = false;

                    lock (m_Monitor)
                    {
                        if (RefreshMonitorMemory() == 0)
                        {
                            //Test m_tmrMonitor.Enabled = true;
                            m_bScanflag = true;
                        }
                        else
                        {
                            //Test m_tmrMonitor.Enabled = false;
                            m_bScanflag = false;
                        }
                    }

                }
        */
        void m_tmrMonitor_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            //Test m_tmrMonitor.Enabled = false;
            m_bScanflag = false;

            lock (m_Monitor)
            {
                if (RefreshMonitorMemory() == 0)
                {
                    //Test m_tmrMonitor.Enabled = true;
                    m_bScanflag = true;
                }
                else
                {
                    //Test m_tmrMonitor.Enabled = false;
                    m_bScanflag = false;
                }
            }
        }

        private void StopMonitorThread()
        {
            try
            {
                ////if (m_MonitorThread != null && m_MonitorThread.IsAlive)
                ////{
                ////    m_bMonitoring = false;
                ////    m_MonitorThread.Join();
                ////    m_MonitorThread = null;
                ////}

                //Test m_tmrMonitor.Enabled = false;
                m_bScanflag = false;
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("StopMonitorThread:" + ex.StackTrace);
                Log.log(1, "StopMonitorThread", "error", Category.ERROR, "", "StopMonitorThread error", ex.StackTrace.ToString());
            }
        }

        private void StartMonitor()
        {
            //m_bEnableMonitoring = true;
            //Test m_tmrMonitor.Enabled = true;
            m_bScanflag = true;
        }

        private void StopMonitor()
        {
            //m_bEnableMonitoring = false;
            //Test m_tmrMonitor.Enabled = false;
            m_bScanflag = false;
        }

        private object m_Monitor = new object();
        private void DoMonitor()
        {
            int iRet = 0;
            while (m_bMonitoring)
            {
                Thread.Sleep(m_nMonitorInterval);
                if (!this.Enabled && !this.Connected)
                    continue;

                if (m_bEnableMonitoring)
                {
                    lock (m_Monitor)
                    {
                        RefreshMonitorMemory();
                    }
                }
            }
        }

        protected override void ChangeConnectionState(FBConnectionState connectionState)
        {
            base.ChangeConnectionState(connectionState);
            switch ((int)connectionState.Value())
            {
                case FBConnectionState._CONNECTED:
                    StartMonitor();
                    break;
                case FBConnectionState._DISCONNECTED:
                    StopMonitor();
                    break;
            }
        }

        #endregion

        #region Fenet 관련 Function

        private short m_sChannelNo;
        public short ChannelNo
        {
            get
            {
                return m_sChannelNo;
            }
            set
            {
                m_sChannelNo = value;
            }
        }
        private short m_sStationNo;
        public short StationNo
        {
            get
            {
                return m_sStationNo;
            }
            set
            {
                m_sStationNo = value;
            }
        }
        private short m_sMode;
        public short Mode
        {
            get
            {
                return m_sMode;
            }
            set
            {
                m_sMode = value;
            }
        }
        private int m_iPath;
        public int Path
        {
            get
            {
                return m_iPath;
            }
        }

        [DllImport("kernel32.dll")]
        private static extern bool SetProcessWorkingSetSize(IntPtr hProcess, int iMinimumWorkingSetSize, int iMaximumWorkingSetSize);
        private static short mdOpen(short sChan, short sMode, ref int iPath)
        {
            return 0;
        }
        private static short mdClose(int iPath)
        {
            return 0;
        }

        private static short mdDevRst(short sAddress, enumDeviceType DeviceType)
        {
            lock (socketlockObject)
            {
                Dictionary<string, byte[]> frameList = new Dictionary<string, byte[]>();
                frameList.Add("COMPANYID", new byte[10] { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T', 0x00, 0x00 });
                frameList.Add("PLCINFO", new byte[2] { 0x00, 0x00 });
                frameList.Add("CPUINFO", new byte[1] { 0xA0 });
                frameList.Add("SOURCEFRAME", new byte[1] { 0x33 });
                frameList.Add("INVOKEID", new byte[2] { 0x10, 0x01 });
                frameList.Add("LENGTH", new byte[2] { 0x10, 0x00 });
                frameList.Add("POSITION", new byte[1] { 0x00 });
                int checksum = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    foreach (byte b in frame)
                    {
                        checksum = checksum + b;
                    }
                }
                string strMemAddress = sAddress.ToString();
                frameList.Add("CHECKSUM", new byte[1] { (byte)checksum });
                frameList.Add("COMMAND", new byte[2] { 0x58, 0x00 });
                frameList.Add("DATATYPE", new byte[2] { 0x00, 0x00 });      //연속읽기
                frameList.Add("RESERVED1", new byte[2] { 0x00, 0x00 });
                frameList.Add("BLOCKCOUNT", new byte[2] { 0x01, 0x00 });
                frameList.Add("VARIALBLE_LENGTH", new byte[2] { (byte)(0x03 + strMemAddress.Length), 0x00 });
                frameList.Add("DEVICE", new byte[3] { (byte)'%', (byte)'M', (byte)'X' });//%MB0

                byte[] bAddressTemp = new byte[strMemAddress.Length];
                for (int x = 0; x < bAddressTemp.Length; x++)
                {
                    bAddressTemp[x] = (byte)strMemAddress.ToCharArray()[x];
                }
                frameList.Add("DEVICEADDRESS", bAddressTemp);//%MB0
                string strTemp = 1.ToString("0###");
                int b2 = int.Parse(strTemp.Substring(0, 2));
                int b1 = int.Parse(strTemp.Substring(2, 2));
                frameList.Add("DATACOUNT", new byte[2] { (byte)b1, (byte)b2 }); //1400bytes 가능
                byte[] dataBlock = new byte[1] { 0x00 };
                frameList.Add("DATABLOCK", dataBlock); //1400bytes 가능

                int dataLength = frameList["COMMAND"].Length + frameList["DATATYPE"].Length +
                                frameList["RESERVED1"].Length + frameList["BLOCKCOUNT"].Length +
                                frameList["VARIALBLE_LENGTH"].Length + frameList["DEVICE"].Length +
                                frameList["DEVICEADDRESS"].Length + frameList["DATACOUNT"].Length +
                                frameList["DATABLOCK"].Length;
                frameList["LENGTH"] = new byte[2] { (byte)dataLength, (byte)(dataLength >> 8) };
                int count = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    count = count + frame.Length;
                }

                byte[] sendFrame = new byte[count];
                count = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    foreach (byte b in frame)
                    {
                        sendFrame[count] = b;
                        count++;
                    }
                }
                plcSocket.Send(sendFrame, 0, sendFrame.Length, SocketFlags.None);
                Thread.Sleep(20);
                byte[] buffer = new byte[32000];

                int recvCount = plcSocket.Receive(buffer);
                Thread.Sleep(20);
            }
            return 0;
        }

        private static short mdDevSet(short sAddress, enumDeviceType DeviceType)
        {
            lock (socketlockObject)
            {
                Dictionary<string, byte[]> frameList = new Dictionary<string, byte[]>();
                frameList.Add("COMPANYID", new byte[10] { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T', 0x00, 0x00 });
                frameList.Add("PLCINFO", new byte[2] { 0x00, 0x00 });
                frameList.Add("CPUINFO", new byte[1] { 0xA0 });
                frameList.Add("SOURCEFRAME", new byte[1] { 0x33 });
                frameList.Add("INVOKEID", new byte[2] { 0x20, 0x01 });
                frameList.Add("LENGTH", new byte[2] { 0x10, 0x00 });
                frameList.Add("POSITION", new byte[1] { 0x00 });
                int checksum = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    foreach (byte b in frame)
                    {
                        checksum = checksum + b;
                    }
                }
                string strMemAddress = sAddress.ToString();
                frameList.Add("CHECKSUM", new byte[1] { (byte)checksum });
                frameList.Add("COMMAND", new byte[2] { 0x58, 0x00 });
                frameList.Add("DATATYPE", new byte[2] { 0x00, 0x00 });      //연속읽기
                frameList.Add("RESERVED1", new byte[2] { 0x00, 0x00 });
                frameList.Add("BLOCKCOUNT", new byte[2] { 0x01, 0x00 });
                frameList.Add("VARIALBLE_LENGTH", new byte[2] { (byte)(0x03 + strMemAddress.Length), 0x00 });
                frameList.Add("DEVICE", new byte[3] { (byte)'%', (byte)'M', (byte)'X' });//%MB0

                byte[] bAddressTemp = new byte[strMemAddress.Length];
                for (int x = 0; x < bAddressTemp.Length; x++)
                {
                    bAddressTemp[x] = (byte)strMemAddress.ToCharArray()[x];
                }
                frameList.Add("DEVICEADDRESS", bAddressTemp);//%MB0
                string strTemp = 1.ToString("0###");
                int b2 = int.Parse(strTemp.Substring(0, 2));
                int b1 = int.Parse(strTemp.Substring(2, 2));
                frameList.Add("DATACOUNT", new byte[2] { (byte)b1, (byte)b2 }); //1400bytes 가능
                byte[] dataBlock = new byte[1] { 0x01 };
                frameList.Add("DATABLOCK", dataBlock); //1400bytes 가능

                int dataLength = frameList["COMMAND"].Length + frameList["DATATYPE"].Length +
                                frameList["RESERVED1"].Length + frameList["BLOCKCOUNT"].Length +
                                frameList["VARIALBLE_LENGTH"].Length + frameList["DEVICE"].Length +
                                frameList["DEVICEADDRESS"].Length + frameList["DATACOUNT"].Length +
                                frameList["DATABLOCK"].Length;
                frameList["LENGTH"] = new byte[2] { (byte)dataLength, (byte)(dataLength >> 8) };
                int count = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    count = count + frame.Length;
                }

                byte[] sendFrame = new byte[count];
                count = 0;
                foreach (byte[] frame in frameList.Values)
                {
                    foreach (byte b in frame)
                    {
                        sendFrame[count] = b;
                        count++;
                    }
                }
                plcSocket.Send(sendFrame, 0, sendFrame.Length, SocketFlags.None);
                Thread.Sleep(20);
                byte[] buffer = new byte[32000];

                int recvCount = plcSocket.Receive(buffer);
                Thread.Sleep(20);
            }
            return 0;
        }

        private static short mdSend(int iPath, short sStNo, short sDevTyp, short sDevNo, ref short sSize, [In] short[] asData)
        {
            lock (socketlockObject)
            {
                try
                {
                    int retryCount = 0;
                    while (true)
                    {
                        if (plcSocket.Connected == true)
                        {
                            break;
                        }
                        retryCount++;
                        if (retryCount > 3)
                        {
                            return -2;
                        }
                        Thread.Sleep(100);
                    }

                    if (sDevTyp == 23)//B
                    {
                        return DirectWriteToPLC((byte)'M', sDevNo, ref sSize, asData);
                    }
                    else if (sDevTyp == 22) //R
                    {
                        return DirectWriteToPLC((byte)'R', sDevNo, ref sSize, asData);
                    }
                    else if (sDevTyp == 24) //W
                    {
                        return DirectWriteToPLC((byte)'D', sDevNo, ref sSize, asData);
                    }

                }
                catch (Exception ex)
                {
                    System.Console.WriteLine(ex.StackTrace);
                    Log.log(1, "mdSend", "error", Category.ERROR, "", "mdSend error", ex.StackTrace.ToString());

                    return -2;
                }
            }
            return 0;

        }
        private static short mdReceive(int iPath, short sStNo, short sDevTyp, short sDevNo, ref short sSize, [Out] short[] asData)
        {
            lock (socketlockObject)
            {
                try
                {
                    int retryCount = 0;
                    while (true)
                    {
                        if (plcSocket.Connected == true)
                        {
                            break;
                        }
                        retryCount++;
                        if (retryCount > 3)
                        {
                            return -2;
                        }
                        Thread.Sleep(100);
                    }

                    if (sDevTyp == 23)//B
                    {
                        return DirectReadFromPLC((byte)'M', sDevNo, ref sSize, asData);
                    }
                    else if (sDevTyp == 22) //R
                    {
                        return DirectReadFromPLC((byte)'R', sDevNo, ref sSize, asData);
                    }
                    else if (sDevTyp == 24) //W
                    {
                        return DirectReadFromPLC((byte)'D', sDevNo, ref sSize, asData);
                    }

                }
                catch (Exception ex)
                {
                    System.Console.WriteLine(ex.StackTrace);
                    Log.log(1, "mdReceive", "error", Category.ERROR, "", "mdReceive error", ex.StackTrace.ToString());

                    return -2;
                }
            }
            return 0;

        }

        private static short DirectWriteToPLC(byte deviceType, short deviceAddress, ref short sSize, [Out] short[] asData)
        {

            lock (socketlockObject)
            {
                try
                {

                    Dictionary<string, byte[]> frameList = new Dictionary<string, byte[]>();
                    frameList.Add("COMPANYID", new byte[10] { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T', 0x00, 0x00 });
                    frameList.Add("PLCINFO", new byte[2] { 0x00, 0x00 });
                    frameList.Add("CPUINFO", new byte[1] { 0xA0 });
                    frameList.Add("SOURCEFRAME", new byte[1] { 0x33 });
                    frameList.Add("INVOKEID", new byte[2] { 0x30, 0x01 });
                    frameList.Add("LENGTH", new byte[2] { 0x10, 0x00 });
                    frameList.Add("POSITION", new byte[1] { 0x00 });
                    int checksum = 0;
                    foreach (byte[] frame in frameList.Values)
                    {
                        foreach (byte b in frame)
                        {
                            checksum = checksum + b;
                        }
                    }
                    string strMemAddress = (deviceAddress * 2).ToString();
                    frameList.Add("CHECKSUM", new byte[1] { (byte)checksum });
                    frameList.Add("COMMAND", new byte[2] { 0x58, 0x00 });
                    frameList.Add("DATATYPE", new byte[2] { 0x14, 0x00 });      //연속읽기
                    frameList.Add("RESERVED1", new byte[2] { 0x00, 0x00 });
                    frameList.Add("BLOCKCOUNT", new byte[2] { 0x01, 0x00 });
                    frameList.Add("VARIALBLE_LENGTH", new byte[2] { (byte)(0x03 + strMemAddress.Length), 0x00 });
                    if (deviceType == 77)   //M
                    {
                        frameList.Add("DEVICE", new byte[3] { (byte)'%', deviceType, (byte)'B' });//%MB0
                    }
                    else
                    {
                        frameList.Add("DEVICE", new byte[3] { (byte)'%', deviceType, (byte)'B' });//%MB0
                    }

                    byte[] bAddressTemp = new byte[strMemAddress.Length];
                    for (int x = 0; x < bAddressTemp.Length; x++)
                    {
                        bAddressTemp[x] = (byte)strMemAddress.ToCharArray()[x];
                    }
                    frameList.Add("DEVICEADDRESS", bAddressTemp);//%MB0
                    string strTemp = (asData.Length * 2).ToString("0###");
                    int b2 = int.Parse(strTemp.Substring(0, 2));
                    int b1 = int.Parse(strTemp.Substring(2, 2));
                    frameList.Add("DATACOUNT", new byte[2] { (byte)b1, (byte)b2 }); //1400bytes 가능
                    byte[] dataBlock = new byte[asData.Length * 2];
                    frameList.Add("DATABLOCK", dataBlock); //1400bytes 가능

                    int dataLength = frameList["COMMAND"].Length + frameList["DATATYPE"].Length +
                                    frameList["RESERVED1"].Length + frameList["BLOCKCOUNT"].Length +
                                    frameList["VARIALBLE_LENGTH"].Length + frameList["DEVICE"].Length +
                                    frameList["DEVICEADDRESS"].Length + frameList["DATACOUNT"].Length +
                                    frameList["DATABLOCK"].Length;
                    frameList["LENGTH"] = new byte[2] { (byte)dataLength, (byte)(dataLength >> 8) };

                    for (int i = 0; i < asData.Length; i++)
                    {
                        dataBlock[i * 2 + 1] = (byte)(asData[i] >> 8);
                        dataBlock[i * 2] = (byte)(asData[i]);
                    }
                    int count = 0;
                    foreach (byte[] frame in frameList.Values)
                    {
                        count = count + frame.Length;
                    }

                    byte[] sendFrame = new byte[count];
                    count = 0;
                    foreach (byte[] frame in frameList.Values)
                    {
                        foreach (byte b in frame)
                        {
                            sendFrame[count] = b;
                            count++;
                        }
                    }
                    plcSocket.Send(sendFrame, 0, sendFrame.Length, SocketFlags.None);
                    Thread.Sleep(20);
                    byte[] buffer = new byte[32000];

                    int recvCount = plcSocket.Receive(buffer);

                    Thread.Sleep(20);

                }
                catch (Exception ex)
                {
                    System.Console.WriteLine(ex.StackTrace);
                    Log.log(1, "PLC Write", "error", Category.ERROR, "", "PLC Write error", ex.StackTrace.ToString());
                    return 0;
                }
                return 0;
            }
        }


        private static short DirectReadFromPLC(byte deviceType, short deviceAddress, ref short sSize, [Out] short[] asData)
        {
            lock (socketlockObject)
            {
                int index = 0;
                byte countBuffer = 1;
                try
                {
                    for (int memAddress = 0; memAddress < sSize; memAddress = memAddress + 500)
                    {
                        countBuffer++;
                        Dictionary<string, byte[]> frameList = new Dictionary<string, byte[]>();
                        frameList.Add("COMPANYID", new byte[10] { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T', 0x00, 0x00 });
                        frameList.Add("PLCINFO", new byte[2] { 0x00, 0x00 });
                        frameList.Add("CPUINFO", new byte[1] { 0xA0 });
                        frameList.Add("SOURCEFRAME", new byte[1] { 0x33 });
                        frameList.Add("INVOKEID", new byte[2] { 0x40, countBuffer });
                        frameList.Add("LENGTH", new byte[2] { 0x10, 0x00 });
                        frameList.Add("POSITION", new byte[1] { 0x00 });
                        int checksum = 0;
                        foreach (byte[] frame in frameList.Values)
                        {
                            foreach (byte b in frame)
                            {
                                checksum = checksum + b;
                            }
                        }
                        string strMemAddress = (memAddress + deviceAddress).ToString();
                        frameList.Add("CHECKSUM", new byte[1] { (byte)checksum });
                        frameList.Add("COMMAND", new byte[2] { 0x54, 0x00 });
                        frameList.Add("DATATYPE", new byte[2] { 0x14, 0x00 });      //연속읽기
                        frameList.Add("RESERVED1", new byte[2] { 0x00, 0x00 });
                        frameList.Add("BLOCKCOUNT", new byte[2] { 0x01, 0x00 });
                        frameList.Add("VARIALBLE_LENGTH", new byte[2] { (byte)(0x03 + strMemAddress.Length), 0x00 });
                        if (deviceType == 77)   //M
                        {
                            frameList.Add("DEVICE", new byte[3] { (byte)'%', deviceType, (byte)'B' });//%MB0
                        }
                        else
                        {
                            frameList.Add("DEVICE", new byte[3] { (byte)'%', deviceType, (byte)'B' });//%MB0
                        }

                        byte[] bAddressTemp = new byte[strMemAddress.Length];
                        for (int x = 0; x < bAddressTemp.Length; x++)
                        {
                            bAddressTemp[x] = (byte)strMemAddress.ToCharArray()[x];
                        }
                        frameList.Add("DEVICEADDRESS", bAddressTemp);//%MB0
                        int readCount = 0;
                        if (memAddress + 500 > sSize)
                        {
                            readCount = sSize;
                        }
                        else
                        {
                            readCount = 500;
                        }
                        //string strTemp = sSize.ToString("0###");
                        string strTemp = readCount.ToString("0###");
                        int b2 = int.Parse(strTemp.Substring(0, 2));
                        int b1 = int.Parse(strTemp.Substring(2, 2));
                        frameList.Add("DATACOUNT", new byte[2] { (byte)b1, (byte)b2 }); //1400bytes 가능

                        int dataLength = frameList["COMMAND"].Length + frameList["DATATYPE"].Length +
                                        frameList["RESERVED1"].Length + frameList["BLOCKCOUNT"].Length +
                                        frameList["VARIALBLE_LENGTH"].Length + frameList["DEVICE"].Length +
                                        frameList["DEVICEADDRESS"].Length + frameList["DATACOUNT"].Length;

                        frameList["LENGTH"] = new byte[2] { (byte)dataLength, (byte)(dataLength >> 8) };

                        int count = 0;
                        foreach (byte[] frame in frameList.Values)
                        {
                            count = count + frame.Length;
                        }

                        byte[] sendFrame = new byte[count];
                        count = 0;
                        foreach (byte[] frame in frameList.Values)
                        {
                            foreach (byte b in frame)
                            {
                                sendFrame[count] = b;
                                count++;
                            }
                        }
                        plcSocket.Send(sendFrame, 0, sendFrame.Length, SocketFlags.None);
                        //Log.log(1, "PLC Read", "error", Category.ERROR, "", "PLC Read error", ex.StackTrace.ToString());
                        Thread.Sleep(50);
                        byte[] buffer = new byte[32000];

                        int recvCount = plcSocket.Receive(buffer);
                        int RECV_HEADER = 32;
                        for (int i = RECV_HEADER; i < readCount + RECV_HEADER; i = i + 2)
                        {
                            int temp = buffer[i] | buffer[i + 1] << 8; //확인해야 할 부분 
                            asData[index] = (short)temp;
                            index++;
                            if (index >= asData.Length)
                            {
                                break;
                            }
                        }
                        Thread.Sleep(20);
                    }
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine(ex.StackTrace);
                    Log.log(1, "PLC Read", "error", Category.ERROR, "", "PLC Read error", ex.StackTrace.ToString());

                    //plcSocket = null;
                    //plcSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    //IPAddress[] IPs = Dns.GetHostAddresses(GlobalConstant.PLC_IP);
                    //plcSocket.Blocking = false;
                    //plcSocket.Connect(IPs[0], GlobalConstant.PLC_PORT);
                    return 0;
                }
                return 0;
            }
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <returns></returns>
        public bool SetBit(short sAddress, enumDeviceType DeviceType)
        {
            return SetBit(sAddress, m_sStationNo, DeviceType);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <returns></returns>
        public bool SetBit(short sAddress, short sStationNo, enumDeviceType DeviceType)
        {
            if (!this.Enabled)
                return false;

            int iRet;
            //iRet = mdDevSet(m_iPath, sStationNo, (short)DeviceType, sAddress);
            iRet = mdDevSet(sAddress, DeviceType);
            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <returns></returns>
        public bool ResetBit(short sAddress, enumDeviceType DeviceType)
        {
            return ResetBit(sAddress, m_sStationNo, DeviceType);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <returns></returns>
        public bool ResetBit(short sAddress, short sStationNo, enumDeviceType DeviceType)
        {
            if (!this.Enabled)
                return false;

            int iRet;
            //iRet = mdDevRst(m_iPath, sStationNo, (short)DeviceType, sAddress);
            iRet = mdDevRst(sAddress, DeviceType);
            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="DeviceType"></param>
        /// <param name="bValue"></param>
        /// <returns></returns>
        public bool ReadBit(short sAddress, int nIndex, enumDeviceType DeviceType, out bool bValue)
        {
            return ReadBit(sAddress, nIndex, m_sStationNo, DeviceType, out bValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="bValue"></param>
        /// <returns></returns>
        public bool ReadBit(short sAddress, int nIndex, short sStationNo, enumDeviceType DeviceType, out bool bValue)
        {
            bValue = false;
            if (!this.Enabled)
                return false;

            int iRet;
            bool bRet = false;
            short[] asData = new short[1];
            short nSize = 2;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        short nStartAddress = (short)((sAddress / 0x10) * 0x10);


                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            bValue = false;
                            bRet = false;
                        }
                        else
                        {
                            bValue = ((asData[0] & (1 << (sAddress - nStartAddress))) != 0);
                            bRet = true;
                        }
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            bValue = false;
                            bRet = false;
                        }
                        else
                        {
                            bValue = ((asData[0] & (1 << nIndex)) != 0);
                            bRet = true;
                        }
                    }
                    break;
            }
            return bRet;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="DeviceType"></param>
        /// <param name="bValue"></param>
        /// <returns></returns>
        public bool WriteBit(short sAddress, int nIndex, enumDeviceType DeviceType, bool bValue)
        {
            return WriteBit(sAddress, nIndex, m_sStationNo, DeviceType, bValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="bValue"></param>
        /// <returns></returns>
        public bool WriteBit(short sAddress, int nIndex, short sStationNo, enumDeviceType DeviceType, bool bValue)
        {
            if (!this.Enabled)
                return false;

            int iRet = -1;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        if (bValue)
                        {
                            //iRet = mdDevSet(m_iPath, sStationNo, (short)DeviceType, sAddress);
                            iRet = mdDevSet(sAddress, DeviceType);
                        }
                        else
                        {
                            //iRet = mdDevRst(m_iPath, sStationNo, (short)DeviceType, sAddress);
                            iRet = mdDevRst(sAddress, DeviceType);
                        }
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        short[] asData = new short[1];
                        short nSize = 2;
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                        }
                        if (IsSuccess(iRet))
                        {
                            if (bValue)
                                asData[0] = (short)((ushort)asData[0] | (1 << nIndex));
                            else
                                asData[0] = (short)(asData[0] & ~(1 << nIndex));

                            iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        }
                    }
                    break;
            }

            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, enumDeviceType DeviceType, out short sValue)
        {
            return ReadWord(sAddress, m_sStationNo, DeviceType, out sValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, short sStationNo, enumDeviceType DeviceType, out short sValue)
        {
            sValue = 0;
            if (!this.Enabled)
                return false;

            int iRet;
            //bool bRet;
            short[] asData = new short[1];
            short nSize = 2;
            int retryCount = 0;
        jmp:
            iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
            if (iRet != 0)
            {
                retryCount++;
                if (retryCount < 3)
                {
                    goto jmp;
                }
                System.Console.WriteLine("PLC Error");
                Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");

            }
            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, enumDeviceType DeviceType, short sSize, out short[] asValue)
        {
            return ReadWord(sAddress, m_sStationNo, DeviceType, sSize, out asValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sSize"></param>
        /// <param name="asValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, short sStationNo, enumDeviceType DeviceType, short sSize, out short[] asValue)
        {
            asValue = new short[sSize];
            if (!this.Enabled)
                return false;

            int iRet;
            //bool bRet;

            short nSize = (short)(sSize * 2);
            int retryCount = 0;
        jmp:
            iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asValue);
            if (iRet != 0)
            {
                retryCount++;
                if (retryCount < 3)
                {
                    goto jmp;
                }
                System.Console.WriteLine("PLC Error");
                Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");
            }
            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="nLength"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, enumDeviceType DeviceType, int nLength, out short sValue)
        {
            return ReadWord(sAddress, m_sStationNo, DeviceType, nLength, out sValue);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="nLength"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, short sStationNo, enumDeviceType DeviceType, int nLength, out short sValue)
        {
            sValue = 0;
            if (!this.Enabled)
                return false;

            int iRet;
            bool bRet = false;
            short[] asData = new short[1];
            short nSize = 2;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        short nStartAddress = (short)((sAddress / 0x10) * 0x10);
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            sValue = 0;
                            bRet = false;
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            int iTemp = asData[0] & (iMask << (sAddress - nStartAddress));
                            sValue = (short)(iTemp >> (sAddress - nStartAddress));
                            bRet = true;

                            ////ushort usMask = 0;
                            ////for (int i = 0; i < nLength; i++)
                            ////{
                            ////    usMask |= (ushort)(1 << i);
                            ////}
                            ////sValue = (short)(asData[0] & (usMask << (sAddress - nStartAddress)));
                            ////sValue = (short)(sValue >> (sAddress - nStartAddress));
                            ////bRet = true;
                        }
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            sValue = 0;
                            bRet = false;
                        }
                        else
                        {
                            sValue = asData[0];
                            bRet = true;
                        }
                    }
                    break;
            }

            return bRet;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="nIndex"></param>
        /// <param name="nLength"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, enumDeviceType DeviceType, int nIndex, int nLength, out short sValue)
        {
            return ReadWord(sAddress, m_sStationNo, DeviceType, nIndex, nLength, out sValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="nIndex"></param>
        /// <param name="nLength"></param>
        /// <param name="sValue"></param>
        /// <returns></returns>
        public bool ReadWord(short sAddress, short sStationNo, enumDeviceType DeviceType, int nIndex, int nLength, out short sValue)
        {
            sValue = 0;
            if (!this.Enabled)
                return false;

            int iRet;
            bool bRet = false;
            short[] asData = new short[1];
            short nSize = 2;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        short nStartAddress = (short)((sAddress / 0x10) * 0x10);
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            sValue = 0;
                            bRet = false;
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            int iTemp = asData[0] & (iMask << (sAddress - nStartAddress));
                            sValue = (short)(iTemp >> (sAddress - nStartAddress));
                            bRet = true;

                            ////ushort usMask = 0;
                            ////for (int i = 0; i < nLength; i++)
                            ////{
                            ////    usMask |= (ushort)(1 << i);
                            ////}
                            ////sValue = (short)(asData[0] & (usMask << (sAddress - nStartAddress)));
                            ////sValue = (short)(sValue >> (sAddress - nStartAddress));
                            ////bRet = true;
                        }
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "ReadWord", "error", Category.ERROR, "", "ReadWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                            sValue = 0;
                            bRet = false;
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            int iTemp = asData[0] & (iMask << nIndex);
                            sValue = (short)(iTemp >> nIndex);
                            bRet = true;
                            ////ushort usMask = 0;
                            ////for (int i = 0; i < nLength; i++)
                            ////{
                            ////    usMask |= (ushort)(1 << i);
                            ////}
                            ////sValue = (short)(asData[0] & (usMask << nIndex));
                            ////sValue = (short)(sValue >> nIndex);
                            ////bRet = true;
                        }
                    }
                    break;
            }

            return bRet;
        }



        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, enumDeviceType DeviceType, short sData)
        {
            return this.WriteWord(sAddress, m_sStationNo, DeviceType, sData);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, short sStationNo, enumDeviceType DeviceType, short sData)
        {
            if (!this.Enabled)
                return false;

            int iRet;
            short[] asData = new short[1];
            asData[0] = sData;
            short nSize = 2;
            iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
            return IsSuccess(iRet);
            ;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nLength"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, int nLength, enumDeviceType DeviceType, short sData)
        {
            return this.WriteWord(sAddress, nLength, m_sStationNo, DeviceType, sData);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nLength"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, int nLength, short sStationNo, enumDeviceType DeviceType, short sData)
        {
            if (!this.Enabled)
                return false;
            short nSize = 2;
            int iRet = 0;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        short[] asData = new short[1];
                        asData[0] = 0;
                        short nStartAddress = (short)((sAddress / 0x10) * 0x10);
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, m_sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "WriteWord", "error", Category.ERROR, "", "WriteWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            iMask = iMask << (sAddress - nStartAddress);

                            int iData = sData << (sAddress - nStartAddress);
                            int iValue = asData[0] & ~iMask;
                            iValue = iValue | (iData & iMask);
                            asData[0] = (short)iValue;

                            iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                            ////ushort usMask = 0;
                            ////for (int i = 0; i < nLength; i++)
                            ////{
                            ////    usMask |= (ushort)(1 << i);
                            ////}
                            ////usMask = (ushort)(usMask << (sAddress - nStartAddress));
                            ////sData = (short)(sData << (sAddress - nStartAddress));
                            ////short sValue = (short)(asData[0] & ~usMask);
                            ////sValue = (short)(sValue | (short)(sData & usMask));
                            ////asData[0] = sValue;
                            ////iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        }
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        short[] asData = new short[1];
                        asData[0] = sData;
                        iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                    }
                    break;
            }

            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="nLength"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, int nIndex, int nLength, enumDeviceType DeviceType, short sData)
        {
            return this.WriteWord(sAddress, nIndex, nLength, m_sStationNo, DeviceType, sData);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="nIndex"></param>
        /// <param name="nLength"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sData"></param>
        /// <returns></returns>
        public bool WriteWord(short sAddress, int nIndex, int nLength, short sStationNo, enumDeviceType DeviceType, short sData)
        {
            if (!this.Enabled)
                return false;
            short nSize = 2;
            int iRet = 0;

            switch (DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    {
                        short[] asData = new short[1];
                        asData[0] = 0;
                        short nStartAddress = (short)((sAddress / 0x10) * 0x10);

                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, m_sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "WriteWord", "error", Category.ERROR, "", "WriteWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            iMask = iMask << (sAddress - nStartAddress);

                            int iData = sData << (sAddress - nStartAddress);
                            int iValue = asData[0] & ~iMask;
                            iValue = iValue | (iData & iMask);

                            asData[0] = (short)iValue;
                            iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        }
                        ////short[] asData = new short[1];
                        ////asData[0] = 0;
                        ////short nStartAddress = (short)((sAddress / 0x10) * 0x10);
                        ////iRet = mdReceive(m_iPath, m_sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        ////if (!IsSuccess(iRet))
                        ////{
                        ////}
                        ////else
                        ////{
                        ////    ushort usMask = 0;
                        ////    for (int i = 0; i < nLength; i++)
                        ////    {
                        ////        usMask |= (ushort)(1 << i);
                        ////    }
                        ////    usMask = (ushort)(usMask << (sAddress - nStartAddress));
                        ////    sData = (short)(sData << (sAddress - nStartAddress));
                        ////    short sValue = (short)(asData[0] & ~usMask);
                        ////    sValue = (short)(sValue | (short)(sData & usMask));
                        ////    asData[0] = sValue;
                        ////    iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, nStartAddress, ref nSize, asData);
                        ////}
                    }
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WW:
                case enumDeviceType.WR:
                case enumDeviceType.R:
                    {
                        short[] asData = new short[1];
                        asData[0] = sData;
                        int retryCount = 0;
                    jmp:
                        iRet = mdReceive(m_iPath, m_sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        if (iRet != 0)
                        {
                            retryCount++;
                            if (retryCount < 3)
                            {
                                goto jmp;
                            }
                            System.Console.WriteLine("PLC Error");
                            Log.log(1, "WriteWord", "error", Category.ERROR, "", "WriteWord error", "PLC Error");
                        }
                        if (!IsSuccess(iRet))
                        {
                        }
                        else
                        {
                            int iMask = 0;
                            for (int i = 0; i < nLength; i++)
                            {
                                iMask |= 1 << i;
                            }
                            iMask = iMask << nIndex;

                            int iData = sData << nIndex;
                            int iValue = asData[0] & ~iMask;
                            iValue = iValue | (iData & iMask);

                            asData[0] = (short)iValue;
                            iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        }
                        ////short[] asData = new short[1];
                        ////asData[0] = sData;
                        ////iRet = mdReceive(m_iPath, m_sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        ////if (!IsSuccess(iRet))
                        ////{
                        ////}
                        ////else
                        ////{
                        ////    ushort usMask = 0;
                        ////    for (int i = 0; i < nLength; i++)
                        ////    {
                        ////        usMask |= (ushort)(1 << i);
                        ////    }
                        ////    usMask = (ushort)(usMask << nIndex);
                        ////    sData = (short)(sData << nIndex);
                        ////    short sValue = (short)(asData[0] & ~usMask);
                        ////    sValue = (short)(sValue | (short)(sData & usMask));
                        ////    asData[0] = sValue;
                        ////    iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref nSize, asData);
                        ////}
                    }
                    break;
            }

            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sLength"></param>
        /// <param name="strData"></param>
        /// <returns></returns>
        public bool WriteWordA(short sAddress, enumDeviceType DeviceType, short sLength, string strData)
        {
            return WriteWordA(sAddress, m_sStationNo, DeviceType, sLength, strData);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sLength"></param>
        /// <param name="strData"></param>
        /// <returns></returns>
        public bool WriteWordA(short sAddress, short sStationNo, enumDeviceType DeviceType, short sLength, string strData)
        {
            if (!this.Enabled)
                return false;

            short sMemoryLength = (short)sLength;
            // WordLength 보다 큰 Data의 입력시 Length만큼 Data를 잘라냄
            if (strData.Length > sLength * 2)
                strData = strData.Substring(0, sLength * 2);

            strData = strData.PadRight(sMemoryLength * 2, (char)0);

            short[] asData = new short[sMemoryLength];
            for (int i = 0; i < sMemoryLength; i++)
            {
                asData[i] = 0;
                asData[i] = Convert.ToInt16(strData[i * 2]);
                asData[i] = Convert.ToInt16((ushort)asData[i] | (Convert.ToInt16(strData[i * 2 + 1]) << 8));
            }

            int iRet;

            if (this.Connected)
            {

            }

            if (DeviceType == enumDeviceType.R)
            {
                sMemoryLength *= 2;
                iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref sMemoryLength, asData);
            }
            else
            {
                sMemoryLength *= 2;
                iRet = mdSend(m_iPath, sStationNo, (short)DeviceType, sAddress, ref sMemoryLength, asData);
            }


            return IsSuccess(iRet);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sLength"></param>
        /// <param name="strValue"></param>
        /// <returns></returns>
        public bool ReadWordA(short sAddress, enumDeviceType DeviceType, short sLength, out string strValue)
        {
            return ReadWordA(sAddress, m_sStationNo, DeviceType, sLength, out strValue);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sAddress"></param>
        /// <param name="sStationNo"></param>
        /// <param name="DeviceType"></param>
        /// <param name="sLength"></param>
        /// <param name="strValue"></param>
        /// <returns></returns>
        public bool ReadWordA(short sAddress, short sStationNo, enumDeviceType DeviceType, short sLength, out string strValue)
        {
            strValue = "";
            if (!this.Enabled)
                return false;

            short sMemoryLength = (short)sLength;
            int iRet;
            bool bRet;
            short[] asData = new short[sMemoryLength];
            sMemoryLength *= 2;
            int retryCount = 0;
        jmp:
            iRet = mdReceive(m_iPath, sStationNo, (short)DeviceType, sAddress, ref sMemoryLength, asData);
            if (iRet != 0)
            {
                retryCount++;
                if (retryCount < 3)
                {
                    goto jmp;
                }
                System.Console.WriteLine("PLC Error");
                Log.log(1, "ReadWordA", "error", Category.ERROR, "", "ReadWordA error", "PLC Error");
            }
            if (!IsSuccess(iRet))
            {
                strValue = "";
                return false;
            }
            else
            {
                for (int i = 0; i < sMemoryLength / 2; i++)
                {
                    strValue = strValue + Convert.ToChar(asData[i] & 0xFF);
                    strValue = strValue + Convert.ToChar((asData[i] >> 8) & 0xFF);
                }
                bRet = true;
            }

            strValue = strValue.Trim();
            if (strValue.Length > sLength * 2)
            {
                strValue = strValue.Substring(0, sLength * 2);
            }
            strValue = strValue.Trim(" \0".ToCharArray());

            return bRet;
        }
        #endregion

        #region Interfaces

        protected override void InitConnectionString(string sCnnString)
        {
            this.connectionInfo = (DriverConnectionString)new FenetConnection();
            this.connectionInfo.Parse(sCnnString);

            UpdateConnectionInfo();
        }

        private void UpdateConnectionInfo()
        {
            m_sChannelNo = (short)this.connectionInfo["CHANNEL_NO", 51];
            m_sMode = (short)this.connectionInfo["MODE", -1];
            m_sStationNo = (short)this.connectionInfo["STATION_NO", 255];
            m_nMonitorInterval = this.connectionInfo["MONITOR_INTERVAL", 100];

            m_MonitorMemory.Clear();
            string[] DeviceTypeNames = Enum.GetNames(typeof(enumDeviceType));
            for (int i = 0; i < DeviceTypeNames.Length; i++)
            {
                if (this.connectionInfo.ContainsKey(string.Format("{0}_START_ADDR", DeviceTypeNames[i])) &&
                    this.connectionInfo.ContainsKey(string.Format("{0}_END_ADDR", DeviceTypeNames[i])))
                {
                    m_MonitorMemory.Add(DeviceTypeNames[i],
                                        new FenetMemory((enumDeviceType)Enum.Parse(typeof(enumDeviceType), DeviceTypeNames[i]),
                                        (short)this.connectionInfo[string.Format("{0}_START_ADDR", DeviceTypeNames[i]), 0],
                                        (short)this.connectionInfo[string.Format("{0}_END_ADDR", DeviceTypeNames[i]), 0]));
                }
            }
        }

        private bool CheckConnection()
        {
            lock (socketlockObject)
            {
                short sSize = 2;
                short[] asData = new short[1];
                int retryCount = 0;
            jmp:
                short sRet = mdReceive(m_iPath, m_sStationNo, (short)enumDeviceType.B, 0, ref sSize, asData);
                if (sRet != 0)
                {
                    retryCount++;
                    if (retryCount < 3)
                    {
                        goto jmp;
                    }
                    System.Console.WriteLine("PLC Error");
                    Log.log(1, "CheckConnection", "error", Category.ERROR, "", "CheckConnection error", "PLC Error");
                }
                return (sRet == 0) || (sRet == 66) || (sRet == -28150);
            }
        }

        public override int Open()
        {
            if (this.Connected)
                Close();

            if (this.connectionInfo != null)
            {
                UpdateConnectionInfo();
            }

            short sRet;
            sRet = mdOpen(m_sChannelNo, m_sMode, ref m_iPath);
            // 실제 보드가 장착된 PC에서 되는지 확인할것.

            if ((sRet == 0 || sRet == 66) && CheckConnection())
            {
                this.ConnectionState = FBConnectionState.CONNECTED;
                this.InitMonitorThread();
            }
            else
            {
                //mdClose(m_iPath);
                return -1;
            }

            return 0;
        }


        public override int Open(string strConnectionString)
        {
            this.connectionInfo = new FenetConnection();
            this.connectionInfo.Parse(strConnectionString);
            UpdateConnectionInfo();

            short sRet;
            sRet = mdOpen(m_sChannelNo, m_sMode, ref m_iPath);
            // 실제 보드가 장착된 PC에서 되는지 확인할것.

            if (sRet == 0 || sRet == 66)
            {
                if (CheckConnection())
                {
                    this.ConnectionState = FBConnectionState.CONNECTED;
                    this.InitMonitorThread();
                }
                else
                    return -1;
            }
            else
            {
                //mdClose(m_iPath);
                return sRet;
            }

            return 0;
        }


        public override int Close()
        {
            this.ConnectionState = FBConnectionState.DISCONNECTED;
            //mdClose(m_iPath);

            return 0;
        }

        public override int Write(Observable observable, object value)
        {
            if (!this.Connected)
                return 1;

            if (observable.ConnectionInfoString == string.Empty)
                return 1;

            if (observable.AccessType != AccessType.OUT && observable.AccessType != AccessType.IN_OUT)
                return 1;

            switch (observable.DataType.Value())
            {
                case DataType._BOOLEAN:
                    {
                        short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        if (sCnnInfoStationNo >= 0)
                        {
                            WriteBit((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                observable.ConnectionInfo["BIT_INDEX", 0],
                                sCnnInfoStationNo,
                                (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "B"]),
                                (bool)value);
                        }
                        else
                        {
                            WriteBit((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                observable.ConnectionInfo["BIT_INDEX", 0],
                                (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "B"]),
                                (bool)value);
                        }
                    }
                    break;
                case DataType._SHORT:
                    {
                        //////short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        //////if (sCnnInfoStationNo >= 0)
                        //////{
                        //////    WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                        //////        observable.ConnectionInfo["BIT_LENGTH", 0],
                        //////        sCnnInfoStationNo,
                        //////        (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                        //////        (short)value);
                        //////}
                        //////else
                        //////{
                        //////    WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                        //////       observable.ConnectionInfo["BIT_LENGTH", 0],
                        //////       (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                        //////       (short)value);
                        //////}



                        short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        if (sCnnInfoStationNo >= 0)
                        {
                            if (observable.ConnectionInfo.ContainsKey("BIT_INDEX"))
                            {
                                WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                    observable.ConnectionInfo["BIT_INDEX", 0],
                                    observable.ConnectionInfo["BIT_LENGTH", 0],
                                    sCnnInfoStationNo,
                                    (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                    (short)value);
                            }
                            else
                            {
                                WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                    observable.ConnectionInfo["BIT_LENGTH", 0],
                                    sCnnInfoStationNo,
                                    (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                    (short)value);
                            }
                        }
                        else
                        {
                            if (observable.ConnectionInfo.ContainsKey("BIT_INDEX"))
                            {
                                WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                   observable.ConnectionInfo["BIT_INDEX", 0],
                                   observable.ConnectionInfo["BIT_LENGTH", 0],
                                   (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                   (short)value);
                            }
                            else
                            {
                                WriteWord((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                   observable.ConnectionInfo["BIT_LENGTH", 0],
                                   (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                   (short)value);
                            }
                        }

                    }
                    break;
                case DataType._STRING:
                    {
                        short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        if (sCnnInfoStationNo >= 0)
                        {
                            WriteWordA((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                sCnnInfoStationNo,
                                (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                (short)observable.ConnectionInfo["LENGTH", 0],
                                (string)value);
                        }
                        else
                        {
                            string[] tem = observable.ConnectionInfoString.Split('=');
                            string flag = tem[1].Substring(0, 1);


                            WriteWordA((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                (short)observable.ConnectionInfo["LENGTH", 0],
                                (string)value);
                        }
                    }
                    break;
                case DataType._SHORT_ARRAY:
                    {
                        short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        short addressNo = (short)(observable.ConnectionInfo["ADDRESS_NO", 0, 16]);
                        if (sCnnInfoStationNo >= 0)
                        {
                            for (short i = 0; i < observable.ListSize; i++)
                            {
                                WriteWord((short)(addressNo + i),
                                    observable.ConnectionInfo["BIT_LENGTH", 0],
                                    sCnnInfoStationNo,
                                    (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                                    ((List<short>)value)[i]);
                            }
                        }
                    }
                    break;
                case DataType._BOOL_ARRAY:
                    {
                        enumDeviceType deviceType = (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "B"]);
                        short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        short sAddressNo = (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16];
                        short sBitIndex = (short)observable.ConnectionInfo["BIT_INDEX", 0];
                        List<bool> bList = (List<bool>)value;
                        for (int i = 0; i < observable.ListSize; i++)
                        {
                            if (sCnnInfoStationNo >= 0)
                            {
                                if (FenetMemory.GetMemoryType(deviceType) == enumMemoryType.Bit)
                                    WriteBit((short)(sAddressNo + i), 0, sCnnInfoStationNo, deviceType, bList[i]);
                                else
                                    WriteBit((short)(sAddressNo + (i + sBitIndex) / 16), (short)((i + sBitIndex) % 16), sCnnInfoStationNo, deviceType, bList[i]);
                            }
                            else
                            {
                                if (FenetMemory.GetMemoryType(deviceType) == enumMemoryType.Bit)
                                    WriteBit((short)(sAddressNo + i), 0, deviceType, bList[i]);
                                else
                                    WriteBit((short)(sAddressNo + (i + sBitIndex) / 16), (short)((i + sBitIndex) % 16), deviceType, bList[i]);
                            }
                        }
                    }
                    break;
                case DataType._BYTE:
                case DataType._INTEGER:
                case DataType._LONG:
                case DataType._DOUBLE:
                    break;
            }

            return 0;
        }

        public override int Read(Observable observable, out object value)
        {
            value = null;

            if (!this.Connected)
                return 1;

            if (observable.ConnectionInfoString == string.Empty)
                return 1;
            if (this.ConnectionState == FBConnectionState.DISCONNECTED)
                return 1;

            lock (m_Monitor)
            {
            }

            switch (observable.DataType.Value())
            {
                case DataType._BOOLEAN:
                    {
                        bool bValue;

                        if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "B"]].ReadBit(
                            (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                            (short)observable.ConnectionInfo["BIT_INDEX", 0],
                            out bValue))
                        {
                        }

                        value = bValue;
                    }
                    break;
                case DataType._SHORT:
                    {
                        //////short sValue;
                        //////if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "W"]].ReadWord(
                        //////    (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                        //////    observable.ConnectionInfo["BIT_LENGTH", 0],
                        //////    out sValue))
                        //////{
                        //////}
                        //////value = sValue;
                        short sValue;
                        if (observable.ConnectionInfo.ContainsKey("BIT_INDEX"))
                        {
                            if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "B"]].ReadWord(
                                (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                observable.ConnectionInfo["BIT_INDEX", 0],
                                observable.ConnectionInfo["BIT_LENGTH", 0],
                                out sValue))
                            {
                            }
                        }
                        else
                        {
                            if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "B"]].ReadWord(
                                (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                                observable.ConnectionInfo["BIT_LENGTH", 0],
                                out sValue))
                            {
                            }
                        }
                        value = sValue;
                    }
                    break;
                case DataType._STRING:
                    {
                        string sValue;

                        if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "W"]].ReadWordA(
                            (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                            (short)(observable.ConnectionInfo["LENGTH", 0] * 2),
                            out sValue))
                        {
                        }

                        value = sValue;
                        //string strValue;
                        //short sCnnInfoStationNo = Convert.ToInt16(observable.ConnectionInfo["STATION_NO", -1]);
                        //if (sCnnInfoStationNo >= 0)
                        //{
                        //    if (!ReadWordA((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                        //        sCnnInfoStationNo,
                        //        (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                        //        (short)observable.ConnectionInfo["LENGTH", 0],
                        //        out strValue))
                        //    {
                        //    }
                        //}
                        //else
                        //{

                        //    if (!ReadWordA((short)observable.ConnectionInfo["ADDRESS_NO", 0, 16],
                        //    (enumDeviceType)Enum.Parse(typeof(enumDeviceType), observable.ConnectionInfo["DEVICE_TYPE", "W"]),
                        //    (short)observable.ConnectionInfo["LENGTH", 0],
                        //    out strValue))
                        //    {
                        //    }

                        //}
                        //value = strValue;
                    }
                    break;
                case DataType._SHORT_ARRAY:
                    {
                        List<short> sListvalue = new List<short>(observable.ListSize);
                        short sValue = 0;

                        for (short i = 0; i < observable.ListSize; i++)
                        {
                            if (!m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "W"]].ReadWord(
                                (short)(observable.ConnectionInfo["ADDRESS_NO", 0, 16] + i),
                                1,
                                out sValue))
                            {
                            }
                            sListvalue.Add(sValue);

                        }
                        value = sListvalue;
                    }
                    break;
                case DataType._BOOL_ARRAY:
                    {
                        List<bool> bListvalue = new List<bool>(observable.ListSize);

                        FenetMemory mem = m_MonitorMemory[observable.ConnectionInfo["DEVICE_TYPE", "B"]];
                        short sAddressNo = (short)observable.ConnectionInfo["ADDRESS_NO", 0, 16];
                        short sBitIndex = (short)observable.ConnectionInfo["BIT_INDEX", 0];

                        bool bValue = false;
                        for (int i = 0; i < observable.ListSize; i++)
                        {
                            if (mem.MemoryType == enumMemoryType.Bit)
                            {
                                mem.ReadBit((short)(sAddressNo + i), 0, out bValue);
                            }
                            else
                            {
                                mem.ReadBit((short)(sAddressNo + (i + sBitIndex) / 16), (short)((i + sBitIndex) % 16), out bValue);
                            }
                            bListvalue.Add(bValue);
                        }
                        value = bListvalue;
                    }
                    break;
                case DataType._BYTE:
                case DataType._INTEGER:
                case DataType._LONG:
                case DataType._DOUBLE:
                    break;
            }

            return 0;
        }
        #endregion
    }
}
