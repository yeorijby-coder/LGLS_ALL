using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

using ECP.Event;
using ECP.Service.Logger;


namespace ECP.Service.EventBroker
{

    [Serializable]
    public delegate void RemoteEventHandler(ECPEvent ecpEvent);

    /// <summary>
    /// ECPEvent를 Publish하는 Class
    /// .Net Remoting 및 대리자의 Event subscirbe기능을 포함하고 있는
    /// 대리자 역활의 Class이다.
    /// </summary>
    public class RemoteEvent : MarshalByRefObject
    {

        private const string CLASS = ".RemoteEvent";

        /// <summary>
        /// ECPEvent의 Subject명
        /// </summary>
        public string subject = "";

        /// <summary>
        /// Remote Event Handler
        /// </summary>
        public event RemoteEventHandler RemoteEventReceived;

        /// <summary>
        /// 기본 생성자
        /// </summary>
        public RemoteEvent()
        {
        }

        /// <summary>
        /// 주어진 subject id로 RemoteEvent생성
        /// </summary>
        /// <param name="subject">subject Id</param>
        public RemoteEvent(string subject)
        {
            this.subject = subject;
        }

        /// <summary>
        /// .NetRemoting Object인 RemoteEvent의 생명 주기를 무한대로 설정
        /// </summary>
        /// <returns></returns>
        public override object InitializeLifetimeService()
        {
            return null;
        }

        /// <summary>
        /// RemoteEvent객체의 RemoteEventHander에 Subscribe되어진 Subscriber의 갯수를 Return
        /// </summary>
        /// <returns></returns>
        public int GetInvocationListCount()
        {
            return RemoteEventReceived.GetInvocationList().Length;
        }

        /// <summary>
        /// 주어진 ECPEvent를 RemoteEventReceived Handler에 등록되어진
        /// 모든 Subscriber에게 Publish함
        /// 
        /// 동기 Method Call 메카니즘을 사용함
        /// 만약 Dest Subscribe가 통신상태 나 Hang상태가 감지 될 경우
        /// Subscription List에서 자동 삭제 하여 Error를 복구한다.
        /// 
        /// 만약 비동기 Method Call 메카니즘을 사용할 경우
        /// Dest Subscribe가 문제가 발생하여도, 비동기 임으로
        /// Subcription List에 삭제와 같은 행위를 할 수 없게 된다.
        /// 따라서, RemoteEvent객체를 가진 EventBrokerManager는 Hang 상태로
        /// 빠질 위험에 처할 수 있다.
        /// </summary>
        /// <param name="ecpEvent"></param>
        public void Publish(ECPEvent ecpEvent)
        {
            if (RemoteEventReceived != null)
            {
                RemoteEventHandler eventDelegate = null;
                Delegate[] invocationList_ = null;
                try
                {
                    invocationList_ = RemoteEventReceived.GetInvocationList();
                }
                catch (MemberAccessException ex)
                {
                    throw ex;
                }
                if (invocationList_ != null)
                {
                    lock (this)
                    {
                        foreach (Delegate del in invocationList_)
                        {
                            try
                            {
                                eventDelegate = (RemoteEventHandler)del;
                                eventDelegate(ecpEvent);
                                //eventDelegate.BeginInvoke(ecpEvent, null, null); //Asynchronous Invocation
                            }
                            catch (Exception ex)
                            {
                                // 만약 Dest Subscriber에 문제 발생시
                                // Subscription List에서 Subscriber를 자동 삭제함
                                RemoteEventReceived -= eventDelegate;
                                Console.WriteLine(ex.Message);
                                Console.WriteLine(ex.StackTrace);
                                Log.log(5, "AddUserDefinedProperty", "debug", ECP.Service.Logger.Category.DEBUG, "", "Publish", ex.Message.ToString());
                            }
                        }
                    }
                }

            }
        }
        
    }
}
