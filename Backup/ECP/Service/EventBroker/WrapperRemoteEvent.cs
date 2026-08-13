using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using ECP.Event;


namespace ECP.Service.EventBroker
{
 
    /// <summary>
    /// RemoteEvent 객체의 .Net Remoting 기능을 위한 
    /// Wrapper객체, 이 Class없이는 External(Inter Process간, Inter Node) 간의
    /// Subscription 및 Publish가 불가능함
    /// </summary>
    public class WrapperRemoteEvent : MarshalByRefObject
	{ 

        public event RemoteEventHandler WrapperRemoteEventReceived;

        public WrapperRemoteEvent()
        {
        }

        ~WrapperRemoteEvent()
        {
        }

        public void WrapperRemoteEventReceivedHandler(ECPEvent ecpEvent)
        {
            if (WrapperRemoteEventReceived != null)
                WrapperRemoteEventReceived(ecpEvent);
        }

        public override object InitializeLifetimeService()
        {
            return null;
        }
       
	}
}
