using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global.Com;
using ECP.Com.FieldBus;
using ECP.Interfaces.Unit;

namespace HECS.Global.Interface
{
    /// <summary>
    /// Equipments를 대표하는 Equipemnt
    /// </summary>
    public interface IECS : IRepresentativeEquipment
    {
        /// <summary>
        /// FB Driver 상태 
        /// </summary>
        FBConnectionState DriverConnectionStatus
        {
            get;
            set;
        }

        /// <summary>
        /// 버젼 정보
        /// </summary>
        string SoftVersion
        {
            get;
            set;
        }

        #region Public Methods

        /// <summary>
        /// 버젼
        /// </summary>
        /// <param name="strVersion"></param>
        void SetVersion(string strVersion);

        /// <summary>
        /// Controller Start up
        /// </summary>
        void ControllerStartUp();

        #endregion Public Methods
    }
}
