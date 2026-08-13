using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Service.User
{
    /// <summary>
    /// Permission의 설정에 따라 동작할 대상이 되는 Target개체의 행동에 대한 인터페이스를 제공합니다.
    /// 예를 들어 Target개체에 특정 기능에 대한 Permission을 가지고 있을 경우 Apply() 메소드에 기능에 대한
    /// 권한 설정을 Enable 하여 사용가능 하도록 할 수 있습니다.
    /// </summary>
    public interface IPermission
    {
        /// <summary>
        /// 개체에 등록된 Permission Id를 가져옵니다.
        /// </summary>
        /// <returns></returns>
        string GetId();
        /// <summary>
        /// 개체에 구현된 권한을 설정합니다.
        /// </summary>
        void Apply();
        /// <summary>
        /// 개체에 구현된 권한을 해제합니다.
        /// </summary>
        void Cancel();
    }
}
