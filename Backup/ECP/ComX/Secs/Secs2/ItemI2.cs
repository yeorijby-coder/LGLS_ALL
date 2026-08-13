using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemI2 : ItemN
    {
        public const String TYPE = "I2";
        private short[] data_;

        public short[] GetData()
        {
            return data_;
        }

        public ItemI2()
        {
            data_ = null;
        }

        public ItemI2(short datum)
        {
            data_ = new short[1];
            data_[0] = datum;
        }

        public ItemI2(int datum) :this((short)datum)
        {
  
        }

        public ItemI2(short[] data)
        {
            data_ = data;
        }

	    public void SetValue(short[] data)
	    {
            data_ = data;
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
            return GetData()[index] ;
        }

        public override void Init(int count)
        {
            data_ = new short[count];
        }

        public override void Read(StreamTokenizer st)
        {
            ParseSML(st, /* integer = */ true, /* unsigned = */ false);
        }

        public override void Read(BinaryReader dis)
        {
            int len = Count();
            for (int i = 0; i < len; i++)
            {
                data_[i] = System.Net.IPAddress.NetworkToHostOrder( dis.ReadInt16());
                
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

        public  override void SetDatum(int i, double val)
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
                return data_[i];
            }
            else
            {
                return 0;
            }
        }

        public override String ToString(int i)
        {
            return "" + data_[i];
        }

        public override int NumBytesPerDatum()
        {
            return 2;
        }
    }
}
