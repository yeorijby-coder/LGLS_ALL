using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs1
{
    public class Secs1Header : SecsHeader
    {
        public Secs1Header()
            : base()
        {
          
        }

        
        public Secs1Header(byte[] header)
            :base(header)
        {
         
        }

       
        public override SecsHeader NewPrimaryHeader()
        {
            if (IsSecondaryMessage())
            {
                Secs1Header secs1Header = new Secs1Header(ToByteArray());
                secs1Header.SetFunction(secs1Header.GetFunction() - 1);
                secs1Header.SetWbit(true);
                return secs1Header;
            }
            else
            {
                return null;
            }
        }

        
        public override SecsHeader NewSecondaryHeader()
        {
            if (IsPrimaryMessage())
            {
                Secs1Header secs1Header = new Secs1Header(ToByteArray());
                secs1Header.SetFunction(secs1Header.GetFunction() + 1);
                secs1Header.SetWbit(false);
                return secs1Header;
            }
            else
            {
                return null;
            }
        }

       
        public override String ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("RWE(");
            result.Append((GetRbit() == true) ? '1' : '0');
            result.Append((GetWbit() == true) ? '1' : '0');
            result.Append((GetEbit() == true) ? '1' : '0');
            result.Append(") DevId(");
            result.Append(GetDeviceId());
            result.Append(") S");
            result.Append(GetStream());
            result.Append("F");
            result.Append(GetFunction());
            result.Append(" BlkId(");
            result.Append(GetBlockId());
            result.Append(") SrcId(");
            result.Append(GetSourceId());
            result.Append(") TransId(");
            result.Append(GetTransactionId());
            result.Append(")");

            return result.ToString();
        }

        
        public bool GetRbit()
        {
            return (ToByteArray()[0] & 0x80) != 0;
        }

        
        public void SetRbit(bool value)
        {
            if (value)
            {
                ToByteArray()[0] |= 0x80;
            }
            else
            {
                ToByteArray()[0] &= 0x7F;
            }
        }

        
        public static void SetRbit(byte[] header, bool value)
        {
            if (value)
            {
                header[0] |= 0x80;
            }
            else
            {
                header[0] &= 0x7F;
            }
        }

        
        public bool GetEbit()
        {
            return (ToByteArray()[4] & 0x80) != 0;
        }

       
        public void SetEbit(bool value)
        {
            if (value)
            {
                ToByteArray()[4] |= 0x80;
            }
            else
            {
                ToByteArray()[4] &= 0x7F;
            }
        }

        
        public int GetDeviceId()
        {
            return ((ToByteArray()[0] & 0x7F) << 8) | (ToByteArray()[1] & 0xFF);
        }

        
        public void SetDeviceId(int deviceId)
        {
            ToByteArray()[0] = (byte)((deviceId >> 8) & 0x7F);
            ToByteArray()[1] = (byte)(deviceId & 0xFF);
        }

        
        public static void SetDeviceId(byte[] header, int deviceId)
        {
            header[0] = (byte)((deviceId >> 8) & 0x7F);
            header[1] = (byte)(deviceId & 0xFF);
        }

        
        public int GetBlockId()
        {
            return ((ToByteArray()[4] & 0x7F) << 8) | (ToByteArray()[5] & 0xFF);
        }

       
        public void SetBlockId(int blockId)
        {
            ToByteArray()[4] = (byte)((blockId >> 8) & 0x7F);
            ToByteArray()[5] = (byte)(blockId & 0xFF);
        }
    }
}
