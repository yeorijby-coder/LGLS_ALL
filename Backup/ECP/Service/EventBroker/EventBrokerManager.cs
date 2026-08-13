using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;
using ECP.Event;

using ECP.Db;
using ECP.Util;

namespace ECP.Service.EventBroker
{
    /// <summary>
    /// EventBrokerManager는 Publish로 부터 요청된 Subject를
    /// Subscribe하고 있는 모든 ECPFramework 내의 객체를 조사하여
    /// 해당 객체에게 ECPEvent를 Publish하는 Manage이다. 
    /// 
    /// EventBrokerManager는 RemoteEvent객체의 Container를 가지고
    /// 있으며, 이 RemoteEvent객체가 실제 Subscriber의 주소를 
    /// 가지고 있을 수 있으며, RemoteEvent객체는 하나의 Subject에 대하여
    /// 여러개의 Subscribe을 담을 수 있다.
    /// <seealso cref="RemoteEvent"/>
    /// </summary>
    public class EventBrokerManager : Manager
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private const string CLASS = "EventBrokerManager";

        /// <summary>
        /// Subject의 고정 길이형 Wild 카드 문자
        /// </summary> 
        public const int FIXED_LENGTH_WILDCARD = '*';

        /// <summary>
        /// Subject의 가변길이형 Wild 카드 문자
        /// </summary>
        public const int VARIABLE_LENGTH_WILDCARD = '>';

        /// <summary>
        /// Subject의 구분자
        /// </summary>
        public const int TOKEN_DELIMITER = '.';

        /// <summary>
        /// Pulish 되어질 모든 Subject에 대한 Remote Event 객체를 담고 있는 Container
        /// </summary>
        public static Dictionary<string, RemoteEvent> remoteEvents = new Dictionary<string, RemoteEvent>();

        public delegate void DelegateEventPublisher(string subject, ECPEvent ecpEvent);

        /// <summary>
        /// 주어진 Id로 Manager를 생성하는 생성자
        /// </summary>
        /// <param name="ownerId"></param>
        /// <param name="id"></param>
        public EventBrokerManager(string ownerId, string id)
            : base(ownerId, id)
        {
        }

        /// <summary>
        /// Data를 data base에 Add함
        /// 저장하고자 하는 Property에 대하여 저장될 수 있도록
        /// Sub Class에 의하여 Implement되어져야 하며, 항상 bass.Add()가 미리 Call되어져야 한다.
        /// </summary>
        /// <param name="theDB"> theDB 사용되어지는 DBConnection</param>
        protected override void Add(DBConnection theDB)
        {
            base.Add(theDB);
        }

        /// <summary>
        /// Database로 부터 data를 Delete
        /// Sub Class에 의하여 Override되어 져야 하며, base.Delete()항상 먼저 Call되어져야 함
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection </param>
        protected override void Delete(DBConnection theDB)
        {
            base.Delete(theDB);
        }

        /// <summary>
        /// Data base로 부터 Data를 Load함. 항상 Sub Class에 의하여 Override되어
        /// Implement되어져야 함 항상 base.Load()가 먼저 Call되어져야 함
        /// 
        /// SubClass가 Instance화 된 직후, 이 Method을 Call함으로 Configration되어있는
        /// 기준 정보에 의하여 Sub Object가 초기화 되어짐
        /// 
        /// </summary>
        /// <param name="theDB">theDB 사용되어지는 DBConnection</param>
        protected override void Load(DBConnection theDB)
        {
            base.Load(theDB);
        }

        /// <summary>
        /// Data base로 Data를 Save함
        /// Sub Class는 이 Method를 Override하여 Implement하여야 하며,
        /// 항상 base.Save() 먼저 Call되어 상속 계층에 있는 Parent Class가 먼저 
        /// 저장되도록 하여야 함
        /// </summary>
        /// <param name="theDB"></param>
        protected override void Save(DBConnection theDB)
        {
            base.Save(theDB);
        }

        /// <summary>
        /// 주어진 subject의 RemoteEvent 객체를 Return
        /// </summary>
        /// <param name="subject">subject Id</param>
        /// <returns>RemoteEvent</returns>
        public RemoteEvent GetRemoteEventForSubscribe(string subject)
        {
            RemoteEvent remoteEvent = null;

            if (remoteEvents.ContainsKey(subject))
            {
                return remoteEvents[subject] as RemoteEvent;
            }
            else
            {
                remoteEvent = new RemoteEvent(subject);
                remoteEvents.Add(subject, remoteEvent);
            }
            return remoteEvent;
        }

        /// <summary>
        /// 주어진 Subject Id에 해당하는 RemoteEvent객체를 Manager가 가지고 있는
        /// RemoteEvent객체의 Container에서 제거하고, Return함
        /// </summary>
        /// <param name="subject">subject Id</param>
        /// <returns>RemoteEvent</returns>
        public RemoteEvent GetRemoteEventForUnsubscribe(string subject)
        {

            RemoteEvent remoteEvent = null;

            if (remoteEvents.ContainsKey(subject))
            {
                remoteEvent = remoteEvents[subject] as RemoteEvent;
                if (remoteEvent.GetInvocationListCount() <= 1)
                {
                    remoteEvents.Remove(subject);
                }
                return remoteEvent;
            }
            return remoteEvent;
        }      


        /// <summary>
        /// 주어진 subject와 ECPEvent를 Publish
        /// </summary>
        /// <param name="subject">Subject Id</param>
        /// <param name="ecpEvent">ECP Event</param>
        public void Publish(string subject, ECPEvent ecpEvent)
        {
            DelegateEventPublisher publisher = new DelegateEventPublisher(PublishECPEvent);
            // PublishECPEvent(subject, ecpEvent);
            publisher.BeginInvoke(subject, ecpEvent, null, null);
        }

        private void PublishECPEvent(string subject, ECPEvent ecpEvent)
        {
            foreach (string keys in remoteEvents.Keys)
            {
                if (MatchSubjects(keys, subject))
                {
                    remoteEvents[keys].Publish(ecpEvent);
                }
                else
                {
                    // do not publish.
                }
            }
        }

        /// <summary>
        /// Publish 하고자 하는 Subject가 Subcribed된 Subject와 Match하는 지를 검사 
        /// <example>
        /// 만약 Publish되어진 Subject가
        ///    a.b.c
        /// 라고 하면
        /// 
        /// 다음과 같이 Subsribe되어진 Subject는 Match하게 된다.
        ///    a.b.c
        ///    a.b.*
        ///    a.>
        ///    a.b.>
        ///    >
        /// 가변 길이형 Wild Card인 '>'은 모든 Subject에 대하여 Listening이 가능하게 됨
        /// 으로 많은 부하를 유발할 수 있다. 
        /// </example> 
        /// </summary>
        /// 
        /// <param name="theSubscribedSubject">Subscribe되어진 Subject</param>
        /// <param name="thePublishedSubject">Publish하고자하는 Subject</param>
        /// <returns>Match할 경우 true</returns>
        public bool MatchSubjects(string theSubscribedSubject,
                                         string thePublishedSubject)
        {
            StringTokenizer theSubscribedSubjectTokenizer =
                            new StringTokenizer(theSubscribedSubject, Convert.ToString((char)TOKEN_DELIMITER));

            StringTokenizer thePublishedSubjectTokenizer =
                            new StringTokenizer(thePublishedSubject, Convert.ToString((char)TOKEN_DELIMITER));
            bool match = true;
            bool considerTokenCount = true;
            bool equalTokens =
                (theSubscribedSubjectTokenizer.countTokens() ==
                    thePublishedSubjectTokenizer.countTokens()) ? true : false;

            while (theSubscribedSubjectTokenizer.hasMoreTokens() &&
                  thePublishedSubjectTokenizer.hasMoreTokens())
            {
                string theNextSubscribedSubjectToken = theSubscribedSubjectTokenizer.nextToken();
                string theNextPublishedSubjectToken = thePublishedSubjectTokenizer.nextToken();
                if (theNextSubscribedSubjectToken.Equals(Convert.ToString((char)VARIABLE_LENGTH_WILDCARD)))
                {
                    considerTokenCount = false;
                    match = true;
                    break;
                }
                else if (theNextSubscribedSubjectToken.Equals(Convert.ToString((char)FIXED_LENGTH_WILDCARD)) ||
                         theNextSubscribedSubjectToken.Equals(theNextPublishedSubjectToken))
                {
                    continue;
                }
                else
                {
                    match = false;
                    break;
                }
            }

            return ((considerTokenCount ? (equalTokens && match) : match));
        }

        /// <summary>
        /// Publish 하고자 하는 Subject의 Token화된 List가 Subcribed된 Subject의 Token List와 
        /// Match하는 지를 검사 
        /// </summary>
        /// <param name="theSubscribedSubjectTokenList">Subcribed된 Subject의 Token List</param>
        /// <param name="thePublishedSubjectTokenList">Publish 하고자 하는 Subject의 Token화된 List</param>
        /// <returns>Match할 경우 true</returns>
        public bool MatchSubjects(string[] theSubscribedSubjectTokenList,
                                 string[] thePublishedSubjectTokenList)
        {
            if (theSubscribedSubjectTokenList.Length < thePublishedSubjectTokenList.Length &&
                !theSubscribedSubjectTokenList[theSubscribedSubjectTokenList.Length - 1].
                    Equals(Convert.ToString((char)VARIABLE_LENGTH_WILDCARD)))
            {
                return (false);
            }

            int limit = Math.Min(theSubscribedSubjectTokenList.Length,
                                 thePublishedSubjectTokenList.Length);
            for (int i = 0; i < limit; i++)
            {
                if (theSubscribedSubjectTokenList[i].
                        Equals(Convert.ToString((char)VARIABLE_LENGTH_WILDCARD)))
                {
                    return (true);
                }
                else if (theSubscribedSubjectTokenList[i].
                            Equals(Convert.ToString((char)FIXED_LENGTH_WILDCARD)) ||
                         theSubscribedSubjectTokenList[i].
                            Equals(thePublishedSubjectTokenList[i]))
                {
                    continue;
                }
                else
                {
                    return (false);
                }
            }

            return (theSubscribedSubjectTokenList.Length ==
                        thePublishedSubjectTokenList.Length
                            ? true : false);
        }


        #region 내부 개발 테스트용 코드
        public void Test()
        {
            this.ProcessingMessage();
        }

        public void ProcessingMessage()
        {
            //  this.init();  
            this.InitByConfigfile();

            Console.WriteLine("Launcher started. In Processing Message....");
            Console.ReadLine();
        }

        public void Init()
        {
            TcpChannel c = new TcpChannel(9000);
            ChannelServices.RegisterChannel(c, false);

            RemotingConfiguration.RegisterWellKnownServiceType(
                typeof(RemoteEvent), "RemoteEvent", WellKnownObjectMode.Singleton);
        }

        public void InitByConfigfile()
        {
            RemotingConfiguration.Configure(Environment.CurrentDirectory + @"\Test.exe.config", false);
        }
        #endregion
    }
}
