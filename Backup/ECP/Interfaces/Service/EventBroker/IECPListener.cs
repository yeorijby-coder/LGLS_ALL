using System;
using System.Collections.Generic;
using System.Text;

using ECP.Event;

namespace ECP.Interfaces.Service.EventBroker
{
    public interface IECPListener
    {
        void ReceiveECPEvent(ECPEvent ecpEvent); 
    }
}
