using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.Tcp
{
    public class WMSCom
    {
        private string remoteAddress = "";
        private int remotePort = 0;

        public WMSCom(string address, int port)
        {
            this.remoteAddress = address;
            this.remotePort = port;
        }

    }
}
