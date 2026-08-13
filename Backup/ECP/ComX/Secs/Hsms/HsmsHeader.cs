using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Hsms
{
    public class HsmsHeader : SecsHeader
    {
        public HsmsHeader():base()
        {

        }

        
        public HsmsHeader(HsmsHeader hsmsHeader):base(hsmsHeader)
        {

        }

        
        public HsmsHeader(byte[] header):base(header)
        {

        }

        
        public override SecsHeader NewPrimaryHeader()
        {
            if (IsSecondaryMessage())
            {
                HsmsHeader hsmsHeader = new HsmsHeader(ToByteArray());
                hsmsHeader.SetFunction(hsmsHeader.GetFunction() - 1);
                hsmsHeader.SetWbit(true);
                return hsmsHeader;
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
                HsmsHeader hsmsHeader = new HsmsHeader(ToByteArray());
                hsmsHeader.SetFunction(hsmsHeader.GetFunction() + 1);
                hsmsHeader.SetWbit(false);
                return hsmsHeader;
            }
            else
            {
                return null;
            }
        }

        
        public override String ToString()
        {
            StringBuilder result = new StringBuilder();

            switch (GetSType())
            {
                case 0: // Data message
                    // Give the standard header
                    result.Append("Wbit(");
                    result.Append((GetWbit() == true) ? '1' : '0');
                    result.Append(") SesId(");
                    result.Append(GetSessionId());
                    result.Append(") S");
                    result.Append(GetStream());
                    result.Append("F");
                    result.Append(GetFunction());
                    result.Append(" PType(");
                    result.Append(GetPType());
                    result.Append(") SType(");
                    result.Append(GetSType());
                    result.Append(") SysBytes(");
                    result.Append(GetSystemBytes());
                    result.Append(")");
                    break;
                case 1:
                    result.Append("Select Request");
                    break;
                case 2:
                    result.Append("Select Response - ");
                    switch (GetFunction())
                    {
                        case 0:
                            result.Append("Accepted");
                            break;
                        case 1:
                            result.Append("Already Active");
                            break;
                        case 2:
                            result.Append("Not Ready");
                            break;
                        case 3:
                            result.Append("Connect Exhaust");
                            break;
                        default:
                            result.Append("Unknown status" + GetFunction());
                            break;
                    }
                    break;
                case 3:
                    result.Append("Deselect Request");
                    break;
                case 4:
                    result.Append("Deselect Response - ");
                    switch (GetFunction())
                    {
                        case 0:
                            result.Append("Communication Ended");
                            break;
                        case 1:
                            result.Append("Communication Not Established");
                            break;
                        case 2:
                            result.Append("Communication Busy");
                            break;
                        default:
                            result.Append("Unknown status" + GetFunction());
                            break;
                    }
                    break;
                case 5:
                    result.Append("Linktest Request");
                    break;
                case 6:
                    result.Append("Linktest Response");
                    break;
                case 7:
                    result.Append("Reject - ");
                    switch (GetFunction())
                    {
                        case 1:
                            result.Append("SType not supported");
                            break;
                        case 2:
                            result.Append("PType not supported");
                            break;
                        case 3:
                            result.Append("Transaction not open");
                            break;
                        case 4:
                            result.Append("Not selected");
                            break;
                        default:
                            result.Append("Unknown reason code " + GetFunction());
                            break;
                    }
                    break;
                case 9:
                    result.Append("Separate request");
                    break;
                default:
                    result.Append("Unknown SType " + GetSType());
                    break;
            }

            return result.ToString();
        }

        
        public static void SetSessionId(byte[] header, int sessionId)
        {
            header[0] = (byte)((sessionId >> 8) & 0xFF);
            header[1] = (byte)(sessionId & 0xFF);
        }

        
        public int GetByte2()
        {
            return ToByteArray()[2] & 0xFF;
        }

        
        public void SetByte2(int value)
        {
            ToByteArray()[2] = (byte)(value & 0xFF);
        }

        
        public int GetByte3()
        {
            return ToByteArray()[3] & 0xFF;
        }

        
        public void SetByte3(int value)
        {
            ToByteArray()[3] = (byte)(value & 0xFF);
        }

        
        public int GetPType()
        {
            return ToByteArray()[4] & 0xFF;
        }

        
        public void SetPType(int pType)
        {
            ToByteArray()[4] = (byte)(pType & 0xFF);
        }

        
        public static void SetPType(byte[] header, int pType)
        {
            header[4] = (byte)(pType & 0xFF);
        }

        
        public int GetSType()
        {
            return ToByteArray()[5] & 0xFF;
        }

       
        public void SetSType(int sType)
        {
            ToByteArray()[5] = (byte)(sType & 0xFF);
        }

       
        public static void SetSType(byte[] header, int sType)
        {
            header[4] = (byte)(sType & 0xFF);
        }
    }
}
