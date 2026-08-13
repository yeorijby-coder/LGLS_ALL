using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F33 : Secs2Message
    {
        public const String LIST = "0";
        public const String DATAID = "0.0";
        public const String RPTID_LIST = "0.1";
        public const String RPTID_SUFFIX = ".0";
        public const String VID_LIST_SUFFIX = ".1";

        private ItemL lastRptid = null;

      
        public S2F33()
            :base(2, 33, new ItemL(
                                   new ItemU4(0),
                                   new ItemL()
                                   )
                  )
        {
           
        }

        
        public S2F33( SecsMessage secsMessage )
            : base( secsMessage )
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
            list.Set( dataIdItem, 0 );
        }

        
        public String GetDATAID()
        {
            return Find(DATAID).GetString();
        }

        
        public int getDATAIDInteger()
        {
            ItemN dataId = (ItemN)Find(DATAID);
            return (int)dataId.GetDatum(0);
        }

        
        public void AddRPTID( String rptid )
        {
            Item rptList = Find( RPTID_LIST );
            ItemL report = new ItemL( new ItemA(rptid), new ItemL() );
            rptList.Add( report );
            lastRptid = report;
        }

       
        public void AddRPTID( String type, int rptid )
        {
            Item rptList = Find( RPTID_LIST );
            ItemN rptidItem = (ItemN)Item.GetItem( type );
            rptidItem.SetDatum( 0, rptid );
            ItemL report = new ItemL( rptidItem, new ItemL() );
            rptList.Add( report );
            lastRptid = report;
        }

        
        public void AddRPTID( String type, Object rptid )
        {
            Item rptList = Find( RPTID_LIST );
            ItemN rptidItem = (ItemN)Item.GetItem( type );
            rptidItem.SetValue( rptid );
            ItemL report = new ItemL( rptidItem, new ItemL() );
            rptList.Add( report );
            lastRptid = report;
        }

        
        public int getReportCount()
        {
            Item reportList = Find(RPTID_LIST);
            return reportList.Count();
        }

        
        public int GetRPTIDCount()
        {
            Item reportList = Find(RPTID_LIST);
            return reportList.Count();
        }

        
        public String GetRPTID(int index)
        {
            return Find( RPTID_LIST + "." + index + RPTID_SUFFIX ).GetString();
        }

        
        public int GetRPTIDInteger(int index)
        {
            ItemN rptid = (ItemN)Find( RPTID_LIST + "." + index + RPTID_SUFFIX );
            return (int)rptid.GetDatum(0);
        }

        
        public void AddVID( String vid )
        {
            ItemL vidList = (ItemL)lastRptid.Get(1); // Get the VID list
            vidList.Add( new ItemA(vid) );
        }

        
        public void AddVID( String type, int vid )
        {
            ItemL vidList = (ItemL)lastRptid.Get(1); // Get the VID list
            ItemN vidItem = (ItemN)Item.GetItem( type );
            vidItem.SetDatum( 0, vid );
            vidList.Add( vidItem );
        }

        
        public void AddVID( String type, Object vid )
        {
            ItemL vidList = (ItemL)lastRptid.Get(1); // Get the VID list
            ItemN vidItem = (ItemN)Item.GetItem( type );
            vidItem.SetValue( vid );
            vidList.Add( vidItem );
        }

        
        public int GetVIDCount( int index )
        {
            Item vidList = Find(RPTID_LIST + "." + index + VID_LIST_SUFFIX );
            return vidList.Count();
        }

        
        public String GetVID( int reportIndex, int vidIndex )
        {
            return Find(
                RPTID_LIST + "." + reportIndex + VID_LIST_SUFFIX + "." + vidIndex
                ).GetString();
        }

        
        public int GetVIDInteger( int reportIndex, int vidIndex )
        {
            ItemN vid = (ItemN)Find(
                RPTID_LIST + "." + reportIndex + VID_LIST_SUFFIX + "." + vidIndex
                );
            return (int)vid.GetDatum( 0 );
        }

        
        public  override Secs2Message GetSecondary()
        {
            return new S2F34( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
