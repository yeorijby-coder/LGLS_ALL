using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using ECP.Util;
using System.Net;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemU2 : ItemN
    {
        public const String TYPE = "U2";
        private short[] data_;

        public short[] GetData()
        {
            return data_;
        }

        public ItemU2()
        {
            SetValue(0);
        }

        public ItemU2(short datum)
        {
            SetValue(datum);
        }

        public ItemU2(int datum):this((short)datum)
        {
            
        }

        public ItemU2(short[] data)
        {
            data_ = data;
        }

	    public void SetValue(short[] data)
	    {
            data_ = data;
	    }

        public void SetValue(int datum)
        {
            SetValue((short)datum);
        }

        public void SetValue(short datum)
        {
            data_ = new short[1];
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
            if( o is short[] )
            {
                SetValue( (short[])o );
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
            data_ = new short[count];
        }

        public override void Read(BinaryReader dis)
        {
            int len = Count();
            for (int i = 0; i < len; i++)
            {
               //-- 
                data_[i] =  IPAddress.NetworkToHostOrder( dis.ReadInt16());
            }
        }

        public override void Read(StreamTokenizer st)
        {
            ParseSML(st, /* integer = */ true, /* unsigned = */ true);
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            for (int i = 0; i < data_.Length; i++)
            {
                //--

                dos.Write(IPAddress.HostToNetworkOrder(data_[i]));
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
            data_[i] = (short)val;
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

        public override int NumBytesPerDatum()
        {
            return 2;
        }

	    
	    private int GetDatumUnsigned( int i )
	    {
		    return data_[i] & 0xFFFF;
	    }
    }
}
