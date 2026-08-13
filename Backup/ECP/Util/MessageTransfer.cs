using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Util
{
    /// <summary>
    /// MessageTransfer class는 Thread간에 객체를 전달하기 위하여 사용되어진다.
    /// MessageTransfer class는 Sending단과 Receiving단 양쪽 끝에서 사용되어진다.
    /// 다음 열거되는 것들이 이기능을 수행하기 위하여 사용되어진다
    /// 
    /// 1.Queue가 Incoming Message를 Hold하기 위하여 사용되어진다.
    /// 2.Message를 주고 받기 위한 Method가 제공되어진다. 
    /// 여러 카테고리에 따라, 많은 Send Method가 제공되어진다.
    ///   1- 특정 Dest로 Send하는 Method
    ///   2- Listner들에 일괄 Send하는 Method
    ///   3- Reply를 기대하면서 Send
    ///   4- Reply를 Send
    /// 두개의 Receive Method는 Queue가 Empty일 경우 대기하는 또는 대기 하지 않는 형태로
    /// Message를 가져 오기 위하여 사용되어진다.
    /// 
    /// 3. Reply가 요구되어지는 경우에 Message을 Send하는 것이 가능하다. 이경우
    /// MessageTransfer객체는 어떤 MessageTransfer객체가 Reply를 받아야 하는지를
    /// 기억한다. 
    /// 4. MessageTransfer객체의 User가 명명한 Instance을 담고 있는 Table은 
    /// Remote MessageTransfer객체에 Reference을 가지지 않고, Message을 Send하는 것을
    /// 가능하도록 하기 위하여, 유지되어져야 한다.
    /// 
    /// 5. Listening Message Transfer 객체의 List는 유지 되어야 한다. 
    /// Primary Message를 send하는 경우, 각 Listener Message Queue에 Reference가 추가 되어야 한다.
    /// </summary>
    public class MessageTransfer
    {
        private static Dictionary<string, MessageTransfer> messageTransferInstances = 
            new Dictionary<string, MessageTransfer>();

        private static long uniqueId = 0;

        private string messageTransferInstanceName;		// key in the hashtable

        private FIFOQueue inMessages;


        private Dictionary<string, MessageTransfer> listeners;	// MsgXfers to which messages are sent.
        private Dictionary<string, MessageTransfer> listeningTo;	// MsgXfers from which msgs are recvd.
        private Dictionary<object, MessageTransfer> replyTable;

        private bool connectionClosed;

        /// <summary>
        /// 기본 생성자
        /// Unique한 Name이 자동적으로 Message Transfer 객체에 할당되어진다.
        /// 이 Instance는 Instance table에 추가 되어지지 않는다. 왜냐하면
        /// 다른 Thread는 이 Thread의 Name을 결코 알 수 없기 때문이다.
        /// </summary>
        public MessageTransfer():this(null)
        {
              
        }

        /// <summary>
        /// Message Transfer객체를 생성한다. 이 Instance는 Message Tranfser table에 추가 되어진다. 
        /// </summary>
        /// <param name="name">MessageTranfer객체를 인식하기 위하여 사용되어지는 Name</param>
        public MessageTransfer(string name)
        {
            inMessages = new FIFOQueue();
            listeners = new Dictionary<string, MessageTransfer>();
            listeningTo = new Dictionary<string, MessageTransfer>();
            replyTable = new Dictionary<object, MessageTransfer>();

            connectionClosed = false;
            if (name == null)
            {
                messageTransferInstanceName = GetUniqueName();
            }
            else
            {
                messageTransferInstanceName = name;
                messageTransferInstances.Add(name, this);
            }
        }

        /// <summary>
        /// Message Tranfser Instance을 위한 Unique한 이름을 생성
        /// </summary>
        /// <returns>유일한 이름을 담고 있는 String</returns>
        private string GetUniqueName()
        {
            string name;
            long newName = 0;

            lock( typeof(MessageTransfer) )
            {
                newName = uniqueId++;
            }
            name =  Convert.ToString( newName );

            return name;
        }

        /// <summary>
        /// 주어진 이름의 MessageTransfer객체를 Return
        /// </summary>
        /// <param name="name">MessageTransfer객체의 Name</param>
        /// <returns></returns>
        public static MessageTransfer GetInstance(string name)
        {
            if (messageTransferInstances.ContainsKey(name))
            {
                return (MessageTransfer)messageTransferInstances[name];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// MessageTranferInstance의 Name을 Return
        /// </summary>
        /// <returns></returns>
        public string GetName()
        {
            return messageTransferInstanceName;
        }

        /// <summary>
        /// 2개의 MessageTransfer객체 사이에, 2 Way Connection을 설정한다.
        /// 이것은 다른 Trnasfer객체의 Listener List에 자신을 Add함으로 수행되어진다.
        /// </summary>
        /// <param name="name">Remote MessageTranfer객체의 Name</param>
        /// <returns>Connection이 이루어지면 True, 아니면 False</returns>
        public bool EstablishConnection(string name)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                remote.AddListener(this);
                AddListener(remote);
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 특정 MessageTransfer객체에 1 Way Send Connection을 Establish한다.
        /// 이것은 다른 Trnasfer객체의 Listener List에 자신을 Add함으로 수행되어진다. 
        /// </summary>
        /// <param name="name">Remote MessageTranfer객체의 Name</param>
        /// <returns>Connection이 이루어지면 True, 아니면 False</returns>
        public bool EstablishSendConnection(string name)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                AddListener(remote);
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 특정 MessageTransfer객체에 1 Way Receive Connection을 Establish한다.
        /// 이것은 다른 Trnasfer객체의 Listener List에 자신을 Add함으로 수행되어진다.  
        /// </summary>
        /// <param name="name">Remote MessageTranfer객체의 Name</param>
        /// <returns>Connection이 이루어지면 True, 아니면 False</returns>
        public bool EstablishReceiveConnection(string name)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                remote.AddListener(this);
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// MessageTransfer객체를 Close한 메카니즘을 제공한다.
        /// MessageTransfer객체의 사용을 끝낼때, 이 Mehtod는 호출되어지며,
        /// 더이상의 Message는 Queue에 저장되지 않으며, Instance Table로 부터
        /// 객체가 제거되어진다. 연결된 DBConnection은 제거 되어 진다.
        /// </summary>
        public void Close()
        {
            connectionClosed = true;

            messageTransferInstances.Remove(messageTransferInstanceName);

            BreakAllConnections();
        }

        /// <summary>
        /// MesseageTransfer객체가 모든 Connection을 닫는다.
        /// </summary>
        public void BreakAllConnections()
        {
           
            AGAIN1:
            // First break all send connections.
            foreach (MessageTransfer remote in listeners.Values)
            {
                RemoveListener(remote);
                goto AGAIN1;
            }

            AGAIN2:
            // Next break all receive connections.
            foreach (MessageTransfer remote in listeningTo.Values)
            {
                remote.RemoveListener(this);
                goto AGAIN2;
            }
            
        }

        /// <summary>
        /// 주어진 이름의 객체와의 Connection을 Close한다.
        /// </summary>
        /// <param name="name">Close하고자하는 RemoteMessageTransfer객체의 이름</param>
        public void BreakConnection(string name)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                remote.RemoveListener(this);
                RemoveListener(remote);
            }
        }

        /// <summary>
        /// 이 MessageTransfer객체의 모든 Listener를 Return
        /// </summary>
        /// <returns>이 MessageTransfer객체의 모든 Listener</returns>
        public Dictionary<string, MessageTransfer> GetListeners()
        {
            return listeners;
        }

        /// <summary>
        /// Remote MessageTransfer객체를 Local객체의  Listner의 List에 Add한다. 또한
        /// Local MessageTransfer객체를 Remote MessageTrnasfer의 List에 Add한다.
        /// </summary>
        /// <param name="mt">Remote MessageTransfer객체</param>
        private void AddListener(MessageTransfer mt)
        {
            listeners.Add(mt.GetName(), mt);
            mt.AddListeningTo(this);
        }

        /// <summary>
        /// ListeningTo List에 Remote MessageTransfer을 Add함
        /// </summary>
        /// <param name="mt">Remote MessageTrasfer</param>
        private void AddListeningTo(MessageTransfer mt)
        {
            listeningTo.Add(mt.GetName(), mt);
        }

        /// <summary>
        /// Remote MessageTransfer를 Listener의 List에서 삭제 
        /// 또한 이 MessageTransfer를 ListenerTo List에서 삭제함
        /// </summary>
        /// <param name="mt">Remote MessageTransfer</param>
        private void RemoveListener(MessageTransfer mt)
        {
            mt.RemoveListeningTo(this);
            listeners.Remove(mt.GetName());
        }

        /// <summary>
        /// Remote MessageTransfer를 ListenerTo List에서 삭제
        /// </summary>
        /// <param name="mt">Remote MessageTransfer</param>
        private void RemoveListeningTo(MessageTransfer mt)
        {
            listeningTo.Remove(mt.GetName());
        }

        /// <summary>
        /// Message Queue로부터 객체를 Receive함
        /// 객체가 Queue에 없을 경우는 하나가 발생할때까지
        /// Queue에 대기함
        /// </summary>
        /// <returns>MessageTransfer에 보내어질 객체</returns>
        public Object GetMessageWait()
        {
            Object message;

            message = inMessages.Get();
            return message;
        }

        /// <summary>
        /// Message Queue로부터 객체를 Receive함
        /// 만일 객체가 Queue에 없을 경우는, Timeout시간 까지 Queue에 대하여
        /// 대기함. 만약 TimeOut이 발생하면, Null을 Return
        /// </summary>
        /// <param name="timeout">Timeout (mili sencond단위)</param>
        /// <returns>MessageTransfer에 보내어질 객체</returns>
        public Object GetMessageWithTimeout(int timeout)
        {
            Object message;

            message = inMessages.Get(timeout);
            return message;
        }

        /// <summary>
        /// Message Queue로부터 객체를 Receive함
        /// 만일 객체가 Queue에 없을 경우는 이 Method는 즉시 Return함
        /// </summary>
        /// <returns>MessageTransfer에 보내어질 객체, Queue에 객체가 없을 경우 null</returns>
        public Object GetMessageNoWait()
        {
            Object message = null;

            message = inMessages.GetNoWait();
            return message;
        }

        /// <summary>
        /// MessageTranfer객체에 Message를 Send함. 이 Send는 Listner의 List을 사용하지 않으며,
        /// 제공된 구체적인 Dest에 Send함. Instance Table에서 주어진 Name에 해당하는 Remote객체를
        /// 찾아서 Send을 수행함. 
        /// </summary>
        /// <param name="name">Remote MessageTransfer의 Name</param>
        /// <param name="message">전달되어질 Obejct</param>
        /// <returns>보내어질 경우 True,아니면 false</returns>
        public static bool SendMessage(string name, Object message)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                return remote.PutMessage(message);
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// MessageTransfer객체에 Object을 Send. 이 Send는 Listner의 List을 사용하지 않으며,
        /// 제공된 구체적인 Dest에 Send함. Instance Table에서 주어진 Name에 해당하는 Remote객체를
        /// 찾아서 Send을 수행함. 
        /// </summary>
        /// <param name="remote">Remote MessageTransfer</param>
        /// <param name="message">전달되어질 Obejct</param>
        /// <returns>보내어질 경우 True,아니면 false</returns>
        public static bool SendMessage(MessageTransfer remote, Object message)
        {
            return remote.PutMessage(message);
        }


        

        /// <summary>
        /// 주어진 Name에 해당하는 MessageTransfer객체에 Message을 Send한다.
        /// 이 Send Method는 Send하기 전에 Listner의 List에서 Remote MessageTransfer가 있는 것을 확인한 후에
        /// Message를 Send한다. 이것은 RemoteMessageTransfer가 Instance Table에 있는 지를 확인한 후에
        /// Listener의 List에서 발견되어지면, Send한다.
        /// </summary>
        /// <param name="name">Remote MessageTransfer객체의 Name</param>
        /// <param name="message">Send되어질 Object</param>
        /// <returns>보내어질 경우 True,아니면 false</returns>
        public bool SendMessageToListener(string name, object message)
        {
            MessageTransfer remote;
            bool rtnval;

            if (listeners.ContainsKey(name) &&  null != (remote = (MessageTransfer)listeners[name]))
            {
                if (false == (rtnval = remote.PutMessage(message)))
                {
                    RemoveListener(remote);
                }
                return rtnval;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        ///  MessageTransfer객체에 Message을 Send한다.
        /// 이 Method는 진행하기전에 Listener List에 Remote MessageTransfer가 있는 것을 
        /// 확인한 후에 Message를 Send한다. 
        /// </summary>
        /// <param name="remote">Remote MessageTrannsfer객체</param>
        /// <param name="message">전달되어질 Object</param>
        /// <returns>보내어질 경우 True,아니면 false</returns>
        public bool SendMessageToListener(
                            MessageTransfer remote, object message)
        {
            return SendMessageToListener(remote.GetName(), message);
        }

        /// <summary>
        /// 모든 Listener에게 Message를 Send한다.
        /// </summary>
        /// <param name="message">전달하고자 하는 Object</param>
        public void SendMessageToListeners(object message)
        {
            lock (this.listeners)
            {
                foreach (MessageTransfer remote in listeners.Values)
                {
                    if (false == remote.PutMessage(message))
                    {
                        RemoveListener(remote);
                    }
                }
            }
        }

        /// <summary>
        /// Reply를 기대하면서 Message Transfer Object에 Message를 Send한다.
        /// </summary>
        /// <param name="remote"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        public bool SendMessageWithReply(
                            MessageTransfer remote, object message)
        {
            return remote.PutMessageWithReply(this, message);
        }

        /// <summary>
        /// Reply를 기대하면서, 주어진 Name으로, MessageTranfer 객체에 Message를 Send
        /// Listner의 List는 Message를 Send하는데에 사용되지 않는다. Remote객체는 Reply를 위하여
        /// 그것의 Listner List를 사용하지 않는다. 다시 말하면, Connection이 Establish되어질
        /// 필요가 없다.
        /// </summary>
        /// <param name="name">Remote Message Tranfer의 이름</param>
        /// <param name="message">전달되어질 Object</param>
        /// <returns>보내어질 경우 True,아니면 false</returns>
        public bool SendMessageWithReply(string name, Object message)
        {
            MessageTransfer remote;

            if (null != (remote = GetInstance(name)))
            {
                return remote.PutMessageWithReply(this, message);
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 모든 Listener에게 각각의 Remote객체가 응답할것을 기대하면서, Message를 Send한다.
        /// Listener의 List가 Message를 Send하기 위하여 사용되어진다. 그러나
        /// Remote객체는 Reply하기 위하여 Listner의 List를 사용하지는 않는다. 
        /// 다시 말하면, Connection이 Establish되어질 필요가 없다.
        /// </summary>
        /// <param name="message">전달되어질 Object</param>
        public void SendMessageToListenersWithReply(object message)
        {
        
            foreach (MessageTransfer remote in listeners.Values)
            {
                if (false == remote.PutMessageWithReply(this, message))
                {
                    RemoveListener(remote);
                }

            }
        }

        /// <summary>
        /// 조금전에 받은 Message에 대한 Reply Message를 Send한다. 
        /// 조금전 받은 Message는 Reply를 기다리기 Message Transfer를 찾기 위하여
        /// 사용되어진다. Reply는 즉시 보내어진다. 
        /// </summary>
        /// <param name="message">좀 전에 받은 Message</param>
        /// <param name="replyMessage">Reply로 보내져야 할 Message</param>
        /// <returns>Reply Message가 보내어질 경우 True, 아니면 false</returns>
        public bool SendReplyMessage(object message, object replyMessage)
        {
            MessageTransfer mt;
      
            // Look for a listener for this message.
            if (replyTable.ContainsKey(message) &&  null != (mt = replyTable[message]))
            {
                replyTable.Remove(message);
                // Found one - Send only to it.
                mt.PutReplyMessage(replyMessage);
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// 이 루틴은 incomming queue에 Message를 Add하기 위하여 Remote MessageTransfer객체에 의하여
        /// 호출되어진다. 
        /// </summary>
        /// <param name="message">전달되어질 Message</param>
        /// <returns>Queue에 Add되어진 경우 true, 아니면 false</returns>
        private bool PutMessage(object message)
        {
            if (connectionClosed)
            {
                return false;
            }
            else
            {
                inMessages.Put(message);
                return true;
            }
        }

        /// <summary>
        /// 이 루틴은 incomming queue에 Message를 Add하기 위하여 Remote MessageTransfer객체에 의하여
        /// 호출되어진다. 
        /// 
        /// 이것은 다음과 같은 면에 PutMessage Method와는 다른다.
        /// Connection이 Close된 경우, PutMessage는 Message를 Reject하는데에 반하여, 
        /// Reply Message는 항상 Send되어진다. 왜냐하면, Primary Message Sender가 항상 기다리기 때문이다.
        /// </summary>
        /// <param name="message">전송되어진 Object</param>
        private void PutReplyMessage(object message)
        {
            inMessages.Put(message);
        }


        /// <summary>
        /// 이 루틴은 incomming queue에 Message를 Add하기 위하여 Remote MessageTransfer객체에 의하여
        /// 호출되어진다. 이것은 또한 Reply Look Up Table에 Remote를 Add하기 위하여
        /// 이 Message를 Remote에 Put하기 위하여 사용되어진다. 
        /// </summary>
        /// <param name="mt"></param>
        /// <param name="message"></param>
        /// <returns></returns>
        private bool PutMessageWithReply(MessageTransfer mt, object message)
        {
            if (connectionClosed)
            {
                return false;
            }
            else
            {
                replyTable.Add(message, mt);
                inMessages.Put(message);
                return true;
            }
        }


        public override String ToString()
        {

            StringBuilder sb = new StringBuilder();

            sb.Append("Name: " + messageTransferInstanceName);
            sb.Append("\nMessages in queue: " + inMessages.GetSize());


            foreach (MessageTransfer mt in listeners.Values)
            {
                sb.Append("\nHas listener: " + mt.GetName());
            } 

            if (connectionClosed)
            {
                sb.Append("\nConnections are closed");
            }
            else
            {
                sb.Append("\nConnections are open");
            }

            return sb.ToString();
        }


    }
}
