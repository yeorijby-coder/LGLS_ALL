using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F14 : Secs2Message
    {
        public const String	COMMACK = "0.0";

        public const String	PARAMS = "0.1";
        public const String	MDLN = "0.1.0";
        public const String	SOFTREV = "0.1.1";

        
        public S1F14( SecsHeader secsHeader ):this( secsHeader, (byte)0 )
        {
          
        }

        
        public S1F14( SecsHeader secsHeader, byte itsCommAck )
         : base( secsHeader, new ItemL(
                                        new ItemB(itsCommAck),
                                        new ItemL()
                                        )
                )
        {
           
        }

        
        public S1F14( SecsMessage secsMessage )
            : base( secsMessage )
        {
           
        }

        
        public void SetCOMMACK(byte commAck)
        {
            ItemB commAckItem = (ItemB)Find( COMMACK );
            commAckItem.SetDatum( 0, commAck );
        }

        
        public byte GetCOMMACK()
        {
            ItemB commAck = (ItemB)Find( COMMACK );
            return commAck.GetData()[0];
        }

        
        public int GetPARAMCount()
        {
            Item list = Find( PARAMS );
            return list.Count();
        }

        
        public void SetMDLN(String mdln)
        {
            Item list = Find( PARAMS );
            if( list.Count() == 0 )
            {
                AddParams();
            }

            Item item = Find(MDLN);
            item.SetValue(mdln);
        }

        
        public String GetMDLN()
        {
            return Find(MDLN).GetString();
        }

        
        public void SetSOFTREV(String softrev)
        {
            Item list = Find( PARAMS );
            if( list.Count() == 0 )
            {
                AddParams();
            }

            Item item = Find(SOFTREV);
            item.SetValue(softrev);
        }

        
        public String GetSOFTREV()
        {
            return Find(SOFTREV).GetString();
        }


      
        private void AddParams()
        {
            Item list = Find( PARAMS );
            list.Add( new ItemA() );
            list.Add( new ItemA() );
        }

    }
}
