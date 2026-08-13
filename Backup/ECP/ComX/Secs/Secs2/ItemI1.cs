using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemI1 : ItemN
    {
        public const String TYPE = "I1";
        private byte[] data_;

        public byte[] GetData()
        {
            return data_;
        }

        public void SetData(byte[] data)
        {
            data_ = data;
        }

        public ItemI1() :this((byte)0)
        {

        }

        public ItemI1(byte datum)
        {
            SetValue(datum);
        }

	    public void SetValue(byte[] data)
	    {
		    data_= data;
	    }

        public void SetValue(byte datum)
        {
            SetData(new byte[1]);
            GetData()[0] = datum;
        }

        public void SetValue(Double d)
        {
            SetValue((byte)((int)d));
        }

        public void SetValue(int i)
        {
            SetValue((byte)i);
        }

        public override void SetValue(Object o)
        {
            if( o is byte[] )
            {
                SetValue( (byte[])o );
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

        public ItemI1(int datum) : this((byte)datum)
        {

        }

        public ItemI1(byte[] data)
        {
            SetData(data);
        }

        public override void Init(int count)
        {
            SetData(new byte[count]);
        }

        public override void Read(StreamTokenizer st) 
        {
            ParseSML(st, /* integer = */ true, /* unsigned = */ false);
        }

        public override void Read(BinaryReader dis)
        {
            dis.Read(GetData(), 0, Count());
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            dos.Write(GetData(), 0, Count());
        }

        public  override String Type()
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
                return data_[i];
            }
            else
            {
                return 0;
            }
        }
        //public  void SetDatum(int i, Double val)
        //{
        //    GetData()[i] = (byte)val;
        //}

        public override String ToString(int i)
        {
            return "" + GetData()[i];
        }

        public override int NumBytesPerDatum()
        {
            return 1;
        }
    }
}
