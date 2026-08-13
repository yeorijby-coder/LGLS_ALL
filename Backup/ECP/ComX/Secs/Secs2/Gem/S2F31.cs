using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F31 : Secs2Message
    {
        public const String TIME = "0";

        public const String DATE_FORMAT_YYMMDDHHMMSS = "yyMMddHHmmss";
        public const String DATE_FORMAT_YYYYMMDDHHMMSSCC = "yyyyMMddHHmmssffff";

     

        
        public S2F31()
            : base( 2, 31, new ItemA( GemDate.GetDate( true )) )
        {
            
        }

        
        public S2F31( bool centuryFormat )
            :base( 2, 31, new ItemA( GemDate.GetDate( centuryFormat )) )
        {
            
        }

        
        public S2F31(String itsDateTime)
            :base( 2, 31, new ItemA(itsDateTime) )
        {
           
        }

        
        public S2F31( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

      
        public override Secs2Message GetSecondary()
        {
            return new S2F32(  GetSecsHeader().NewSecondaryHeader() );
        }

        
        public void SetTIME( String time )
        {
            SetMessageBody( new ItemA(time) );
        }

        
        public void SetTIME( bool centuryFormat )
        {
            SetMessageBody( new ItemA(GemDate.GetDate( centuryFormat )) );
        }


        
        public String GetTIME()
        {
            return Find( TIME ).GetString();
        }
    }
}
