    using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;

namespace ECP.Db.Entity
{
    [Serializable]
    public class EntityEnumData : ECPObject
    {
        public enum STARTMODE
        {
            Client = 1,
            ECS = 2
        }

        public enum COMMANDTYPE
        {
            INGO = 1,
            OUTGO = 2,
            MOVE = 6
        }

        public enum MOVEMENTTYPE
        {
            SAP = 1,
            ECS = 2,
            MOVE = 3,
            WMS = 4,
        }

        public enum TRANSFERSTATUS
        {
            Wait = 1,
            Transferring = 2,
            Complete = 3
        }

        public enum TRANSFERDETAILSTATUS
        {
            Wait = 1,
            Transferring = 2,
            Complete = 3
        }

        public enum HIRACKSTRANSFERSTATUS
        {
            Wait = 1,
            Transferring = 2,
            Complete = 3
        }

        public enum PALLETTRANSFERSTATUS
        {
            Wait = 1,
            Transferring = 2,
            Complete = 3
        }

        public enum HIRACKSTATUS
        {
            Empty = 1,
            Reserved = 2,
            Full = 3,
            Hold = 4,
            Blank = 5
        }

        public enum PALLETSTATUS
        {
            Assigned = 1,
            Empty = 2,
            Reserved = 3,
            Transferring = 4,
            Hold = 5
        }

        public enum TRANSFERRESULTCODE
        {
            SUCCESS = 1,
            CANCEL = 2,
            RETRY = 3
        }

        public enum PALLETBLOCKINSTATUS
        {
            Hold = 1,
            Unhold = 2
        }

        public enum TRANSACTIONTYPE
        {
            Insert = 1,
            Update = 2,
            Delete = 3
        }

        public enum FIELDBUSDRIVERTYPE
        {
            SIMULATION = 1,
            MELSECNET = 3,
        }
    }
}
