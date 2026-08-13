using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemA : Item
    {
        public const String TYPE = "A";
        private byte[] data;

	    public byte[] GetData()
	    {
		    return data;
	    }

	    public void SetData( byte[] data )
	    {
		    this.data = data;
	    }

        public ItemA()
        {
            data = null;
        }

        public ItemA(byte[] data)
        {
            this.data = data;
        }

        public ItemA(String s)
        {
            SetValue(s);
        }

        public override void Init(int count)
        {
            data = new byte[count];
        }

        public override int Count()
        {
            return data.Length;
        }

        public void SetString(String s)
        {
            data = new System.Text.ASCIIEncoding().GetBytes(s);
        }

        public override Object GetValue(int index)
        {
            return GetString();
        }

        public override void SetValue(Object o)
        {
            if (o is String)
            {
                String s = (String)o;
                SetString(s);
            }
            else
            {
                String s = o.ToString();
                SetString(s);
            }
        }

        public override void SetValue(Object o, int index)
        {
            SetValue(o);
        }

        public override void Read(StreamTokenizer st)
        {
            try
            {
			    st.NextToken();

			    if (st.ttype == StreamTokenizer.TT_EOF)
			    {
				    throw new FormatException(
						    "Failed to find ASCII Item's value" );
			    }

			    String s = st.sval;
			    st.NextToken();

			    if( st.ttype == SML.TT_END )
			    {
				    SetString(s);
			    }
			    else
			    {
				    throw new FormatException(
						    "ASCII Item must end with a '>' character" );
			    }
            }
            catch (IOException ioe)
            {
                throw new FormatException("caught " + ioe);
            }
        }

        public override void Read(BinaryReader dis)
        {
            dis.Read(data, 0, Count());
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            dos.Write(data, 0, Count());
        }

        public override int SizeWithHeader()
        {
            int size = Count();
            size += (NumLengthBytes(size) + 1);
            return size;
        }

        public override String Type()
        {
            return TYPE;
        }

        public override String GetString()
        {
            return (data == null) ? "" : new System.Text.ASCIIEncoding().GetString(data);
            
        }

        public override String ToString()
        {
            return ToSMLString();
        }

        public override String ToSMLString()
        {
            return ("<" + Type() + " '" + GetString() + "'>");
        }

        public override String ToString( int index )
        {
            return GetString();
        }
    }
}
