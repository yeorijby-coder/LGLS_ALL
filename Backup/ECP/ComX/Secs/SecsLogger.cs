using System;
using System.Collections.Generic;
using System.Text;
using ECP.ComX.Secs;
using ECP.ComX.Secs.Secs2;

namespace ECP.ComX.Secs
{
    public class SecsLogger
    {

        private SecsLoggingLevel loggingLevel = SecsLoggingLevel.NO_LOGGING;

        protected const String _in =  "-->I ";
        protected const String _out = "<--O ";

       
        public  void SetLoggingLevel( SecsLoggingLevel loggingLevel )
        {
            this.loggingLevel =  loggingLevel;
        }

        
        public  SecsLoggingLevel GetLoggingLevel()
        {
            return loggingLevel;
        }

        
        public  bool IsLogging( SecsLoggingLevel loggingLevel )
        {
            return this.loggingLevel.IsLogging( loggingLevel );
        }

        
        public virtual void Log( int machineId, String id, String message )
        {
            if( IsLogging( SecsLoggingLevel.ERROR_LOGGING ) )
            {
                Console.WriteLine( "Err: " + id + ": " + message );
            }
        }

        
        public virtual void Log( int machineId, String id, String message, 
            bool sent, byte[] comMessage )
        {
            if( IsLogging( SecsLoggingLevel.ERROR_LOGGING ) )
            {
                Console.WriteLine( "Err: " + id + ": " + (sent?_out:_in) + message );
                PrintByteArray( comMessage );
            }
        }

        
        public virtual void Debug( int machineId, String id, String message )
        {
            if( IsLogging( SecsLoggingLevel.DEBUG_LOGGING ) )
            {
                Console.WriteLine( "Dbg: " + id + ": " + message );
            }
        }

        
        public virtual void LogProtocol( int machineId, String id, bool sent, SecsMessage secsMessage )
        {
            if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
            {
                
                    Console.WriteLine( "Com: " + id + ":" +
                                (sent?_out:_in) +
                                secsMessage.GetSecsHeader().ToString() );
               
            }
        }

        
        public virtual void LogProtocol( int machineId, String id, bool sent, byte[] secsHeader )
        {
            if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
            {
                
                    SecsHeader sh = new SecsHeader( secsHeader );
                    Console.WriteLine( "Com: " + id + ":" +
                                (sent?_out:_in) +
							    sh.ToString() );
                    PrintByteArray( sh.ToByteArray() );
                
            }
        }

        
        public virtual void LogProtocol( int machineId, String id, bool sent, byte protocolByte )
        {
            if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
            {
                Console.WriteLine( "Com: " + id + ":" +
                                (sent?_out:_in) +
							    protocolByte );
            }
        }

        
        public virtual void LogMessage( String id, bool sent, SecsMessage secsMessage )
        {
            if( IsLogging( SecsLoggingLevel.MESSAGE_LOGGING ) )
            {                
    
                Secs2Message s2m;
                if( secsMessage is Secs2Message )
                {
                    s2m = (Secs2Message)secsMessage;
                }
                else
                {
                    s2m = new Secs2Message( secsMessage );
                }

                Console.WriteLine( "Com: " + id + ":" +
                                (sent?_out:_in) +
							    s2m.ToIndentedSMLString() );
            }
        }

        static char[] hexDigits = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        public static string ToHexString(byte[] bytes)
        {
            char[] chars = new char[bytes.Length * 2];
            for (int i = 0; i < bytes.Length; i++)
            {
                int b = bytes[i];
                chars[i * 2] = hexDigits[b >> 4];
                chars[i * 2 + 1] = hexDigits[b & 0xF];
            }
            return new string(chars);
        }        


        private void PrintByteArray( byte[] byteArray )
        {               
            string hexString = ToHexString(byteArray);

            for (int i = 0; i < hexString.Length; i++)
            {
                char c = hexString[i];
                if (i != 0 && i % 20 == 0)
                {
                    Console.WriteLine("");
                }

                Console.Write(c);
                if (i != 0 && i % 2 == 0)
                {
                    Console.Write(" ");
                }
            }
            Console.WriteLine("");
        }
    }
}
