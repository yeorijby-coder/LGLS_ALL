using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;


namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F15 : Secs2Message
    {
        public const String EC_LIST = "0";
        public const String ECID_SUFFIX = ".0";
        public const String ECV_SUFFIX = ".1";

        
        public S2F15()
            : base (2, 15, new ItemL())
        {

        }

        
        public S2F15( SecsMessage secsMessage )
            :base(secsMessage)
        {

        }

        
        public void AddECID_ECV(String ecidType, Object ecid,
                                        String ecvType, Object ecv )
        {
            Item ecList = Find(EC_LIST);

            Item ecidItem = Item.GetItem(ecidType);
            ecidItem.SetValue(ecid);

            Item ecvItem = Item.GetItem(ecvType);
            ecvItem.SetValue(ecv);

            ecList.Add(new ItemL(
                                 ecidItem,
                                 ecvItem
                                 )
                       );
        }

        
        public void AddECID_ECV(String ecidType, Object ecid, String[] ecv )
        {
            Item ecList = Find(EC_LIST);

            Item ecidItem = Item.GetItem(ecidType);
            ecidItem.SetValue(ecid);

            ItemL ecvItem = new ItemL();
            for (int i = 0; i < ecv.Length; i++)
            {
                ecvItem.Add(new ItemA(ecv[i]));
            }

            ecList.Add(new ItemL(
                                 ecidItem,
                                 ecvItem
                                 )
                       );
        }

        
        public void AddECID_ECV(String ecid,
                                        String ecvType, int ecv )
        {
            Item ecList = Find(EC_LIST);

            ItemN ecvItem = (ItemN)Item.GetItem(ecvType);
            ecvItem.SetDatum( 0, ecv );

            ecList.Add(new ItemL(
                                 new ItemA( ecid ),
                                 ecvItem
                                 )
                       );
        }

        
        public void AddECID_ECV(String ecidType, int ecid,
                                        String ecvType, int ecv )
        {
            Item ecList = Find(EC_LIST);

            ItemN ecidItem = (ItemN)Item.GetItem(ecidType);
            ecidItem.SetDatum( 0, ecid );

            ItemN ecvItem = (ItemN)Item.GetItem(ecvType);
            ecvItem.SetDatum( 0, ecv );

            ecList.Add(new ItemL(
                                 ecidItem,
                                 ecvItem
                                 )
                       );
        }

        
        public int GetECIDCount()

        {
            return Find(EC_LIST).Count();
        }

        
        public String GetECID( int index )
        {
            return Find(EC_LIST + "." + index + ECID_SUFFIX).GetString();
        }

        
        public int GetECIDInteger( int index )
        {
            return (int)((ItemN)Find(EC_LIST + "." + index + ECID_SUFFIX)).GetDatum(0);
        }

       
        public String GetECV( int index )
        {
            return Find(EC_LIST + "." + index + ECV_SUFFIX).GetString();
        }

        
        public int GetECVInteger( int index )
        {
            return (int)((ItemN)Find(EC_LIST + "." + index + ECV_SUFFIX)).GetDatum(0);
        }

       
        public  override Secs2Message GetSecondary()
        {
            return new S2F16( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
