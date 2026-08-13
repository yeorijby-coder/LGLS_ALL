using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Interfaces.Unit
{
    /// <summary>
    /// Equipment들을 대표하는 Equipment Interface
    /// </summary>
    public interface IRepresentativeEquipment
    {
        /// <summary>
        /// 버젼
        /// </summary>
        /// <param name="strVersion"></param>
        void LoadAbstractFieldBusDrivers();

        /// <summary>
        /// FB Driver의 Event를 처리
        /// </summary>
        void AbstractFieldBusDriverEventSubscribe();

    }
}
