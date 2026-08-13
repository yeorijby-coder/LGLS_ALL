using System;
using System.Collections.Generic;
using System.Text;
using ECP.Device.Observe;

namespace HECS.Device.Unit
{
   
    public partial class Conveyor
    {
        #region Properties for Observable Access
        public Observable @__IO_IN_READY_02
        {
            get
            {
                return this.Observables["IN_READY_02"];
            }
        }
        public Observable @__IO_LOAD_COMPLETE_01
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_01"];
            }
        }
        public Observable @__IO_LOAD_COMPLETE_02
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_02"];
            }
        }
        public Observable @__IO_OPERATION_MODE
        {
            get
            {
                return this.Observables["OPERATION_MODE"];
            }
        }
        public Observable @__IO_PALLET_EXIST01
        {
            get
            {
                return this.Observables["PALLET_EXIST01"];
            }
        }
        public Observable @__IO_PALLET_EXIST02
        {
            get
            {
                return this.Observables["PALLET_EXIST02"];
            }
        }
        public Observable @__IO_PALLET_EXIST03
        {
            get
            {
                return this.Observables["PALLET_EXIST03"];
            }
        }
        public Observable @__IO_PALLET_EXIST04
        {
            get
            {
                return this.Observables["PALLET_EXIST04"];
            }
        }
        public Observable @__IO_PALLET_EXIST05
        {
            get
            {
                return this.Observables["PALLET_EXIST05"];
            }
        }

//LGLS 林籍 贸府
        //public Observable @__IO_PALLET_EXIST06
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST06"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST07
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST07"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST08
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST08"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST09
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST09"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST10
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST10"];
        //    }
        //}

        public Observable @__IO_PALLET_EXIST_FLAG_01
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_01"];
            }
        }
        public Observable @__IO_PALLET_EXIST_FLAG_02
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_02"];
            }
        }
        public Observable @__IO_PALLET_EXIST_FLAG_03
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_03"];
            }
        }
        public Observable @__IO_PALLET_EXIST_FLAG_04
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_04"];
            }
        }
        public Observable @__IO_PALLET_EXIST_FLAG_05
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_05"];
            }
        }

        //LGLS 林籍 贸府
        //public Observable @__IO_PALLET_EXIST_FLAG_06
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_06"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST_FLAG_07
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_07"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST_FLAG_08
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_08"];
        //    }
        //}
        //public Observable @__IO_PALLET_EXIST_FLAG_09
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_09"];
        //    }
        //}

        public Observable @__IO_UNLOAD_COMPLETE_01
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_01"];
            }
        }
        public Observable @__IO_UNLOAD_COMPLETE_02
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_02"];
            }
        }
        public Observable @__IO_WAIT_IN
        {
            get
            {
                return this.Observables["WAIT_IN"];
            }
        }
        public Observable @__IO_WAIT_OUT
        {
            get
            {
                return this.Observables["WAIT_OUT"];
            }
        }

        public Observable @__IO_UNLOAD_REQUEST_02
        {
            get
            {
                return this.Observables["UNLOAD_REQUEST_02"];
            }
        }
        public Observable @__IO_UNLOAD_REQUEST_ACK_02
        {
            get
            {
                return this.Observables["UNLOAD_REQUEST_ACK_02"];
            }
        }

        public Observable @__IO_DIRECTION_MODE
        {
            get
            {
                return this.Observables["DIRECTION_MODE"];
            }
        }

        //LGLS 林籍 贸府
        //public Observable @__IO_OVERLOAD_ALARM
        //{
        //    get
        //    {
        //        return this.Observables["OVERLOAD_ALARM"];
        //    }
        //}
        //public Observable @__IO_OVERTIME_ALARM
        //{
        //    get
        //    {
        //        return this.Observables["OVERTIME_ALARM"];
        //    }
        //}
        #endregion

        #region Properties for Observable Access By Value
        public string IO_IN_READY_02
        {
            get
            {
                return this.Observables["IN_READY_02"].AsString;
            }
            set
            {
                this.Observables["IN_READY_02"].AsString = value;
            }
        }
        public string IO_LOAD_COMPLETE_01
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_01"].AsString;
            }
            set
            {
                this.Observables["LOAD_COMPLETE_01"].AsString = value;
            }
        }
        public string IO_LOAD_COMPLETE_02
        {
            get
            {
                return this.Observables["LOAD_COMPLETE_02"].AsString;
            }
            set
            {
                this.Observables["LOAD_COMPLETE_02"].AsString = value;
            }
        }
        public string IO_OPERATION_MODE
        {
            get
            {
                return this.Observables["OPERATION_MODE"].AsString;
            }
            set
            {
                this.Observables["OPERATION_MODE"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST_FLAG_01
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_01"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG_01"].AsString = value;
            }
        }

        public string IO_PALLET_EXIST_FLAG_02
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_02"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG_02"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST_FLAG_03
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_03"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG_03"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST_FLAG_04
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_04"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG_04"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST_FLAG_05
        {
            get
            {
                return this.Observables["PALLET_EXIST_FLAG_05"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST_FLAG_05"].AsString = value;
            }
        }

        //LGLS 林籍 贸府
        //public string IO_PALLET_EXIST_FLAG_06
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_06"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST_FLAG_06"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST_FLAG_07
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_07"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST_FLAG_07"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST_FLAG_08
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_08"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST_FLAG_08"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST_FLAG_09
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST_FLAG_09"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST_FLAG_09"].AsString = value;
        //    }
        //}

        public string IO_UNLOAD_COMPLETE_01
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_01"].AsString;
            }
            set
            {
                this.Observables["UNLOAD_COMPLETE_01"].AsString = value;
            }
        }
        public string IO_UNLOAD_COMPLETE_02
        {
            get
            {
                return this.Observables["UNLOAD_COMPLETE_02"].AsString;
            }
            set
            {
                this.Observables["UNLOAD_COMPLETE_02"].AsString = value;
            }
        }
        public string IO_WAIT_IN
        {
            get
            {
                return this.Observables["WAIT_IN"].AsString;
            }
            set
            {
                this.Observables["WAIT_IN"].AsString = value;
            }
        }
        public string IO_WAIT_OUT
        {
            get
            {
                return this.Observables["WAIT_OUT"].AsString;
            }
            set
            {
                this.Observables["WAIT_OUT"].AsString = value;
            }
        }

        public string IO_PALLET_EXIST01
        {
            get
            {
                return this.Observables["PALLET_EXIST01"].AsString;
                                         
            }
            set
            {
                this.Observables["PALLET_EXIST01"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST02
        {
            get
            {
                return this.Observables["PALLET_EXIST02"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST02"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST03
        {
            get
            {
                return this.Observables["PALLET_EXIST03"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST03"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST04
        {
            get
            {
                return this.Observables["PALLET_EXIST04"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST04"].AsString = value;
            }
        }
        public string IO_PALLET_EXIST05
        {
            get
            {
                return this.Observables["PALLET_EXIST05"].AsString;
            }
            set
            {
                this.Observables["PALLET_EXIST05"].AsString = value;
            }
        }

        public string IO_UNLOAD_REQUEST_02
        {
            get
            {
                return this.Observables["UNLOAD_REQUEST_02"].AsString;
            }
            set
            {
                this.Observables["UNLOAD_REQUEST_02"].AsString = value;
            }
        }
        public string IO_UNLOAD_REQUEST_ACK_02
        {
            get
            {
                return this.Observables["UNLOAD_REQUEST_ACK_02"].AsString;
            }
            set
            {
                this.Observables["UNLOAD_REQUEST_ACK_02"].AsString = value;
            }
        }

        public string IO_DIRECTION_MODE
        {
            get
            {
                return this.Observables["DIRECTION_MODE"].AsString;
            }
            set
            {
                this.Observables["DIRECTION_MODE"].AsString = value;
            }
        }

        //LGLS 林籍 贸府
        //public string IO_PALLET_EXIST06
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST06"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST06"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST07
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST07"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST07"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST08
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST08"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST08"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST09
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST09"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST09"].AsString = value;
        //    }
        //}
        //public string IO_PALLET_EXIST10
        //{
        //    get
        //    {
        //        return this.Observables["PALLET_EXIST10"].AsString;
        //    }
        //    set
        //    {
        //        this.Observables["PALLET_EXIST10"].AsString = value;
        //    }
        //}
        //public List<short> IO_OVERLOAD_ALARM
        //{
        //    get
        //    {
        //        return this.Observables["OVERLOAD_ALARM"].AsShortArray ;
        //    }
        //    set
        //    {
        //        this.Observables["OVERLOAD_ALARM"].AsShortArray = value;
        //    }
        //}
        //public List<short> IO_OVERTIME_ALARM
        //{
        //    get
        //    {
        //        return this.Observables["OVERTIME_ALARM"].AsShortArray;
        //    }
        //    set
        //    {
        //        this.Observables["OVERTIME_ALARM"].AsShortArray = value;
        //    }
        //}

        #endregion
    }
}
