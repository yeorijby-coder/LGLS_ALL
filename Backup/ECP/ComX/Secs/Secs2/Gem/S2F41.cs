using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public  class S2F41 : Secs2Message
    {
        public const String COMMAND_LIST = "0";
        public const String RCMD = "0.0";
        public const String PARAMETER_LIST = "0.1";
        public const String CPNAME_SUFFIX = ".0";
        public const String CPVAL_SUFFIX = ".1";

        
        public S2F41():this("")
        {
            
        }

        public S2F41(String name)
            : base(2, 41, new ItemL(
                                   new ItemA(name),
                                   new ItemL()
                                   )
                  )
        {
            
        }

        
        public S2F41(byte rcmd)
            : base(2, 41, new ItemL(
                                   new ItemU1(rcmd),
                                   new ItemL()
                                   )
                  )
        {
            
        }

        
        public S2F41( SecsMessage secsMessage )
           : base( secsMessage )
        {
            
        }

       
        public void SetRCMD( String type, Object rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            Item item = Item.GetItem(type);
            item.SetValue( rcmd );
            clist.Set( item, 0 );
        }

        
        public void SetRCMD( String rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            clist.Set( new ItemA( rcmd ), 0 );
        }

        
        public void SetRCMD( byte rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            clist.Set( new ItemU1( rcmd ), 0 );
        }

        
        public void SetRCMD( int rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            clist.Set( new ItemI1( rcmd ), 0 );
        }

        
        public String GetRCMD()
        {
            return Find( RCMD ).GetString();
        }

        
        public int GetRCMDInteger()
        {
            ItemN rcmd = (ItemN)Find( RCMD );
            return (int)rcmd.GetDatum(0);
        }

        
        public void AddCPNAME_CPVAL(String cpname, String type, Object cpval)
        {
            Item cplist = Find( PARAMETER_LIST );
            Item item;
		    // If they sent us an item, we can ignore the type and just use it as is
		    if( cpval is Item )
		    {
			    item = (Item)cpval;
		    }
		    else
		    {
			    item = Item.GetItem(type);
			    item.SetValue(cpval);
		    }
            cplist.Add(new ItemL(
                                 new ItemA(cpname),
                                 item
                                 )
                       );
        }

        
        public void AddCPNAME_CPVAL(
                    String ntype, Object cpname, String vtype, Object cpval)
        {
            Item cplist = Find( PARAMETER_LIST );
            Item nitem = Item.GetItem(ntype);
            nitem.SetValue(cpname);
            Item vitem;
		    // If they sent us an item, we can ignore the type and just use it as is
		    if( cpval is Item )
		    {
			    vitem = (Item)cpval;
		    }
		    else
		    {
			    vitem = Item.GetItem(vtype);
			    vitem.SetValue(cpval);
		    }
            cplist.Add(new ItemL(
                                 nitem,
                                 vitem
                                 )
                       );
        }

        
        public int GetCPNAME_CPVALCount()
        {
            Item plist = Find( PARAMETER_LIST );
            return plist.Count();
        }

        
        public String GetCPNAME( int index )
        {
            return Find( PARAMETER_LIST + "." + index + CPNAME_SUFFIX ).GetString();
        }

        
        public int GetCPNAMEInteger( int index )
        {
            ItemN cpname = (ItemN)Find(
                    PARAMETER_LIST + "." + index + CPNAME_SUFFIX );
            return (int)cpname.GetDatum(0);
        }

       
        public String GetCPVAL( int index )
        {
            return Find( PARAMETER_LIST + "." + index + CPVAL_SUFFIX ).GetString();
        }

        
        public byte[] GetCPVALBinary( int index )
        {
            ItemB cpval = (ItemB)Find(
                    PARAMETER_LIST + "." + index + CPVAL_SUFFIX );
            return cpval.GetData();
        }

        
        public int GetCPVALInteger( int index )
        {
            ItemN cpval = (ItemN)Find(
                    PARAMETER_LIST + "." + index + CPVAL_SUFFIX );
            return (int)cpval.GetDatum(0);
        }

        
        public bool GetCPVALBoolean( int index )
        {
            ItemBOOLEAN cpval = (ItemBOOLEAN)Find( PARAMETER_LIST + "." + index + CPVAL_SUFFIX );
            return cpval.GetDatum(0);
        }

        
        public Item GetCPVALItem( int index )
        {
            return Find( PARAMETER_LIST + "." + index + CPVAL_SUFFIX );
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S2F42( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
