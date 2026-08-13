using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S2F50 : Secs2Message
    {
        public const String COMMAND_LIST = "0";
        public const String HCACK = "0.0";
        public const String CEPACK_LIST = "0.1";
        public const String CPNAME_SUFFIX = ".0.0";
        public const String CEPACK_SUFFIX = ".0.1";

        // HCACK values
        public const byte DONE = 0;
        public const byte COMMAND_NOT_FOUND = 1;
        public const byte CANNOT_PERFORM = 2;
        public const byte INVALID_PARAMETER = 3;
        public const byte WILL_BE_DONE = 4;
        public const byte ALREADY_DONE = 5;
        public const byte NO_SUCH_OBJECT = 6;

        // CEPACK values
        public const byte NO_ERROR = 0;
        public const byte INVALID_CPNAME = 1;
        public const byte ILLEGAL_VALUE = 2;
        public const byte ILLEGAL_FORMAT = 3;
        public const byte ILLEGAL_USEAGE = 4;

        
        public S2F50( SecsHeader secsHeader )
            :this( secsHeader, DONE )
        {

        }

       
        public S2F50( SecsHeader secsHeader, byte hcack )
            : base( secsHeader, new ItemL(
                                   new ItemB(hcack),
                                   new ItemL()
                                   )
                  )
        {
            
        }

        
        public S2F50( SecsMessage secsMessage )
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

        
        public void AddCPNAME_CEPACK(String cpname, byte cpack)
        {
            Item cpacklist = Find( CEPACK_LIST );
            cpacklist.Add(new ItemL(
                                    new ItemA(cpname),
                                    new ItemB(cpack)
                                    )
                          );
        }

        
        public void AddCPNAME_CPVAL( String type, int cpname, byte cpack )
        {
            Item cpacklist = Find(CEPACK_LIST);
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
            Item plist = Find( CEPACK_LIST );
            return plist.Count();
        }

        
        public String GetCPNAME( int index )
        {
            return Find( CEPACK_LIST + "." + index + CPNAME_SUFFIX ).GetString();
        }

        public int GetCPNAMEInteger( int index )
        {
            ItemN cpname = (ItemN)Find( CEPACK_LIST + "." + index + CPNAME_SUFFIX );
            return (int)cpname.GetDatum(0);
        }

        
        public byte GetCEPACK( int index )
        {
            ItemB cpname = (ItemB)Find( CEPACK_LIST + "." + index + CEPACK_SUFFIX );
            return cpname.GetData()[0];
        }
    }
}
