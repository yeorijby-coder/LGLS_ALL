using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemU8 : ItemN
    {
        public const String TYPE = "U8";
        private long[] data_;

        public ItemU8()
        {
            data_ = null;
        }

        public ItemU8(long datum)
        {
            data_ = new long[1];
            data_[0] = datum;
        }

        public ItemU8(long[] data)
        {
            data_ = data;
        }

        public override Object GetValue(int index)
        {
		    byte[] byteData = new byte[]
		    {
			    (byte)((data_[index] >> 56) & 0xFF),
			    (byte)((data_[index] >> 48) & 0xFF),
			    (byte)((data_[index] >> 40) & 0xFF),
			    (byte)((data_[index] >> 32) & 0xFF),
			    (byte)((data_[index] >> 24) & 0xFF),
			    (byte)((data_[index] >> 16) & 0xFF),
			    (byte)((data_[index] >>  8) & 0xFF),
			    (byte)((data_[index] >>  0) & 0xFF)
		    };

            return data_[index];
		    //return new BigInteger( 1/*positive value*/, byteData );
        }

	    public void SetValue(long[] data)
	    {
            data_ = data;
	    }

        public override void SetValue(Object o)
        {
            if( o is long[] )
            {
                SetValue( (long[])o );
            }
            else
		    {
			    base.SetValue(o);
		    }
        }

        public override void Init(int count)
        {
            data_ = new long[count];
        }

        public override void Read(StreamTokenizer st) 
        {
            ParseSML(st, /* integer = */ true, /* unsigned = */ true);
        }

        public override void Read(BinaryReader dis) 
        {
            int len = Count();
            for (int i = 0; i < len; i++)
            {

                data_[i] = System.Net.IPAddress.NetworkToHostOrder( dis.ReadInt64());
            }
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            for (int i = 0; i < data_.Length; i++)
            {
                dos.Write(System.Net.IPAddress.HostToNetworkOrder( data_[i]));
            }
        }

        public override String Type()
        {
            return TYPE;
        }

        public override int Count()
        {
            return (data_ == null) ? 0 : data_.Length;
        }

        public override void SetDatum(int i, double val)
        {
            if( data_ == null )
            {
                Init( 1 );
            }
            data_[i] = (long)val;
        }

	    
        public override double GetDatum( int i )
        {
            if( i < data_.Length )
            {
                return data_[i];
            }
            else
            {
                return 0;
            }
        }

        public override String ToString(int i)
        {
            return "" + GetValue(i).ToString();
        }

        public override int NumBytesPerDatum()
        {
            return 8;
        }
    }
}
