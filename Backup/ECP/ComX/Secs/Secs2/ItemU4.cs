using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemU4 : ItemN
    {
        public const String TYPE = "U4";
        private int[] data_;

        public int[] GetData()
        {
            return data_;
        }

        public ItemU4()
        {
            SetValue(0);
        }

        public ItemU4(int datum)
        {
            SetValue(datum);
        }

        public ItemU4(long datum):this((int)datum)
        {
            
        }

        public ItemU4(int[] data)
        {
            data_ = data;
        }

	    public void SetValue(int[] data)
	    {
            data_ = data;
	    }

        public void SetValue(int datum)
        {
            data_ = new int[1];
		    data_[0] = datum;
        }

        //public void SetValue(int i)
        //{
        //    SetValue(i);
        //}

        public void SetValue(Double d)
        {
            SetValue((int)d);
        }

        public override void SetValue(Object o)
        {
            if( o is int[] )
            {
                SetValue( (int[])o );
            }
            else
		    {
			    base.SetValue(o);
		    }
        }

        public override Object GetValue(int index)
        {
            return GetDatumUnsigned(index) ;
        }

        public override void Init(int count)
        {
            data_ = new int[count];
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
                data_[i] = System.Net.IPAddress.NetworkToHostOrder( dis.ReadInt32());
            }
        }

        public override void Write(BinaryWriter dos) 
        {
            WriteHeader(dos);
            for (int i = 0; i < data_.Length; i++)
            {

                dos.Write(System.Net.IPAddress.HostToNetworkOrder(data_[i]));
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
            data_[i] = (int)val;
        }

        public override double GetDatum(int i)
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

        public override int NumBytesPerDatum()
        {
            return 4;
        }

	    
	    private long GetDatumUnsigned( int i )
	    {
		    // Note the & only works on ints, that is why we are bit shifting
		    return (((long)data_[i] << 32) >> 32);
	    }
    }
}
