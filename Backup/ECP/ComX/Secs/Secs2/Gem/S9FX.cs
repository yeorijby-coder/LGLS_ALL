using System;
using System.Collections.Generic;
using System.Text;


namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S9FX : Secs2Message
    {
        public const String MHEAD = "0";
        public static byte[] emptyHeader = {0,0,0,0,0,0,0,0,0,0};

        
        public S9FX( int function, SecsHeader mhead ) : base( 9, function, new ItemB(mhead.ToByteArray( )) )
        {            
            this.SetWbit( false );
        }

        
        public void SetMHEAD( SecsHeader mhead )
        {
            this.SetMessageBody( new ItemB(mhead.ToByteArray( )) );
        }

        
        public SecsHeader GetMHEAD()
        {
            ItemB mhead = (ItemB)Find( MHEAD );
            return new SecsHeader( mhead.GetData() );
        }

        
        public S9FX( SecsMessage secsMessage ): base( secsMessage )
        {
            
        }
    }
}
