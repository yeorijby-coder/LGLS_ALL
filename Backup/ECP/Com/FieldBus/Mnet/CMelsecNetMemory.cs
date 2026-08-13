/// <summary>
/// 
/// </summary>
/// <filename></filename>
/// <version>1.0.0.0</version>
/// <modifications>
/// 	1.  ver 1.0.0.0     2006-00-00 00:00:00     Jongseok Song   -
///         : Created.
/// </modifications>
/// <copyright>Copyright (c) 2005~2007. EzControl, LG CNS All rights reserved.</copyright>
/// 

using System;
using System.Collections.Generic;
using System.Text;

namespace EzControl.Driver.MelsecNet
{
    public enum enumDeviceType
    {
        X = 1,
        Y = 2,
        SB = 5,
        SW = 14,
        B = 23,
        W = 24,
        WW = 36,
        WR = 37,
        R = 22
    }

    public enum enumMemoryType
    {
        Bit,
        Word
    }

    #region CMelsecNetMemory
    /// <summary>
    /// CMelsecNetMemory에 대한 요약 설명입니다.
    /// </summary>
    public class CMelsecNetMemory
    {
        private int m_nMemorySize;
        public int RawMemorySize
        {
            get { return m_nMemorySize; }
        }
        private short[] m_asRawMemory;
        public short[] RawMemory
        {
            get { return m_asRawMemory; }
        }
        private enumDeviceType m_DeviceType;
        public enumDeviceType DeviceType
        {
            get { return m_DeviceType; }
        }

        private enumMemoryType m_MemoryType;
        public enumMemoryType MemoryType
        {
            get { return m_MemoryType; }
        }

        private short m_sStartAddress;
        public short StartAdderess
        {
            get { return m_sStartAddress; }
        }
        private short m_sEndAddress;
        public short EndAdderess
        {
            get { return m_sEndAddress; }
        }

        public CMelsecNetMemory()
        {
            m_DeviceType = enumDeviceType.B;
            m_sStartAddress = 0;
            m_sEndAddress = 0;
            m_nMemorySize = 0;
            m_asRawMemory = null;
        }

        public CMelsecNetMemory(enumDeviceType sDeviceType, short sStartAddress, short sEndAddress)
        {
            m_DeviceType = sDeviceType;
            switch (m_DeviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    m_MemoryType = enumMemoryType.Bit;
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WR:
                case enumDeviceType.WW:
                case enumDeviceType.R:
                    m_MemoryType = enumMemoryType.Word;
                    break;
            }

            m_sStartAddress = sStartAddress;
            m_sEndAddress = sEndAddress;


            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    m_nMemorySize = (m_sEndAddress - m_sStartAddress) / 0x10 + 1;
                    break;
                case enumMemoryType.Word:
                    m_nMemorySize = m_sEndAddress - m_sStartAddress + 1;
                    break;
            }
            m_asRawMemory = new short[m_nMemorySize];
            for (int i = 0; i < m_nMemorySize; i++)
            {
                m_asRawMemory[i] = 0;
            }
        }

        public static enumMemoryType GetMemoryType(enumDeviceType deviceType)
        {
            enumMemoryType memoryType = enumMemoryType.Bit;
            switch (deviceType)
            {
                case enumDeviceType.X:
                case enumDeviceType.Y:
                case enumDeviceType.B:
                case enumDeviceType.SB:
                    memoryType = enumMemoryType.Bit;
                    break;
                case enumDeviceType.W:
                case enumDeviceType.SW:
                case enumDeviceType.WR:
                case enumDeviceType.WW:
                case enumDeviceType.R:
                    memoryType = enumMemoryType.Word;
                    break;
            }

            return memoryType;
        }

        public bool ReadBit(short sAddress, short nIndex, out bool bValue)
        {
            bValue = false;
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = (sAddress - m_sStartAddress) / 0x10;
                        bValue = ((m_asRawMemory[nStartAddress] & (1 << (sAddress & 0x000F))) != 0);
                    }
                    break;
                case enumMemoryType.Word:
                    bValue = ((m_asRawMemory[sAddress - m_sStartAddress] & (1 << nIndex)) != 0);
                    break;
            }

            return true;
        }

        public bool WriteBit(short sAddress, int nIndex, bool bValue)
        {
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = (sAddress - m_sStartAddress) / 0x10;
                        if (bValue)
                        {
                            m_asRawMemory[nStartAddress] |= (short)(1 << (sAddress & 0x000F));
                        }
                        else
                        {
                            m_asRawMemory[nStartAddress] &= (short)~(1 << (sAddress & 0x000F));
                        }
                    }
                    break;
                case enumMemoryType.Word:
                    if (bValue)
                        m_asRawMemory[sAddress - m_sStartAddress] = (short)((ushort)m_asRawMemory[sAddress - m_sStartAddress] | (1 << nIndex));
                    else
                        m_asRawMemory[sAddress - m_sStartAddress] = (short)((ushort)m_asRawMemory[sAddress - m_sStartAddress] & ~(1 << nIndex));
                    break;
            }

            return true;
        }

        public bool ReadWord(short sAddress, out short sValue)
        {
            sValue = 0;
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = (sAddress - m_sStartAddress) / 0x10;
                        sValue = m_asRawMemory[nStartAddress];
                    }
                    break;
                case enumMemoryType.Word:
                    sValue = m_asRawMemory[sAddress - m_sStartAddress];
                    break;
            }

            return true;
        }

        public bool ReadWord(short sAddress, int nLength, out short sValue)
        {
            sValue = 0;
           
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = ((sAddress - m_sStartAddress) / 0x10) * 0x10;
                        int iTemp = m_asRawMemory[nStartAddress / 0x10];
                        int iMask = 0;
                        for (int i = 0; i < nLength; i++)
                        {
                            iMask |= 1 << i;
                        }
                        iTemp = iTemp & (iMask << (sAddress - nStartAddress));
                        sValue = (short)(iTemp >> (sAddress - nStartAddress));

                        ////ushort tempSValue = 0; //<-신승원 추가 
                        ////int nStartAddress = ((sAddress - m_sStartAddress) / 0x10) * 0x10;
                        //////sValue = m_asRawMemory[nStartAddress / 0x10];
                        ////tempSValue = (ushort)m_asRawMemory[nStartAddress / 0x10]; //<-신승원 변경
                        ////ushort usMask = 0;
                        ////for (int i = 0; i < nLength; i++)
                        ////{
                        ////    usMask |= (ushort)(1 << i);
                        ////}
                        ////tempSValue = (ushort)(tempSValue & (usMask << (sAddress - nStartAddress)));
                        ////tempSValue = (ushort)(tempSValue >> (sAddress - nStartAddress));
                        ////sValue = (short)tempSValue;

                        //////sValue = (short)(sValue & (usMask << (sAddress - nStartAddress)));
                        //////sValue = (short)(sValue >> (sAddress - nStartAddress));
                    }
                    break;
                case enumMemoryType.Word:
                    sValue = m_asRawMemory[sAddress - m_sStartAddress];
                    break;
            }

            return true;
        }

        public bool ReadWord(short sAddress, int nIndex, int nLength, out short sValue)
        {
            sValue = 0;
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = ((sAddress - m_sStartAddress) / 0x10) * 0x10;
                        int iTemp = m_asRawMemory[nStartAddress / 0x10];
                        int iMask = 0;
                        for (int i = 0; i < nLength; i++)
                        {
                            iMask |= 1 << i;
                        }
                        iTemp = iTemp & (iMask << (sAddress - nStartAddress));
                        sValue = (short)(iTemp >> (sAddress - nStartAddress));
                        ////int nStartAddress = ((sAddress - m_sStartAddress) / 0x10) * 0x10;
                        ////sValue = m_asRawMemory[nStartAddress / 0x10];
                        ////ushort usMask = 0;
                        ////for (int i = 0; i < nLength; i++)
                        ////{
                        ////    usMask |= (ushort)(1 << i);
                        ////}
                        ////sValue = (short)(sValue & (usMask << (sAddress - nStartAddress)));
                        ////sValue = (short)(sValue >> (sAddress - nStartAddress));
                    }
                    break;
                case enumMemoryType.Word:
                    {
                        int iTemp = m_asRawMemory[sAddress - m_sStartAddress];
                        int iMask = 0;
                        for (int i = 0; i < nLength; i++)
                        {
                            iMask |= 1 << i;
                        }
                        iTemp = iTemp & (iMask << nIndex);
                        sValue = (short)(iTemp >> nIndex);
                        ////sValue = m_asRawMemory[sAddress - m_sStartAddress];
                        ////ushort usMask = 0;
                        ////for (int i = 0; i < nLength; i++)
                        ////{
                        ////    usMask |= (ushort)(1 << i);
                        ////}
                        ////sValue = (short)(sValue & (usMask << nIndex));
                        ////sValue = (short)(sValue >> nIndex);
                    }
                    break;
            }

            return true;
        }

        public bool WriteWord(short sAddress, short sData)
        {
            if (m_sStartAddress > sAddress || m_sEndAddress < sAddress) return false;

            switch (m_MemoryType)
            {
                case enumMemoryType.Bit:
                    {
                        int nStartAddress = (sAddress - m_sStartAddress) / 0x10;
                        m_asRawMemory[nStartAddress] = sData;
                    }
                    break;
                case enumMemoryType.Word:
                    m_asRawMemory[sAddress - m_sStartAddress] = sData;
                    break;
            }

            return true;
        }

        public bool ReadWordA(short sAddress, short sLength, out string strValue)
        {
            if (m_MemoryType != enumMemoryType.Word) 
            {
                strValue = string.Empty;
                return false;
            }

            strValue = "";
            int nMemoryLength = (sLength + 1) / 2;
            if (m_sStartAddress > sAddress || m_sEndAddress < (sAddress + nMemoryLength - 1)) return false;
            int nStartAddress = sAddress - m_sStartAddress;

            for (int i = nStartAddress; i < nStartAddress + nMemoryLength; i++)
            {
                strValue = strValue + Convert.ToChar(m_asRawMemory[i] & 0xFF);
                strValue = strValue + Convert.ToChar((m_asRawMemory[i] >> 8) & 0xFF);
            }

            strValue = strValue.Substring(0, sLength);
            strValue = strValue.Trim(" \0".ToCharArray());
            return true;
        }

        public bool WriteWordA(short sAddress, short sLength, string strData)
        {
            if (m_MemoryType != enumMemoryType.Word) 
            {
                return false;
            }

            int nMemoryLength = (sLength + 1) / 2;
            if (m_sStartAddress > sAddress || m_sEndAddress < (sAddress + nMemoryLength - 1)) return false;

            // WordLength 보다 큰 Data의 입력시 Length만큼 Data를 잘라냄
            if (strData.Length > sLength)
                strData = strData.Substring(0, sLength);
            strData = strData.PadRight(nMemoryLength * 2);

            int nStartAddress = sAddress - m_sStartAddress;

            for (int i = nStartAddress; i < nStartAddress + nMemoryLength; i++)
            {
                m_asRawMemory[i] = 0;
                m_asRawMemory[i] = Convert.ToInt16(strData[i * 2]);
                m_asRawMemory[i] = Convert.ToInt16((ushort)m_asRawMemory[i] | (Convert.ToInt16(strData[i * 2 + 1]) << 8));
            }

            return true;
        }
    }
    #endregion

    public sealed class CMelsecNetMemoryCollection : Dictionary<string, CMelsecNetMemory>
    {
    }
}
