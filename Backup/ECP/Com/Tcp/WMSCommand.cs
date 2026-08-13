using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace ECP.Com.Tcp
{
    public class WMSCommand
    {
        public enum MESSAGE_ERROR
        {
            NO_ERROR,
            CHECKSUM,
            SIZE
        }

        public enum REPLY_ACK_TYPE
        {
            A, //ACK
            N, //NAK
        }
        public enum MESSAGE_TYPE
        {
            M,  //모드변경 
            O,  //작업지시 
            R,  //재작업지시
            S,  //상태보고 
            E,  //에러보고
            F,  //작업완료보고
            Unknown,    //알수없음.
        }

        public enum MESSAGE_NAME
        {
            ECS_MBX,  //ECS 송신 
            WMS_MBX,  //WMS 송신
        }

        protected readonly int HEADER_SIZE = 15;
        protected readonly int BODY_SIZE = 11;
        protected readonly int S_BODY_SIZE = 43;
        protected readonly int E_BODY_SIZE = 23;
        protected readonly int F_BODY_SIZE = 10;
        protected readonly byte START_OF_TEXT = 2;
        protected readonly byte END_OF_TEXT = 3;


        protected byte[] header = null;
        protected byte[] body = null;

        #region HeadMessage

        #endregion
        public string MessageBox
        {
            get
            {
                return Encoding.ASCII.GetString(header, 0, 10);

            }
            set
            {
                string strTemp = value.PadRight(10, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, header, 0);
            }
        }

        public string MessageSize
        {
            get
            {
                return Encoding.ASCII.GetString(header, 10, 4);

            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, header, 10);
            }
        }

        #region replyMessage
        public string ReplyMessageType
        {
            get
            {
                return Encoding.ASCII.GetString(body, 1, 1);

            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 1);
            }
        }

        public string ReplyACK
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 1);

            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }

        public string ReplyReason
        {
            get
            {
                return Encoding.ASCII.GetString(body, 3, 2);

            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }
        public string ReplyLuggageNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 5, 4);

            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 5);
            }
        }
        public string ReplyStepCount
        {
            get
            {
                return Encoding.ASCII.GetString(body, 6, 1);

            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 6);
            }
        }
        #endregion



        public MESSAGE_TYPE MessageType
        {
            get
            {
                string messageType = Encoding.ASCII.GetString(body, 1, 1);
                switch (messageType)
                {
                    case "M":
                        return MESSAGE_TYPE.M;
                    case "O":
                        return MESSAGE_TYPE.O;
                    case "E":
                        return MESSAGE_TYPE.E;
                    case "R":
                        return MESSAGE_TYPE.R;
                    case "S":
                        return MESSAGE_TYPE.S;
                    case "F":
                        return MESSAGE_TYPE.F;
                    default:
                        return MESSAGE_TYPE.Unknown;
                }
            }
            set
            {
                string strTemp = value.ToString().PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 1);
            }
        }

        #region M_Message
        public string M_ConveyorNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }

        public string M_IOMode
        {
            get
            {
                return Encoding.ASCII.GetString(body, 5, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 5);
            }
        }
        #endregion

        #region O_Message
        public string O_JobDefine
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }

        public string O_LuggageNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 3, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }
        public string O_SeqNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 7, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 7);
            }
        }

        public string O_PalletNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 11, 7);
            }
            set
            {
                string strTemp = value.PadRight(7, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 11);
            }
        }
        public string O_SourceStn
        {
            get
            {
                return Encoding.ASCII.GetString(body, 18, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 18);
            }
        }
        public string O_SourceBank
        {
            get
            {
                return Encoding.ASCII.GetString(body, 21, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 21);
            }
        }
        public string O_SourceBay
        {
            get
            {
                return Encoding.ASCII.GetString(body, 23, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 23);
            }
        }
        public string O_SourceLevel
        {
            get
            {
                return Encoding.ASCII.GetString(body, 26, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 26);
            }
        }
        public string O_RouteStn
        {
            get
            {
                return Encoding.ASCII.GetString(body, 28, 3);

            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 28);
            }
        }
        public string O_DestStn
        {
            get
            {
                return Encoding.ASCII.GetString(body, 31, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }
        public string O_DestBank
        {
            get
            {
                return Encoding.ASCII.GetString(body, 34, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 34);
            }
        }
        public string O_DestBay
        {
            get
            {
                return Encoding.ASCII.GetString(body, 36, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 36);
            }
        }
        public string O_DestLevel
        {
            get
            {
                return Encoding.ASCII.GetString(body, 39, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 39);
            }
        }
        public string O_Priority
        {
            get
            {
                return Encoding.ASCII.GetString(body, 41, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 41);
            }
        }

        public string O_JobRouting
        {
            get
            {
                return Encoding.ASCII.GetString(body, 44, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 44);
            }
        }
        public string O_ProductID
        {
            get
            {
                return Encoding.ASCII.GetString(body, 45, 20);
            }
            set
            {
                string strTemp = value.PadRight(20, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 45);
            }
        }
        #endregion

        #region R_Message
        public string R_JobDefine
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }

        public string R_LuggageNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 3, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }

        public string R_SourceStn
        {
            get
            {
                return Encoding.ASCII.GetString(body, 7, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 7);
            }
        }
        public string R_SourceBank
        {
            get
            {
                return Encoding.ASCII.GetString(body, 10, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 10);
            }
        }
        public string R_SourceBay
        {
            get
            {
                return Encoding.ASCII.GetString(body, 12, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 12);
            }
        }
        public string R_SourceLevel
        {
            get
            {
                return Encoding.ASCII.GetString(body, 15, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 15);
            }
        }

        public string R_DestStn
        {
            get
            {
                return Encoding.ASCII.GetString(body, 17, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 17);
            }
        }
        public string R_DestBank
        {
            get
            {
                return Encoding.ASCII.GetString(body, 20, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 20);
            }
        }
        public string R_DestBay
        {
            get
            {
                return Encoding.ASCII.GetString(body, 22, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 22);
            }
        }
        public string R_DestLevel
        {
            get
            {
                return Encoding.ASCII.GetString(body, 25, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 25);
            }
        }
        public string R_Kind
        {
            get
            {
                return Encoding.ASCII.GetString(body, 27, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 27);
            }
        }
        public string R_StackNumber
        {
            get
            {
                return Encoding.ASCII.GetString(body, 28, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 28);
            }
        }
        #endregion


        #region S_Message
        public string S_SC01
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }
        public string S_SC01Status
        {
            get
            {
                return Encoding.ASCII.GetString(body, 4, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 4);
            }
        }
        public string S_LuggageNo01
        {
            get
            {
                return Encoding.ASCII.GetString(body, 5, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 5);
            }
        }
        public string S_SC02
        {
            get
            {
                return Encoding.ASCII.GetString(body, 9, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 9);
            }
        }
        public string S_SC02Status
        {
            get
            {
                return Encoding.ASCII.GetString(body, 11, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 11);
            }
        }
        public string S_LuggageNo02
        {
            get
            {
                return Encoding.ASCII.GetString(body, 12, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 12);
            }
        }
        public string S_SC03
        {
            get
            {
                return Encoding.ASCII.GetString(body, 16, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 16);
            }
        }
        public string S_SC03Status
        {
            get
            {
                return Encoding.ASCII.GetString(body, 18, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 18);
            }
        }
        public string S_LuggageNo03
        {
            get
            {
                return Encoding.ASCII.GetString(body, 19, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 19);
            }
        }
        public string S_SC04
        {
            get
            {
                return Encoding.ASCII.GetString(body, 23, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 23);
            }
        }
        public string S_SC04Status
        {
            get
            {
                return Encoding.ASCII.GetString(body, 25, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 25);
            }
        }
        public string S_LuggageNo04
        {
            get
            {
                return Encoding.ASCII.GetString(body, 26, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 26);
            }
        }
        public string S_SC05
        {
            get
            {
                return Encoding.ASCII.GetString(body, 30, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 30);
            }
        }
        public string S_SC05Status
        {
            get
            {
                return Encoding.ASCII.GetString(body, 32, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 32);
            }
        }
        public string S_LuggageNo05
        {
            get
            {
                return Encoding.ASCII.GetString(body, 33, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 33);
            }
        }
        public string S_ExtInputOnly
        {
            get
            {
                return Encoding.ASCII.GetString(body, 37, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 37);
            }
        }
        public string S_PickingWorkConveyor
        {
            get
            {
                return Encoding.ASCII.GetString(body, 38, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 38);
            }
        }
        public string S_MaterialInputConveyor
        {
            get
            {
                return Encoding.ASCII.GetString(body, 39, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 39);
            }
        }
        public string S_InputConveyorMode
        {
            get
            {
                return Encoding.ASCII.GetString(body, 40, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 40);
            }
        }
        public string S_SC01ConveyorMode
        {
            get
            {
                return Encoding.ASCII.GetString(body, 41, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 41);
            }
        }

        #endregion

        #region E_Message
        public string E_DeviceClass
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }

        public string E_DeviceNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 3, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }
        public string E_ErrorKind
        {
            get
            {
                return Encoding.ASCII.GetString(body, 6, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 6);
            }
        }
        public string E_ErrorCode
        {
            get
            {
                return Encoding.ASCII.GetString(body, 7, 4);
            }
            set
            {
                string strTemp = value.PadRight(4, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 7);
            }
        }
        public string E_LuggageNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 11, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 11);
            }
        }
        public string E_ErrorBank
        {
            get
            {
                return Encoding.ASCII.GetString(body, 15, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 15);
            }
        }
        public string E_ErrorBay
        {
            get
            {
                return Encoding.ASCII.GetString(body, 17, 3);
            }
            set
            {
                string strTemp = value.PadLeft(3, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 17);
            }
        }
        public string E_ErrorLevel
        {
            get
            {
                return Encoding.ASCII.GetString(body, 20, 2);
            }
            set
            {
                string strTemp = value.PadLeft(2, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 20);
            }
        }
        #endregion



        #region F_Message

        public string F_JobDefine
        {
            get
            {
                return Encoding.ASCII.GetString(body, 2, 1);
            }
            set
            {
                string strTemp = value.PadRight(1, ' ');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 2);
            }
        }
        public string F_LuggageNo
        {
            get
            {
                return Encoding.ASCII.GetString(body, 3, 4);
            }
            set
            {
                string strTemp = value.PadLeft(4, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 3);
            }
        }
        public string F_CompleteClass
        {
            get
            {
                return Encoding.ASCII.GetString(body, 7, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 7);
            }
        }
        public string F_StepCount
        {
            get
            {
                return Encoding.ASCII.GetString(body, 8, 1);
            }
            set
            {
                string strTemp = value.PadLeft(1, '0');
                Encoding.ASCII.GetBytes(strTemp, 0, strTemp.Length, body, 8);
            }
        }

        #endregion




        private MESSAGE_ERROR error = MESSAGE_ERROR.NO_ERROR;

        public MESSAGE_ERROR Error
        {
            get
            {
                return error;
            }
            set
            {
                error = value;
            }
        }

        protected byte[] replyBody = null;

        //Reply Message 만들기
        public WMSCommand()
        {
            header = new byte[HEADER_SIZE];
            for (int i = 0; i < header.Length; i++)
            {
                header[i] = (byte)' ';
            }
            header[HEADER_SIZE - 1] = (byte)' ';

            body = new byte[BODY_SIZE];
            for (int i = 0; i < body.Length; i++)
            {
                if (i == 0)
                {
                    body[i] = START_OF_TEXT;
                }
                else if (i == (body.Length - 1))
                {
                    body[i] = END_OF_TEXT;
                } 
                else
                {
                    body[i] = (byte)' ';
                }
            }
        }

        //일반 Message 만들기
        public WMSCommand(MESSAGE_TYPE messageType)
        {
            int iBodySize = 0;

            header = new byte[HEADER_SIZE];
            for (int i = 0; i < header.Length; i++)
            {
                header[i] = (byte)' ';
            }
            header[HEADER_SIZE - 1] = (byte)' ';

            if (messageType == MESSAGE_TYPE.S)
            {
                iBodySize = S_BODY_SIZE;
            }
            else if (messageType == MESSAGE_TYPE.E)
            {
                iBodySize = E_BODY_SIZE;
            }
            else if (messageType == MESSAGE_TYPE.F)
            {
                iBodySize = F_BODY_SIZE;
            }
            else
            {
                iBodySize = BODY_SIZE;
            }

            body = new byte[iBodySize];
            for (int i = 0; i < body.Length; i++)
            {
                if (i == 0)
                {
                    body[i] = START_OF_TEXT;
                }
                else if (i == (body.Length - 1))
                {
                    body[i] = END_OF_TEXT;
                }
                else
                {
                    body[i] = (byte)' ';
                }
            }
        }

        //수신 메시지 size
        public WMSCommand(string[] message)
        {
            header = new byte[HEADER_SIZE];
            for (int i = 0; i < header.Length; i++)
            {
                header[i] = (byte)' ';
            }

            if (message.Length == 2)
            {
                body = new byte[message[1].Length];
                for (int i = 0; i < body.Length; i++)
                {
                    if (i == 0)
                    {
                        body[i] = START_OF_TEXT;
                    }
                    else if (i == (body.Length - 1))
                    {
                        body[i] = END_OF_TEXT;
                    }
                    else
                    {
                        body[i] = (byte)' ';
                    }
                }
            }
            else
            {
                body = new byte[BODY_SIZE];

                for (int i = 0; i < body.Length; i++)
                {
                    if (i == 0)
                    {
                        body[i] = START_OF_TEXT;
                    }
                    else if (i == (body.Length - 1))
                    {
                        body[i] = END_OF_TEXT;
                    }
                    else
                    {
                        body[i] = (byte)' ';
                    }
                }
            }
        }

        public string BinaryString
        {
            get
            {
                byte[] binaryByte = new byte[header.Length + body.Length];


                for (int i = 0; i < header.Length; i++)
                {
                    binaryByte[i] = header[i];
                }

                for (int i = 0; i < body.Length; i++)
                {
                    binaryByte[HEADER_SIZE + i] = body[i];
                }

                return Encoding.ASCII.GetString(binaryByte);
            }
            set
            {
                body = new byte[value.Length - HEADER_SIZE];
                byte[] binaryByte = new byte[value.Length];
                Encoding.ASCII.GetBytes(value, 0, value.Length, binaryByte, 0);
                Array.Copy(binaryByte, 0, header, 0, HEADER_SIZE);
                Array.Copy(binaryByte, HEADER_SIZE, body, 0, body.Length);
            }
        }

        public WMSCommand checkMessage()
        {
            WMSCommand replyCommand = new WMSCommand();

            replyCommand.MessageBox = MESSAGE_NAME.ECS_MBX.ToString();
            replyCommand.MessageSize = BODY_SIZE.ToString();
            
            replyCommand.ReplyLuggageNo = "0000";
            replyCommand.ReplyStepCount = "0";
            
            
            //1. 메세지 사이즈 확인 07
            if (int.Parse(this.MessageSize) != body.Length)
            {
                replyCommand.ReplyMessageType = "x"; 
                replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
                replyCommand.ReplyReason = "07";

                return replyCommand;
            }

            //2. STX 확인 01
            if (body[0] != START_OF_TEXT)
            {
                replyCommand.ReplyMessageType = "x"; 
                replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
                replyCommand.ReplyReason = "01";

                return replyCommand;
            }
            
            //3. ETX 확인 02
            if (body[body.Length - 1] != END_OF_TEXT)
            {
                replyCommand.ReplyMessageType = "x"; 
                replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
                replyCommand.ReplyReason = "02";
            }//작업 번호 확인 03
            
            //4. Invalid MailBox Name 13
            if ((this.MessageBox.Trim() != MESSAGE_NAME.WMS_MBX.ToString()))
            {
                replyCommand.ReplyMessageType = "x"; 
                replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
                replyCommand.ReplyReason = "13";

                return replyCommand;
            }

            //4. Invalid Mail Type Name 16

            bool typeFlag = true;
            switch (this.MessageType)
            {
                case MESSAGE_TYPE.M:
                    typeFlag = true;
                    break;
                case MESSAGE_TYPE.O:
                    typeFlag = true;
                    break;
                case MESSAGE_TYPE.E:
                    typeFlag = true;
                    break;
                case MESSAGE_TYPE.R:
                    typeFlag = true;
                    break;
                case MESSAGE_TYPE.S:
                    typeFlag = true;
                    break;
                case MESSAGE_TYPE.F:
                    typeFlag = true;
                    break;
                default:
                    typeFlag = false;
                    break;
            }

            if (!typeFlag)
            {
                replyCommand.ReplyMessageType = "x";
                replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
                replyCommand.ReplyReason = "16";

                return replyCommand;
            }

            replyCommand.ReplyMessageType = this.MessageType.ToString().ToLower();

            if (this.MessageType == MESSAGE_TYPE.O)
            {
                replyCommand.ReplyLuggageNo = this.O_LuggageNo;
            }
            else if (this.MessageType == MESSAGE_TYPE.R)
            {
                replyCommand.ReplyLuggageNo = this.O_LuggageNo;
            }

            replyCommand.ReplyACK = REPLY_ACK_TYPE.A.ToString();
            replyCommand.ReplyReason = "00";

            return replyCommand;
        }

        public WMSCommand errorReplyMessage()
        {
            WMSCommand replyCommand = new WMSCommand();

            replyCommand.MessageBox = MESSAGE_NAME.ECS_MBX.ToString();
            replyCommand.MessageSize = BODY_SIZE.ToString();

            replyCommand.ReplyMessageType = "x";

            replyCommand.ReplyLuggageNo = "0000";
            replyCommand.ReplyStepCount = "0";


            replyCommand.ReplyACK = REPLY_ACK_TYPE.N.ToString();
            replyCommand.ReplyReason = "01";

            return replyCommand;
        }


        public void sendReplyMessage(WMSCommand command)
        {
            WMSCommand replyCommand = new WMSCommand();

        }
    }
}
