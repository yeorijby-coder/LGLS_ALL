using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.Secs
{
    public delegate void ReceivedMessage(object sender, SecsEventArgs e);
    public delegate void UnknownMessage(object sender, SecsEventArgs e);
    public delegate void TimeoutOccured(object sender, SecsEventArgs e);
    public delegate void ReceivedInvalidMessage(object sender, SecsEventArgs e);
    public delegate void Connected(object sender, SecsEventArgs e);
    public delegate void Disconnected(object sender, SecsEventArgs e);
    public delegate void AbortMessage(object sender, SecsEventArgs e);
    public delegate void Secs1Log(object sender, SecsEventArgs e);
    public delegate void Secs2Log(object sender, SecsEventArgs e);
    public delegate void SendMessage(object sender, SecsEventArgs e); 
    public delegate void ReplyMessage(object sender, SecsEventArgs e);
    public abstract class  AbstractSecsDriver
    {
        public enum CONNECTION_MODE
        {
            ACTIVE = 1,
            PASSIVE = 2
        }

        public enum COMMUNICATION_TYPE
        {
            SECS1 = 1,
            HSMS = 2 
        }

        //public delegate void OnSecsParameterChange(object sender, EventArgs e);   

        private string deviceName;
        public string DeviceName
        {
            get { return deviceName; }
            set { deviceName = value; }
        }

        private string deviceId;
        public string DeviceId
        {
            get { return deviceId; }
            set { deviceId = value; }
        }

        private CONNECTION_MODE connectionMode;
        public CONNECTION_MODE ConnectionMode
        {
            get { return connectionMode; }
            set { connectionMode = value; }
        }

        private bool isConnected = false;
        public bool IsConnected
        {
            get { return isConnected; }
            set { isConnected = value; }
        }

        private bool isSEC1LogEnable = false;
        public bool IsSEC1LogEnable
        {
            get { return isSEC1LogEnable; }
            set { isSEC1LogEnable = value; }
        }
        
        private bool isHSMSLogEnable = true;
        public bool IsHSMSLogEnable
        {
            get { return isHSMSLogEnable; }
            set { isHSMSLogEnable = value; }
        }
        
        private bool isParameterAutoApply = true;
        public bool IsParameterAutoApply
        {
            get { return isParameterAutoApply; }
            set { isParameterAutoApply = value; }
        }

        private SecsProperty secsProperty = new SecsProperty();
        public SecsProperty SecsProperty
        {
            get { return secsProperty; }
            set { secsProperty = value; }
        }


        public AbstractSecsDriver()
        {
            secsProperty.OnPropertyChange += new SecsPropertyChange(secsProperty_OnPropertyChange);
        }

        void secsProperty_OnPropertyChange(object sender, ProperyChangeEventArgs e)
        {
            //System.Console.WriteLine("Parameter가 변경되었습니다."); 
        }







        abstract public void Initialize(SecsProperty prop);
        abstract public void Open();
        abstract public void Reopen();
        abstract public void Close();
        abstract public void TerminateSEComDriver();
        abstract public void ReplyMessage(string sxfx, object secsTransaction, SecsValue secsValue);
        abstract public void SendMessage(string sxfx, string param, object obj1);
        abstract public void SendMessage(string sxfx, string param, object obj1, object obj2);
        abstract public void SendMessage(string sxfx, string param, object obj1, object obj2, object obj3);
        abstract public void SendMessage(string sxfx, string param, object obj1, object obj2, object obj3, object obj4);
        abstract public void SendMessage(string sxfx, string param, object obj1, object obj2, object obj3, object obj4, object obj5);
        abstract public void SendMessage(string sxfx, string param, object[] objs);

        abstract public void SendMessage(string sxfx, object obj1);
        abstract public void SendMessage(string sxfx, object obj1, object obj2);
        abstract public void SendMessage(string sxfx, object obj1, object obj2, object obj3);
        abstract public void SendMessage(string sxfx, object obj1, object obj2, object obj3, object obj4);
        abstract public void SendMessage(string sxfx, object obj1, object obj2, object obj3, object obj4, object obj5);
        abstract public void SendMessage(string sxfx, object[] objs);

        abstract public event ReceivedMessage OnReceiveMessage;
        abstract public event UnknownMessage OnUnknownMessage; 
        abstract public event TimeoutOccured OnTimeoutOccured; 
        abstract public event ReceivedInvalidMessage OnInvalidMessage; 
        abstract public event Connected OnConnected;
        abstract public event Disconnected OnDisconnected; 
        abstract public event AbortMessage OnAbortMessage; 
        abstract public event Secs1Log OnSecs1Log; 
        abstract public event Secs2Log OnSecs2Log;
        abstract public event SendMessage OnSendMessage;


    }
}
