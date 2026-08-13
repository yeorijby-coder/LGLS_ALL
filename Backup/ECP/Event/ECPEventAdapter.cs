using System;
using System.Collections.Generic;
using System.Text;

using ECP.Interfaces.Service.EventBroker;
using ECP.Service.EventBroker;
using ECP.Event;
using ECP.Util.Client;
using ECP.Service.Logger;

namespace ECP.Event
{
    public abstract class ECPEventAdapter : IECPListener
    {
        protected bool theSubscriptionStatus = false;

        public bool TheSubscriptionStatus
        {
            get { return this.theSubscriptionStatus; }
            set { this.theSubscriptionStatus = value; }
        }

        WrapperRemoteEvent wre = new WrapperRemoteEvent();


        public ECPEventAdapter()
        {
        }   

        public abstract string GetSubscriptionSubject();


        public void Subscribe()
        {
            Unsubscribe();

            EventBrokerManager ebm = Naming.GetEventBrokerManager();

            RemoteEvent re = ebm.GetRemoteEventForSubscribe(this.GetSubscriptionSubject());

       
            if (re != null)

                try
                {
                    re.RemoteEventReceived += new RemoteEventHandler(wre.WrapperRemoteEventReceivedHandler);

                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message.ToString());
                    Console.WriteLine(ex.StackTrace);
                    Log.log(5, "Subscribe", "debug", ECP.Service.Logger.Category.DEBUG, "", "Subscribe", ex.Message.ToString());
                }

            //Associate wrapper event with current form event handler.
            wre.WrapperRemoteEventReceived += new RemoteEventHandler(ReceiveECPEvent);

            this.TheSubscriptionStatus = true;

        }

        abstract public void ReceiveECPEvent(ECPEvent ecpEvent);


        public void Unsubscribe()
        {
            if (!this.TheSubscriptionStatus) return;


            EventBrokerManager ebm = Naming.GetEventBrokerManager();

            RemoteEvent re = ebm.GetRemoteEventForUnsubscribe(this.GetSubscriptionSubject());

            if (re == null) return;

            try
            {
                re.RemoteEventReceived -= new RemoteEventHandler(wre.WrapperRemoteEventReceivedHandler);
                wre.WrapperRemoteEventReceived -= new RemoteEventHandler(ReceiveECPEvent);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
                Log.log(5, "Unsubscribe", "debug", ECP.Service.Logger.Category.DEBUG, "", "Unsubscribe", ex.Message.ToString());
            }

            TheSubscriptionStatus = false;

        }


    }
}
