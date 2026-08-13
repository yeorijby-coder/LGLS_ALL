using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F13 : Secs2Message
    {
        public const String	PARAMS = "0";
        public const String	MDLN = "0.0";
        public const String	SOFTREV = "0.1";

       
        public S1F13() : base(1, 13, new ItemL())
        {
        }

     
        public S1F13(String itsModelNumber, String itsSoftwareRev)
            : base(1, 13, new ItemL(new ItemA(itsModelNumber),
                                   new ItemA(itsSoftwareRev)))
        {
           
        }

        
        public S1F13( SecsMessage secsMessage )
            :base( secsMessage )
        {
            
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

        
        public override Secs2Message GetSecondary()
        {
            return new S1F14( GetSecsHeader().NewSecondaryHeader(), (byte)0 );
        }

        
        private void AddParams()
        {
            Item list = Find( PARAMS );
            list.Add( new ItemA() );
            list.Add( new ItemA() );
        }
    }
}
