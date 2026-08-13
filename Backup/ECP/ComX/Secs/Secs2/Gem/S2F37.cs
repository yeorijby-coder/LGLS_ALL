using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F37 : Secs2Message
    {
        public const String CEED = "0.0";
        public const String CEID_LIST = "0.1";

        
        public S2F37()
            :this(true)
        {
            
        }

        
        public S2F37(bool ceed)
           : base(2, 37, new ItemL(
                                   new ItemBOOLEAN(ceed),
                                   new ItemL()
                                   )
                  )
        {
        }

       
        public S2F37( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

       
        public void SetCEED( bool ceed )
        {
            ItemBOOLEAN ceedItem = (ItemBOOLEAN)Find(CEED);
            ceedItem.SetValue( ceed );
        }

        
        public bool GetCEED( )
        {
            ItemBOOLEAN ceedItem = (ItemBOOLEAN)Find(CEED);
            return ceedItem.GetDatum(0);
        }

      
        public void AddCEID(String ceid)
        {
            Item ceidList = Find(CEID_LIST);
            ceidList.Add(new ItemA(ceid));
        }

       
        public void AddCEID(String type, int ceid)
        {
            Item ceidList = Find(CEID_LIST);
            ItemN ceidItem = (ItemN)Item.GetItem( type );
            ceidItem.SetDatum( 0, ceid );
            ceidList.Add( ceidItem );
        }

       
        public void AddCEID(String type, Object ceid)
        {
            Item ceidList = Find(CEID_LIST);
            ItemN ceidItem = (ItemN)Item.GetItem( type );
            ceidItem.SetValue( ceid );
            ceidList.Add( ceidItem );
        }

      
        public int GetCEIDCount()
        {
            Item ceidList = Find(CEID_LIST);
            return ceidList.Count();
        }

        
        public String GetCEID(int index)
        {
            return Find(CEID_LIST + "." + index).GetString();
        }

        
        public int GetCEIDInteger( int index )
        {
            return (int)((ItemN)Find(CEID_LIST + "." + index)).GetDatum(0);
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S2F38( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
