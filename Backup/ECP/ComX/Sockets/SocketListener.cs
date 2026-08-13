using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Sockets
{
    public interface SocketListener
    {
        void SocketChanged(SocketEvent e);
    }
}
