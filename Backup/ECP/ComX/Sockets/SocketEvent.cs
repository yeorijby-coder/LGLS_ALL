using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Sockets
{
    public class SocketEvent
    {
        public const int CONNECTION_ESTABLISHED = 1;
        public const int CONNECTION_LOST = 2;
        public const int CONNECTION_ERROR = 3;

        private int type;

        public int Type
        {
            get { return type; }
        }

        private PersistentSocket socket;

        public PersistentSocket Socket
        {
            get { return socket; }
        }

        private string text = "";

        public string Text
        {
            get { return text; }
        }

        public SocketEvent(int type, PersistentSocket socket)
        {
            this.type = type;
            this.socket = socket;
        }

        public SocketEvent(int type, PersistentSocket socket, String text)
        {
            this.type = type;
            this.socket = socket;
            this.text = text;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();

            result.Append("SocketEvent[");

            switch (type)
            {
                case CONNECTION_ESTABLISHED:
                    result.Append("CONNECTION_ESTABLISHED");
                    break;

                case CONNECTION_LOST:
                    result.Append("CONNECTION_LOST");
                    break;

                case CONNECTION_ERROR:
                    result.Append("CONNECTION_ERROR ").Append(text);
                    break;

                default:
                    result.Append("UNKNOWN[");
                    result.Append(type);
                    result.Append("]");
                    break;
            }

            result.Append(",");
            result.Append(socket);
            result.Append("]");

            return result.ToString();
        }

    }
}
