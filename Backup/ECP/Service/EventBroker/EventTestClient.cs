using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels.Http;

using ECP.Event;



namespace ECP.Service.EventBroker
{
    public class EventTestClient : MarshalByRefObject
    {
        public  RemoteEvent remoteEvent = null;
        public  WrapperRemoteEvent wrapperRemoteEvent = null; 

        public  void init()
        {
            TcpChannel tcpChannel = new TcpChannel();
            ChannelServices.RegisterChannel(tcpChannel, false);
            remoteEvent = (RemoteEvent)Activator.GetObject(typeof(RemoteEvent), "tcp://localhost:9000/RemoteEvent");            
        }

        public void initByConfigfile()
        {
            RemotingConfiguration.Configure(Environment.CurrentDirectory+@"\TestClient.exe.config", false);
        }

        public void subscribe()
        {
            //Create an instance of wrapper class.
            remoteEvent = (RemoteEvent)Activator.GetObject(typeof(RemoteEvent), "tcp://localhost:9000/RemoteEvent");
            wrapperRemoteEvent = new WrapperRemoteEvent();
     
            //Associate remote object event with wrapper method.
            try
            {
               // remoteEvent.RemoteEventReceived += new RemoteEventHandler(wrapperRemoteEvent.WrapperRemoteEventReceivedHandler);
                remoteEvent.RemoteEventReceived += new RemoteEventHandler(wapperRemoteEvent_wrapperRemoteEventReceived);
              
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
                Console.WriteLine(ex.StackTrace);
            }

            //Associate wrapper event with current form event handler.
            //wrapperRemoteEvent.WrapperRemoteEventReceived += new RemoteEventHandler(wapperRemoteEvent_wrapperRemoteEventReceived);
            
        }

        public void unsubscribe()
        {
            try
            {
                remoteEvent.RemoteEventReceived -= new RemoteEventHandler(wrapperRemoteEvent.WrapperRemoteEventReceivedHandler);
                wrapperRemoteEvent.WrapperRemoteEventReceived -= new RemoteEventHandler(wapperRemoteEvent_wrapperRemoteEventReceived);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
            }
        }



        public void wapperRemoteEvent_wrapperRemoteEventReceived(object eventData)
        {

            Console.WriteLine("Published Message : " + eventData.ToString());
                           
        }       

        public void test()
        {
            //this.init();
             this.initByConfigfile();

            this.subscribe();
            int i =0;
            while(true)
            {
                i++;
                remoteEvent.Publish(new ECPEvent(Convert.ToString(i)));
                System.Threading.Thread.Sleep(100);
                Console.ReadLine();
            }

            //Console.ReadLine();
            //this.unsubscribe();
            //Console.ReadLine();
        }

        


    }
}
