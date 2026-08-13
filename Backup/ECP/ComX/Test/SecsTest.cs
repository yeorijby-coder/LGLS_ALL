using System;
using System.Collections.Generic;
using System.Text;

using ECP.ComX.Secs;
using ECP.ComX.Secs.Secs2;
using ECP.ComX.Secs.Secs2.Gem;

using ECP.Util;

namespace ECP.ComX.Test
{
    public class SecsTest
    {
        private const String CLASS = "SecsTest";

	    Frame1 frame;

        public SecsTest( Frame1 frame )
        {
		    this.frame = frame;
        }

        public Secs2Message BuildBigMultiBlock()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 1 );
            s2m.AddRPTID( "U4", 1 );
            s2m.AddV( new byte[20000] );
            return s2m;
        }

        public Secs2Message BuildTestBoolean()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 1 );
            s2m.AddRPTID( "U4", 1 );
            s2m.AddV( new ItemA("These values should all be true") );
            s2m.AddV( new ItemBOOLEAN("1") );
            s2m.AddV( new ItemBOOLEAN("true") );
            s2m.AddV( new ItemBOOLEAN("256") );
            s2m.AddV( new ItemBOOLEAN("Whatever") );
            s2m.AddV( new ItemBOOLEAN(true) );
            s2m.AddV( new ItemBOOLEAN(true) );
            s2m.AddV( new ItemA("These values should all be false") );
            s2m.AddV( new ItemBOOLEAN("false") );
            s2m.AddV( new ItemBOOLEAN("0") );
            s2m.AddV( new ItemBOOLEAN(false) );
            s2m.AddV( new ItemBOOLEAN(false) );
            return s2m;
        }
        
        public Secs2Message BuildInstall()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );

		    String carrierId = frame.Ask(
				    "Please input the carrier id: " );
		    if( carrierId == null ) { return null; }
		    String carrierLoc = frame.Ask(
				    "Please input the carrier location: " );
		    if( carrierLoc == null ) { return null; }

		    try
		    {
               s2m.MessageBodyFromSMLString(
			    "<L[2]" +
			    "  <A 'INSTALL'>" +
			    "  <L[2]" +
			    "    <L[2]" +
			    "      <A CARRIERID>" +
			    "      <A \"" + carrierId + "\">" +
			    "    >" +
			    "    <L[2]" +
			    "      <A CARRIERLOC>" +
			    "      <A \"" + carrierLoc + "\">" +
			    "    >" +
			    "  >" +
			    ">"
			    );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }

        public Secs2Message BuildRemove()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );

		    String carrierId = frame.Ask(
				    "Please input the carrier id: " );
		    if( carrierId == null ) { return null; }

		    try
		    {
               s2m.MessageBodyFromSMLString(
				    "<L[2]" +
				    "  <A 'REMOVE'>" +
				    "  <L[1]" +
				    "    <L[2]" +
				    "      <A CARRIERID>" +
				    "      <A \"" + carrierId + "\">" +
				    "    >" +
				    "  >" +
				    ">"
                  );
		    }
		    catch( Exception e )
		    {
                Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }


        public Secs2Message BuildPause()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );

		    try
		    {
               s2m.MessageBodyFromSMLString(
                  "<L [2] " +
                  "    <A PAUSE>" +
                  "    <L [0]>" +
                  ">"
                  );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }

        public Secs2Message BuildResume()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );

		    try
		    {
               s2m.MessageBodyFromSMLString(
                  "<L [2] " +
                  "    <A RESUME>" +
                  "    <L [0]>" +
                  ">"
                  );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }


        public Secs2Message BuildCancel()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );
		    String commandId = frame.Ask(
				    "Please input the transfer command id: " );
		    if( commandId == null ) { return null; }

		    try
		    {
               s2m.MessageBodyFromSMLString(
                    "<L [2]" +
                    "    <A CANCEL>" +
                    "    <L [1]" +
                    "        <L [2]" +
                    "            <A COMMANDID>" +
                    "            <A \"" + commandId + "\">" +
                    "        >" +
                    "    >" +
                    ">"
                    );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }

        public Secs2Message BuildAbort()
        {
            Secs2Message s2m = new Secs2Message( 2, 41, null );
		    String commandId = frame.Ask(
				    "Please input the transfer command id: " );
		    if( commandId == null ) { return null; }

		    try
		    {
               s2m.MessageBodyFromSMLString(
                    "<L [2]" +
                    "    <A ABORT>" +
                    "    <L [1]" +
                    "        <L [2]" +
                    "            <A COMMANDID>" +
                    "            <A \"" + commandId + "\">" +
                    "        >" +
                    "    >" +
                    ">"
                    );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }

        public Secs2Message BuildTransfer()
        {
            Secs2Message s2m = new Secs2Message( 2, 49, null );
		    String commandId = frame.Ask(
				    "Please input the transfer command id: " );
		    if( commandId == null ) { return null; }
		    String carrierId = frame.Ask(
				    "Please input the carrier id: " );
		    if( carrierId == null ) { return null; }
		    String source = frame.Ask(
				    "Please input the source: " );
		    if( source == null ) { return null; }
		    String dest = frame.Ask(
				    "Please input the dest: " );
		    if( dest == null ) { return null; }

		    try
		    {
               s2m.MessageBodyFromSMLString(
				    "<L [4]" +
				    "	<U2 0>" +
				    "	<A \"\">" +
				    "	<A TRANSFER>" +
				    "	<L[2]" +
				    "		<L[2]" +
				    "			<A COMMANDINFO>" +
				    "			<L [3]" +
				    "				<L [2]" +
				    "					<A COMMANDID>" +
				    "					<A \"" + commandId + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A PRIORITY>" +
				    "					<U2 99>" +
				    "				>" +
				    "			>" +
				    "		>" +
				    "		<L [2]" +
				    "			<A TRANSFERINFO>" +
				    "			<L [3]" +
				    "				<L [2]" +
				    "					<A CARRIERID>" +
				    "					<A \"" + carrierId + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A SOURCEPORT>" +
				    "					<A \"" + source + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A DESTPORT>" +
				    "					<A \"" + dest + "\">" +
				    "				>" +
				    "			>" +
				    "		>" +
				    "	>" +
				    ">"
				    );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    return s2m;
        }

        public Secs2Message[] BuildTransfer100()
        {
            Secs2Message s2m = new Secs2Message( 2, 49, null );
		    String commandId = "100";
		    String carrierId = "Carrier100";
		    String source = "Source100";
		    String dest = "Dest100";

		    try
		    {
               s2m.MessageBodyFromSMLString(
				    "<L [4]" +
				    "	<U2 0>" +
				    "	<A \"\">" +
				    "	<A TRANSFER>" +
				    "	<L[2]" +
				    "		<L[2]" +
				    "			<A COMMANDINFO>" +
				    "			<L [3]" +
				    "				<L [2]" +
				    "					<A COMMANDID>" +
				    "					<A \"" + commandId + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A PRIORITY>" +
				    "					<U2 99>" +
				    "				>" +
				    "			>" +
				    "		>" +
				    "		<L [2]" +
				    "			<A TRANSFERINFO>" +
				    "			<L [3]" +
				    "				<L [2]" +
				    "					<A CARRIERID>" +
				    "					<A \"" + carrierId + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A SOURCEPORT>" +
				    "					<A \"" + source + "\">" +
				    "				>" +
				    "				<L [2]" +
				    "					<A DESTPORT>" +
				    "					<A \"" + dest + "\">" +
				    "				>" +
				    "			>" +
				    "		>" +
				    "	>" +
				    ">"
				    );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

		    int messageCount=100;
		    Secs2Message[] s2ma = new Secs2Message[messageCount];
		    s2ma[0]=s2m;
            for( int i=1; i<100; i++ )
		    {
			    s2ma[i] = new Secs2Message( s2m.GetStream(), s2m.GetFunction(),
                        s2m.GetMessageBody() );
		    }
		    return s2ma;
        }

        public Secs2Message[] BuildInitialize()
        {

            List<Secs2Message> v = new List<Secs2Message>();
            Secs2Message s2m;

		    try
		    {
               s2m = new Secs2Message(1,17,null);
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 6>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 79>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(2,15,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <L[2]" +
				       "    <U4 62>" +
				       "    <A 'CDC02'>" +
				       "  >" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(2,37,null);
               s2m.MessageBodyFromSMLString(
				       "<L[2]" +
				       "  <BOOLEAN F>" +
				       "  <L[0]" +
				       "  >" +
				       ">" );
               v.Add( s2m );

               s2m = new Secs2Message(2,37,null);
               s2m.MessageBodyFromSMLString(
				       "<L[2]" +
				       "  <BOOLEAN T>" +
				       "  <L[34]" +
				       "    <U4 1>" +
				       "    <U4 2>" +
				       "    <U4 3>" +
				       "    <U4 51>" +
				       "    <U4 52>" +
				       "    <U4 53>" +
				       "    <U4 54>" +
				       "    <U4 55>" +
				       "    <U4 56>" +
				       "    <U4 57>" +
				       "    <U4 101>" +
				       "    <U4 102>" +
				       "    <U4 103>" +
				       "    <U4 104>" +
				       "    <U4 105>" +
				       "    <U4 106>" +
				       "    <U4 107>" +
				       "    <U4 108>" +
				       "    <U4 109>" +
				       "    <U4 110>" +
				       "    <U4 151>" +
				       "    <U4 152>" +
				       "    <U4 153>" +
				       "    <U4 154>" +
				       "    <U4 155>" +
				       "    <U4 156>" +
				       "    <U4 157>" +
				       "    <U4 158>" +
				       "    <U4 159>" +
				       "    <U4 160>" +
				       "    <U4 201>" +
				       "    <U4 202>" +
				       "    <U4 251>" +
				       "    <U4 253>" +
				       "  >" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(5,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[2]" +
				       "  <B 0x00>" +
				       "  <U4 >" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(5,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[2]" +
				       "  <B 0x80>" +
				       "  <U4 >" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 51>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 330>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 52>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 53>" +
				       ">"
				       );
               v.Add( s2m );

               s2m = new Secs2Message(1,3,null);
               s2m.MessageBodyFromSMLString(
				       "<L[1]" +
				       "  <U4 4>" +
				       ">"
				       );
               v.Add( s2m );
		    }
		    catch( Exception e )
		    {
			    Console.WriteLine(e.StackTrace.ToString());
		    }

            Secs2Message[] s2ms = new Secs2Message[v.Count];


            v.CopyTo(s2ms);

		    return s2ms;
        }

        public S1F1 BuildS1F1()
        {
            S1F1 s2m = new S1F1();
		    s1f17Allowed = true;
            return s2m;
        }

        public S1F3 BuildS1F3()
        {
            S1F3 s2m = new S1F3();
            s2m.AddSVID( "U4", 53 );
            return s2m;
        }

        public S1F13 BuildS1F13()
        {
            S1F13 s2m = new S1F13();
		    s1f13Allowed = true;
            return s2m;
        }
        public S1F17 BuildS1F17()
        {
            S1F17 s2m = new S1F17();
            return s2m;
        }
        //public S2F17 BuildS2F17()
        //{
        //    S2F17 s2m = new S2F17();
        //    return s2m;
        //}
        public S2F31 BuildS2F31()
        {
            S2F31 s2m = new S2F31();
            return s2m;
        }
        public S2F33 BuildS2F33DeleteReports()
        {
            S2F33 s2m = new S2F33();
            s2m.SetDATAID( "U4", 1 );
            return s2m;
        }
        public S2F33 BuildS2F33CreateReports()
        {
            S2F33 s2m = new S2F33();
            s2m.SetDATAID( "U4", 1 );
		    s2m.AddRPTID( "U4", 356 );
		    s2m.AddVID( "U4", 207 );
		    s2m.AddRPTID( "U4", 371 );
		    s2m.AddVID( "U4", 216 );
		    s2m.AddRPTID( "U4", 358 );
		    s2m.AddVID( "U4", 205 );
            return s2m;
        }
        public S2F33 BuildS2F33CreateReports2()
        {
            S2F33 s2m = new S2F33();
            s2m.SetDATAID( "U4", 1 );
		    s2m.AddRPTID( "U4", 356 );
		    s2m.AddVID( "U4", 207 );
		    s2m.AddVID( "U4", 212 );
		    s2m.AddRPTID( "U4", 371 );
		    s2m.AddVID( "U4", 216 );
		    s2m.AddRPTID( "U4", 358 );
		    s2m.AddVID( "U4", 205 );
            return s2m;
        }
        public S2F35 BuildS2F35LinkReports()
        {
            S2F35 s2m = new S2F35();
            s2m.SetDATAID( "U4", 1 );
            s2m.AddCEID( "U4", 356 );
            s2m.AddRPTID( "U4", 356 );
            s2m.AddCEID( "U4", 371 );
            s2m.AddRPTID( "U4", 371 );
            s2m.AddCEID( "U4", 358 );
            s2m.AddRPTID( "U4", 358 );
            return s2m;
        }
        public S2F37 BuildS2F37DisableEvents()
        {
            S2F37 s2m = new S2F37();
            s2m.SetCEED( false );
            return s2m;
        }
        public S2F37 BuildS2F37EnableAllEvents()
        {
            S2F37 s2m = new S2F37();
            s2m.SetCEED( true );
            return s2m;
        }
        public S5F1 BuildS5F1()
        {
            S5F1 s2m = new S5F1();
            s2m.SetALCD( (byte)1 );
            s2m.SetALCDAlarmOnStatus( true );
            s2m.SetALID( "U4", 7 );
            s2m.SetALTX( "Alarm" );
            return s2m;
        }
        public S5F1 BuildS5F1_Again()
        {
            S5F1 s2m = new S5F1( (byte)(122), "U4", 1, "Alarm on text" );
            return s2m;
        }

        //public S6F5 BuildS6F5()
        //{
        //    S6F5 s2m = new S6F5();
        //    return s2m;
        //}
        public S5F3 BuildS5F3()
        {
            S5F3 s2m = new S5F3();
            s2m.SetALED( true );
            s2m.SetALID( "U4", 1 );
            return s2m;
        }

        //public S10F1 BuildS10F1()
        //{
        //    S10F1 s2m = new S10F1((byte)1,"Hello World");
        //    return s2m;
        //}

        //public S10F5 BuildS10F5()
        //{
        //    S10F5 s2m = new S10F5();
        //    s2m.AddText( "This is some long text that will need to be broken up into some chunks that can be displayed", 20 );
        //    return s2m;
        //}

        public S6F11 BuildS6F11()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID("U2", 0);
            s2m.SetCEID( "U2", 1 );

            s2m.AddRPTID( "U2", 1 );
            s2m.AddV( new byte[100] );
            s2m.AddV("Hello World");
            return s2m;
        }


        public S6F11 BuildS6F11_CraneDurationReport()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID("U2", 0);
            s2m.SetCEID("U2", 2);


            s2m.AddRPTID("U2", 62);
            s2m.AddV("A", "398");
            s2m.AddV("A", "P8TSTK04016");
            s2m.AddV("A", "P8TCST100048");


            Item itemL = new ItemL();
            itemL.Add(new ItemA("19.6"));
            itemL.Add(new ItemA("20.1"));
            itemL.Add(new ItemU2(33));

            s2m.AddV(itemL);

            s2m.AddRPTID("U2", 77);

            s2m.AddV(new ItemA("GGG"));
            s2m.AddV(new ItemA("244"));
            s2m.AddV(new ItemU2(33));

            return s2m;
        }

        public S6F11 BuildOfflineEvent()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 352 );
            return s2m;
        }

        public S6F11 BuildLocalEvent()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 353 );
            return s2m;
        }

        public S6F11 BuildRemoteEvent()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 354 );
            return s2m;
        }

        public S6F11 BuildSCStateAutoEvent()
        {
            S6F11 s2m = new S6F11();
            s2m.SetDATAID( "" );
            s2m.SetCEID( "U2", 53 );
            return s2m;
        }


	    private static bool s1f13Allowed = true;
	    private static bool s1f17Allowed = true;
	    public SecsMessage[] BuildAllowNothing()
        {
		    s1f13Allowed = false;
		    s1f17Allowed = false;
		    return new SecsMessage[0];
        }

        public Secs2Message BuildBadStreamWithReply()
        {
            Secs2Message s2m = new Secs2Message( 123, 123, null );
            s2m.SetWbit(true);
            return s2m;
        }

        public Secs2Message BuildBadStream()
        {
            Secs2Message s2m = new Secs2Message( 123, 123, null );
            s2m.SetWbit(false);
            return s2m;
        }

	    /*492
        public void S1F1( SecsMessage primary, SecsMessage secondary )
        {
            S1F1 p = (S1F1)primary;
            S1F2 s = (S1F2)secondary;

            //s.SetMDLN_SOFTREV( "hi", "there" );
        }
	    */

       public Secs2Message BuildUnknownHex()
       {
           

           return null;
       }

        public void S1F3( SecsMessage primary, SecsMessage secondary )
        {
            S1F3 p = (S1F3)primary;
            S1F4 s = (S1F4)secondary;

		    int vid = p.GetSVIDInteger(0);

		    switch( vid )
		    {
                case 4: // Alarms
                    s.AddSV( new ItemL() );  // No Alarms On
                    break;
                case 6: // Equipment State
                    s.AddSV( new ItemU2(5) );  // Online - Remote
                    break;
                case 51: // Active Carriers in the system
                    s.AddSV( new ItemL() );    // No Carriers
                    break;
                case 52: // Active Transfers
                    s.AddSV( new ItemL() );   // No Transfers
                    break;
                case 53: // Active Zones
                    s.AddSV( new ItemL(new ItemL( new ItemA("zone1"), new ItemU2(40) )) );   // One Zone
                    break;
			    case 73: // Device Activity IBSEM
				    s.AddSV( new ItemU2( 2 ) ); // Paused
				    break;
			    case 79: // Device Activity Stocker
				    s.AddSV( new ItemU2( 2 ) ); // Paused
				    break;
		    }

        }

        public void S1F13( SecsMessage primary, SecsMessage secondary )
        {
            S1F14 s = (S1F14)secondary;

		    if( ! s1f13Allowed )
		    {
			    s.SetCOMMACK( (byte)1 );
		    }

            s.SetMDLN( "hi" );
            s.SetSOFTREV( "there" );
        }

        public void S1F17( SecsMessage primary, SecsMessage secondary )
        {
            S1F18 s = (S1F18)secondary;

		    if( ! s1f17Allowed )
		    {
			    s.SetONLACK( (byte)1 );
		    }
        }

	    public void S2F31( SecsMessage primary, SecsMessage secondary )
	    {
		    S2F31 p = (S2F31)primary;
            frame.Log( GemDate.ParseDate(p.GetTIME()).ToString() );
	    }

        public void S2F41( SecsMessage primary, SecsMessage secondary )
        {
        }

        public void S5F1( SecsMessage primary, SecsMessage secondary )
        {
            S5F1 p = (S5F1)primary;
            frame.Log( "S5F1 ALCD " + p.GetALCD() );
		    frame.Log( "S5F1 ALCD ON/OFF " + p.GetALCDAlarmOnStatus() );
		    frame.Log( "S5F1 ALID " + p.GetALID() );
		    frame.Log( "S5F1 ALTX " + p.GetALTX() );
        }

        public void S5F3( SecsMessage primary, SecsMessage secondary )
        {
            S5F3 p = (S5F3)primary;
            if( 0!=p.GetALIDCount() )
            {
                frame.Log( "* S5F3 * " + p.GetALID( 0 ) );
            }
            else
            {
                frame.Log( "* S5F3 * NO ALID Data!" );
            }

            //frame.Log( "S6F11 " + p.GetDATAID() + " " + p.GetCEID() + " " + p.GetRPTID(0) );
        }

        public void S6F11( SecsMessage primary, SecsMessage secondary )
        {
            S6F11 p = (S6F11)primary;

            switch (p.GetCEIDInteger())
            {
                case 1:
                   
                    break;
                case 2:
                    frame.logger.Log("Report ID : "+ p.GetRPTID(0));
                    frame.logger.Log("Location id : "+((ItemL)((p.GetVItem(0, 3)))).GetValue(0).ToString());
                    
                    break;
            }


            //frame.Log( "S6F11 " + p.GetDATAID() + " " + p.GetCEID() + " " + p.GetRPTID(0) );

            
        }

        public void S2F49(SecsMessage primary, SecsMessage secondary)
        {
            S2F49 p = (S2F49)primary;

           
            
        }
    }
}
