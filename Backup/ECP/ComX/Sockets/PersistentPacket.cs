using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

using ECP.ComX.Sockets;
using ECP.Device.Observe;
using ECP.Global.Device;
using ECP.Com.FieldBus;

namespace ECP.ComX.Sockets
{
    public class PersistentPacket
    {
        protected byte[] header;
        protected byte[] body;

        protected ObservableCollection mRegisteredObservableList;


        public PersistentPacket()
        {
            header = null;
            body = null;
        }

        public PersistentPacket(int headersize)
        {
            header = new byte[headersize];
            body = null;
        }

        public byte[] bHeader
        {
            get
            {
                return header;
            }
            set
            {
                header = new byte[value.Length];
                Buffer.BlockCopy(value, 0, header, 0, value.Length);
            }
        }

        public byte[] bBody
        {
            get
            {
                return body;
            }
            set
            {
                body = new byte[value.Length];
                Buffer.BlockCopy(value, 0, body, 0, value.Length);
            }
        }

        public void RegisterObservables(ObservableCollection registeredObservableList)
        {
            mRegisteredObservableList = registeredObservableList;
        }

        
        virtual public int Read(SocketInputStream in_stream)
        {
            return 0;
        }

        virtual public void Write(SocketOutputStream out_stream)
        {
            ;
        }
    }
}
