using System;
using System.Collections.Generic;
using System.Text;

using System.Runtime.Remoting;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting.Channels.Tcp;

using ECP.Global;
using ECP.Util.Server;
using ECP.Service.Logger;


namespace ECP.Boot
{
    /// <summary>
    /// Server는 Operating System의 Process에 해당한다.
    /// 하나의 Process는 여러개의 Launcher을 가질 수 있도록 Configuration 될 수 있으며,
    /// 하나의 Launcher는 여러개의 Manager를 가질 수 있도록 Configuration 될 수 있다.
    /// 
    /// Manager의 분산 원칙에 따라, 하나의 Process에서 모든 Manager를 할당 할 수도 있으며,
    /// 경우에 따라, 여러 Operating System의 Process로 분산 되어 질 수 있다.
    /// 
    /// 어떤 경우는 Process을 여러대의 Box(node)에 나누어 분산 배치도 가능하다.
    /// 이것은 Manager들이 Network상의 .Remoting 객체로 서비스를 제공하기 때문이다.
    /// </summary>
    public class Server : ECPObject
    {
        /// <summary>
        /// Class Name
        /// </summary>
        public static readonly string CLASS = "Server";
        

        /// <summary>
        /// Laucher Class
        /// </summary>
        Launcher launcher = null;

        #region Constructor

        /// <summary>
        /// 생성자
        /// Launcher를 기동한다.
        /// </summary>
        public Server()
        {
            launcher = new Launcher();
        }

        /// <summary>
        /// 주어진 환경 설정 파일로 Remote Server를 최기화 한다.
        /// </summary>
        /// <param name="remoteConfigFilename">Remoting 환경 설정 파일 Name</param>
        public void Init(string remoteConfigFilename)
        {
            Naming.Init(remoteConfigFilename);
            RemotingServices.Marshal(this.launcher, "Launcher");
        }

        /// <summary>
        /// Remote를 사용하지 않고 launcher를 Local로 초기화
        /// </summary>
        public void Init()
        {
            Naming.Init(launcher);
        }

        #endregion Constructor

        #region Method

        /// <summary>
        /// Debug 용 Method
        /// Server를 Console상에서 foreground로 기동시 Event를 대기하기 위한 Main Thread를 Wait하는 Method
        /// </summary>
        public void ProcessingMessage()
        {
            Console.WriteLine("Server started. In Processing Message....");
            Console.ReadLine();
        }

        /// <summary>
        /// Server의 상태를 Ping하는 Method
        /// </summary>
        /// <returns>살아 있을 경우 true</returns>
        public override bool IsAlive()
        {
            return true;
        }
        #endregion Method
    }
}
