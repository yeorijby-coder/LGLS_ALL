using System;
using System.Collections.Generic;
using System.Text;

using System.IO;
using ECP.Util;

namespace ECP.ComX.Secs.Secs2
{
    public class ItemF4 : ItemN
    {
        public const String TYPE = "F4";
	    public const String POS_INFINITY =  "3.4028236E38";
	    public const String NEG_INFINITY = "-3.4028236E38";

        private float[] data_;

        public float[] GetData()
        {
            return data_;
        }

        public ItemF4()
        {
            data_ = null;
        }

        public ItemF4(float datum)
        {
            data_ = new float[1];
            data_[0] = datum;
        }

        public ItemF4(double datum) : this((float)datum)
        {
            
        }

        public ItemF4(float[] data)
        {
            data_ = data;
        }

	    public void SetValue(float[] data)
	    {
            data_ = data;
	    }

        public override void SetValue(Object o)
        {
            if( o is float[] )
            {
                SetValue( (float[])o );
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
            data_ = new float[count];
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
                data_[i] = dis.ReadSingle(); //read float
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
            data_[i] = (float)val;
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
         
		    if( float.IsInfinity(data_[i]) )
		    {
			    if( data_[i] < 0f )
			    {
				    return NEG_INFINITY;
			    }
			    else
			    {
				    return POS_INFINITY;
			    }
		    }
		    return Convert.ToString( data_[i] );
        }

        public override int NumBytesPerDatum()
        {
            return 4;
        }
    }
}
