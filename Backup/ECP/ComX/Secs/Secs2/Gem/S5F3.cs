using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S5F3 : Secs2Message
    {
        public const String LIST = "0";
        public const String ALED = "0.0";
        public const String ALID = "0.1";

        public const byte ENABLE = (byte)0x80;
        public const byte DISABLE = (byte)0x00;

        
        public S5F3()
            : this(true, new int[0])
        {
          
        }

        
        public S5F3(bool enabled)
            : this(enabled, new int[0] )
        {
        
        }

        
        public S5F3(int alarmid)
            : this(true, alarmid)
        {
           
        }

        
        public S5F3(bool enabled, int alarmid)
            :base(5, 3, new ItemL(
                                   new ItemB(enabled ? ENABLE : DISABLE),
                                   new ItemU4(alarmid)
                                   )
                  )
        {
            
        }

       
        public S5F3(bool enabled, int[] alarmidList)
            :base(5, 3, new ItemL(
                                   new ItemB(enabled ? ENABLE : DISABLE),
                                   new ItemU4(alarmidList)
                                   )
                  )
        {
            
        }

        
        public S5F3( SecsMessage secsMessage )
            :base( secsMessage )
        {
          
        }

        
        public void SetALED( bool enabled )
        {
            ItemB aled = (ItemB)Find(ALED);
            aled.SetDatum( 0, enabled ? ENABLE : DISABLE );
        }

        
        public bool GetALED( )
        {
            ItemB aled = (ItemB)Find(ALED);
            return aled.GetData()[0] == ENABLE ? true : false;
        }

        
        public void SetALID( String type, int alarmid )
        {
            ItemL list = (ItemL)Find( LIST );

            ItemN alidItem = (ItemN)Item.GetItem( type );
            alidItem.SetDatum( 0, alarmid );

            list.Set( alidItem, 1 );
        }

        
        public void SetALID( String type, int[] alarmids )
        {
            ItemL list = (ItemL)Find( LIST );

            ItemN alidItem = (ItemN)Item.GetItem( type );
            alidItem.Init( alarmids.Length );
            for( int i=0; i<alarmids.Length; i++ )
            {
                alidItem.SetDatum( i, alarmids[i] );
            }

            list.Set( alidItem, 1 );
        }

       
        public int GetALIDCount()
        {
            ItemN alid = (ItemN)Find( ALID );
            return alid.Count();
        }

        
        public int GetALID( int index )
        {
            ItemN alid = (ItemN)Find( ALID );
            return (int)alid.GetDatum( index );
        }

        
        public override Secs2Message GetSecondary()
        {
            return new S5F4( GetSecsHeader().NewSecondaryHeader() );
        }
    }
}
