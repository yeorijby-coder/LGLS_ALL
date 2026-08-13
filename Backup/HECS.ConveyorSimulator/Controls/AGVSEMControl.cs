using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Com.Tcp;
using HECS.ConveyorSimulator.Devices;
using System.Runtime.InteropServices;
using System.Reflection;

namespace HECS.ConveyorSimulator.Controls
{
    public partial class AGVSEMControl : UserControl
    {

        private delegate void DisplayDelegate(string str);
        private TcpClient agvClient = null;
        private int port = 9900;
        Timer timer = new Timer();

        public AGVSEMControl()
        {

            InitializeComponent();
            timer.Interval = 3000;
            timer.Start();
            timer.Enabled = true;
            timer.Tick += new EventHandler(timer_Tick);
            agvClient = new TcpClient("127.0.0.1", port);
            agvClient.OnConnect += new TcpClient.ConnectionDelegate(agvClient_OnConnect);
            agvClient.OnDisconnect += new TcpClient.ConnectionDelegate(agvClient_OnDisconnect);
            agvClient.OnError += new TcpClient.ErrorDelegate(agvClient_OnError);
            agvClient.OnRead += new TcpClient.ReadDelegate(agvClient_OnRead);
            agvClient.OnWrite += new TcpClient.ConnectionDelegate(agvClient_OnWrite);
            agvVehicleControl1.VehicleObject = DeviceManager.GetVehicle("VEHICLE:1");
            agvVehicleControl2.VehicleObject = DeviceManager.GetVehicle("VEHICLE:2");
            agvVehicleControl3.VehicleObject = DeviceManager.GetVehicle("VEHICLE:3");
            agvVehicleControl1.VehicleObject.ClientSocket = agvClient;
            agvVehicleControl2.VehicleObject.ClientSocket = agvClient;
            agvVehicleControl3.VehicleObject.ClientSocket = agvClient;
            Font font = new Font(FontFamily.GenericMonospace, 9);

            this.richTextBoxAGVAll.Font = font;


        }

        void agvClient_OnWrite(System.Net.Sockets.Socket soc)
        {
            AddAgvCommunicationLog("Send To Server :" + agvClient.WriteText);
        }

        void agvClient_OnRead(System.Net.Sockets.Socket soc, string receivedString)
        {
            //string recv = string.Copy(receivedString);

            //string[] message = null;
            //message = recv.Split((char)2);
            //for (int i = 0; i < message.Length; i++)
            //{
            //    if (message[i].IndexOf((char)3, 0) < 0)
            //    {
            //        continue;
            //    }
            //    WMSCommand recvCommand = new WMSCommand();
            //    recvCommand.BinaryString = ((char)2 + message[i]).PadRight(76, ' ');
            //    AddAgvCommunicationLog("Received From  :" + recvCommand.BinaryString);
            //    //logFile.WriteLine("******받았음:" + recvCommand.CommandId);
            //    if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
            //    {
            //        WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
            //        errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //        errorCommand.ACK = "0";

            //        SendServer(errorCommand.BinaryString);
            //    }
            //    else if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.SIZE)
            //    {
            //        if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
            //        {
            //            WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
            //            errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
            //            errorCommand.ACK = "1";

            //            SendServer(errorCommand.BinaryString);

            //        }
            //    }
            //    HandleCommand(recvCommand);
            //}
        }

        //void agvClient_OnRead(System.Net.Sockets.Socket soc)
        //{
        //    //string recv = string.Copy(agvClient.ReceivedText);

        //    //string[] message = null;
        //    //message = recv.Split((char)2);
        //    //for (int i = 0; i < message.Length; i++)
        //    //{
        //    //    if (message[i].IndexOf((char)3, 0) < 0)
        //    //    {
        //    //        continue;
        //    //    }
        //    //    WMSCommand recvCommand = new WMSCommand();
        //    //    recvCommand.BinaryString = ((char)2 + message[i]).PadRight(76, ' ');
        //    //    AddAgvCommunicationLog("Received From  :" + recvCommand.BinaryString);
        //    //    //logFile.WriteLine("******받았음:" + recvCommand.CommandId);
        //    //    if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
        //    //    {
        //    //        WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
        //    //        errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    //        errorCommand.ACK = "0";

        //    //        SendServer(errorCommand.BinaryString);
        //    //    }
        //    //    else if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.SIZE)
        //    //    {
        //    //        if (recvCommand.Error == WMSCommand.MESSAGE_ERROR.CHECKSUM)
        //    //        {
        //    //            WMSCommand errorCommand = new WMSCommand(WMSCommand.AGV_COMMAND.IllegalMessage_629);
        //    //            errorCommand.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    //            errorCommand.ACK = "1";

        //    //            SendServer(errorCommand.BinaryString);

        //    //        }
        //    //    }
        //    //    HandleCommand(recvCommand);
        //    //}
        //}


        public void SendServer(string value)
        {
            this.agvClient.SendText(value);
            //AddAgvCommunicationLog("Send to Server :" + value);
        }

        public void AddAgvCommunicationLog(string message)
        {
            DisplayDelegate addLog = delegate(string str)
            {
                this.richTextBoxAGVAll.AppendText(str + "\r\n");
                this.richTextBoxAGVAll.Select(this.richTextBoxAGVAll.Text.Length - 2, 1);
                this.richTextBoxAGVAll.ScrollToCaret();

            };
            BeginInvoke(addLog, new string[] { message });
        }



        void agvClient_OnError(string ErroMessage, System.Net.Sockets.Socket soc, int ErroCode)
        {
            AddAgvCommunicationLog("OnError : " + ErroCode.ToString());
        }

        void agvClient_OnDisconnect(System.Net.Sockets.Socket soc)
        {
            AddAgvCommunicationLog("Disconnected from " + soc.RemoteEndPoint);
        }

        void agvClient_OnConnect(System.Net.Sockets.Socket soc)
        {
            AddAgvCommunicationLog("Connected from " + soc.RemoteEndPoint);
        }

        void timer_Tick(object sender, EventArgs e)
        {
            if (agvClient == null)
            {
                return;
            }
            if (agvClient.Connected == false)
            {
                AddAgvCommunicationLog("Retry Connect to Host!!");
                agvClient.Connect();
                return;
            }

        }






        //private void HandleCommand(WMSCommand recvCommand)
        //{
        //    string sapCommandID = "";
        //    string sapCommandSeq = "";
        //    string vehicleId = "";
        //    string palletId = "";
        //    string from = "";
        //    string to = "";
        //    string priority = "";
        //    WMSCommand command = null;
        //    Vehicle vehicle = null;


        //    switch (recvCommand.CommandId)
        //    {
        //        case "627":
        //            command = new WMSCommand(WMSCommand.AGV_COMMAND.DateTimeSetReportAck_628);
        //            command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //            command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
        //            command.Sequence = recvCommand.Sequence;
        //            command.ACK = "0";
        //            SendServer(command.BinaryString);
        //            break;
        //        case "621":
        //            command = new WMSCommand(WMSCommand.AGV_COMMAND.HostCommunicationAck_622);
        //            command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //            command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
        //            command.ACK = "0";
        //            SendServer(command.BinaryString);
        //            break;
        //        case "601":

        //            sapCommandID = recvCommand.SAPCommandID;
        //            sapCommandSeq = recvCommand.CommandSequence;
        //            vehicleId = recvCommand.VehicleId;
        //            palletId = recvCommand.PalletId;
        //            from = recvCommand.From;
        //            to = recvCommand.To;
        //            priority = recvCommand.Priority;


        //            vehicle = DeviceManager.GetVehicle("VEHICLE:" + (int.Parse(vehicleId)).ToString());
        //            if (vehicle == null)
        //            {
        //                //NAK 처리 
        //                return; 
        //            }

        //            Port fromPort = DeviceManager.GetPort("PORT:" + (int.Parse(from)).ToString());
        //            Port toPort = DeviceManager.GetPort("PORT:" + (int.Parse(to)).ToString());

        //            if (fromPort == null)
        //            {
        //                //NAK 처리 
        //                return; 
        //            }

        //            if (toPort == null)
        //            {
        //                //NAK 처리 
        //                return; 
        //            }
                    


        //            command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferRequestAck_602);
        //            command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //            command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
        //            command.ACK = "1";
        //            command.Sequence = recvCommand.Sequence;


        //            SendServer(command.BinaryString);

        //            vehicle.AgvFrom = (int.Parse(from)).ToString();
        //            vehicle.AgvTo = (int.Parse(to)).ToString();
        //            vehicle.AgvSapCommandId = sapCommandID;
        //            vehicle.AgvSapSeq = sapCommandSeq;
        //            vehicle.AgvCommandSeq = recvCommand.Sequence;
        //            vehicle.AgvCommandId = recvCommand.CommandId; 
        //            vehicle.AgvPalletId = palletId;
        //            vehicle.AgvPriority = (int.Parse(priority)).ToString();
        //            vehicle.AgvSapSeq = sapCommandSeq;
        //            vehicle.VehicleState = Vehicle.VEHICLE_STATE.REQUEST_TRANSFER;
        //            vehicle.StepCount = 0;
        //            break;
        //        case "619":
        //            vehicleId = recvCommand.VehicleIdForState;

        //            vehicle = DeviceManager.GetVehicle("VEHICLE:" + (int.Parse(vehicleId)).ToString());
        //            if (vehicle == null)
        //            {
        //                return;
        //            }
        //            else
        //            {
        //                command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStateReportAck_620);
        //                command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //                command.Direction = WMSCommand.MESSAGE_DIRECTION.AGVC_TO_HOST;
        //                command.VehicleIdForState = vehicleId.PadLeft(2, ' ');
        //                command.VehicleLocationForState = vehicle.AgvMarkPlate.PadLeft(2, ' ');
        //                command.VehicleSAPCommandId = vehicle.AgvSapCommandId.PadLeft(12, ' ');
        //                command.VehicleCommandSequence = vehicle.AgvSapSeq.PadLeft(4, ' ');
        //                command.VehicleState = ((int)vehicle.VehicleRunState).ToString();
        //            }
        //            SendServer(command.BinaryString);
        //            break;
        //        //case "603":
        //        //    HandleCommand603(recvCommand);
        //        //    break;
        //        //case "605":
        //        //    HandleCommand605(recvCommand);
        //        //    break;
        //        //case "607":
        //        //    HandleCommand607(recvCommand);
        //        //    break;
        //        //case "609":
        //        //    HandleCommand609(recvCommand);
        //        //    break;
        //        //case "611":
        //        //    HandleCommand611(recvCommand);
        //        //    break;
        //        //case "613":
        //        //    HandleCommand613(recvCommand);
        //        //    break;
        //        //case "615":
        //        //    HandleCommand615(recvCommand);
        //        //    break;
        //        //case "617":
        //        //    HandleCommand617(recvCommand);
        //        //    break;
        //        //case "623":
        //        //    HandleCommand623(recvCommand);
        //        //    break;
        //        //case "625":
        //        //    HandleCommand625(recvCommand);
        //        //    break;
        //        //case "626":
        //        //    HandleCommand625(recvCommand);
        //        //    break;
        //        //case "602":
        //        //case "604":
        //        //case "606":
        //        //case "608":
        //        //case "610":
        //        //case "612":
        //        //case "614":
        //        //case "616":
        //        //case "618":
        //        //    //logFile.WriteLine("===========================================================");
        //        //    //logFile.WriteLine("CommandId:" + recvCommand.CommandId);
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("Received From AGVC:" + recvCommand.CommandId + "=" + (recvCommand.ACK == "0" ? "[0]SUCCESS" : "[" + recvCommand.ACK + "]NAK"));
        //        //    //logFile.WriteLine("===========================================================");
        //        //    break;
        //        //case "620":
        //        //    //logFile.WriteLine("===========================================================");
        //        //    //logFile.WriteLine("VehicleState");
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("");
        //        //    //logFile.WriteLine("CommandId:" + recvCommand.CommandId);
        //        //    //logFile.WriteLine("VehicleIdForState:" + recvCommand.VehicleIdForState);
        //        //    //logFile.WriteLine("VehicleLocationForState:" + recvCommand.VehicleLocationForState);
        //        //    //logFile.WriteLine("VehicleSAPCommandId:" + recvCommand.VehicleSAPCommandId);
        //        //    //logFile.WriteLine("VehicleCommandSequence:" + recvCommand.VehicleCommandSequence);
        //        //    //logFile.WriteLine("VehicleState:" + recvCommand.VehicleState);
        //        //    //logFile.WriteLine("===========================================================");
        //        //    break;
        //        //case "622":
        //        //    //logFile.WriteLine("===========================================================");
        //        //    //logFile.WriteLine("CommunicationState");
        //        //    //logFile.WriteLine("CommandId:" + recvCommand.CommandId);
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("");
        //        //    //logFile.WriteLine("" + recvCommand.CommuncationState);
        //        //    //logFile.WriteLine("===========================================================");
        //        //    break;
        //        //case "628":
        //        //    //logFile.WriteLine("===========================================================");
        //        //    //logFile.WriteLine("DateTimeSet Request");
        //        //    //logFile.WriteLine("CommandId:" + recvCommand.CommandId);
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("");
        //        //    //logFile.WriteLine("" + recvCommand.ACK);
        //        //    //logFile.WriteLine("===========================================================");
        //        //    break;
        //        //case "629":
        //        //    //logFile.WriteLine("===========================================================");
        //        //    //logFile.WriteLine("Message Format Exception(Illegal Data Format)");
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("");
        //        //    //logFile.WriteLine("" + recvCommand.ACK + "=>" + (recvCommand.ACK == "0" ? "CheckSumError" : "SizeError"));
        //        //    //logFile.WriteLine("===========================================================");
        //        //    break;
        //        //default:
        //        //    //logFile.WriteLine("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
        //        //    //logFile.WriteLine("알수없는 Command : " + recvCommand.CommandId);
        //        //    //logFile.WriteLine("Direction:" + recvCommand.Direction);
        //        //    //logFile.WriteLine("IsRequest:" + recvCommand.IsRequestReply);
        //        //    //logFile.WriteLine("Sequence:" + recvCommand.Sequence);
        //        //    //logFile.WriteLine("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
        //        //    break;
        //    }

        //}



        //private void HandleCommand603(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[TransferStartAck_604]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("StartTime :" + recvCommand.StartTime);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferStartAck_604);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand605(WMSCommand recvCommand)
        //{

        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[VehicleAssignAck_606]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]" + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]" + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleAssignAck_606);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand607(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[VehicleArrivedPortAck_608]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("ArrivedLocation :" + recvCommand.ArrivedLocation);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleArrivedPortAck_608);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand609(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[PalletLoadCompleteAck_610]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("PalletId :" + recvCommand.PalletId);
        //    //logFile.WriteLine("LoadLocation :" + recvCommand.LoadLocation);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.PalletLoadCompleteAck_610);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand611(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[VehicleStartPortAck_612]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("PalletId :" + recvCommand.PalletId);
        //    //logFile.WriteLine("StartLocation :" + recvCommand.StartLocation);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleStartPortAck_612);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand613(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[PalletUnloadCompleteAck_614]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("PalletId :" + recvCommand.PalletId);
        //    //logFile.WriteLine("UnloadLocation :" + recvCommand.UnloadLocation);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.PalletUnloadCompleteAck_614);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand615(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[VehicleReleasedAck_616]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.VehicleReleasedAck_616);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}
        //private void HandleCommand617(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[TransferCompleteAck_618]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("SAPCommandID :" + recvCommand.SAPCommandID);
        //    //logFile.WriteLine("CommandSequence :" + recvCommand.CommandSequence);
        //    //logFile.WriteLine("VehicleId :" + recvCommand.VehicleId);
        //    //logFile.WriteLine("PalletId :" + recvCommand.PalletId);
        //    //logFile.WriteLine("StartLocation :" + recvCommand.StartLocation);
        //    //logFile.WriteLine("EndLocation :" + recvCommand.EndLocation);
        //    //logFile.WriteLine("CompletedTime :" + recvCommand.CompletedTime);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.TransferCompleteAck_618);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}

        //private void HandleCommand623(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[AlarmSetReportAck_624]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("AlarmSetTime :" + recvCommand.AlarmSetTime);
        //    //logFile.WriteLine("AlarmVehicleId :" + recvCommand.AlarmVehicleId);
        //    //logFile.WriteLine("AlarmCode :" + recvCommand.AlarmCode);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.AlarmSetReportAck_624);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}

        //private void HandleCommand625(WMSCommand recvCommand)
        //{
        //    //logFile.WriteLine("*************************************************************");
        //    //logFile.WriteLine("CommandId :" + recvCommand.CommandId + "[AlarmResetReportAck_626]");
        //    //logFile.WriteLine("Direction :" + recvCommand.Direction.ToString() + "[" + (recvCommand.Direction == ezSocket.AGVCommand.MESSAGE_DIRECTION.AGVC_TO_HOST ? "HOST" : "AGVC") + "]");
        //    //logFile.WriteLine("IsRequestReply :" + (recvCommand.IsRequestReply == AGVCommand.REQUEST_REPLY.NEED_REPLY ? "REPLY_REQUST" : "NO_REPLY"));
        //    //logFile.WriteLine("Sequence :" + recvCommand.Sequence);
        //    //logFile.WriteLine("");
        //    //logFile.WriteLine("AlarmSetTime :" + recvCommand.AlarmSetTime);
        //    //logFile.WriteLine("AlarmVehicleId :" + recvCommand.AlarmVehicleId);
        //    //logFile.WriteLine("AlarmCode :" + recvCommand.AlarmCode);
        //    //logFile.WriteLine("AlarmResetTime :" + recvCommand.AlarmResetTime);
        //    //logFile.WriteLine("*************************************************************");
        //    WMSCommand command = new WMSCommand(WMSCommand.AGV_COMMAND.AlarmResetReportAck_626);
        //    command.IsRequestReply = WMSCommand.REQUEST_REPLY.NO_REPLY;
        //    command.Sequence = recvCommand.Sequence;
        //    command.ACK = "0";
        //    command.AckVehicleId = recvCommand.VehicleId;
        //    SendServer(command.BinaryString);
        //}



    }
}
