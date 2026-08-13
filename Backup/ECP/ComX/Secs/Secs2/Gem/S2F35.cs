using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F35 : Secs2Message
    {
        public const String LIST = "0";
        public const String DATAID = "0.0";
        public const String CEID_LIST = "0.1";
        public const String CEID_SUFFIX = ".0";
        public const String RPTID_LIST_SUFFIX = ".1";

        private ItemL lastCeid;

        
        public S2F35()
            :base(2, 35, new ItemL(
                                   new ItemU4(0),
                                   new ItemL()
                                   )
                  )
        {
          
        }

       
        public S2F35( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

       
        public void SetDATAID( String dataId )
        {
            ItemL list = (ItemL)Find( LIST );
            list.Set( new ItemA(dataId), 0 );
        }

        public void SetDATAID( String type, int dataId )
        {
            ItemL list = (ItemL)Find( LIST );
            ItemN dataIdItem = (ItemN)Item.GetItem( type );
            dataIdItem.SetDatum( 0, dataId );
            list.Set( dataIdItem,0 );
        }

        
        public void SetDATAID( String type, Object dataId )
        {
            ItemL list = (ItemL)Find( LIST );
            ItemN dataIdItem = (ItemN)Item.GetItem( type );
            dataIdItem.SetValue( dataId );
            list.Set( dataIdItem,0 );
        }

        
        public String GetDATAID()
        {
            return Find(DATAID).GetString();
        }

        
        public int GetDATAIDInteger()
        {
            ItemN dataId = (ItemN)Find(DATAID);
            return (int)dataId.GetDatum(0);
        }


       
        public void AddCEID( String ceid )
        {
            Item ceidList = Find( CEID_LIST );
            ItemL ceidItem = new ItemL( new ItemA(ceid), new ItemL() );
            ceidList.Add( ceidItem );
            lastCeid = ceidItem;
        }

       
        public void AddCEID( String type, int ceid )
        {
            Item ceidList = Find( CEID_LIST );
            ItemN ceidItem = (ItemN)Item.GetItem( type );
            ceidItem.SetDatum( 0, ceid );
            ItemL ceidListItem = new ItemL( ceidItem, new ItemL() );
            ceidList.Add( ceidListItem );
            lastCeid = ceidListItem;
        }

      
        public void AddCEID( String type, Object ceid )
        {
            Item ceidList = Find( CEID_LIST );
            ItemN ceidItem = (ItemN)Item.GetItem( type );
            ceidItem.SetValue( ceid );
            ItemL ceidListItem = new ItemL( ceidItem, new ItemL() );
            ceidList.Add( ceidListItem );
            lastCeid = ceidListItem;
        }

        
        public int GetCEIDCount()
        {
            Item ceidList = Find(CEID_LIST);
            return ceidList.Count();
        }

        
        public String GetCEID(int index)
        {
            return Find( CEID_LIST + "." + index + CEID_SUFFIX ).GetString();
        }

        
        public int GetCEIDInteger(int index)
        {
            ItemN ceid = (ItemN)Find( CEID_LIST + "." + index + CEID_SUFFIX );
            return (int)ceid.GetDatum( 0 );
        }

        
        public void AddRPTID( String rptid )
        {
            ItemL rptidList = (ItemL)lastCeid.Get(1); // Get the RPTID list
            rptidList.Add( new ItemA(rptid) );
        }

        
        public void AddRPTID( String type, int rptid )
        {
            ItemL rptidList = (ItemL)lastCeid.Get(1); // Get the RPTID list
            ItemN rptidItem = (ItemN)Item.GetItem( type );
            rptidItem.SetDatum( 0, rptid );
            rptidList.Add( rptidItem );
        }

       
        public void AddRPTID( String type, Object rptid )
        {
            ItemL rptidList = (ItemL)lastCeid.Get(1); // Get the RPTID list
            ItemN rptidItem = (ItemN)Item.GetItem( type );
            rptidItem.SetValue( rptid );
            rptidList.Add( rptidItem );
        }

        
        public int GetRPTIDCount( int index )
        {
            Item rptidList = Find(CEID_LIST + "." + index + RPTID_LIST_SUFFIX );
            return rptidList.Count();
        }

        
        public String GetRPTID( int ceidIndex, int rptidIndex )
        {
            return Find(
                CEID_LIST + "." + ceidIndex + RPTID_LIST_SUFFIX + "." + rptidIndex
                ).GetString();
        }

        
        public int GetRPTIDInteger( int ceidIndex, int rptidIndex )
        {
            ItemN rptid = (ItemN)Find(
                CEID_LIST + "." + ceidIndex + RPTID_LIST_SUFFIX + "." + rptidIndex
                );
            return (int)rptid.GetDatum( 0 );
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S2F36( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
