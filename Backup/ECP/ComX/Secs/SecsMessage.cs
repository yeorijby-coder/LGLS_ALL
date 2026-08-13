using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs
{
    public abstract class SecsMessage
    {

        private SecsHeader secsHeader;

        private SecsMessage primaryMessage;


        protected SecsMessage()
        {
            secsHeader = new SecsHeader();
        }


        protected SecsMessage(byte[] secsHeader)
        {
            this.secsHeader = new SecsHeader(secsHeader);
        }


        protected SecsMessage(SecsHeader secsHeader)
        {
            this.secsHeader = secsHeader;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append(GetSecsHeader().GetStringId());

            return result.ToString();
        }

        
        public SecsHeader GetSecsHeader()
        {
            return secsHeader;
        }


        public void SetSecsHeader(SecsHeader secsHeader)
        {
            this.secsHeader = secsHeader;
        }

 
        public int GetMessageSizeWithHeader()
        {
            return GetMessageSize() + GetHeaderSize();
        }

    
        public int HashCode()
        {
            return GetSecsHeader().HashCode();
        }

        public override int GetHashCode()
        {
            return GetSecsHeader().HashCode();
        }


        public override bool Equals(Object obj)
        {
            try
            {
                SecsMessage secsMessage = (SecsMessage)obj;
                return GetSecsHeader().Equals(secsMessage.GetSecsHeader());
            }
            catch (Exception e)
            {
                return false;
            }
        }

 
        public String GetStringId()
        {
            return GetSecsHeader().GetStringId();
        }


        public int GetMessageIndex()
        {
            return GetSecsHeader().GetMessageIndex();
        }

     
        public bool IsPrimaryMessage()
        {
            return GetSecsHeader().IsPrimaryMessage();
        }

   
        public bool IsSecondaryMessage()
        {
            return GetSecsHeader().IsSecondaryMessage();
        }

        public byte[] HeaderToByteArray()
        {
            return GetSecsHeader().ToByteArray();
        }

 
        public int GetSessionId()
        {
            return GetSecsHeader().GetSessionId();
        }

        public void SetSessionId(int value)
        {
            GetSecsHeader().SetSessionId(value);
        }

        public bool GetWbit()
        {
            return GetSecsHeader().GetWbit();
        }

 
        public void SetWbit(bool value)
        {
            GetSecsHeader().SetWbit(value);
        }

  
        public int GetStream()
        {
            return GetSecsHeader().GetStream();
        }


        public void SetStream(int stream)
        {
            GetSecsHeader().SetStream(stream);
        }

   
        public int GetFunction()
        {
            return GetSecsHeader().GetFunction();
        }

  
        public void SetFunction(int function)
        {
            GetSecsHeader().SetFunction(function);
        }

    
        public int GetSourceId()
        {
            return GetSecsHeader().GetSourceId();
        }

        
        public void SetSourceId(int sourceId)
        {
            GetSecsHeader().SetSourceId(sourceId);
        }

        
        public int GetTransactionId()
        {
            return GetSecsHeader().GetTransactionId();
        }

        
        public void SetTransactionId(int transId)
        {
            GetSecsHeader().SetTransactionId(transId);
        }

        
        public int GetSystemBytes()
        {
            return GetSecsHeader().GetSystemBytes();
        }

        
        public void SetSystemBytes(int sysBytes)
        {
            GetSecsHeader().SetSystemBytes(sysBytes);
        }

       
        public void SetSystemBytes()
        {
            GetSecsHeader().SetSystemBytes();
        }

        
        public int GetMessageId()
        {
            return GetSecsHeader().GetMessageId();
        }

        
        public int GetHeaderSize()
        {
            return GetSecsHeader().GetSize();
        }

        
        public void SetHeader(byte[] header)
        {
            GetSecsHeader().SetHeader(header);
        }

        
        public void SetPrimaryMessage(SecsMessage primaryMessage)
        {
            this.primaryMessage = primaryMessage;
        }

        
        public SecsMessage GetPrimaryMessage()
        {
            return primaryMessage;
        }

        
        abstract public int GetMessageSize();

       
        abstract public byte[] ToByteArray();

        
        abstract public void FromByteArray(byte[] message);
    }
}
