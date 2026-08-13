using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.Com.FieldBus.Fenet
{
    public class FenetCommand
    {
        private byte[] _companyId = { (byte)'L', (byte)'S', (byte)'I', (byte)'S', (byte)'-', (byte)'X', (byte)'G', (byte)'T' };
        public byte[] CompanyId
        {
            get { return _companyId; }
        }
        private byte[] _reserved = { 0, 0 };
        public byte[] Reserved
        {
            get { return _reserved; }
        }
        private byte[] _plcInfo = { 0, 0 };
        public byte[] PlcInfo
        {
            get { return _plcInfo; }
            set { _plcInfo = value; }
        }
        private byte _cpuInfo = 0xA0;
        public byte CpuInfo
        {
            get { return _cpuInfo; }
            set { _cpuInfo = value; }
        }
        private byte _sourceFrame = 0x33;
        public byte SourceFrame
        {
            get { return _sourceFrame; }
            set { _sourceFrame = value; }
        }

        private byte[] _invokeId = { (byte)0x00, (byte)0x01 };
        public byte[] InvokeId
        {
            get { return _invokeId; }
            set { _invokeId = value; }
        }
        private short _length = 0;

        public short Length
        {
            get { return _length; }
            set { _length = value; }
        }
        private byte _fEnetPosition = 0x10;

        public byte FEnetPosition
        {
            get { return _fEnetPosition; }
            set { _fEnetPosition = value; }
        }
        private byte _reserved2 = 0x00;
        public byte Reserved2
        {
            get { return _reserved2; }
        }


        private byte[] _command = { (byte)0x00, (byte)0x54 };   //쓰기 0x58
        public byte[] Command
        {
            get { return _command; }
            set { _command = value; }
        }

        //BIT h’00 LWORD h’04
        //BYTE h’01 DWORD h’03
        //WORD h’02 LWORD h’04
        //DWORD h’03 연속 h’14

        private byte[] _dataTye = { (byte)0x00, (byte)0x01 };

        public byte[] DataTye
        {
            get { return _dataTye; }
            set { _dataTye = value; }
        }

        private byte[] _CommandRerserved = { (byte)0x00, (byte)0x00 };

        public byte[] CommandRerserved
        {
            get { return _CommandRerserved; }
            set { _CommandRerserved = value; }
        }

        private byte[] _blockCount = { (byte)0x00, (byte)0x01 };

        public byte[] BlockCount
        {
            get { return _blockCount; }
            set { _blockCount = value; }
        }
        public byte[] _varilableLength = { (byte)0x00, (byte)0x00 };
        //frameList.Add("command", new byte[2] { 0x54, 0x00 });
        //  frameList.Add("dataType", new byte[2] { 0x14, 0x00 });
        //  frameList.Add("reserved1", new byte[2] { 0x00, 0x00 });
        //  frameList.Add("blockCount", new byte[2] { 0x01, 0x00 });
        //  frameList.Add("variableLength", new byte[2] { 0x04, 0x00 });
        //  frameList.Add("dataAddress", new byte[4] { 0x25, 0x4D, 0x42, 0x30 });//%MB0
        //  frameList.Add("dataCount", new byte[2] { 0x20, 0x00 });

    }
}
