using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F3 : Secs2Message
    {
        public const String SVID_LIST = "0";

       
        public S1F3()
            : base(1, 3, new ItemL())
        {
           
        }

       
        public S1F3( SecsMessage secsMessage )
            : base( secsMessage )
        {
           
        }

       
        public void AddSVID(String type, int svid)
        {
            Item svidList = Find(SVID_LIST);
            ItemN svidItem = (ItemN)Item.GetItem( type );
            svidItem.SetDatum( 0, svid );
            svidList.Add( svidItem );
        }

        
        public void AddSVID(String svid)
        {
            Item svidList = Find(SVID_LIST);
            svidList.Add(new ItemA(svid));
        }

        
        public void AddSVIDs(String type, int[] svids)
        {
            ItemN svidItem;

            Item svidList = Find(SVID_LIST);
            for (int i = 0; i < svids.Length; i++)
            {
                svidItem = (ItemN)Item.GetItem( type );
                svidItem.SetDatum( 0, svids[i] );
                svidList.Add( svidItem );
            }
        }

        
        public int GetSVIDCount()
        {
            return Find(SVID_LIST).Count();
        }

        
        public String GetSVID( int index )
        {
            return Find(SVID_LIST + "." + index ).GetString();
        }

        
        public int GetSVIDInteger( int index )
        {
            ItemN svid = (ItemN)Find(SVID_LIST + "." + index );
            return (int)svid.GetDatum(0);
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S1F4( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
