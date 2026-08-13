using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F49 : Secs2Message
    {
        public const String COMMAND_LIST = "0";
        public const String DATAID = "0.0";
        public const String OBJSPEC = "0.1";
        public const String RCMD = "0.2";
        public const String PARAMETER_LIST = "0.3";
        public const String CPNAME_SUFFIX = ".0";
        public const String CEPVAL_SUFFIX = ".1";

	    
	    private ItemL cepvalLastUsed = null;

	    
	    private bool containsPairs = true;



        public S2F49()
            : this(0, "", "")
        {
           
        }

       
        public S2F49(int dataid, String objspec, String name)
            :base( 2, 49, new ItemL(
					    new ItemU4(dataid),
					    new ItemA(objspec),
					    new ItemA(name),
					    new ItemL() )
				    )
        {
          
        }

       
        public S2F49( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

        
        public void SetDATAID( String type, Object dataid )
        {
            ItemL list = (ItemL)Find( COMMAND_LIST );
            Item item = Item.GetItem( type );
            item.SetValue( dataid );
            list.Set( item, 0 );
        }

        
        public void SetDATAID( String dataId )
        {
            ItemL list = (ItemL)Find( COMMAND_LIST );
            list.Set( new ItemA(dataId), 0 );
        }

       
        public void SetDATAID( String type, int dataId )
        {
            ItemL list = (ItemL)Find( COMMAND_LIST );
            ItemN dataIdItem = (ItemN)Item.GetItem( type );
            dataIdItem.SetDatum( 0, dataId );
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

        
        public void SetOBJSPEC( String objspec )
        {
            ItemL clist = (ItemL)Find( COMMAND_LIST );
            clist.Set( new ItemA( objspec ), 1 );
        }

       
        public String GetOBJSPEC()
        {
            return Find( OBJSPEC ).GetString();
        }

        
        public void SetRCMD( String type, Object rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            Item item = Item.GetItem(type);
            item.SetValue( rcmd );
            clist.Set( item, 2 );
        }

        
        public void SetRCMD( String rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            clist.Set( new ItemA( rcmd ), 2 );
        }

       
        public void SetRCMD( byte rcmd )
        {
            ItemL clist = (ItemL)Find(COMMAND_LIST);
            clist.Set( new ItemU1( rcmd ), 2 );
        }

       
        public String GetRCMD()
        {
            return Find( RCMD ).GetString();
        }

        
        public void AddCPNAME_CEPVAL( String cpname )
        {
            Item cplist = Find( PARAMETER_LIST );
            cepvalLastUsed = new ItemL();
            cplist.Add(new ItemL(
					    new ItemA(cpname),
					    cepvalLastUsed
					    )
				    );
        }

        
        public void AddCPNAME_CEPVAL( String type, Object cpname )
        {
            Item cplist = Find( PARAMETER_LIST );
            Item nitem = Item.GetItem(type);
            nitem.SetValue(cpname);
            cepvalLastUsed = new ItemL();
            cplist.Add(new ItemL(
					    nitem,
					    cepvalLastUsed
					    )
				    );
        }

        
        public void AddCPNAME_CPVAL(String cpname, String type, Object cpval)
        {
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
            cepvalLastUsed.Add(new ItemL(
					    new ItemA(cpname),
					    item
					    )
				    );
        }

        
        public void AddCPNAME_CPVAL(
                    String ntype, Object cpname, String vtype, Object cpval)
        {
            Item nitem = Item.GetItem(ntype);
            nitem.SetValue(cpname);
            Item vitem;

		    if( cpval is Item )
		    {
			    vitem = (Item)cpval;
		    }
		    else
		    {
			    vitem = Item.GetItem(vtype);
			    vitem.SetValue(cpval);
		    }
            cepvalLastUsed.Add(new ItemL(
					    nitem,
					    vitem
					    )
				    );
        }

        
        public void addCPVAL(String type, Object cpval)
        {
            Item item;
		    if( cpval is Item )
		    {
			    item = (Item)cpval;
		    }
		    else
		    {
			    item = Item.GetItem(type);
			    item.SetValue(cpval);
		    }
            cepvalLastUsed.Add( item );
        }


        
        public void addCPNAME_CEPVAL(String cpname, String type, Object cepval)
        {
            Item cplist = Find( PARAMETER_LIST );
            Item item;
		    if( cepval is Item )
		    {
			    item = (Item)cepval;
		    }
		    else
		    {
			    item = Item.GetItem(type);
			    item.SetValue(cepval);
		    }
            cplist.Add(new ItemL(
					    new ItemA(cpname),
					    item
					    )
				    );
        }

        
        public void AddCPNAME_CEPVAL(
                    String ntype, Object cpname, String vtype, Object cepval)
        {
            Item cplist = Find( PARAMETER_LIST );
            Item nitem = Item.GetItem(ntype);
            nitem.SetValue(cpname);
            Item vitem;
		    // If they sent us an item, we can ignore the type and just use it as is
		    if( cepval is Item )
		    {
			    vitem = (Item)cepval;
		    }
		    else
		    {
			    vitem = Item.GetItem(vtype);
			    vitem.SetValue(cepval);
		    }
            cplist.Add(new ItemL(
					    nitem,
					    vitem
					    )
				    );
        }

        
        public int getCPNAME_CEPVALCount()
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

        
        public String GetCEPVAL( int index )
        {
            return Find( PARAMETER_LIST + "." + index + CEPVAL_SUFFIX ).GetString();
        }

       
        public byte[] GetCEPVALBinary( int index )
        {
            ItemB cepval = (ItemB)Find(
                    PARAMETER_LIST + "." + index + CEPVAL_SUFFIX );
            return cepval.GetData();
        }

       
        public int GetCEPVALInteger( int index )
        {
            ItemN cepval = (ItemN)Find(
                    PARAMETER_LIST + "." + index + CEPVAL_SUFFIX );
            return (int)cepval.GetDatum(0);
        }

       
        public bool GetCEPVALBoolean( int index )
        {
            ItemBOOLEAN cepval = (ItemBOOLEAN)Find( PARAMETER_LIST + "." + index + CEPVAL_SUFFIX );
            return cepval.GetDatum(0);
        }

       
        public Item GetCEPVALItem( int index )
        {
            return Find( PARAMETER_LIST + "." + index + CEPVAL_SUFFIX );
        }
    	
        
        public int GetCPNAME_CPVALCount( int cepvalIndex )
        {
            cepvalLastUsed = (ItemL)Find( PARAMETER_LIST + "." + cepvalIndex +
				    CEPVAL_SUFFIX );
		    containsPairs = true;
            return cepvalLastUsed.Count();
        }

        
        public int GetCPVALCount( int cepvalIndex )
        {
            cepvalLastUsed = (ItemL)Find( PARAMETER_LIST + "." + cepvalIndex +
				    CEPVAL_SUFFIX );
		    containsPairs = false;
            return cepvalLastUsed.Count();
        }

        
        public String GetCEPVAL_CPNAME( int index )
        {
		    return( ((ItemL)cepvalLastUsed.Get(index)).Get(0).GetString() );
        }

       
        public Item GetCEPVAL_CPNAMEItem( int index )
        {
		    return( ((ItemL)cepvalLastUsed.Get(index)).Get(0) );
        }

        
        public String GetCEPVAL_CPVAL( int index )
        {
		    if( containsPairs ) 
            {
			    return( ((ItemL)cepvalLastUsed.Get(index)).Get(1).GetString() );
		    }
		    else 
            {
			    return( cepvalLastUsed.Get(index).GetString() );
		    }
        }

        
        public Item GetCEPVAL_CPVALItem( int index )
        {
		    if( containsPairs ) 
            {
			    return( ((ItemL)cepvalLastUsed.Get(index)).Get(1) );
		    }
		    else 
            {
			    return( cepvalLastUsed.Get(index) );
		    }
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S2F50( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
