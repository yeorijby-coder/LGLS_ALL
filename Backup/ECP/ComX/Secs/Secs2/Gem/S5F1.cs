using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S5F1 : Secs2Message
    {
        public const String LIST= "0";
        public const String ALCD= "0.0";
        public const String ALID= "0.1";
        public const String ALTX= "0.2";

        
        public S5F1() :this((byte)0, "U4", 0, "" )
        {
           
        }

        
        public S5F1(byte alcd,
                    String alidType,
                    int alid,
                    String altx)
            :base(5, 1, null )
        {
         

            ItemN alidItem = (ItemN)Item.GetItem( alidType );
            alidItem.SetDatum( 0, alid );

            SetMessageBody( new ItemL(new ItemB(alcd),
                                  alidItem,
                                  new ItemA(altx)));
        }

        
        public S5F1( SecsMessage secsMessage )
            :base( secsMessage )
        {
           
        }

      
        public void SetALCD( byte alcd )
        {
            ItemB alcdItem = (ItemB)Find(ALCD);
            alcdItem.SetDatum( 0, alcd );
        }

        
        public void SetALCDAlarmOnStatus( bool alarmOn )
        {
            ItemB alcdItem = (ItemB)Find(ALCD);
            byte alcd = alcdItem.GetData()[0];
            if( alarmOn )
            {
                alcd |= 0x80;    // 0x80 = 1000 0000
            }
            else
            {
                alcd &= 0x7F;    // 0x7F = 0111 1111
            }
            alcdItem.SetDatum( 0, alcd );
        }

	    
        public void SetALCDAlarmCategory( byte severity )
        {
            ItemB alcdItem = (ItemB)Find(ALCD);
            byte alcd = alcdItem.GetData()[0];
                // Remove everything except the on/off status
            alcd &= 0x80;    // 0x80 = 1000 0000
                // Add the new severity
            alcd |= severity;
            alcdItem.SetDatum( 0, alcd );
        }

       
        public int GetALCD()
        {
            Item alcd = Find(ALCD);
            return (int)alcd.GetValue(0);
        }

        
        public bool GetALCDAlarmOnStatus()
        {
            Item alcdItem = Find(ALCD);
            int alcd = (int)alcdItem.GetValue(0);
            // High order bit set means Alarm=On.
            // "&" the AlarmCode with "10000000"->(128)
            if ((alcd & 128) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

	    
        public byte GetALCDAlarmCategory()
        {
            Item alcdItem = Find(ALCD);
            int alcd = (int)alcdItem.GetValue(0);
            // 7 Low order bits contain Alarm Severity.
            // "&" the AlarmCode with "01111111"->(127)
            return (byte)(alcd & 127);
        }

        
        public void SetALID( String type, int alid )
        {
            ItemL list = (ItemL)Find(LIST);
            ItemN alidItem = (ItemN)Item.GetItem( type );
            alidItem.SetDatum( 0, alid );
            list.Set( alidItem, 1 );
        }

       
        public int GetALID()
        {
            Item alid = Find(ALID);
            return (int)alid.GetValue(0);
        }

        
        public void SetALTX( String altx )
        {
            Item altxItem = Find(ALTX);
            altxItem.SetValue( altx );
        }

        
        public String GetALTX()
        {
            Item altx = Find(ALTX);
            return (String)altx.GetValue(0);
        }

        
        public override String ToString()
        {
            return "ALCD = " + GetALCD() + ": ALID = " + GetALID() + " (" +
                String.Format("{0:X}", GetALCD())+                   
                    " hex), ALTX = " + GetALTX();
        }

       
        public  override Secs2Message GetSecondary()
        {
            return new S5F2( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
