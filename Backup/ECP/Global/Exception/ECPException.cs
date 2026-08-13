using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Exception
{
    public class ECPException : System.ApplicationException
    {
        public ECPExceptionType type = null;

        public string value = string.Empty;

        public string reason = string.Empty;

        public ECPException(ECPExceptionType type, string value, string reason)
        {
            this.type = type;
            this.value = value;
            this.reason = reason; 
        }
    }
}
