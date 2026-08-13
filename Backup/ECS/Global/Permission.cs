using System;
using System.Collections.Generic;
using System.Text;

using ECP.Service.User;
using ECP.Util.Client;
using HECS.Device;

namespace HECS.Global
{
    class StockerPermission
    {
        //사용자 권한 문자열 상수

        public const string STOCKER_CST_MOVE = "STOCKER.CST.MOVE";
        public const string STOCKER_CST_INSTALL_REMOVE = "STOCKER.CST.INSTALL_REMOVE";
        public const string STOCKER_SHELF_PROHIBIT = "STOCKER.SHELF.PROHIBIT";
        public const string STOCKER_EQP_AUTORUN_STOP = "STOCKER.EQP.AUTORUN_STOP";
        public const string STOCKER_ERROR_RESET = "STOCKER.ERROR.RESET";
        public const string STOCKER_ERROR_MOVE = "STOCKER.ERROR.MOVE";
        public const string STOCKER_CYCLE_TEST = "STOCKER.CYCLE_TEST";
        public const string STOCKER_ON_OFFLINE = "STOCKER.ON_OFFLINE";
        public const string STOCKER_RECONCILE = "STOCKER.RECONCILE";
        public const string STOCKER_SHUTDOWN = "STOCKER.SHUTDOWN";
        public const string STOCKER_EQP_PORTDIRECTION = "STOCKER.EQP.PORTDIRECTION";
        public const string STOCKER_OPTION_CHANGE = "STOCKER.OPTION_CHANGE";
        public const string STOCKER_TRANSFER_CANCEL_ABORT = "STOCKER.TRANSFER.CANCEL_ABORT";
        public const string STOCKER_BCR_MODIFY = "STOCKER.BCR.MODIFY";
        public const string STOCKER_EQP_POWERON_OFF = "STOCKER.EQP.POWERON_OFF";
        public const string STOCKER_RESUME_PAUSE = "STOCKER.RESUME_PAUSE";
        public const string STOCKER_ODDMETER_CLEAR = "STOCKER.ODOMETER_CLEAR";
        public const string STOCKER_DISPATCHER_VIEW = "STOCKER.DISPATCHER_VIEW";
        public const string STOCKER_DISPATCHER_CHANGE = "STOCKER.DISPATCHER_CHANGE";
        public const string STOCKER_DISPATCHER_TRANSFER_SORT = "STOCKER.DISPATCHER.TRANSFER_SORT";
        public const string STOCKER_DISPATCHER_TRANSFER_FILTER = "STOCKER.DISPATCHER.TRANSFER_FILTER";
        public const string STOCKER_DISPATCHER_GLOBAL_OPTION = "STOCKER.DISPATCHER.GLOBAL_OPTION";
        public const string STOCKER_DISPATCHER_PORT_OPTION = "STOCKER.DISPATCHER.PORT_OPTION";


    }



}
