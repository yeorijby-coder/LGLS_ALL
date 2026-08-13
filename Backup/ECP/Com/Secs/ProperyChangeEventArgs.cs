using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.Secs
{
    public class ProperyChangeEventArgs : EventArgs
    {
        private string parameterKey;
        public string ParameterKey
        {
            get { return parameterKey; }
            set { parameterKey = value; }
        }

        private string parameterValue;
        public string ParameterValue
        {
            get { return parameterValue; }
            set { parameterValue = value; }
        }


        public ProperyChangeEventArgs(string paramKey, string paramValue)
        {
            this.parameterKey = paramKey;
            this.parameterValue = paramValue;
        }

    }


}
