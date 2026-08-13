using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs
{
    public class SecsEvent
    {
        public const int CONNECTION_ESTABLISHED	= 1;
        public const int CONNECTION_LOST = 2;
        public const int CONNECTION_ERROR = 3;
        public const int MESSAGE_RECEIVED = 4;
        public const int SEND_FAILED = 5;
        public const int REPLY_NOT_RECEIVED = 6;

        private int    	type;
	    private String	text = "";
        private SecsMessage message = null;

       
        public SecsEvent(int type)
        {
            this.type = type;
        }

        
        public SecsEvent(int type, SecsMessage message)
        {
            this.type = type;
            this.message = message;
        }

        
        public SecsEvent(int type, String text)
        {
            this.type = type;
            this.text = text;
        }

        
        new public int GetType()
        {
            return type;
        }

       
        public SecsMessage GetMessage()
        {
            return message;
        }

        
        public override String ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("SecsEvent[");

            switch (type)
            {
            case CONNECTION_ESTABLISHED:
                result.Append("CONNECTION_ESTABLISHED");
                break;

            case CONNECTION_LOST:
                result.Append("CONNECTION_LOST");
                break;

            case CONNECTION_ERROR:
                result.Append("CONNECTION_ERROR, " + text);
                break;

            case MESSAGE_RECEIVED:
                result.Append("MESSAGE_RECEIVED, ");
                result.Append(message);
                break;

            case SEND_FAILED:
                result.Append("SEND_FAILED, ");
                result.Append(message);
                break;

            case REPLY_NOT_RECEIVED:
                result.Append("REPLY_NOT_RECEIVED, ");
                result.Append(message);
                break;

            default:
                result.Append("UNKNOWN");
                break;
            }

            result.Append(']');

            return result.ToString();
        }
    }
}
