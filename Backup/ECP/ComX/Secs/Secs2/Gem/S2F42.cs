using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F42 : Secs2Message
    {
        public const String COMMAND_LIST = "0";
        public const String HCACK = "0.0";
        public const String CPACK_LIST = "0.1";
        public const String CPNAME_SUFFIX = ".0.0";
        public const String CPACK_SUFFIX = ".0.1";

        // HCACK values
        public const byte DONE = 0;
        public const byte COMMAND_NOT_FOUND = 1;
        public const byte CANNOT_PERFORM = 2;
        public const byte INVALID_PARAMETER = 3;
        public const byte WILL_BE_DONE = 4;
        public const byte ALREADY_DONE = 5;

        // CPACK values
        public const byte INVALID_CPNAME = 1;
        public const byte ILLEGAL_VALUE = 2;
        public const byte ILLEGAL_FORMAT = 3;

        
        public S2F42( SecsHeader secsHeader )
          :this( secsHeader, DONE )
        {
           
        }

        
        public S2F42( SecsHeader secsHeader, byte hcack )
            :base( secsHeader, new ItemL(
                                   new ItemB(hcack),
                                   new ItemL()
                                   )
                  )
        {
           
        }

        
        public S2F42( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

        
        public void SetHCACK( byte hcack )
        {
            ItemL clist = (ItemL)Find( COMMAND_LIST );
            clist.Set( new ItemB(hcack), 0 );
        }

        
        public byte GetHCACK()
        {
            return ((ItemU1)Find( HCACK )).GetData()[0];
        }

        
        public void AddCPNAME_CPACK(String cpname, byte cpack)
        {
            Item cpacklist = Find( CPACK_LIST );
            cpacklist.Add(new ItemL(
                                    new ItemA(cpname),
                                    new ItemB(cpack)
                                    )
                          );
        }

        
        public void AddCPNAME_CPVAL( String type, int cpname, byte cpack )
        {
            Item cpacklist = Find(CPACK_LIST);
            ItemN item = (ItemN)Item.GetItem(type);
            item.SetDatum( 0, cpname );
            cpacklist.Add(new ItemL(
                                    item,
                                    new ItemB(cpack)
                                    )
                          );
        }

        
        public int GetCPNAME_CPVALCount()
        {
            Item plist = Find( CPACK_LIST );
            return plist.Count();
        }

        
        public String GetCPNAME( int index )
        {
            return Find( CPACK_LIST + "." + index + CPNAME_SUFFIX ).GetString();
        }

        
        public int GetCPNAMEInteger( int index )
        {
            ItemN cpname = (ItemN)Find( CPACK_LIST + "." + index + CPNAME_SUFFIX );
            return (int)cpname.GetDatum(0);
        }

        
        public byte GetCPACK( int index )
        {
            ItemB cpname = (ItemB)Find( CPACK_LIST + "." + index + CPACK_SUFFIX );
            return cpname.GetData()[0];
        }

    }
}
