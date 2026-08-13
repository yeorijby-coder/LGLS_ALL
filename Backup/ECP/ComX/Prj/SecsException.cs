using System;
using System.Collections.Generic;
using System.Text;

using ECP.ComX.Secs;

namespace ECP.ComX.Prj
{
    public class SecsException : Exception
    {
        	
         private SecsMessage sm = null;

	    
        private int type = 0;

	    
        public const int CONNECTION_ESTABLISHED	= 1;

	    
        public const int CONNECTION_LOST		= 2;

	    
        public const int CONNECTION_ERROR		= 3;

	   
        public const int MESSAGE_RECEIVED		= 4;

	    
        public const int SEND_FAILED			= 5;

	    
        public const int REPLY_NOT_RECEIVED		= 6;

	    
        public const int FUNCTION_ZERO       = 100;

	   
        public SecsException(
                    int type, String textMessage )
            :base(textMessage)
        {
            
            this.type = type;
        }

	    
        public SecsException(
                    int type, String textMessage, SecsMessage secsMessage )
            : base (textMessage)
        {
            
            this.type = type;
            this.sm = secsMessage;
        }

	   
        public int GetType()
        {
            return type;
        }

	    
        public SecsMessage GetSecsMessage()
        {
            return sm;
        }
    }
}
