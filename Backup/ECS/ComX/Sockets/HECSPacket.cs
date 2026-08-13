using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

using ECP.ComX.Sockets;
using ECP.Device.Observe;
using ECP.Global.Device;
using ECP.Service.Logger;
using ECP.Util.Client;
using ECP.Event;

namespace HECS.ComX.Sockets
{
    public class HECSTimer : System.Timers.Timer
    {
        HECSPacket pkt;
        public HECSPacket Packet
        {
            get
            {
                return pkt;
            }
            set
            {
                pkt = value;
            }
        }
        public string SeqNo
        {
            get
            {
                return pkt.SeqNo;
            }
            set
            {
                pkt.SeqNo = value;
            }
        }
    }

    public class HECSPacket : PersistentPacket
    {
        const string CLASS = "HECSPacket";

        byte STX = 0x02;
        byte ETX = 0x03;
        byte[] checksum;
        int mRetryLimit;
        int mRetryCount;

        HECSTimer mWaitTimer;
        Hashtable mBodyList;

        public HECSPacket()
        {
            header = null;
            body = null;
            mBodyList = new Hashtable();
            checksum = null;
        }

        public HECSPacket(int headersize)
        {
            header = new byte[headersize];
            body = null;
            mBodyList = new Hashtable();
            checksum = null;
        }

        public HECSTimer WaitTimer
        {
            get
            {
                return mWaitTimer;
            }
            set
            {
                mWaitTimer = value;
            }
        }

        public Hashtable BodyList
        {
            get
            {
                return mBodyList;
            }
        }

        public int RetryLimit
        {
            get
            {
                return mRetryLimit;
            }
            set
            {
                mRetryLimit = value;
            }
        }
        public int RetryCount
        {
            get
            {
                return mRetryCount;
            }
            set
            {
                mRetryCount = value;
            }
        }

        public string Header
        {
            get
            {
                return System.Text.Encoding.Default.GetString(header);
            }
            set
            {
                byte[] buf = System.Text.Encoding.Default.GetBytes(value);
                header = new byte[buf.Length];
                Buffer.BlockCopy(buf, 0, header, 0, buf.Length);
            }
        }

        public string Body
        {
            get
            {
                return System.Text.Encoding.Default.GetString(body);
            }
            set
            {
                byte[] buf = System.Text.Encoding.Default.GetBytes(value);
                body = new byte[buf.Length];
                Buffer.BlockCopy(buf, 0, body, 0, buf.Length);
            }
        }
        public string CheckSum
        {
            get
            {
                return System.Text.Encoding.Default.GetString(checksum);
            }
            set
            {
                byte[] buf = System.Text.Encoding.Default.GetBytes(value);
                checksum = new byte[buf.Length];
                Buffer.BlockCopy(buf, 0, checksum, 0, buf.Length);
            }
        }

        public string Direction
        {
            get
            {
                byte[] tmp = new byte[1];
                tmp[0] = header[0];

                return System.Text.Encoding.Default.GetString(tmp);
            }
            set
            {
                byte[] tmp = System.Text.Encoding.Default.GetBytes(value);
                header[0] = tmp[0];
            }
        }


        public string Reply
        {
            get
            {
                byte[] tmp = new byte[1];
                tmp[0] = header[1];

                return System.Text.Encoding.Default.GetString(tmp);
            }
            set
            {
                byte[] tmp = System.Text.Encoding.Default.GetBytes(value);
                header[1] = tmp[0];
            }
        }

        public string CmdID
        {
            get
            {
                byte[] tmp = new byte[3];
                Buffer.BlockCopy(header, 2, tmp, 0, tmp.Length);

                return System.Text.Encoding.Default.GetString(tmp);
            }
            set
            {
                byte[] tmp = System.Text.Encoding.Default.GetBytes(value);
                Buffer.BlockCopy(tmp, 0, header, 2, 3);
            }
        }

        public string SeqNo
        {
            get
            {
                byte[] tmp = new byte[17];
                Buffer.BlockCopy(header, 5, tmp, 0, tmp.Length);

                return System.Text.Encoding.Default.GetString(tmp);
            }
            set
            {
                byte[] tmp = System.Text.Encoding.Default.GetBytes(value);
                Buffer.BlockCopy(tmp, 0, header, 5, 17);
            }
        }



        public string GetCheckSum()
        {
            byte v = 0;
            int i = 0;

            for (i = 0; i < header.Length; i++)
            {
                v += header[i];
            }
            for (i = 0; i < body.Length; i++)
            {
                v += body[i];
            }

            return v.ToString("X2");
        }

        override public int Read(SocketInputStream in_stream)
        {
            int ret = 0;
            char rCharactor;
            string rString = "";
            byte[] rBuf = new byte[1];

            if (header == null)
                return ret;

            do
            {
                ret += in_stream.Read(rBuf);

                if (rBuf[0] != STX && rBuf[0] != ETX)
                {
                    rCharactor = (char)rBuf[0];
                    rString += rCharactor;
                }
            } while (rBuf[0] != ETX);

            byte[] buf = System.Text.Encoding.Default.GetBytes(rString);

            Buffer.BlockCopy(buf, 0, header, 0, header.Length);

            body = new byte[buf.Length - header.Length - 2];
            Buffer.BlockCopy(buf, header.Length, body, 0, buf.Length - header.Length - 2);

            checksum = new byte[2];
            Buffer.BlockCopy(buf, header.Length + body.Length, checksum, 0, 2);

            return ret;
        }

        public void OnRead()
        {
            string sBuf = "";
            int rIndex = 0;
            int fieldwidth = 0;
            bool bExit = true;
            int index = -1;

            if (GetCheckSum() == this.CheckSum)
            {
                while (true)
                {
                    index += 1;

                    foreach (Observable observable in mRegisteredObservableList.Values)
                    {
                        if (observable.AccessType != AccessType.IN &&
                           observable.IsMonitoring &&
                           observable.ConnectionInfo["CMD_ID"].ToString() == this.CmdID &&
                           observable.ConnectionInfo["INDEX"].ToString() == index.ToString())
                        {
                            fieldwidth = int.Parse(observable.Max.ToString());
                            if (index == 0)
                            {
                                sBuf = Header.Substring(rIndex, fieldwidth);
                            }
                            else
                            {
                                sBuf = Body.Substring(rIndex, fieldwidth);
                                rIndex += fieldwidth;
                            }

                            if (index > 0)
                                BodyList.Add(index.ToString(), sBuf);

                            observable.AsString = sBuf;
                            sBuf = observable.AsString;

                            bExit = bool.Parse(observable.ConnectionInfo["IS_LAST"].ToString());

                            break;
                        }
                    }

                    if (bExit)
                        break;
                }
            }
            else
            {
                // checksum 에러
                foreach (Observable observable in mRegisteredObservableList.Values)
                {
                    if (observable.AccessType != AccessType.IN &&
                       observable.IsMonitoring &&
                       observable.ConnectionInfo["INDEX"].ToString() == index.ToString() &&
                       index == -1)
                    {
                        observable.AsString = sBuf;
                        sBuf = observable.AsString;
                        break;
                    }
                }
            }
        }

        override public void Write(SocketOutputStream out_stream)
        {
            /* 데이터를 쓰기전에 BodyList로 Body를 업데이트 한다. */

            body = null;
            byte[] b = null;
            int sOffset = 0;
            int total = 0;

            for (int i = 0; i < BodyList.Count; i++)
            {
                b = System.Text.Encoding.Default.GetBytes(BodyList[((int)(i + 1)).ToString()].ToString());

                total += b.Length;
            }

            body = new byte[total];

            for (int i = 0; i < BodyList.Count; i++)
            {
                b = System.Text.Encoding.Default.GetBytes(BodyList[((int)(i + 1)).ToString()].ToString());

                Buffer.BlockCopy(b, 0, body, sOffset, b.Length);
                sOffset += b.Length;
            }
            /* body 업데이트 끝 */

            // STX를 씀
            out_stream.Write(STX);

            // 헤더를 씀
            out_stream.Write(header);

            // Body를 씀
            out_stream.Write(body);
           
            // CheckSum을 씀
            b = System.Text.Encoding.Default.GetBytes(GetCheckSum());
            checksum = new byte[b.Length];
            Buffer.BlockCopy(b, 0, checksum, 0, b.Length);
            out_stream.Write(checksum);

            // ETX를 씀
            out_stream.Write(ETX);

            OnWrite();
        }

        public void OnWrite()
        {
            const string METHOD = CLASS + ".OnWrite";

            string sBuf = "";
            int rIndex = 0;
            int fieldwidth = 0;
            bool bExit = true;
            int index = -1;


            while (true)
            {
                index += 1;

                foreach (Observable observable in mRegisteredObservableList.Values)
                {
                    string cmdid = System.Text.Encoding.Default.GetString(header, 2, 3);

                    if (observable.AccessType != AccessType.IN &&
                       observable.IsMonitoring &&
                       observable.ConnectionInfo["CMD_ID"].ToString() == cmdid &&
                       observable.ConnectionInfo["INDEX"].ToString() == index.ToString())
                    {
                        fieldwidth = int.Parse(observable.Max.ToString());
                        if (index == 0)
                        {
                            sBuf = Header.Substring(rIndex, fieldwidth);
                        }
                        else
                        {
                            sBuf = Body.Substring(rIndex, fieldwidth);
                            rIndex += fieldwidth;
                        }

                        observable.AsString = sBuf;
                        sBuf = observable.AsString;

                        bExit = bool.Parse(observable.ConnectionInfo["IS_LAST"].ToString());
                        break;
                    }
                }

                if (bExit)
                    break;
            }

            //Log.log(5, METHOD, "debug", Category.DEBUG, "", "fieldbus",
            //                string.Format("Send Time {0}\r\n\r\n", DateTime.Now.ToString("yyyy/MM/dd hh:mm:ss.fff")));
        }
    }
}
