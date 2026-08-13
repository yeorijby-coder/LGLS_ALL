using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Hsms
{
    public class HsmsMessage : SecsMessage
    {
        public const int DATA_MESSAGE = 0;
        public const int SELECT_REQ = 1;
        public const int SELECT_RSP = 2;
        public const int DESELECT_REQ = 3;
        public const int DESELECT_RSP = 4 ;
        public const int LINKTEST_REQ = 5;
        public const int LINKTEST_RSP = 6;
        public const int REJECT_REQ = 7;
        public const int SEPARATE_REQ = 9;

        
        private byte[] messageData = null;

       
        public HsmsMessage():base(new HsmsHeader())
        {
            
        }

       
        public HsmsMessage( byte[] messageData ):base(new HsmsHeader( messageData ))
        {
            this.messageData = messageData;
        }

        
        public HsmsMessage( int pType, SecsMessage secsMessage ) :base(secsMessage.GetSecsHeader())
        {

            byte[] messageData = secsMessage.ToByteArray();

            // Update the Ptype, and Stype in the messageData.
            HsmsHeader.SetSType( messageData, DATA_MESSAGE );
            HsmsHeader.SetPType( messageData, pType );

            FromByteArray( messageData );
        }

        
        public HsmsMessage( int sessionId, int pType, SecsMessage secsMessage ) 
            : base (secsMessage.GetSecsHeader())
        {
                

            byte[] messageData = secsMessage.ToByteArray();

            HsmsHeader.SetSessionId( messageData, sessionId );
            HsmsHeader.SetSType( messageData, DATA_MESSAGE );
            HsmsHeader.SetPType( messageData, pType );

            FromByteArray( messageData );
        }


       
        public HsmsMessage( HsmsHeader hsmsHeader ) :base(hsmsHeader)
        {

        }


       
        public HsmsHeader GetHsmsHeader()
        {
            return (HsmsHeader)GetSecsHeader();
        }

        
        public override byte[] ToByteArray()
        {
            if( messageData == null )
            {
                return GetSecsHeader().ToByteArray();
            }
            else
            {
                return messageData;
            }
        }

        
        public override void FromByteArray( byte[] message )
        {
            SetSecsHeader(new HsmsHeader( message ));
            messageData = message;
        }

        
        public override int GetMessageSize()
        {
            return messageData.Length;
        }

        
        public int GetByte2()
        {
            return GetHsmsHeader().GetByte2();
        }

        
        public void SetByte2( int value )
        {
            GetHsmsHeader().SetByte2( value );
        }

        
        public int GetByte3()
        {
            return GetHsmsHeader().GetByte3();
        }

        
        public void SetByte3( int value )
        {
            GetHsmsHeader().SetByte3( value );
        }

        
        public int GetPType()
        {
            return GetHsmsHeader().GetPType();
        }

        
        public void SetPType(int pType)
        {
            GetHsmsHeader().SetPType(pType);
        }
     
        
        public int GetSType()
        {
            return GetHsmsHeader().GetSType();
        }
     
        
        public void SetSType(int sType)
        {
            GetHsmsHeader().SetSType(sType);
        }
    }
}
