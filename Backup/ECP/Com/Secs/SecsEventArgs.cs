using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.Secs
{
    public class SecsEventArgs : EventArgs
    {

        private SecsValue secsValue = null;

        public SecsValue SecsValue
        {
            get { return secsValue; }
            set { secsValue = value; }
        }

        private string messageBody = "";

        public string MessageBody
        {
            get { return messageBody; }
            set { messageBody = value; }
        }
        private string errorCode = "";

        public string ErrorCode
        {
            get { return errorCode; }
            set { errorCode = value; }
        }
        private string errorContext = "";

        public string ErrorContext
        {
            get { return errorContext; }
            set { errorContext = value; }
        }

        private string direction = "";

        public string Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        private string stream = "";

        public string Stream
        {
            get { return stream; }
            set { stream = value; }
        }
        private string function = "";

        public string Function
        {
            get { return function; }
            set { function = value; }
        }
        private object secsTransaction = null;

        public object SecsTransaction
        {
            get
            {
                return secsTransaction;
            }
            set
            {
                secsTransaction = value;
            }
        }
        private string raiseDate = "";

        public string RaiseDate
        {
            get { return raiseDate; }
            set { raiseDate = value; }
        }

        public SecsEventArgs(string stream, string function, string messageBody)
        {
            this.stream = stream;
            this.function = function;
            this.messageBody = messageBody;
        }
        public SecsEventArgs(string stream, string function, string direction, string messageBody)
        {
            this.stream = stream;
            this.function = function;
            this.direction = direction;
            this.messageBody = messageBody;
        }
        public SecsEventArgs(string stream, string function, string messageBody, string errorCode, string errorContext)
        {
            this.stream = stream;
            this.function = function; 
            this.messageBody = messageBody;
            this.errorCode = errorCode;
            this.errorContext = errorContext;
        }


        public SecsEventArgs(  string stream, string function, SecsValue secsValue)
        {
            this.stream = stream;
            this.function = function;
            this.secsValue = secsValue;
        }

 
        public SecsEventArgs(  string stream, string function, SecsValue secsValue, object secsTransaction)
        {
            this.stream = stream;
            this.function = function;
            this.secsValue = secsValue;
            this.secsTransaction = secsTransaction;
        }
    }
}
