using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemF8 : ItemN
    {
        public const String TYPE = "F8";
        private double[] data_;

        public double[] GetData()
        {
            return data_;
        }

        public ItemF8()
        {
            data_ = null;
        }

        public ItemF8(double datum)
        {
            data_ = new double[1];
            data_[0] = datum;
        }

        public ItemF8(float datum):this((double)datum)
        {

        }

        public ItemF8(double[] data)
        {
            data_ = data;
        }

	    public void SetValue(double[] data)
	    {
            data_ = data;
	    }

        public override void SetValue(Object o)
        {
            if( o is double[] )
            {
                SetValue( (double[])o );
            }
            else
		    {
			    base.SetValue(o);
		    }
        }

        public override Object GetValue(int index)
        {
            return  GetData()[index] ;
        }

        public override void Init(int count)
        {
            data_ = new double[count];
        }

        public override void Read(StreamTokenizer st) 
        {
            ParseSML(st, /* integer = */ false, /* unsigned = */ false);
        }

        public override void Read(BinaryReader dis) 
        {
         
            int len = Count();
            for (int i = 0; i < len; i++)
            {
                data_[i] = dis.ReadDouble();
            }
        }

        public override void Write(BinaryWriter dos)
        {
            WriteHeader(dos);
            for (int i = 0; i < data_.Length; i++)
            {
                dos.Write(data_[i]);
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
            data_[i] = val;
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
            return 8;
        }
    }
}
