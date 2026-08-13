using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Event
{
    /// <summary>
    /// ECP Framework은 내부 Event Publish and Subscribe 메카니즘을 갖는다.
    /// ECPEvent Class는 ECP Framework의 여러 Object들간의 상호 전달되는
    /// Message의 최상위 Class가 된다.
    /// 
    /// Event는 subject와 전달되는 Event Body를 가질 수 있다.
    /// ECPEvent는 Serialize되어 .Net Remote 통신 Channel로 객체들에 전달
    /// 되어 진다.
    /// 
    /// </summary>
    [Serializable]
    public class ECPEvent
    {
        /// <summary>
        /// Class Name
        /// </summary>
        private static readonly string CLASS = "ECP.Event.ECPEvent";

        /// <summary>
        /// Event를 구별하기 위하여 Event가 발생한 시각의 Tick을 Assign
        /// 현재의 Message Number, 이런 방법은 현재의 Message는 항상 과거의 Message보다
        /// 큰수를 가지는 것을 보장한다.
        /// </summary>
        private static long currentMsgNum = DateTime.Today.Ticks;

        /// <summary>
        /// Event의 Subject
        /// Event의 Subject는 a.b.c와 같은 형태가 될수 있다.
        /// Event는 Subject는 하위 Class로 상속되어 나갈 수록
        /// 계층화 되어 진다.
        /// </summary>
        protected string theSubject;

        /// <summary>
        /// Publisher ID
        /// </summary>
        public string publisher;

        /// <summary>
        /// Message의 Number, 이 숫자는 이 Server에 의하여 Publish되어지는
        /// 모든 Event에 대하여 Unique하게 되어진다.
        /// </summary>
        public long msgNum;

        /// <summary>
        /// 기본 생성자
        /// </summary>
        static ECPEvent()
        {

        }

        /// <summary>
        /// 기본 생성자
        /// </summary>
        public ECPEvent() : this("")
        {
        }
    
        /// <summary>
        /// ECPEvent를 주어진 subjectId로 생성한다.
        /// </summary>
        /// <param name="theSubject">Subject ID</param>
        public ECPEvent(string theSubject)
        {
            this.theSubject = theSubject;
        }

        /// <summary>
        /// ECP Object의 Data를 string 형으로 Return
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return (CLASS + "::" + "theSubject=" + this.theSubject);
        }

        /// <summary>
        /// Subejct Id을 Retrun
        /// </summary>
        /// <returns></returns>
        public  string GetSubject()
        {
            return this.theSubject;
        }

        /// <summary>
        /// Subject Id을 주어진 subjectid로 설정
        /// </summary>
        /// <param name="theSubject">Subject Id</param>
        public  void SetSubject(string theSubject)
        {
            this.theSubject = theSubject;
        }


        public  void AppendToSubject(string appendee)
        {
            theSubject += appendee;
        }

    }
}
