using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Logger;

namespace ECP.Com.Secs
{
    //Declare Secs Property Change Event 
    public delegate void SecsPropertyChange(object sender, ProperyChangeEventArgs e);

    public class SecsProperty
    {
        private string driverName = "STKC";

        public string DriverName
        {
            get { return driverName; }
            set
            {
                driverName = value;
                AddValue("DRIVERNAME", driverName);
            }
        }
        private string deviceID = "1";

        public string DeviceID
        {
            get { return deviceID; }
            set
            {
                deviceID = value;
                AddValue("DEVICEID", deviceID);
            }
        }
        private string hsmsT3 = "45";
        public string HsmsT3
        {
            get
            {
                return hsmsT3;
            }
            set
            {
                hsmsT3 = value;
                AddValue("HSMST3", hsmsT3);
            }
        }

        private string hsmsT5 = "10";
        public string HsmsT5
        {
            get
            {
                return hsmsT5;
            }
            set
            {
                hsmsT5 = value;
                AddValue("HSMST5", hsmsT5);
            }
        }

        private string hsmsT6 = "5";
        public string HsmsT6
        {
            get
            {
                return hsmsT6;
            }
            set
            {
                hsmsT6 = value;
                AddValue("HSMST6", hsmsT6);
            }
        }

        private string hsmsT7 = "10";
        public string HsmsT7
        {
            get
            {
                return hsmsT7;
            }
            set
            {
                hsmsT7 = value;
                AddValue("HSMST7", hsmsT7);

            }
        }

        private string hsmsT8 = "10";
        public string HsmsT8
        {
            get { return hsmsT8; }
            set
            {
                hsmsT8 = value;
                AddValue("HSMST8", hsmsT8);

            }
        }

        private string secs1T1 = "0.5";
        public string Secs1T1
        {
            get
            {
                return secs1T1;
            }
            set
            {
                secs1T1 = value;
                AddValue("SECST1", secs1T1);

            }
        }

        private string secs1T2 = "10";
        public string Secs1T2
        {
            get
            {
                return secs1T2;
            }
            set
            {
                secs1T2 = value;
                AddValue("SECST2", secs1T2);
            }
        }

        private string secs1T3 = "45";
        public string Secs1T3
        {
            get
            {
                return secs1T3;
            }
            set
            {
                secs1T3 = value;
                AddValue("SECST3", secs1T3);
            }
        }

        private string secs1T4 = "45";
        public string Secs1T4
        {
            get
            {
                return secs1T4;
            }
            set
            {
                secs1T4 = value;
                AddValue("SECST4", secs1T4);
            }
        }

        private string serailPort = "COM1";
        public string SerailPort
        {
            get
            {
                return serailPort;
            }
            set
            {
                serailPort = value;
                AddValue("SERIALPORT", serailPort);
            }
        }

        private string baudRate = "AUTO";
        public string BaudRate
        {
            get
            {
                return baudRate;
            }
            set
            {
                baudRate = value;
                AddValue("BAUDRATE", baudRate);
            }
        }

        private string retryCount = "3";
        public string RetryCount
        {
            get
            {
                return retryCount;
            }
            set
            {
                retryCount = value;
                AddValue("RETRYCOUNT", retryCount);
            }
        }

        private string master = "TRUE";
        public string Master
        {
            get
            {
                return master;
            }
            set
            {
                master = value;
                AddValue("MASTER", master);
            }
        }

        private string interleave = "fasle";
        public string Interleave
        {
            get
            {
                return interleave;
            }
            set
            {
                interleave = value;
                AddValue("INTERLEAVE", interleave );
            }
        }

        private string localhost = "127.0.0.1";
        public string Localhost
        {
            get
            {
                return localhost;
            }
            set
            {
                localhost = value;
                AddValue("LOCALHOST", localhost);
            }
        }

        private string remoteHost = "127.0.0.1";
        public string RemoteHost
        {
            get
            {
                return remoteHost;
            }
            set
            {
                remoteHost = value;
                AddValue("REMOTEHOST", remoteHost);
            }
        }

        private string localPort = "0";
        public string LocalPort
        {
            get
            {
                return localPort;
            }
            set
            {
                localPort = value;
                AddValue("LOCALPORT", remoteHost);
            }
        }

        private string remotePort = "0";
        public string RemotePort
        {
            get
            {
                return remotePort;
            }
            set
            {
                remotePort = value;
                AddValue("REMOTEPORT", remoteHost);
            }
        }

        public event SecsPropertyChange OnPropertyChange;
        private Dictionary<string, string> props = new Dictionary<string, string>();

        public SecsProperty()
        {
            props.Add("DRIVERNAME", driverName);
            props.Add("DEVICEID", deviceID);
            props.Add("HSMST3", hsmsT3);
            props.Add("HSMST5", hsmsT5);
            props.Add("HSMST6", hsmsT6);
            props.Add("HSMST7", hsmsT7);
            props.Add("HSMST8", hsmsT8);
            props.Add("SECS1T1", secs1T1);
            props.Add("SECS1T2", secs1T2);
            props.Add("SECS1T3", secs1T3);
            props.Add("SECS1T4", secs1T4);
            props.Add("SERIALPORT", serailPort);
            props.Add("BAUDRATE", baudRate);
            props.Add("RETRYCOUNT", retryCount);
            props.Add("MASTER", master);
            props.Add("INTERLEAVE", interleave);
            props.Add("LOCALHOST", localhost);
            props.Add("REMOTEHOST", remoteHost);
            props.Add("LOCALPORT", localPort);
            props.Add("REMOTEPORT", remotePort);
        }


        public string GetValuesAll()
        {
            return null;
        }
        public string GetValue(string key)
        {
            return "";
        }
        public string[] GetKeys()
        {
            return null;
        }

        public void AddValue(string key, string value)
        {
            try
            {
                if (IsExistKey(key))
                {
                    props.Remove(key);
                    props.Add(key, value);
                    OnPropertyChange(this, new ProperyChangeEventArgs(key, value));
                }
                else
                {
                    props.Add(key, value);
                    OnPropertyChange(this, new ProperyChangeEventArgs(key, value));
                }
            }
            catch (Exception ex)
            {
                //System.Console.WriteLine("SecsProperty 설정 중에 에러가 발생하였습니다.");
                //System.Console.WriteLine("Exception: " + e.ToString());
                Log.log(5, "AddValue", "debug", Log.DEBUG,"",ex.Message.ToString(),ex.StackTrace.ToString());
            }

        }
        public bool IsExistKey(string key)
        {
            if (props.ContainsKey(key))
            {
                return true;
            }
            return false;
        }


        public int ValueCount()
        {
            return props.Count ;
        }


    }
}
