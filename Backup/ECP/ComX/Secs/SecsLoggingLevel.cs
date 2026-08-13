using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs
{
    public class SecsLoggingLevel
    {

        public const int _NO_LOGGING = 0;


        public const int _DEBUG_LOGGING = 1;


        public const int _ERROR_LOGGING = 2;


        public const int _PROTOCOL_LOGGING = 4;


        public const int _MESSAGE_LOGGING = 8;


        public const int _ALL_LOGGING = _DEBUG_LOGGING | _ERROR_LOGGING |
                                            _PROTOCOL_LOGGING | _MESSAGE_LOGGING;

	    public static SecsLoggingLevel NO_LOGGING =
									    new SecsLoggingLevel( _NO_LOGGING );
	    public static SecsLoggingLevel DEBUG_LOGGING =
									    new SecsLoggingLevel( _DEBUG_LOGGING );
	    public static SecsLoggingLevel ERROR_LOGGING =
									    new SecsLoggingLevel( _ERROR_LOGGING );
	    public static SecsLoggingLevel PROTOCOL_LOGGING =
									    new SecsLoggingLevel( _PROTOCOL_LOGGING );
	    public static SecsLoggingLevel MESSAGE_LOGGING =
									    new SecsLoggingLevel( _MESSAGE_LOGGING );
	    public static SecsLoggingLevel ALL_LOGGING =
									    new SecsLoggingLevel( _ALL_LOGGING );

        private int loggingLevel = _NO_LOGGING;


	    private SecsLoggingLevel( int loggingLevel )
	    {
		    this.loggingLevel = loggingLevel;
	    }


	    public SecsLoggingLevel Add( SecsLoggingLevel level )
	    {
		    return new SecsLoggingLevel( loggingLevel | level.loggingLevel );
	    }

	   
	    public SecsLoggingLevel Remove( SecsLoggingLevel level )
	    {
		    return new SecsLoggingLevel(
					    loggingLevel ^ (loggingLevel & level.loggingLevel) );
	    }

	    
	    public bool IsLogging( SecsLoggingLevel level )
	    {
		    return (level.loggingLevel & loggingLevel) == level.loggingLevel;
	    }
    }
}
