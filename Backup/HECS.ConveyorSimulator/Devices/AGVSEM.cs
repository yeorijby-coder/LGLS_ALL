using System;
using System.Collections.Generic;
using System.Text;
using ECP.Com.Tcp;

namespace HECS.ConveyorSimulator.Devices
{
    [Serializable]
    public class AGVSEM : Device
    {
        private TcpClient clientSocket = null;

        public TcpClient ClientSocket
        {
            get
            {
                return clientSocket;
            }
            set
            {
                clientSocket = value;
            }
        }
        public enum CONNECT_STATE
        {
            DISCONNECTED = 0, 
            CONNECTED = 1, 
        }
        private CONNECT_STATE connectState = CONNECT_STATE.DISCONNECTED;

        public CONNECT_STATE ConnectState
        {
            get
            {
                return connectState;
            }
            set
            {
                connectState = value;
            }
        }
        private bool isError = false;

        public bool IsError
        {
            get
            {
                return isError;
            }
            set
            {
                isError = value;
            }
        }
        public void SendServer(string value)
        {
            if (clientSocket ==  null)
            {
                return;
            }
            if (clientSocket.Connected == false)
            {
                return;
            }
            this.clientSocket.SendText(value);
        }
    }
}
