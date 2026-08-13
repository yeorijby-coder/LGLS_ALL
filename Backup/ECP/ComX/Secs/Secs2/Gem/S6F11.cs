using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S6F11 : Secs2Message
    {
        public const String LIST = "0";
        public const String DATAID = "0.0";
        public const String CEID = "0.1";
        public const String RPTID_LIST = "0.2";
        public const String RPTID_SUFFIX = ".0";
        public const String V_LIST_SUFFIX = ".1";

        
        private ItemL lastRptid;


        
        public S6F11()
            :base( 6, 11, new ItemL(
                                   new ItemU4(0),  // blank data id
                                   new ItemU4(0),  // blank ceid
                                   new ItemL()      // no reports
                                   ))
        {
            
        }

        
        public S6F11( SecsMessage secsMessage )
            : base(secsMessage)
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
            list.Set( dataIdItem, 0 );
        }

        
        public void SetDATAID( String type, Object dataId )
        {
            ItemL list = (ItemL)Find( LIST );
            Item dataIdItem = Item.GetItem( type );
            dataIdItem.SetValue( dataId );
            list.Set( dataIdItem, 0 );
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

        
        public void SetCEID( String ceid )
        {
            ItemL list = (ItemL)Find( LIST );
            list.Set( new ItemA(ceid), 1 );
        }

        
        public void SetCEID( String type, int ceid )
        {
            ItemL list = (ItemL)Find( LIST );
            ItemN ceidItem = (ItemN)Item.GetItem( type );
            ceidItem.SetDatum( 0, ceid );
            list.Set( ceidItem, 1 );
        }

        
        public void SetCEID( String type, Object ceid )
        {
            ItemL list = (ItemL)Find( LIST );
            Item ceidItem = Item.GetItem(type);
            ceidItem.SetValue( ceid );
            list.Set( ceidItem, 1 );
        }

        
        public String GetCEID()
        {
            return Find(CEID).GetString();
        }

       
        public int GetCEIDInteger()
        {
            ItemN ceid = (ItemN)Find(CEID);
            return (int)ceid.GetDatum(0);
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
            ItemL rptList = (ItemL)Find( RPTID_LIST );
            ItemN rptidItem = (ItemN)Item.GetItem( type );
            rptidItem.SetDatum( 0, rptid );
            ItemL report = new ItemL( rptidItem, new ItemL() );
            rptList.Add( report );
            lastRptid = report;
        }

        
        public void AddRPTID( String type, Object rptid )
        {
            Item rptList = Find( RPTID_LIST );
            Item rptidItem = Item.GetItem( type );
            rptidItem.SetValue( rptid );
            ItemL report = new ItemL( rptidItem, new ItemL() );
            rptList.Add( report );
            lastRptid = report;
        }

       
        public int GetRPTIDCount()
        {
            Item reportList = Find(RPTID_LIST);
            return reportList.Count();
        }

        public String GetRPTID(int index)
        {
            Item rptid = Find( RPTID_LIST + "." + index + RPTID_SUFFIX );
            if (rptid != null)
            {
                return rptid.GetString();
            }
            else
            {
                return "";
            }
        }

       
        public int GetRPTIDInteger(int index)
        {
            ItemN rptid = (ItemN)Find( RPTID_LIST + "." + index + RPTID_SUFFIX );
            if (rptid != null)
            {
                return (int)rptid.GetDatum( index );
            }
            else
            {
                return 0;
            }
        }


        
        public void AddV( Item v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            vList.Add( v );
        }

        
        public void AddV( String v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            vList.Add( new ItemA(v) );
        }

        
        public void AddV( byte[] v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            vList.Add( new ItemB(v) );
        }

        
        public void AddV( bool v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            vList.Add( new ItemBOOLEAN(v) );
        }

        
        public void AddV( String type, double v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            ItemN vItem = (ItemN)Item.GetItem( type );
            vItem.SetDatum( 0, v );
            vList.Add( vItem );
        }

        
        public void AddV( String type, Object v )
        {
            ItemL vList = (ItemL)lastRptid.Get(1); // Get the V list
            Item vItem = Item.GetItem( type );
            vItem.SetValue( v );
            vList.Add( vItem );
        }

        
        public int GetVCount( int index )
        {
            ItemL vList = (ItemL)Find( RPTID_LIST + "." + index + V_LIST_SUFFIX );
            if (vList != null)
            {
                return vList.Count();
            }
            else
            {
                return 0;
            }
        }

        
        public String GetV(int rptIndex, int valIndex)
        {
            return Find(
                    RPTID_LIST + "." + rptIndex + V_LIST_SUFFIX + "." + valIndex ).GetString();
        }

        
        public byte[] GetVBinary(int rptIndex, int valIndex)
        {
            ItemB v = (ItemB)Find(
                    RPTID_LIST + "." + rptIndex + V_LIST_SUFFIX + "." + valIndex );
            return v.GetData();
        }

        
        public Item GetVItem(int rptIndex, int valIndex)
        {
            return Find(
                    RPTID_LIST + "." + rptIndex + V_LIST_SUFFIX + "." + valIndex );
        }

        
        public double GetVInteger(int rptIndex, int valIndex)
        {
            ItemN v = (ItemN)Find(
                    RPTID_LIST + "." + rptIndex + V_LIST_SUFFIX + "." + valIndex );
            return v.GetDatum(0);
        }

        
        public bool GetVBoolean(int rptIndex, int valIndex)
        {
            ItemBOOLEAN v = (ItemBOOLEAN)Find(
                    RPTID_LIST + "." + rptIndex + V_LIST_SUFFIX + "." + valIndex );
            return v.GetDatum(0);
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S6F12( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
