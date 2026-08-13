using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ECP.Com.Tcp;
using HECS.Device;
using HECS.Device.Unit;
using HECS.Util;

namespace HECS
{
    public partial class FormTest : Form
    {
        Timer timerCommand = new Timer();
        Timer timerRefresh = new Timer();
        private Vehicle vehicle = null;
        private string currentBank = "01";
        private string currentColumn = "01";
        private string currentRow = "01";
        private bool sendTransferCommand = false;
        private List<string> commandList = new List<string>();
        protected readonly char NULL_CHAR = '@';
        protected readonly char START_CHAR = '^';
        protected readonly char END_CHAR = '#';

        TcpServer tcpServer = null;
        TcpClient tcpClient = null;

        private int tcpServerPort = 0;

        public int TcpServerPort
        {
            get { return tcpServerPort; }
            set { tcpServerPort = value; }
        }

        private int tcpClientPort = 0;

        public int TcpClientPort
        {
            get { return tcpClientPort; }
            set { tcpClientPort = value; }
        }

        private string tcpClientIp = "127.0.0.1";

        public string TcpClientIp
        {
            get { return tcpClientIp; }
            set { tcpClientIp = value; }
        }

        public FormTest()
        {
            InitializeComponent();
            vehicle = ECSDeviceManager.GetVehicle("VEHICLE:"+ textBoxVehicle.Text.Trim());
            timerCommand.Interval = 500;
            timerCommand.Enabled = true;
            timerCommand.Stop();
            timerCommand.Tick += new EventHandler(timerCommand_Tick);
            timerRefresh.Interval = 500;
            timerRefresh.Enabled = true;
            timerRefresh.Start();
            timerRefresh.Tick += new EventHandler(timerRefresh_Tick);
            for (int i = 0; i < 14; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    string path01 = "01" + (i + 1).ToString().PadLeft(2, '0') + (j + 1).ToString().PadLeft(2, '0');
                    string path02 = "02" + (i + 1).ToString().PadLeft(2, '0') + (j + 1).ToString().PadLeft(2, '0');
                    commandList.Add(path01);
                    commandList.Add(path02);
                }
            }
        }

        void timerRefresh_Tick(object sender, EventArgs e)
        {
            Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:" + textBoxVehicle.Text.Trim());
            if (vehicle == null)
            {
                return;
            }
            this.textTestBank03.Text = vehicle.TransferCompleteLocation01 ;
            this.textTestColumn03.Text = vehicle.TransferCompleteLocation02;
            this.textTestRow03.Text = vehicle.TransferCompleteLocation03;
            this.textTestPallet02.Text = vehicle.PalletIdOnVehicle;

            //textTestBank01.Text = vehicle.IO_FROM_01;
            //textTestColumn01.Text = vehicle.IO_FROM_02;
            //textTestRow01.Text = vehicle.IO_FROM_03;
        }

        void timerCommand_Tick(object sender, EventArgs e)
        {
            Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:" + textBoxVehicle.Text.Trim());
            if (vehicle == null)
            {
                return;
            }
            if (sendTransferCommand == true)
            {
                if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.RUN)
                {
                    sendTransferCommand = false;
                }
            if (sendTransferCommand == true)
            {
                return;
            }
         
            }
            if (vehicle.VehicleState == Vehicle.VEHICLE_STATE.IDLE)
            {
                for (int i = 0; i < commandList.Count; i++)
                {
                    if (commandList[i] == (currentBank + currentColumn + currentRow))
                    {
                        if (i + 1 == commandList.Count)
                        {
                            MessageBox.Show("테스트가 완료되었습니다.", "확인", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            timerCommand.Stop();
                            return;
                        }
                        vehicle.IO_FROM_01 = currentBank;
                        vehicle.IO_FROM_02 =currentColumn;
                        vehicle.IO_FROM_03 = currentRow;
                        currentBank = commandList[i + 1].Substring(0, 2);
                        currentColumn = commandList[i + 1].Substring(2, 2);
                        currentRow = commandList[i + 1].Substring(4, 2);
                        
                        vehicle.IO_TO_01 = currentBank;
                        vehicle.IO_TO_02 = currentColumn;
                        vehicle.IO_TO_03 = currentRow;
                        vehicle.IO_PALLET_ID = textTestPallet01.Text;
                        vehicle.IO_TRANSFER_REQUEST = true;
                        sendTransferCommand = true;
                        return;
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:" + textBoxVehicle.Text.Trim());
            if (vehicle == null)
            {
                return;
            }
            vehicle.IO_FROM_01 = textBoxFrom01.Text;
            vehicle.IO_FROM_02 = textBoxFrom02.Text;
            vehicle.IO_FROM_03 = textBoxFrom03.Text;
            vehicle.IO_TO_01 = textBoxTo01.Text;
            vehicle.IO_TO_02 = textBoxTo02.Text;
            vehicle.IO_TO_03 = textBoxTo03.Text;
            vehicle.IO_PALLET_ID = textBoxPalletIdVehicle.Text;
            vehicle.IO_TRANSFER_REQUEST = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Conveyor conveyor = ECSDeviceManager.GetConveyor(comboBoxConveyor.Text);
            if (conveyor.DirectionMode == Conveyor.CONVEYOR_DIRECTION_MODE.RGVOUTGO)
            {
                if (conveyor == null)
                {
                    return;
                }

                conveyor.IO_PALLET_EXIST01 = textBoxPalletID01.Text;

                conveyor.Observables["PALLET_EXIST_FLAG_01"].AsBoolean = true;
            }
            else
            {
                Util.Util.ShowMesagePopup("Direction Mode 확인");
                return;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Conveyor conveyor = ECSDeviceManager.GetConveyor(comboBoxConveyor.Text);
            if (conveyor.DirectionMode == Conveyor.CONVEYOR_DIRECTION_MODE.RGVINGO)
            {
                if (conveyor == null)
                {
                    return;
                }

                conveyor.IO_PALLET_EXIST02 = textBoxPalletID02.Text;

                conveyor.Observables["PALLET_EXIST_FLAG_02"].AsBoolean = true;
            }
            else
            {
                Util.Util.ShowMesagePopup("Direction Mode 확인");
                return;
            }
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            currentBank = textTestBank01.Text;
            currentColumn = textTestColumn01.Text;
            currentRow = textTestRow01.Text;
            timerCommand.Start();
            sendTransferCommand = false;
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            sendTransferCommand = false;
            timerCommand.Stop();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string from3 = textBoxFrom03.Text;
            Port port = ECSDeviceManager.GetPort("PORT:" + textBoxFrom03.Text);
            if (port == null)
            {
                Util.Util.ShowMesagePopup("FROM_3을 입력하세요");
                return;
            }
            else
            {
                if (from3.Length == 1)
                {
                    from3 = "0" + from3;
                }

                if (port.IsPalletExist == false || port.PalletId == "") 
                {
                    Util.Util.ShowMesagePopup("Port 확인");
                    return;
                }
                Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:1");
                if (vehicle == null)
                {
                    return;
                }
                vehicle.IO_FROM_01 = "00";
                vehicle.IO_FROM_02 = "00";
                vehicle.IO_FROM_03 = from3;
                vehicle.IO_TO_01 = "00";
                vehicle.IO_TO_02 = "00";
                vehicle.IO_TO_03 = textBoxTo03.Text;
                vehicle.IO_PALLET_ID = textBoxPalletIdVehicle.Text;
                vehicle.IO_TRANSFER_REQUEST = true;
            }
        }

        //private void button6_Click(object sender, EventArgs e)
        //{
        //    Vehicle vehicle = ECSDeviceManager.GetVehicle("VEHICLE:7");
        //    if (vehicle == null)
        //    {
        //        return;
        //    }
        //    vehicle.IO_FROM_01 = "00";
        //    vehicle.IO_FROM_02 = "00";
        //    vehicle.IO_FROM_03 = textBoxFrom03.Text;
        //    vehicle.IO_TO_01 = "00";
        //    vehicle.IO_TO_02 = "00";
        //    vehicle.IO_TO_03 = textBoxTo03.Text;
        //    vehicle.IO_PALLET_ID = textBoxPalletIdVehicle.Text   ;
        //    vehicle.IO_TRANSFER_REQUEST = true;
        //}

        private void buttonClient_Click(object sender, EventArgs e)
        {
            //Server는 무조건 삭제시킴. (TEST Form에서만 적용)
            tcpServer = null;

            string cIp = textBoxIp.Text;
            string cPort = textBoxPort.Text;

            if (string.IsNullOrEmpty(cIp))
            {
                cIp = "127.0.0.1";
            }

            if (string.IsNullOrEmpty(cPort))
            {
                cPort = "9900";
            }

            TcpClientPort = int.Parse(cPort);
            TcpClientIp = cIp;

            if (tcpClient != null)
            {
                if (!tcpClient.Connected)
                {
                    tcpClient = null;
                }
            }

            tcpClient = new TcpClient(TcpClientIp, TcpClientPort);
            tcpClient.OnConnect += new TcpClient.ConnectionDelegate(tcpClient_OnConnect);
            tcpClient.OnDisconnect += new TcpClient.ConnectionDelegate(tcpClient_OnDisconnect);
            tcpClient.OnError += new TcpClient.ErrorDelegate(tcpClient_OnError);
            tcpClient.OnRead += new TcpClient.ReadDelegate(tcpClient_OnRead);
            tcpClient.OnWrite +=new TcpClient.ConnectionDelegate(tcpClient_OnWrite);
           
            tcpClient.Connect();
        }

        void tcpClient_OnConnect(System.Net.Sockets.Socket soc)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            string msg = "(Server:" + tcpClientIp + "), (Port:" + tcpClientPort.ToString() + ") 에 연결되었습니다.";

            clearData(textBoxLog, now + msg);
        }

        void tcpClient_OnDisconnect(System.Net.Sockets.Socket soc)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            string msg = "(Server:" + tcpClientIp + "), (Port:" + tcpClientPort.ToString() + ") 에 연결 해제되었습니다.";

            clearData(textBoxLog, now + msg);
        }

        void tcpClient_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            string msg = "(ERROR:" + ErroCode.ToString() + ")" + ErroMessage;

            clearData(textBoxLog, now + msg);
        }

        void tcpClient_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            //ETX일 경우 Pass
            if (receivedString.Length == 1)
            {
                if (char.Parse(receivedString) == (char)3)
                {
                    return;
                }
            }

            string strReceived = string.Copy(receivedString);
            string strDisplay = "";

            if (!string.IsNullOrEmpty(strReceived))
            {
                strDisplay = strReceived.Replace((char)0, NULL_CHAR);
                strDisplay = strDisplay.Replace((char)2, START_CHAR);
                strDisplay = strDisplay.Replace((char)3, END_CHAR);
            }

            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");

            clearData(textBoxReceived, now + strDisplay);
        }

        //void tcpClient_OnRead(System.Net.Sockets.Socket soc)
        //{
        //    string strReceived = string.Copy(tcpClient.ReceivedText);
        //    string strDisplay = "";

        //    if (!string.IsNullOrEmpty(strReceived))
        //    {
        //        strDisplay = strReceived.Replace((char)0, NULL_CHAR);
        //        strDisplay = strDisplay.Replace((char)2, START_CHAR);
        //        strDisplay = strDisplay.Replace((char)3, END_CHAR);

        //        strDisplay = strDisplay.Remove(strReceived.Length - 1);
        //    }

        //    string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");

        //    clearData(textBoxReceived, now + strDisplay);
        //}

        void  tcpClient_OnWrite(System.Net.Sockets.Socket soc)
        {
 	        //throw new NotImplementedException();
        }

        private void buttonClientClose_Click(object sender, EventArgs e)
        {
            if (tcpClient != null)
            {
                if (tcpClient.Connected)
                {
                    tcpClient.Disconnect();
                    tcpClient = null;
                }
            }
        }

        private void buttonServer_Click(object sender, EventArgs e)
        {
            //Client는 무조건 삭제시킴. (TEST Form에서만 적용)
            tcpClient = null;

            string sPort = textBoxPort.Text;

            if (string.IsNullOrEmpty(sPort))
            {
                sPort = "9900";
            }

            TcpServerPort = int.Parse(sPort);

            tcpServer = new TcpServer(TcpServerPort);

            tcpServer.OnConnect += new TcpServer.ConnectionDelegate(tcpServer_OnConnect);
            tcpServer.OnDisconnect += new TcpServer.ConnectionDelegate(tcpServer_OnDisconnect);
            tcpServer.OnError += new TcpServer.ErrorDelegate(tcpServer_OnError);
            tcpServer.OnListen += new TcpServer.ListenDelegate(tcpServer_OnListen);
            tcpServer.OnRead += new TcpServer.ReadDelegate(tcpServer_OnRead);
            tcpServer.OnSendFile += new TcpServer.ConnectionDelegate(tcpServer_OnSendFile);
            tcpServer.OnWrite += new TcpServer.ConnectionDelegate(tcpServer_OnWrite);

            tcpServer.Start();
        }

        void tcpServer_OnConnect(System.Net.Sockets.Socket soc)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");

            int iClientIndex = tcpServer.IndexOf(soc);
            string msg = "Clent(" + iClientIndex.ToString() + ") 가 연결되었습니다.";

            clearData(textBoxLog, now + msg);
        }

        void tcpServer_OnDisconnect(System.Net.Sockets.Socket soc)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            int iClientIndex = tcpServer.IndexOf(soc);
            string msg = "Clent(" + iClientIndex.ToString() + ") 가 종료되었습니다.";

            clearData(textBoxLog, now + msg);
        }

        void tcpServer_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            string msg = "(ERROR:" + ErroCode.ToString() + ")" + ErroMessage;

            clearData(textBoxLog, now + msg);
        }

        void tcpServer_OnListen()
        {
            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
            string msg = "Listenning Port가 오픈되었습니다. (Port:" + tcpServer.Port.ToString() + ")";

            clearData(textBoxLog, now + msg);
        }

        void tcpServer_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            //ETX일 경우 Pass
            if (receivedString.Length == 1)
            {
                if (char.Parse(receivedString) == (char)3)
                {
                    return;
                }
            }

            string strReceived = string.Copy(receivedString);
            string strDisplay = "";
            WMSCommand rtnCommand = null;
            WMSCommand recvCommand = null;

            if (!string.IsNullOrEmpty(strReceived))
            {
                strDisplay = strReceived.Replace((char)0, NULL_CHAR);
                strDisplay = strDisplay.Replace((char)2, START_CHAR);
                strDisplay = strDisplay.Replace((char)3, END_CHAR);
            }


            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");

            clearData(textBoxReceived, now + strDisplay);

            string[] message = null;
            message = strReceived.Split((char)2);

            if (message.Length != 2)   //message 사이즈는 2가 되어야 함.
            {
                recvCommand = new WMSCommand(); 
                rtnCommand = recvCommand.errorReplyMessage();
            }
            else
            {
                recvCommand = new WMSCommand(message);
                recvCommand.BinaryString = strReceived;

                rtnCommand = recvCommand.checkMessage();
            }

            if (tcpServer != null)
            {
                int iCnt = tcpServer.ActiveConnections;

                if (iCnt > 0)
                {
                    for (int i = 0; i < iCnt; i++)
                    {
                        tcpServer.SendText(rtnCommand.BinaryString, i);
                    }
                }
            }
        }

        //void tcpServer_OnRead(System.Net.Sockets.Socket soc)
        //{
        //    string strReceived = string.Copy(tcpServer.ReceivedText);
        //    string strDisplay = "";

        //    if (!string.IsNullOrEmpty(strReceived))
        //    {
        //        strDisplay = strReceived.Replace((char)0, NULL_CHAR);
        //        strDisplay = strDisplay.Replace((char)2, START_CHAR);
        //        strDisplay = strDisplay.Replace((char)3, END_CHAR);

        //        strDisplay = strDisplay.Remove(strDisplay.Length - 1);
        //        strReceived = strReceived.Remove(strReceived.Length - 1);
        //    }


        //    string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");

        //    clearData(textBoxReceived, now + strDisplay);

        //    string[] message = null;
        //    message = strReceived.Split((char)2);

        //    WMSCommand recvCommand = new WMSCommand(message);
        //    recvCommand.BinaryString = strReceived;

        //    WMSCommand rtnCommand = null;
        //    rtnCommand = recvCommand.checkMessage();


        //    if (tcpServer != null)
        //    {
        //        int iCnt = tcpServer.ActiveConnections;

        //        if (iCnt > 0)
        //        {
        //            for (int i = 0; i < iCnt; i++)
        //            {
        //                tcpServer.SendText(rtnCommand.BinaryString, i);
        //            }
        //        }
        //    }
        //}

        void tcpServer_OnSendFile(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }

        void tcpServer_OnWrite(System.Net.Sockets.Socket soc)
        {
            //throw new NotImplementedException();
        }

        private void buttonServerClose_Click(object sender, EventArgs e)
        {
            if (tcpServer != null)
            {
                if (tcpServer.MListening)
                {
                    tcpServer.Stop();

                    string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
                    string msg = "Listenning Port가 종료되었습니다. (Port:" + tcpServer.Port.ToString() + ")";

                    clearData(textBoxLog, now + msg);
                }
            }
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            string sendMsg = "";

            sendMsg = textBoxMsg.Text;

            if (!string.IsNullOrEmpty(sendMsg))
            {
                sendMsg = sendMsg.Replace(NULL_CHAR, (char)0);
                sendMsg = sendMsg.Replace(START_CHAR, (char)2);
                sendMsg = sendMsg.Replace(END_CHAR, (char)3);

                if (tcpClient != null)
                {
                    if (tcpClient.Connected)
                    {
                        tcpClient.SendText(sendMsg);
                    }
                    else
                    {
                        string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
                        string msg = "서버에 연결되지 않았습니다. ";

                        clearData(textBoxLog, now + msg);
                    }
                }
                else
                {
                    if (tcpServer != null)
                    {
                        int iCnt = tcpServer.ActiveConnections;

                        if (iCnt > 0)
                        {
                            for (int i = 0; i < iCnt; i++)
                            {
                                tcpServer.SendText(sendMsg, i);
                            }
                        }
                        else
                        {
                            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
                            string msg = "접속한 Client가 없습니다. ";

                            clearData(textBoxLog, now + msg);
                        }

                    }
                    else
                    {
                        if (tcpClient == null)
                        {
                            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
                            string msg = "서버 접속 정보가 없습니다. . ";

                            clearData(textBoxLog, now + msg);
                        }

                        if (tcpServer == null)
                        {
                            string now = DateTime.Now.ToString("[ yyyy/mm/dd hh:mm:ss.fff ] ");
                            string msg = "서버가 기동되지 않았습니다. ";

                            clearData(textBoxLog, now + msg);
                        }
                    }
                }
            }

        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            clearData(textBoxLog, "");
        }

        private void buttonClear1_Click(object sender, EventArgs e)
        {
            clearData(textBoxReceived, "");
        }

        delegate void ClearDataDelegate(TextBox tbBox, string msgData);
        private void clearData(TextBox tbBox, string msgData)
        {
            if (tbBox.InvokeRequired)
            {
                ClearDataDelegate tempDelegate = new ClearDataDelegate(clearData);
                this.Invoke(tempDelegate, new object[] { tbBox, msgData });
            }
            else
            {
                if (string.IsNullOrEmpty(msgData))
                {
                    tbBox.Text = msgData;
                }
                else
                {
                    if (string.IsNullOrEmpty(tbBox.Text))
                    {
                        tbBox.Text = msgData;
                    }
                    else
                    {
                        tbBox.Text = tbBox.Text + "\r\n" + msgData;
                    }
                }
            }
        }

        private void buttonMode_Click(object sender, EventArgs e)
        {
            string sendModeData = "";
            sendModeData = "WMS_MBX   " +
                          "0007 " +
                //NULL_CHAR +
                          START_CHAR +
                          "M" +
                          "101" +
                          "1" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendModeData);
        }

        private void buttonOrder_Click(object sender, EventArgs e)
        {
            string sendOrderData = "";
            sendOrderData = "WMS_MBX   " +
                          "0066 " +
                //NULL_CHAR +
                          START_CHAR +
                          "O" +
                          "1" +
                          "0001" +
                          "0000" +
                          "1234   " +
                          "101" +
                          "00" +
                          "000" +
                          "00" +
                          "000" +
                          "001" +
                          "01" +
                          "001" +
                          "01" +
                          "100" +
                          " " +
                          "11111111111111111111" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendOrderData);
        }

        private void buttonReOrder_Click(object sender, EventArgs e)
        {
            string sendReOrderData = "";
            sendReOrderData = "WMS_MBX   " +
                          "0031 " +
                //NULL_CHAR +
                          START_CHAR +
                          "R" +
                          " " +
                          "0001" +
                          "101" +
                          "00" +
                          "000" +
                          "00" +
                          "001" +
                          "01" +
                          "001" +
                          "02" +
                          "1" +
                          "01" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendReOrderData);
        }

        private void buttonStatus_Click(object sender, EventArgs e)
        {
            string sendModData = "";
            sendModData = "WMS_MBX   " +
                          "0043 " +
                //NULL_CHAR +
                          START_CHAR +
                          "S" +
                          "01" +
                          "0" +
                          "0000" +
                          "02" +
                          "0" +
                          "0000" +
                          "03" +
                          "0" +
                          "0000" +
                          "04" +
                          "0" +
                          "0000" +
                          "05" +
                          "0" +
                          "0000" +
                          "0" +
                          "0" +
                          "0" +
                          "0" +
                          "0" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendModData);
        }

        private void buttonError_Click(object sender, EventArgs e)
        {
            string sendModData = "";
            sendModData = "ECS_MBX   " +
                          "0023 " +
                //NULL_CHAR +
                          START_CHAR +
                          "E" +
                          "1" +
                          "101" +
                          "1" +
                          "    " +
                          "0001" +
                          "01" +
                          "001" +
                          "01" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendModData);
        }

        private void buttonFinished_Click(object sender, EventArgs e)
        {
            string sendFinishedData = "";
            sendFinishedData = "ECS_MBX   " +
                          "0010 " +
                //NULL_CHAR +
                          START_CHAR +
                          "F" +
                          "1" +
                          "0001" +
                          "1" +
                          "1" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendFinishedData);
        }

        private void buttonReply_Click(object sender, EventArgs e)
        {
            string sendReplyData = "";
            sendReplyData = "ECS_MBX   " +
                          "0011 " +
                //NULL_CHAR +
                          START_CHAR +
                          "f" +
                          "A" +
                          "00" +
                          "0001" +
                          "1" +
                          END_CHAR;

            clearData(textBoxMsg, "");
            clearData(textBoxMsg, sendReplyData);
        }

        private void textTestPallet01_TextChanged(object sender, EventArgs e)
        {

        }
    }
}