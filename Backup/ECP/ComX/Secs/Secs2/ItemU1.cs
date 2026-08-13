using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;


namespace ECP.ComX.Secs.Secs2
{
    public class ItemU1 : ItemN
    {
        private const String TYPE = "U1";
        private byte[] data_;

        public byte[] GetData()
        {
            return data_;
        }

        public ItemU1(): this(0)
        {
            
        }

        public ItemU1(byte datum)
        {
            SetValue(datum);
        }

        public ItemU1(int datum): this((byte)datum)
        {
           
        }

        public ItemU1(byte[] data)
        {
            data_ = data;
        }

        public void SetValue(byte datum)
        {
            data_ = new byte[1];
		    data_[0] = datum;
        }

        public virtual void SetValue(Object o)
        {
            if( o is byte[] )
            {
			    data_ = (byte[])o;
            }
		    else if( o is int )
		    {
			    SetValue( (byte) ((int)o) );
		    }
		    else if( o is double )
		    {
			    SetValue( (byte) ((double)o) );
		    }
            else
		    {
                base.SetValue(o);
		    }
        }

        public override Object GetValue(int index)
        {
            return  GetDatumUnsigned(index) ;
        }

        public override void Init(int count)
        {
            data_ = new byte[count];
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
                data_[i] = dis.ReadByte();
            }
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            dos.Write(data_, 0, data_.Length);
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
            data_[i] = (byte)val;
        }

        public override double GetDatum( int i )
        {
            if( i < data_.Length )
            {
                return GetDatumUnsigned(i);
            }
            else
            {
                return 0;
            }
        }

        public override String ToString(int i)
        {
            return "" + GetDatumUnsigned(i);
        }

        public int NumBytesPerDatum()
        {
            return 1;
        }

	    
	    private int GetDatumUnsigned( int i )
	    {
		    return data_[i] & 0xFF;
	    }
    }
}
