using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs
{
    public class SecsHeader
    {
        private static int systemBytes = 0;
        private byte[] headerData = new byte[10];

        public SecsHeader()
        {
            SetSystemBytes();
        }

        public SecsHeader(SecsHeader secsHeader)
        {
            FromByteArray(secsHeader.ToByteArray());
        }

        public SecsHeader(byte[] header)
        {
            FromByteArray(header);
        }


        public virtual SecsHeader NewPrimaryHeader()
        {
            if (IsSecondaryMessage())
            {
                SecsHeader secsHeader = new SecsHeader(this);
                secsHeader.SetFunction(secsHeader.GetFunction() - 1);
                secsHeader.SetWbit(true);
                return secsHeader;
            }
            else
            {
                return null;
            }
        }

        public virtual SecsHeader NewSecondaryHeader()
        {
            if (IsPrimaryMessage())
            {
                SecsHeader secsHeader = new SecsHeader(this);
                secsHeader.SetFunction(secsHeader.GetFunction() + 1);
                secsHeader.SetWbit(false);
                return secsHeader;
            }
            else
            {
                return null;
            }
        }


        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("Wbit(");
            result.Append((GetWbit() == true) ? '1' : '0');
            result.Append(") S");
            if (GetStream() < 10)
            {
                result.Append('0');
            }
            result.Append(GetStream());
            result.Append("F");
            if (GetFunction() < 10)
            {
                result.Append('0');
            }
            result.Append(GetFunction());
            result.Append(" SysBytes(");
            result.Append(GetSystemBytes());
            result.Append(")");

            return result.ToString();
        }
        public int HashCode()
        {
            return GetSystemBytes();
        }

        public override int GetHashCode()
        {
            return GetSystemBytes();
        }

        public override bool Equals(object obj)
        {
            SecsHeader secsHeader = (SecsHeader)obj;
            if (GetSystemBytes() == secsHeader.GetSystemBytes())
            {
                return true;
            }
            return false;
        }

        

        public String GetDefaultName()
        {
            return "S" + GetStream() + "F" + GetFunction();
        }

        public String GetStringId()
        {
            StringBuilder result = new StringBuilder();

            result.Append('S');
            if (GetStream() < 10)
            {
                result.Append('0');
            }
            result.Append(GetStream());

            result.Append('F');
            if (GetFunction() < 10)
            {
                result.Append('0');
            }
            result.Append(GetFunction());

            if (GetWbit())
            {
                result.Append("[W]");
            }

            return result.ToString();
        }

        public int GetMessageIndex()
        {
            return GetStream() * 1000 + GetFunction();
        }

        public int GetMessageId()
        {
            return (GetStream() << 8) | GetFunction();
        }

        public bool IsPrimaryMessage()
        {
            return (headerData[3] & 1) != 0;
        }


        public bool IsSecondaryMessage()
        {
            return (headerData[3] & 1) == 0;
        }


        public byte[] ToByteArray()
        {
            return headerData;
        }

        public void FromByteArray(byte[] headerData)
        {
            Array.Copy(headerData, 0, this.headerData, 0, 10);
        }



        public void SetHeader(byte[] headerData)
        {
            this.headerData = headerData;
        }


        public int GetSessionId()
        {
            return ((headerData[0] & 0xFF) << 8) | (headerData[1] & 0xFF);
        }

        
        public void SetSessionId(int sessionId)
        {
            headerData[0] = (byte)((sessionId >> 8) & 0xFF);
            headerData[1] = (byte)(sessionId & 0xFF);
        }



        public bool GetWbit()
        {
            return (headerData[2] & 0x80) != 0;
        }

  

        public void SetWbit(bool value)
        {
            if (value)
            {
                headerData[2] |= 0x80;
            }
            else
            {
                headerData[2] &= 0x7F;
            }
        }


        public int GetStream()
        {
            return headerData[2] & 0x7F;
        }

        public void SetStream(int stream)
        {
            headerData[2] = (byte)(stream & 0x7F);
        }

        
        public int GetFunction()
        {
            return headerData[3] & 0xFF;
        }


        public void SetFunction(int function)
        {
            headerData[3] = (byte)(function & 0xFF);
        }


        public int GetSourceId()
        {
            return ((headerData[6] & 0xFF) << 8) | (headerData[7] & 0xFF);
        }


        public void SetSourceId(int sourceId)
        {
            headerData[6] = (byte)((sourceId >> 8) & 0xFF);
            headerData[7] = (byte)(sourceId & 0xFF);
        }

        public int GetTransactionId()
        {
            return ((headerData[8] & 0xFF) << 8) | (headerData[9] & 0xFF);
        }


        public void SetTransactionId(int transId)
        {
            headerData[8] = (byte)((transId >> 8) & 0xFF);
            headerData[9] = (byte)(transId & 0xFF);
        }


        
        public int GetSystemBytes()
        {
            return ((headerData[6] & 0xFF) << 24) |
                   ((headerData[7] & 0xFF) << 16) |
                   ((headerData[8] & 0xFF) << 8) |
                    (headerData[9] & 0xFF);
        }
        
        public void SetSystemBytes(int sysBytes)
        {
            headerData[6] = (byte) ((sysBytes >> 24) & 0xFF);
            headerData[7] = (byte) ((sysBytes >> 16) & 0xFF);
            headerData[8] = (byte) ((sysBytes >>  8) & 0xFF);
            headerData[9] = (byte) ( sysBytes & 0xFF);
        }


        public void SetSystemBytes()
        {
            int systemBytes;

            lock( this )
            {
                systemBytes = SecsHeader.systemBytes++;
            }
            SetSystemBytes(systemBytes);
        }

        public int GetSize()
        {
            return 10;
        }

    }
}
