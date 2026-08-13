using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Global.Service
{

    /// <summary>
    /// This enum defines the possible values for the direction for COM messages. 
    /// </summary>
    enum Direction
    {
        /**
         * The message is coming into the system (from the physical
         * device or MES).
        **/
        IN_MESSAGE,

        /**
         * The message is going out of the system (to the physical
         * device or MES).
        **/
        OUT_MESSAGE
    };


    /// <summary>
    /// This enum defines the possible values for the message Category
    /// </summary>

    enum Category
    {
        /**
         * A communication message. Generally a message to/from
         * a physical device or the MES.
        **/
        COM,

        /**
         * A debug message used by system developers or other 
         * knowledgeable users to solve system issues and/or
         * determine why something occured in the system.
        **/
        DEBUG,

        /**
         * An abnormal situation occured which may require
         * manual intervention.
        **/
        ERROR,

        /**
         * A message which is of general interest to the user.
        **/
        INFO
    };
}
